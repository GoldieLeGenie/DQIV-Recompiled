#pragma ipa file
#include "ov016/UnkTownMenuDraw/UnkTownMenuDraw_02173d7c.hpp"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/status/ShopList.hpp"
#include "main/status/UseItem.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/menu/UnkMenuPartsDraw.hpp"

// not in the ROM (dead-stripped), its parts list and const locals are still in .data/.rodata
THUMB void unkfunc_unused(int active)
{
    const int unk0 = 0;
    const int unk1 = 1;
    const int unk2 = 5;
    const int unk3 = 1;
    const int unk4 = 4;
    const int unk5 = 2;
    const int unk6 = 3;
    const int unk7 = 2;
    static UnkMenuParts parts[] = {
        { 0x14, 0xc0, 0, -1, 0x58, 0x58, 0x48, 0x10 },
        { 0x0d, 0x09, (short)0xf000, 0, 0x5a, 0x5a, 0x42, 0xa },
        { 0x14, 0xc0, 0, -1, 0xb0, 0x58, 0x48, 0x10 },
        { 0x0d, 0x09, (short)0xf000, 1, 0xb2, 0x5a, 0x42, 0xa },
        { 0x14, 0xc0, 0, -1, 0x58, 0x70, 0x48, 0x10 },
        { 0x0d, 0x09, (short)0xf000, 2, 0x5a, 0x72, 0x42, 0xa },
        { 0x14, 0xc0, 0, -1, 0xb0, 0x70, 0x48, 0x10 },
        { 0x0d, 0x09, (short)0xf000, 3, 0xb2, 0x72, 0x42, 0xa },
        { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
    };
    unkfunc_02050ed0(parts, &active, 1);
}

THUMB void unkfunc_02173d7c()
{
    static UnkMenuParts parts[] = {
        { 0x0d, 0x08, (short)0xf000, 0, 0x10, 0x51, 0x38, 0x20 },
        { 0x0d, 0x08, (short)0xf000, 1, 0x10, 0x6e, 0x38, 0x10 },
        { 0x0d, 0x08, (short)0xf000, 2, 0x10, 0x8a, 0x50, 0x10 },
        { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
    };
    int param[3];
    param[0] = 0xa000006a;
    param[1] = 0xa000006b;
    param[2] = 0xa000006d;
    unkfunc_02050ed0(parts, param, 1);
}

THUMB void unkfunc_02173da4(int x, int y)
{
    static UnkMenuParts parts[] = {
        { 0x0d, 0x09, (short)0xf000, 0, 2, 2, 0xa, 0xa },
        { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
    };
    const char* number[5] = { "1", "2", "3", "4", "5" };
    int param;
    for (int i = 0; i < 5; i++) {
        param = (int)number[i];
        unkfunc_02050ee0(parts, &param, x + i * 32, y, 1);
    }
}

THUMB void unkfunc_02173dec(int value, int index, int bookOfBeasts, int heroLevel)
{
    int param[3];
    int name[9] = { 0xa0000191, 0xa0000192, 0xa0000193, 0xa0000194, 0xa0000195, 0xa0000196, 0xa0000197, 0xa000019c, 0xa0000198 };
    int unit[9] = { 0xa000019d, 0xa000019e, 0xa000003c, 0xa000019d, 0xa000019d, 0xa000019d, 0xa00001a0, 0xa000003f, 0xa000019e };
    static UnkMenuParts parts[] = {
        { 0x0d, 0x08, (short)0xf000, 0, 0x18, 8, 0x68, 0xc },
        { 0x0f, 0x0a, (short)0xf000, 1, 0x78, 8, 0x60, 0xc },
        { 0x0d, 0x0a, (short)0xf000, 2, 0xd8, 8, 0x10, 0xc },
        { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
    };
    if (index == 7 && heroLevel == 1) {
        param[0] = name[7];
        param[1] = value;
        param[2] = unit[7];
    } else if (index == 7 && bookOfBeasts == 1) {
        param[0] = name[8];
        param[1] = value;
        param[2] = unit[8];
    } else {
        param[0] = name[index];
        param[1] = value;
        param[2] = unit[index];
    }
    unkfunc_02050ee0(parts, param, 0, (index + 1) * 14, 1);
}

THUMB void unkfunc_02173e7c(int time, int type)
{
    static UnkMenuParts parts[] = {
        { 0x0d, 0x08, (short)0xf000, 0, 0x18, 8, 0x78, 0x10 },
        { 0x0f, 0x0a, (short)0xf000, 1, 0x98, 8, 8, 0x10 },
        { 0x0f, 0x0a, (short)0xf000, 2, 0xa0, 8, 8, 0x10 },
        { 0x0f, 0x0a, (short)0xf000, 3, 0xa8, 8, 8, 0x10 },
        { 0x0d, 0x0a, (short)0xf000, 4, 0xae, 8, 0x18, 0x10 },
        { 0x0f, 0x0a, (short)0xf000, 5, 0xc8, 8, 8, 0x10 },
        { 0x0f, 0x0a, (short)0xf000, 6, 0xd0, 8, 8, 0x10 },
        { 0x0d, 0x0a, (short)0xf000, 7, 0xd8, 8, 0x10, 0x10 },
        { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
    };
    int hour = time / 216000;
    int minute = time % 216000 / 3600;
    int param[8];
    param[0] = 0xa000002d;
    param[1] = hour / 100;
    param[2] = hour / 10 % 10;
    param[3] = hour % 10;
    param[4] = 0xa000002e;
    param[5] = minute / 10;
    param[6] = minute % 10;
    param[7] = 0xa000002f;
    if (type == 1) {
        param[0] = 0xa000019a;
    }
    if (type == 2) {
        param[0] = 0xa000019b;
    }
    unkfunc_02050ee0(parts, param, 0, 0, 1);
}

THUMB void unkfunc_02173f24(int title)
{
    static UnkMenuParts parts[] = {
        { 0x0d, 0x08, (short)0xf000, 0, 0x18, 0x90, 0x20, 0xc },
        { 0x0d, 0x05, (short)0xf000, 1, 0x38, 0x90, 0xa0, 0x18 },
        { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
    };
    int param[2];
    param[0] = 0xa0000199;
    param[1] = title - 0x20000000;
    unkfunc_02050ed0(parts, param, 1);
}

THUMB void unkfunc_02173f4c(int type)
{
    static UnkMenuParts parts[] = {
        { 0x0d, 0x08, (short)0xf000, 0, 0x18, 0xa8, 0x78, 0xc },
        { 0x0c, 0xf3, 0, 1, 0xb0, 0x98, 0x20, 0x20 },
        { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
    };
    int param[2];
    if (type != 0) {
        param[0] = 0xa000019b;
    } else {
        param[0] = 0xa000019a;
    }
    param[1] = 0x11;
    unkfunc_02050ed0(parts, param, 1);
}

THUMB void unkfunc_02173f7c(int index)
{
    unkfunc_02050698(0, 0);
    int msg[3] = { 0x80000084, 0x80000085, 0x80000086 };
    static UnkMenuParts titleParts[] = {
        { 0x0d, 0x09, (short)0xf000, 0, 8, 9, 0x98, 0xe },
        { 0x01, 0x00, (short)0xf000, 0, 0, 0, 0xa8, 0x88 },
        { 0x16, 0x00, (short)0xf000, 0, 0, 0x18, 0xa8, 0 },
        { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
    };
    static UnkMenuParts itemParts[] = {
        { 0x0d, 0x08, (short)0xf000, 0, 0xc, 0x22, 0x68, 0xe },
        { 0x0f, 0x0a, (short)0xf000, 1, 0x78, 0x22, 0x28, 0xe },
        { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
    };
    int title;
    if (index > 2) {
        title = msg[index - 3];
    } else {
        title = msg[index];
    }
    unkfunc_02050ee0(titleParts, &title, 0x30, 0x20, 1);
    int param[2];
    int item[7] = { 0 };
    int num = 0;
    int count = 0;
    int skip = 0;
    int price[2] = { 0 };
    switch (index) {
        case 0: {
            char name[3] = { 0 };
            name[0] = g_Stage.getMapName()[0];
            name[1] = g_Stage.getMapName()[1];
            num = status::g_Shop.getShopCount(2);
            for (int i = 0; i < num; i++) {
                if (dss::strcmp(name, "mf") == 0 && status::g_Story.chapter_ == 3 && i > 2) {
                    if (status::g_Shop.sideJobItemFlag_[i] == 1) {
                        item[count] = status::g_Shop.getShopItem(2, i);
                        count++;
                    } else {
                        skip++;
                    }
                } else {
                    item[i] = status::g_Shop.getShopItem(2, i);
                    count++;
                }
            }
            num -= skip;
            break;
        }
        case 1:
            num = status::g_Shop.getShopCount(3);
            for (int i = 0; i < num; i++) {
                item[i] = status::g_Shop.getShopItem(3, i);
            }
            break;
        case 2: {
            char name[3] = { 0 };
            name[0] = g_Stage.getMapName()[0];
            name[1] = g_Stage.getMapName()[1];
            num = status::g_Shop.getShopCount(4);
            if (dss::strcmp(name, "mg") == 0) {
                item[0] = 0x6f;
                item[1] = 7;
                price[0] = 8;
                price[1] = 10;
            } else {
                for (int i = 0; i < num; i++) {
                    item[i] = status::g_Shop.getShopItem(4, i);
                }
            }
            break;
        }
        case 3:
            num = status::g_Shop.getShopCount(8);
            for (int i = 0; i < num; i++) {
                item[i] = status::g_Shop.getShopItem(8, i);
            }
            break;
        case 4:
            num = status::g_Shop.getShopCount(9);
            for (int i = 0; i < num; i++) {
                item[i] = status::g_Shop.getShopItem(9, i);
            }
            break;
        case 5:
            num = status::g_Shop.getShopCount(10);
            for (int i = 0; i < num; i++) {
                item[i] = status::g_Shop.getShopItem(10, i);
            }
            break;
    }
    for (int i = 0; i < num; i++) {
        param[0] = item[i] + 0x40000000;
        if (price[0] != 0) {
            param[1] = price[i];
        } else {
            param[1] = status::UseItem::getBuyPrice(item[i]);
        }
        unkfunc_02050ee0(itemParts, param, 0x30, 0x20 + i * 16, 1);
    }
}
