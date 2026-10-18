#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"
#include "main/object/DisplayCharacter.hpp"

struct PokerCardPolygon : PolygonObject {
    virtual void draw();

    unsigned short angle_;                      // 0x84

    PokerCardPolygon();
    ~PokerCardPolygon();
};

struct PokerCard {
    static const int MAX_FRAME = 30;

    PokerCardPolygon m_front_polygon;                   // 0x000
    PokerCardPolygon m_back_polygon;                   // 0x088
    PokerCardPolygon m_shine_polygon;                   // 0x110
    PokerCardPolygon m_glow_polygon;                   // 0x198
    dss::Flag32 m_ctrl;                         // 0x220
    int m_effect_frame;                         // 0x224
    int m_shine_scroll_speed;                                // 0x228
    int m_effect_enable;                        // 0x22C
    BillboardTexCoord m_shine_tex_coord;                  // 0x230

    PokerCard();
    ~PokerCard();
    void initialize();
    void setScale(dss::Fix32 scale);
    void setVertex(const PolygonVertex* vertex);
    void setPosition(dss::Fix32Vector3& pos);
    void setAngle(int angle);
    void setFrontTexCoord(const BillboardTexCoord* texCoord);
    void setBackTexCoord(const BillboardTexCoord* texCoord);
    void setShineTexCoord(const BillboardTexCoord* texCoord);
    void setGlowTexCoord(const BillboardTexCoord* texCoord);
    void setFrontTexture(void* texture);
    void setBackTexture(void* texture);
    void setShineTexture(void* texture);
    void setGlowTexture(void* texture);
    void draw();
    void setEffect();
    void setVisible(bool enable);
};
