#pragma once
#include "SoundWave.hpp"
#include <array>
#include <memory>
#include <span>
namespace pc {
// The game's 16 SDK sound channels, mixed on the owning game thread. SDL only
// receives the resulting PCM; it never invokes game code from its audio thread.
class SoundMixer {
public:
    explicit SoundMixer(unsigned outputRate=48000);
    void setupWave(unsigned channel,std::shared_ptr<const SoundWave> wave);
    // Borrowed PCM is deliberately read during playback: NNS refills stream
    // buffers in place. The caller must keep it alive until the channel stops.
    void setupPcm(unsigned channel,unsigned format,std::span<const std::uint8_t> data,
                  unsigned loop,unsigned loopStartWords,unsigned lengthWords,unsigned timer);
    void setupPsg(unsigned channel,unsigned duty,unsigned timer);
    void setupNoise(unsigned channel,unsigned timer);
    void volume(unsigned mask,unsigned value,unsigned shift);
    void pan(unsigned mask,unsigned value);
    void timer(unsigned mask,unsigned value);
    void masterVolume(unsigned value);
    void masterPan(int value);
    void outputSelector(unsigned left,unsigned right,bool bypass1,bool bypass3);
    void invalidate(const void* start,const void* end);
    void start(unsigned mask);
    void stop(unsigned mask,bool hold=false);
    unsigned activeMask() const;
    void render(std::span<std::int16_t> interleavedStereo);
    unsigned outputRate() const {return rate_;}
    std::uint64_t renderedFrames() const {return frames_;}
private:
    enum class Source {None,Wave,Pcm,Psg,Noise};
    struct Channel {
        Source source=Source::None;
        std::shared_ptr<const SoundWave> wave;
        std::span<const std::uint8_t> data;
        std::uint64_t phase=0;
        std::size_t position=0,length=0,loopStart=0;
        unsigned format=0,loop=2,timer=512,volume=127,shift=0,pan=64,duty=0,delay=0;
        std::uint16_t noise=0x7fff;
        AdpcmState adpcm{},loopState{};
        int sample=0;
        bool running=false,hold=false,loopSaved=false;
    };
    Channel& channel(unsigned id);
    void loadSample(Channel& c);
    void advance(Channel& c);
    std::array<Channel,16> channels_{};
    unsigned rate_,master_=127;
    int masterPan_=-1;
    unsigned leftOutput_=0,rightOutput_=0;
    bool bypass1_=false,bypass3_=false;
    std::uint64_t frames_=0;
};
}
