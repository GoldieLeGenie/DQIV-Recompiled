#include <nitro/native_g3.h>
#pragma ipa file
#define RENDER_OBJECT_TU
#include "main/dss/RenderObjectLiterals.hpp"
#include "main/dss/RenderObject.hpp"
#ifdef DQIV_NATIVE
#include "DualScreen.hpp"
#endif
#include "main/dss/Position.hpp"
#include "main/dss/Billboard.hpp"
#include "main/dss/PolygonObject.hpp"
#include "main/dss/UnkSprite2D.hpp"
#include "main/dss/Display.hpp"
#include "main/object/DSSAObject.hpp"
#include "nitro/g3.hpp"
#include "nitro/fx/fx_trig.h"

ARM inline void Position::setScale(dss::Fix32 scale)
{
    scale_.vx = scale;
    scale_.vy = scale;
    scale_.vz = scale;
}

ARM inline void Position::setRotationIdx(const dss::Vector3<unsigned short>& rotation)
{
    unk_28 = rotation;
}

ARM inline void Position::setRotation(const dss::Fix32Vector3& rotation)
{
    rotation_ = rotation;
}

ARM inline void Position::setScale(const dss::Fix32Vector3& scale)
{
    scale_ = scale;
}

ARM inline void Position::setPosition(const dss::Fix32Vector3& position)
{
    position_ = position;
}

















dss::Vector3<int> data_0211d380(renderLiteral_B1x, renderLiteral_B1y, renderLiteral_B1z);
#pragma explicit_zero_data on
int renderLiteral_B3z = 0;
#pragma explicit_zero_data off
#pragma explicit_zero_data on
int renderLiteral_B9x = 0;
#pragma explicit_zero_data off
extern const VecFx32 renderTemplate0 = { 10 << FX32_SHIFT, FX32_ONE, 10 << FX32_SHIFT };
#pragma explicit_zero_data on
int renderLiteral_B7z = 0;
#pragma explicit_zero_data off
dss::Vector3<int> s_bgGradientBottomUpLeft(renderLiteral_B2x, renderLiteral_B2y, renderLiteral_B2z);
#pragma explicit_zero_data on
int renderLiteral_B2x = 0;
#pragma explicit_zero_data off
#pragma explicit_zero_data on
int renderLiteral_B5x = 0;
#pragma explicit_zero_data off
dss::Vector3<int> s_bgGradientBottomUpRight(renderLiteral_B3x, renderLiteral_B3y, renderLiteral_B3z);
#pragma explicit_zero_data on
int renderLiteral_B9y = 0;
#pragma explicit_zero_data off
int data_0211d36c;
#pragma explicit_zero_data on
int renderLiteral_B1z = 0;
#pragma explicit_zero_data off
#pragma explicit_zero_data on
int renderLiteral_B2z = 0;
#pragma explicit_zero_data off
#pragma explicit_zero_data on
int renderLiteral_B2y = 0;
#pragma explicit_zero_data off
#pragma explicit_zero_data on
int renderLiteral_B8z = 0;
#pragma explicit_zero_data off
#pragma explicit_zero_data on
int renderLiteral_B3x = 0;
#pragma explicit_zero_data off
unsigned int g_billboardMtxFifoCmd[15] = { 0x171012, 1, 2, FX32_ONE, 0, 0, 0, FX32_ONE, 0, 0, 0, FX32_ONE, 0, 0, 0 };
#pragma explicit_zero_data on
int renderLiteral_B4y = 0;
#pragma explicit_zero_data off
extern const VecFx32 renderTemplate4 = { 0, 0, 10 << FX32_SHIFT };
int s_bgGradientEnabled;
dss::Vector3<int> s_bgGradientBottomDownLeft(renderLiteral_B4x, renderLiteral_B4y, renderLiteral_B4z);
#pragma explicit_zero_data on
int renderLiteral_B4z = 0;
#pragma explicit_zero_data off
#pragma explicit_zero_data on
int renderLiteral_B6y = 0;
#pragma explicit_zero_data off
#pragma explicit_zero_data on
int renderLiteral_B1y = 0;
#pragma explicit_zero_data off
dss::Vector3<int> s_bgGradientBottomDownRight(renderLiteral_B5x, renderLiteral_B5y, renderLiteral_B5z);
#pragma explicit_zero_data on
int renderLiteral_B5y = 0;
#pragma explicit_zero_data off
#pragma explicit_zero_data on
int renderLiteral_B5z = 0;
#pragma explicit_zero_data off
dss::Vector3<int> s_bgGradientTopUpLeft(renderLiteral_B6x, renderLiteral_B6y, renderLiteral_B6z);
#pragma explicit_zero_data on
int renderLiteral_B6x = 0;
#pragma explicit_zero_data off
dss::Vector3<int> s_bgGradientTopUpRight(renderLiteral_B7x, renderLiteral_B7y, renderLiteral_B7z);
#pragma explicit_zero_data on
int renderLiteral_B1x = 0;
#pragma explicit_zero_data off
dss::Vector3<int> s_bgGradientTopDownLeft(renderLiteral_B8x, renderLiteral_B8y, renderLiteral_B8z);
int s_subScreenShadeEnabled;
#pragma explicit_zero_data on
int renderLiteral_B9z = 0;
#pragma explicit_zero_data off
#pragma explicit_zero_data on
int renderLiteral_B7y = 0;
#pragma explicit_zero_data off
#pragma explicit_zero_data on
int renderLiteral_B8x = 0;
#pragma explicit_zero_data off
#pragma explicit_zero_data on
int renderLiteral_B8y = 0;
#pragma explicit_zero_data off
extern const VecFx32 renderTemplate3 = { 0, FX32_ONE, 0 };
#pragma explicit_zero_data on
int renderLiteral_B4x = 0;
#pragma explicit_zero_data off
#pragma explicit_zero_data on
long renderLiteral_one = 1;
#pragma explicit_zero_data off
#pragma explicit_zero_data on
int renderLiteral_B7x = 0;
#pragma explicit_zero_data off
extern const VecFx32 renderTemplate5 = { 0, FX32_ONE, 0 };
extern const VecFx32 renderTemplate6 = { 0, 0, 10 << FX32_SHIFT };
unsigned int data_020c4364[15] = { 0x171012, 1, 2, FX32_ONE, 0, 0, 0, FX32_ONE, 0, 0, 0, FX32_ONE, 0, 0, 0 };
extern const VecFx32 renderTemplate7 = { 0, FX32_ONE, 0 };
#pragma explicit_zero_data on
int renderLiteral_B6z = 0;
#pragma explicit_zero_data off
extern const VecFx32 renderTemplate1 = { 10 << FX32_SHIFT, FX32_ONE, FX32_ONE };
extern const VecFx32 renderTemplate2 = { 0, 0, 10 << FX32_SHIFT };
#pragma explicit_zero_data on
int renderLiteral_B3y = 0;
#pragma explicit_zero_data off
dss::Vector3<int> s_bgGradientTopDownRight(renderLiteral_B9x, renderLiteral_B9y, renderLiteral_B9z);

