#ifndef BIN_TYPES_H
#define BIN_TYPES_H

#include <nitro/types.h>

/* Nitro binary resource file header. */
typedef struct UnkBinFileHeader {
    u32 unk_00;                 // 0x00 signature
    u16 unk_04;                 // 0x04 byte order mark
    u16 unk_06;                 // 0x06 version
    u32 unk_08;                 // 0x08 file size
    u16 unk_0c;                 // 0x0C header size
    u16 unk_0e;                 // 0x0E number of blocks
} UnkBinFileHeader;

typedef struct UnkBinBlockHeader {
    u32 unk_00;                 // 0x00 block kind
    u32 unk_04;                 // 0x04 block size
} UnkBinBlockHeader;

UnkBinBlockHeader* func_02068a5c(UnkBinFileHeader* header, u32 kind);

#endif
