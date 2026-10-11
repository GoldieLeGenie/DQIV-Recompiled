#pragma ipa file
#include "ov009/casino/CasinoPokerDraw.hpp"
#include "main/object/DSSAObject.hpp"
#include "main/cmn/CommonEffectData.hpp"
#include "nitro/g3.hpp"
#include "nitro/fx/fx_trig.h"

static PolygonVertex s_vertex;
static BillboardTexCoord s_texCoord;
static dss::Vector3<dss::Fix16> s_vertexLeftTop(-0.21f, -0.0f, 0.0f);
static dss::Vector3<dss::Fix16> s_vertexRightTop(0.21f, -0.0f, 0.0f);
static dss::Vector3<dss::Fix16> s_vertexRightBottom(0.21f, -0.0f, -0.64f);
static dss::Vector3<dss::Fix16> s_vertexLeftBottom(-0.21f, -0.0f, -0.64f);
static dss::Vector2<dss::Fix32> s_texCoordLeftTop(0, 0);
static dss::Vector2<dss::Fix32> s_texCoordRightTop(42, 0);
static dss::Vector2<dss::Fix32> s_texCoordRightBottom(42, 64);
static dss::Vector2<dss::Fix32> s_texCoordLeftBottom(0, 64);

ARM CasinoPokerDraw::CasinoPokerDraw()
{
}

ARM CasinoPokerDraw::~CasinoPokerDraw()
{
}

ARM CasinoPokerDraw* CasinoPokerDraw::getSingleton()
{
    static CasinoPokerDraw m_singleton;
    return &m_singleton;
}

ARM void CasinoPokerDraw::initialize()
{
    unk_4c[0].setup("data/minigame/poker/slime1.tex", 0, 0);
    unk_4c[1].setup("data/minigame/poker/crown1.tex", 0, 0);
    unk_4c[2].setup("data/minigame/poker/tate1.tex", 0, 0);
    unk_4c[3].setup("data/minigame/poker/ken1.tex", 0, 0);
    unk_4c[4].setup("data/minigame/poker/joker_omote.tex", 0, 0);
    unk_4c[5].setup("data/minigame/poker/hikari.tex", 0, 0);
    for (int i = 0; i < 6; i++) {
        unk_ac[i] = unk_4c[i].getAddr();
        ((TextureObject*)unk_ac[i])->unkfunc_02086798(1);
    }
    s_vertex.v[0] = s_vertexLeftTop;
    s_vertex.v[1] = s_vertexRightTop;
    s_vertex.v[2] = s_vertexRightBottom;
    s_vertex.v[3] = s_vertexLeftBottom;
    s_texCoord.v[0] = s_texCoordLeftBottom;
    s_texCoord.v[1] = s_texCoordRightBottom;
    s_texCoord.v[2] = s_texCoordRightTop;
    s_texCoord.v[3] = s_texCoordLeftTop;
    m_card_distance.value = 0x3744;
    m_card_space.value = 0xe64;
    m_card_scale.value = 0x4000;
    m_card_depth.value = -0x2640;
    setPoolPosition();
    unkfunc_02121804();
    unkfunc_0212198c();
    for (int i = 0; i < 5; i++) {
        m_card[i].setAngle(0x8000);
    }
}

ARM void CasinoPokerDraw::setPoolPosition()
{
    for (int i = 0; i < 5; i++) {
        dss::Fix32Vector3 pos;
        pos.vy = m_card_distance;
        pos.vx.value = m_card_space.value * (i * 2 - 4);
        pos.vz.value = m_card_depth.value;
        m_default_pos[i] = pos;
        m_card[i].unkfunc_021249dc(m_card_scale.value);
        m_card[i].unkfunc_02124a70(&s_vertex);
        m_card[i].setPosition(pos);
        m_card[i].unkfunc_02124e14(true);
        setCardReverse(i);
    }
}

ARM void CasinoPokerDraw::terminate()
{
    for (int i = 0; i < 6; i++) {
        ((TextureObject*)unk_ac[i])->unkfunc_02086868();
        unk_4c[i].cleanup();
    }
}

ARM void CasinoPokerDraw::draw()
{
    for (int i = 0; i < 5; i++) {
        m_card[i].draw();
    }
}

ARM void CasinoPokerDraw::setCardJoker(int index)
{
    setCardTexture(index, 4, 0);
}

ARM void CasinoPokerDraw::setCardReverse(int index)
{
    setCardTexture(index, 4, 1);
}

