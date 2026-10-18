#pragma ipa file
#include "ov016/UnkMaterielMenuDraw/MaterielMenuScreens.hpp"
#include "ov016/UnkMaterielMenuDraw/MaterielMenuDraw.hpp"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MaterielMenuPlayerControl.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/status/UseItem.hpp"
#include "main/status/ShopList.hpp"
#include "ov028/MaterielMenu_SHOP/MaterielMenu_SHOP.hpp"
#include "main/menu/MenuPartsDraw.hpp"
#include "main/menu/MenuCommonDraw.hpp"

static UnkMenuParts s_partsSlotCoins[] = {
    { 0x01, 0x00, (short)0xf000, 0, 0xc0, 0, 0x40, 0x30 },
    { 0x0d, 0x08, (short)0xf000, 0, 0xc8, 0xa, 0x30, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 1, 0xc8, 0x1a, 0x30, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};

// not in the ROM (dead-stripped), keeps the const locals of this file
THUMB void unkfunc_unused_9()
{
    const int unk0 = 0;
    const int unk1 = 0;
    const int unk2 = 1;
}

THUMB void drawShopRootScreen()
{
    drawShopRootCommands(false, 0);
    drawMoneyAndPartyWindows();
}

THUMB void drawShopBuyScreen(int* fukuroItemCount, int flag)
{
    drawShopBuyList(flag);
    drawShopItemKind();
    drawMoneyWindow(0x88, 0xa0, flag);
    MaterielMenuPlayerControl* ctrl = MaterielMenuPlayerControl::getSingleton();
    int item = MaterielMenu_SHOP_MANAGER::getSingleton()->getItem(ctrl->activeItem_);
    drawShopSackCount(fukuroItemCount);
    if (status::UseItem::getItemType(item) <= 4) {
        drawShopEquipCompare();
    } else {
        drawShopHeldCounts();
    }
}

THUMB void drawShopWhoseScreen(int flag)
{
    drawShopGiveCompare();
    drawPartyIcons(0, 1);
    drawActiveCharaHp(0);
    drawMoneyWindow(0x88, 0xa0, flag);
    unkfunc_0201e234();
    drawCharaItemNames();
}

THUMB void drawShopWhoSellScreen()
{
    drawPartyIcons(0, 1);
    drawActiveCharaHp(0);
    drawMoneyWindow(0x88, 0xa0, 0);
    unkfunc_0201e234();
    drawSellPriceList();
}

THUMB void drawShopSellItemScreen()
{
    drawActiveCharaHp(0);
    drawMoneyWindow(0x88, 0xa0, 0);
    unkfunc_0201e234();
    drawSellPrice(0);
    drawItemPageNumber(0, -1, 0, 0);
    drawItemPage(0);
    drawItemDetail(-1);
}

THUMB void drawShopSellSackScreen()
{
    drawActiveCharaHp(0);
    drawMoneyWindow(0x88, 0xa0, 0);
    unkfunc_0201e234();
    drawSellPrice(1);
    drawItemPageNumber(1, -1, 0, 0);
    drawItemPage(1);
    drawItemDetail(-1);
}

THUMB void drawShopSellQuantityScreen(int quantity)
{
    drawSellQuantity(quantity);
    drawShopRootCommands(false, 0);
    drawCommandCursor(0xa4, 0x28);
    drawItemDetail(-1);
    drawMoneyWindow(-1, -1, 0);
}

THUMB void drawShopScreen()
{
    switch (MaterielMenu_SHOP_MANAGER::getSingleton()->getShopAction()) {
    case 0:
        drawCommandCursor(0xa4, 0x10);
        break;
    case 1:
        drawCommandCursor(0xa4, 0x28);
        break;
    case 2:
        drawCommandCursor(0xa4, 0x40);
        break;
    }
    drawShopRootScreen();
}

THUMB void drawChurchScreen(int activeCommand, int firstFlag)
{
    if (activeCommand == -1 && g_TownMenuMessage.isMessageWAITPROG() && firstFlag == 0) {
        drawChurchCommands();
    }
    drawMoneyAndPartyWindows();
}

THUMB void drawChurchMiracleScreen(int count)
{
    int chara = MaterielMenuPlayerControl::getSingleton()->activeChara_;
    drawPartyIcons(0x28, 0);
    drawActiveCharaHp(1);
    drawCharaStatusWindow(chara, 1);
    drawMoneyWindow(-1, -1, 0);
}

THUMB void drawChangeGiftScreen()
{
    drawPartyStatusWindows();
}

THUMB void drawBankRootScreen()
{
    drawShopRootCommands(true, 0);
    drawMoneyAndPartyWindows();
}

THUMB void drawBankAmountScreen(int a, int b, int money)
{
    if (a == 0) {
        if (b) {
            drawCommandCursor(0xa4, 0x10);
        } else {
            drawCommandCursor(0xa4, 0x28);
        }
        drawShopRootCommands(true, 1);
        drawBankDigits(money, 1);
    }
    drawMoneyAndPartyWindows();
}

THUMB void drawCoinSaleroomScreen(int coin, int flag)
{
    if (flag) {
        drawBankDigits(coin, 0);
    }
    drawMoneyAndPartyWindows();
}

THUMB void drawPokerHighLowScreen(int coin, int flag)
{
    drawPokerCoins(coin, flag);
    drawHighLowCommands(flag);
}

THUMB void drawSlotCoins(int coin, int flag)
{
    int param[2] = { (int)0xa0000138, coin };
    if (coin > 999999) {
        coin = 999999;
    }
    param[1] = coin;
    if (flag) {
        s_partsSlotCoins[2].type_ = 0;
    } else {
        s_partsSlotCoins[2].type_ = 0xf;
    }
    drawMenuPartsColored(s_partsSlotCoins, param, 1);
}

THUMB void drawMonsterBookScreen(int* monsterName, int* monsterFlag)
{
    drawMonsterBookPage(monsterName, monsterFlag);
}

THUMB void drawMonsterBookDetailScreen(int monsterNo, int monsterName)
{
    drawMonsterBookDetail(monsterNo, monsterName);
}

THUMB void drawPresentExpScreen(int activeChara, int extraExp)
{
    drawCharaStatusList(activeChara);
}

THUMB void drawHostageScreen()
{
    drawPartyIcons(0x28, 0);
}

THUMB void drawColosseumScreen()
{
    drawActiveCharaHpMp();
}

THUMB void drawNeneSellScreen()
{
    int flag = 0;
    if (MaterielMenuPlayerControl::getSingleton()->activeChara_ == 1) {
        flag = 1;
    }
    drawActiveCharaHp(0);
    drawMoneyWindow(0x88, 0xa0, 0);
    drawSellPrice(flag);
    drawItemPageNumber(0, -1, 0, 0);
    drawItemPage(flag);
    drawItemDetail(-1);
}

THUMB void drawNeneStorageScreen(int count, int page, int pageMax)
{
    MaterielMenuPlayerControl* ctrl = MaterielMenuPlayerControl::getSingleton();
    int item = status::g_Shop.haveItemNene_.getItem(ctrl->activeItem_ + page * 6);
    drawNeneItemPage(count, page, pageMax);
    drawItemPageNumber(1, count, page, pageMax);
    drawItemDetail(item);
}

THUMB void drawChapterTitleScreen(int chapter, int chapterEnd)
{
    drawChapterTitleText(chapter, chapterEnd);
}

THUMB void drawTagModeRootScreen()
{
    drawTagModeRootMenu();
}

THUMB void drawTagModeTenantScreen(int count, int page, int pageMax, int index, int flag, int select)
{
    drawTagModeTenantList(count, page, pageMax, flag);
    if (count > 0) {
        drawTagModeTenantDetail(index, 0, select);
    }
}

THUMB void drawTagModeTenantListScreen(int mode, int count, int page, int pageMax, int index)
{
    if (mode == 0) {
        drawTagModeTenantList(count, page, pageMax, 0);
        drawTagModeTenantDetail(index, 0, 0);
    }
}

THUMB void drawTagModeAmbassadorScreen(int mode, int active, int page, int value)
{
    if (mode == 0) {
        drawTagModeIconGrid(page, value);
    } else {
        drawNameEditIcon(active, 0x48);
    }
}

THUMB void drawNameEditScreen(int mode, int active)
{
    if (mode < 4) {
        drawNameEditIcon(active, 0);
    }
    switch (mode) {
    case 1:
        drawNameEditSex();
        break;
    case 2:
        drawNameEditAge();
        break;
    case 3:
        drawNameEditSkill();
        break;
    case 4:
        drawTagModeProfile(active, 1);
        break;
    case 6:
        drawNameEditComment();
        break;
    case 8:
        break;
    }
}

THUMB void drawSideJobScreen()
{
    drawSideJobItems();
}

THUMB void drawMoneyAndPartyWindows()
{
    drawMoneyWindow(-1, -1, 0);
    drawPartyStatusWindows();
}
