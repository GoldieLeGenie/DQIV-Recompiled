#include "g3d_types.h"

/* Animation objects and render objects. */

extern u32 data_020c3dc0;                   // number of entries in data_020c3de4 (data of g3d_anmblend.c)
extern UnkAnmInitEntry data_020c3de4[];     // animation init functions (data_020c3de8 = &[0].unk_04)
extern UnkBlendFunc data_020c3dd8;          // default visibility blend function
extern UnkBlendFunc data_020c3ddc;          // default joint blend function
extern UnkBlendFunc data_020c3de0;          // default material blend function

extern void func_0206785c(u32 value, void* dest, u32 size);

/* Returns the size of an animation object for this animation and model. */
u32 func_0206a26c(const UnkResAnmHeader* resAnm, const UnkResMdlInfo* resMdl) {
    switch (resAnm->unk_00) {
    case 'M':
        return (resMdl->unk_18 * 2 + 0x1c) & ~3;
    case 'J':
    case 'V':
        return (resMdl->unk_17 * 2 + 0x1c) & ~3;
    default:
        return 0;
    }
}

void func_0206a2bc(UnkAnmObj* anm, UnkResAnmHeader* resAnm, const UnkResMdlInfo* resMdl, void* resTex) {
    u32 i;

    anm->unk_00 = 0;
    anm->unk_08 = resAnm;
    anm->unk_10 = NULL;
    anm->unk_18 = 127;
    anm->unk_04 = 0x1000;
    anm->unk_14 = resTex;

    for (i = 0; i < data_020c3dc0; i++) {
        if (resAnm->unk_00 == data_020c3de4[i].unk_00 && resAnm->unk_02 == data_020c3de4[i].unk_02) {
            data_020c3de4[i].unk_04(anm, resAnm, resMdl);
            return;
        }
    }
}

void func_0206a34c(UnkRenderObj* obj, void* resMdl) {
    func_0206785c(0, obj, sizeof(UnkRenderObj));
    obj->unk_0c = data_020c3de0;
    obj->unk_14 = data_020c3ddc;
    obj->unk_1c = data_020c3dd8;
    obj->unk_04 = resMdl;
}

/* Inserts anm (and its chain) into a list sorted by priority. */
void func_0206a3a0(UnkAnmObj** list, UnkAnmObj* anm) {
    UnkAnmObj* p = *list;

    if (p == NULL) {
        *list = anm;
        return;
    }

    if (p->unk_10 == NULL) {
        if (p->unk_18 > anm->unk_18) {
            UnkAnmObj* tail = anm;

            while (tail->unk_10 != NULL) {
                tail = tail->unk_10;
            }
            tail->unk_10 = p;
            *list = anm;
        } else {
            p->unk_10 = anm;
        }
    } else {
        UnkAnmObj* q = p->unk_10;

        while (q != NULL) {
            if (q->unk_18 >= anm->unk_18) {
                UnkAnmObj* tail = anm;

                while (tail->unk_10 != NULL) {
                    tail = tail->unk_10;
                }
                p->unk_10 = anm;
                tail->unk_10 = q;
                return;
            }
            p = q;
            q = q->unk_10;
        }
        p->unk_10 = anm;
    }
}

/* Marks the entries animated by anm (and its chain) in a bit field. */
void func_0206a458(u32* bits, UnkAnmObj* anm) {
    if (anm == NULL) {
        return;
    }
    do {
        int i;

        for (i = 0; i < anm->unk_19; i++) {
            if (anm->unk_1a[i] & 0x100) {
                bits[i >> 5] |= 1 << (i & 31);
            }
        }
        anm = anm->unk_10;
    } while (anm != NULL);
}

void func_0206a4c0(UnkRenderObj* obj, UnkAnmObj* anm) {
    switch (anm->unk_08->unk_00) {
    case 'M':
        func_0206a458(obj->unk_3c, anm);
        func_0206a3a0(&obj->unk_08, anm);
        break;
    case 'J':
        func_0206a458(obj->unk_44, anm);
        func_0206a3a0(&obj->unk_10, anm);
        break;
    case 'V':
        func_0206a458(obj->unk_4c, anm);
        func_0206a3a0(&obj->unk_18, anm);
        break;
    }
}

/* Unlinks anm from a list, returns TRUE when it was found. */
BOOL func_0206a538(UnkAnmObj** list, UnkAnmObj* anm) {
    UnkAnmObj* q;
    UnkAnmObj* p = *list;

    if (p == NULL) {
        return FALSE;
    }
    if (p == anm) {
        *list = p->unk_10;
        anm->unk_10 = NULL;
        return TRUE;
    }
    q = p->unk_10;
    while (q != NULL) {
        if (q == anm) {
            p->unk_10 = q->unk_10;
            q->unk_10 = NULL;
            return TRUE;
        }
        p = q;
        q = q->unk_10;
    }
    return FALSE;
}

void func_0206a5ac(UnkRenderObj* obj, UnkAnmObj* anm) {
    if (func_0206a538(&obj->unk_08, anm) || func_0206a538(&obj->unk_10, anm) || func_0206a538(&obj->unk_18, anm)) {
        obj->unk_00 |= 0x10;
    }
}
