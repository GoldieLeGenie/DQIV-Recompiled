#include "ov009/casino/PokerDoubleupSelectCard.hpp"
#include "main/dss/Random.hpp"

ARM PokerDoubleupSelectCard* PokerDoubleupSelectCard::getSingleton()
{
    static PokerDoubleupSelectCard pokerDoubleupSelectCard;
    return &pokerDoubleupSelectCard;
}

ARM void PokerDoubleupSelectCard::startDoubleUp(int no)
{
    if (debugCard_.targetCardType_ != TYPE_NONE) {
        selectDoubleup_[no].targetCard_ = debugCard_.targetCard_;
    } else {
        selectDoubleup_[no].targetCard_ = dssrand::rand(TRUMP_CARD_MAX);
    }
    for (int i = 0; i < SELECT_CARD_COUNT; i++) {
        int checkCard;
        int loopCount = 0;
        if (debugCard_.selectCardType_[i] != TYPE_NONE) {
            checkCard = debugCard_.selectCard_[i];
        } else {
            checkCard = dssrand::rand(TRUMP_CARD_MAX);
            while (loopCount < i) {
                if (checkCard == selectDoubleup_[no].targetCard_ || checkCard == selectDoubleup_[no].selectCard_[loopCount]) {
                    checkCard = dssrand::rand(TRUMP_CARD_MAX);
                    loopCount = 0;
                } else {
                    loopCount++;
                }
            }
            while (i == 0 && checkCard == selectDoubleup_[no].targetCard_) {
                checkCard = dssrand::rand(TRUMP_CARD_MAX);
            }
        }
        selectDoubleup_[no].selectCard_[i] = checkCard;
    }
    clearDebugCard();
}

ARM PokerDoubleupSelectCard::DOUBLEUP_RESULT PokerDoubleupSelectCard::getResult(int no)
{
    int target = getCardNo(no, -1);
    int select = getCardNo(no, selectDoubleup_[no].selectActive_);
    if (target == -1) {
        return LOSE;
    }
    if (select == -1) {
        return WIN;
    }
    if (target == select) {
        return DRAW;
    }
    return target > select ? LOSE : WIN;
}

ARM int PokerDoubleupSelectCard::getCardNo(int no, int index)
{
    if (index == -1) {
        int card = selectDoubleup_[no].targetCard_;
        if (card == TRUMP_CARD_MAX - 1) {
            return -1;
        }
        int number = card % 13;
        if (number == 0) {
            number = 13;
        }
        return number;
    } else {
        int card = selectDoubleup_[no].selectCard_[index];
        if (card == TRUMP_CARD_MAX - 1) {
            return -1;
        }
        int number = card % 13;
        if (number == 0) {
            number = 13;
        }
        return number;
    }
}

ARM void PokerDoubleupSelectCard::clearDebugCard()
{
    debugCard_.targetCard_ = -1;
    debugCard_.targetCardType_ = TYPE_NONE;
    for (int i = 0; i < SELECT_CARD_COUNT; i++) {
        debugCard_.selectCard_[i] = -1;
        debugCard_.selectCardType_[i] = TYPE_NONE;
    }
}
