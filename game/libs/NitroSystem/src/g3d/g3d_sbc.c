#include <nitro/native_g3.h>
#include "g3d_internal.h"

// Billboard command block: ops, mode params, 4x3 matrix and scale
typedef struct UnkBBCmd {
    u32 op;        // 0x00
    u32 param[2];  // 0x04
    MtxFx43 mtx;   // 0x0C
    VecFx32 scale; // 0x3C
} UnkBBCmd;

void func_0206bcc0(NNSG3dRS* rs, u32 opt, const NNSG3dResMatData* mat, u32 idxMat);
void func_0206c198(NNSG3dRS* rs, u32 opt, const NNSG3dResShpData* shp, u32 idxShp);

// TEXIMAGE_PARAM (0x2a) command + parameter sent by the projection map / environment map commands
static u32 data_020c3e5c[2] = {0x2a, 0};
static u32 data_020c3e64[2] = {0x2a, 0};
// material / shape drawing functions indexed by item tag
UnkMatFunc data_020c3e6c[4] = {func_0206bcc0};
UnkShpFunc data_020c3e7c[4] = {func_0206c198};
// projection map texture matrix
static MtxFx44 data_020c3e8c = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x10000, 0, 0, 0, 0, 0x10000};
// billboard command blocks
static UnkBBCmd data_020c3ecc = {0x1b171012, {1, 2}, {0x1000, 0, 0, 0, 0x1000, 0, 0, 0, 0x1000, 0, 0, 0}, {0, 0, 0}};
static UnkBBCmd data_020c3f14 = {0x1b171012, {1, 2}, {0x1000, 0, 0, 0, 0x1000, 0, 0, 0, 0x1000, 0, 0, 0}, {0, 0, 0}};

// mask per 3-bit material flag field value
static const u32 data_020ba5c0[8] = {0x00000000, 0x00007fff, 0x7fff0000, 0x7fff7fff,
                                     0x00008000, 0x0000ffff, 0x7fff8000, 0x7fffffff};
// for each pivot index the 4 other matrix elements set by a pivot-compressed rotation
static const u8 data_020ba5e0[9][4] = {{4, 5, 7, 8}, {3, 5, 6, 8}, {3, 4, 6, 7}, {1, 2, 7, 8}, {0, 2, 6, 8},
                                       {0, 1, 6, 7}, {1, 2, 4, 5}, {0, 2, 3, 5}, {0, 1, 3, 4}};

typedef struct UnkEvpMtx {
    MtxFx43 m;    // 0x00
    MtxFx33 invN; // 0x30
} UnkEvpMtx;


void G3d_SBCRender_NOP(NNSG3dRS* rs, u32 opt) {
    if (rs->cbVecFunc[0]) {
        rs->cbVecFunc[0](rs);
    }
    rs->c++;
}

void G3d_SBCRender_END(NNSG3dRS* rs, u32 opt) {
    if (rs->cbVecFunc[1]) {
        rs->cbVecFunc[1](rs);
    }
    rs->flag |= 0x20;
}

// NODE: visibility of a node
void func_0206ba78(NNSG3dRS* rs, u32 opt) {
    if (!(rs->flag & 0x200)) {
        u32 timing;
        BOOL cbFlag;
        u32 nodeID = rs->c[1];

        rs->currentNode = nodeID;
        rs->flag |= 4;
        rs->pVisAnmResult = &rs->tmpVisAnmResult;

        cbFlag = CallBackCheck_A(rs, 2, &timing);
        if (!cbFlag) {
            NNSG3dRenderObj* obj = rs->pRenderObj;
            if (!obj->anmVis || !(obj->hintVisAnmExist[nodeID >> 5] & (1 << (nodeID & 0x1f))) ||
                !obj->funcBlendVis(rs->pVisAnmResult, obj->anmVis, nodeID)) {
                rs->pVisAnmResult->isVisible = rs->c[2] & 1;
            }
        }

        cbFlag = CallBackCheck_B(rs, 2, &timing);
        if (!cbFlag) {
            if (rs->pVisAnmResult->isVisible) {
                rs->flag |= 1;
            } else {
                rs->flag &= ~1;
            }
        }

        CallBackCheck_C(rs, 2, timing);
    }
    rs->c += 3;
}

