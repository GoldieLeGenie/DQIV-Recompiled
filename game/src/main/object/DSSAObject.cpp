#include <nitro/native_g3.h>
#include "main/object/DSSAObject.hpp"
#include "main/dss/RenderObject.hpp"
#include "main/dss/Display.hpp"
#include "main/dss/CharacterPalette.hpp"
#include "main/dss/Sprite2D.hpp"
#include "nitro/g3.hpp"
#include "nitro/fx/fx_trig.h"
#include "main/status/HaveBattleStatus.hpp"

int DSSAObject::posX_;
int DSSAObject::posY_;
int DSSAObject::sizeX_;
int DSSAObject::sizeY_;
int DSSAObject::scaleX_;
int DSSAObject::scaleY_;
int DSSAObject::angle_;
int DSSAObject::priority_;
long DSSAObject::trans_;
int DSSAObject::calcType_;
static dss::Fix32 defaultScale2_;
static dss::Fix32 defaultScale_(1L);
dss::Fix32Vector3 DSSAObject::baseScale_;

ARM int DSSAParts::getPartsIndex()
{
    return partsIndex_;
}

ARM int DSSAParts::getType()
{
    return flag_ & 0xf;
}

ARM int DSSAParts::getPosX()
{
    return posX_ << 12;
}

ARM int DSSAParts::getPosY()
{
    return posY_ << 12;
}

ARM int DSSAParts::getScaleX()
{
    return scaleX_;
}

ARM int DSSAParts::getScaleY()
{
    return scaleY_;
}

ARM int DSSAParts::getAngle()
{
    return angle_;
}

ARM int DSSAParts::getTrans()
{
    return trans_;
}

ARM int DSSAParts::getPriority()
{
    return priority_ << 12;
}

ARM bool DSSAParts::getFlipX()
{
    return (flag_ & 0x10) != 0;
}

ARM bool DSSAParts::getFlipY()
{
    return (flag_ & 0x20) != 0;
}

ARM bool DSSAParts::getAlpha()
{
    return (flag_ & 0x40) != 0;
}

ARM void DSSAParts::print()
{
}

ARM DSSAData::DSSAData()
{
    for (int i = 0; i < 10; i++) {
        defineNullInfo[i] = 0;
    }
}

ARM DSSAData::~DSSAData()
{
}

ARM void DSSAData::setup(void* data)
{
    data_ = data;
    count_ = ((DSSAHeader*)data)->count_;
    frame_ = ((DSSAHeader*)data)->frame_;
    data = (DSSAHeader*)data + 1;
    basicInfo_ = (BasicInfo*)data;
    for (int i = 0; i < count_; i++) {
        data = (BasicInfo*)data + 1;
    }
    for (int i = 0; i < count_; i++) {
        if (basicInfo_[i].type_[0] == 2) {
            defineNullInfo[basicInfo_[i].type_[1]] = i;
        }
    }
    boundingBox_ = (DSSABoundingBox*)data;
    offset_ = (int*)(boundingBox_ + 1);
    align(frame_, 2);
    for (int i = 0; i < frame_; i++) {
        setParts(i);
    }
}

ARM int DSSAData::align(int value, int alignment)
{
    return (value + (alignment - 1)) & ~(alignment - 1);
}

ARM void DSSAData::cleanup()
{
}

ARM void DSSAData::setParts(int frame)
{
    setParts((char*)data_ + offset_[frame * 2]);
}

ARM void DSSAData::setParts(void* frame)
{
    DSSAFrame* header = (DSSAFrame*)frame;
    currentFrame_ = header->currentFrame_;
    usableCount_ = header->usableCount_;
    parts_ = (DSSAParts*)(header + 1);
    parts_->print();
}

ARM DSSAParts* DSSAData::getParts(int index)
{
    return &parts_[index];
}

ARM int DSSAData::getAreaTop(int index)
{
    return basicInfo_[index].area_[0];
}

ARM int DSSAData::getAreaLeft(int index)
{
    return basicInfo_[index].area_[1];
}

ARM int DSSAData::getAreaBottom(int index)
{
    return basicInfo_[index].area_[2];
}

ARM int DSSAData::getAreaRight(int index)
{
    return basicInfo_[index].area_[3];
}

ARM int DSSAData::getOriginX(int index)
{
    return basicInfo_[index].origin_[1];
}

ARM int DSSAData::getOriginY(int index)
{
    return basicInfo_[index].origin_[0];
}

ARM int DSSAData::getNullIndex(int index)
{
    return defineNullInfo[index];
}

inline const long& zeroL() { return 0L; }

ARM DSSAObject::DSSAObject() : data_(0), palette_(0), flag_(0)
{
    defaultScale2_.value = 0x1000;
    calcType_ = 0;
}

