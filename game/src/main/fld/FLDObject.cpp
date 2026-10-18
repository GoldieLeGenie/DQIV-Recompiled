#include <nitro/native_g3.h>
#include "main/fld/FLDObject.hpp"
#include "main/fld/Coll.hpp"
#include "main/fld/FldStage.hpp"
#include "main/object/DSSAObject.hpp"
#include "main/dss/UnkVramTransfer.hpp"
#ifdef DQIV_NATIVE
#include "DualScreen.hpp"
#endif
#include "nitro/g3.hpp"
#ifdef DQIV_NATIVE
#include "main/global/Global.hpp"
#endif
#include "nitro/fx/fx_trig.h"
#include <mem.h>
#include <string.h>
#include <nitro/std.h>
#include <nitro/fx/fx_atan.h>

#define FLD_ID(a, b, c, d) (((d) << 24) | ((c) << 16) | ((b) << 8) | (a))
#define FLD_LERP(a, b, t) ((a) + FX_Mul((b) - (a), (t)))

static inline fx32 FLD_LerpFx(fx32 a, fx32 b, fx32 rate)
{
    return a + FX_Mul(b - a, rate);
}

#define FLD_LERP_5(x0, x1, rate)                                                               \
    if ((x0) != (x1)) {                                                                        \
        (x0) = FLD_LerpFx((x0) << FX32_SHIFT, (x1) << FX32_SHIFT, (rate)) >> FX32_SHIFT;        \
    }

// uses the caller's c0, c1, x0, x1, col
#define FLD_LERP_RGB(dst, src0, src1, rate) \
    c0 = (src0);                            \
    c1 = (src1);                            \
    if (c0 != c1) {                         \
        x0 = c0 & 0x1f;                     \
        x1 = c1 & 0x1f;                     \
        FLD_LERP_5(x0, x1, rate);           \
        col = x0 & 0x1f;                    \
        x0 = (c0 >> 5) & 0x1f;              \
        x1 = (c1 >> 5) & 0x1f;              \
        FLD_LERP_5(x0, x1, rate);           \
        col |= (x0 & 0x1f) << 5;            \
        x0 = (c0 >> 10) & 0x1f;             \
        x1 = (c1 >> 10) & 0x1f;             \
        FLD_LERP_5(x0, x1, rate);           \
        col |= (x0 & 0x1f) << 10;           \
        c0 = col;                           \
    }                                       \
    dst = c0;

#define FLD_LERP_16(x0, x1, sh, rate)                                                         \
    if ((x0) != (x1)) {                                                                       \
        (x0) = (short)(FLD_LerpFx((x0) << (sh), (x1) << (sh), (rate)) >> (sh));                \
    }

// uses the caller's c0, c1, x0, x1, col
#define FLD_LERP_XY(dst, src0, src1, sh, rate) \
    c0 = (src0);                               \
    c1 = (src1);                               \
    if (c0 != c1) {                            \
        x0 = (short)c0;                        \
        x1 = (short)c1;                        \
        FLD_LERP_16(x0, x1, sh, rate);         \
        col = (unsigned short)x0;              \
        x0 = (short)(c0 >> 16);                \
        x1 = (short)(c1 >> 16);                \
        FLD_LERP_16(x0, x1, sh, rate);         \
        col |= x0 << 16;                       \
        c0 = col;                              \
    }                                          \
    dst = c0;

// same as FLD_LERP_XY with the words in x0/x1 and the halves in c0/c1
#define FLD_LERP_ST(dst, src0, src1, sh, rate) \
    x0 = (src0);                               \
    x1 = (src1);                               \
    if (x0 != x1) {                            \
        c0 = (short)x0;                        \
        c1 = (short)x1;                        \
        FLD_LERP_16(c0, c1, sh, rate);         \
        col = (unsigned short)c0;              \
        c0 = (short)(x0 >> 16);                \
        c1 = (short)(x1 >> 16);                \
        FLD_LERP_16(c0, c1, sh, rate);         \
        col |= c0 << 16;                       \
        x0 = col;                              \
    }                                          \
    dst = x0;

