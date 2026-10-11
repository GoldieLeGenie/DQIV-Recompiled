#include "g3d_f.h"


/* sends joint SRT (Maya segment scale compensation) */
void func_02070a5c(const UnkJntAnmResult* result)
{
    BOOL trFlag = FALSE;

    if (!(result->flag & 4)) {
        trFlag = TRUE;
    }

    if ((result->flag & 0x20) && !(result->flag & 8)) {
        if (trFlag) {
            func_0206de34(0x1c, &result->trans, 3);
            trFlag = FALSE;
        }
        func_0206de34(0x1b, &result->scaleEx0, 3);
    }

    if (!(result->flag & 2)) {
        if (trFlag) {
            func_0206de34(0x19, &result->rot, 12);
        } else {
            func_0206de34(0x1a, &result->rot, 9);
        }
    } else {
        if (trFlag) {
            func_0206de34(0x1c, &result->trans, 3);
        }
    }

    if (!(result->flag & 1)) {
        func_0206de34(0x1b, &result->scale, 3);
    }
}

/* joint scale (Maya segment scale compensation) */
void func_02070b20(UnkJntAnmResult* result, const UnkVecFx32* p, const u8* data, u32 srtflag)
{
    u32 flag = data[3];

    if (srtflag & 4) {
        result->flag |= 1;
        if (flag & 2) {
            u32 nodeID = data[1];
            data_0210d18c->isScaleCacheOne[nodeID >> 5] |= 1 << (nodeID & 31);
        }
    } else {
        result->scale.x = p[0].x;
        result->scale.y = p[0].y;
        result->scale.z = p[0].z;
        if (flag & 2) {
            u32 nodeID = data[1];
            data_0210d18c->isScaleCacheOne[nodeID >> 5] &= ~(1 << (nodeID & 31));
            data_0210d190.scaleCache[nodeID].inv.x = p[1].x;
            data_0210d190.scaleCache[nodeID].inv.y = p[1].y;
            data_0210d190.scaleCache[nodeID].inv.z = p[1].z;
        }
    }

    if (flag & 1) {
        u32 parentID = data[2];
        result->flag |= 0x20;
        if (data_0210d18c->isScaleCacheOne[parentID >> 5] & (1 << (parentID & 31))) {
            result->flag |= 8;
        } else {
            result->scaleEx0 = data_0210d190.scaleCache[parentID].inv;
        }
    }
    result->flag |= 0x10;
}

