#pragma ipa file
#include "main/part/BattleMenuPart.hpp"
#include "main/global/Global.hpp"
#include "main/data/FileLoader.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Pad.hpp"
#include "main/btl/BattleMapList.hpp"
#include "main/encount/Encount.hpp"
#include "main/menu/MenuAPI.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "nitro/g3.hpp"

// the dss header templates: their int instances are emitted in this file
namespace dss {
    template <typename T> T max(T a, T b)
    {
        return (a > b) ? a : b;
    }

    template <typename T> T min(T a, T b)
    {
        return (a < b) ? a : b;
    }

    template <typename T> T clamp(T a, T b, T c)
    {
        return min(max(a, b), c);
    }

    template <typename T> T loop(T x, T a, T b)
    {
        if (x > b) {
            return a;
        }
        if (x < a) {
            return b;
        }
        return x;
    }
}

BattleMenuPart g_BattleMenuPart;

ARM BattleMenuPart::BattleMenuPart()
{
    tileId_ = 0x40;
    timeZone_ = 0;
    group_ = 0;
    monster_[0] = 0;
    count_[0] = 1;
    monster_[1] = 0;
    count_[1] = 0;
    monster_[2] = 0;
    count_[2] = 0;
    monster_[3] = 0;
    count_[3] = 0;
}

ARM void BattleMenuPart::initialize()
{
    x_ = 0;
    y_ = 2;
    cursor_ = 0;
    mode_ = 0;
    isStart_ = 0;
    if (g_battleMapList.isLoaded() == 0) {
        g_battleMapList.load("data/btlmap/_list.txt");
    }
    MenuAPI::setBattleBackDrop(0);
    g_Global.fadeIn(30);
}

ARM void BattleMenuPart::terminate()
{
}

ARM void BattleMenuPart::onExecutePart()
{
    char path[128];

    if (dss::g_Pad.edge() & 0x40) {
        cursor_--;
    }
    if (dss::g_Pad.edge() & 0x80) {
        cursor_++;
    }
    g_DebugConsole.unkfunc_0207e810();
    if (cursor_ == 0) {
        if (dss::g_Pad.edge() & 0x10) {
            mode_ = 1;
        }
        if (dss::g_Pad.edge() & 0x20) {
            mode_ = 0;
        }
    }
    if (mode_ == 0) {
        updateEncountMode();
        cursor_ = dss::loop<int>(cursor_, 0, 4);
    }
    if (mode_ == 1) {
        updateSelectMode();
        cursor_ = dss::loop<int>(cursor_, 0, 5);
    }
    if (isStart_ != 0) {
        return;
    }
    if (!(dss::g_Pad.edge() & 1)) {
        return;
    }
    isStart_ = 1;
    dss::sprintf_s(path, sizeof(path), "data/btlmap/%s.lz", g_battleMapList.getMapName());
    if (!dss::g_File.isExist(path)) {
        return;
    }
    if (mode_ == 0) {
        status::g_Story.setChapter(1);
        encount::Encount::getSingleton()->setTileId(tileId_);
        encount::Encount::getSingleton()->setTimeZone((TIME_ZONE)timeZone_);
        encount::Encount::getSingleton()->setPartyCount(1);
        encount::Encount::getSingleton()->brew();
    } else if (mode_ == 1) {
        for (int i = 0; i < 4; i++) {
            encount::Encount::getSingleton()->setMonsterIndex(i, monster_[i]);
            encount::Encount::getSingleton()->setMonsterCount(i, count_[i]);
        }
    }
    g_Stage.setBtlMapName(g_battleMapList.getMapName());
    g_Global.directStartBattle();
}

ARM void BattleMenuPart::onDrawPart()
{
    g_DebugConsole.unkfunc_0207e88c(x_, y_ + cursor_, ">");
    if (mode_ == 0) {
        g_DebugConsole.unkfunc_0207e88c(x_ + 1, y_, "1.Mode : Encount");
        drawEncountMode();
    }
    if (mode_ == 1) {
        g_DebugConsole.unkfunc_0207e88c(x_ + 1, y_, "1.Mode : Select");
        drawSelectMode();
    }
}

ARM void BattleMenuPart::onWindowPart()
{
}

ARM void BattleMenuPart::onDebugPart()
{
    g_DebugConsole.unkfunc_0207e88c(0, 0, "Battle Menu Part", REG_GFX_RAM_COUNT);
}