namespace fld {

static int s_frameStep;
static int s_cameraNo;
static VecFx32 s_cameraPos;
static VecFx32 s_plane0;
static VecFx32 s_plane1;

static int compareObjPriority(void* a, void* b);
static void calcMapObjRender(FLD_MAP_OBJ* obj, VecFx32* ofs);
static int updateFldAnim(FLD_ANIM* anim, FLD_ANIM_FUNC func, void* work);
static int applyCameraKey(void* key0, void* key1, fx32 rate, void* work);
static int applyObjKey(void* key0, void* key1, fx32 rate, void* work);
static int applyObjKeyRotOffset(void* key0, void* key1, fx32 rate, void* work);
static int storeSeqKeys(void* key0, void* key1, fx32 rate, void* work);
static int applySeqKey(void* key0, void* key1, fx32 rate, void* work);
static int applyShapeMorph(void* key0, void* key1, fx32 rate, void* work);
static int applyLightKey(void* key0, void* key1, fx32 rate, void* work);
static int isMapObjInView(FLD_RENDER* render, fx32 far);
static int boxTestMapObj(VecFx32* pos, VecFx32* rot, VecFx32* scale, VecFx32* box, fx32 rate);
static void skipSbcBillboard(NNSG3dRS* rs, unsigned int opt);
static int calcMapObjFadeAlpha(FLD_MAP_OBJ* obj, FLD_MODEL* model);
static void setMdlMatAlpha(NNSG3dResMdl* mdl, int alpha, unsigned char* base);

ARM FLDObject::FLDObject()
{
    m_flag = 0;
    init();
}

ARM FLDObject::~FLDObject()
{
    Final();
}

ARM void FLDObject::init()
{
    m_chunk = NULL;
    m_mdl = NULL;
    m_map = NULL;
    m_model = NULL;
    m_scene = NULL;
    m_seq = NULL;
    m_seq_num = 0;
    m_anim = NULL;
    m_camera_anim = NULL;
    m_pltt_anim = NULL;
    m_pltt = NULL;
    m_light = NULL;
    m_event = NULL;
    m_coll = NULL;
    m_tex = NULL;
    for (int i = 0; i < 2; i++) {
        m_camera_no[i] = 0;
        SetCameraPos(0, 0, 0, i);
        SetCameraCent(0, 0, FX32_ONE, i);
        SetCameraUp(0, FX32_ONE, 0, i);
    }
    m_frame_flip_flop = 0;
    m_vanish_flag = 0;
    m_common = NULL;
    m_bbox_far = FX32_ONE * 60;
    m_simple_bby = 1;
    m_box_test = 0;
    m_mat_alpha = NULL;
    m_event_anim_id = 0;
    m_scale.x = m_scale.y = m_scale.z = FX32_ONE;
}

ARM int FLDObject::SetCameraNo(int no, int screen)
{
    FLD_ANIM* anim = NULL;
    if (m_camera_anim == NULL || no > m_camera_anim->num || no < 0) {
        SetCameraPos(0, 0, 0, screen);
        SetCameraCent(0, 0, FX32_ONE, screen);
        SetCameraUp(0, FX32_ONE, 0, screen);
        return 0;
    }
    m_camera_no[screen] = no;
    if (no > 0) {
        anim = (FLD_ANIM*)m_camera_anim->res[no - 1];
    }
    if (anim != NULL && anim->loop != 0 && anim->key_num != 0) {
        anim->loop = anim->loop_start;
        anim->wait = anim->wait_start;
        anim->frame = 0;
        s_cameraNo = screen;
        applyCameraKey(anim->key, anim->key, 0, this);
    } else {
        SetCameraPos(0, 0, 0, screen);
        SetCameraCent(0, 0, FX32_ONE, screen);
        SetCameraUp(0, FX32_ONE, 0, screen);
    }
    return 1;
}

ARM int FLDObject::Setup(void* mdl, void* data, void* coll, int heap)
{
    int i;
    if (coll != NULL) {
        coll = (unsigned char*)coll + 0x10;
    }
    if (m_flag & 1) {
        Final();
    }
    func_02068a44(&m_allocator, heap, 4);
    m_chunk = (FLD_CHUNK*)((unsigned char*)data + 0x10);
    m_mdl = mdl;
    FLD_CHUNK* chunk = m_chunk;
    while (chunk->id != 0) {
        switch (chunk->id) {
        case FLD_ID('F', 'M', 'A', 'P'):
            m_map = (FLD_MAP*)chunk;
            ((FLD_MAP*)chunk)->order = (short**)&((FLD_MAP*)chunk)->obj[((FLD_MAP*)chunk)->obj_num];
            m_map->index = (short*)&m_map->order[m_map->obj_num];
            break;
        case FLD_ID('F', 'M', 'D', 'L'):
            m_model = (FLD_MODEL_TBL*)chunk;
            break;
        case FLD_ID('F', 'S', 'Q', 'd'):
            if (m_seq_num < 0x80) {
                m_seq_data[m_seq_num] = (FLD_RES_TBL*)chunk;
                m_seq_num++;
                for (i = 0; i < ((FLD_RES_TBL*)chunk)->num; i++) {
                    ((FLD_RES_TBL*)chunk)->res[i] = (unsigned char*)data + (int)((FLD_RES_TBL*)chunk)->res[i];
                }
            }
            break;
        case FLD_ID('F', 'P', 'M', 'v'):
            m_anim = (FLD_RES_TBL*)chunk;
            for (i = 0; i < ((FLD_RES_TBL*)chunk)->num; i++) {
                ((FLD_RES_TBL*)chunk)->res[i] = (unsigned char*)data + (int)((FLD_RES_TBL*)chunk)->res[i];
            }
            break;
        case FLD_ID('F', 'E', 'M', 'v'):
            m_camera_anim = (FLD_RES_TBL*)chunk;
            for (i = 0; i < ((FLD_RES_TBL*)chunk)->num; i++) {
                ((FLD_RES_TBL*)chunk)->res[i] = (unsigned char*)data + (int)((FLD_RES_TBL*)chunk)->res[i];
            }
            break;
        case FLD_ID('F', 'P', 'A', 'M'):
            m_pltt_anim = (FLD_RES_TBL*)chunk;
            for (i = 0; i < ((FLD_RES_TBL*)chunk)->num; i++) {
                ((FLD_RES_TBL*)chunk)->res[i] = (unsigned char*)data + (int)((FLD_RES_TBL*)chunk)->res[i];
            }
            break;
        case FLD_ID('F', 'P', 'L', 'P'):
            m_pltt = (FLD_PLTT_TBL*)chunk;
            break;
        case FLD_ID('F', 'C', 'A', 'N'): {
            FLD_COMMON_KEY* key;
            m_common = (FLD_COMMON_TBL*)chunk;
            key = (FLD_COMMON_KEY*)&m_common->anim[m_common->num];
            for (i = 0; i < m_common->num; i++) {
                m_common->anim[i].key = key;
                key += m_common->anim[i].num;
            }
            break;
        }
        case FLD_ID('F', 'S', 'R', 'A'):
            m_light = (FLD_RES_TBL*)chunk;
            for (i = 0; i < ((FLD_RES_TBL*)chunk)->num; i++) {
                ((FLD_RES_TBL*)chunk)->res[i] = (unsigned char*)data + (int)((FLD_RES_TBL*)chunk)->res[i];
            }
            break;
        case FLD_ID('F', 'E', 'A', 'N'):
            m_event = (FLD_EVENT_TBL*)chunk;
            break;
        case FLD_ID('F', 'E', 'T', 'C'):
            m_scene = (FLD_SCENE*)chunk;
            break;
        case FLD_ID('F', 'S', 'E', 'q'):
            m_seq = (FLD_SEQ_TBL*)chunk;
            break;
        case FLD_ID('F', 'A', 'T', 'B'):
        case FLD_ID('F', 'T', 'T', 'B'):
        case FLD_ID('F', 'N', 'A', 'M'):
        case FLD_ID('F', 'H', 'D', 'R'):
        case FLD_ID('F', 'C', 'R', 'S'):
        case FLD_ID('F', 'C', 'R', 'T'):
        case FLD_ID('F', 'T', 'E', 'X'):
        case FLD_ID('F', 'P', 'T', 'X'):
        case FLD_ID('F', 'F', 'O', 'g'):
        case FLD_ID('F', 'L', 'G', 't'):
            break;
        }
        chunk = (FLD_CHUNK*)((unsigned char*)chunk + chunk->size + sizeof(FLD_CHUNK));
    }

    FLD_MODEL_TBL* model = m_model;
    FLD_MAP* map = m_map;
    m_mat_alpha = (unsigned char**)func_02068a1c(&m_allocator, model->num * sizeof(unsigned char*));
    memset(m_mat_alpha, 0, model->num * sizeof(unsigned char*));
    for (int i = 0; i < model->num; i++) {
        FLD_MODEL* info = &model->model[i];
        info->mdl = NNS_G3dGetMdlByIdx(func_0206e8c0(mdl), i);
        info->unk_18 = (unsigned char*)data + (int)info->unk_18;
        info->unk_14 = 0;
        if (info->fade != NULL) {
            info->fade = (FLD_FADE*)((unsigned char*)data + (int)info->fade);
        }
        if (info->unk_1c != NULL) {
            info->unk_1c = (unsigned char*)data + (int)info->unk_1c;
        }
        if (info->mdl->info.numMat != 0) {
            m_mat_alpha[i] = (unsigned char*)func_02068a1c(&m_allocator, info->mdl->info.numMat);
            for (unsigned int j = 0; j < info->mdl->info.numMat; j++) {
                m_mat_alpha[i][j] = func_0206e630(info->mdl, j);
            }
        }
    }

    for (i = 0; i < map->obj_num; i++) {
        FLD_MAP_OBJ* obj = &map->obj[i];
        FLD_MODEL* info = &model->model[obj->model];
        obj->model_info = info;
        obj->render = (FLD_RENDER*)func_02068a1c(&m_allocator, sizeof(FLD_RENDER));
        if (obj->render == NULL) {
            Final();
            return 0;
        }
        obj->render->render_obj = func_0206e3d8(&m_allocator);
        func_0206a34c(obj->render->render_obj, info->mdl);
        obj->render->alpha = func_0206e630(obj->render->render_obj->resMdl, 0);
        if (info->flag & 0x200) {
            obj->flag |= 0x100000;
        }
        if (info->flag & 0x400) {
            obj->flag |= 0x200000;
        }
        NNSG3dResMdl* resMdl = info->mdl;
        obj->render->box_x = resMdl->info.boxX;
        obj->render->box_y = resMdl->info.boxY;
        obj->render->box_w = resMdl->info.boxW;
        obj->render->box_h = resMdl->info.boxH;
        obj->render->box_d = resMdl->info.boxD;
        obj->render->box_scale = resMdl->info.boxPosScale;
        int z = resMdl->info.boxZ - resMdl->info.boxD;
        if (z <= -0x8000 || z >= 0x8000) {
            obj->render->box_z = z >> 1;
            obj->render->box_x >>= 1;
            obj->render->box_y >>= 1;
            obj->render->box_w >>= 1;
            obj->render->box_h >>= 1;
            obj->render->box_d >>= 1;
            obj->render->box_scale <<= 1;
        } else {
            obj->render->box_z = z;
        }
        obj->priority <<= 4;
        obj->render->flag = 7;
        calcMapObjRender(obj, NULL);
    }

    sortMapObjOrder();
    m_tex = createPlttTexCopy();

    unsigned int mdlNum;
    FLD_RES_TBL* light = m_light;
    if (light != NULL) {
        NNSG3dResMdlSet* mdlSet = func_0206e8c0(m_mdl);
        mdlNum = mdlSet->dict.numEntry;
        FLD_MAT_LIST* list = (FLD_MAT_LIST*)func_02068a1c(&m_allocator, mdlNum * 8 + 4);
        if (list == NULL) {
            for (i = 0; i < light->num; i++) {
                ((FLD_LIGHT*)light->res[i])->mat_list = NULL;
            }
            Final();
            return 0;
        }
        for (i = 0; i < light->num; i++) {
            FLD_LIGHT* l = (FLD_LIGHT*)light->res[i];
            list->num = 0;
            for (unsigned int j = 0; j < mdlNum; j++) {
                char name[0x10];
                NNSG3dResMdl* resMdl = NNS_G3dGetMdlByIdx(mdlSet, j);
                NNSG3dResMat* mat = NNS_G3dGetMat(resMdl);
                MI_CpuSet(name, 0, sizeof(name));
                MI_CpuCopyU8(l->name, name, sizeof(name));
                int idx = func_0206e7a0(&mat->dict, name);
                if (idx != -1) {
                    list->list[list->num].mat = idx;
                    list->list[list->num].mdl = resMdl;
                    list->num++;
                }
            }
            if (list->num != 0) {
                l->mat_list = (FLD_MAT_LIST*)func_02068a1c(&m_allocator, list->num * 8 + 4);
                if (l->mat_list == NULL) {
                    Final();
                    break;
                }
                MI_CpuCopyU8(list, l->mat_list, list->num * 8 + 4);
            }
        }
        if (i < light->num) {
            for (; i < light->num; i++) {
                ((FLD_LIGHT*)light->res[i])->mat_list = NULL;
            }
            func_02068a30(&m_allocator, list);
            return 0;
        }
        func_02068a30(&m_allocator, list);
    }

    m_coll = (COLL_HEADER*)coll;
    if (!coll_init((COLL_HEADER*)coll, &m_allocator)) {
        Final();
        return 0;
    }
    func_0206dcb0(&m_ge_buffer);
    m_flag |= 1;
    return 1;
}

ARM static int compareObjPriority(void* a, void* b)
{
    return ((FLD_OBJ_UID*)a)->uid - ((FLD_OBJ_UID*)b)->uid;
}

ARM void FLDObject::sortMapObjOrder()
{
    int i;
    FLD_OBJ_UID* uid = (FLD_OBJ_UID*)m_map->order;
    for (i = 0; i < m_map->obj_num; i++) {
        uid[i].uid = m_map->obj[i].priority;
        uid[i].obj_id = i;
    }
    int num = m_map->obj_num;
    void* stack = func_02068a1c(&m_allocator, MATH_QSORT_STACK_SIZE(num));

    if (stack == NULL) {
        for (i = 0; i < m_map->obj_num; i++) {
            m_map->order[i] = &m_map->index[i];
        }
        return;
    }
    func_02066e34(uid, m_map->obj_num, sizeof(FLD_OBJ_UID), compareObjPriority, stack);
    func_02068a30(&m_allocator, stack);
    for (i = 0; i < m_map->obj_num; i++) {
        m_map->order[i] = &m_map->index[uid[i].obj_id];
    }
}

ARM static void calcMapObjRender(FLD_MAP_OBJ* obj, VecFx32* ofs)
{
    FLD_RENDER* render = obj->render;
    if (render->flag & 8) {
        obj->pos.x = render->trans.x;
        obj->pos.y = render->trans.y;
        obj->pos.z = render->trans.z;
    } else if (render->flag & 1) {
        render->trans.x = obj->pos.x;
        render->trans.y = obj->pos.y;
        render->trans.z = obj->pos.z;
    }
    if (render->flag & 0x10) {
        obj->scale.x = render->scale.x;
        obj->scale.y = render->scale.y;
        obj->scale.z = render->scale.z;
    } else if (render->flag & 2) {
        render->scale.x = obj->scale.x;
        render->scale.y = obj->scale.y;
        render->scale.z = obj->scale.z;
    }
    if (render->flag & 0x20) {
        obj->rot.x = render->rot.x;
        obj->rot.y = render->rot.y;
        obj->rot.z = render->rot.z;
    } else if (render->flag & 4) {
        render->rot.x = obj->rot.x;
        render->rot.y = obj->rot.y;
        render->rot.z = obj->rot.z;
    }
    MtxFx33 mtx;
    MtxFx33 rot;
    func_02061b88(&rot);
    func_02061c88(&rot, FX_SinIdx((unsigned short)render->rot.y), FX_CosIdx((unsigned short)render->rot.y));
    func_02061b88(&mtx);
    func_02061c6c(&mtx, FX_SinIdx((unsigned short)render->rot.x), FX_CosIdx((unsigned short)render->rot.x));
    MTX_Concat33(&mtx, &rot, &rot);
    func_02061b88(&mtx);
    func_02061ca4(&mtx, FX_SinIdx((unsigned short)render->rot.z), FX_CosIdx((unsigned short)render->rot.z));
    MTX_Concat33(&mtx, &rot, &mtx);
    if (ofs != NULL) {
        VecFx32 v;
        func_02061edc(ofs, &mtx, &v);
        func_02062f64(&v, &render->trans, &render->trans);
    }
    render->mtx = mtx;
    MtxFx33 scale;
    fx32 s = render->box_scale;
    func_02061bac(&mtx, &scale, FX_Mul(render->scale.x, s), FX_Mul(render->scale.y, s), FX_Mul(render->scale.z, s));
    VecFx32 box;
    VecFx32 min;
    box.x = render->box_x;
    box.y = render->box_y;
    box.z = render->box_z;
    func_02061edc(&box, &scale, &min);
    box.x += render->box_w >> 1;
    box.y += render->box_h >> 1;
    box.z += render->box_d >> 1;
    func_02061edc(&box, &scale, &render->center);
    render->radius2 = FX_Mul(render->center.x - min.x, render->center.x - min.x) + FX_Mul(render->center.y - min.y, render->center.y - min.y) + FX_Mul(render->center.z - min.z, render->center.z - min.z);
    render->radius = FX_Sqrt(render->radius2);
    func_02062f64(&render->center, &render->trans, &render->center);
    render->flag = 0;
}

ARM static int updateFldAnim(FLD_ANIM* anim, FLD_ANIM_FUNC func, void* work)
{
    unsigned char* keys = (unsigned char*)anim->key;
    if (anim->flag & 4) {
        keys = (unsigned char*)((FLD_MAP_OBJ*)work)->render->keys;
    }
    int ret = -1;
    int num = anim->key_num;
    if (num != 0 && anim->loop_start != 0) {
        if (anim->wait > 0) {
            ret = func(keys, keys, 0, work);
            anim->wait -= s_frameStep;
        } else {
            unsigned char* key0;
            unsigned char* key1;
            fx32 rate;
            if (anim->frame == anim->frame_max) {
                key1 = keys + (num - 1) * anim->key_size;
                key0 = key1;
            } else {
                int size = anim->key_size;
                int lo = 0;
                while (lo < num) {
                    int mid = (lo + num) >> 1;
                    if (*(int*)(keys + mid * size) < anim->frame) {
                        lo = mid + 1;
                    } else {
                        num = mid;
                    }
                }
                key1 = keys + lo * size;
                key0 = key1;
                if (*(int*)key1 > anim->frame) {
                    key0 = key1 + -1 * size;
                } else if (*(int*)key1 < anim->frame) {
                    key1 += size;
                }
            }
            if (key0 == key1) {
                rate = 0;
            } else {
                rate = FX_Divide((anim->frame - *(int*)key0) << FX32_SHIFT, (*(int*)key1 - *(int*)key0) << FX32_SHIFT);
                if (rate == FX32_ONE) {
                    key0 = key1;
                    rate = 0;
                } else if (rate == 0) {
                    key1 = key0;
                }
            }
            ret = func(key0, key1, rate, work);
            if (!(anim->flag & 2)) {
                anim->frame += s_frameStep;
            }
            if (anim->frame > anim->frame_max) {
                if (anim->loop > 0) {
                    anim->loop--;
                }
                if (anim->loop == 0) {
                    anim->frame = anim->frame_max;
                } else {
                    anim->frame -= anim->frame_max;
                }
                anim->wait = anim->wait_init;
            }
        }
    }
    return ret;
}

ARM static int applyCameraKey(void* key0, void* key1, fx32 rate, void* work)
{
    FLD_CAMERA_KEY* k0 = (FLD_CAMERA_KEY*)key0;
    FLD_CAMERA_KEY* k1 = (FLD_CAMERA_KEY*)key1;
    FLDObject* fld = (FLDObject*)work;
    int no = s_cameraNo;
    fld->SetCameraPos(FLD_LERP(k0->pos.x, k1->pos.x, rate), FLD_LERP(k0->pos.y, k1->pos.y, rate), FLD_LERP(k0->pos.z, k1->pos.z, rate), no);
    fld->SetCameraCent(FLD_LERP(k0->tag.x, k1->tag.x, rate), FLD_LERP(k0->tag.y, k1->tag.y, rate), FLD_LERP(k0->tag.z, k1->tag.z, rate), no);
    fld->SetCameraUp(FLD_LERP(k0->vec.x, k1->vec.x, rate), FLD_LERP(k0->vec.y, k1->vec.y, rate), FLD_LERP(k0->vec.z, k1->vec.z, rate), no);
    return 0;
}

ARM void FLDObject::SetCameraPos(fx32 x, fx32 y, fx32 z, int screen)
{
    m_x32_camera_pos[screen].x = x;
    m_x32_camera_pos[screen].y = y;
    m_x32_camera_pos[screen].z = z;
}

ARM void FLDObject::SetCameraCent(fx32 x, fx32 y, fx32 z, int screen)
{
    m_x32_camera_tag[screen].x = x;
    m_x32_camera_tag[screen].y = y;
    m_x32_camera_tag[screen].z = z;
}

ARM void FLDObject::SetCameraUp(fx32 x, fx32 y, fx32 z, int screen)
{
    m_x32_camera_vec[screen].x = x;
    m_x32_camera_vec[screen].y = y;
    m_x32_camera_vec[screen].z = z;
}

ARM void FLDObject::updateCameraAnim(int screen)
{
    FLD_RES_TBL* camera = m_camera_anim;
    int no = m_camera_no[screen];
    if (camera == NULL || no == 0) {
        return;
    }
    if (no > camera->num) {
        return;
    }
    FLD_ANIM* anim = (FLD_ANIM*)camera->res[no - 1];
    s_cameraNo = screen;
    updateFldAnim(anim, applyCameraKey, this);
}

ARM static int applyObjKey(void* key0, void* key1, fx32 rate, void* work)
{
    FLD_OBJ_KEY* k0 = (FLD_OBJ_KEY*)key0;
    FLD_OBJ_KEY* k1 = (FLD_OBJ_KEY*)key1;
    FLD_MAP_OBJ* obj = (FLD_MAP_OBJ*)work;
    if (rate == 0) {
        obj->render->trans.x = obj->pos.x + k0->pos.x;
        obj->render->trans.y = obj->pos.y + k0->pos.y;
        obj->render->trans.z = obj->pos.z + k0->pos.z;
        obj->render->rot.x = obj->rot.x + k0->rot.x;
        obj->render->rot.y = obj->rot.y + k0->rot.y;
        obj->render->rot.z = obj->rot.z + k0->rot.z;
        obj->render->scale.x = FX_Mul(obj->scale.x, k0->scale.x);
        obj->render->scale.y = FX_Mul(obj->scale.y, k0->scale.y);
        obj->render->scale.z = FX_Mul(obj->scale.z, k0->scale.z);
    } else {
        obj->render->trans.x = obj->pos.x + FX_Mul(k1->pos.x - k0->pos.x, rate) + k0->pos.x;
        obj->render->trans.y = obj->pos.y + FX_Mul(k1->pos.y - k0->pos.y, rate) + k0->pos.y;
        obj->render->trans.z = obj->pos.z + FX_Mul(k1->pos.z - k0->pos.z, rate) + k0->pos.z;
        obj->render->rot.x = obj->rot.x + FX_Mul(k1->rot.x - k0->rot.x, rate) + k0->rot.x;
        obj->render->rot.y = obj->rot.y + FX_Mul(k1->rot.y - k0->rot.y, rate) + k0->rot.y;
        obj->render->rot.z = obj->rot.z + FX_Mul(k1->rot.z - k0->rot.z, rate) + k0->rot.z;
        obj->render->scale.x = FX_Mul(obj->scale.x, FLD_LERP(k0->scale.x, k1->scale.x, rate));
        obj->render->scale.y = FX_Mul(obj->scale.y, FLD_LERP(k0->scale.y, k1->scale.y, rate));
        obj->render->scale.z = FX_Mul(obj->scale.z, FLD_LERP(k0->scale.z, k1->scale.z, rate));
    }
    obj->render->flag = 0;
    calcMapObjRender(obj, NULL);
    return 0;
}

ARM static int applyObjKeyRotOffset(void* key0, void* key1, fx32 rate, void* work)
{
    FLD_OBJ_KEY* k0 = (FLD_OBJ_KEY*)key0;
    FLD_OBJ_KEY* k1 = (FLD_OBJ_KEY*)key1;
    FLD_MAP_OBJ* obj = (FLD_MAP_OBJ*)work;
    VecFx32 ofs;
    if (rate == 0) {
        ofs.x = k0->pos.x;
        ofs.y = k0->pos.y;
        ofs.z = k0->pos.z;
        obj->render->trans.x = obj->pos.x;
        obj->render->trans.y = obj->pos.y;
        obj->render->trans.z = obj->pos.z;
        obj->render->rot.x = obj->rot.x + k0->rot.x;
        obj->render->rot.y = obj->rot.y + k0->rot.y;
        obj->render->rot.z = obj->rot.z + k0->rot.z;
        obj->render->scale.x = FX_Mul(obj->scale.x, k0->scale.x);
        obj->render->scale.y = FX_Mul(obj->scale.y, k0->scale.y);
        obj->render->scale.z = FX_Mul(obj->scale.z, k0->scale.z);
    } else {
        ofs.x = FLD_LERP(k0->pos.x, k1->pos.x, rate);
        ofs.y = FLD_LERP(k0->pos.y, k1->pos.y, rate);
        ofs.z = FLD_LERP(k0->pos.z, k1->pos.z, rate);
        obj->render->trans.x = obj->pos.x;
        obj->render->trans.y = obj->pos.y;
        obj->render->trans.z = obj->pos.z;
        obj->render->rot.x = obj->rot.x + FX_Mul(k1->rot.x - k0->rot.x, rate) + k0->rot.x;
        obj->render->rot.y = obj->rot.y + FX_Mul(k1->rot.y - k0->rot.y, rate) + k0->rot.y;
        obj->render->rot.z = obj->rot.z + FX_Mul(k1->rot.z - k0->rot.z, rate) + k0->rot.z;
        obj->render->scale.x = FX_Mul(obj->scale.x, FLD_LERP(k0->scale.x, k1->scale.x, rate));
        obj->render->scale.y = FX_Mul(obj->scale.y, FLD_LERP(k0->scale.y, k1->scale.y, rate));
        obj->render->scale.z = FX_Mul(obj->scale.z, FLD_LERP(k0->scale.z, k1->scale.z, rate));
    }
    obj->render->flag = 0;
    calcMapObjRender(obj, &ofs);
    return 0;
}

ARM void FLDObject::updateMapObjAnim(int no)
{
    short index = m_map->index[no];
    FLD_MAP_OBJ* obj = &m_map->obj[index];
    if (index == -1) {
        return;
    }
    if (obj->flag & 0x1000000) {
        return;
    }
    if (obj->anim < 0) {
        return;
    }
    FLD_ANIM* anim = (FLD_ANIM*)m_anim->res[obj->anim];
    if (anim->flag & 4) {
        anim = &obj->render->anim;
    }
    if (obj->flag & 0x400000) {
        anim->flag |= 2;
        if (anim->flag & 4) {
            updateFldAnim(anim, applyObjKeyRotOffset, obj);
        } else {
            updateFldAnim(anim, applyObjKey, obj);
        }
        anim->flag &= ~2;
    } else if (anim->flag & 4) {
        updateFldAnim(anim, applyObjKeyRotOffset, obj);
    } else {
        updateFldAnim(anim, applyObjKey, obj);
    }
}

ARM void FLDObject::updateMapObjAnims()
{
    FLD_MAP* map = m_map;
    if (m_anim == NULL) {
        return;
    }
    for (int i = 0; i < map->obj_num; i++) {
        updateMapObjAnim(i);
    }
}

ARM static int storeSeqKeys(void* key0, void* key1, fx32 rate, void* work)
{
    FLD_MAP_OBJ* obj = (FLD_MAP_OBJ*)work;
    obj->render->key0 = key0;
    obj->render->key1 = key1;
    obj->render->rate = rate;
    return 0;
}

ARM static int applySeqKey(void* key0, void* key1, fx32 rate, void* work)
{
    FLD_SEQ_KEY* k0 = (FLD_SEQ_KEY*)key0;
    FLD_SEQ_KEY* k1 = (FLD_SEQ_KEY*)key1;
    FLD_MAP_OBJ* obj = (FLD_MAP_OBJ*)work;
    obj->render->key0 = key0;
    obj->render->key1 = key1;
    obj->render->rate = rate;
    if (rate == 0) {
        obj->render->trans.x = obj->pos.x + k0->pos.x;
        obj->render->trans.y = obj->pos.y + k0->pos.y;
        obj->render->trans.z = obj->pos.z + k0->pos.z;
        obj->render->rot.x = obj->rot.x + k0->rot.x;
        obj->render->rot.y = obj->rot.y + k0->rot.y;
        obj->render->rot.z = obj->rot.z + k0->rot.z;
        obj->render->scale.x = FX_Mul(obj->scale.x, k0->scale.x);
        obj->render->scale.y = FX_Mul(obj->scale.y, k0->scale.y);
        obj->render->scale.z = FX_Mul(obj->scale.z, k0->scale.z);
    } else {
        obj->render->trans.x = obj->pos.x + FX_Mul(k1->pos.x - k0->pos.x, rate) + k0->pos.x;
        obj->render->trans.y = obj->pos.y + FX_Mul(k1->pos.y - k0->pos.y, rate) + k0->pos.y;
        obj->render->trans.z = obj->pos.z + FX_Mul(k1->pos.z - k0->pos.z, rate) + k0->pos.z;
        obj->render->rot.x = obj->rot.x + FX_Mul(k1->rot.x - k0->rot.x, rate) + k0->rot.x;
        obj->render->rot.y = obj->rot.y + FX_Mul(k1->rot.y - k0->rot.y, rate) + k0->rot.y;
        obj->render->rot.z = obj->rot.z + FX_Mul(k1->rot.z - k0->rot.z, rate) + k0->rot.z;
        obj->render->scale.x = FX_Mul(obj->scale.x, FLD_LERP(k0->scale.x, k1->scale.x, rate));
        obj->render->scale.y = FX_Mul(obj->scale.y, FLD_LERP(k0->scale.y, k1->scale.y, rate));
        obj->render->scale.z = FX_Mul(obj->scale.z, FLD_LERP(k0->scale.z, k1->scale.z, rate));
    }
    obj->render->flag = 0;
    calcMapObjRender(obj, NULL);
    return 0;
}

ARM static int applyShapeMorph(void* key0, void* key1, fx32 rate, void* work)
{
    FLD_SEQ_KEY* k0;
    FLD_SEQ_KEY* k1;
    FLD_MAP_OBJ* obj;
    int num;
    NNSG3dResShp* shp;
    int i;
    int j;
    NNSG3dResShpData* s0;
    NNSG3dResShpData* s1;
    NNSG3dResShpData* sd;
    unsigned int* dl0;
    unsigned int* dld;
    unsigned int* dl1;
    long pos;
    unsigned int c0, c1, x0, x1, col;
    unsigned int size;
    unsigned int cmd;
    k0 = (FLD_SEQ_KEY*)key0;
    k1 = (FLD_SEQ_KEY*)key1;
    obj = (FLD_MAP_OBJ*)work;
    num = obj->model_info->shape_num;
    if (obj->model_info->shape0 == k0->shape && obj->model_info->shape1 == k1->shape && obj->model_info->shape_rate == rate) {
        return 0;
    }
    obj->model_info->shape0 = k0->shape;
    obj->model_info->shape1 = k1->shape;
    obj->model_info->shape_rate = rate;
    shp = NNS_G3dGetShp(obj->render->render_obj->resMdl);
    for (i = 0; i < num; i++) {
        s0 = NNS_G3dGetShpDataByIdx(shp, num * (k0->shape + 2) + i);
        sd = NNS_G3dGetShpDataByIdx(shp, i);
        s1 = NNS_G3dGetShpDataByIdx(shp, num * (k1->shape + 2) + i);
        dl0 = (unsigned int*)((unsigned char*)s0 + s0->ofsDL);
        dld = (unsigned int*)((unsigned char*)sd + sd->ofsDL);
        dl1 = (unsigned int*)((unsigned char*)s1 + s1->ofsDL);
        size = NNS_G3dGetShpDLSize(sd);
        cmd = dld[0];
        pos = 1;
        while (1) {
            for (j = 0; j < 4; j++) {
                switch ((unsigned char)cmd) {
                case 0x20:
                    FLD_LERP_RGB(dld[pos], dl0[pos], dl1[pos], rate);
                    pos++;
                    break;
                case 0x22:
                    FLD_LERP_ST(dld[pos], dl0[pos], dl1[pos], 8, rate);
                    pos++;
                    break;
                case 0x23:
                    FLD_LERP_XY(dld[pos], dl0[pos], dl1[pos], 0, rate);
                    c0 = dl0[pos + 1];
                    c1 = dl1[pos + 1];
                    if (c0 != c1) {
                        x0 = (short)c0;
                        x1 = (short)c1;
                        FLD_LERP_16(x0, x1, 0, rate);
                        c0 = (unsigned short)x0;
                    }
                    dld[pos + 1] = c0;
                    pos += 2;
                    break;
                case 0x40:
                    pos++;
                    break;
                case 0:
                case 0x41:
                    break;
                }
                cmd >>= 8;
            }
            if (pos >= size / 4) {
                break;
            }
            cmd = dld[pos];
            pos++;
        }
    }
    DC_CleanAll();
    return 0;
}

ARM void FLDObject::updateMapObjSeq(FLD_MAP_OBJ* obj)
{
    FLD_SEQ* seq;
    FLD_ANIM* anim;
    FLD_SEQ_KEY* keys;
    FLD_SEQ_KEY* last;
    int frame;
    int num;
    if (m_seq == NULL) {
        return;
    }
    if (obj->seq < 0) {
        return;
    }
    seq = &m_seq->seq[obj->seq];
    if (seq->data < 0) {
        return;
    }
    anim = (FLD_ANIM*)m_seq_data[seq->data]->res[seq->anim];
    anim->wait = seq->wait;
    anim->wait_init = seq->wait_init;
    anim->loop = seq->loop;
    anim->frame = seq->frame;
    anim->loop_start = seq->loop_start;
    num = anim->key_num;
    if (anim->flag & 1) {
        if (anim->flag & 8) {
            keys = (FLD_SEQ_KEY*)anim->key;
            last = &keys[num - 1];
            frame = last->frame;
            if (obj->flag & 2) {
                *last = keys[0];
            } else {
                *last = keys[num - 2];
            }
            keys[anim->key_num - 1].frame = frame;
        } else {
            FLD_SHAPE_KEY* keys = (FLD_SHAPE_KEY*)anim->key;
            int shape;
            if (obj->flag & 2) {
                shape = keys[0].shape;
            } else {
                shape = keys[num - 2].shape;
            }
            keys[num - 1].shape = shape;
        }
    } else {
        anim->key_num = num - 1;
    }
    FLD_ANIM_FUNC func = (anim->flag & 8) ? applySeqKey : storeSeqKeys;
    if (obj->flag & 0x400000) {
        anim->flag |= 2;
        updateFldAnim(anim, func, obj);
        anim->flag &= ~2;
    } else {
        updateFldAnim(anim, func, obj);
    }
    seq->wait = anim->wait;
    seq->wait_init = anim->wait_init;
    seq->loop = anim->loop;
    seq->frame = anim->frame;
    seq->loop_start = anim->loop_start;
    anim->key_num = num;
}

ARM void FLDObject::applyMapObjShapeMorph(FLD_MAP_OBJ* obj)
{
    if (m_seq == NULL) {
        return;
    }
    if (obj->seq < 0) {
        return;
    }
    if (m_seq->seq[obj->seq].data < 0) {
        return;
    }
    applyShapeMorph(obj->render->key0, obj->render->key1, obj->render->rate, obj);
}

ARM void FLDObject::updatePlttAnims()
{
    int i;
    FLD_PLTT_TBL* pltt = m_pltt;
    FLD_RES_TBL* anim = m_pltt_anim;
    FLD_PLTT_ENTRY* entry;
    FLD_PLTT_SET* set;
    int index;
    int wait;
    if (pltt == NULL || anim == NULL) {
        return;
    }
    for (i = 0; i < pltt->num; i++) {
        entry = &pltt->entry[i];
        set = (FLD_PLTT_SET*)anim->res[entry->anim];
        index = set->index;
        if (entry->event != 0) {
            if (entry->event == m_event_anim_id) {
                continue;
            }
            index = 0;
        }
        copyPlttAnimFrame(entry, set, index);
    }
    if (m_event_anim_id > 0) {
        for (i = 0; i < pltt->num; i++) {
            entry = &pltt->entry[i];
            if (entry->event == m_event_anim_id) {
                set = (FLD_PLTT_SET*)anim->res[entry->anim];
                copyPlttAnimFrame(entry, set, set->index);
            }
        }
    }
    for (i = 0; i < anim->num; i++) {
        FLD_PLTT_SET* set = (FLD_PLTT_SET*)anim->res[i];
        if (set->flag & 1) {
            wait = ((FLD_PLTT256*)set->data)[set->index].wait;
        } else {
            wait = ((FLD_PLTT16*)set->data)[set->index].wait;
        }
        set->timer += s_frameStep;
        if (set->timer >= wait) {
            set->timer -= wait;
            set->index++;
            if (set->index >= set->count) {
                set->index = 0;
            }
        }
    }
    m_flag |= 0x80;
}

ARM void FLDObject::copyPlttAnimFrame(FLD_PLTT_ENTRY* entry, FLD_PLTT_SET* set, int index)
{
    int ofs = m_tex->plttInfo.sizePltt;
    unsigned short* src;
    int size;
    if (set->flag & 1) {
        size = sizeof(((FLD_PLTT256*)set->data)[index].col);
        src = ((FLD_PLTT256*)set->data)[index].col;
    } else {
        size = sizeof(((FLD_PLTT16*)set->data)[index].col);
        src = ((FLD_PLTT16*)set->data)[index].col;
    }
    if (m_flag & 0x40) {
        MI_CpuCopyU8(src, entry->pltt + ofs * 4, size);
    } else {
        MI_CpuCopyU8(src, entry->pltt, size);
    }
}

ARM void FLDObject::applyRgbRateToPlttAnims()
{
    int i;
    FLD_PLTT_ENTRY* entry;
    FLD_PLTT_SET* set;
    int j;
    unsigned short* data;
    for (i = 0; i < m_pltt->num; i++) {
        entry = &m_pltt->entry[i];
        if (STD_CompareNString(entry->name, "tz_", 3) == 0) {
            continue;
        }
        set = (FLD_PLTT_SET*)m_pltt_anim->res[entry->anim];
        if (set->flag & 2) {
            continue;
        }
        if (set->flag & 1) {
            for (j = 0, data = (unsigned short*)set->data; j < set->count; j++, data += sizeof(FLD_PLTT256) / 2) {
                applyRgbRate(((FLD_PLTT256*)data)->col, ((FLD_PLTT256*)data)->col, 0x100);
            }
        } else {
            for (j = 0, data = (unsigned short*)set->data; j < set->count; j++, data += sizeof(FLD_PLTT16) / 2) {
                applyRgbRate(((FLD_PLTT16*)data)->col, ((FLD_PLTT16*)data)->col, 0x10);
            }
        }
        set->flag |= 2;
    }
}

ARM static int applyLightKey(void* key0, void* key1, fx32 rate, void* work)
{
    FLD_LIGHT_KEY* k0 = (FLD_LIGHT_KEY*)key0;
    FLD_LIGHT_KEY* k1 = (FLD_LIGHT_KEY*)key1;
    FLD_LIGHT* light = (FLD_LIGHT*)work;
    unsigned short diff;
    unsigned short amb;
    unsigned short spec;
    unsigned short emi;
    int alpha;
    unsigned int i;
    NNSG3dResMdl* mdl;
    if (k0 == k1) {
        amb = k0->amb;
        diff = k0->diff;
        spec = k0->spec;
        emi = k0->emi;
        alpha = k0->alpha;
    } else {
        unsigned int c0, c1, x0, x1, col;
        FLD_LERP_RGB(amb, k0->amb, k1->amb, rate);
        FLD_LERP_RGB(spec, k0->spec, k1->spec, rate);
        FLD_LERP_RGB(emi, k0->emi, k1->emi, rate);
        FLD_LERP_RGB(diff, k0->diff, k1->diff, rate);
        c0 = k0->alpha;
        c1 = k1->alpha;
        if (c0 != c1) {
            x0 = c0 & 0x1f;
            x1 = c1 & 0x1f;
            FLD_LERP_5(x0, x1, rate);
            c0 = x0 & 0x1f;
        }
        alpha = c0;
    }
    FLD_MAT_LIST* list = light->mat_list;
    if (list != NULL) {
        for (i = 0; i < list->num; i++) {
            mdl = list->list[i].mdl;
            int mat = list->list[i].mat;
            func_0206e528(mdl, mat, amb);
            func_0206e56c(mdl, mat, spec);
            func_0206e5b0(mdl, mat, emi);
            func_0206e4e4(mdl, mat, diff);
            func_0206e5f4(mdl, mat, alpha);
        }
    }
    return 0;
}

ARM void FLDObject::updateLightAnims()
{
    FLD_RES_TBL* light = m_light;
    if (light == NULL) {
        return;
    }
    for (int i = 0; i < light->num; i++) {
        FLD_LIGHT* l = (FLD_LIGHT*)light->res[i];
        updateFldAnim(&l->anim, applyLightKey, l);
    }
}

ARM void FLDObject::calc()
{
    if (!(m_flag & 1)) {
        return;
    }
    if (m_flag & 4) {
        return;
    }
    m_frame_flip_flop ^= 1;
    if (m_frame_flip_flop == 0) {
        s_frameStep = 2;
    } else {
        s_frameStep = 0;
    }
    updateCameraAnim(0);
    if (m_camera_no[0] != m_camera_no[1]) {
        updateCameraAnim(1);
    } else {
        m_x32_camera_pos[1].x = m_x32_camera_pos[0].x;
        m_x32_camera_pos[1].y = m_x32_camera_pos[0].y;
        m_x32_camera_pos[1].z = m_x32_camera_pos[0].z;
        m_x32_camera_tag[1].x = m_x32_camera_tag[0].x;
        m_x32_camera_tag[1].y = m_x32_camera_tag[0].y;
        m_x32_camera_tag[1].z = m_x32_camera_tag[0].z;
        m_x32_camera_vec[1].x = m_x32_camera_vec[0].x;
        m_x32_camera_vec[1].y = m_x32_camera_vec[0].y;
        m_x32_camera_vec[1].z = m_x32_camera_vec[0].z;
    }
    updateMapObjAnims();
    updatePlttAnims();
    updateLightAnims();
    if (m_flag & 0x100) {
        return;
    }
    if (m_flag & 0x40) {
        applyRgbRateToPltt();
    }
    if (m_flag & 0xc0) {
        transferPltt();
    }
}

ARM static int isMapObjInView(FLD_RENDER* render, fx32 far)
{
    fx32 dx = render->center.x - s_cameraPos.x;
    fx32 dz = render->center.z - s_cameraPos.z;
    if (FX_Mul(dx, dx) + FX_Mul(dz, dz) - render->radius2 >= far) {
        return 0;
    }
    VecFx32 pos;
    VecFx32 dir;
    func_02063330(render->radius, &s_plane0, &render->center, &pos);
    func_02062f98(&pos, &s_cameraPos, &dir);
    if (func_02062fcc(&dir, &s_plane0) < 0) {
        return 0;
    }
    func_02063330(render->radius, &s_plane1, &render->center, &pos);
    func_02062f98(&pos, &s_cameraPos, &dir);
    return func_02062fcc(&dir, &s_plane1) >= 0;
}

ARM static int boxTestMapObj(VecFx32* pos, VecFx32* rot, VecFx32* scale, VecFx32* box, fx32 rate)
{
    int result;
    G3_PushMtx();
    G3_Translate(pos->x, pos->y, pos->z);
    func_02065c60(FX_SinIdx((unsigned short)rot->y), FX_CosIdx((unsigned short)rot->y));
    func_02065c24(FX_SinIdx((unsigned short)rot->x), FX_CosIdx((unsigned short)rot->x));
    func_02065c9c(FX_SinIdx((unsigned short)rot->z), FX_CosIdx((unsigned short)rot->z));
    G3_Scale(scale->x, scale->y, scale->z);
    G3_Scale(rate, rate, rate);
    G3_WRITE(0x70, box->x);
    G3_WRITE(0x70, box->y);
    G3_WRITE(0x70, box->z);
    while (func_02065544(&result)) {
    }
    G3_PopMtx(1);
    return result;
}

ARM static void skipSbcBillboard(NNSG3dRS* rs, unsigned int opt)
{
    int len = 2;
    if (opt & 0x40) {
        len++;
    }
    if (opt & 0x20) {
        len++;
    }
    rs->c += len;
}

ARM static int calcMapObjFadeAlpha(FLD_MAP_OBJ* obj, FLD_MODEL* model)
{
    FLD_FADE* fade = model->fade;
    int alpha = 0x1f;
    fx32 dist = func_0206338c(&obj->render->trans, &data_0210cf28.camPos);
    if (dist < fade->end) {
        fx32 rate = 0;
        if (fade->end - fade->start > 0) {
            rate = FX_Divide(dist - fade->start, fade->end - fade->start);
            if (rate < 0) {
                rate = 0;
            } else if (rate >= FX32_ONE) {
                rate = FX32_ONE;
            }
        }
        alpha = (fade->min + FX_Mul(rate, FX32_ONE - fade->min)) >> 7;
        if (alpha < 0) {
            alpha = 0;
        } else if (alpha >= 0x1f) {
            alpha = 0x1f;
        }
    }
    return alpha;
}

ARM static void setMdlMatAlpha(NNSG3dResMdl* mdl, int alpha, unsigned char* base)
{
    for (unsigned int i = 0; i < mdl->info.numMat; i++) {
        func_0206e5f4(mdl, i, alpha * base[i] / 31);
    }
}

#define FLD_DRAW_OBJ()                                                                                       \
    if ((model->flag & 0x1f) != obj->render->alpha) {                                                        \
        model->flag = (model->flag & ~0x1f) | obj->render->alpha;                                            \
        setMdlMatAlpha(obj->render->render_obj->resMdl, obj->render->alpha, m_mat_alpha[obj->model]);      \
    }                                                                                                        \
    if (!(flag & 0x24) && state != 0) {                                                                      \
        state = 0;                                                                                           \
        data_020c3f5c[7] = skipSbcBillboard;                                                                 \
        data_020c3f5c[8] = skipSbcBillboard;                                                                 \
    } else if ((flag & 4) && state != 4) {                                                                   \
        if (m_simple_bby) {                                                                                  \
            state = 0;                                                                                       \
            data_020c3f5c[7] = skipSbcBillboard;                                                             \
            data_020c3f5c[8] = skipSbcBillboard;                                                             \
        } else {                                                                                             \
            state = 4;                                                                                       \
            data_020c3f5c[7] = G3d_SBCRender_008;                                                            \
            data_020c3f5c[8] = G3d_SBCRender_008;                                                            \
        }                                                                                                    \
    } else if ((flag & 0x20) && state != 0x20) {                                                             \
        state = 0x20;                                                                                        \
        data_020c3f5c[7] = G3d_SBCRender_007;                                                                \
        data_020c3f5c[8] = G3d_SBCRender_007;                                                                \
    }                                                                                                        \
    if (!(m_flag & 4)) {                                                                                     \
        applyMapObjShapeMorph(obj);                                                                               \
    }                                                                                                        \
    func_0206de34(0x17, &data_0210cf28.cameraMtx, 12);                                                                 \
    func_0206de34(0x1c, &obj->render->trans, 3);                                                             \
    if (m_simple_bby == 1 && (flag & 4)) {                                                                   \
        func_0206de34(0x1a, &rot, 9);                                                                        \
    } else {                                                                                                 \
        func_0206de34(0x1a, &obj->render->mtx, 9);                                                           \
    }                                                                                                        \
    func_0206de34(0x1b, &obj->render->scale, 3);                                                             \
    G3d_Render(obj->render->render_obj);

#define FLD_DRAW_OBJ2(obj)                                                                                       \
    if ((model->flag & 0x1f) != obj->render->alpha) {                                                        \
        model->flag = (model->flag & ~0x1f) | obj->render->alpha;                                            \
        setMdlMatAlpha(obj->render->render_obj->resMdl, obj->render->alpha, m_mat_alpha[obj->model]);      \
    }                                                                                                        \
    if (!(flag & 0x24) && state != 0) {                                                                      \
        state = 0;                                                                                           \
        data_020c3f5c[7] = skipSbcBillboard;                                                                 \
        data_020c3f5c[8] = skipSbcBillboard;                                                                 \
    } else if ((flag & 4) && state != 4) {                                                                   \
        if (m_simple_bby) {                                                                                  \
            state = 0;                                                                                       \
            data_020c3f5c[7] = skipSbcBillboard;                                                             \
            data_020c3f5c[8] = skipSbcBillboard;                                                             \
        } else {                                                                                             \
            state = 4;                                                                                       \
            data_020c3f5c[7] = G3d_SBCRender_008;                                                            \
            data_020c3f5c[8] = G3d_SBCRender_008;                                                            \
        }                                                                                                    \
    } else if ((flag & 0x20) && state != 0x20) {                                                             \
        state = 0x20;                                                                                        \
        data_020c3f5c[7] = G3d_SBCRender_007;                                                                \
        data_020c3f5c[8] = G3d_SBCRender_007;                                                                \
    }                                                                                                        \
    if (!(m_flag & 4)) {                                                                                     \
        applyMapObjShapeMorph(obj);                                                                               \
    }                                                                                                        \
    func_0206de34(0x17, &data_0210cf28.cameraMtx, 12);                                                                 \
    func_0206de34(0x1c, &obj->render->trans, 3);                                                             \
    if (m_simple_bby == 1 && (flag & 4)) {                                                                   \
        func_0206de34(0x1a, &rot, 9);                                                                        \
    } else {                                                                                                 \
        func_0206de34(0x1a, &obj->render->mtx, 9);                                                           \
    }                                                                                                        \
    func_0206de34(0x1b, &obj->render->scale, 3);                                                             \
    G3d_Render(obj->render->render_obj);

ARM void FLDObject::draw()
{
    MtxFx33 rot;
    MtxFx43 saved;
    int state;
    FLD_MAP* map;
    short** p;
    short** end;
    FLD_MAP_OBJ* robj;
    FLD_MODEL* model;
    int flag;
    FLD_MAP_OBJ* obj = NULL;
    int deferred;
    MtxFx43* cam;
    fx32 far2;
    map = m_map;
    state = -1;
    if (!(m_flag & 1)) {
        return;
    }
#ifdef DQIV_NATIVE
    // The opening's sky is made for the DS screen's width: widened, it covers
    // the joined picture instead of ending in black wedges at its sides.
    pc::StretchScope stretch(pc::unifiedFrame() && strcmp(g_Global.getMapName(), "ev01") == 0 ? pc::backdropStretch() : 1.0f);
#endif
    cam = &data_0210cf28.cameraMtx;
    end = map->order + map->obj_num;
    saved = *cam;
    func_02061fb4(&saved, cam, m_scale.x, m_scale.y, m_scale.z);
    if (m_simple_bby) {
        const VecFx32* pos = &data_0210cf28.camPos;
        const VecFx32* tgt = &data_0210cf28.camTarget;
        unsigned short rotY = FX_Atan2Idx(tgt->z - pos->z, tgt->x - pos->x) + 0x8000;
        func_02061b88(&rot);
        func_02061c88(&rot, FX_CosIdx(rotY), FX_SinIdx(rotY));
    }
    {
        VecFx32 scale = {FX32_ONE, FX32_ONE, FX32_ONE};
        VecFx32 trans = {0, 0, 0};
        MtxFx33 baseRot;
        func_02061b88(&baseRot);
        func_0206ae30(&scale);
        func_02067940(&baseRot, &data_0210cf28.prmBaseRot);
        data_0210cf28.flag &= ~0xa4;
        func_0206ae08((dss::Fix32Vector3*)&trans);
        func_0206adcc();
    }
    {
        VecFx32 far[4];
        VecFx32 near;
        VecFx32 v0;
        VecFx32 v1;
        int x1;
        int y1;
        int x2;
        int y2;
        fx32 z;
        func_0206b294(&x1, &y1, &x2, &y2);
        x1--;
        y1--;
        x2++;
        y2++;
        func_0206e154(x1, y1, &near, &far[0]);
        func_0206e154(x1, y2, &near, &far[1]);
        func_0206e154(x2, y1, &near, &far[2]);
        func_0206e154(x2, y2, &near, &far[3]);
        near.x = data_0210cf28.camPos.x;
        near.y = data_0210cf28.camPos.y;
        z = data_0210cf28.camPos.z;
        s_cameraPos.x = near.x;
        s_cameraPos.y = near.y;
        s_cameraPos.z = z;
        //wtf this match ??? 
        near.z = z;
        near.z = z;
        near.z = z;
        near.z = z;
        func_02062f98(&far[1], &near, &v0);
        func_02062f98(&far[0], &near, &v1);
        func_02063008(&v0, &v1, &s_plane0);
        func_020630ec(&s_plane0, &s_plane0);
        func_02062f98(&far[2], &near, &v0);
        func_02062f98(&far[3], &near, &v1);
        func_02063008(&v0, &v1, &s_plane1);
        func_020630ec(&s_plane1, &s_plane1);
    }
    m_target = data_0210cf28.camTarget;
    if (m_box_test == 0) {
        far2 = FX_Mul(m_bbox_far, m_bbox_far);
        for (p = map->order; p != end; p++) {
            if (**p == -1) {
                continue;
            }
            obj = &map->obj[**p];
            flag = obj->flag;
            if (flag & 0x1000000) {
                continue;
            }
            if (obj->render->flag != 0) {
                calcMapObjRender(obj, NULL);
            }
            if (!(m_flag & 4)) {
                updateMapObjSeq(obj);
            }
            if (flag & 0x2000000) {
                continue;
            }
            if (isMapObjInView(obj->render, far2)) {
                obj->flag &= ~0x20000;
            } else {
                obj->flag |= 0x20000;
            }
        }
    } else if (m_box_test == 1) {
        G3_MtxMode(3);
        G3_Identity();
        G3_MtxMode(2);
        G3_PolygonAttr(1, 0, 0xc0, 0, 0, 0x3000);
        G3_Begin(0);
        G3_End();
        for (p = map->order; p != end; p++) {
            if (**p == -1) {
                continue;
            }
            obj = &map->obj[**p];
            flag = obj->flag;
            if (flag & 0x1000000) {
                continue;
            }
            if (obj->render->flag != 0) {
                calcMapObjRender(obj, NULL);
            }
            if (!(m_flag & 4)) {
                updateMapObjSeq(obj);
            }
            if (flag & 0x2000000) {
                continue;
            }
            if (boxTestMapObj(&obj->render->trans, &obj->render->rot, &obj->render->scale, (VecFx32*)&obj->render->box_x, obj->render->box_scale)) {
                obj->flag &= ~0x20000;
            } else {
                obj->flag |= 0x20000;
            }
        }
    } else if (m_box_test == 2) {
        if (mainCameraFlag_ == 1) {
            m_far_town = 0x79000;
        } else {
            m_far_town = 0xe1000;
            m_target.y -= 0x5659;
        }
        for (p = map->order; p != end; p++) {
            if (**p == -1) {
                continue;
            }
            obj = &map->obj[**p];
            flag = obj->flag;
            if (flag & 0x1000000) {
                continue;
            }
            if (obj->render->flag != 0) {
                calcMapObjRender(obj, NULL);
            }
            if (!(m_flag & 4)) {
                updateMapObjSeq(obj);
            }
            if (flag & 0x2000000) {
                continue;
            }
#ifdef DQIV_NATIVE
            // The distance cull is tuned for one screen; the unified picture shows both.
            if (pc::unifiedFrame() || isMapObjInTownView(obj->render)) {
#else
            if (isMapObjInTownView(obj->render)) {
#endif
                obj->flag &= ~0x20000;
            } else {
                obj->flag |= 0x20000;
            }
        }
    }
    deferred = 0;
    for (p = map->order; p != end; p++) {
        if (**p == -1) {
            continue;
        }
        robj = &map->obj[**p];
        flag = robj->flag;
        model = robj->model_info;
        if (flag & 0x3000000) {
            continue;
        }
        if (flag & 0x20000) {
            continue;
        }
        if (flag & 0x300000) {
            if (m_vanish_flag == 0) {
                robj->render->alpha = 0x1f;
            } else {
                robj->render->alpha = calcMapObjFadeAlpha(robj, model);
            }
        }
        if (robj->render->alpha != 0x1f && !(flag & 0x4000000)) {
            deferred = 1;
            robj->flag |= 0x8000000;
            continue;
        }
        FLD_DRAW_OBJ2(robj);
    }
    if (deferred) {
        for (p = map->order; p != end; p++) {
            if (**p == -1) {
                continue;
            }
            obj = &map->obj[**p];
            flag = obj->flag;
            model = obj->model_info;
            if (!(flag & 0x8000000)) {
                continue;
            }
            obj->flag = flag & ~0x8000000;
            FLD_DRAW_OBJ();
        }
    }
    func_0206dcf0();
    data_020c3f5c[7] = G3d_SBCRender_007;
    data_020c3f5c[8] = G3d_SBCRender_008;
    *cam = saved;
}

ARM void FLDObject::Final()
{
    FLD_MAP* map;
    int i;
    FLD_RES_TBL* light;
    FLD_LIGHT* l;
    map = m_map;
    if (!(m_flag & 1)) {
        return;
    }
    for (i = 0; i < map->obj_num; i++) {
        if (map->obj[i].render != NULL) {
            func_0206e3e8(&m_allocator, map->obj[i].render->render_obj);
            func_02068a30(&m_allocator, map->obj[i].render);
            map->obj[i].render = NULL;
        }
    }
    light = m_light;
    if (light != NULL) {
        for (i = 0; i < light->num; i++) {
            l = (FLD_LIGHT*)light->res[i];
            if (l->mat_list != NULL) {
                func_02068a30(&m_allocator, l->mat_list);
            }
        }
    }
    func_0206dcd0();
    if (m_coll != NULL && m_coll->ext_data != NULL) {
        func_02068a30(&m_allocator, m_coll->ext_data);
    }
    func_02068a30(&m_allocator, m_tex);
    if (m_mat_alpha != NULL) {
        for (i = 0; i < m_model->num; i++) {
            if (*(unsigned char**)((unsigned char*)m_mat_alpha + i * (int)sizeof(unsigned char*)) != NULL) {
                func_02068a30(&m_allocator, *(unsigned char**)((unsigned char*)m_mat_alpha + i * (int)sizeof(unsigned char*)));
            }
        }
        func_02068a30(&m_allocator, m_mat_alpha);
    }
    m_flag &= ~1;
    init();
}

ARM void FLDObject::SetMapObjAlpha(int obj, int alpha, int flag)
{
    if (alpha < 0) {
        return;
    }
    if (alpha > 0x1f) {
        return;
    }
    if (obj < 0) {
        return;
    }
    FLD_MAP* map = m_map;
    if (obj >= map->obj_num) {
        return;
    }
    short index = map->index[obj];
    if (index < 0) {
        return;
    }
    FLD_MAP_OBJ* o = &map->obj[index];
    o->render->alpha = alpha;
    if (flag) {
        o->flag |= 0x4000000;
    }
}

ARM void FLDObject::SetMapObjOnOff(int obj, int flag)
{
    if (obj < 0) {
        return;
    }
    FLD_MAP* map = m_map;
    if (obj >= map->obj_num) {
        return;
    }
    short index = map->index[obj];
    if (index < 0) {
        return;
    }
    if (flag == 0) {
        map->obj[index].flag &= ~0x2000000;
    } else {
        map->obj[index].flag |= 0x2000000;
    }
}

ARM void FLDObject::SetMapObjPosFX32(int obj, VecFx32* pos)
{
    FLD_MAP_OBJ* o = GetMapObjPtr(obj);
    if (o == NULL) {
        return;
    }
    o->pos.x = pos->x;
    o->pos.y = pos->y;
    o->pos.z = pos->z;
    o->render->flag |= 1;
}

ARM void FLDObject::setMapObjRotFX32(int obj, VecFx32* rot)
{
    FLD_MAP_OBJ* o = GetMapObjPtr(obj);
    if (o == NULL) {
        return;
    }
    o->rot.x = rot->x;
    o->rot.y = rot->y;
    o->rot.z = rot->z;
    o->rot.x = (unsigned short)o->rot.x;
    o->rot.y = (unsigned short)o->rot.y;
    o->rot.z = (unsigned short)o->rot.z;
    o->render->flag |= 4;
}

ARM void FLDObject::AddMapObjPosFX32(int obj, VecFx32* pos)
{
    FLD_MAP_OBJ* o = GetMapObjPtr(obj);
    if (o == NULL) {
        return;
    }
    func_02062f64(&o->pos, pos, &o->pos);
    o->render->flag |= 1;
}

ARM void FLDObject::SetMapUidAlpha(int uid, int alpha, int flag)
{
    for (int i = 0; i < m_map->obj_num; i++) {
        short index = m_map->index[i];
        if (index >= 0 && uid == m_map->obj[index].uid) {
            SetMapObjAlpha(i, alpha, flag);
        }
    }
}

ARM void FLDObject::SetMapUidOnOff(int uid, int flag)
{
    for (int i = 0; i < m_map->obj_num; i++) {
        short index = m_map->index[i];
        if (index >= 0 && uid == m_map->obj[index].uid) {
            SetMapObjOnOff(i, flag);
        }
    }
}

ARM void FLDObject::SetMapUidPosFX32(int uid, VecFx32* pos)
{
    for (int i = 0; i < m_map->obj_num; i++) {
        short index = m_map->index[i];
        if (index >= 0 && uid == m_map->obj[index].uid) {
            SetMapObjPosFX32(i, pos);
        }
    }
}

ARM void FLDObject::setMapUidRotFX32(int uid, VecFx32* rot)
{
    for (int i = 0; i < m_map->obj_num; i++) {
        short index = m_map->index[i];
        if (index >= 0 && uid == m_map->obj[index].uid) {
            setMapObjRotFX32(i, rot);
        }
    }
}

ARM void FLDObject::AddMapUidPosFX32(int uid, VecFx32* pos)
{
    for (int i = 0; i < m_map->obj_num; i++) {
        short index = m_map->index[i];
        if (index >= 0 && uid == m_map->obj[index].uid) {
            AddMapObjPosFX32(i, pos);
        }
    }
}

ARM int FLDObject::CollGetPolyNoByMapUid(int uid, int start)
{
    int i;
    unsigned short id;
    if (m_coll == NULL) {
        return -1;
    }
    if (start < 0) {
        start = 0;
    }
    for (i = start; i < m_coll->poly_size + m_coll->ext_data->ext_num; i++) {
        if (i >= m_coll->poly_size) {
            id = m_coll->ext_data->ext_coll[i - m_coll->poly_size].uid;
        } else {
            id = m_coll->poly[i].uid;
        }
        if (id == uid) {
            return i;
        }
    }
    return -1;
}

ARM void FLDObject::CollEraseMapUid(int uid)
{
    int start = 0;
    int i;
    if (uid == -1 || uid == 0) {
        return;
    }
    do {
        i = CollGetPolyNoByMapUid(uid, start);
        if (i != -1) {
            if (i >= m_coll->poly_size) {
                m_coll->ext_data->ext_coll[i - m_coll->poly_size].flag |= 1;
            } else {
                m_coll->poly[i].flag |= 1;
            }
        }
        start = i + 1;
    } while (i >= 0);
}

ARM void FLDObject::CollResetMapUid(int uid)
{
    int start = 0;
    int i;
    if (uid == -1 || uid == 0) {
        return;
    }
    do {
        i = CollGetPolyNoByMapUid(uid, start);
        if (i != -1) {
            if (i >= m_coll->poly_size) {
                m_coll->ext_data->ext_coll[i - m_coll->poly_size].flag &= ~1;
            } else {
                m_coll->poly[i].flag &= ~1;
            }
        }
        start = i + 1;
    } while (i >= 0);
}

ARM void FLDObject::CollAddPolyPosByMapUid(int uid, VecFx32* pos)
{
    int start = 0;
    int i;
    if (uid == -1 || uid == 0) {
        return;
    }
    do {
        i = CollGetPolyNoByMapUid(uid, start);
        if (i != -1) {
            coll_AddPolyPos(m_coll, i, pos);
        }
        start = i + 1;
    } while (i >= 0);
}

ARM void FLDObject::CollAddPolyPosByMapObj(int obj, VecFx32* pos)
{
    int start = 0;
    int i;
    if (obj == -1) {
        return;
    }
    do {
        i = coll_GetPolyNoByMapObj(m_coll, obj, start);
        if (i != -1) {
            coll_AddPolyPos(m_coll, i, pos);
        }
        start = i + 1;
    } while (i >= 0);
}

ARM int FLDObject::setEventAnimation(int id)
{
    int i;
    int j;
    int start;
    int end;
    int num;
    FLD_PLTT_ENTRY* entry;
    FLD_RES_TBL* light;
    FLD_PLTT_SET* set;
    FLD_PLTT_TBL* pltt;
    FLD_LIGHT* l;
    FLD_RES_TBL* pltt_anim;
    if (m_event == NULL || id < 0 || id > m_event->num) {
        return 0;
    }
    m_event_anim_id = id;
    if (id == 0) {
        end = m_map->uid_num;
        for (i = 0; i < end; i++) {
            setMapObjIndex(i, i);
        }
        if (m_map->extra != 0) {
            setMapObjIndex(i - 1, -1);
        }
        for (; i < m_map->obj_num; i++) {
            setMapObjIndex(i, -1);
        }
        if (m_map->extra != 0) {
            setMapObjIndex(i - 1, m_map->obj_num - 1);
        }
    } else {
        start = m_event->event[id - 1].start;
        end = m_event->event[id - 1].end;
        num = m_map->uid_num;
        if (m_map->extra != 0) {
            num--;
        }
        for (i = start; i < end; i++) {
            setMapObjIndex(i, i);
            resetMapObjAnim(i);
            if (m_anim != NULL) {
                updateMapObjAnim(i);
            }
            for (j = 0; j < num; j++) {
                if (m_map->obj[i].uid != 0 && m_map->obj[i].uid == m_map->obj[j].uid) {
                    setMapObjIndex(j, -1);
                }
            }
        }
        light = m_light;
        if (light != NULL) {
            for (i = 0; i < light->num; i++) {
                l = (FLD_LIGHT*)light->res[i];
                if (l->anim.event_id > 0 && l->anim.event_id == m_event_anim_id) {
                    l->anim.loop = l->anim.loop_start;
                    l->anim.wait = l->anim.wait_start;
                    l->anim.frame = 0;
                    updateFldAnim(&l->anim, applyLightKey, l);
                }
            }
        }
        pltt = m_pltt;
        if (pltt != NULL && (pltt_anim = m_pltt_anim) != NULL) {
            i = 0;
            if (pltt->num > 0) {
                entry = pltt->entry;
                do {
                    set = (FLD_PLTT_SET*)pltt_anim->res[entry->anim];
                    if (entry->event != 0 && entry->event == m_event_anim_id) {
                        set->index = 0;
                        set->timer = 0;
                    }
                    i++;
                    entry++;
                } while (i < pltt->num);
            }
        }
    }
    return 1;
}

ARM void FLDObject::setMapObjIndex(short obj, short index)
{
    if (obj < 0) {
        return;
    }
    if (obj >= m_map->obj_num) {
        return;
    }
    m_map->obj[obj].render->index = index;
    m_map->index[obj] = index;
}

ARM int FLDObject::SetCommonAnimation(int obj_id, int no)
{
    FLD_MAP_OBJ* obj;
    FLD_COMMON_KEY* key;
    FLD_ANIM* src;
    FLD_ANIM* dst;
    FLD_SEQ* seq;
    FLD_SEQ* seq_src;
    int index;
    if (m_common == NULL) {
        return 0;
    }
    if (no < 0) {
        return 0;
    }
    if (obj_id <= -1 || obj_id >= m_map->obj_num) {
        return 0;
    }
    index = m_map->index[obj_id];
    if (index <= -1 || index >= m_map->obj_num) {
        return 0;
    }
    obj = &m_map->obj[index];
    if (obj->seq == -1) {
        return 0;
    }
    if (no == 0) {
        if (obj->model != obj->def_model) {
            func_0206e3e8(&m_allocator, obj->render->render_obj);
            obj->model_info = &m_model->model[obj->def_model];
            obj->render->render_obj = func_0206e3d8(&m_allocator);
            func_0206a34c(obj->render->render_obj, obj->model_info->mdl);
        }
        obj->model = obj->def_model;
        obj->anim = obj->def_anim;
        obj->render->flag = 7;
        calcMapObjRender(obj, NULL);
        m_seq->seq[obj->seq].data = obj->def_seq_data;
    } else {
        if (no > m_common->anim[obj->common_anim].num) {
            return 0;
        }
        key = &m_common->anim[obj->common_anim].key[no - 1];
        if (obj->model != key->model) {
            func_0206e3e8(&m_allocator, obj->render->render_obj);
            obj->model_info = &m_model->model[key->model];
            obj->render->render_obj = func_0206e3d8(&m_allocator);
            func_0206a34c(obj->render->render_obj, obj->model_info->mdl);
            obj->render->flag = 7;
            calcMapObjRender(obj, NULL);
        }
        obj->model = key->model;
        obj->anim = key->anim;
        if (obj->anim != -1) {
            src = (FLD_ANIM*)m_anim->res[obj->anim];
            dst = &obj->render->anim;
            dst->flag = src->flag;
            dst->key_num = src->key_num;
            dst->loop = src->loop;
            dst->wait = src->wait;
            dst->wait_init = src->wait_init;
            dst->key_size = src->key_size;
            dst->frame = src->frame;
            dst->frame_max = src->frame_max;
            dst->event_id = src->event_id;
            dst->wait_start = src->wait_start;
            dst->loop_start = src->loop_start;
            dst->key[0] = src->key[0];
            obj->render->keys = ((FLD_ANIM*)m_anim->res[obj->anim])->key;
        }
        seq = &m_seq->seq[obj->seq];
        if (key->seq == -1) {
            seq->data = -1;
        } else {
            seq_src = &m_seq->seq[key->seq];
            seq->data = seq_src->data;
            seq->anim = seq_src->anim;
            seq->wait = seq_src->wait;
            seq->wait_init = seq_src->wait_init;
            seq->loop = seq_src->loop;
            seq->loop_start = seq_src->loop_start;
            seq->frame = 0;
        }
    }
    obj->common_anim_no = no;
    return 1;
}

ARM int FLDObject::GetCommonAnimationNum(int obj)
{
    FLD_COMMON_TBL* common = m_common;
    if (common == NULL) {
        return 0;
    }
    if (obj <= -1 || obj >= m_map->obj_num) {
        return 0;
    }
    int index = m_map->index[obj];
    if (index <= -1 || index >= m_map->obj_num) {
        return 0;
    }
    short anim = m_map->obj[index].common_anim;
    if (anim == -1) {
        return 0;
    }
    return common->anim[anim].num;
}

ARM int FLDObject::IsCommonAnimationEnd(int obj)
{
    if (m_common == NULL) {
        return 1;
    }
    if (obj <= -1 || obj >= m_map->obj_num) {
        return 1;
    }
    int index = m_map->index[obj];
    if (index <= -1 || index >= m_map->obj_num) {
        return 1;
    }
    FLD_MAP_OBJ* o = &m_map->obj[index];
    if (o->common_anim == -1) {
        return 1;
    }
    if (o->anim != -1 && o->render->anim.loop != 0) {
        return 0;
    }
    if (o->seq != -1) {
        FLD_SEQ* seq = &m_seq->seq[o->seq];
        if (seq->data != -1 && seq->loop != 0) {
            return 0;
        }
    }
    return 1;
}

ARM void FLDObject::resetMapObjAnim(int obj)
{
    if (obj <= -1) {
        return;
    }
    FLD_MAP* map = m_map;
    if (obj >= map->obj_num) {
        return;
    }
    int index = map->index[obj];
    if (index <= -1) {
        return;
    }
    if (index >= map->obj_num) {
        return;
    }
    FLD_MAP_OBJ* o = &map->obj[index];
    if (m_seq != NULL && o->seq != -1) {
        FLD_SEQ* seq = &m_seq->seq[o->seq];
        if (seq->data != -1) {
            seq->loop = seq->loop_start;
            seq->wait = seq->wait_start;
            seq->frame = 0;
        }
    }
    if (m_anim != NULL && o->anim != -1) {
        FLD_ANIM* anim = (FLD_ANIM*)m_anim->res[o->anim];
        if (anim->flag & 4) {
            return;
        }
        anim->loop = anim->loop_start;
        anim->wait = anim->wait_start;
        anim->frame = 0;
    }
}

ARM FLD_MAP_OBJ* FLDObject::GetMapObjPtr(int obj)
{
    if (obj < 0 || obj >= m_map->obj_num) {
        return NULL;
    }
    short index = m_map->index[obj];
    if (index < 0) {
        return NULL;
    }
    return &m_map->obj[index];
}

ARM int FLDObject::GetMapObjUid(int obj)
{
    FLD_MAP_OBJ* o = GetMapObjPtr(obj);
    if (o != NULL) {
        return o->uid;
    }
    return 0;
}

ARM VecFx32* FLDObject::GetMapObjRotFX32(int obj)
{
    FLD_MAP_OBJ* o = GetMapObjPtr(obj);
    if (o != NULL) {
        return &o->rot;
    }
    return NULL;
}

ARM int FLDObject::getMapObjAnimFrame(int obj)
{
    FLD_MAP_OBJ* o = GetMapObjPtr(obj);
    int frame = 0;
    if (o != NULL) {
        if (m_anim != NULL && o->anim != -1) {
            frame = ((FLD_ANIM*)m_anim->res[o->anim])->frame;
        } else if (m_seq != NULL && o->seq != -1) {
            frame = m_seq->seq[o->seq].frame;
        }
    }
    return frame;
}

ARM int FLDObject::getMapObjAnimFrameMax(int obj)
{
    FLD_MAP_OBJ* o = GetMapObjPtr(obj);
    int frame = 0;
    if (o != NULL) {
        if (m_anim != NULL && o->anim != -1) {
            frame = ((FLD_ANIM*)m_anim->res[o->anim])->frame_max;
        } else if (m_seq != NULL && o->seq != -1) {
            FLD_SEQ* seq = &m_seq->seq[o->seq];
            frame = ((FLD_ANIM*)m_seq_data[seq->data]->res[seq->anim])->frame_max;
        }
    }
    return frame;
}

ARM int FLDObject::GetCommonAnimationNo(int obj)
{
    FLD_MAP_OBJ* o = GetMapObjPtr(obj);
    if (o != NULL) {
        return o->common_anim_no;
    }
    return -1;
}

ARM int FLDObject::GetMapObjCommonId(int obj)
{
    FLD_MAP_OBJ* o = GetMapObjPtr(obj);
    if (o != NULL) {
        return o->common_id;
    }
    return -1;
}

ARM void FLDObject::SetSepia()
{
    m_flag |= 0x100;
}

ARM void FLDObject::SetRGBRate(VecFx32* rate, int real_time)
{
    m_rgb_rate = *rate;
    m_flag |= 0x80;

    if (real_time != 0) {
        m_flag |= 0x40;
        return;
    }

    m_flag &= ~0x40;

    if (m_pltt != NULL)
        applyRgbRateToPlttAnims();

    applyRgbRateToPltt();
}

ARM NNSG3dResTex* FLDObject::createPlttTexCopy()
{
    NNSG3dResTex* tex;
    int plttNum;
    int plttSize;
    NNSG3dResTex* buf;
    unsigned char* src;
    unsigned char* dst;
    unsigned short* data;
    unsigned char* pltt;
    int i;
    FLD_PLTT_ENTRY* entry;
    FLD_PLTT_TBL* tbl;
    tex = func_0206e8d0(m_mdl);
    plttNum = tex->plttInfo.sizePltt;
    plttSize = plttNum * 8;
    buf = (NNSG3dResTex*)func_02068a1c(&m_allocator, tex->header.size - tex->texInfo.sizeTex * 8 - tex->tex4x4Info.sizeTex * 8 + plttNum * 8);
    buf->header.kind = tex->header.kind;
    buf->header.size = tex->header.size;
    buf->texInfo.vramKey = tex->texInfo.vramKey;
    buf->texInfo.sizeTex = tex->texInfo.sizeTex;
    buf->texInfo.ofsDict = tex->texInfo.ofsDict;
    buf->texInfo.flag = tex->texInfo.flag;
    buf->texInfo.dummy_ = tex->texInfo.dummy_;
    buf->texInfo.ofsTex = tex->texInfo.ofsTex;
    buf->tex4x4Info.vramKey = tex->tex4x4Info.vramKey;
    buf->tex4x4Info.sizeTex = tex->tex4x4Info.sizeTex;
    buf->tex4x4Info.ofsDict = tex->tex4x4Info.ofsDict;
    buf->tex4x4Info.flag = tex->tex4x4Info.flag;
    buf->tex4x4Info.dummy_ = tex->tex4x4Info.dummy_;
    buf->tex4x4Info.ofsTex = tex->tex4x4Info.ofsTex;
    buf->tex4x4Info.ofsTexPlttIdx = tex->tex4x4Info.ofsTexPlttIdx;
    buf->plttInfo.vramKey = tex->plttInfo.vramKey;
    buf->plttInfo.sizePltt = tex->plttInfo.sizePltt;
    buf->plttInfo.flag = tex->plttInfo.flag;
    buf->plttInfo.ofsDict = tex->plttInfo.ofsDict;
    buf->plttInfo.dummy_ = tex->plttInfo.dummy_;
    buf->plttInfo.ofsPlttData = tex->plttInfo.ofsPlttData;
    src = (unsigned char*)&tex->dict;
    dst = (unsigned char*)&buf->dict;
    MI_CpuCopyU8(src, dst, tex->dict.sizeDictBlk);
    MI_CpuCopyU8(src + tex->dict.sizeDictBlk, dst + tex->dict.sizeDictBlk, ((NNSG3dResDict*)(src + tex->dict.sizeDictBlk))->sizeDictBlk);
    src = (unsigned char*)tex + tex->plttInfo.ofsPlttData;
    pltt = (unsigned char*)buf + buf->texInfo.ofsTex;
    MI_CpuCopyU8(src, pltt, plttSize);
    MI_CpuCopyU8(src, pltt + plttNum * 8, plttSize);
    buf->plttInfo.ofsPlttData = pltt - (unsigned char*)buf;
    m_tex = buf;
    tbl = m_pltt;
    if (tbl != NULL) {
        i = 0;
        if (tbl->num > 0) {
            entry = tbl->entry;
            do {
                data = (unsigned short*)func_0206e664((NNSG3dResDict*)((unsigned char*)buf + buf->plttInfo.ofsDict), entry);
                entry->pltt = (unsigned short*)((unsigned char*)buf + buf->plttInfo.ofsPlttData + *data * 8);
                i++;
                entry++;
            } while (i < tbl->num);
        }
    }
    return buf;
}

ARM void FLDObject::applyRgbRate(unsigned short* src, unsigned short* dst, int num)
{
    fx32 rateR;
    fx32 rateG;
    fx32 rateB;
    int i;
    unsigned short col;
    fx32 r;
    fx32 g;
    fx32 b;
    rateR = m_rgb_rate.x;
    rateG = m_rgb_rate.y;
    rateB = m_rgb_rate.z;
    for (i = 0; i < num; i++) {
        col = src[i];
        r = (col & 0x1f) * rateR;
        g = ((col & 0x3e0) >> 5) * rateG;
        b = ((col & 0x7c00) >> 10) * rateB;
        r = MATH_MIN(r, 0x1f000);
        g = MATH_MIN(g, 0x1f000);
        b = MATH_MIN(b, 0x1f000);
        dst[i] = (unsigned char)(r >> FX32_SHIFT) | ((unsigned char)(g >> FX32_SHIFT) << 5) | ((unsigned char)(b >> FX32_SHIFT) << 10);
    }
}

ARM void FLDObject::applyRgbRateToPltt()
{
    unsigned char* pltt;
    int size;
    unsigned char* src;
    NNSG3dResDict* dict;
    unsigned int num;
    NNSG3dResTex* tex;
    int i;
    NNSG3dResDictEntryHeader* hdr;
    int ofs;
    int len;
    tex = m_tex;
    pltt = (unsigned char*)tex + tex->plttInfo.ofsPlttData;
    dict = (NNSG3dResDict*)((unsigned char*)tex + tex->plttInfo.ofsDict);
    num = dict->numEntry;
    size = tex->plttInfo.sizePltt;
    src = pltt + size * 8;
    for (i = 0; i < num; i++) {
        hdr = (NNSG3dResDictEntryHeader*)((unsigned char*)dict + dict->ofsEntry);
        if (strncmp((char*)((unsigned char*)hdr + hdr->ofsName + i * sizeof(NNSG3dResName)), "tz_", 3) == 0) {
            continue;
        }
        ofs = ((NNSG3dResDictPlttData*)(hdr->data + hdr->sizeUnit * i))->offset * 8;
        if (i + 1 != num) {
            len = ((NNSG3dResDictPlttData*)(hdr->data + hdr->sizeUnit * (i + 1)))->offset * 8 - ofs;
        } else {
            len = size * 8 - ofs;
        }
        if ((unsigned int)len <= 0x200) {
            applyRgbRate((unsigned short*)(src + ofs), (unsigned short*)(pltt + ofs), len / 2);
        }
    }
}

ARM void FLDObject::transferPltt()
{
    NNSG3dResTex* tex = m_tex;
    unsigned int ofs = tex->plttInfo.ofsPlttData;
    unsigned int size = tex->plttInfo.sizePltt * 8;
    DC_CleanRange((unsigned char*)tex + ofs, size);
    data_0211e450.unkfunc_02086378(1, (unsigned char*)tex + ofs, (tex->plttInfo.vramKey & 0xffff) << 3, size, 0);
    m_flag &= ~0x80;
}

ARM void FLDObject::setScale(fx32 x, fx32 y, fx32 z)
{
    m_scale.x = x;
    m_scale.y = y;
    m_scale.z = z;
}

ARM int FLDObject::isMapObjInTownView(FLD_RENDER* render)
{
    fx32 dx = render->center.x - m_target.x;
    fx32 dz = render->center.z - m_target.z;
    if (FX_Mul(dx, dx) + FX_Mul(dz, dz) - render->radius2 >= m_far_town) {
        fx32 dy = render->center.y - m_target.y;
        if (dy < 0) {
            dy = -dy;
        }
        if (dy < FX32_ONE) {
            return 0;
        }
    }
    VecFx32 pos;
    VecFx32 dir;
    func_02063330(render->radius, &s_plane0, &render->center, &pos);
    func_02062f98(&pos, &s_cameraPos, &dir);
    if (func_02062fcc(&dir, &s_plane0) < 0) {
        return 0;
    }
    func_02063330(render->radius, &s_plane1, &render->center, &pos);
    func_02062f98(&pos, &s_cameraPos, &dir);
    return func_02062fcc(&dir, &s_plane1) >= 0;
}

ARM int FLDObject::GetMapObjAlpha(int obj)
{
    if (obj < 0 || obj >= m_map->obj_num) {
        return 0;
    }
    return m_map->obj[m_map->index[obj]].render->alpha;
}

}  // namespace fld
