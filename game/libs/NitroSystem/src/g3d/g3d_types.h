#ifndef G3D_TYPES_H
#define G3D_TYPES_H

#include <nitro/types.h>

/* Common animation resource header. */
typedef struct UnkResAnmHeader {
    u8 unk_00;                  // 0x00 category ('M' material, 'J' joint, 'V' visibility)
    u8 unk_01;                  // 0x01
    u16 unk_02;                 // 0x02 sub category
} UnkResAnmHeader;

struct UnkAnmObj;

/* Fills the result of one material/joint/visibility for an animation object. */
typedef void (*UnkAnmFunc)(void* result, struct UnkAnmObj* anm, u32 dataIdx);

/* Animation object (0x1A + 2 * unk_19 bytes). */
typedef struct UnkAnmObj {
    s32 unk_00;                 // 0x00 frame
    s32 unk_04;                 // 0x04 blend ratio
    UnkResAnmHeader* unk_08;    // 0x08 animation resource
    UnkAnmFunc unk_0c;          // 0x0C animation function
    struct UnkAnmObj* unk_10;   // 0x10 next object
    void* unk_14;               // 0x14 texture resource
    u8 unk_18;                  // 0x18 priority
    u8 unk_19;                  // 0x19 number of map entries
    u16 unk_1a[1];              // 0x1A map: bit 8 = animated, low byte = data index
} UnkAnmObj;

/* Model resource info fields used here. */
typedef struct UnkResMdlInfo {
    u8 unk_00[0x17];            // 0x00
    u8 unk_17;                  // 0x17 number of nodes
    u8 unk_18;                  // 0x18 number of materials
} UnkResMdlInfo;

typedef BOOL (*UnkBlendFunc)(void* result, UnkAnmObj* anm, u32 id);

typedef void (*UnkAnmInitFunc)(UnkAnmObj* anm, void* resAnm, const UnkResMdlInfo* resMdl);

/* animation object init function for one resource category */
typedef struct UnkAnmInitEntry {
    u8 unk_00;                  // 0x00 category
    u16 unk_02;                 // 0x02 sub category
    UnkAnmInitFunc unk_04;      // 0x04 init function
} UnkAnmInitEntry;

/* Render object (0x54 bytes). */
typedef struct UnkRenderObj {
    u32 unk_00;                 // 0x00 flags
    void* unk_04;               // 0x04 model resource
    UnkAnmObj* unk_08;          // 0x08 material animations
    UnkBlendFunc unk_0c;        // 0x0C material blend function
    UnkAnmObj* unk_10;          // 0x10 joint animations
    UnkBlendFunc unk_14;        // 0x14 joint blend function
    UnkAnmObj* unk_18;          // 0x18 visibility animations
    UnkBlendFunc unk_1c;        // 0x1C visibility blend function
    u8 unk_20[0x1c];            // 0x20
    u32 unk_3c[2];              // 0x3C animated material bits
    u32 unk_44[2];              // 0x44 animated joint bits
    u32 unk_4c[2];              // 0x4C animated visibility bits
} UnkRenderObj;

#endif
