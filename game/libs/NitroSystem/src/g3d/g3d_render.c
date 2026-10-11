#include "g3d_internal.h"

NNSG3dRS*     data_0210d18c; // current render state
UnkG3dRSOnGlb data_0210d190; // render state buffers

/* joint matrix / joint scale functions of the scaling rules (basic, Maya, SI3D) */
void func_020709a0(const NNSG3dJntAnmResult* r);
void func_02070a5c(const NNSG3dJntAnmResult* r);
void func_02071254(const NNSG3dJntAnmResult* r);
void func_02070a1c(NNSG3dJntAnmResult* r, const void* p, const u8* cmd, u32 flag);
void func_02070b20(NNSG3dJntAnmResult* r, const void* p, const u8* cmd, u32 flag);
void func_02071380(NNSG3dJntAnmResult* r, const void* p, const u8* cmd, u32 flag);
/* texture matrix functions of the texture matrix modes (Maya, SI3D, 3dsMax, XSI) */
void func_0207110c(NNSG3dMatAnmResult* r);
void func_0207159c(NNSG3dMatAnmResult* r);
void func_02071c64(NNSG3dMatAnmResult* r);
void func_0207228c(NNSG3dMatAnmResult* r);

UnkJntMtxFunc data_020c3e34[3] = {func_020709a0, func_02070a5c, func_02071254};   // indexed by scalingRule
UnkJntScaleFunc data_020c3e40[3] = {func_02070a1c, func_02070b20, func_02071380}; // indexed by scalingRule
UnkTexMtxFunc data_020c3e4c[4] = {func_0207110c, func_0207159c, func_02071c64, func_0207228c}; // indexed by texMtxMode

// Executes SBC commands until the end flag is set
void G3d_RenderSBCCommands(NNSG3dRS* rs) {
    do {
        rs->flag &= ~0x40;
        data_020c3f5c[*rs->c & 0x1f](rs, *rs->c & 0xe0);
    } while (!(rs->flag & 0x20));
}

// Sets up a render state for a render object and runs its SBC
void func_0206b768(NNSG3dRS* rs, NNSG3dRenderObj* obj) {
    MI_CpuFill(0, rs, sizeof(NNSG3dRS));
    rs->isMatCached[2] = 1;
    rs->flag = 1;

    if (obj->ptrUserSbc) {
        rs->c = obj->ptrUserSbc;
    } else {
        rs->c = (u8*)obj->resMdl + obj->resMdl->ofsSbc;
    }
    rs->pRenderObj = obj;
    rs->pResNodeInfo = &obj->resMdl->nodeInfo;
    rs->pResMat = (NNSG3dResMat*)((u8*)obj->resMdl + obj->resMdl->ofsMat);
    rs->pResShp = (NNSG3dResShp*)((u8*)obj->resMdl + obj->resMdl->ofsShp);
    rs->funcJntScale = data_020c3e40[obj->resMdl->info.scalingRule];
    rs->funcJntMtx = data_020c3e34[obj->resMdl->info.scalingRule];
    rs->funcTexMtx = data_020c3e4c[obj->resMdl->info.texMtxMode];
    rs->posScale = obj->resMdl->info.posScale;
    rs->invPosScale = obj->resMdl->info.invPosScale;

    if (obj->cbFunc && obj->cbCmd < 32) {
        rs->cbVecFunc[obj->cbCmd] = obj->cbFunc;
        rs->cbVecTiming[obj->cbCmd] = obj->cbTiming;
    }

    if (obj->flag & 1) {
        rs->flag |= 0x80;
    }
    if (obj->flag & 2) {
        rs->flag |= 0x100;
    }
    if (obj->flag & 4) {
        rs->flag |= 0x200;
    }
    if (obj->flag & 8) {
        rs->flag |= 0x400;
    }

    if (obj->cbInitFunc) {
        obj->cbInitFunc(rs);
    }

    G3d_RenderSBCCommands(rs);
    obj->flag &= ~1;
}

// Sets the hint bits of all the nodes/materials animated by an animation list
void func_0206b8e4(u32* hint, const NNSG3dAnmObj* anm) {
    while (anm) {
        int i;
        for (i = 0; i < anm->numMapData; i++) {
            if (anm->mapData[i] & 0x100) {
                hint[i >> 5] |= 1 << (i & 0x1f);
            }
        }
        anm = anm->next;
    }
}

void G3d_Render(NNSG3dRenderObj* obj) {
    NNSG3dRS rs;

    if ((obj->flag & 0x10) == 0x10) {
        func_0206785c(0, obj->hintMatAnmExist, sizeof(obj->hintMatAnmExist));
        func_0206785c(0, obj->hintJntAnmExist, sizeof(obj->hintJntAnmExist));
        func_0206785c(0, obj->hintVisAnmExist, sizeof(obj->hintVisAnmExist));
        if (obj->anmMat) {
            func_0206b8e4(obj->hintMatAnmExist, obj->anmMat);
        }
        if (obj->anmJnt) {
            func_0206b8e4(obj->hintJntAnmExist, obj->anmJnt);
        }
        if (obj->anmVis) {
            func_0206b8e4(obj->hintVisAnmExist, obj->anmVis);
        }
        obj->flag &= ~0x10;
    }

    if (data_0210d18c) {
        func_0206b768(data_0210d18c, obj);
    } else {
        data_0210d18c = &rs;
        func_0206b768(&rs, obj);
        data_0210d18c = NULL;
    }
}
