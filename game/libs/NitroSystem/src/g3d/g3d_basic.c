#include "g3d_f.h"

/* sends joint SRT (basic scaling) */
void func_020709a0(const UnkJntAnmResult* result)
{
    if (!(result->flag & 4)) {
        if (!(result->flag & 2)) {
            func_0206de34(0x19, &result->rot, 12);
        } else {
            func_0206de34(0x1c, &result->trans, 3);
        }
    } else {
        if (!(result->flag & 2)) {
            func_0206de34(0x1a, &result->rot, 9);
        }
    }

    if (!(result->flag & 1)) {
        func_0206de34(0x1b, &result->scale, 3);
    }
}

/* joint scale (basic scaling) */
void func_02070a1c(UnkJntAnmResult* result, const UnkVecFx32* p, const u8* data, u32 srtflag)
{
    if (srtflag & 4) {
        result->flag |= 1;
    } else {
        result->scale.x = p->x;
        result->scale.y = p->y;
        result->scale.z = p->z;
    }
    result->flag |= 0x18;
}
