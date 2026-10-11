#include "ov016/TownMenu_OPERATION/TownMenu_OPERATION_SHIFT_PARTY.hpp"
#include "ov016/TownMenu_OPERATION/TownMenu_OPERATION_ROOT.hpp"
#include "ov016/TownMenu_ROOT/TownMenu_ROOT.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/cmn/GameManager.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "ov016/MenuTemplate/MenuTemplate_town.hpp"
#include "ov016/UnkTownMenuDraw/UnkTownMenuDraw_0217d560.hpp"

THUMB void TownMenu_OPERATION_SHIFT_PARTY::menuSetup()
{
    status::g_Party.setMemberShiftMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    carriageEnable_ = status::g_Party.getCarriageEnableOnGame();
    selectCharaMaxCount_ = 0;
    enableEnter_ = 0;
    page_ = 0;
    for (int i = 0; i < 4; i++) {
        selectCharaAllocation_[i] = -1;
    }
}

THUMB void TownMenu_OPERATION_SHIFT_PARTY::menuExecute()
{
    status::g_Party.setMemberShiftMode();
    int selectCount = selectCharaMaxCount_;
    int count = status::g_Party.getCount() - selectCount;
    if (count == 0 && carriageEnable_ == 0) {
        count = 1;
    }
    if (selectCount != 0 && carriageEnable_ != 0) {
        count++;
    }
    MenuTemplate_town::TOWN_ICON32_5x2(&menuItem_, menuItem_.active_, count);
    MenuTemplate_town::TOWN_CANCEL(&cancelItem_);
}

THUMB void TownMenu_OPERATION_SHIFT_PARTY::menuDraw()
{
    status::g_Party.setMemberShiftMode();
    int partyCount = status::g_Party.getCount();
    char list[11] = { 0 };
    for (char i = 0; i < 11; i++) {
        list[i] = 0;
    }
    int found = 0;
    int count = 0;
    int active = menuItem_.active_;
    for (char i = 0; i < status::g_Party.getCount(); i++) {
        for (char j = 0; j < selectCharaMaxCount_; j++) {
            if (i == selectCharaAllocation_[j]) {
                found = 1;
            }
        }
        if (!found) {
            list[count] = i;
            count++;
        }
        found = 0;
    }
    list[count] = -1;
    unkfunc_0217deb4(selectCharaAllocation_, list, active, count);
    menuItem_.drawActive();
    cancelItem_.drawActive();
}

THUMB void TownMenu_OPERATION_SHIFT_PARTY::menuUpdate()
{
    status::g_Party.setMemberShiftMode();
    if (enableEnter_ == 1) {
        unkfunc_0216cdd4();
        return;
    }
    if (data_020ed1bc.isOpen()) {
        if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK || data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            close();
            gTownMenu_ROOT.open();
            gTownMenu_ROOT.menuItem_.active_ = TownMenu_ROOT::ROOT_OPERATION;
            redraw_ = 1;
        }
        return;
    }
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        if (selectCharaMaxCount_ == 0) {
            close();
            gTownMenu_OPERATION_ROOT.open();
            redraw_ = 1;
        } else {
            selectCharaMaxCount_--;
            selectCharaAllocation_[selectCharaMaxCount_] = -1;
        }
        redraw_ = 1;
        return;
    }
    int selectCount = selectCharaMaxCount_;
    int count = status::g_Party.getCount() - selectCount;
    if (selectCount != 0 && carriageEnable_ != 0) {
        count++;
    }
    navigator_.setup(5, 2, count);
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result == 0) {
        return;
    }
    if (result == 2) {
        unkfunc_0216ccbc();
    }
    redraw_ = 1;
}

THUMB void TownMenu_OPERATION_SHIFT_PARTY::unkfunc_0216ccbc()
{
    status::g_Party.setMemberShiftMode();
    int found = 0;
    char list[11] = { 0 };
    for (int i = 0; i < 11; i++) {
        list[i] = 0;
    }
    int active = menuItem_.active_;
    int partyCount = status::g_Party.getCount();
    int selectCount = selectCharaMaxCount_;
    int rest = partyCount - selectCount;
    if (active == rest && selectCount != 0) {
        enableEnter_ = 1;
        return;
    }
    if (selectCount == 4 || rest == 0) {
        return;
    }
    int n = 0;
    for (char i = 0; i < partyCount; i++) {
        for (int j = 0; j < selectCharaMaxCount_; j++) {
            if (i == selectCharaAllocation_[j]) {
                found = 1;
            }
        }
        if (!found) {
            list[n] = i;
            n++;
        }
        found = 0;
    }
    rest = partyCount - selectCharaMaxCount_;
    if (active == rest && selectCharaMaxCount_ != 0) {
        enableEnter_ = 1;
        return;
    }
    selectCharaAllocation_[selectCharaMaxCount_] = list[active];
    selectCharaMaxCount_++;
    if (selectCharaMaxCount_ == 4 || partyCount - selectCharaMaxCount_ == 0) {
        if (carriageEnable_ == 1) {
            menuItem_.active_ = partyCount - 4;
        } else {
            menuItem_.active_ = 0;
        }
    } else if (active == partyCount - selectCharaMaxCount_) {
        menuItem_.active_ = active - 1;
    }
}

THUMB void TownMenu_OPERATION_SHIFT_PARTY::unkfunc_0216cdd4()
{
    status::g_Party.setMemberShiftMode();
    for (int i = 0; i < 4; i++) {
        if (selectCharaAllocation_[i] >= 0) {
            selectCharaAllocation_[i] = status::g_Party.getPlayerIndex(selectCharaAllocation_[i]);
        } else {
            selectCharaAllocation_[i] = 0;
        }
    }
    status::g_Party.reorder(selectCharaAllocation_[0], selectCharaAllocation_[1], selectCharaAllocation_[2], selectCharaAllocation_[3]);
    cmn::GameManager::getSingleton()->resetParty();
    close();
    stat_ = MENUBASE_STAT_OK;
    gTownMenu_ROOT.stat_ = MENUBASE_STAT_OK;
    if (data_0210bb94.unkfunc_02058114(0xe) == 1) {
        SoundManager::fieldPlay();
    }
}
