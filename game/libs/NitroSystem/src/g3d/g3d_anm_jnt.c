#include "g3d_internal.h"

// Joint animation resource
typedef struct UnkResJntAnm {
    u8 anmHeader[4]; // 0x00
    u16 numFrame;    // 0x04
    u16 numNode;     // 0x06
    u32 flag;        // 0x08
    u32 ofsRot3;     // 0x0C
    u32 ofsRot9;     // 0x10
    u16 ofsTag[1];   // 0x14
} UnkResJntAnm;

typedef struct UnkScale32 {
    fx32 s;
    fx32 inv;
} UnkScale32;

typedef struct UnkScale16 {
    fx16 s;
    fx16 inv;
} UnkScale16;

extern UnkAnmFunc data_020c3dc8;   // default joint animation function
/* for each pivot index the 4 other matrix elements set by a pivot-compressed rotation (also used by g3d_anm_jnt_rot.c) */
const u8 data_020ba604[9][4] = {{4, 5, 7, 8}, {3, 5, 6, 8}, {3, 4, 6, 7}, {1, 2, 7, 8}, {0, 2, 6, 8},
                                {0, 1, 6, 7}, {1, 2, 4, 5}, {0, 2, 3, 5}, {0, 1, 3, 4}};

void func_0206eccc(const UnkResJntAnm* res, u32 idx, fx32 frame, NNSG3dJntAnmResult* r);
void func_0206f080(fx32* out, fx32 frame, const u32* p, const UnkResJntAnm* res);
void func_0206f1e0(fx32* out, fx32 frame, const u32* p, const UnkResJntAnm* res);
void func_0206f33c(fx32* out, fx32 frame, const u32* p, const UnkResJntAnm* res);
void func_0206f530(fx32* out, fx32 frame, const u32* p, const UnkResJntAnm* res);
void func_0206f6c4(MtxFx33* out, fx32 frame, const u32* p, const UnkResJntAnm* res);
void func_0206facc(MtxFx33* out, fx32 frame, const u32* p, const UnkResJntAnm* res);
BOOL func_0206fe00(MtxFx33* out, const void* rot3, const void* rot9, u32 info);

// Initializes an animation object for a joint animation
void func_0206e9f8(NNSG3dAnmObj* obj, const UnkResJntAnm* res, const NNSG3dResMdl* mdl) {
    u32 i;

    obj->resAnm = (void*)res;
    obj->funcAnm = data_020c3dc8;
    MI_CpuFillU16(0, obj->mapData, (obj->numMapData = mdl->info.numNode) * 2);

    {
        const u16* ofsTag = res->ofsTag;
        for (i = 0; i < res->numNode; i++) {
            obj->mapData[i] = (*(const u32*)((u8*)res + ofsTag[i]) >> 24) | 0x100;
        }
    }
}

// Default joint animation function
void func_0206ea74(NNSG3dJntAnmResult* r, const NNSG3dAnmObj* obj, u32 idx) {
    const UnkResJntAnm* res = (const UnkResJntAnm*)obj->resAnm;
    fx32 frame = obj->frame;

    if (frame >= (res->numFrame << 12)) {
        frame = (res->numFrame << 12) - 1;
    } else if (frame < 0) {
        frame = 0;
    }
    func_0206eccc(res, idx, frame, r);
}

// Gets the translation of the current node from the model
void func_0206eab0(NNSG3dJntAnmResult* r) {
    const UnkResNodeData* nd = GetNodeDataByIdx(data_0210d18c->pResNodeInfo, data_0210d18c->c[1]);
    const fx32* p = (const fx32*)(nd + 1);

    if (nd->flag & 1) {
        r->flag |= 4;
    } else {
        r->trans.x = p[0];
        r->trans.y = p[1];
        r->trans.z = p[2];
    }
}

