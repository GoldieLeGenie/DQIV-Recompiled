#pragma once
#include <globaldefs.h>

struct UnkPaletteBuffer;

// BG character buffer of the dss engine (tiles of 0x20 bytes)
struct UnkCharBuffer {
    void* buf_;                                 // 0x00
    int width_;                                 // 0x04
    int height_;                                // 0x08
    int pitch_;                                 // 0x0C
};

// BG screen buffer of the dss engine (one entry per cell)
struct UnkScreenBuffer {
    unsigned short* buf_;                       // 0x00
    unsigned short* base_;                      // 0x04
    int width_;                                 // 0x08
    int height_;                                // 0x0C
    int pitch_;                                 // 0x10
};

void unkfunc_02080110(UnkCharBuffer* buffer, void* buf, int width, int height);
void* unkfunc_0208011c(UnkCharBuffer* buffer, int x, int y);            // tile address
void unkfunc_02080130(UnkCharBuffer* buffer, unsigned int pattern);     // fill
void unkfunc_0208015c(UnkCharBuffer* buffer, int x, int y, int w, int h, unsigned int pattern);  // fill tiles
void unkfunc_020801ec(UnkCharBuffer* buffer, int x, int y, int color);  // set a pixel
void unkfunc_0208021c(UnkCharBuffer* buffer, int x, int y, int flip, int bank, unsigned short chr);   // copy a system tile
void unkfunc_02080278(UnkScreenBuffer* buffer, void* buf, int width, int height);
void unkfunc_02080288(UnkScreenBuffer* buffer, void* buf, int width, int height, int pitch);
unsigned short* unkfunc_020802a0(UnkScreenBuffer* buffer, int x, int y);    // cell address
void unkfunc_020802b4(UnkScreenBuffer* buffer, unsigned short value);   // fill
void unkfunc_02080334(UnkScreenBuffer* buffer, int x, int y, int w, int h, unsigned short value);   // fill a rect
void unkfunc_020803b8(UnkScreenBuffer* buffer, int x, int y, int w, int h, int palette, unsigned short chr);
void unkfunc_020803ec(UnkScreenBuffer* buffer, int x, int y, int w, int h, int palette, int vflip, int hflip, unsigned short chr);
void unkfunc_02080430(UnkScreenBuffer* buffer, int x, int y, unsigned short value);
void unkfunc_02080444(UnkScreenBuffer* buffer, int x, int y, int w, int h, int palette);   // map the character buffer to a rect
void unkfunc_020805bc(UnkScreenBuffer* buffer, int x, int y, const char* text);     // ascii text (palette 15)
void unkfunc_0208060c(UnkScreenBuffer* buffer, int palette);            // clear (-1: palette 15)
void unkfunc_0208062c(UnkScreenBuffer* buffer, int x, int y, int w, int h, int palette);   // clear a rect
void unkfunc_0208066c(UnkScreenBuffer* buffer, int x, int y, int palette);
void unkfunc_02080694(UnkScreenBuffer* buffer, int x, int y, int w, int h, int palette);
void unkfunc_020806dc(UnkScreenBuffer* buffer, int x, int y, int w, int h, int palette);   // window frame
void unkfunc_020808cc(UnkScreenBuffer* buffer, int x, int y, int w, int h, int palette);   // window frame
void unkfunc_02080a68(UnkScreenBuffer* buffer, int x, int y, int w, int palette, int type);    // horizontal line
void unkfunc_02080b7c(void* tile, int x, int y, int color);             // set a pixel
void unkfunc_02080ba0(void* tile, const void* src);                     // flip horizontally (NULL: in place)
void unkfunc_02080c20(void* dst, const void* src);                      // draw a tile (color 0 is transparent)
void unkfunc_02080c84(void* dst, const void* src);                      // draw a tile flipped vertically



