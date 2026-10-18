#pragma ipa file
#include "main/menu/MenuAPI.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/TownMenu_PARTY_TALK.hpp"
#include "main/menu/CommonMenu_YESNO.hpp"
#include "main/global/Global.hpp"
#include "main/cmn/PartyTalk.hpp"
#include "ov003/btl/BattleMonsterMask.hpp"
#include "ov015/btl/BattleMenu.hpp"
#include "ov016/MaterielMenu_LOAD/MaterielMenu_LOAD.hpp"
#include "ov026/MaterielMenu_SURECHIGAI/MaterielMenu_SURECHIGAI.hpp"

static menu::MenuBase* s_rootmenu;
static int s_menuIndex;

static const char* s_menuName[] = {
    0,
    "gMaterielMenu_LOAD",
    "gCommonMenu_APITEST",
    "gTownMenu_ROOT",
    "gTownMenu_MESSAGE",
    "gCmmonMenu_YESNO",
    "gTownMenu_TOWN",
    "gTownShopMenu",
    "gMaterielMenu_SURECHIGAI_ROOT",
    "gMaterielMenu_SURECHIGAI_SELECT_OBJECT",
};

THUMB void MenuAPI::openMenuByIndex()
{
    const char* name = 0;
    if (s_menuIndex != 0) {
        name = s_menuName[s_menuIndex];
    }
    if (name != 0) {
        menu::MenuBase* menu = 0;
        if (dss::strcmp(name, "gMaterielMenu_LOAD") == 0) {
            menu = &gMaterielMenu_LOAD;
        }
        if (dss::strcmp(name, "gCommonMenu_APITEST") == 0) {
            menu = &data_020ed068;
        }
        if (dss::strcmp(name, "gTownMenu_ROOT") == 0) {
            menu = &gTownMenu_ROOT;
        }
        if (dss::strcmp(name, "gTownMenu_MESSAGE") == 0) {
            menu = &data_020ed1bc;
        }
        if (dss::strcmp(name, "gCmmonMenu_YESNO") == 0) {
            menu = &data_020ed094;
        }
        if (dss::strcmp(name, "gTownMenu_TOWN") == 0) {
            menu = &data_020ed11c;
        }
        if (dss::strcmp(name, "gTownShopMenu") == 0) {
            menu = &gTownShopMenu;
        }
        if (dss::strcmp(name, "gMaterielMenu_SURECHIGAI_ROOT") == 0) {
            menu = &gMaterielMenu_SURECHIGAI_ROOT;
        }
        if (dss::strcmp(name, "gMaterielMenu_SURECHIGAI_SELECT_OBJECT") == 0) {
            menu = &gMaterielMenu_SURECHIGAI_SELECT_OBJECT;
        }
        if (menu != 0) {
            clearMenuAll();
            openMenu(menu);
        }
    }
}

THUMB void MenuAPI::changeMenuModeNormal()
{
    clearMenuAll();
    MenuManager::requestMenuMode(MENUDISPLAY_NORMAL);
}

THUMB void MenuAPI::changeMenuModeExtra()
{
    clearMenuAll();
    MenuManager::requestMenuMode(MENUDISPLAY_EXTRA);
}

THUMB int MenuAPI::isMenuModeNormal()
{
    return MenuManager::isMenuMode(MENUDISPLAY_NORMAL);
}

THUMB int MenuAPI::isMenuModeExtra()
{
    return MenuManager::isMenuMode(MENUDISPLAY_EXTRA);
}

THUMB void MenuAPI::openMenu(menu::MenuBase* menu)
{
    clearMenuAll();
    s_rootmenu = menu;
    menu->open();
}

THUMB void MenuAPI::closeMenu()
{
    s_rootmenu->close();
    s_rootmenu = 0;
}

THUMB int MenuAPI::isFinishMenu()
{
    if (s_rootmenu == 0) {
        return true;
    }
    if (s_rootmenu->stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
        return true;
    }
    if (s_rootmenu->stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
        return true;
    }
    return false;
}

THUMB void MenuAPI::clearMenuAll()
{
    MenuManager::clearMenuAll();
    s_rootmenu = 0;
}

THUMB void MenuAPI::openTownMenu()
{
    if (s_menuIndex != 0) {
        openMenuByIndex();
    } else {
        openMenu(&gTownMenu_ROOT);
    }
}