// MTX: restores a matrix from the matrix stack
void func_0206bbf4(NNSG3dRS* rs, u32 opt) {
    if (!(rs->flag & 0x200) && (rs->flag & 1)) {
        u32 timing;
        BOOL cbFlag;

        cbFlag = CallBackCheck_A(rs, 3, &timing);
        if (!cbFlag) {
            u32 arg = rs->c[1];
            if (!(rs->flag & 0x100)) {
                func_0206de34(0x14, &arg, 1);
            }
        }
        CallBackCheck_C(rs, 3, timing);
    }
    rs->c += 2;
}

// Default material setup
void func_0206bcc0(NNSG3dRS* rs, u32 opt, const NNSG3dResMatData* mat, u32 idxMat) {
    u32 timing;
    BOOL cbFlag;
    NNSG3dMatAnmResult* pResult;

    rs->currentMat = idxMat;
    rs->flag |= 8;
    rs->pMatAnmResult = &rs->tmpMatAnmResult;

    cbFlag = CallBackCheck_A(rs, 4, &timing);
    if (!cbFlag) {
        NNSG3dMatAnmResult* matCache = rs->pRenderObj->recMatAnm;

        if (matCache && !(rs->flag & 0x80)) {
            pResult = &matCache[idxMat];
        } else if ((opt == 0x20 || opt == 0x40) && (rs->isMatCached[idxMat >> 5] & (1 << (idxMat & 0x1f)))) {
            if (matCache) {
                pResult = &matCache[idxMat];
            } else {
                pResult = &data_0210d190.matCache[idxMat];
            }
        } else {
            NNSG3dRenderObj* obj;

            if (matCache) {
                rs->isMatCached[idxMat >> 5] |= 1 << (idxMat & 0x1f);
                pResult = &rs->pRenderObj->recMatAnm[idxMat];
            } else if (opt == 0x40) {
                rs->isMatCached[idxMat >> 5] |= 1 << (idxMat & 0x1f);
                pResult = &data_0210d190.matCache[idxMat];
            } else {
                pResult = &rs->tmpMatAnmResult;
            }

            pResult->flag = 0;
            if (GetMatDataByIdx(rs->pResMat, idxMat)->flag & 0x20) {
                pResult->flag |= 0x20;
            }

            {
                u32 mask = data_020ba5c0[(mat->flag >> 6) & 7];
                pResult->prmMatColor0 = (data_0210cf28.prmMatColor0 & ~mask) | (mat->diffAmb & mask);
            }
            {
                u32 mask = data_020ba5c0[(mat->flag >> 9) & 7];
                pResult->prmMatColor1 = (data_0210cf28.prmMatColor1 & ~mask) | (mat->specEmi & mask);
            }
            pResult->prmPolygonAttr = (data_0210cf28.prmPolygonAttr & ~mat->polyAttrMask) | (mat->polyAttr & mat->polyAttrMask);
            pResult->prmTexImage = mat->texImageParam;
            pResult->prmTexPltt = mat->texPlttBase;

            if (mat->flag & 1) {
                const fx32* p = (const fx32*)mat->texMtx;

                if (!(mat->flag & 2)) {
                    pResult->scaleS = *p++;
                    pResult->scaleT = *p++;
                } else {
                    pResult->flag |= 1;
                }

                if (!(mat->flag & 4)) {
                    pResult->rotSin = ((const fx16*)p)[0];
                    pResult->rotCos = ((const fx16*)p)[1];
                    p++;
                } else {
                    pResult->flag |= 2;
                }

                if (!(mat->flag & 8)) {
                    pResult->transS = p[0];
                    pResult->transT = p[1];
                } else {
                    pResult->flag |= 4;
                }
                pResult->flag |= 8;
            }

            obj = rs->pRenderObj;
            if (obj->anmMat && (obj->hintMatAnmExist[idxMat >> 5] & (1 << (idxMat & 0x1f)))) {
                obj->funcBlendMat(pResult, obj->anmMat, idxMat);
            }

            if (pResult->flag & 0x18) {
                pResult->origWidth = mat->origWidth;
                pResult->origHeight = mat->origHeight;
                pResult->magW = mat->magW;
                pResult->magH = mat->magH;
            }
        }
        rs->pMatAnmResult = pResult;
    }

    cbFlag = CallBackCheck_B(rs, 4, &timing);
    if (!cbFlag) {
        NNSG3dMatAnmResult* r = rs->pMatAnmResult;
        if (r->prmPolygonAttr & 0x1f0000) {
            if (r->flag & 0x20) {
                r->prmPolygonAttr &= ~0x1f0000;
            }
            rs->flag &= ~2;
            if (!(rs->flag & 0x100)) {
                u32 cmd[7];
                cmd[0] = 0x293130;
                cmd[1] = r->prmMatColor0;
                cmd[2] = r->prmMatColor1;
                cmd[3] = r->prmPolygonAttr;
                cmd[4] = 0x2b2a;
                cmd[5] = r->prmTexImage;
                cmd[6] = r->prmTexPltt;
                func_0206de34(cmd[0], &cmd[1], 6);
                if (r->flag & 0x18) {
                    rs->funcTexMtx(r);
                }
            }
        } else {
            rs->flag |= 2;
        }
    }
    CallBackCheck_C(rs, 4, timing);
}

