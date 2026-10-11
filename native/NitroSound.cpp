#include "NitroSound.hpp"
#include "SoundSequence.hpp"
#include <algorithm>
#include <array>
#include <exception>
#include <stdexcept>
#include <string>
#include <unordered_set>
extern "C" {
#include "../game/libs/NitroSDK/src/snd/snd_internal.h"
extern BOOL data_021145a0;
extern UnkSndSharedWork* data_02116100;
}
namespace pc {
namespace {thread_local SoundRuntime* current=nullptr;}
struct SoundRuntime {
    struct Alarm {std::uint64_t next=0;unsigned tick=0,period=0;std::uint8_t id=0;bool configured=false,active=false;};
    SoundMixer mixer;
    SoundVoices voices;
    SoundSequence sequence;
    std::array<Alarm,8> alarms{};
    std::exception_ptr failure;
    std::uint64_t elapsed=0;
    bool rendering=false;
    std::uint64_t driverNext=0;
    std::unordered_set<const u32*> absoluteWaveSlots;
    explicit SoundRuntime(unsigned rate):mixer(rate),voices(mixer),sequence(voices) {
        if(current)throw std::runtime_error("Native sound runtime already active");
        data_021145a0=FALSE;data_02116100=nullptr;current=this;
        driverNext=ticks(2728);
    }
    ~SoundRuntime() {data_021145a0=FALSE;data_02116100=nullptr;current=nullptr;}
    static SoundRuntime& get() {if(!current)throw std::runtime_error("No native sound runtime is active");return *current;}
    void check() {if(failure)std::rethrow_exception(failure);}
    void status() {if(data_02116100)data_02116100->channelStatus=static_cast<u16>(mixer.activeMask());}
    std::uint64_t ticks(unsigned count) const {return std::uint64_t(count)*32*mixer.outputRate();}
    void dispatch(const UnkSndCommand& cmd) {
        const auto* a=cmd.arg;
        switch(cmd.id) {
        case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:case 10:case 11:
        case 30:case 31:sequence.command(cmd.id,a);break;
        case 12:case 13: {
            if(a[1])throw std::runtime_error("Native sound capture is not implemented yet");
            if(a[2]&~255u)throw std::runtime_error("Invalid sound alarm mask");
            if(cmd.id==12)mixer.start(a[0]);else mixer.stop(a[0],(a[3]&1)!=0);
            for(unsigned i=0;i<8;++i)if(a[2]&(1u<<i)) {
                auto& alarm=alarms[i];
                if(cmd.id==12) {
                    if(!alarm.configured)throw std::runtime_error("Starting an unconfigured sound alarm");
                    if(!alarm.active) {alarm.next=elapsed+ticks(alarm.tick);alarm.active=true;}
                }else alarm.active=false;
            }
            break;
        }
        case 14: {
            const unsigned id=a[0]&65535,format=(a[3]>>24)&3,loop=(a[3]>>26)&3,start=a[3]&65535,len=a[2]&0x3fffff;
            const auto bytes=(std::size_t(start)+len)*4;
            if(!a[1])throw std::runtime_error("Null sound PCM buffer");
            mixer.setupPcm(id,format,{reinterpret_cast<const std::uint8_t*>(a[1]),bytes},loop,start,len,a[0]>>16);
            mixer.volume(1u<<id,a[2]>>24,(a[2]>>22)&3);mixer.pan(1u<<id,(a[3]>>16)&127);break;
        }
        case 15:case 16: {
            if(cmd.id==15)mixer.setupPsg(a[0],a[3],a[2]>>8);else mixer.setupNoise(a[0],a[2]>>8);
            mixer.volume(1u<<a[0],a[1]&255,a[1]>>8);mixer.pan(1u<<a[0],a[2]&255);break;
        }
        case 18: {
            if(a[0]>=8)throw std::runtime_error("Invalid sound alarm number");
            alarms[a[0]]={0,a[1],a[2],static_cast<std::uint8_t>(a[3]),true,false};break;
        }
        case 19:mixer.timer(a[0],a[1]);break;
        case 20:mixer.volume(a[0],a[1],a[2]);break;
        case 21:mixer.pan(a[0],a[1]);break;
        case 23:mixer.masterVolume(a[0]);break;
        case 24:mixer.masterPan(static_cast<int>(a[0]));break;
        case 25:
            if(a[2]>1 || a[3]>1)throw std::runtime_error("Invalid channel output bypass");
            mixer.outputSelector(a[0],a[1],a[2]!=0,a[3]!=0);break;
        case 26:voices.lock(a[0],a[1]);break;
        case 27:if(a[0]&~65535u)throw std::runtime_error("Invalid channel unlock mask");voices.unlock(a[0],a[1]);break;
        case 28:voices.stopUnlocked(a[0]);break;
        case 29:
            if(reinterpret_cast<UnkSndSharedWork*>(a[0])!=data_02116100)throw std::runtime_error("Invalid shared sound state");
            break;
        case 32:
            voices.invalidate(reinterpret_cast<void*>(a[0]),reinterpret_cast<void*>(a[1]));
            mixer.invalidate(reinterpret_cast<void*>(a[0]),reinterpret_cast<void*>(a[1]));break;
        default:throw std::runtime_error("Native sound command not implemented yet: "+std::to_string(cmd.id));
        }
        status();
    }
    void process(bool periodic) {
        sequence.update(periodic);voices.update(periodic);status();sequence.advanceRandom();
    }
    void render(std::span<std::int16_t> output) {
        check();if(output.size()&1)throw std::runtime_error("Invalid stereo frame count");
        if(rendering)throw std::runtime_error("Native sound render reentered from a scheduler callback");
        struct RenderingGuard {bool& value;~RenderingGuard(){value=false;}} guard{rendering};rendering=true;
        while(!output.empty()) {
            std::uint64_t step=std::min<std::uint64_t>(output.size()/2,
                driverNext<=elapsed?0:(driverNext-elapsed+16756990)/16756991);
            for(const auto& alarm:alarms)if(alarm.active) {
                const auto count=alarm.next<=elapsed?0:(alarm.next-elapsed+16756990)/16756991;
                step=std::min(step,count);
            }
            if(step) {mixer.render(output.first(static_cast<std::size_t>(step)*2));output=output.subspan(static_cast<std::size_t>(step)*2);elapsed+=step*16756991;}
            unsigned callbacks=0;
            for(;;) {
                unsigned i=8;
                for(unsigned n=0;n<8;++n)if(alarms[n].active && alarms[n].next<=elapsed &&
                    (i==8 || alarms[n].next<alarms[i].next))i=n;
                if(driverNext<=elapsed && (i==8 || driverNext<=alarms[i].next)) {
                    voices.apply();process(true);driverNext+=ticks(2728);continue;
                }
                if(i==8)break;
                if(++callbacks>100000)throw std::runtime_error("Sound alarm continually restarts at the same time");
                auto& alarm=alarms[i];
                    const auto id=alarm.id;
                    if(alarm.period)alarm.next+=ticks(alarm.period);else alarm.active=false;
                    // Runs on the game thread; callbacks may wake SDK workers
                    // and refill the same PCM buffer before its next sample.
                    func_0207aed4(static_cast<s32>(i|(unsigned(id)<<8)));
            }
        }
        status();
    }
};
NitroSound::NitroSound(unsigned rate):impl_(std::make_unique<SoundRuntime>(rate)) {}
NitroSound::~NitroSound()=default;
void NitroSound::render(std::span<std::int16_t> stereo) {impl_->render(stereo);}
SoundMixer& NitroSound::mixer() {return impl_->mixer;}
}
extern "C" {
void pcSndRequire() {(void)pc::SoundRuntime::get();}
BOOL pcSndExecute(UnkSndCommand* list) {
    auto& runtime=pc::SoundRuntime::get();
    try {
        runtime.check();unsigned count=0;
        runtime.voices.apply();
        for(auto* cmd=list;cmd;cmd=cmd->next) {if(++count>256)throw std::runtime_error("Invalid sound command list");runtime.dispatch(*cmd);}
        if(!data_02116100)throw std::runtime_error("Missing sound shared work");
        runtime.process(false);
        data_02116100->finishCommandTag=data_02116100->finishCommandTag+1;
        return TRUE;
    }catch(...) {runtime.failure=std::current_exception();return FALSE;}
}
void pcSndRethrow() {pc::SoundRuntime::get().check();}
void pcSndRegisterSequence(const void* file,unsigned offset,unsigned size) {
    if(!file || offset>=size)throw std::runtime_error("Invalid sequence file bounds");
    pc::SoundRuntime::get().sequence.registerData({static_cast<const std::uint8_t*>(file)+offset,size-offset});
}
void pcSndWait() {
    pc::SoundRuntime::get().check();
    throw std::runtime_error("Waiting for a sound command list that was not submitted");
}
void pcSndMarkWaveAddress(const u32* slot) {pc::SoundRuntime::get().absoluteWaveSlots.insert(slot);}
const void* pcSndResolveWaveAddress(const void* arc,const u32* slot) {
    if(!*slot)return nullptr;
    if(pc::SoundRuntime::get().absoluteWaveSlots.contains(slot))return reinterpret_cast<const void*>(*slot);
    return static_cast<const std::uint8_t*>(arc)+*slot;
}
void pcSndForgetWaveAddresses(const u32* first,u32 count) {
    auto& slots=pc::SoundRuntime::get().absoluteWaveSlots;
    for(u32 i=0;i<count;++i)slots.erase(first+i);
}
}
