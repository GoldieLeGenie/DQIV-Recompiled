#pragma ipa file
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_02177bac.hpp"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/menu/UnkMenuPartsDraw.hpp"

// parts lists (their definition order sets the .data layout)
static UnkMenuParts s_parts_02183a2c[] = {
    { 0x0c, 0xf0, 0, 0, 0, 0x98, 0x20, 0x20 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183930[] = {
    { 0x15, 0xb0, 0, 0, 0, 0, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021839bc[] = {
    { 0x0f, 0x09, (short)0xf000, 0, 0, 0, 0x10, 0xe },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183f12[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x30, 0xa8, 0x64, 0xa },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 0x64, 0xa },
    { 0x0d, 0x0b, (short)0xf000, 2, 0, 0, 0x64, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183f82[] = {
    { 0x05, 0x00, (short)0xf000, 6, 0xb0, 0x18, 8, 8 },
    { 0x03, 0xd0, (short)0xf000, 0, 0xb0, 0x18, 0x48, 8 },
    { 0x05, 0x01, (short)0xf000, 6, 0xf8, 0x18, 8, 8 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183fba[] = {
    { 0x05, 0x00, (short)0xf000, 6, 8, 0x98, 8, 8 },
    { 0x03, 0xd0, (short)0xf000, 0, 8, 0x98, 0xf0, 8 },
    { 0x05, 0x01, (short)0xf000, 6, 0xf8, 0x98, 8, 8 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183e5c[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0, 0x98, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 1, 0xa0, 0, 0x58, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183a48[] = {
    { 0x0d, 0x09, (short)0xf000, 0, 0, 0, 0, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183ce2[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x18, 0x60, 0x38, 0xe },
    { 0x0d, 0x0a, (short)0xf000, 1, 0x30, 0x60, 0x2a, 0xe },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183ff2[] = {
    { 0x05, 0x00, (short)0xf000, 6, 0xb0, 0x18, 8, 8 },
    { 0x03, 0xd0, (short)0xf000, 0, 0xb0, 0x18, 0x48, 8 },
    { 0x05, 0x01, (short)0xf000, 6, 0xf8, 0x18, 8, 8 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183b60[] = {
    { 0x0f, 0x0a, (short)0xf000, 0, 0, 0, 0x38, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183b7c[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x98, 0, 0x40, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184612[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x18, 8, 0x38, 0xe },
    { 0x0d, 0x08, (short)0xf000, 1, 0x18, 0x18, 0xe, 0xe },
    { 0x0f, 0x0a, (short)0xf000, 2, 0x18, 0x18, 0x2a, 0xe },
    { 0x0d, 0x08, (short)0xf000, 3, 0x18, 0x26, 0xe, 0xe },
    { 0x0f, 0x0a, (short)0xf000, 4, 0x18, 0x26, 0x2a, 0xe },
    { 0x0d, 0x08, (short)0xf000, 5, 0x18, 0x34, 0xe, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 6, 0x18, 0x34, 0x2a, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183ab8[] = {
    { 0x0c, 0xf4, 0, 0, 0xe0, 0x9c, 0x18, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183bec[] = {
    { 0x0d, 0x05, (short)0xf000, 0, 0, 0, 0xc, 0xe },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183a9c[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0xf, 0x54, 0x60, 8 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0218409a[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x18, 0x60, 0x46, 0xe },
    { 0x0d, 0x0a, (short)0xf000, 1, 0x50, 0x60, 0xc, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 2, 0x5a, 0x60, 0x18, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183d36[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x18, 0x60, 0x46, 0xe },
    { 0x0d, 0x0a, (short)0xf000, 1, 0x64, 0x60, 0xc, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021843fe[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x30, 0xa0, 0x50, 0xc },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 0x10, 0xc },
    { 0x0d, 0x0b, (short)0xf000, 2, 0, 0, 0x30, 0xc },
    { 0x0d, 0x08, (short)0xf000, 3, 0x30, 0xac, 0x48, 0xc },
    { 0x0d, 0x08, (short)0xf000, 4, 0x70, 0xac, 0x2a, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021839d8[] = {
    { 0x0d, 0x09, (short)0xf000, 0, 0x10, 0, 0x14, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183d60[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0xf, 0x54, 0x10, 0xe },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 0x50, 0xe },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183af0[] = {
    { 0x15, 0xb0, 0, 0, 0xdd, 0x20, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183d8a[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x19, 6, 0x58, 0xa },
    { 0x0d, 0x0a, (short)0xf000, 1, 0x98, 6, 0x30, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183db4[] = {
    { 0x14, 0xc0, 0, -1, 0x24, 0x3c, 0x20, 0x20 },
    { 0x0c, 0xf1, 0, 0, 0x20, 0x38, 0x20, 0x20 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021842a0[] = {
    { 0x0f, 0x0a, (short)0xf000, 0, 8, 0, 0x20, 0xe },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 0x20, 0xe },
    { 0x0d, 0x0a, (short)0xf000, 2, 0x28, 0, 0x18, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 3, 0xb8, 0, 0x40, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0218394c[] = {
    { 0x15, 0xb5, 0, 0, 0, 0, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021842e6[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x18, 0x60, 0x38, 0xe },
    { 0x0f, 0x0a, (short)0xf000, 1, 0x30, 0x60, 0x2a, 0xe },
    { 0x0d, 0x08, (short)0xf000, 2, 0x5c, 0x60, 0xe, 0xe },
    { 0x0f, 0x0a, (short)0xf000, 3, 0x55, 0x60, 0x2a, 0xe },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184452[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x18, 0x60, 0x38, 0xe },
    { 0x0f, 0x0a, (short)0xf000, 1, 0x30, 0x60, 0x2a, 0xe },
    { 0x0d, 0x0b, (short)0xf000, 2, 0, 0, 0xe, 0xe },
    { 0x0d, 0x0b, (short)0xf000, 3, 0, 0, 0x2a, 0xe },
    { 0x0d, 0x0b, (short)0xf000, 4, 0, 0, 0x54, 0xe },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021839f4[] = {
    { 0x0f, 0x0a, (short)0xf000, 0, 0, 0, 0x30, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021840d2[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x18, 0x60, 0x46, 0xe },
    { 0x0d, 0x0a, (short)0xf000, 1, 0x50, 0x60, 0xc, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 2, 0x5a, 0x60, 0x18, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0218410a[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0xf, 0x54, 0x68, 0xa },
    { 0x0d, 0x0a, (short)0xf000, 1, 0x69, 0x54, 8, 0xa },
    { 0x0f, 0x0b, (short)0xf000, 2, 0, 0, 0xc, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183a80[] = {
    { 0x0f, 0x0a, (short)0xf000, 0, 0xc8, 8, 0x30, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static int s_int_021838e4[6] = { 0xa000000c, 0xa000000d, 0xa0000007, 0xa0000008, 0xa000000b, 0xa000000a };
static UnkMenuParts s_parts_02184142[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x19, 6, 0x50, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 1, 0x90, 6, 0x30, 0xa },
    { 0x0d, 0x0b, (short)0xf000, 2, 0, 0, 0x18, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0218417a[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0xf, 6, 0x10, 0xa },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 0x58, 0xa },
    { 0x0d, 0x0a, (short)0xf000, 2, 0x98, 6, 0x30, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183e08[] = {
    { 0x14, 0xc0, 0, -1, 4, 0, 0x21, 0x10 },
    { 0x0d, 0x09, (short)0xf000, 0, 0, 2, 0x29, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0218480a[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0, 0, 0x10, 0xa },
    { 0x0d, 0x08, (short)0xf000, 1, 0x10, 0, 0x10, 0xa },
    { 0x0d, 0x08, (short)0xf000, 2, 0x20, 0, 0x10, 0xa },
    { 0x0d, 0x08, (short)0xf000, 3, 0x30, 0, 0x10, 0xa },
    { 0x0d, 0x08, (short)0xf000, 4, 0x40, 0, 0x10, 0xa },
    { 0x0d, 0x08, (short)0xf000, 5, 0x58, 0, 0x10, 0xa },
    { 0x0d, 0x08, (short)0xf000, 6, 0x68, 0, 0x10, 0xa },
    { 0x0d, 0x08, (short)0xf000, 7, 0x78, 0, 0x10, 0xa },
    { 0x0d, 0x08, (short)0xf000, 8, 0x88, 0, 0x10, 0xa },
    { 0x0d, 0x08, (short)0xf000, 9, 0x98, 0, 0x10, 0xa },
    { 0x0d, 0x08, (short)0xf000, 0xa, 0xb0, 0, 0x28, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183b0c[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0, 0, 0x90, 0xe },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183e32[] = {
    { 0x14, 0xc0, 0, -1, 0, 4, 0x21, 0x10 },
    { 0x0d, 0x09, (short)0xf000, 0, 0, 6, 0x20, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0218432c[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0xf, 6, 0x10, 0xa },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 0x50, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 2, 0x90, 6, 0x30, 0xa },
    { 0x0d, 0x0b, (short)0xf000, 3, 0, 0, 0x18, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021841b2[] = {
    { 0x0d, 0x08, (short)0xf000, 4, 0x8a, 0, 0xe, 0xc },
    { 0x0d, 0x0b, (short)0xf000, 7, 0, 0, 0x18, 0xc },
    { 0x0f, 0x0b, (short)0xf000, 5, 8, 0, 0x18, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183b44[] = {
    { 0x0d, 0x0a, (short)0xf000, 0, 0, 0, 0x14, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021844fa[] = {
    { 0x14, 0xc2, 0, 0, 0, 0, 0x1c, 0x1c },
    { 0x0c, 0xf1, 0, 0, 0, 0, 0x20, 0x20 },
    { 0x0d, 0x08, (short)0xf000, 1, 0, 0, 0xa, 0xa },
    { 0x0d, 0x08, (short)0xf000, 2, 0, 0, 0xa, 0xa },
    { 0x0d, 0x08, (short)0xf000, 3, 0, 0, 0xa, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static int s_int_021838fc[6] = { 0xa000000c, 0xa000000d, 0xa0000007, 0xa0000008, 0xa000000b, 0xa000000a };
static UnkMenuParts s_parts_02183e86[] = {
    { 0x14, 0xc2, 0, 0, 0x1e, 0x2e, 0x1c, 0x1c },
    { 0x0c, 0xf1, 0, 0, 0x18, 0x28, 0x20, 0x20 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021841ea[] = {
    { 0x05, 0x00, (short)0xf000, 5, 0xb0, 0, 8, 8 },
    { 0x03, 0xd0, (short)0xf000, 0, 0xb0, 8, 8, 0x10 },
    { 0x05, 0x00, (short)0xf000, 0xb, 0xb0, 0x18, 8, 8 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184222[] = {
    { 0x0f, 0x08, (short)0xf000, 0, 8, 0, 0xa, 0xc },
    { 0x0d, 0x0b, (short)0xf000, 7, 0, 0, 0xe, 0xc },
    { 0x0d, 0x0b, (short)0xf000, 1, 8, 0, 0x50, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183b98[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0, 0, 0x58, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183a10[] = {
    { 0x0d, 0x09, (short)0xf000, 0, 0, 0x55, 0x100, 0xe },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183c24[] = {
    { 0x0d, 0x0a, (short)0xf000, 0, 0, 0, 0x30, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183bd0[] = {
    { 0x01, 0x00, (short)0xf000, 0, 0, 0, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183eb0[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x20, 0, 0x98, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 1, 0xb8, 0, 0x40, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static int s_int_02183c40[10] = { 1, 2, 4, 5, 8, 0xa, 0x14, 0x64, 0xfa, 0x1f4 };
static UnkMenuParts s_parts_02183eda[] = {
    { 0x05, 0x00, (short)0xf000, 5, 0xb0, 0, 8, 8 },
    { 0x03, 0xd0, (short)0xf000, 0, 0xb0, 8, 8, 0x38 },
    { 0x05, 0x00, (short)0xf000, 0xb, 0xb0, 0x40, 8, 8 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0218425a[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x30, 0xa0, 0x48, 0xc },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 0x18, 0xc },
    { 0x0d, 0x0b, (short)0xf000, 2, 0, 0, 0xa8, 0xc },
    { 0x0d, 0x08, (short)0xf000, 3, 0x30, 0xac, 0xf0, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183914[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x18, 0, 0xc, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static int s_int_02183c68[10] = { 0xa0000135, 0xa0000134, 0xa0000133, 0xa0000132, 0xa0000131, 0xa0000130, 0xa000012f, 0xa000012e, 0xa000012d, 0xa000012c };
static UnkMenuParts s_parts_0218454e[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x18, 8, 0x38, 0xe },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 0xe, 0xe },
    { 0x0d, 0x08, (short)0xf000, 2, 0x18, 0x18, 0xe, 0xe },
    { 0x0f, 0x0a, (short)0xf000, 3, 0x18, 0x18, 0x2a, 0xe },
    { 0x0d, 0x08, (short)0xf000, 4, 0x18, 0x26, 0xe, 0xe },
    { 0x0f, 0x0a, (short)0xf000, 5, 0x18, 0x26, 0x2a, 0xe },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021845b0[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x30, 0xa0, 0x50, 0xc },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 0x10, 0xc },
    { 0x0d, 0x0b, (short)0xf000, 2, 0, 0, 0x30, 0xc },
    { 0x0d, 0x08, (short)0xf000, 3, 0x30, 0xac, 0x48, 0xc },
    { 0x0d, 0x08, (short)0xf000, 4, 0x70, 0xac, 0x10, 0xc },
    { 0x0f, 0x0b, (short)0xf000, 5, 0, 0, 0x20, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183f4a[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x30, 0xa0, 0x50, 0xc },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 0x10, 0xc },
    { 0x0d, 0x0b, (short)0xf000, 2, 0, 0, 0x30, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021839a0[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0, 0, 0x38, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183cb8[] = {
    { 0x0f, 0x0a, (short)0xf000, 0, 0xb8, 8, 0x30, 0x10 },
    { 0x0d, 0x0a, (short)0xf000, 1, 0xe8, 8, 0x10, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0218402a[] = {
    { 0x05, 0x00, (short)0xf000, 6, 0xb0, 0x48, 8, 8 },
    { 0x03, 0xd0, (short)0xf000, 0, 0xb0, 0x48, 0x48, 8 },
    { 0x05, 0x01, (short)0xf000, 6, 0xf8, 0x48, 8, 8 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183984[] = {
    { 0x15, 0xb1, 0, 0, 0xdd, 0x20, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184682[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x10, 0, 0x64, 0x10 },
    { 0x0d, 0x0b, (short)0xf000, 6, 0, 0, 0xa, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 1, 0x74, 0, 0xa, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 2, 0x7e, 0, 0x18, 0x24 },
    { 0x0d, 0x0b, (short)0xf000, 3, 0, 0, 0xa, 0x10 },
    { 0x0f, 0x0b, (short)0xf000, 4, 0, 0, 8, 0xe },
    { 0x0d, 0x0b, (short)0xf000, 5, 0, 0, 0xc, 0xe },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184062[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x18, 0x60, 0x38, 0xe },
    { 0x0d, 0x0a, (short)0xf000, 1, 0x4b, 0x60, 0x1c, 0xe },
    { 0x0f, 0x0a, (short)0xf000, 2, 0x55, 0x60, 0x2a, 0xe },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183d0c[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x18, 0x60, 0x46, 0xe },
    { 0x0d, 0x0a, (short)0xf000, 1, 0x64, 0x60, 0xc, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183bb4[] = {
    { 0x01, 0x00, (short)0xf000, 0, 0, 0, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184762[] = {
    { 0x0f, 0x08, (short)0xf000, 0, 8, 0, 0xe, 0xc },
    { 0x0d, 0x0b, (short)0xf000, 7, 0, 0, 0xe, 0xc },
    { 0x0d, 0x0b, (short)0xf000, 1, 8, 0, 0x50, 0xc },
    { 0x0d, 0x08, (short)0xf000, 2, 0x58, 0, 0x28, 0xc },
    { 0x0f, 0x0a, (short)0xf000, 6, 0xa8, 0, 0x18, 0xc },
    { 0x0f, 0x0b, (short)0xf000, 0xb, 0, 0, 0xa, 0xc },
    { 0x0f, 0x0b, (short)0xf000, 0xc, 0, 0, 0xa, 0xc },
    { 0x0d, 0x0b, (short)0xf000, 7, 0, 0, 8, 0xc },
    { 0x0f, 0x0b, (short)0xf000, 8, 0, 0, 0xa, 0xc },
    { 0x0f, 0x0b, (short)0xf000, 9, 0, 0, 0xa, 0xc },
    { 0x0d, 0x08, (short)0xf000, 0xa, 0x74, 0xc, 0x50, 0xe },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183c08[] = {
    { 0x15, 0xb0, 0, 0, 0, 0, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183968[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0, 0, 0x38, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183dde[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x98, 0, 0x40, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 1, 0xc0, 0, 0x38, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183a64[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0, 0, 0x38, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021846f2[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x30, 0xa0, 0x50, 0xc },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 0x10, 0xc },
    { 0x0d, 0x0b, (short)0xf000, 2, 0, 0, 0x30, 0xc },
    { 0x0d, 0x08, (short)0xf000, 3, 0x30, 0xac, 0x48, 0xc },
    { 0x0f, 0x08, (short)0xf000, 4, 0x70, 0xac, 0x20, 0xc },
    { 0x0d, 0x0b, (short)0xf000, 5, 0, 0, 0x10, 0xc },
    { 0x0f, 0x0b, (short)0xf000, 6, 0, 0, 0x20, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183ad4[] = {
    { 0x15, 0xb3, 0, 0, 0, 0, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static int s_int_02183c90[10] = { 0xa0000135, 0xa0000134, 0xa0000133, 0xa0000132, 0xa0000131, 0xa0000130, 0xa000012f, 0xa000012e, 0xa000012d, 0xa000012c };
static UnkMenuParts s_parts_021844a6[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x18, 6, 0x50, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 1, 0x90, 6, 0x30, 0xa },
    { 0x0d, 0x0b, (short)0xf000, 2, 0, 0, 8, 0xa },
    { 0x0d, 0x0a, (short)0xf000, 3, 0xc8, 6, 0x18, 0xa },
    { 0x0f, 0x0b, (short)0xf000, 4, 0, 0, 0x18, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02184372[] = {
    { 0x14, 0xc0, 0, -1, 0, 0, 0x58, 0x18 },
    { 0x14, 0xc0, 0, -1, 0x68, 0, 0x58, 0x18 },
    { 0x0d, 0x09, (short)0xf000, 0, 0, 6, 0x58, 0x28 },
    { 0x0d, 0x09, (short)0xf000, 1, 0x68, 6, 0x58, 0x28 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02183b28[] = {
    { 0x15, 0xb5, 0, 0, 0, 0, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021843b8[] = {
    { 0x0c, 0xf3, 0, 0, -32, -6, 0x20, 0x20 },
    { 0x0c, 0xf3, 0, 1, 8, -6, 0x20, 0x20 },
    { 0x0d, 0x0a, (short)0xf000, 2, 0, 0, 8, 0xc },
    { 0x0f, 0x0b, (short)0xf000, 3, 0, 0, 0x10, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};

// not in the ROM (dead-stripped), its local array initializer is still in .rodata
THUMB void unkfunc_unused_1(int index)
{
    int message[4] = { 0x8000012c, 0x8000012d, 0x8000012c, 0x8000012d };
    unkfunc_02050ed0(s_parts_02183a2c, &message[index], 1);
}

THUMB void unkfunc_02177bac(int x, int y, int w, int h, int lineY)
{
    s_parts_02183bd0[0].x_ = x;
    s_parts_02183bd0[0].y_ = y;
    s_parts_02183bd0[0].w_ = w;
    s_parts_02183bd0[0].h_ = h;
    unkfunc_02050ea8(s_parts_02183bd0, 0);
    if (lineY >= 1) {
        UnkMenuParts line[] = {
            { 0x16, 0x00, (short)0xf000, 0, 0, 0, 0, 0 },
            { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
        };
        line[0].x_ = x;
        line[0].y_ = lineY;
        line[0].w_ = w;
        unkfunc_02050ea8(line, 0);
    }
}

THUMB void unkfunc_02177c00(int x, int y, int w, int h, int lineY)
{
    s_parts_02183bb4[0].x_ = x;
    s_parts_02183bb4[0].y_ = y;
    s_parts_02183bb4[0].w_ = w;
    s_parts_02183bb4[0].h_ = h;
    unkfunc_02050ea8(s_parts_02183bb4, 0);
    if (lineY >= 1) {
        UnkMenuParts line[] = {
            { 0x16, 0x00, (short)0xf000, 0, 0, 0, 0, 0 },
            { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
        };
        line[0].x_ = x;
        line[0].y_ = lineY;
        line[0].w_ = w;
        unkfunc_02050ea8(line, 0);
    }
}

THUMB void unkfunc_02177c54(int flag)
{
    if (flag) {
        unkfunc_02050ea8(s_parts_02183eda, 0);
    } else {
        unkfunc_02050ea8(s_parts_021841ea, 0);
    }
}

THUMB void unkfunc_02177c78(bool flag)
{
    if (flag) {
        unkfunc_02050ea8(s_parts_02183984, 0);
    } else {
        unkfunc_02050ea8(s_parts_02183af0, 0);
    }
}

THUMB void unkfunc_02177c9c(int* message, int count, int x, int y)
{
    int param = 0;
    s_parts_02183b98[0].x_ = x;
    s_parts_02183b98[0].y_ = y;
    for (int i = 0; i < count; i++) {
        param = message[i];
        unkfunc_02050ee0(s_parts_02183b98, &param, 0, i * 16, 1);
    }
}

THUMB void unkfunc_02177ce0(int* message, int count, int x, int y)
{
    int param = 0;
    s_parts_02183b0c[0].x_ = x;
    s_parts_02183b0c[0].y_ = y;
    for (int i = 0; i < count; i++) {
        param = message[i];
        unkfunc_02050ee0(s_parts_02183b0c, &param, 0, i * 14, 1);
    }
}

// not in the ROM (dead-stripped), its local array initializer is still in .rodata
THUMB void unkfunc_unused_2(int index)
{
    int message[4] = { 0x800000cf, 0x800000c8, 0x800000c9, 0x800000cf };
    unkfunc_02050ed0(s_parts_021839bc, &message[index], 1);
}

THUMB void unkfunc_02177d24(int coin, int bet, int a, int b)
{
    int param[2] = { 0xa0000138, 0 };
    param[1] = coin;
    int flag = 1;
    unkfunc_02050ee0(s_parts_02183dde, param, 0, 8, flag);
    param[1] = bet;
    if (a) {
        param[0] = 0xa000013a;
    } else {
        param[0] = 0xa0000139;
    }
    if (b) {
        flag = 5;
    }
    if (bet == 0xffff) {
        unkfunc_02050ee0(s_parts_02183b7c, param, 0, 0x1a, flag);
    } else {
        unkfunc_02050ee0(s_parts_02183dde, param, 0, 0x1a, flag);
    }
}

// not in the ROM (dead-stripped), its local array initializer is still in .rodata
THUMB void unkfunc_unused_3(int coin)
{
    int param[2] = { 0, 0xa000003c };
    param[0] = coin;
    unkfunc_02050ed0(s_parts_02183f12, param, 1);
}

// not in the ROM (dead-stripped), its local array initializer and strings are still in .rodata/.data
THUMB void unkfunc_unused_4(int rank, int time)
{
    const char* name[15] = { "\xff\xfe\xb8\x24", "\xff\xfe\x21\xff", "\xff\xfe\x9e\x24", "\xff\xfe\x9f\x24", "\xff\xfe\xa0\x24",
                             "\xff\xfe\xa1\x24", "\xff\xfe\xa2\x24", "\xff\xfe\xa3\x24", "\xff\xfe\xa4\x24", "\xff\xfe\xa5\x24",
                             "\xff\xfe\x9d\x24\xff\xfe\x9c\x24", "\xff\xfe\x2a\xff", "\xff\xfe\x31\xff", "\xff\xfe\x2b\xff", (const char*)1 };
    int param[3];
    param[0] = (int)name[rank];
    param[1] = (int)"\xff\xfe\xa9\x24";
    param[2] = (int)":";
    unkfunc_02050ed0(s_parts_02183f82, param, time);
}

THUMB void unkfunc_02177da0(int* cards, int a)
{
    int param = 0;
    int x[5] = { 0, 0x30, 0x60, 0x90, 0xc0 };
    for (int i = 0; i < 5; i++) {
        if (a == 1) {
            param = 0xa000013b;
        } else if (cards[i] == 1) {
            param = 0x80000191;
        } else {
            param = 0x80000190;
        }
        if (a != 1 || i != 0) {
            unkfunc_02050ee0(s_parts_02183e08, &param, x[i] + 0xb, 0x7a, 1);
        }
    }
    if (a != 1) {
        param = 0x80000192;
        unkfunc_02050ee0(s_parts_02183e32, &param, 0x6f, 0x8c, 1);
    }
}

THUMB void unkfunc_02177e34(int coin, int a, int b)
{
    unkfunc_02050698(0, 0);
    int param[2] = { 0, 0 };
    if (coin == 0) {
        coin = 1;
    }
    for (int i = 9; i >= 0; i--) {
        param[0] = s_int_02183c68[i];
        param[1] = s_int_02183c40[i] * coin;
        if (a == i) {
            if (b) {
                unkfunc_02050ee0(s_parts_02183e5c, param, 0, 0xa4 - i * 16, 5);
            }
        } else {
            unkfunc_02050ee0(s_parts_02183e5c, param, 0, 0xa4 - i * 16, 1);
        }
    }
}

THUMB void unkfunc_02177eb8(int monsterID, int diameter, int index, int orderCount)
{
    if (monsterID < 0) {
        return;
    }
    const char* order[5] = { "", " A", " B", " C", " D" };
    int param[7];
    param[0] = monsterID + 0x60000000;
    param[1] = (int)"\xff\xfe\xa9\x24";
    param[2] = diameter / 10;
    param[3] = (int)"@.";
    param[4] = diameter % 10;
    param[5] = 0xa000013c;
    param[6] = (int)order[orderCount + 1];
    unkfunc_02050ee0(s_parts_02184682, param, 0, index * 0x18 + 0x10, 1);
}

THUMB void unkfunc_02177f38(int index)
{
    unkfunc_02050ebc(s_parts_02183b28, 0, 0xc, index * 0x18 + 0x16);
}

THUMB void unkfunc_02177f54(int coin, int x, int y, int flag)
{
    if (flag == 1) {
        int digit = coin / 10;
        unkfunc_02050ee0(s_parts_02183b60, &digit, x + 0xb2, y + 0xa, 1);
        digit = coin % 10;
        unkfunc_02050ee0(s_parts_02183b60, &digit, x + 0xb8, y + 0xa, 1);
        return;
    }
    int param = coin;
    unkfunc_02050ee0(s_parts_02183b60, &param, x + 0xb8, y + 0xa, 1);
    if (flag != 0 && coin < 10) {
        param = 0;
        unkfunc_02050ee0(s_parts_02183b60, &param, x + 0xb0, y + 0xa, 1);
    }
}

THUMB void unkfunc_02177fe0(int* message, int x, int y, int w, int h)
{
    s_parts_02183a48[0].x_ = x;
    s_parts_02183a48[0].y_ = y + ((h - y) >> 1);
    s_parts_02183a48[0].w_ = w;
    s_parts_02183a48[0].h_ = 10;
    unkfunc_02050ed0(s_parts_02183a48, message, 1);
}

THUMB void unkfunc_0217800c(int index, char* name, int chapter, int level, int town, int time, int y, int flag)
{
    UnkMenuParts* draw = s_parts_02184762;
    int mode;
    if (flag) {
        mode = 1;
    } else {
        mode = 2;
    }
    unsigned int hour = time / 216000;
    unsigned int minute = time % 216000 / 3600;
    int param[13];
    param[0] = index + 1;
    param[1] = (int)name;
    param[4] = 0xa000003f;
    param[5] = level;
    param[6] = hour / 100;
    param[7] = (int)":";
    param[8] = minute / 10;
    param[9] = minute % 10;
    param[10] = town;
    param[11] = hour % 100 / 10;
    param[12] = hour % 10;
    s_parts_02184762[3].type_ = 0xd;
    switch (chapter) {
        case 0:
            param[2] = 0xa0000323;
            break;
        case 1:
            param[2] = 0xa0000324;
            break;
        case 2:
            param[2] = 0xa0000325;
            break;
        case 3:
            param[2] = 0xa0000326;
            break;
        case 4:
            param[2] = 0xa0000327;
            break;
        case 5:
            param[2] = 0xa0000328;
            break;
        case 6:
            param[2] = 0xa0000329;
            break;
        default:
            s_parts_02184762[3].type_ = 0;
            break;
    }
    if (town == -1) {
        param[10] = 0x80000032;
    }
    if (name == 0) {
        param[1] = 0xa0000321;
        draw = s_parts_02184222;
        if (flag) {
            mode = 2;
        } else {
            mode = 1;
        }
    }
    unkfunc_02050ee0(draw, param, 0x10, y + index * 0x1a, mode);
    if (chapter >= 5) {
        unkfunc_02050ee0(s_parts_021841b2, param, 0x10, y + index * 0x1a, mode);
    }
}

THUMB void unkfunc_02178180(int* name, int type)
{
    int y = 0x10;
    if (type == 1) {
        y += 0x18;
    }
    for (int i = 0; i < 6; i++) {
        int param[11];
        for (int j = 0; j < 11; j++) {
            param[j] = *name++;
        }
        unkfunc_02050ee0(s_parts_0218480a, param, 0x18, i * 12 + 0x44 + y, 1);
    }
}

THUMB void unkfunc_021781c4(int country, int* name, int count, int max, int mode, int flag)
{
    int i;
    int x = 0x8c - (max >> 1) * 14 - (max % 2) * 7;
    int y = 0x18;
    if (flag == 1) {
        x -= 8;
        y += 4;
    }
    for (i = 0; i < max; i++) {
        int param = 0;
        if (count < i) {
            param = (int)"@\x81Q";
            if (mode == 2) {
                param = (int)"@ ";
            }
        } else {
            param = name[i];
        }
        if (count != i) {
            unkfunc_02050ee0(s_parts_02183bec, &param, x + i * 12, y, 1);
        } else if (mode == 0) {
            unkfunc_02050ee0(s_parts_02183c08, 0, x + i * 12 + 6, 0x28, 1);
        }
    }
}

THUMB void unkfunc_02178278(int* name, int count)
{
    for (int i = 0; i < 45; i++) {
        int param = name[i];
        int x = 0;
        if (count < i) {
            param = (int)"@*";
            x = 2;
        }
        if (count != i) {
            unkfunc_02050ee0(s_parts_02183914, &param, x + i % 15 * 14, i / 15 * 16 + 0xe, 1);
        } else {
            unkfunc_02050ee0(s_parts_02183930, 0, i % 15 * 14 + 0x1e, i / 15 * 16 + 0x1c, 1);
        }
    }
}

// not in the ROM (dead-stripped), keeps the unused parts lists of this file and its const locals
THUMB void unkfunc_unused_5(int* param)
{
    const int unk0 = 0x58;
    const int unk1 = 0x90;
    const int unk2 = 0x30;
    const int unk3 = 0xa;
    const int unk4 = 0x10;
    const int unk5 = 0x10;
    const int unk6 = 0xe;
    unkfunc_02050ea8(s_parts_02183a2c, param);
    unkfunc_02050ea8(s_parts_021839bc, param);
    unkfunc_02050ea8(s_parts_02183f12, param);
    unkfunc_02050ea8(s_parts_02183f82, param);
    unkfunc_02050ea8(s_parts_02183fba, param);
    unkfunc_02050ea8(s_parts_02183ce2, param);
    unkfunc_02050ea8(s_parts_02183ff2, param);
    unkfunc_02050ea8(s_parts_02184612, param);
    unkfunc_02050ea8(s_parts_02183ab8, param);
    unkfunc_02050ea8(s_parts_02183a9c, param);
    unkfunc_02050ea8(s_parts_0218409a, param);
    unkfunc_02050ea8(s_parts_02183d36, param);
    unkfunc_02050ea8(s_parts_021843fe, param);
    unkfunc_02050ea8(s_parts_021839d8, param);
    unkfunc_02050ea8(s_parts_02183d60, param);
    unkfunc_02050ea8(s_parts_02183d8a, param);
    unkfunc_02050ea8(s_parts_02183db4, param);
    unkfunc_02050ea8(s_parts_021842a0, param);
    unkfunc_02050ea8(s_parts_0218394c, param);
    unkfunc_02050ea8(s_parts_021842e6, param);
    unkfunc_02050ea8(s_parts_02184452, param);
    unkfunc_02050ea8(s_parts_021839f4, param);
    unkfunc_02050ea8(s_parts_021840d2, param);
    unkfunc_02050ea8(s_parts_0218410a, param);
    unkfunc_02050ea8(s_parts_02183a80, param);
    unkfunc_02050ea8((UnkMenuParts*)s_int_021838e4, param);
    unkfunc_02050ea8(s_parts_02184142, param);
    unkfunc_02050ea8(s_parts_0218417a, param);
    unkfunc_02050ea8(s_parts_0218432c, param);
    unkfunc_02050ea8(s_parts_02183b44, param);
    unkfunc_02050ea8(s_parts_021844fa, param);
    unkfunc_02050ea8((UnkMenuParts*)s_int_021838fc, param);
    unkfunc_02050ea8(s_parts_02183e86, param);
    unkfunc_02050ea8(s_parts_02183a10, param);
    unkfunc_02050ea8(s_parts_02183c24, param);
    unkfunc_02050ea8(s_parts_02183eb0, param);
    unkfunc_02050ea8(s_parts_0218425a, param);
    unkfunc_02050ea8(s_parts_0218454e, param);
    unkfunc_02050ea8(s_parts_021845b0, param);
    unkfunc_02050ea8(s_parts_02183f4a, param);
    unkfunc_02050ea8(s_parts_021839a0, param);
    unkfunc_02050ea8(s_parts_02183cb8, param);
    unkfunc_02050ea8(s_parts_0218402a, param);
    unkfunc_02050ea8(s_parts_02184062, param);
    unkfunc_02050ea8(s_parts_02183d0c, param);
    unkfunc_02050ea8(s_parts_02183968, param);
    unkfunc_02050ea8(s_parts_02183a64, param);
    unkfunc_02050ea8(s_parts_021846f2, param);
    unkfunc_02050ea8(s_parts_02183ad4, param);
    unkfunc_02050ea8((UnkMenuParts*)s_int_02183c90, param);
    unkfunc_02050ea8(s_parts_021844a6, param);
    unkfunc_02050ea8(s_parts_02184372, param);
    unkfunc_02050ea8(s_parts_021843b8, param);
}