// MAT: material command
void func_0206c118(NNSG3dRS* rs, u32 opt) {
    if (!(rs->flag & 0x200)) {
        u32 matID = rs->c[1];
        if ((rs->flag & 1) || !(rs->flag & 8) || matID != rs->currentMat) {
            NNSG3dResMatData* mat = GetMatDataByIdx(rs->pResMat, matID);
            data_020c3e6c[mat->itemTag](rs, opt, mat, matID);
        }
    }
    rs->c += 2;
}

// Default shape drawing
void func_0206c198(NNSG3dRS* rs, u32 opt, const NNSG3dResShpData* shp, u32 idxShp) {
    u32 timing;
    BOOL cbFlag;

    cbFlag = CallBackCheck_A(rs, 5, &timing);
    if (!cbFlag && !(rs->flag & 0x100)) {
        func_0206dd80((const u32*)((u8*)shp + shp->ofsDL), shp->sizeDL);
    }
    CallBackCheck_B(rs, 5, &timing);
    CallBackCheck_C(rs, 5, timing);
}

// SHP: shape command
void func_0206c26c(NNSG3dRS* rs, u32 opt) {
    if (!(rs->flag & 0x200) && (rs->flag & 1) && !(rs->flag & 2)) {
        u32 shpID = rs->c[1];
        NNSG3dResShpData* shp = GetShpDataByIdx(rs->pResShp, shpID);
        data_020c3e7c[shp->itemTag](rs, opt, shp, shpID);
    }
    rs->c += 2;
}

