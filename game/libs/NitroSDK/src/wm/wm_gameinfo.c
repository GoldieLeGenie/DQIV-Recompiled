#include <nitro/types.h>
#include <nitro/os/cache.h>
#include <nitro/mi/cpumem.h>
#include "wm_bssdesc.h"

/* Checks the game info header of a parent description (magic 0x2348 / 0xBD8A, version 4). */
BOOL func_0205e828(UnkWmBssDesc* desc) {
    vu16 info[4];

    if (desc == NULL) {
        return FALSE;
    }
    if (desc->unk_3c == 0) {
        return FALSE;
    }
    MI_CpuCopyU8(desc->unk_50, (void*)info, sizeof(info));
    DC_CleanRange((void*)info, sizeof(info));
    if (desc->unk_44 == 0 && (info[0] == 0x2348 || info[0] == 0xBD8A) && info[3] == 4) {
        return TRUE;
    }
    return FALSE;
}