void func_02070c70(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    fx32 w = anm->origW << 12;
    fx32 h = anm->origH << 12;
    fx32 ssCos;
    fx32 ssSin;
    fx32 stSin;
    fx32 stCos;

    FX_DivAsync(h, w);
    ssSin = UNK_MUL64(anm->scaleS, anm->sinR);
    ssCos = UNK_MUL64(anm->scaleS, anm->cosR);
    stSin = UNK_MUL64(anm->scaleT, anm->sinR);
    stCos = UNK_MUL64(anm->scaleT, anm->cosR);
    m->m[0][0] = ssCos;
    m->m[1][1] = stCos;
    m->m[0][1] = (-stSin * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    m->m[3][0] = ((anm->origW * (anm->scaleS - (ssSin + ssCos))) << 3) - anm->origW * UNK_MUL64_8(anm->scaleS, anm->transS);
    m->m[3][1] = ((anm->origH * ((stSin - stCos) - anm->scaleT + 0x2000)) << 3) + anm->origH * UNK_MUL64_8(anm->scaleT, anm->transT);
    m->m[1][0] = (ssSin * FX_GetDivResult()) >> 12;
}

void func_02070d78(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    fx32 w = anm->origW << 12;
    fx32 h = anm->origH << 12;

    FX_DivAsync(h, w);
    m->m[0][0] = anm->cosR;
    m->m[1][1] = anm->cosR;
    m->m[0][1] = (-anm->sinR * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    m->m[3][0] = ((anm->origW * (-(anm->sinR + anm->cosR) + 4096L)) << 3) - ((anm->transS * anm->origW) << 4);
    m->m[3][1] = ((anm->origH * (anm->sinR - anm->cosR + 0x1000)) << 3) + ((anm->transT * anm->origH) << 4);
    m->m[1][0] = (anm->sinR * FX_GetDivResult()) >> 12;
}

void func_02070e48(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    m->m[0][0] = anm->scaleS;
    m->m[1][1] = anm->scaleT;
    m->m[0][1] = 0;
    m->m[3][0] = anm->origW * -UNK_MUL64_8(anm->scaleS, anm->transS);
    m->m[3][1] = anm->origH * UNK_MUL64_8(anm->scaleT, anm->transT) + ((anm->origH * (-anm->scaleT - anm->scaleT + 0x2000)) << 3);
    m->m[1][0] = 0;
}

void func_02070ec4(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    m->m[0][0] = 0x1000;
    m->m[1][1] = 0x1000;
    m->m[0][1] = 0;
    m->m[3][0] = -(anm->transS * anm->origW) << 4;
    m->m[3][1] = (anm->transT * anm->origH) << 4;
    m->m[1][0] = 0;
}

void func_02070f0c(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    fx32 w = anm->origW << 12;
    fx32 h = anm->origH << 12;
    fx32 ssCos;
    fx32 ssSin;
    fx32 stSin;
    fx32 stCos;

    FX_DivAsync(h, w);
    ssSin = UNK_MUL64(anm->scaleS, anm->sinR);
    ssCos = UNK_MUL64(anm->scaleS, anm->cosR);
    stSin = UNK_MUL64(anm->scaleT, anm->sinR);
    stCos = UNK_MUL64(anm->scaleT, anm->cosR);
    m->m[0][0] = ssCos;
    m->m[1][1] = stCos;
    m->m[0][1] = (-stSin * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    m->m[3][0] = (anm->origW * (anm->scaleS - (ssSin + ssCos))) << 3;
    m->m[3][1] = (anm->origH * ((stSin - stCos) - anm->scaleT + 0x2000)) << 3;
    m->m[1][0] = (ssSin * FX_GetDivResult()) >> 12;
}

void func_02070fec(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    fx32 w = anm->origW << 12;
    fx32 h = anm->origH << 12;

    FX_DivAsync(h, w);
    m->m[0][0] = anm->cosR;
    m->m[1][1] = anm->cosR;
    m->m[0][1] = (-anm->sinR * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    m->m[3][0] = (anm->origW * (-(anm->sinR + anm->cosR) + 4096L)) << 3;
    m->m[3][1] = (anm->origH * (anm->sinR - anm->cosR + 0x1000)) << 3;
    m->m[1][0] = (anm->sinR * FX_GetDivResult()) >> 12;
}

void func_020710a4(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    m->m[0][0] = anm->scaleS;
    m->m[1][1] = anm->scaleT;
    m->m[0][1] = 0;
    m->m[3][0] = 0;
    m->m[3][1] = (anm->origH * (-anm->scaleT - anm->scaleT + 0x2000)) << 3;
    m->m[1][0] = 0;
}

void func_020710e8(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    m->m[0][0] = 0x1000;
    m->m[0][1] = 0;
    m->m[1][0] = 0;
    m->m[1][1] = 0x1000;
    m->m[3][0] = 0;
    m->m[3][1] = 0;
}

/* texture matrix calculation per texture SRT flag combination */
UnkCalcTexMtxFunc data_020c3fdc[8] = {
    func_02070c70, func_02070d78, func_02070e48, func_02070ec4,
    func_02070f0c, func_02070fec, func_020710a4, func_020710e8,
};

/* sends the texture matrix (Maya) */
void func_0207110c(const UnkMatAnmResult* anm)
{
    UnkTexMtxCmd buf;

    if (anm->flag & 8) {
        buf.cmd = 0x101610;
    } else {
        buf.cmd = 0x101810;
    }
    buf.mtxMode = 3;
    buf.mtxMode2 = 2;
    buf.m.m[3][2] = 0;
    buf.m.m[2][3] = 0;
    buf.m.m[2][2] = 0;
    buf.m.m[2][1] = 0;
    buf.m.m[2][0] = 0;
    buf.m.m[1][3] = 0;
    buf.m.m[1][2] = 0;
    buf.m.m[0][3] = 0;
    buf.m.m[0][2] = 0;
    buf.m.m[3][3] = 0x1000;

    data_020c3fdc[anm->flag & 7](&buf.m, anm);

    if (anm->magW != 0x1000) {
        buf.m.m[0][0] = UNK_MUL64(anm->magW, buf.m.m[0][0]);
        buf.m.m[0][1] = UNK_MUL64(anm->magW, buf.m.m[0][1]);
        buf.m.m[3][0] = UNK_MUL64(anm->magW, buf.m.m[3][0]);
    }
    if (anm->magH != 0x1000) {
        buf.m.m[1][0] = UNK_MUL64(anm->magH, buf.m.m[1][0]);
        buf.m.m[1][1] = UNK_MUL64(anm->magH, buf.m.m[1][1]);
        buf.m.m[3][1] = UNK_MUL64(anm->magH, buf.m.m[3][1]);
    }

    func_0206de34(buf.cmd, (u32*)&buf + 1, 0x12);
}