ARM DSSAObject::~DSSAObject()
{
}

ARM void DSSAObject::setup(void* data)
{
    data_ = data;
    frame_ = 0;
    flag_ = 0;
    alpha_ = dss::Fix32(1L);
    dssaData_.setup(data);
    displayPartsCount_ = 0;
}

ARM void DSSAObject::setTexture(void* texture)
{
    dssaData_.texture_ = texture;
}

ARM void DSSAObject::setPalette(void* palette)
{
    palette_ = palette;
}

ARM void DSSAObject::cleanup()
{
    dssaData_.cleanup();
    data_ = 0;
}

ARM void DSSAObject::draw()
{
    if (!isEnable()) {
        return;
    }
    G3_PushMtx();
    G3_Identity();
    setupDraw();
    dssaData_.setParts(frame_);
    setupRoot();
    G3_PushMtx();
    displayPartsCount_ = 0;
    for (int i = 1; i < dssaData_.usableCount_; i++) {
        DSSAParts parts = *dssaData_.getParts(i);
        if (parts.getType() == 2) {
            continue;
        }
        int index = parts.getPartsIndex();
        posX_ = parts.getPosX();
        posY_ = parts.getPosY();
        sizeX_ = (dssaData_.getAreaRight(index) - dssaData_.getAreaLeft(index) + 1) << 12;
        sizeY_ = (dssaData_.getAreaBottom(index) - dssaData_.getAreaTop(index) + 1) << 12;
        priority_ = parts.getPriority() / 4096;
        if (priority_ > 63) {
            priority_ = 50;
        }
        angle_ = parts.getAngle();
        if (angle_ != 0) {
            int angle = angle_ << 12;
            angle_ = (unsigned short)(((angle / 4096) << 16) / 25735);
        }
        scaleX_ = parts.getScaleX();
        scaleY_ = parts.getScaleY();
        trans_ = parts.getTrans();
        if (trans_ == 0) {
            continue;
        }
        trans_ = (dss::Fix32(trans_) * alpha_).value >> 12;
        G3_WRITE(0x29, (priority_ << 24) | 0xc0 | (trans_ << 16));
        if (parts.getAlpha()) {
            ((TextureObject*)dssaData_.texture_)->unkfunc_02086b68();
            ((TextureObject*)dssaData_.texture_)->unkfunc_02086bd8();
        } else {
            ((TextureObject*)dssaData_.texture_)->unkfunc_02086abc();
            ((TextureObject*)dssaData_.texture_)->unkfunc_02086b3c();
            if (palette_) {
                ((CharacterPalette*)palette_)->unkfunc_02086dac();
            }
        }
        if (trans_ != 0) {
            displayPartsCount_++;
            G3_PushMtx();
            drawParts(&parts);
            G3_PopMtx(1);
        }
    }
    G3_PopMtx(1);
    G3_PopMtx(1);
}

ARM void DSSAObject::execute()
{
    if (!isEnable()) {
        return;
    }
    if (flag_ & 1) {
        return;
    }
    if (!(getDisplayFrame() & 1)) {
        return;
    }
    frame_++;
    if (frame_ == dssaData_.frame_) {
        flag_ |= 2;
    }
    frame_ = status::HaveBattleStatus::getClampValue(frame_, 0, dssaData_.frame_ - 1);
}

ARM void DSSAObject::start(int frame)
{
    frame_ = frame;
    flag_ = 0;
}

ARM void DSSAObject::pause(bool pause)
{
    if (pause) {
        flag_ |= 1;
    } else {
        flag_ &= ~1;
    }
}

ARM int DSSAObject::isEnd()
{
    return (flag_ & 2) != 0;
}

ARM int DSSAObject::isEnable()
{
    return data_ != 0;
}

ARM void DSSAObject::setAlpha(dss::Fix32 alpha)
{
    alpha_ = alpha;
}

ARM void DSSAObject::setCurrentFrame(int frame)
{
    frame_ = frame;
    frame_ = dss::clamp<int>(frame, 0, dssaData_.frame_ - 1);
}

ARM dss::Fix32Vector3 DSSAObject::getNullPosition(int index)
{
    int nullIndex = dssaData_.getNullIndex(index);
    dss::Fix32Vector3 position;
    position.vx = (long)dssaData_.getOriginX(nullIndex);
    position.vx *= defaultScale_;
    position.vy = (long)dssaData_.getOriginY(nullIndex);
    position.vy *= defaultScale_;
    return position;
}

ARM dss::Vector3<int> DSSAObject::getNullPositionInt(int index)
{
    int nullIndex = dssaData_.getNullIndex(index);
    dss::Vector3<int> position;
    position.vx = (long)dssaData_.getOriginX(nullIndex);
    position.vy = (long)dssaData_.getOriginY(nullIndex);
    return position;
}

