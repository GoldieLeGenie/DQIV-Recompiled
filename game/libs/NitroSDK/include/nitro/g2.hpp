#include <nitro/native_video.h>
#pragma once
#include "nitro/gx.h"

// OAM shape (attr0 shape | attr1 size) and color mode values for G2_SetOBJAttr
#define GX_OAM_SHAPE_8x8        0x00000000
#define GX_OAM_SHAPE_16x8       0x00004000
#define GX_OAM_SHAPE_8x16       0x00008000
#define GX_OAM_SHAPE_16x16      0x40000000
#define GX_OAM_SHAPE_32x16      0x80004000
#define GX_OAM_SHAPE_16x32      0x80008000
#define GX_OAM_SHAPE_32x32      0x80000000
#define GX_OAM_SHAPE_64x32      0xC0004000
#define GX_OAM_SHAPE_32x64      0xC0008000
#undef GX_OAM_SHAPE_64x64                     // gx.h defines the attr0 shape bits only
#define GX_OAM_SHAPE_64x64      0xC0000000
#define GX_OAM_COLORMODE_16     0x0000
#define GX_OAM_COLORMODE_256    0x2000
#define GX_OAM_MODE_NORMAL      0
#define GX_OAM_EFFECT_NONE      0
#define GX_OAM_EFFECT_FLIP_H    0x10000000
#define GX_OAM_EFFECT_FLIP_V    0x20000000
#define GX_OAM_EFFECT_FLIP_HV   0x30000000

inline void G2_SetOBJAttr(GXOamAttr* oam, int x, int y, int priority, int mode, int mosaic, int effect, int shape,
                          int color, int charName, int cParam, int rsParam)
{
    oam->attr01 = (u32)((y & 0xff) | (mode << 10) | (mosaic << 12) | effect | color | shape | ((x & 0x1ff) << 16) |
                        (rsParam << 25));
    oam->attr2 = (u16)(charName | (priority << 10) | (cParam << 12));
}

inline void G2_SetWnd0Position(int x1, int y1, int x2, int y2)
{
    REG_WIN0H = (u16)(((x1 << 8) & 0xff00) | (x2 & 0xff));
    REG_WIN0V = (u16)(((y1 << 8) & 0xff00) | (y2 & 0xff));
}

inline void G2_SetWnd1Position(int x1, int y1, int x2, int y2)
{
    REG_WIN1H = (u16)(((x1 << 8) & 0xff00) | (x2 & 0xff));
    REG_WIN1V = (u16)(((y1 << 8) & 0xff00) | (y2 & 0xff));
}

// BG control: text (size, color mode, screen base, character base, extended palette slot), bitmaps and affine
#define G2_BGCNT(reg) (*VIDEO_REG(vu16, reg))
#define G2_DEFINE_BG_CONTROL(name, reg)                                                                  \
    inline void name(int screenSize, int colorMode, int screenBase, int charBase, int extPltt)            \
    {                                                                                                     \
        G2_BGCNT(reg) = (u16)((G2_BGCNT(reg) & 0x43) | (screenSize << 14) | (colorMode << 7) |               \
                              (screenBase << 8) | (charBase << 2) | (extPltt << 13));                      \
    }
#define G2_DEFINE_BG_CONTROL_TEXT(name, reg)                                                             \
    inline void name(int screenSize, int colorMode, int screenBase, int charBase)                         \
    {                                                                                                     \
        G2_BGCNT(reg) = (u16)((G2_BGCNT(reg) & 0x43) | (screenSize << 14) | (colorMode << 7) |               \
                              (screenBase << 8) | (charBase << 2));                                       \
    }
#define G2_DEFINE_BG_CONTROL_DCBMP(name, reg)                                                            \
    inline void name(int screenSize, int areaOver, int screenBase)                                        \
    {                                                                                                     \
        G2_BGCNT(reg) = (u16)((G2_BGCNT(reg) & 0x43) | (screenSize << 14) | (areaOver << 13) |               \
                              (screenBase << 8) | 0x84);                                                  \
    }
#define G2_DEFINE_BG_CONTROL_256X16PLTT(name, reg)                                                       \
    inline void name(int screenSize, int areaOver, int screenBase, int charBase)                          \
    {                                                                                                     \
        G2_BGCNT(reg) = (u16)((G2_BGCNT(reg) & 0x43) | (screenSize << 14) | (areaOver << 13) |               \
                              (screenBase << 8) | (charBase << 2));                                       \
    }
#define G2_DEFINE_BG_PRIORITY(name, reg)                                                                 \
    inline void name(int priority)                                                                        \
    {                                                                                                     \
        G2_BGCNT(reg) = (u16)((G2_BGCNT(reg) & ~3) | priority);                                            \
    }

G2_DEFINE_BG_CONTROL(G2_SetBG0Control, 0x04000008)
G2_DEFINE_BG_CONTROL(G2_SetBG1Control, 0x0400000a)
G2_DEFINE_BG_CONTROL_TEXT(G2_SetBG2ControlText, 0x0400000c)
G2_DEFINE_BG_CONTROL_DCBMP(G2_SetBG2ControlDCBmp, 0x0400000c)
G2_DEFINE_BG_CONTROL_256X16PLTT(G2_SetBG3Control256x16Pltt, 0x0400000e)
G2_DEFINE_BG_CONTROL_TEXT(G2_SetBG3ControlText, 0x0400000e)
G2_DEFINE_BG_CONTROL(G2S_SetBG0Control, 0x04001008)
G2_DEFINE_BG_CONTROL(G2S_SetBG1Control, 0x0400100a)
G2_DEFINE_BG_CONTROL_TEXT(G2S_SetBG2ControlText, 0x0400100c)
G2_DEFINE_BG_CONTROL_DCBMP(G2S_SetBG3ControlDCBmp, 0x0400100e)
G2_DEFINE_BG_CONTROL_TEXT(G2S_SetBG3ControlText, 0x0400100e)
G2_DEFINE_BG_PRIORITY(G2_SetBG0Priority, 0x04000008)
G2_DEFINE_BG_PRIORITY(G2_SetBG1Priority, 0x0400000a)
G2_DEFINE_BG_PRIORITY(G2_SetBG2Priority, 0x0400000c)
G2_DEFINE_BG_PRIORITY(G2_SetBG3Priority, 0x0400000e)
G2_DEFINE_BG_PRIORITY(G2S_SetBG0Priority, 0x04001008)
G2_DEFINE_BG_PRIORITY(G2S_SetBG1Priority, 0x0400100a)
G2_DEFINE_BG_PRIORITY(G2S_SetBG2Priority, 0x0400100c)
G2_DEFINE_BG_PRIORITY(G2S_SetBG3Priority, 0x0400100e)

inline void G2_SetWndOBJInsidePlane(int wnd, int effect)
{
    u32 tmp = (*VIDEO_REG(vu16, 0x0400004a) & ~0x3f00) | (wnd << 8);
    if (effect) {
        tmp |= 0x2000;
    }
    *VIDEO_REG(vu16, 0x0400004a) = (u16)tmp;
}

inline void G2_SetWndOutsidePlane(int wnd, int effect)
{
    u32 tmp = (*VIDEO_REG(vu16, 0x0400004a) & ~0x3f) | wnd;
    if (effect) {
        tmp |= 0x20;
    }
    *VIDEO_REG(vu16, 0x0400004a) = (u16)tmp;
}
