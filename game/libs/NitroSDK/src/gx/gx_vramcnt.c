#include "gx_internal.h"

UnkGxVramState data_0210ce5c;

void func_02079f00(u16 bank, u16 lockId);

/* Initialize the VRAM bank allocation state and the VRAM control registers */
void func_02063960(void) {
    data_0210ce5c.lcdc          = 0;
    data_0210ce5c.bg            = 0;
    data_0210ce5c.obj           = 0;
    data_0210ce5c.arm7          = 0;
    data_0210ce5c.tex           = 0;
    data_0210ce5c.texPltt       = 0;
    data_0210ce5c.clearImage    = 0;
    data_0210ce5c.bgExtPltt     = 0;
    data_0210ce5c.objExtPltt    = 0;
    data_0210ce5c.subBg         = 0;
    data_0210ce5c.subObj        = 0;
    data_0210ce5c.subBgExtPltt  = 0;
    data_0210ce5c.subObjExtPltt = 0;

    REG_VRAM_CNT_ABCD = 0;
    REG_VRAM_CNT_E    = 0;
    REG_VRAM_CNT_F    = 0;
    REG_VRAM_CNT_G    = 0;
    REG_VRAM_CNT_HI   = 0;
}

void SetLCDC(u32 lcdc) {
    if (lcdc & 0x001) REG_VRAM_CNT_A = 0x80;
    if (lcdc & 0x002) REG_VRAM_CNT_B = 0x80;
    if (lcdc & 0x004) REG_VRAM_CNT_C = 0x80;
    if (lcdc & 0x008) REG_VRAM_CNT_D = 0x80;
    if (lcdc & 0x010) REG_VRAM_CNT_E = 0x80;
    if (lcdc & 0x020) REG_VRAM_CNT_F = 0x80;
    if (lcdc & 0x040) REG_VRAM_CNT_G = 0x80;
    if (lcdc & 0x080) REG_VRAM_CNT_H = 0x80;
    if (lcdc & 0x100) REG_VRAM_CNT_I = 0x80;
}

void GX_SetBankForBg(s32 bg) {
    data_0210ce5c.lcdc = ~bg & (data_0210ce5c.lcdc | data_0210ce5c.bg);
    data_0210ce5c.bg   = bg;

    switch (bg) {
    case 0x08:
        REG_VRAM_CNT_D = 0x81;
        break;
    case 0x0c:
        REG_VRAM_CNT_D = 0x89;
    case 0x04:
        REG_VRAM_CNT_C = 0x81;
        break;
    case 0x0e:
        REG_VRAM_CNT_D = 0x91;
    case 0x06:
        REG_VRAM_CNT_C = 0x89;
    case 0x02:
        REG_VRAM_CNT_B = 0x81;
        break;
    case 0x0f:
        REG_VRAM_CNT_D = 0x99;
    case 0x07:
        REG_VRAM_CNT_C = 0x91;
    case 0x03:
        REG_VRAM_CNT_B = 0x89;
    case 0x01:
        REG_VRAM_CNT_A = 0x81;
        break;
    case 0x0b:
        REG_VRAM_CNT_A = 0x81;
        REG_VRAM_CNT_B = 0x89;
        REG_VRAM_CNT_D = 0x91;
        break;
    case 0x0d:
        REG_VRAM_CNT_D = 0x91;
    case 0x05:
        REG_VRAM_CNT_A = 0x81;
        REG_VRAM_CNT_C = 0x89;
        break;
    case 0x09:
        REG_VRAM_CNT_A = 0x81;
        REG_VRAM_CNT_D = 0x89;
        break;
    case 0x0a:
        REG_VRAM_CNT_B = 0x81;
        REG_VRAM_CNT_D = 0x89;
        break;
    case 0x70:
        REG_VRAM_CNT_G = 0x99;
    case 0x30:
        REG_VRAM_CNT_F = 0x91;
    case 0x10:
        REG_VRAM_CNT_E = 0x81;
        break;
    case 0x50:
        REG_VRAM_CNT_G = 0x91;
        REG_VRAM_CNT_E = 0x81;
        break;
    case 0x60:
        REG_VRAM_CNT_G = 0x89;
    case 0x20:
        REG_VRAM_CNT_F = 0x81;
        break;
    case 0x40:
        REG_VRAM_CNT_G = 0x81;
        break;
    default:
        break;
    }
    SetLCDC(data_0210ce5c.lcdc);
}

