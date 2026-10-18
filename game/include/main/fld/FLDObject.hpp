#pragma once
#ifdef DQIV_NATIVE
#include <bit>
#endif
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"
#include "nnsys/fnd.hpp"
#include "nnsys/g3d.hpp"
#include "main/fld/Coll.hpp"
#include "nitro/os.hpp"

struct COLL_HEADER;
struct COLL_POLY;

namespace fld {

// chunk of the field data file
struct FLD_CHUNK {
    unsigned int id;                            // 0x00
    unsigned int size;                          // 0x04
};

// table of resources (offsets relocated by FLDObject::Setup)
struct FLD_RES_TBL {
    FLD_CHUNK header;                           // 0x00
    int num;                                    // 0x08
    void* res[1];                               // 0x0C
};

// keyframe animation, keys follow the header (or are pointed by FLD_RENDER::keys)
struct FLD_ANIM {
    short flag;                                 // 0x00  2: hold, 4: per object copy, 8: shape morph keys
    short key_num;                              // 0x02
    short loop;                                 // 0x04
    short wait;                                 // 0x06
    short wait_init;                            // 0x08
    short key_size;                             // 0x0A
    int frame;                                  // 0x0C
    int frame_max;                              // 0x10
    int event_id;                               // 0x14
    int wait_start;                             // 0x18
    int loop_start;                             // 0x1C
    int key[1];                                 // 0x20
};

typedef int (*FLD_ANIM_FUNC)(void* key0, void* key1, fx32 rate, void* work);

struct FLD_CAMERA_KEY {
    int frame;                                  // 0x00
    VecFx32 pos;                                // 0x04
    VecFx32 tag;                                // 0x10
    VecFx32 vec;                                // 0x1C
};

struct FLD_OBJ_KEY {
    int frame;                                  // 0x00
    VecFx32 pos;                                // 0x04
    VecFx32 rot;                                // 0x10
    VecFx32 scale;                              // 0x1C
};

struct FLD_SHAPE_KEY {
    int frame;                                  // 0x00
    int shape;                                  // 0x04
};

struct FLD_SEQ_KEY {
    int frame;                                  // 0x00
    int shape;                                  // 0x04
    VecFx32 pos;                                // 0x08
    VecFx32 rot;                                // 0x14
    VecFx32 scale;                              // 0x20
};

struct FLD_LIGHT_KEY {
    int frame;                                  // 0x00
    unsigned short amb;                         // 0x04
    unsigned short diff;                        // 0x06
    unsigned short spec;                        // 0x08
    unsigned short emi;                         // 0x0A
    int alpha;                                  // 0x0C
};

struct FLD_MAT_LIST {
    unsigned int num;                           // 0x00
    struct {
        NNSG3dResMdl* mdl;                      // 0x00
        int mat;                                // 0x04
    } list[1];                                  // 0x04
};

struct FLD_LIGHT {
    union {
        char name[0x10];                        // 0x00
        FLD_MAT_LIST* mat_list;                 // 0x00
    };
    FLD_ANIM anim;                              // 0x10
};

// per map object render data (allocated by Setup)
struct FLD_RENDER {
    MtxFx33 mtx;                                // 0x00
    VecFx32 trans;                              // 0x24
    VecFx32 scale;                              // 0x30
    VecFx32 rot;                                // 0x3C
    NNSG3dRenderObj* render_obj;                // 0x48
    int flag;                                   // 0x4C
    int alpha;                                  // 0x50
    int index;                                  // 0x54
    FLD_ANIM anim;                              // 0x58
    void* keys;                                 // 0x7C
    void* key0;                                 // 0x80
    void* key1;                                 // 0x84
    fx32 rate;                                  // 0x88
    fx16 box_x;                                 // 0x8C
    fx16 box_y;                                 // 0x8E
    fx16 box_z;                                 // 0x90
    fx16 box_w;                                 // 0x92
    fx16 box_h;                                 // 0x94
    fx16 box_d;                                 // 0x96
    fx32 box_scale;                             // 0x98
    VecFx32 center;                             // 0x9C
    fx32 radius2;                               // 0xA8
    fx32 radius;                                // 0xAC
};

struct FLD_FADE {
    fx32 start;                                 // 0x00
    fx32 end;                                   // 0x04
    fx32 min;                                   // 0x08
};

// 'FMDL' entry
struct FLD_MODEL {
    int unk_00;                                 // 0x00
    int unk_04;                                 // 0x04
    NNSG3dResMdl* mdl;                          // 0x08
    int flag;                                   // 0x0C
    FLD_FADE* fade;                             // 0x10
    int unk_14;                                 // 0x14
    void* unk_18;                               // 0x18
    void* unk_1c;                               // 0x1C
    int shape_num;                              // 0x20
    int shape0;                                 // 0x24
    int shape1;                                 // 0x28
    fx32 shape_rate;                            // 0x2C
};

struct FLD_MODEL_TBL {
    FLD_CHUNK header;                           // 0x00
    int num;                                    // 0x08
    int unk_0c;                                 // 0x0C
    FLD_MODEL model[1];                         // 0x10
};

struct FLD_MAP_OBJ {
    VecFx32 pos;                                // 0x00
    int unk_0c;                                 // 0x0C
    VecFx32 rot;                                // 0x10
    short model;                                // 0x1C
    short common_id;                            // 0x1E
    VecFx32 scale;                              // 0x20
    int uid;                                    // 0x2C
    int flag;                                   // 0x30
    int priority;                               // 0x34
    FLD_RENDER* render;                         // 0x38
    FLD_MODEL* model_info;                      // 0x3C
    short seq;                                  // 0x40
    short anim;                                 // 0x42
    short def_model;                            // 0x44
    short def_seq_data;                         // 0x46
    short def_anim;                             // 0x48
    short common_anim;                          // 0x4A
    short common_anim_no;                       // 0x4C
    short unk_4e;                               // 0x4E
};

struct FLD_OBJ_UID {
    short uid;                                  // 0x00
    short obj_id;                               // 0x02
};

// 'FMAP'
struct FLD_MAP {
    FLD_CHUNK header;                           // 0x00
    int uid_num;                                // 0x08
    int obj_num;                                // 0x0C
    int unk_10;                                 // 0x10
    short* index;                               // 0x14
    short** order;                              // 0x18
    int extra;                                  // 0x1C
    FLD_MAP_OBJ obj[1];                         // 0x20
};

// 'FETC'
struct FLD_SCENE {
    FLD_CHUNK header;                           // 0x00
    int backColor;                              // 0x08
    char unk_0c[0x18 - 0xc];                    // 0x0C
    int cameraLimitR;                           // 0x18
    int cameraLimitL;                           // 0x1C
};

struct FLD_SEQ {
    short unk_00;                               // 0x00
    short data;                                 // 0x02
    short anim;                                 // 0x04
    short wait;                                 // 0x06
    short wait_init;                            // 0x08
    short loop;                                 // 0x0A
    short wait_start;                           // 0x0C
    short loop_start;                           // 0x0E
    short frame;                                // 0x10
    short unk_12;                               // 0x12
};

// 'FSEq'
struct FLD_SEQ_TBL {
    FLD_CHUNK header;                           // 0x00
    int num;                                    // 0x08
    FLD_SEQ seq[1];                             // 0x0C
};

struct FLD_COMMON_KEY {
    short model;                                // 0x00
    short seq;                                  // 0x02
    short anim;                                 // 0x04
    short unk_06;                               // 0x06
};

// 'FCAN'
struct FLD_COMMON_TBL {
    FLD_CHUNK header;                           // 0x00
    int num;                                    // 0x08
    struct {
        int num;                                // 0x00
        FLD_COMMON_KEY* key;                    // 0x04
    } anim[1];                                  // 0x0C
};

// 'FEAN'
struct FLD_EVENT_TBL {
    FLD_CHUNK header;                           // 0x00
    int num;                                    // 0x08
    int unk_0c;                                 // 0x0C
    struct {
        short start;                            // 0x00
        short end;                              // 0x02
    } event[1];                                 // 0x10
};

struct FLD_PLTT16  { short wait; unsigned short col[0x10];  };   // 0x22
struct FLD_PLTT256 { short wait; unsigned short col[0x100]; };   // 0x202

// 'FPAM' entry
struct FLD_PLTT_SET {
    short timer;                                // 0x00
    short index;                                // 0x02
    short count;                                // 0x04
    short flag;                                 // 0x06  1: 256 colors, 2: rgb rate applied
    unsigned char data[1];                      // 0x08
};

struct FLD_PLTT_ENTRY {
    char name[0x10];                            // 0x00
    unsigned char anim;                         // 0x10
    unsigned char event;                        // 0x11
    unsigned char unk_12[2];                    // 0x12
    unsigned short* pltt;                       // 0x14
};

// 'FPLP'
struct FLD_PLTT_TBL {
    FLD_CHUNK header;                           // 0x00
    int num;                                    // 0x08
    FLD_PLTT_ENTRY entry[1];                    // 0x0C
};

struct FLDObject
{
    int m_flag;                                 // 0x000
    FLD_CHUNK* m_chunk;                         // 0x004
    void* m_mdl;                                // 0x008
    FLD_MAP* m_map;                             // 0x00C
    FLD_MODEL_TBL* m_model;                     // 0x010
    FLD_SCENE* m_scene;                         // 0x014
    FLD_SEQ_TBL* m_seq;                         // 0x018
    int m_seq_num;                              // 0x01C
    FLD_RES_TBL* m_seq_data[0x80];              // 0x020
    FLD_RES_TBL* m_anim;                        // 0x220
    FLD_RES_TBL* m_camera_anim;                 // 0x224
    FLD_RES_TBL* m_pltt_anim;                   // 0x228
    FLD_PLTT_TBL* m_pltt;                       // 0x22C
    FLD_RES_TBL* m_light;                       // 0x230
    FLD_EVENT_TBL* m_event;                     // 0x234
    NNSFndAllocator m_allocator;                // 0x238
    COLL_HEADER* m_coll;                        // 0x248
    int m_camera_no[2];                         // 0x24C
    VecFx32 m_x32_camera_pos[2];                // 0x254
    VecFx32 m_x32_camera_tag[2];                // 0x26C
    VecFx32 m_x32_camera_vec[2];                // 0x284
    int m_frame_flip_flop;                      // 0x29C
    int m_vanish_flag;                          // 0x2A0
    FLD_COMMON_TBL* m_common;                   // 0x2A4
    fx32 m_bbox_far;                            // 0x2A8
    NNSG3dGeBuffer m_ge_buffer;                 // 0x2AC
    VecFx32 m_cross_pos;                        // 0x5B0
    VecFx32 m_cross_dir;                        // 0x5BC
    fx32 m_cross_len;                           // 0x5C8
    int m_simple_bby;                           // 0x5CC
    int m_box_test;                             // 0x5D0
    VecFx32 m_rgb_rate;                         // 0x5D4
    NNSG3dResTex* m_tex;                        // 0x5E0
    unsigned char** m_mat_alpha;                // 0x5E4
    int m_event_anim_id;                        // 0x5E8
    VecFx32 m_scale;                            // 0x5EC
    fx32 m_far_town;                            // 0x5F8
    VecFx32 m_target;                           // 0x5FC
    int mainCameraFlag_;                        // 0x608
    int clipping;                               // 0x60C

