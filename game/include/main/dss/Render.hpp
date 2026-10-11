#pragma once
#include <globaldefs.h>

struct Render;
struct RenderObject;
struct UnkModelAnimation;
namespace dss {
    struct Camera;
}

// base of the 3D scenes drawn by Render (FldStage)
struct UnkRenderModel {
    virtual void setup() = 0;                   // slot 0
    virtual void cleanup() = 0;                 // slot 1
    virtual void draw() = 0;                    // slot 2

    Render* render_;                            // 0x04

    void unkfunc_0208532c(Render* render);      // setRender
    void unkfunc_02085348();                    // removeRender
};

// BattleSystem2::render_ (type Render)
struct Render {
    dss::Camera* unk_000;                       // 0x000 camera
    UnkRenderModel* unk_004[32];                // 0x004 3D scenes
    RenderObject* unk_084[64];                  // 0x084 3D objects
    RenderObject* unk_184[32];                  // 0x184 3D objects drawn with a camera only
    UnkModelAnimation* unk_204[128];            // 0x204 animations
    RenderObject* unk_404[128];                 // 0x404 2D sprites
    int unk_604;                                // 0x604

    Render();
    void unkfunc_02084efc();
    void unkfunc_02084f50();
    void unkfunc_02084fa4();                    // draw everything
    void unkfunc_02085118(UnkRenderModel* model);
    void unkfunc_02085140(UnkRenderModel* model);
    void unkfunc_0208516c();
    void unkfunc_02085184();
    void unkfunc_020851c0(RenderObject* object);
    void unkfunc_020851e8(RenderObject* object);
    void unkfunc_0208521c();
    void unkfunc_02085254();
    void unkfunc_02085298();
    void unkfunc_020852c8(RenderObject* object);    // add to the first free slot of unk_404
    void unkfunc_020852f0();
};
