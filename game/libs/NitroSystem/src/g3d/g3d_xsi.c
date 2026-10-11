#include "g3d_f.h"


void func_02071dac(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    fx32 w = anm->origW << 12;
    fx32 h = anm->origH << 12;
    fx32 ss;
    fx32 st;
    fx32 sinR;
    fx32 cosR;
    fx32 stCos;
    fx32 ssSin;
    s64 a;
    s64 b;
    fx32 tS;
    fx32 tT;

    FX_DivAsync(h, w);
    cosR = anm->cosR;
    ss = anm->scaleS;
    sinR = anm->sinR;
    st = anm->scaleT;
    ssSin = UNK_MUL64(ss, sinR);
    stCos = UNK_MUL64(st, cosR);
    m->m[0][0] = UNK_MUL64(ss, cosR);
    m->m[1][1] = stCos;
    m->m[0][1] = (UNK_MUL64(st, sinR) * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    b = ((s64)anm->transS * anm->sinR - (s64)anm->transT * anm->cosR) >> 12;
    a = ((s64)anm->transS * anm->cosR + (s64)anm->transT * anm->sinR) >> 12;
    tT = stCos + (fx32)((b * anm->scaleT) >> 12) - 0x1000;
    tS = ssSin - (fx32)((a * anm->scaleS) >> 12);
    m->m[3][0] = (anm->origW * tS) << 4;
    m->m[3][1] = (-anm->origH * tT) << 4;
    m->m[1][0] = (-ssSin * FX_GetDivResult()) >> 12;
}

void func_02071f00(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    fx32 w = anm->origW << 12;
    fx32 h = anm->origH << 12;
    fx32 a;
    fx32 b;

    FX_DivAsync(h, w);
    m->m[0][0] = anm->cosR;
    m->m[1][1] = anm->cosR;
    m->m[0][1] = (anm->sinR * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    a = (fx32)(((s64)anm->transS * anm->cosR + (s64)anm->transT * anm->sinR) >> 12);
    b = (fx32)(((s64)anm->transS * anm->sinR - (s64)anm->transT * anm->cosR) >> 12);
    m->m[3][0] = (anm->origW * (anm->sinR - a)) << 4;
    m->m[3][1] = (-anm->origH * (anm->cosR + b - 0x1000)) << 4;
    m->m[1][0] = (-anm->sinR * FX_GetDivResult()) >> 12;
}

void func_02071fe0(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    fx32 a;
    fx32 b;

    m->m[0][0] = anm->scaleS;
    m->m[1][1] = anm->scaleT;
    m->m[0][1] = 0;
    a = UNK_MUL64(anm->transS, anm->scaleS);
    b = UNK_MUL64(-anm->transT, anm->scaleT);
    m->m[3][0] = (anm->origW * -a) << 4;
    m->m[3][1] = (-anm->origH * (anm->scaleT + b - 0x1000)) << 4;
    m->m[1][0] = 0;
}

void func_02072064(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    fx32 tS;
    fx32 tT;

    m->m[0][0] = 0x1000;
    m->m[1][1] = 0x1000;
    m->m[0][1] = 0;
    tS = -anm->transS;
    tT = -anm->transT;
    m->m[3][0] = (anm->origW * tS) << 4;
    m->m[3][1] = (-anm->origH * tT) << 4;
    m->m[1][0] = 0;
}

void func_020720b8(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    fx32 w = anm->origW << 12;
    fx32 h = anm->origH << 12;
    fx32 ss;
    fx32 st;
    fx32 sinR;
    fx32 cosR;
    fx32 stCos;
    fx32 ssSin;

    FX_DivAsync(h, w);
    cosR = anm->cosR;
    ss = anm->scaleS;
    st = anm->scaleT;
    sinR = anm->sinR;
    ssSin = UNK_MUL64(ss, sinR);
    stCos = UNK_MUL64(st, cosR);
    m->m[0][0] = UNK_MUL64(ss, cosR);
    m->m[1][1] = stCos;
    m->m[0][1] = (UNK_MUL64(st, sinR) * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    m->m[3][0] = (anm->origW * ssSin) << 4;
    m->m[3][1] = (-anm->origH * (stCos - 0x1000)) << 4;
    m->m[1][0] = (-ssSin * FX_GetDivResult()) >> 12;
}

void func_02072184(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    fx32 w = anm->origW << 12;
    fx32 h = anm->origH << 12;

    FX_DivAsync(h, w);
    m->m[0][0] = anm->cosR;
    m->m[1][1] = anm->cosR;
    m->m[0][1] = (anm->sinR * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    m->m[3][0] = (anm->origW * anm->sinR) << 4;
    m->m[3][1] = (-anm->origH * (anm->cosR - 0x1000)) << 4;
    m->m[1][0] = (-anm->sinR * FX_GetDivResult()) >> 12;
}

void func_02072228(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    m->m[0][0] = anm->scaleS;
    m->m[1][1] = anm->scaleT;
    m->m[0][1] = 0;
    m->m[3][0] = 0;
    m->m[3][1] = (-anm->origH * (anm->scaleT - 0x1000)) << 4;
    m->m[1][0] = 0;
}

void func_02072268(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    m->m[0][0] = 0x1000;
    m->m[0][1] = 0;
    m->m[1][0] = 0;
    m->m[1][1] = 0x1000;
    m->m[3][0] = 0;
    m->m[3][1] = 0;
}

/* texture matrix calculation per texture SRT flag combination */
UnkCalcTexMtxFunc data_020c401c[8] = {
    func_02071dac, func_02071f00, func_02071fe0, func_02072064,
    func_020720b8, func_02072184, func_02072228, func_02072268,
};

/* sends the texture matrix (XSI) */
void func_0207228c(UnkMatAnmResult* anm)
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

    if (anm->flag & 1) {
        anm->scaleT = 0x1000;
        anm->scaleS = 0x1000;
    }
    if (anm->flag & 2) {
        anm->cosR = 0x1000;
        anm->sinR = 0;
    }
    if (anm->flag & 4) {
        anm->transT = 0;
        anm->transS = 0;
    }

    data_020c401c[anm->flag & 7](&buf.m, anm);

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
