#include "SoundMixer.hpp"
#include <algorithm>
#include <stdexcept>
namespace pc {
namespace {
constexpr std::uint64_t timerClock=33513982/2;
constexpr unsigned shifts[4]={0,1,2,4};
unsigned scale(unsigned value) {return value==127?128:value;}
void checkMask(unsigned mask) {if(mask&~0xffffu)throw std::runtime_error("Invalid sound channel mask");}
void checkTimer(unsigned timer) {if(timer<16 || timer>65535)throw std::runtime_error("Invalid sound channel timer");}
int pcm16(std::span<const std::uint8_t> bytes,std::size_t offset) {
    const unsigned value=bytes[offset]|unsigned(bytes[offset+1])<<8;
    return value<32768?int(value):int(value)-65536;
}
}
SoundMixer::SoundMixer(unsigned rate):rate_(rate) {
    if(rate<8000 || rate>192000)throw std::runtime_error("Invalid native audio sample rate");
}
SoundMixer::Channel& SoundMixer::channel(unsigned id) {
    if(id>=channels_.size())throw std::runtime_error("Invalid sound channel");
    return channels_[id];
}
void SoundMixer::setupWave(unsigned id,std::shared_ptr<const SoundWave> wave) {
    if(!wave || wave->samples.empty() || (wave->loop && wave->loopStart>=wave->samples.size()))
        throw std::runtime_error("Invalid native sound wave");
    checkTimer(wave->timer);
    Channel next;next.source=Source::Wave;next.wave=std::move(wave);next.length=next.wave->samples.size();
    next.loopStart=next.wave->loopStart;next.loop=next.wave->loop?1:2;next.timer=next.wave->timer;next.format=next.wave->format;
    channel(id)=std::move(next);
}
void SoundMixer::setupPcm(unsigned id,unsigned format,std::span<const std::uint8_t> data,
                          unsigned loop,unsigned start,unsigned len,unsigned timerValue) {
    checkTimer(timerValue);
    const std::uint64_t bytes=(std::uint64_t(start)+len)*4;
    if(format>2 || loop>2 || start>65535 || len>0x3fffff || !bytes || bytes>data.size() || (loop==1 && !len))
        throw std::runtime_error("Invalid sound PCM buffer");
    if(loop==0)throw std::runtime_error("Unbounded manual PCM playback needs a native buffer provider");
    Channel next;next.source=Source::Pcm;next.format=format;next.data=data.first(static_cast<std::size_t>(bytes));
    next.loop=loop;next.timer=timerValue;
    if(format<2) {next.length=next.data.size()>>(format==1?1:0);next.loopStart=std::size_t(start)*4>>(format==1?1:0);}
    else {
        if(bytes<=4 || (loop==1 && !start) || data[2]>88)throw std::runtime_error("Invalid hardware ADPCM data");
        next.length=(next.data.size()-4)*2;next.loopStart=start?(std::size_t(start)*4-4)*2:0;
    }
    channel(id)=std::move(next);
}
void SoundMixer::setupPsg(unsigned id,unsigned duty,unsigned timerValue) {
    checkTimer(timerValue);if(id<8 || id>13 || duty>7)throw std::runtime_error("Invalid PSG channel or duty");
    Channel next;next.source=Source::Psg;next.duty=duty;next.timer=timerValue;next.loop=1;channel(id)=std::move(next);
}
void SoundMixer::setupNoise(unsigned id,unsigned timerValue) {
    checkTimer(timerValue);if(id<14 || id>15)throw std::runtime_error("Invalid noise channel");
    Channel next;next.source=Source::Noise;next.timer=timerValue;next.loop=1;channel(id)=std::move(next);
}
void SoundMixer::volume(unsigned mask,unsigned value,unsigned shift) {
    checkMask(mask);if(value>127 || shift>3)throw std::runtime_error("Invalid channel volume");
    for(unsigned i=0;i<16;++i)if(mask&(1u<<i)) {channels_[i].volume=value;channels_[i].shift=shift;}
}
void SoundMixer::pan(unsigned mask,unsigned value) {
    checkMask(mask);if(value>127)throw std::runtime_error("Invalid channel pan");
    for(unsigned i=0;i<16;++i)if(mask&(1u<<i))channels_[i].pan=value;
}
void SoundMixer::timer(unsigned mask,unsigned value) {
    checkMask(mask);checkTimer(value);
    for(unsigned i=0;i<16;++i)if(mask&(1u<<i))channels_[i].timer=value;
}
void SoundMixer::masterVolume(unsigned value) {
    if(value>127)throw std::runtime_error("Invalid master volume");master_=value;
}
void SoundMixer::masterPan(int value) {
    if(value<-1 || value>127)throw std::runtime_error("Invalid master pan");masterPan_=value;
}
void SoundMixer::outputSelector(unsigned left,unsigned right,bool bypass1,bool bypass3) {
    if(left>3 || right>3)throw std::runtime_error("Invalid sound output selector");
    leftOutput_=left;rightOutput_=right;bypass1_=bypass1;bypass3_=bypass3;
}
void SoundMixer::invalidate(const void* begin,const void* end) {
    const auto first=reinterpret_cast<std::uintptr_t>(begin),last=reinterpret_cast<std::uintptr_t>(end);
    if(last<first)throw std::runtime_error("Invalid sound memory range");
    if(last==first)return;
    for(auto& c:channels_)if(c.source==Source::Pcm) {
        const auto address=reinterpret_cast<std::uintptr_t>(c.data.data());
        if(address<last && (address>=first || c.data.size()>first-address))c=Channel{};
    }
}
void SoundMixer::start(unsigned mask) {
    checkMask(mask);
    for(unsigned i=0;i<16;++i)if((mask&(1u<<i)) && channels_[i].source==Source::None)
        throw std::runtime_error("Starting an unconfigured sound channel");
    for(unsigned i=0;i<16;++i)if(mask&(1u<<i)) {
        auto& c=channels_[i];if(c.running)continue;
        c.running=true;c.hold=false;c.phase=0;c.position=0;c.sample=0;c.noise=0x7fff;c.loopSaved=false;
        c.delay=(c.source==Source::Psg || c.source==Source::Noise)?1:(c.format==2?11:3);
        if(c.source==Source::Pcm && c.format==2)c.adpcm={pcm16(c.data,0),c.data[2]};
    }
}
void SoundMixer::stop(unsigned mask,bool hold) {
    checkMask(mask);for(unsigned i=0;i<16;++i)if(mask&(1u<<i)) {
        auto& c=channels_[i];c.running=false;c.hold=hold;if(!hold)c.sample=0;
    }
}
unsigned SoundMixer::activeMask() const {
    unsigned mask=0;
    for(unsigned i=0;i<16;++i) {
        const auto& c=channels_[i];
        if(c.running && (c.delay || c.loop!=2 || c.position+1<c.length))mask|=1u<<i;
    }
    return mask;
}
void SoundMixer::loadSample(Channel& c) {
    if(c.source==Source::Wave)c.sample=c.wave->samples[c.position];
    else if(c.source==Source::Pcm) {
        if(c.format==0) {const int value=c.data[c.position];c.sample=(value<128?value:value-256)*256;}
        else if(c.format==1)c.sample=pcm16(c.data,c.position*2);
        else {
            if(c.position==c.loopStart && !c.loopSaved) {c.loopState=c.adpcm;c.loopSaved=true;}
            const auto byte=c.data[4+c.position/2];c.sample=c.adpcm.decode(byte>>((c.position&1)*4),true);
        }
    }else if(c.source==Source::Psg)c.sample=(c.duty<7 && (c.position&7)>=7-c.duty)?32767:-32767;
    else if(c.source==Source::Noise) {
        const bool low=(c.noise&1)!=0;c.noise>>=1;if(low)c.noise^=0x6000;
        c.sample=low?-32767:32767;
    }
}
void SoundMixer::advance(Channel& c) {
    if(c.delay) {if(!--c.delay)loadSample(c);return;}
    ++c.position;
    if((c.source==Source::Pcm || c.source==Source::Wave) && c.position==c.length) {
        if(c.loop==1) {
            c.position=c.loopStart;
            if(c.source==Source::Pcm && c.format==2)c.adpcm=c.loopState;
        }else {c.running=false;if(!c.hold)c.sample=0;return;}
    }
    loadSample(c);
}
void SoundMixer::render(std::span<std::int16_t> output) {
    if(output.size()&1)throw std::runtime_error("Stereo output must have an even sample count");
    for(std::size_t frame=0;frame<output.size()/2;++frame) {
        std::array<std::int64_t,4> left{},right{};
        for(unsigned id=0;id<16;++id) {
            auto& c=channels_[id];
            // Keep fractional bits through division, volume and panning. They
            // matter when many quiet channels are summed before master volume.
            const auto value=((std::int64_t(c.sample)*16)>>shifts[c.shift])*scale(c.volume);
            const auto panValue=scale(masterPan_>=0?static_cast<unsigned>(masterPan_):c.pan);
            const auto l=value*(128-panValue),r=value*panValue;
            if(!((id==1 && bypass1_) || (id==3 && bypass3_))) {left[0]+=l;right[0]+=r;}
            if(id==1 || id==3) {const unsigned target=id==1?1:2;left[target]=l;right[target]=r;left[3]+=l;right[3]+=r;}
            if(c.running) {
                c.phase+=timerClock;const std::uint64_t period=std::uint64_t(c.timer)*rate_;
                while(c.phase>=period && c.running) {c.phase-=period;advance(c);}
            }
        }
        // Retain 16-bit host output (no DS DAC's final 10-bit quantization).
        const auto gain=scale(master_);
        output[frame*2]=static_cast<std::int16_t>(std::clamp<std::int64_t>((left[leftOutput_]*gain)>>25,-32768,32767));
        output[frame*2+1]=static_cast<std::int16_t>(std::clamp<std::int64_t>((right[rightOutput_]*gain)>>25,-32768,32767));
    }
    frames_+=output.size()/2;
}
}
