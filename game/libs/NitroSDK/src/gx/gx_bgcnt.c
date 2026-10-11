#include "gx_internal.h"

/* BG screen / character base address getters */

void *func_02064ad0(void) {
    s32 blockOffset = (REG_BG0CNT & 0x1f00) >> 8;
    u32 offset      = (REG_DISPCNT_V & 0x38000000) >> 27;
    return (void *)(0x6000000 + offset * 0x10000 + blockOffset * 0x800);
}

void *func_02064b04(void) {
    s32 blockOffset = (REG_BG0CNT_SUB & 0x1f00) >> 8;
    return (void *)(0x6200000 + blockOffset * 0x800);
}

void *func_02064b24(void) {
    s32 blockOffset = (REG_BG1CNT & 0x1f00) >> 8;
    u32 offset      = (REG_DISPCNT_V & 0x38000000) >> 27;
    return (void *)(0x6000000 + offset * 0x10000 + blockOffset * 0x800);
}

void *func_02064b58(void) {
    s32 blockOffset = (REG_BG1CNT_SUB & 0x1f00) >> 8;
    return (void *)(0x6200000 + blockOffset * 0x800);
}

void *func_02064b78(void) {
    u32 bgMode = REG_DISPCNT_V & 7;
    u32 bgcnt  = REG_BG2CNT;
    u32 offset = ((REG_DISPCNT_V & 0x38000000) >> 27) * 0x10000;
    u32 block  = (bgcnt & 0x1f00) >> 8;

    switch (bgMode) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        return (void *)(0x6000000 + offset + block * 0x800);
    case 5:
        if (bgcnt & 0x80) {
            return (void *)(0x6000000 + block * 0x4000);
        } else {
            return (void *)(0x6000000 + offset + block * 0x800);
        }
    case 6:
        return (void *)0x6000000;
    default:
        return NULL;
    }
}

void *func_02064bfc(void) {
    u32 bgMode = REG_DISPCNT_SUB_V & 7;
    u32 bgcnt  = REG_BG2CNT_SUB;
    u32 block  = (bgcnt & 0x1f00) >> 8;

    switch (bgMode) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        return (void *)(0x6200000 + block * 0x800);
    case 5:
        if (bgcnt & 0x80) {
            return (void *)(0x6200000 + block * 0x4000);
        } else {
            return (void *)(0x6200000 + block * 0x800);
        }
    case 6:
        return NULL;
    default:
        return NULL;
    }
}

void *func_02064c70(void) {
    u32 bgMode = REG_DISPCNT_V & 7;
    u32 bgcnt  = REG_BG3CNT;
    u32 offset = ((REG_DISPCNT_V & 0x38000000) >> 27) * 0x10000;
    u32 block  = (bgcnt & 0x1f00) >> 8;

    switch (bgMode) {
    case 0:
    case 1:
    case 2:
        return (void *)(0x6000000 + offset + block * 0x800);
    case 3:
    case 4:
    case 5:
        if (bgcnt & 0x80) {
            return (void *)(0x6000000 + block * 0x4000);
        } else {
            return (void *)(0x6000000 + offset + block * 0x800);
        }
    case 6:
        return NULL;
    default:
        return NULL;
    }
}

void *func_02064cf4(void) {
    u32 bgMode = REG_DISPCNT_SUB_V & 7;
    u32 bgcnt  = REG_BG3CNT_SUB;
    u32 block  = (bgcnt & 0x1f00) >> 8;

    switch (bgMode) {
    case 0:
    case 1:
    case 2:
        return (void *)(0x6200000 + block * 0x800);
    case 3:
    case 4:
    case 5:
        if (bgcnt & 0x80) {
            return (void *)(0x6200000 + block * 0x4000);
        } else {
            return (void *)(0x6200000 + block * 0x800);
        }
    case 6:
        return NULL;
    default:
        return NULL;
    }
}

void *func_02064d68(void) {
    s32 blockOffset = (REG_BG0CNT & 0x3c) >> 2;
    u32 offset      = (REG_DISPCNT_V & 0x7000000) >> 24;
    return (void *)(0x6000000 + offset * 0x10000 + blockOffset * 0x4000);
}

void *func_02064d9c(void) {
    s32 blockOffset = (REG_BG0CNT_SUB & 0x3c) >> 2;
    return (void *)(0x6200000 + blockOffset * 0x4000);
}

void *func_02064dbc(void) {
    s32 blockOffset = (REG_BG1CNT & 0x3c) >> 2;
    u32 offset      = (REG_DISPCNT_V & 0x7000000) >> 24;
    return (void *)(0x6000000 + offset * 0x10000 + blockOffset * 0x4000);
}

void *func_02064df0(void) {
    s32 blockOffset = (REG_BG1CNT_SUB & 0x3c) >> 2;
    return (void *)(0x6200000 + blockOffset * 0x4000);
}

void *func_02064e10(void) {
    s32 bgMode = REG_DISPCNT_V & 7;
    u32 bgcnt  = REG_BG2CNT;

    if (bgMode < 5 || !(bgcnt & 0x80)) {
        u32 offset = (REG_DISPCNT_V & 0x7000000) >> 24;
        u32 block  = (bgcnt & 0x3c) >> 2;
        return (void *)(0x6000000 + offset * 0x10000 + block * 0x4000);
    }
    return NULL;
}

void *func_02064e60(void) {
    s32 bgMode = REG_DISPCNT_SUB_V & 7;
    u32 bgcnt  = REG_BG2CNT_SUB;

    if (bgMode < 5 || !(bgcnt & 0x80)) {
        u32 block = (bgcnt & 0x3c) >> 2;
        return (void *)(0x6200000 + block * 0x4000);
    }
    return NULL;
}

void *func_02064ea0(void) {
    s32 bgMode = REG_DISPCNT_V & 7;
    u32 bgcnt  = REG_BG3CNT;

    if (bgMode < 3 || (bgMode < 6 && !(bgcnt & 0x80))) {
        u32 offset = (REG_DISPCNT_V & 0x7000000) >> 24;
        u32 block  = (bgcnt & 0x3c) >> 2;
        return (void *)(0x6000000 + offset * 0x10000 + block * 0x4000);
    }
    return NULL;
}

void *func_02064ef8(void) {
    s32 bgMode = REG_DISPCNT_SUB_V & 7;
    u32 bgcnt  = REG_BG3CNT_SUB;

    if (bgMode < 3 || (bgMode < 6 && !(bgcnt & 0x80))) {
        u32 block = (bgcnt & 0x3c) >> 2;
        return (void *)(0x6200000 + block * 0x4000);
    }
    return NULL;
}
