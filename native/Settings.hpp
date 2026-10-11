#pragma once
#include <string>
// Player settings of the PC version, kept in saves\settings.ini beside the game.
namespace pc {
// How fast dialogue text appears.
enum TextSpeed {textSlow,textNormal,textFast,textInstant,textSpeedCount};
struct GameSettings {
    int language=2;          // game language: 0 Japanese, 1 English, 2 French, 3 German, 4 Italian, 5 Spanish
    bool showFps=false;
    bool fullscreen=false;
    int windowSize=1;        // index into windowSizes
    bool wide=true;          // 16:9 picture, or the DS screens' width
    bool stretch=false;
    bool vsync=true;
    int quality=0;           // 0 automatic, otherwise the supersampling factor (1..4)
    bool smoothTextures=true;
    int volume=100;          // percent
    int textSpeed=textNormal; // TextSpeed
    int frameRate=0;          // index into frameRates
    int battleBoost=0;        // battles run 1, 2, 3 or 4 times as fast
    // Camera of towns, dungeons and the world map.
    int cameraZoom=100;       // percent: above 100 closer, below farther
    bool mobileSound=true;    // the smartphone version's music and sounds instead of the DS sequences
    // Controls, in the order of controlBits then party chat: an SDL keycode
    // and an SDL gamepad button.
    unsigned keys[13]={'z','x','s','a','q','w',0x0d,0x400000e5,0x40000052,0x40000051,0x40000050,0x4000004f,'e'};
    // Party chat on B: the game only takes it outside menus (B does nothing there).
    int pads[13]={0,1,3,2,9,10,6,4,11,12,13,14,1};
};
// The DS buttons the controls set, in the order the settings list them:
// A, B, X, Y, L, R, Start, Select, Up, Down, Left, Right (bits of the DS pad).
inline constexpr unsigned controlBits[12]={0,1,10,11,9,8,3,2,6,7,5,4};
inline constexpr int buttonControlCount=12;
// After the DS buttons: talk to the party at once.
inline constexpr int controlPartyChat=12;
inline constexpr int controlCount=13;
// Whether the party chat control was pressed since the window last read the
// input (the game takes it once).
void setPartyChatRequest(bool pressed);
bool takePartyChatRequest();
}
// The game's frame count (with the power functions).
extern "C" unsigned pcVBlankCount();
namespace pc {
// Pages of the settings: game, display, keyboard, gamepad.
enum SettingsPage {pageGame,pageDisplay,pageKeyboard,pageGamepad,pageCount};
const char* pageTitle(int page,int language);
const char* controlName(int action,int language);
// The key or gamepad button an action uses (defined with the window's input).
std::string bindingName(int page,int action,int language);
// Waits for the next key (keyboard page) or gamepad button (gamepad page)
// to bind it to `action`; the window completes it.
void startRebind(int page,int action);
bool rebindWaiting();
void cancelRebind();
int rebindPage();
int rebindAction();
const char* rebindPrompt(int language);
const char* pageHint(int language);
// Restores the default controls of a page.
void resetControls(int page);
const char* resetLabel(int language);
inline constexpr int windowSizes[][2]={{1280,720},{1600,900},{1920,1080},{2560,1440}};
// Frames per second shown (0: unlimited); above 60, frames between the game's are interpolated.
inline constexpr int frameRates[]={60,120,144,165,240,0};
inline constexpr int frameRateCount=6;
inline constexpr int windowSizeCount=4;
GameSettings& gameSettings();
// Reads the file when present (missing keys keep their defaults).
void loadSettings();
void saveSettings();

// The settings, as the game's menus list them.
enum SettingItem {settingLanguage,settingFps,settingFullscreen,settingWindowSize,settingPicture,settingStretch,
    settingVsync,settingQuality,settingTextures,settingVolume,settingTextSpeed,settingFrameRate,settingBattleBoost,
    settingCameraZoom,settingMobileSound,settingCount};
inline constexpr int cameraZoomMin=50,cameraZoomMax=200;
// Changes the camera zoom by `step` (percent); for the mouse wheel and the right stick.
void zoomCamera(int step);
// Texts in a game language (1..5, as GameSettings::language).
const char* settingLabel(int item,int language);
std::string settingValue(int item,int language);
const char* settingsTitle(int language); // the entry in the adventure log menu
const char* settingsBack(int language);
const char* settingHint(int item,int language);
// Changes a setting by `step` (-1 or +1); false when nothing changed.
bool changeSetting(int item,int step);
// The game's own options (kept in its save), as the settings show them.
const char* battleSpeedLabel(int language);
const char* battleSpeedValue(int speed,int language); // 0 fastest .. 4 waits for a key
const char* musicLabel(int language);
const char* soundsLabel(int language);
// Settings changed outside the window (the game's menu) wait to be applied by it.
void requestSettingsApply();
bool takeSettingsApply();
}
