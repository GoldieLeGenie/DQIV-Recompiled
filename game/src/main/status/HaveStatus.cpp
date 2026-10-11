#include "main/status/HaveStatus.hpp"
#include "main/dss/Random.hpp"

status::BaseStatus status::HaveStatus::diffStatus_;


THUMB status::HaveStatus::HaveStatus(){
    return;
}

THUMB status::HaveStatus::~HaveStatus(){
    return;
}


THUMB void status::HaveStatus::setup(unsigned short playerIndex, int isPlayer){
    if (isPlayer == 1) {
        setupPlayer(playerIndex);
    }
    else {
        setupMonster(playerIndex, 1);
    }
    return;
}


THUMB void status::HaveStatus::setupPlayer(unsigned short index) {
    const param::CharInitData* charData = status::excelParam.getCharaInitData();
    this->playerIndex_ = index;
    this->charaIndex_ = charData[index].character;
    this->iconIndex_ = charData[index].icon;
    this->exp_ = 0;
    this->level_ = charData[index].level;

    this->baseStatus_.strength_ = charData[index].strength & 0xff;
    this->baseStatus_.agility_ = charData[index].agility;
    this->baseStatus_.protection_ = ((unsigned int)(charData[index].agility << 0x17) >> 0x18);
    this->baseStatus_.wisdom_ = charData[index].intelligence;
    this->baseStatus_.luck_ = charData[index].luck;
    this->baseStatus_.hp_ = charData[index].HP;
    this->baseStatus_.hpMax_ = charData[index].HP;
    this->baseStatus_.mp_ = charData[index].MP;

    unsigned short mp = charData[index].MP;
    this->baseStatus_.mpMax_ = mp;

    if (mp == 0xFF) {
        this->baseStatus_.mp_ = 1000;
        this->baseStatus_.mpMax_ = 1000;
    }

    if (this->baseStatus_.hp_ == 0) {
        this->baseStatus_.hp_ = 10;
        this->baseStatus_.hpMax_ = 10;
    }

    this->job_ = (JobType)charData[index].job;
    this->isPlayer_ = 0;
    this->isBattleNpc_ = 0;
    this->isNoBattleNpc_ = 0;
    this->isSpecialNpc_ = 0;
    this->isMonster_ = 0;

    char isPlayerBit = charData[index].byte_1 & 0x01;
    if (isPlayerBit != 0) {
        this->isPlayer_ = 1;
    }

    int isBattleNpcBit = charData[index].byte_1 & 0x02;
    if ((isBattleNpcBit << 0x17) >> 0x18) {
        this->isBattleNpc_ = 1;
    }

    int isNoBattleNpcBit = charData[index].byte_1 & 0x04;
    if ((isNoBattleNpcBit << 0x16) >> 0x18) {
        this->isNoBattleNpc_ = 1;
    }

    int isSpecialNpcBits = charData[index].byte_1 & 0x08;
    if ((isSpecialNpcBits << 0x15) >> 0x18) {
        this->isSpecialNpc_ = 1;
    }

    if (this->isPlayer_ != 0) {
        unsigned char* expTableBase = (unsigned char*)getLevelupInfo(this->playerIndex_);

        unsigned char* expEntry = expTableBase + (this->level_ + 1) * 0x18;
        this->baseStatus_.exp_ = *(unsigned int*)expEntry;

        unsigned char lvl = this->level_;
        unsigned int offset = lvl * 0x18;
        this->exp_ = *(int*)(expTableBase + offset);
    }
    else {
        this->exp_ = -1;
    }

    this->sex_ = (charData[index].byte_1 & 0x30) >> 4;
}


