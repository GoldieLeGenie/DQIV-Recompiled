#pragma ipa file
#include "ov009/casino/PokerCard.hpp"
#include "main/dss/DssVectorDefault.hpp"

ARM PokerCard::PokerCard()
{
    unkfunc_021249cc();
}

ARM PokerCard::~PokerCard()
{
}

ARM void PokerCard::unkfunc_021249cc()
{
    m_ctrl.clear();
    m_ctrl.flag_ |= 1;
}

ARM void PokerCard::unkfunc_021249dc(dss::Fix32 scale)
{
    unk_000.setScale(scale);
    unk_088.setScale(scale);
    unk_110.setScale(scale);
    unk_198.setScale(scale);
}

ARM void PokerCard::unkfunc_02124a70(const PolygonVertex* vertex)
{
    unk_000.unkfunc_02083734(vertex);
    unk_088.unkfunc_02083734(vertex);
    unk_110.unkfunc_02083734(vertex);
    unk_198.unkfunc_02083734(vertex);
}

ARM void PokerCard::setPosition(dss::Fix32Vector3& pos)
{
    unk_000.setPosition(pos);
    unk_088.setPosition(pos);
    unk_198.setPosition(pos);
    pos.vz.value += 0x100;
    unk_110.setPosition(pos);
}

ARM void PokerCard::setAngle(int angle)
{
    unk_000.angle_ = angle;
    unk_088.angle_ = angle + 0x8000;
}

ARM void PokerCard::unkfunc_02124b24(const BillboardTexCoord* texCoord)
{
    unk_000.unkfunc_020837d0(texCoord);
}

ARM void PokerCard::unkfunc_02124b30(const BillboardTexCoord* texCoord)
{
    unk_088.unkfunc_020837d0(texCoord);
}

ARM void PokerCard::unkfunc_02124b40(const BillboardTexCoord* texCoord)
{
    unk_110.unkfunc_020837d0(texCoord);
    unk_110.setAlpha(12);
    unk_230.v[0] = texCoord->v[0];
    unk_230.v[1] = texCoord->v[1];
    unk_230.v[2] = texCoord->v[2];
    unk_230.v[3] = texCoord->v[3];
}

ARM void PokerCard::unkfunc_02124bcc(const BillboardTexCoord* texCoord)
{
    unk_198.unkfunc_020837d0(texCoord);
}

ARM void PokerCard::unkfunc_02124bdc(void* texture)
{
    unk_000.texture_ = texture;
}

ARM void PokerCard::unkfunc_02124be4(void* texture)
{
    unk_088.texture_ = texture;
}

ARM void PokerCard::unkfunc_02124bec(void* texture)
{
    unk_110.texture_ = texture;
}

ARM void PokerCard::unkfunc_02124bf4(void* texture)
{
    unk_198.texture_ = texture;
    unk_198.setAlpha(1);
}

ARM void PokerCard::draw()
{
    if (!m_ctrl.check(1)) {
        return;
    }
    unk_000.draw();
    unk_088.draw();
    unk_198.draw();
    if (m_effect_enable) {
        BillboardTexCoord texCoord;
        texCoord.v[0] = unk_230.v[0];
        texCoord.v[1] = unk_230.v[1];
        texCoord.v[2] = unk_230.v[2];
        texCoord.v[3] = unk_230.v[3];
        texCoord.v[0].vy.value = unk_228 * m_effect_frame + unk_230.v[0].vy.value;
        texCoord.v[1].vy.value = unk_228 * m_effect_frame + unk_230.v[1].vy.value;
        texCoord.v[2].vy.value = unk_228 * m_effect_frame + unk_230.v[2].vy.value;
        texCoord.v[3].vy.value = unk_228 * m_effect_frame + unk_230.v[3].vy.value;
        unk_110.unkfunc_020837d0(&texCoord);
        unk_110.draw();
        m_effect_frame++;
    }
    if (m_effect_frame == MAX_FRAME) {
        m_effect_enable = 0;
    }
}

ARM void PokerCard::setEffect()
{
    m_effect_enable = 1;
    dss::Fix32Vector3 pos;
    m_effect_frame = 0;
    pos = *unk_088.getPosition();
    pos.vy.value += 0x100;
    unk_110.setPosition(pos);
    unk_198.setPosition(pos);
    unk_228 = 0x3333;
}

ARM void PokerCard::unkfunc_02124e14(bool enable)
{
    if (enable) {
        m_ctrl.flag_ |= 1;
    } else {
        m_ctrl.flag_ &= ~1;
    }
}
