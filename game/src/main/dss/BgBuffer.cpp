#include "main/dss/BgBuffer.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/SystemText.hpp"

ARM void initCharBuffer(BgCharBuffer* buffer, void* buf, int width, int height)
{
    buffer->buf_ = buf;
    buffer->width_ = width;
    buffer->height_ = height;
    buffer->pitch_ = width;
}

ARM void* getCharBufferTile(BgCharBuffer* buffer, int x, int y)
{
    return (unsigned char*)buffer->buf_ + (y * buffer->pitch_ + x) * 0x20;
}

ARM void fillCharBuffer(BgCharBuffer* buffer, unsigned int pattern)
{
    fillCharBufferRect(buffer, 0, 0, buffer->width_, buffer->height_, pattern);
}

ARM void fillCharBufferRect(BgCharBuffer* buffer, int x, int y, int w, int h, unsigned int pattern)
{
    if (x > buffer->width_ || y > buffer->height_) {
        return;
    }
    if (x + w > buffer->width_) {
        w = buffer->width_ - x;
    }
    if (y + h > buffer->height_) {
        h = buffer->height_ - y;
    }
    if (w == 0 || h == 0) {
        return;
    }
    unsigned int size = w * 0x20;
    for (int i = 0; i < h; i++) {
        MI_CpuFill(pattern, getCharBufferTile(buffer, x, y + i), size);
    }
}

ARM void setCharBufferPixel(BgCharBuffer* buffer, int x, int y, int color)
{
    setTilePixel(getCharBufferTile(buffer, x >> 3, y >> 3), x & 7, y & 7, color);
}

ARM void drawCharBufferSystemTile(BgCharBuffer* buffer, int x, int y, int flip, int bank, unsigned short chr)
{
    unsigned char* src = (unsigned char*)getSystemTextData(bank != 0) + chr * 0x20;
    void* dst = getCharBufferTile(buffer, x, y);
    if (flip) {
        drawTileTransparentVFlip(dst, src);
    } else {
        drawTileTransparent(dst, src);
    }
}

ARM void initScreenBuffer(BgScreenBuffer* buffer, void* buf, int width, int height)
{
    initScreenBufferWithPitch(buffer, buf, width, height, width);
}

ARM void initScreenBufferWithPitch(BgScreenBuffer* buffer, void* buf, int width, int height, int pitch)
{
    buffer->buf_ = (unsigned short*)buf;
    buffer->base_ = (unsigned short*)buf;
    buffer->width_ = width;
    buffer->height_ = height;
    buffer->pitch_ = pitch;
}

ARM unsigned short* getScreenBufferCell(BgScreenBuffer* buffer, int x, int y)
{
    return buffer->buf_ + (y * buffer->pitch_ + x);
}

ARM void fillScreenBuffer(BgScreenBuffer* buffer, unsigned short value)
{
    if (buffer->width_ == buffer->pitch_) {
        MI_CpuFillU16(value, buffer->buf_, buffer->width_ * buffer->height_ * 2);
        return;
    }
    for (int y = 0; y < buffer->height_; y++) {
        MI_CpuFillU16(value, getScreenBufferCell(buffer, 0, y), buffer->width_ * 2);
    }
}

ARM void fillScreenBufferRect(BgScreenBuffer* buffer, int x, int y, int w, int h, unsigned short value)
{
    unsigned short* p = getScreenBufferCell(buffer, x, y);
    if (w == buffer->pitch_) {
        MI_CpuFillU16(value, p, w * h * 2);
        return;
    }
    for (int i = 0; i < h; i++) {
        MI_CpuFillU16(value, getScreenBufferCell(buffer, x, y + i), w * 2);
    }
}

ARM void fillScreenBufferTiles(BgScreenBuffer* buffer, int x, int y, int w, int h, int palette, unsigned short chr)
{
    fillScreenBufferRect(buffer, x, y, w, h, chr | (palette << 12));
}

ARM void fillScreenBufferTilesFlipped(BgScreenBuffer* buffer, int x, int y, int w, int h, int palette, int vflip, int hflip, unsigned short chr)
{
    fillScreenBufferRect(buffer, x, y, w, h, chr | (hflip << 10) | (vflip << 11) | (palette << 12));
}

