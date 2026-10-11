#include "main/status/BattleHistory.hpp"
#include "main/status/GameStatus.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/BattleResult.hpp"
#include "main/status/BasePartyStatus.hpp"

status::BattleHistory status::g_BattleHistory;

THUMB status::BattleHistory::BattleHistory(){
    return;
}


THUMB status::BattleHistory::~BattleHistory(){
    return;
}

THUMB void status::BattleHistory::initialize()
{
    int i;

    for (i = 0; i < 3; i++) {
        this->adventureTime_[i] = 0;
        this->battleCount_[i] = 0;
        this->monsterCount_[i] = 0;
        this->totalGold_[i] = 0;
        this->victoryGold_[i] = 0;
        this->wipeoutCount_[i] = 0;
        this->escapeCount_[i] = 0;
        this->maxDamage_[i] = 0;
        this->heroLevel_[i] = 0;
        this->title_[i] = 0;
        this->restMonster_[i] = 0;
        i + 1;
    }
    this->chapterBattleCount_ = 0;
    this->chapterWipeoutCount_ = 0;
    this->chapterEscapeCount_ = 0;
    return;
}


THUMB void status::BattleHistory::setAdventureTime(unsigned int time)
{
    this->adventureTime_[this->historyType_] = time;
    return;
}


THUMB void status::BattleHistory::regenesisAdventureTime()
{
    unsigned int playTime = g_Game.getPlayTime();

    this->adventureTime_[this->historyType_] = playTime;

    unsigned int result = BasePartyStatus::getClampValue(
        this->adventureTime_[this->historyType_],
        0,
        0x0CDFD7F0
    );

    this->adventureTime_[this->historyType_] = result;
    return;
}


THUMB unsigned int status::BattleHistory::getAdventureTime() {
    return this->adventureTime_[this->historyType_];
}

THUMB int status::BattleHistory::getBattleCount()

{
    return this->battleCount_[this->historyType_];
}


THUMB void status::BattleHistory::regenesisMonsterCount()
{
    if (this->historyType_ > RightNow) {
        this->monsterCount_[this->historyType_] = this->monsterCount_[RightNow];
        return;
    }
    this->monsterCount_[this->historyType_]++;
    this->monsterCount_[this->historyType_] =
        BasePartyStatus::getClampValue(this->monsterCount_[this->historyType_], 0, 9999999);

    return;
}



THUMB int status::BattleHistory::getMonsterCount()
{
    return this->monsterCount_[this->historyType_];
}


THUMB void status::BattleHistory::regenesisTotalGold(unsigned int gold)
{

    if (this->historyType_ > RightNow) {
        this->totalGold_[this->historyType_] = this->totalGold_[RightNow];
        return;
    }

    this->totalGold_[this->historyType_] = this->totalGold_[historyType_] + gold;
    this->totalGold_[this->historyType_] = BasePartyStatus::getClampValue(this->totalGold_[this->historyType_], 0, 999999999);
    return;
}


THUMB int status::BattleHistory::getTotalGold()
{
    return this->totalGold_[this->historyType_];
}

THUMB void status::BattleHistory::regenesisVictoryCount()
{
    if (this->historyType_ > RightNow) {
        this->victoryGold_[this->historyType_] = this->victoryGold_[RightNow];
        return;
    }
    this->victoryGold_[this->historyType_]++;
    this->victoryGold_[this->historyType_] = BasePartyStatus::getClampValue(this->victoryGold_[this->historyType_], 0, 99999);
    return;
}

THUMB int status::BattleHistory::getVictoryCount()

{
    return this->victoryGold_[this->historyType_];
}



THUMB void status::BattleHistory::setWipeoutCount(unsigned int count)

{
    this->wipeoutCount_[this->historyType_] = count;
    return;
}

THUMB int status::BattleHistory::getWipeoutCount()

{
    return this->wipeoutCount_[this->historyType_];
}

THUMB void status::BattleHistory::setEscapeCount(unsigned int count)
{
    this->escapeCount_[this->historyType_] = count;
    return;
}


THUMB int status::BattleHistory::getEscapeCount()

{
    return this->escapeCount_[this->historyType_];
}

THUMB void status::BattleHistory::regenesisMaxDamage(unsigned short damage)
{
    unsigned short clampedDamage;

    if (this->maxDamage_[this->historyType_] < damage) {
        this->maxDamage_[this->historyType_] = damage;
        clampedDamage = BaseStatus::getClampValue2(this->maxDamage_[this->historyType_], 0, 9999);
        this->maxDamage_[this->historyType_] = clampedDamage;
    }
    if (this->historyType_ > 0) {
        this->maxDamage_[this->historyType_] = this->maxDamage_[0];
    }
    return;
}


THUMB unsigned short status::BattleHistory::getMaxDamage()

{
    return this->maxDamage_[this->historyType_];
}

THUMB void status::BattleHistory::regenesisHeroLevel(unsigned char level)

{
    if (this->heroLevel_[this->historyType_] < level) {
        this->heroLevel_[this->historyType_] = level;
    }
    return;
}

THUMB unsigned char status::BattleHistory::getHeroLevel()

{
    return this->heroLevel_[this->historyType_];
}


THUMB unsigned char status::BattleHistory::getRestMonsterCount()
{
    int encounterCount;
    unsigned char remainingMonsters;

    remainingMonsters = this->restMonster_[this->historyType_];
    if (remainingMonsters == 0) {
        encounterCount = g_BattleResult.getEncountCount();
        remainingMonsters = 0xd2 - encounterCount;
    }
    return remainingMonsters;
}

