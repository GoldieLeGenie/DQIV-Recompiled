#include "g3d_f.h"


void func_02071768(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    fx32 w = anm->origW << 12;
    fx32 h = anm->origH << 12;
    fx32 ssCos;
    fx32 ssSin;
    fx32 stCos;
    fx32 stSin;
    fx32 tS;
    fx32 tT;

    FX_DivAsync(h, w);
    ssCos = UNK_MUL64(anm->scaleS, anm->cosR);
    ssSin = UNK_MUL64(anm->scaleS, anm->sinR);
    stCos = UNK_MUL64(anm->scaleT, anm->cosR);
    stSin = UNK_MUL64(anm->scaleT, anm->sinR);
    m->m[0][0] = ssCos;
    m->m[1][1] = stCos;
    m->m[0][1] = (stSin * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    tS = (-anm->origW << 11) - anm->transS * anm->origW;
    tT = anm->transT * anm->origH + (-anm->origH << 11);
    m->m[3][0] = (fx32)(((s64)ssCos * tS - (s64)ssSin * tT) >> 8) + (anm->origW << 15);
    m->m[3][1] = (fx32)(((s64)stSin * tS + (s64)stCos * tT) >> 8) + (anm->origH << 15);
    m->m[1][0] = (-ssSin * FX_GetDivResult()) >> 12;
}

void func_02071870(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    fx32 w = anm->origW << 12;
    fx32 h = anm->origH << 12;
    fx32 tS;
    fx32 tT;

    FX_DivAsync(h, w);
    m->m[0][0] = anm->cosR;
    m->m[1][1] = anm->cosR;
    m->m[0][1] = (anm->sinR * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    tS = (-anm->origW << 11) - anm->transS * anm->origW;
    tT = anm->transT * anm->origH + (-anm->origH << 11);
    m->m[3][0] = (fx32)(((s64)anm->cosR * tS - (s64)anm->sinR * tT) >> 8) + (anm->origW << 15);
    m->m[3][1] = (fx32)(((s64)anm->sinR * tS + (s64)anm->cosR * tT) >> 8) + (anm->origH << 15);
    m->m[1][0] = (-anm->sinR * FX_GetDivResult()) >> 12;
}

void func_02071958(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    fx32 tS;
    fx32 tT;

    m->m[0][0] = anm->scaleS;
    m->m[1][1] = anm->scaleT;
    m->m[0][1] = 0;
    tS = (-anm->origW << 11) - anm->transS * anm->origW;
    tT = anm->transT * anm->origH + (-anm->origH << 11);
    m->m[3][0] = UNK_MUL64_8(anm->scaleS, tS) + (anm->origW << 15);
    m->m[3][1] = UNK_MUL64_8(anm->scaleT, tT) + (anm->origH << 15);
    m->m[1][0] = 0;
}

void func_020719d8(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    m->m[0][0] = 0x1000;
    m->m[1][1] = 0x1000;
    m->m[0][1] = 0;
    m->m[3][0] = (-anm->transS * anm->origW) << 4;
    m->m[3][1] = (anm->transT * anm->origH) << 4;
    m->m[1][0] = 0;
}

void func_02071a20(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    fx32 w = anm->origW << 12;
    fx32 h = anm->origH << 12;
    fx32 ssCos;
    fx32 ssSin;
    fx32 stCos;
    fx32 stSin;
    fx32 tS;
    fx32 tT;

    FX_DivAsync(h, w);
    ssCos = UNK_MUL64(anm->scaleS, anm->cosR);
    ssSin = UNK_MUL64(anm->scaleS, anm->sinR);
    stCos = UNK_MUL64(anm->scaleT, anm->cosR);
    stSin = UNK_MUL64(anm->scaleT, anm->sinR);
    m->m[0][0] = ssCos;
    m->m[1][1] = stCos;
    m->m[0][1] = (stSin * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    tS = -anm->origW << 11;
    tT = -anm->origH << 11;
    m->m[3][0] = (fx32)(((s64)ssCos * tS - (s64)ssSin * tT) >> 8) + (anm->origW << 15);
    m->m[3][1] = (fx32)(((s64)stSin * tS + (s64)stCos * tT) >> 8) + (anm->origH << 15);
    m->m[1][0] = (-ssSin * FX_GetDivResult()) >> 12;
}

void func_02071b18(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    fx32 w = anm->origW << 12;
    fx32 h = anm->origH << 12;
    fx32 tS;
    fx32 tT;

    FX_DivAsync(h, w);
    m->m[0][0] = anm->cosR;
    m->m[1][1] = anm->cosR;
    m->m[0][1] = (anm->sinR * FX_GetDivResult()) >> 12;
    FX_DivAsync(w, h);
    tS = -anm->origW << 11;
    tT = -anm->origH << 11;
    m->m[3][0] = (fx32)(((s64)anm->cosR * tS - (s64)anm->sinR * tT) >> 8) + (anm->origW << 15);
    m->m[3][1] = (fx32)(((s64)anm->sinR * tS + (s64)anm->cosR * tT) >> 8) + (anm->origH << 15);
    m->m[1][0] = (-anm->sinR * FX_GetDivResult()) >> 12;
}

void func_02071bf0(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    m->m[0][0] = anm->scaleS;
    m->m[1][1] = anm->scaleT;
    m->m[0][1] = 0;
    m->m[3][0] = ((0x1000 - anm->scaleS) * anm->origW) << 3;
    m->m[3][1] = ((0x1000 - anm->scaleT) * anm->origH) << 3;
    m->m[1][0] = 0;
}

void func_02071c40(UnkMtxFx44* m, const UnkMatAnmResult* anm)
{
    m->m[0][0] = 0x1000;
    m->m[0][1] = 0;
    m->m[1][0] = 0;
    m->m[1][1] = 0x1000;
    m->m[3][0] = 0;
    m->m[3][1] = 0;
}

/* texture matrix calculation per texture SRT flag combination */
UnkCalcTexMtxFunc data_020c3ffc[8] = {
    func_02071768, func_02071870, func_02071958, func_020719d8,
    func_02071a20, func_02071b18, func_02071bf0, func_02071c40,
};

/* sends the texture matrix (3ds Max) */
void func_02071c64(const UnkMatAnmResult* anm)
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

    data_020c3ffc[anm->flag & 7](&buf.m, anm);

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