ARM Position::Position()
{
    scale_.vx.value = FX32_ONE;
    scale_.vy.value = FX32_ONE;
    scale_.vz.value = FX32_ONE;
}

ARM Position::~Position()
{
}

ARM dss::Fix32Vector3* Position::getPosition()
{
    return &position_;
}

ARM dss::Fix32Vector3* Position::getScale()
{
    return &scale_;
}

ARM RenderObject::RenderObject()
{
    polygonID_ = 0;
    alpha_ = 31;
    enable_ = 1;
}

ARM void RenderObject::unkfunc_02083680()
{
    G3_PolygonAttr(0, 0, 0x80, polygonID_, alpha_, 0x8000);
}

ARM void RenderObject::unkfunc_020836a8()
{
    G3_PolygonAttr(1, 3, 0x40, 0, 12, 0);
}

ARM void RenderObject::unkfunc_020836c0()
{
    G3_PolygonAttr(1, 3, 0x80, 1, 12, 0);
}

ARM void RenderObject::setPolygonID(int id)
{
    polygonID_ = id;
}

ARM void RenderObject::setAlpha(int alpha)
{
    alpha_ = alpha;
}

ARM void RenderObject::setRender(Render* render)
{
    render_ = render;
    render->unkfunc_020851c0(this);
}

ARM void RenderObject::removeRender()
{
    if (render_ != NULL) {
        render_->unkfunc_020851e8(this);
        render_ = NULL;
    }
}

