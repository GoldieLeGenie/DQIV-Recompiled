#pragma ipa file
#include "main/debug/DebugInfo.hpp"
#include "main/dss/Display.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/cmn/WorldLocation.hpp"
#include "main/data/DataObject.hpp"
#include "main/dss/Camera.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/VramManager.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/status/GameStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "ov000/town/TownPlayerManager.hpp"

static int s_frame;
UnkDebugInfo g_DebugInfo;

THUMB void UnkDebugInfo::initialize()
{
    lastX_ = 0xfe02;
    lastY_ = 0xfe02;
    lastZ_ = 0xfe02;
    lastDir_ = 0xfe02;
    clearMessageIDs();
}

THUMB void UnkDebugInfo::draw()
{
    char* name = g_Stage.getMapName();
    if (name != NULL) {
        debugPrint(13, 0, name);
    }
    if (showArrayWarning_) {
        drawArrayWarnings(1, 2);
        return;
    }
    if (showPosition_) {
        drawPosition(1, 21);
    }
    if (showMessageID_) {
        drawMessageIDs(25, 2);
    }
    if (showFlag_) {
        if (showFlag_) {
            drawFlags(0, 0);
        }
    } else {
        if (showWalk_) {
            drawWalkCounter(1, 16);
        }
        if (showWorldTime_) {
            drawWorldTime(1, 1);
        }
        if (showPlayTime_) {
            drawPlayTime(12, 2);
        }
        if (showHeap_) {
            drawHeap(1, 7);
        }
    }
}

THUMB void UnkDebugInfo::drawArrayWarnings(int x, int y)
{
    int count = getArrayErrorCount();
    debugPrint(x + 8, y, "ARRAY WARNING!");
    for (int i = 0; i < 16; i++) {
        if (i < count) {
            UnkArrayWarning* warning = getArrayError(i);
            debugConsolePrintf(x, y + i + 1, "ARRAY OVER %3d/%3d %08x", warning->index_, warning->size_, warning->caller_);
        } else {
            debugPrint(x, y + i + 1, "----");
        }
    }
    drawDebugWindow(x, y, 30, 17);
    s_frame++;
}

THUMB void UnkDebugInfo::drawPosition(int x, int y)
{
    dss::Fix32Vector3 pos;
    short dir;
    g_cmnPartyInfo.getPartyInfo(&pos, &dir);
    if (g_GlobalDQ4.unkfunc_0205810c() == 12) {
        if (lastX_ != pos.vx.value) {
            lastX_ = pos.vx.value;
            dss::sprintf_s(xText_, 0x20, "X:%10f", pos.vx.getfloat());
        }
        if (lastY_ != pos.vy.value) {
            lastY_ = pos.vy.value;
            dss::sprintf_s(yText_, 0x20, "Y:%10f", pos.vy.getfloat());
        }
        if (lastZ_ != pos.vz.value) {
            lastZ_ = pos.vz.value;
            dss::sprintf_s(zText_, 0x20, "Z:%10f", pos.vz.getfloat());
        }
        if (lastDir_ != dir) {
            lastDir_ = dir;
            dss::sprintf_s(dirText_, 0x20, "D:%6d %04x", dir, (unsigned short)dir);
        }
    } else {
        if (lastX_ != pos.vx.value) {
            lastX_ = pos.vx.value;
            dss::sprintf_s(xText_, 0x20, "X:%-7.2f", pos.vx.getfloat());
        }
        if (lastY_ != pos.vy.value) {
            lastY_ = pos.vy.value;
            dss::sprintf_s(yText_, 0x20, "Y:%-7.2f", pos.vy.getfloat());
        }
        if (lastZ_ != pos.vz.value) {
            lastZ_ = pos.vz.value;
            dss::sprintf_s(zText_, 0x20, "Z:%-7.2f", pos.vz.getfloat());
        }
        if (lastDir_ != dir) {
            lastDir_ = dir;
            dss::sprintf_s(dirText_, 0x20, "D:%6d %04x", dir, (unsigned short)dir);
        }
    }
    debugPrint(x, y, xText_);
    debugPrint(x + 13, y, yText_);
    debugPrint(x, y + 1, zText_);
    debugPrint(x + 13, y + 1, dirText_);
    drawDebugWindow(x, y, 26, 2);
}

THUMB void UnkDebugInfo::clearMessageIDs()
{
    for (int i = 0; i < 16; i++) {
        messageID_[i] = -1;
    }
}

THUMB void UnkDebugInfo::addMessageID(int messageID)
{
    for (int i = 0; i < 16; i++) {
        if (messageID_[i] == -1) {
            messageID_[i] = messageID;
            return;
        }
    }
}

