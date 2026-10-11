#include <stdarg.h>
#include "main/debug/UnkDebugDisplay.hpp"
#include "main/debug/UnkDebugInfo.hpp"
#include "main/debug/UnkDebugBattleInfo.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/menu/UnkMenuBg.hpp"
#include "main/global/GlobalDQ4.hpp"
#include <stdio.h>

static int s_messageTime;
static int s_arrayWarningCount;
static UnkDebugDisplay* s_display[3];
static char s_message[0x24];

THUMB void unkfunc_0202c17c(int x, int y, const char* fmt, ...)
{
    char buf[0x80];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, 0x80, fmt, args);
    data_02116ce0.unkfunc_0207e864(x, y, buf);
}

THUMB void unkfunc_0202c1b4(int x, int y, const char* str)
{
    if (!MenuManager::isRequesting()) {
        data_02116ce0.unkfunc_0207e864(x, y, str);
    }
}

THUMB void unkfunc_0202c1d8(int x, int y, int w, int h)
{
    if (!MenuManager::isRequesting()) {
        unkfunc_02080694(data_020facb8.back_.unkfunc_020504d4(), x, y, w, h, 0xf);
    }
}

THUMB void unkfunc_0202c20c(int x, int y, int w, int h)
{
    if (!MenuManager::isRequesting()) {
        unkfunc_020808cc(data_020facb8.window_.unkfunc_0204fe58(), x - 1, y - 1, w + 2, h + 2, 0xf);
        unkfunc_020806dc(data_020facb8.back_.unkfunc_020504d4(), x - 1, y - 1, w + 2, h + 2, 0xf);
    }
}

THUMB void unkfunc_0202c25c()
{
    UnkDebugDisplay* display = s_display[unkfunc_0202c350()];
    if (display != NULL) {
        display->initialize();
    }
    data_02116ce0.unkfunc_0207e810();
}

THUMB void unkfunc_0202c284()
{
    UnkDebugDisplay* display = s_display[unkfunc_0202c350()];
    if (display != NULL) {
        display->draw();
    }
    data_02121094.unkfunc_02089b34();
    if (s_arrayWarningCount != unkfunc_020895fc()) {
        s_arrayWarningCount = unkfunc_020895fc();
        unkfunc_0202c300(1);
        data_020f1d88.unkfunc_0203d00c(1);
    }
    if (s_messageTime != 0) {
        if (s_messageTime & 1) {
            data_02116ce0.unkfunc_0207e864(0, 0x18, s_message);
        }
        s_messageTime--;
    }
}

THUMB void unkfunc_0202c300(int type)
{
    int index = unkfunc_0202c350();
    s_display[index] = unkfunc_0202c330(type);
    if (s_display[index] != NULL) {
        s_display[index]->initialize();
    }
    data_02116ce0.unkfunc_0207e810();
}

THUMB UnkDebugDisplay* unkfunc_0202c330(int type)
{
    switch (type) {
    case 1:
        return &data_020f1d88;
    case 2:
        return &data_020f2008;
    }
    return NULL;
}

THUMB int unkfunc_0202c350()
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

