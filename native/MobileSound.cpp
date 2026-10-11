#include "MobileSound.hpp"
#include "Settings.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <list>
#include <mutex>
#include <string>
#include <unordered_map>
extern "C" {
#include "VorbisDecoder.h"
}
namespace pc {
namespace {
std::uint16_t u16(std::span<const std::uint8_t> d,std::size_t at) {return std::uint16_t(d[at]|d[at+1]<<8);}
std::int32_t s32(std::span<const std::uint8_t> d,std::size_t at) {return std::int32_t(d[at]|d[at+1]<<8|d[at+2]<<16|std::uint32_t(d[at+3])<<24);}
float f32(std::span<const std::uint8_t> d,std::size_t at) {const std::uint32_t bits=std::uint32_t(s32(d,at));float value;std::memcpy(&value,&bits,4);return value;}
}

bool parseAkb(std::span<const std::uint8_t> file,AkbInfo& info) {
    if(file.size()<0x44||std::memcmp(file.data(),"AKB ",4)!=0)return false;
    const std::size_t header=u16(file,6);
    if(header<0x44||std::size_t(s32(file,8))!=file.size())return false;
    info.codec=file[0x0c];info.channels=file[0x0d];info.rate=u16(file,0x0e);
    info.samples=s32(file,0x10);info.loopStart=s32(file,0x14);info.loopEnd=s32(file,0x18);
    info.volume=f32(file,0x20);
    if(file[0x2b]&8)return false; // encrypted (later versions)
    const std::size_t extra=header+u16(file,0x28);
    info.data=extra+u16(file,0x1c);
    if(info.channels<1||info.channels>2||!info.rate||info.data>=file.size())return false;
    if(!(info.volume>0&&info.volume<=4))info.volume=1;
    if(info.codec==2) {
        if(extra+0x10>file.size())return false;
        info.frameSize=u16(file,extra+2);info.samples=s32(file,extra+4);
        return info.frameSize>7*info.channels&&info.samples>0;
    }
    return info.codec==5;
}

std::vector<std::int16_t> decodeMsAdpcm(std::span<const std::uint8_t> data,int channels,int frameSize,int samples) {
    static constexpr int coef1[7]={256,512,0,192,240,460,392},coef2[7]={0,-256,0,64,0,-208,-232};
    static constexpr int adapt[16]={230,230,230,230,307,409,512,614,768,614,512,409,307,230,230,230};
    std::vector<std::int16_t> out(std::size_t(samples)*channels);
    std::size_t written=0;
    for(std::size_t at=0;at+frameSize<=data.size()&&written<std::size_t(samples);at+=frameSize) {
        const auto frame=data.subspan(at,frameSize);
        int predictor[2]{},delta[2]{},s1[2]{},s2[2]{};
        for(int c=0;c<channels;++c) {
            predictor[c]=std::min<int>(frame[c],6);
            delta[c]=std::int16_t(u16(frame,channels+2*c));
            s1[c]=std::int16_t(u16(frame,3*channels+2*c));
            s2[c]=std::int16_t(u16(frame,5*channels+2*c));
        }
        auto put=[&](int c,int value){if(written<std::size_t(samples))out[written*channels+c]=std::int16_t(value);};
        for(int c=0;c<channels;++c)put(c,s2[c]);
        if(++written>=std::size_t(samples))break;
        for(int c=0;c<channels;++c)put(c,s1[c]);
        if(++written>=std::size_t(samples))break;
        for(std::size_t n=7*channels;n<frame.size();++n) {
            const int nibbles[2]={frame[n]>>4,frame[n]&15};
            for(int k=0;k<2;++k) {
                // Mono: two samples per byte; stereo: left then right.
                const int c=channels==2?k:0,nibble=nibbles[k];
                int predicted=(s1[c]*coef1[predictor[c]]+s2[c]*coef2[predictor[c]])/256;
                predicted=std::clamp(predicted+(nibble>=8?nibble-16:nibble)*delta[c],-32768,32767);
                s2[c]=s1[c];s1[c]=predicted;
                delta[c]=std::max(16,adapt[nibble]*delta[c]/256);
                put(c,predicted);
                if((channels==1||k==1)&&++written>=std::size_t(samples))return out;
            }
        }
    }
    return out;
}

namespace {
// The DS sound volume (0..127) as an amplitude (decibel-like curve).
float curve(int volume) {const float v=std::clamp(volume,0,127)/127.0f;return v*v;}

struct Stream {
    const void* handle=nullptr;
    AkbInfo info;
    std::vector<std::uint8_t> file;                      // Ogg data stays alive while decoding
    std::shared_ptr<const std::vector<std::int16_t>> pcm; // MS ADPCM, decoded
    DqivVorbis* vorbis=nullptr;
    std::vector<float> window;   // decoded stereo frames from windowStart (Ogg)
    std::uint64_t windowStart=0;
    std::uint64_t decoded=0;     // virtual frames decoded so far (loops unrolled)
    std::int64_t decoderAt=0;    // the decoder's position in the recording
    double position=0;           // virtual source frame
    double step=1;
    float base=1;                // the recording's level at the reference volume
    int volume=127,reference=127;
    std::int64_t fade=0,fadeLength=0; // output frames left / total while fading out
    bool fading=false,finished=false;
    ~Stream() {if(vorbis)dqiv_vorbis_close(vorbis);}
    bool loops() const {return info.loopEnd>info.loopStart&&info.loopEnd<=info.samples;}
    // The source frame of a virtual frame (loops unrolled), or -1 past the end.
    std::int64_t source(std::uint64_t frame) const {
        if(!loops())return frame<std::uint64_t(info.samples)?std::int64_t(frame):-1;
        if(frame<std::uint64_t(info.loopEnd))return std::int64_t(frame);
        const std::uint64_t length=std::uint64_t(info.loopEnd-info.loopStart);
        return info.loopStart+std::int64_t((frame-info.loopEnd)%length);
    }
    // Stereo frame `frame` (virtual), or false past the end.
    bool frameAt(std::uint64_t frame,float& left,float& right) {
        if(pcm) {
            const auto at=source(frame);if(at<0)return false;
            const auto* p=pcm->data()+std::size_t(at)*info.channels;
            left=p[0]/32768.0f;right=p[info.channels-1]/32768.0f;return true;
        }
        while(decoded<=frame) {
            // Decode on, from the loop start once the loop end is reached.
            if(loops()&&decoderAt>=info.loopEnd) {dqiv_vorbis_seek(vorbis,unsigned(info.loopStart));decoderAt=info.loopStart;}
            float buffer[2048*2];
            int want=2048;
            if(loops())want=int(std::min<std::int64_t>(want,info.loopEnd-decoderAt));
            const int got=want>0?dqiv_vorbis_read(vorbis,buffer,want):0;
            if(got<=0) {
                if(loops()&&decoderAt>info.loopStart) {decoderAt=info.loopEnd;continue;} // ended early: wrap
                return false;
            }
            if(info.channels==1)for(int i=0;i<got;++i)buffer[2*i+1]=buffer[2*i];
            window.insert(window.end(),buffer,buffer+got*2);decoded+=got;decoderAt+=got;
        }
        if(frame<windowStart)return false;
        const std::size_t at=std::size_t(frame-windowStart)*2;
        left=window[at];right=window[at+1];return true;
    }
    void trim() {
        if(pcm)return;
        const std::uint64_t keep=std::uint64_t(position)>2?std::uint64_t(position)-2:0;
        if(keep>windowStart+8192) {
            window.erase(window.begin(),window.begin()+std::ptrdiff_t(keep-windowStart)*2);windowStart=keep;
        }
    }
};
}

struct MobileSound::Impl {
    const Assets& assets;
    unsigned rate;
    std::mutex lock;
    std::list<std::unique_ptr<Stream>> streams;
    std::unordered_map<const void*,bool> owned; // handles whose last sound is a recording
    std::vector<float> levels;                  // snd_vol.dat: level per sound number
    // Decoded effects, most recently used first.
    std::list<std::pair<std::string,std::shared_ptr<const std::vector<std::int16_t>>>> effects;
    std::size_t effectBytes=0;
    Impl(const Assets& a,unsigned r):assets(a),rate(r) {
        if(assets.contains("data/SOUND/MOBILE/snd_vol.dat")) {
            const auto data=assets.read("data/SOUND/MOBILE/snd_vol.dat");
            for(std::size_t at=0;at+4<=data.size();at+=4)levels.push_back(f32(data,at));
        }
    }
    static std::string name(MobileSoundKind kind,int id) {
        char text[40];
        if(kind==mobileEffect)std::snprintf(text,sizeof text,"data/SOUND/MOBILE/SE%04d.akb",id);
        else std::snprintf(text,sizeof text,"data/SOUND/MOBILE/BGM%02d.akb",kind==mobileStream?id+1:id);
        return text;
    }
    std::shared_ptr<const std::vector<std::int16_t>> effect(const std::string& path,std::span<const std::uint8_t> file,const AkbInfo& info) {
        for(auto it=effects.begin();it!=effects.end();++it)if(it->first==path) {
            effects.splice(effects.begin(),effects,it);return effects.front().second;
        }
        auto pcm=std::make_shared<const std::vector<std::int16_t>>(
            decodeMsAdpcm(file.subspan(info.data),info.channels,info.frameSize,info.samples));
        effectBytes+=pcm->size()*2;effects.emplace_front(path,pcm);
        while(effectBytes>(64u<<20)&&effects.size()>1) {effectBytes-=effects.back().second->size()*2;effects.pop_back();}
        return pcm;
    }
    Stream* find(const void* handle) {
        for(auto& s:streams)if(s->handle==handle&&!s->finished)return s.get();
        return nullptr;
    }
};

namespace {
MobileSound::Impl* current=nullptr;
// The recordings are mixed quieter than the DS synthesizer: brought to about
// the same loudness (measured on the same scenes).
constexpr float loudness=2.6f;
}

MobileSound::MobileSound(const Assets& assets,unsigned rate):impl_(std::make_unique<Impl>(assets,rate)) {current=impl_.get();}
MobileSound::~MobileSound() {if(current==impl_.get())current=nullptr;}

void MobileSound::mix(std::span<std::int16_t> stereo) {
    auto& m=*impl_;std::lock_guard guard(m.lock);
    if(m.streams.empty())return;
    const std::size_t frames=stereo.size()/2;
    std::vector<float> sum(stereo.size(),0.0f);
    for(auto& s:m.streams) {
        if(s->finished)continue;
        const float gain=s->base*curve(s->volume)/std::max(curve(s->reference),1e-4f);
        for(std::size_t i=0;i<frames;++i) {
            float fade=1;
            if(s->fading) {
                if(s->fade<=0) {s->finished=true;break;}
                fade=float(s->fade)/float(std::max<std::int64_t>(s->fadeLength,1));--s->fade;
            }
            const auto frame=std::uint64_t(s->position);const float t=float(s->position-double(frame));
            float l0,r0,l1,r1;
            if(!s->frameAt(frame,l0,r0)) {s->finished=true;break;}
            if(!s->frameAt(frame+1,l1,r1)) {l1=l0;r1=r0;}
            sum[2*i]+=(l0+(l1-l0)*t)*gain*fade;sum[2*i+1]+=(r0+(r1-r0)*t)*gain*fade;
            s->position+=s->step;
        }
        s->trim();
    }
    m.streams.remove_if([](const auto& s){return s->finished;});
    for(std::size_t i=0;i<stereo.size();++i) {
        float value=stereo[i]/32768.0f+sum[i];
        // Gentle limiter instead of hard clipping when everything plays at once.
        if(value>0.9f)value=0.9f+0.1f*std::tanh((value-0.9f)*10);
        else if(value<-0.9f)value=-0.9f-0.1f*std::tanh((-value-0.9f)*10);
        stereo[i]=std::int16_t(std::clamp(value*32767.0f,-32768.0f,32767.0f));
    }
}

bool mobileSoundStart(const void* handle,MobileSoundKind kind,int id,int volume,int reference) {
    if(!current||!gameSettings().mobileSound)return false;
    auto& m=*current;std::lock_guard guard(m.lock);
    for(auto& s:m.streams)if(s->handle==handle)s->finished=true;
    m.owned.erase(handle);
    const auto path=MobileSound::Impl::name(kind,id);
    if(!m.assets.contains(path))return false;
    auto stream=std::make_unique<Stream>();
    stream->file=m.assets.read(path);
    if(!parseAkb(stream->file,stream->info))return false;
    if(stream->info.codec==2)stream->pcm=m.effect(path,stream->file,stream->info),stream->file.clear();
    else {
        stream->vorbis=dqiv_vorbis_open(stream->file.data()+stream->info.data,int(stream->file.size()-stream->info.data));
        if(!stream->vorbis)return false;
    }
    const int number=kind==mobileStream?id+1:id;
    const float level=std::size_t(number)<m.levels.size()&&m.levels[std::size_t(number)]>0?m.levels[std::size_t(number)]:1.0f;
    stream->handle=handle;stream->base=stream->info.volume*level*loudness;
    stream->volume=volume;stream->reference=std::max(1,reference);
    stream->step=double(stream->info.rate)/double(m.rate);
    m.streams.push_back(std::move(stream));
    m.owned[handle]=true;
    return true;
}
void mobileSoundStop(const void* handle,int fadeFrames) {
    if(!current)return;
    auto& m=*current;std::lock_guard guard(m.lock);
    if(auto* s=m.find(handle)) {
        const std::int64_t length=std::int64_t(std::max(fadeFrames,0))*m.rate/60;
        if(length<=0) {s->finished=true;return;}
        if(!s->fading||length<s->fade) {s->fading=true;s->fade=s->fadeLength=length;}
    }
}
void mobileSoundRelease(const void* handle) {
    if(!current)return;
    auto& m=*current;std::lock_guard guard(m.lock);
    for(auto& s:m.streams)if(s->handle==handle)s->finished=true;
    m.owned.erase(handle);
}
bool mobileSoundOwns(const void* handle) {
    if(!current)return false;
    auto& m=*current;std::lock_guard guard(m.lock);
    return m.owned.count(handle)!=0;
}
bool mobileSoundPlaying(const void* handle) {
    if(!current)return false;
    auto& m=*current;std::lock_guard guard(m.lock);
    return m.find(handle)!=nullptr;
}
void mobileSoundVolume(const void* handle,int volume) {
    if(!current)return;
    auto& m=*current;std::lock_guard guard(m.lock);
    if(auto* s=m.find(handle))s->volume=volume;
}
}
