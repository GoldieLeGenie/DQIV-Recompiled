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
    void unkfunc_021249cc();
    void unkfunc_021249dc(dss::Fix32 scale);
    void unkfunc_02124a70(const PolygonVertex* vertex);
    void setPosition(dss::Fix32Vector3& pos);
    void setAngle(int angle);
    void unkfunc_02124b24(const BillboardTexCoord* texCoord);
    void unkfunc_02124b30(const BillboardTexCoord* texCoord);
    void unkfunc_02124b40(const BillboardTexCoord* texCoord);
    void unkfunc_02124bcc(const BillboardTexCoord* texCoord);
    void unkfunc_02124bdc(void* texture);
    void unkfunc_02124be4(void* texture);
    void unkfunc_02124bec(void* texture);
    void unkfunc_02124bf4(void* texture);
    void draw();
    void setEffect();
    void unkfunc_02124e14(bool enable);
};
