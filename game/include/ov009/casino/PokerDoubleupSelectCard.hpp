#pragma once
#include <globaldefs.h>

struct PokerDoubleupSelectCard {
    static const int SELECT_CARD_COUNT = 4;
    static const int DOUBLEUP_GAME_MAX = 12;
    static const int TRUMP_CARD_MAX = 53;

    enum {
        TYPE_NONE,
        TYPE_SLIME,
        TYPE_CROWN,
        TYPE_SWORD,
        TYPE_SHIELD,
        TYPE_JOKER,
    };

    typedef enum {
        WIN,
        LOSE,
        DRAW,
    } DOUBLEUP_RESULT;

    typedef struct {
        short targetCard_;                      // 0x00
        short selectCard_[SELECT_CARD_COUNT];   // 0x02
        short selectActive_;                    // 0x0A
    } SELECT_DOUBLEUP;

    typedef struct {
        short targetCard_;                      // 0x00
        short targetCardType_;                  // 0x02
        short selectCard_[SELECT_CARD_COUNT];   // 0x04
        short selectCardType_[SELECT_CARD_COUNT]; // 0x0C
    } DEBUG_CARD;

    SELECT_DOUBLEUP selectDoubleup_[DOUBLEUP_GAME_MAX]; // 0x00
    DEBUG_CARD debugCard_;                      // 0x90

    static PokerDoubleupSelectCard* getSingleton();
    void startDoubleUp(int no);
    DOUBLEUP_RESULT getResult(int no);
    int getCardNo(int no, int index);
    void clearDebugCard();
    short getSelectCard(int count, int active) { return selectDoubleup_[count].selectCard_[active - 1]; }
};
