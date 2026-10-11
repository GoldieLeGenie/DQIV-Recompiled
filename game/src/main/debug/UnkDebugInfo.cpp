#pragma ipa file
#include "main/debug/UnkDebugInfo.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/cmn/WorldLocation.hpp"
#include "main/data/DataObject.hpp"
#include "main/dss/Camera.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/UnkVramTransfer.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/status/GameStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "ov000/town/TownPlayerManager.hpp"

static int s_frame;
UnkDebugInfo data_020f1d88;

THUMB void UnkDebugInfo::initialize()
{
    lastX_ = 0xfe02;
    lastY_ = 0xfe02;
    lastZ_ = 0xfe02;
    lastDir_ = 0xfe02;
    unkfunc_0203cc0c();
}

THUMB void UnkDebugInfo::draw()
{
    char* name = g_Stage.getMapName();
    if (name != NULL) {
        unkfunc_0202c1b4(13, 0, name);
    }
    if (showArrayWarning_) {
        unkfunc_0203c9b0(1, 2);
        return;
    }
    if (showPosition_) {
        unkfunc_0203ca2c(1, 21);
    }
    if (showMessageID_) {
        unkfunc_0203cc4c(25, 2);
    }
    if (showFlag_) {
        if (showFlag_) {
            unkfunc_0203ccac(0, 0);
        }
    } else {
        if (showWalk_) {
            unkfunc_0203cebc(1, 16);
        }
        if (showWorldTime_) {
            unkfunc_0203ce28(1, 1);
        }
        if (showPlayTime_) {
            unkfunc_0203cf14(12, 2);
        }
        if (showHeap_) {
            unkfunc_0203cfa8(1, 7);
        }
    }
}

THUMB void UnkDebugInfo::unkfunc_0203c9b0(int x, int y)
{
    int count = unkfunc_020895fc();
    unkfunc_0202c1b4(x + 8, y, "ARRAY WARNING!");
    for (int i = 0; i < 16; i++) {
        if (i < count) {
            UnkArrayWarning* warning = unkfunc_020895e8(i);
            unkfunc_0202c17c(x, y + i + 1, "ARRAY OVER %3d/%3d %08x", warning->index_, warning->size_, warning->caller_);
        } else {
            unkfunc_0202c1b4(x, y + i + 1, "----");
        }
    }
    unkfunc_0202c20c(x, y, 30, 17);
    s_frame++;
}

THUMB void UnkDebugInfo::unkfunc_0203ca2c(int x, int y)
{
    dss::Fix32Vector3 pos;
    short dir;
    g_cmnPartyInfo.getPartyInfo(&pos, &dir);
    if (data_0210bb94.unkfunc_0205810c() == 12) {
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
    unkfunc_0202c1b4(x, y, xText_);
    unkfunc_0202c1b4(x + 13, y, yText_);
    unkfunc_0202c1b4(x, y + 1, zText_);
    unkfunc_0202c1b4(x + 13, y + 1, dirText_);
    unkfunc_0202c20c(x, y, 26, 2);
}

THUMB void UnkDebugInfo::unkfunc_0203cc0c()
{
    for (int i = 0; i < 16; i++) {
        messageID_[i] = -1;
    }
}

THUMB void UnkDebugInfo::unkfunc_0203cc20(int messageID)
{
    for (int i = 0; i < 16; i++) {
        if (messageID_[i] == -1) {
            messageID_[i] = messageID;
            return;
        }
    }
}

THUMB void UnkDebugInfo::unkfunc_0203cc4c(int x, int y)
{
    unkfunc_0202c1b4(x, y, "MessID");
    for (int i = 0; i < 16; i++) {
        if (messageID_[i] != -1) {
            unkfunc_0202c17c(x, y + i + 1, "%6d", messageID_[i]);
        } else {
            unkfunc_0202c1b4(x, y + i + 1, "------");
        }
    }
    unkfunc_0202c20c(x, y, 6, 17);
}

THUMB void UnkDebugInfo::unkfunc_0203ccac(int x, int y)
{
    unkfunc_0203cd44(x, y, 'G', &g_AreaFlag, areaFlagIndex_, 5, &areaFlag_);
    unkfunc_0203cd44(x, y + 7, 'A', &g_LocalFlag, localFlagIndex_, 5, &localFlag_);
    unkfunc_0203cd44(x, y + 14, 'L', &g_GlobalFlag, globalFlagIndex_, 5, &globalFlag_);
    unkfunc_0202c1d8(0, 0, 24, 6);
    unkfunc_0202c1d8(0, 7, 24, 6);
    unkfunc_0202c1d8(0, 14, 24, 6);
}

THUMB void UnkDebugInfo::unkfunc_0203cd44(int x, int y, char name, status::GameFlag* flag, int index, int lines, status::GameFlag* prev)
{
    static char line[] = "+--- - - - - - - - - - -";
    static char header[] = "-flg 0 1 2 3 4 5 6 7 8 9";
    int blink;
    if (unkfunc_02081254() & 2) {
        blink = 1;
    } else {
        blink = 0;
    }
    header[0] = name;
    unkfunc_0202c1b4(x, y, header);
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
        unkfunc_0202c1b4(x, y + 1 + i, line);
    }
}

THUMB void UnkDebugInfo::unkfunc_0203ce28(int x, int y)
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
    unkfunc_0202c17c(x, y, "WORLDTIME");
    unkfunc_0202c17c(x, y + 1, "%4d/%4d", g_Stage.getWorldTime(), 3392);
    unkfunc_0202c17c(x + 2, y + 2, zone);
    unkfunc_0202c20c(x, y, 9, 3);
}

THUMB void UnkDebugInfo::unkfunc_0203cebc(int x, int y)
{
    if (data_0210bb94.unkfunc_0205810c() == 12) {
        unkfunc_0202c17c(x, y, "WALK");
        TownPlayerManager* manager = TownPlayerManager::getSingleton();
        unkfunc_0202c17c(x, y + 1, "%4d", manager->walkCounter_ >> 4);
        unkfunc_0202c20c(x, y, 4, 2);
    } else if (data_0210bb94.unkfunc_0205810c() == 14) {
    }
}

THUMB void UnkDebugInfo::unkfunc_0203cf14(int x, int y)
{
    unsigned int time = status::g_Game.getPlayTime();
    unkfunc_0202c17c(x, y, "PLAY TIME");
    unsigned int hour = time % 216000;
    unsigned int minute = hour % 3600;
    unkfunc_0202c17c(x, y + 1, "%03d:%02d.%02d%02d", time / 216000, hour / 3600, minute / 60, minute % 60 * 100 / 60);
    unkfunc_0202c20c(x, y, 11, 2);
}

THUMB void UnkDebugInfo::unkfunc_0203cfa8(int x, int y)
{
    unkfunc_0202c17c(x, y, "HEAP");
    unkfunc_0202c17c(x, y + 1, "%3dK", unkfunc_0207f86c(&data_0211a60c) >> 10);
    unkfunc_0202c17c(x, y + 2, "VRAM");
    unkfunc_0202c17c(x, y + 3, "%3dk", data_0211e450.unk_08 >> 10);
    unkfunc_0202c20c(x, y, 5, 4);
}

THUMB void UnkDebugInfo::unkfunc_0203d00c(int show)
{
    showArrayWarning_ = show;
}
