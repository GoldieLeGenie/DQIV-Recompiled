#include "main/status/HaveAction.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/HaveStatus.hpp"

int status::HaveAction::mode_;


THUMB status::HaveAction::HaveAction() {
    clear();
}



THUMB status::HaveAction::~HaveAction()

{
  return;
}

THUMB void status::HaveAction::setBattleMode()
{
  status::HaveAction::mode_ = 1;
}


THUMB int status::HaveAction::isBattleMode()
{
  return status::HaveAction::mode_ == 1;
}

THUMB void status::HaveAction::setTownMode()
{
  status::HaveAction::mode_ = 0;
}


THUMB int status::HaveAction::isTownMode()
{
  return status::HaveAction::mode_ == 0;
}


THUMB status::HaveAction::ActionMode status::HaveAction::getActionMode()
{
  return (status::HaveAction::ActionMode)status::HaveAction::mode_;
}



THUMB void status::HaveAction::setActionMode(status::HaveAction::ActionMode md)
{
  status::HaveAction::mode_ = md;
}


THUMB void status::HaveAction::setup(int index) {
    param::CharInitData* charaData;
    
    clear();
    
    charaData = excelParam.getCharaInitData();
 
    add(charaData[index].battle1, charaData[index].magic1);
    add(charaData[index].battle2, charaData[index].magic2);
    add(charaData[index].battle3, charaData[index].magic3);
    add(charaData[index].battle4, charaData[index].magic4);
    add(charaData[index].battle5, charaData[index].magic5);
    add(charaData[index].battle6, charaData[index].magic6);
    add(charaData[index].battle7, charaData[index].magic7);
    add(charaData[index].battle8, charaData[index].magic8);
    add(charaData[index].battle9, charaData[index].magic9);
    add(charaData[index].battle10, charaData[index].magic10);
    add(charaData[index].battle11, charaData[index].magic11);
    add(charaData[index].battle12, charaData[index].magic12);
    add(charaData[index].battle13, charaData[index].magic13);
    
    forceLearn();
    return;
}


THUMB void status::HaveAction::clear() {
    status::Action* p = this->newAction_;
    for (int i = 0; i < 24; i++, p++) {
        p->clear();
    }
    return;
}


THUMB void status::HaveAction::levelup(int index, int level, int wisdom, int flag) {
    LevelEntry* levelData = (LevelEntry*)HaveStatus::getLevelupInfo(index);
    unsigned short battleAction = levelData[level].battleAction;
    unsigned short usuallyAction = levelData[level].usuallyAction;
    if (battleAction == 0 && usuallyAction != 0) {
        battleAction = usuallyAction;
    }
    if (battleAction != 0 && usuallyAction == 0) {
        usuallyAction = battleAction;
    }
    if ((usuallyAction != 0 || battleAction != 0) && isActionAlreadyLearned(usuallyAction) == 0) {
        int i = 0;
        status::Action* p = this->newAction_;
        do {
            if (p->isEnable() == 0) {
                unsigned char type = levelData[level].actionType;
                if (type == 1) {
                    this->newAction_[i].setup(usuallyAction, battleAction, level, 1);
                }
                else if (type == 2) {
                    this->newAction_[i].setup(usuallyAction, battleAction, level, 1);
                }
                break;
            }
            i++;
            p++;
        } while (i < 24);
    }
    if (flag != 0) {
        learn(level, wisdom);
    }
}

THUMB void status::HaveAction::add(int battleAction, int usuallyAction) {
    int i = 0;
    status::Action* p = this->newAction_;
    for (i; i < 24; i++, p++) {
        if (p->isEnable() == 0) {
            this->newAction_[i].add(battleAction, usuallyAction);
            return;
        }
    }
}


THUMB int status::HaveAction::learn(int level, int wisdom) {
    int i = 0;
    status::Action* p = this->newAction_;
    for (; i < 24; i++, p++) {
        p->clearRemembering();
    }
    i = 0;
    p = this->newAction_;
    for (; i < 24; i++, p++) {
        if (p->isEnable() != 0 && p->isRemember() == 0) {
            getLearnTypeA(i, level, wisdom);
        }
    }
    return 0;
}


THUMB void status::HaveAction::getLearnTypeA(int index, int level, int wisdom) {
    this->newAction_[index].learnTypeA(level, wisdom);
}



THUMB int status::HaveAction::getAction(int index) {
    if (mode_ == 0) {
        if (this->newAction_[index].getUsuallyAction() != 0) {
            return this->newAction_[index].getUsuallyAction();
        }
        return this->newAction_[index].getAction();
    }
    return this->newAction_[index].getAction();
}


THUMB int status::HaveAction::getHaveAction(int index) {
    return getAction(index);
}


THUMB int status::HaveAction::getCount() {
    int count = 0;
    int i = 0;
    status::Action* p = this->newAction_;
    for (; i < 24; p++, i++) {
        if (p->isRemember() != 0) {
            count++;
        }
    }
    return count;
}

THUMB int status::HaveAction::getAllCount() {
    int count = 0;
    int i = count;
    status::Action* p = this->newAction_;
    for (; i < 24; p++,i++) {
        if (p->isEnable() != 0) {
            count++;
        }
    }
    return count;
}


THUMB int status::HaveAction::isAction(int actionIndex) {
    int count = getCount();
    for (int i = 0; i < count; i++) {
        if (actionIndex == getAction(i)) {
            return 1;
        }
    }
    return 0;
}

THUMB int status::HaveAction::isActionAlreadyLearned(int actionIndex) {
    for (int i = 0; i < getAllCount(); i++) {
        if (actionIndex == getHaveAction(i)) {
            return 1;
        }
    }
    return 0;
}


THUMB int status::HaveAction::getRememberingAction() {
    int i = 0;
    status::Action* p = this->newAction_;
    for (i; i < 24;  p++ ,i++) {
        if (p->isRemembering() != 0) {
            this->newAction_[i].clearRemembering();
            return getAction(i);
        }
    }
    return 0;
}


THUMB void status::HaveAction::forceLearn() {
    status::Action* p = this->newAction_;
    for (int i = 0; i < 24; i++, p++) {
        p->remember();
    }
}

