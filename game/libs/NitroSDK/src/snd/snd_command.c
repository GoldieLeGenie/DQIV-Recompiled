#include "snd_internal.h"
#ifdef DQIV_NATIVE
void pcSndRequire(void);
BOOL pcSndExecute(UnkSndCommand* list);
void pcSndRethrow(void);
void pcSndWait(void);
#define OS_Delay(count) pcSndWait()
#endif

/* Command manager state: separate file statics (each one gets its own alias class, which lets the
   scheduler move the command-table load past the recvIndex store). The definition order gives the
   .bss layout under -ipa file. */
static u32            data_021145c4; /* finished tag */
static u32            data_021145e0; /* current tag */
static UnkSndCommand* data_021145d0; /* free list end */
static UnkSndCommand* data_021145c8; /* reserve list */
static UnkSndCommand* data_021145c0; /* free list */
static UnkSndCommand* data_021145cc; /* reserve list end */
static s32            data_021145d4; /* receive index */
static s32            data_021145d8; /* send index */
static s32            data_021145dc; /* send count */
static UnkSndCommand* data_021145e4[9];
static ATTRIBUTE_ALIGN(32) UnkSndSharedWork data_02114620;
static ATTRIBUTE_ALIGN(32) UnkSndCommand data_021148a0[256];
extern UnkSndSharedWork* data_02116100;


void func_0207a744(void) {
    s32 i;

    func_0207ad44();
    data_021145c0 = data_021148a0;
    for (i = 0; i < 255; i++) {
        data_021148a0[i].next = &data_021148a0[i + 1];
    }
    data_021148a0[255].next = NULL;
    data_021145d0 = &data_021148a0[255];
    data_021145c8 = NULL;
    data_021145cc = NULL;
    data_021145dc = 0;
    data_021145d4 = 0;
    data_021145d8 = 0;
    data_021145e0 = 1;
    data_021145c4 = 0;
    data_02116100 = &data_02114620;
    func_0207afa8(&data_02114620);

    {
        UnkSndCommand* cmd = func_0207a928(1);
        if (cmd == NULL) {
            return;
        }
        cmd->id = 29;
        cmd->arg[0] = (u32)data_02116100;
        func_0207a9b0(cmd);
        func_0207a9e8(1);
    }
}

UnkSndCommand* func_0207a818(u32 flags) {
    u32 enabled = OS_DisableIRQ();
    UnkSndCommand* head;
    UnkSndCommand* tail;

    if (flags & 1) {
        while (data_021145c4 == func_0207af80()) {
            OS_RestoreIRQ(enabled);
            OS_Delay(100);
            enabled = OS_DisableIRQ();
        }
    } else if (data_021145c4 == func_0207af80()) {
        OS_RestoreIRQ(enabled);
        return NULL;
    }

    head = data_021145e4[data_021145d4];
    data_021145d4++;
    if (data_021145d4 > 8) {
        data_021145d4 = 0;
    }

    tail = head;
    while (tail->next != NULL) {
        tail = tail->next;
    }

    if (data_021145d0 != NULL) {
        data_021145d0->next = head;
    } else {
        data_021145c0 = head;
    }
    data_021145d0 = tail;
    data_021145dc--;
    data_021145c4++;

    OS_RestoreIRQ(enabled);
    return head;
}

UnkSndCommand* func_0207a928(u32 flags) {
    UnkSndCommand* cmd;

    if (!func_0207ae14()) {
        return NULL;
    }
    cmd = func_0207adcc();
    if (cmd != NULL) {
        return cmd;
    }
    if (!(flags & 1)) {
        return NULL;
    }

    if (func_0207ad04() > 0) {
        while (func_0207a818(0) != NULL) {
        }
        cmd = func_0207adcc();
        if (cmd != NULL) {
            return cmd;
        }
    } else {
        func_0207a9e8(1);
    }

    func_0207ada4();
    do {
        func_0207a818(1);
        cmd = func_0207adcc();
    } while (cmd == NULL);
    return cmd;
}

void func_0207a9b0(UnkSndCommand* cmd) {
    u32 enabled = OS_DisableIRQ();

    if (data_021145cc == NULL) {
        data_021145c8 = cmd;
        data_021145cc = cmd;
    } else {
        data_021145cc->next = cmd;
        data_021145cc = cmd;
    }
    cmd->next = NULL;

    OS_RestoreIRQ(enabled);
}

