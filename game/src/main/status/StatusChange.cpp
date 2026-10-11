#include "main/status/StatusChange.hpp"
#include "main/status/UseAction.hpp"
#include "main/dss/Random.hpp"

int status::StatusChange::actionIndexFizzleZone_;
status::StatusChangeOne status::StatusChange::statusFizzleZone_;

THUMB status::StatusChange::StatusChange()
{
   return;
}


THUMB  status::StatusChange::~StatusChange()
{
    return;
}

THUMB bool status::StatusChange::setup(int actionIndex, bool flag)
{
    int damageType;
    status::StatusChangeOne* statusData = this->status_;
    int iVar1;
    damageType = status::UseAction::getDamageType(actionIndex);
    iVar1 = canPerformAction(damageType);
    if (iVar1 == 0) {
        return 0;
    }
    switch (damageType) {
    case 8:
        statusData[8].setup(damageType, flag);
        this->status_[8].turn_ = '\x04';
        this->status_[8].astron_ = '\x01';
        this->actionIndex_[8] = actionIndex;
        break;
    case 9:
        statusData[9].setup(damageType, flag);
        this->actionIndex_[9] = actionIndex;
        break;
    case 10:
        statusData[10].setup(damageType, flag);
        this->actionIndex_[10] = actionIndex;
        break;
    case 0xb:
        statusData[0xb].setup(damageType, flag);
        this->actionIndex_[0xb] = actionIndex;
        if (actionIndex == 0x1e0) {
            this->actionIndex_[0xb] = 0x1f;
        }
        if (actionIndex == 0x21c) {
            this->actionIndex_[0xb] = 0x1f;
        }
        break;
    case 0xc:
        statusData[0xc].setup(damageType, flag);
        this->actionIndex_[0xc] = actionIndex;
        break;
    case 0xd:
        statusData[0xd].setup(damageType, flag);
        this->actionIndex_[0xd] = actionIndex;
        break;
    case 0xe:
        statusData[0xe].setup(damageType, flag);
        this->actionIndex_[0xe] = actionIndex;
        break;
    case 0xf:
        statusData[0xf].setup(damageType, flag);
        this->actionIndex_[0xf] = actionIndex;
        break;
    case 0x10:
        statusData[0x10].setup(damageType, flag);
        this->actionIndex_[0x10] = actionIndex;
        break;
    case 0x12:
        statusData[0x12].setup(damageType, flag);
        this->actionIndex_[0x12] = actionIndex;
        if (actionIndex == 0x150) {
            this->actionIndex_[0x12] = 0x1e;
        }
        break;
    case 0x13:
        statusData[0x13].setup(damageType, flag);
        this->actionIndex_[0x13] = actionIndex;
        break;
    case 0x14:
        statusData[0x14].setup(damageType, flag);
        this->actionIndex_[0x14] = actionIndex;
        break;
    case 0x16:
        statusData[0x16].setup(damageType, flag);
        this->actionIndex_[0x16] = actionIndex;
        break;
    case 0x18:
        statusData[0x18].setup(damageType, flag);
        this->actionIndex_[0x18] = actionIndex;
        break;
    case 0x19:
        statusData[0x19].setup(damageType, flag);
        this->actionIndex_[0x19] = actionIndex;
        break;
    case 0x1a:
        statusData[0x1a].setup(damageType, flag);
        this->actionIndex_[0x1a] = actionIndex;
        this->status_[0x1a].turn_ = 1;
        break;
    case 0x27:
        statusData[0x1b].setup(damageType, flag);
        this->actionIndex_[0x1b] = actionIndex;
        break;
    case 0x28:
        statusFizzleZone_.setup(damageType, flag);
        actionIndexFizzleZone_ = actionIndex;
        break;
    case 0x15:
        statusData[0x15].setup(damageType, flag);
        this->actionIndex_[0x15] = actionIndex;
        break;
    case 0x11:
        statusData[0x11].setup(damageType, flag);
        this->status_[0x11].turn_ = '\x02';
        this->actionIndex_[0x11] = actionIndex;
        break;
    case 7:
        if (actionIndex == 0x40 || actionIndex == 0xca || actionIndex == 0xa2 || actionIndex == 0x15e) {
            statusData[0x16].cleanup();
        }
        if (actionIndex == 0x41 || actionIndex == 0xa5 || actionIndex == 0x161) {
            statusData[9].cleanup();
        }
        if (actionIndex == 0x46) {
            statusData[10].cleanup();
        }
        if (actionIndex == 0x15b) {
            statusData[0x14].cleanup();
        }
        if (actionIndex == 0x6a || actionIndex == 0x154) {
            statusData[0xf].cleanup();
            statusData[0xe].cleanup();
            statusData[0x18].cleanup();
            statusData[0xd].cleanup();
            statusData[0xc].cleanup();
            statusData[0x10].cleanup();
            statusData[8].cleanup();
            statusData[0x12].cleanup();
            statusData[0xb].cleanup();
            statusData[0x13].cleanup();
            statusData[0x19].cleanup();
            statusFizzleZone_.cleanup();
        }
        break;
    case 0x1c:
        statusData[11].setup(damageType, flag);
        this->actionIndex_[11] = actionIndex;
        this->status_[0xb].turn_ = dssrand::rand(0x2) + 1;
        break;
    case 0x29:
        statusData[30].setup(damageType, flag);
        this->actionIndex_[30] = actionIndex;
        break;
    }
    if (actionIndex == 0x1d0) {
        statusFizzleZone_.setup(0x28, flag);
        actionIndexFizzleZone_ = actionIndex;
    }
    if (actionIndex == 0x4b || (unsigned int)(actionIndex - 0x9b) <= 1) {
        statusData[0x1b].setup(0x27, flag);
    }
    if (actionIndex == 0x10a || actionIndex == 0x155 || actionIndex == 0x1b8 ||
        actionIndex == 0x212 || (unsigned int)(actionIndex - 0x218) <= 2) {
        statusData[0x15].setup(damageType, flag);
        statusData[0x15].setup(0x15, flag);
        statusData[0x15].turn_ = 1;
        this->actionIndex_[0x15] = actionIndex;
    }
    checkValidAfter(damageType);
    return 1;
}