// NODEDESC: calculates the matrix of a node
void func_0206c2d8(NNSG3dRS* rs, u32 opt) {
    u32 cmdLen = 4;
    u32 idxNode = rs->c[1];
    u32 timing;
    BOOL cbFlag;

    rs->currentNodeDesc = idxNode;
    rs->flag |= 0x10;

    if (rs->flag & 0x400) {
        if (opt == 0x40 || opt == 0x60) {
            cmdLen++;
        }
        if (opt == 0x20 || opt == 0x60) {
            cmdLen++;
            if (!(rs->flag & 0x100)) {
                u32 arg = rs->c[4];
                func_0206de34(0x14, &arg, 1);
            }
        }
        rs->c += cmdLen;
        return;
    }

    if (opt == 0x40 || opt == 0x60) {
        u32 arg;
        if (opt == 0x40) {
            arg = rs->c[4];
        } else {
            arg = rs->c[5];
        }
        cmdLen++;
        if (!(rs->flag & 0x100)) {
            func_0206de34(0x14, &arg, 1);
        }
    }

    rs->pJntAnmResult = &rs->tmpJntAnmResult;
    cbFlag = CallBackCheck_A(rs, 6, &timing);
    if (!cbFlag) {
        NNSG3dJntAnmResult* pResult;
        BOOL done;

        if (rs->pRenderObj->recJntAnm) {
            pResult = &rs->pRenderObj->recJntAnm[idxNode];
            done = (rs->flag & 0x80) ? FALSE : TRUE;
        } else {
            pResult = &rs->tmpJntAnmResult;
            done = FALSE;
        }

        if (!done) {
            NNSG3dRenderObj* obj;
            pResult->flag = 0;
            obj = rs->pRenderObj;
            if (!obj->anmJnt || !obj->funcBlendJnt(pResult, obj->anmJnt, idxNode)) {
                UnkResNodeData* nd = GetNodeDataByIdx(rs->pResNodeInfo, idxNode);
                const fx32* p = (const fx32*)(nd + 1);

                if (nd->flag & 1) {
                    pResult->flag |= 4;
                } else {
                    pResult->trans.x = *p++;
                    pResult->trans.y = *p++;
                    pResult->trans.z = *p++;
                }

                if (nd->flag & 2) {
                    pResult->flag |= 2;
                } else if (nd->flag & 8) {
                    u32 idxPivot = (nd->flag & 0xf0) >> 4;
                    fx32 B, A;

                    A = ((const fx16*)p)[0];
                    B = ((const fx16*)p)[1];

                    MI_CpuClear24(&pResult->rot);
                    ((fx32*)&pResult->rot)[idxPivot] = (nd->flag & 0x100) ? -FX32_ONE : FX32_ONE;
                    ((fx32*)&pResult->rot)[data_020ba5e0[idxPivot][0]] = A;
                    ((fx32*)&pResult->rot)[data_020ba5e0[idxPivot][1]] = B;
                    ((fx32*)&pResult->rot)[data_020ba5e0[idxPivot][2]] = (nd->flag & 0x200) ? -B : B;
                    ((fx32*)&pResult->rot)[data_020ba5e0[idxPivot][3]] = (nd->flag & 0x400) ? -A : A;
                    p++;
                } else {
                    const fx16* q = (const fx16*)p;
                    pResult->rot._00 = nd->_00;
                    pResult->rot._01 = q[0];
                    pResult->rot._02 = q[1];
                    pResult->rot._10 = q[2];
                    pResult->rot._11 = q[3];
                    pResult->rot._12 = q[4];
                    pResult->rot._20 = q[5];
                    pResult->rot._21 = q[6];
                    pResult->rot._22 = q[7];
                    p += 4;
                }

                rs->funcJntScale(pResult, p, rs->c, nd->flag);
            }
        }
        rs->pJntAnmResult = pResult;
    }

    cbFlag = CallBackCheck_B(rs, 6, &timing);
    if (!cbFlag && !(rs->flag & 0x100)) {
        rs->funcJntMtx(rs->pJntAnmResult);
    }
    rs->pJntAnmResult = NULL;

    cbFlag = CallBackCheck_C(rs, 6, timing);
    if (opt == 0x20 || opt == 0x60) {
        cmdLen++;
        if (!cbFlag && !(rs->flag & 0x100)) {
            u32 arg = rs->c[4];
            func_0206de34(0x13, &arg, 1);
        }
    }
    rs->c += cmdLen;
}

// BB: billboard
void G3d_SBCRender_007(NNSG3dRS* rs, u32 opt) {
    u32 cmdLen = 2;
    VecFx32* trans = (VecFx32*)&data_020c3ecc.mtx._30;
    VecFx32* scale = &data_020c3ecc.scale;
    u32 timing;
    BOOL cbFlag;

    if (rs->flag & 0x200) {
        if (opt == 0x40 || opt == 0x60) {
            cmdLen++;
        }
        if (opt == 0x20 || opt == 0x60) {
            cmdLen++;
        }
        rs->c += cmdLen;
        return;
    }

    if (opt == 0x40 || opt == 0x60) {
        cmdLen++;
        if (!(rs->flag & 0x100)) {
            u32 arg;
            if (opt == 0x40) {
                arg = rs->c[2];
            } else {
                arg = rs->c[3];
            }
            func_0206de34(0x14, &arg, 1);
        }
    }

    cbFlag = CallBackCheck_A(rs, 7, &timing);
    if (!(rs->flag & 0x100) && !cbFlag) {
        MtxFx44 m;

        func_0206dcf0();
        G3_WRITE(0x0, 0x151110);
        G3_WRITE(0x0, 0);
        G3_WRITE(0x0, 0);

        while (func_020653a8(&m)) {
        }

        if (data_0210cf28.flag & 1) {
            MtxFx44 tmp;
            func_02061f80(func_0206b1bc(), &tmp);
            func_020627a0(&m, &tmp, &m);
        } else if (data_0210cf28.flag & 2) {
            MtxFx44 tmp;
            func_02061f80(&data_0210cf28.cameraMtx, &tmp);
            func_020627a0(&m, &tmp, &m);
        }

        trans->x = m._30;
        trans->y = m._31;
        trans->z = m._32;
        scale->x = func_0206308c((VecFx32*)&m._00);
        scale->y = func_0206308c((VecFx32*)&m._10);
        scale->z = func_0206308c((VecFx32*)&m._20);

        if (data_0210cf28.flag & 1) {
            G3_WRITE(0x0, 0x171012);
            G3_SEND(&data_020c3ecc.param[0], 8);
            G3_SEND(func_0206b1f4(), 0x30);
            G3_WRITE(0x0, 0x1b19);
            G3_SEND(&data_020c3ecc.mtx, 0x3c);
        } else if (data_0210cf28.flag & 2) {
            G3_WRITE(0x0, 0x171012);
            G3_SEND(&data_020c3ecc.param[0], 8);
            G3_SEND(func_0206aed4(), 0x30);
            G3_WRITE(0x0, 0x1b19);
            G3_SEND(&data_020c3ecc.mtx, 0x3c);
        } else {
            G3_SEND(&data_020c3ecc, 0x48);
        }
    }

    cbFlag = CallBackCheck_C(rs, 7, timing);
    if (opt == 0x20 || opt == 0x60) {
        cmdLen++;
        if (!cbFlag && !(rs->flag & 0x100)) {
            u32 arg = rs->c[2];
            func_0206de34(0x13, &arg, 1);
        }
    }
    rs->c += cmdLen;
}

