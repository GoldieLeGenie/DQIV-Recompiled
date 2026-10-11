#include "g2d_font.h"
#include "../bin/bin_types.h"

/* Font resource (NFTR) loading: relocates block offsets and returns the font information block. */

#define SIG_NFTR 0x4e465452
#define SIG_FINF 0x46494e46
#define SIG_CGLP 0x43474c50
#define SIG_CWDH 0x43574448
#define SIG_CMAP 0x434d4150

extern void OS_Terminate(void);

typedef struct UnkFontInfoBlock {
    UnkBinBlockHeader unk_00;   // 0x00
    UnkFontInfo unk_08;         // 0x08
} UnkFontInfoBlock;

typedef struct UnkFontWidthBlock {
    UnkBinBlockHeader unk_00;   // 0x00
    UnkFontWidth unk_08;        // 0x08
} UnkFontWidthBlock;

typedef struct UnkFontCodeMapBlock {
    UnkBinBlockHeader unk_00;   // 0x00
    UnkFontCodeMap unk_08;      // 0x08
} UnkFontCodeMapBlock;

static inline BOOL IsValidSignature(UnkBinFileHeader* header) {
    if (header != NULL && header->unk_00 == SIG_NFTR) {
        return TRUE;
    }
    return FALSE;
}

static inline BOOL IsValidVersion(UnkBinFileHeader* header, u16 version) {
    if (header != NULL && header->unk_06 >= version) {
        return TRUE;
    }
    return FALSE;
}

static inline BOOL IsValidResource(UnkBinFileHeader* header, u16 version) {
    if (header != NULL) {
        return (IsValidSignature(header) != FALSE) && (IsValidVersion(header, version) != FALSE);
    }
    return FALSE;
}

BOOL func_02069ee4(void* resource, UnkFontInfo** info) {
    UnkBinFileHeader* header = (UnkBinFileHeader*)resource;
    BOOL oldVersion = FALSE;
    UnkBinBlockHeader* block;

    if (!IsValidResource(header, 0x101)) {
        if (!IsValidResource(header, 0x100)) {
            OS_Terminate();
        }
        oldVersion = TRUE;
    }

    func_0206a038(header);
    block = func_02068a5c(header, SIG_FINF);
    if (block == NULL) {
        *info = NULL;
        return FALSE;
    }
    *info = &((UnkFontInfoBlock*)block)->unk_08;
    if (oldVersion) {
        (*info)->unk_08->unk_07 = 0;
    }
    return TRUE;
}

/* Converts the file-relative offsets of every block into pointers. */
void func_0206a038(void* resource) {
    UnkBinFileHeader* header = (UnkBinFileHeader*)resource;
    UnkBinBlockHeader* block = (UnkBinBlockHeader*)((u32)header + header->unk_0c);
    int i;

    for (i = 0; i < header->unk_0e; i++) {
        switch (block->unk_00) {
        case SIG_FINF: {
            UnkFontInfo* finf = &((UnkFontInfoBlock*)block)->unk_08;

            finf->unk_08 = (UnkFontGlyph*)((u32)finf->unk_08 + (u32)header);
            if (finf->unk_0c != NULL) {
                finf->unk_0c = (UnkFontWidth*)((u32)finf->unk_0c + (u32)header);
            }
            if (finf->unk_10 != NULL) {
                finf->unk_10 = (UnkFontCodeMap*)((u32)finf->unk_10 + (u32)header);
            }
        } break;
        case SIG_CGLP:
            break;
        case SIG_CWDH: {
            UnkFontWidth* cwdh = &((UnkFontWidthBlock*)block)->unk_08;

            if (cwdh->unk_04 != NULL) {
                cwdh->unk_04 = (UnkFontWidth*)((u32)cwdh->unk_04 + (u32)header);
            }
        } break;
        case SIG_CMAP: {
            UnkFontCodeMap* cmap = &((UnkFontCodeMapBlock*)block)->unk_08;

            if (cmap->unk_08 != NULL) {
                cmap->unk_08 = (UnkFontCodeMap*)((u32)cmap->unk_08 + (u32)header);
            }
        } break;
        }
        block = (UnkBinBlockHeader*)((u32)block + block->unk_04);
    }
}
