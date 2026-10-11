#pragma once
#include "GpuRenderer.hpp"
#include "TextCanvas.hpp"
#include <cstdint>
#include <functional>
#include <memory>
#include <span>

struct SDL_Window;
struct SDL_Gamepad;

namespace pc {
class AudioOutput;

// Game window: OpenGL presentation, input, audio output, frame pacing and the
// PC interface (settings screen, frame counter).
class GameWindow {
public:
    GameWindow(bool hidden,bool mute);
    ~GameWindow();
    bool poll();
    GpuRenderer& gpu() {return *gpu_;}
    // Drawable size in pixels and the size the canvas occupies inside it.
    void drawableSize(int& width,int& height) const;
    void canvasPixels(const GpuLayout&,float& width,float& height) const;
    void present(const GpuLayout&,std::span<const std::uint32_t> recipes);
    void audio(std::span<const std::int16_t> samples);
    void pace();
    // Frame rates above 60: the presenter draws frames between the game's
    // (`alpha` of the way from the previous one), until the next one is due.
    void setInterpolator(std::function<bool(float)> interpolator) {interpolator_=std::move(interpolator);}
    bool interpolating() const;
    int frameRateTarget() const; // 0: unlimited
    // Applies gameSettings() to the window, the renderer and the audio.
    void applySettings();
private:
    SDL_Window* window_=nullptr;
    void* context_=nullptr;
    SDL_Gamepad* gamepad_=nullptr;
    std::unique_ptr<AudioOutput> audio_;
    std::unique_ptr<GpuRenderer> gpu_;
    GpuLayout layout_;
    std::span<const std::uint32_t> recipes_;
    std::uint64_t deadline_=0,fpsStart_=0;
    unsigned fpsFrames_=0,fpsShown_=0;
    bool running_=true,stretch_=false,screenshot_=false,hidden_=false,settingsUsed_=false,fpsChanged_=true,suppressInput_=false;
    std::unique_ptr<TextCanvas> fpsPicture_;
    std::function<bool(float)> interpolator_;
    bool stickZoom_=false; // the right stick is zooming (saved when released)
    bool partyChatHeld_=false; // the party chat control was down at the last poll
    void measureBetween(std::uint64_t cost);
    std::uint64_t betweenCost_=4'000'000; // nanoseconds an interpolated frame takes
    void drawInterface(int width,int height);
    void setFullscreen(bool);
};
}