THUMB void status::StatusChange::setup2(Status status, int flag)
{
    status::StatusChangeOne* statusData = this->status_;
    switch (status) {
    case StatusAstoron:
        statusData[status].setup(8, flag);
        return;
    case StatusSpazz:
        statusData[status].setup(9, flag);
        return;
    case StatusSleep:
        statusData[status].setup(10, flag);
        return;
    case StatusManusa:
        statusData[status].setup(11, flag);
        return;
    case StatusBaikiruto:
        statusData[status].setup(12, flag);
        return;
    case StatusFubaha:
        statusData[status].setup(13, flag);
        return;
    case StatusMahokanta:
        statusData[status].setup(14, flag);
        return;
    case StatusMahosute:
        statusData[status].setup(15, flag);
        return;
    case StatusMosyasu:
        statusData[status].setup(16, flag);
        return;
    case StatusMahoton:
        statusData[status].setup(18, flag);
        return;
    case StatusDragoram:
        statusData[status].setup(19, flag);
        return;
    case StatusConfusion:
        statusData[status].setup(20, flag);
        return;
    case StatusPoison:
        statusData[status].setup(22, flag);
        return;
    case StatusDefenceChange:
        statusData[status].setup(24, flag);
        return;
    case StatusAgilityChange:
        statusData[status].setup(25, flag);
        return;
    case StatusFeather:
        statusData[status].setup(26, flag);
        return;
    case StatusDefence:
        statusData[status].setup(39, flag);
        return;
    case StatusFizzleZone:
        statusFizzleZone_.setup(40, flag);
        return;
    case StatusPath1:
        statusData[status].setup(21, flag);
        return;
    case StatusPowerSave:
        statusData[status].setup(17, flag);
        return;
    case StatusTimeStop:
        statusData[status].setup(41, flag);
        return;
    case StatusLight:
        statusData[status].setup(28, flag);
        return;
    default:
        return;
    }
}

THUMB void status::StatusChange::clear() {
    status::StatusChangeOne *ptr = this->status_;
    for (int i = 0; i < 35; i++, ptr++){
        ptr->clear();
    }
    return;
}