/* Like GX_SetBankForBg, but with the E/F/G banks and the A-D banks given separately */
void func_02063d08(s32 efg, s32 abcd) {
    s32 bg = efg | abcd;
    data_0210ce5c.lcdc = ~bg & (data_0210ce5c.lcdc | data_0210ce5c.bg);
    data_0210ce5c.bg   = bg;

    switch (efg) {
    case 0x70:
        REG_VRAM_CNT_G = 0x99;
    case 0x30:
        REG_VRAM_CNT_F = 0x91;
    case 0x10:
        REG_VRAM_CNT_E = 0x81;
        break;
    case 0x50:
        REG_VRAM_CNT_G = 0x91;
        REG_VRAM_CNT_E = 0x81;
        break;
    case 0x60:
        REG_VRAM_CNT_G = 0x89;
    case 0x20:
        REG_VRAM_CNT_F = 0x81;
        break;
    case 0x40:
        REG_VRAM_CNT_G = 0x81;
        break;
    default:
        break;
    }

    switch (abcd) {
    case 0x08:
        REG_VRAM_CNT_D = 0x89;
        break;
    case 0x0c:
        REG_VRAM_CNT_D = 0x91;
    case 0x04:
        REG_VRAM_CNT_C = 0x89;
        break;
    case 0x0e:
        REG_VRAM_CNT_D = 0x99;
    case 0x06:
        REG_VRAM_CNT_C = 0x91;
    case 0x02:
        REG_VRAM_CNT_B = 0x89;
        break;
    case 0x07:
        REG_VRAM_CNT_C = 0x99;
    case 0x03:
        REG_VRAM_CNT_B = 0x91;
    case 0x01:
        REG_VRAM_CNT_A = 0x89;
        break;
    case 0x0b:
        REG_VRAM_CNT_A = 0x89;
        REG_VRAM_CNT_B = 0x91;
        REG_VRAM_CNT_D = 0x99;
        break;
    case 0x0d:
        REG_VRAM_CNT_D = 0x99;
    case 0x05:
        REG_VRAM_CNT_A = 0x89;
        REG_VRAM_CNT_C = 0x91;
        break;
    case 0x09:
        REG_VRAM_CNT_A = 0x89;
        REG_VRAM_CNT_D = 0x91;
        break;
    case 0x0a:
        REG_VRAM_CNT_B = 0x89;
        REG_VRAM_CNT_D = 0x91;
        break;
    default:
        break;
    }
    SetLCDC(data_0210ce5c.lcdc);
}

void func_02063f50(s32 efg, s32 abcd) {
    func_02063d08(efg, abcd);
}

void GX_SetBankForObj(s32 obj) {
    data_0210ce5c.lcdc = ~obj & (data_0210ce5c.lcdc | data_0210ce5c.obj);
    data_0210ce5c.obj  = obj;

    switch (obj) {
    case 0x03:
        REG_VRAM_CNT_B = 0x8a;
    case 0x01:
        REG_VRAM_CNT_A = 0x82;
    case 0x00:
        break;
    case 0x02:
        REG_VRAM_CNT_B = 0x82;
        break;
    case 0x70:
        REG_VRAM_CNT_G = 0x9a;
    case 0x30:
        REG_VRAM_CNT_F = 0x92;
    case 0x10:
        REG_VRAM_CNT_E = 0x82;
        break;
    case 0x50:
        REG_VRAM_CNT_G = 0x92;
        REG_VRAM_CNT_E = 0x82;
        break;
    case 0x60:
        REG_VRAM_CNT_G = 0x8a;
    case 0x20:
        REG_VRAM_CNT_F = 0x82;
        break;
    case 0x40:
        REG_VRAM_CNT_G = 0x82;
        break;
    default:
        break;
    }
    SetLCDC(data_0210ce5c.lcdc);
}

