#include "g3d_types.h"

/* Texture / palette resources: VRAM loading and binding to model materials. */

typedef struct UnkResDict {
    u8 unk_00;                  // 0x00 revision
    u8 unk_01;                  // 0x01 number of entries
    u16 unk_02;                 // 0x02 dictionary size
    u16 unk_04;                 // 0x04
    u16 unk_06;                 // 0x06 offset of the entry table
} UnkResDict;

typedef struct UnkResDictEntryHeader {
    u16 unk_00;                 // 0x00 size of one data unit
    u16 unk_02;                 // 0x02 offset of the name table
    u8 unk_04[1];               // 0x04 data units
} UnkResDictEntryHeader;

typedef struct UnkResTexInfo {
    u32 unk_00;                 // 0x00 VRAM key
    u16 unk_04;                 // 0x04 data size / 8
    u16 unk_06;                 // 0x06 dictionary offset
    u16 unk_08;                 // 0x08 flags (bit 0: loaded)
    u16 unk_0a;                 // 0x0A
    u32 unk_0c;                 // 0x0C data offset
} UnkResTexInfo;

typedef struct UnkResTex4x4Info {
    u32 unk_00;                 // 0x00 VRAM key
    u16 unk_04;                 // 0x04 data size / 8
    u16 unk_06;                 // 0x06 dictionary offset
    u16 unk_08;                 // 0x08 flags (bit 0: loaded)
    u16 unk_0a;                 // 0x0A
    u32 unk_0c;                 // 0x0C data offset
    u32 unk_10;                 // 0x10 palette index data offset
} UnkResTex4x4Info;

typedef struct UnkResPlttInfo {
    u32 unk_00;                 // 0x00 VRAM key
    u16 unk_04;                 // 0x04 data size / 8
    u16 unk_06;                 // 0x06 flags (bit 0: loaded)
    u16 unk_08;                 // 0x08 dictionary offset
    u16 unk_0a;                 // 0x0A
    u32 unk_0c;                 // 0x0C data offset
} UnkResPlttInfo;

typedef struct UnkResTex {
    u32 unk_00;                 // 0x00 block kind
    u32 unk_04;                 // 0x04 block size
    UnkResTexInfo unk_08;       // 0x08 textures
    UnkResTex4x4Info unk_18;    // 0x18 4x4 compressed textures
    UnkResPlttInfo unk_2c;      // 0x2C palettes
    UnkResDict unk_3c;          // 0x3C texture dictionary
} UnkResTex;

typedef struct UnkResDictTexData {
    u32 unk_00;                 // 0x00 TEXIMAGE_PARAM value
    u32 unk_04;                 // 0x04 width (bits 0-10) / height (bits 11-21)
} UnkResDictTexData;

typedef struct UnkResDictPlttData {
    u16 unk_00;                 // 0x00 offset / 8
    u16 unk_02;                 // 0x02 flags (bit 0: 4-color palette)
} UnkResDictPlttData;

typedef struct UnkResDictToMatIdxData {
    u16 unk_00;                 // 0x00 offset of the material index list
    u8 unk_02;                  // 0x02 number of materials
    u8 unk_03;                  // 0x03 flags (bit 0: bound)
} UnkResDictToMatIdxData;

typedef struct UnkResMat {
    u16 unk_00;                 // 0x00 texture -> material dictionary offset
    u16 unk_02;                 // 0x02 palette -> material dictionary offset
    UnkResDict unk_04;          // 0x04 material dictionary
} UnkResMat;

typedef struct UnkResMatData {
    u8 unk_00[0x14];            // 0x00
    u32 unk_14;                 // 0x14 TEXIMAGE_PARAM
    u8 unk_18[4];               // 0x18
    u16 unk_1c;                 // 0x1C palette base
    u16 unk_1e;                 // 0x1E
    u16 unk_20;                 // 0x20 original width
    u16 unk_22;                 // 0x22 original height
    s32 unk_24;                 // 0x24 width scale
    s32 unk_28;                 // 0x28 height scale
} UnkResMatData;

typedef struct UnkResMdl {
    u32 unk_00;                 // 0x00
    u32 unk_04;                 // 0x04
    u32 unk_08;                 // 0x08 material block offset
} UnkResMdl;

typedef struct UnkResMdlSet {
    u32 unk_00;                 // 0x00 block kind
    u32 unk_04;                 // 0x04 block size
    UnkResDict unk_08;          // 0x08 model dictionary
} UnkResMdlSet;

typedef struct UnkResDictMdlSetData {
    u32 unk_00;                 // 0x00 model offset
} UnkResDictMdlSetData;

extern void func_02066958(void);
extern void func_020669b4(const void* src, u32 dest, u32 size);
extern void func_02066af4(void);
extern void func_02066b40(void);
extern void func_02066b74(const void* src, u32 dest, u32 size);
extern void func_02066be0(void);
extern s32 FX_Divide(s32 numer, s32 denom);
extern void* func_0206e664(const UnkResDict* dict, const void* name);

#define VRAM_KEY_ADDR(key) (((key) & 0xffff) << 3)