THUMB void status::HaveStatus::setupMonster(unsigned short index, bool flag) {
    param::MonsterData* monsterData = status::excelParam.monsterData_;

    if (flag != 0) {
        this->playerIndex_ = index;
        this->charaIndex_ = 0;
        this->iconIndex_ = 0;
        this->level_ = monsterData[index].level;
        this->exp_ = monsterData[index].exp;
        this->gold_ = monsterData[index].money;
        this->isPlayer_ = 0;
        this->isBattleNpc_ = 0;
        this->isNoBattleNpc_ = 0;
        this->isSpecialNpc_ = 0;
        this->isMonster_ = 1;
    }

    this->baseStatus_.strength_ = monsterData[index].attack;
    this->baseStatus_.agility_ = monsterData[index].agility;
    this->baseStatus_.protection_ = monsterData[index].defence;
    this->baseStatus_.hp_ = monsterData[index].HP;
    this->baseStatus_.hpMax_ = monsterData[index].HP;
    this->baseStatus_.mp_ = monsterData[index].MP;

    unsigned short mp = monsterData[index].MP;
    this->baseStatus_.mpMax_ = mp;

    if (mp == 0xFF) {
        this->baseStatus_.mp_ = 1000;
        this->baseStatus_.mpMax_ = 1000;
    }
}

