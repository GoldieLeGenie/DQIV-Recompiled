#pragma ipa file
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216ce4c.hpp"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MaterielMenuPlayerControl.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/status/UseItem.hpp"
#include "main/status/ShopList.hpp"
#include "ov028/MaterielMenu_SHOP/MaterielMenu_SHOP.hpp"
#include "main/menu/UnkMenuPartsDraw.hpp"
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
    unkfunc_0216ce4c(false, 0);
    unkfunc_0216ff18();
}

THUMB void unkfunc_0216fb24(int* fukuroItemCount, int flag)
{
    unkfunc_0216d154(flag);
    unkfunc_0216d0d4();
    unkfunc_0201e350(0x88, 0xa0, flag);
    MaterielMenuPlayerControl* ctrl = MaterielMenuPlayerControl::getSingleton();
    int item = MaterielMenu_SHOP_MANAGER::getSingleton()->getItem(ctrl->activeItem_);
    unkfunc_0216d058(fukuroItemCount);
    if (status::UseItem::getItemType(item) <= 4) {
        unkfunc_0216ced4();
    } else {
        unkfunc_0216cfac();
    }
}

THUMB void unkfunc_0216fb6c(int flag)
{
    unkfunc_0216d398();
    unkfunc_0216d554(0, 1);
    unkfunc_0216e1c4(0);
    unkfunc_0201e350(0x88, 0xa0, flag);
    unkfunc_0201e234();
    unkfunc_0216d26c();
}

THUMB void unkfunc_0216fb98()
{
    unkfunc_0216d554(0, 1);
    unkfunc_0216e1c4(0);
    unkfunc_0201e350(0x88, 0xa0, 0);
    unkfunc_0201e234();
    unkfunc_0216d798();
}

THUMB void unkfunc_0216fbbc()
{
    unkfunc_0216e1c4(0);
    unkfunc_0201e350(0x88, 0xa0, 0);
    unkfunc_0201e234();
    unkfunc_0216da80(0);
    unkfunc_0216db94(0, -1, 0, 0);
    unkfunc_0216d8e4(0);
    unkfunc_0216dc54(-1);
}

THUMB void unkfunc_0216fbf4()
{
    unkfunc_0216e1c4(0);
    unkfunc_0201e350(0x88, 0xa0, 0);
    unkfunc_0201e234();
    unkfunc_0216da80(1);
    unkfunc_0216db94(1, -1, 0, 0);
    unkfunc_0216d8e4(1);
    unkfunc_0216dc54(-1);
}

THUMB void unkfunc_0216fc2c(int quantity)
{
    unkfunc_0216ddf4(quantity);
    unkfunc_0216ce4c(false, 0);
    unkfunc_0216e2bc(0xa4, 0x28);
    unkfunc_0216dc54(-1);
    unkfunc_0201e350(-1, -1, 0);
}

THUMB void unkfunc_0216fc58()
{
    switch (MaterielMenu_SHOP_MANAGER::getSingleton()->getShopAction()) {
    case 0:
        unkfunc_0216e2bc(0xa4, 0x10);
        break;
    case 1:
        unkfunc_0216e2bc(0xa4, 0x28);
        break;
    case 2:
        unkfunc_0216e2bc(0xa4, 0x40);
        break;
    }
    unkfunc_0216fb14();
}

THUMB void unkfunc_0216fc94(int activeCommand, int firstFlag)
{
    if (activeCommand == -1 && data_020ed1bc.isMessageWAITPROG() && firstFlag == 0) {
        unkfunc_0216e0bc();
    }
    unkfunc_0216ff18();
}

THUMB void unkfunc_0216fcbc(int count)
{
    int chara = MaterielMenuPlayerControl::getSingleton()->activeChara_;
    unkfunc_0216d554(0x28, 0);
    unkfunc_0216e1c4(1);
    unkfunc_0201e3f4(chara, 1);
    unkfunc_0201e350(-1, -1, 0);
}

THUMB void unkfunc_0216fce8()
{
    unkfunc_0201e260();
}

THUMB void unkfunc_0216fcf0()
{
    unkfunc_0216ce4c(true, 0);
    unkfunc_0216ff18();
}

