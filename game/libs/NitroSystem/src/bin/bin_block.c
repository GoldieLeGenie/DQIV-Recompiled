#include "bin_types.h"

/* Finds the first block of the given kind in a binary resource file. */
UnkBinBlockHeader* func_02068a5c(UnkBinFileHeader* header, u32 kind) {
    UnkBinBlockHeader* block = (UnkBinBlockHeader*)((u32)header + header->unk_0c);
    u16 i;

    for (i = 0; i < header->unk_0e; i++) {
        if (block->unk_00 == kind) {
            return block;
        }
        block = (UnkBinBlockHeader*)((u32)block + block->unk_04);
    }
    return NULL;
}