void GX_SetBankForBgExtPltt(s32 bgExtPltt) {
    data_0210ce5c.lcdc      = ~bgExtPltt & (data_0210ce5c.lcdc | data_0210ce5c.bgExtPltt);
    data_0210ce5c.bgExtPltt = bgExtPltt;

    switch (bgExtPltt) {
    case 0x10:
        REG_DISPCNT |= 0x40000000;
        REG_VRAM_CNT_E = 0x84;
        break;
    case 0x40:
        REG_DISPCNT |= 0x40000000;
        REG_VRAM_CNT_G = 0x8c;
        break;
    case 0x60:
        REG_VRAM_CNT_G = 0x8c;
    case 0x20:
        REG_VRAM_CNT_F = 0x84;
        REG_DISPCNT |= 0x40000000;
        break;
    case 0x00:
        REG_DISPCNT &= ~0x40000000;
        break;
    default:
        break;
    }
    SetLCDC(data_0210ce5c.lcdc);
}

void GX_SetBankForObjExtPltt(s32 objExtPltt) {
    data_0210ce5c.lcdc       = ~objExtPltt & (data_0210ce5c.lcdc | data_0210ce5c.objExtPltt);
    data_0210ce5c.objExtPltt = objExtPltt;

    switch (objExtPltt) {
    case 0x20:
        REG_DISPCNT |= 0x80000000;
        REG_VRAM_CNT_F = 0x85;
        break;
    case 0x40:
        REG_DISPCNT |= 0x80000000;
        REG_VRAM_CNT_G = 0x85;
        break;
    case 0x00:
        REG_DISPCNT &= ~0x80000000;
        break;
    default:
        break;
    }
    SetLCDC(data_0210ce5c.lcdc);
}

void GX_SetBankForTex(s32 tex) {
    data_0210ce5c.lcdc = ~tex & (data_0210ce5c.lcdc | data_0210ce5c.tex);
    data_0210ce5c.tex  = tex;

    if (tex == 0) {
        REG_DISP3DCNT &= (u16)~(1 | 0x1000 | 0x2000);
    } else {
        REG_DISP3DCNT = (u16)((REG_DISP3DCNT & ~(0x1000 | 0x2000)) | 1);

        switch (tex) {
        case 0x05:
            REG_VRAM_CNT_A = 0x83;
            REG_VRAM_CNT_C = 0x8b;
            break;
        case 0x09:
            REG_VRAM_CNT_A = 0x83;
            REG_VRAM_CNT_D = 0x8b;
            break;
        case 0x0a:
            REG_VRAM_CNT_B = 0x83;
            REG_VRAM_CNT_D = 0x8b;
            break;
        case 0x0b:
            REG_VRAM_CNT_A = 0x83;
            REG_VRAM_CNT_B = 0x8b;
            REG_VRAM_CNT_D = 0x93;
            break;
        case 0x0d:
            REG_VRAM_CNT_A = 0x83;
            REG_VRAM_CNT_C = 0x8b;
            REG_VRAM_CNT_D = 0x93;
            break;
        case 0x08:
            REG_VRAM_CNT_D = 0x83;
            break;
        case 0x0c:
            REG_VRAM_CNT_D = 0x8b;
        case 0x04:
            REG_VRAM_CNT_C = 0x83;
            break;
        case 0x0e:
            REG_VRAM_CNT_D = 0x93;
        case 0x06:
            REG_VRAM_CNT_C = 0x8b;
        case 0x02:
            REG_VRAM_CNT_B = 0x83;
            break;
        case 0x0f:
            REG_VRAM_CNT_D = 0x9b;
        case 0x07:
            REG_VRAM_CNT_C = 0x93;
        case 0x03:
            REG_VRAM_CNT_B = 0x8b;
        case 0x01:
            REG_VRAM_CNT_A = 0x83;
            break;
        default:
            break;
        }
    }
    SetLCDC(data_0210ce5c.lcdc);
}

