#pragma once
#include <globaldefs.h>

struct PokerDoubleupHighAndLow {
    static const int DOUBLEUP_GAME_MAX = 12;
    static const int TRUMP_CARD_MAX = 53;

    typedef enum {
        WIN,
        LOSE,
        DRAW,
    } DOUBLEUP_RESULT;

    int highAndLow_[13];                        // 0x00
    int answer_[DOUBLEUP_GAME_MAX];             // 0x34 

    static PokerDoubleupHighAndLow* getSingleton();
    void startHighAndLow();
    int getResult(int count);
    int getCardNo(int count);
};
