#include "ov016/casino/FightStadiumManager.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Random.hpp"
#include "main/encount/Encount.hpp"
#include "main/global/Global.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"

FightCardData FightStadiumManager::cardDataList_[MAX_CARD_COUNT] = {
    { 1, 32, 9, 47, 1, 44, 5, -1, -1 },
    { 2, 26, 5, 26, 5, 26, 5, 26, 5 },
    { 3, 71, 10, 28, 1, 71, 10, -1, -1 },
    { 4, 7, 1, 38, 13, 38, 13, -1, -1 },
    { 5, 39, 18, 68, 1, 44, 18, 85, 13 },
    { 6, 28, 100, 65, 100, 65, 100, 28, 100 },
    { 7, 14, 5, 38, 3, 14, 5, -1, -1 },
    { 8, 0, 5, 1, 5, 4, 4, 182, 4 },
    { 9, 63, 5, 55, 3, 58, 3, -1, -1 },
    { 10, 2, 5, 3, 11, 5, 1, -1, -1 },
    { 11, 11, 5, 15, 3, 11, 5, -1, -1 },
    { 12, 8, 4, 9, 4, 20, 4, -1, -1 },
    { 13, 27, 3, 10, 5, 13, 5, -1, -1 },
    { 14, 34, 1, 19, 33, 19, 33, 19, 33 },
    { 15, 61, 5, 34, 3, 61, 5, -1, -1 },
    { 16, 47, 5, 7, 10, 85, 8, 42, 3 },
    { 17, 22, 4, 6, 4, 24, 4, -1, -1 },
    { 18, 74, 5, 17, 3, 74, 5, -1, -1 },
    { 19, 36, 3, 64, 4, 36, 3, -1, -1 },
    { 20, 38, 4, 184, 4, 38, 4, -1, -1 },
    { 21, 54, 6, 43, 4, 50, 5, 54, 6 },
    { 22, 6, 4, 110, 3, 74, 4, -1, -1 },
    { 23, 63, 11, 47, 5, 33, 5, 58, 8 },
    { 24, 183, 5, 185, 3, 10, 4, -1, -1 },
    { 25, 45, 5, 31, 5, 60, 5, 45, 5 },
    { 26, 54, 1, 37, 4, 56, 7, -1, -1 },
    { 27, 75, 3, 67, 10, 78, 3, -1, -1 },
    { 28, 51, 3, 75, 3, 62, 5, -1, -1 },
    { 29, 92, 3, 95, 7, 165, 3, -1, -1 },
    { 30, 67, 3, 67, 3, 67, 3, -1, -1 },
    { 31, 163, 4, 87, 3, 163, 4, -1, -1 },
    { 32, 69, 4, 77, 4, 83, 3, -1, -1 },
    { 33, 88, 1, 73, 9, 73, 9, 59, 7 },
    { 34, 163, 8, 94, 5, 84, 5, 84, 5 },
    { 35, 69, 7, 78, 4, 83, 1, -1, -1 },
    { 36, 100, 3, 100, 3, 111, 9, -1, -1 },
    { 37, 78, 5, 82, 3, 104, 5, -1, -1 },
    { 38, 117, 2, 115, 4, -1, -1, -1, -1 },
    { 39, 135, 2, 133, 4, -1, -1, -1, -1 },
    { 40, 68, 100, 140, 100, 82, 100, -1, -1 },
    { 41, 0, 200, 107, 20, 124, 2, 182, 200 },
    { 42, 113, 10, 124, 200, -1, -1, -1, -1 },
    { 43, 136, 130, 136, 130, -1, -1, -1, -1 },
    { 44, 99, 2, 96, 3, 166, 4, -1, -1 },
    { 45, 120, 5, 120, 5, 120, 5, -1, -1 },
    { 46, 116, 20, 114, 1, -1, -1, -1, -1 },
};

THUMB void FightStadiumManager::setup()
{
    dss::memset(monster_, -1, sizeof(monster_));
    dss::memset(diameter_, -1, sizeof(diameter_));
    encount_ = 0;
    int index = dssrand::rand(getCardCount());
    encount_ = cardDataList_[index].encount;
    monster_[0] = cardDataList_[index].mobA;
    monster_[1] = cardDataList_[index].mobB;
    monster_[2] = cardDataList_[index].mobC;
    monster_[3] = cardDataList_[index].mobD;
    diameter_[0] = cardDataList_[index].resultA * 10 + (dssrand::rand(9) + 1);
    diameter_[1] = cardDataList_[index].resultB * 10 + (dssrand::rand(9) + 1);
    diameter_[2] = cardDataList_[index].resultC * 10 + (dssrand::rand(9) + 1);
    diameter_[3] = cardDataList_[index].resultD * 10 + (dssrand::rand(9) + 1);
    cardCount_ = 0;
    while (cardCount_ < MAX_GROUP_NUM && monster_[cardCount_] != -1) {
        cardCount_++;
    }
    dss::memset(orderNumber_, -1, sizeof(orderNumber_));
    for (int i = 0; i < cardCount_; i++) {
        short count = 0;
        for (int j = 0; j < cardCount_; j++) {
            if (monster_[i] == monster_[j]) {
                count++;
                if (i > j) {
                    orderNumber_[i]++;
                }
            }
        }
        if (count > 1) {
            orderNumber_[i]++;
        }
    }
}

THUMB FightStadiumManager* FightStadiumManager::getSingleton()
{
    static FightStadiumManager fightStadiumManager;
    return &fightStadiumManager;
}

THUMB int FightStadiumManager::getCardCount()
{
    if ((short)status::g_Story.chapter_ < CHAPTER_FIVE) {
        return FIRST_CARD_COUNT;
    }
    status::g_Party.setPlayerMode();
    int level = 0;
    for (int i = 0; i < status::g_Party.getCount(); i++) {
        if (level < status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.level_) {
            level = status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.level_;
        }
    }
    if (level >= 35) {
        return LV35_CARD_COUNT;
    }
    if (level >= 30) {
        return LV30_CARD_COUNT;
    }
    if (level >= 25) {
        return LV25_CARD_COUNT;
    }
    if (level >= 15) {
        return LV15_CARD_COUNT;
    }
    return MAIN_CARD_COUNT;
}

THUMB int FightStadiumManager::getMonsterID(int index)
{
    return monster_[index];
}

THUMB int FightStadiumManager::getOrderCount(int index)
{
    return orderNumber_[index];
}

THUMB int FightStadiumManager::getDiameter(int index)
{
    return diameter_[index];
}

THUMB void FightStadiumManager::battleStart()
{
    for (int i = 0; i < MAX_GROUP_NUM; i++) {
        g_Stage.setBtlMapName("btlcc2");
        short monster = monster_[i];
        if (monster != -1) {
            encount::Encount::getSingleton()->monsterIndex_[i] = monster;
            encount::Encount::getSingleton()->monsterCount_[i] = 1;
        } else {
            encount::Encount::getSingleton()->monsterIndex_[i] = 0;
            encount::Encount::getSingleton()->monsterCount_[i] = 0;
        }
    }
    encount::Encount::getSingleton()->encountParam_.tileLevel_ = 99;
    g_Global.fightStadiumResult_ = 1;
    g_Global.fightStadiumFlag_ = 1;
    g_Global.startBattle();
}
