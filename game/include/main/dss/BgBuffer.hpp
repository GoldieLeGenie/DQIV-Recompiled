#pragma once
#include <globaldefs.h>

struct PaletteBuffer;

// BG character buffer of the dss engine (tiles of 0x20 bytes)
struct BgCharBuffer {
    void* buf_;                                 // 0x00
    int width_;                                 // 0x04
    int height_;                                // 0x08
    int pitch_;                                 // 0x0C
};

// BG screen buffer of the dss engine (one entry per cell)
struct BgScreenBuffer {
    unsigned short* buf_;                       // 0x00
    unsigned short* base_;                      // 0x04
    int width_;                                 // 0x08
    int height_;                                // 0x0C
    int pitch_;                                 // 0x10
};

void initCharBuffer(BgCharBuffer* buffer, void* buf, int width, int height);
void* getCharBufferTile(BgCharBuffer* buffer, int x, int y);            // tile address
void fillCharBuffer(BgCharBuffer* buffer, unsigned int pattern);     // fill
void fillCharBufferRect(BgCharBuffer* buffer, int x, int y, int w, int h, unsigned int pattern);  // fill tiles
void setCharBufferPixel(BgCharBuffer* buffer, int x, int y, int color);  // set a pixel
void drawCharBufferSystemTile(BgCharBuffer* buffer, int x, int y, int flip, int bank, unsigned short chr);   // copy a system tile
void initScreenBuffer(BgScreenBuffer* buffer, void* buf, int width, int height);
void initScreenBufferWithPitch(BgScreenBuffer* buffer, void* buf, int width, int height, int pitch);
unsigned short* getScreenBufferCell(BgScreenBuffer* buffer, int x, int y);    // cell address
void fillScreenBuffer(BgScreenBuffer* buffer, unsigned short value);   // fill
void fillScreenBufferRect(BgScreenBuffer* buffer, int x, int y, int w, int h, unsigned short value);   // fill a rect
void fillScreenBufferTiles(BgScreenBuffer* buffer, int x, int y, int w, int h, int palette, unsigned short chr);
void fillScreenBufferTilesFlipped(BgScreenBuffer* buffer, int x, int y, int w, int h, int palette, int vflip, int hflip, unsigned short chr);
void setScreenBufferCell(BgScreenBuffer* buffer, int x, int y, unsigned short value);
void mapCharBufferToScreen(BgScreenBuffer* buffer, int x, int y, int w, int h, int palette);   // map the character buffer to a rect
void drawScreenBufferText(BgScreenBuffer* buffer, int x, int y, const char* text);     // ascii text (palette 15)
void clearScreenBuffer(BgScreenBuffer* buffer, int palette);            // clear (-1: palette 15)
void clearScreenBufferRect(BgScreenBuffer* buffer, int x, int y, int w, int h, int palette);   // clear a rect
void unkfunc_0208066c(BgScreenBuffer* buffer, int x, int y, int palette);
void fillWindowBackRect(BgScreenBuffer* buffer, int x, int y, int w, int h, int palette);
void drawWindowBack(BgScreenBuffer* buffer, int x, int y, int w, int h, int palette);   // window frame
void drawWindowBorder(BgScreenBuffer* buffer, int x, int y, int w, int h, int palette);   // window frame
void drawScreenBufferHLine(BgScreenBuffer* buffer, int x, int y, int w, int palette, int type);    // horizontal line
void setTilePixel(void* tile, int x, int y, int color);             // set a pixel
void flipTileHorizontal(void* tile, const void* src);                     // flip horizontally (NULL: in place)
void drawTileTransparent(void* dst, const void* src);                      // draw a tile (color 0 is transparent)
void drawTileTransparentVFlip(void* dst, const void* src);                      // draw a tile flipped vertically