THUMB void unkfunc_0216fd00(int a, int b, int money)
{
    if (a == 0) {
        if (b) {
            unkfunc_0216e2bc(0xa4, 0x10);
        } else {
            unkfunc_0216e2bc(0xa4, 0x28);
        }
        unkfunc_0216ce4c(true, 1);
        unkfunc_0216e12c(money, 1);
    }
    unkfunc_0216ff18();
}

THUMB void unkfunc_0216fd34(int coin, int flag)
{
    if (flag) {
        unkfunc_0216e12c(coin, 0);
    }
    unkfunc_0216ff18();
}

THUMB void unkfunc_0216fd48(int coin, int flag)
{
    unkfunc_0216de34(coin, flag);
    unkfunc_0216dea8(flag);
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
    unkfunc_02050ed0(s_parts_02181fd0, param, 1);
}

THUMB void unkfunc_0216fda0(int* monsterName, int* monsterFlag)
{
    unkfunc_0216decc(monsterName, monsterFlag);
}

THUMB void unkfunc_0216fda8(int monsterNo, int monsterName)
{
    unkfunc_0216df9c(monsterNo, monsterName);
}

THUMB void unkfunc_0216fdb0(int activeChara, int extraExp)
{
    unkfunc_0216e2d0(activeChara);
}

THUMB void unkfunc_0216fdb8()
{
    unkfunc_0216d554(0x28, 0);
}

THUMB void unkfunc_0216fdc4()
{
    unkfunc_0216e844();
}

THUMB void unkfunc_0216fdcc()
{
    int flag = 0;
    if (MaterielMenuPlayerControl::getSingleton()->activeChara_ == 1) {
        flag = 1;
    }
    unkfunc_0216e1c4(0);
    unkfunc_0201e350(0x88, 0xa0, 0);
    unkfunc_0216da80(flag);
    unkfunc_0216db94(0, -1, 0, 0);
    unkfunc_0216d8e4(flag);
    unkfunc_0216dc54(-1);
}

THUMB void unkfunc_0216fe10(int count, int page, int pageMax)
{
    MaterielMenuPlayerControl* ctrl = MaterielMenuPlayerControl::getSingleton();
    int item = status::g_Shop.haveItemNene_.getItem(ctrl->activeItem_ + page * 6);
    unkfunc_0216e944(count, page, pageMax);
    unkfunc_0216db94(1, count, page, pageMax);
    unkfunc_0216dc54(item);
}

THUMB void unkfunc_0216fe50(int chapter, int chapterEnd)
{
    unkfunc_0216e9fc(chapter, chapterEnd);
}

THUMB void unkfunc_0216fe58()
{
    unkfunc_0216eac0();
}

THUMB void unkfunc_0216fe60(int count, int page, int pageMax, int index, int flag, int select)
{
    unkfunc_0216eb18(count, page, pageMax, flag);
    if (count > 0) {
        unkfunc_0216edf8(index, 0, select);
    }
}

THUMB void unkfunc_0216fe7c(int mode, int count, int page, int pageMax, int index)
{
    if (mode == 0) {
        unkfunc_0216eb18(count, page, pageMax, 0);
        unkfunc_0216edf8(index, 0, 0);
    }
}

THUMB void unkfunc_0216fe9c(int mode, int active, int page, int value)
{
    if (mode == 0) {
        unkfunc_0216f0c0(page, value);
    } else {
        unkfunc_0216f1b0(active, 0x48);
    }
}

THUMB void unkfunc_0216feb8(int mode, int active)
{
    if (mode < 4) {
        unkfunc_0216f1b0(active, 0);
    }
    switch (mode) {
    case 1:
        unkfunc_0216f1e0();
        break;
    case 2:
        unkfunc_0216f258();
        break;
    case 3:
        unkfunc_0216f318();
        break;
    case 4:
        unkfunc_0216efdc(active, 1);
        break;
    case 6:
        unkfunc_0216f450();
        break;
    case 8:
        break;
    }
}

THUMB void unkfunc_0216ff10()
{
    unkfunc_0216f4bc();
}

THUMB void unkfunc_0216ff18()
{
    unkfunc_0201e350(-1, -1, 0);
    unkfunc_0201e260();
}