ARM Render* RenderObject::getRender()
{
    return render_;
}

ARM void PolygonObject::unkfunc_02083734(const PolygonVertex* vertex)
{
    vertex_ = *vertex;
}

ARM void PolygonObject::unkfunc_020837d0(const BillboardTexCoord* texCoord)
{
    texCoord_ = *texCoord;
}

ARM void PolygonObject::draw()
{
    unkfunc_02083848();
}

ARM void PolygonObject::unkfunc_02083848()
{
    if (!enable_) {
        return;
    }
    func_0206adcc();
    func_0206dcf0();
    G3_PushMtx();
    G3_Translate(position_.vx.value, position_.vy.value, position_.vz.value);
    func_02065c60(FX_SinIdx(unk_28.vy), FX_CosIdx(unk_28.vy));
    G3_Scale(scale_.vx.value, scale_.vy.value, scale_.vz.value);
    G3_MaterialColorDiffAmb(0x7fff, 0x4210, 1);
    G3_MaterialColorSpecEmi(0x4210, 0, 0);
    if (texture_) {
        ((TextureObject*)texture_)->unkfunc_02086abc();
        ((TextureObject*)texture_)->unkfunc_02086b3c();
    } else {
        disableTexture();
    }
    unkfunc_02083680();
    G3_Begin(1);
    for (int i = 0; i < 4; i++) {
        G3_Color(0x7fff);
        G3_TexCoord(texCoord_.v[i].vx.value, texCoord_.v[i].vy.value);
        G3_Vtx(vertex_.v[i].vx.value, vertex_.v[i].vy.value, vertex_.v[i].vz.value);
    }
    G3_End();
    G3_PopMtx(1);
}

ARM void UnkShadowPolygon::draw()
{
    if (!enable_) {
        return;
    }
    func_0206adcc();
    func_0206dcf0();
    G3_PushMtx();
    G3_Translate(position_.vx.value, position_.vy.value, position_.vz.value);
    func_02065c60(FX_SinIdx(unk_28.vy), FX_CosIdx(unk_28.vy));
    G3_Scale(scale_.vx.value, scale_.vy.value, scale_.vz.value);
    disableTexture();
    if (data_0211d36c == 0) {
        G3_MaterialColorDiffAmb(0, 0, 0);
        G3_MaterialColorSpecEmi(0, 0, 0);
        unkfunc_020836a8();
    } else {
        G3_MaterialColorDiffAmb(0, 0, 1);
        G3_MaterialColorSpecEmi(0, 0, 0);
        unkfunc_020836c0();
    }
    G3_Begin(1);
    G3_Color(unk_80.vx | (unk_80.vy << 5) | (unk_80.vz << 10));
    for (int i = 0; i < 4; i++) {
        G3_Vtx(vertex_.v[i].vx.value, vertex_.v[i].vy.value, vertex_.v[i].vz.value);
    }
    G3_End();
    G3_PopMtx(1);
}

ARM void UnkShadowPolygon::unkfunc_02083b1c()
{
    if (!enable_) {
        return;
    }
    func_0206adcc();
    func_0206dcf0();
    G3_PushMtx();
    G3_Translate(position_.vx.value, position_.vy.value, position_.vz.value);
    func_02065c60(FX_SinIdx(unk_28.vy), FX_CosIdx(unk_28.vy));
    G3_Scale(scale_.vx.value, scale_.vy.value, scale_.vz.value);
    disableTexture();
    G3_MaterialColorDiffAmb(0, 0, 0);
    G3_MaterialColorSpecEmi(0, 0, 0);
    unkfunc_020836a8();
    G3_Begin(1);
    G3_Color(unk_80.vx | (unk_80.vy << 5) | (unk_80.vz << 10));
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 4; i++) {
            G3_Vtx(unk_84[j].v[i].vx.value, unk_84[j].v[i].vy.value, unk_84[j].v[i].vz.value);
        }
    }
    G3_End();
    G3_MaterialColorDiffAmb(0, 0, 1);
    G3_MaterialColorSpecEmi(0, 0, 0);
    unkfunc_020836c0();
    G3_Begin(1);
    G3_Color(unk_80.vx | (unk_80.vy << 5) | (unk_80.vz << 10));
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 4; i++) {
            G3_Vtx(unk_84[j].v[i].vx.value, unk_84[j].v[i].vy.value, unk_84[j].v[i].vz.value);
        }
    }
    G3_End();
    G3_PopMtx(1);
}

