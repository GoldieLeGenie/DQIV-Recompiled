#include "Input.hpp"
#include "main/dss/DssCore.hpp"
#include "main/dss/UnkMemory.hpp"
#include "nitro/pad.h"
#include <algorithm>
#include <chrono>
#include <cstdarg>
#include <cstdio>
#include <thread>

namespace {
std::uint16_t buttons = 0;
bool folded = false;
std::function<void()>& waitCallback() { static std::function<void()> value; return value; }
std::function<void()>& frameCallback() { static std::function<void()> value; return value; }
std::array<std::string, 24>& console() { static std::array<std::string, 24> value; return value; }
}
namespace pc {
void setButtons(std::uint16_t value) { buttons = value & PAD_ALL; }
void setFolded(bool value) { folded = value; }
void setTouch(bool down,int x,int y) {
    g_TouchPanel.touch_=down&&x>=0&&x<256&&y>=0&&y<192;
    if(g_TouchPanel.touch_) {g_TouchPanel.x_=x;g_TouchPanel.y_=y;}
}
void setWaitCallback(std::function<void()> value) { waitCallback() = std::move(value); }
void setFrameCallback(std::function<void()> value) { frameCallback() = std::move(value); }
void completeNativeFrame() {if(frameCallback())frameCallback()();}
const std::array<std::string, 24>& debugConsole() { return console(); }
}
extern "C" u16 PAD_Read() { return buttons; }
extern "C" BOOL PAD_DetectFold() { return folded ? 1 : 0; }
extern "C" void OS_Wait() {
    if (waitCallback()) waitCallback()();
    else std::this_thread::sleep_for(std::chrono::milliseconds(16));
}
UnkDebugConsole g_DebugConsole;
void UnkDebugConsole::unkfunc_0207e800() { for (auto& row : console()) row.assign(32, ' '); }
void UnkDebugConsole::unkfunc_0207e804() { unkfunc_0207e800(); }
void UnkDebugConsole::unkfunc_0207e810() { unkfunc_0207e800(); }
void UnkDebugConsole::unkfunc_0207e824(int x, int y, int w, int h) {
    for (int row = std::max(y, 0); row < std::min(y + h, 24); ++row)
        for (int col = std::max(x, 0); col < std::min(x + w, 32); ++col) console()[row][col] = ' ';
}
void UnkDebugConsole::unkfunc_0207e864(int x, int y, const char* text) {
    if (y < 0 || y >= 24) return;
    for (; *text && x < 32; ++text, ++x) if (x >= 0) console()[y][x] = *text;
}
void UnkDebugConsole::unkfunc_0207e88c(int x, int y, const char* format, ...) {
    char buffer[65]; va_list args; va_start(args, format);
    std::vsnprintf(buffer, sizeof(buffer), format, args); va_end(args);
    unkfunc_0207e864(x, y, buffer);
}
void UnkDebugConsole::unkfunc_0207e8e0(int x, int y, int w, int h) { unkfunc_0207e824(x, y, w, h); }
void UnkDebugConsole::unkfunc_0207e8f4(int x, int y) { unkfunc_0207e864(x, y, ">"); }
