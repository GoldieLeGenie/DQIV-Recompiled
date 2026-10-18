#include "GameWindow.hpp"
#include "AudioOutput.hpp"
#include "GlLoader.hpp"
#include "Input.hpp"
#include "DualScreen.hpp"
#include "Profile.hpp"
#include "Settings.hpp"
#include <SDL3/SDL.h>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>

namespace pc {
namespace {
void require(bool ok) {if(!ok)throw std::runtime_error(SDL_GetError());}
enum Overlay : unsigned {overlayFps=1};
// Trigger travel (0..32767) past which it counts as pressed.
constexpr int triggerThreshold=16000;
}

GameWindow::GameWindow(bool hidden,bool mute):hidden_(hidden) {
    require(SDL_InitSubSystem(SDL_INIT_VIDEO|SDL_INIT_GAMEPAD));
    try {
        require(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3));
        require(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3));
        require(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE));
        require(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1));
        window_=SDL_CreateWindow("Dragon Quest IV",1280,720,
            SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE|SDL_WINDOW_HIGH_PIXEL_DENSITY|(hidden?SDL_WINDOW_HIDDEN:0));
        require(window_!=nullptr);
        context_=SDL_GL_CreateContext(window_);
        require(context_!=nullptr);
        require(SDL_GL_MakeCurrent(window_,static_cast<SDL_GLContext>(context_)));
        SDL_GL_SetSwapInterval(1);
        gl::load();
        gpu_=std::make_unique<GpuRenderer>();
        std::printf("OpenGL: %s / %s\n",reinterpret_cast<const char*>(gl::GetString(GL_RENDERER)),reinterpret_cast<const char*>(gl::GetString(GL_VERSION)));
        if(!mute) {audio_=std::make_unique<AudioOutput>();audio_->pause(false);}
        int count=0;auto* ids=SDL_GetGamepads(&count);if(count>0)gamepad_=SDL_OpenGamepad(ids[0]);SDL_free(ids);
        deadline_=fpsStart_=SDL_GetTicksNS();
    } catch(...) {
        audio_.reset();gpu_.reset();
        if(context_)SDL_GL_DestroyContext(static_cast<SDL_GLContext>(context_));
        SDL_DestroyWindow(window_);
        SDL_QuitSubSystem(SDL_INIT_VIDEO|SDL_INIT_GAMEPAD);throw;
    }
}
GameWindow::~GameWindow() {
    setButtons(0);setTouch(false,0,0);audio_.reset();SDL_CloseGamepad(gamepad_);
    gpu_.reset();
    SDL_GL_DestroyContext(static_cast<SDL_GLContext>(context_));SDL_DestroyWindow(window_);
    SDL_QuitSubSystem(SDL_INIT_VIDEO|SDL_INIT_GAMEPAD);
}
void GameWindow::setFullscreen(bool on) {
    SDL_SetWindowFullscreen(window_,on);SDL_SyncWindow(window_);
}
void GameWindow::applySettings() {
    const auto& s=gameSettings();
    settingsUsed_=true;
    if(!s.fullscreen) {
        setFullscreen(false);
        SDL_SetWindowSize(window_,windowSizes[s.windowSize][0],windowSizes[s.windowSize][1]);
        SDL_SetWindowPosition(window_,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED);
    } else setFullscreen(true);
    // Strict vertical sync: the adaptive one (-1) shows a frame that misses
    // the refresh at once, and the game's own 60 Hz pace makes that steady,
    // a tear across the middle of the screen.
    SDL_GL_SetSwapInterval(s.vsync?1:0);
    int interval=0;SDL_GL_GetSwapInterval(&interval);
    std::printf("Vertical sync: %d\n",interval);
    dualScreen().canvasWidth=s.wide?384*16/9.f:256.f;
    stretch_=s.stretch;
    gpu_->settings.autoQuality=s.quality==0;
    gpu_->settings.supersample=s.quality==0?4:s.quality;
    gpu_->settings.smoothTextures=s.smoothTextures;
    // Textures not smoothed: the DS pixels as they are, without Scale2x.
    gpu_->settings.textureScale=s.smoothTextures?4:1;
    const int rate=frameRates[std::clamp(s.frameRate,0,frameRateCount-1)];
    gpu_->settings.frameMilliseconds=1000.0/(rate?rate:240);
    fpsChanged_=true;
}
void GameWindow::drawableSize(int& width,int& height) const {require(SDL_GetWindowSizeInPixels(window_,&width,&height));}
void GameWindow::canvasPixels(const GpuLayout& layout,float& width,float& height) const {
    int w=0,h=0;drawableSize(w,h);
    if(stretch_) {width=float(w);height=float(h);return;}
    const float scale=std::min(w/layout.canvasWidth,h/layout.canvasHeight);
    width=layout.canvasWidth*scale;height=layout.canvasHeight*scale;
}
bool GameWindow::poll() {
    // Settings changed in the game's own menu.
    if(takeSettingsApply())applySettings();
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if(e.type==SDL_EVENT_QUIT)running_=false;
        if(e.type==SDL_EVENT_MOUSE_WHEEL&&e.wheel.y!=0) {
            // The mouse wheel zooms the camera.
            zoomCamera(e.wheel.y>0?10:-10);
            if(settingsUsed_)saveSettings();
        }

        if(e.type==SDL_EVENT_GAMEPAD_ADDED&&!gamepad_)gamepad_=SDL_OpenGamepad(e.gdevice.which);
        if(e.type==SDL_EVENT_GAMEPAD_REMOVED&&gamepad_&&SDL_GetGamepadID(gamepad_)==e.gdevice.which) {
            SDL_CloseGamepad(gamepad_);gamepad_=nullptr;
        }
        if(e.type==SDL_EVENT_KEY_DOWN&&rebindWaiting()) {
            // A control being bound: Escape cancels, a key binds the keyboard page.
            if(!e.key.repeat) {
                if(e.key.key==SDLK_ESCAPE)cancelRebind();
                else if(rebindPage()==pageKeyboard) {
                    gameSettings().keys[rebindAction()]=static_cast<unsigned>(e.key.key);
                    cancelRebind();saveSettings();suppressInput_=true;
                }
            }
            continue;
        }
        if(e.type==SDL_EVENT_GAMEPAD_BUTTON_DOWN&&rebindWaiting()) {
            if(rebindPage()==pageGamepad) {
                gameSettings().pads[rebindAction()]=e.gbutton.button;
                cancelRebind();saveSettings();suppressInput_=true;
            }
            continue;
        }
        // A trigger pressed past half its travel binds like a button.
        if(e.type==SDL_EVENT_GAMEPAD_AXIS_MOTION&&rebindWaiting()) {
            const bool trigger=e.gaxis.axis==SDL_GAMEPAD_AXIS_LEFT_TRIGGER||e.gaxis.axis==SDL_GAMEPAD_AXIS_RIGHT_TRIGGER;
            if(rebindPage()==pageGamepad&&trigger&&e.gaxis.value>triggerThreshold) {
                gameSettings().pads[rebindAction()]=e.gaxis.axis==SDL_GAMEPAD_AXIS_LEFT_TRIGGER?padLeftTrigger:padRightTrigger;
                cancelRebind();saveSettings();suppressInput_=true;
            }
            continue;
        }
        if(e.type==SDL_EVENT_KEY_DOWN) {
            const auto key=e.key.key;
            if(e.key.repeat)continue;
            auto& s=gameSettings();
            if(key==SDLK_F11) {s.fullscreen=!s.fullscreen;setFullscreen(s.fullscreen);if(settingsUsed_)saveSettings();}
            if(key==SDLK_F10) {s.stretch=stretch_=!stretch_;if(settingsUsed_)saveSettings();}
            if(key==SDLK_F12)screenshot_=true;
            // Camera: keypad + / - zoom.
            if(key==SDLK_KP_PLUS||key==SDLK_KP_MINUS) {zoomCamera(key==SDLK_KP_PLUS?10:-10);if(settingsUsed_)saveSettings();}
            if(key==SDLK_F8) {
                // 16:9 picture, or the DS screens' own width.
                s.wide=!(dualScreen().canvasWidth>256);
                dualScreen().canvasWidth=s.wide?384*16/9.f:256.f;
                if(settingsUsed_)saveSettings();
            }
            if(key==SDLK_F9) {
                s.windowSize=(s.windowSize+1)%windowSizeCount;s.fullscreen=false;
                setFullscreen(false);
                SDL_SetWindowSize(window_,windowSizes[s.windowSize][0],windowSizes[s.windowSize][1]);
                if(settingsUsed_)saveSettings();
            }
        }
    }
    std::uint16_t buttons=0;bool partyChat=false;
    const auto* keys=SDL_GetKeyboardState(nullptr);
    const auto& controls=gameSettings();
    auto held=[&](SDL_Keycode key){const auto scan=SDL_GetScancodeFromKey(key,nullptr);return scan!=SDL_SCANCODE_UNKNOWN&&keys[scan];};
    auto bound=[&](SDL_Keycode key){for(const auto k:controls.keys)if(k==static_cast<unsigned>(key))return true;return false;};
    const bool focus=(SDL_GetWindowFlags(window_)&SDL_WINDOW_INPUT_FOCUS)!=0;
    if(focus) {
        // The player's keys (settings), with Space for A and Backspace and
        // Escape for B while they are bound to nothing else.
        for(int i=0;i<buttonControlCount;++i)if(held(static_cast<SDL_Keycode>(controls.keys[i])))buttons|=1u<<controlBits[i];
        partyChat=held(static_cast<SDL_Keycode>(controls.keys[controlPartyChat]));
        if(!bound(SDLK_SPACE)&&keys[SDL_SCANCODE_SPACE])buttons|=1;
        if(!bound(SDLK_BACKSPACE)&&keys[SDL_SCANCODE_BACKSPACE])buttons|=2;
        if(!bound(SDLK_ESCAPE)&&keys[SDL_SCANCODE_ESCAPE])buttons|=2;
    }
    if(gamepad_&&focus) {
        // A bound button, or a trigger pressed past half its travel.
        auto pressed=[&](int pad) {
            if(pad==padLeftTrigger)return SDL_GetGamepadAxis(gamepad_,SDL_GAMEPAD_AXIS_LEFT_TRIGGER)>triggerThreshold;
            if(pad==padRightTrigger)return SDL_GetGamepadAxis(gamepad_,SDL_GAMEPAD_AXIS_RIGHT_TRIGGER)>triggerThreshold;
            return SDL_GetGamepadButton(gamepad_,static_cast<SDL_GamepadButton>(pad));
        };
        for(int i=0;i<buttonControlCount;++i)if(pressed(controls.pads[i]))buttons|=1u<<controlBits[i];
        if(pressed(controls.pads[controlPartyChat]))partyChat=true;
        // The left stick moves too.
        const int x=SDL_GetGamepadAxis(gamepad_,SDL_GAMEPAD_AXIS_LEFTX),y=SDL_GetGamepadAxis(gamepad_,SDL_GAMEPAD_AXIS_LEFTY);
        if(x>16000)buttons|=16;if(x<-16000)buttons|=32;if(y<-16000)buttons|=64;if(y>16000)buttons|=128;
        // The right stick, up and down, zooms the camera.
        const int zoom=SDL_GetGamepadAxis(gamepad_,SDL_GAMEPAD_AXIS_RIGHTY);
        if(zoom<-16000||zoom>16000) {zoomCamera(zoom<0?1:-1);stickZoom_=true;}
        else if(stickZoom_) {stickZoom_=false;if(settingsUsed_)saveSettings();}
        // Left and right, it turns the camera as R and L do (in towns, while held).
        const int turn=SDL_GetGamepadAxis(gamepad_,SDL_GAMEPAD_AXIS_RIGHTX);
        if(turn>16000)buttons|=256;if(turn<-16000)buttons|=512;
    }
    // While a control is being bound, and until the key that bound it is
    // released, the game gets no input.
    if(rebindWaiting()) {buttons=0;partyChat=false;}
    else if(suppressInput_) {if(buttons)buttons=0;else suppressInput_=false;partyChat=false;}
    setPartyChatRequest(partyChat&&!partyChatHeld_);partyChatHeld_=partyChat;
    // Touch: the bottom DS screen, centred at the foot of the canvas.
    float mouseX=0,mouseY=0;const auto mouse=SDL_GetMouseState(&mouseX,&mouseY);
    int width=0,height=0;require(SDL_GetWindowSize(window_,&width,&height));
    float canvasWidth=float(width),canvasHeight=float(height);
    if(!stretch_) {
        const float scale=std::min(width/layout_.canvasWidth,height/layout_.canvasHeight);
        canvasWidth=layout_.canvasWidth*scale;canvasHeight=layout_.canvasHeight*scale;
    }
    float x=(mouseX-(width-canvasWidth)/2)/std::max(canvasWidth,1.f)*layout_.canvasWidth-(layout_.canvasWidth-256)/2;
    float y=(mouseY-(height-canvasHeight)/2)/std::max(canvasHeight,1.f)*layout_.canvasHeight;
    if(layout_.placed) {
        // The bottom screen's own rectangle.
        const auto& r=layout_.screens[1];
        const float cx=x+(layout_.canvasWidth-256)/2;
        x=r[2]>0?(cx-r[0])/r[2]*256:-1;y=r[3]>0?(y-r[1])/r[3]*192+192:-1;
    }
    const bool down=focus&&(mouse&SDL_BUTTON_LMASK)&&x>=0&&x<256&&y>=192&&y<384;
    setTouch(down,static_cast<int>(x),static_cast<int>(y)-192);
    setButtons(buttons);return running_;
}
void GameWindow::drawInterface(int width,int height) {
    const float scale=std::max(0.6f,height/720.f);
    if(gameSettings().showFps&&settingsUsed_) {
        const bool rebuild=fpsChanged_||!fpsPicture_||fpsPicture_->height()!=int(std::lround(30*scale));
        if(rebuild) {
            fpsPicture_=std::make_unique<TextCanvas>(int(std::lround(110*scale)),int(std::lround(30*scale)));
            fpsPicture_->clear(0xa0000000);
            fpsPicture_->text(0,int(std::lround(4*scale)),std::to_string(fpsShown_)+" FPS",int(std::lround(18*scale)),
                fpsShown_>=58?0xff8ef08e:fpsShown_>=45?0xfff0d070:0xfff07a6a,true,1,fpsPicture_->width());
            fpsChanged_=false;
        }
        gpu_->drawOverlay(overlayFps,fpsPicture_->pixels(),fpsPicture_->width(),fpsPicture_->height(),rebuild,
            int(std::lround(10*scale)),int(std::lround(10*scale)),width,height);
    }
}
void GameWindow::present(const GpuLayout& layout,std::span<const std::uint32_t> recipes) {
    layout_=layout;recipes_=recipes;
    int width=0,height=0;drawableSize(width,height);
    {ProfileScope timing("present-compose");gpu_->present(layout,recipes,width,height,stretch_);}
    if(screenshot_) {
        // F12: PNG beside the save file.
        screenshot_=false;
        int w=0,h=0;auto pixels=gpu_->readPresented(w,h);
        if(auto* surface=SDL_CreateSurfaceFrom(w,h,SDL_PIXELFORMAT_ARGB8888,pixels.data(),w*4)) {
            if(char* base=SDL_GetPrefPath(nullptr,"DQIV-PC")) {
                const auto path=std::string(base)+"capture-"+std::to_string(SDL_GetTicks())+".png";
                SDL_SavePNG(surface,path.c_str());SDL_free(base);
            }
            SDL_DestroySurface(surface);
        }
    }
    // Frames per second, counted over each second.
    ++fpsFrames_;
    const auto now=SDL_GetTicksNS();
    if(now-fpsStart_>=1'000'000'000) {
        const unsigned shown=unsigned(std::lround(fpsFrames_*1e9/double(now-fpsStart_)));
        if(shown!=fpsShown_) {fpsShown_=shown;fpsChanged_=true;}
        fpsFrames_=0;fpsStart_=now;
    }
    drawInterface(width,height);
    ProfileScope timing("present-swap");
    require(SDL_GL_SwapWindow(window_));
}
void GameWindow::audio(std::span<const std::int16_t> samples) {
    if(!audio_)return;
    const int volume=settingsUsed_?gameSettings().volume:100;
    if(volume>=100) {audio_->write(samples);return;}
    std::vector<std::int16_t> scaled(samples.begin(),samples.end());
    for(auto& s:scaled)s=static_cast<std::int16_t>(s*volume/100);
    audio_->write(scaled);
}
int GameWindow::frameRateTarget() const {return frameRates[std::clamp(gameSettings().frameRate,0,frameRateCount-1)];}
void GameWindow::measureBetween(std::uint64_t cost) {
    betweenCost_=std::clamp<std::uint64_t>((betweenCost_*7+cost)/8,500'000,8'000'000);
}
bool GameWindow::interpolating() const {
    if(hidden_||!interpolator_||frameRateTarget()==60)return false;
    // With vertical sync, the screen's own rate bounds the frames shown.
    if(!gameSettings().vsync)return true;
    const auto* mode=SDL_GetCurrentDisplayMode(SDL_GetDisplayForWindow(window_));
    return mode&&mode->refresh_rate>61;
}
void GameWindow::pace() {
    if(hidden_)return; // diagnostics run as fast as possible
    constexpr std::uint64_t period=1'000'000'000/60;
    const auto start=deadline_;
    deadline_+=period;
    if(interpolating()) {
        // Frames between the game's, as often as the chosen rate allows and
        // as long as one more fits before the next game frame.
        const int target=frameRateTarget();
        const std::uint64_t step=target?1'000'000'000ull/std::uint64_t(target):0;
        // The frames keep one cadence across the game's frames: the time left
        // after a game frame is used from where the cadence stands, not from a
        // fresh start (which would fit only two frames of 144 in each 60th).
        const auto keepCadence=[&](std::uint64_t now) {nextShown_=now-nextShown_<step?nextShown_+step:now+step;};
        bool shown=false;
        for(;;) {
            const auto now=SDL_GetTicksNS();
            if(now+betweenCost_>=deadline_)break;
            // One that would end after the game's next frame starts comes
            // early instead (up to half a step), as long as it fits now.
            if(now<nextShown_&&(nextShown_+betweenCost_<deadline_||nextShown_-now>step/2)) {
                if(nextShown_+betweenCost_>=deadline_)break;
                SDL_DelayPrecise(nextShown_-now);continue;
            }
            const float alpha=std::clamp(float(double(std::int64_t(now-start))/double(period)),0.f,1.f);
            if(!interpolator_(alpha))break;
            measureBetween(SDL_GetTicksNS()-now);
            shown=true;keepCadence(now);
        }
        if(!shown) {
            const auto now=SDL_GetTicksNS();
            if(interpolator_(1.f))measureBetween(SDL_GetTicksNS()-now),keepCadence(now);
            // An estimate made by a slow frame (a map loading) comes back down.
            betweenCost_=betweenCost_*3/4;
        }
    }
    const auto now=SDL_GetTicksNS();
    ProfileScope timing("pace-wait");
    if(deadline_>now)SDL_DelayPrecise(deadline_-now);
    else if(now-deadline_>100'000'000)deadline_=now;
}
}
