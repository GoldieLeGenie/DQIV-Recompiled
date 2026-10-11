#pragma once
#include "Assets.hpp"
#include <cstdint>
#include <memory>
#include <span>
#include <vector>
// The smartphone version's recordings (assets/data/SOUND/MOBILE: AKB files,
// Ogg Vorbis music and MS ADPCM effects), played in place of the DS
// sequences: BGMnn is sequence nn (BGM01 the overture stream), SEnnnn is
// effect nnnn of the effect archive.
namespace pc {
enum MobileSoundKind {mobileBgm,mobileEffect,mobileStream};

struct AkbInfo {
    int codec=0,channels=0,rate=0;
    int samples=0,loopStart=0,loopEnd=0; // loop when loopEnd>loopStart
    float volume=1;
    std::size_t data=0;                  // first byte of the coded data
    int frameSize=0;                     // MS ADPCM block size
};
// False when the file is not an AKB this player handles.
bool parseAkb(std::span<const std::uint8_t> file,AkbInfo& info);
std::vector<std::int16_t> decodeMsAdpcm(std::span<const std::uint8_t> data,int channels,int frameSize,int samples);

class MobileSound {
public:
    MobileSound(const Assets& assets,unsigned rate);
    ~MobileSound();
    MobileSound(const MobileSound&)=delete;
    MobileSound& operator=(const MobileSound&)=delete;
    // Adds the playing sounds to one frame of game audio.
    void mix(std::span<std::int16_t> stereo);
    struct Impl; // shared with the handle functions below
private:
    std::unique_ptr<Impl> impl_;
};

// For the game's sound handles (DS volumes 0..127; `reference` is the volume
// the sound is meant to be heard at, so that the recording's own level is
// kept there). Start returns false when no recording replaces the sound (or
// the setting is off): the DS sequence plays then.
bool mobileSoundStart(const void* handle,MobileSoundKind kind,int id,int volume,int reference);
void mobileSoundStop(const void* handle,int fadeFrames);
// Forgets the handle's recording (a DS sequence plays on it now).
void mobileSoundRelease(const void* handle);
// Whether the handle's last sound was a recording.
bool mobileSoundOwns(const void* handle);
bool mobileSoundPlaying(const void* handle);
void mobileSoundVolume(const void* handle,int volume);
}