// BBY: Y-axis billboard
void G3d_SBCRender_008(NNSG3dRS* rs, u32 opt) {
    u32 cmdLen = 2;
    VecFx32* trans = (VecFx32*)&data_020c3f14.mtx._30;
    VecFx32* scale = &data_020c3f14.scale;
    MtxFx43* mtx = &data_020c3f14.mtx;
    u32 timing;
    BOOL cbFlag;

    if (rs->flag & 0x200) {
        if (opt == 0x40 || opt == 0x60) {
            cmdLen++;
        }
        if (opt == 0x20 || opt == 0x60) {
            cmdLen++;
        }
        rs->c += cmdLen;
        return;
    }

    if (opt == 0x40 || opt == 0x60) {
        cmdLen++;
        if (!(rs->flag & 0x100)) {
            u32 arg;
            if (opt == 0x40) {
                arg = rs->c[2];
            } else {
                arg = rs->c[3];
            }
            func_0206de34(0x14, &arg, 1);
        }
    }

    cbFlag = CallBackCheck_A(rs, 8, &timing);
    if (!(rs->flag & 0x100) && !cbFlag) {
        MtxFx44 m;

        func_0206dcf0();
        G3_WRITE(0x0, 0x151110);
        G3_WRITE(0x0, 0);
        G3_WRITE(0x0, 0);

        while (func_020653a8(&m)) {
        }

        if (data_0210cf28.flag & 1) {
            MtxFx44 tmp;
            func_02061f80(func_0206b1bc(), &tmp);
            func_020627a0(&m, &tmp, &m);
        } else if (data_0210cf28.flag & 2) {
            MtxFx44 tmp;
            func_02061f80(&data_0210cf28.cameraMtx, &tmp);
            func_020627a0(&m, &tmp, &m);
        }

        trans->x = m._30;
        trans->y = m._31;
        trans->z = m._32;
        scale->x = func_0206308c((VecFx32*)&m._00);
        scale->y = func_0206308c((VecFx32*)&m._10);
        scale->z = func_0206308c((VecFx32*)&m._20);

        if (m._11 != 0 || m._12 != 0) {
            func_020630ec((VecFx32*)&m._10, (VecFx32*)&mtx->_10);
            mtx->_21 = -mtx->_12;
            mtx->_22 = mtx->_11;
        } else {
            func_020630ec((VecFx32*)&m._20, (VecFx32*)&mtx->_20);
            mtx->_12 = -mtx->_21;
            mtx->_11 = mtx->_22;
        }

        if (data_0210cf28.flag & 1) {
            G3_WRITE(0x0, 0x171012);
            G3_SEND(&data_020c3f14.param[0], 8);
            G3_SEND(func_0206b1f4(), 0x30);
            G3_WRITE(0x0, 0x1b19);
            G3_SEND(&data_020c3f14.mtx, 0x3c);
        } else if (data_0210cf28.flag & 2) {
            G3_WRITE(0x0, 0x171012);
            G3_SEND(&data_020c3f14.param[0], 8);
            G3_SEND(func_0206aed4(), 0x30);
            G3_WRITE(0x0, 0x1b19);
            G3_SEND(&data_020c3f14.mtx, 0x3c);
        } else {
            G3_SEND(&data_020c3f14, 0x48);
        }
    }

    cbFlag = CallBackCheck_C(rs, 8, timing);
    if (opt == 0x20 || opt == 0x60) {
        cmdLen++;
        if (!cbFlag && !(rs->flag & 0x100)) {
            u32 arg = rs->c[2];
            func_0206de34(0x13, &arg, 1);
        }
    }
    rs->c += cmdLen;
}