ARM void UnkShadowPolygon::unkfunc_02083d00(int index, const PolygonVertex* vertex)
{
    unk_84[index] = *vertex;
}

ARM Billboard::Billboard()
{
    setScale(renderLiteral_one);
    color_ = 0x7fff;
}

ARM void Billboard::unkfunc_020840c8(const BillboardVertex* vertex)
{
    vertex_ = *vertex;
}

ARM BillboardVertex* Billboard::unkfunc_02084134()
{
    return &vertex_;
}

ARM void Billboard::unkfunc_0208413c(const BillboardTexCoord* texCoord)
{
    texCoord_ = *texCoord;
}

ARM BillboardTexCoord* Billboard::unkfunc_020841a8()
{
    return &texCoord_;
}

ARM void Billboard::unkfunc_020841b0(const dss::Vector2<dss::Fix32>* offset)
{
    unk_a8 = *offset;
}

ARM void Billboard::unkfunc_020841d4()
{
    drawVertex_ = vertex_;
    for (int i = 0; i < 4; i++) {
        drawTexCoord_.v[i].vx = texCoord_.v[i].vx + unk_a8.vx;
        drawTexCoord_.v[i].vy = texCoord_.v[i].vy + unk_a8.vy;
    }
}

ARM void Billboard::unkfunc_020842b8(int color)
{
    color_ = color;
}

ARM void Billboard::draw()
{
    G3_PushMtx();
    G3_Translate(position_.vx.value, position_.vy.value, position_.vz.value);
    loadBillboardMatrix();
    G3_Scale(scale_.vx.value, scale_.vx.value, scale_.vx.value);
    if (texture_) {
        ((TextureObject*)texture_)->unkfunc_02086abc();
        ((TextureObject*)texture_)->unkfunc_02086b3c();
    } else {
        disableTexture();
    }
    unkfunc_02083680();
    unkfunc_020841d4();
    G3_Begin(1);
    G3_Color(color_);
    for (int i = 0; i < 4; i++) {
        G3_TexCoord(drawTexCoord_.v[i].vx.value, drawTexCoord_.v[i].vy.value);
        G3_Vtx(drawVertex_.v[i].vx.value, drawVertex_.v[i].vy.value, 0);
    }
    G3_End();
    G3_PopMtx(1);
}

ARM void loadBillboardMatrix()
{
    unsigned int* trans = &g_billboardMtxFifoCmd[12];
    MtxFx44 clip;
    func_0206dcf0();
    G3_WRITE(0x0, 0x151110);
    G3_WRITE(0x0, 0);
    while (func_020653a8(&clip) != 0) {
    }
    trans[0] = clip.m[3][0];
    trans[1] = clip.m[3][1];
    trans[2] = clip.m[3][2];
    G3_SEND(g_billboardMtxFifoCmd, sizeof(g_billboardMtxFifoCmd));
}

ARM UnkSprite2D::UnkSprite2D()
{
    unk_1c = 0;
    unk_1e = 0;
    unk_20 = 0;
    unk_22 = 0;
    unk_24 = 0;
    unk_26 = 0;
    unk_28 = 0;
    unk_30 = 0;
    unk_32 = 0;
    unk_34 = 0x7fff;
}

ARM void UnkSprite2D::unkfunc_02084534(int x, int y)
{
    unk_14.vx.value = x << FX32_SHIFT;
    unk_14.vy.value = y << FX32_SHIFT;
}

ARM void UnkSprite2D::unkfunc_02084548(dss::Fix32 x, dss::Fix32 y)
{
    unk_14.vx = x;
    unk_14.vy = y;
}

ARM void UnkSprite2D::unkfunc_0208456c(int w, int h)
{
    unk_1c = w;
    unk_1e = h;
}

ARM void UnkSprite2D::unkfunc_02084578(int u0, int v0, int u1, int v1)
{
    unk_20 = u0;
    unk_22 = v0;
    unk_24 = u1;
    unk_26 = v1;
}

