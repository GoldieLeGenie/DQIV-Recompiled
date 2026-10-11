#include "main/dss/UnkBgBuffer.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/UnkSystemText.hpp"

ARM void unkfunc_02080110(UnkCharBuffer* buffer, void* buf, int width, int height)
{
    buffer->buf_ = buf;
    buffer->width_ = width;
    buffer->height_ = height;
    buffer->pitch_ = width;
}

ARM void* unkfunc_0208011c(UnkCharBuffer* buffer, int x, int y)
{
    return (unsigned char*)buffer->buf_ + (y * buffer->pitch_ + x) * 0x20;
}

ARM void unkfunc_02080130(UnkCharBuffer* buffer, unsigned int pattern)
{
    unkfunc_0208015c(buffer, 0, 0, buffer->width_, buffer->height_, pattern);
}

ARM void unkfunc_0208015c(UnkCharBuffer* buffer, int x, int y, int w, int h, unsigned int pattern)
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
        MI_CpuFill(pattern, unkfunc_0208011c(buffer, x, y + i), size);
    }
}

ARM void unkfunc_020801ec(UnkCharBuffer* buffer, int x, int y, int color)
{
    unkfunc_02080b7c(unkfunc_0208011c(buffer, x >> 3, y >> 3), x & 7, y & 7, color);
}

ARM void unkfunc_0208021c(UnkCharBuffer* buffer, int x, int y, int flip, int bank, unsigned short chr)
{
    unsigned char* src = (unsigned char*)unkfunc_02086e50(bank != 0) + chr * 0x20;
    void* dst = unkfunc_0208011c(buffer, x, y);
    if (flip) {
        unkfunc_02080c84(dst, src);
    } else {
        unkfunc_02080c20(dst, src);
    }
}

ARM void unkfunc_02080278(UnkScreenBuffer* buffer, void* buf, int width, int height)
{
    unkfunc_02080288(buffer, buf, width, height, width);
}

ARM void unkfunc_02080288(UnkScreenBuffer* buffer, void* buf, int width, int height, int pitch)
{
    buffer->buf_ = (unsigned short*)buf;
    buffer->base_ = (unsigned short*)buf;
    buffer->width_ = width;
    buffer->height_ = height;
    buffer->pitch_ = pitch;
}

ARM unsigned short* unkfunc_020802a0(UnkScreenBuffer* buffer, int x, int y)
{
    return buffer->buf_ + (y * buffer->pitch_ + x);
}

ARM void unkfunc_020802b4(UnkScreenBuffer* buffer, unsigned short value)
{
    if (buffer->width_ == buffer->pitch_) {
        MI_CpuFillU16(value, buffer->buf_, buffer->width_ * buffer->height_ * 2);
        return;
    }
    for (int y = 0; y < buffer->height_; y++) {
        MI_CpuFillU16(value, unkfunc_020802a0(buffer, 0, y), buffer->width_ * 2);
    }
}

ARM void unkfunc_02080334(UnkScreenBuffer* buffer, int x, int y, int w, int h, unsigned short value)
{
    unsigned short* p = unkfunc_020802a0(buffer, x, y);
    if (w == buffer->pitch_) {
        MI_CpuFillU16(value, p, w * h * 2);
        return;
    }
    for (int i = 0; i < h; i++) {
        MI_CpuFillU16(value, unkfunc_020802a0(buffer, x, y + i), w * 2);
    }
}

ARM void unkfunc_020803b8(UnkScreenBuffer* buffer, int x, int y, int w, int h, int palette, unsigned short chr)
{
    unkfunc_02080334(buffer, x, y, w, h, chr | (palette << 12));
}

ARM void unkfunc_020803ec(UnkScreenBuffer* buffer, int x, int y, int w, int h, int palette, int vflip, int hflip, unsigned short chr)
{
    unkfunc_02080334(buffer, x, y, w, h, chr | (hflip << 10) | (vflip << 11) | (palette << 12));
}

ARM void unkfunc_02080430(UnkScreenBuffer* buffer, int x, int y, unsigned short value)
{
    *unkfunc_020802a0(buffer, x, y) = value;
}

ARM void unkfunc_02080444(UnkScreenBuffer* buffer, int x, int y, int w, int h, int palette)
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
        unsigned short* p = unkfunc_020802a0(buffer, x, y + j);
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