// Gets the scale of the current node from the model
void func_0206eb18(NNSG3dJntAnmResult* r) {
    NNSG3dRS* rs = data_0210d18c;
    const u8* c = rs->c;
    const UnkResNodeData* nd = GetNodeDataByIdx(rs->pResNodeInfo, c[1]);
    const u8* p = (const u8*)(nd + 1);

    if (!(nd->flag & 1)) {
        p += 0xc;
    }
    if (!(nd->flag & 2)) {
        if (nd->flag & 8) {
            p += 4;
        } else {
            p += 0x10;
        }
    }
    rs->funcJntScale(r, p, c, nd->flag);
}

// Gets the rotation of the current node from the model
void func_0206eb7c(NNSG3dJntAnmResult* r) {
    long idxPivot;
    const UnkResNodeData* nd = GetNodeDataByIdx(data_0210d18c->pResNodeInfo, data_0210d18c->c[1]);
    const fx16* p = (const fx16*)(nd + 1);

    if (!(nd->flag & 1)) {
        p += 6;
    }

    if (!(nd->flag & 2)) {
        if (nd->flag & 8) {
            fx32 A;
            fx32 B;

            idxPivot = (nd->flag & 0xf0) >> 4;
            A = p[0];
            B = p[1];

            MI_CpuClear24(&r->rot);
            ((fx32*)&r->rot)[idxPivot] = (nd->flag & 0x100) ? -FX32_ONE : FX32_ONE;
            ((fx32*)&r->rot)[data_020ba604[idxPivot][0]] = A;
            ((fx32*)&r->rot)[data_020ba604[idxPivot][1]] = B;
            ((fx32*)&r->rot)[data_020ba604[idxPivot][2]] = (nd->flag & 0x200) ? -B : B;
            ((fx32*)&r->rot)[data_020ba604[idxPivot][3]] = (nd->flag & 0x400) ? -A : A;
        } else {
            r->rot._00 = nd->_00;
            r->rot._01 = p[0];
            r->rot._02 = p[1];
            r->rot._10 = p[2];
            r->rot._11 = p[3];
            r->rot._12 = p[4];
            r->rot._20 = p[5];
            r->rot._21 = p[6];
            r->rot._22 = p[7];
        }
    } else {
        r->flag |= 2;
    }
}