ARM void UnkSprite2D::draw()
{
    if (!enable_ || !alpha_) {
        return;
    }
#ifdef DQIV_NATIVE
    if (pc::spritesOnBothScreens()) {
        if (!(unk_2c & 3)) {
            return;
        }
    } else
#endif
    if (getDisplayFrame() & 1) {
        if (!(unk_2c & 1)) {
            return;
        }
    } else if (!(unk_2c & 2)) {
        return;
    }
#ifdef DQIV_NATIVE
    // Wide field: sprites of the top screen only (the map information) belong
    // to its inset, not to the field drawn over the whole canvas.
    const unsigned previousLayer = pc::drawLayer();
    if (pc::wideFieldFrame() && (unk_2c & 3) == 2) {
        pc::setDrawLayer(1);
    }
#endif
    G3_PushMtx();
    if (texture_) {
        if (unk_32 == 0) {
            ((TextureObject*)texture_)->unkfunc_02086abc();
            ((TextureObject*)texture_)->unkfunc_02086b3c();
        } else {
            ((TextureObject*)texture_)->unkfunc_02086af4();
            ((TextureObject*)texture_)->unkfunc_02086b3c();
        }
    } else {
        disableTexture();
    }
#ifdef DQIV_NATIVE
    G3_PolygonAttr(0, 0, 0xc0, polygonID_, alpha_, pc::spriteDecal() ? 0xc000 : 0x8000);
#else
    G3_PolygonAttr(0, 0, 0xc0, polygonID_, alpha_, 0x8000);
#endif
    G3_Identity();
    G3_Color(unk_34);
    G3_Translate(unk_14.vx.value, unk_14.vy.value, (unk_28 - 0x400) << FX32_SHIFT);
    G3_Translate(unk_1c * FX32_ONE / 2, unk_1e * FX32_ONE / 2, 0);
    func_02065c9c(FX_SinIdx(unk_30), FX_CosIdx(unk_30));
    G3_Translate(unk_1c * -FX32_ONE / 2, unk_1e * -FX32_ONE / 2, 0);
    G3_Scale(unk_1c << FX32_SHIFT, unk_1e << FX32_SHIFT, FX32_ONE);
    fx32 u0 = unk_20 << FX32_SHIFT;
    fx32 u1 = unk_24 << FX32_SHIFT;
    fx32 v0 = unk_22 << FX32_SHIFT;
    fx32 v1 = unk_26 << FX32_SHIFT;
    G3_Begin(1);
    G3_TexCoord(u0, v1);
    G3_Vtx(0, FX32_ONE, 0);
    G3_TexCoord(u1, v1);
    G3_Vtx(FX32_ONE, FX32_ONE, 0);
    G3_TexCoord(u1, v0);
    G3_Vtx(FX32_ONE, 0, 0);
    G3_TexCoord(u0, v0);
    G3_Vtx(0, 0, 0);
    G3_End();
    G3_PopMtx(1);
#ifdef DQIV_NATIVE
    pc::setDrawLayer(previousLayer);
#endif
}

// Never called: keeps the two VecFx32 initializers at the start of .rodata and data_020c4364 in the .data pool
ARM void unkfunc_unused_29()
{
    VecFx32 camPos = renderTemplate0;
    VecFx32 camUp = renderTemplate1;
    VecFx32 target = { 0, 0, 0 };
    func_02065a98(&camPos, &camUp, &target, 1, NULL);
    G3_SEND(data_020c4364, sizeof(data_020c4364));
}

ARM void setupOrtho2D()
{
    func_0206dcf0();
    G3_Viewport(0, 0, 255, 191);
    func_020657d4(0, 192 << FX32_SHIFT, 0, 256 << FX32_SHIFT, -1024 << FX32_SHIFT, 1024 << FX32_SHIFT, FX32_ONE, 1, NULL);
    G3_StoreMtx(0);
    G3_MtxMode(1);
    VecFx32 camPos = renderTemplate2;
    VecFx32 camUp = renderTemplate3;
    VecFx32 target = { 0, 0, 0 };
    func_02065a98(&camPos, &camUp, &target, 1, NULL);
}

