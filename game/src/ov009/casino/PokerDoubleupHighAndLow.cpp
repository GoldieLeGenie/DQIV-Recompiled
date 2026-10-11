#include "ov009/casino/PokerDoubleupHighAndLow.hpp"
#include "main/dss/Random.hpp"

ARM PokerDoubleupHighAndLow* PokerDoubleupHighAndLow::getSingleton()
{
    static PokerDoubleupHighAndLow pokerDoubleupHighAndLow;
    return &pokerDoubleupHighAndLow;
}

ARM void PokerDoubleupHighAndLow::startHighAndLow()
{
    for (int i = 0; i < DOUBLEUP_GAME_MAX; i++) {
        highAndLow_[i] = dssrand::rand(TRUMP_CARD_MAX);
        if (i != 0) {
            int count = 0;
            while (count < i) {
                if (highAndLow_[i] == highAndLow_[count]) {
                    highAndLow_[i] = dssrand::rand(TRUMP_CARD_MAX);
                    count = 0;
                } else {
                    count++;
                }
            }
        }
    }
}

ARM int PokerDoubleupHighAndLow::getResult(int count)
{
    int baseCard = getCardNo(count);
    int compareCard = getCardNo(count + 1);
    if (baseCard > compareCard && answer_[count] == 1) {
        return WIN;
    }
    if (baseCard < compareCard && answer_[count] == 0) {
        return WIN;
    }
    if (baseCard == compareCard) {
        return DRAW;
    }
    return LOSE;
}

ARM int PokerDoubleupHighAndLow::getCardNo(int count)
{
    int card = highAndLow_[count];
    if (card == TRUMP_CARD_MAX - 1) {
        return 14;
    }
    int number = card % 13;
    if (number == 0) {
        number = 13;
    }
    return number;
}