// Calculates the joint animation result of a node
void func_0206eccc(const UnkResJntAnm* res, u32 idx, fx32 frame, NNSG3dJntAnmResult* r) {
    const u32* tag = (const u32*)((u8*)res + res->ofsTag[idx]);
    u32 flag = *tag;
    BOOL interp;
    const u32* p;
    struct {
        VecFx32 s;
        VecFx32 inv;
    } scale;

    if (flag & 1) {
        r->flag = 7;
    } else {
        p = tag + 1;
        interp = ((frame & 0xfff) && (res->flag & 1)) ? TRUE : FALSE;
        r->flag = 0;

        if (!(flag & 6)) {
            if (!(flag & 8)) {
                if (interp) {
                    func_0206f1e0(&r->trans.x, frame, p, res);
                } else {
                    func_0206f080(&r->trans.x, frame, p, res);
                }
                p += 2;
            } else {
                r->trans.x = (fx32)*p++;
            }

            if (!(flag & 0x10)) {
                if (interp) {
                    func_0206f1e0(&r->trans.y, frame, p, res);
                } else {
                    func_0206f080(&r->trans.y, frame, p, res);
                }
                p += 2;
            } else {
                r->trans.y = (fx32)*p++;
            }

            if (!(flag & 0x20)) {
                if (interp) {
                    func_0206f1e0(&r->trans.z, frame, p, res);
                } else {
                    func_0206f080(&r->trans.z, frame, p, res);
                }
                p += 2;
            } else {
                r->trans.z = (fx32)*p++;
            }
        } else if (flag & 2) {
            r->flag |= 4;
        } else {
            func_0206eab0(r);
        }

        if (!(flag & 0xc0)) {
            if (!(flag & 0x100)) {
                if (interp) {
                    func_0206facc(&r->rot, frame, p, res);
                } else {
                    func_0206f6c4(&r->rot, frame, p, res);
                }
                p += 2;
            } else {
                if (func_0206fe00(&r->rot, (u8*)res + res->ofsRot3, (u8*)res + res->ofsRot9, *p)) {
                    fx32 x = (r->rot._01 * r->rot._12 - r->rot._02 * r->rot._11) >> 12;
                    fx32 y = (r->rot._02 * r->rot._10 - r->rot._00 * r->rot._12) >> 12;
                    fx32 z = (r->rot._00 * r->rot._11 - r->rot._01 * r->rot._10) >> 12;
                    r->rot._20 = x;
                    r->rot._21 = y;
                    r->rot._22 = z;
                }
                p++;
            }
        } else if (flag & 0x40) {
            r->flag |= 2;
        } else {
            func_0206eb7c(r);
        }

        if (!(flag & 0x600)) {
            if (!(flag & 0x800)) {
                fx32 tmp[2];
                if (interp) {
                    func_0206f530(tmp, frame, p, res);
                } else {
                    func_0206f33c(tmp, frame, p, res);
                }
                scale.s.x = tmp[0];
                scale.inv.x = tmp[1];
            } else {
                scale.s.x = p[0];
                scale.inv.x = p[1];
            }

            if (!(flag & 0x1000)) {
                fx32 tmp[2];
                if (interp) {
                    func_0206f530(tmp, frame, p + 2, res);
                } else {
                    func_0206f33c(tmp, frame, p + 2, res);
                }
                scale.s.y = tmp[0];
                scale.inv.y = tmp[1];
            } else {
                scale.s.y = p[2];
                scale.inv.y = p[3];
            }

            if (!(flag & 0x2000)) {
                fx32 tmp[2];
                if (interp) {
                    func_0206f530(tmp, frame, p + 4, res);
                } else {
                    func_0206f33c(tmp, frame, p + 4, res);
                }
                scale.s.z = tmp[0];
                scale.inv.z = tmp[1];
            } else {
                scale.s.z = p[4];
                scale.inv.z = p[5];
            }
        } else if (flag & 0x200) {
            r->flag |= 1;
        } else {
            func_0206eb18(r);
            return;
        }
    }

    data_0210d18c->funcJntScale(r, &scale, data_0210d18c->c, (r->flag & 1) ? 4 : 0);
}
// Gets a translation value of a frame (no interpolation between frames)
void func_0206f080(fx32* out, fx32 frame, const u32* p, const UnkResJntAnm* res) {
    u32 b;
    u32 a;
    u32 lastInterp;
    u32 info;
    u32 idx;
    u32 f;
    const void* data;

    f = (u32)(frame >> 12);
    data = (u8*)res + p[1];
    info = p[0];

    if (info & 0xc0000000) {
        lastInterp = (info & 0x1fff0000) >> 16;

        if (info & 0x40000000) {
            if (f & 1) {
                if (f > lastInterp) {
                    f = (lastInterp >> 1) + 1;
                    goto direct;
                } else {
                    idx = f >> 1;
                    goto half;
                }
            } else {
                f >>= 1;
                goto direct;
            }
        } else {
            if (f & 3) {
                if (f > lastInterp) {
                    f = (lastInterp >> 2) + (f & 3);
                    goto direct;
                }
                if (f & 1) {
                    if (f & 2) {
                        b = f >> 2;
                        a = b + 1;
                    } else {
                        a = f >> 2;
                        b = a + 1;
                    }
                    if (info & 0x20000000) {
                        const fx16* d = (const fx16*)data;
                        fx32 t = d[a] * 3;
                        *out = (t + d[b]) >> 2;
                    } else {
                        const fx32* d = (const fx32*)data;
                        fx64 t = (fx64)d[a] * 3;
                        *out = (fx32)((t + d[b]) >> 2);
                    }
                    return;
                } else {
                    idx = f >> 2;
                    goto half;
                }
            } else {
                f >>= 2;
                goto direct;
            }
        }

    half:
        if (info & 0x20000000) {
            *out = (((const fx16*)data)[idx] + *((const fx16*)data + idx + 1)) >> 1;
        } else {
            *out = (((const fx32*)data)[idx] >> 1) + (*((const fx32*)data + idx + 1) >> 1);
        }
        return;
    }

direct:
    if (info & 0x20000000) {
        *out = ((const fx16*)data)[f];
    } else {
        *out = ((const fx32*)data)[f];
    }
}

