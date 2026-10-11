#include "SoundSequence.hpp"
#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <string>
extern "C" {
#include "../game/libs/NitroSDK/src/snd/snd_internal.h"
extern UnkSndSharedWork* data_02116100;
extern const short data_020ba8ac[128];
}
namespace pc {
namespace {
constexpr unsigned active=1,prepared=2,paused=4;
constexpr unsigned noteWait=2,muted=4,tied=8,finishWait=16,portamento=32,comparison=64,strongMask=128;
void flag(std::uint8_t& flags,unsigned bit,bool on) {flags=static_cast<std::uint8_t>(on?flags|bit:flags&~bit);}
int decibel(unsigned value) {
    if(value>127)throw std::runtime_error("Sequence volume exceeds seven bits");
    return data_020ba8ac[value];
}
void parameter(void* data,unsigned capacity,unsigned offset,unsigned value,unsigned size) {
    if((size!=1 && size!=2 && size!=4) || offset>capacity || size>capacity-offset)
        throw std::runtime_error("Invalid sequence parameter write");
    std::memcpy(static_cast<std::uint8_t*>(data)+offset,&value,size);
}
bool within(const void* pointer,unsigned first,unsigned last) {
    const auto value=reinterpret_cast<std::uintptr_t>(pointer);return value>=first && value<=last;
}
}
SoundSequence::SoundSequence(SoundVoices& voices):voices_(voices) {
    for(unsigned i=0;i<16;++i) {players_[i].p.id=static_cast<std::uint8_t>(i);players_[i].p.tracks.fill(255);}
}
void SoundSequence::registerData(std::span<const std::uint8_t> data) {
    if(data.empty())throw std::runtime_error("Empty native sequence data");
    data_[data.data()]=data.size();
}
unsigned SoundSequence::random() {random_=random_*1664525u+1013904223u;return random_>>16;}
void SoundSequence::advanceRandom() {(void)random();}
unsigned SoundSequence::read(Track& t,unsigned bytes) {
    if(t.cursor>t.data.size() || bytes>t.data.size()-t.cursor)throw std::runtime_error("Sequence reads past its data block");
    unsigned result=0;for(unsigned i=0;i<bytes;++i)result|=unsigned(t.data[t.cursor++])<<(i*8);return result;
}
void SoundSequence::seek(Track& t,std::size_t offset) {
    if(offset>=t.data.size())throw std::runtime_error("Sequence jumps outside its data block");
    t.cursor=offset;t.running=true;
}
volatile std::int16_t* SoundSequence::variable(Player& p,unsigned index) {
    if(index>=32)throw std::runtime_error("Invalid sequence variable index");
    if(!data_02116100)throw std::runtime_error("Sequence has no SDK shared work");
    return index<16?&data_02116100->player[p.p.id].variable[index]:&data_02116100->globalVariable[index-16];
}
int SoundSequence::value(Track& t,Player& p,unsigned type) {
    if(type==0)return read(t);
    if(type==1)return read(t,2);
    if(type==2) {
        unsigned result=0;
        for(unsigned i=0;i<5;++i) {
            const auto b=read(t);if(result>0xffffff)throw std::runtime_error("Sequence variable-length value overflow");
            result=(result<<7)|(b&127);if(!(b&128))return static_cast<int>(result);
        }
        throw std::runtime_error("Unterminated sequence variable-length value");
    }
    if(type==3) {
        const int low=static_cast<std::int16_t>(read(t,2)),high=static_cast<std::int16_t>(read(t,2));
        return low+static_cast<int>((std::int64_t(random())*(high-low+1))>>16);
    }
    if(type==4)return *variable(p,read(t));
    throw std::runtime_error("Invalid sequence value type");
}
int SoundSequence::allocateTrack() {
    for(unsigned i=0;i<tracks_.size();++i)if(!(tracks_[i].p.flags&active)) {
        tracks_[i]=Track{};tracks_[i].p.flags=active|noteWait|comparison;return static_cast<int>(i);
    }
    return -1;
}
SoundSequence::Track* SoundSequence::track(Player& p,unsigned index) {
    if(index>=16 || p.p.tracks[index]==255)return nullptr;
    return &tracks_.at(p.p.tracks[index]);
}
void SoundSequence::prepare(Player& p,const void* base,unsigned offset,const void* bank) {
    auto found=data_.find(static_cast<const std::uint8_t*>(base));
    if(found==data_.end())throw std::runtime_error("Sequence data was not registered by its archive loader");
    stop(p);const auto id=p.p.id;p=Player{};p.p.id=id;p.p.tracks.fill(255);p.bank=bank;
    if(data_02116100) {
        auto& shared=data_02116100->player[id];shared.tickCounter=0;std::fill(std::begin(shared.variable),std::end(shared.variable),std::int16_t(-1));
    }
    const int first=allocateTrack();if(first<0)return;
    p.p.tracks[0]=static_cast<std::uint8_t>(first);auto& t=tracks_[first];t.data={found->first,found->second};seek(t,offset);
    if(read(t)==0xfe) {
        const auto mask=read(t,2);
        for(unsigned i=1;i<16;++i)if(mask&(1u<<i)) {
            const int allocated=allocateTrack();if(allocated<0)break;p.p.tracks[i]=static_cast<std::uint8_t>(allocated);
        }
    }else --t.cursor;
    p.p.flags=active;
}
void SoundSequence::updateVoices(Track& t,Player& p,bool releaseNotes) {
    const int owner=static_cast<int>(&t-tracks_.data());
    const int volume=std::max(-32768,decibel(t.p.volume)+decibel(t.p.expression)+decibel(p.p.volume));
    const int fade=std::max(-32768,int(t.p.fade)+p.p.fade);
    const int pitch=((t.p.bend*(t.p.bendRange*64))>>7)+t.p.pitch;
    int pan=t.p.pan;if(t.p.panRange!=127)pan=(pan*t.p.panRange+64)>>7;
    pan=std::clamp(pan+t.p.externalPan,-128,127);
    for(auto& v:voices_.voices)if(v.owner==owner) {
        v.fade=fade;if(v.stage==3)continue;
        v.decay=volume;v.pitch=static_cast<std::int16_t>(pitch);v.userPan=pan;v.panRange=t.p.panRange;v.mod=t.p.modulation;
        if(!v.length && releaseNotes)voices_.release(v);
    }
}
void SoundSequence::release(Track& t,Player& p,int coefficient,bool detach) {
    updateVoices(t,p,false);const int owner=static_cast<int>(&t-tracks_.data());
    for(auto& v:voices_.voices)if(v.owner==owner) {
        if(v.active) {if(coefficient>=0)voices_.release(v,coefficient);voices_.release(v);}
        if(detach)v.owner=-1;
    }
}
void SoundSequence::stopTrack(Player& p,unsigned index) {
    if(auto* t=track(p,index)) {release(*t,p,-1,true);t->p.flags&=~active;t->running=false;p.p.tracks[index]=255;}
}
void SoundSequence::stop(Player& p) {for(unsigned i=0;i<16;++i)stopTrack(p,i);p.p.flags&=~active;}
void SoundSequence::mute(Track& t,Player& p,unsigned mode) {
    if(mode>3)throw std::runtime_error("Invalid sequence mute mode");
    flag(t.p.flags,muted,mode!=0);
    if(mode>=2)release(t,p,mode==3?127:-1,mode==3);
}
void SoundSequence::note(Track& t,Player& p,int key,int velocity,int length) {
    const int owner=static_cast<int>(&t-tracks_.data());SoundVoices::Voice* voice=nullptr;
    if(t.p.flags&tied)for(auto& v:voices_.voices)if(v.owner==owner && (!voice || v.serial>voice->serial))voice=&v;
    if(voice) {voice->key=key;voice->velocity=velocity;}
    else {
        const auto instrument=SoundVoices::instrument(p.bank,t.p.program,key);if(!instrument)return;
        unsigned mask=0;
        switch(instrument->type) {case 1:case 4:mask=65535;break;case 2:mask=0x3f00;break;case 3:mask=0xc000;break;default:return;}
        const int id=voices_.allocate(mask&t.p.channelMask,p.p.priority+t.p.priority,(t.p.flags&strongMask)!=0,owner);
        if(id<0)return;
        voice=&voices_.voices[id];
        if(!voices_.start(id,p.bank,*instrument,key,velocity,(t.p.flags&tied)?-1:length)) {voice->priority=0;voice->owner=-1;return;}
    }
    if(t.p.attack!=255)voices_.attack(*voice,t.p.attack);
    if(t.p.decay!=255)voices_.decay(*voice,t.p.decay);
    if(t.p.sustain!=255)voices_.sustain(*voice,t.p.sustain);
    if(t.p.release!=255)voices_.release(*voice,t.p.release);
    voice->sweep=static_cast<std::int16_t>(t.p.sweep+((t.p.flags&portamento)?(t.p.portamentoKey-key)*64:0));
    if(!t.p.portamentoTime) {voice->sweepLength=length;voice->autoSweep=false;}
    else voice->sweepLength=(t.p.portamentoTime*t.p.portamentoTime*std::abs(voice->sweep))>>11;
    voice->sweepCounter=0;
}
bool SoundSequence::step(Track& t,Player& p,bool playNotes) {
    const int owner=static_cast<int>(&t-tracks_.data());bool hasVoice=false;
    for(auto& v:voices_.voices)if(v.owner==owner) {
        hasVoice=true;if(v.length>0)--v.length;if(!v.autoSweep && v.sweepCounter<v.sweepLength)++v.sweepCounter;
    }
    if(t.p.flags&finishWait) {if(hasVoice)return true;t.p.flags&=~finishWait;}
    if(t.wait>0 && --t.wait>0)return true;
    unsigned instructions=0;
    while(!t.wait && !(t.p.flags&finishWait)) {
        if(++instructions>100000)throw std::runtime_error("Sequence loops without advancing musical time");
        unsigned cmd=read(t),special=255;bool run=true;
        if(cmd==0xa2) {cmd=read(t);run=(t.p.flags&comparison)!=0;}
        if(cmd==0xa0) {cmd=read(t);special=3;}
        if(cmd==0xa1) {cmd=read(t);special=4;}
        auto argument=[&](unsigned type) {return value(t,p,special==255?type:special);};
        if(cmd<128) {
            const auto velocity=read(t);const int length=argument(2);if(!run)continue;
            const int key=std::clamp(int(cmd)+t.p.transpose,0,127);
            if(!(t.p.flags&muted) && playNotes)note(t,p,key,velocity,length>0?length:-1);
            t.p.portamentoKey=static_cast<std::uint8_t>(key);
            if(t.p.flags&noteWait) {t.wait=length;if(!length)t.p.flags|=finishWait;}
            continue;
        }
        if(cmd==0x80 || cmd==0x81) {
            const int n=argument(2);if(run) {if(cmd==0x80)t.wait=n;else if(n<65536)t.p.program=static_cast<std::uint16_t>(n);}continue;
        }
        if(cmd==0x93) {
            const unsigned index=read(t),offset=read(t,3);
            if(run)if(auto* next=track(p,index);next && next!=&t) {release(*next,p,-1,true);next->data=t.data;seek(*next,offset);}continue;
        }
        if(cmd==0x94 || cmd==0x95) {
            const unsigned offset=read(t,3);if(run) {
                if(cmd==0x94)seek(t,offset);
                else if(t.depth<3) {t.returns[t.depth++]=t.cursor;seek(t,offset);}
            }continue;
        }
        if(cmd>=0xb0 && cmd<=0xbd) {
            const auto index=read(t);const auto n=static_cast<std::int16_t>(argument(1));if(!run)continue;
            auto* var=variable(p,index);int v=*var;
            switch(cmd) {
            case 0xb0:v=n;break;case 0xb1:v+=n;break;case 0xb2:v-=n;break;case 0xb3:v*=n;break;case 0xb4:if(n)v/=n;break;
            case 0xb5:
                if(n>=0)v=n>=32?0:static_cast<std::int32_t>(static_cast<std::uint32_t>(v)<<n);
                else v=-int(n)>=32?(v<0?-1:0):v>>-int(n);break;
            case 0xb6:v=static_cast<int>((random()*(unsigned(std::abs(int(n)))+1))>>16);if(n<0)v=-v;break;
            case 0xb8:flag(t.p.flags,comparison,v==n);break;case 0xb9:flag(t.p.flags,comparison,v>=n);break;
            case 0xba:flag(t.p.flags,comparison,v>n);break;case 0xbb:flag(t.p.flags,comparison,v<=n);break;
            case 0xbc:flag(t.p.flags,comparison,v<n);break;case 0xbd:flag(t.p.flags,comparison,v!=n);break;
            }
            if(cmd<0xb8)*var=static_cast<std::int16_t>(v);continue;
        }
        if(cmd>=0xc0 && cmd<=0xd7) {
            const auto n=static_cast<std::uint8_t>(argument(0));if(!run)continue;
            switch(cmd) {
            case 0xc0:t.p.pan=static_cast<std::int8_t>(n-64);break;
            case 0xc1:t.p.volume=n;break;case 0xc2:p.p.volume=n;break;case 0xc3:t.p.transpose=static_cast<std::int8_t>(n);break;
            case 0xc4:t.p.bend=static_cast<std::int8_t>(n);break;case 0xc5:t.p.bendRange=n;break;case 0xc6:t.p.priority=n;break;
            case 0xc7:flag(t.p.flags,noteWait,n&1);break;
            case 0xc8:flag(t.p.flags,tied,n&1);release(t,p,-1,true);break;
            case 0xc9:t.p.portamentoKey=static_cast<std::uint8_t>(n+t.p.transpose);t.p.flags|=portamento;break;
            case 0xca:t.p.modulation.depth=n;break;case 0xcb:t.p.modulation.speed=n;break;
            case 0xcc:t.p.modulation.target=n;break;case 0xcd:t.p.modulation.range=n;break;
            case 0xce:flag(t.p.flags,portamento,n&1);break;case 0xcf:t.p.portamentoTime=n;break;
            case 0xd0:t.p.attack=n;break;case 0xd1:t.p.decay=n;break;case 0xd2:t.p.sustain=n;break;case 0xd3:t.p.release=n;break;
            case 0xd4:if(t.depth<3) {t.returns[t.depth]=t.cursor;t.loops[t.depth++]=n;}break;
            case 0xd5:t.p.expression=n;break;
            case 0xd6:break; // Print-variable is disabled by the retail driver.
            case 0xd7:mute(t,p,n);break;
            }continue;
        }
        if(cmd==0xe0 || cmd==0xe1 || cmd==0xe3) {
            const auto n=static_cast<std::uint16_t>(argument(1));if(run) {
                if(cmd==0xe0)t.p.modulation.delay=n;else if(cmd==0xe1)p.p.tempo=n;else t.p.sweep=static_cast<std::int16_t>(n);
            }continue;
        }
        if(cmd==0xfc) {
            if(run && t.depth) {auto& remaining=t.loops[t.depth-1];if(remaining && --remaining==0)--t.depth;else seek(t,t.returns[t.depth-1]);}continue;
        }
        if(cmd==0xfd) {if(run && t.depth)seek(t,t.returns[--t.depth]);continue;}
        if(cmd==0xfe)continue;
        if(cmd==0xff) {if(run)return false;continue;}
        throw std::runtime_error("Unknown sequence opcode: "+std::to_string(cmd));
    }
    return true;
}
bool SoundSequence::tick(Player& p,bool playNotes) {
    bool playing=false;
    for(unsigned i=0;i<16;++i)if(auto* t=track(p,i);t && t->running) {
        if(step(*t,p,playNotes))playing=true;else stopTrack(p,i);
    }
    return playing;
}
void SoundSequence::update(bool periodic) {
    unsigned status=0;
    for(auto& p:players_)if(p.p.flags&active) {
        if(p.p.flags&prepared) {
            if(periodic && !(p.p.flags&paused)) {
                const unsigned ticks=p.p.counter/240;p.p.counter%=240;unsigned completed=0;
                for(;completed<ticks;++completed)if(!tick(p,true)) {stop(p);break;}
                if(data_02116100)data_02116100->player[p.p.id].tickCounter+=completed;
                p.p.counter=static_cast<std::uint16_t>(p.p.counter+((unsigned(p.p.tempo)*p.p.ratio)>>8));
            }
            for(unsigned i=0;i<16;++i)if(auto* t=track(p,i))updateVoices(*t,p,true);
        }
        if(p.p.flags&active)status|=1u<<p.p.id;
    }
    if(data_02116100)data_02116100->playerStatus=status;
}
void SoundSequence::command(unsigned id,const unsigned* a) {
    if(id==30 || id==31) {
        for(auto& p:players_)if(p.p.flags&active) {
            bool invalid=id==31 && within(p.bank,a[0],a[1]);
            if(id==30)for(unsigned i=0;i<16;++i)if(auto* t=track(p,i);t && t->running && within(t->data.data()+t->cursor,a[0],a[1]))invalid=true;
            if(invalid)stop(p);
        }
        if(id==30)for(auto it=data_.begin();it!=data_.end();) {if(within(it->first,a[0],a[1]))it=data_.erase(it);else ++it;}
        return;
    }
    if(id==11) {if(a[0]>=16 || !data_02116100)throw std::runtime_error("Invalid global sequence variable");data_02116100->globalVariable[a[0]]=static_cast<std::int16_t>(a[1]);return;}
    const unsigned player=id==7?a[0]&0xffffff:a[0];if(player>=16)throw std::runtime_error("Invalid sequence player");
    auto& p=players_[player];
    switch(id) {
    case 0:case 2:prepare(p,reinterpret_cast<void*>(a[1]),a[2],reinterpret_cast<void*>(a[3]));if(id==0)p.p.flags|=prepared;break;
    case 1:stop(p);break;
    case 3:p.p.flags|=prepared;break;
    case 4:
        flag(p.p.flags,paused,a[1]!=0);if(a[1])for(unsigned i=0;i<16;++i)if(auto* t=track(p,i))release(*t,p,127,true);break;
    case 5: {
        for(unsigned i=0;i<16;++i)if(auto* t=track(p,i))release(*t,p,127,true);
        unsigned completed=0;for(;completed<a[1];++completed)if(!tick(p,false)) {stop(p);break;}
        if(data_02116100)data_02116100->player[player].tickCounter+=completed;break;
    }
    case 6:parameter(&p.p,sizeof(p.p),a[1],a[2],a[3]);break;
    case 7:case 8:case 9:
        for(unsigned i=0;i<16;++i)if(a[1]&(1u<<i))if(auto* t=track(p,i)) {
            if(id==7)parameter(&t->p,sizeof(t->p),a[2],a[3],a[0]>>24);
            else if(id==8)mute(*t,p,a[2]);
            else {t->p.channelMask=static_cast<std::uint16_t>(a[2]);t->p.flags|=strongMask;}
        }break;
    case 10:if(a[1]>=16)throw std::runtime_error("Invalid local sequence variable");*variable(p,a[1])=static_cast<std::int16_t>(a[2]);break;
    default:throw std::runtime_error("Invalid native sequence command");
    }
}
}
