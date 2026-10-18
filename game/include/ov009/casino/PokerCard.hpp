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

    PokerCardPolygon unk_000;                   // 0x000
    PokerCardPolygon unk_088;                   // 0x088
    PokerCardPolygon unk_110;                   // 0x110
    PokerCardPolygon unk_198;                   // 0x198
    dss::Flag32 m_ctrl;                         // 0x220
    int m_effect_frame;                         // 0x224
    int unk_228;                                // 0x228
    int m_effect_enable;                        // 0x22C
    BillboardTexCoord unk_230;                  // 0x230

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
