#include "ov009/casino/PokerJudgement.hpp"
#include "ov009/casino/PokerManager.hpp"

ARM PokerJudgement* PokerJudgement::getSingleton()
{
    static PokerJudgement pokerJugement;
    return &pokerJugement;
}

ARM int PokerJudgement::JudgeCombination()
{
    int threecardFlag = 0;
    int pairsCount = 0;
    PokerManager::getSingleton()->resetCombinationCard();
    sortCard();
    bool flashFlag = judgeFlash();
    int straightFlag = judgeStraight();
    if (flashFlag == true && straightFlag != NO_PAIRS) {
        if (straightFlag == ROYAL_STRAIGHT_FLASH) {
            if (sortCard_[0] == 0 && PokerManager::getSingleton()->getCardType(0) == TYPE_SLIME) {
                return ROYAL_STRAIGHT_SLIME;
            }
            return ROYAL_STRAIGHT_FLASH;
        }
        return STRAIGHT_FLASH;
    }
    if (flashFlag) {
        return FLASH;
    }
    if (straightFlag != NO_PAIRS) {
        return STRAIGHT;
    }
    int combination = judgePairs(&threecardFlag, &pairsCount);
    if (combination != NO_PAIRS) {
        return combination;
    }
    if (threecardFlag == 1 && pairsCount == 1) {
        return FULL_HOUSE;
    }
    if (threecardFlag == 1) {
        if (sortCard_[0] == -1) {
            return FOUR_CARD;
        }
        return THREE_CARD;
    }
    if (pairsCount == 2) {
        if (sortCard_[0] == -1) {
            return FULL_HOUSE;
        }
        return TWO_PAIRS;
    }
    if (pairsCount == 1) {
        if (sortCard_[0] == -1) {
            return THREE_CARD;
        }
        PokerManager::getSingleton()->resetCombinationCard();
        return NO_PAIRS;
    }
    PokerManager::getSingleton()->resetCombinationCard();
    return NO_PAIRS;
}

ARM void PokerJudgement::sortCard()
{
    for (int i = 0; i < 5; i++) {
        sortCard_[i] = PokerManager::getSingleton()->getCardNo(i);
    }
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 5; j++) {
            if (sortCard_[i] > sortCard_[j]) {
                int a = sortCard_[i];
                sortCard_[i] = sortCard_[j];
                sortCard_[j] = a;
            }
        }
    }
}

ARM bool PokerJudgement::judgeFlash()
{
    int deal;
    if (sortCard_[0] == -1) {
        int count = 0;
        while (PokerManager::getSingleton()->getCardType(count) == TYPE_JOKER) {
            count++;
        }
        deal = PokerManager::getSingleton()->getCardType(count);
    } else {
        deal = PokerManager::getSingleton()->getCardType(0);
    }
    for (int i = 0; i < 5; i++) {
        if (PokerManager::getSingleton()->getCardType(i) != TYPE_JOKER) {
            if (deal != PokerManager::getSingleton()->getCardType(i)) {
                return false;
            }
        }
    }
    setWinningPosition();
    return true;
}

ARM int PokerJudgement::judgeStraight()
{
    bool joker = false;
    int first = 0;
    if (sortCard_[0] == -1) {
        joker = true;
        first = 1;
    }
    if (sortCard_[first] == 0 && sortCard_[4] == 12) {
        if (sortCard_[2] == 9 || sortCard_[2] == 10) {
            for (int i = 4; i > first + 1; i--) {
                if (sortCard_[i] - 1 != sortCard_[i - 1]) {
                    if (joker) {
                        joker = false;
                    } else {
                        return NO_PAIRS;
                    }
                }
            }
            setWinningPosition();
            return ROYAL_STRAIGHT_FLASH;
        }
        return NO_PAIRS;
    }
    if (joker == true && sortCard_[1] == 0 && sortCard_[4] == 11) {
        for (int i = 2; i < 4; i++) {
            if (sortCard_[i] + 1 != sortCard_[i + 1]) {
                return NO_PAIRS;
            }
        }
        return ROYAL_STRAIGHT_FLASH;
    }
    for (int i = first; i < 4; i++) {
        if (sortCard_[i] + 1 != sortCard_[i + 1]) {
            if (joker) {
                joker = false;
                if (i == 3 && sortCard_[i] + 2 == sortCard_[i + 1]) {
                    return STRAIGHT;
                }
                if (i == 3) {
                    return NO_PAIRS;
                }
                if (sortCard_[i] == sortCard_[i + 1] || sortCard_[i] + 2 != sortCard_[i + 1]) {
                    return NO_PAIRS;
                }
            } else {
                return NO_PAIRS;
            }
        }
    }
    setWinningPosition();
    if (sortCard_[1] == 9 && joker == true) {
        return ROYAL_STRAIGHT_FLASH;
    }
    return STRAIGHT;
}

ARM int PokerJudgement::judgePairs(int* threeCard, int* pairsCount)
{
    int numberCount[13] = {0};
    for (int i = 0; i < 5; i++) {
        if (sortCard_[i] != -1) {
            numberCount[sortCard_[i]]++;
        }
    }
    for (int i = 0; i < 13; i++) {
        if (numberCount[i] == 4) {
            setWinningPosition(numberCount);
            if (sortCard_[0] == -1) {
                return FIVE_CARD;
            }
            return FOUR_CARD;
        }
        if (numberCount[i] == 3) {
            *threeCard = 1;
        }
        if (numberCount[i] == 2) {
            (*pairsCount)++;
        }
    }
    setWinningPosition(numberCount);
    return NO_PAIRS;
}

ARM void PokerJudgement::setWinningPosition()
{
    for (int i = 0; i < 5; i++) {
        PokerManager::getSingleton()->combinationCard_[i] = 1;
    }
}

ARM void PokerJudgement::setWinningPosition(int* numberCount)
{
    for (int i = 0; i < 13; i++) {
        if (numberCount[i] > 1) {
            for (int j = 0; j < 5; j++) {
                if (i == PokerManager::getSingleton()->getCardNo(j)) {
                    PokerManager::getSingleton()->combinationCard_[j] = 1;
                }
            }
        }
    }
    if (sortCard_[0] == -1) {
        for (int i = 0; i < 5; i++) {
            if (PokerManager::getSingleton()->getCardNo(i) == -1) {
                PokerManager::getSingleton()->combinationCard_[i] = 1;
            }
        }
    }
}
