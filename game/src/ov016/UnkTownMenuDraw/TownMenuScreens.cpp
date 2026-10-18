#pragma ipa file
#include "ov016/UnkTownMenuDraw/TownMenuScreens.hpp"
#include "ov016/UnkTownMenuDraw/TownMenuDraw.hpp"
#include "main/menu/MenuBase.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/UseItem.hpp"
#include "main/status/UseAction.hpp"
#include "main/menu/MenuManager.hpp"
#include "ov016/TownMenuPlayerControl/TownMenuPlayerControl.hpp"
#include "ov016/status/FukuroItemInfo.hpp"
#include "ov016/status/PlayerItemInfo.hpp"
#include "main/menu/MenuPartsDraw.hpp"
#include "main/menu/MenuCommonDraw.hpp"

// not in the ROM (dead-stripped), keeps the const locals of this file
THUMB void unkfunc_unused_11()
{
    const int unk0 = -1;
    const int unk1 = 1;
    const int unk2 = -1;
    const int unk3 = 2;
    const int unk4 = 0;
    const int unk5 = -1;
    const int unk6 = -1;
}

THUMB void drawRootMenuScreen()
{
    if (!data_020ed1bc.isOpen()) {
        drawRootMenuCommands(-1, -1);
        drawRootMenuButtonFrames(0, 0, 1);
    }
    drawPartyStatusWindows();
    drawMoneyWindow(-1, -1, 0);
}

THUMB void drawItemCharaSelectScreen(int chara)
{
    if (!data_020ed1bc.isOpen()) {
        drawItemMenuHeader(chara, 0, 1, 0);
        drawChooseCharaPrompt(1);
        drawCharaSelectWindow(1, 0);
    }
    drawPartyStatusWindows();
    if (!data_020ed1bc.isOpen()) {
        drawCharaItemList(chara, 0);
        drawMenuFrame(0, 0x50, 0x100, 0x70, -1);
    }
}

THUMB void drawItemSelectScreen(int chara, int itemID, int page)
{
    if (!data_020ed1bc.isOpen()) {
        drawItemMenuHeader(chara, 0, 1, 0);
        drawItemListPage(chara, page, 0);
        drawMenuFrame(0, 0, 0x100, 0xa0, 0x70);
        drawMenuDescription(itemID, 0, 0);
        if (TownMenuPlayerControl::getSingleton()->activeFukuro_ != 0) {
            if (status::FukuroItemInfo::getItemMaxCount() > 6) {
                TownMenuPlayerControl* ctrl = TownMenuPlayerControl::getSingleton();
                drawPageIndicator(0xb8, status::FukuroItemInfo::getPageMax() - 1, ctrl->activeItemPage_, 1);
            }
        } else if (status::PlayerItemInfo::getItemMaxCount(chara) > 6) {
            drawPageIndicator(0xd0, 1, TownMenuPlayerControl::getSingleton()->activeItemPage_, 0);
        }
    }
    drawPartyStatusWindows();
    drawMenuFrame(0, 0x78, 0xb0, 0x48, 0x90);
    switch (status::UseItem::getItemType(itemID)) {
    case -1:
        break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        drawEquipCompareWindow(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, itemID, 0, 0);
        break;
    default:
        drawItemUsageInfo(itemID);
        break;
    }
}

