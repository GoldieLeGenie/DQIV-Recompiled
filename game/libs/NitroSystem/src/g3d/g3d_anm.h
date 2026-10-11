#ifndef G3D_ANM_H
#define G3D_ANM_H

#include "g3d_f.h"

typedef struct {
    u8 revision;                    /* 0x00 */
    u8 numEntry;                    /* 0x01 */
    u16 sizeDictBlk;                /* 0x02 */
    u16 dummy_;                     /* 0x04 */
    u16 ofsEntry;                   /* 0x06 */
} UnkResDict;

typedef struct {
    u16 sizeUnit;                   /* 0x00 */
    u16 ofsName;                    /* 0x02 */
    u8 data[4];                     /* 0x04 */
} UnkResDictEntryHeader;

typedef struct {
    char name[16];
} UnkResName;

typedef struct {
    u8 sbcType;                     /* 0x00 */
    u8 scalingRule;                 /* 0x01 */
    u8 texMtxMode;                  /* 0x02 */
    u8 numNode;                     /* 0x03 */
    u8 numMat;                      /* 0x04 */
    u8 numShp;                      /* 0x05 */
    u8 firstUnusedMtxStackID;       /* 0x06 */
    u8 dummy_;                      /* 0x07 */
} UnkResMdlInfo;

typedef struct {
    u32 size;                       /* 0x00 */
    u32 ofsSbc;                     /* 0x04 */
    u32 ofsMat;                     /* 0x08 */
    u32 ofsShp;                     /* 0x0C */
    u32 ofsEvpMtx;                  /* 0x10 */
    UnkResMdlInfo info;             /* 0x14 */
} UnkResMdl;

typedef struct {
    u16 ofsTextureMatchDict;        /* 0x00 */
    u16 ofsPaletteMatchDict;        /* 0x02 */
    UnkResDict dict;                /* 0x04 */
} UnkResMat;

/* common header of the animation resources */
typedef struct {
    u8 category0;                   /* 0x00 */
    u8 revision;                    /* 0x01 */
    u16 category1;                  /* 0x02 */
    u16 numFrame;                   /* 0x04 */
    u16 flag;                       /* 0x06 */
    UnkResDict dict;                /* 0x08 */
} UnkResAnm;

struct UnkAnmObj;
typedef void (*UnkAnmFunc)(void* result, const struct UnkAnmObj* obj, u32 dataIdx);

typedef struct UnkAnmObj {
    fx32 frame;                     /* 0x00 */
    fx32 ratio;                     /* 0x04 */
    void* resAnm;                   /* 0x08 */
    UnkAnmFunc funcAnm;             /* 0x0C */
    struct UnkAnmObj* next;         /* 0x10 */
    const void* resTex;             /* 0x14 */
    u8 priority;                    /* 0x18 */
    u8 numMapData;                  /* 0x19 */
    u16 mapData[1];                 /* 0x1A */
} UnkAnmObj;

static inline const UnkResName* UnkGetResNameByIdx_(const UnkResDict* dict, u32 idx)
{
    const UnkResDictEntryHeader* p = (const UnkResDictEntryHeader*)((const u8*)dict + dict->ofsEntry);
    return (const UnkResName*)((const u8*)p + p->ofsName) + idx;
}

static inline void* UnkGetResDataByIdx_(const UnkResDict* dict, u32 idx)
{
    const UnkResDictEntryHeader* p = (const UnkResDictEntryHeader*)((const u8*)dict + dict->ofsEntry);
    return (void*)(&p->data[0] + p->sizeUnit * idx);
}

int func_0206e7a0(const UnkResDict* dict, const UnkResName* name);
void* func_0206e664(const UnkResDict* dict, const UnkResName* name);

#endif