ARM dss::Fix32 DSSAObject::getDefaultScale2()
{
    return defaultScale2_;
}

ARM void DSSAObject::setDefaultScale(dss::Fix32 scale)
{
    defaultScale_ = scale;
}

ARM dss::Fix32 DSSAObject::getDefaultScale()
{
    return defaultScale_;
}

ARM void DSSAObject::setReverse(int reverse)
{
    if (reverse) {
        flag_ |= 4;
    } else {
        flag_ &= ~4;
    }
}

ARM int DSSAObject::isReverse()
{
    return (flag_ & 4) != 0;
}

ARM void DSSAObject::setupDraw()
{
    VecFx32 scale = { FX32_ONE, FX32_ONE, FX32_ONE };
    func_0206ae30(&scale);
    func_0206ae08(&position_);
    func_0206adcc();
    func_0206dcf0();
    if (dssaData_.texture_) {
        ((TextureObject*)dssaData_.texture_)->unkfunc_02086abc();
        ((TextureObject*)dssaData_.texture_)->unkfunc_02086b3c();
    }
    if (calcType_) {
        baseScale_.vx = defaultScale2_ * scale_.vx;
        baseScale_.vy = defaultScale2_ * scale_.vy;
    } else {
        baseScale_.vx = defaultScale2_;
        baseScale_.vy = defaultScale2_;
    }
}

ARM void DSSAObject::setupRoot()
{
    DSSAParts* root = dssaData_.getParts(0);
    int x = root->getPosX() / 4096;
    int y = root->getPosY() / 4096;
    loadBillboardMatrix();
    if (calcType_) {
        G3_Scale(defaultScale_.value, defaultScale_.value, FX32_ONE);
    } else {
        dss::Fix32Vector3 scale;
        scale.vx = defaultScale_ * scale_.vx;
        scale.vy = defaultScale_ * scale_.vx;
        if (isReverse()) {
            scale.vx.value = -scale.vx.value;
        }
        G3_Scale(scale.vx.value, scale.vy.value, FX32_ONE);
    }
    G3_Translate(x * baseScale_.vx.value, -y * baseScale_.vy.value, 0);
    G3_Color(0x7fff);
}

int DSSAObject::priorityShift_ = 1;
inline const long& zero2L() { return 0L; }

ARM void DSSAObject::setPriority(int priority)
{
    priorityShift_ = priority;
}

ARM void DSSAObject::setupTRS(DSSAParts* parts)
{
    int index = parts->getPartsIndex();
    posY_ *= -1;
    sizeY_ *= -1;
    if (baseScale_.vx == dss::Fix32(FX32_ONE)) {
        G3_Translate(posX_ - sizeX_ / 2, posY_ - sizeY_ / 2, priority_ * (1 << priorityShift_));
    } else {
        G3_Translate(((posX_ - sizeX_ / 2) * baseScale_.vx.value) >> 12, ((posY_ - sizeY_ / 2) * baseScale_.vy.value) >> 12, priority_ * (1 << priorityShift_));
    }
    G3_Translate(dssaData_.getOriginX(index) * baseScale_.vx.value, -(dssaData_.getOriginY(index) * baseScale_.vy.value), 0);
    func_02065c9c(FX_SinIdx(angle_), FX_CosIdx(angle_));
    G3_Scale(scaleX_, scaleY_, FX32_ONE);
    G3_Translate(-(dssaData_.getOriginX(index) * baseScale_.vx.value), dssaData_.getOriginY(index) * baseScale_.vy.value, 0);
    if (baseScale_.vx == dss::Fix32(FX32_ONE)) {
        G3_Scale(sizeX_, sizeY_, FX32_ONE);
    } else {
        G3_Scale((sizeX_ * baseScale_.vx.value) >> 12, (sizeY_ * baseScale_.vy.value) >> 12, FX32_ONE);
    }
}

ARM void DSSAObject::drawParts(DSSAParts* parts)
{
    setupTRS(parts);
    int index = parts->getPartsIndex();
    int left;
    int top;
    int right;
    int bottom;
    if (parts->getFlipX() && parts->getFlipY()) {
        left = dssaData_.getAreaRight(index) << 12;
        top = dssaData_.getAreaBottom(index) << 12;
        right = dssaData_.getAreaLeft(index) << 12;
        bottom = dssaData_.getAreaTop(index) << 12;
    } else if (parts->getFlipX()) {
        left = dssaData_.getAreaRight(index) << 12;
        top = dssaData_.getAreaTop(index) << 12;
        right = dssaData_.getAreaLeft(index) << 12;
        bottom = dssaData_.getAreaBottom(index) << 12;
    } else if (parts->getFlipY()) {
        left = dssaData_.getAreaLeft(index) << 12;
        top = dssaData_.getAreaBottom(index) << 12;
        right = dssaData_.getAreaRight(index) << 12;
        bottom = dssaData_.getAreaTop(index) << 12;
    } else {
        left = dssaData_.getAreaLeft(index) << 12;
        top = dssaData_.getAreaTop(index) << 12;
        right = dssaData_.getAreaRight(index) << 12;
        bottom = dssaData_.getAreaBottom(index) << 12;
    }
    G3_Begin(1);
    G3_TexCoord(left, bottom);
    G3_Vtx(0, FX32_ONE, 0);
    G3_TexCoord(right, bottom);
    G3_Vtx(FX32_ONE, FX32_ONE, 0);
    G3_TexCoord(right, top);
    G3_Vtx(FX32_ONE, 0, 0);
    G3_TexCoord(left, top);
    G3_Vtx(0, 0, 0);
    G3_End();
}

