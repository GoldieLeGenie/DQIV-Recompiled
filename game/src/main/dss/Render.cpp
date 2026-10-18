#include "main/dss/Render.hpp"
#include "main/dss/RenderObject.hpp"
#include "main/dss/Billboard.hpp"
#include "main/dss/Camera.hpp"
#include "main/dss/DssCore.hpp"
#include "main/dss/Pad.hpp"
#include "main/object/ModelObject.hpp"
#include "main/object/DSSAObject.hpp"

unsigned long long s_renderStepTimes[10];

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
    s_renderStepTimes[0] = func_02079bf0() - start;
    setupOrtho2DBackground();
    drawBackgroundGradient();
    s_renderStepTimes[1] = func_02079bf0() - start;
    unkfunc_020852f0();
    s_renderStepTimes[2] = func_02079bf0() - start;
    unkfunc_0208516c();
    s_renderStepTimes[3] = func_02079bf0() - start;
    unkfunc_02085184();
    s_renderStepTimes[4] = func_02079bf0() - start;
    func_0206adcc();
    func_0206dcf0();
    unkfunc_0208521c();
    unkfunc_02085254();
    s_renderStepTimes[5] = func_02079bf0() - start;
    drawSubScreenShade();
    s_renderStepTimes[6] = func_02079bf0() - start;
    unkfunc_02085298();
    s_renderStepTimes[7] = func_02079bf0() - start;
    func_0206dcf0();
}

ARM void Render::unkfunc_02085118(RenderScene* model)
{
    for (int i = 0; i < 32; i++) {
        if (unk_004[i] == NULL) {
            unk_004[i] = model;
            return;
        }
    }
}

ARM void Render::unkfunc_02085140(RenderScene* model)
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
    setupOrtho2D();
    for (int i = 0; i < 128; i++) {
        if (unk_404[i] != NULL) {
            unk_404[i]->draw();
        }
    }
}

ARM void RenderScene::unkfunc_0208532c(Render* render)
{
    render_ = render;
    render->unkfunc_02085118(this);
}

ARM void RenderScene::unkfunc_02085348()
{
    if (render_ != NULL) {
        render_->unkfunc_02085140(this);
        render_ = NULL;
    }
}
