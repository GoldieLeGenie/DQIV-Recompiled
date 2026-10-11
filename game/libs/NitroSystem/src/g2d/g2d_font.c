#include "g2d_font.h"

/* string splitter per character encoding */
const UnkCharSplitter data_020ba520[4] = {func_0206a188, func_0206a174, func_0206a204, func_0206a258};

typedef struct UnkCMapScanEntry {
    u16 unk_00;                 // 0x00 character code
    u16 unk_02;                 // 0x02 glyph index
} UnkCMapScanEntry;

typedef struct UnkCMapScan {
    u16 unk_00;                 // 0x00 number of entries
    UnkCMapScanEntry unk_02[1]; // 0x02 entries sorted by code
} UnkCMapScan;

/* Maps a character code to a glyph index inside one code map block. */
u16 func_02068f68(UnkFontCodeMap* cmap, u16 c) {
    u16 index = 0xffff;

    switch (cmap->unk_04) {
    case 0:
        index = (u16)(c - cmap->unk_00 + cmap->unk_0c[0]);
        break;
    case 1:
        index = cmap->unk_0c[c - cmap->unk_00];
        break;
    case 2: {
        UnkCMapScan* ws = (UnkCMapScan*)cmap->unk_0c;
        UnkCMapScanEntry* st = &ws->unk_02[0];
        UnkCMapScanEntry* ed = &ws->unk_02[ws->unk_00 - 1];

        while (st <= ed) {
            UnkCMapScanEntry* md = st + (ed - st) / 2;

            if (md->unk_00 < c) {
                st = md + 1;
            } else if (c < md->unk_00) {
                ed = md - 1;
            } else {
                index = md->unk_02;
                break;
            }
        }
    } break;
    }
    return index;
}

void func_02069030(UnkFont* font, void* resource) {
    func_02069ee4(resource, &font->unk_00);
    font->unk_04 = data_020ba520[font->unk_00->unk_07];
}

u16 func_02069060(UnkFont* font, u16 c) {
    UnkFontCodeMap* cmap;

    for (cmap = font->unk_00->unk_10; cmap != NULL; cmap = cmap->unk_08) {
        if (cmap->unk_00 <= c && c <= cmap->unk_02) {
            return func_02068f68(cmap, c);
        }
    }
    return 0xffff;
}

UnkCharWidths* func_020690a8(UnkFont* font, u16 index) {
    UnkFontWidth* cwdh;

    for (cwdh = font->unk_00->unk_0c; cwdh != NULL; cwdh = cwdh->unk_04) {
        if (cwdh->unk_00 <= index && index <= cwdh->unk_02) {
            return &cwdh->unk_08[index - cwdh->unk_00];
        }
    }
    return &font->unk_00->unk_04;
}
