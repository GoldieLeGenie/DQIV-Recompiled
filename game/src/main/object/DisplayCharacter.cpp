#pragma ipa file
#include "main/object/DisplayCharacter.hpp"
#include "main/dss/RenderObject.hpp"
#include "main/fld/FldStage.hpp"
#include "main/object/DSSAObject.hpp"
#include "nitro/fx/fx_trig.h"

dss::Fix32 DisplayCharacter::workScale_(2.0f);
int DisplayCharacter::headEnable_ = 1;
int DisplayCharacter::shadowType_ = 0xe10000;
dss::Fix32Vector3 DisplayCharacter::boxTestRotate_;
dss::Fix32Vector3 DisplayCharacter::boxTestScale_(1.25f, 1.25f, 1.25f);
GXBoxTestParam DisplayCharacter::boxTestParam_;
dss::Fix32 DisplayCharacter::boxTestRate_(FX32_ONE);
dss::Fix32 DisplayCharacter::nearBodyOffset_(0.375f);
dss::Fix32 DisplayCharacter::nearHeadOffset_(0.28f);
dss::Fix32 DisplayCharacter::sleepHeight_(0.194f);
dss::Fix32 DisplayCharacter::sleepBodyOffset_(0.27f);
dss::Fix32 DisplayCharacter::sleepHeadOffset_(0.173f);
dss::Fix32Vector3 DisplayCharacter::sleepOffset_[4] = {
    dss::Fix32Vector3(0.0f, 0.0f, 0.5f),
    dss::Fix32Vector3(0.5f, 0.0f, 0.0f),
    dss::Fix32Vector3(0.0f, 0.0f, -0.5f),
    dss::Fix32Vector3(-0.5f, 0.0f, 0.0f),
};

ARM void DisplayCharacter::setShadowType(int type)
{
    shadowType_ = type;
}

ARM void DisplayCharacter::setEnable(int enable)
{
    headEnable_ = enable;
}

ARM DisplayCharacter::DisplayCharacter()
{
}

ARM DisplayCharacter::~DisplayCharacter()
{
}

ARM void DisplayCharacter::setTexture(void* data)
{
    BillboardCharacter::setTexture(data);
    head_.texture_ = RenderObject::texture_;
}

ARM void DisplayCharacter::resetTexture()
{
    BillboardCharacter::resetTexture();
    head_.texture_ = RenderObject::texture_;
}

ARM void DisplayCharacter::setup(const char* name, int sleep)
{
    BillboardCharacter::setup(name);

    BillboardVertex vertex = *unkfunc_02084134();
    BillboardTexCoord texCoord = *unkfunc_020841a8();
    vertex.v[0].vy = vertex.v[3].vy * 7 / 16;
    vertex.v[1].vy = vertex.v[2].vy * 7 / 16;
    texCoord.v[0].vy = texCoord.v[0].vy * 9 / 16;
    texCoord.v[1].vy = texCoord.v[1].vy * 9 / 16;
    head_.unkfunc_020840c8(&vertex);
    head_.unkfunc_0208413c(&texCoord);
    head_.texture_ = RenderObject::texture_;
    head_.setScale(workScale_);
    head_.setPolygonID(1);

    BillboardVertex bodyVertex = *unkfunc_02084134();
    BillboardTexCoord bodyTexCoord = *unkfunc_020841a8();
    bodyVertex.v[3].vy = bodyVertex.v[3].vy * 1 / 2;
    bodyVertex.v[2].vy = bodyVertex.v[2].vy * 1 / 2;
    bodyTexCoord.v[2].vy = bodyTexCoord.v[0].vy * 1 / 2;
    bodyTexCoord.v[3].vy = bodyTexCoord.v[1].vy * 1 / 2;
    unkfunc_020840c8(&bodyVertex);
    unkfunc_0208413c(&bodyTexCoord);
    RenderObject::setPolygonID(1);

    if (sleep) {
        setNearFlag(1);
        setSleepFlag(1);
    }
    boxTestParam_.x = 0;
    boxTestParam_.y = 0;
    boxTestParam_.z = 0;
    boxTestParam_.width = FX32_ONE;
    boxTestParam_.height = FX32_ONE;
    boxTestParam_.depth = FX32_ONE;
}

