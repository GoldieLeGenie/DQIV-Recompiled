#pragma ipa file
#include "ov016/UnkTownMenuDraw/UnkTownMenuDraw_0217ad94.hpp"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/menu/MenuDataCommon.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/UseItem.hpp"
#include "main/status/UseAction.hpp"
#include "main/status/ShopList.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/status/GameStatus.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/cmn/CommonRuraData.hpp"
#include "main/profile/Profile.hpp"
#include "ov016/TownMenuPlayerControl/TownMenuPlayerControl.hpp"
#include "ov016/status/FukuroItemInfo.hpp"
#include "ov016/status/PlayerItemInfo.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216ce4c.hpp"
#include "main/menu/UnkMenuPartsDraw.hpp"
#include "main/menu/UnkMenuCommonDraw_0201e194.hpp"
#include "main/cmn/UnkEnvoyManager.hpp"
static UnkMenuParts s_parts_02184b8c[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0x80, 0xa0, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184ac8[] = {
    { 0x0a, 0x00, 0, 0, 0, 0, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184b54[] = {
    { 0x0d, 0x09, (short)0xf000, 0, 8, 0, 0xe8, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184f7c[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0xa0, 0xaa, 0x10, 0x10 },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 0x10, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 2, 0xc0, 0xaa, 0x38, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184c50[] = {
    { 0x0f, 0x0a, (short)0xf000, 0, 0, 0x5b, 8, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02185732[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0xa, 0x50, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 1, 0x50, 0xa, 0x18, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 2, 0x60, 0xa, 0x30, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 3, 0x38, 0x24, 0x50, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 4, 0x38, 0x34, 0x28, 0x10 },
    { 0x0d, 0x0b, (short)0xf000, 5, 6, 0, 8, 0x10 },
    { 0x0d, 0x0b, (short)0xf000, 6, 6, 0, 0x28, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 7, 0x38, 0x42, 0x28, 0x10 },
    { 0x0d, 0x0b, (short)0xf000, 8, 6, 0, 8, 0x10 },
    { 0x0f, 0x0b, (short)0xf000, 9, 6, 0, 0x20, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 0xa, 0x38, 0x4f, 0x18, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 0xb, 0x48, 0x4f, 0x20, 0x10 },
    { 0x0d, 0x0b, (short)0xf000, 0xc, 4, 0, 8, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 0xd, 0x68, 0x4f, 0x20, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 0xe, 0x38, 0x5c, 0x18, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 0xf, 0x48, 0x5c, 0x20, 0x10 },
    { 0x0d, 0x0b, (short)0xf000, 0x10, 4, 0, 8, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 0x11, 0x68, 0x5c, 0x20, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184c6c[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 2, 0x58, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184df4[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x10, 0x6a, 0x10, 0x10 },
    { 0x0d, 0x0b, (short)0xf000, 1, 4, 0, 0x60, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184e48[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0xaa, 0x80, 0x10 },
    { 0x0d, 0x0a, (short)0xf000, 1, 0x88, 0xaa, 0x10, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184cf8[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0x80, 0xa0, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 1, 8, 0x98, 0xa0, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184ae4[] = {
    { 0x15, 0xb5, 0, 0, 0, 0, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184c88[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x50, 0xaa, 0x28, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184a90[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x38, 0x18, 0xe, 0xe },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184d22[] = {
    { 0x01, 0x00, (short)0xf000, 0, 0, 0, 0x58, 0x20 },
    { 0x07, 0x00, (short)0xf000, 0x19, 0x50, 8, 8, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184aac[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x20, 0, 8, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184d4c[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 2, 0x58, 0x10 },
    { 0x0d, 0x0a, (short)0xf000, 1, 0x60, 2, 0x10, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184be0[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 2, 0x58, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02185572[] = {
    { 0x14, 0xc0, 0, -1, 0, 0, 0x70, 0x38 },
    { 0x0c, 0xf1, 0, 0, -4, 0x10, 0x20, 0x20 },
    { 0x0d, 0x08, (short)0xf000, 1, 0x1c, 3, 0x50, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 2, 0x1f, 0x27, 0x50, 0x10 },
    { 0x0d, 0x0b, (short)0xf000, 0xd, 0, 0, 0x10, 0x10 },
    { 0x0d, 0x09, (short)0xf000, 3, 0x1c, 0x10, 0x10, 0x10 },
    { 0x0d, 0x09, (short)0xf000, 4, 0x48, 0x10, 0x10, 0x10 },
    { 0x0d, 0x09, (short)0xf000, 5, 0x1c, 0x1a, 0x10, 0x10 },
    { 0x0d, 0x09, (short)0xf000, 6, 0x48, 0x1a, 0x10, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 7, 0x28, 0x27, 0x18, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 8, 0x30, 0x10, 0x18, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 9, 0x50, 0x10, 0x18, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 0xa, 0x30, 0x1a, 0x18, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 0xb, 0x50, 0x1a, 0x18, 0x10 },
    { 0x0f, 0x09, (short)0xf000, 0xc, 0, 3, 0x20, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0218514a[] = {
    { 0x0f, 0x0a, (short)0xf000, 0, 0x12, 0, 0x12, 0x10 },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 0x12, 0x10 },
    { 0x0f, 0x0b, (short)0xf000, 2, 0, 0, 0x12, 0x10 },
    { 0x0c, 0xf4, 0, 3, 0x28, 8, 0x18, 0x18 },
    { 0x0c, 0xf4, 0, 4, 8, 8, 0x18, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0218519e[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0xc, 8, 0x40, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 1, 8, 0x18, 0x30, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 2, 0x18, 0x24, 0x20, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 3, 8, 0x30, 0x28, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 4, 0x18, 0x3c, 0x20, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184da0[] = {
    { 0x14, 0xc0, 0, -1, 4, 0x10, 0x20, 0x18 },
    { 0x0c, 0xf1, 0, 0, 0, 0, 0x20, 0x20 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02185078[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0xa, 0x10, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 1, 0x10, 0xa, 0x20, 0xa },
    { 0x0d, 0x08, (short)0xf000, 2, 0x34, 0xa, 0x10, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 3, 0x30, 0xa, 0x20, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184dca[] = {
    { 0x0f, 0x08, (short)0xf000, 0, 0, 0, 0x10, 0x10 },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 0x10, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184b00[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 2, 0x40, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021850be[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0, 0, 0x30, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 1, 0, 0x10, 0x10, 0x10 },
    { 0x0d, 0x0b, (short)0xf000, 2, 0, 0, 0x10, 0x10 },
    { 0x0f, 0x0b, (short)0xf000, 3, 0, 0, 0x10, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184bc4[] = {
    { 0x0d, 0x09, (short)0xf000, 0, 0x94, 0x52, 0x48, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184b38[] = {
    { 0x0f, 0x0a, (short)0xf000, 0, 0, 0, 0x10, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184e9c[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0x80, 0xa0, 0xc },
    { 0x0d, 0x08, (short)0xf000, 1, 8, 0x98, 0xa0, 0xc },
    { 0x0d, 0x08, (short)0xf000, 2, 8, 0xa8, 0xa0, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0218529a[] = {
    { 0x0f, 0x08, (short)0xf000, 0, 8, 8, 0x10, 0x10 },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 0x10, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 2, 8, 0x18, 0x40, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 3, 8, 0x28, 0x30, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 4, 0x18, 0x34, 0x20, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 5, 8, 0x40, 0x30, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 6, 0x18, 0x4c, 0x20, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02185652[] = {
    { 0x0f, 0x08, (short)0xf000, 0, 8, 8, 0x10, 0x10 },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 0x10, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 2, 8, 0x18, 0x40, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 3, 8, 0x2c, 0x18, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 4, 8, 0x4c, 0x18, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 5, 0x18, 0x26, 0x20, 0x10 },
    { 0x0d, 0x0a, (short)0xf000, 6, 0x20, 0x30, 8, 8 },
    { 0x0d, 0x0a, (short)0xf000, 7, 0x28, 0x30, 8, 8 },
    { 0x0d, 0x0a, (short)0xf000, 8, 0x30, 0x30, 8, 8 },
    { 0x0f, 0x0a, (short)0xf000, 9, 0x18, 0x38, 0x20, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 0xa, 0x18, 0x46, 0x20, 0x10 },
    { 0x0d, 0x0a, (short)0xf000, 0xb, 0x20, 0x50, 8, 8 },
    { 0x0d, 0x0a, (short)0xf000, 0xc, 0x28, 0x50, 8, 8 },
    { 0x0d, 0x0a, (short)0xf000, 0xd, 0x30, 0x50, 8, 8 },
    { 0x0f, 0x0a, (short)0xf000, 0xe, 0x18, 0x58, 0x20, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184e1e[] = {
    { 0x0f, 0x0a, (short)0xf000, 0, 0xb6, 0x8a, 0x38, 0x10 },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 0x10, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021853f8[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x10, 8, 0x80, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 1, 0xa0, 8, 0x10, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 2, 0xa8, 8, 0x40, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 3, 0x10, 0x18, 0x90, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 4, 0xa0, 0x18, 0x10, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 5, 0xa8, 0x18, 0x40, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 6, 0x10, 0x30, 0x40, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 7, 0xa0, 0x30, 0x48, 0x10 },
    { 0x0d, 0x0a, (short)0xf000, 8, 0xe8, 0x30, 0x10, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 9, 0x10, 0x40, 0x40, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 0xa, 0x90, 0x40, 0x58, 0x10 },
    { 0x0d, 0x0a, (short)0xf000, 0xb, 0xe8, 0x40, 0x10, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02185246[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0x80, 0xa0, 0xc },
    { 0x0d, 0x08, (short)0xf000, 1, 8, 0x98, 0x48, 0xc },
    { 0x0f, 0x0a, (short)0xf000, 2, 8, 0xa8, 0x18, 0xc },
    { 0x0d, 0x08, (short)0xf000, 3, 0x28, 0xa8, 0x10, 0xc },
    { 0x0f, 0x0a, (short)0xf000, 4, 0x30, 0xa8, 0x20, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184cdc[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x20, 0, 0x80, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184f0c[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0xa0, 8, 0x30, 0x10 },
    { 0x0d, 0x0a, (short)0xf000, 1, 0xd8, 8, 8, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 2, 0xe0, 8, 0x18, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184ca4[] = {
    { 0x0d, 0x09, (short)0xf000, 0, 8, 0x10, 0xf0, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184bfc[] = {
    { 0x0d, 0x09, (short)0xf000, 0, 8, 0x2c, 0xf8, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184c34[] = {
    { 0x0e, 0x08, (short)0xf000, 0, 8, 0, 0x38, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184fb4[] = {
    { 0x14, 0xc0, 0, -1, 0, 8, 0x68, 0x18 },
    { 0x0e, 0x08, (short)0xf000, 0, 4, 8, 0x40, 0x18 },
    { 0x0c, 0xf0, 0, 1, 0x48, 0, 0x20, 0x20 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0218537a[] = {
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
static UnkMenuParts s_parts_02184e72[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0xc, 0x54, 0, 0xe },
    { 0x0d, 0x08, (short)0xf000, 1, 0x18, 0x54, 0x60, 0xe },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184cc0[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 4, 0x38, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184fec[] = {
    { 0x0f, 0x08, (short)0xf000, 0, 0xc, 0, 0xa, 0x10 },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 0xa, 0x10 },
    { 0x0f, 0x0b, (short)0xf000, 2, 0, 0, 0xa, 0x10 },
    { 0x0c, 0xf4, 0, 3, 0x10, 8, 0x18, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02185032[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0x80, 0xa0, 0xc },
    { 0x0d, 0x08, (short)0xf000, 1, 8, 0x98, 0x48, 0xc },
    { 0x0d, 0x08, (short)0xf000, 2, 8, 0xa8, 0x28, 0xc },
    { 0x0f, 0x0a, (short)0xf000, 3, 0x30, 0xa8, 0x20, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184d76[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x10, 0, 0x78, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 1, 0x80, 0, 0x70, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static const int s_tactics[6] = { 0x90000001, 0x90000002, 0x90000005, 0x90000003, 0x90000004, 0x90000006 };
static UnkMenuParts s_parts_021851f2[] = {
    { 0x0c, 0xf4, 0, 0, 8, 8, 0x18, 0x18 },
    { 0x0f, 0x08, (short)0xf000, 1, 0x24, 0x12, 8, 8 },
    { 0x0d, 0x0b, (short)0xf000, 2, 0, 0, 8, 8 },
    { 0x0f, 0x0b, (short)0xf000, 3, 0, 0, 8, 8 },
    { 0x0c, 0xf4, 0, 4, 0x40, 8, 0x18, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184ba8[] = {
    { 0x0e, 0x08, (short)0xf000, 0, 8, 0x78, 0xf0, 0x1c },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184b1c[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x18, 0x72, 0x50, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02185104[] = {
    { 0x14, 0xc0, 0, -1, 0, 8, 0x68, 0x18 },
    { 0x0e, 0x08, (short)0xf000, 0, 4, 8, 0x48, 0x18 },
    { 0x0c, 0xf0, 0, 1, 0x48, 0, 0x20, 0x20 },
    { 0x0f, 0x0a, (short)0xf000, 2, 0x50, 0x18, 0x18, 8 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184ed4[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 6, 0x30, 8 },
    { 0x07, 0x00, (short)0xf000, 0x11, 8, 0, 0x30, 8 },
    { 0x11, 0x00, 0, 1, 0, 0x10, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184b70[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 2, 0x54, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021854ae[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 8, 0x40, 0x10 },
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
static UnkMenuParts s_parts_02184f44[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 3, 0x10, 0x10 },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 8, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 2, 8, 3, 0x30, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184c18[] = {
    { 0x0e, 0x08, (short)0xf000, 0, 8, 0x78, 0xf0, 0x1c },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0218530a[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x10, 0x5b, 0xa0, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 1, 0xb8, 0x5b, 8, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 2, 0xc0, 0x5b, 8, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 3, 0xc8, 0x5b, 8, 0x10 },
    { 0x0d, 0x0a, (short)0xf000, 4, 0xd0, 0x5b, 8, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 5, 0xd8, 0x5b, 8, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 6, 0xe0, 0x5b, 8, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};

THUMB void unkfunc_0217ad94(int type, int x)
{
    int name = 0;
    int kind = 0;
    if (type == -1) {
        for (int i = 0; i < 6; i++) {
            UnkMenuParts kindPart = { 0x0c, 0xf4, 0, 0, 0, 0, 0x18, 0x18 };
            UnkMenuParts namePart = { 0x0d, 0x08, (short)0xf100, 0, 4, 0, 0x30, 0xc };
            kind = menu::MenuDataCommon::getRootMenuKind(i);
            name = menu::MenuDataCommon::getRootMenuName(i);
            unkfunc_02050f1c(&kindPart, &kind, (i % 2) * 0x48 + 0x68, (i / 2) * 32 + 0x60);
            unkfunc_02050f1c(&namePart, &name, (i % 2) * 0x48 + 0x44, (i / 2) * 32 + 0x6a);
        }
        return;
    }
    UnkMenuParts kindPart = { 0x0c, 0xf4, 0, 0, 0, 0, 0x18, 0x18 };
    UnkMenuParts namePart = { 0x0d, 0x08, (short)0xf100, 0, 4, 0, 0x30, 0xc };
    kind = menu::MenuDataCommon::getRootMenuKind(type);
    name = menu::MenuDataCommon::getRootMenuName(type);
    if (x != -1) {
        unkfunc_02050f1c(&kindPart, &kind, x + 0x28, 0xa0);
        unkfunc_02050f1c(&namePart, &name, x + 4, 0xaa);
        return;
    }
    unkfunc_02050f1c(&kindPart, &kind, 0x68, 0xa0);
    unkfunc_02050f1c(&namePart, &name, 0x44, 0xaa);
    unkfunc_0217cce4(0x40, 0xb0);
}

THUMB void unkfunc_0217aee0(status::HaveStatusInfo* info, int x, int y)
{
    int param = info->haveStatus_.iconIndex_;
    unkfunc_0217cf80(&param, x, y);
    if (info->isSpell()) {
        param = 0xa0000044;
        unkfunc_02050ee0(s_parts_02184aac, &param, x - 4, y + 0x14, data_020be244[unkfunc_0217cf70(info)]);
    }
    if (info->isDeath()) {
        param = 0xa0000042;
        unkfunc_02050ee0(s_parts_02184aac, &param, x - 4, y + 8, data_020be244[unkfunc_0217cf70(info)]);
        return;
    }
    if (info->isPoison()) {
        param = 0xa0000043;
        unkfunc_02050ee0(s_parts_02184aac, &param, x - 4, y + 8, data_020be244[unkfunc_0217cf70(info)]);
    }
}

THUMB void unkfunc_0217af90(int index, int x, int y)
{
    int param = 0;
    if (index < 0) {
        if (index == -3) {
            param = 0x19;
        }
        if (index == -2) {
            param = 0x17;
        }
        if (index == -1) {
            param = 0x18;
        }
        unkfunc_0217cf80(&param, x, y);
    }
}

THUMB void unkfunc_0217afc4(status::HaveStatusInfo* info)
{
    unkfunc_02050698(0, 0);
    int param[2];
    param[0] = 0xa0000041;
    for (int i = 0; i < info->haveItem_.getCount(); i++) {
        param[1] = info->haveItem_.getItem(i) + 0x40000000;
        unkfunc_0217cf98(param, (i % 2) * 0x78, (i / 2) * 16 + 6, info->haveItem_.isEquipment(i));
    }
}

THUMB void unkfunc_0217b038()
{
    int pageNum = 1;
    int page = 0;
    unkfunc_02050698(0, 0);
    if (status::FukuroItemInfo::getItemMaxCount() > 6) {
        pageNum = 2;
    }
    for (int n = 0; n < pageNum; n++) {
        for (int i = 0; i < status::FukuroItemInfo::getPageItemCount(page); i++) {
            int param[2];
            param[0] = 0;
            param[1] = status::FukuroItemInfo::getItemId(i, page) + 0x40000000;
            unkfunc_0217cf98(param, (i % 2) * 0x78, (i / 2 + n * 3) * 16 + 6, 0);
        }
        page++;
    }
}

THUMB void unkfunc_0217b0c4(status::HaveStatusInfo* info, int flag, int index, int fukuro)
{
    int param[2];
    param[1] = 0;
    if (index == -2) {
        param[0] = 0x8000006e;
        unkfunc_0217cfe8(param, 0, flag, 0);
        return;
    }
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_ != 0 && fukuro == 0) {
        param[0] = 0x5000005b;
        unkfunc_0217cfe8(param, 0, flag, 0);
        return;
    }
    param[0] = info->haveStatus_.playerIndex_ + 0x50000000;
    param[1] = info->getHp() * 100 / info->getHpMax();
    if (param[1] <= 0 && info->getHp() > 0) {
        param[1] = 1;
    }
    if (info->getHp() > (info->getHpMax() >> 2) - 1 && param[1] < 0x1a) {
        param[1] = 0x1a;
    }
    unkfunc_0217cfe8(param, 1, flag, unkfunc_0217cf70(info));
}

THUMB void unkfunc_0217b184(status::HaveStatusInfo* info)
{
    int param[2];
    if (TownMenuPlayerControl::getSingleton()->targetFukuro_ != 0) {
        param[0] = 0x5000005b;
        unkfunc_0217cfe8(param, 0, 1, 0);
        return;
    }
    param[0] = info->haveStatus_.playerIndex_ + 0x50000000;
    param[1] = info->getHp() * 100 / info->getHpMax();
    unkfunc_0217cfe8(param, 1, 1, unkfunc_0217cf70(info));
}

THUMB void unkfunc_0217b1ec(int item, int x, int y)
{
    if (item == -1) {
        s_parts_02184fb4[1].type_ = 0;
        s_parts_02184fb4[2].type_ = 0;
        unkfunc_02050ebc(s_parts_02184fb4, 0, x, y);
        return;
    }
    int param[2] = { 0, 0 };
    param[0] = item + 0x40000000;
    param[1] = item;
    s_parts_02184fb4[1].type_ = 0xe;
    s_parts_02184fb4[2].type_ = 0xc;
    unkfunc_02050ee0(s_parts_02184fb4, param, x, y, data_020be244[0]);
}

THUMB void unkfunc_0217b254(int item, int index, int x, int y)
{
    int param[3];
    int count;
    param[0] = item + 0x40000000;
    param[1] = item;
    param[1] = item;
    count = status::g_Party.haveItemSack_.getItemCount(index);
    param[2] = count;
    param[2] = count;
    if (status::g_Party.haveItemSack_.getItemCount(index) <= 1) {
        s_parts_02185104[3].type_ = 0;
    } else {
        s_parts_02185104[3].type_ = 0xf;
    }
    unkfunc_02050ee0(s_parts_02185104, param, x, y, data_020be244[0]);
}

THUMB void unkfunc_0217b2b4(int index)
{
    int param = 0xa0000041;
    unkfunc_02050ee0(s_parts_02184a90, &param, ((index % 2) + 1) * 32 + (index % 2) * 0x58, (index / 2) * 8 + (index / 2) * 0x18, data_020be244[0]);
}

THUMB void unkfunc_0217b300(int x, int pageMax, int page, int flag)
{
    int param[5] = { page + 1, (int)"\xff\xfe\xa7\x24", pageMax + 1, 0xd, 0xc };
    if (flag) {
        unkfunc_02050ee0(s_parts_0218514a, param, x, 0x78, data_020be244[0]);
        return;
    }
    unkfunc_02050ee0(s_parts_02184fec, param, x, 0x78, data_020be244[0]);
}

THUMB void unkfunc_0217b368(int x, int pageMax, int page)
{
    int param[5] = { 0xc, page + 1, (int)"\xff\xfe\xa7\x24", pageMax + 1, 0xd };
    unkfunc_02050ee0(s_parts_021851f2, param, x, 0x78, data_020be244[0]);
}

THUMB void unkfunc_0217b3b0(int index, int flag, int mode)
{
    int param;
    if (flag) {
        param = status::UseAction::getMenuMessage(index) + 0x30000000;
    } else {
        param = index + 0x20000000;
    }
    if (mode) {
        if (TownMenuPlayerControl::getSingleton()->targetItem_ == -1) {
            param = 0xcaf62;
        } else {
            param = 0xcaf64;
        }
    }
    unkfunc_02050ed0(s_parts_02184ba8, &param, data_020be244[0]);
}

THUMB void unkfunc_0217b40c(int item)
{
    if (item == 0) {
        return;
    }
    unkfunc_02050698(0, 0);
    int param[2];
    param[0] = menu::MenuDataCommon::getItemMessageNo(item);
    param[1] = menu::MenuDataCommon::getItemMessageNo2(item);
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
    unkfunc_02050ed0(s_parts_02184cf8, param, data_020be244[0]);
}

THUMB void unkfunc_0217b488(status::HaveStatusInfo* info, int item, int equipIndex, int flag)
{
    unkfunc_02050698(0, 0);
    UnkMenuParts* parts = 0;
    int param[6];
    if (item == -1) {
        param[0] = menu::MenuDataCommon::getEquipKind(equipIndex);
        int equip = unkfunc_0217d2e8(info, equipIndex, param);
        if (equipIndex == 0) {
            param[1] = menu::MenuDataCommon::getAbilityKind(0);
        } else if (equipIndex == 4) {
            if (equip == 0) {
                param[1] = 0xa000000d;
            } else if (status::UseItem::getEquipType(equip) == 4) {
                param[1] = 0xa000000b;
            } else {
                param[1] = menu::MenuDataCommon::getAbilityKind(status::UseItem::getEquipType(equip));
            }
        } else {
            param[1] = 0xa000000d;
        }
        unkfunc_02050ed0(s_parts_02185246, param, data_020be244[unkfunc_0217cf70(info)]);
        return;
    }
    param[0] = menu::MenuDataCommon::getEquipKind(status::UseItem::getItemType(item));
    if (status::UseItem::getEquipType(item) == 4) {
        param[1] = 0xa000000b;
    } else {
        param[1] = menu::MenuDataCommon::getAbilityKind(status::UseItem::getEquipType(item));
    }
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_ != 0 && flag == 0) {
        param[2] = 0;
        unkfunc_02050ed0(s_parts_02184e9c, param, data_020be244[0]);
        return;
    }
    switch (unkfunc_0217d140(info, item, param)) {
    case 0:
        parts = s_parts_02184e9c;
        break;
    case 1:
        parts = s_parts_02185246;
        break;
    case 2:
        parts = s_parts_02185032;
        break;
    }
    unkfunc_02050ed0(parts, param, data_020be244[unkfunc_0217cf70(info)]);
}

THUMB void unkfunc_0217b5b0(int mode)
{
    int count = 0;
    int list[6];
    int command[6];
    command[0] = menu::MenuDataCommon::getItemUseCommand(0);
    command[1] = menu::MenuDataCommon::getItemUseCommand(1);
    command[2] = menu::MenuDataCommon::getItemUseCommand(2);
    command[3] = menu::MenuDataCommon::getItemUseCommand(3);
    command[4] = menu::MenuDataCommon::getItemUseCommand(4);
    command[5] = menu::MenuDataCommon::getItemUseCommand(5);
    if ((status::g_Party.getPlayerStatus(TownMenuPlayerControl::getSingleton()->activeChara_)->haveStatusInfo_.haveStatus_.playerIndex_ == 7
         && TownMenuPlayerControl::getSingleton()->activeFukuro_ == 0) || status::g_Story.chapter_ == 3) {
        command[4] = 0x80000089;
    }
    switch (mode) {
    case 0:
        count = 4;
        list[0] = command[0];
        list[1] = command[1];
        list[2] = command[2];
        list[3] = command[5];
        break;
    case 1:
        count = 5;
        list[0] = command[0];
        list[1] = command[1];
        list[2] = command[2];
        list[3] = command[3];
        list[4] = command[5];
        break;
    case 2:
        count = 5;
        list[0] = command[0];
        list[1] = command[1];
        list[2] = command[2];
        list[3] = command[4];
        list[4] = command[5];
        break;
    case 3:
        for (int i = 0; i < 6; i++) {
            unkfunc_02050ee0(s_parts_02184b00, &command[i], (i % 3) * 64 + 0x10 + (i % 3) * 16, (i / 3) * 0x18 + 0x70, data_020be244[0]);
        }
        return;
    }
    for (int i = 0; i < count; i++) {
        unkfunc_02050ee0(s_parts_02184b00, &list[i], (i % 3) * 64 + 0x10 + (i % 3) * 16, (i / 3) * 0x18 + 0x70, data_020be244[0]);
    }
}

THUMB void unkfunc_0217b70c(int mode, int flag)
{
    int w4;
    int x4;
    int w3;
    int x3;
    int w2;
    int x2;
    int w1;
    int x1;
    int count = status::g_Party.getCount();
    if (mode != 0 && status::g_Party.fukuro_ != 0) {
        int n = 0;
        int i;
        for (i = 0; i < count; i++) {
            x1 = (n % 5) * 32 + 0x10;
            w1 = (n % 5) * 8;
            int y;
            if (count < 5) {
                y = 0x70;
            } else {
                y = (n / 5) * 0x28 + 0x48;
            }
            if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.isPlayer_) {
                unkfunc_0217aee0(&status::g_Party.getPlayerStatus(i)->haveStatusInfo_, x1 + w1, y);
                n++;
            }
        }
        status::g_Party.setBattleMode();
        n = 0;
        for (i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
            x2 = (n % 5) * 32 + 0x10;
            w2 = (n % 5) * 8;
            int y;
            if (count < 5) {
                y = 0x70;
            } else {
                y = (n / 5) * 0x28 + 0x48;
            }
            if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.isPlayer_) {
                unkfunc_0217c8bc(i + 1, x2 + w2 - 8, y + 8, i);
                n++;
            }
        }
        unkfunc_0217af90(-1, (count % 5) * 32 + 0x10 + (count % 5) * 8, 0x70);
    } else {
        int i;
        for (i = 0; i < count; i++) {
            x3 = (i % 5) * 32 + 0x10;
            w3 = (i % 5) * 8;
            int y;
            if (count < 6) {
                y = 0x70;
            } else {
                y = (i / 5) * 0x28 + 0x48;
            }
            unkfunc_0217aee0(&status::g_Party.getPlayerStatus(i)->haveStatusInfo_, x3 + w3, y);
            if (flag == 0 && i < status::g_Party.getCarriageOutCount()) {
                unkfunc_0217c8bc(i + 1, x3 + w3 - 8, y + 8, i);
            }
        }
        if (flag == 1) {
            status::g_Party.setBattleMode();
            int n = 0;
            for (i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
                x4 = (n % 5) * 32 + 0x10;
                w4 = (n % 5) * 8;
                int y;
                if (count < 6) {
                    y = 0x70;
                } else {
                    y = (i / 5) * 0x28 + 0x48;
                }
                if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.isPlayer_) {
                    unkfunc_0217c8bc(i + 1, x4 + w4 - 8, y + 8, i);
                    n++;
                }
            }
        }
    }
    status::g_Party.setPlayerMode();
}

THUMB void unkfunc_0217b908(status::HaveStatusInfo* info, int index, int flag)
{
    if (index == status::g_Party.getCount()) {
        status::g_Party.setPlayerMode();
        return;
    }
    int param[14];
    int playerIndex = info->haveStatus_.playerIndex_;
    param[0] = info->haveStatus_.iconIndex_;
    param[1] = playerIndex + 0x50000000;
    param[2] = 0xa000003f;
    param[3] = 0xa000003d;
    param[4] = (int)"\xff\xfe\xa7\x24";
    param[5] = 0xa000003e;
    param[6] = (int)"\xff\xfe\xa7\x24";
    param[7] = info->haveStatus_.level_;
    param[8] = info->haveStatus_.getHp();
    param[9] = info->haveStatus_.getHpMax();
    param[10] = info->haveStatus_.getMp();
    param[11] = info->haveStatus_.getMpMax();
    param[12] = index + 1;
    param[13] = (int)":";
    int condition = unkfunc_0217cf78(info);
    if (!(condition != 2 && condition != 3)) {
        s_parts_02185572[3].type_ = 0xd;
        s_parts_02185572[4].type_ = 0;
        s_parts_02185572[9].type_ = 0;
        param[7] = 0;
        param[13] = 0;
        if (condition == 3) {
            param[2] = menu::MenuDataCommon::convMessage(info, 0xa0000018);
        } else {
            param[2] = menu::MenuDataCommon::convMessage(info, 0xa0000014);
        }
    } else if (info->haveStatus_.isPlayer() == false) {
        s_parts_02185572[3].type_ = 0xd;
        s_parts_02185572[4].type_ = 0xd;
        s_parts_02185572[9].type_ = 0xd;
        param[13] = (int)":";
        param[2] = 0xa000003f;
        param[7] = 0xa0000073;
    } else {
        s_parts_02185572[3].type_ = 0xd;
        s_parts_02185572[4].type_ = 0xd;
        s_parts_02185572[9].type_ = 0xf;
        param[2] = 0xa000003f;
        param[7] = info->haveStatus_.level_;
        param[13] = (int)":";
    }
    if (flag != 0 || (flag == 0 && status::g_Party.isOutsideCarriage(index) == 0)) {
        s_parts_02185572[14].type_ = 0;
        param[12] = 0;
    } else {
        s_parts_02185572[14].type_ = 0xf;
    }
    unkfunc_02050ee0(s_parts_02185572, param, (index % 2) * 0x78 + 0xc, (index / 2) * 64 + 0xc, data_020be244[unkfunc_0217cf70(info)]);
}

THUMB void unkfunc_0217ba8c(status::HaveStatusInfo* info, int pageMax, int page)
{
    unkfunc_02050ebc(s_parts_02184d22, 0, 0x88, 0xa0);
    int param[4] = { 0xa000003e, page, (int)"\xff\xfe\xa7\x24", pageMax };
    unkfunc_02050ee0(s_parts_02185078, param, 0x88, 0xa0, data_020be244[unkfunc_0217cf70(info)]);
}

THUMB void unkfunc_0217bae0(status::HaveStatusInfo* info)
{
    unkfunc_02050698(0, 0);
    int count = info->haveAction_.getCount();
    int n = 0;
    int list[8] = { 0 };
    for (int i = 0; i < count; i++) {
        int action = info->haveAction_.getAction(i);
        if (status::UseAction::isUsuallyUse(action)) {
            list[n] = action;
            n++;
        }
    }
    for (int i = 0; i < n; i++) {
        unkfunc_0217bb74((i % 2) << 7, (i / 2) * 16 + 0x64, unkfunc_0201e674(list[i]), 1, info);
    }
}

THUMB void unkfunc_0217bb74(int x, int y, int value, int flag, status::HaveStatusInfo* info)
{
    int param = value;
    if (flag) {
        unkfunc_02050ee0(s_parts_02184cdc, &param, x, y, data_020be244[unkfunc_0217cf70(info)]);
        return;
    }
    unkfunc_02050ee0(s_parts_02184c6c, &param, x, y, data_020be244[unkfunc_0217cf70(info)]);
}

THUMB void unkfunc_0217bbcc(int page, unsigned char* rura)
{
    int y2;
    int count = cmn::CommonRuraData::getSingleton()->getRuraCount();
    int i = 0;
    for (int index = page * 8; index < count; index++) {
        int x = (i % 2) * 0x78 + 0x1c;
        int y = (i / 2) * 16 + 0xc;
        y2 = (i / 2) * 8;
        int param = menu::MenuDataCommon::getRuraName(rura[index]);
        if (rura[index] == 0xd && data_020f0078.unkfunc_0203a354() == 1) {
            data_020f0078.mode_ = 1;
            param = (int)data_020f0078.unkfunc_0203a820();
        }
        unkfunc_02050ee0(s_parts_02184be0, &param, x, y + y2, data_020be244[0]);
        i++;
        if (i > 7) {
            break;
        }
    }
    if (count > 8) {
        unkfunc_0217b300(0xb8, (count - 1) / 8, page, 1);
    }
    int msg = status::UseAction::getMenuMessage(0xcb) + 0x30000000;
    unkfunc_02050ed0(s_parts_02184c18, &msg, data_020be244[0]);
}

THUMB void unkfunc_0217bc9c(status::HaveStatusInfo* info)
{
    int param[18];
    int icon = info->haveStatus_.iconIndex_;
    int job = info->haveStatus_.job_;
    param[0] = info->haveStatus_.playerIndex_ + 0x50000000;
    param[1] = (int)"\xff\xfe\xac\x24";
    param[2] = 0x80000094;
    param[3] = job + 0xb0000000;
    if (job == 1) {
        if (info->haveStatus_.sex_ == 0) {
            param[3] = 0xa00000c8;
        } else {
            param[3] = 0xa00000c9;
        }
    }
    param[4] = 0xa0000004;
    param[5] = (int)":";
    if (info->haveStatus_.sex_ == 0) {
        param[6] = 0xa0000005;
    } else {
        param[6] = 0xa0000006;
    }
    param[7] = 0xa0000003;
    param[8] = (int)":";
    param[9] = info->haveStatus_.level_;
    param[10] = 0xa0000001;
    param[11] = info->getHp();
    param[12] = (int)"\xff\xfe\xa7\x24";
    param[13] = info->getHpMax();
    param[14] = 0xa0000002;
    param[15] = info->getMp();
    param[16] = (int)"\xff\xfe\xa7\x24";
    param[17] = info->getMpMax();
    if (info->haveStatus_.isPlayer() == false) {
        s_parts_02185732[9].type_ = 0xd;
        param[9] = 0xa0000073;
    } else {
        s_parts_02185732[9].type_ = 0xf;
    }
    unkfunc_02050ed0(s_parts_02185732, param, data_020be244[unkfunc_0217cf70(info)]);
    unkfunc_02050ebc(s_parts_02184ac8, &icon, 6, 0x28);
}

THUMB void unkfunc_0217bdb8(status::HaveStatusInfo* info)
{
    int i = 0;
    int index;
    int y = 0;
    for (; i < 7; i++) {
        index = i + 2;
        if (index > 6) {
            index -= 7;
        }
        int param[3] = { menu::MenuDataCommon::getAbilityKind(index), (int)":", menu::MenuDataCommon::getStatus(index, *info) };
        unkfunc_02050ee0(s_parts_02184f0c, param, 0, y, data_020be244[unkfunc_0217cf70(info)]);
        y += 0xe;
    }
}

THUMB void unkfunc_0217c290(status::HaveStatusInfo* info, int flag)
{
    unkfunc_02050698(0, 0);
    int equip[5] = { 0, 0, 0, 0, 0 };
    for (int i = 0; i < 5; i++) {
        if (info->haveEquipment_.getEquipment((ItemType)i)) {
            equip[i] = info->haveEquipment_.getEquipment((ItemType)i);
        } else {
            equip[i] = -1;
        }
    }
    int param[3] = { 0, 0, 0 };
    for (int i = 0; i < 5; i++) {
        if (equip[i] != -1) {
            param[0] = 0xa0000041;
            param[1] = equip[i] + 0x40000000;
            if (flag) {
                unkfunc_02050ee0(s_parts_02184df4, param, 0x68, i * 16, data_020be244[unkfunc_0217cf70(info)]);
            } else {
                unkfunc_02050ee0(s_parts_02184df4, param, 0, i * 16, data_020be244[unkfunc_0217cf70(info)]);
            }
        }
    }
    if (flag == 0) {
        param[0] = 0xa0000010;
        param[1] = (int)":";
        param[2] = info->getExp();
        if (info->haveStatus_.isPlayer() == false) {
            s_parts_02184f7c[2].type_ = 0xd;
            param[2] = 0xa0000073;
        } else {
            s_parts_02184f7c[2].type_ = 0xf;
        }
        unkfunc_02050ed0(s_parts_02184f7c, param, data_020be244[unkfunc_0217cf70(info)]);
        param[0] = status::g_Party.gold_;
        param[1] = 0xa000003c;
        unkfunc_02050ed0(s_parts_02184e1e, param, data_020be244[unkfunc_0217cf70(info)]);
    }
}

THUMB void unkfunc_0217c3e0(status::HaveStatusInfo* info, int battle)
{
    int n = 0;
    int param = 0;
    int count = info->haveAction_.getCount();
    status::HaveAction::ActionMode mode = status::HaveAction::getActionMode();
    if (battle) {
        unkfunc_02050698(0, 0);
        param = 0xa0000023;
        unkfunc_02050ed0(s_parts_02184ca4, &param, data_020be244[0]);
        status::HaveAction::setBattleMode();
        for (int i = 0; i < count; i++) {
            int action = info->haveAction_.getAction(i);
            if (status::UseAction::isBattleUse(action)) {
                unkfunc_0217bb74((n % 2) << 7, (n / 2) * 12 + 0x34, unkfunc_0201e674(action), 1, info);
                n++;
            }
        }
    } else {
        param = 0xa0000022;
        unkfunc_02050ed0(s_parts_02184ca4, &param, data_020be244[0]);
        status::HaveAction::setTownMode();
        for (int i = 0; i < count; i++) {
            int action = info->haveAction_.getAction(i);
            if (status::UseAction::isUsuallyUse(action)) {
                unkfunc_0217bb74((n % 2) << 7, (n / 2) * 12 + 0x34, unkfunc_0201e674(action), 1, info);
                n++;
            }
        }
    }
    status::HaveAction::setActionMode(mode);
}

THUMB void unkfunc_0217c4f0()
{
    int param[15];
    int num;
    if (status::g_Party.getCount() > 4) {
        num = 4;
    } else {
        num = status::g_Party.getCount();
    }
    for (int i = 0; i < num; i++) {
        status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i)->haveStatusInfo_;
        param[0] = i + 1;
        param[1] = (int)":";
        unkfunc_0217d454(info, param, 0);
        unkfunc_02050ee0(s_parts_02185652, param, i * 0x40, 0, data_020be244[unkfunc_0217cf70(info)]);
    }
    if (status::g_Party.getCount() >= 5) {
        unkfunc_02050698(0, 0);
        num = status::g_Party.getCount() - 4;
        if (num > 4) {
            for (int i = 0; i < 4; i++) {
                status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i + 4)->haveStatusInfo_;
                unkfunc_0217d454(info, param, 1);
                unkfunc_02050ee0(s_parts_021854ae, param, i * 0x40, 0x60, data_020be244[unkfunc_0217cf70(info)]);
            }
            num = status::g_Party.getCount() - 8;
            for (int i = 0; i < num; i++) {
                status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i + 8)->haveStatusInfo_;
                unkfunc_0217d454(info, param, 1);
                unkfunc_02050ee0(s_parts_021854ae, param, i * 0x40, 0, data_020be244[unkfunc_0217cf70(info)]);
            }
        } else {
            for (int i = 0; i < num; i++) {
                status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i + 4)->haveStatusInfo_;
                unkfunc_0217d454(info, param, 1);
                unkfunc_02050ee0(s_parts_021854ae, param, i * 0x40, 0x60, data_020be244[unkfunc_0217cf70(info)]);
            }
        }
    }
}

THUMB void unkfunc_0217c658()
{
    int param[7];
    int num;
    if (status::g_Party.getCount() > 4) {
        num = 4;
    } else {
        num = status::g_Party.getCount();
    }
    for (int i = 0; i < num; i++) {
        status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i)->haveStatusInfo_;
        param[0] = i + 1;
        param[1] = (int)":";
        unkfunc_0217d4f0(info, param, 0);
        unkfunc_02050ee0(s_parts_0218529a, param, i * 0x40, 0, data_020be244[unkfunc_0217cf70(info)]);
        unkfunc_0217d054(info, i, 0x40, 0x58);
    }
    if (status::g_Party.getCount() >= 5) {
        unkfunc_02050698(0, 0);
        num = status::g_Party.getCount() - 4;
        if (num > 4) {
            for (int i = 0; i < 4; i++) {
                status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i + 4)->haveStatusInfo_;
                unkfunc_0217d4f0(info, param, 1);
                unkfunc_02050ee0(s_parts_0218519e, param, i * 0x40, 0x60, data_020be244[unkfunc_0217cf70(info)]);
                unkfunc_0217d054(info, i, 0x40, 0xa8);
            }
            num = status::g_Party.getCount() - 8;
            for (int i = 0; i < num; i++) {
                status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i + 8)->haveStatusInfo_;
                unkfunc_0217d4f0(info, param, 1);
                unkfunc_02050ee0(s_parts_0218519e, param, i * 0x40, 0, data_020be244[unkfunc_0217cf70(info)]);
                unkfunc_0217d054(info, i, 0x40, 0x48);
            }
        } else {
            for (int i = 0; i < num; i++) {
                status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i + 4)->haveStatusInfo_;
                unkfunc_0217d4f0(info, param, 1);
                unkfunc_02050ee0(s_parts_0218519e, param, i * 0x40, 0x60, data_020be244[unkfunc_0217cf70(info)]);
                unkfunc_0217d054(info, i, 0x40, 0xa8);
            }
        }
    }
}

THUMB void unkfunc_0217c7f0()
{
    int param[12] = { 0xa0000029, (int)"\xff\xfe\xa9\x24", 0, 0xa000002a, (int)"\xff\xfe\xa9\x24", 0, 0xa000002b, 0, 0xa000003c, 0xa000002c, 0, 0xa000003c };
    int time[7];
    param[2] = status::g_Party.casinoCoin_;
    param[5] = status::g_Party.medalCoin_ + status::g_Party.playerMedalCoin_;
    param[7] = status::g_Party.gold_;
    param[10] = status::g_Party.bankMoney_;
    int playTime = status::g_Game.getPlayTime();
    int hours = playTime / 216000;
    int minutes = playTime % 216000 / 3600;
    time[0] = 0xa000002d;
    time[1] = hours / 100;
    time[2] = hours / 10 % 10;
    time[3] = hours % 10;
    time[4] = (int)":";
    time[5] = minutes / 10;
    time[6] = minutes % 10;
    unkfunc_02050ed0(s_parts_021853f8, param, data_020be244[0]);
    unkfunc_02050ed0(s_parts_0218530a, time, data_020be244[0]);
}

THUMB void unkfunc_0217c8bc(int number, int x, int y, int index)
{
    if (index < 0) {
        unkfunc_02050ee0(s_parts_02184b38, &number, x, y, data_020be244[0]);
        return;
    }
    unkfunc_02050ee0(s_parts_02184b38, &number, x, y, data_020be244[unkfunc_0217cf70(&status::g_Party.getPlayerStatus(index)->haveStatusInfo_)]);
}

THUMB void unkfunc_0217c918(int x, int y, int value)
{
    unkfunc_02050698(0, 0);
    int param = value;
    unkfunc_02050ee0(s_parts_02184b54, &param, x, y, data_020be244[0]);
}

THUMB void unkfunc_0217c94c(int x, int y, int value)
{
    int param = value;
    unkfunc_02050ee0(s_parts_02184b70, &param, x, y, data_020be244[0]);
}

THUMB void unkfunc_0217c974(status::HaveStatusInfo* info, int x, int y)
{
    unkfunc_02050698(0, 0);
    int param[2];
    param[0] = info->haveStatus_.playerIndex_ + 0x50000000;
    if (status::g_Story.sex_ == 0) {
        param[1] = s_tactics[info->battleCommand_];
    } else {
        param[1] = s_tactics[info->battleCommand_] + 6;
    }
    unkfunc_02050ee0(s_parts_02184d76, param, x, y, data_020be244[unkfunc_0217cf70(info)]);
}

THUMB void unkfunc_0217c9ec()
{
    int param = 0;
    for (int i = 0; i < 6; i++) {
        if (status::g_Story.sex_ == 0) {
            param = s_tactics[i];
        } else {
            param = s_tactics[i] + 6;
        }
        unkfunc_02050ee0(s_parts_02184c34, &param, (i % 3) * 0x38 + 0x1c + (i % 3) * 8, (i / 3) * 0x18 + 8 + (i / 3) * 8, data_020be244[0]);
    }
}

THUMB void unkfunc_0217ca5c(char* order, char* list, int count)
{
    for (int i = 0; i < count; i++) {
        int x = (i % 5) * 32 + 0x10 + (i % 5) * 8;
        int y = (i / 5) * 32 + 0x68 + (i / 5) * 8;
        unkfunc_0217aee0(&status::g_Party.getPlayerStatus(list[i])->haveStatusInfo_, x, y);
        if (!status::g_Party.isInsideBasha(list[i])) {
            unkfunc_0217c8bc(list[i] + 1, x - 8, y + 8, list[i]);
        }
    }
    if (order[0] != -1 && status::g_Party.getCarriageEnableOnGame()) {
        unkfunc_0217af90(-3, (count % 5) * 32 + 0x10 + (count % 5) * 8, (count / 5) * 32 + 0x68 + (count / 5) * 8);
    } else if (count == 0 && !status::g_Party.getCarriageEnableOnGame()) {
        unkfunc_0217af90(-3, 0x10, 0x68);
    }
}

THUMB void unkfunc_0217cb3c(char* order)
{
    int param[4];
    param[1] = (int)":";
    if (status::g_Game.unkfunc_0216e3f0() == 2) {
        param[1] = (int)" :";
    }
    for (int i = 0; i < 4; i++) {
        param[0] = i + 1;
        unkfunc_02050ee0(s_parts_02184dca, param, (i % 2) * 64 + 8, (i / 2) * 0x30 + 8, data_020be244[0]);
    }
    for (int i = 0; order[i] != -1 && i < 4; i++) {
        status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(order[i])->haveStatusInfo_;
        param[0] = info->haveStatus_.playerIndex_ + 0x50000000;
        param[1] = 0xa0000013;
        param[2] = (int)":";
        param[3] = info->haveStatus_.level_;
        int x = (i % 2) * 64 + 0x18;
        int y = (i / 2) * 0x30 + 8;
        if (info->haveStatus_.isPlayer() == false) {
            s_parts_021850be[3].type_ = 0xd;
            param[3] = 0xa0000073;
        } else {
            s_parts_021850be[3].type_ = 0xf;
            param[3] = info->haveStatus_.level_;
        }
        unkfunc_02050ee0(s_parts_021850be, param, x, y, data_020be244[unkfunc_0217cf70(info)]);
    }
}

THUMB void unkfunc_0217cc4c()
{
    int msg = 0x8000006b;
    unkfunc_02050ed0(s_parts_02184c88, &msg, data_020be244[0]);
}

THUMB void unkfunc_0217cc6c()
{
    int msg = 0x8000008b;
    unkfunc_02050ed0(s_parts_02184b1c, &msg, data_020be244[0]);
}

THUMB void unkfunc_0217cc8c()
{
    int msg = 0x80000074;
    unkfunc_02050ed0(s_parts_02184bfc, &msg, data_020be244[0]);
}

THUMB void unkfunc_0217ccac()
{
    int msg = 0x8000008c;
    unkfunc_02050ed0(s_parts_02184bc4, &msg, data_020be244[0]);
    unkfunc_0201e194(0x90, 0x48, 0x50, 0x20, -1);
}

THUMB void unkfunc_0217cce4(int x, int y)
{
    unkfunc_02050ebc(s_parts_02184ae4, 0, x, y);
}

THUMB void unkfunc_0217ccf8(status::HaveStatusInfo* info, int item, int flag)
{
    int param[8];
    int values[5];
    param[0] = item + 0x40000000;
    param[1] = (int)"\xff\xfe\xaf\x24";
    if (TownMenuPlayerControl::getSingleton()->targetFukuro_ != 0) {
        param[2] = 0x5000005b;
    } else {
        param[2] = info->haveStatus_.playerIndex_ + 0x50000000;
    }
    if (status::UseItem::getItemType(item) <= 4 && TownMenuPlayerControl::getSingleton()->targetFukuro_ == 0) {
        if (status::UseItem::getEquipType(item) == 4) {
            param[3] = menu::MenuDataCommon::getAbilityKind(6);
        } else {
            param[3] = menu::MenuDataCommon::getAbilityKind(status::UseItem::getEquipType(item));
        }
        switch (unkfunc_0217d140(info, item, values)) {
        case 0:
            param[3] = values[2];
            param[4] = param[5] = param[6] = 0;
            s_parts_0218537a[4].type_ = s_parts_0218537a[5].type_ = s_parts_0218537a[6].type_ = 0;
            break;
        case 1:
            param[4] = values[2];
            param[5] = values[3];
            param[6] = values[4];
            s_parts_0218537a[4].type_ = 0xf;
            s_parts_0218537a[5].type_ = 0xd;
            s_parts_0218537a[6].type_ = 0xf;
            break;
        case 2:
            param[5] = values[2];
            param[4] = 0;
            param[6] = values[3];
            s_parts_0218537a[4].type_ = 0;
            s_parts_0218537a[5].type_ = 0xd;
            s_parts_0218537a[6].type_ = 0xf;
            break;
        }
    } else {
        if (TownMenuPlayerControl::getSingleton()->targetFukuro_ != 0) {
            if (status::g_Party.haveItemSack_.isItem(item)) {
                param[3] = 0xa0000078;
            } else {
                param[3] = 0xa0000079;
            }
        } else {
            if (info->haveItem_.isItem(item)) {
                param[3] = 0xa0000076;
            } else {
                param[3] = 0xa0000077;
            }
        }
        param[4] = param[5] = param[6] = 0;
        s_parts_0218537a[4].type_ = s_parts_0218537a[5].type_ = s_parts_0218537a[6].type_ = 0;
    }
    if (flag) {
        param[7] = 0;
        unkfunc_02050698(0, 0);
        s_parts_0218537a[0].y_ = 0x12;
        s_parts_0218537a[1].y_ = s_parts_0218537a[2].y_ = 0x22;
        for (int i = 0; i < 4; i++) {
            s_parts_0218537a[i + 3].y_ = 0x3a;
        }
    } else {
        s_parts_0218537a[0].y_ = 0xa;
        s_parts_0218537a[1].y_ = s_parts_0218537a[2].y_ = 0x1a;
        param[7] = item;
        for (int i = 0; i < 4; i++) {
            s_parts_0218537a[i + 3].y_ = 0x32;
        }
    }
    unkfunc_02050ed0(s_parts_0218537a, param, data_020be244[0]);
}

THUMB void unkfunc_0217cea8(int mode)
{
    int param[2] = { 0x8000008d, (int)"\xff\xfe\xac\x24" };
    int y = 0x58;
    if (mode == 1) {
        if (status::g_Party.getCount() + 1 > 5) {
            y = 0x30;
        }
        param[0] = 0x8000008e;
    } else if (status::g_Party.getCount() > 5) {
        y = 0x30;
    }
    unkfunc_02050ee0(s_parts_02184d4c, param, 0, y, data_020be244[0]);
    unkfunc_0201e194(0, y - 8, 0x78, 0x20, -1);
}

THUMB void unkfunc_0217cf1c(int mode)
{
    unkfunc_02050698(0, 0);
    int param[2] = { 0x8000008f, (int)"\xff\xfe\xad\x24" };
    if (mode == 1) {
        param[0] = 0x80000090;
    }
    unkfunc_02050ed0(s_parts_02184e48, param, data_020be244[0]);
    unkfunc_0201e194(0, 0xa0, 0xa0, 0x20, -1);
}

THUMB int unkfunc_0217cf70(status::HaveStatusInfo* info)
{
    return unkfunc_0201e2f4(info);
}

THUMB int unkfunc_0217cf78(status::HaveStatusInfo* info)
{
    return unkfunc_0201e318(info);
}

THUMB void unkfunc_0217cf80(int* param, int x, int y)
{
    unkfunc_02050ebc(s_parts_02184da0, param, x, y);
}

THUMB void unkfunc_0217cf98(int* param, int x, int y, int flag)
{
    if (flag) {
        s_parts_02184e72[0].w_ = 0x10;
        s_parts_02184e72[0].type_ = 0xd;
    } else {
        param[0] = 0;
        s_parts_02184e72[0].type_ = 0;
        s_parts_02184e72[1].w_ = 0x70;
    }
    unkfunc_02050ee0(s_parts_02184e72, param, x, y, data_020be244[0]);
}

THUMB void unkfunc_0217cfe8(int* param, int mode, int flag, int drawFlag)
{
    if (flag) {
        s_parts_02184ed4[0].x_ = s_parts_02184ed4[1].x_ = 0x90;
        s_parts_02184ed4[2].x_ = 0x88;
    } else {
        s_parts_02184ed4[0].x_ = s_parts_02184ed4[1].x_ = 8;
        s_parts_02184ed4[2].x_ = 0;
    }
    s_parts_02184ed4[1].y_ = 0xa0;
    if (mode) {
        s_parts_02184ed4[0].y_ = 0xa6;
        s_parts_02184ed4[2].y_ = 0xb6;
        s_parts_02184ed4[2].type_ = 0x11;
    } else {
        s_parts_02184ed4[0].y_ = 0xaa;
        s_parts_02184ed4[2].type_ = 0;
    }
    unkfunc_02050ed0(s_parts_02184ed4, param, data_020be244[drawFlag]);
}

THUMB void unkfunc_0217d054(status::HaveStatusInfo* info, int index, int width, int y)
{
    int param[3];
    if (unkfunc_0217cf78(info) == 2) {
        param[0] = menu::MenuDataCommon::convMessage(info, 0xa0000014);
        unkfunc_02050ee0(s_parts_02184cc0, param, width * index, y, data_020be244[unkfunc_0217cf70(info)]);
        return;
    }
    if (unkfunc_0217cf78(info) == 3) {
        param[0] = menu::MenuDataCommon::convMessage(info, 0xa0000018);
        unkfunc_02050ee0(s_parts_02184cc0, param, width * index, y, data_020be244[unkfunc_0217cf70(info)]);
        return;
    }
    param[0] = 0xa0000013;
    param[1] = (int)":";
    if (info->haveStatus_.isPlayer() == false) {
        s_parts_02184f44[2].type_ = 0xd;
        param[2] = 0xa0000073;
    } else {
        s_parts_02184f44[2].type_ = 0xf;
        param[2] = info->haveStatus_.level_;
    }
    unkfunc_02050ee0(s_parts_02184f44, param, width * index, y, data_020be244[unkfunc_0217cf70(info)]);
}

THUMB int unkfunc_0217d140(status::HaveStatusInfo* info, int item, int* param)
{
    if (!info->isEquipEnable(item)) {
        param[2] = 0xa0000067;
        return 0;
    }
    switch (status::UseItem::getEquipType(item)) {
    case 2:
        if (info->haveEquipment_.isEquipment(item)) {
            param[2] = 0xa0000041;
            param[3] = info->getStrength(0);
            return 2;
        }
        param[2] = info->getStrength(0);
        param[4] = info->getChangeStrength(item);
        break;
    case 0:
        if (info->haveEquipment_.isEquipment(item)) {
            param[2] = 0xa0000041;
            param[3] = info->getAttack(0);
            return 2;
        }
        param[2] = info->getAttack(0);
        param[4] = info->getStrength(0) + status::UseItem::getEquipValue(item);
        break;
    case 1:
        if (info->haveEquipment_.isEquipment(item)) {
            param[2] = 0xa0000041;
            param[3] = info->getDefence(0);
            return 2;
        }
        param[2] = info->getDefence(0);
        param[4] = info->getChangeDefence(item);
        break;
    case 3:
        if (info->haveEquipment_.isEquipment(item)) {
            param[2] = 0xa0000041;
            param[3] = info->getAgility(0);
            return 2;
        }
        param[2] = info->getAgility(0);
        param[4] = info->getChangeAgility(item);
        break;
    case 5:
        if (info->haveEquipment_.isEquipment(item)) {
            param[2] = 0xa0000041;
            param[3] = info->getWisdom(0);
            return 2;
        }
        param[2] = info->getWisdom(0);
        param[4] = info->getChangeWisdom(item);
        break;
    case 4:
        if (info->haveEquipment_.isEquipment(item)) {
            param[2] = 0xa0000041;
            param[3] = info->getLuck(0);
            return 2;
        }
        param[2] = info->getLuck(0);
        param[4] = info->getChangeLuck(item);
        break;
    }
    param[3] = (int)"\xff\xfe\xaf\x24";
    return 1;
}

THUMB int unkfunc_0217d2e8(status::HaveStatusInfo* info, int equipIndex, int* param)
{
    int equip = info->haveEquipment_.getEquipment((ItemType)equipIndex);
    int value = status::UseItem::getEquipValue(equip);
    switch (equipIndex) {
    case 0:
        if (equip == 0) {
            param[2] = param[4] = info->getAttack(0);
        } else {
            param[2] = info->getAttack(0);
            param[4] = info->getAttack(0) - value;
        }
        break;
    case 1:
    case 2:
    case 3:
        if (equip == 0) {
            param[2] = param[4] = info->getDefence(0);
        } else {
            param[2] = info->getDefence(0);
            param[4] = info->getDefence(0) - value;
        }
        break;
    case 4:
        if (equip == 0) {
            param[2] = param[4] = info->getDefence(0);
            break;
        }
        switch (status::UseItem::getEquipType(equip)) {
        case 2:
            param[2] = info->getStrength(0);
            param[4] = info->getStrength(1);
            break;
        case 0:
            param[2] = info->getAttack(0);
            param[4] = info->getAttack(0) - value;
            break;
        case 1:
            param[2] = info->getDefence(0);
            param[4] = info->getDefence(0) - status::UseItem::getEquipValue(equip);
            break;
        case 5:
            param[2] = info->getWisdom(0);
            param[4] = info->getWisdom(1);
            break;
        case 3:
            param[2] = info->getAgility(0);
            param[4] = info->getAgility(1);
            break;
        case 4:
            param[2] = info->getLuck(0);
            param[4] = info->getLuck(1);
            break;
        }
        break;
    }
    param[4] = dss::clamp(param[4], 0, 999);
    param[3] = (int)"\xff\xfe\xaf\x24";
    return equip;
}

THUMB void unkfunc_0217d454(status::HaveStatusInfo* info, int* param, int flag)
{
    if (flag) {
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
        return;
    }
    param[2] = info->haveStatus_.playerIndex_ + 0x50000000;
    param[3] = 0xa0000001;
    param[4] = 0xa0000002;
    param[5] = info->getHp();
    param[6] = (int)"@\x81\x5c";
    param[7] = (int)"@\x81\x5c";
    param[8] = (int)"@\x81\x5c";
    param[9] = info->getHpMax();
    param[10] = info->getMp();
    param[11] = (int)"@\x81\x5c";
    param[12] = (int)"@\x81\x5c";
    param[13] = (int)"@\x81\x5c";
    param[14] = info->getMpMax();
}

THUMB void unkfunc_0217d4f0(status::HaveStatusInfo* info, int* param, int flag)
{
    if (flag) {
        param[0] = info->haveStatus_.playerIndex_ + 0x50000000;
        param[1] = menu::MenuDataCommon::getAbilityKind(0);
        param[2] = info->getAttack(0);
        param[3] = menu::MenuDataCommon::getAbilityKind(1);
        param[4] = info->getDefence(0);
        return;
    }
    param[2] = info->haveStatus_.playerIndex_ + 0x50000000;
    param[3] = menu::MenuDataCommon::getAbilityKind(0);
    param[4] = info->getAttack(0);
    param[5] = menu::MenuDataCommon::getAbilityKind(1);
    param[6] = info->getDefence(0);
}

// not in the ROM (dead-stripped), keeps the unused parts lists of this file
THUMB void unkfunc_unused_10(int* param)
{
    unkfunc_02050ea8(s_parts_02184b8c, param);
    unkfunc_02050ea8(s_parts_02184c50, param);
}