THUMB void drawItemCommandScreen(int chara, int itemID, int page, int sound)
{
    if (!data_020ed1bc.isOpen() && sound == 0) {
        int x = (TownMenuPlayerControl::getSingleton()->activeItem_ % 2) * 0x78 + 0x14;
        int y = (TownMenuPlayerControl::getSingleton()->activeItem_ / 2) * 32 + 0x14;
        int command;
        if (TownMenuPlayerControl::getSingleton()->activeFukuro_ != 0) {
            command = status::FukuroItemInfo::getItemFukuroCommandFlag();
        } else {
            command = status::PlayerItemInfo::getItemPlayerCommandFlag(chara, TownMenuPlayerControl::getSingleton()->getActiveItemIndexToAll());
        }
        drawItemMenuHeader(chara, 0, 1, 0);
        drawItemUseCommands(command);
        drawMenuFrame(0, 0, 0x100, 0xa0, 0x68);
        drawItemListPage(chara, page, 0);
        drawTownCommandCursor(x, y + 4);
    }
    drawPartyStatusWindows();
    if (!data_020ed1bc.isOpen() && sound == 0) {
        drawMenuFrame(0, 0x78, 0xb0, 0x48, 0x90);
        switch (status::UseItem::getItemType(itemID)) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            drawEquipCompareWindow(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, itemID, 0, 0);
            break;
        default:
            drawItemUsageInfo(itemID);
            break;
        }
    }
}

THUMB void drawItemUseTargetScreen(int active, int itemID, int page)
{
    status::g_Party.setBattleMode();
    if (!data_020ed1bc.isOpen()) {
        drawItemMenuHeader(active + page * 4, 0, 1, 1);
        drawMenuFrame(0, 0, 0x100, 0xa0, -1);
        drawPartyHpMpListBattle(page);
        status::g_Party.setPlayerMode();
    }
    drawPartyStatusWindows();
    if (!data_020ed1bc.isOpen()) {
        drawItemUsageInfo(itemID);
        drawMenuFrame(0, 0x78, 0xb0, 0x48, 0x90);
    }
}

THUMB void drawItemTargetCharaScreen(int activeChara, int targetChara, int itemID, int sound)
{
    if (!data_020ed1bc.isOpen() && sound == 0) {
        drawItemMenuHeader(activeChara, 0, 0, 0);
        drawSmallButtonFrame(0x88, 0xa0);
        drawTargetCharaNameBar(&status::g_Party.getPlayerStatus(targetChara)->haveStatusInfo_);
        drawItemTargetInfo(&status::g_Party.getPlayerStatus(targetChara)->haveStatusInfo_, itemID, 0);
        drawCharaSelectWindow(1, 0);
        drawMenuFrame(0, 0, 0x100, 0x48, 0x28);
    }
    drawCharaItemList(targetChara, 1);
    drawPartyStatusWindows();
    drawMenuFrame(0, 0x50, 0x100, 0x70, -1);
}

THUMB void drawItemTargetItemScreen(int activeChara, int targetChara, int itemID, int page, int sound)
{
    if (!data_020ed1bc.isOpen() && sound == 0) {
        drawItemMenuHeader(activeChara, 0, 0, 0);
        drawSmallButtonFrame(0x88, 0xa0);
        drawTargetCharaNameBar(&status::g_Party.getPlayerStatus(targetChara)->haveStatusInfo_);
        drawMenuFrame(0, 0, 0x100, 0xa0, 0x70);
        drawItemListPage(targetChara, page, 1);
        drawMenuDescription(0, 0, 1);
        if (status::PlayerItemInfo::getItemMaxCount(targetChara) + 1 > 6) {
            drawPageIndicator(0xd0, 1, TownMenuPlayerControl::getSingleton()->targetItemPage_, 0);
        }
    }
    drawItemTargetInfo(&status::g_Party.getPlayerStatus(targetChara)->haveStatusInfo_, itemID, 1);
    drawMenuFrame(0, 8, 0x100, 0x48, 0x30);
    drawCharaItemList(targetChara, 1);
    drawMenuFrame(0, 0x50, 0x100, 0x70, -1);
}

