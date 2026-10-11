#pragma once
#include <cstdint>
#include <span>
#include <vector>
namespace pc {
// Native representation of the SDK's 12-byte SNDWaveParam followed by samples.
// Loop positions below are in decoded samples, never compressed byte offsets.
struct SoundWave {
    std::vector<std::int16_t> samples;
    std::uint32_t loopStart=0;
    std::uint16_t rate=0,timer=0;
    bool loop=false;
    std::uint8_t format=0;
    static SoundWave decode(std::span<const std::uint8_t> data);
};
struct AdpcmState {
    int predictor=0,index=0;
    // Hardware instrument playback clips negative overflow to -32767. The
    // original NitroSystem software stream decoder instead clips to -32768.
    std::int16_t decode(unsigned nibble,bool hardware=false);
};
}
