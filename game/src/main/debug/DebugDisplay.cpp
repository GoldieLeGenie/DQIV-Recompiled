#include <stdarg.h>
#include "main/debug/DebugDisplay.hpp"
#include "main/debug/DebugInfo.hpp"
#include "main/debug/DebugBattleInfo.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/menu/MenuBg.hpp"
#include "main/global/GlobalDQ4.hpp"
#include <stdio.h>

static int s_messageTime;
static int s_arrayWarningCount;
static UnkDebugDisplay* s_display[3];
static char s_message[0x24];

THUMB void debugConsolePrintf(int x, int y, const char* fmt, ...)
{
    char buf[0x80];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, 0x80, fmt, args);
    data_02116ce0.unkfunc_0207e864(x, y, buf);
}

THUMB void debugPrint(int x, int y, const char* str)
{
    if (!MenuManager::isRequesting()) {
        data_02116ce0.unkfunc_0207e864(x, y, str);
    }
}

THUMB void drawDebugBox(int x, int y, int w, int h)
{
    if (!MenuManager::isRequesting()) {
        fillWindowBackRect(data_020facb8.back_.getScreenBuffer(), x, y, w, h, 0xf);
    }
}

THUMB void drawDebugWindow(int x, int y, int w, int h)
{
    if (!MenuManager::isRequesting()) {
        drawWindowBorder(data_020facb8.window_.getScreenBuffer(), x - 1, y - 1, w + 2, h + 2, 0xf);
        drawWindowBack(data_020facb8.back_.getScreenBuffer(), x - 1, y - 1, w + 2, h + 2, 0xf);
    }
}

THUMB void initDebugDisplay()
{
    UnkDebugDisplay* display = s_display[getDebugDisplaySlot()];
    if (display != NULL) {
        display->initialize();
    }
    data_02116ce0.unkfunc_0207e810();
}

THUMB void updateDebugDisplay()
{
    UnkDebugDisplay* display = s_display[getDebugDisplaySlot()];
    if (display != NULL) {
        display->draw();
    }
    data_02121094.update();
    if (s_arrayWarningCount != getArrayErrorCount()) {
        s_arrayWarningCount = getArrayErrorCount();
        setDebugDisplayPage(1);
        data_020f1d88.setShowArrayWarning(1);
    }
    if (s_messageTime != 0) {
        if (s_messageTime & 1) {
            data_02116ce0.unkfunc_0207e864(0, 0x18, s_message);
        }
        s_messageTime--;
    }
}

THUMB void setDebugDisplayPage(int type)
{
    int index = getDebugDisplaySlot();
    s_display[index] = getDebugDisplayPage(type);
    if (s_display[index] != NULL) {
        s_display[index]->initialize();
    }
    data_02116ce0.unkfunc_0207e810();
}

THUMB UnkDebugDisplay* getDebugDisplayPage(int type)
{
    switch (type) {
    case 1:
        return &data_020f1d88;
    case 2:
        return &data_020f2008;
    }
    return NULL;
}

THUMB int getDebugDisplaySlot()
{
    switch (data_0210bb94.unkfunc_0205810c()) {
    case 12:
        return 2;
    case 14:
        return 2;
    case 13:
        return 1;
    }
    return 0;
}