ARM void unkfunc_020805bc(UnkScreenBuffer* buffer, int x, int y, const char* text)
{
    if (buffer->buf_ == NULL) {
        return;
    }
    unsigned short* p = unkfunc_020802a0(buffer, x, y);
    while (*text != '\0') {
        if (x >= 0x20) {
            return;
        }
        x++;
        *p++ = *text++ | 0xf000;
    }
}

ARM void unkfunc_0208060c(UnkScreenBuffer* buffer, int palette)
{
    if (palette == -1) {
        palette = 15;
    }
    unkfunc_020802b4(buffer, palette << 12);
}

ARM void unkfunc_0208062c(UnkScreenBuffer* buffer, int x, int y, int w, int h, int palette)
{
    if (palette == -1) {
        palette = 15;
    }
    unkfunc_02080334(buffer, x, y, w, h, palette << 12);
}

ARM void unkfunc_0208066c(UnkScreenBuffer* buffer, int x, int y, int palette)
{
    if (palette == -1) {
        palette = 15;
    }
    unkfunc_02080430(buffer, x, y, (palette << 12) | 0x7f);
}

ARM void unkfunc_02080694(UnkScreenBuffer* buffer, int x, int y, int w, int h, int palette)
{
    if (palette == -1) {
        palette = 15;
    }
    unkfunc_02080334(buffer, x, y, w, h, (palette << 12) | 7);
}

ARM void unkfunc_020806dc(UnkScreenBuffer* buffer, int x, int y, int w, int h, int palette)
{
    if (palette == -1) {
        palette = 15;
    }
    int pal = palette << 12;
    unkfunc_02080334(buffer, x + 1, y + 1, w - 1, h - 1, pal | 7);
    unkfunc_02080430(buffer, x, y, pal | 4);
    unkfunc_02080430(buffer, x, y + (h - 1), pal | 4 | 0x800);
    unkfunc_02080430(buffer, x + (w - 1), y, pal | 4 | 0x400);
    unkfunc_02080430(buffer, x + (w - 1), y + (h - 1), pal | 4 | 0xc00);
    for (int i = 1; i < w - 1; i++) {
        unkfunc_02080430(buffer, x + i, y, pal | 5);
        unkfunc_02080430(buffer, x + i, y + (h - 1), pal | 5 | 0x800);
    }
    for (int i = 1; i < h - 1; i++) {
        unkfunc_02080430(buffer, x, y + i, pal | 6);
        unkfunc_02080430(buffer, x + (w - 1), y + i, pal | 6 | 0x400);
    }
}

ARM void unkfunc_020808cc(UnkScreenBuffer* buffer, int x, int y, int w, int h, int palette)
{
    if (palette == -1) {
        palette = 15;
    }
    int pal = palette << 12;
    unkfunc_02080430(buffer, x, y, pal | 0x10);
    unkfunc_02080430(buffer, x + (w - 1), y, pal | 0xf);
    unkfunc_02080430(buffer, x, y + (h - 1), pal | 0xe);
    unkfunc_02080430(buffer, x + (w - 1), y + (h - 1), pal | 0xd);
    for (int i = 1; i < w - 1; i++) {
        unkfunc_02080430(buffer, x + i, y, pal | 0x11);
        unkfunc_02080430(buffer, x + i, y + (h - 1), pal | 0x18);
    }
    for (int i = 1; i < h - 1; i++) {
        unkfunc_02080430(buffer, x, y + i, pal | 0x12);
        unkfunc_02080430(buffer, x + (w - 1), y + i, pal | 0x19);
    }
}

ARM void unkfunc_02080a68(UnkScreenBuffer* buffer, int x, int y, int w, int palette, int type)
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
    unkfunc_02080430(buffer, x, y, left | (palette << 12));
    unkfunc_02080430(buffer, x + (w - 1), y, right | (palette << 12));
    for (int i = 1; i < w - 1; i++) {
        unkfunc_02080430(buffer, x + i, y, mid | (palette << 12));
    }
}

ARM void unkfunc_02080b7c(void* tile, int x, int y, int color)
{
    unsigned int* p = (unsigned int*)tile;
    p[y] = (p[y] & ~(0xf << (x * 4))) | (color << (x * 4));
}

ARM void unkfunc_02080ba0(void* tile, const void* src)
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

ARM void unkfunc_02080c20(void* dst, const void* src)
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

ARM void unkfunc_02080c84(void* dst, const void* src)
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