static inline void* GetResDataByIdx(const UnkResDict* dict, int idx) {
    UnkResDictEntryHeader* hdr = (UnkResDictEntryHeader*)((u8*)dict + dict->unk_06);
    return (u8*)hdr->unk_04 + hdr->unk_00 * idx;
}

typedef struct UnkResName {
    char unk_00[16];            // 0x00
} UnkResName;

static inline UnkResName* GetResNameByIdx(const UnkResDict* dict, int idx) {
    UnkResDictEntryHeader* hdr = (UnkResDictEntryHeader*)((u8*)dict + dict->unk_06);
    return &((UnkResName*)((u8*)hdr + hdr->unk_02))[idx];
}

static inline UnkResMatData* GetMatDataByIdx(UnkResMat* mat, int idx) {
    return (UnkResMatData*)((u8*)mat + *(u32*)GetResDataByIdx(&mat->unk_04, idx));
}

static inline UnkResMat* GetMat(UnkResMdl* mdl) {
    return (UnkResMat*)((u8*)mdl + mdl->unk_08);
}

u32 func_0206a600(UnkResTex* tex) {
    return tex->unk_08.unk_04 << 3;
}

u32 func_0206a60c(UnkResTex* tex) {
    return tex->unk_18.unk_04 << 3;
}

void func_0206a618(UnkResTex* tex, u32 texKey, u32 tex4x4Key) {
    if (texKey != 0) {
        tex->unk_08.unk_00 = texKey;
    }
    if (tex4x4Key != 0) {
        tex->unk_18.unk_00 = tex4x4Key;
    }
}

void func_0206a62c(UnkResTex* tex, BOOL execBeginEnd) {
    u32 size;
    u32 size4;

    if (execBeginEnd) {
        func_02066958();
    }

    size = tex->unk_08.unk_04 << 3;
    if (size != 0) {
        func_020669b4((u8*)tex + tex->unk_08.unk_0c, VRAM_KEY_ADDR(tex->unk_08.unk_00), size);
        tex->unk_08.unk_08 |= 1;
    }

    size4 = tex->unk_18.unk_04 << 3;
    if (size4 != 0) {
        u8* data = (u8*)tex + tex->unk_18.unk_0c;
        u8* idx = (u8*)tex + tex->unk_18.unk_10;
        u32 addr = VRAM_KEY_ADDR(tex->unk_18.unk_00);

        func_020669b4(data, addr, size4);
        func_020669b4(idx, 0x20000 + ((addr & 0x1ffff) >> 1) + ((addr & 0x40000) >> 2), size4 >> 1);
        tex->unk_18.unk_08 |= 1;
    }

    if (execBeginEnd) {
        func_02066af4();
    }
}

void func_0206a6e4(UnkResTex* tex, u32* texKey, u32* tex4x4Key) {
    tex->unk_08.unk_08 &= ~1;
    tex->unk_18.unk_08 &= ~1;
    *texKey = tex->unk_08.unk_00;
    tex->unk_08.unk_00 = 0;
    *tex4x4Key = tex->unk_18.unk_00;
    tex->unk_18.unk_00 = 0;
}

u32 func_0206a71c(UnkResTex* tex) {
    return tex->unk_2c.unk_04 << 3;
}

void func_0206a728(UnkResTex* tex, u32 plttKey) {
    tex->unk_2c.unk_00 = plttKey;
}

void func_0206a730(UnkResTex* tex, BOOL execBeginEnd) {
    if (execBeginEnd) {
        func_02066b40();
    }
    func_02066b74((u8*)tex + tex->unk_2c.unk_0c, VRAM_KEY_ADDR(tex->unk_2c.unk_00), tex->unk_2c.unk_04 << 3);
    tex->unk_2c.unk_06 |= 1;
    if (execBeginEnd) {
        func_02066be0();
    }
}

u32 func_0206a780(UnkResTex* tex) {
    u32 key;

    tex->unk_2c.unk_06 &= ~1;
    key = tex->unk_2c.unk_00;
    tex->unk_2c.unk_00 = 0;
    return key;
}

/* Binds one texture to every material that uses it. */
void func_0206a7a0(UnkResMat* mat, UnkResDictToMatIdxData* data, UnkResTex* tex, UnkResDictTexData* texData) {
    u8* idx = (u8*)mat + data->unk_00;
    u16 offset;
    u32 i;

    if ((texData->unk_00 & 0x1c000000) != 0x14000000) {
        offset = VRAM_KEY_ADDR(tex->unk_08.unk_00) >> 3;
    } else {
        offset = VRAM_KEY_ADDR(tex->unk_18.unk_00) >> 3;
    }
    for (i = 0; i < data->unk_02; i++) {
        UnkResMatData* matData = GetMatDataByIdx(mat, idx[i]);
        u32 w;
        u32 h;

        matData->unk_14 |= texData->unk_00 + offset;
        w = texData->unk_04 & 0x7ff;
        h = (texData->unk_04 >> 11) & 0x7ff;
        if (w != matData->unk_20) {
            matData->unk_24 = FX_Divide(w << 12, matData->unk_20 << 12);
        } else {
            matData->unk_24 = 0x1000;
        }
        if (h != matData->unk_22) {
            matData->unk_28 = FX_Divide(h << 12, matData->unk_22 << 12);
        } else {
            matData->unk_28 = 0x1000;
        }
    }
    data->unk_03 |= 1;
}

