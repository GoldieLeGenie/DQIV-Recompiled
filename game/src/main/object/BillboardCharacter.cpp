#pragma ipa file
#define BILLBOARD_CHARACTER_TU
#include "main/object/BillboardCharacter.hpp"
#include "main/object/BillboardLiterals.hpp"
namespace dss {
template<typename T> inline Vector3<T>::Vector3() : vz(0L), vy(0L), vx(0L) {}
template<typename T> inline Vector2<T>::Vector2() : vy(bcLiteral_Ey), vx(bcLiteral_Ex) {}
}
#include "main/dss/Matrix43.hpp"
#include "main/dss/UnkMemory.hpp"
#include "main/dss/PaletteEffect.hpp"
#include "main/formation/FormationId.hpp"
#include "nitro/fx/fx_atan.h"
#include "nitro/os.hpp"

namespace dss {
#ifndef DQIV_NATIVE
    template <>
    inline Vector3<Fix16>::Vector3() : vz(bcLiteral_Tz), vy(bcLiteral_Ty), vx(bcLiteral_Tx) {}
#endif
}


struct BillboardAnimationFrame {
    int index;
    unsigned int time;
};



static dss::Vector2<dss::Fix16> s_vertexLeftBottom(-0.24f, -0.0f);
static dss::Vector2<dss::Fix16> s_vertexRightBottom(0.24f, -0.0f);
static dss::Vector2<dss::Fix16> s_vertexRightTop(0.24f, 0.64f);
static dss::Vector2<dss::Fix16> s_vertexLeftTop(-0.24f, 0.64f);
#pragma explicit_zero_data on
long bcLiteral_Ex = 0;
#pragma explicit_zero_data off
#pragma explicit_zero_data on
unsigned short bcLiteral_C0 = 0;
#pragma explicit_zero_data off
static dss::Vector2<dss::Fix32> s_texCoordLeftTop(0, 0);
static dss::Vector2<dss::Fix32> s_texCoordRightTop(0x18, 0);
static dss::Vector2<dss::Fix32> s_texCoordRightBottom(0x18, 0x20);
static dss::Vector2<dss::Fix32> s_texCoordLeftBottom(0, 0x20);
static dss::Vector2<dss::Fix32> s_texCoordStep(0x18, 0);
static BillboardVertex s_vertex(s_vertexLeftBottom, s_vertexRightBottom, s_vertexRightTop, s_vertexLeftTop);
#pragma explicit_zero_data on
unsigned short bcLiteral_C1 = 0;
#pragma explicit_zero_data off
static BillboardTexCoord s_texCoord(s_texCoordLeftBottom, s_texCoordRightBottom, s_texCoordRightTop, s_texCoordLeftTop);
static void* s_shadowTexture;
#pragma explicit_zero_data on
float bcLiteral_Scale2 = 1.927f;
#pragma explicit_zero_data off
int BillboardCharacter::changeAngle_;
#pragma explicit_zero_data on
float bcLiteral_Scale = 1.915f;
#pragma explicit_zero_data off
dss::BitFlag<unsigned char> BillboardCharacter::allFlag_(4);
#pragma explicit_zero_data on
long bcLiteral_Tx = 0;
#pragma explicit_zero_data off
int BillboardCharacter::allAnimLock;
static dss::Fix32 s_scale(bcLiteral_Scale);
dss::Fix32 data_020f22c4(bcLiteral_Scale2);
static dss::Vector3<dss::Fix16> s_shadowVertexRightTop(1.0f, 0.0f, 1.0f);
static dss::Vector3<dss::Fix16> s_shadowVertexRightBottom(1.0f, 0.0f, -1.0f);
dss::Camera* BillboardCharacter::camera_;
#pragma explicit_zero_data on
unsigned short bcLiteral_C2 = 0;
#pragma explicit_zero_data off
static int s_shadowEnable = 1;
static dss::Vector3<dss::Fix16> s_shadowVertexLeftBottom(-1.0f, 0.0f, -1.0f);
static dss::Vector3<dss::Fix16> s_shadowVertexLeftTop(-1.0f, 0.0f, 1.0f);
static dss::Vector2<dss::Fix32> s_shadowTexCoordLeftTop(0, 0);
static dss::Vector2<dss::Fix32> s_shadowTexCoordLeftBottom(0, 0x20);
static dss::Vector2<dss::Fix32> s_shadowTexCoordRightBottom(0x20, 0x20);
static dss::Vector2<dss::Fix32> s_shadowTexCoordRightTop(0x20, 0);
static PolygonVertex s_shadowVertex(s_shadowVertexRightTop, s_shadowVertexRightBottom, s_shadowVertexLeftBottom,
                                    s_shadowVertexLeftTop);