THUMB void drawMagicRootScreen(int mp1, int mp2, int magicID, int chara, int mode)
{
    if (!data_020ed1bc.isOpen()) {
        status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(chara)->haveStatusInfo_;
        drawMagicMenuHeader(chara);
        drawCasterMpBar(info, mp1, mp2);
        if (mode == 0) {
            drawChooseCharaPrompt(0);
            drawCharaSelectWindow(0, 0);
            drawTownSpellList(info);
            drawMenuFrame(0, 0x58, 0x100, 0x58, -1);
        } else {
            drawFieldSpellList(chara);
            drawMenuDescription(magicID, 1, 0);
            drawMenuFrame(0, 0, 0x100, 0xa0, 0x70);
            unkfunc_0217cf1c(0);
        }
    }
    drawPartyStatusWindows();
}

THUMB void drawMagicCharaSelectScreen(int mp, int useMp, int chara, int page)
{
    if (!data_020ed1bc.isOpen()) {
        drawMagicMenuHeader(chara);
        drawCasterMpBar(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, mp, useMp);
        drawMenuFrame(0, 0, 0x100, 0xa0, -1);
        drawPartyHpMpList(page);
    }
    drawPartyStatusWindows();
    drawMoneyWindow(-1, -1, 0);
}

THUMB void drawWarpTownSelectScreen(int mp, int useMp, int chara, int page, unsigned char* town)
{
    if (!data_020ed1bc.isOpen()) {
        if (mp != -1 && useMp != -1) {
            drawRootMenuCommands(1, -1);
            drawRootMenuButtonFrames(0x40, 0xa0, 0);
            drawCasterMpBar(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, mp, useMp);
            drawSmallButtonFrame(0, 0xa0);
            drawActiveCharaNameBar(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, 0, chara, 0);
        } else {
            drawRootMenuCommands(2, -1);
            drawRootMenuButtonFrames(0x40, 0xa0, 0);
            drawSmallButtonFrame(0x88, 0xa0);
            drawActiveCharaNameBar(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, 1, chara, 0);
        }
        drawMenuFrame(0, 0, 0x100, 0xa0, 0x70);
        drawRuraTownList(page, town);
        unkfunc_0217cf1c(1);
    }
    drawPartyStatusWindows();
}

THUMB void drawStatusMainPage(int index, int page)
{
    drawStatusMenuHeader(index, page);
    drawStatusCharaProfile(&status::g_Party.getPlayerStatus(index)->haveStatusInfo_);
    drawStatusAbilities(&status::g_Party.getPlayerStatus(index)->haveStatusInfo_);
    drawMenuFrame(0, 0, 0x98, 0x70, 0x18);
    drawMenuFrame(0x98, 0, 0x68, 0x70, -1);
    drawStatusEquipment(&status::g_Party.getPlayerStatus(index)->haveStatusInfo_, 0);
    drawMenuFrame(0, 0x60, 0x98, 0x60, -1);
    drawMenuFrame(0x98, 0xa0, 0x68, 0x20, -1);
    drawMenuFrame(0x98, 0x80, 0x68, 0x20, -1);
}

THUMB void drawStatusSpellPage(int index, int page)
{
    drawStatusMenuHeader(index, page);
    drawStatusSpellList(&status::g_Party.getPlayerStatus(index)->haveStatusInfo_, 0);
    drawMenuFrame(0, 0, 0x100, 0x70, 0x28);
    drawStatusSpellList(&status::g_Party.getPlayerStatus(index)->haveStatusInfo_, 1);
    drawMenuFrame(0, 0, 0x100, 0xc0, 0x28);
}

THUMB void drawStatusPartySummaryScreen(int index, int type, int page)
{
    drawStatusMenuHeader(index, page);
    drawPartySummaryStats(type);
}

THUMB void drawStatusMoneyScreen(int page)
{
    drawStatusMenuHeader(-2, page);
    drawPartyMoneyAndPlayTime();
    drawMenuFrame(0, 0, 0x100, 0x70, 0x28);
    drawMenuFrameLine(0, 0x50, 0x100);
}

