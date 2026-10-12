#pragma once
#include <globaldefs.h>
#include "main/dss/DssCore.hpp"

// texture file of the dss engine: texture + palette (+ second palette for the types 1 and 2), loaded in VRAM
struct TextureObject {
    unsigned char unk_00[0x14];                 // 0x00
    volatile int unk_14;                        // 0x14 bit 0: loaded in VRAM
    int unk_18;                                 // 0x18 type
    int unk_1c;                                 // 0x1C
    int unk_20;                                 // 0x20 texture format
    int unk_24;                                 // 0x24 width (GX size S)
    int unk_28;                                 // 0x28 height (GX size T)
    int unk_2c;                                 // 0x2C
    unsigned int unk_30;                        // 0x30 texture size
    int unk_34;                                 // 0x34 texture data (file offset, then address)
    unsigned int unk_38;                        // 0x38 palette size
    int unk_3c;                                 // 0x3C palette data (file offset, then address)
    int unk_40;                                 // 0x40 texture VRAM handle
    int unk_44;                                 // 0x44 palette VRAM handle
    int unk_48;                                 // 0x48 texture VRAM address
    int unk_4c;                                 // 0x4C palette VRAM address
    unsigned int unk_50;                        // 0x50 second texture size
    int unk_54;                                 // 0x54 second texture data
    unsigned int unk_58;                        // 0x58 second palette size
    int unk_5c;                                 // 0x5C second palette data
    int unk_60;                                 // 0x60
    int unk_64;                                 // 0x64 second palette VRAM handle
    int unk_68;                                 // 0x68
    int unk_6c;                                 // 0x6C second palette VRAM address

    TextureObject();
    ~TextureObject();
    void unkfunc_02086798(int flag);            // relocate, allocate the VRAM and transfer
#ifdef DQIV_NATIVE
    void setupReadOnly(const TextureObject& source, int flag);
    int relocated(int field) const;             // a file offset made an address, once
#endif
    void unkfunc_02086868();                    // free the VRAM
    void unkfunc_020868dc();                    // allocate the VRAM
    void unkfunc_02086968(int flag);            // transfer to the VRAM
    void unkfunc_020869ec(TextureObject* src, int flag);    // transfer the data of another texture
    int unkfunc_02086a94();                     // texture address
    int unkfunc_02086a9c();                     // texture data
    int unkfunc_02086aa4();                     // palette address
    int unkfunc_02086aac();                     // palette data
    int unkfunc_02086ab4();                     // palette size
    void unkfunc_02086abc();                    // G3 texture parameters (repeat)
    void unkfunc_02086af4();                    // G3 texture parameters
    void unkfunc_02086b3c();                    // G3 palette base
    void unkfunc_02086b68();                    // G3 texture parameters of the second texture
    void unkfunc_02086bd8();                    // G3 palette base of the second palette
    int unkfunc_02086c18();                     // width
    int unkfunc_02086c64();                     // height
};

void unkfunc_02086b28();                        // G3 no texture
