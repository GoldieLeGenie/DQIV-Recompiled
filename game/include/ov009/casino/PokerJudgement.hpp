#pragma once
#include <globaldefs.h>

struct PokerJudgement {
    enum {
        NO_PAIRS,
        ONE_PAIRS,
        TWO_PAIRS,
        THREE_CARD,
        STRAIGHT,
        FLASH,
        FULL_HOUSE,
        FOUR_CARD,
        STRAIGHT_FLASH,
        FIVE_CARD,
        ROYAL_STRAIGHT_FLASH,
        ROYAL_STRAIGHT_SLIME,
    };

    int sortCard_[5];                           // 0x00

    static PokerJudgement* getSingleton();
    int JudgeCombination();
    void sortCard();
    bool judgeFlash();
    int judgeStraight();
    int judgePairs(int* threeCard, int* pairsCount);
    void setWinningPosition();
    void setWinningPosition(int* numberCount);
};
