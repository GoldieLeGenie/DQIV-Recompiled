#pragma once
#include "main/menu/MenuBase.hpp"
#include "main/menu/TownMenu_TOWN.hpp"
#include "main/menu/CommonMenu_APITEST.hpp"
#include "ov016/TownShopMenu/TownShopMenu.hpp"
#include "ov016/UnkTownMenu_02178a58/UnkTownMenu_02178a58.hpp"

namespace MenuAPI {
    void unkfunc_0200d3c8();
    void changeMenuModeNormal();
    void changeMenuModeExtra();
    int isMenuModeNormal();
    int isMenuModeExtra();
    void openMenu(menu::MenuBase* menu);
    void closeMenu();
    int isFinishMenu();
    void clearMenuAll();
    void openTownMenu();
    int isTownMenuRoot();
    void openBattleMenu();
    void setBattleBackDrop(unsigned short backDrop);
    void openBattleStadiumAbort();
    void closeBattleStadiumAbort();
    void openMessage(int message, int count);
    void openCommonMessage();
    void addCommonMessage(int message);
    void waitCommonMessage();
    void clearCommonMessage();
    bool isWaitMessage();
    int isMessageWaitTrigger();
    void clearMessageWaitTriggerSE();
    void clearMessageWaitTriggerNOSE();
    void openEncountMessage();
    void openBattleMessage();
    void addMessage(int message);
    void addMessageSerial(int message);
    void catMessage(int message);
    void shakeMessage();
    void setMessageCursor(bool flag);
    void openMessageWindowMenu();
    int isFinishMessageWindow();
    int isFinishMessage();
    int isEndMessage();
    void suspendMessageKeyInput(int flag);
}


