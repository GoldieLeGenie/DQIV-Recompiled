#pragma once
#include <cstdint>
#include <span>
struct SDL_AudioStream;
namespace pc {
class AudioOutput {
public:
    explicit AudioOutput(unsigned rate=48000);
    ~AudioOutput();
    AudioOutput(const AudioOutput&)=delete;
    AudioOutput& operator=(const AudioOutput&)=delete;
    // Initially paused, allowing the game to prime its audio before playback.
    void pause(bool paused);
    void write(std::span<const std::int16_t> interleavedStereo);
    unsigned queuedFrames() const;
    void clear();
    void flush();
    std::uint32_t deviceId() const;
private:
    SDL_AudioStream* stream_=nullptr;
    unsigned rate_;
};
}