/* Unbinds one texture from its materials. */
void func_0206a894(UnkResMat* mat, UnkResDictToMatIdxData* data) {
    u8* idx = (u8*)mat + data->unk_00;
    u32 i;

    for (i = 0; i < data->unk_02; i++) {
        UnkResMatData* matData = GetMatDataByIdx(mat, idx[i]);

        matData->unk_14 &= 0xc00f0000;
        matData->unk_24 = 0x1000;
        matData->unk_28 = 0x1000;
    }
    data->unk_03 &= ~1;
}

BOOL func_0206a910(UnkResMdl* mdl, UnkResTex* tex) {
    UnkResMat* mat = GetMat(mdl);
    UnkResDict* dict = (UnkResDict*)((u8*)mat + mat->unk_00);
    int i;
    BOOL result = TRUE;

    for (i = 0; (u32)i < dict->unk_01; i++) {
        UnkResDictTexData* texData = func_0206e664(&tex->unk_3c, GetResNameByIdx(dict, i));

        if (texData != NULL) {
            UnkResDictToMatIdxData* data = GetResDataByIdx(dict, i);

            if (!(data->unk_03 & 1)) {
                func_0206a7a0(mat, data, tex, texData);
            }
        } else {
            result = FALSE;
        }
    }
    return result;
}

void func_0206a9b8(UnkResMdl* mdl) {
    UnkResMat* mat = GetMat(mdl);
    UnkResDict* dict = (UnkResDict*)((u8*)mat + mat->unk_00);
    u32 i;

    for (i = 0; i < dict->unk_01; i++) {
        UnkResDictToMatIdxData* data = GetResDataByIdx(dict, i);

        if (data->unk_03 & 1) {
            func_0206a894(mat, data);
        }
    }
}

/* Binds one palette to every material that uses it. */
void func_0206aa18(UnkResMat* mat, UnkResDictToMatIdxData* data, UnkResTex* tex, UnkResDictPlttData* plttData) {
    u8* idx = (u8*)mat + data->unk_00;
    u16 plttBase = plttData->unk_00;
    u16 vramOffset = VRAM_KEY_ADDR(tex->unk_2c.unk_00) >> 3;
    u32 i;

    if (!(plttData->unk_02 & 1)) {
        plttBase >>= 1;
        vramOffset >>= 1;
    }
    for (i = 0; i < data->unk_02; i++) {
        UnkResMatData* matData = GetMatDataByIdx(mat, idx[i]);

        matData->unk_1c = (u16)(plttBase + vramOffset);
    }
    data->unk_03 |= 1;
}

BOOL func_0206aab0(UnkResMdl* mdl, UnkResTex* tex) {
    UnkResMat* mat = GetMat(mdl);
    UnkResDict* dict = (UnkResDict*)((u8*)mat + mat->unk_02);
    int i;
    BOOL result = TRUE;

    for (i = 0; (u32)i < dict->unk_01; i++) {
        UnkResDictPlttData* plttData =
            func_0206e664((UnkResDict*)((u8*)tex + tex->unk_2c.unk_08), GetResNameByIdx(dict, i));

        if (plttData != NULL) {
            UnkResDictToMatIdxData* data = GetResDataByIdx(dict, i);

            if (!(data->unk_03 & 1)) {
                func_0206aa18(mat, data, tex, plttData);
            }
        } else {
            result = FALSE;
        }
    }
    return result;
}

void func_0206ab5c(UnkResMdl* mdl) {
    UnkResMat* mat = GetMat(mdl);
    UnkResDict* dict = (UnkResDict*)((u8*)mat + mat->unk_02);
    u32 i;

    for (i = 0; i < dict->unk_01; i++) {
        UnkResDictToMatIdxData* data = GetResDataByIdx(dict, i);

        if (data->unk_03 & 1) {
            data->unk_03 &= ~1;
        }
    }
}

BOOL func_0206abb4(UnkResMdlSet* mdlSet, UnkResTex* tex) {
    u32 i;
    BOOL result = TRUE;

    for (i = 0; i < mdlSet->unk_08.unk_01; i++) {
        UnkResMdl* mdl = (UnkResMdl*)((u8*)mdlSet + ((UnkResDictMdlSetData*)GetResDataByIdx(&mdlSet->unk_08, i))->unk_00);

        result &= func_0206a910(mdl, tex);
        result &= func_0206aab0(mdl, tex);
    }
    return result;
}

void func_0206ac24(UnkResMdlSet* mdlSet) {
    u32 i;

    for (i = 0; i < mdlSet->unk_08.unk_01; i++) {
        UnkResMdl* mdl = (UnkResMdl*)((u8*)mdlSet + ((UnkResDictMdlSetData*)GetResDataByIdx(&mdlSet->unk_08, i))->unk_00);

        func_0206a9b8(mdl);
        func_0206ab5c(mdl);
    }
}