ARM void setupOrtho2DFine()
{
    G3_Viewport(0, 0, 255, 191);
    func_020657d4(0, 192 << FX32_SHIFT, 0, 256 << FX32_SHIFT, -1024 << FX32_SHIFT, 1024 << FX32_SHIFT, 0x40, 1, NULL);
    G3_StoreMtx(0);
    G3_MtxMode(1);
    VecFx32 camPos = renderTemplate4;
    VecFx32 camUp = renderTemplate5;
    VecFx32 target = { 0, 0, 0 };
    func_02065a98(&camPos, &camUp, &target, 1, NULL);
}

ARM void setupOrtho2DBackground()
{
    G3_Viewport(0, 0, 255, 191);
    func_020657d4(0, 192 << FX32_SHIFT, 0, 256 << FX32_SHIFT, -1024 << FX32_SHIFT, 1024 << FX32_SHIFT, 1024 << FX32_SHIFT, 1, NULL);
    G3_StoreMtx(0);
    G3_MtxMode(1);
    VecFx32 camPos = renderTemplate6;
    VecFx32 camUp = renderTemplate7;
    VecFx32 target = { 0, 0, 0 };
    func_02065a98(&camPos, &camUp, &target, 1, NULL);
}

ARM void drawBackgroundGradient()
{
    if (s_bgGradientEnabled == 0) {
        return;
    }
    G3_PushMtx();
    G3_PolygonAttr(0, 0, 0xc0, 0, 31, 0);
    G3_Identity();
    G3_Color(data_0211d380.vx | (data_0211d380.vy << 5) | (data_0211d380.vz << 10));
    G3_Translate(0, 0, 1024 << FX32_SHIFT);
    G3_Scale(256 << FX32_SHIFT, 192 << FX32_SHIFT, FX32_ONE);
    if (getDisplayFrame() & 1) {
        G3_Begin(1);
        G3_Color(s_bgGradientBottomDownLeft.vx | (s_bgGradientBottomDownLeft.vy << 5) | (s_bgGradientBottomDownLeft.vz << 10));
        G3_TexCoord(0, 0);
        G3_Vtx(0, FX32_ONE, 0);
        G3_Color(s_bgGradientBottomDownRight.vx | (s_bgGradientBottomDownRight.vy << 5) | (s_bgGradientBottomDownRight.vz << 10));
        G3_TexCoord(0, 0);
        G3_Vtx(FX32_ONE, FX32_ONE, 0);
        G3_Color(s_bgGradientBottomUpRight.vx | (s_bgGradientBottomUpRight.vy << 5) | (s_bgGradientBottomUpRight.vz << 10));
        G3_TexCoord(0, 0);
        G3_Vtx(FX32_ONE, 0, 0);
        G3_Color(s_bgGradientBottomUpLeft.vx | (s_bgGradientBottomUpLeft.vy << 5) | (s_bgGradientBottomUpLeft.vz << 10));
        G3_TexCoord(0, 0);
        G3_Vtx(0, 0, 0);
        G3_End();
    } else {
        G3_Begin(1);
        G3_Color(s_bgGradientTopDownLeft.vx | (s_bgGradientTopDownLeft.vy << 5) | (s_bgGradientTopDownLeft.vz << 10));
        G3_TexCoord(0, 0);
        G3_Vtx(0, FX32_ONE, 0);
        G3_Color(s_bgGradientTopDownRight.vx | (s_bgGradientTopDownRight.vy << 5) | (s_bgGradientTopDownRight.vz << 10));
        G3_TexCoord(0, 0);
        G3_Vtx(FX32_ONE, FX32_ONE, 0);
        G3_Color(s_bgGradientTopUpRight.vx | (s_bgGradientTopUpRight.vy << 5) | (s_bgGradientTopUpRight.vz << 10));
        G3_TexCoord(0, 0);
        G3_Vtx(FX32_ONE, 0, 0);
        G3_Color(s_bgGradientTopUpLeft.vx | (s_bgGradientTopUpLeft.vy << 5) | (s_bgGradientTopUpLeft.vz << 10));
        G3_TexCoord(0, 0);
        G3_Vtx(0, 0, 0);
        G3_End();
    }
    G3_PopMtx(1);
}