ARM void DSSAScreenObject::setupDraw()
{
    func_0206ae08(&position_);
    func_0206adcc();
    func_0206dcf0();
    setupOrtho2D();
    if (dssaData_.texture_) {
        ((TextureObject*)dssaData_.texture_)->unkfunc_02086abc();
        ((TextureObject*)dssaData_.texture_)->unkfunc_02086abc();
    }
    baseScale_.vx = 1L;
    baseScale_.vy = 1L;
    baseScale_.vz = 1L;
}

ARM void DSSAScreenObject::setupRoot()
{
    DSSAParts* root = dssaData_.getParts(0);
    int x = root->getPosX() / 4096;
    int y = root->getPosY() / 4096;
    isReverse();
    G3_Scale(FX32_ONE, FX32_ONE, FX32_ONE);
    G3_Translate((x + 128) * baseScale_.vx.value, (y + 96) * baseScale_.vy.value, 0);
    G3_Color(0x7fff);
}

ARM void DSSAScreenObject::setupTRS(DSSAParts* parts)
{
    int index = parts->getPartsIndex();
    int z = priority_ << 12;
    int y = (baseScale_.vy.value >> 12) * (posY_ - sizeY_ / 2);
    y += position_.vy.value;
    int x = (baseScale_.vx.value >> 12) * (posX_ - sizeX_ / 2);
    G3_Translate(x + position_.vx.value, y, z);
    G3_Translate(dssaData_.getOriginX(index) * baseScale_.vx.value, dssaData_.getOriginY(index) * baseScale_.vy.value, 0);
    func_02065c9c(-FX_SinIdx(angle_), FX_CosIdx(angle_));
    G3_Scale(scaleX_, scaleY_, FX32_ONE);
    G3_Translate(-(dssaData_.getOriginX(index) * baseScale_.vx.value), -(dssaData_.getOriginY(index) * baseScale_.vy.value), 0);
    G3_Scale(sizeX_ * (baseScale_.vx.value >> 12), sizeY_ * (baseScale_.vy.value >> 12), FX32_ONE);
}

ARM void DSSAScreenObject::drawParts(DSSAParts* parts)
{
    if (parts->getAlpha()) {
        return;
    }
    setupTRS(parts);
    int index = parts->getPartsIndex();
    int left;
    int top;
    int right;
    int bottom;
    if (parts->getFlipX() && parts->getFlipY()) {
        left = dssaData_.getAreaRight(index) << 12;
        top = dssaData_.getAreaBottom(index) << 12;
        right = dssaData_.getAreaLeft(index) << 12;
        bottom = dssaData_.getAreaTop(index) << 12;
    } else if (parts->getFlipX()) {
        left = dssaData_.getAreaRight(index) << 12;
        top = dssaData_.getAreaTop(index) << 12;
        right = dssaData_.getAreaLeft(index) << 12;
        bottom = dssaData_.getAreaBottom(index) << 12;
    } else if (parts->getFlipY()) {
        left = dssaData_.getAreaLeft(index) << 12;
        top = dssaData_.getAreaBottom(index) << 12;
        right = dssaData_.getAreaRight(index) << 12;
        bottom = dssaData_.getAreaTop(index) << 12;
    } else {
        left = dssaData_.getAreaLeft(index) << 12;
        top = dssaData_.getAreaTop(index) << 12;
        right = dssaData_.getAreaRight(index) << 12;
        bottom = dssaData_.getAreaBottom(index) << 12;
    }
    G3_Begin(1);
    G3_TexCoord(left, bottom);
    G3_Vtx(0, FX32_ONE, 0);
    G3_TexCoord(right, bottom);
    G3_Vtx(FX32_ONE, FX32_ONE, 0);
    G3_TexCoord(right, top);
    G3_Vtx(FX32_ONE, 0, 0);
    G3_TexCoord(left, top);
    G3_Vtx(0, 0, 0);
    G3_End();
}