THUMB int status::StatusChange::canPerformAction(int damageType) {
    status::StatusChangeOne* statusData = this->status_;

    if ((isEnable(StatusAstoron) != 0) && (StatusChangeOne::isValidOnAstron(damageType) == 0)) {
        return 0;
    }

    if ((isEnable(StatusSpazz) != 0) && (StatusChangeOne::isValidOnSpazz(damageType) == 0)) {
        return 0;
    }

    return 1;
}


THUMB void status::StatusChange::checkValidAfter(int damageType)
{
    if (this->isEnable(StatusSleep)) {
        if (!StatusChangeOne::isAfterOnSleep(damageType)) {
            this->release(StatusSleep);
        }
    }

    if (this->isEnable(StatusPath1)) {
        if (!StatusChangeOne::isAfterOnPath1(damageType)) {
            this->release(StatusPath1);
        }
    }

    if (this->isEnable(StatusConfusion)) {
        if (!StatusChangeOne::isAfterOnConfuze(damageType)) {
            this->release(StatusConfusion);
        }
    }
    if (this->isEnable(StatusPowerSave)) {
        if (!StatusChangeOne::isAfterOnConfuze(damageType)) {
            this->release(StatusPowerSave);
        }
    }
}


THUMB int status::StatusChange::isEnable(Status status)
{
    int isEnable;
    if (status == StatusFizzleZone) {
        isEnable = statusFizzleZone_.isEnable();
        return isEnable;
    }
    isEnable = this->status_[status].isEnable();
    return isEnable;
}

THUMB void status::StatusChange::setTurn(Status status, char turn)
{
    if (status == StatusFizzleZone) {
        statusFizzleZone_.turn_ = turn;
        return;
    }
    this->status_[status].turn_ = turn;
}


THUMB void status::StatusChange::execStartOfTurn() {
    status::StatusChangeOne *ptr = this->status_;
    
    for (int i = 0; i < 35; i++, ptr++) {
        ptr->execStartOfTurn();
        if (ptr->isCancel()) {
            ptr->setEnable(false);
            ptr->setRelease(true);
        }
    }
    return;
}


THUMB void status::StatusChange::execEndOfTurn()
{
    status::StatusChangeOne* ptr = this->status_;
    for (int i = 0; i < 0x23; i++, ptr++) {
        ptr->execEndOfTurn();
        if (ptr->isCancel()) {
            ptr->setEnable(0);
            ptr->setRelease(1);
        }
    }
    return;
}

THUMB void status::StatusChange::execEndOfRound()
{
    status::StatusChangeOne* ptr = this->status_;
    for (int i = 0; i < 0x23; i++, ptr++) {
        ptr->execEndOfRound();
        if (ptr->isCancel()) {
            ptr->setEnable(0);
        }
    }
    return;
}

THUMB void status::StatusChange::execEndOfBattle()
{
    StatusChangeOne* ptr = this->status_;
    for (int i = 0; i < 0x23; i++, ptr++) {
        if (i != 0x16) {
            ptr->clear();
        }
    }
    statusFizzleZone_.clear();
    return;
}

THUMB int status::StatusChange::isRelease(){
    status::StatusChangeOne *ptr = this->status_;
    
    for (int i = 0; i < 35; i++) {
        if (ptr->isRelease() != 0) {
            return i;
        }
        ptr++;
    }
    
    return 0;
}

THUMB int status::StatusChange::getExecMessage1(Status status) {
    int result;
    if (status == StatusFizzleZone) {
        result = statusFizzleZone_.isRelease();
        if (result != 0) {
            return 1;
        }
        return 0;
    }
    result = this->status_[status].isRelease();
    if (result != 0) {
        return 1;
    }
    return 0;
}

THUMB void status::StatusChange::release(Status status)
{
    this->status_[status].clear();
    return;
}

THUMB int status::StatusChange::getActionIndex(Status status)
{
    if (status == StatusFizzleZone) {
        return actionIndexFizzleZone_;
    }
    return this->actionIndex_[status];
}

THUMB int status::StatusChange::getExecMessage(Status status) {
    int resultEnable;
    int resultExecMessage;
    
    if (status == StatusFizzleZone) {
        resultEnable = statusFizzleZone_.isEnable();
        if (resultEnable != 0) {
            resultExecMessage = statusFizzleZone_.getExecMessage();
            return resultExecMessage;
        }
    } else {
        resultEnable = this->status_[status].isEnable();
        if (resultEnable != 0) {
            resultExecMessage = this->status_[status].getExecMessage();
            return resultExecMessage;
        }
    }
    return 0;
}