void GX_SetBankForTexPltt(s32 texPltt) {
    data_0210ce5c.lcdc    = ~texPltt & (data_0210ce5c.lcdc | data_0210ce5c.texPltt);
    data_0210ce5c.texPltt = texPltt;

    switch (texPltt) {
    case 0x60:
        REG_VRAM_CNT_G = 0x8b;
    case 0x20:
        REG_VRAM_CNT_F = 0x83;
        break;
    case 0x40:
        REG_VRAM_CNT_G = 0x83;
        break;
    case 0x70:
        REG_VRAM_CNT_G = 0x9b;
    case 0x30:
        REG_VRAM_CNT_F = 0x93;
    case 0x10:
        REG_VRAM_CNT_E = 0x83;
        break;
    case 0x00:
    default:
        break;
    }
    SetLCDC(data_0210ce5c.lcdc);
}

void GX_SetBankForClearImage(s32 clearImage) {
    data_0210ce5c.lcdc       = ~clearImage & (data_0210ce5c.lcdc | data_0210ce5c.clearImage);
    data_0210ce5c.clearImage = clearImage;

    switch (clearImage) {
    case 0x03:
        REG_VRAM_CNT_A = 0x93;
    case 0x02:
        REG_VRAM_CNT_B = 0x9b;
        REG_DISP3DCNT |= 0x4000;
        break;
    case 0x0c:
        REG_VRAM_CNT_C = 0x93;
    case 0x08:
        REG_VRAM_CNT_D = 0x9b;
        REG_DISP3DCNT |= 0x4000;
        break;
    case 0x00:
        REG_DISP3DCNT &= ~0x4000;
        break;
    case 0x01:
        REG_VRAM_CNT_A = 0x9b;
        REG_DISP3DCNT |= 0x4000;
        break;
    case 0x04:
        REG_VRAM_CNT_C = 0x9b;
        REG_DISP3DCNT |= 0x4000;
        break;
    default:
        break;
    }
    SetLCDC(data_0210ce5c.lcdc);
}

void GX_SetBankForLcdc(s32 lcdc) {
    data_0210ce5c.lcdc |= lcdc;
    SetLCDC(lcdc);
}

void GX_SetBankForSubBg(s32 subBg) {
    data_0210ce5c.lcdc  = ~subBg & (data_0210ce5c.lcdc | data_0210ce5c.subBg);
    data_0210ce5c.subBg = subBg;

    switch (subBg) {
    case 0x004:
        REG_VRAM_CNT_C = 0x84;
        break;
    case 0x180:
        REG_VRAM_CNT_I = 0x81;
    case 0x080:
        REG_VRAM_CNT_H = 0x81;
        break;
    case 0x000:
    default:
        break;
    }
    SetLCDC(data_0210ce5c.lcdc);
}

void GX_SetBankForSubObj(s32 subObj) {
    data_0210ce5c.lcdc   = ~subObj & (data_0210ce5c.lcdc | data_0210ce5c.subObj);
    data_0210ce5c.subObj = subObj;

    switch (subObj) {
    case 0x008:
        REG_VRAM_CNT_D = 0x84;
        break;
    case 0x100:
        REG_VRAM_CNT_I = 0x82;
        break;
    case 0x000:
    default:
        break;
    }
    SetLCDC(data_0210ce5c.lcdc);
}

