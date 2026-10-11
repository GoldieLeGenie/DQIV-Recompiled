#include "main/status/StatusChangeOne.hpp"
#include "main/dss/Random.hpp"

THUMB status::StatusChangeOne::StatusChangeOne() {
    this->flag_.flag_ = 0;
    this->damageType_ = 0;
    this->count_ = '\0';
    this->turn_ = '\0';
    this->pattern_ = '\0';
    this->astron_ = '\0';
    this->flag_.flag_ = 0;

}

THUMB status::StatusChangeOne::~StatusChangeOne() {
    return;
}

THUMB void status::StatusChangeOne::setup(int damageType, int flag) {
    clear();
    this->flag_.flag_ = this->flag_.flag_ | 1;
    this->damageType_ = damageType;
    this->count_ = '\0';
    this->turn_ = excelParam.abreactTurn_[damageType].turn;
    this->pattern_ = excelParam.abreactTurn_[damageType].pattern;
    if (flag) {
        this->turn_ = this->turn_ + addTurn();
    }
    return;
}


THUMB void status::StatusChangeOne::cleanup() {
    clear();
    return;
}


THUMB void status::StatusChangeOne::clear()
{
    this->flag_.flag_ = 0;
    this->count_ = '\0';
    this->turn_ = '\0';
    this->pattern_ = '\0';
    return;
}


THUMB int status::StatusChangeOne::addTurn() {
    char pattern;

    pattern = this->pattern_;
    switch (pattern) {
    case 0:
        return 0;
    case 1:
        return dssrand::rand(1) + 1;
    case 2:
        if (dssrand::rand(8) <= 0) {
            return 1;
        }
        if (dssrand::rand(8) <= 2) {
            return 2;
        }
        if (dssrand::rand(8) <= 4) {
            return 3;
        }
        dssrand::rand(8);
        return 4;
    case 3:
        if (dssrand::rand(8) <= 2) {
            return 1;
        }
        if (dssrand::rand(8) <= 4) {
            return 2;
        }
        if (dssrand::rand(8) <= 6) {
            return 3;
        }
        dssrand::rand(8);
        return 4;
    case 4:
        if (dssrand::rand(8) <= 4) {
            return 1;
        }
        if (dssrand::rand(8) <= 5) {
            return 2;
        }
        if (dssrand::rand(8) <= 6) {
            return 3;
        }
        dssrand::rand(8);
        return 4;
    case 5:
        return 1;
    default:
        return 0;
    }
}


THUMB int status::StatusChangeOne::isCancel() {
    int result = 1;

    if ((this->flag_.flag_ & 1) == 0) {
        return 0;
    }
    if (this->count_ < this->turn_) {
        result = 0;
    }
    return result;
}

THUMB void status::StatusChangeOne::execStartOfTurn() {
    if (((this->flag_.flag_ & 1) != 0) && ((char)(excelParam.abreactTurn_[this->damageType_].byte_1 & 1) != 0)) {
        this->count_ = this->count_ + '\x01';
    }
    return;
}

THUMB void status::StatusChangeOne::execEndOfTurn() {
    if ((this->flag_.flag_ & 1) != 0) {
        if ((int)((excelParam.abreactTurn_[this->damageType_].byte_1 & 2) << 23 >> 24) != 0) {
            this->count_ = this->count_ + '\x01';
        }
    }
    return;
}

THUMB void status::StatusChangeOne::execEndOfRound() {
    if ((this->flag_.flag_ & 1) != 0) {
        if ((int)((excelParam.abreactTurn_[this->damageType_].byte_1 & 4) << 22 >> 24) != 0) {
            this->count_ = this->count_ + '\x01';
        }
        if (this->astron_ != 0) {
            this->count_ = this->count_ + '\x01';
        }
    }
    return;
}

THUMB void status::StatusChangeOne::setEnable(int flag) {
    if (flag) {
        this->flag_.flag_ |= 1;
    } else {
        this->flag_.flag_ &= ~1;
    }
}

THUMB int status::StatusChangeOne::isEnable() {
    return (this->flag_.flag_ & 1) != 0;
}

THUMB void status::StatusChangeOne::setRelease(bool flag)
{
    if (flag) {
        this->flag_.flag_ = this->flag_.flag_ | 2;
        return;
    }
    else {
        this->flag_.flag_ = this->flag_.flag_ & 0xfffffffd;
        return;
    }
    return;
}

THUMB bool status::StatusChangeOne::isRelease() {
    if ((this->flag_.flag_ & 2) != 0) {
        return 1;
    }
    return 0;
}

THUMB int status::StatusChangeOne::getExecMessage()
{
    return excelParam.abreactTurn_[this->damageType_].msg1;
}


THUMB int status::StatusChangeOne::getResultMessage()
{
    return excelParam.abreactTurn_[this->damageType_].msg2;
}

THUMB int status::StatusChangeOne::getResultPlayerMessage() {
    return excelParam.abreactTurn_[this->damageType_].msg3;
}

THUMB int status::StatusChangeOne::getResultMonsterMessage() {
    return excelParam.abreactTurn_[this->damageType_].msg4;
}

THUMB int status::StatusChangeOne::getAgainEnableMessage() {
    return excelParam.abreactTurn_[this->damageType_].msg5;
}

THUMB int status::StatusChangeOne::getAgainDisableMessage()
{
    return excelParam.abreactTurn_[this->damageType_].msg6;
}

THUMB int status::StatusChangeOne::getReleaseMessage() {
    return excelParam.abreactTurn_[this->damageType_].msg7;
}

THUMB bool status::StatusChangeOne::isValidOnAstron(int damageType) {
    if ((char)((excelParam.abreactTurn_[damageType].byte_1 & 0x20) >> 5) != 1) {
        return 1;
    }
    return 0;
}

THUMB bool status::StatusChangeOne::isValidOnSpazz(int damageType) {
    if ((int)((excelParam.abreactTurn_[damageType].byte_1 & 0x40) << 18 >> 24) != 1) {
        return 1;
    }
    return 0;
}


THUMB bool status::StatusChangeOne::isAfterOnSleep(int damageType) {
    if ((char)(excelParam.abreactTurn_[damageType].byte_2 & 1) != 1) {
        return 1;
    }
    return 0;
}



THUMB bool status::StatusChangeOne::isAfterOnPath1(int damageType) {
    if ((int)((excelParam.abreactTurn_[damageType].byte_2 & 2) << 23 >> 24) != 1) {
        return 1;
    }
    return 0;
}

THUMB bool status::StatusChangeOne::isAfterOnConfuze(int damageType) {
    if ((int)((excelParam.abreactTurn_[damageType].byte_2 & 4) << 22 >> 24) != 1) {
        return 1;
    }
    return 0;
}











