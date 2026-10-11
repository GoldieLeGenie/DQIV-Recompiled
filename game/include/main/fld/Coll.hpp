#pragma once
#include <globaldefs.h>
#include "nitro/fx.hpp"

struct NNSFndAllocator;

struct COLL_LINE {
    short val;                                  // 0x00
    unsigned short poly_id;                     // 0x02
};

struct COLL_POLY {
    VecFx32 vertex[4];                          // 0x00
    VecFx32 normal;                             // 0x30
    unsigned short type;                        // 0x3C
    short id;                                   // 0x3E
    short flag;                                 // 0x40
    short obj_id;                               // 0x42
    VecFx32 bbox[2];                            // 0x44
    unsigned short uid;                         // 0x5C
    short pad;                                  // 0x5E
};

struct COLL_EXT_DATA {
    COLL_LINE* id_list;                         // 0x00
    int ext_num;                                // 0x04
    COLL_POLY ext_coll[1];                      // 0x08
};

struct COLL_HEADER {
    unsigned short poly_size;                   // 0x00
    unsigned short floor_poly_size;             // 0x02
    unsigned short wall_poly_size;              // 0x04
    unsigned short common_poly_size;            // 0x06
    unsigned int id_size;                       // 0x08
    VecFx32 check_point[2];                     // 0x0C
    COLL_POLY* poly;                            // 0x24
    COLL_EXT_DATA* ext_data;                    // 0x28
    COLL_LINE* x0;                              // 0x2C
    COLL_LINE* x1;                              // 0x30
    COLL_LINE* y0;                              // 0x34
    COLL_LINE* y1;                              // 0x38
    COLL_LINE* z0;                              // 0x3C
    COLL_LINE* z1;                              // 0x40
    char* check;                                // 0x44
    char* check2;                               // 0x48
};

// collision file as loaded: 0x10-byte file header, then the COLL_HEADER block
struct COLL_FILE {
    unsigned char unk_00[0x10];                 // 0x00
    COLL_HEADER header;                         // 0x10
};

enum COLL_ADD_RESULT_TYPE {
    MEMORY_ALLOC_ERROR,
    ALLOC_MAX_OVER_ERROR,
    NUM_MAX_OVER_ERROR,
    RESULT_OK
};

extern "C" {
    int coll_init(COLL_HEADER* header, NNSFndAllocator* allocator);
    int coll_GetPoly(COLL_HEADER* header, int poly_no, COLL_POLY* poly);
    int coll_Id2PolyNo(COLL_HEADER* header, int surface_id);
    fx32 coll_GetCrossPoint3D(VecFx32* point, VecFx32* vertex, VecFx32* normal, VecFx32* cross);
    int coll_CheckPolyPointOne(COLL_POLY* poly, VecFx32* point);
    int coll_CheckPolyPoint(COLL_POLY* poly, VecFx32* point);
    int coll_CheckLinePoint(const VecFx32* posP, fx32 r, const VecFx32* posA, const VecFx32* posB, const VecFx32* nml, VecFx32* cross);
    int coll_PreSearchWallPoly(COLL_HEADER* header, VecFx32* point0, VecFx32* point1);
    int coll_CheckWallNo(COLL_HEADER* header, VecFx32* center, fx32 r, int start, VecFx32* ret);
    int coll_GetCollLinePosL(COLL_LINE* line, int size, short val);
    int coll_GetCollLinePosG(COLL_LINE* line, int size, short val);
    int coll_PreSearchFloorPoly(COLL_HEADER* header, VecFx32* point);
    int coll_SearchFloorPoly(COLL_HEADER* header, VecFx32* point, fx32 height, VecFx32* ret);
    int coll_GetNextMove(COLL_HEADER* header, VecFx32* old_center, VecFx32* center, fx32 r, VecFx32* ret);
    int coll_GetObjId(COLL_HEADER* header, int poly_no);
    int coll_GetSurface(COLL_HEADER* header, int poly_no);
    void coll_EraseObjId(COLL_HEADER* header, int obj_id);
    void coll_ResetObjId(COLL_HEADER* header, int obj_id);
    int coll_GetPolyNoBySurface(COLL_HEADER* header, int surface_id, int start);
    int coll_GetPolyNoByMapObj(COLL_HEADER* header, int obj_id, int start);
    fx32 get_xz_len(VecFx32* a, VecFx32* b);
    void coll_MovePolyPos(COLL_HEADER* header, int poly_no, COLL_POLY* new_poly);
    void coll_AddPolyPos(COLL_HEADER* header, int poly_no, VecFx32* add_vec);
    int coll_PreSearchPoly(COLL_HEADER* header, VecFx32* point0, VecFx32* point1);
    int coll_TriangleIntersect(VecFx32* pos, VecFx32* dir, COLL_POLY* poly, int flag, fx32* ret_t, fx32* ret_u, fx32* ret_v);
    int coll_CrossCheck(COLL_HEADER* header, VecFx32* pos, VecFx32* dir, fx32 len, int start, fx32* ret_len);
    int coll_SearchFloorPoly2(COLL_HEADER* header, VecFx32* point, fx32 height, int start, fx32 judgeLen, VecFx32* ret);
    int collCheckA(VecFx32* bboxA, VecFx32* bboxB, VecFx32* point);
    COLL_ADD_RESULT_TYPE coll_AddCollPoly2(int extraNo, int polyNo, COLL_HEADER* header, COLL_POLY* new_poly, NNSFndAllocator* allocator, int& allocFlag);

    int coll_CheckBoxWallNo(COLL_HEADER* header, VecFx32* center, fx32 r, int start, VecFx32* ret);
    int coll_GetNextMoveBox(COLL_HEADER* header, VecFx32* old_center, VecFx32* center, fx32 r, VecFx32* ret);
    int coll_GetObjWallNo(COLL_HEADER* header, int obj_id, int poly_no);
}
