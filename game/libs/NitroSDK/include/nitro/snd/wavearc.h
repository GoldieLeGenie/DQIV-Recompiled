#ifndef NITRO_SND_WAVEARC_H
#define NITRO_SND_WAVEARC_H

#include <nitro/types.h>

typedef struct UnkSndWaveArc UnkSndWaveArc;

typedef struct UnkSndWaveArcLink {
    UnkSndWaveArc* waveArc;
    struct UnkSndWaveArcLink* next;
} UnkSndWaveArcLink;

/* The variable-length wave table follows the 0x3c-byte archive header. */
struct UnkSndWaveArc {
    u8 unk_00[8];
    u32 fileSize;                  // 0x08
    u8 unk_0c[4];
    u8 blockHeader[8];             // 0x10
    UnkSndWaveArcLink* topLink;     // 0x18
    u32 reserved[7];               // 0x1c
    u32 waveCount;                 // 0x38
    u32 waveOffset[0];             // 0x3c
};

#endif