ARM void BattleMenuPart::updateEncountMode()
{
    switch (cursor_) {
    case 0:
        break;
    case 1:
        if (dss::g_Pad.repeat() & 0x10) {
            tileId_++;
        }
        if (dss::g_Pad.repeat() & 0x20) {
            tileId_--;
        }
        break;
    case 2:
        if (dss::g_Pad.repeat() & 0x10) {
            timeZone_++;
        }
        if (dss::g_Pad.repeat() & 0x20) {
            timeZone_--;
        }
        timeZone_ = dss::clamp<int>(timeZone_, 0, 2);
        break;
    case 3:
        if (dss::g_Pad.repeat() & 0x10) {
            g_battleMapList.nextGroup();
        }
        if (dss::g_Pad.repeat() & 0x20) {
            g_battleMapList.prevGroup();
        }
        break;
    case 4:
        if (dss::g_Pad.repeat() & 0x10) {
            g_battleMapList.nextMap();
        }
        if (dss::g_Pad.repeat() & 0x20) {
            g_battleMapList.prevMap();
        }
        break;
    }
}

ARM void BattleMenuPart::updateSelectMode()
{
    switch (cursor_) {
    case 0:
        break;
    case 1:
        if (dss::g_Pad.repeat() & 0x10) {
            group_++;
        }
        if (dss::g_Pad.repeat() & 0x20) {
            group_--;
        }
        group_ = dss::clamp<int>(group_, 0, 3);
        break;
    case 2:
        if (dss::g_Pad.pad() & 0x100) {
            if (dss::g_Pad.repeat() & 0x10) {
                monster_[group_] += 10;
            }
            if (dss::g_Pad.repeat() & 0x20) {
                monster_[group_] -= 10;
            }
        } else {
            if (dss::g_Pad.repeat() & 0x10) {
                monster_[group_]++;
            }
            if (dss::g_Pad.repeat() & 0x20) {
                monster_[group_]--;
            }
        }
        monster_[group_] = dss::clamp<int>(monster_[group_], 0, 0x140);
        break;
    case 3:
        if (dss::g_Pad.repeat() & 0x10) {
            count_[group_]++;
        }
        if (dss::g_Pad.repeat() & 0x20) {
            count_[group_]--;
        }
        count_[group_] = dss::clamp<int>(count_[group_], 0, 8);
        break;
    case 4:
        if (dss::g_Pad.repeat() & 0x10) {
            g_battleMapList.nextGroup();
        }
        if (dss::g_Pad.repeat() & 0x20) {
            g_battleMapList.prevGroup();
        }
        break;
    case 5:
        if (dss::g_Pad.repeat() & 0x10) {
            g_battleMapList.nextMap();
        }
        if (dss::g_Pad.repeat() & 0x20) {
            g_battleMapList.prevMap();
        }
        break;
    }
}

ARM void BattleMenuPart::drawEncountMode()
{
    g_DebugConsole.unkfunc_0207e88c(x_ + 1, y_ + 1, "2.Encount Tile : %3d", tileId_);
    char timeZone[3][8] = { "None", "Day", "Night" };
    g_DebugConsole.unkfunc_0207e88c(x_ + 1, y_ + 2, "3.Time Zone    : %s", timeZone[timeZone_]);
    g_DebugConsole.unkfunc_0207e88c(x_ + 1, y_ + 3, "4.Group        : %s", g_battleMapList.getGroupName());
    g_DebugConsole.unkfunc_0207e88c(x_ + 1, y_ + 4, "5.Map          : %s", g_battleMapList.getMapName());
}

ARM void BattleMenuPart::drawSelectMode()
{
    g_DebugConsole.unkfunc_0207e88c(x_ + 1, y_ + 1, "2.GROUP   : %d", group_ + 1);
    g_DebugConsole.unkfunc_0207e88c(x_ + 1, y_ + 2, "3.MONSTER : m%03d", monster_[group_]);
    g_DebugConsole.unkfunc_0207e88c(x_ + 1, y_ + 3, "4.COUNT   : %d", count_[group_]);
    g_DebugConsole.unkfunc_0207e88c(x_ + 1, y_ + 4, "5.Group   : %s", g_battleMapList.getGroupName());
    g_DebugConsole.unkfunc_0207e88c(x_ + 1, y_ + 5, "6.Map     : %s", g_battleMapList.getMapName());
    g_DebugConsole.unkfunc_0207e88c(x_ + 0x18, y_ + 1, "m%03d(%d)", monster_[0], count_[0]);
    g_DebugConsole.unkfunc_0207e88c(x_ + 0x18, y_ + 2, "m%03d(%d)", monster_[1], count_[1]);
    g_DebugConsole.unkfunc_0207e88c(x_ + 0x18, y_ + 3, "m%03d(%d)", monster_[2], count_[2]);
    g_DebugConsole.unkfunc_0207e88c(x_ + 0x18, y_ + 4, "m%03d(%d)", monster_[3], count_[3]);
}
