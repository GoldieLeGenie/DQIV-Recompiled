#ifndef WM_BSSDESC_H
#define WM_BSSDESC_H

#include <nitro/types.h>

/* Known fields of the wireless parent description shared by these modules. */
typedef struct UnkWmBssDesc {
    u16 length;                    // 0x00, in halfwords
    u8 unk_02[0x3a];
    u16 unk_3c;                    // 0x3c, game info length
    u8 unk_3e[6];
    u32 unk_44;                    // 0x44
    u8 unk_48[8];
    u8 unk_50[8];                  // 0x50, start of game info
} UnkWmBssDesc;

#endif