static inline BOOL BitVecCheck(const u32* vec, u32 idx) {
    return vec[idx >> 5] & (1 << (idx & 0x1f));
}

static inline void BitVecSet(u32* vec, u32 idx) {
    vec[idx >> 5] |= 1 << (idx & 0x1f);
}

#define WMUL(w, x) (((w) * (x)) >> 12)

// NODEMIX: blends the envelope matrices of several joints
void G3d_SBCRender_SKN(NNSG3dRS* rs, u32 opt) {
    fx64 w;
    const u8* p;
    UnkEvpCache* cache;
    MtxFx33* pN;
    const UnkEvpMtx* evp;
    u32 numMtx;
    u32 i;
    NNSG3dResMdl* mdl;
    struct {
        MtxFx43 m;
        MtxFx33 n;
    } sum;

    mdl = rs->pRenderObj->resMdl;
    numMtx = rs->c[2];
    evp = (const UnkEvpMtx*)((u8*)mdl + mdl->ofsEvpMtx);
    p = rs->c + 3;
    w = 0;
    MI_CpuFill(0, &sum, sizeof(sum));

    func_0206dcf0();
    G3_WRITE(0x10, 0);
    G3_WRITE(0x13, 1);
    G3_WRITE(0x15, 0);
    G3_WRITE(0x10, 2);

    for (i = 0; i < numMtx; i++) {
        u32 idxJnt = p[1];
        u32 off;
        BOOL cached;

        off = idxJnt * sizeof(UnkEvpCache);
        cached = BitVecCheck(rs->isScaleCacheOne, idxJnt);
        cache = (UnkEvpCache*)((u8*)data_0210d190.evpCache + off);
        if (!cached) {
            BitVecSet(rs->isScaleCacheOne, idxJnt);
            G3_WRITE(0x14, p[0]);
            G3_WRITE(0x10, 1);
            func_02065050(&evp[idxJnt].m);
        }

        if (i != 0) {
            sum.n._00 += WMUL(w, pN->_00);
            sum.n._01 += WMUL(w, pN->_01);
            sum.n._02 += WMUL(w, pN->_02);
            sum.n._10 += WMUL(w, pN->_10);
            sum.n._11 += WMUL(w, pN->_11);
            sum.n._12 += WMUL(w, pN->_12);
            sum.n._20 += WMUL(w, pN->_20);
            sum.n._21 += WMUL(w, pN->_21);
            sum.n._22 += WMUL(w, pN->_22);
        }

        if (!cached) {
            while (func_020653a8(&cache->m)) {
            }
            G3_WRITE(0x10, 2);
            func_0206506c(&evp[idxJnt].invN);
        }

        w = p[2] << 4;
        sum.m._00 += WMUL(w, cache->m._00);
        sum.m._01 += WMUL(w, cache->m._01);
        sum.m._02 += WMUL(w, cache->m._02);
        sum.m._10 += WMUL(w, cache->m._10);
        sum.m._11 += WMUL(w, cache->m._11);
        sum.m._12 += WMUL(w, cache->m._12);
        sum.m._20 += WMUL(w, cache->m._20);
        sum.m._21 += WMUL(w, cache->m._21);
        sum.m._22 += WMUL(w, cache->m._22);
        sum.m._30 += WMUL(w, cache->m._30);
        sum.m._31 += WMUL(w, cache->m._31);
        sum.m._32 += WMUL(w, cache->m._32);
        p += 3;

        pN = (MtxFx33*)((u8*)&data_0210d190.evpCache[0].n + off);
        if (!cached) {
            while (func_020653d8(pN)) {
            }
        }
    }

    sum.n._00 += WMUL(w, pN->_00);
    sum.n._01 += WMUL(w, pN->_01);
    sum.n._02 += WMUL(w, pN->_02);
    sum.n._10 += WMUL(w, pN->_10);
    sum.n._11 += WMUL(w, pN->_11);
    sum.n._12 += WMUL(w, pN->_12);
    sum.n._20 += WMUL(w, pN->_20);
    sum.n._21 += WMUL(w, pN->_21);
    sum.n._22 += WMUL(w, pN->_22);

    func_02065034((const MtxFx43*)&sum.n);
    G3_WRITE(0x10, 1);
    func_02065034(&sum.m);
    G3_WRITE(0x10, 0);
    G3_WRITE(0x14, 1);
    G3_WRITE(0x10, 2);
    G3_WRITE(0x13, rs->c[1]);
    rs->c += (rs->c[2] + 1) * 3;
}

