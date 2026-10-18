#include "main/dss/UnkSystemText.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/dss/UnkBgBuffer.hpp"
#include "main/menu/UnkMenuIconDisplay.hpp"

void* data_0211fd20[5];

ARM void unkfunc_02086de4(void* res0, void* res1, void* res2, void* res3, void* res4)
{
    for (int i = 0; i < 5; i++) {
        data_0211fd20[i] = NULL;
    }
    data_0211fd20[0] = res0;
    data_0211fd20[1] = res1;
    data_0211fd20[2] = res2;
    data_0211fd20[3] = res3;
    data_0211fd20[4] = res4;
    unsigned char* tile = (unsigned char*)unkfunc_02086e50(1);
    for (int i = 0; i < 0xa0; i++) {
        unkfunc_02080ba0(tile, NULL);
        tile += 0x20;
    }
}

ARM void* unkfunc_02086e50(int index)
{
    return data_0211fd20[index];
}

ARM void* unkfunc_02086e60(int index)
{
    return data_0211fd20[index];
}

ARM void unkfunc_02086e70(int type, int screen1, int screen2)
{
    if (type == 0) {
        return;
    }
    int sub = 0;
    int second = 1;
    if (type == 3) {
        sub = 1;
        second = 0;
    }
    unkfunc_020827f0(unkfunc_02082a30(screen1), 0, unkfunc_02086e50(0), 0x1000);
    UnkCharBuffer buffer;
    void* chr = unkfunc_02086e50(3);
    unkfunc_02080110(&buffer, chr, 0x20, 8);
    unkfunc_020827f0(0x13, 0x2000, chr, 0x2000);
    if (sub) {
        unkfunc_020827f0(unkfunc_02082a30(screen1), 0x8000, unkfunc_02086e50(0), 0x1000);
    }
    if (second) {
        unkfunc_020827f0(unkfunc_02082a30(screen2), 0, unkfunc_02086e50(0), 0x1000);
    }
}

ARM void loadSystemTextPalettes(int type, int screen1, int screen2)
{
    if (type == 0) {
        return;
    }
    int second = 1;
    if (type == 3) {
        second = 0;
    }
    unsigned short* palette1 = (unsigned short*)unkfunc_02086e60(2);
    unsigned short* palette2 = (unsigned short*)unkfunc_02086e60(4);
    unsigned short defaultPalette[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x7fff};
    if (palette1 == NULL) {
        palette1 = defaultPalette;
    }
    if (palette2 == NULL) {
        palette2 = defaultPalette;
    }
#ifdef DQIV_NATIVE
    // Text color 12 (green, shading 13 and 14) for the PC version's shop
    // comparisons; no DS text uses colors 12 to 14 of this palette.
    alignas(4) unsigned short textPalette[16];
    for (int i = 0; i < 16; i++) {
        textPalette[i] = palette1[i];
    }
    textPalette[12] = 6 | (29 << 5) | (6 << 10);
    textPalette[13] = 2 | (15 << 5) | (3 << 10);
    textPalette[14] = 1 | (7 << 5) | (1 << 10);
#else
    unsigned short* textPalette = palette1;
#endif
    UnkPaletteBuffer* bg = unkfunc_02081364(screen1);
    bg->loadColors(0xe, 0, palette1 + 0x10, 0x10);
    bg->loadColors(0xf, 0, textPalette, 0x10);
    UnkPaletteBuffer* obj = unkfunc_020813e0(0);
    obj->loadColors(0xe, 0, palette2 + 0x10, 0x10);
    obj->loadColors(0xf, 0, palette2, 0x10);
    if (second == 0) {
        return;
    }
    bg = unkfunc_02081364(screen2);
    bg->loadColors(0xe, 0, palette1 + 0x10, 0x10);
    bg->loadColors(0xf, 0, textPalette, 0x10);
    obj = unkfunc_020813e0(1);
    obj->loadColors(0xe, 0, palette2 + 0x10, 0x10);
    obj->loadColors(0xf, 0, palette2, 0x10);
}