THUMB void drawOperationRootScreen(int mode, char* command, int count, int chara)
{
    if (!data_020ed1bc.isOpen()) {
        drawRootMenuCommands(5, -1);
        drawRootMenuButtonFrames(0x40, 0xa0, 0);
        if (mode == 2) {
            drawRootMenuCommands(5, -1);
            drawRootMenuButtonFrames(0x40, 0xa0, 0);
            drawSmallButtonFrame(0x88, 0xa0);
            if (chara < 0) {
                drawActiveCharaNameBar(&status::g_Party.getPlayerStatus(0)->haveStatusInfo_, 1, chara, 0);
            } else {
                drawActiveCharaNameBar(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, 1, chara, 0);
            }
            status::g_Party.setBattleMode();
            char order[12];
            char n = 0;
            for (char i = 0; i < status::g_Party.getCount(); i++) {
                status::PlayerStatus* player = status::g_Party.getPlayerStatus(i);
                if (player->haveStatusInfo_.haveStatus_.isPlayer() != false) {
                    order[n] = i;
                    n++;
                }
            }
            order[n] = -1;
            drawPartyOrderIcons(order, mode);
        } else {
            drawTacticsCommandList(mode, command, count);
        }
    }
    drawPartyStatusWindows();
    if (chara != -2 && mode == 2) {
        drawCharaItemList(chara, 0);
        drawMenuFrame(0, 0x50, 0x100, 0x70, -1);
    }
}

THUMB void drawTacticsScreen(char* chara, int index)
{
    drawRootMenuCommands(5, -1);
    drawRootMenuButtonFrames(0x40, 0xa0, 0);
    drawSmallButtonFrame(0x88, 0xa0);
    if (index < 0) {
        drawActiveCharaNameBar(&status::g_Party.getPlayerStatus(0)->haveStatusInfo_, 1, index, 0);
    } else {
        drawActiveCharaNameBar(&status::g_Party.getPlayerStatus(index)->haveStatusInfo_, 1, index, 0);
    }
    drawPartyOrderIcons(chara, -1);
    drawPartyTacticsList(chara);
}

THUMB void drawTacticsSelectScreen(char* chara, int index, int active)
{
    int x = (active % 5) * 0x28 + 0x14;
    int y = (active / 5) * 0x28 + 0x64;
    int count = status::g_Party.getCount();
    if (status::g_Party.getSortIndex(1) != -1 || status::g_Party.getSortIndex(2) != -1) {
        count--;
    }
    if (count < 5) {
        y = 0x8c;
    }
    drawTacticsOptions();
    drawMenuFrame(0x10, 0, 0xd0, 0x48, -1);
    drawTownCommandCursor(x, y);
    drawTacticsScreen(chara, index);
}

THUMB void drawShiftPartyScreen(char* select, char* list, int active, int count)
{
    if (!data_020ed1bc.isOpen()) {
        drawShiftPartyIconList(select, list, count);
        drawMenuFrame(0, 0x68, 0xe0, 0x58, -1);
        drawShiftPartyFrontSlots(select);
        drawMenuFrame(0, 0, 0x90, 0x68, -1);
        if (list[active] != -1) {
            drawCharaStatusWindow(list[active], 0);
            if (status::g_Party.getCarriageEnableOnGame()) {
                drawStatusSpellList(&status::g_Party.getPlayerStatus(list[active])->haveStatusInfo_, 1);
                drawMenuFrame(0, 0, 0x100, 0xc0, 0x28);
            } else {
                drawPartyStatusWindows();
            }
        } else {
            unkfunc_0217ccac();
            drawPartyStatusWindows();
        }
    }
}

