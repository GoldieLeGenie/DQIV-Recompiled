#include <nitro/native_g3.h>
// The SDK reserves both sprite state records.
#pragma define_section sdk_state ".data" ".bss.keep" ".rodata" RW
#include <nitro/types.h>
#include <nitro/reg.h>
#include <nitro/fx/fx_trig.h>

/* Software sprites drawn as textured quads through the 3D geometry engine. */

#define REG_GFX_FIFO_VERTEX_10 (*(vu32*)0x04000490)

typedef struct UnkG2dImageAttr {
    int unk_00;                 // 0x00 texture width (size code)
    int unk_04;                 // 0x04 texture height (size code)
    int unk_08;                 // 0x08 texture format
    int unk_0c;                 // 0x0C
    int unk_10;                 // 0x10 palette color 0 transparent
    int unk_14;                 // 0x14
} UnkG2dImageAttr;

/* Same layout as the game's UnkG2dSprite (simple 0x0C / basic 0x1C / extended 0x3C parts). */
typedef struct UnkG2dSprite {
    s16 unk_00;                 // 0x00 x
    s16 unk_02;                 // 0x02 y
    s16 unk_04;                 // 0x04 width
    s16 unk_06;                 // 0x06 height
    u16 unk_08;                 // 0x08 rotation Z
    u8 unk_0a;                  // 0x0A priority (z)
    u8 unk_0b;                  // 0x0B alpha
    UnkG2dImageAttr* unk_0c;    // 0x0C image attributes
    u32 unk_10;                 // 0x10 texture address
    u32 unk_14;                 // 0x14 palette address
    u16 unk_18;                 // 0x18 palette number
    u16 unk_1a;                 // 0x1A color
    s32 unk_1c;                 // 0x1C upper-left u
    s32 unk_20;                 // 0x20 upper-left v
    s32 unk_24;                 // 0x24 lower-right u
    s32 unk_28;                 // 0x28 lower-right v
    BOOL unk_2c;                // 0x2C flip u
    BOOL unk_30;                // 0x30 flip v
    s16 unk_34;                 // 0x34 rotation origin x
    s16 unk_36;                 // 0x36 rotation origin y
    u16 unk_38;                 // 0x38 rotation X
    u16 unk_3a;                 // 0x3A rotation Y
} UnkG2dSprite;

typedef struct UnkG2dSpriteState {
    u16 unk_00;                 // 0x00 polygon id
    u32 unk_04;                 // 0x04 enabled attribute flags
} UnkG2dSpriteState;

NITRO_SDK_STATE UnkG2dSpriteState data_0210cee4;
NITRO_SDK_STATE UnkG2dSprite      data_0210ceec; // not referenced by the code in the ROM

extern void func_02065c24(s16 s, s16 c);
extern void func_02065c60(s16 s, s16 c);
extern void func_02065c9c(s16 s, s16 c);

#define TEXCOORD(s, t) ((u32)(u16)(s16)((s) >> 8) | ((u32)(u16)(s16)((t) >> 8) << 16))

u32 func_02068ed8(u32 flag);

static inline void G3_Translate(s32 x, s32 y, s32 z) {
    G3_WRITE(0x1c, x);
    G3_WRITE(0x1c, y);
    G3_WRITE(0x1c, z);
}

static inline void G3_TexImageParam(int fmt, int gen, int s, int t, int repeat, int flip, int pltt0, u32 addr) {
    G3_WRITE(0x2a, (addr >> 3) | (fmt << 26) | (gen << 30) | (s << 20) | (t << 23) | (repeat << 16) | (flip << 18) | (pltt0 << 29));
}

static inline void G3_TexPlttBase(u32 addr, int fmt) {
    G3_WRITE(0x2b, addr >> (4 - (fmt == 2)));
}

static inline void G3_Scale(s32 x, s32 y, s32 z) {
    G3_WRITE(0x1b, x);
    G3_WRITE(0x1b, y);
    G3_WRITE(0x1b, z);
}