ARM void setScreenBufferCell(BgScreenBuffer* buffer, int x, int y, unsigned short value)
{
    *getScreenBufferCell(buffer, x, y) = value;
}

ARM void mapCharBufferToScreen(BgScreenBuffer* buffer, int x, int y, int w, int h, int palette)
{
    if (x > buffer->width_ || y > buffer->height_) {
        return;
    }
    if (x + w > buffer->width_) {
        w = buffer->width_ - x;
    }
    if (y + h > buffer->height_) {
        h = buffer->height_ - y;
    }
    if (w == 0 || h == 0) {
        return;
    }
    if (w > buffer->width_) {
        w = buffer->width_;
    }
    if (h > buffer->height_) {
        h = buffer->height_;
    }
    unsigned int offset = ((unsigned int)buffer->buf_ - (unsigned int)buffer->base_) / 2;
    unsigned int left = offset % buffer->pitch_;
    unsigned int top = offset / buffer->pitch_;
    for (int j = 0; j < h; j++) {
        unsigned short* p = getScreenBufferCell(buffer, x, y + j);
        unsigned short chr;
        int step;
        unsigned short pal;
        if (palette == -1) {
            chr = 0;
            step = 0;
            pal = 15;
        } else {
            chr = x + (left + 0x80) + ((top + y + j) % 24) * 0x20;
            pal = palette;
            step = 1;
        }
        for (int i = 0; i < w; i++) {
            *p++ = chr | (pal << 12);
            chr += step;
        }
    }
}

ARM void drawScreenBufferText(BgScreenBuffer* buffer, int x, int y, const char* text)
{
    if (buffer->buf_ == NULL) {
        return;
    }
    unsigned short* p = getScreenBufferCell(buffer, x, y);
    while (*text != '\0') {
        if (x >= 0x20) {
            return;
        }
        x++;
        *p++ = *text++ | 0xf000;
    }
}

ARM void clearScreenBuffer(BgScreenBuffer* buffer, int palette)
{
    if (palette == -1) {
        palette = 15;
    }
    fillScreenBuffer(buffer, palette << 12);
}

ARM void clearScreenBufferRect(BgScreenBuffer* buffer, int x, int y, int w, int h, int palette)
{
    if (palette == -1) {
        palette = 15;
    }
    fillScreenBufferRect(buffer, x, y, w, h, palette << 12);
}

ARM void unkfunc_0208066c(BgScreenBuffer* buffer, int x, int y, int palette)
{
    if (palette == -1) {
        palette = 15;
    }
    setScreenBufferCell(buffer, x, y, (palette << 12) | 0x7f);
}

ARM void fillWindowBackRect(BgScreenBuffer* buffer, int x, int y, int w, int h, int palette)
{
    if (palette == -1) {
        palette = 15;
    }
    fillScreenBufferRect(buffer, x, y, w, h, (palette << 12) | 7);
}

ARM void drawWindowBack(BgScreenBuffer* buffer, int x, int y, int w, int h, int palette)
{
    if (palette == -1) {
        palette = 15;
    }
    int pal = palette << 12;
    fillScreenBufferRect(buffer, x + 1, y + 1, w - 1, h - 1, pal | 7);
    setScreenBufferCell(buffer, x, y, pal | 4);
    setScreenBufferCell(buffer, x, y + (h - 1), pal | 4 | 0x800);
    setScreenBufferCell(buffer, x + (w - 1), y, pal | 4 | 0x400);
    setScreenBufferCell(buffer, x + (w - 1), y + (h - 1), pal | 4 | 0xc00);
    for (int i = 1; i < w - 1; i++) {
        setScreenBufferCell(buffer, x + i, y, pal | 5);
        setScreenBufferCell(buffer, x + i, y + (h - 1), pal | 5 | 0x800);
    }
    for (int i = 1; i < h - 1; i++) {
        setScreenBufferCell(buffer, x, y + i, pal | 6);
        setScreenBufferCell(buffer, x + (w - 1), y + i, pal | 6 | 0x400);
    }
}

