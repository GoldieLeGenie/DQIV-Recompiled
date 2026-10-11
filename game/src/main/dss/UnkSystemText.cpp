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

ARM void unkfunc_02086f6c(int type, int screen1, int screen2)
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
    UnkPaletteBuffer* bg = unkfunc_02081364(screen1);
    bg->unkfunc_020826d8(0xe, 0, palette1 + 0x10, 0x10);
    bg->unkfunc_020826d8(0xf, 0, palette1, 0x10);
    UnkPaletteBuffer* obj = unkfunc_020813e0(0);
    obj->unkfunc_020826d8(0xe, 0, palette2 + 0x10, 0x10);
    obj->unkfunc_020826d8(0xf, 0, palette2, 0x10);
    if (second == 0) {
        return;
    }
    bg = unkfunc_02081364(screen2);
    bg->unkfunc_020826d8(0xe, 0, palette1 + 0x10, 0x10);
    bg->unkfunc_020826d8(0xf, 0, palette1, 0x10);
    obj = unkfunc_020813e0(1);
    obj->unkfunc_020826d8(0xe, 0, palette2 + 0x10, 0x10);
    obj->unkfunc_020826d8(0xf, 0, palette2, 0x10);
}
