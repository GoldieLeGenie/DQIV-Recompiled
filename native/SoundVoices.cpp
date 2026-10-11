#include "SoundVoices.hpp"
#include <algorithm>
#include <cstring>
#include <stdexcept>
extern "C" {
extern const short data_020ba8ac[128];
extern const unsigned short data_020ba9ac[768];
unsigned short func_0207b024(int);
const void* pcSndResolveWaveAddress(const void*,const unsigned*);
}
namespace pc {
namespace {
// These three tables were checked byte-for-byte against DQIV's ARM7 image:
// allocation 0238eee0, attack 0238eef0, quarter-sine 0238ecb8.
constexpr unsigned allocation[16]={4,5,6,7,2,0,3,1,8,9,10,11,14,12,15,13};
constexpr unsigned attacks[19]={0,1,5,14,26,38,51,63,73,84,92,100,109,116,123,127,132,137,143};
constexpr int sine[33]={0,6,12,19,25,31,37,43,49,54,60,65,71,76,81,85,90,94,98,102,106,109,112,115,117,120,122,123,125,126,126,127,127};
constexpr unsigned shifts[4]={0,1,2,4};
unsigned u16(const std::uint8_t* p) {return p[0]|unsigned(p[1])<<8;}
unsigned u32(const std::uint8_t* p) {return u16(p)|u16(p+2)<<16;}
void seven(unsigned n) {if(n>127)throw std::runtime_error("Invalid seven-bit instrument parameter");}
unsigned decayCoefficient(unsigned n) {seven(n);return n==127?65535:n==126?15360:n<50?n*2+1:7680/(126-n);}
int sinValue(unsigned phase) {
    phase&=127;if(phase<=32)return sine[phase];if(phase<=64)return sine[64-phase];
    return phase<=96?-sine[phase-64]:-sine[128-phase];
}
unsigned level(const SoundVoices::Voice& v) {return ((v.volume&255)<<4)>>shifts[(v.volume>>8)&3];}
bool inRange(const void* p,const void* begin,const void* end) {
    auto n=reinterpret_cast<std::uintptr_t>(p);return n>=reinterpret_cast<std::uintptr_t>(begin) && n<=reinterpret_cast<std::uintptr_t>(end);
}
}
std::optional<SoundInstrument> SoundVoices::instrument(const void* pointer,unsigned program,unsigned key) {
    if(!pointer)throw std::runtime_error("Missing sequence instrument bank");
    auto* bank=static_cast<const std::uint8_t*>(pointer);const auto bytes=u32(bank+8);
    auto field=[&](std::size_t offset,std::size_t size) {
        if(offset>bytes || size>bytes-offset)throw std::runtime_error("Instrument points outside sound bank");
        return bank+offset;
    };
    const auto count=u32(field(0x38,4));if(program>=count)return {};
    const unsigned entry=u32(field(0x3c+std::size_t(program)*4,4));unsigned type=entry&255;
    const auto offset=entry>>8;const std::uint8_t* param;
    if(type>=1 && type<=5)param=field(offset,10);
    else if(type==16) {
        const auto* limits=field(offset,2);if(key<limits[0] || key>limits[1])return {};
        const auto* nested=field(offset+2+(key-limits[0])*12,12);type=u16(nested);param=nested+2;
    }else if(type==17) {
        const auto* keys=field(offset,8);unsigned region=0;while(region<8 && key>keys[region])++region;
        if(region==8)return {};const auto* nested=field(offset+8+region*12,12);type=u16(nested);param=nested+2;
    }else return {};
    return SoundInstrument{type,u16(param),u16(param+2),param[4],param[5],param[6],param[7],param[8],param[9]};
}
int SoundVoices::allocate(unsigned mask,unsigned priority,bool strong,int owner) {
    mask&=~locked_;if(!strong)mask&=~weakLocked_;
    int selected=-1;
    for(unsigned id:allocation)if(mask&(1u<<id)) {
        if(selected<0 || voices[id].priority<voices[selected].priority ||
            (voices[id].priority==voices[selected].priority && level(voices[id])<level(voices[selected])))selected=static_cast<int>(id);
    }
    if(selected<0 || priority<voices[selected].priority)return -1;
    Voice fresh;fresh.sync=2;fresh.priority=priority&255;fresh.owner=owner;fresh.serial=++serial_;
    voices[selected]=std::move(fresh);return selected;
}
bool SoundVoices::start(int id,const void* bank,const SoundInstrument& inst,int key,int velocity,int length) {
    auto& v=voices.at(id);seven(inst.root);seven(inst.pan);seven(velocity);v.bank=bank;
    v.type=inst.type;v.duty=inst.wave;v.key=key;v.root=inst.root;v.velocity=velocity;v.initialPan=int(inst.pan)-64;
    if(inst.type==1 || inst.type==4) {
        const std::uint8_t* raw=nullptr;
        if(inst.type==4)raw=reinterpret_cast<const std::uint8_t*>((inst.archive<<16)|inst.wave);
        else {
            if(inst.archive>=4)throw std::runtime_error("Invalid instrument wave archive");
            const void* arc;std::memcpy(&arc,static_cast<const std::uint8_t*>(bank)+0x18+inst.archive*8,sizeof(arc));
            // The driver reads bank metadata directly, as ARM7 did. Calling
            // ARM9's mutex-protected getter here can reschedule the game while
            // a driver tick is only partly applied.
            if(arc && inst.wave<u32(static_cast<const std::uint8_t*>(arc)+0x38))
                raw=static_cast<const std::uint8_t*>(pcSndResolveWaveAddress(arc,reinterpret_cast<const unsigned*>(static_cast<const std::uint8_t*>(arc)+0x3c)+inst.wave));
        }
        if(!raw) {v.priority=0;v.owner=-1;return false;}
        auto cached=waves_.find(raw);
        if(cached==waves_.end()) {
            const auto lengthBytes=(std::uint64_t(u16(raw+6))+u32(raw+8))*4;
            if(lengthBytes>0x1000000)throw std::runtime_error("Invalid instrument wave length");
            cached=waves_.emplace(raw,std::make_shared<SoundWave>(SoundWave::decode({raw,static_cast<std::size_t>(lengthBytes)+12}))).first;
        }
        v.wave=cached->second;v.waveData=raw;v.timer=v.wave->timer;
    }else if(inst.type==2) {if(id<8 || id>13)return false;}
    else if(inst.type==3) {if(id<14 || id>15)return false;}
    else return false;
    attack(v,inst.attack);decay(v,inst.decay);sustain(v,inst.sustain);release(v,inst.release==255?0:inst.release);
    v.length=inst.release==255?-1:length;v.active=true;v.start=true;return true;
}
void SoundVoices::attack(Voice& v,unsigned value) {seven(value);v.attack=value<109?255-value:attacks[127-value];}
void SoundVoices::decay(Voice& v,unsigned value) {v.decayRate=decayCoefficient(value);}
void SoundVoices::sustain(Voice& v,unsigned value) {seven(value);v.sustain=value;}
void SoundVoices::release(Voice& v,unsigned value) {v.releaseRate=decayCoefficient(value);}
void SoundVoices::release(Voice& v) {if(v.active) {v.stage=3;v.priority=1;}}
void SoundVoices::stopUnlocked(unsigned mask) {
    for(unsigned i=0;i<16;++i)if((mask&~locked_)&(1u<<i)) {mixer_.stop(1u<<i);voices[i]=Voice{};voices[i].priority=0;voices[i].volume=0;}
}
void SoundVoices::lock(unsigned mask,unsigned flags) {stopUnlocked(mask);if(flags&1)weakLocked_|=mask;else locked_|=mask;}
void SoundVoices::unlock(unsigned mask,unsigned flags) {if(flags&1)weakLocked_&=~mask;else locked_&=~mask;}
void SoundVoices::invalidate(const void* first,const void* last) {
    for(unsigned i=0;i<16;++i)if(voices[i].active && voices[i].waveData && inRange(voices[i].waveData,first,last)) {
        mixer_.stop(1u<<i);voices[i].start=false;
    }
    for(auto it=waves_.begin();it!=waves_.end();) {if(inRange(it->first,first,last))it=waves_.erase(it);else ++it;}
}
unsigned SoundVoices::pitchTimer(unsigned base,int pitch) {
    int shift=-pitch/768;int rem=-pitch%768;if(rem<0) {rem+=768;--shift;}
    std::uint64_t value=std::uint64_t(65536+data_020ba9ac[rem])*base;
    shift-=16;
    if(shift<0)value=-shift>=64?0:value>>-shift;
    else if(shift>=32 || value>(65535ull>>shift))return 65535;
    else value<<=shift;
    return static_cast<unsigned>(std::clamp<std::uint64_t>(value,16,65535));
}
void SoundVoices::apply() {
    for(unsigned i=0;i<16;++i) {
        auto& v=voices[i];const unsigned mask=1u<<i;
        if(v.sync&2)mixer_.stop(mask);
        if(v.sync&1) {
            if(v.wave)mixer_.setupWave(i,v.wave);
            else if(v.type==2)mixer_.setupPsg(i,v.duty,v.timer);
            else if(v.type==3)mixer_.setupNoise(i,v.timer);
            else throw std::runtime_error("Starting an invalid sequence voice");
        }
        if(v.sync&(1|4))mixer_.timer(mask,v.timer);
        if(v.sync&(1|8))mixer_.volume(mask,v.volume&255,v.volume>>8);
        if(v.sync&(1|16))mixer_.pan(mask,v.pan);
    }
    for(unsigned i=0;i<16;++i) {if(voices[i].sync&1)mixer_.start(1u<<i);voices[i].sync=0;}
}
void SoundVoices::update(bool periodic) {
    for(unsigned id=0;id<16;++id) {
        auto& v=voices[id];if(!v.active)continue;
        if(v.start) {v.start=false;v.sync|=1;}
        else if(!(mixer_.activeMask()&(1u<<id))) {v.active=false;v.owner=-1;v.priority=0;v.volume=0;continue;}
        if(periodic) {
            if(v.stage==0) {v.envelope=-((-v.envelope*v.attack)>>8);if(!v.envelope)v.stage=1;}
            else if(v.stage==1) {
                v.envelope-=v.decayRate;const int floor=data_020ba8ac[v.sustain]*128;
                if(v.envelope<=floor) {v.envelope=floor;v.stage=2;}
            }else if(v.stage==3)v.envelope-=v.releaseRate;
        }
        int volume=data_020ba8ac[v.velocity]+(v.envelope>>7)+v.decay+v.fade;
        int pitch=(v.key-v.root)*64+v.pitch;
        if(v.sweep && v.sweepCounter<v.sweepLength) {
            pitch+=static_cast<int>(std::int64_t(v.sweep)*(v.sweepLength-v.sweepCounter)/v.sweepLength);
            if(periodic && v.autoSweep)++v.sweepCounter;
        }
        int modulation=0;
        if(v.mod.depth && v.modDelay>=v.mod.delay)modulation=sinValue(v.modCounter>>8)*v.mod.depth*v.mod.range;
        modulation=static_cast<int>((std::int64_t(modulation)*(v.mod.target==1?60:64))>>14);
        int pan=0;
        if(v.mod.target==0)pitch+=modulation;
        else if(v.mod.target==1 && volume>-32768)volume+=modulation;
        else if(v.mod.target==2)pan+=modulation;
        if(periodic) {if(v.modDelay<v.mod.delay)++v.modDelay;else v.modCounter=(v.modCounter+(v.mod.speed<<6))&32767;}
        pan+=v.initialPan;if(v.panRange!=127)pan=(pan*v.panRange+64)>>7;pan+=v.userPan;
        if(v.stage==3 && volume<=-723) {v.sync=2;v.active=false;v.owner=-1;v.priority=0;v.volume=0;continue;}
        const auto newVolume=func_0207b024(volume);
        unsigned newTimer=pitchTimer(v.wave?v.wave->timer:8006,pitch);if(v.type==2)newTimer&=~3u;
        const auto newPan=static_cast<unsigned>(std::clamp(pan+64,0,127));
        if(v.volume!=newVolume) {v.volume=newVolume;v.sync|=8;}
        if(v.timer!=newTimer) {v.timer=newTimer;v.sync|=4;}
        if(v.pan!=newPan) {v.pan=newPan;v.sync|=16;}
    }
}
}