void func_02068aa4(UnkG2dSprite* simple, UnkG2dSprite* basic, UnkG2dSprite* ext) {
    s32 u0 = 0;
    s32 u1 = simple->unk_04 << 12;
    s32 v0 = 0;
    s32 v1 = simple->unk_06 << 12;

    if (func_02068ed8(8)) {
        u0 = ext->unk_1c;
        u1 = ext->unk_24;
        v0 = ext->unk_20;
        v1 = ext->unk_28;
    }
    if (func_02068ed8(0x10)) {
        s32 tmp;

        if (ext->unk_2c) {
            tmp = u0;
            u0 = u1;
            u1 = tmp;
        }
        if (ext->unk_30) {
            tmp = v0;
            v0 = v1;
            v1 = tmp;
        }
    }

    G3_Translate((simple->unk_00 + (simple->unk_04 >> 1)) << 12, (simple->unk_02 + (simple->unk_06 >> 1)) << 12, simple->unk_0a << 12);

    if (func_02068ed8(0x20)) {
        G3_Translate(ext->unk_34 << 12, ext->unk_36 << 12, 0);
    }
    if (func_02068ed8(0x40)) {
        func_02065c24(FX_SinIdx(ext->unk_38), FX_CosIdx(ext->unk_38));
        func_02065c60(FX_SinIdx(ext->unk_3a), FX_CosIdx(ext->unk_3a));
    }
    func_02065c9c(FX_SinIdx(simple->unk_08), FX_CosIdx(simple->unk_08));
    if (func_02068ed8(0x20)) {
        G3_Translate(-ext->unk_34 << 12, -ext->unk_36 << 12, 0);
    }

    G3_Scale(simple->unk_04 << 12, simple->unk_06 << 12, 0x1000);

    if (func_02068ed8(2) && basic->unk_0c != NULL) {
        UnkG2dImageAttr* attr = basic->unk_0c;

        G3_TexImageParam(attr->unk_08, 1, attr->unk_00, attr->unk_04, 0, 0, attr->unk_10, basic->unk_10);
        /* palette formats 2..4 (unsigned range check); the second read of the format goes through an int pointer,
           so the IR optimizer keeps it apart and the back end reuses the range-check load */
        if ((u32)(attr->unk_08 - 2) <= 2) {
            G3_TexPlttBase(basic->unk_14 + basic->unk_18 * 32, *(int*)&attr->unk_08);
        }
    }

    if (func_02068ed8(1)) {
        if (simple->unk_0b == 0) {
            return;
        }
        G3_WRITE(0x29, (data_0210cee4.unk_00 << 24) | 0xc0 | (simple->unk_0b << 16));
    }
    if (func_02068ed8(4)) {
        G3_WRITE(0x20, basic->unk_1a);
    }

    G3_WRITE(0x40, 1);
    G3_WRITE(0x22, TEXCOORD(u0, v1));
    G3_WRITE(0x24, 0x83e0);
    G3_WRITE(0x22, TEXCOORD(u1, v1));
    G3_WRITE(0x24, 0x8020);
    G3_WRITE(0x22, TEXCOORD(u1, v0));
    G3_WRITE(0x24, 0xf8020);
    G3_WRITE(0x22, TEXCOORD(u0, v0));
    G3_WRITE(0x24, 0xf83e0);
    G3_WRITE(0x41, 0);
}

/* Draws a unit quad with the given texture coordinates. */
void func_02068e14(s32 u0, s32 u1, s32 v0, s32 v1) {
    G3_WRITE(0x40, 1);
    G3_WRITE(0x22, TEXCOORD(u0, v1));
    G3_WRITE(0x23, 0x10000000);
    G3_WRITE(0x23, 0);
    G3_WRITE(0x22, TEXCOORD(u1, v1));
    G3_WRITE(0x23, 0x10001000);
    G3_WRITE(0x23, 0);
    G3_WRITE(0x22, TEXCOORD(u1, v0));
    G3_WRITE(0x23, 0x1000);
    G3_WRITE(0x23, 0);
    G3_WRITE(0x22, TEXCOORD(u0, v0));
    G3_WRITE(0x23, 0);
    G3_WRITE(0x23, 0);
    G3_WRITE(0x41, 0);
}

/* Sets the enabled sprite attribute flags (declared in the game as unkfunc_02068ec8(int)). */
void _Z16unkfunc_02068ec8i(u32 flags) {
    data_0210cee4.unk_04 = flags;
}

u32 func_02068ed8(u32 flag) {
    return data_0210cee4.unk_04 & flag;
}

/* Draws an extended sprite (declared in the game as unkfunc_02068eec(UnkG2dSprite*)). */
void _Z16unkfunc_02068eecP12UnkG2dSprite(UnkG2dSprite* sprite) {
    func_02068aa4(sprite, sprite, sprite);
}

/* Draws a textured rectangle at (x, y, z) of size (w, h) with texture coordinates (u0, v0)-(u1, v1). */
void func_02068f00(int x, int y, int z, int w, int h, int u0, int v0, int u1, int v1) {
    G3_WRITE(0x1c, x << 12);
    G3_WRITE(0x1c, y << 12);
    G3_WRITE(0x1c, z << 12);
    G3_WRITE(0x1b, w << 12);
    G3_WRITE(0x1b, h << 12);
    G3_WRITE(0x1b, 0x1000);
    func_02068e14(u0 << 12, u1 << 12, v0 << 12, v1 << 12);
}
