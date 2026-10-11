#pragma ipa file
#include "main/dss/UnkDisplay.hpp"
#include "main/effect/UnkScreenEffect.hpp"
#include "main/effect/UnkEffectCamera.hpp"
#include "main/object/DisplayCharacter.hpp"
#include "main/dss/UnkBgBuffer.hpp"
#include "main/data/DataObject.hpp"
#include "main/object/DSSAObject.hpp"
#include "nitro/g3.hpp"

static PolygonVertex s_vertex;
static BillboardTexCoord s_texCoord;
static dss::Vector3<dss::Fix16> s_vertexLeftBottom(-0.5f, -0.375f, -0.0f);
static dss::Vector3<dss::Fix16> s_vertexRightBottom(0.5f, -0.375f, -0.0f);
short unusedScreenEffectValue = 1;  // unreferenced (dead-stripped at link), keeps the .bss/.data order
static dss::Vector3<dss::Fix16> s_vertexRightTop(0.5f, 0.375f, -0.0f);
static dss::Vector3<dss::Fix16> s_vertexLeftTop(-0.5f, 0.375f, -0.0f);
static dss::Vector2<dss::Fix32> s_texCoordLeftTop(0, 0);
static dss::Vector2<dss::Fix32> s_texCoordRightTop(256, 0);
UnkScreenEffectTile* UnkScreenEffect::tiles_;
static dss::Vector2<dss::Fix32> s_texCoordRightBottom(256, 192);
static dss::Vector2<dss::Fix32> s_texCoordLeftBottom(0, 192);

THUMB UnkScreenEffect::UnkScreenEffect()
{
    scale_.value = 0x8000;
    columns_ = 32;
    rows_ = 24;
    alpha_ = 31;
    wait_ = 0;
    counter_ = 0;
    tiles_ = NULL;
}

THUMB UnkScreenEffect::~UnkScreenEffect()
{
}

THUMB void UnkScreenEffect::start()
{
    UnkEffectCamera::getSingleton()->initialize();
    dss::g_DISPLAYPLUGIN_DOUBLE3D.SetBlur(0x10, 0);
    flag_.clear();
    s_vertex.v[0] = s_vertexLeftBottom;
    s_vertex.v[1] = s_vertexRightBottom;
    s_vertex.v[2] = s_vertexRightTop;
    s_vertex.v[3] = s_vertexLeftTop;
    s_texCoord.v[0] = s_texCoordLeftBottom;
    s_texCoord.v[1] = s_texCoordRightBottom;
    s_texCoord.v[2] = s_texCoordRightTop;
    s_texCoord.v[3] = s_texCoordLeftTop;
    if (tiles_ == NULL) {
        tiles_ = (UnkScreenEffectTile*)unkfunc_0207f834(&data_0211a60c, sizeof(UnkScreenEffectTile) * columns_ * rows_, 0x20);
    }
    counter_ = 0;
}

THUMB int UnkScreenEffect::unkfunc_0202b244()
{
    if (wait_ < 2) {
        wait_++;
        return 0;
    }
    const int width = 256;
    const int height = 192;
    wait_ = 0;
    unkfunc_020817d8();
    for (int i = 0; i < columns_; i++) {
        for (int j = 0; j < rows_; j++) {
            int index = j + i * rows_;
            for (int k = 0; k < 4; k++) {
                tiles_[index].vertex[k].vx = s_vertex.v[k].vx.value / columns_ + 1;
                tiles_[index].vertex[k].vy = s_vertex.v[k].vy.value / rows_ + 1;
                tiles_[index].vertex[k].vz = s_vertex.v[k].vz.value;
            }
            tiles_[index].texCoord[3][0] = ((i * width) << 12) / columns_;
            tiles_[index].texCoord[3][1] = ((j * height) << 12) / rows_;
            tiles_[index].texCoord[2][0] = (((i + 1) * width) << 12) / columns_ + 0x1000;
            tiles_[index].texCoord[2][1] = ((j * height) << 12) / rows_;
            tiles_[index].texCoord[1][0] = (((i + 1) * width) << 12) / columns_ + 0x1000;
            tiles_[index].texCoord[1][1] = (((j + 1) * height) << 12) / rows_ + 0x1000;
            tiles_[index].texCoord[0][0] = ((i * width) << 12) / columns_;
            tiles_[index].texCoord[0][1] = (((j + 1) * height) << 12) / rows_ + 0x1000;
            if (tiles_[index].texCoord[2][0] >= (width << 12)) {
                tiles_[index].texCoord[2][0] = width << 12;
                tiles_[index].texCoord[1][0] = width << 12;
            }
            if (tiles_[index].texCoord[1][1] >= (height << 12)) {
                tiles_[index].texCoord[1][1] = height << 12;
                tiles_[index].texCoord[0][1] = height << 12;
            }
            tiles_[index].x = (i << 12) / columns_ - 0x800 + 0x1000 / (columns_ * 2);
            tiles_[index].y = 0x600 - (j * 0xc00) / rows_ - 0xc00 / (rows_ * 2);
            tiles_[index].z = 0;
        }
    }
    return 1;
}

THUMB void UnkScreenEffect::unkfunc_0202b474()
{
    unkfunc_0207f840(&data_0211a60c, tiles_);
    tiles_ = NULL;
    UnkEffectCamera::getSingleton()->terminate();
}

THUMB void UnkScreenEffect::unkfunc_0202b498()
{
    if (flag_.check(1) == false && flag_.check(2) == false) {
        if (!unkfunc_0202b244()) {
            return;
        }
        flag_.flag_ |= 1;
    }
    if (unkfunc_0208198c()) {
        if (flag_.check(4) == false) {
            if (counter_ >= 2) {
                flag_.flag_ |= 4;
                counter_ = 0;
            } else {
                counter_++;
            }
        } else if (!flag_.check(2)) {
            flag_.flag_ |= 2;
        }
    }
}

THUMB void UnkScreenEffect::unkfunc_0202b510(int polygonID)
{
    if (unkfunc_02081254() & 1) {
        G3_TexImageParam(7, 1, 5, 5, 0, 0, 1, 0x20000);
    } else {
        G3_TexImageParam(7, 1, 5, 5, 0, 0, 1, 0);
    }
    G3_PolygonAttr(0, 0, 0xc0, polygonID, alpha_, 0);
}

THUMB void UnkScreenEffect::draw()
{
    if (flag_.check(2) != false) {
        UnkEffectCamera::getSingleton()->draw();
        func_0206adcc();
        func_0206dcf0();
        for (int i = 0; i < columns_; i++) {
            for (int j = 0; j < rows_; j++) {
                G3_PushMtx();
                int index = j + i * rows_;
                G3_Scale(scale_.value, scale_.value, scale_.value);
                G3_Translate(tiles_[index].x, tiles_[index].y, tiles_[index].z);
                unkfunc_0202b510(0x18);
                G3_Begin(1);
                for (int k = 0; k < 4; k++) {
                    G3_Color(0x7fff);
                    G3_TexCoord(tiles_[index].texCoord[k][0], tiles_[index].texCoord[k][1]);
                    G3_Vtx(tiles_[index].vertex[k].vx, tiles_[index].vertex[k].vy, tiles_[index].vertex[k].vz);
                }
                G3_End();
                G3_PopMtx(1);
            }
        }
        counter_++;
    }
}

#include "main/dss/DssVectorDefault.hpp"