// CALLDL: sends a display list embedded in the SBC
void G3d_SBCRender_00A(NNSG3dRS* rs, u32 opt) {
    u32 timing;
    BOOL cbFlag;

    cbFlag = CallBackCheck_A(rs, 0xa, &timing);
    if (!(rs->flag & 0x100) && !cbFlag) {
        u32 rel = rs->c[1] | (rs->c[2] << 8) | (rs->c[3] << 16) | (rs->c[4] << 24);
        u32 size = rs->c[5] | (rs->c[6] << 8) | (rs->c[7] << 16) | (rs->c[8] << 24);
        func_0206dd80((const u32*)(rs->c + rel), size);
    }
    CallBackCheck_C(rs, 0xa, timing);
    rs->c += 9;
}

// POSSCALE: applies the position scale of the model
void G3d_SBCRender_SCL(NNSG3dRS* rs, u32 opt) {
    if (!(rs->flag & 0x100) && !(rs->flag & 0x200)) {
        VecFx32 s;
        if (opt == 0) {
            s.x = s.y = s.z = rs->posScale;
        } else {
            s.x = s.y = s.z = rs->invPosScale;
        }
        func_0206de34(0x1b, &s, 3);
    }
    rs->c++;
}

// ENVMAP: environment mapping
void func_0206d61c(NNSG3dRS* rs, u32 opt) {
    if (!(rs->flag & 0x200) && (rs->flag & 1)) {
        u32 timing;
        BOOL cbFlag;
        MtxFx33 m;
        VecFx32 s;

        if ((rs->pMatAnmResult->prmTexImage & 0xc0000000) != 0x80000000) {
            rs->pMatAnmResult->prmTexImage &= ~0xc0000000;
            rs->pMatAnmResult->prmTexImage |= 0x80000000;
            data_020c3e64[1] = rs->pMatAnmResult->prmTexImage;
            func_0206de34(data_020c3e64[0], &data_020c3e64[1], 1);
        }

        {
            u32 arg = 3;
            func_0206de34(0x10, &arg, 1);
        }

        cbFlag = CallBackCheck_A(rs, 0xc, &timing);
        if (!cbFlag) {
            fx32 w, h;
            u32 arg;

            h = rs->pMatAnmResult->origHeight;
            w = rs->pMatAnmResult->origWidth;
            s.x = w << 15;
            s.y = -h << 15;
            s.z = 0x10000;
            func_0206de34(0x1b, &s, 3);
            arg = (u16)(fx16)(w << 3) | ((u16)(fx16)(h << 3) << 16);
            func_0206de34(0x22, &arg, 1);
        }

        cbFlag = CallBackCheck_B(rs, 0xc, &timing);
        if (!cbFlag) {
            NNSG3dResMatData* mat = GetMatDataByIdx(rs->pResMat, rs->c[1]);
            if (mat->flag & 0x2000) {
                const u8* p = mat->texMtx;
                if (!(mat->flag & 2)) {
                    p += 8;
                }
                if (!(mat->flag & 4)) {
                    p += 4;
                }
                if (!(mat->flag & 8)) {
                    p += 8;
                }
                func_0206de34(0x18, p, 0x10);
            }
        }

        cbFlag = CallBackCheck_C(rs, 0xc, timing);
        if (!cbFlag) {
            u32 arg2 = 2;
            u32 arg3;

            func_0206de34(0x10, &arg2, 1);
            func_0206df18(NULL, &m);
            arg3 = 3;
            func_0206de34(0x10, &arg3, 1);

            if (data_0210cf28.flag & 1) {
                func_0206de34(0x1a, &data_0210cf28.cameraMtx, 9);
                func_0206de34(0x1a, &data_0210cf28.prmBaseRot, 9);
                func_0206de34(0x1a, &m, 9);
            } else if (data_0210cf28.flag & 2) {
                func_0206de34(0x1a, &data_0210cf28.cameraMtx, 9);
                func_0206de34(0x1a, &m, 9);
            } else {
                func_0206de34(0x1a, &m, 9);
            }
        }

        {
            u32 arg = 2;
            func_0206de34(0x10, &arg, 1);
        }
    }
    rs->c += 3;
}