THUMB void drawEquipScreen(int itemType, int chara, int page, unsigned char* list, int count, int active)
{
    if (!data_020ed1bc.isOpen()) {
        drawRootMenuCommands(5, 0);
        drawRootMenuButtonFrames(0, 0xa0, 0);
        unkfunc_0217cc4c();
        drawMenuFrame(0x48, 0xa0, 0x40, 0x20, -1);
        drawSmallButtonFrame(0x88, 0xa0);
        drawActiveCharaNameBar(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, 1, chara, 0);
        if (itemType == -1) {
            drawCharaSelectWindow(0, 1);
        } else {
            drawEquipItemList(chara, list, count, itemType, page);
            if (count > 6) {
                drawPageIndicatorWide(0x50, (count - 1) / 6, page);
            }
            unkfunc_0217cc6c();
            drawMenuFrame(0, 0, 0x100, 0xa0, -1);
        }
    }
    status::g_Party.setPlayerMode();
    if (itemType != -1) {
        if (active == -1) {
            drawEquipItemInfo(chara, -1, itemType);
        } else {
            drawEquipItemInfo(chara, active, itemType);
        }
        drawMenuFrame(0, 0x78, 0x68, 0x48, 0x90);
    }
    drawStatusEquipment(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, 1);
    drawMenuFrame(0x68, 0x60, 0x98, 0x60, -1);
}

THUMB void drawItemMenuHeader(int chara, int flag, int mode, int fukuro)
{
    drawRootMenuCommands(2, -1);
    drawRootMenuButtonFrames(0x40, 0xa0, flag);
    if (mode != 0) {
        drawSmallButtonFrame(0x88, 0xa0);
    } else {
        drawSmallButtonFrame(0, 0xa0);
    }
    drawActiveCharaNameBar(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, mode, chara, fukuro);
}

THUMB void drawMagicMenuHeader(int chara)
{
    drawRootMenuCommands(1, -1);
    drawRootMenuButtonFrames(0x40, 0xa0, 0);
    drawSmallButtonFrame(0, 0xa0);
    drawActiveCharaNameBar(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, 0, chara, 0);
}

THUMB void drawStatusMenuHeader(int index, int page)
{
    drawRootMenuCommands(4, -1);
    drawRootMenuButtonFrames(0x40, 0xa0, 0);
    drawSmallButtonFrame(0x88, 0xa0);
    if (index == -2) {
        drawActiveCharaNameBar(&status::g_Party.getPlayerStatus(0)->haveStatusInfo_, 1, index, 0);
    } else {
        drawActiveCharaNameBar(&status::g_Party.getPlayerStatus(index)->haveStatusInfo_, 1, index, 0);
    }
    drawStatusPartyIcons(page);
}

THUMB void drawCharaSelectWindow(int mode, int flag)
{
    int count;
    if (mode != 0 && status::g_Party.fukuro_ != 0) {
        count = status::g_Party.getCount() + 1;
    } else {
        count = status::g_Party.getCount();
    }
    drawPartyIconRow(mode, flag);
    if (count < 5) {
        drawMenuFrame(0, 0x70, 0xb8, 0x30, -1);
    } else if (count == 5) {
        drawMenuFrame(0, 0x70, 0xe0, 0x30, -1);
    } else {
        drawMenuFrame(0, 0x48, 0xe0, 0x58, -1);
    }
}

THUMB void drawStatusPartyIcons(int page)
{
    status::g_Party.setBattleMode();
    int count = status::g_Party.getCount();
    if (page == 0) {
        if (count > 4) {
            count = 5;
            drawPageIndicator(0xd0, status::g_Party.getCount() / 5, page, 0);
        }
        for (int i = 0; i < count; i++) {
            drawPartyMemberIcon(&status::g_Party.getPlayerStatus(i)->haveStatusInfo_, i * 0x28 + 0x10, 0x70);
        }
        for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
            drawCarriageOrderNumber(i + 1, i * 0x28 + 8, 0x78, i);
        }
        if (status::g_Party.getCount() < 4) {
            int x = status::g_Party.getCount() * 0x28 + 0x10;
            unkfunc_0217af90(-2, x, 0x70);
            drawMenuFrame(0, 0x70, 0xb8, 0x30, -1);
        } else if (status::g_Party.getCount() == 4) {
            unkfunc_0217af90(-2, 0xb0, 0x70);
            drawMenuFrame(0, 0x70, 0xe0, 0x30, -1);
        } else {
            drawMenuFrame(0, 0x70, 0x100, 0x30, -1);
        }
    } else {
        if (page == 1) {
            if (count > 10) {
                count = 10;
            }
            for (int i = 5; i < count; i++) {
                drawPartyMemberIcon(&status::g_Party.getPlayerStatus(i)->haveStatusInfo_, (i - page * 5) * 0x28 + 0x10, 0x70);
            }
            if (status::g_Party.getCount() < 10) {
                int x = (status::g_Party.getCount() - 5) * 0x28 + 0x10;
                unkfunc_0217af90(-2, x, 0x70);
            }
        } else {
            unkfunc_0217af90(-2, 0x10, 0x70);
        }
        drawPageIndicator(0xd0, status::g_Party.getCount() / 5, page, 0);
        drawMenuFrame(0, 0x70, 0x100, 0x30, -1);
    }
}

