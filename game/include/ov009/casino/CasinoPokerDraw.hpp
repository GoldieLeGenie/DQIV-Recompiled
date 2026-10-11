#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"
#include "main/data/DataObject.hpp"
#include "ov009/casino/PokerCard.hpp"

struct CasinoPokerDraw {
    static const int POKER_CARD_MAX = 5;

    dss::Fix32 m_card_distance;                 // 0x00
    dss::Fix32 m_card_depth;                    // 0x04
    dss::Fix32 m_card_space;                    // 0x08
    dss::Fix32 m_card_scale;                    // 0x0C
    dss::Fix32Vector3 m_default_pos[5];         // 0x10
    DataObject unk_4c[6];                       // 0x4C
    void* unk_ac[6];                            // 0xAC
    PokerCard m_card[5];                        // 0xC4

    CasinoPokerDraw();
    ~CasinoPokerDraw();
    static CasinoPokerDraw* getSingleton();     
    void initialize();
    void setPoolPosition();
    void terminate();
    void draw();
    void setCardJoker(int index);
    void setCardReverse(int index);
    void setCardTexture(int index, int type, int number);
    void unkfunc_02121804();
    void unkfunc_0212198c();
    void setCardPosition(int index, dss::Fix32Vector3 pos);
    dss::Fix32 getDistance();
    dss::Fix32 getSpace();
    dss::Fix32 getDepth();
    void setCardAngle(int index, int angle);
    void setEffect(int index);
};