static BillboardTexCoord s_shadowTexCoord(s_shadowTexCoordLeftTop, s_shadowTexCoordLeftBottom, s_shadowTexCoordRightBottom,
                                          s_shadowTexCoordRightTop);
static int s_shadowType;
static DataObject s_shadowData;
#pragma explicit_zero_data on
long bcLiteral_Ty = 0;
#pragma explicit_zero_data off
static BillboardAnimationFrame s_animation[] = {
    { 0, 12 },
    { 1, 12 },
    { 2, 12 },
    { 1, 12 },
    { -1, -1 },
};
static dss::Fix32Vector3 s_shadowScale(0.347f, 10.0f, 0.206f);
static dss::Vector3<short> s_unused;
static dss::Vector3<dss::Fix16> s_sideVertex[4];
static dss::Fix32Vector3 s_sideDirection[32];
static PolygonVertex s_side;
ARM int unkfunc_unused_33(int index) { static const unsigned char table[12] = {1,2,3}; return table[index]; }
ARM int unkfunc_unused_32(int index) { static const unsigned char table[512] = {1,2,3}; return table[index]; }
#pragma explicit_zero_data on
long bcLiteral_Tz = 0;
#pragma explicit_zero_data off
ARM int unkfunc_unused_31(int index) { static const unsigned char table[512] = {1,2,3}; return table[index]; }
static ShadowPolygon s_shadowPolygons[32];
#pragma explicit_zero_data on
long bcLiteral_Ey = 0;
#pragma explicit_zero_data off

ARM BillboardCharacter::BillboardCharacter() : direction_(0), flag_(0)
{
    flag_ = 0;
    texture_ = NULL;
    changeAngle_ = 1;
}

ARM void BillboardCharacter::setTexture(void* data)
{
    textureNum_ = getArchiveFileCount(data);
    for (int i = 0; i < textureNum_; i++) {
        textures_[i] = getArchiveFile(data, i);
    }
    OS_Wait();
    unkfunc_02058680(&s_vertex, &s_texCoord, textures_[0]);
    textureIndex_ = -1;
}

ARM void BillboardCharacter::resetTexture()
{
    setTexture(data_.getAddr());
    textureIndex_ = -1;
}

ARM void BillboardCharacter::unkfunc_02049190()
{
    ((TextureObject*)RenderObject::texture_)->unkfunc_02086868();
}

ARM void BillboardCharacter::setup(const char* name)
{
    data_.setup(name, 0, 0);
    unkfunc_020491cc();
}

ARM void BillboardCharacter::unkfunc_020491cc()
{
    enable_ = 1;
    flag_ |= FLAG_DEFAULT;
    setTexture(data_.getAddr());
    anmIndex_ = 0;
    anmTime_ = 0;
    textureIndex_ = -1;
    preDirection_ = 5;
    setScale(s_scale);
    shadow_.unkfunc_02049ed8(0);
}

ARM void BillboardCharacter::cleanup()
{
    removeRender();
    unkfunc_020586c4();
    data_.cleanup();
    shadow_.unkfunc_02049f70();
    enable_ = 0;
    flag_ = 0;
}

ARM void BillboardCharacter::draw()
{
    if (!enable_) {
        flag_ |= FLAG_DRAW_RESET;
        return;
    }
    setScale(s_scale);
    unkfunc_020498e8(s_scale);
    setCameraDirection(&camera_->getDirection());
    if (flag_ & FLAG_ANIM_PALLET) {
        applyTexturePaletteRgbRate((TextureObject*)texture_, &unk_fc.vx);
    }
    execute();
    Billboard::draw();
    if (flag_ & FLAG_SHADOW) {
        shadow_.draw();
    }
    flag_ &= ~FLAG_DRAW_RESET;
}

