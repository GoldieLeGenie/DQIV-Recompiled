#ifndef FS_OVERLAY_INTERNAL_H
#define FS_OVERLAY_INTERNAL_H

#include <nitro/types.h>
#include <nitro/reg.h>
#include <nitro/fs/file.h>

typedef void (*UnkOverlayCtor)(void);

/* Overlay table entry (0x20 bytes from the OVT) + where its file lives on the card. */
typedef struct UnkOverlayInfo {
    /* 0x00 */ u32             id;
    /* 0x04 */ u8*             addr;
    /* 0x08 */ u32             ramSize;
    /* 0x0C */ u32             bssSize;
    /* 0x10 */ UnkOverlayCtor* ctorStart;
    /* 0x14 */ UnkOverlayCtor* ctorEnd;
    /* 0x18 */ u32             fileId;
    /* 0x1C */ u32             compressedSize : 24;
    /* 0x1C */ u32             flags          : 8;
    /* 0x20 */ u32             target;
    /* 0x24 */ u32             filePos;
    /* 0x28 */ u32             fileLen;
} UnkOverlayInfo; /* size 0x2C */

/* Element of the global destructor chain (Metrowerks runtime). */
typedef struct UnkDestructorChain {
    /* 0x00 */ struct UnkDestructorChain* next;
    /* 0x04 */ void (*dtor)(void* object);
    /* 0x08 */ void* object;
} UnkDestructorChain;

/* HMAC key descriptor used to check overlay digests. */
typedef struct UnkDigestKey {
    /* 0x00 */ const void* key;
    /* 0x04 */ u32         size;
} UnkDigestKey;

extern FS_Record           fsi_arc_rom;   /* 0x0210cdfc .bss */
extern CartridgeRegion     fsi_ovt9;      /* 0x0210cdec .bss */
extern CartridgeRegion     fsi_ovt7;      /* 0x0210cdf4 .bss */
extern UnkDestructorChain* __global_destructor_chain; /* 0x020c4f6c .bss */
extern const u8            data_020b634c[0x40]; /* 0x020b634c .rodata, digest HMAC key */
extern UnkDigestKey        data_020c3db0; /* 0x020c3db0 .data, 8 bytes {data_020b634c, 0x40} */
/* Overlay digest table (20 bytes per overlay): start and end labels, both 0x020c4a08 in this ROM */
extern u8 data_020c4a08[];
extern u8 data_020c4a08_end[];

/* file system functions (fs/file.c) */
void func_02060e04(FS_File* file);
BOOL func_0206102c(FS_File* file, FS_Record* record, u32 start, u32 end, u32 idx);
BOOL func_02061074(FS_File* file, FS_FileIdentifier iden);
BOOL func_0206112c(FS_File* file);
s32  func_02061270(FS_File* file, void* dest, s32 size);
/* HMAC (digest, src, size, key, keySize) */
void func_0205f4dc(void* digest, const void* src, u32 size, const void* key, u32 keySize);

void IC_InvalidateRange(void* addr, u32 count);
void DC_InvalidateRange(void* addr, u32 count);
void DC_PurgeRange(const void* addr, u32 count);
void MI_CpuSet(void* dest, u8 value, u32 count);
void MI_CpuCopyU8(const void* src, void* dest, u32 count);
void MIi_UncompressBackward(void* bottom);
void OS_Terminate(void);
u32  OS_DisableIRQ(void);
u32  OS_RestoreIRQ(u32 irq);

#endif
