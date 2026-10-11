#pragma once
#include <globaldefs.h>

enum {
    TYPE_SLIME,
    TYPE_CROWN,
    TYPE_SWORD,
    TYPE_SHIELD,
    TYPE_JOKER,
};
typedef int CARD_TYPE;

typedef struct {
    short defaultNo_;                           // 0x00
    short type_;                                // 0x02
    short no_;                                  // 0x04
} POKER_CARD;

struct PokerManager {
    int defaultNo_[5];                          // 0x00
    int chengeCard_[5];                         // 0x14
    int cardNo_[5];                             // 0x28
    CARD_TYPE cardType_[5];                     // 0x3C
    int cardPosition_;                          // 0x50
    int winningCombination_;                    // 0x54
    int combinationCard_[5];                    // 0x58 
    int betCoin_;                               // 0x6C
    int getCoin_;                               // 0x70
    int groundSlum_;                            // 0x74
    POKER_CARD gameCard_[5];                    // 0x78
    POKER_CARD changeCard_[5];                  // 0x96
    POKER_CARD debugCard_[5];                   // 0xB4

    static PokerManager* getSingleton();
    void allClear();
    void clearCard();
    void clearDebugCard(int index);
    void dealCard(int index);
    void setBetCoin(int bet, int haveCoin);
    int judgementCombination();
    int getMultiple();
    void resetCombinationCard();
    void startSelectCard(int count);
    void setSelectCard(int count, int active);
    int getSelectCard(int count);
    int getSelectCardNo(int count, int active);
    int getSelectCardType(int count, int active);
    int getSelectCardResult(int count);
    void startHighAndLow();
    void setAnswer(int count, int answer);
    int getHighAndLowResult(int count);
    void setGameCard(POKER_CARD* card, int index, int defaultNo);
    int changeCardNo(int cardNo);
    int changeCardType(int cardNo);
    int getCardNo(int index) { return gameCard_[index].no_; }
    int getCardType(int index) { return gameCard_[index].type_; }
    void setGetCoin(int get) { getCoin_ = get; }
};