THUMB void drawCharaItemList(int chara, int target)
{
    if ((target != 0 && TownMenuPlayerControl::getSingleton()->targetFukuro_ != 0)
        || (target == 0 && TownMenuPlayerControl::getSingleton()->activeFukuro_ != 0)) {
        drawBagItemNameList();
    } else {
        drawCharaItemNameList(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_);
    }
}

THUMB void drawItemListPage(int chara, int page, int target)
{
    TownMenuPlayerControl::getSingleton();
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_ == 0) {
        TownMenuPlayerControl::getSingleton();
    }
    if ((target != 0 && TownMenuPlayerControl::getSingleton()->targetFukuro_ != 0)
        || (target == 0 && TownMenuPlayerControl::getSingleton()->activeFukuro_ != 0)) {
        int count = status::FukuroItemInfo::getPageItemCount(page);
        for (int i = 0; i < count; i++) {
            int item = status::FukuroItemInfo::getItemId(i, page);
            int index = status::FukuroItemInfo::getIndexToAll(i, page);
            drawBagItemSlot(item, index, (i % 2 + 1) * 16 + (i % 2) * 0x68 + 4, (i / 2) * 8 + (i / 2) * 0x18 + 4);
        }
        return;
    }
    int count = status::PlayerItemInfo::getItemMaxCount(chara) - page * 6;
    if (count > 6) {
        count = 6;
    }
    for (int i = 0; i < count; i++) {
        if (page == 0) {
            int item = status::PlayerItemInfo::getItemIndex(chara, i);
            drawItemSlot(item, (i % 2 + 1) * 16 + (i % 2) * 0x68 + 4, (i / 2) * 8 + (i / 2) * 0x18 + 4);
            if (status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveItem_.isEquipment(i)) {
                drawItemEquippedMark(i);
            }
        } else {
            int item = status::PlayerItemInfo::getItemIndex(chara, i + 6);
            drawItemSlot(item, (i % 2 + 1) * 16 + (i % 2) * 0x68 + 4, (i / 2) * 8 + (i / 2) * 0x18 + 4);
        }
    }
    if (!data_020ed1bc.isOpen() && target != 0 && count < 6) {
        drawItemSlot(-1, (count % 2 + 1) * 16 + (count % 2) * 0x68 + 4, (count / 2) * 8 + (count / 2) * 0x18 + 4);
    }
}

THUMB void drawPartyHpMpList(int page)
{
    int i;
    int end = (page + 1) * 4;
    if (end >= status::g_Party.getCount()) {
        end = status::g_Party.getCount();
    }
    for (i = page * 4; i < end; i++) {
        drawCharaSummaryPanel(&status::g_Party.getPlayerStatus(i)->haveStatusInfo_, i - page * 4, page);
    }
    if (status::g_Party.getCount() > 4) {
        drawPageIndicatorWide(0x50, (status::g_Party.getCount() - 1) / 4, page);
    }
}

