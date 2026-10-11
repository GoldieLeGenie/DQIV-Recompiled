#pragma once
#include "SoundMixer.hpp"
#include <optional>
#include <unordered_map>
namespace pc {
struct SoundInstrument {
    unsigned type=0,wave=0,archive=0,root=60,attack=127,decay=127,sustain=127,release=127,pan=64;
};
struct SoundModulation {std::uint8_t target=0,speed=16,depth=0,range=1;std::uint16_t delay=0;};
class SoundVoices {
public:
    struct Voice {
        bool active=false,start=false,autoSweep=true;
        unsigned sync=0,type=0,duty=0,priority=0,volume=127,timer=8006,pan=64;
        int owner=-1,length=0,key=60,root=60,velocity=127,initialPan=0,panRange=127;
        int decay=0,fade=0,pitch=0,userPan=0;
        int envelope=-92544,stage=0,attack=0,sustain=127;
        unsigned decayRate=65535,releaseRate=65535;
        int sweep=0,sweepLength=0,sweepCounter=0;
        SoundModulation mod{};
        unsigned modCounter=0,modDelay=0;
        std::uint64_t serial=0;
        const void* bank=nullptr;
        const std::uint8_t* waveData=nullptr;
        std::shared_ptr<const SoundWave> wave;
    };
    explicit SoundVoices(SoundMixer& mixer):mixer_(mixer) {}
    std::array<Voice,16> voices{};
    static std::optional<SoundInstrument> instrument(const void* bank,unsigned program,unsigned key);
    int allocate(unsigned mask,unsigned priority,bool strong,int owner);
    bool start(int id,const void* bank,const SoundInstrument& instrument,int key,int velocity,int length);
    void attack(Voice& voice,unsigned value);
    void decay(Voice& voice,unsigned value);
    void sustain(Voice& voice,unsigned value);
    void release(Voice& voice,unsigned value);
    void release(Voice& voice);
    void lock(unsigned mask,unsigned flags);
    void unlock(unsigned mask,unsigned flags);
    void stopUnlocked(unsigned mask);
    void invalidate(const void* first,const void* last);
    void apply();
    void update(bool periodic);
    static unsigned pitchTimer(unsigned base,int pitch);
private:
    SoundMixer& mixer_;
    unsigned locked_=0,weakLocked_=0;
    std::uint64_t serial_=0;
    std::unordered_map<const std::uint8_t*,std::shared_ptr<const SoundWave>> waves_;
};
}
