#pragma ipa file
#include "ov016/UnkMaterielMenuDraw/MaterielMenuDraw.hpp"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/menu/MenuDataCommon.hpp"
#include "main/menu/MaterielMenuPlayerControl.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/UseItem.hpp"
#include "main/status/BattleResult.hpp"
#include "main/status/ShopList.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/GameFlag.hpp"
#include "main/profile/Profile.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/text/TextAPI.hpp"
#include "main/status/GameStatus.hpp"
#include "ov009/casino/PokerManager.hpp"
#include "ov028/MaterielMenu_SHOP/MaterielMenu_SHOP.hpp"
#include "main/menu/MenuPartsDraw.hpp"
#include "main/menu/MenuCommonDraw.hpp"
#ifdef DQIV_NATIVE
#include "main/dss/BgText.hpp"
#endif
#include "main/cmn/EnvoyManager.hpp"

static UnkMenuParts s_parts_02181b86[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0xa, 0x80, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 1, 0xc, 0x1a, 0x10, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 2, 0x18, 0x1a, 0x40, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 3, 8, 0x32, 0x80, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 4, 0xa8, 0x32, 0x1c, 0x10 },
    { 0x0d, 0x0a, (short)0xf000, 5, 0xc8, 0x32, 0xc, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 6, 0xb0, 0x32, 0x44, 0x10 },
    { 0x0c, 0xf0, 0, 7, 0xd8, 4, 0x20, 0x20 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021812fe[] = {
    { 0x14, 0xc0, 0, -1, 0, 0x10, 0x20, 0x18 },
    { 0x0c, 0xf5, 0, 0, 0, 0, 0x20, 0x20 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181e96[] = {
    { 0x0f, 0x08, (short)0xf000, 0, 0x18, 0x18, 0x50, 0xa },
    { 0x0d, 0x08, (short)0xf000, 1, 0x18, 0x28, 0x68, 0xa },
    { 0x0d, 0x08, (short)0xf000, 2, 0x98, 0x18, 0x50, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 3, 0x98, 0x18, 0x50, 0xa },
    { 0x0d, 0x08, (short)0xf000, 4, 0x98, 0x28, 0x38, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 5, 0x98, 0x28, 0x50, 0xa },
    { 0x0d, 0x08, (short)0xf000, 6, 0x98, 0x38, 0x50, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 7, 0x98, 0x38, 0x50, 0xa },
    { 0x0d, 0x08, (short)0xf000, 8, 0x98, 0x48, 0x50, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 9, 0x98, 0x48, 0x50, 0xa },
    { 0x0d, 0x08, (short)0xf000, 0xa, 0x98, 0x58, 0x50, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 0xb, 0x98, 0x58, 0x50, 0xa },
    { 0x0d, 0x08, (short)0xf000, 0xc, 0x98, 0x68, 0x50, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 0xd, 0x98, 0x68, 0x50, 0xa },
    { 0x0d, 0x0a, (short)0xf000, 0xe, 0x70, 0x78, 0x78, 0xa },
    { 0x0d, 0x0a, (short)0xf000, 0xf, 0x78, 0x88, 0x70, 0xa },
    { 0x0d, 0x08, (short)0xf000, 0x10, 0xa8, 0x98, 0x40, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 0x11, 0x98, 0x98, 0x50, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181328[] = {
    { 0x0c, 0xf0, 0, 0, 0, 0xa0, 0x20, 0x20 },
    { 0x0d, 0x08, (short)0xf000, 1, 0x28, 0xaa, 0x50, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021810dc[] = {
    { 0x0d, 0x09, (short)0xf000, 0, 8, 0x3a, 0xf0, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021811bc[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 2, 0x54, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021813a6[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0, 0x30, 0xa },
    { 0x0d, 0x08, (short)0xf000, 1, 0x3c, 0, 0x48, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0218154a[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0x56, 0x40, 0x10 },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 0x18, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 2, 0x18, 0x56, 0x20, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181b24[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0x58, 0xa0, 0xa },
    { 0x0d, 0x08, (short)0xf000, 1, 8, 0x70, 0xa0, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 2, 8, 0x80, 0x1c, 0xa },
    { 0x0d, 0x0a, (short)0xf000, 3, 0x28, 0x80, 8, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 4, 0x30, 0x80, 0x1c, 0xa },
    { 0x0e, 0x08, (short)0xf000, 5, 8, 0x98, 0xa0, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021815ba[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0, 0x30, 0xa },
    { 0x0d, 0x0a, (short)0xf000, 1, 0x50, 0, 0x10, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 2, 0x58, 0, 0x20, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181184[] = {
    { 0x0d, 0x09, (short)0xf000, 0, 0, 0x1a, 0x40, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181050[] = {
    { 0x0d, 0x09, (short)0xf000, 0, 0x80, 0x1a, 0x40, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021815f2[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x10, 0x38, 0xb0, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 1, 0x10, 0x48, 0xb0, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 2, 0x10, 0x58, 0xb0, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02180f70[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x10, 0xc, 0x90, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021811d8[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0x56, 0x40, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021810f8[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 2, 0x38, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0218162a[] = {
    { 0x14, 0xc0, 0, -1, 0, 0xc, 0x68, 0x18 },
    { 0x0e, 0x08, (short)0xf000, 0, 0xc, 8, 0x40, 0x18 },
    { 0x0c, 0xf5, 0, 1, 0x44, 0, 0x20, 0x20 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021817ea[] = {
    { 0x0f, 0x0a, (short)0xf000, 0, 0xd0, 0x78, 0x10, 0xa },
    { 0x0d, 0x08, (short)0xf000, 1, 0xe0, 0x78, 8, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 2, 0xe8, 0x78, 0x10, 0xa },
    { 0x0c, 0xf4, 0, 3, 0xe0, 0x84, 0x18, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02180f38[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 2, 0x2c, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181034[] = {
    { 0x0c, 0xf5, 0, 0, 0, 0, 0x20, 0x20 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021811a0[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x14, 2, 0x60, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181662[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0x58, 0xa0, 0xa },
    { 0x0d, 0x08, (short)0xf000, 1, 8, 0x70, 0xa0, 0x18 },
    { 0x0e, 0x08, (short)0xf000, 2, 8, 0x98, 0xa0, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021813d0[] = {
    { 0x0d, 0x0a, (short)0xf000, 0, 0xc0, 0x5e, 8, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 1, 0xc8, 0x5e, 0x18, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181114[] = {
    { 0x0e, 0x05, (short)0xf000, 0, 0x20, 0x3a, 0xc0, 0x20 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181130[] = {
    { 0x0e, 0x05, (short)0xf000, 0, 0, 0x60, 0x100, 0x30 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181830[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0x58, 0xa0, 0xa },
    { 0x0d, 0x08, (short)0xf000, 1, 8, 0x70, 0xa0, 0xa },
    { 0x0d, 0x08, (short)0xf000, 2, 8, 0x80, 0xa0, 0xa },
    { 0x0e, 0x08, (short)0xf000, 3, 8, 0x98, 0xa0, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021813fa[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0xa0, 0x40, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 1, 0x40, 0xa0, 8, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 2, 0x48, 0xa0, 0x58, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181876[] = {
    { 0x0c, 0xf4, 0, 0, 0x58, 0xa4, 0x18, 0x18 },
    { 0x0f, 0x0a, (short)0xf000, 1, 0x74, 0xa8, 8, 0xc },
    { 0x0d, 0x0b, (short)0xf000, 2, 0, 0, 0x78, 0xc },
    { 0x0f, 0x0a, (short)0xf000, 3, 0x84, 0xa8, 8, 0xc },
    { 0x0c, 0xf4, 0, 4, 0x90, 0xa4, 0x18, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02180fe0[] = {
    { 0x0f, 0x08, (short)0xf000, 0, 0, 0, 0x10, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181168[] = {
    { 0x0d, 0x09, (short)0xf000, 0, 0x30, 0x18, 0x50, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02180f1c[] = {
    { 0x0d, 0x09, (short)0xf000, 0, 0x38, 0x1a, 0x48, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02180ee4[] = {
    { 0x0d, 0x09, (short)0xf000, 0, 8, 0x3a, 0xf0, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02180f54[] = {
    { 0x15, 0xb5, 0, 0, 0, 0, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181d8c[] = {
    { 0x0d, 0x09, (short)0xf000, 0, 8, 0x18, 0xb0, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 1, 0x30, 0x30, 0x30, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 2, 0x5c, 0x30, 8, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 3, 0x68, 0x30, 0x60, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 4, 0x30, 0x3c, 0x30, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 5, 0x5c, 0x3c, 8, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 6, 0x68, 0x3c, 0x60, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 7, 0x30, 0x48, 0x30, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 8, 0x5c, 0x48, 8, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 9, 0x68, 0x48, 0x60, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 0xa, 0x14, 0x58, 0x2c, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 0xb, 0x44, 0x58, 8, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 0xc, 0x50, 0x58, 0x40, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 0xd, 0x14, 0x64, 0x30, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 0xe, 0x44, 0x64, 8, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 0xf, 0x50, 0x64, 0x60, 0x10 },
    { 0x14, 0xc0, 0, -1, 0xc, 0x38, 0x20, 0x18 },
    { 0x0c, 0xf5, 0, 0x10, 8, 0x28, 0x20, 0x20 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181018[] = {
    { 0x15, 0xb6, 0, 0, 0x78, 0x12, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021816d2[] = {
    { 0x14, 0xc0, 0, -1, 0x70, 0x90, 0x20, 0x10 },
    { 0x14, 0xc0, 0, -1, 0x70, 0xa8, 0x20, 0x10 },
    { 0x0d, 0x09, (short)0xf000, 0, 0x70, 0x92, 0x20, 0x10 },
    { 0x0d, 0x09, (short)0xf000, 1, 0x70, 0xaa, 0x20, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_partsShopCompare[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0, 0x30, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 1, 0x30, 0, 0x20, 0xa },
    { 0x0d, 0x0a, (short)0xf000, 2, 0x50, 0, 0x10, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 3, 0x58, 0, 0x20, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181280[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0, 0x68, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 1, 0x70, 0, 0x40, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_partsShopCannotEquip[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0, 0x30, 0xa },
    { 0x0d, 0x08, (short)0xf000, 1, 0x48, 0, 0x48, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0218146a[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x50, 0xa6, 0x30, 8 },
    { 0x07, 0x00, (short)0xf000, 0x11, 0x50, 0xa0, 0x30, 8 },
    { 0x11, 0x00, 0, 1, 0x48, 0xb0, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021811f4[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 2, 0x68, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02180ec8[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0x10, 0x58, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021814a2[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 6, 0x28, 8 },
    { 0x07, 0x00, (short)0xf000, 0x11, 8, 0, 0x30, 8 },
    { 0x11, 0x00, 0, 1, 0, 0x10, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static int s_int_02180e94[6] = { 0xc0000064, 0xc0000065, 0xc0000066, 0xc0000067, 0xc0000068, 0xc0000069 };
static UnkMenuParts s_parts_02180fc4[] = {
    { 0x0d, 0x09, (short)0xf000, 0, 0x10, 0x3e, 0x80, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181256[] = {
    { 0x0d, 0x0a, (short)0xf000, 0, 0x88, 0x1a, 0x38, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 1, 0xc0, 0x1a, 0x38, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181cc8[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0xc, 8, 0x40, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 1, 8, 0x1c, 0x18, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 2, 8, 0x3c, 0x18, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 3, 0x18, 0x16, 0x20, 0x10 },
    { 0x0d, 0x0a, (short)0xf000, 4, 0x20, 0x20, 8, 8 },
    { 0x0d, 0x0a, (short)0xf000, 5, 0x28, 0x20, 8, 8 },
    { 0x0d, 0x0a, (short)0xf000, 6, 0x30, 0x20, 8, 8 },
    { 0x0f, 0x0a, (short)0xf000, 7, 0x18, 0x28, 0x20, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 8, 0x18, 0x36, 0x20, 0x10 },
    { 0x0d, 0x0a, (short)0xf000, 9, 0x20, 0x40, 8, 8 },
    { 0x0d, 0x0a, (short)0xf000, 0xa, 0x28, 0x40, 8, 8 },
    { 0x0d, 0x0a, (short)0xf000, 0xb, 0x30, 0x40, 8, 8 },
    { 0x0f, 0x0a, (short)0xf000, 0xc, 0x18, 0x48, 0x20, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02180f8c[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 2, 0x50, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0218175e[] = {
    { 0x14, 0xc0, 0, -1, 0, 8, 0x68, 0x18 },
    { 0x0e, 0x08, (short)0xf000, 0, 4, 8, 0x40, 0x18 },
    { 0x0c, 0xf0, 0, 1, 0x48, 0, 0x20, 0x20 },
    { 0x0f, 0x0a, (short)0xf000, 2, 0x50, 0x18, 0x18, 8 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181582[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0x7c, 0x30, 0xa },
    { 0x0d, 0x0a, (short)0xf000, 1, 0x50, 0x7c, 0x10, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 2, 0x58, 0x7c, 0x20, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021817a4[] = {
    { 0x0f, 0x0a, (short)0xf000, 0, 0x14, 0x80, 0x30, 0xc },
    { 0x0d, 0x0a, (short)0xf000, 1, 8, 0x80, 0x48, 0xc },
    { 0x0d, 0x08, (short)0xf000, 2, 0x60, 0x80, 8, 0xc },
    { 0x0f, 0x0a, (short)0xf000, 3, 0x60, 0x80, 0x20, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02180f00[] = {
    { 0x0d, 0x09, (short)0xf000, 0, 8, 0x10, 0xb0, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0218191e[] = {
    { 0x0c, 0xf4, 0, 0, 0xb0, 0x30, 0x18, 0x18 },
    { 0x0f, 0x0a, (short)0xf000, 1, 0xcc, 0x3c, 8, 0xc },
    { 0x0d, 0x0b, (short)0xf000, 2, 0, 0, 8, 0xc },
    { 0x0f, 0x0a, (short)0xf000, 3, 0xdc, 0x3c, 8, 0xc },
    { 0x0c, 0xf4, 0, 4, 0xe0, 0x30, 0x18, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0218137c[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0x7c, 0x30, 0xa },
    { 0x0d, 0x08, (short)0xf000, 1, 0x34, 0x7c, 0x48, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181972[] = {
    { 0x14, 0xc0, 0, -1, 0, 8, 0x68, 0x18 },
    { 0x0e, 0x08, (short)0xf000, 0, 4, 8, 0x40, 0x18 },
    { 0x0c, 0xf0, 0, 1, 0x48, 0, 0x20, 0x20 },
    { 0x0f, 0x0a, (short)0xf000, 2, 0x50, 0x18, 0x18, 8 },
    { 0x0d, 0x08, (short)0xf000, 3, 0x48, 0x18, 0xe, 0xe },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0218169a[] = {
    { 0x0e, 0x08, (short)0xf000, 0, 0x14, 6, 0x90, 0x18 },
    { 0x0e, 0x08, (short)0xf000, 1, 0xb0, 6, 0x10, 0x18 },
    { 0x0e, 0x08, (short)0xf000, 2, 0xc0, 6, 0x38, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181a1a[] = {
    { 0x0f, 0x0a, (short)0xf000, 0, 0xcc, 0x78, 0x10, 0xa },
    { 0x0d, 0x08, (short)0xf000, 1, 0xdc, 0x78, 8, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 2, 0xe4, 0x78, 0x10, 0xa },
    { 0x0c, 0xf4, 0, 3, 0xc0, 0x84, 0x18, 0x18 },
    { 0x0c, 0xf4, 0, 4, 0xe0, 0x84, 0x18, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0218114c[] = {
    { 0x0e, 0x04, (short)0xf000, 0, 0x20, 0x3a, 0xc0, 0x20 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021810c0[] = {
    { 0x0d, 0x04, (short)0xf000, 0, 0x70, 0x18, 0x70, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021810a4[] = {
    { 0x14, 0xc0, 0, -1, 0, 0x10, 0x20, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0218122c[] = {
    { 0x0d, 0x0a, (short)0xf000, 0, 0x88, 0xa, 0x40, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 1, 0xc0, 0xa, 0x38, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02180eac[] = {
    { 0x0e, 0x05, (short)0xf000, 0, 0x20, 0x60, 0xc0, 0x30 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181088[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x20, 0, 0xe, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181432[] = {
    { 0x0d, 0x08, (short)0xf000, 1, 0x64, 0x9e, 0x10, 0xc },
    { 0x0d, 0x0b, (short)0xf000, 2, 0, 0, 0x28, 0xc },
    { 0x0f, 0x0b, (short)0xf000, 0, 0, 0, 0x28, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02180ffc[] = {
    { 0x0f, 0x09, (short)0xf000, 0, 0xb0, 0xa, 0x10, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021812d4[] = {
    { 0x0d, 0x09, (short)0xf000, 0, 0, 0xc, 0x100, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 1, 8, 0x9c, 0x100, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021814da[] = {
    { 0x0d, 0x0a, (short)0xf000, 0, 0x9a, 0, 0x38, 0xa },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 8, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 2, 0xd0, 0, 0x20, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181512[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0xa0, 0x96, 0x10, 0xa },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 8, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 2, 0xb8, 0x96, 0x38, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181210[] = {
    { 0x0d, 0x09, (short)0xf000, 0, 0x30, 0x18, 0x50, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02180fa8[] = {
    { 0x0d, 0x09, (short)0xf000, 0, 0x30, 0x10, 0xa0, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181352[] = {
    { 0x14, 0xc0, 0, -1, 0, 0xc, 0x20, 0x18 },
    { 0x0c, 0xf1, 0, 0, 0, 0, 0x20, 0x20 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0218106c[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x58, 0xaa, 0x28, 8 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021819c6[] = {
    { 0x14, 0xc0, 0, -1, 0, 8, 0x68, 0x18 },
    { 0x0e, 0x08, (short)0xf000, 0, 4, 8, 0x40, 0x18 },
    { 0x0c, 0xf0, 0, 1, 0x48, 0, 0x20, 0x20 },
    { 0x0f, 0x0a, (short)0xf000, 2, 0x50, 0x18, 0x18, 8 },
    { 0x0d, 0x08, (short)0xf000, 3, 0x48, 0x18, 0xe, 0xe },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181a6e[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0x58, 0xa0, 0xa },
    { 0x0d, 0x08, (short)0xf000, 1, 8, 0x70, 0xa0, 0xa },
    { 0x0d, 0x0a, (short)0xf000, 2, 0x28, 0x80, 8, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 3, 0x30, 0x80, 0x1c, 0xa },
    { 0x0e, 0x08, (short)0xf000, 4, 8, 0x98, 0xa0, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181c04[] = {
    { 0x0d, 0x09, (short)0xf000, 0, 8, 0x5e, 0xa0, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 1, 8, 0x78, 0x40, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 2, 0x40, 0x78, 8, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 3, 0x48, 0x78, 0x58, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 4, 8, 0x85, 0x40, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 5, 0x40, 0x85, 8, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 6, 0x48, 0x85, 0x58, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 7, 8, 0x93, 0x40, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 8, 0x40, 0x93, 8, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 9, 0x48, 0x93, 0x58, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 0xa, 8, 0xac, 0x40, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 0xb, 0x40, 0xac, 8, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 0xc, 0x48, 0xac, 0x60, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02181ac2[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0xc, 0, 8, 0xc },
    { 0x0d, 0x08, (short)0xf000, 1, 0x14, 0, 0x60, 0xc },
    { 0x0f, 0x0a, (short)0xf000, 2, 0x88, 0, 0x30, 0xc },
    { 0x0d, 0x0a, (short)0xf000, 3, 0x78, 0, 0x50, 0xc },
    { 0x0d, 0x08, (short)0xf000, 4, 0xd8, 0, 8, 0xc },
    { 0x0f, 0x0a, (short)0xf000, 5, 0xe0, 0, 0x18, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021818ca[] = {
    { 0x0c, 0xf4, 0, 0, 0xb0, 8, 0x18, 0x18 },
    { 0x0f, 0x0a, (short)0xf000, 1, 0xc8, 0xe, 8, 0xc },
    { 0x0d, 0x0b, (short)0xf000, 2, 0, 0, 0x78, 0xc },
    { 0x0f, 0x0a, (short)0xf000, 3, 0xd8, 0xe, 8, 0xc },
    { 0x0c, 0xf4, 0, 4, 0xe0, 8, 0x18, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};

THUMB void drawShopRootCommands(bool bank, int type)
{
    int command[3];
    int mode = 0;
    if (type) {
        mode = 3;
    }
    menu::MenuDataCommon::getShopCommand(command, bank);
    for (int i = 0; i < 3; i++) {
        drawMenuParts(s_parts_021811bc, &command[i], 0xa4, i * 16 + 8 + i * 8, g_menuTextColors[mode]);
    }
    drawMenuFrame(0x98, 0, 0x68, 0x50, -1);
}

ARM void drawPartsInTextColorAt(UnkMenuParts* parts, int* param, int x, int y, int mode)
{
    drawMenuParts(parts, param, x, y, g_menuTextColors[mode]);
}

THUMB void drawShopEquipCompare()
{
    setMenuDrawOffset(0, 0);
    UnkMenuParts* parts = 0;
    for (int i = 0; i < status::g_Party.getCount(); i++) {
        int param[4];
        switch (getShopEquipCompare(&status::g_Party.getPlayerStatus(i)->haveStatusInfo_, param, -1)) {
            case 0:
                parts = s_partsShopCannotEquip;
                break;
            case 1:
                s_partsShopCompare[1].type_ = 0xf;
                parts = s_partsShopCompare;
                break;
            case 2:
                s_partsShopCompare[1].type_ = 0;
                parts = s_partsShopCompare;
                param[3] = param[2];
                param[2] = param[1];
                param[1] = 0;
                break;
        }
        drawMenuParts(parts, param, (i % 2) << 7, ((i / 2) << 4) + 0x28, g_menuTextColors[0]);
    }
    drawShopItemHeader(-1);
    drawMenuFrame(0, 0, 0x100, 0x90, 0x20);
    drawMenuFrame(0, 0x90, 0x100, 0x30, -1);
}

ARM status::HaveStatusInfo* getPlayerHaveStatusInfo(status::PlayerStatus* player)
{
    return &player->haveStatusInfo_;
}

THUMB void drawShopHeldCounts()
{
    setMenuDrawOffset(0, 0);
    for (int i = 0; i < status::g_Party.getCount(); i++) {
        int param[3];
        UnkMenuParts* parts;
        if (getShopHeldCount(&status::g_Party.getPlayerStatus(i)->haveStatusInfo_, param, -1)) {
            parts = s_parts_021815ba;
        } else {
            parts = s_parts_021813a6;
        }
        drawMenuParts(parts, param, ((i % 2) << 7) - ((i % 2) << 2), ((i / 2) << 4) + 0x28, g_menuTextColors[0]);
    }
    drawShopItemHeader(-1);
    drawMenuFrame(0, 0, 0x100, 0x90, 0x20);
    drawMenuFrame(0, 0x90, 0x100, 0x30, -1);
}

THUMB void drawShopSackCount(int* count)
{
    setMenuDrawOffset(0, 0);
    int item = MaterielMenuPlayerControl::getSingleton()->activeItem_;
    if (status::g_Party.fukuro_) {
        int param[3];
        UnkMenuParts* parts;
        param[0] = 0x5000005b;
        if (count[item] == 0) {
            param[1] = 0xa0000079;
            parts = s_parts_0218137c;
        } else {
            param[1] = (int)"\xff\xfe\xa9\x24";
            param[2] = count[item];
            parts = s_parts_02181582;
        }
        drawMenuPartsColored(parts, param, g_menuTextColors[0]);
    }
}

ARM void drawPartsInTextColor(UnkMenuParts* parts, int* param, int mode)
{
    drawMenuPartsColored(parts, param, g_menuTextColors[mode]);
}

THUMB void drawShopItemKind()
{
    int item = MaterielMenuPlayerControl::getSingleton()->activeItem_;
    int itemID = MaterielMenu_SHOP_MANAGER::getSingleton()->getItem(item);
    int param[2];
    if (MaterielMenu_SHOP_MANAGER::getSingleton()->getExtraShop() == 1) {
        if (item == 0) {
            param[0] = 0x6f;
        } else {
            param[0] = 7;
        }
    } else {
        param[0] = itemID;
    }
    if (status::UseItem::getItemType(itemID) <= 4) {
        param[1] = menu::MenuDataCommon::getEquipKind(status::UseItem::getItemType(itemID));
    } else {
        param[1] = 0x80000065;
    }
    drawMenuPartsColored(s_parts_02181328, param, g_menuTextColors[0]);
    drawMenuFrame(0, 0xa0, 0x88, 0x20, -1);
}

#ifdef DQIV_NATIVE
bool unkfunc_native_shopFilterMatch(int filter, int itemID)
{
    const int type = status::UseItem::getItemType(itemID);
    switch (filter) {
    case SHOP_FILTER_WEAPON:
        return type == ITEM_WEAPON;
    case SHOP_FILTER_ARMOR:
        return type == ITEM_ARMOR;
    case SHOP_FILTER_SHIELD:
        return type == ITEM_SHIELD;
    case SHOP_FILTER_HELMET:
        return type == ITEM_HELMET;
    case SHOP_FILTER_ACCESSORY:
        return type == ITEM_ACCESSORY;
    case SHOP_FILTER_ITEM:
        return type > ITEM_ACCESSORY;
    }
    return true;
}

void unkfunc_native_shopBuyList(const int* rows, int count)
{
    // As drawShopBuyList, below the category tabs, with each item's icon left of its name.
    UnkMenuParts parts[] = {
        { 0x0c, 0xfb, 0, 5, 0x0c, 0, 0x10, 0x10 },
        { 0x0d, 0x08, (short)0xf000, 0, 0x20, 2, 0x78, 0xc },
        { 0x0d, 0x08, (short)0xf000, 1, 0x9c, 2, 8, 0xc },
        { 0x0f, 0x0b, (short)0xf000, 2, 0, 0, 8, 0xc },
        { 0x0f, 0x0a, (short)0xf000, 3, 0xa8, 2, 0x38, 0xc },
        { 0x0d, 0x0b, (short)0xf000, 4, 0, 0, 8, 0xc },
        { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
    };
    MaterielMenu_SHOP_MANAGER* manager = MaterielMenu_SHOP_MANAGER::getSingleton();
    for (int k = 0; k < count; k++) {
        const int i = rows[k];
        int param[6];
        int itemID = manager->getItem(i);
        if (manager->getExtraShop() == 1) {
            itemID = i == 0 ? 0x6f : 7;
        }
        param[0] = itemID + 0x40000000;
        param[1] = (int)"\xff\xfe\xa9\x24";
        param[2] = manager->getItemQuantity(i);
        param[3] = manager->getItemPriceSum(i);
        param[4] = 0xa000003c;
        param[5] = itemID;
        drawMenuParts(parts, param, 0xc, unkfunc_native_shopRowY(k), g_menuTextColors[0]);
    }
    drawMenuFrame(0, 0x18, 0x100, 0x88, -1);
}

int unkfunc_native_shopRowY(int row)
{
    return 0x20 + row * 0x10;
}

void unkfunc_native_tabs(const char* const* names, int count, int active, int x, int w, void (*frame)(int, int, int, int, int))
{
    // L, a small window per tab as wide as its name, R; the chosen tab's name
    // in yellow. Texts are drawn in 8-pixel cells: tabs on that grid. When
    // they are too many, the ones around the chosen tab.
    UnkMenuParts parts[] = {
        { 0x0d, 0x09, (short)0xf000, 0, 0, 5, 0x10, 0xe },
        { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
    };
    enum { MAX_TABS = 8 };
    int widths[MAX_TABS];
    count = count < MAX_TABS ? count : MAX_TABS;
    for (int k = 0; k < count; k++) {
        setBgTextFontSize(10);
        drawBgTextWrapped(NULL, 0, 0, 0, names[k], 0);
        widths[k] = (getBgTextWidth() + 16 + 7) & ~7;
    }
    const int room = (w - 0x20) & ~7;
    int first = active, last = active + 1, total = count > 0 ? widths[active] : 0;
    for (bool grew = true; grew;) {
        grew = false;
        if (last < count && total + widths[last] <= room) {
            total += widths[last++];
            grew = true;
        }
        if (first > 0 && total + widths[first - 1] <= room) {
            total += widths[--first];
            grew = true;
        }
    }
    int tabX = (x + (w - total) / 2) & ~7;
    if (tabX < x + 0x10) {
        tabX = x + 0x10;
    }
    for (int k = first; k < last; k++) {
        int param = (int)names[k];
        parts[0].w_ = widths[k];
        drawMenuParts(parts, &param, tabX, 0, k == active ? g_menuTextColors[1] : g_menuTextColors[0]);
        // Japanese glyphs are taller than the frame's inside: names alone.
        if (status::g_Game.language != Japanese) {
            frame(tabX, 0, widths[k], 0x18, -1);
        }
        tabX += widths[k];
    }
    if (count > 1) {
        int param = (int)"L";
        parts[0].w_ = 0x10;
        drawMenuParts(parts, &param, x, 0, g_menuTextColors[0]);
        param = (int)"R";
        drawMenuParts(parts, &param, x + w - 0x10, 0, g_menuTextColors[0]);
    }
}

void unkfunc_native_shopTabs(const int* filters, int count, int active)
{
    const char* names[SHOP_FILTER_COUNT];
    for (int k = 0; k < count && k < SHOP_FILTER_COUNT; k++) {
        names[k] = unkfunc_native_shopFilterName(filters[k]);
    }
    unkfunc_native_tabs(names, count, active, 0, 0x100, drawMenuFrame);
}

const char* unkfunc_native_shopFilterName(int filter)
{
    static const char* const names[SHOP_FILTER_COUNT][6] = {
        { "\xe3\x81\x99\xe3\x81\xb9\xe3\x81\xa6", "All", "Tout", "Alle", "Tutto", "Todo" },
        { "\xe3\x81\xb6\xe3\x81\x8d", "Weapons", "Armes", "Waffen", "Armi", "Armas" },
        { "\xe3\x82\x88\xe3\x82\x8d\xe3\x81\x84", "Armour", "Armures", "R\xc3\xbc""stungen", "Armature", "Armaduras" },
        { "\xe3\x81\x9f\xe3\x81\xa6", "Shields", "Boucliers", "Schilde", "Scudi", "Escudos" },
        { "\xe3\x81\x8b\xe3\x81\xb6\xe3\x81\xa8", "Helmets", "Casques", "Helme", "Elmi", "Cascos" },
        { "\xe3\x81\x9d\xe3\x81\x86\xe3\x81\x97\xe3\x82\x87\xe3\x81\x8f\xe3\x81\xb2\xe3\x82\x93", "Accessories", "Accessoires", "Accessoires", "Accessori", "Accesorios" },
        { "\xe3\x81\xa9\xe3\x81\x86\xe3\x81\x90", "Items", "Objets", "Gegenst\xc3\xa4""nde", "Oggetti", "Objetos" },
    };
    int language = status::g_Game.language;
    if (language < 0 || language > 5) {
        language = 1;
    }
    return names[filter][language];
}

#endif

THUMB void drawShopBuyList(int flag)
{
    UnkMenuParts partsA[] = {
        { 0x0d, 0x08, (short)0xf000, 0, 0xa, 2, 0x80, 0xc },
        { 0x0d, 0x08, (short)0xf000, 1, 0x9c, 2, 8, 0xc },
        { 0x0f, 0x0b, (short)0xf000, 2, 0, 0, 8, 0xc },
        { 0x0f, 0x0a, (short)0xf000, 3, 0xa8, 2, 0x38, 0xc },
        { 0x0d, 0x0b, (short)0xf000, 4, 0, 0, 8, 0xc },
        { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
    };
    UnkMenuParts partsB[] = {
        { 0x0d, 0x08, (short)0xf000, 0, 0xa, 2, 0x80, 0xc },
        { 0x0f, 0x0a, (short)0xf000, 1, 0xa8, 2, 0x38, 0xc },
        { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
    };
    int param[5];
    int count = MaterielMenu_SHOP_MANAGER::getSingleton()->getSellItemCount();
    for (int i = 0; i < count; i++) {
        if (flag) {
            param[0] = MaterielMenu_SHOP_MANAGER::getSingleton()->getItem(i) + 0x40000000;
            param[1] = MaterielMenu_SHOP_MANAGER::getSingleton()->getItemPriceSum(i);
            drawMenuParts(partsB, param, 0xc, i * 16 + 0xc + i * 8, g_menuTextColors[0]);
        } else {
            if (MaterielMenu_SHOP_MANAGER::getSingleton()->getExtraShop() == 1) {
                if (i == 0) {
                    param[0] = 0x4000006f;
                } else {
                    param[0] = 0x40000007;
                }
            } else {
                param[0] = MaterielMenu_SHOP_MANAGER::getSingleton()->getItem(i) + 0x40000000;
            }
            param[1] = (int)"\xff\xfe\xa9\x24";
            param[2] = MaterielMenu_SHOP_MANAGER::getSingleton()->getItemQuantity(i);
            param[3] = MaterielMenu_SHOP_MANAGER::getSingleton()->getItemPriceSum(i);
            param[4] = 0xa000003c;
            drawMenuParts(partsA, param, 0xc, i * 16 + 0xc + i * 8, g_menuTextColors[0]);
        }
    }
    drawMenuFrame(0, 0, 0x100, 0xa0, -1);
}

// not in the ROM (dead-stripped), its local array initializer is still in .rodata
THUMB void unkfunc_unused_6(int index)
{
    int message[6] = { 0x800000ca, 0x800000cb, 0x800000cc, 0x800000cd, 0x800000ce, 0x800000cf };
    drawMenuPartsColored(s_parts_02181168, &message[index], 1);
}

THUMB void drawCharaItemNames()
{
    setMenuDrawOffset(0, 0);
    UnkMenuParts parts[] = {
        { 0x0d, 0x08, (short)0xf000, 0, 0, 0, 8, 0x10 },
        { 0x0d, 0x08, (short)0xf000, 1, 0xc, 0, 0x68, 0x10 },
        { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
    };
    int param[2];
    int item;
    int count;
    int i;
    int chara = MaterielMenuPlayerControl::getSingleton()->activeChara_;
    if (status::g_Party.fukuro_ && chara == status::g_Party.getCount()) {
        count = status::g_Party.haveItemSack_.getCount();
        if (count > 12) {
            count = 12;
        }
    } else {
        count = status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveItem_.getCount();
    }
    for (i = 0; i < count; i++) {
        int x = (i % 2) * 0x70 + 0xc;
        int y = ((i / 2) << 4) + 0x58;
        if (status::g_Party.fukuro_ && chara == status::g_Party.getCount()) {
            item = status::g_Party.haveItemSack_.getItem(i);
            parts[0].type_ = 0xd;
            param[0] = 0;
        } else {
            item = status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveItem_.getItem(i);
            if (status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveItem_.isEquipment(i)) {
                parts[0].type_ = 0xd;
                param[0] = 0xa0000041;
            } else {
                parts[0].type_ = 0;
                param[0] = 0;
            }
        }
        param[1] = item + 0x40000000;
        drawMenuParts(parts, param, x, y, g_menuTextColors[0]);
    }
    drawMenuFrame(0, 0x50, 0x100, 0x70, -1);
}

THUMB void drawShopGiveCompare()
{
    int itemID;
    int chara;
    int item;
    int extra;
    int param[8];
    int value[5];
    item = MaterielMenuPlayerControl::getSingleton()->activeItem_;
    chara = MaterielMenuPlayerControl::getSingleton()->activeChara_;
    extra = MaterielMenu_SHOP_MANAGER::getSingleton()->getExtraShop();
    itemID = MaterielMenu_SHOP_MANAGER::getSingleton()->getItem(item);
    if (extra == 1) {
        if (item == 0) {
            itemID = 0x6f;
            param[0] = itemID + 0x40000000;
            param[7] = itemID;
        } else {
            itemID = 7;
            param[0] = itemID + 0x40000000;
            param[7] = itemID;
        }
    } else {
        param[0] = itemID + 0x40000000;
        param[7] = itemID;
    }
    param[1] = (int)"\xff\xfe\xaf\x24";
    param[2] = status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveStatus_.playerIndex_ + 0x50000000;
    if (chara == status::g_Party.getCount()) {
        param[2] = 0x5000005b;
    }
    if (status::UseItem::getItemType(itemID) <= 4 && chara != status::g_Party.getCount()) {
        if (status::UseItem::getEquipType(itemID) == 4) {
            param[3] = menu::MenuDataCommon::getAbilityKind(6);
        } else {
            param[3] = menu::MenuDataCommon::getAbilityKind(status::UseItem::getEquipType(itemID));
        }
        switch (getShopEquipCompare(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, value, -1)) {
            case 0:
                param[3] = value[1];
                param[4] = param[5] = param[6] = 0;
                s_parts_02181b86[4].type_ = s_parts_02181b86[5].type_ = s_parts_02181b86[6].type_ = 0;
                break;
            case 1:
                param[4] = value[1];
                param[5] = value[2];
                param[6] = value[3];
                s_parts_02181b86[4].type_ = 0xf;
                s_parts_02181b86[5].type_ = 0xd;
                s_parts_02181b86[6].type_ = 0xf;
                break;
            case 2:
                param[4] = 0;
                param[5] = value[1];
                param[6] = value[2];
                s_parts_02181b86[4].type_ = 0;
                s_parts_02181b86[5].type_ = 0xd;
                s_parts_02181b86[6].type_ = 0xf;
                break;
        }
    } else {
        param[4] = param[5] = param[6] = 0;
        s_parts_02181b86[4].type_ = s_parts_02181b86[5].type_ = s_parts_02181b86[6].type_ = 0;
        if (status::g_Party.fukuro_ && chara == status::g_Party.getCount()) {
            if (activeCharaHasItem(itemID)) {
                param[3] = 0xa0000078;
            } else {
                param[3] = 0xa0000079;
            }
        } else if (activeCharaHasItem(itemID)) {
            param[3] = 0xa0000076;
        } else {
            param[3] = 0xa0000077;
        }
    }
    drawMenuPartsColored(s_parts_02181b86, param, g_menuTextColors[0]);
    drawMenuFrame(0, 0, 0x100, 0x48, 0x28);
}

THUMB void drawPartyIcons(int y, int flag)
{
    int param = 0;
    for (int i = 0; i < status::g_Party.getCount(); i++) {
        int x = ((i % 5) << 5) + 0x14 + ((i % 5) << 3);
        int yy = (i / 5) * 0x28 + 0x48 - y;
        status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i)->haveStatusInfo_;
        param = info->haveStatus_.iconIndex_;
        drawMenuPartsAt(s_parts_02181352, &param, x, yy);
        if (info->isSpell()) {
            param = 0xa0000044;
            drawMenuParts(s_parts_02181088, &param, x - 6, yy + 0x14, g_menuTextColors[getHpTextColorIndex(info)]);
        }
        if (info->isDeath()) {
            param = 0xa0000042;
            drawMenuParts(s_parts_02181088, &param, x - 6, yy + 8, g_menuTextColors[getHpTextColorIndex(info)]);
        } else if (info->isPoison()) {
            param = 0xa0000043;
            drawMenuParts(s_parts_02181088, &param, x - 6, yy + 8, g_menuTextColors[getHpTextColorIndex(info)]);
        }
    }
    if (flag == 1) {
        status::g_Party.setBattleMode();
        int n = 0;
        for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
            int x = ((n % 5) << 5) + 0x10 + ((n % 5) << 3);
            status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i)->haveStatusInfo_;
            bool player = status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.isPlayer_;
            if (player == true) {
                param = i + 1;
                drawMenuParts(s_parts_02180fe0, &param, x, 0x50 - y, g_menuTextColors[getHpTextColorIndex(info)]);
                n++;
            }
        }
        status::g_Party.setPlayerMode();
    } else {
        for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
            int m = i % 5;
            status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i)->haveStatusInfo_;
            param = i + 1;
            drawMenuParts(s_parts_02180fe0, &param, (m << 5) + 0x10 + (m << 3), 0x50 - y, g_menuTextColors[getHpTextColorIndex(info)]);
        }
    }
    if (status::g_Party.fukuro_ && y == 0) {
        int count = status::g_Party.getCount();
        param = 0x18;
        drawMenuPartsAt(s_parts_02181352, &param, ((count % 5) << 5) + 0x14 + ((count % 5) << 3), (count / 5) * 0x28 + 0x48);
    }
    drawMenuFrame(0, 0x48 - y, 0xe0, 0x58, -1);
}

THUMB void drawSellPriceList()
{
    setMenuDrawOffset(0, 0);
    int chara = MaterielMenuPlayerControl::getSingleton()->activeChara_;
    int param[6];
    if (chara == status::g_Party.getCount()) {
        int count;
        if (status::g_Party.haveItemSack_.getCount() > 12) {
            count = 12;
        } else {
            count = status::g_Party.haveItemSack_.getCount();
        }
        s_parts_02181ac2[0].type_ = 0;
        for (int i = 0; i < count; i++) {
            if (status::UseItem::getSellType(status::g_Party.haveItemSack_.getItem(i)) == 1) {
                s_parts_02181ac2[2].type_ = s_parts_02181ac2[4].type_ = s_parts_02181ac2[5].type_ = 0;
            } else {
                s_parts_02181ac2[2].type_ = s_parts_02181ac2[5].type_ = 0xf;
                s_parts_02181ac2[4].type_ = 0xd;
            }
            getSellListRow(param, chara, i);
            drawMenuParts(s_parts_02181ac2, param, 0, i * 14 + 0xc, g_menuTextColors[0]);
        }
    } else {
        status::HaveItem* haveItem = &status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveItem_;
        int count = haveItem->getCount();
        s_parts_02181ac2[4].type_ = s_parts_02181ac2[5].type_ = 0;
        for (int i = 0; i < count; i++) {
            if (haveItem->isEquipment(i)) {
                s_parts_02181ac2[0].type_ = 0xd;
            } else {
                s_parts_02181ac2[0].type_ = 0;
            }
            if (status::UseItem::getSellType(haveItem->getItem(i)) == 1) {
                s_parts_02181ac2[2].type_ = 0;
            } else {
                s_parts_02181ac2[2].type_ = 0xf;
            }
            getSellListRow(param, chara, i);
            drawMenuParts(s_parts_02181ac2, param, 0, i * 14 + 0xc, g_menuTextColors[0]);
        }
    }
    drawMenuFrame(0, 0, 0x100, 0xc0, -1);
}

THUMB void drawItemPage(int flag)
{
    int page = MaterielMenuPlayerControl::getSingleton()->activeItemPage_;
    int end = (page + 1) * 6;
    int param[4];
    if (flag) {
        status::BaseHaveItem* sack = &status::g_Party.haveItemSack_;
        int n = 0;
        if (end > sack->getCount()) {
            end = sack->getCount();
        }
        for (int i = page * 6; i < end; i++) {
            param[0] = sack->getItem(i) + 0x40000000;
            param[1] = sack->getItem(i);
            param[3] = 0;
            s_parts_02181972[4].type_ = 0;
            if (sack->getItemCount(i) <= 1) {
                param[2] = 0;
                s_parts_02181972[3].type_ = 0;
            } else {
                param[2] = sack->getItemCount(i);
                if (param[2] > 99) {
                    param[2] = 99;
                }
                s_parts_02181972[3].type_ = 0xf;
            }
            drawMenuParts(s_parts_02181972, param, (n % 2) * 0x78 + 0x14, ((n / 2) << 5) + 4, g_menuTextColors[0]);
            n++;
        }
    } else {
        MaterielMenuPlayerControl* ctrl = MaterielMenuPlayerControl::getSingleton();
        int n = 0;
        status::HaveItem* haveItem = &status::g_Party.getPlayerStatus(ctrl->activeChara_)->haveStatusInfo_.haveItem_;
        if (end > haveItem->getCount()) {
            end = haveItem->getCount();
        }
        for (int i = page * 6; i < end; i++) {
            param[0] = haveItem->getItem(i) + 0x40000000;
            param[1] = haveItem->getItem(i);
            param[2] = 0;
            s_parts_02181972[3].type_ = 0;
            if (haveItem->isEquipment(i)) {
                param[3] = 0xa0000041;
                s_parts_02181972[4].type_ = 0xd;
            } else {
                param[3] = 0;
                s_parts_02181972[4].type_ = 0;
            }
            drawMenuParts(s_parts_02181972, param, (n % 2) * 0x78 + 0x14, ((n / 2) << 5) + 4, g_menuTextColors[0]);
            n++;
        }
    }
    drawMenuFrame(0, 0, 0x100, 0xa0, 0x70);
}

THUMB void drawSellPrice(int flag)
{
    MaterielMenuPlayerControl* control = MaterielMenuPlayerControl::getSingleton();
    int start;
    int item;
    item = control->activeItem_;
    start = control->activeItemPage_ * 6;
    int param[4];
    if (flag) {
        status::BaseHaveItem* sack = &status::g_Party.haveItemSack_;
        if (status::UseItem::getSellType(sack->getItem(item + start)) == 1) {
            param[1] = 0xa000007f;
            param[0] = param[2] = param[3] = 0;
            s_parts_021817a4[0].type_ = s_parts_021817a4[2].type_ = s_parts_021817a4[3].type_ = 0;
        } else {
            param[0] = status::UseItem::getSellPrice(sack->getItem(item + start));
            param[1] = 0xa000003c;
            param[2] = (int)"\xff\xfe\xa9\x24";
            param[3] = sack->getItemCount(item + start);
            if (param[3] > 99) {
                param[3] = 99;
            }
            s_parts_021817a4[0].type_ = s_parts_021817a4[3].type_ = 0xf;
            s_parts_021817a4[2].type_ = 0xd;
        }
        drawMenuPartsColored(s_parts_021817a4, param, g_menuTextColors[0]);
    } else {
        param[2] = param[3] = 0;
        s_parts_021817a4[2].type_ = s_parts_021817a4[3].type_ = 0;
        status::HaveItem* haveItem = &status::g_Party.getPlayerStatus(MaterielMenuPlayerControl::getSingleton()->activeChara_)->haveStatusInfo_.haveItem_;
        if (status::UseItem::getSellType(haveItem->getItem(item + start)) == 1) {
            param[1] = 0xa000007f;
            param[0] = 0;
            s_parts_021817a4[0].type_ = 0;
        } else {
            param[0] = status::UseItem::getSellPrice(haveItem->getItem(item + start));
            param[1] = 0xa000003c;
            s_parts_021817a4[0].type_ = 0xf;
        }
        drawMenuPartsColored(s_parts_021817a4, param, g_menuTextColors[0]);
    }
}

THUMB void drawItemPageNumber(int mode, int a, int b, int c)
{
    int param[5];
    param[1] = (int)"\xff\xfe\xa7\x24";
    param[3] = 0xc;
    param[4] = 0xd;
    UnkMenuParts* parts = s_parts_02181a1a;
    if (mode) {
        if (a != -1) {
            if (a <= 6) {
                return;
            }
            param[0] = b + 1;
            param[2] = c + 1;
        } else {
            if (status::g_Party.haveItemSack_.getCount() <= 6) {
                return;
            }
            param[0] = MaterielMenuPlayerControl::getSingleton()->activeItemPage_ + 1;
            param[2] = (status::g_Party.haveItemSack_.getCount() - 1) / 6 + 1;
        }
    } else {
        int chara = MaterielMenuPlayerControl::getSingleton()->activeChara_;
        int count;
        if (chara == status::g_Party.getCount()) {
            count = status::g_Party.haveItemSack_.getCount();
        } else {
            count = status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveItem_.getCount();
        }
        if (count <= 6) {
            return;
        }
        param[0] = MaterielMenuPlayerControl::getSingleton()->activeItemPage_ + 1;
        param[2] = count / 6 + 1;
        if (count == 12) {
            param[2] = 2;
        }
    }
    drawMenuPartsColored(parts, param, g_menuTextColors[0]);
}

THUMB void drawItemDetail(int item)
{
    UnkMenuParts* parts = s_parts_02181662;
    int param[6];
    int value[4];
    int chara;
    status::HaveStatusInfo* info;
    int start;
    int index;
    setMenuDrawOffset(0, 0);
    MaterielMenuPlayerControl* control = MaterielMenuPlayerControl::getSingleton();
    index = control->activeItem_;
    start = control->activeItemPage_ * 6;
    chara = MaterielMenuPlayerControl::getSingleton()->activeChara_;
    if (item == -1) {
        if (chara == status::g_Party.getCount()) {
            item = status::g_Party.haveItemSack_.getItem(index + start);
        } else {
            info = &status::g_Party.getPlayerStatus(MaterielMenuPlayerControl::getSingleton()->activeChara_)->haveStatusInfo_;
            item = info->haveItem_.getItem(index + start);
            switch (getShopEquipCompare(info, value, item)) {
                case 0:
                    param[2] = value[1];
                    parts = s_parts_02181830;
                    param[3] = item + 0x20000000;
                    break;
                case 1:
                    param[2] = value[1];
                    parts = s_parts_02181b24;
                    param[3] = value[2];
                    param[4] = value[3];
                    param[5] = item + 0x20000000;
                    break;
                case 2:
                    param[2] = value[1];
                    parts = s_parts_02181a6e;
                    param[3] = value[2];
                    param[4] = item + 0x20000000;
                    break;
            }
        }
    }
    if (status::UseItem::getItemType(item) <= 4) {
        param[0] = menu::MenuDataCommon::getEquipKind(status::UseItem::getItemType(item));
        if (status::UseItem::getEquipType(item) == 4) {
            param[1] = menu::MenuDataCommon::getAbilityKind(6);
        } else {
            param[1] = menu::MenuDataCommon::getAbilityKind(status::UseItem::getEquipType(item));
        }
        if (parts == s_parts_02181662) {
            param[2] = item + 0x20000000;
        }
    } else {
        parts = s_parts_02181662;
        param[0] = menu::MenuDataCommon::getItemMessageNo(item);
        param[1] = menu::MenuDataCommon::getItemMessageNo2(item);
        param[2] = item + 0x20000000;
        if (param[0] == 0) {
            if (status::UseItem::getItemType(item) == 10) {
                param[0] = 0x8000008a;
            } else {
                param[0] = 0x80000065;
            }
        }
        if (param[1] == 0) {
            if (status::UseItem::isLost(item)) {
                param[1] = 0xcaf5e;
            } else {
                param[1] = 0xcaf60;
            }
        }
    }
    drawMenuPartsColored(parts, param, g_menuTextColors[0]);
    drawMenuFrame(0, 0x50, 0xb0, 0x70, 0x68);
    drawMenuFrameLine(0, 0x90, 0xb0);
}

THUMB void drawSellQuantity(int quantity)
{
    int param[2] = { (int)"\xff\xfe\xa9\x24", quantity };
    drawMenuPartsColored(s_parts_021813d0, param, g_menuTextColors[0]);
    drawMenuFrame(0x98, 0x50, 0x68, 0x28, -1);
}

THUMB void drawPokerCoins(int coin, int flag)
{
    int param[2] = { (int)0xa0000029, coin };
    int mode = 0;
    drawMenuPartsColored(s_parts_0218122c, param, g_menuTextColors[0]);
    param[1] = PokerManager::getSingleton()->getCoin_;
    if (flag) {
        param[0] = 0xa000013a;
        mode = 2;
    } else {
        param[0] = 0xa0000139;
    }
    drawMenuPartsColored(s_parts_02181256, param, g_menuTextColors[mode]);
    drawMenuFrame(0x78, 0, 0x88, 0x38, -1);
}

THUMB void drawHighLowCommands(int win)
{
    int command[2];
    menu::MenuDataCommon::getHighAndLowCommand(command, win);
    drawMenuPartsColored(s_parts_021816d2, command, g_menuTextColors[0]);
}

THUMB void drawMonsterBookPage(int* monsterName, int* monsterFlag)
{
    int param[4];
    int count;
    int page;
    page = MaterielMenuPlayerControl::getSingleton()->activeItemPage_;
    count = 16;
    if (page == 13) {
        count = 2;
    }
    for (int i = 0; i < count; i++) {
        if (monsterFlag[i] == 1) {
            if (monsterName[i] == 0xa9) {
                param[0] = 0xa0000261;
            } else if (monsterName[i] == 0xb5) {
                param[0] = 0xa0000262;
            } else {
                param[0] = monsterName[i] + 0x60000000;
            }
        } else {
            param[0] = 0xa0000259;
        }
        drawMenuParts(s_parts_02180ec8, param, (i % 2) * 0x70 + 0x20, (i / 2) * 12 + (i / 2) * 4 + 0xd, g_menuTextColors[4]);
    }
    param[0] = page + 1;
    param[1] = 0xa0000258;
    param[2] = (int)" ";
    drawMenuPartsColored(s_parts_02181432, param, g_menuTextColors[4]);
}

THUMB void drawMonsterBookDetail(int monsterNo, int monsterName)
{
    int count = status::g_BattleResult.getMonsterCount(monsterNo);
    int item = status::g_BattleResult.getItemIndex(monsterNo, 0);
    int param[18];
    param[0] = monsterNo + 1;
    param[1] = monsterName + 0x60000000;
    param[2] = 0xa000025a;
    param[3] = count;
    param[4] = 0xa000025b;
    param[5] = status::g_BattleResult.getMaxExp(monsterNo);
    param[6] = 0xa000025c;
    param[7] = status::g_BattleResult.getExpTotal(monsterNo);
    param[8] = 0xa000025d;
    param[9] = status::g_BattleResult.getMaxGold(monsterNo);
    param[10] = 0xa000025e;
    param[11] = status::g_BattleResult.getGoldTotal(monsterNo);
    param[12] = 0xa000025f;
    param[13] = status::g_BattleResult.getItemCount(monsterNo);
    param[15] = 0xa0000260;
    param[16] = 0xa0000263;
    param[17] = status::g_BattleResult.getLevel(monsterNo);
    if (monsterName == 0xa9) {
        param[1] = 0xa0000261;
    } else if (monsterName == 0xb5) {
        param[1] = 0xa0000262;
    }
    param[14] = -1;
    if (count > 0) {
        param[14] = status::g_BattleResult.getItemIndex(monsterNo, 1);
    }
    if (param[14] == 0) {
        param[14] = 0xa0000264;
    } else if (count > 0x13 || item != 0) {
        param[14] = status::g_BattleResult.getItemIndex(monsterNo, 1) + 0x40000000;
    } else {
        param[14] = 0xa0000259;
    }
    drawMenuPartsColored(s_parts_02181e96, param, g_menuTextColors[4]);
}

THUMB void drawChurchCommands()
{
    int command[6];
    int count = MaterielMenuPlayerControl::getSingleton()->churchCommandNum_;
    bool extra = false;
    if (count != 6) {
        extra = true;
    }
    menu::MenuDataCommon::getChurchCommand(command, extra);
    for (int i = 0; i < count; i++) {
        drawMenuParts(s_parts_02180f8c, &command[i], (i % 2) * 0x60 + 0x44, (i / 2) * 0x18 + 0xc, g_menuTextColors[0]);
    }
    drawMenuFrame(0x30, 0, 0xd0, 0x58, -1);
}

THUMB void drawBankDigits(int money, int flag)
{
    int div = 100000;
    int y = 0;
    int param = 0;
    if (flag) {
        y = 0x50;
    }
    for (int i = 0; i < 6; i++) {
        param = money / div;
        if (flag == 0 && i == 0) {
            money %= div;
            div /= 10;
        } else {
            drawMenuParts(s_parts_02180ffc, &param, i * 10 + 5, y, g_menuTextColors[0]);
            money %= div;
            div /= 10;
        }
    }
    drawMenuFrame(0xa8, y, 0x58, 0x20, -1);
}

THUMB void drawActiveCharaHp(int flag)
{
    int chara = MaterielMenuPlayerControl::getSingleton()->activeChara_;
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(chara)->haveStatusInfo_;
    int param[2];
    if (chara == status::g_Party.getCount()) {
        param[0] = 0x5000005b;
        drawMenuFrame(0x48, 0xa0, 0x40, 0x20, -1);
        drawMenuPartsColored(s_parts_0218106c, param, g_menuTextColors[0]);
        return;
    }
    param[0] = info->haveStatus_.playerIndex_ + 0x50000000;
    param[1] = info->getHp() * 100 / info->getHpMax();
    if (param[1] <= 0 && info->getHp() > 0) {
        param[1] = 1;
    }
    if (flag) {
        drawMenuFrame(0, 0, 0x40, 0x20, -1);
        drawMenuPartsColored(s_parts_021814a2, param, g_menuTextColors[getHpTextColorIndex(info)]);
    } else {
        drawMenuFrame(0x48, 0xa0, 0x40, 0x20, -1);
        drawMenuPartsColored(s_parts_0218146a, param, g_menuTextColors[getHpTextColorIndex(info)]);
    }
}

THUMB void drawCommandCursor(int x, int y)
{
    drawMenuPartsAt(s_parts_02180f54, 0, x, y);
}

#pragma dont_inline on
THUMB void drawCharaStatusList(int chara)
{
    status::HaveStatusInfo* info = getPlayerHaveStatusInfo(status::g_Party.getPlayerStatus(chara));
    int param[3];
    param[1] = (int)":";
    if (status::g_Game.unkfunc_0216e3f0() == 2) {
        param[1] = (int)" :";
    }
    for (int i = 0; i < 9; i++) {
        if (i == 5 || i == 6) {
            param[0] = menu::MenuDataCommon::getAbilityKind(i - 5);
            param[2] = menu::MenuDataCommon::getStatus(i - 5, *info);
        } else if (i >= 7) {
            param[0] = menu::MenuDataCommon::getAbilityKind(i);
            param[2] = menu::MenuDataCommon::getStatus(i, *info);
        } else {
            param[0] = menu::MenuDataCommon::getAbilityKind(i + 2);
            param[2] = menu::MenuDataCommon::getStatus(i + 2, *info);
        }
        drawPartsInTextColorAt(s_parts_021814da, param, 0, i * 14 + 0x18, 0);
    }
    param[0] = 0xa0000010;
    param[2] = info->getExp();
    drawPartsInTextColor(s_parts_02181512, param, 0);
    drawMenuFrame(0x90, 0x10, 0x68, 0xa0, -1);
}
#pragma dont_inline reset

THUMB void drawActiveCharaHpMp()
{
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(MaterielMenuPlayerControl::getSingleton()->activeChara_)->haveStatusInfo_;
    int param[13];
    param[0] = info->haveStatus_.playerIndex_ + 0x50000000;
    param[1] = 0xa0000001;
    param[2] = 0xa0000002;
    param[3] = info->getHp();
    param[4] = (int)"@\x81\x5c";
    param[5] = (int)"@\x81\x5c";
    param[6] = (int)"@\x81\x5c";
    param[7] = info->getHpMax();
    param[8] = info->getMp();
    param[9] = (int)"@\x81\x5c";
    param[10] = (int)"@\x81\x5c";
    param[11] = (int)"@\x81\x5c";
    param[12] = info->getMpMax();
    drawMenuPartsColored(s_parts_02181cc8, param, g_menuTextColors[getHpTextColorIndex(info)]);
    if (getStatusCondition(info) == 3) {
        param[0] = menu::MenuDataCommon::convMessage(info, 0xa0000018);
        drawMenuPartsColored(s_parts_021811d8, param, g_menuTextColors[getHpTextColorIndex(info)]);
    } else {
        param[0] = 0xa000003f;
        param[1] = (int)":";
        param[2] = info->haveStatus_.level_;
        drawMenuPartsColored(s_parts_0218154a, param, g_menuTextColors[getHpTextColorIndex(info)]);
    }
    drawMenuFrame(0, 0, 0x40, 0x68, -1);
}

THUMB void drawNeneItemPage(int count, int page, int pageMax)
{
    int num = 6;
    if (page == pageMax) {
        num = count - pageMax * 6;
    }
    for (int i = 0; i < num; i++) {
        int param[4];
        param[0] = status::g_Shop.haveItemNene_.getItem(page * 6 + i) + 0x40000000;
        param[1] = status::g_Shop.haveItemNene_.getItem(page * 6 + i);
        if (status::g_Shop.haveItemNene_.getItemCount(page * 6 + i) <= 1) {
            param[2] = 0;
            s_parts_0218175e[3].type_ = 0;
        } else {
            param[2] = status::g_Shop.haveItemNene_.getItemCount(page * 6 + i);
            s_parts_0218175e[3].type_ = 0xf;
        }
        drawMenuParts(s_parts_0218175e, param, (i % 2) * 0x78 + 0x14, (i / 2) * 32 + 4, g_menuTextColors[0]);
    }
    drawMenuFrame(0, 0, 0x100, 0xa0, 0x70);
}

THUMB void drawChapterTitleText(int chapter, int chapterEnd)
{
    int param = 0xa000032a;
    if (chapter == 0 && chapterEnd == 1) {
        param = 0x30db7;
        drawMenuParts(s_parts_02181130, &param, 0, -16, g_menuTextColors[0]);
    } else if (chapter == 0) {
        drawMenuPartsColored(s_parts_02181114, &s_int_02180e94[chapter], g_menuTextColors[0]);
    } else {
        int y = 0;
        if (chapterEnd == 0) {
            if (chapter == 1) y = -25;
            if (chapter == 2) y = -25;
            if (chapter == 3) y = -25;
            if (chapter == 4) y = 0x4a;
            if (chapter == 5) y = 0x4a;
            if (chapter == 6) y = 0x4a;
        }
        drawMenuParts(s_parts_0218114c, &s_int_02180e94[chapter], 0, y, g_menuTextColors[0]);
    }
    if (chapter != 0 && chapterEnd == 1) {
        param = 0xa000032a;
        drawMenuPartsColored(s_parts_02180eac, &param, g_menuTextColors[0]);
    }
}

THUMB void drawTagModeRootMenu()
{
    drawMenuPartsPlain(s_parts_02181018, 0);
    for (int i = 0; i < 5; i++) {
        int param = menu::MenuDataCommon::getSuretigaiRootMenu(i);
        drawMenuParts(s_parts_021811a0, &param, 0, i * 16 + 0xc + i * 4, g_menuTextColors[0]);
    }
    drawMenuFrame(0, 0, 0x88, 0x78, -1);
}

THUMB void drawTagModeTenantList(int count, int page, int pageMax, int flag)
{
    int param[5] = { 0, 0, 0, 0, 0 };
    g_EnvoyManager.getEnvoyCount();
    int num = g_EnvoyManager.getEnvoyCount() - page * 8;
    if (num <= 0) {
        int offset = 8 - g_EnvoyManager.getEnvoyCount() % 8;
        if (offset == 8) {
            offset = 0;
        }
        num = dss::max(dss::min(count - page * 8, 8), 0);
        for (int i = 0; i < num; i++) {
            int pages = (g_EnvoyManager.getEnvoyCount() - 1) / 8 + 1;
            if (g_EnvoyManager.getEnvoyCount() == 0) {
                pages = 0;
            }
            int index = offset + (i + (page - pages) * 8);
            int x = (i % 2) * 0x78 + 0x14;
            int y = (i / 2) * 32 + 0x20;
            if (status::excelParam.surechigaiTenant_[index].index >= 4 && g_AreaFlag.check(0x1fd) == 1) {
                index++;
            }
            if (status::excelParam.surechigaiTenant_[index].index == 0x16 && g_AreaFlag.check(0x1b8) == 0) {
                index++;
            }
            if (status::excelParam.surechigaiTenant_[index].index == 0x17 && g_AreaFlag.check(0x1fd) == 0) {
                break;
            }
            param[0] = menu::MenuDataCommon::getItemMessage(status::excelParam.surechigaiTenant_[index].name);
            param[1] = status::excelParam.surechigaiTenant_[index].icon + 0x37;
            drawMenuParts(s_parts_0218162a, param, x, y, g_menuTextColors[0]);
        }
    } else {
        num = dss::max(dss::min(num, 8), 0);
        int i;
        for (i = 0; i < num; i++) {
            g_EnvoyManager.selectEnvoy(page * 8 + i);
            param[0] = (int)g_EnvoyManager.getName();
            param[1] = g_EnvoyManager.getType() + 1;
            drawMenuParts(s_parts_0218162a, param, (i % 2) * 0x78 + 0x14, (i / 2) * 32 + 0x20, g_menuTextColors[0]);
        }
        if (num < 8 && flag == 1) {
            int end = count;
            if (end > 7) {
                end = 8;
            }
            for (i = num; i < end; i++) {
                int x = (i % 2) * 0x78 + 0x14;
                int y = (i / 2) * 32 + 0x20;
                int index = i - num;
                if (status::excelParam.surechigaiTenant_[index].index >= 4 && g_AreaFlag.check(0x1fd) == 1) {
                    index++;
                }
                if (status::excelParam.surechigaiTenant_[index].index == 0x16 && g_AreaFlag.check(0x1b8) == 0) {
                    index++;
                }
                if (status::excelParam.surechigaiTenant_[index].index == 0x17 && g_AreaFlag.check(0x1fd) == 0) {
                    break;
                }
                param[0] = menu::MenuDataCommon::getItemMessage(status::excelParam.surechigaiTenant_[index].name);
                param[1] = status::excelParam.surechigaiTenant_[index].icon + 0x37;
                drawMenuParts(s_parts_0218162a, param, x, y, g_menuTextColors[0]);
            }
        }
    }
    if (flag == 1) {
        param[0] = 0x800001f6;
    } else {
        param[0] = 0x800001f7;
    }
    param[1] = (int)"\xff\xfe\xac\x24";
    param[2] = 0xa00002ce;
    drawMenuPartsColored(s_parts_0218169a, param, g_menuTextColors[0]);
    if (count > 8) {
        param[0] = 0xe;
        param[1] = page + 1;
        param[2] = (int)"\xff\xfe\xa7\x24";
        param[3] = pageMax;
        param[4] = 0xf;
        drawMenuPartsColored(s_parts_02181876, param, g_menuTextColors[0]);
    }
    drawMenuFrame(0, 0, 0x100, 0xc0, 0x20);
}

THUMB void drawTagModeTenantDetail(int index, int mode, int flag)
{
    setMenuDrawOffset(0, 0);
    if (mode == 0 && g_EnvoyManager.getEnvoyCount() == 0 && flag == 0) {
        return;
    }
    int param[14] = { 0 };
    for (int i = 0; i < 14; i++) {
        param[i] = 0;
    }
    param[1] = 0xa0000004;
    param[2] = (int)":";
    param[4] = 0xa00002c9;
    param[5] = (int)":";
    param[7] = 0xa00002ca;
    param[8] = (int)":";
    param[10] = 0xa00002cc;
    param[11] = (int)":";
    if (flag == 1) {
        if (status::excelParam.surechigaiTenant_[index].index >= 4 && g_AreaFlag.check(0x1fd) == 1) {
            index++;
        }
        if (status::excelParam.surechigaiTenant_[index].index == 0x16 && g_AreaFlag.check(0x1b8) == 0) {
            index++;
        }
        if (status::excelParam.surechigaiTenant_[index].index == 0x17 && g_AreaFlag.check(0x1fd) == 0) {
            return;
        }
        param[0] = menu::MenuDataCommon::getItemMessage(status::excelParam.surechigaiTenant_[index].name);
        param[3] = menu::MenuDataCommon::getSuretigaiSex((char)(status::excelParam.surechigaiTenant_[index].byte_1 & 3));
        param[6] = (char)((status::excelParam.surechigaiTenant_[index].byte_1 & 0xe0) >> 5) + 0xa00002c1;
        param[9] = status::excelParam.surechigaiTenant_[index].tokugi + 0xe0000001;
        if (status::excelParam.surechigaiTenant_[index].from == 0x1d) {
            g_EnvoyManager.mode_ = 1;
            param[12] = (int)g_EnvoyManager.getTownName();
        } else {
            param[12] = menu::MenuDataCommon::getRuraName(status::excelParam.surechigaiTenant_[index].from);
        }
        drawMenuPartsColored(s_parts_02181c04, param, g_menuTextColors[0]);
        drawMenuFrame(0, 0x50, 0xb0, 0x70, 0x70);
        return;
    }
    param[13] = 0;
    if (mode == 1) {
        g_EnvoyManager.mode_ = 1;
    } else {
        g_EnvoyManager.selectEnvoy(index);
    }
    param[0] = (int)g_EnvoyManager.getName();
    param[3] = menu::MenuDataCommon::getSuretigaiSex(g_EnvoyManager.getSex());
    param[6] = g_EnvoyManager.getAge() + 0xa00002c1;
    param[9] = g_EnvoyManager.getSkill() + 0xe0000001;
    param[12] = (int)g_EnvoyManager.getTownName();
    drawMenuPartsColored(s_parts_02181c04, param, g_menuTextColors[0]);
    param[0] = 0xa00002cb;
    param[1] = (int)":";
    param[2] = (int)g_EnvoyManager.getHeroName();
    drawMenuPartsColored(s_parts_021813fa, param, g_menuTextColors[0]);
    drawMenuFrame(0, 0x50, 0xb0, 0x70, 0x70);
}

THUMB void drawTagModeProfile(int index, int mode)
{
    int param[17] = { 0 };
    for (int i = 0; i < 17; i++) {
        param[i] = 0;
    }
    param[1] = 0xa0000004;
    param[2] = (int)":";
    param[4] = 0xa00002c9;
    param[5] = (int)":";
    param[7] = 0xa00002ca;
    param[8] = (int)":";
    param[10] = 0xa00002cb;
    param[11] = (int)":";
    param[13] = 0xa00002cc;
    param[14] = (int)":";
    if (mode == 1) {
        g_EnvoyManager.mode_ = 1;
    } else {
        g_EnvoyManager.selectEnvoy(index);
    }
    param[0] = (int)g_EnvoyManager.getName();
    param[3] = menu::MenuDataCommon::getSuretigaiSex(g_EnvoyManager.getSex());
    param[6] = g_EnvoyManager.getAge() + 0xa00002c1;
    param[9] = g_EnvoyManager.getSkill() + 0xe0000001;
    param[12] = (int)g_EnvoyManager.getHeroName();
    param[15] = (int)g_EnvoyManager.getTownName();
    param[16] = g_EnvoyManager.getType() + 1;
    drawMenuPartsColored(s_parts_02181d8c, param, g_menuTextColors[0]);
    drawMenuFrame(0, 8, 0xc0, 0x70, 0x28);
}

THUMB void drawTagModeIconGrid(int page, int value)
{
    int num = 0xc;
    if (page == 4) {
        num = 2;
    }
    for (int i = 0; i < num; i++) {
        int param = page * 12 + i + 1;
        int x = (i % 6) * 0x28 + 0xc;
        int y = (i / 6) * 0x28 + 0x20;
        if (g_EnvoyManager.isTypeSelectable(page * 12 + i) == 1) {
            drawMenuParts(s_parts_02181034, &param, x, y, g_menuTextColors[0]);
        }
        drawMenuParts(s_parts_021810a4, 0, x, y, g_menuTextColors[0]);
    }
    int msg = 0xa00002bc;
    drawMenuPartsColored(s_parts_02180f70, &msg, g_menuTextColors[0]);
    int param2[5] = { 0xe, page + 1, (int)"\xff\xfe\xa7\x24", value, 0xf };
    drawMenuPartsColored(s_parts_021818ca, param2, g_menuTextColors[0]);
    drawMenuFrame(0, 0, 0x100, 0x78, 0x20);
}

THUMB void drawNameEditIcon(int active, int y)
{
    int param = active + 1;
    drawMenuPartsAt(s_parts_021812fe, &param, 0xc, y);
    drawMenuFrame(0, y, 0x38, 0x30, -1);
}

THUMB void drawNameEditSex()
{
    g_EnvoyManager.mode_ = 1;
    drawNameEditName(g_EnvoyManager.getName(), 1);
    for (int i = 0; i < 3; i++) {
        int param = menu::MenuDataCommon::getSuretigaiSex(i);
        drawMenuParts(s_parts_021810f8, &param, i * 0x48 + 0x18, 0x54, g_menuTextColors[0]);
    }
    int msg = 0xa00002bd;
    drawMenuPartsColored(s_parts_021810dc, &msg, g_menuTextColors[0]);
    drawMenuFrame(0, 0x30, 0x100, 0x40, 0x48);
}

THUMB void drawNameEditAge()
{
    g_EnvoyManager.mode_ = 1;
    drawNameEditName(g_EnvoyManager.getName(), 0);
    int param = menu::MenuDataCommon::getSuretigaiSex(g_EnvoyManager.getSex());
    drawMenuPartsColored(s_parts_02181050, &param, g_menuTextColors[0]);
    drawMenuFrame(0x80, 0x10, 0x40, 0x20, -1);
    for (int i = 0; i < 8; i++) {
        int aetas = i + 0xa00002c1;
        drawMenuParts(s_parts_02180f38, &aetas, (i % 4) * 0x38 + 0x14, (i / 4) * 0x18 + 0x54, g_menuTextColors[0]);
    }
    param = 0xa00002be;
    drawMenuPartsColored(s_parts_02180ee4, &param, g_menuTextColors[0]);
    drawMenuFrame(0, 0x30, 0x100, 0x58, 0x48);
}

THUMB void drawNameEditSkill()
{
    g_EnvoyManager.mode_ = 1;
    int param[5] = { 0 };
    drawNameEditName(g_EnvoyManager.getName(), 0);
    param[0] = menu::MenuDataCommon::getSuretigaiSex(g_EnvoyManager.getSex());
    drawMenuParts(s_parts_02181184, param, 0x80, 0, g_menuTextColors[0]);
    drawMenuFrame(0x80, 0x10, 0x40, 0x20, -1);
    param[0] = g_EnvoyManager.getAge() + 0xa00002c1;
    drawMenuParts(s_parts_02181184, param, 0xc0, 0, g_menuTextColors[0]);
    drawMenuFrame(0xc0, 0x10, 0x40, 0x20, -1);
    int page = MaterielMenuPlayerControl::getSingleton()->activeChiausSkillPage_;
    for (int i = 0; i < 8; i++) {
        int skill = page * 8 + i + 0xe0000001;
        drawMenuParts(s_parts_021811f4, &skill, (i % 2) * 0x78 + 0x10, (i / 2) * 0x18 + 0x5c, g_menuTextColors[0]);
    }
    param[0] = 0xc;
    param[1] = page + 1;
    param[2] = (int)"\xff\xfe\xa7\x24";
    param[3] = 8;
    param[4] = 0xd;
    drawMenuPartsColored(s_parts_0218191e, param, g_menuTextColors[0]);
    param[0] = 0xa00002bf;
    drawMenuPartsColored(s_parts_02180fc4, param, g_menuTextColors[0]);
    drawMenuFrame(0, 0x30, 0x100, 0x90, 0x50);
}

THUMB void drawNameEditComment()
{
    int param[3];
    char line1[0x40];
    char line2[0x40];
    char line3[0x40];
    g_EnvoyManager.mode_ = 1;
    splitCommentLines(line1, line2, line3, (char*)g_EnvoyManager.getComment());
    param[0] = (int)line1;
    param[1] = (int)line2;
    param[2] = (int)line3;
    drawMenuPartsColored(s_parts_021815f2, param, g_menuTextColors[0]);
    param[0] = 0xa00002d9;
    drawMenuPartsColored(s_parts_02180f00, param, g_menuTextColors[0]);
    drawMenuFrame(0, 8, 0xc0, 0x70, 0x20);
}

THUMB void drawSideJobItems()
{
    int i = 0;
    int y = 8;
    for (; i < 6; i++) {
        if (status::g_Shop.sideJobItemFlag_[i] == 1) {
            int param[2];
            param[0] = status::g_Shop.getShopItem(2, i) + 0x40000000;
            param[1] = status::g_Shop.getShopPrice(2, i);
            drawMenuParts(s_parts_02181280, param, 0, y, g_menuTextColors[0]);
            y += 0x10;
        }
    }
    drawMenuFrame(0, 0, 0xb8, 0x70, -1);
}

THUMB void drawShopItemHeader(int item)
{
    setMenuDrawOffset(0, 0);
    int active = MaterielMenuPlayerControl::getSingleton()->activeItem_;
    if (item == -1) {
        item = MaterielMenu_SHOP_MANAGER::getSingleton()->getItem(active);
    }
    s_parts_021812d4[0].type_ = 0xd;
    s_parts_021812d4[1].type_ = 0;
    int param[2];
    if (status::UseItem::getItemType(item) <= 4) {
        if (status::UseItem::getEquipType(item) == 4) {
            param[0] = menu::MenuDataCommon::getAbilityKind(6);
        } else {
            param[0] = menu::MenuDataCommon::getAbilityKind(status::UseItem::getEquipType(item));
        }
    } else {
        param[0] = 0xa0000082;
    }
    param[1] = 0;
    drawMenuPartsColored(s_parts_021812d4, param, g_menuTextColors[0]);
    s_parts_021812d4[0].type_ = 0;
    s_parts_021812d4[1].type_ = 0xd;
    param[0] = 0;
    if (MaterielMenu_SHOP_MANAGER::getSingleton()->getExtraShop() == 1) {
        if (active == 0) {
            param[1] = 0x2000006f;
        } else {
            param[1] = 0x20000007;
        }
    } else {
        param[1] = item + 0x20000000;
    }
    drawMenuPartsColored(s_parts_021812d4, param, g_menuTextColors[0]);
}

THUMB int getShopEquipCompare(status::HaveStatusInfo* status, int* param, int item)
{
    param[0] = status->haveStatus_.playerIndex_ + 0x50000000;
    if (item == -1) {
        MaterielMenuPlayerControl* ctrl = MaterielMenuPlayerControl::getSingleton();
        item = MaterielMenu_SHOP_MANAGER::getSingleton()->getItem(ctrl->activeItem_);
        if (MaterielMenu_SHOP_MANAGER::getSingleton()->getExtraShop() == 1) {
            item = 7;
        }
    }
    if (!status->isEquipEnable(item)) {
        param[1] = 0xa0000067;
        return 0;
    }
    switch (status::UseItem::getEquipType(item)) {
    case 2:
        if (status->haveEquipment_.isEquipment(item)) {
            param[1] = 0xa0000041;
            param[2] = status->getStrength(0);
            return 2;
        }
        param[1] = status->getStrength(0);
        param[3] = status->getChangeStrength(item);
        break;
    case 0:
        if (status->haveEquipment_.isEquipment(item)) {
            param[1] = 0xa0000041;
            param[2] = status->getAttack(0);
            return 2;
        }
        param[1] = status->getAttack(0);
        param[3] = status->getChangeAttack(item);
        break;
    case 1:
        if (status->haveEquipment_.isEquipment(item)) {
            param[1] = 0xa0000041;
            param[2] = status->getDefence(0);
            return 2;
        }
        param[1] = status->getDefence(0);
        param[3] = status->getChangeDefence(item);
        break;
    case 3:
        if (status->haveEquipment_.isEquipment(item)) {
            param[1] = 0xa0000041;
            param[2] = status->getAgility(0);
            return 2;
        }
        param[1] = status->getAgility(0);
        param[3] = status->getChangeAgility(item);
        break;
    case 5:
        if (status->haveEquipment_.isEquipment(item)) {
            param[1] = 0xa0000041;
            param[2] = status->getWisdom(0);
            return 2;
        }
        param[1] = status->getWisdom(0);
        param[3] = status->getChangeWisdom(item);
        break;
    case 4:
        if (status->haveEquipment_.isEquipment(item)) {
            param[1] = 0xa0000041;
            param[2] = status->getLuck(0);
            return 2;
        }
        param[1] = status->getLuck(0);
        param[3] = status->getChangeLuck(item);
        break;
    }
    param[2] = (int)"\xff\xfe\xaf\x24";
    return 1;
}

THUMB int getShopHeldCount(status::HaveStatusInfo* status, int* param, int item)
{
    param[0] = status->haveStatus_.playerIndex_ + 0x50000000;
    int active = MaterielMenuPlayerControl::getSingleton()->activeItem_;
    if (item == -1) {
        item = MaterielMenu_SHOP_MANAGER::getSingleton()->getItem(active);
    }
    int count = 0;
    if (MaterielMenu_SHOP_MANAGER::getSingleton()->getExtraShop() == 1) {
        if (active == 0) {
            item = 0x6f;
        } else {
            item = 7;
        }
    }
    for (int i = 0; i < status->haveItem_.getCount(); i++) {
        if (item == status->haveItem_.getItem(i)) {
            count++;
        }
    }
    if (count > 0) {
        param[1] = (int)"\xff\xfe\xa9\x24";
        param[2] = count;
        return 1;
    }
    param[1] = 0xa0000077;
    return 0;
}

THUMB int activeCharaHasItem(int itemID)
{
    int j;
    int chara = MaterielMenuPlayerControl::getSingleton()->activeChara_;
    if (status::g_Party.fukuro_ != 0 && chara == status::g_Party.getCount()) {
        for (int i = 0; i < status::g_Party.haveItemSack_.getCount(); i++) {
            if (itemID == status::g_Party.haveItemSack_.getItem(i)) {
                return 1;
            }
        }
    } else {
        for (j = 0; j < status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveItem_.getCount(); j++) {
            if (itemID == status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveItem_.getItem(j)) {
                return 1;
            }
        }
    }
    return 0;
}

THUMB void getSellListRow(int* param, int chara, int index)
{
    if (chara == status::g_Party.getCount()) {
        status::HaveItemSack* sack = &status::g_Party.haveItemSack_;
        param[0] = 0;
        param[1] = sack->getItem(index) + 0x40000000;
        if (status::UseItem::getSellType(sack->getItem(index)) == 1) {
            param[3] = 0xa000007f;
            param[2] = param[4] = param[5] = 0;
            return;
        }
        param[2] = status::UseItem::getSellPrice(sack->getItem(index));
        param[3] = 0xa000003c;
        param[4] = (int)"\xff\xfe\xa9\x24";
        param[5] = sack->getItemCount(index);
        if (param[5] > 99) {
            param[5] = 99;
        }
        return;
    }
    status::HaveItem* items = &status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveItem_;
    param[0] = items->isEquipment(index) ? 0xa0000041 : 0;
    if (status::UseItem::getSellType(items->getItem(index)) == 1) {
        param[3] = 0xa000007f;
        param[2] = param[4] = param[5] = 0;
    } else {
        param[2] = status::UseItem::getSellPrice(items->getItem(index));
        param[3] = 0xa000003c;
        param[4] = param[5] = 0;
    }
    param[1] = items->getItem(index) + 0x40000000;
}

THUMB void drawNameEditName(unsigned char* name, int flag)
{
    int param[1] = { 0 };
    param[0] = (int)name;
    if (flag == 1) {
        drawMenuPartsColored(s_parts_021810c0, param, g_menuTextColors[0]);
        drawMenuFrame(0x38, 0x10, 0xa0, 0x20, -1);
    } else {
        drawMenuPartsColored(s_parts_02180f1c, param, g_menuTextColors[0]);
        drawMenuFrame(0x38, 0x10, 0x48, 0x20, -1);
    }
}

THUMB void splitCommentLines(char* line1, char* line2, char* line3, char* comment)
{
    Utf8Iterator src;
    src.setText(comment);
    Utf8Iterator dst1;
    dst1.initBuffer(line1, 0x400);
    Utf8Iterator dst2;
    dst2.initBuffer(line2, 0x400);
    Utf8Iterator dst3;
    dst3.initBuffer(line3, 0x400);
    for (int i = 0; ; i++) {
        int c = src.getChar();
        src.nextChar();
        if (c == 0) {
            break;
        }
        dst1.putChar(c);
        if (i == 0xe) {
            break;
        }
    }
    for (int i = 0; ; i++) {
        int c = src.getChar();
        src.nextChar();
        if (c == 0) {
            break;
        }
        dst2.putChar(c);
        if (i == 0xe) {
            break;
        }
    }
    for (int i = 0; ; i++) {
        int c = src.getChar();
        src.nextChar();
        if (c == 0) {
            break;
        }
        dst3.putChar(c);
        if (i == 0xe) {
            break;
        }
    }
}

// not in the ROM (dead-stripped), its local array initializer is still in .rodata
THUMB void unkfunc_unused_7(int index)
{
    int message[5] = { 0x800000cb, 0x800000cc, 0x800000cd, 0x800000ce, 0x800000cf };
    drawMenuPartsColored(s_parts_02181210, &message[index], 1);
}

// not in the ROM (dead-stripped), keeps the unused parts lists of this file and its const locals
THUMB void unkfunc_unused_8(int* param)
{
    const int unk0 = 1;
    const int unk1 = 0;
    drawMenuPartsPlain(s_parts_02180fa8, param);
    drawMenuPartsPlain(s_parts_021817ea, param);
    drawMenuPartsPlain(s_parts_021819c6, param);
}