ARM void CasinoPokerDraw::setCardTexture(int index, int type, int number)
{
    int x = number % 6;
    int y = number / 6;
    dss::Vector2<dss::Fix32> leftTop(x * 42, y * 64);
    dss::Vector2<dss::Fix32> rightTop((x + 1) * 42, y * 64);
    dss::Vector2<dss::Fix32> rightBottom((x + 1) * 42, (y + 1) * 64);
    dss::Vector2<dss::Fix32> leftBottom(x * 42, (y + 1) * 64);
    m_card[index].unkfunc_02124bdc(unk_4c[type].getAddr());
    s_texCoord.v[0] = leftBottom;
    s_texCoord.v[1] = rightBottom;
    s_texCoord.v[2] = rightTop;
    s_texCoord.v[3] = leftTop;
    m_card[index].unkfunc_02124b24(&s_texCoord);
}

ARM void CasinoPokerDraw::unkfunc_02121804()
{
    for (int i = 0; i < 5; i++) {
        dss::Vector2<dss::Fix32> leftTop(42, 0);
        dss::Vector2<dss::Fix32> rightTop(84, 0);
        dss::Vector2<dss::Fix32> rightBottom(84, 64);
        dss::Vector2<dss::Fix32> leftBottom(42, 64);
        m_card[i].unkfunc_02124be4(unk_4c[4].getAddr());
        s_texCoord.v[0] = leftBottom;
        s_texCoord.v[1] = rightBottom;
        s_texCoord.v[2] = rightTop;
        s_texCoord.v[3] = leftTop;
        m_card[i].unkfunc_02124b30(&s_texCoord);
    }
}

ARM void CasinoPokerDraw::unkfunc_0212198c()
{
    for (int i = 0; i < 5; i++) {
        dss::Vector2<dss::Fix32> leftTop(0, 0);
        dss::Vector2<dss::Fix32> rightTop(42, 0);
        dss::Vector2<dss::Fix32> rightBottom(42, 64);
        dss::Vector2<dss::Fix32> leftBottom(0, 64);
        m_card[i].unkfunc_02124bec(unk_4c[5].getAddr());
        s_texCoord.v[0] = leftBottom;
        s_texCoord.v[1] = rightBottom;
        s_texCoord.v[2] = rightTop;
        s_texCoord.v[3] = leftTop;
        m_card[i].unkfunc_02124b40(&s_texCoord);
    }
    for (int i = 0; i < 5; i++) {
        dss::Vector2<dss::Fix32> leftTop(42, 0);
        dss::Vector2<dss::Fix32> rightTop(84, 0);
        dss::Vector2<dss::Fix32> rightBottom(84, 64);
        dss::Vector2<dss::Fix32> leftBottom(42, 64);
        m_card[i].unkfunc_02124bf4(unk_4c[5].getAddr());
        s_texCoord.v[0] = leftBottom;
        s_texCoord.v[1] = rightBottom;
        s_texCoord.v[2] = rightTop;
        s_texCoord.v[3] = leftTop;
        m_card[i].unkfunc_02124bcc(&s_texCoord);
    }
}

ARM void CasinoPokerDraw::setCardPosition(int index, dss::Fix32Vector3 pos)
{
    m_card[index].setPosition(pos);
}

ARM dss::Fix32 CasinoPokerDraw::getDistance()
{
    return m_card_distance;
}

ARM dss::Fix32 CasinoPokerDraw::getSpace()
{
    return m_card_space;
}

ARM dss::Fix32 CasinoPokerDraw::getDepth()
{
    return m_card_depth;
}

ARM void CasinoPokerDraw::setCardAngle(int index, int angle)
{
    m_card[index].setAngle(angle);
}

ARM void CasinoPokerDraw::setEffect(int index)
{
    m_card[index].setEffect();
}

ARM PokerCardPolygon::PokerCardPolygon()
{
    angle_ = 0;
}

ARM PokerCardPolygon::~PokerCardPolygon()
{
}

ARM void PokerCardPolygon::draw()
{
    if (!enable_) {
        return;
    }
    func_0206adcc();
    func_0206dcf0();
    G3_PushMtx();
    G3_Translate(position_.vx.value, position_.vy.value, position_.vz.value);
    func_02065c9c(FX_SinIdx(angle_), FX_CosIdx(angle_));
    G3_Scale(scale_.vx.value, scale_.vy.value, scale_.vz.value);
    G3_MaterialColorDiffAmb(0x7fff, 0x4210, 1);
    G3_MaterialColorSpecEmi(0x4210, 0, 0);
    if (texture_) {
        ((TextureObject*)texture_)->unkfunc_02086abc();
        ((TextureObject*)texture_)->unkfunc_02086b3c();
    } else {
        unkfunc_02086b28();
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

// The original TU also emitted RenderObject3D's weak vtable (merged with main's by the linker).
// Our headers keep it external, so this dead-stripped stand-in recreates that 0x3C vtable for the data layout.
struct CasinoPokerDrawWeakVtable : RenderObject, Position {
};

void CasinoPokerDraw_weakVtable()
{
    CasinoPokerDrawWeakVtable unused;
}

#include "main/dss/DssVectorDefault.hpp"
