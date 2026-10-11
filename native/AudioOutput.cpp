#include "AudioOutput.hpp"
#include <SDL3/SDL.h>
#include <limits>
#include <stdexcept>
#include <string>
namespace pc {
namespace {
void requireAudio(bool success) {if(!success)throw std::runtime_error(std::string("Native audio: ")+SDL_GetError());}
}
AudioOutput::AudioOutput(unsigned rate):rate_(rate) {
    if(rate<8000 || rate>192000)throw std::runtime_error("Invalid native audio output rate");
    requireAudio(SDL_InitSubSystem(SDL_INIT_AUDIO));
    const SDL_AudioSpec spec{SDL_AUDIO_S16,2,static_cast<int>(rate)};
    stream_=SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,&spec,nullptr,nullptr);
    if(!stream_) {
        const std::string error=SDL_GetError();SDL_QuitSubSystem(SDL_INIT_AUDIO);
        throw std::runtime_error("Native audio device: "+error);
    }
}
AudioOutput::~AudioOutput() {SDL_DestroyAudioStream(stream_);SDL_QuitSubSystem(SDL_INIT_AUDIO);}
void AudioOutput::pause(bool paused) {
    requireAudio(paused?SDL_PauseAudioStreamDevice(stream_):SDL_ResumeAudioStreamDevice(stream_));
}
void AudioOutput::write(std::span<const std::int16_t> stereo) {
    if((stereo.size()&1) || stereo.size_bytes()>std::numeric_limits<int>::max())throw std::runtime_error("Invalid native stereo audio buffer");
    // The sound card's clock drifts from the game's 60 frames per second: in
    // a long session the queue would grow without end (seconds of delay).
    // Past 300 ms queued, this frame's sound is dropped instead.
    if(stereo.size()/2+queuedFrames()>std::uint64_t(rate_)*3/10)return;
    if(!stereo.empty())requireAudio(SDL_PutAudioStreamData(stream_,stereo.data(),static_cast<int>(stereo.size_bytes())));
}
unsigned AudioOutput::queuedFrames() const {
    const int bytes=SDL_GetAudioStreamQueued(stream_);requireAudio(bytes>=0);return static_cast<unsigned>(bytes)/4;
}
void AudioOutput::clear() {requireAudio(SDL_ClearAudioStream(stream_));}
void AudioOutput::flush() {requireAudio(SDL_FlushAudioStream(stream_));}
std::uint32_t AudioOutput::deviceId() const {return SDL_GetAudioStreamDevice(stream_);}
}