// Gets a translation value of a frame (interpolated)
void func_0206f1e0(fx32* out, fx32 frame, const u32* p, const UnkResJntAnm* res) {
    const void* data;
    u32 f;
    u32 info;

    f = (u32)(frame >> 12);
    data = (u8*)res + p[1];
    info = p[0];

    if (res->numFrame - 1 == frame >> 12) {
        if (info & 0xc0000000) {
            if (info & 0x40000000) {
                f = (f & 1) + (f >> 1);
            } else {
                f = (f & 3) + (f >> 2);
            }
        }

        if (res->flag & 2) {
            fx32 frac = frame & 0xfff;
            fx32 v0, v1;
            if (info & 0x20000000) {
                v0 = ((const fx16*)data)[f];
                v1 = ((const fx16*)data)[0];
            } else {
                v0 = ((const fx32*)data)[f];
                v1 = ((const fx32*)data)[0];
            }
            *out = v0 + (((v1 - v0) * frac) >> 12);
        } else {
            *out = (info & 0x20000000) ? ((const fx16*)data)[f] : ((const fx32*)data)[f];
        }
    } else {
        fx32 mul;
        int shift;
        fx32 v0, v1;

        if (info & 0xc0000000) {
            u32 lastInterp = (info & 0x1fff0000) >> 16;
            if (info & 0x40000000) {
                if (f >= lastInterp) {
                    f = lastInterp >> 1;
                    goto step1;
                }
                f >>= 1;
                frame &= 0x1fff;
                mul = 2;
                shift = 1;
            } else {
                if (f >= lastInterp) {
                    f = (f & 3) + (f >> 2);
                    goto step1;
                }
                f >>= 2;
                frame &= 0x3fff;
                mul = 4;
                shift = 2;
            }
        } else {
        step1:
            frame &= 0xfff;
            mul = 1;
            shift = 0;
        }

        if (info & 0x20000000) {
            v0 = ((const fx16*)data)[f];
            v1 = *((const fx16*)data + f + 1);
        } else {
            v0 = ((const fx32*)data)[f];
            v1 = *((const fx32*)data + f + 1);
        }
        *out = (fx32)(v0 * mul + (((v1 - v0) * frame) >> 12)) >> shift;
    }
}

// Gets a scale value pair of a frame (no interpolation between frames)
#pragma opt_propagation off
void func_0206f33c(fx32* out, fx32 frame, const u32* p, const UnkResJntAnm* res) {
    u32 idx;
    u32 lastInterp;
    const void* data;
    u32 a;
    u32 info;
    u32 b;
    u32 f;

    f = (u32)(frame >> 12);
    data = (u8*)res + p[1];
    info = p[0];

    if (info & 0xc0000000) {
        lastInterp = (info & 0x1fff0000) >> 16;

        if (info & 0x40000000) {
            if (f & 1) {
                if (f > lastInterp) {
                    f = (lastInterp >> 1) + 1;
                    goto direct;
                } else {
                    idx = f >> 1;
                    goto half;
                }
            } else {
                f >>= 1;
                goto direct;
            }
        } else {
            if (f & 3) {
                if (f > lastInterp) {
                    f = (lastInterp >> 2) + (f & 3);
                    goto direct;
                }
                if (f & 1) {
                    if (f & 2) {
                        b = f >> 2;
                        a = b + 1;
                    } else {
                        a = f >> 2;
                        b = a + 1;
                    }
                    if (info & 0x20000000) {
                        const UnkScale16* d = (const UnkScale16*)data;
                        fx32 x;
                        x = d[a].s;
                        out[0] = (d[b].s + (x + x * 2)) >> 2;
                        x = d[a].inv;
                        out[1] = (d[b].inv + (x + x * 2)) >> 2;
                    } else {
                        const UnkScale32* d = (const UnkScale32*)data;
                        fx32 x;
                        fx32 y;
                        y = d[b].s;
                        x = d[a].s;
                        out[0] = (fx32)(((fx64)x * 3 - -(fx64)y) >> 2);
                        x = d[a].inv;
                        out[1] = (fx32)(((fx64)x * 3 + d[b].inv) >> 2);
                    }
                    return;
                } else {
                    idx = f >> 2;
                    goto half;
                }
            } else {
                f >>= 2;
                goto direct;
            }
        }

    }

direct:
    if (info & 0x20000000) {
        const UnkScale16* d = (const UnkScale16*)data;
        out[0] = d[f].s;
        out[1] = d[f].inv;
    } else {
        const UnkScale32* d = (const UnkScale32*)data;
        out[0] = d[f].s;
        out[1] = d[f].inv;
    }
    return;

half:
    if (info & 0x20000000) {
        const UnkScale16* d = (const UnkScale16*)data;
        out[0] = (d[idx].s + (d + idx + 1)->s) >> 1;
        out[1] = ((d + idx)->inv + (d + idx + 1)->inv) >> 1;
    } else {
        const UnkScale32* d = (const UnkScale32*)data;
        out[0] = (d[idx].s + (d + idx + 1)->s) >> 1;
        out[1] = ((d + idx)->inv + (d + idx + 1)->inv) >> 1;
    }
}