ARM void BillboardCharacter::execute()
{
    if (allAnimLock == 1) {
        return;
    }
    if (flag_ & FLAG_ANIM_NEUTRAL) {
        if (!(allFlag_.flag_ & 4)) {
            return;
        }
    } else if (!(flag_ & FLAG_ANIM)) {
        return;
    }
    if (flag_ & FLAG_WRIGGLE) {
        anmTime_ += 2;
    } else {
        anmTime_ += 1;
    }
    if (anmTime_ >= s_animation[anmIndex_].time) {
        anmTime_ = 0;
        anmIndex_++;
        if (s_animation[anmIndex_].index == -1) {
            anmTime_ = 0;
            anmIndex_ = 0;
        }
    }
    startAnimation(anmIndex_);
}

ARM void BillboardCharacter::startAnimation(int index)
{
    uvOffset.set(s_texCoordStep.vx * s_animation[index].index, s_texCoordStep.vy * s_animation[index].index);
    unkfunc_020841b0(&uvOffset);
}

ARM void BillboardCharacter::setRotate(unsigned short direction)
{
    direction_ = direction;
}

ARM void BillboardCharacter::unkfunc_02049494(int index)
{
    if (textureIndex_ == index && !(flag_ & FLAG_RELOAD)) {
        return;
    }
    textureIndex_ = index;
    unkfunc_020586d4((int)textures_[index]);
    flag_ &= ~FLAG_RELOAD;
}

