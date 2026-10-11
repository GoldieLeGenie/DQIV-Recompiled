#pragma once
#include "SoundMixer.hpp"
#include <memory>
namespace pc {
struct SoundRuntime;
class NitroSound {
public:
    explicit NitroSound(unsigned rate=48000);
    ~NitroSound();
    NitroSound(const NitroSound&)=delete;
    NitroSound& operator=(const NitroSound&)=delete;
    void render(std::span<std::int16_t> stereo);
    SoundMixer& mixer();
private:
    std::unique_ptr<SoundRuntime> impl_;
};
}
