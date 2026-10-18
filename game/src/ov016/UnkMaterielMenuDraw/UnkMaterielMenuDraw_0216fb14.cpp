#pragma ipa file
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"
#include "ov016/UnkMaterielMenuDraw/MaterielMenuDraw.hpp"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MaterielMenuPlayerControl.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/status/UseItem.hpp"
#include "main/status/ShopList.hpp"
#include "ov028/MaterielMenu_SHOP/MaterielMenu_SHOP.hpp"
#include "main/menu/MenuPartsDraw.hpp"
#include "main/menu/UnkMenuCommonDraw_0201e194.hpp"

static UnkMenuParts s_parts_02181fd0[] = {
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

THUMB void unkfunc_0216fb14()
{
    drawShopRootCommands(false, 0);
    unkfunc_0216ff18();
}

THUMB void unkfunc_0216fb24(int* fukuroItemCount, int flag)
{
    drawShopBuyList(flag);
    drawShopItemKind();
    unkfunc_0201e350(0x88, 0xa0, flag);
    MaterielMenuPlayerControl* ctrl = MaterielMenuPlayerControl::getSingleton();
    int item = MaterielMenu_SHOP_MANAGER::getSingleton()->getItem(ctrl->activeItem_);
    drawShopSackCount(fukuroItemCount);
    if (status::UseItem::getItemType(item) <= 4) {
        drawShopEquipCompare();
    } else {
        drawShopHeldCounts();
    }
}

THUMB void unkfunc_0216fb6c(int flag)
{
    drawShopGiveCompare();
    drawPartyIcons(0, 1);
    drawActiveCharaHp(0);
    unkfunc_0201e350(0x88, 0xa0, flag);
    unkfunc_0201e234();
    drawCharaItemNames();
}

THUMB void unkfunc_0216fb98()
{
    drawPartyIcons(0, 1);
    drawActiveCharaHp(0);
    unkfunc_0201e350(0x88, 0xa0, 0);
    unkfunc_0201e234();
    drawSellPriceList();
}

THUMB void unkfunc_0216fbbc()
{
    drawActiveCharaHp(0);
    unkfunc_0201e350(0x88, 0xa0, 0);
    unkfunc_0201e234();
    drawSellPrice(0);
    drawItemPageNumber(0, -1, 0, 0);
    drawItemPage(0);
    drawItemDetail(-1);
}

THUMB void unkfunc_0216fbf4()
{
    drawActiveCharaHp(0);
    unkfunc_0201e350(0x88, 0xa0, 0);
    unkfunc_0201e234();
    drawSellPrice(1);
    drawItemPageNumber(1, -1, 0, 0);
    drawItemPage(1);
    drawItemDetail(-1);
}

THUMB void unkfunc_0216fc2c(int quantity)
{
    drawSellQuantity(quantity);
    drawShopRootCommands(false, 0);
    drawCommandCursor(0xa4, 0x28);
    drawItemDetail(-1);
    unkfunc_0201e350(-1, -1, 0);
}

THUMB void unkfunc_0216fc58()
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
    unkfunc_0216fb14();
}

THUMB void unkfunc_0216fc94(int activeCommand, int firstFlag)
{
    if (activeCommand == -1 && data_020ed1bc.isMessageWAITPROG() && firstFlag == 0) {
        drawChurchCommands();
    }
    unkfunc_0216ff18();
}

THUMB void unkfunc_0216fcbc(int count)
{
    int chara = MaterielMenuPlayerControl::getSingleton()->activeChara_;
    drawPartyIcons(0x28, 0);
    drawActiveCharaHp(1);
    unkfunc_0201e3f4(chara, 1);
    unkfunc_0201e350(-1, -1, 0);
}

THUMB void unkfunc_0216fce8()
{
    unkfunc_0201e260();
}

THUMB void unkfunc_0216fcf0()
{
    drawShopRootCommands(true, 0);
    unkfunc_0216ff18();
}

THUMB void unkfunc_0216fd00(int a, int b, int money)
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
    unkfunc_0216ff18();
}

THUMB void unkfunc_0216fd34(int coin, int flag)
{
    if (flag) {
        drawBankDigits(coin, 0);
    }
    unkfunc_0216ff18();
}

THUMB void unkfunc_0216fd48(int coin, int flag)
{
    drawPokerCoins(coin, flag);
    drawHighLowCommands(flag);
}

THUMB void unkfunc_0216fd58(int coin, int flag)
{
    int param[2] = { (int)0xa0000138, coin };
    if (coin > 999999) {
        coin = 999999;
    }
    param[1] = coin;
    if (flag) {
        s_parts_02181fd0[2].type_ = 0;
    } else {
        s_parts_02181fd0[2].type_ = 0xf;
    }
    drawMenuPartsColored(s_parts_02181fd0, param, 1);
}

THUMB void unkfunc_0216fda0(int* monsterName, int* monsterFlag)
{
    drawMonsterBookPage(monsterName, monsterFlag);
}

THUMB void unkfunc_0216fda8(int monsterNo, int monsterName)
{
    drawMonsterBookDetail(monsterNo, monsterName);
}

THUMB void unkfunc_0216fdb0(int activeChara, int extraExp)
{
    drawCharaStatusList(activeChara);
}

THUMB void unkfunc_0216fdb8()
{
    drawPartyIcons(0x28, 0);
}

THUMB void unkfunc_0216fdc4()
{
    drawActiveCharaHpMp();
}

THUMB void unkfunc_0216fdcc()
{
    int flag = 0;
    if (MaterielMenuPlayerControl::getSingleton()->activeChara_ == 1) {
        flag = 1;
    }
    drawActiveCharaHp(0);
    unkfunc_0201e350(0x88, 0xa0, 0);
    drawSellPrice(flag);
    drawItemPageNumber(0, -1, 0, 0);
    drawItemPage(flag);
    drawItemDetail(-1);
}

THUMB void unkfunc_0216fe10(int count, int page, int pageMax)
{
    MaterielMenuPlayerControl* ctrl = MaterielMenuPlayerControl::getSingleton();
    int item = status::g_Shop.haveItemNene_.getItem(ctrl->activeItem_ + page * 6);
    drawNeneItemPage(count, page, pageMax);
    drawItemPageNumber(1, count, page, pageMax);
    drawItemDetail(item);
}

THUMB void unkfunc_0216fe50(int chapter, int chapterEnd)
{
    drawChapterTitleText(chapter, chapterEnd);
}

THUMB void unkfunc_0216fe58()
{
    drawTagModeRootMenu();
}

THUMB void unkfunc_0216fe60(int count, int page, int pageMax, int index, int flag, int select)
{
    drawTagModeTenantList(count, page, pageMax, flag);
    if (count > 0) {
        drawTagModeTenantDetail(index, 0, select);
    }
}

THUMB void unkfunc_0216fe7c(int mode, int count, int page, int pageMax, int index)
{
    if (mode == 0) {
        drawTagModeTenantList(count, page, pageMax, 0);
        drawTagModeTenantDetail(index, 0, 0);
    }
}

THUMB void unkfunc_0216fe9c(int mode, int active, int page, int value)
{
    if (mode == 0) {
        drawTagModeIconGrid(page, value);
    } else {
        drawNameEditIcon(active, 0x48);
    }
}

THUMB void unkfunc_0216feb8(int mode, int active)
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

THUMB void unkfunc_0216ff10()
{
    drawSideJobItems();
}

THUMB void unkfunc_0216ff18()
{
    unkfunc_0201e350(-1, -1, 0);
    unkfunc_0201e260();
}