ARM void BillboardCharacter::setCameraDirection(const dss::Fix32Vector3* direction)
{
    if (!(flag_ & FLAG_SLEEP)) {
#ifdef DQIV_NATIVE
        // A requested reload (after the travel door) is done when the facing's
        // texture is chosen. On the DS the party skips every other frame's
        // draw, which forces that choice; the joined picture draws it every
        // frame, so the reload waited for the first step (thin lines meanwhile).
        if (direction_ == preDirection_ && changeAngle_ == 0 && !(flag_ & (FLAG_DRAW_RESET | FLAG_RELOAD))) {
#else
        if (direction_ == preDirection_ && changeAngle_ == 0 && !(flag_ & FLAG_DRAW_RESET)) {
#endif
            return;
        }
        preDirection_ = direction_;
    }
    u16 angle = FX_Atan2Idx(direction->vx.value, direction->vz.value);
    int i;
    dispDirection_ = angle - direction_ + 0x10000;
    dispDirection_ = (u16)dispDirection_;
    if (textureNum_ == 4) {
        dispDirection_ = (u16)(dispDirection_ + 0x2000);
        for (int min = i = 0; i < 4; i++, min += 0x4000) {
            if (formation::FormationId::isRange(dispDirection_, min, min + 0x4000)) {
                if (flag_ & FLAG_SLEEP) {
                    if (i == 0) {
                        i = 2;
                    }
                    unkfunc_02049494(i);
                    reverse_ = i % 2;
                } else {
                    unkfunc_02049494(i);
                }
                break;
            }
        }
    } else if (!(flag_ & FLAG_SLEEP)) {
        dispDirection_ = (u16)(dispDirection_ + 0x1000);
        for (int min = i = 0; i < 8; i++, min += 0x2000) {
            if (formation::FormationId::isRange(dispDirection_, min, min + 0x2000)) {
                unkfunc_02049494(i);
            }
        }
    } else {
        dispDirection_ = (u16)(dispDirection_ + 0x2000);
        for (i = 0; i < 4; i++) {
            if (formation::FormationId::isRange(dispDirection_, i << 14, (i << 14) + 0x4000)) {
                if (i == 0) {
                    i = 2;
                }
                unkfunc_02049494(i * 2);
                reverse_ = i % 2;
            }
        }
    }
    if (!CharacterShadow::unkfunc_0204a034()) {
        return;
    }
    dss::Vector3<unsigned short> rotation;
    rotation.set(bcLiteral_C0, bcLiteral_C1, bcLiteral_C2);
    rotation.vy = angle;
    shadow_.setRotationIdx(rotation);
}

ARM void BillboardCharacter::setPosition(const dss::Fix32Vector3& position)
{
    Position::setPosition(position);
    dss::Fix32Vector3 shadowPosition = position;
    if (flag_ & FLAG_STAY) {
        shadow_.setPosition(position_);
    }
}

ARM void BillboardCharacter::setShadowPos(const dss::Fix32Vector3* position)
{
    shadow_.setPosition(*position);
}

ARM void BillboardCharacter::unkfunc_0204977c(int type)
{
    CharacterShadow::unkfunc_0204a024(type);
}

ARM void BillboardCharacter::setShadowAlpha(int alpha)
{
    shadow_.setAlpha(alpha);
}

ARM void BillboardCharacter::setDisplayEnable(int flag)
{
    if (flag) {
        flag_ |= FLAG_ENABLE;
    } else {
        flag_ &= ~FLAG_ENABLE;
    }
}

ARM void BillboardCharacter::unkfunc_020497bc(int flag)
{
    if (flag) {
        flag_ |= FLAG_DISPLAY;
    } else {
        flag_ &= ~FLAG_DISPLAY;
    }
}

ARM int BillboardCharacter::unkfunc_020497d4()
{
    return (flag_ & FLAG_DISPLAY) ? 1 : 0;
}

ARM int BillboardCharacter::isDisplayEnable()
{
    return (flag_ & FLAG_ENABLE) ? 1 : 0;
}

ARM void BillboardCharacter::setShadowStay(int flag)
{
    if (flag) {
        flag_ |= FLAG_STAY;
    } else {
        flag_ &= ~FLAG_STAY;
    }
}

ARM void BillboardCharacter::setAnimFlag(int type)
{
    if (type == 1) {
        flag_ |= FLAG_ANIM;
        flag_ &= ~FLAG_ANIM_NEUTRAL;
    } else if (type == 2) {
        flag_ &= ~FLAG_ANIM;
        flag_ |= FLAG_ANIM_NEUTRAL;
    } else {
        flag_ &= ~FLAG_ANIM;
        flag_ &= ~FLAG_ANIM_NEUTRAL;
    }
}

ARM void BillboardCharacter::setNearFlag(int flag)
{
    if (flag) {
        flag_ |= FLAG_NEAR;
    } else {
        flag_ &= ~FLAG_NEAR;
    }
}

ARM void BillboardCharacter::setShadowFlag(int flag)
{
    if (flag) {
        flag_ |= FLAG_SHADOW;
    } else {
        flag_ &= ~FLAG_SHADOW;
    }
}

ARM void BillboardCharacter::setSleepFlag(int flag)
{
    if (!flag) {
        flag_ &= ~FLAG_SLEEP;
        return;
    }
    anmIndex_ = 1;
    startAnimation(1);
    flag_ |= FLAG_SLEEP;
}

ARM void BillboardCharacter::setWriggleFlag(int flag)
{
    if (flag) {
        flag_ |= FLAG_WRIGGLE;
    } else {
        flag_ &= ~FLAG_WRIGGLE;
    }
}

ARM void BillboardCharacter::unkfunc_020498e8(dss::Fix32 scale)
{
    dss::Fix32Vector3& cameraPosition = camera_->getPosition();
    dss::Fix32 rate = cameraPosition.lengthsq(*getPosition()) / camera_->getDistanceSq();
    rate = rate.sqrt();
    setScale(scale * rate);
}

ARM void BillboardCharacter::setCamera(dss::Camera* camera)
{
    camera_ = camera;
}

ARM dss::Camera* BillboardCharacter::unkfunc_02049994()
{
    return camera_;
}

ARM void BillboardCharacter::setAllCharaAnim(int flag)
{
    if (flag != 0) {
        allFlag_.flag_ |= 4;
    } else {
        allFlag_.flag_ &= ~4;
    }
}

ARM bool BillboardCharacter::isAllAnimation()
{
    return allFlag_.check(4);
}

ARM void BillboardCharacter::unkfunc_020499f0(const dss::Fix32Vector3* rate)
{
    unk_fc = *rate;
}

ARM void BillboardCharacter::unkfunc_02049a00(int flag)
{
    if (flag) {
        flag_ |= FLAG_ANIM_PALLET;
    } else {
        flag_ &= ~FLAG_ANIM_PALLET;
    }
}

ARM void BillboardCharacter::unkfunc_02049a18()
{
    flag_ |= FLAG_RELOAD;
    shadow_.unkfunc_02049f70();
    shadow_.unkfunc_02049ed8(0);
}

ARM CharacterShadow::CharacterShadow()
{
}



ARM CharacterShadow::~CharacterShadow()
{
}

ARM void CharacterShadow::unkfunc_02049b94()
{
    unkfunc_02049eb4();
    unkfunc_02049ba4();
}

ARM void CharacterShadow::unkfunc_02049ba4()
{
    s_shadowData.setup("data/common.tex", 0, 0);
    s_shadowTexture = s_shadowData.getAddr();
    ((TextureObject*)s_shadowTexture)->unkfunc_02086798(1);
    dss::Matrix43 matrix;
    dss::Fix32Vector3 direction;
    dss::Fix32Vector3 work;
    direction.set(1.0f, 0.0f, 0.0f);
    s_sideDirection[0] = direction;
    for (int i = 1, angle = 0x2000; i < 9; angle += 0x2000, i++) {
        matrix.unkfunc_020886d0(angle);
        s_sideDirection[i] = matrix * direction;
    }
    for (int i = 0; i < 8; i++) {
        s_sideVertex[0].vx = s_sideDirection[i].vx;
        s_sideVertex[0].vy = s_sideDirection[i].vy;
        s_sideVertex[0].vz = s_sideDirection[i].vz;
        s_sideVertex[0].vy = -2.0f;
        s_sideVertex[1].vx = s_sideDirection[i + 1].vx;
        s_sideVertex[1].vy = s_sideDirection[i + 1].vy;
        s_sideVertex[1].vz = s_sideDirection[i + 1].vz;
        s_sideVertex[1].vy = -2.0f;
        s_sideVertex[2].vx = s_sideDirection[i + 1].vx;
        s_sideVertex[2].vy = s_sideDirection[i + 1].vy;
        s_sideVertex[2].vz = s_sideDirection[i + 1].vz;
        s_sideVertex[2].vy = 2.0f;
        s_sideVertex[3].vx = s_sideDirection[i].vx;
        s_sideVertex[3].vy = s_sideDirection[i].vy;
        s_sideVertex[3].vz = s_sideDirection[i].vz;
        s_sideVertex[3].vy = 2.0f;
        s_side.v[0] = s_sideVertex[0];
        s_side.v[1] = s_sideVertex[1];
        s_side.v[2] = s_sideVertex[2];
        s_side.v[3] = s_sideVertex[3];
        s_shadowPolygons[i].unkfunc_02083734(&s_side);
        s_shadowPolygons[i].setScale(s_shadowScale);
        s_shadowPolygons[0].unkfunc_02083d00(i, &s_side);
    }
}

ARM void CharacterShadow::unkfunc_02049eb4()
{
    ((TextureObject*)s_shadowTexture)->unkfunc_02086868();
    s_shadowData.cleanup();
}

ARM void CharacterShadow::unkfunc_02049ed8(int type)
{
    s_shadowType = type;
    unkfunc_02083734(&s_shadowVertex);
    unkfunc_020837d0(&s_shadowTexCoord);
    texture_ = s_shadowTexture;
    setAlpha(12);
    dss::Fix32Vector3 offset;
    offset.set(0, 0, 0);
    offset.vy.value = 0x100;
    setScale(s_shadowScale);
}

ARM void CharacterShadow::unkfunc_02049f70()
{
}

ARM void CharacterShadow::draw()
{
    if (!s_shadowEnable || !alpha_) {
        return;
    }
    if (s_shadowType == 0) {
        setScale(s_shadowScale);
        position_.vy.value += 0x100;
        PolygonObject::draw();
        position_.vy.value -= 0x100;
        return;
    }
    s_shadowPolygons[0].setRotationIdx(unk_28);
    s_shadowPolygons[0].setPosition(position_);
    s_shadowPolygons[0].unkfunc_02083b1c();
}

ARM void CharacterShadow::unkfunc_0204a024(int type)
{
    s_shadowType = type;
}

ARM int CharacterShadow::unkfunc_0204a034()
{
    return s_shadowEnable;
}
