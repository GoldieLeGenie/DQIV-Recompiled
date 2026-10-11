#pragma once
#include <array>
#include <cstdint>
#include <functional>
#include <string>
namespace pc {
void setButtons(std::uint16_t buttons);
void setFolded(bool folded);
void setTouch(bool down,int x,int y);
void setWaitCallback(std::function<void()> callback);
void setFrameCallback(std::function<void()> callback);
void completeNativeFrame();
const std::array<std::string, 24>& debugConsole();
}