THUMB void drawPartyHpMpListBattle(int page)
{
    status::g_Party.setBattleMode();
    drawPartyHpMpList(page);
    status::g_Party.setPlayerMode();
}

THUMB void drawFieldSpellList(int chara)
{
    int count = status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveAction_.getCount();
    int n = 0;
    for (int i = 0; i < count; i++) {
        int action = status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveAction_.getAction(i);
        if (status::UseAction::isUsuallyUse(action)) {
            int message = getActionNameMessage(action);
            status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(chara)->haveStatusInfo_;
            drawSpellName((n % 2) * 0x78 + 0x1c, (n / 2) * 16 + 0xc + (n / 2) * 8, message, 0, info);
            n++;
        }
    }
}

THUMB void drawPartySummaryStats(int type)
{
    if (status::g_Party.getCount() < 5) {
        if (type != 0) {
            drawPartyHpMpSummary();
        } else {
            drawPartyAttackDefenceSummary();
        }
        drawMenuFrame(0, 0, status::g_Party.getCount() * 64, 0x70, -1);
        return;
    }
    if (type != 0) {
        drawPartyHpMpSummary();
    } else {
        drawPartyAttackDefenceSummary();
    }
    unkfunc_02050698(0, 0xc1);
    drawMenuFrame(0, 0, 0x100, 0x70, -1);
    unkfunc_02050698(0, 0);
    int count = status::g_Party.getCount() - 4;
    if (count > 4) {
        drawMenuFrame(0, 0x60, 0x100, 0x60, -1);
        drawMenuFrame(0, 0, (count - 4) * 64, 0x60, -1);
    } else {
        drawMenuFrame(0, 0x60, count * 64, 0x60, -1);
    }
}

THUMB void drawTacticsCommandList(int mode, char* command, int count)
{
    int tactics[9] = { 0x8000006f, 0x8000006b, 0x80000071, 0x80000070, 0x80000072, 0x80000073, 0x80000082, 0x80000074, 0x80000083 };
    int setting[2] = { 0xa000006e, 0xa000006f };
    if (mode == 1) {
        for (int i = 0; i < 2; i++) {
            drawCommandLabel((i % 2) * 0x50 + 0x10 + (i % 2) * 16, 0x88, setting[i]);
        }
        drawMenuFrame(0, 0x80, 0xd8, 0x20, -1);
        return;
    }
    for (int i = 0; i < count; i++) {
        drawCommandLabel((i % 2) * 0x50 + 0x20 + (i % 2) * 16, (i / 2) * 16 + 0x24 + (i / 2) * 8, tactics[command[i]]);
    }
    drawMenuFrame(0x10, 0x18, 0xd0, ((count - 1) / 2) * 0x18 + 0x28, -1);
}

