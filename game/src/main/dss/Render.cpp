#include "main/dss/Render.hpp"
#include "main/dss/RenderObject.hpp"
#include "main/dss/Billboard.hpp"
#include "main/dss/Camera.hpp"
#include "main/dss/DssCore.hpp"
#include "main/dss/Pad.hpp"
#include "main/object/ModelObject.hpp"
#include "main/object/DSSAObject.hpp"

unsigned long long data_0211d3e0[10];

ARM Render::Render()
{
    unk_604 = 0;
}

ARM void Render::unkfunc_02084efc()
{
    unk_000 = NULL;
    for (int i = 0; i < 32; i++) {
        unk_004[i] = NULL;
    }
    for (int i = 0; i < 64; i++) {
        unk_084[i] = NULL;
    }
    for (int i = 0; i < 128; i++) {
        unk_204[i] = NULL;
    }
}

ARM void Render::unkfunc_02084f50()
{
    unk_000 = NULL;
    for (int i = 0; i < 32; i++) {
        unk_004[i] = NULL;
    }
    for (int i = 0; i < 64; i++) {
        unk_084[i] = NULL;
    }
    for (int i = 0; i < 128; i++) {
        unk_204[i] = NULL;
    }
}

ARM void Render::unkfunc_02084fa4()
{
    unsigned long long start = func_02079bf0();
    VecFx32 scale = { FX32_ONE, FX32_ONE, FX32_ONE };
    VecFx32 trans = { 0, 0, 0 };
    func_0206ae30(&scale);
    func_0206ae08((dss::Fix32Vector3*)&trans);
    func_0206adcc();
    dss::g_Pad.edge();
    data_0211d3e0[0] = func_02079bf0() - start;
    unkfunc_02084964();
    unkfunc_02084a1c();
    data_0211d3e0[1] = func_02079bf0() - start;
    unkfunc_020852f0();
    data_0211d3e0[2] = func_02079bf0() - start;
    unkfunc_0208516c();
    data_0211d3e0[3] = func_02079bf0() - start;
    unkfunc_02085184();
    data_0211d3e0[4] = func_02079bf0() - start;
    func_0206adcc();
    func_0206dcf0();
    unkfunc_0208521c();
    unkfunc_02085254();
    data_0211d3e0[5] = func_02079bf0() - start;
    unkfunc_02084de4();
    data_0211d3e0[6] = func_02079bf0() - start;
    unkfunc_02085298();
    data_0211d3e0[7] = func_02079bf0() - start;
    func_0206dcf0();
}

ARM void Render::unkfunc_02085118(UnkRenderModel* model)
{
    for (int i = 0; i < 32; i++) {
        if (unk_004[i] == NULL) {
            unk_004[i] = model;
            return;
        }
    }
}

ARM void Render::unkfunc_02085140(UnkRenderModel* model)
{
    for (int i = 0; i < 32; i++) {
        if (unk_004[i] == model) {
            unk_004[i] = NULL;
            return;
        }
    }
}

ARM void Render::unkfunc_0208516c()
{
    if (unk_000 != NULL) {
        unk_000->applyCamera();
    }
}

ARM void Render::unkfunc_02085184()
{
    for (int i = 0; i < 32; i++) {
        if (unk_004[i] != NULL) {
            unk_004[i]->draw();
        }
    }
    func_0206dcf0();
}

ARM void Render::unkfunc_020851c0(RenderObject* object)
{
    for (int i = 0; i < 64; i++) {
        if (unk_084[i] == NULL) {
            unk_084[i] = object;
            return;
        }
    }
}

ARM void Render::unkfunc_020851e8(RenderObject* object)
{
    for (int i = 0; i < 64; i++) {
        if (unk_084[i] != NULL && unk_084[i] == object) {
            unk_084[i] = NULL;
            return;
        }
    }
}

ARM void Render::unkfunc_0208521c()
{
    for (int i = 0; i < 64; i++) {
        if (unk_084[i] != NULL) {
            unk_084[i]->draw();
        }
    }
}

ARM void Render::unkfunc_02085254()
{
    if (unk_000 == NULL) {
        return;
    }
    for (int i = 0; i < 32; i++) {
        if (unk_184[i] != NULL) {
            unk_184[i]->draw();
        }
    }
}

ARM void Render::unkfunc_02085298()
{
    for (int i = 0; i < 128; i++) {
        if (unk_204[i] != NULL) {
            unk_204[i]->unkfunc_02082bb4();
        }
    }
}

ARM void Render::unkfunc_020852c8(RenderObject* object)
{
    for (int i = 0; i < 128; i++) {
        if (unk_404[i] == NULL) {
            unk_404[i] = object;
            return;
        }
    }
}

ARM void Render::unkfunc_020852f0()
{
    unkfunc_020847e8();
    for (int i = 0; i < 128; i++) {
        if (unk_404[i] != NULL) {
            unk_404[i]->draw();
        }
    }
}

ARM void UnkRenderModel::unkfunc_0208532c(Render* render)
{
    render_ = render;
    render->unkfunc_02085118(this);
}

ARM void UnkRenderModel::unkfunc_02085348()
{
    if (render_ != NULL) {
        render_->unkfunc_02085140(this);
        render_ = NULL;
    }
}
