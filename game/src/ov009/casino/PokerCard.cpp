#pragma ipa file
#include "ov009/casino/PokerCard.hpp"
#include "main/dss/DssVectorDefault.hpp"

ARM PokerCard::PokerCard()
{
    initialize();
}

ARM PokerCard::~PokerCard()
{
}

ARM void PokerCard::initialize()
{
    m_ctrl.clear();
    m_ctrl.flag_ |= 1;
}

ARM void PokerCard::setScale(dss::Fix32 scale)
{
    m_front_polygon.setScale(scale);
    m_back_polygon.setScale(scale);
    m_shine_polygon.setScale(scale);
    m_glow_polygon.setScale(scale);
}

ARM void PokerCard::setVertex(const PolygonVertex* vertex)
{
    m_front_polygon.unkfunc_02083734(vertex);
    m_back_polygon.unkfunc_02083734(vertex);
    m_shine_polygon.unkfunc_02083734(vertex);
    m_glow_polygon.unkfunc_02083734(vertex);
}

ARM void PokerCard::setPosition(dss::Fix32Vector3& pos)
{
    m_front_polygon.setPosition(pos);
    m_back_polygon.setPosition(pos);
    m_glow_polygon.setPosition(pos);
    pos.vz.value += 0x100;
    m_shine_polygon.setPosition(pos);
}

ARM void PokerCard::setAngle(int angle)
{
    m_front_polygon.angle_ = angle;
    m_back_polygon.angle_ = angle + 0x8000;
}

ARM void PokerCard::setFrontTexCoord(const BillboardTexCoord* texCoord)
{
    m_front_polygon.unkfunc_020837d0(texCoord);
}

ARM void PokerCard::setBackTexCoord(const BillboardTexCoord* texCoord)
{
    m_back_polygon.unkfunc_020837d0(texCoord);
}

ARM void PokerCard::setShineTexCoord(const BillboardTexCoord* texCoord)
{
    m_shine_polygon.unkfunc_020837d0(texCoord);
    m_shine_polygon.setAlpha(12);
    m_shine_tex_coord.v[0] = texCoord->v[0];
    m_shine_tex_coord.v[1] = texCoord->v[1];
    m_shine_tex_coord.v[2] = texCoord->v[2];
    m_shine_tex_coord.v[3] = texCoord->v[3];
}

ARM void PokerCard::setGlowTexCoord(const BillboardTexCoord* texCoord)
{
    m_glow_polygon.unkfunc_020837d0(texCoord);
}

ARM void PokerCard::setFrontTexture(void* texture)
{
    m_front_polygon.texture_ = texture;
}

ARM void PokerCard::setBackTexture(void* texture)
{
    m_back_polygon.texture_ = texture;
}

ARM void PokerCard::setShineTexture(void* texture)
{
    m_shine_polygon.texture_ = texture;
}

ARM void PokerCard::setGlowTexture(void* texture)
{
    m_glow_polygon.texture_ = texture;
    m_glow_polygon.setAlpha(1);
}

ARM void PokerCard::draw()
{
    if (!m_ctrl.check(1)) {
        return;
    }
    m_front_polygon.draw();
    m_back_polygon.draw();
    m_glow_polygon.draw();
    if (m_effect_enable) {
        BillboardTexCoord texCoord;
        texCoord.v[0] = m_shine_tex_coord.v[0];
        texCoord.v[1] = m_shine_tex_coord.v[1];
        texCoord.v[2] = m_shine_tex_coord.v[2];
        texCoord.v[3] = m_shine_tex_coord.v[3];
        texCoord.v[0].vy.value = m_shine_scroll_speed * m_effect_frame + m_shine_tex_coord.v[0].vy.value;
        texCoord.v[1].vy.value = m_shine_scroll_speed * m_effect_frame + m_shine_tex_coord.v[1].vy.value;
        texCoord.v[2].vy.value = m_shine_scroll_speed * m_effect_frame + m_shine_tex_coord.v[2].vy.value;
        texCoord.v[3].vy.value = m_shine_scroll_speed * m_effect_frame + m_shine_tex_coord.v[3].vy.value;
        m_shine_polygon.unkfunc_020837d0(&texCoord);
        m_shine_polygon.draw();
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
    pos = *m_back_polygon.getPosition();
    pos.vy.value += 0x100;
    m_shine_polygon.setPosition(pos);
    m_glow_polygon.setPosition(pos);
    m_shine_scroll_speed = 0x3333;
}

ARM void PokerCard::setVisible(bool enable)
{
    if (enable) {
        m_ctrl.flag_ |= 1;
    } else {
        m_ctrl.flag_ &= ~1;
    }
}
