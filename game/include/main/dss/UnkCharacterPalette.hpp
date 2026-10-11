#pragma once
#include <globaldefs.h>
#include "main/dss/TextureObject.hpp"

// copy of the palette of a texture, loaded in its own palette VRAM block
struct UnkCharacterPalette {
    unsigned char unk_000[0x400];               // 0x000 palette
    int unk_400;                                // 0x400 texture format
    int unk_404;                                // 0x404 palette size
    int unk_408;                                // 0x408 palette VRAM address
    int unk_40c;                                // 0x40C palette VRAM handle
    int enable_;                                // 0x410

    UnkCharacterPalette();
    ~UnkCharacterPalette();
    void unkfunc_02086ccc(TextureObject* texture, int flag);    // copy the palette of a texture
    void unkfunc_02086d4c();                    // free the VRAM
    void unkfunc_02086d78(int flag);            // transfer to the VRAM
    void unkfunc_02086dac();                    // G3 palette base
};