THUMB int status::StatusChange::getResultMessage1(Status status){
    int isEnable;
    int ResultMessage;
    
    if (status == StatusFizzleZone) {
        isEnable = statusFizzleZone_.isEnable();
        if (isEnable != 0) {
            ResultMessage = statusFizzleZone_.getResultMessage();
            return ResultMessage;
        }
    }
    else {
        isEnable = this->status_[status].isEnable();
        if (isEnable != 0) {
            ResultMessage = this->status_[status].getResultMessage();
            return ResultMessage;
        }
    }
    return 0;
}

THUMB int status::StatusChange::getResultMessage2(Status status, CharacterType type){
    int result;
    
    if (status == StatusFizzleZone) {
        result = statusFizzleZone_.isEnable();
        if (result != 0) {
            if (type == PLAYER) {
                result = statusFizzleZone_.getResultPlayerMessage();
                return result;
            }
            if (type == MONSTER) {
                result = statusFizzleZone_.getResultMonsterMessage();
                return result;
            }
        }
    }
    else {
        result = this->status_[status].isEnable();
        if (result != 0) {
            if (type == PLAYER) {
                result = this->status_[status].getResultPlayerMessage();
                return result;
            }
            if (type == MONSTER) {
                result = this->status_[status].getResultMonsterMessage();
                return result;
            }
        }
    }
    return 0;
}

THUMB int status::StatusChange::getResultMessage3(Status status, CharacterType type){
    int result;
    
    if (status == StatusFizzleZone) {
        if (type == PLAYER) {
            result = statusFizzleZone_.getResultPlayerMessage();
            return result;
        }
        if (type == MONSTER) {
            result = statusFizzleZone_.getResultMonsterMessage();
            return result;
        }
    }
    else {
        if (type == PLAYER) {
            result = this->status_[status].getResultPlayerMessage();
            return result;
        }
        if (type == MONSTER) {
            result = this->status_[status].getResultMonsterMessage();
            return result;
        }
    }
    return 0;
}

THUMB int status::StatusChange::getAgainEnableMessage(Status status) {
    if (status == StatusFizzleZone) {
        return statusFizzleZone_.getAgainEnableMessage();
    }
    return this->status_[status].getAgainEnableMessage();
}

THUMB int status::StatusChange::getAgainDisableMessage(Status status) {
    return this->status_[status].getAgainDisableMessage();
}

THUMB int status::StatusChange::getReleaseMessage() {
    int i = 0;
    status::StatusChangeOne *ptr;
    int isEnable;
    int isReleaseMessage;
    ptr = this->status_;
    for (; i < 35; i++, ptr++) {
        isEnable = ptr->isRelease();
        if (isEnable != 0) {
            this->status_[i].setRelease(0);
            isReleaseMessage = this->status_[i].getReleaseMessage();
            return isReleaseMessage;
        }
    }
    return 0;
}

THUMB void status::StatusChange::setupFizzleZone()
{
    statusFizzleZone_.setup(0x28, true);
    return;
}

THUMB void status::StatusChange::print()
{
    this->status_[0xc].isEnable();
    this->status_[0xd].isEnable();
    this->status_[0xe].isEnable();
    this->status_[0xf].isEnable();
    this->status_[0xb].isEnable();
    this->status_[0x12].isEnable();
    this->status_[0x8].isEnable();
    this->status_[0x10].isEnable();
    this->status_[0x19].isEnable();
    this->status_[0x18].isEnable();
    this->status_[0x16].isEnable();
    this->status_[0xa].isEnable();
    this->status_[0x9].isEnable();
    this->status_[0x1b].isEnable();
    this->status_[0x15].isEnable();
    this->status_[0x11].isEnable();
    this->status_[0x14].isEnable();
    this->status_[0x13].isEnable();
    this->status_[0x1a].isEnable();
    this->status_[0x1e].isEnable();
    this->status_[0x17].isEnable();
    this->isEnable(StatusFizzleZone);
    return;
}