THUMB int MenuAPI::isTownMenuRoot()
{
    if (s_rootmenu == &gTownMenu_ROOT) {
        return true;
    }
    return false;
}

THUMB void MenuAPI::openBattleMenu()
{
    BattleMonsterMask::getSingleton()->setup();
    if (g_Global.fightStadiumFlag_ == 0) {
        BattleMenuJudge::getSingleton()->baseSetup();
        openMenu(&gBattleMenu_ROOT);
    }
}

THUMB void MenuAPI::setBattleBackDrop(unsigned short backDrop)
{
    unkfunc_02081264(backDrop);
}

THUMB void MenuAPI::openBattleStadiumAbort()
{
    gBattleMenu_StadiumAbort.open();
    gBattleMenu_StadiumAbort.menuSetup();
}

THUMB void MenuAPI::closeBattleStadiumAbort()
{
    gBattleMenu_StadiumAbort.close();
}

THUMB void MenuAPI::openMessage(int message, int count)
{
    openMenu(&data_020ed1bc);
    data_020ed1bc.openMessageForTALK();
    data_020ed1bc.addMessageCount(message, count);
}

THUMB void MenuAPI::openCommonMessage()
{
    openMenu(&data_020ed1bc);
    data_020ed1bc.openMessageForTALK();
}

THUMB void MenuAPI::addCommonMessage(int message)
{
    data_020ed1bc.addMessage(message);
}

THUMB void MenuAPI::waitCommonMessage()
{
    data_020ed1bc.addMessageWAITKEY();
}

THUMB void MenuAPI::clearCommonMessage()
{
    data_020ed1bc.clearMessageWAITPROG();
}

THUMB bool MenuAPI::isWaitMessage()
{
    return data_020ed1bc.isMessageWAITPROG();
}

THUMB int MenuAPI::isMessageWaitTrigger()
{
    return s_draw->isWaitTrigger();
}

THUMB void MenuAPI::clearMessageWaitTriggerSE()
{
    s_draw->clearWaitTriggerSE();
}

THUMB void MenuAPI::clearMessageWaitTriggerNOSE()
{
    s_draw->clearWaitTriggerNoSE();
}

THUMB void MenuAPI::openEncountMessage()
{
    openMenu(&data_020ed1bc);
    data_020ed1bc.openMessageForENCOUNT();
    if (g_Global.fightStadiumFlag_ == 0) {
        gBattleMenuSub_HISTORY.open();
        gBattleMenuSub_HISTORY.history_ = 0;
    }
}

THUMB void MenuAPI::openBattleMessage()
{
    int abort = gBattleMenu_StadiumAbort.isOpen();
    openMenu(&data_020ed1bc);
    data_020ed1bc.openMessageForBATTLE();
    if (g_Global.fightStadiumFlag_ == 0) {
        gBattleMenuSub_HISTORY.open();
        gBattleMenuSub_HISTORY.history_ = 1;
        if (abort) {
            gBattleMenu_StadiumAbort.open();
        }
    }
}

THUMB void MenuAPI::addMessage(int message)
{
    data_020ed1bc.addMessage(message);
}

THUMB void MenuAPI::addMessageSerial(int message)
{
    data_020ed1bc.addMessageSerial(message);
}

THUMB void MenuAPI::catMessage(int message)
{
    data_020ed1bc.addMessageNOWAIT(message);
}

THUMB void MenuAPI::shakeMessage()
{
    MessageWindow* window = s_draw;
    window->shake_ = 1;
    window->shakeCount_ = 0;
}

THUMB void MenuAPI::setMessageCursor(bool flag)
{
    data_020ed1bc.setMessageIntervalCursor(flag);
    data_020ed1bc.setMessageLastCursor(flag);
}

THUMB void MenuAPI::openMessageWindowMenu()
{
    data_020ed1bc.openMessageForMENU();
}

THUMB int MenuAPI::isFinishMessageWindow()
{
    if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
        return true;
    }
    if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
        return true;
    }
    return false;
}

THUMB int MenuAPI::isFinishMessage()
{
    return s_draw->isFinish();
}

THUMB int MenuAPI::isEndMessage()
{
    return s_draw->isEnd();
}

THUMB void MenuAPI::suspendMessageKeyInput(int flag)
{
    data_020ed1bc.suspendInput_ = flag;
}
