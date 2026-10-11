// Sequence archive: sequence entry lookup.
#include "snd_internal.h"

typedef struct UnkSndSeqArcSeqInfo {
    /* 0x00 */ u32 offset;
    /* 0x04 */ u16 bankNo;
    /* 0x06 */ u8 volume;
    /* 0x07 */ u8 channelPrio;
    /* 0x08 */ u8 playerPrio;
    /* 0x09 */ u8 playerNo;
    /* 0x0A */ u16 unk_0a;
} UnkSndSeqArcSeqInfo;

typedef struct UnkSndSeqArc {
    /* 0x00 */ u8 unk_00[0x1C];
    /* 0x1C */ u32 count;
    /* 0x20 */ UnkSndSeqArcSeqInfo table[1];
} UnkSndSeqArc;

UnkSndSeqArcSeqInfo* func_02077204(UnkSndSeqArc* seqArc, int index) {
    UnkSndSeqArcSeqInfo* info;

    if (index < 0) {
        return NULL;
    }
    if (index >= seqArc->count) {
        return NULL;
    }

    info = &seqArc->table[index];
    if (info->offset == 0xFFFFFFFF) {
        return NULL;
    }
    return info;
}