THUMB void status::BattleHistory::setTitle(unsigned short title)

{
    this->title_[this->historyType_] = title;
    return;
}

THUMB unsigned short status::BattleHistory::getTitle()

{
    return this->title_[this->historyType_];
}

THUMB void status::BattleHistory::setChapterBattleCount(unsigned int count) {
    this->chapterBattleCount_ = count;
    return;
}

THUMB void status::BattleHistory::regenesisChapterBattleCount()
{
    if (this->historyType_ > RightNow) {
        this->battleCount_[this->historyType_] = this->battleCount_[RightNow];
        return;
    }
    this->chapterBattleCount_++;
    this->chapterBattleCount_ = BasePartyStatus::getClampValue(this->chapterBattleCount_, 0, 99999);
    this->battleCount_[RightNow]++;
    this->battleCount_[RightNow] = BasePartyStatus::getClampValue(this->battleCount_[RightNow], 0, 99999);
}

THUMB unsigned int status::BattleHistory::getChapterBattleCount()
{
    return this->chapterBattleCount_;
}

THUMB void status::BattleHistory::setChapterWipeoutCount(unsigned int count){
  this->chapterWipeoutCount_ = count;
  return;
}


THUMB void status::BattleHistory::regenesisChapterWipeoutCount()
{
    if (this->historyType_ > RightNow) {
        this->wipeoutCount_[this->historyType_] = this->wipeoutCount_[RightNow];
        return;
    }
    this->chapterWipeoutCount_++;
    this->chapterWipeoutCount_ = BasePartyStatus::getClampValue(this->chapterWipeoutCount_, 0, 99999);
    this->wipeoutCount_[RightNow]++;
    this->wipeoutCount_[RightNow] = BasePartyStatus::getClampValue(this->wipeoutCount_[RightNow], 0, 99999);
}


THUMB unsigned int status::BattleHistory::getChapterWipeoutCount()

{
    return this->chapterWipeoutCount_;
}



THUMB void status::BattleHistory::setChapterEscapeCount(unsigned int count)
{
    this->chapterEscapeCount_ = count;
    return;
}

THUMB void status::BattleHistory::regenesisChapterEscapeCount()
{
    if (this->historyType_ > RightNow) {
        this->escapeCount_[this->historyType_] = this->escapeCount_[RightNow];
        return;
    }

    this->chapterEscapeCount_++;
    this->chapterEscapeCount_ = BasePartyStatus::getClampValue(this->chapterEscapeCount_, 0, 99999);

    this->escapeCount_[this->historyType_]++;
    this->escapeCount_[this->historyType_] = BasePartyStatus::getClampValue(this->escapeCount_[this->historyType_], 0, 99999);
    return;
}

THUMB unsigned int status::BattleHistory::getChapterEscapeCount()

{
  return this->chapterEscapeCount_;
}



THUMB void status::BattleHistory::setSaveData(profile::PROFILE_PARTY *pPA, profile::PROFILE_HISTORY *pPH)
{
    int i;
    profile::PROFILE_HISTORY *pHistory;
    
    pHistory = pPH;
    
    for (i = 0; i < 3; i++) {
        pHistory->ADVENTURE_TIME = this->adventureTime_[i];
        pHistory->BATTLE_COUNT = this->battleCount_[i];
        pHistory->MONSTER_COUNT = this->monsterCount_[i];
        pHistory->TOTALGOLD = this->totalGold_[i];
        pHistory->VICTORY_GOLD = this->victoryGold_[i];
        pHistory->WIPEOUT_COUNT = this->wipeoutCount_[i];
        pHistory->ESCAPE_COUNT = this->escapeCount_[i];
        pHistory->MAX_DAMAGE = this->maxDamage_[i];
        pHistory->HERO_LEVEL = this->heroLevel_[i];
        pHistory->HERO_TITLE = this->title_[i];
        pHistory++;
    }
    
    pPA->CHAPTERBATTLE_COUNT = this->chapterBattleCount_;
    pPA->CHAPTERWIPEOUT_COUNT = this->chapterWipeoutCount_;
    pPA->CHAPTERESCAPE_COUNT = this->chapterEscapeCount_;
    return;
}

THUMB void status::BattleHistory::setLoadData(profile::PROFILE_PARTY *pPA, profile::PROFILE_HISTORY *pPH)
{
    int i;
    profile::PROFILE_HISTORY *pHistory;
    
    pHistory = pPH;
    
    for (i = 0; i < 3; i++) {
        this->adventureTime_[i] = pHistory->ADVENTURE_TIME;
        this->battleCount_[i] = pHistory->BATTLE_COUNT;
        this->monsterCount_[i] = pHistory->MONSTER_COUNT;
        this->totalGold_[i] = pHistory->TOTALGOLD;
        this->victoryGold_[i] = pHistory->VICTORY_GOLD;
        this->wipeoutCount_[i] = pHistory->WIPEOUT_COUNT;
        this->escapeCount_[i] = pHistory->ESCAPE_COUNT ;
        this->maxDamage_[i] = pHistory->MAX_DAMAGE ;
        this->heroLevel_[i] = pHistory->HERO_LEVEL;
        this->title_[i] = pHistory->HERO_TITLE ;
        pHistory++;
    }
    
    this->chapterBattleCount_ = pPA->CHAPTERBATTLE_COUNT;
    this->chapterWipeoutCount_ = pPA->CHAPTERWIPEOUT_COUNT;
    this->chapterEscapeCount_ = pPA->CHAPTERESCAPE_COUNT;
    return;
}


