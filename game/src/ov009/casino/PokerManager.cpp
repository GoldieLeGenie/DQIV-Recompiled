#include "ov009/casino/PokerManager.hpp"
#include "ov009/casino/PokerJudgement.hpp"
#include "ov009/casino/PokerDoubleupSelectCard.hpp"
#include "ov009/casino/PokerDoubleupHighAndLow.hpp"
#include "main/dss/Random.hpp"

ARM PokerManager* PokerManager::getSingleton()
{
    static PokerManager pokerManager;
    return &pokerManager;
}

ARM void PokerManager::allClear()
{
    for (int i = 0; i < 5; i++) {
        defaultNo_[i] = 0;
        chengeCard_[i] = 0;
        cardNo_[i] = 0;
        cardType_[i] = TYPE_SLIME;
        combinationCard_[i] = 0;
    }
    betCoin_ = 0;
    cardPosition_ = 0;
    winningCombination_ = 0;
    groundSlum_ = 0;
    clearCard();
    clearDebugCard(-1);
}

ARM void PokerManager::clearCard()
{
    for (int i = 0; i < 5; i++) {
        gameCard_[i].defaultNo_ = -1;
        gameCard_[i].type_ = -1;
        gameCard_[i].no_ = -1;
        changeCard_[i].defaultNo_ = -1;
        changeCard_[i].type_ = -1;
        changeCard_[i].no_ = -1;
    }
}

ARM void PokerManager::clearDebugCard(int index)
{
    if (index == -1) {
        for (int i = 0; i < 5; i++) {
            debugCard_[i].defaultNo_ = -1;
            debugCard_[i].type_ = -1;
            debugCard_[i].no_ = -1;
        }
    } else {
        debugCard_[index].defaultNo_ = -1;
        debugCard_[index].type_ = -1;
        debugCard_[index].no_ = -1;
    }
}

ARM void PokerManager::dealCard(int index)
{
    short count = 0;
    if (index == -1) {
        for (int i = 0; i < 5; i++) {
            chengeCard_[i] = -1;
        }
        for (int i = 0; i < 5; i++) {
            if (debugCard_[i].defaultNo_ != -1) {
                setGameCard(gameCard_, i, debugCard_[i].defaultNo_);
            } else {
                short changeCard = dssrand::rand(53);
                combinationCard_[i] = 0;
                while (count < i) {
                    if (changeCard == gameCard_[count].defaultNo_) {
                        changeCard = dssrand::rand(53);
                        count = 0;
                    } else {
                        count++;
                    }
                }
                setGameCard(gameCard_, i, changeCard);
                count = 0;
            }
        }
        clearDebugCard(-1);
    } else if (debugCard_[index].defaultNo_ != -1) {
        changeCard_[index].defaultNo_ = gameCard_[index].defaultNo_;
        setGameCard(gameCard_, index, debugCard_[index].defaultNo_);
    } else {
        short changeCard = dssrand::rand(53);
        while (count < 5) {
            if (changeCard == gameCard_[count].defaultNo_) {
                changeCard = dssrand::rand(53);
                count = 0;
            } else if (changeCard_[count].defaultNo_ != -1) {
                if (changeCard == changeCard_[count].defaultNo_) {
                    changeCard = dssrand::rand(53);
                    count = 0;
                } else {
                    count++;
                }
            } else {
                count++;
            }
        }
        changeCard_[index].defaultNo_ = gameCard_[index].defaultNo_;
        setGameCard(gameCard_, index, changeCard);
    }
}

ARM void PokerManager::setBetCoin(int bet, int haveCoin)
{
    if (bet > haveCoin) {
        betCoin_ = haveCoin;
    } else {
        betCoin_ = bet;
    }
}

ARM int PokerManager::judgementCombination()
{
    winningCombination_ = PokerJudgement::getSingleton()->JudgeCombination();
    if (winningCombination_ == PokerJudgement::NO_PAIRS || winningCombination_ == PokerJudgement::ONE_PAIRS) {
        return PokerJudgement::NO_PAIRS;
    }
    return winningCombination_;
}

ARM int PokerManager::getMultiple()
{
    static int diameter[] = {1, 2, 4, 5, 8, 10, 20, 100, 250, 500};
    if (judgementCombination() == PokerJudgement::NO_PAIRS) {
        return 0;
    }
    return diameter[judgementCombination() - PokerJudgement::TWO_PAIRS];
}

ARM void PokerManager::resetCombinationCard()
{
    for (int i = 0; i < 5; i++) {
        combinationCard_[i] = 0;
    }
}

ARM void PokerManager::startSelectCard(int count)
{
    PokerDoubleupSelectCard::getSingleton()->startDoubleUp(count);
}

ARM void PokerManager::setSelectCard(int count, int active)
{
    PokerDoubleupSelectCard::getSingleton()->selectDoubleup_[count].selectActive_ = active;
}

ARM int PokerManager::getSelectCard(int count)
{
    return PokerDoubleupSelectCard::getSingleton()->selectDoubleup_[count].selectActive_;
}

ARM int PokerManager::getSelectCardNo(int count, int active)
{
    if (active == 0) {
        return changeCardNo(PokerDoubleupSelectCard::getSingleton()->selectDoubleup_[count].targetCard_);
    }
    return changeCardNo(PokerDoubleupSelectCard::getSingleton()->getSelectCard(count, active));
}

ARM int PokerManager::getSelectCardType(int count, int active)
{
    if (active == 0) {
        return changeCardType(PokerDoubleupSelectCard::getSingleton()->selectDoubleup_[count].targetCard_);
    }
    return changeCardType(PokerDoubleupSelectCard::getSingleton()->getSelectCard(count, active));
}

ARM int PokerManager::getSelectCardResult(int count)
{
    return PokerDoubleupSelectCard::getSingleton()->getResult(count);
}

ARM void PokerManager::startHighAndLow()
{
    PokerDoubleupHighAndLow::getSingleton()->startHighAndLow();
}

ARM void PokerManager::setAnswer(int count, int answer)
{
    PokerDoubleupHighAndLow::getSingleton()->answer_[count] = answer;
}

ARM int PokerManager::getHighAndLowResult(int count)
{
    return PokerDoubleupHighAndLow::getSingleton()->getResult(count);
}

ARM void PokerManager::setGameCard(POKER_CARD* card, int index, int defaultNo)
{
    card[index].defaultNo_ = defaultNo;
    card[index].type_ = defaultNo / 13;
    if (defaultNo == 52) {
        card[index].no_ = -1;
    } else {
        card[index].no_ = defaultNo % 13;
    }
}

ARM int PokerManager::changeCardNo(int cardNo)
{
    if (cardNo == 52) {
        return -1;
    }
    return cardNo % 13;
}

ARM int PokerManager::changeCardType(int cardNo)
{
    return cardNo / 13;
}