static inline u32 PackTexCoord(fx32 s, fx32 t) {
    return (u16)(fx16)(s >> 8) | ((u16)(fx16)(t >> 8) << 16);
}

// PRJMAP: projection mapping
void func_0206d930(NNSG3dRS* rs, u32 opt) {
    if (!(rs->flag & 0x200) && (rs->flag & 1)) {
        u32 timing;
        BOOL cbFlag;
        MtxFx43 m;
        MtxFx44 texMtx;

        func_0206df18(&m, NULL);
        {
            u32 arg = 0x1e;
            func_0206de34(0x13, &arg, 1);
        }

        if ((rs->pMatAnmResult->prmTexImage & 0xc0000000) != 0xc0000000) {
            rs->pMatAnmResult->prmTexImage &= ~0xc0000000;
            rs->pMatAnmResult->prmTexImage |= 0xc0000000;
            data_020c3e5c[1] = rs->pMatAnmResult->prmTexImage;
            func_0206de34(data_020c3e5c[0], &data_020c3e5c[1], 1);
        }

        cbFlag = CallBackCheck_A(rs, 0xd, &timing);
        if (!cbFlag) {
            fx32 w, h;

            h = rs->pMatAnmResult->origHeight;
            w = rs->pMatAnmResult->origWidth;
            data_020c3e8c._00 = w << 15;
            data_020c3e8c._11 = -h << 15;
            data_020c3e8c._30 = w << 15;
            data_020c3e8c._31 = h << 15;
            func_0206de34(0x16, &data_020c3e8c, 0x10);
        }

        cbFlag = CallBackCheck_B(rs, 0xd, &timing);
        if (!cbFlag) {
            NNSG3dResMatData* mat = GetMatDataByIdx(rs->pResMat, rs->c[1]);
            if (mat->flag & 0x2000) {
                const u8* p = mat->texMtx;
                if (!(mat->flag & 2)) {
                    p += 8;
                }
                if (!(mat->flag & 4)) {
                    p += 4;
                }
                if (!(mat->flag & 8)) {
                    p += 8;
                }
                func_0206de34(0x18, p, 0x10);
            }
        }

        cbFlag = CallBackCheck_C(rs, 0xd, timing);
        if (!cbFlag) {
            u32 arg;

            if (data_0210cf28.flag & 1) {
                func_0206de34(0x1c, &data_0210cf28.unk_0e0, 3);
                func_0206de34(0x1a, &data_0210cf28.prmBaseRot, 9);
                func_0206de34(0x19, &m, 0xc);
            } else if (data_0210cf28.flag & 2) {
                func_0206de34(0x19, &m, 0xc);
            } else {
                func_0206de34(0x19, func_0206aed4(), 0xc);
                func_0206de34(0x19, &m, 0xc);
            }

            func_0206dcf0();
            G3_WRITE(0x10, 0);
            G3_WRITE(0x11, 0);
            G3_WRITE(0x15, 0);
            while (func_020653a8(&texMtx)) {
            }
            G3_WRITE(0x12, 1);
            G3_WRITE(0x10, 3);
            func_0206de34(0x16, &texMtx, 0x10);

            arg = PackTexCoord(texMtx._30 >> 4, texMtx._31 >> 4);
            func_0206de34(0x22, &arg, 1);
        }

        {
            u32 arg = 2;
            func_0206de34(0x10, &arg, 1);
        }
        {
            u32 arg = 0x1e;
            func_0206de34(0x14, &arg, 1);
        }
    }
    rs->c += 3;
}

// SBC command functions indexed by command number
UnkSbcFunc data_020c3f5c[32] = {
    G3d_SBCRender_NOP, G3d_SBCRender_END, func_0206ba78, func_0206bbf4, func_0206c118, func_0206c26c,
    func_0206c2d8, G3d_SBCRender_007, G3d_SBCRender_008, G3d_SBCRender_SKN, G3d_SBCRender_00A, G3d_SBCRender_SCL,
    func_0206d61c, func_0206d930,
};
