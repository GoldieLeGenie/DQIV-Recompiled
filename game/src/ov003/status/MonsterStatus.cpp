#include "ov003/status/MonsterStatus.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/dss/Random.hpp"


THUMB status::MonsterStatus::MonsterStatus() {
    this->eventFlag_.flag_ = 0;
    this->eventFlag_.flag_ = 0;
}

THUMB status::MonsterStatus::~MonsterStatus(){
    return;
}

THUMB void status::MonsterStatus::setup(int monsterGroup, int monsterIndex, int sortIndex) {
    this->characterGroup_ = monsterGroup;
    this->characterIndex_ = monsterIndex;
    this->sortIndex_ = sortIndex;
    this->characterType_ = MONSTER;
    this->CharacterStatus::setup(MONSTER, monsterGroup, monsterIndex);
    this->type_.flag_ = 0;
    if ((int)((status::excelParam.monsterData_[this->characterIndex_].byte_4 & 0xC0) << 0x12) >> 0x18 != 0) {
        this->type_.flag_ |= 1;
    }
    if ((char)(status::excelParam.monsterData_[this->characterIndex_].byte_5 & 3)) {
        this->type_.flag_ |= 2;
    }
    if ((int)((status::excelParam.monsterData_[this->characterIndex_].byte_5 & 0xC) << 0x16) >> 0x18 != 0) {
        this->type_.flag_ |= 4;
    }
    if ((int)((status::excelParam.monsterData_[this->characterIndex_].byte_5 & 0x30) << 0x14) >> 0x18 != 0) {
        this->type_.flag_ |= 8;
    }
    if ((int)((status::excelParam.monsterData_[this->characterIndex_].byte_5 & 0xC0) << 0x12) >> 0x18 != 0) {
        this->type_.flag_ |= 0x10;
    }
}

THUMB void status::MonsterStatus::cleanup()
{
  this->type_.flag_ = 0;
  this->characterIndex_ = -1;
  return;
}

THUMB bool status::MonsterStatus::isEnable() {
    if (this->characterIndex_ != -1) {
        return 1;
    }
    return 0;
}

THUMB bool status::MonsterStatus::isBattleEnable() {
    if (this->characterIndex_ == -1) {
        return false;
    } else if (this->haveStatusInfo_.isDeath()) {
        return false;
    } else {
        return !this->haveBattleStatus_.escape_;
    }
}

THUMB bool status::MonsterStatus::isBeingEnable()
{
  return this->characterIndex_ != -1;
}

THUMB int status::MonsterStatus::getDropItem()
{
    unsigned short dropItemId = 0; 
    param::MonsterData* data = &status::excelParam.monsterData_[this->characterIndex_];
    unsigned short itemCandidate = data->item;
    unsigned char ratioType = data->itemRatio;

    switch (ratioType) 
    {
        case 0: 
            if (dssrand::rand(0x1) == 0) dropItemId = itemCandidate;
            break;

        case 1: 
            if (dssrand::rand(0x8) == 0) dropItemId = itemCandidate;
            break;

        case 2: 
            if (dssrand::rand(0x10) == 0) dropItemId = itemCandidate;
            break;

        case 3: 
            if (dssrand::rand(0x20) == 0) dropItemId = itemCandidate;
            break;

        case 4: 
            if (dssrand::rand(0x40) == 0) dropItemId = itemCandidate;
            break;

        case 5: 
            if (dssrand::rand(0x80) == 0) dropItemId = itemCandidate;
            break;

        case 6: 
            if (dssrand::rand(0x100) == 0) dropItemId = itemCandidate;
            break;

        case 7:
            if (dssrand::rand(0x1000) == 0) dropItemId = itemCandidate;
            break;

        default:
            break;
    }

    return dropItemId; 
}

THUMB int status::MonsterStatus::getHaveDropItem()
{
  return status::excelParam.monsterData_[this->characterIndex_].item;
}

THUMB void status::MonsterStatus::print()

{
  return;
}

THUMB void status::MonsterStatus::printAction()
{
  return;
}


THUMB void status::MonsterStatus::naturalRecovery() {
    int heal;
    if (this->haveStatusInfo_.isDeath()) {
        return;
    }
    heal = 0;
    switch (status::excelParam.monsterData_[this->characterIndex_].heal) {
    case 0:
        heal = 0;
        break;
    case 1:
        heal = dssrand::rand(9) + 0x10;
        break;
    case 2:
        heal = dssrand::rand(0xD) + 0x2C;
        break;
    case 3:
        heal = dssrand::rand(0x15) + 0x5A;
        break;
    }
    this->haveStatusInfo_.addBaseHp(heal);
}

THUMB void status::MonsterStatus::setStartStatus() {
    bool flag = false;
    switch (status::excelParam.monsterData_[this->characterIndex_].initRatio) {
    case 0:
        flag = true;
        break;
    case 1:
        if (dssrand::rand(2) == 0) {
            flag = true;
        }
        break;
    case 2:
        if (dssrand::rand(8) == 0) {
            flag = true;
        }
        break;
    case 3:
        if (dssrand::rand(0x10) == 0) {
            flag = true;
        }
        break;
    case 4:
        if (dssrand::rand(0x20) == 0) {
            flag = true;
        }
        break;
    case 5:
        if (dssrand::rand(0x40) == 0) {
            flag = true;
        }
        break;
    case 6:
        if (dssrand::rand(0x100) == 0) {
            flag = true;
        }
        break;
    case 7:
        break;
    default:
        break;
    }
    if (flag) {
        switch (status::excelParam.monsterData_[this->characterIndex_].init) {
        case 0:
            this->haveStatusInfo_.statusChange_.setup2(StatusChange::StatusSleep, true);
            return;
        case 1:
            this->haveStatusInfo_.statusChange_.setup2(StatusChange::StatusSpazz, true);
            return;
        case 2:
            this->haveStatusInfo_.statusChange_.setup2(StatusChange::StatusConfusion, true);
            return;
        case 3:
            this->haveStatusInfo_.statusChange_.setup(StatusChange::StatusMax, true);
            return;
        }
    }
}

THUMB void status::MonsterStatus::execStartOfRound()
{
  this->haveBattleStatus_.setSelectCommand(HaveBattleStatus::AISelect,0);
  return;
}

THUMB bool status::MonsterStatus::isDeathDisable() {
    switch (this->haveBattleStatus_.index_) {
        case 0xAE:
        case 0xCD:
        case 0xCE:
        case 0xCF:
        case 0xD0:
        case 0xD1:
        case 0xD2:
        case 0x130:
        case 0x131:
        case 0x132:
        case 0x133:
        case 0x134:
        case 0x135:
            return 1;
        default:
            return 0;
    }
}

THUMB void status::MonsterStatus::setMosyasChange(int monsterIndex)
{
  this->haveBattleStatus_.newBaseChangeMonster(monsterIndex);
  this->haveStatusInfo_.setFirstMosyas(1);
  return;
}



THUMB void status::MonsterStatus::setMosyasRelease()
{
    this->haveStatusInfo_.setMosyasRelease(1);
    this->haveStatusInfo_.releaseMosyas();
    this->mosyasTarget_->haveStatusInfo_.setMosyasTarget(0);
    this->haveBattleStatus_.newBaseChangeMonsterWithHpMp(0x7a);
}