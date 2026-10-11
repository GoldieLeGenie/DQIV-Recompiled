#pragma once
#include "SoundVoices.hpp"
#include <span>

namespace pc {
// The first bytes of these records are part of the SDK command protocol.
// Keep their layout even though execution uses native pointers and containers.
struct SequencePlayerParams {
    std::uint8_t flags=0,id=0,padding[2]{},priority=64,volume=127;
    std::int16_t fade=0;
    std::array<std::uint8_t,16> tracks{};
    std::uint16_t tempo=120,ratio=256,counter=240,padding2=0;
};
struct SequenceTrackParams {
    std::uint8_t flags=0,panRange=127;
    std::uint16_t program=0;
    std::uint8_t volume=127,expression=127;
    std::int8_t bend=0;
    std::uint8_t bendRange=2;
    std::int8_t pan=0,externalPan=0;
    std::int16_t fade=0,pitch=0;
    std::uint8_t attack=255,decay=255,sustain=255,release=255,priority=64;
    std::int8_t transpose=0;
    std::uint8_t portamentoKey=60,portamentoTime=0;
    std::int16_t sweep=0;
    SoundModulation modulation{};
    std::uint16_t channelMask=65535;
};
static_assert(sizeof(SequencePlayerParams)==32 && offsetof(SequencePlayerParams,tempo)==24);
static_assert(sizeof(SequenceTrackParams)==32 && offsetof(SequenceTrackParams,modulation)==24);

class SoundSequence {
public:
    struct Player {SequencePlayerParams p;const void* bank=nullptr;};
    struct Track {
        SequenceTrackParams p;
        std::span<const std::uint8_t> data;
        std::size_t cursor=0;
        bool running=false;
        int wait=0;
        std::array<std::size_t,3> returns{};
        std::array<std::uint8_t,3> loops{};
        unsigned depth=0;
    };
    explicit SoundSequence(SoundVoices& voices);
    void registerData(std::span<const std::uint8_t> data);
    void command(unsigned id,const unsigned* args);
    void update(bool periodic);
    void advanceRandom();
    const std::array<Player,16>& players() const {return players_;}
    const std::array<Track,32>& tracks() const {return tracks_;}
private:
    SoundVoices& voices_;
    std::array<Player,16> players_{};
    std::array<Track,32> tracks_{};
    std::unordered_map<const std::uint8_t*,std::size_t> data_;
    std::uint32_t random_=0x12345678;
    unsigned random();
    int allocateTrack();
    Track* track(Player& player,unsigned index);
    void prepare(Player& player,const void* base,unsigned offset,const void* bank);
    void stop(Player& player);
    void stopTrack(Player& player,unsigned index);
    void release(Track& track,Player& player,int coefficient,bool detach);
    void mute(Track& track,Player& player,unsigned mode);
    void updateVoices(Track& track,Player& player,bool releaseNotes);
    void note(Track& track,Player& player,int key,int velocity,int length);
    bool tick(Player& player,bool playNotes);
    bool step(Track& track,Player& player,bool playNotes);
    unsigned read(Track& track,unsigned bytes=1);
    int value(Track& track,Player& player,unsigned type);
    volatile std::int16_t* variable(Player& player,unsigned index);
    void seek(Track& track,std::size_t offset);
};
}