BOOL func_0207a9e8(u32 flags) {
    u32 enabled = OS_DisableIRQ();

    if (data_021145c8 == NULL) {
        OS_RestoreIRQ(enabled);
        return TRUE;
    }

    if (data_021145dc >= 8) {
        if (!(flags & 1)) {
            OS_RestoreIRQ(enabled);
            return FALSE;
        }
        do {
            func_0207a818(1);
        } while (data_021145dc >= 8);

        if (data_021145c8 == NULL) {
            OS_RestoreIRQ(enabled);
            return TRUE;
        }
    }

    DC_PurgeRange(data_021148a0, sizeof(data_021148a0));
#ifdef DQIV_NATIVE
    if (!pcSndExecute(data_021145c8)) {
        OS_RestoreIRQ(enabled);
        pcSndRethrow();
        return FALSE;
    }
#else
    if (PXI_SendWordByFifo(7, (u32)data_021145c8, FALSE) < 0) {
        if (!(flags & 1)) {
            OS_RestoreIRQ(enabled);
            return FALSE;
        }
        while (data_021145dc >= 8 || PXI_SendWordByFifo(7, (u32)data_021145c8, FALSE) < 0) {
            OS_RestoreIRQ(enabled);
            func_0207a818(0);
            enabled = OS_DisableIRQ();
            DC_PurgeRange(data_021148a0, sizeof(data_021148a0));
            if (data_021145c8 == NULL) {
                OS_RestoreIRQ(enabled);
                return TRUE;
            }
        }
    }

#endif
    data_021145e4[data_021145d8] = data_021145c8;
    data_021145d8++;
    if (data_021145d8 > 8) {
        data_021145d8 = 0;
    }
    data_021145c8 = NULL;
    data_021145cc = NULL;
    data_021145dc++;
    data_021145e0++;

    OS_RestoreIRQ(enabled);

    if (flags & 2) {
        func_0207ada4();
    }
    return TRUE;
}

void func_0207aba4(u32 tag) {
    if (func_0207ac3c(tag)) {
        return;
    }
    while (func_0207a818(0) != NULL) {
    }
    if (func_0207ac3c(tag)) {
        return;
    }
    func_0207ada4();
    if (func_0207ac3c(tag)) {
        return;
    }
    do {
        func_0207a818(1);
    } while (!func_0207ac3c(tag));
}

u32 func_0207ac10(void) {
    u32 tag;
    u32 enabled = OS_DisableIRQ();

    if (data_021145c8 == NULL) {
        tag = data_021145c4;
    } else {
        tag = data_021145e0;
    }
    OS_RestoreIRQ(enabled);
    return tag;
}

BOOL func_0207ac3c(u32 tag) {
    BOOL result;
    u32 enabled = OS_DisableIRQ();

    if (tag > data_021145c4) {
        if (tag - data_021145c4 < 0x80000000) {
            result = FALSE;
        } else {
            result = TRUE;
        }
    } else {
        if (data_021145c4 - tag < 0x80000000) {
            result = TRUE;
        } else {
            result = FALSE;
        }
    }
    OS_RestoreIRQ(enabled);
    return result;
}

s32 func_0207ac8c(void) {
    u32 enabled = OS_DisableIRQ();
    s32 count = 0;
    UnkSndCommand* cmd;

    for (cmd = data_021145c0; cmd != NULL; cmd = cmd->next) {
        count++;
    }
    OS_RestoreIRQ(enabled);
    return count;
}

s32 func_0207acc8(void) {
    u32 enabled = OS_DisableIRQ();
    s32 count = 0;
    UnkSndCommand* cmd;

    for (cmd = data_021145c8; cmd != NULL; cmd = cmd->next) {
        count++;
    }
    OS_RestoreIRQ(enabled);
    return count;
}

s32 func_0207ad04(void) {
    s32 freeCount = func_0207ac8c();
    s32 reserveCount = func_0207acc8();
    return 256 - freeCount - reserveCount;
}

void func_0207ad20(u32 tag, u32 data, BOOL err) {
    u32 enabled = OS_DisableIRQ();
    func_0207aed4(data);
    OS_RestoreIRQ(enabled);
}

void func_0207ad44(void) {
#ifdef DQIV_NATIVE
    pcSndRequire();
#else
    func_0207a180(7, func_0207ad20);
    if (!func_0207ae14()) {
        return;
    }
    while (!func_0207a1cc(7, 1)) {
        OS_Delay(100);
    }
#endif
}

void func_0207ada4(void) {
#ifdef DQIV_NATIVE
    // Native command lists execute synchronously when flushed.
    pcSndRethrow();
#else
    while (PXI_SendWordByFifo(7, 0, FALSE) < 0) {
    }
#endif
}

UnkSndCommand* func_0207adcc(void) {
    UnkSndCommand* cmd;
    u32 enabled = OS_DisableIRQ();

    cmd = data_021145c0;
    if (cmd == NULL) {
        OS_RestoreIRQ(enabled);
        return NULL;
    }
    data_021145c0 = cmd->next;
    if (data_021145c0 == NULL) {
        data_021145d0 = NULL;
    }
    OS_RestoreIRQ(enabled);
    return cmd;
}

BOOL func_0207ae14(void) {
#ifdef DQIV_NATIVE
    pcSndRequire();
    return TRUE;
#else
    u32 enabled;
    u32 result;

    if (!func_02078d40()) {
        return TRUE;
    }
    enabled = OS_DisableIRQ();
    *(vu32*)0x04fff200 = 0x10;
    result = *(vu32*)0x04fff200;
    OS_RestoreIRQ(enabled);
    return result ? TRUE : FALSE;
#endif
}
