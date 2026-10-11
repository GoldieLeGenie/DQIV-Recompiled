#include "main/status/BattleResult.hpp"

status::BattleResult status::g_BattleResult;


THUMB status::BattleResult::BattleResult()
{
}


THUMB status::BattleResult::~BattleResult()

{
  return;
}

THUMB void status::BattleResult::initialize() {
    for (unsigned int i = 0; i < 210; i++) {
        this->result_[i].monsterCount_ = 0;
        this->result_[i].itemCount_ = 0;
        this->result_[i].level_ = 0;
        this->defaultMonsterNo_[i] = 0;
        setEncount(i, 0);
        for (int j = 0; j < 309; j++) {
            if (excelParam.bookData_[i].name == excelParam.monsterData_[j].name) {
                this->defaultMonsterNo_[i] = excelParam.monsterData_[j].name;
            }
        }
    }

    this->playerVictory_ = 0;
    this->playerDemolition_ = 0;
    this->playerDemolitionMessage_ = 2;
    this->disablePlayerDemolition_ = 0;
    this->battleExecute_ = 0;
    this->playerFirstAttack_ = 0;
    this->monsterFirstAttack_ = 0;
    this->battleTurnCount_ = 0;
}

THUMB void status::BattleResult::setMonsterCount(unsigned int monsterIndex, unsigned short count)
{
    this->result_[monsterIndex].monsterCount_ = count;
    return;
}

THUMB void status::BattleResult::addMonsterCount(unsigned int monsterIndex)
{
    this->result_[monsterIndex].monsterCount_ = this->result_[monsterIndex].monsterCount_ + 1;
    this->result_[monsterIndex].monsterCount_ = BaseStatus::getClampValue2(this->result_[monsterIndex].monsterCount_, 0, 999);
    return;
}

THUMB unsigned short status::BattleResult::getMonsterCount(unsigned int monsterIndex)
{
    return this->result_[monsterIndex].monsterCount_;
}

THUMB unsigned int status::BattleResult::getMaxExp(unsigned int monsterIndex) {
    return excelParam.monsterData_[this->defaultMonsterNo_[monsterIndex]].exp;
}

THUMB unsigned int status::BattleResult::getMaxGold(unsigned int monsterIndex)

{
    return excelParam.monsterData_[this->defaultMonsterNo_[monsterIndex]].money;
}

THUMB void status::BattleResult::setItemCount(unsigned int monsterIndex, unsigned short count)
{
    this->result_[monsterIndex].itemCount_ = count;
    return;
}

THUMB void status::BattleResult::addItemCount(unsigned int monsterIndex) {
    this->result_[monsterIndex].itemCount_++;
    this->result_[monsterIndex].itemCount_ = BaseStatus::getClampValue2(
        this->result_[monsterIndex].itemCount_,
        0,
        999
    );
    return;
}

THUMB unsigned short status::BattleResult::getItemCount(unsigned int monsterIndex)
{
    return this->result_[monsterIndex].itemCount_;
}

THUMB unsigned int status::BattleResult::getItemIndex(unsigned int monsterIndex, int getItemID) {
    unsigned int result;

    if (this->result_[monsterIndex].itemCount_ || getItemID == 1) {
        unsigned short item = excelParam.monsterData_[this->defaultMonsterNo_[monsterIndex]].item;
        result = item & 0xff;
        if ((item & 0xff) == 0) {
            return 0;
        }
    }
    else {
        result = 0;
    }
    return result;
}

THUMB void status::BattleResult::setLevel(unsigned int monsterIndex, unsigned short level)
{
    this->result_[monsterIndex].level_ = level;
    return;
}

THUMB void status::BattleResult::regenesisLevel(unsigned int monsterIndex, unsigned char level)
{
    if (this->result_[monsterIndex].level_ == 0) {
        this->result_[monsterIndex].level_ = level;
        this->result_[monsterIndex].level_ = BaseStatus::getClampValue(this->result_[monsterIndex].level_, 1, 99);
        return;
    }
    return;
}

THUMB unsigned char status::BattleResult::getLevel(unsigned int monsterIndex)

{
  return this->result_[monsterIndex].level_;
}



THUMB unsigned int status::BattleResult::getExpTotal(unsigned int monsterIndex) {
    return getMaxExp(monsterIndex) * this->result_[monsterIndex].monsterCount_;
}

THUMB unsigned int status::BattleResult::getGoldTotal(unsigned int monsterIndex)
{
    return this->result_[monsterIndex].monsterCount_ * getMaxGold(monsterIndex);
}

THUMB void status::BattleResult::setEncount(unsigned int monsterIndex, int flag)
{
    if (flag == 1) {
        this->encountFlag_.flag_[monsterIndex >> 5].flag_ =
            this->encountFlag_.flag_[monsterIndex >> 5].flag_ | 1 << (monsterIndex & 0x1f);
        return;
    }
    else {
        this->encountFlag_.flag_[monsterIndex >> 5].flag_ =
            this->encountFlag_.flag_[monsterIndex >> 5].flag_ & ~(1 << (monsterIndex & 0x1f));
        return;
    }
    return;
}


THUMB bool status::BattleResult::isEncount(unsigned int monsterIndex)
{
    unsigned int bit = monsterIndex & 0x1f;
    unsigned int mask = 1 << bit;
    unsigned int flag = this->encountFlag_.flag_[monsterIndex >> 5].flag_;
    return (flag & mask);
}

THUMB int status::BattleResult::getEncountCount()
{
    int result = 0;
    for (unsigned int i = 0; i < 210; i++) {
        unsigned int bit = i & 0x1f;
        unsigned int mask = 1 << bit;
        unsigned int flag = this->encountFlag_.flag_[i >> 5].flag_;
        int found = (flag & mask) != 0;
        if (found == 1) {
            result++;
        }
    }
    return result;
}

THUMB unsigned int status::BattleResult::getAllMonsterTotalExp()
{
    unsigned int total = 0;
    for (unsigned int i = 0; i < 210; i++)
    {
        total += getExpTotal(i);
    }
    return total;
}

THUMB void status::BattleResult::setDisablePlayerDemolition(bool flag) {
    this->disablePlayerDemolition_ = flag;
    return;
}

THUMB int status::BattleResult::isDisablePlayerDemolition()
{
    return this->disablePlayerDemolition_;
}