THUMB void UnkDebugInfo::drawMessageIDs(int x, int y)
{
    debugPrint(x, y, "MessID");
    for (int i = 0; i < 16; i++) {
        if (messageID_[i] != -1) {
            debugConsolePrintf(x, y + i + 1, "%6d", messageID_[i]);
        } else {
            debugPrint(x, y + i + 1, "------");
        }
    }
    drawDebugWindow(x, y, 6, 17);
}

THUMB void UnkDebugInfo::drawFlags(int x, int y)
{
    drawFlagTable(x, y, 'G', &g_AreaFlag, areaFlagIndex_, 5, &areaFlag_);
    drawFlagTable(x, y + 7, 'A', &g_LocalFlag, localFlagIndex_, 5, &localFlag_);
    drawFlagTable(x, y + 14, 'L', &g_GlobalFlag, globalFlagIndex_, 5, &globalFlag_);
    drawDebugBox(0, 0, 24, 6);
    drawDebugBox(0, 7, 24, 6);
    drawDebugBox(0, 14, 24, 6);
}

THUMB void UnkDebugInfo::drawFlagTable(int x, int y, char name, status::GameFlag* flag, int index, int lines, status::GameFlag* prev)
{
    static char line[] = "+--- - - - - - - - - - -";
    static char header[] = "-flg 0 1 2 3 4 5 6 7 8 9";
    int blink;
    if (getDisplayFrame() & 2) {
        blink = 1;
    } else {
        blink = 0;
    }
    header[0] = name;
    debugPrint(x, y, header);
    for (int i = 0; i < lines; i++) {
        int row = index + i * 10;
        line[1] = '0' + row / 100;
        int n = row % 100;
        line[2] = '0' + n / 10;
        n = n % 10;
        line[3] = '0' + n;
        char* p;
        int k;
        k = row;
        p = &line[5];
        for (int j = 0; j < 10; j++) {
            bool cur = flag->check(k);
            bool old = prev->check(k);
            *p = '0' + cur;
            if (cur != old && blink) {
                *p = ' ';
            }
            p += 2;
            k++;
        }
        debugPrint(x, y + 1 + i, line);
    }
}

THUMB void UnkDebugInfo::drawWorldTime(int x, int y)
{
    const char* zone = "";
    switch (cmn::WorldLocation::getCurrentTimeZone()) {
    case TIME_ZONE_NONE:
        break;
    case TIME_ZONE_MORNING:
        zone = "MORNING";
        break;
    case TIME_ZONE_DAYTIME:
        zone = "DAYTIME";
        break;
    case TIME_ZONE_EVENING:
        zone = "EVENING";
        break;
    case TIME_ZONE_NIGHT:
        zone = "  NIGHT";
        break;
    }
    debugConsolePrintf(x, y, "WORLDTIME");
    debugConsolePrintf(x, y + 1, "%4d/%4d", g_Stage.getWorldTime(), 3392);
    debugConsolePrintf(x + 2, y + 2, zone);
    drawDebugWindow(x, y, 9, 3);
}

THUMB void UnkDebugInfo::drawWalkCounter(int x, int y)
{
    if (g_GlobalDQ4.unkfunc_0205810c() == 12) {
        debugConsolePrintf(x, y, "WALK");
        TownPlayerManager* manager = TownPlayerManager::getSingleton();
        debugConsolePrintf(x, y + 1, "%4d", manager->walkCounter_ >> 4);
        drawDebugWindow(x, y, 4, 2);
    } else if (g_GlobalDQ4.unkfunc_0205810c() == 14) {
    }
}

THUMB void UnkDebugInfo::drawPlayTime(int x, int y)
{
    unsigned int time = status::g_Game.getPlayTime();
    debugConsolePrintf(x, y, "PLAY TIME");
    unsigned int hour = time % 216000;
    unsigned int minute = hour % 3600;
    debugConsolePrintf(x, y + 1, "%03d:%02d.%02d%02d", time / 216000, hour / 3600, minute / 60, minute % 60 * 100 / 60);
    drawDebugWindow(x, y, 11, 2);
}

THUMB void UnkDebugInfo::drawHeap(int x, int y)
{
    debugConsolePrintf(x, y, "HEAP");
    debugConsolePrintf(x, y + 1, "%3dK", getAppHeapFreeSize(&g_appHeap) >> 10);
    debugConsolePrintf(x, y + 2, "VRAM");
    debugConsolePrintf(x, y + 3, "%3dk", g_VramTransfer.unk_08 >> 10);
    drawDebugWindow(x, y, 5, 4);
}

THUMB void UnkDebugInfo::setShowArrayWarning(int show)
{
    showArrayWarning_ = show;
}