ARM void drawWindowBorder(BgScreenBuffer* buffer, int x, int y, int w, int h, int palette)
{
    if (palette == -1) {
        palette = 15;
    }
    int pal = palette << 12;
    setScreenBufferCell(buffer, x, y, pal | 0x10);
    setScreenBufferCell(buffer, x + (w - 1), y, pal | 0xf);
    setScreenBufferCell(buffer, x, y + (h - 1), pal | 0xe);
    setScreenBufferCell(buffer, x + (w - 1), y + (h - 1), pal | 0xd);
    for (int i = 1; i < w - 1; i++) {
        setScreenBufferCell(buffer, x + i, y, pal | 0x11);
        setScreenBufferCell(buffer, x + i, y + (h - 1), pal | 0x18);
    }
    for (int i = 1; i < h - 1; i++) {
        setScreenBufferCell(buffer, x, y + i, pal | 0x12);
        setScreenBufferCell(buffer, x + (w - 1), y + i, pal | 0x19);
    }
}

ARM void drawScreenBufferHLine(BgScreenBuffer* buffer, int x, int y, int w, int palette, int type)
{
    int left;
    int mid;
    int right;
    if (palette == -1) {
        palette = 15;
    }
    switch (type) {
    case 0:
        left = 0x13;
        mid = 0x14;
        right = 0x17;
        break;
    case 1:
        left = 8;
        mid = 9;
        right = 10;
        break;
    case 2:
        left = 0x15;
        mid = 0x11;
        right = 0x16;
        break;
    case 3:
        left = 0x1a;
        mid = 0x18;
        right = 0x1b;
        break;
    default:
        right = mid = left = 0xc;
        break;
    }
    setScreenBufferCell(buffer, x, y, left | (palette << 12));
    setScreenBufferCell(buffer, x + (w - 1), y, right | (palette << 12));
    for (int i = 1; i < w - 1; i++) {
        setScreenBufferCell(buffer, x + i, y, mid | (palette << 12));
    }
}

ARM void setTilePixel(void* tile, int x, int y, int color)
{
    unsigned int* p = (unsigned int*)tile;
    p[y] = (p[y] & ~(0xf << (x * 4))) | (color << (x * 4));
}

ARM void flipTileHorizontal(void* tile, const void* src)
{
    if (src == NULL) {
        src = tile;
    }
    const unsigned int* s = (const unsigned int*)src;
    unsigned int* d = (unsigned int*)tile;
    for (int i = 0; i < 8; i++) {
        unsigned int v = *s++;
        unsigned int r = 0;
        r = (r << 4) | (v & 0xf);
        v >>= 4;
        r = (r << 4) | (v & 0xf);
        v >>= 4;
        r = (r << 4) | (v & 0xf);
        v >>= 4;
        r = (r << 4) | (v & 0xf);
        v >>= 4;
        r = (r << 4) | (v & 0xf);
        v >>= 4;
        r = (r << 4) | (v & 0xf);
        v >>= 4;
        r = (r << 4) | (v & 0xf);
        v >>= 4;
        r = (r << 4) | (v & 0xf);
        v >>= 4;
        *d++ = r;
    }
}

ARM void drawTileTransparent(void* dst, const void* src)
{
    unsigned int* d = (unsigned int*)dst;
    const unsigned int* s = (const unsigned int*)src;
    for (int i = 0; i < 8; i++) {
        unsigned int v = *s++;
        if (v != 0) {
            unsigned int mask = (v | (v >> 1) | (v >> 2) | (v >> 3)) & 0x11111111;
            mask = mask | (mask << 1) | (mask << 2) | (mask << 3);
            *d = (*d & ~mask) | (v & mask);
        }
        d++;
    }
}

ARM void drawTileTransparentVFlip(void* dst, const void* src)
{
    unsigned int* d = (unsigned int*)dst + 7;
    const unsigned int* s = (const unsigned int*)src;
    for (int i = 0; i < 8; i++) {
        unsigned int v = *s++;
        if (v != 0) {
            unsigned int mask = (v | (v >> 1) | (v >> 2) | (v >> 3)) & 0x11111111;
            mask = mask | (mask << 1) | (mask << 2) | (mask << 3);
            *d = (*d & ~mask) | (v & mask);
        }
        d--;
    }
}
