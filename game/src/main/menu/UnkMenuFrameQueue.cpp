#include "main/menu/UnkMenuFrameDisplay.hpp"
#include "nitro/g2.hpp"

THUMB void UnkMenuFrameDisplay::unkfunc_0204ee30(int x, int y, int w, int h, int priority, int palette, int corner, int xlu)
{
    if (palette == -1) {
        palette = 15;
    }
    int width = w;
    int height = h;
    if (corner) {
        width -= 8;
        height -= 8;
    }
    if (width >= 64 && height >= 64) {
        unkfunc_0204f030(x, y, w, h, priority, palette, 64, 64, GX_OAM_SHAPE_64x64, corner, xlu);
        return;
    }
    if (width >= 64 && height >= 32) {
        unkfunc_0204f030(x, y, w, h, priority, palette, 64, 32, GX_OAM_SHAPE_64x32, corner, xlu);
        return;
    }
    if (width >= 32 && height >= 64) {
        unkfunc_0204f030(x, y, w, h, priority, palette, 32, 64, GX_OAM_SHAPE_32x64, corner, xlu);
        return;
    }
    if (width >= 32 && height >= 32) {
        unkfunc_0204f030(x, y, w, h, priority, palette, 32, 32, GX_OAM_SHAPE_32x32, corner, xlu);
        return;
    }
    if (width >= 32 && height >= 16) {
        unkfunc_0204f030(x, y, w, h, priority, palette, 32, 16, GX_OAM_SHAPE_32x16, corner, xlu);
        return;
    }
    if (width >= 16 && height >= 32) {
        unkfunc_0204f030(x, y, w, h, priority, palette, 16, 32, GX_OAM_SHAPE_16x32, corner, xlu);
        return;
    }
    if (width >= 16 && height >= 16) {
        unkfunc_0204f030(x, y, w, h, priority, palette, 16, 16, GX_OAM_SHAPE_16x16, corner, xlu);
        return;
    }
    if (width >= 16 && height >= 8) {
        unkfunc_0204f030(x, y, w, h, priority, palette, 16, 8, GX_OAM_SHAPE_16x8, corner, xlu);
        return;
    }
    if (width >= 8 && height >= 16) {
        unkfunc_0204f030(x, y, w, h, priority, palette, 8, 16, GX_OAM_SHAPE_8x16, corner, xlu);
        return;
    }
    unkfunc_0204f030(x, y, w, h, priority, palette, 8, 8, GX_OAM_SHAPE_8x8, corner, xlu);
}

THUMB void UnkMenuFrameDisplay::unkfunc_0204f030(int x, int y, int w, int h, int priority, int palette, int cellW,
                                                 int cellH, int shape, int corner, int xlu)
{
    for (int j = 0; j < h; j += cellH) {
        for (int i = 0; i < w; i += cellW) {
            int offsetX = cellW - (w - i);
            int offsetY = cellH - (h - j);
            if (offsetX <= 0) {
                offsetX = 0;
            }
            if (offsetY <= 0) {
                offsetY = 0;
            }
            int effect = GX_OAM_EFFECT_NONE;
            int chr = 0x101;
            if (corner) {
                if (i == 0 && j == 0) {
                    chr = 0x100;
                }
                if (i == 0 && j + cellH >= h) {
                    chr = 0x100;
                    effect = GX_OAM_EFFECT_FLIP_V;
                }
                if (i + cellW >= w && j == 0) {
                    chr = 0x100;
                    effect = GX_OAM_EFFECT_FLIP_H;
                }
                if (i + cellW >= w && j + cellH >= h) {
                    chr = 0x100;
                    effect = GX_OAM_EFFECT_FLIP_HV;
                }
            }
            requests_[requestCount_].chr_ = chr;
            requests_[requestCount_].shape_ = shape;
            requests_[requestCount_].x_ = x + i - offsetX;
            requests_[requestCount_].y_ = y + j - offsetY;
            requests_[requestCount_].priority_ = priority;
            requests_[requestCount_].palette_ = palette;
            requests_[requestCount_].effect_ = effect;
            if (xlu == 1) {
                requests_[requestCount_].mode_ = 1;
            } else {
                requests_[requestCount_].mode_ = 0;
            }
            requestCount_++;
        }
    }
}