void GX_SetBankForSubBgExtPltt(s32 subBgExtPltt) {
    data_0210ce5c.lcdc         = ~subBgExtPltt & (data_0210ce5c.lcdc | data_0210ce5c.subBgExtPltt);
    data_0210ce5c.subBgExtPltt = subBgExtPltt;

    switch (subBgExtPltt) {
    case 0x080:
        REG_DISPCNT_SUB |= 0x40000000;
        REG_VRAM_CNT_H = 0x82;
        break;
    case 0x000:
        REG_DISPCNT_SUB &= ~0x40000000;
        break;
    }
    SetLCDC(data_0210ce5c.lcdc);
}

void GX_SetBankForSubObjExtPltt(s32 subObjExtPltt) {
    data_0210ce5c.lcdc          = ~subObjExtPltt & (data_0210ce5c.lcdc | data_0210ce5c.subObjExtPltt);
    data_0210ce5c.subObjExtPltt = subObjExtPltt;

    switch (subObjExtPltt) {
    case 0x100:
        REG_DISPCNT_SUB |= 0x80000000;
        REG_VRAM_CNT_I = 0x83;
        break;
    case 0x000:
        REG_DISPCNT_SUB &= ~0x80000000;
        break;
    }
    SetLCDC(data_0210ce5c.lcdc);
}

s32 ResetBank(u16 *bank) {
    s32 vram = *bank;
    *bank = 0;
    data_0210ce5c.lcdc |= vram;
    SetLCDC(vram);
    return vram;
}

s32 func_020648b8(void) {
    return ResetBank(&data_0210ce5c.bg);
}

s32 func_020648cc(void) {
    return ResetBank(&data_0210ce5c.obj);
}

s32 GX_ResetBankForBgExtPltt(void) {
    REG_DISPCNT &= ~0x40000000;
    return ResetBank(&data_0210ce5c.bgExtPltt);
}

s32 GX_ResetBankForOBJExtPltt(void) {
    REG_DISPCNT &= ~0x80000000;
    return ResetBank(&data_0210ce5c.objExtPltt);
}

s32 GX_ResetBankForTex(void) {
    return ResetBank(&data_0210ce5c.tex);
}

s32 func_0206493c(void) {
    return ResetBank(&data_0210ce5c.texPltt);
}

s32 func_02064950(void) {
    return ResetBank(&data_0210ce5c.clearImage);
}

s32 GX_ResetBankForSubBg(void) {
    return ResetBank(&data_0210ce5c.subBg);
}

s32 GX_ResetBankForSubObj(void) {
    return ResetBank(&data_0210ce5c.subObj);
}

s32 GX_ResetBankForSubBgExtPltt(void) {
    REG_DISPCNT_SUB &= ~0x40000000;
    return ResetBank(&data_0210ce5c.subBgExtPltt);
}

s32 GX_ResetBankForSubObjExtPltt(void) {
    REG_DISPCNT_SUB &= ~0x80000000;
    return ResetBank(&data_0210ce5c.subObjExtPltt);
}

s32 DisableBank(u16 *bank) {
    s32 vram = *bank;
    *bank = 0;

    if (vram & 0x001) REG_VRAM_CNT_A = 0;
    if (vram & 0x002) REG_VRAM_CNT_B = 0;
    if (vram & 0x004) REG_VRAM_CNT_C = 0;
    if (vram & 0x008) REG_VRAM_CNT_D = 0;
    if (vram & 0x010) REG_VRAM_CNT_E = 0;
    if (vram & 0x020) REG_VRAM_CNT_F = 0;
    if (vram & 0x040) REG_VRAM_CNT_G = 0;
    if (vram & 0x080) REG_VRAM_CNT_H = 0;
    if (vram & 0x100) REG_VRAM_CNT_I = 0;
    func_02079f00((u16)vram, data_0210ce5a);
    return vram;
}

s32 func_02064abc(void) {
    return DisableBank(&data_0210ce5c.lcdc);
}