THUMB bool status::HaveStatus::isLevelup()
{
    if (this->isPlayer_ == 0) {
        return 0;
    }
    if (this->level_ == 99) {
        return 0;
    }
    if (this->exp_ >= this->baseStatus_.exp_) {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatus::debugLevelup(int level)
{
    setup(this->playerIndex_, 1);
    if (level > this->level_) {
        do {
            levelup(1);
        } while (this->level_ != level);
    }
    return;
}


THUMB void status::HaveStatus::levelup(int flag) {

    this->level_++;

    LevelEntry* levelData = (LevelEntry*)getLevelupInfo(this->playerIndex_);

    if (flag != 0) {
        this->exp_ = this->baseStatus_.exp_;
    }

    status::BaseStatus localStatus;

    // Strength
    localStatus.strength_ = (unsigned char)(levelData[this->level_].strength - levelData[this->level_ - 1].strength);
    diffStatus_.strength_ = levelupAdd(localStatus.strength_) & 0xFF;
    localStatus.strength_ = this->baseStatus_.strength_;
    this->baseStatus_.strength_ = status::BaseStatus::getClampValue2(
        levelupAjust(this->baseStatus_.strength_, diffStatus_.strength_, levelData[this->level_].strength),
        0, 255) & 0xFF;
    diffStatus_.strength_ = (this->baseStatus_.strength_ - localStatus.strength_) & 0xFF;

    // Agility
    localStatus.agility_ = (unsigned char)(levelData[this->level_].agility - levelData[this->level_ - 1].agility);
    diffStatus_.agility_ = levelupAdd(localStatus.agility_);
    localStatus.agility_ = this->baseStatus_.agility_;
    this->baseStatus_.agility_ = status::BaseStatus::getClampValue2(
        levelupAjust(this->baseStatus_.agility_, diffStatus_.agility_, levelData[this->level_].agility),
        0, 255);
    diffStatus_.agility_ = this->baseStatus_.agility_ - localStatus.agility_;

    // Protection
    localStatus.protection_ = (unsigned char)(levelData[this->level_].agility / 2 - levelData[this->level_ - 1].agility / 2);
    diffStatus_.protection_ = levelupAdd(localStatus.protection_) & 0xFF;
    localStatus.protection_ = this->baseStatus_.protection_;
    this->baseStatus_.protection_ = status::BaseStatus::getClampValue2(
        levelupAjust(this->baseStatus_.protection_, diffStatus_.protection_, levelData[this->level_].agility >> 1),
        0, 255) & 0xFF;
    diffStatus_.protection_ = (this->baseStatus_.protection_ - localStatus.protection_) & 0xFF;

    // Wisdom
    localStatus.wisdom_ = (unsigned char)(levelData[this->level_].wisdom - levelData[this->level_ - 1].wisdom);
    diffStatus_.wisdom_ = levelupAdd(localStatus.wisdom_);
    localStatus.wisdom_ = this->baseStatus_.wisdom_;
    this->baseStatus_.wisdom_ = status::BaseStatus::getClampValue2(
        levelupAjust(this->baseStatus_.wisdom_, diffStatus_.wisdom_, levelData[this->level_].wisdom),
        0, 255);
    diffStatus_.wisdom_ = this->baseStatus_.wisdom_ - localStatus.wisdom_;

    // Luck
    localStatus.luck_ = (unsigned char)(levelData[this->level_].luck - levelData[this->level_ - 1].luck);
    diffStatus_.luck_ = levelupAdd(localStatus.luck_);
    localStatus.luck_ = this->baseStatus_.luck_;
    this->baseStatus_.luck_ = status::BaseStatus::getClampValue2(
        levelupAjust(this->baseStatus_.luck_, diffStatus_.luck_, levelData[this->level_].luck),
        0, 255);
    diffStatus_.luck_ = this->baseStatus_.luck_ - localStatus.luck_;

    // HP Max
    localStatus.hpMax_ = (unsigned char)(levelData[this->level_].hpMax - levelData[this->level_ - 1].hpMax);
    diffStatus_.hpMax_ = levelupAdd(localStatus.hpMax_) & 0xFF;
    localStatus.hpMax_ = this->baseStatus_.hpMax_;
    this->baseStatus_.hpMax_ = status::BaseStatus::getClampValue2(
        levelupAjust(this->baseStatus_.hpMax_, diffStatus_.hpMax_, levelData[this->level_].hpMax),
        0, 999);
    diffStatus_.hpMax_ = this->baseStatus_.hpMax_ - localStatus.hpMax_;

    // MP Max
    localStatus.mpMax_ = (unsigned char)(levelData[this->level_].mpMax - levelData[this->level_ - 1].mpMax);
    diffStatus_.mpMax_ = levelupAdd(localStatus.mpMax_) & 0xFF;
    localStatus.mpMax_ = this->baseStatus_.mpMax_;
    this->baseStatus_.mpMax_ = status::BaseStatus::getClampValue2(
        levelupAjust(this->baseStatus_.mpMax_, diffStatus_.mpMax_, levelData[this->level_].mpMax),
        0, 999);
    diffStatus_.mpMax_ = this->baseStatus_.mpMax_ - localStatus.mpMax_;

    // Next level exp
    this->baseStatus_.exp_ = levelData[this->level_ + 1].exp;

    if (flag != 0) {
        this->baseStatus_.hp_ = this->baseStatus_.hpMax_;
        this->baseStatus_.mp_ = this->baseStatus_.mpMax_;
    }

}

THUMB unsigned int status::HaveStatus::levelupAdd(unsigned short value)
{
    unsigned short randomFactor = dssrand::rand(50) + 75;
    unsigned short numerator = value * randomFactor + 50;
   unsigned short result = numerator / 100;
    if (result < value) {
        result = value;
    }
    return result;
}



THUMB unsigned short status::HaveStatus::levelupAjust(unsigned short value, unsigned short diff, unsigned short normal) {

    unsigned short thresholdHigh = (normal * 150) / 100;
    unsigned short thresholdLow = (normal * 75) / 100;
    if (value > thresholdHigh) {
        return value + dssrand::rand(2);
    }
    int combined = value + diff;

    if (combined > thresholdHigh) {
        return value + dssrand::rand(2);
    }
    if (combined >= thresholdLow) {
        thresholdLow = combined;
    }

    return thresholdLow;
}

THUMB void* status::HaveStatus::getLevelupInfo(int playerIndex) {
    void* result;
    result = 0;

    switch (playerIndex) {
    case 1:
    case 2:
        result = status::excelParam.getHeroData();
        return result;
    case 3:
        result = status::excelParam.getWarriorData();
        return result;
    case 4:
        result = status::excelParam.getPrincessData();
        return result;
    case 5:
        result = status::excelParam.getPriestData();
        return result;
    case 6:
        result = status::excelParam.getMageData();
        return result;
    case 7:
        result = status::excelParam.getTraderData();
        return result;
    case 8:
        result = status::excelParam.getWarlockData();
        return result;
    case 9:
        result = status::excelParam.getDancerData();
        return result;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
        break;
    case 25:
        result = status::excelParam.getPissaroData();
        return result;
    default:
        break;
    }

    return result;
}


THUMB void status::HaveStatus::addExp(int exp)
{
    int totalExp;

    if (this->exp_ != -1) {
        totalExp = this->exp_ + exp;
        this->exp_ = totalExp;
        totalExp = dss::clamp<int>(totalExp, 0, 0xffffff);
        this->exp_ = totalExp;
    }
    return;
}

THUMB void status::HaveStatus::setExp(int exp) {
    if (this->exp_ != -1) {
        this->exp_ = exp;
        this->exp_ = dss::clamp<int>(exp, 0, 0xffffff);
    }
}


THUMB void status::HaveStatus::setLevel(unsigned char lvl) {
    void* LevelupInfo;
    if (this->isPlayer_ != 0) {
        LevelupInfo = getLevelupInfo(this->playerIndex_);
        this->level_ = lvl;
        this->baseStatus_.exp_ = *(int*)((char*)LevelupInfo + (this->level_ + 1) * 0x18);
    }
}

THUMB int status::HaveStatus::getLevelupExp()
{
    return this->baseStatus_.exp_ - this->exp_;
}

THUMB void status::HaveStatus::addStrength(char str) {
    this->baseStatus_.addStrength(str);
    return;
}

THUMB unsigned char status::HaveStatus::getAgility() {
    return this->baseStatus_.getClampValue2(this->baseStatus_.agility_, 0, 0xff);
}

THUMB void status::HaveStatus::AddAgility(char agi) {
    this->baseStatus_.addAgility(agi);
    return;
}

THUMB void status::HaveStatus::addProtection(char prot)

{
    this->baseStatus_.addProtection(prot);
    return;
}

THUMB unsigned char  status::HaveStatus::getWisdom() {
    return this->baseStatus_.getClampValue2(this->baseStatus_.wisdom_, 0, 0xFF);
}

THUMB void status::HaveStatus::addWisdom(char wisd)

{
    this->baseStatus_.addWisdom(wisd);
    return;
}

THUMB unsigned char status::HaveStatus::getLuck()
{
    return this->baseStatus_.getClampValue2(this->baseStatus_.luck_, 0, 0xff);
}


THUMB unsigned int status::HaveStatus::getHp() {
    return this->baseStatus_.getClampValue2(this->baseStatus_.hp_, 0, 9999);
}



THUMB void status::HaveStatus::setHp(unsigned short hp)
{
    this->baseStatus_.hp_ = hp;
}

THUMB unsigned short status::HaveStatus::getHpMax() {
    return this->baseStatus_.getClampValue2(this->baseStatus_.hpMax_, 0, 9999);
}

THUMB void status::HaveStatus::addHpMax(char hp)
{
    if (this->baseStatus_.mp_ != 1000) {
        baseStatus_.addHpMax(hp);
    }
    return;
}


THUMB unsigned int status::HaveStatus::getMp() {
    return this->baseStatus_.getClampValue2(this->baseStatus_.mp_, 0, 999);
}

THUMB void status::HaveStatus::setMp(unsigned short mp)
{
    this->baseStatus_.mp_ = mp;
}



THUMB unsigned short status::HaveStatus::getMpMax() {
    return this->baseStatus_.getClampValue2(this->baseStatus_.mpMax_, 0, 999);
}



THUMB void status::HaveStatus::addMpMax(char mp)
{
    this->baseStatus_.addMpMax(mp);
    return;
}

THUMB void status::HaveStatus::addBaseHp(int hp) {
    this->baseStatus_.hp_ = dss::clamp<int>(this->baseStatus_.hp_ + hp, 0, this->baseStatus_.hpMax_);
    return;
}

THUMB void status::HaveStatus::addBaseMp(int mp) {
    if (this->baseStatus_.mp_ != 0x3E8) {
        this->baseStatus_.mp_ = dss::clamp<int>(this->baseStatus_.mp_ + mp, 0, this->baseStatus_.mpMax_);
    }
    return;
}