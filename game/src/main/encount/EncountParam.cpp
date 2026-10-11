#include "main/encount/EncountParam.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/dss/Random.hpp"
using namespace btl;


THUMB encount::EncountParam::EncountParam()
{
  this->eventEncount_ = 0;
  return;
}

THUMB encount::EncountParam::~EncountParam()
{
  return;
}



THUMB void encount::EncountParam::setup(int index) {
    if (index == -1) {
        index = 0;
    }

    param::EncountData* encountData = status::excelParam.encountData_;

    this->monsterIndex_[0] = encountData[index].monsterA;
    this->monsterIndex_[1] = encountData[index].monsterB;
    this->monsterIndex_[2] = encountData[index].monsterC;
    this->monsterIndex_[3] = encountData[index].monsterD;
    this->monsterIndex_[4] = encountData[index].monsterE;
    this->monsterIndex_[5] = encountData[index].monsterF;
    this->monsterIndex_[6] = encountData[index].monsterG;
    this->monsterIndex_[7] = encountData[index].monsterH;
    this->monsterIndex_[8] = encountData[index].monsterI;
    this->monsterIndex_[9] = encountData[index].monsterJ;
    this->monsterIndex_[10] = encountData[index].monsterK;
    this->monsterIndex_[11] = encountData[index].monsterL;
    this->monsterIndex_[12] = encountData[index].specialM;
    this->monsterIndex_[13] = encountData[index].specialN;

    this->tileLevel_ = encountData[index].tileLevel;
    this->tileRate_ = encountData[index].ratio;
    this->formationId_ = encountData[index].formation;
    this->teamAddDisable_ = encountData[index].invite;
    this->escapeDisable_ = encountData[index].escape;

    this->eventEncount_ = encountData[index].event;
    this->sound_ = encountData[index].sound;


    switch (encountData[index].firstattack) {
    case 0:
        if (dssrand::rand(32) == 0) {
            this->firstAttack_ = FirstAttackPlayer;
            return;
        }
        if (dssrand::rand(32) == 0) {
            this->firstAttack_ = FirstAttackMonster;
            return;
        }
        this->firstAttack_ = FirstAttackNone;
        return;
    case 1:
        if (dssrand::rand(32) == 0) {
            this->firstAttack_ = FirstAttackPlayer;
            return;
        }
        if (dssrand::rand(4) == 7) {
            this->firstAttack_ = FirstAttackMonster;
            return;
        }
        this->firstAttack_ = FirstAttackNone;
        return;
    case 2:
        if (dssrand::rand(4) == 0) {
            this->firstAttack_ = FirstAttackPlayer;
            return;
        }
        if (dssrand::rand(32) == 0) {
            this->firstAttack_ = FirstAttackMonster;
            return;
        }
        this->firstAttack_ = FirstAttackNone;
        return;
    case 3:
        if (dssrand::rand(4) == 0) {
            this->firstAttack_ = FirstAttackPlayer;
            return;
        }
        if (dssrand::rand(4) == 0) {
            this->firstAttack_ = FirstAttackMonster;
            return;
        }
        this->firstAttack_ = FirstAttackNone;
        return;
    case 6:
        this->firstAttack_ = FirstAttackNone;
        return;
    default:
        return;
    }
}

THUMB int encount::EncountParam::getFormationId()
{
  return this->formationId_;
}

THUMB btl::FirstAttack  encount::EncountParam::getFirstAttack()
{
  return this->firstAttack_;
}

THUMB bool encount::EncountParam::isEventEncount()
{
  return this->eventEncount_ != 0;
}

THUMB int encount::EncountParam::getMonsterIndex(EncountType type)
{
  return this->monsterIndex_[type];
}

THUMB int encount::EncountParam::getSound()
{
  return this->sound_;
}