#include "main/dss/SystemText.hpp"
#include "main/dss/Display.hpp"
#include "main/dss/BgBuffer.hpp"
#include "main/menu/UnkMenuIconDisplay.hpp"

void* g_systemTextResources[5];

ARM void initSystemText(void* res0, void* res1, void* res2, void* res3, void* res4)
{
    for (int i = 0; i < 5; i++) {
        g_systemTextResources[i] = NULL;
    }
    g_systemTextResources[0] = res0;
    g_systemTextResources[1] = res1;
    g_systemTextResources[2] = res2;
    g_systemTextResources[3] = res3;
    g_systemTextResources[4] = res4;
    unsigned char* tile = (unsigned char*)getSystemTextData(1);
    for (int i = 0; i < 0xa0; i++) {
        flipTileHorizontal(tile, NULL);
        tile += 0x20;
    }
}

ARM void* getSystemTextData(int index)
{
    return g_systemTextResources[index];
}

ARM void* getSystemTextPalette(int index)
{
    return g_systemTextResources[index];
}

ARM void loadSystemTextChars(int type, int screen1, int screen2)
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
    requestVramCopy(getBgCharVramType(screen1), 0, getSystemTextData(0), 0x1000);
    UnkCharBuffer buffer;
    void* chr = getSystemTextData(3);
    initCharBuffer(&buffer, chr, 0x20, 8);
    requestVramCopy(0x13, 0x2000, chr, 0x2000);
    if (sub) {
        requestVramCopy(getBgCharVramType(screen1), 0x8000, getSystemTextData(0), 0x1000);
    }
    if (second) {
        requestVramCopy(getBgCharVramType(screen2), 0, getSystemTextData(0), 0x1000);
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
    unsigned short* palette1 = (unsigned short*)getSystemTextPalette(2);
    unsigned short* palette2 = (unsigned short*)getSystemTextPalette(4);
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
    UnkPaletteBuffer* bg = getBgPaletteBuffer(screen1);
    bg->loadColors(0xe, 0, palette1 + 0x10, 0x10);
    bg->loadColors(0xf, 0, textPalette, 0x10);
    UnkPaletteBuffer* obj = getObjPaletteBuffer(0);
    obj->loadColors(0xe, 0, palette2 + 0x10, 0x10);
    obj->loadColors(0xf, 0, palette2, 0x10);
    if (second == 0) {
        return;
    }
    bg = getBgPaletteBuffer(screen2);
    bg->loadColors(0xe, 0, palette1 + 0x10, 0x10);
    bg->loadColors(0xf, 0, textPalette, 0x10);
    obj = getObjPaletteBuffer(1);
    obj->loadColors(0xe, 0, palette2 + 0x10, 0x10);
    obj->loadColors(0xf, 0, palette2, 0x10);
}
