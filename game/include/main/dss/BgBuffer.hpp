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

void initCharBuffer(UnkCharBuffer* buffer, void* buf, int width, int height);
void* getCharBufferTile(UnkCharBuffer* buffer, int x, int y);            // tile address
void fillCharBuffer(UnkCharBuffer* buffer, unsigned int pattern);     // fill
void fillCharBufferRect(UnkCharBuffer* buffer, int x, int y, int w, int h, unsigned int pattern);  // fill tiles
void setCharBufferPixel(UnkCharBuffer* buffer, int x, int y, int color);  // set a pixel
void drawCharBufferSystemTile(UnkCharBuffer* buffer, int x, int y, int flip, int bank, unsigned short chr);   // copy a system tile
void initScreenBuffer(UnkScreenBuffer* buffer, void* buf, int width, int height);
void initScreenBufferWithPitch(UnkScreenBuffer* buffer, void* buf, int width, int height, int pitch);
unsigned short* getScreenBufferCell(UnkScreenBuffer* buffer, int x, int y);    // cell address
void fillScreenBuffer(UnkScreenBuffer* buffer, unsigned short value);   // fill
void fillScreenBufferRect(UnkScreenBuffer* buffer, int x, int y, int w, int h, unsigned short value);   // fill a rect
void fillScreenBufferTiles(UnkScreenBuffer* buffer, int x, int y, int w, int h, int palette, unsigned short chr);
void fillScreenBufferTilesFlipped(UnkScreenBuffer* buffer, int x, int y, int w, int h, int palette, int vflip, int hflip, unsigned short chr);
void setScreenBufferCell(UnkScreenBuffer* buffer, int x, int y, unsigned short value);
void mapCharBufferToScreen(UnkScreenBuffer* buffer, int x, int y, int w, int h, int palette);   // map the character buffer to a rect
void drawScreenBufferText(UnkScreenBuffer* buffer, int x, int y, const char* text);     // ascii text (palette 15)
void clearScreenBuffer(UnkScreenBuffer* buffer, int palette);            // clear (-1: palette 15)
void clearScreenBufferRect(UnkScreenBuffer* buffer, int x, int y, int w, int h, int palette);   // clear a rect
void unkfunc_0208066c(UnkScreenBuffer* buffer, int x, int y, int palette);
void fillWindowBackRect(UnkScreenBuffer* buffer, int x, int y, int w, int h, int palette);
void drawWindowBack(UnkScreenBuffer* buffer, int x, int y, int w, int h, int palette);   // window frame
void drawWindowBorder(UnkScreenBuffer* buffer, int x, int y, int w, int h, int palette);   // window frame
void drawScreenBufferHLine(UnkScreenBuffer* buffer, int x, int y, int w, int palette, int type);    // horizontal line
void setTilePixel(void* tile, int x, int y, int color);             // set a pixel
void flipTileHorizontal(void* tile, const void* src);                     // flip horizontally (NULL: in place)
void drawTileTransparent(void* dst, const void* src);                      // draw a tile (color 0 is transparent)
void drawTileTransparentVFlip(void* dst, const void* src);                      // draw a tile flipped vertically



