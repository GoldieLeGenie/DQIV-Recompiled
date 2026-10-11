#include "ov003/btl/BattleSelectTargetParam.hpp"


THUMB btl::BattleSelectTargetParam::BattleSelectTargetParam()
{
  clear();
}

THUMB btl::BattleSelectTargetParam::~BattleSelectTargetParam()
{
}

THUMB void btl::BattleSelectTargetParam::clear()
{
    for (int i = 0; i < 18; i++) {
        sourceCharacterStatus[i] = 0;
    }

    for (int i = 0; i < 18; i++) {
        targetCharacterStatus[i] = 0;
    }

    sourceCount_ = 0;
    targetCount_ = 0;
    actionIndex_ = 0;
    targetGroup_ = -1;
    targetIndex_ = -1;
    callTarget_ = StartRound;
}

THUMB void btl::BattleSelectTargetParam::setSourceCharacterStatus(int index,status::CharacterStatus *characterStatus)
{
  sourceCharacterStatus[index] = characterStatus;
}

THUMB status::CharacterStatus * btl::BattleSelectTargetParam::getSourceCharacterStatus(int index)
{
  return sourceCharacterStatus[index];
}

THUMB int btl::BattleSelectTargetParam::getSourceCountForGroup(int group)
{
    int count = 0;
    
    for (int i = 0; i < this->sourceCount_; i++) {
        status::CharacterStatus* status = this->sourceCharacterStatus[i];
        
        if ( group == status->characterGroup_) {
            if (!status->haveStatusInfo_.isDeath()) {
                count++;
            }
        }
    }
    
    return count;
}

THUMB status::CharacterStatus * btl::BattleSelectTargetParam::getSourceCharacterStatusForGroup(int group,int index)
{
    int count;
    int i;

    count = 0;

    for (i = 0; i < sourceCount_; i++) {
        status::CharacterStatus *status = sourceCharacterStatus[i];

        if (group == status->characterGroup_) {
            if (status->haveStatusInfo_.isDeath() == 0) {
                if (count == index) {
                    return sourceCharacterStatus[i];
                }

                count++;
            }
        }
    }

    return 0;
}                                                           

THUMB int btl::BattleSelectTargetParam::getSourceCountForGroupDead(int group)
{
    int count;
    int i;

    count = 0;

    for (i = 0; i < sourceCount_; i++) {
        if (sourceCharacterStatus[i]->characterGroup_  == group) {
            count++;
        }
    }

    return count;
}

THUMB status::CharacterStatus * btl::BattleSelectTargetParam::getSourceCharacterStatusForGroupDead(int group,int index)
{
    int count;
    int i;

    count = 0;
    i = 0;
    while (i < sourceCount_) {
        if (group == sourceCharacterStatus[i]->characterGroup_) {
            if (count == index)
                return sourceCharacterStatus[i];
            count++;
        }

        i++;
    }

    return 0;
}

THUMB void btl::BattleSelectTargetParam::setTargetCharacterStatus(int index,status::CharacterStatus *characterStatus)
{
  targetCharacterStatus[index] = characterStatus;
}

THUMB status::CharacterStatus * btl::BattleSelectTargetParam::getTargetCharacterStatus(int index)
{
  return this->targetCharacterStatus[index];
}