ARM void DisplayCharacter::cleanup()
{
    BillboardCharacter::cleanup();
}

ARM void DisplayCharacter::setRender(Render* render)
{
    RenderObject::setRender(render);
}

ARM void DisplayCharacter::removeRender()
{
    RenderObject::removeRender();
}

ARM void DisplayCharacter::setAlpha(int alpha)
{
    RenderObject::setAlpha(alpha);
    head_.setAlpha(alpha);
    setShadowAlpha((unsigned char)(alpha * 12 / 31));
}

ARM int DisplayCharacter::box_testx1()
{
    if (flag_ & 0x2000) {
        return 0;
    }
    dss::Fix32Vector3 position = *getPosition();
    position.vx.value -= 0x800;
    unkfunc_020484ec((VecFx32*)&position, (VecFx32*)&boxTestRotate_, (VecFx32*)&boxTestScale_, (VecFx32*)&boxTestParam_, &boxTestRate_);
    int result;
    while (func_02065544(&result)) {
    }
    return result == 0;
}

ARM void DisplayCharacter::draw()
{
    if (!isDisplayEnable() || !unkfunc_020497d4() || !enable_ || !alpha_) {
        flag_ |= 0x1000;
        return;
    }
    if (box_testx1()) {
        if (!((flag_ & 0x80) ? 1 : 0)) {
            execute();
        }
        flag_ |= 0x1000;
        return;
    }
    dss::Fix32Vector3 position = *getPosition();
    exec();
    if (flag_ & 0x80) {
        startAnimation(1);
        drawSleepCharacter();
    } else {
        BillboardCharacter::draw();
        if (headEnable_) {
            execScale();
            head_.unkfunc_020841b0(&uvOffset);
            head_.draw();
        }
    }
    setPosition(position);
}

ARM void DisplayCharacter::drawSleepCharacter()
{
    if (!enable_) {
        return;
    }
    setScale(data_020f22c4);
    unkfunc_020498e8(data_020f22c4);
    setCameraDirection(&unkfunc_02049994()->getDirection());
    for (int i = 0; i < 4; i++) {
        sleepOffset_[i].vy = sleepHeight_;
    }
    dss::Fix32Vector3 position = *head_.getPosition() + sleepOffset_[(direction_ / 0x4000) & 3];

    BillboardVertex* vertex = unkfunc_02084134();
    BillboardTexCoord texCoord = *unkfunc_020841a8();
    dss::Vector2<dss::Fix32> offset = uvOffset;
    for (int i = 0; i < 4; i++) {
        texCoord.v[i].vx += offset.vx;
        texCoord.v[i].vy += offset.vy;
    }
    drawQuad(position, vertex, &texCoord);

    BillboardVertex* headVertex = head_.unkfunc_02084134();
    BillboardTexCoord headTexCoord = *head_.unkfunc_020841a8();
    dss::Vector2<dss::Fix32> headOffset = uvOffset;
    for (int i = 0; i < 4; i++) {
        headTexCoord.v[i].vx += headOffset.vx;
        headTexCoord.v[i].vy += headOffset.vy;
    }
    drawQuad(position, headVertex, &headTexCoord);
}

