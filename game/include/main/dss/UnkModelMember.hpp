#pragma once
#include <globaldefs.h>
#include "nnsys/g3d.hpp"

// model file of a model object: binds its textures (loaded in VRAM) to its model set
struct UnkModelMember {
    void* unk_00;                               // 0x00 model file
    NNSG3dResMdl* unk_04;                       // 0x04 last model of the set
    NNSG3dResTex* unk_08;                       // 0x08 textures

    UnkModelMember();
    void unkfunc_02083354(void* data);         // set the model file
    void unkfunc_02083360();                    // release
    void unkfunc_0208336c();                    // reload the textures
    void unkfunc_02083384(void* data);
    void unkfunc_02083454();
    void unkfunc_02083460();                    // load the textures
    void unkfunc_02083530();                    // release the textures
    void* unkfunc_020835c8();
    NNSG3dResMdl* unkfunc_020835d0();
};

extern NNSG3dResTex* data_0211d364;             // last loaded textures

NNSG3dResTex* unkfunc_020835d8();
