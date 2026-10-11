#include "Settings.hpp"
#include <SDL3/SDL.h>
#include <string>

namespace pc {
namespace {
// Gamepad buttons by their position (south, east, ...), as an Xbox pad labels them.
const char* padButtonName(int button,int language) {
    const bool fr=language==2;
    switch(button) {
    case SDL_GAMEPAD_BUTTON_SOUTH:return "A";
    case SDL_GAMEPAD_BUTTON_EAST:return "B";
    case SDL_GAMEPAD_BUTTON_WEST:return "X";
    case SDL_GAMEPAD_BUTTON_NORTH:return "Y";
    case SDL_GAMEPAD_BUTTON_BACK:return "Back";
    case SDL_GAMEPAD_BUTTON_GUIDE:return "Guide";
    case SDL_GAMEPAD_BUTTON_START:return "Start";
    case SDL_GAMEPAD_BUTTON_LEFT_STICK:return fr?"Stick G.":"LS";
    case SDL_GAMEPAD_BUTTON_RIGHT_STICK:return fr?"Stick D.":"RS";
    case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER:return "LB";
    case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER:return "RB";
    case SDL_GAMEPAD_BUTTON_DPAD_UP:return fr?"Croix haut":"D-pad up";
    case SDL_GAMEPAD_BUTTON_DPAD_DOWN:return fr?"Croix bas":"D-pad down";
    case SDL_GAMEPAD_BUTTON_DPAD_LEFT:return fr?"Croix gauche":"D-pad left";
    case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:return fr?"Croix droite":"D-pad right";
    case SDL_GAMEPAD_BUTTON_MISC1:return "Share";
    }
    return "?";
}
}
std::string bindingName(int page,int action,int language) {
    if(action<0||action>=controlCount)return {};
    const auto& s=gameSettings();
    if(page==pageGamepad)return padButtonName(s.pads[action],language);
    const char* name=SDL_GetKeyName(static_cast<SDL_Keycode>(s.keys[action]));
    if(!name||!*name)return "?";
    // The game's font has no arrows: plain words for the arrow keys.
    const std::string text=name;
    const bool fr=language==2;
    if(text=="Up")return fr?"Haut":"Up";
    if(text=="Down")return fr?"Bas":"Down";
    if(text=="Left")return fr?"Gauche":"Left";
    if(text=="Right")return fr?"Droite":"Right";
    if(text=="Return")return fr?"Entr\xc3\xa9""e":"Enter";
    if(text=="Space")return fr?"Espace":"Space";
    if(text=="Right Shift")return fr?"Maj droite":"Right Shift";
    if(text=="Left Shift")return fr?"Maj gauche":"Left Shift";
    return text;
}
}