ARM void DisplayCharacter::drawQuad(dss::Fix32Vector3& position, BillboardVertex* vertex, BillboardTexCoord* texCoord)
{
    G3_PushMtx();
    G3_Translate(position.vx.value, position.vy.value, position.vz.value);
    unkfunc_020843d4();
    G3_Scale(scale_.vx.value, scale_.vx.value, scale_.vx.value);
    if (RenderObject::texture_) {
        ((TextureObject*)RenderObject::texture_)->unkfunc_02086abc();
        ((TextureObject*)RenderObject::texture_)->unkfunc_02086b3c();
    } else {
        unkfunc_02086b28();
    }
    unkfunc_02083680();
    unkfunc_020841d4();
    G3_PushMtx();
    unsigned short angle = 0xa000 - dispDirection_;
    func_02065c9c(FX_SinIdx(angle), FX_CosIdx(angle));
    if (reverse_) {
        G3_Begin(1);
        G3_Color(0x7fff);
        for (int i = 0; i < 4; i++) {
            G3_TexCoord(texCoord->v[i].vx.value, texCoord->v[i].vy.value);
            G3_Vtx(vertex->v[i].vx.value, vertex->v[i].vy.value, 0);
        }
        G3_End();
    } else {
        G3_Begin(1);
        G3_Color(0x7fff);
        for (int i = 0; i < 4; i++) {
            G3_TexCoord(texCoord->v[i].vx.value, texCoord->v[i].vy.value);
            G3_Vtx(vertex->v[i].vx.value, vertex->v[i].vy.value, 0);
        }
        G3_End();
    }
    G3_PopMtx(1);
    G3_PopMtx(1);
}

ARM void DisplayCharacter::execScale()
{
    dss::Fix32Vector3& cameraPosition = unkfunc_02049994()->getPosition();
    dss::Fix32Vector3 position = *getPosition();
    dss::Fix32Vector3 distance = cameraPosition - position;
    dss::Fix32 length = distance.length();
    dss::Fix32 offset;
    if (!(flag_ & 0x80)) {
        offset = nearBodyOffset_;
    } else {
        offset = sleepBodyOffset_;
    }
    dss::Fix32 rate = length - offset.value;
    workScale_ = getScale()->vx * rate / length;
    setScale(workScale_);

    dss::Fix32 headOffset;
    if (!(flag_ & 0x80)) {
        headOffset = nearHeadOffset_;
    } else {
        headOffset = sleepHeadOffset_;
    }
    dss::Fix32 headRate = length - headOffset.value;
    workScale_ = getScale()->vx * headRate / length;
    head_.setScale(workScale_);
}

ARM void DisplayCharacter::exec()
{
    dss::Fix32Vector3& cameraPosition = unkfunc_02049994()->getPosition();
    dss::Fix32Vector3 position = *getPosition();
    dss::Fix32Vector3 distance = cameraPosition - position;
    dss::Fix32 length = distance.length();
    distance.normalize();
    dss::Fix32 offset;
    if (!(flag_ & 0x80)) {
        offset = nearBodyOffset_;
    } else {
        offset = sleepBodyOffset_;
    }
    dss::Fix32Vector3 move = distance * offset;
    position = *getPosition() + move;
    setPosition(position);
    dss::Fix32 rate = length - offset.value;
    workScale_ = getScale()->vx * rate / length;
    setScale(workScale_);

    dss::Fix32 headOffset;
    if (!(flag_ & 0x80)) {
        headOffset = nearHeadOffset_;
    } else {
        headOffset = sleepHeadOffset_;
    }
    dss::Fix32Vector3 headMove = distance * headOffset;
    position = *getPosition() + headMove;
    head_.setPosition(position);
    dss::Fix32 headRate = length - headOffset.value;
    workScale_ = getScale()->vx * headRate / length;
    head_.setScale(workScale_);
}

ARM void DisplayCharacter::setColor(int color)
{
    unkfunc_020842b8(color);
    head_.unkfunc_020842b8(color);
}

ARM void DisplayCharacter::setBoxTestOff(bool off)
{
    if (off) {
        flag_ |= 0x2000;
    } else {
        flag_ &= ~0x2000;
    }
}

ARM void DisplayCharacter::setSleep(int sleep)
{
    setNearFlag(sleep);
    setSleepFlag(sleep);
}