    FLDObject();
    ~FLDObject();
    void init();
    int SetCameraNo(int no, int screen);
    int Setup(void* mdl, void* data, void* coll, int heap);
    void sortMapObjOrder();
    void SetCameraPos(fx32 x, fx32 y, fx32 z, int screen);
    void SetCameraCent(fx32 x, fx32 y, fx32 z, int screen);
    void SetCameraUp(fx32 x, fx32 y, fx32 z, int screen);
    void updateCameraAnim(int screen);
    void updateMapObjAnim(int obj);
    void updateMapObjAnims();
    void updateMapObjSeq(FLD_MAP_OBJ* obj);
    void applyMapObjShapeMorph(FLD_MAP_OBJ* obj);
    void updatePlttAnims();
    void copyPlttAnimFrame(FLD_PLTT_ENTRY* entry, FLD_PLTT_SET* set, int index);
    void applyRgbRateToPlttAnims();
    void updateLightAnims();
    void calc();                                    // calc
    void draw();                                    // draw
    void Final();
    void SetMapObjAlpha(int obj, int alpha, int flag);
    void SetMapObjOnOff(int obj, int flag);
    void SetMapObjPosFX32(int obj, VecFx32* pos);
    void setMapObjRotFX32(int obj, VecFx32* rot);
    void AddMapObjPosFX32(int obj, VecFx32* pos);
    void SetMapUidAlpha(int uid, int alpha, int flag);
    void SetMapUidOnOff(int uid, int flag);
    void SetMapUidPosFX32(int uid, VecFx32* pos);
    void setMapUidRotFX32(int uid, VecFx32* rot);
    void AddMapUidPosFX32(int uid, VecFx32* pos);
    int CollGetPolyNoByMapUid(int uid, int start);
    void CollEraseMapUid(int uid);
    void CollResetMapUid(int uid);
    void CollAddPolyPosByMapUid(int uid, VecFx32* pos);
    void CollAddPolyPosByMapObj(int obj, VecFx32* pos);
    int setEventAnimation(int id);                               // event animation
    void setMapObjIndex(short obj, short index);
    int SetCommonAnimation(int obj, int no);
    int GetCommonAnimationNum(int obj);
    int IsCommonAnimationEnd(int obj);
    void resetMapObjAnim(int obj);
    FLD_MAP_OBJ* GetMapObjPtr(int obj);
    int GetMapObjUid(int obj);
    VecFx32* GetMapObjRotFX32(int obj);
    int getMapObjAnimFrame(int obj);                              // animation frame
    int getMapObjAnimFrameMax(int obj);                              // animation frame max
    int GetCommonAnimationNo(int obj);
    int GetMapObjCommonId(int obj);
    void SetSepia();
    void SetRGBRate(VecFx32* rate, int real_time);
    NNSG3dResTex* createPlttTexCopy();
    void applyRgbRate(unsigned short* src, unsigned short* dst, int num);
    void applyRgbRateToPltt();
    void transferPltt();
    void setScale(fx32 x, fx32 y, fx32 z);              // scale
    int isMapObjInTownView(FLD_RENDER* render);
    int GetMapObjAlpha(int obj);
    void SetPause(int pause) { if (pause) { m_flag |= 4; } else { m_flag &= ~4; } }
    int CollCrossCheck(VecFx32* start, VecFx32* end, int poly, fx32* dist) {
        if (poly == 0) {
            m_cross_pos = *start;
            func_02062f98(end, start, &m_cross_dir);
            func_020630ec(&m_cross_dir, &m_cross_dir);
            m_cross_len = func_0206338c(start, end);
        }
        return coll_CrossCheck(m_coll, &m_cross_pos, &m_cross_dir, m_cross_len, poly, dist);
    }
};

}  // namespace fld

extern "C" {
    void func_02066e34(void* head, unsigned int num, unsigned int width, int (*compare)(void*, void*), void* stack);   // MATH_QSort
}

inline unsigned int MATH_CountLeadingZeros(unsigned int x)
{
#ifdef DQIV_NATIVE
    return std::countl_zero(x);
#else
    asm { clz x, x }
    return x;
#endif
}

inline int MATH_ILog2(unsigned int x)
{
    return (int)(31 - MATH_CountLeadingZeros(x));
}

#define MATH_QSORT_STACK_SIZE(num) ((MATH_ILog2(num) <= 0) ? sizeof(int) : ((MATH_ILog2(num) + 1) * sizeof(int) * 2))
