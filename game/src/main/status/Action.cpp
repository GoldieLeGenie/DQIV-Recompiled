#include "main/status/Action.hpp"
#include "main/dss/Random.hpp"


THUMB status::Action::Action()

{
  clear();
  return;
}


THUMB void status::Action::setup(int battleAction, int usuallyAction, int actionLevel, int actionFlag)
{
    this->battleAction_ = battleAction;
    this->usuallyAction_ = usuallyAction;
    this->actionLevel_ = actionLevel;
    this->actionFlag_ = actionFlag;
}

THUMB void status::Action::clear() {
    this->battleAction_ = 0;
    this->usuallyAction_ = 0;
    this->actionLevel_ = 0;
    this->actionFlag_ = 0;
}

THUMB int status::Action::isEnable() {
    if ((this->battleAction_ != 0) || (this->usuallyAction_ != 0)) {
        return 1;
    }
    return 0;
}

THUMB void status::Action::add(int battleAction, int usuallyAction) {
    this->battleAction_ = battleAction;
    this->usuallyAction_ = usuallyAction;
    this->actionFlag_ = 4 | this->actionFlag_;
}


THUMB int status::Action::getAction()
{
  return this->battleAction_;
}


THUMB int status::Action::getUsuallyAction()
{
  return this->usuallyAction_;
}


THUMB void status::Action::remember() {
    if (isEnable() != 0) {
        this->actionFlag_ = 4;
    }
}

THUMB int status::Action::isRemember() {
    if ((isEnable()) && (4 & this->actionFlag_)) {
        return 1;
    }
    return 0;
}

THUMB int status::Action::isRemembering() {
    if ((isEnable()) && (8 & this->actionFlag_)) {
        return 1;
    }
    return 0;
}


THUMB void status::Action::clearRemembering() {
    unsigned char flag = this->actionFlag_;
    if (8 & flag) {
        this->actionFlag_ = flag & ~8;
    }
}




THUMB void status::Action::learnTypeA(int actionIndex, int characterLevel)
{
    unsigned char currentFlags;
    int randomValue;
    int currentActionLevel;

    currentFlags = this->actionFlag_;

    if ((currentFlags & 1) != 0) {

        if (characterLevel <= 0xF) {
            currentActionLevel = this->actionLevel_;

            if (currentActionLevel + 1 == actionIndex) {
                randomValue = dssrand::rand(2);
                if (randomValue != 0) {
                    this->actionFlag_ = this->actionFlag_ | 0xC;
                    return;
                }
            }
            else if (currentActionLevel + 2 == actionIndex) {
                this->actionFlag_ = currentFlags | 0xC;
                return;
            }
        }
        else {
            if (0x28 <= characterLevel) {
                this->actionFlag_ = currentFlags | 0xC;
                return;
            }

            currentActionLevel = this->actionLevel_;

            if (currentActionLevel == actionIndex) {
                randomValue = dssrand::rand(2);
                if (randomValue != 0) {
                    this->actionFlag_ = this->actionFlag_ | 0xC;
                    return;
                }
            }
            else if ((currentActionLevel + 1) <= actionIndex) {
                this->actionFlag_ = currentFlags | 0xC;
            }
        }
    }
    return;
}