THUMB void drawPartyOrderIcons(char* order, int mode)
{
    status::g_Party.setBattleMode();
    int carriageOut = status::g_Party.getCarriageOutCount();
    int count = status::g_Party.getCount();
    if (mode != 2 && (status::g_Party.getSortIndex(1) != -1 || status::g_Party.getSortIndex(2) != -1)) {
        count--;
    }
    for (int index = 0; index < status::g_Party.getCount(); index++) {
        status::PlayerStatus* player = status::g_Party.getPlayerStatus(index);
        if (player->haveStatusInfo_.haveStatus_.isPlayer() == false) {
            count--;
        }
    }
    int y = 0x48;
    int i = 0;
    if (count < 5) {
        y = 0x70;
    }
    if (count < 5) {
        drawMenuFrame(0, 0x70, 0xe0, 0x30, -1);
    } else {
        drawMenuFrame(0, 0x48, 0xe0, 0x58, -1);
    }
    for (; order[i] != -1 && i < 5; i++) {
        drawPartyMemberIcon(&status::g_Party.getPlayerStatus(order[i])->haveStatusInfo_, i * 32 + 16 + i * 8, y);
    }
    int n = 0;
    for (int j = 0; j < carriageOut; j++) {
        if (mode != 2) {
            if (status::g_Party.getPlayerStatus(j)->haveStatusInfo_.haveStatus_.playerIndex_ == 1) {
                continue;
            }
            if (status::g_Party.getPlayerStatus(j)->haveStatusInfo_.haveStatus_.playerIndex_ == 2) {
                continue;
            }
        }
        status::PlayerStatus* player = status::g_Party.getPlayerStatus(j);
        if (player->haveStatusInfo_.haveStatus_.isPlayer() != false) {
            int x = n * 32 + 16 + n * 8;
            drawCarriageOrderNumber(j + 1, x - 8, y + 8, order[n]);
            n++;
        }
    }
    if (order[i] == -1 || i < 5) {
        int x = i * 32 + 16 + i * 8;
        if (i == 5) {
            x = 16;
        }
        unkfunc_0217af90(-2, x, 0x70);
        return;
    }
    for (; order[i] != -1 && i < 9; i++) {
        drawPartyMemberIcon(&status::g_Party.getPlayerStatus(order[i])->haveStatusInfo_, (i - 5) * 32 + 16 + (i - 5) * 8, 0x70);
    }
    unkfunc_0217af90(-2, (i - 5) * 32 + 16 + (i - 5) * 8, 0x70);
}

THUMB void drawPartyTacticsList(char* order)
{
    int i = 0;
    drawMenuHeaderText(0, 0xe, 0x80000091);
    for (; order[i] != -1; i++) {
        drawCharaTacticsRow(&status::g_Party.getPlayerStatus(order[i])->haveStatusInfo_, 0, i * 16 + 0x30);
    }
    drawMenuFrame(0, 0, 0x100, 0xc0, 0x20);
}

THUMB void drawEquipItemList(int chara, unsigned char* list, int count, int itemType, int page)
{
    if (itemType == -1) {
        for (int i = 0; i < count; i++) {
            if (status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveItem_.isEquipment(list[i])) {
                int item = status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveItem_.getItem(list[i]);
                drawItemSlot(item, (i % 2 + 1) * 16 + 4 + (i % 2) * 0x68, (i / 2) * 8 + (i / 2) * 0x18 + 4);
                drawItemEquippedMark(i);
            }
        }
        return;
    }
    if (page == 0) {
        if (count > 6) {
            count = 6;
        }
        for (int i = 0; i < count; i++) {
            drawItemSlot(list[i], (i % 2 + 1) * 16 + 4 + (i % 2) * 0x68, (i / 2) * 8 + (i / 2) * 0x18 + 4);
        }
        if (count > 0 && status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveEquipment_.isEquipment(list[0])) {
            drawItemEquippedMark(0);
        }
        return;
    }
    for (int i = 6; i < count; i++) {
        drawItemSlot(list[i], ((i - 6) % 2 + 1) * 16 + 4 + ((i - 6) % 2) * 0x68, ((i - 6) / 2) * 8 + ((i - 6) / 2) * 0x18 + 4);
    }
}

THUMB void drawEquipItemInfo(int chara, int index, int itemType)
{
    if (itemType != -1) {
        if (index == -1) {
            drawEquipCompareWindow(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, -1, itemType, 0);
        } else {
            int item = status::PlayerItemInfo::getItemIndex(chara, index);
            drawEquipCompareWindow(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, item, itemType, 0);
        }
    }
}

THUMB void drawRootMenuButtonFrames(int x, int y, int flag)
{
    if (flag != 0) {
        for (int i = 0; i < 6; i++) {
            drawMenuFrame((i % 2) * 0x48 + 0x40, (i / 2) * 32 + 0x60, 0x48, 0x20, -1);
        }
        return;
    }
    drawMenuFrame(x, y, 0x48, 0x20, -1);
}

THUMB void drawSmallButtonFrame(int x, int y)
{
    drawMenuFrame(x, y, 0x40, 0x20, -1);
}