ARM void setBackgroundColor(int r, int g, int b)
{
    s_bgGradientBottomUpLeft.vx = r;
    s_bgGradientBottomUpLeft.vy = g;
    s_bgGradientBottomUpLeft.vz = b;
    s_bgGradientBottomUpRight.vx = r;
    s_bgGradientBottomUpRight.vy = g;
    s_bgGradientBottomUpRight.vz = b;
    s_bgGradientBottomDownLeft.vx = r;
    s_bgGradientBottomDownLeft.vy = g;
    s_bgGradientBottomDownLeft.vz = b;
    s_bgGradientBottomDownRight.vx = r;
    s_bgGradientBottomDownRight.vy = g;
    s_bgGradientBottomDownRight.vz = b;
    s_bgGradientTopUpLeft.vx = r;
    s_bgGradientTopUpLeft.vy = g;
    s_bgGradientTopUpLeft.vz = b;
    s_bgGradientTopUpRight.vx = r;
    s_bgGradientTopUpRight.vy = g;
    s_bgGradientTopUpRight.vz = b;
    s_bgGradientTopDownLeft.vx = r;
    s_bgGradientTopDownLeft.vy = g;
    s_bgGradientTopDownLeft.vz = b;
    s_bgGradientTopDownRight.vx = r;
    s_bgGradientTopDownRight.vy = g;
    s_bgGradientTopDownRight.vz = b;
    s_bgGradientEnabled = 1;
}

ARM void setBackgroundGradient(unsigned char* bottomUpLeft, unsigned char* bottomUpRight, unsigned char* bottomDownLeft, unsigned char* bottomDownRight,
                          unsigned char* topUpLeft, unsigned char* topUpRight, unsigned char* topDownLeft, unsigned char* topDownRight)
{
    s_bgGradientTopUpLeft.vx = topUpLeft[0];
    s_bgGradientTopUpLeft.vy = topUpLeft[1];
    s_bgGradientTopUpLeft.vz = topUpLeft[2];
    s_bgGradientTopUpRight.vx = topUpRight[0];
    s_bgGradientTopUpRight.vy = topUpRight[1];
    s_bgGradientTopUpRight.vz = topUpRight[2];
    s_bgGradientTopDownLeft.vx = topDownLeft[0];
    s_bgGradientTopDownLeft.vy = topDownLeft[1];
    s_bgGradientTopDownLeft.vz = topDownLeft[2];
    s_bgGradientTopDownRight.vx = topDownRight[0];
    s_bgGradientTopDownRight.vy = topDownRight[1];
    s_bgGradientTopDownRight.vz = topDownRight[2];
    s_bgGradientBottomUpLeft.vx = bottomUpLeft[0];
    s_bgGradientBottomUpLeft.vy = bottomUpLeft[1];
    s_bgGradientBottomUpLeft.vz = bottomUpLeft[2];
    s_bgGradientBottomUpRight.vx = bottomUpRight[0];
    s_bgGradientBottomUpRight.vy = bottomUpRight[1];
    s_bgGradientBottomUpRight.vz = bottomUpRight[2];
    s_bgGradientBottomDownLeft.vx = bottomDownLeft[0];
    s_bgGradientBottomDownLeft.vy = bottomDownLeft[1];
    s_bgGradientBottomDownLeft.vz = bottomDownLeft[2];
    s_bgGradientBottomDownRight.vx = bottomDownRight[0];
    s_bgGradientBottomDownRight.vy = bottomDownRight[1];
    s_bgGradientBottomDownRight.vz = bottomDownRight[2];
    s_bgGradientEnabled = 1;
}

ARM void setBackgroundGradientEnable(int enable)
{
    s_bgGradientEnabled = enable;
}

ARM void drawSubScreenShade()
{
    if (s_subScreenShadeEnabled == 0) {
        return;
    }
    if (getDisplayFrame() & 1) {
        return;
    }
    setupOrtho2D();
    G3_PushMtx();
    G3_TexImageParam(0, 1, 0, 0, 0, 0, 1, 0);
    G3_PolygonAttr(0, 0, 0xc0, 0, 15, 0);
    G3_Identity();
    G3_Color(0);
    func_02068f00(0, 0, 0x3ff, 256, 192, 0, 0, 255, 191);
    G3_PopMtx(1);
}

ARM void UnkSprite2D::setColor(int r, int g, int b)
{
    unk_34 = r | (g << 5) | (b << 10);
}

ARM void UnkSprite2D::setColor(int color)
{
    unk_34 = color;
}



#include "main/dss/DssVectorDefault.hpp"