#pragma opt_propagation reset

// Gets a scale value pair of a frame (interpolated)
void func_0206f530(fx32* out, fx32 frame, const u32* p, const UnkResJntAnm* res) {
    const void* data;
    u32 f;
    u32 info;
    u32 next;
    fx32 mul;
    int shift;
    fx32 s0, inv0, s1, inv1;

    f = (u32)(frame >> 12);
    data = (u8*)res + p[1];
    info = p[0];

    if (res->numFrame - 1 == frame >> 12) {
        if (info & 0xc0000000) {
            if (info & 0x40000000) {
                f = (f & 1) + (f >> 1);
            } else {
                f = (f & 3) + (f >> 2);
            }
        }

        if (res->flag & 2) {
            next = 0;
            goto step1;
        }

        if (info & 0x20000000) {
            const UnkScale16* d = (const UnkScale16*)data;
            out[0] = d[f].s;
            out[1] = d[f].inv;
        } else {
            const UnkScale32* d = (const UnkScale32*)data;
            out[0] = d[f].s;
            out[1] = d[f].inv;
        }
        return;
    }

    if (info & 0xc0000000) {
        u32 lastInterp = (info & 0x1fff0000) >> 16;
        if (info & 0x40000000) {
            if (f >= lastInterp) {
                f = lastInterp >> 1;
                next = f + 1;
                goto step1;
            }
            f >>= 1;
            next = f + 1;
            frame &= 0x1fff;
            mul = 2;
            shift = 1;
        } else {
            if (f >= lastInterp) {
                f = (f & 3) + (f >> 2);
                next = f + 1;
                goto step1;
            }
            f >>= 2;
            next = f + 1;
            frame &= 0x3fff;
            mul = 4;
            shift = 2;
        }
    } else {
        next = f + 1;
    step1:
        frame &= 0xfff;
        mul = 1;
        shift = 0;
    }

    if (info & 0x20000000) {
        const UnkScale16* d = (const UnkScale16*)data;
        s0 = d[f].s;
        inv0 = d[f].inv;
        s1 = d[next].s;
        inv1 = d[next].inv;
    } else {
        const UnkScale32* d = (const UnkScale32*)data;
        s0 = d[f].s;
        inv0 = d[f].inv;
        s1 = d[next].s;
        inv1 = d[next].inv;
    }
    out[0] = (fx32)(s0 * mul + (((s1 - s0) * frame) >> 12)) >> shift;
    out[1] = (fx32)(inv0 * mul + (((inv1 - inv0) * frame) >> 12)) >> shift;
}
