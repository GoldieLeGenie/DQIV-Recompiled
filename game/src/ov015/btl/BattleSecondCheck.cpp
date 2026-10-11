#include "ov003/btl/BattleMonster.hpp"
#include "main/dss/DssUtils.hpp"
#include "ov015/btl/BattleSecondCheck.hpp"
#include "main/status/UseAction.hpp"
#include "ov003/status/MonsterParty.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/HaveStatus.hpp"

#pragma profile on

THUMB bool btl::BattleSecondCheck::personalCheckRandom(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::CharacterStatus* targetCharacterStatus[18] = {0};
    unsigned char targetCharacterGroup[18] = {0};
    unsigned char targetCharacterIndex[18] = {0};
    int count;
    int g;
    int otherCount;
    int group;
    int validCount;
    int selected;

    for (int i = 0; i < 18; i++) {
        targetCharacterStatus[i] = 0;
        targetCharacterGroup[i] = 0;
        targetCharacterIndex[i] = 0;
    }

    group = battleSelectTargetParam->targetGroup_;
    count = battleSelectTargetParam->getSourceCountForGroup(group);
    validCount = 0;

    for (int i = 0; i < count; i++) {
        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(group, i);
        targetCharacterGroup[validCount] = group;
        targetCharacterIndex[validCount] = i;
        validCount++;
    }

    if (validCount == 0 || battleSelectTargetParam->actorCharacterStatus_->characterType_ == MONSTER) {
        for (g = 0; g < 4; g++) {
            if (group != g) {
                otherCount = battleSelectTargetParam->getSourceCountForGroup(g);

                if (otherCount > 0) {
                    for (int i = 0; i < otherCount; i++) {
                        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(g, i);
                        targetCharacterGroup[validCount] = g;
                        targetCharacterIndex[validCount] = i;
                        validCount++;
                    }
                }
            }
        }
    }

    if (validCount == 0)
        return false;

    selected = dssrand::rand(validCount);

    battleSelectTargetParam->setTargetCharacterStatus(0, targetCharacterStatus[selected]);
    battleSelectTargetParam->targetCount_ = 1;
    battleSelectTargetParam->targetGroup_ = targetCharacterGroup[selected];
    battleSelectTargetParam->targetIndex_ = targetCharacterIndex[selected];

    return true;
}

THUMB bool btl::BattleSecondCheck::personalCheckMahokanta(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::CharacterStatus* targetCharacterStatus[18] = {0};
    unsigned char targetCharacterGroup[18] = {0};
    unsigned char targetCharacterIndex[18] = {0};
    int count;
    int g;
    int otherCount;
    int group;
    int validCount;
    int selected;

    for (int i = 0; i < 18; i++) {
        targetCharacterStatus[i] = 0;
        targetCharacterGroup[i] = 0;
        targetCharacterIndex[i] = 0;
    }

    group = battleSelectTargetParam->targetGroup_;
    count = battleSelectTargetParam->getSourceCountForGroup(group);
    validCount = 0;

    for (int i = 0; i < count; i++) {
        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(group, i);
        targetCharacterGroup[validCount] = group;
        targetCharacterIndex[validCount] = i;

        status::StatusChange* statusChange = &targetCharacterStatus[validCount]->haveStatusInfo_.statusChange_;

        if (statusChange->isEnable(status::StatusChange::StatusMahokanta) == 0 &&
            statusChange->isEnable(status::StatusChange::StatusMahosute) == 0)
            validCount++;
    }

    if (validCount == 0 || battleSelectTargetParam->actorCharacterStatus_->characterType_ == MONSTER) {
        for (g = 0; g < 4; g++) {
            if (group != g) {
                otherCount = battleSelectTargetParam->getSourceCountForGroup(g);

                if (otherCount > 0) {
                    for (int i = 0; i < otherCount; i++) {
                        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(g, i);
                        targetCharacterGroup[validCount] = g;
                        targetCharacterIndex[validCount] = i;

                        status::StatusChange* statusChange = &targetCharacterStatus[validCount]->haveStatusInfo_.statusChange_;

                        if (statusChange->isEnable(status::StatusChange::StatusMahokanta) == 0 &&
                            statusChange->isEnable(status::StatusChange::StatusMahosute) == 0)
                            validCount++;
                    }
                }
            }
        }
    }

    if (validCount == 0)
        return false;

    selected = dssrand::rand(validCount);

    battleSelectTargetParam->setTargetCharacterStatus(0, targetCharacterStatus[selected]);
    battleSelectTargetParam->targetCount_ = 1;
    battleSelectTargetParam->targetGroup_ = targetCharacterGroup[selected];
    battleSelectTargetParam->targetIndex_ = targetCharacterIndex[selected];

    return true;
}

THUMB bool btl::BattleSecondCheck::personalCheckRarihoma(BattleSelectTargetParam *battleSelectTargetParam)
{
    return personalCheckNormalCharacter(battleSelectTargetParam, status::StatusChange::StatusSleep);
}

THUMB bool btl::BattleSecondCheck::personalCheckMedapani(BattleSelectTargetParam *battleSelectTargetParam)
{
    return personalCheckNormalCharacter(battleSelectTargetParam, status::StatusChange::StatusConfusion);
}

THUMB bool btl::BattleSecondCheck::personalCheckBothOne(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::CharacterStatus* targetCharacterStatus[18];
    status::CharacterStatus* actor;
    int count;
    int i;
    int selected;
    int group;

    for (i = 0; i < 18; i++)
        targetCharacterStatus[i] = 0;

    count = 0;

    for (i = 0; i < battleSelectTargetParam->sourceCount_; i++) {
        actor = battleSelectTargetParam->actorCharacterStatus_;

        if (battleSelectTargetParam->getSourceCharacterStatus(i) != actor) {
            targetCharacterStatus[count] = battleSelectTargetParam->getSourceCharacterStatus(i);
            count++;
        }
    }

    selected = dssrand::rand(count);

    battleSelectTargetParam->setTargetCharacterStatus(0, targetCharacterStatus[selected]);
    battleSelectTargetParam->targetCount_ = 1;

    group = targetCharacterStatus[selected]->characterGroup_;
    battleSelectTargetParam->targetGroup_ = group;

    for (i = 0; i < battleSelectTargetParam->getSourceCountForGroup(group); i++) {
        if (battleSelectTargetParam->getSourceCharacterStatusForGroup(group, i) == targetCharacterStatus[selected])
            battleSelectTargetParam->targetIndex_ = i;
    }

    return true;
}

THUMB bool btl::BattleSecondCheck::personalCheckMahotora(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::CharacterStatus* targetCharacterStatus[18] = {0};
    unsigned char targetCharacterGroup[18] = {0};
    unsigned char targetCharacterIndex[18] = {0};
    int count;
    int g;
    int otherCount;
    int group;
    int validCount;
    int selected;

    for (int i = 0; i < 18; i++) {
        targetCharacterStatus[i] = 0;
        targetCharacterGroup[i] = 0;
        targetCharacterIndex[i] = 0;
    }

    group = battleSelectTargetParam->targetGroup_;
    count = battleSelectTargetParam->getSourceCountForGroup(group);
    validCount = 0;

    for (int i = 0; i < count; i++) {
        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(group, i);
        targetCharacterGroup[validCount] = group;
        targetCharacterIndex[validCount] = i;

        if (targetCharacterStatus[validCount]->haveStatusInfo_.haveStatus_.getMp() != 0)
            validCount++;
    }

    if (validCount == 0 || battleSelectTargetParam->actorCharacterStatus_->characterType_ == MONSTER) {
        for (g = 0; g < 4; g++) {
            if (group != g) {
                otherCount = battleSelectTargetParam->getSourceCountForGroup(g);

                if (otherCount > 0) {
                    for (int i = 0; i < otherCount; i++) {
                        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(g, i);
                        targetCharacterGroup[validCount] = g;
                        targetCharacterIndex[validCount] = i;

                        if (targetCharacterStatus[validCount]->haveStatusInfo_.haveStatus_.getMp() != 0)
                            validCount++;
                    }
                }
            }
        }
    }

    if (validCount == 0)
        return false;

    selected = dssrand::rand(validCount);

    battleSelectTargetParam->setTargetCharacterStatus(0, targetCharacterStatus[selected]);
    battleSelectTargetParam->targetCount_ = 1;
    battleSelectTargetParam->targetGroup_ = targetCharacterGroup[selected];
    battleSelectTargetParam->targetIndex_ = targetCharacterIndex[selected];

    return true;
}

THUMB bool btl::BattleSecondCheck::personalCheckRukani(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::CharacterStatus* targetCharacterStatus[18] = {0};
    unsigned char targetCharacterGroup[18] = {0};
    unsigned char targetCharacterIndex[18] = {0};
    int count;
    int g;
    int otherCount;
    int group;
    int validCount;
    int selected;

    for (int i = 0; i < 18; i++) {
        targetCharacterStatus[i] = 0;
        targetCharacterGroup[i] = 0;
        targetCharacterIndex[i] = 0;
    }

    group = battleSelectTargetParam->targetGroup_;
    count = battleSelectTargetParam->getSourceCountForGroup(group);
    validCount = 0;

    for (int i = 0; i < count; i++) {
        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(group, i);
        targetCharacterGroup[validCount] = group;
        targetCharacterIndex[validCount] = i;

        if (targetCharacterStatus[validCount]->haveStatusInfo_.getDefence(0) != 0)
            validCount++;
    }

    if (validCount == 0 || battleSelectTargetParam->actorCharacterStatus_->characterType_ == MONSTER) {
        for (g = 0; g < 4; g++) {
            if (group != g) {
                otherCount = battleSelectTargetParam->getSourceCountForGroup(g);

                if (otherCount > 0) {
                    for (int i = 0; i < otherCount; i++) {
                        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(g, i);
                        targetCharacterGroup[validCount] = g;
                        targetCharacterIndex[validCount] = i;

                        if (targetCharacterStatus[validCount]->haveStatusInfo_.getDefence(0) != 0)
                            validCount++;
                    }
                }
            }
        }
    }

    if (validCount == 0)
        return false;

    selected = dssrand::rand(validCount);

    battleSelectTargetParam->setTargetCharacterStatus(0, targetCharacterStatus[selected]);
    battleSelectTargetParam->targetCount_ = 1;
    battleSelectTargetParam->targetGroup_ = targetCharacterGroup[selected];
    battleSelectTargetParam->targetIndex_ = targetCharacterIndex[selected];

    return true;
}

THUMB bool btl::BattleSecondCheck::personalCheckSukara(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::CharacterStatus* targetCharacterStatus[18] = {0};
    unsigned char targetCharacterGroup[18] = {0};
    unsigned char targetCharacterIndex[18] = {0};
    int count;
    int g;
    int otherCount;
    int group;
    int validCount;
    int selected;

    for (int i = 0; i < 18; i++) {
        targetCharacterStatus[i] = 0;
        targetCharacterGroup[i] = 0;
        targetCharacterIndex[i] = 0;
    }

    group = battleSelectTargetParam->targetGroup_;
    count = battleSelectTargetParam->getSourceCountForGroup(group);
    validCount = 0;

    for (int i = 0; i < count; i++) {
        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(group, i);
        targetCharacterGroup[validCount] = group;
        targetCharacterIndex[validCount] = i;

        if (targetCharacterStatus[validCount]->haveStatusInfo_.getDefence(0) != 999)
            validCount++;
    }

    if (validCount == 0 || battleSelectTargetParam->actorCharacterStatus_->characterType_ == MONSTER) {
        for (g = 0; g < 4; g++) {
            if (group != g) {
                otherCount = battleSelectTargetParam->getSourceCountForGroup(g);

                if (otherCount > 0) {
                    for (int i = 0; i < otherCount; i++) {
                        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(g, i);
                        targetCharacterGroup[validCount] = g;
                        targetCharacterIndex[validCount] = i;

                        if (targetCharacterStatus[validCount]->haveStatusInfo_.getDefence(0) != 999)
                            validCount++;
                    }
                }
            }
        }
    }

    if (validCount == 0)
        return false;

    selected = dssrand::rand(validCount);

    battleSelectTargetParam->setTargetCharacterStatus(0, targetCharacterStatus[selected]);
    battleSelectTargetParam->targetCount_ = 1;
    battleSelectTargetParam->targetGroup_ = targetCharacterGroup[selected];
    battleSelectTargetParam->targetIndex_ = targetCharacterIndex[selected];

    return true;
}

THUMB bool btl::BattleSecondCheck::personalCheckBaikiruto(BattleSelectTargetParam *battleSelectTargetParam)
{
    return personalCheckNormalCharacter(battleSelectTargetParam, status::StatusChange::StatusBaikiruto);
}

THUMB bool btl::BattleSecondCheck::personalCheckHoimi(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::CharacterStatus* targetCharacterStatus[18] = {0};
    unsigned char targetCharacterGroup[18] = {0};
    unsigned char targetCharacterIndex[18] = {0};
    int count;
    int g;
    int otherCount;
    int group;
    status::HaveStatus* haveStatus;
    status::HaveStatus* otherHaveStatus;
    int validCount;
    int selected;

    for (int i = 0; i < 18; i++) {
        targetCharacterStatus[i] = 0;
        targetCharacterGroup[i] = 0;
        targetCharacterIndex[i] = 0;
    }

    group = battleSelectTargetParam->targetGroup_;
    count = battleSelectTargetParam->getSourceCountForGroup(group);
    validCount = 0;

    for (int i = 0; i < count; i++) {
        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(group, i);
        targetCharacterGroup[validCount] = group;
        targetCharacterIndex[validCount] = i;

        haveStatus = &targetCharacterStatus[validCount]->haveStatusInfo_.haveStatus_;

        if ((int)haveStatus->getHp() <= (haveStatus->getHpMax() >> 1))
            validCount++;
    }

    if (validCount == 0 || battleSelectTargetParam->actorCharacterStatus_->characterType_ == MONSTER) {
        for (g = 0; g < 4; g++) {
            if (group != g) {
                otherCount = battleSelectTargetParam->getSourceCountForGroup(g);

                if (otherCount > 0) {
                    for (int i = 0; i < otherCount; i++) {
                        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(g, i);
                        targetCharacterGroup[validCount] = g;
                        targetCharacterIndex[validCount] = i;

                        otherHaveStatus = &targetCharacterStatus[validCount]->haveStatusInfo_.haveStatus_;

                        if ((int)otherHaveStatus->getHp() <= (otherHaveStatus->getHpMax() >> 1))
                            validCount++;
                    }
                }
            }
        }
    }

    if (validCount == 0)
        return false;

    selected = dssrand::rand(validCount);

    battleSelectTargetParam->setTargetCharacterStatus(0, targetCharacterStatus[selected]);
    battleSelectTargetParam->targetCount_ = 1;
    battleSelectTargetParam->targetGroup_ = targetCharacterGroup[selected];
    battleSelectTargetParam->targetIndex_ = targetCharacterIndex[selected];

    return true;
}


THUMB bool btl::BattleSecondCheck::personalCheckMyHoimi(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::HaveStatusInfo* info = &battleSelectTargetParam->actorCharacterStatus_->haveStatusInfo_;

    if ((int)info->haveStatus_.getHp() < (info->haveStatus_.getHpMax() >> 1))
        return personalCheckFreeOne(battleSelectTargetParam);

    return false;
}

THUMB bool btl::BattleSecondCheck::personalCheckThereHoimi(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::CharacterStatus* targetCharacterStatus[18] = {0};
    unsigned char targetCharacterGroup[18] = {0};
    unsigned char targetCharacterIndex[18] = {0};
    int count;
    int g;
    int otherCount;
    int group;
    status::HaveStatusInfo* info;
    status::HaveStatusInfo* otherInfo;
    int validCount;
    int selected;

    for (validCount = 0; validCount < 18; validCount++) {
        targetCharacterStatus[validCount] = 0;
        targetCharacterGroup[validCount] = 0;
        targetCharacterIndex[validCount] = 0;
    }

    group = battleSelectTargetParam->targetGroup_;
    count = battleSelectTargetParam->getSourceCountForGroup(group);
    validCount = 0;

    for (int i = 0; i < count; i++) {
        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(group, i);
        targetCharacterGroup[validCount] = group;
        targetCharacterIndex[validCount] = i;

        status::StatusChange* statusChange = &targetCharacterStatus[validCount]->haveStatusInfo_.statusChange_;

        if (statusChange->isEnable(status::StatusChange::StatusMahokanta) == 0 &&
            statusChange->isEnable(status::StatusChange::StatusMahosute) == 0) {
            if (group != battleSelectTargetParam->actorCharacterStatus_->characterGroup_ ||
                i != battleSelectTargetParam->actorCharacterStatus_->characterIndex_) {
                info = &targetCharacterStatus[validCount]->haveStatusInfo_;

                if ((int)info->haveStatus_.getHp() <= (info->haveStatus_.getHpMax() >> 1))
                    validCount++;
            }
        }
    }

    if (validCount == 0 || battleSelectTargetParam->actorCharacterStatus_->characterType_ == MONSTER) {
        for (g = 0; g < 4; g++) {
            if (group != g) {
                otherCount = battleSelectTargetParam->getSourceCountForGroup(g);

                if (otherCount > 0) {
                    for (int i = 0; i < otherCount; i++) {
                        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(g, i);
                        targetCharacterGroup[validCount] = g;
                        targetCharacterIndex[validCount] = i;

                        status::StatusChange* statusChange = &targetCharacterStatus[validCount]->haveStatusInfo_.statusChange_;

                        if (statusChange->isEnable(status::StatusChange::StatusMahokanta) == 0 &&
                            statusChange->isEnable(status::StatusChange::StatusMahosute) == 0) {
                            if (group != battleSelectTargetParam->actorCharacterStatus_->characterGroup_ ||
                                g != battleSelectTargetParam->actorCharacterStatus_->characterIndex_) {
                                otherInfo = &targetCharacterStatus[validCount]->haveStatusInfo_;

                                if ((int)otherInfo->haveStatus_.getHp() <= (otherInfo->haveStatus_.getHpMax() >> 1))
                                    validCount++;
                            }
                        }
                    }
                }
            }
        }
    }

    if (validCount == 0) {
        info = &battleSelectTargetParam->actorCharacterStatus_->haveStatusInfo_;

        if ((int)info->haveStatus_.getHp() <= (info->haveStatus_.getHpMax() >> 1))
            return personalCheckFreeOne(battleSelectTargetParam);

        return false;
    }

    selected = dssrand::rand(validCount);

    battleSelectTargetParam->setTargetCharacterStatus(0, targetCharacterStatus[selected]);
    battleSelectTargetParam->targetCount_ = 1;
    battleSelectTargetParam->targetGroup_ = targetCharacterGroup[selected];
    battleSelectTargetParam->targetIndex_ = targetCharacterIndex[selected];

    return true;
}

THUMB bool btl::BattleSecondCheck::personalCheckZaoriku(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::CharacterStatus* targetCharacterStatus[18] = {0};
    unsigned char targetCharacterGroup[18] = {0};
    unsigned char targetCharacterIndex[18] = {0};
    int count;
    int g;
    int otherCount;
    int group;
    int validCount;
    int selected;

    for (validCount = 0; validCount < 18; validCount++) {
        targetCharacterStatus[validCount] = 0;
        targetCharacterGroup[validCount] = 0;
        targetCharacterIndex[validCount] = 0;
    }

    group = battleSelectTargetParam->targetGroup_;
    count = battleSelectTargetParam->getSourceCountForGroupDead(group);
    validCount = 0;

    for (int i = 0; i < count; i++) {
        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroupDead(group, i);
        targetCharacterGroup[validCount] = group;
        targetCharacterIndex[validCount] = i;

        status::HaveStatusInfo* info = &targetCharacterStatus[validCount]->haveStatusInfo_;

        if (info->isDeath() != 0 && info->isEscapeFlag() == 0 && info->isDisappearFlag() == 0)
            validCount++;
    }

    if (validCount == 0 || battleSelectTargetParam->actorCharacterStatus_->characterType_ == MONSTER) {
        for (g = 0; g < 4; g++) {
            if (group != g) {
                otherCount = battleSelectTargetParam->getSourceCountForGroupDead(g);

                if (otherCount > 0) {
                    for (int i = 0; i < otherCount; i++) {
                        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroupDead(g, i);
                        targetCharacterGroup[validCount] = g;
                        targetCharacterIndex[validCount] = i;

                        status::HaveStatusInfo* info = &targetCharacterStatus[validCount]->haveStatusInfo_;

                        if (info->isDeath() != 0 && info->isEscapeFlag() == 0 && info->isDisappearFlag() == 0)
                            validCount++;
                    }
                }
            }
        }
    }

    if (validCount == 0)
        return false;

    if (battleSelectTargetParam->actorCharacterStatus_->characterType_ == MONSTER) {
        selected = -1;

        for (int i = 0; i < validCount; i++) {
            if (btl::BattleMonsterDraw2::getSingleton()->isCallFriend(targetCharacterStatus[i]->characterIndex_) != 0) {
                selected = i;
                break;
            }
        }

        if (selected == -1)
            return false;
    } else {
        selected = dssrand::rand(validCount);
    }

    battleSelectTargetParam->setTargetCharacterStatus(0, targetCharacterStatus[selected]);
    battleSelectTargetParam->targetCount_ = 1;
    battleSelectTargetParam->targetGroup_ = targetCharacterGroup[selected];
    battleSelectTargetParam->targetIndex_ = targetCharacterIndex[selected];

    return true;
}


THUMB bool btl::BattleSecondCheck::personalCheckTaiatari(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::HaveStatus* haveStatus = &battleSelectTargetParam->actorCharacterStatus_->haveStatusInfo_.haveStatus_;

    int hpMax = haveStatus->getHpMax();

    if ((hpMax >> 1) <= (int)haveStatus->getHp())
        return false;

    return btl::BattleSecondCheck::personalCheckRandom(battleSelectTargetParam);
}

THUMB bool btl::BattleSecondCheck::personalCheckRarihomaForGod(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::CharacterStatus* targetCharacterStatus[18] = {0};
    unsigned char targetCharacterGroup[18] = {0};
    unsigned char targetCharacterIndex[18] = {0};
    int count;
    int g;
    int otherCount;
    int group;
    int validCount;
    int selected;

    for (int i = 0; i < 18; i++) {
        targetCharacterStatus[i] = 0;
        targetCharacterGroup[i] = 0;
        targetCharacterIndex[i] = 0;
    }

    group = battleSelectTargetParam->targetGroup_;
    count = battleSelectTargetParam->getSourceCountForGroup(group);
    validCount = 0;

    for (int i = 0; i < count; i++) {
        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(group, i);
        targetCharacterGroup[validCount] = group;
        targetCharacterIndex[validCount] = i;

        status::StatusChange* statusChange = &targetCharacterStatus[validCount]->haveStatusInfo_.statusChange_;

        if (statusChange->isEnable(status::StatusChange::StatusSleep) == 0 &&
            statusChange->isEnable(status::StatusChange::StatusMahokanta) == 0 &&
            statusChange->isEnable(status::StatusChange::StatusMahosute) == 0)
            validCount++;
    }

    if (validCount == 0 || battleSelectTargetParam->actorCharacterStatus_->characterType_ == MONSTER) {
        for (g = 0; g < 4; g++) {
            if (group != g) {
                otherCount = battleSelectTargetParam->getSourceCountForGroup(g);

                if (otherCount > 0) {
                    for (int i = 0; i < otherCount; i++) {
                        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(g, i);
                        targetCharacterGroup[validCount] = g;
                        targetCharacterIndex[validCount] = i;

                        status::StatusChange* statusChange = &targetCharacterStatus[validCount]->haveStatusInfo_.statusChange_;

                        if (statusChange->isEnable(status::StatusChange::StatusSleep) == 0 &&
                            statusChange->isEnable(status::StatusChange::StatusMahokanta) == 0 &&
                            statusChange->isEnable(status::StatusChange::StatusMahosute) == 0)
                            validCount++;
                    }
                }
            }
        }
    }

    if (validCount == 0)
        return false;

    selected = dssrand::rand(validCount);

    battleSelectTargetParam->setTargetCharacterStatus(0, targetCharacterStatus[selected]);
    battleSelectTargetParam->targetCount_ = 1;
    battleSelectTargetParam->targetGroup_ = targetCharacterGroup[selected];
    battleSelectTargetParam->targetIndex_ = targetCharacterIndex[selected];

    return true;
}

THUMB bool btl::BattleSecondCheck::personalCheckMedapaniForGod(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::CharacterStatus* targetCharacterStatus[18] = {0};
    unsigned char targetCharacterGroup[18] = {0};
    unsigned char targetCharacterIndex[18] = {0};
    int count;
    int g;
    int otherCount;
    int group;
    int validCount;
    int selected;

    for (int i = 0; i < 18; i++) {
        targetCharacterStatus[i] = 0;
        targetCharacterGroup[i] = 0;
        targetCharacterIndex[i] = 0;
    }

    group = battleSelectTargetParam->targetGroup_;
    count = battleSelectTargetParam->getSourceCountForGroup(group);
    validCount = 0;

    for (int i = 0; i < count; i++) {
        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(group, i);
        targetCharacterGroup[validCount] = group;
        targetCharacterIndex[validCount] = i;

        status::StatusChange* statusChange = &targetCharacterStatus[validCount]->haveStatusInfo_.statusChange_;

        if (statusChange->isEnable(status::StatusChange::StatusConfusion) == 0 &&
            statusChange->isEnable(status::StatusChange::StatusMahokanta) == 0 &&
            statusChange->isEnable(status::StatusChange::StatusMahosute) == 0)
            validCount++;
    }

    if (validCount == 0 || battleSelectTargetParam->actorCharacterStatus_->characterType_ == MONSTER) {
        for (g = 0; g < 4; g++) {
            if (group != g) {
                otherCount = battleSelectTargetParam->getSourceCountForGroup(g);

                if (otherCount > 0) {
                    for (int i = 0; i < otherCount; i++) {
                        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(g, i);
                        targetCharacterGroup[validCount] = g;
                        targetCharacterIndex[validCount] = i;

                        status::StatusChange* statusChange = &targetCharacterStatus[validCount]->haveStatusInfo_.statusChange_;

                        if (statusChange->isEnable(status::StatusChange::StatusConfusion) == 0 &&
                            statusChange->isEnable(status::StatusChange::StatusMahokanta) == 0 &&
                            statusChange->isEnable(status::StatusChange::StatusMahosute) == 0)
                            validCount++;
                    }
                }
            }
        }
    }

    if (validCount == 0)
        return false;

    selected = dssrand::rand(validCount);

    battleSelectTargetParam->setTargetCharacterStatus(0, targetCharacterStatus[selected]);
    battleSelectTargetParam->targetCount_ = 1;
    battleSelectTargetParam->targetGroup_ = targetCharacterGroup[selected];
    battleSelectTargetParam->targetIndex_ = targetCharacterIndex[selected];

    return true;
}


THUMB bool btl::BattleSecondCheck::personalCheckMahotoraForGod(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::CharacterStatus* targetCharacterStatus[18] = {0};
    unsigned char targetCharacterGroup[18] = {0};
    unsigned char targetCharacterIndex[18] = {0};
    int count;
    int g;
    int otherCount;
    int group;
    int validCount;
    int selected;

    for (int i = 0; i < 18; i++) {
        targetCharacterStatus[i] = 0;
        targetCharacterGroup[i] = 0;
        targetCharacterIndex[i] = 0;
    }

    group = battleSelectTargetParam->targetGroup_;
    count = battleSelectTargetParam->getSourceCountForGroup(group);
    validCount = 0;

    for (int i = 0; i < count; i++) {
        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(group, i);
        targetCharacterGroup[validCount] = group;
        targetCharacterIndex[validCount] = i;

        status::StatusChange* statusChange = &targetCharacterStatus[validCount]->haveStatusInfo_.statusChange_;

        if (targetCharacterStatus[validCount]->haveStatusInfo_.haveStatus_.getMp() != 0 &&
            statusChange->isEnable(status::StatusChange::StatusMahokanta) == 0 &&
            statusChange->isEnable(status::StatusChange::StatusMahosute) == 0)
            validCount++;
    }

    if (validCount == 0 || battleSelectTargetParam->actorCharacterStatus_->characterType_ == MONSTER) {
        for (g = 0; g < 4; g++) {
            if (group != g) {
                otherCount = battleSelectTargetParam->getSourceCountForGroup(g);

                if (otherCount > 0) {
                    for (int i = 0; i < otherCount; i++) {
                        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(g, i);
                        targetCharacterGroup[validCount] = g;
                        targetCharacterIndex[validCount] = i;

                        status::StatusChange* statusChange = &targetCharacterStatus[validCount]->haveStatusInfo_.statusChange_;

                        if (targetCharacterStatus[validCount]->haveStatusInfo_.haveStatus_.getMp() != 0 &&
                            statusChange->isEnable(status::StatusChange::StatusMahokanta) == 0 &&
                            statusChange->isEnable(status::StatusChange::StatusMahosute) == 0)
                            validCount++;
                    }
                }
            }
        }
    }

    if (validCount == 0)
        return false;

    selected = dssrand::rand(validCount);

    battleSelectTargetParam->setTargetCharacterStatus(0, targetCharacterStatus[selected]);
    battleSelectTargetParam->targetCount_ = 1;
    battleSelectTargetParam->targetGroup_ = targetCharacterGroup[selected];
    battleSelectTargetParam->targetIndex_ = targetCharacterIndex[selected];

    return true;
}

THUMB bool btl::BattleSecondCheck::personalCheckMahosute(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::CharacterStatus* targetCharacterStatus[18] = {0};
    unsigned char targetCharacterGroup[18] = {0};
    unsigned char targetCharacterIndex[18] = {0};
    int count;
    int g;
    int otherCount;
    int notMahoton;
    int group;
    int validCount;
    int hasMp;
    int selected;

    for (int i = 0; i < 18; i++) {
        targetCharacterStatus[i] = 0;
        targetCharacterGroup[i] = 0;
        targetCharacterIndex[i] = 0;
    }

    group = battleSelectTargetParam->targetGroup_;
    count = battleSelectTargetParam->getSourceCountForGroup(group);
    validCount = 0;

    for (int i = 0; i < count; i++) {
        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(group, i);
        targetCharacterGroup[validCount] = group;
        targetCharacterIndex[validCount] = i;

        status::StatusChange* statusChange = &targetCharacterStatus[validCount]->haveStatusInfo_.statusChange_;

        if (statusChange->isEnable(status::StatusChange::StatusMahokanta) == 0 &&
            statusChange->isEnable(status::StatusChange::StatusMahosute) == 0)
            validCount++;
    }

    if (validCount == 0 || battleSelectTargetParam->actorCharacterStatus_->characterType_ == MONSTER) {
        for (g = 0; g < 4; g++) {
            if (group != g) {
                otherCount = battleSelectTargetParam->getSourceCountForGroup(g);

                if (otherCount > 0) {
                    for (int i = 0; i < otherCount; i++) {
                        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(g, i);
                        targetCharacterGroup[validCount] = g;
                        targetCharacterIndex[validCount] = i;

                        status::StatusChange* statusChange = &targetCharacterStatus[validCount]->haveStatusInfo_.statusChange_;

                        if (statusChange->isEnable(status::StatusChange::StatusMahokanta) == 0 &&
                            statusChange->isEnable(status::StatusChange::StatusMahosute) == 0)
                            validCount++;
                    }
                }
            }
        }
    }

    if (validCount == 0)
        return false;

    hasMp = 0;
    notMahoton = 0;

    if (battleSelectTargetParam->actorCharacterStatus_->characterType_ == PLAYER) {
        for (int i = 0; i < g_monster.getCount(); i++) {
            status::HaveStatusInfo* info = &g_monster.getMonsterStatus(i)->haveStatusInfo_;

            if (info->isDeath() == 0) {
                if (info->getMp() != 0)
                    hasMp = 1;

                if (info->statusChange_.isEnable(status::StatusChange::StatusMahoton) == 0)
                    notMahoton = 1;
            }
        }
    } else {
        status::g_Party.setBattleMode();

        for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
            status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i)->haveStatusInfo_;

            if (info->isDeath() == 0) {
                if (info->getMp() != 0)
                    hasMp = 1;

                if (info->statusChange_.isEnable(status::StatusChange::StatusMahoton) == 0)
                    notMahoton = 1;
            }
        }
    }

    if (hasMp == 0)
        return false;

    if (notMahoton == 0)
        return false;

    selected = dssrand::rand(validCount);

    battleSelectTargetParam->setTargetCharacterStatus(0, targetCharacterStatus[selected]);
    battleSelectTargetParam->targetCount_ = 1;
    battleSelectTargetParam->targetGroup_ = targetCharacterGroup[selected];
    battleSelectTargetParam->targetIndex_ = targetCharacterIndex[selected];

    return true;
}


THUMB bool btl::BattleSecondCheck::personalCheckRukaniForGod(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::CharacterStatus* targetCharacterStatus[18] = {0};
    unsigned char targetCharacterGroup[18] = {0};
    unsigned char targetCharacterIndex[18] = {0};
    int count;
    int g;
    int otherCount;
    int group;
    status::HaveStatusInfo* info;
    status::HaveStatusInfo* otherInfo;
    int validCount;
    int selected;

    for (int i = 0; i < 18; i++) {
        targetCharacterStatus[i] = 0;
        targetCharacterGroup[i] = 0;
        targetCharacterIndex[i] = 0;
    }

    group = battleSelectTargetParam->targetGroup_;
    count = battleSelectTargetParam->getSourceCountForGroup(group);
    validCount = 0;

    for (int i = 0; i < count; i++) {
        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(group, i);
        targetCharacterGroup[validCount] = group;
        targetCharacterIndex[validCount] = i;

        status::StatusChange* statusChange = &targetCharacterStatus[validCount]->haveStatusInfo_.statusChange_;
        info = &targetCharacterStatus[validCount]->haveStatusInfo_;

        if (info->getDefence(0) != 0 &&
            statusChange->isEnable(status::StatusChange::StatusMahokanta) == 0 &&
            statusChange->isEnable(status::StatusChange::StatusMahosute) == 0 &&
            info->actionDefence_.exec(ACT_DEF_RUKANI, ACT_DEF_A) != 0)
            validCount++;
    }

    if (validCount == 0 || battleSelectTargetParam->actorCharacterStatus_->characterType_ == MONSTER) {
        for (g = 0; g < 4; g++) {
            if (group != g) {
                otherCount = battleSelectTargetParam->getSourceCountForGroup(g);

                if (otherCount > 0) {
                    for (int i = 0; i < otherCount; i++) {
                        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(g, i);
                        targetCharacterGroup[validCount] = g;
                        targetCharacterIndex[validCount] = i;

                        status::StatusChange* statusChange = &targetCharacterStatus[validCount]->haveStatusInfo_.statusChange_;
                        otherInfo = &targetCharacterStatus[validCount]->haveStatusInfo_;

                        if (otherInfo->getDefence(0) != 0 &&
                            statusChange->isEnable(status::StatusChange::StatusMahokanta) == 0 &&
                            statusChange->isEnable(status::StatusChange::StatusMahosute) == 0 &&
                            otherInfo->actionDefence_.exec(ACT_DEF_RUKANI, ACT_DEF_A) != 0)
                            validCount++;
                    }
                }
            }
        }
    }

    if (validCount == 0)
        return false;

    selected = dssrand::rand(validCount);

    battleSelectTargetParam->setTargetCharacterStatus(0, targetCharacterStatus[selected]);
    battleSelectTargetParam->targetCount_ = 1;
    battleSelectTargetParam->targetGroup_ = targetCharacterGroup[selected];
    battleSelectTargetParam->targetIndex_ = targetCharacterIndex[selected];

    return true;
}

THUMB bool btl::BattleSecondCheck::personalCheckSukaraForGod(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::CharacterStatus* targetCharacterStatus[18] = {0};
    unsigned char targetCharacterGroup[18] = {0};
    unsigned char targetCharacterIndex[18] = {0};
    int count;
    int g;
    int otherCount;
    int group;
    int validCount;
    int selected;

    for (int i = 0; i < 18; i++) {
        targetCharacterStatus[i] = 0;
        targetCharacterGroup[i] = 0;
        targetCharacterIndex[i] = 0;
    }

    group = battleSelectTargetParam->targetGroup_;
    count = battleSelectTargetParam->getSourceCountForGroup(group);
    validCount = 0;

    for (int i = 0; i < count; i++) {
        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(group, i);
        targetCharacterGroup[validCount] = group;
        targetCharacterIndex[validCount] = i;

        status::StatusChange* statusChange = &targetCharacterStatus[validCount]->haveStatusInfo_.statusChange_;

        if (targetCharacterStatus[validCount]->haveStatusInfo_.getDefence(0) != 999 &&
            statusChange->isEnable(status::StatusChange::StatusMahokanta) == 0 &&
            statusChange->isEnable(status::StatusChange::StatusMahosute) == 0)
            validCount++;
    }

    if (validCount == 0 || battleSelectTargetParam->actorCharacterStatus_->characterType_ == MONSTER) {
        for (g = 0; g < 4; g++) {
            if (group != g) {
                otherCount = battleSelectTargetParam->getSourceCountForGroup(g);

                if (otherCount > 0) {
                    for (int i = 0; i < otherCount; i++) {
                        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(g, i);
                        targetCharacterGroup[validCount] = g;
                        targetCharacterIndex[validCount] = i;

                        status::StatusChange* statusChange = &targetCharacterStatus[validCount]->haveStatusInfo_.statusChange_;

                        if (targetCharacterStatus[validCount]->haveStatusInfo_.getDefence(0) != 999 &&
                            statusChange->isEnable(status::StatusChange::StatusMahokanta) == 0 &&
                            statusChange->isEnable(status::StatusChange::StatusMahosute) == 0)
                            validCount++;
                    }
                }
            }
        }
    }

    if (validCount == 0)
        return false;

    selected = dssrand::rand(validCount);

    battleSelectTargetParam->setTargetCharacterStatus(0, targetCharacterStatus[selected]);
    battleSelectTargetParam->targetCount_ = 1;
    battleSelectTargetParam->targetGroup_ = targetCharacterGroup[selected];
    battleSelectTargetParam->targetIndex_ = targetCharacterIndex[selected];

    return true;
}

THUMB bool btl::BattleSecondCheck::personalCheckBaikirutoForGod(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::CharacterStatus* targetCharacterStatus[18] = {0};
    unsigned char targetCharacterGroup[18] = {0};
    unsigned char targetCharacterIndex[18] = {0};
    int count;
    int g;
    int otherCount;
    int group;
    int validCount;
    int selected;

    for (int i = 0; i < 18; i++) {
        targetCharacterStatus[i] = 0;
        targetCharacterGroup[i] = 0;
        targetCharacterIndex[i] = 0;
    }

    group = battleSelectTargetParam->targetGroup_;
    count = battleSelectTargetParam->getSourceCountForGroup(group);
    validCount = 0;

    for (int i = 0; i < count; i++) {
        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(group, i);
        targetCharacterGroup[validCount] = group;
        targetCharacterIndex[validCount] = i;

        status::StatusChange* statusChange = &targetCharacterStatus[validCount]->haveStatusInfo_.statusChange_;

        if (statusChange->isEnable(status::StatusChange::StatusBaikiruto) == 0 &&
            statusChange->isEnable(status::StatusChange::StatusMahokanta) == 0 &&
            statusChange->isEnable(status::StatusChange::StatusMahosute) == 0)
            validCount++;
    }

    if (validCount == 0 || battleSelectTargetParam->actorCharacterStatus_->characterType_ == MONSTER) {
        for (g = 0; g < 4; g++) {
            if (group != g) {
                otherCount = battleSelectTargetParam->getSourceCountForGroup(g);

                if (otherCount > 0) {
                    for (int i = 0; i < otherCount; i++) {
                        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(g, i);
                        targetCharacterGroup[validCount] = g;
                        targetCharacterIndex[validCount] = i;

                        status::StatusChange* statusChange = &targetCharacterStatus[validCount]->haveStatusInfo_.statusChange_;

                        if (statusChange->isEnable(status::StatusChange::StatusBaikiruto) == 0 &&
                            statusChange->isEnable(status::StatusChange::StatusMahokanta) == 0 &&
                            statusChange->isEnable(status::StatusChange::StatusMahosute) == 0)
                            validCount++;
                    }
                }
            }
        }
    }

    if (validCount == 0)
        return false;

    selected = dssrand::rand(validCount);

    battleSelectTargetParam->setTargetCharacterStatus(0, targetCharacterStatus[selected]);
    battleSelectTargetParam->targetCount_ = 1;
    battleSelectTargetParam->targetGroup_ = targetCharacterGroup[selected];
    battleSelectTargetParam->targetIndex_ = targetCharacterIndex[selected];

    return true;
}


THUMB bool btl::BattleSecondCheck::personalCheckMosyasu(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::CharacterStatus* targetCharacterStatus[18] = {0};
    unsigned char targetCharacterGroup[18] = {0};
    unsigned char targetCharacterIndex[18] = {0};
    int count;
    int g;
    int otherCount;
    int group;
    status::HaveStatusInfo* info;
    status::HaveStatusInfo* otherInfo;
    int validCount;
    int selected;

    for (int i = 0; i < 18; i++) {
        targetCharacterStatus[i] = 0;
        targetCharacterGroup[i] = 0;
        targetCharacterIndex[i] = 0;
    }

    group = battleSelectTargetParam->targetGroup_;
    count = battleSelectTargetParam->getSourceCountForGroup(group);
    validCount = 0;

    for (int i = 0; i < count; i++) {
        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(group, i);
        targetCharacterGroup[validCount] = group;
        targetCharacterIndex[validCount] = i;

        info = &targetCharacterStatus[validCount]->haveStatusInfo_;

        if (info->isMosyasTarget() == 0 && info->haveStatus_.isPlayer_ != 0)
            validCount++;
    }

    if (validCount == 0 || battleSelectTargetParam->actorCharacterStatus_->characterType_ == MONSTER) {
        for (g = 0; g < 4; g++) {
            if (group != g) {
                otherCount = battleSelectTargetParam->getSourceCountForGroup(g);

                if (otherCount > 0) {
                    for (int i = 0; i < otherCount; i++) {
                        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(g, i);
                        targetCharacterGroup[validCount] = g;
                        targetCharacterIndex[validCount] = i;

                        otherInfo = &targetCharacterStatus[validCount]->haveStatusInfo_;

                        if (otherInfo->isMosyasTarget() == 0 && otherInfo->haveStatus_.isPlayer_ != 0)
                            validCount++;
                    }
                }
            }
        }
    }

    if (validCount == 0)
        return false;

    selected = dssrand::rand(validCount);

    battleSelectTargetParam->setTargetCharacterStatus(0, targetCharacterStatus[selected]);
    battleSelectTargetParam->targetCount_ = 1;
    battleSelectTargetParam->targetGroup_ = targetCharacterGroup[selected];
    battleSelectTargetParam->targetIndex_ = targetCharacterIndex[selected];

    return true;
}

THUMB bool btl::BattleSecondCheck::personalCheckNormalCharacter(BattleSelectTargetParam *battleSelectTargetParam, status::StatusChange::Status statusType)
{
    status::CharacterStatus* targetCharacterStatus[18] = {0};
    unsigned char targetCharacterGroup[18] = {0};
    unsigned char targetCharacterIndex[18] = {0};
    int count;
    int g;
    int otherCount;
    int group;
    int validCount;
    int selected;

    for (int i = 0; i < 18; i++) {
        targetCharacterStatus[i] = 0;
        targetCharacterGroup[i] = 0;
        targetCharacterIndex[i] = 0;
    }

    group = battleSelectTargetParam->targetGroup_;
    count = battleSelectTargetParam->getSourceCountForGroup(group);
    validCount = 0;

    for (int i = 0; i < count; i++) {
        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(group, i);
        targetCharacterGroup[validCount] = group;
        targetCharacterIndex[validCount] = i;

        if (targetCharacterStatus[validCount]->haveStatusInfo_.statusChange_.isEnable(statusType) == 0)
            validCount++;
    }

    if (validCount == 0 || battleSelectTargetParam->actorCharacterStatus_->characterType_ == MONSTER) {
        for (g = 0; g < 4; g++) {
            if (group != g) {
                otherCount = battleSelectTargetParam->getSourceCountForGroup(g);

                if (otherCount > 0) {
                    for (int i = 0; i < otherCount; i++) {
                        targetCharacterStatus[validCount] = battleSelectTargetParam->getSourceCharacterStatusForGroup(g, i);
                        targetCharacterGroup[validCount] = g;
                        targetCharacterIndex[validCount] = i;

                        if (targetCharacterStatus[validCount]->haveStatusInfo_.statusChange_.isEnable(statusType) == 0)
                            validCount++;
                    }
                }
            }
        }
    }

    if (validCount == 0)
        return false;

    selected = dssrand::rand(validCount);

    battleSelectTargetParam->setTargetCharacterStatus(0, targetCharacterStatus[selected]);
    battleSelectTargetParam->targetCount_ = 1;
    battleSelectTargetParam->targetGroup_ = targetCharacterGroup[selected];
    battleSelectTargetParam->targetIndex_ = targetCharacterIndex[selected];

    return true;
}


THUMB bool btl::BattleSecondCheck::personalCheckMyMahokanta(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::StatusChange* statusChange;

    statusChange = &battleSelectTargetParam->actorCharacterStatus_->haveStatusInfo_.statusChange_;

    if (statusChange->isEnable(status::StatusChange::StatusMahokanta) != 0)
        return false;

    if (statusChange->isEnable(status::StatusChange::StatusMahosute) != 0)
        return false;

    return personalCheckFreeOne(battleSelectTargetParam);
}

THUMB bool btl::BattleSecondCheck::personalCheckChargeAttack(BattleSelectTargetParam *battleSelectTargetParam)
{
    if (battleSelectTargetParam->actorCharacterStatus_->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusPowerSave) != 0)
        return false;

    return personalCheckFreeOne(battleSelectTargetParam);
}

THUMB bool btl::BattleSecondCheck::personalCheckPartThree(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::HaveStatus* haveStatus = &battleSelectTargetParam->actorCharacterStatus_->haveStatusInfo_.haveStatus_;

    int hpMax = haveStatus->getHpMax();
    int hp = (int)haveStatus->getHp();

    if (hpMax * 10 / 3 < hp * 10)
        return false;

    return personalCheckFreeOne(battleSelectTargetParam);
}

THUMB bool btl::BattleSecondCheck::personalCheckAnkoku(BattleSelectTargetParam *battleSelectTargetParam)
{
    if (battleSelectTargetParam->actorCharacterStatus_->haveStatusInfo_.haveStatus_.getMp() != 0)
        return false;

    return personalCheckFreeOne(battleSelectTargetParam);
}

THUMB bool btl::BattleSecondCheck::personalCheckFizzleZone(BattleSelectTargetParam *battleSelectTargetParam)
{
    if (battleSelectTargetParam->actorCharacterStatus_->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusFizzleZone) != 0)
        return false;

    return personalCheckFreeOne(battleSelectTargetParam);
}

THUMB bool btl::BattleSecondCheck::personalCheckEscape(BattleSelectTargetParam *battleSelectTargetParam)
{
    int actorLevel;
    int tileLevel;
    int maxLevel;
    int i;
    int diffActor;
    int diffTile;

    if (g_Global.fightStadiumFlag_ != 0)
        return false;

    actorLevel = battleSelectTargetParam->actorCharacterStatus_->haveBattleStatus_.level_;
    tileLevel = encount::Encount::getSingleton()->encountParam_.tileLevel_;
    maxLevel = 0;

    status::g_Party.setPlayerMode();

    for (i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
        if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath() == 0) {
            if (maxLevel < status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.level_)
                maxLevel = status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.level_;
        }
    }

    actorLevel = maxLevel - actorLevel;
    tileLevel = maxLevel - tileLevel;

    if (actorLevel >= 5 || tileLevel >= 5)
        return personalCheckFreeOne(battleSelectTargetParam);

    return false;
}


THUMB bool btl::BattleSecondCheck::personalCheckNakamayobi(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::CharacterStatus* actor;
    int monsterIndex;

    if (g_Global.fightStadiumFlag_ != 0)
        return false;

    actor = battleSelectTargetParam->actorCharacterStatus_;
    monsterIndex = 0;

    switch (battleSelectTargetParam->actionIndex_) {
    case 270:
    case 271:
    case 272:
        monsterIndex = actor->characterIndex_;
        break;
    case 273:
        monsterIndex = 12;
        break;
    case 274:
        monsterIndex = 39;
        break;
    case 275:
        monsterIndex = 76;
        break;
    case 276:
        monsterIndex = 49;
        break;
    case 277:
        monsterIndex = 71;
        break;
    case 278:
        monsterIndex = 105;
        break;
    case 279:
        monsterIndex = 70;
        break;
    case 280:
        monsterIndex = 134;
        break;
    case 281:
        monsterIndex = 79;
        break;
    case 282:
        monsterIndex = 235;
        break;
    }

    if (btl::BattleMonsterDraw2::getSingleton()->isCallFriend(monsterIndex) != 0 &&
        g_monster.getMonsterCountInGroup(actor->characterGroup_) < 8)
        return personalCheckFreeOne(battleSelectTargetParam);

    return false;
}

THUMB bool btl::BattleSecondCheck::personalCheckGattai(BattleSelectTargetParam *battleSelectTargetParam)
{
    int group;
    int i;
    status::CharacterStatus* monster;

    group = battleSelectTargetParam->actorCharacterStatus_->characterGroup_;

    if (g_monster.getMonsterCountInGroup(group) != 8)
        return false;

    for (i = 0; i < 8;) {
        monster = g_monster.getMonsterInGroup(group, i);
        i++;

        if (monster->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusTimeStop) != 0 ||
            monster->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusSleep) != 0 ||
            monster->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusSpazz) != 0 ||
            monster->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusConfusion) != 0 ||
            monster->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusPath1) != 0)
            break;
    }

    if (i < 8)
        return false;

    return personalCheckFreeOne(battleSelectTargetParam);
}

THUMB bool btl::BattleSecondCheck::personalCheckMahokantaForGod(BattleSelectTargetParam *battleSelectTargetParam)
{
    int i;
    int hasMp;
    int notMahoton;
    status::HaveStatusInfo* info;
    status::StatusChange* statusChange;

    statusChange = &battleSelectTargetParam->actorCharacterStatus_->haveStatusInfo_.statusChange_;

    if (statusChange->isEnable(status::StatusChange::StatusMahokanta) != 0)
        return false;

    if (statusChange->isEnable(status::StatusChange::StatusMahosute) != 0)
        return false;

    hasMp = 0;
    notMahoton = 0;

    if (battleSelectTargetParam->actorCharacterStatus_->characterType_ == PLAYER) {
        for (i = 0; i < g_monster.getCount(); i++) {
            info = &g_monster.getMonsterStatus(i)->haveStatusInfo_;

            if (info->isDeath() == 0) {
                if (info->getMp() != 0)
                    hasMp = 1;

                if (info->statusChange_.isEnable(status::StatusChange::StatusMahoton) == 0)
                    notMahoton = 1;
            }
        }
    } else {
        status::g_Party.setBattleMode();

        for (i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
            info = &status::g_Party.getPlayerStatus(i)->haveStatusInfo_;

            if (info->isDeath() == 0) {
                if (info->getMp() != 0)
                    hasMp = 1;

                if (info->statusChange_.isEnable(status::StatusChange::StatusMahoton) == 0)
                    notMahoton = 1;
            }
        }
    }

    if (hasMp != 0 && notMahoton != 0)
        return personalCheckFreeOne(battleSelectTargetParam);

    return false;
}

THUMB bool btl::BattleSecondCheck::personalCheckFreeOne(BattleSelectTargetParam *battleSelectTargetParam)
{
    int group;
    int count;
    int i;

    if (g_Global.fightStadiumFlag_ != 0) {
        int actionIndex = battleSelectTargetParam->actionIndex_;

        if (actionIndex == 260 || (actionIndex >= 270 && actionIndex <= 282))
            return false;
    }

    group = battleSelectTargetParam->actorCharacterStatus_->characterGroup_;

    if (status::UseAction::isTargetDeadOrAlive(battleSelectTargetParam->actionIndex_)) {
        count = battleSelectTargetParam->getSourceCountForGroup(group);

        for (i = 0; i < count; i++) {
            if (battleSelectTargetParam->getSourceCharacterStatusForGroup(group, i) ==
                battleSelectTargetParam->actorCharacterStatus_)
                battleSelectTargetParam->targetIndex_ = i;
        }
    } else {
        count = battleSelectTargetParam->getSourceCountForGroupDead(group);

        for (i = 0; i < count; i++) {
            if (battleSelectTargetParam->getSourceCharacterStatusForGroupDead(group, i) ==
                battleSelectTargetParam->actorCharacterStatus_)
                battleSelectTargetParam->targetIndex_ = i;
        }
    }

    battleSelectTargetParam->setTargetCharacterStatus(0, battleSelectTargetParam->actorCharacterStatus_);
    battleSelectTargetParam->targetGroup_ = group;
    battleSelectTargetParam->targetCount_ = 1;

    return true;
}


THUMB bool btl::BattleSecondCheck::personalCheckWeapon(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::CharacterStatus* actor = battleSelectTargetParam->actorCharacterStatus_;
    CharacterType characterType = actor->characterType_;
    int sourceCount;
    int actionIndex = battleSelectTargetParam->actionIndex_;

    if (characterType == PLAYER && actionIndex == 71) {
        if (actor->haveStatusInfo_.haveEquipment_.isEquipment(39)) {
            battleSelectTargetParam->getSourceCountForGroup(battleSelectTargetParam->targetGroup_);
            battleSelectTargetParam->targetIndex_ = 0;
            return personalCheckRandomGroup(battleSelectTargetParam);
        } else if (actor->haveStatusInfo_.haveEquipment_.isEquipment(40)) {
            sourceCount = battleSelectTargetParam->sourceCount_;

            if (sourceCount == 1)
                return btl::BattleSecondCheck::personalCheckRandom(battleSelectTargetParam);

            for (int i = 0; i < sourceCount; i++) {
                battleSelectTargetParam->setTargetCharacterStatus(
                    i,
                    battleSelectTargetParam->getSourceCharacterStatus(i));
            }

            battleSelectTargetParam->targetCount_ = sourceCount;
        } else {
            return btl::BattleSecondCheck::personalCheckRandom(battleSelectTargetParam);
        }
    } else {
        return btl::BattleSecondCheck::personalCheckRandom(battleSelectTargetParam);
    }

    return true;
}

THUMB bool btl::BattleSecondCheck::personalCheckRandomGroup(BattleSelectTargetParam *battleSelectTargetParam)
{
    int groups[4];
    int groupCount;
    int i;
    int* p;
    int selectedGroup;
    int count;
    status::CharacterStatus* actor;

    dss::memset(groups, -1, sizeof(groups));

    groupCount = 0;
    i = 0;
    p = groups;

    do {
        if (battleSelectTargetParam->getSourceCountForGroup(i) != 0) {
            *p++ = i;
            groupCount++;
        }

        i++;
    } while (i < 4);

    actor = battleSelectTargetParam->actorCharacterStatus_;
    selectedGroup = actor->haveBattleStatus_.selectedGroup_;

    if (selectedGroup == -1 || actor->characterType_ == MONSTER) {
        selectedGroup = groups[dssrand::rand(groupCount)];
    } else if (battleSelectTargetParam->getSourceCountForGroup(selectedGroup) == 0) {
        selectedGroup = groups[dssrand::rand(groupCount)];
    }

    count = battleSelectTargetParam->getSourceCountForGroup(selectedGroup);

    for (i = 0; i < count; i++) {
        battleSelectTargetParam->setTargetCharacterStatus(
            i,
            battleSelectTargetParam->getSourceCharacterStatusForGroup(selectedGroup, i));
    }

    battleSelectTargetParam->targetCount_ = count;
    battleSelectTargetParam->targetGroup_ = selectedGroup;

    return true;
}

THUMB bool btl::BattleSecondCheck::personalCheckMahokantaGroup(BattleSelectTargetParam *battleSelectTargetParam)
{
    int groups[4];
    int* p;
    int j;
    int count;
    int validCount;
    int selectedGroup;
    status::CharacterStatus* source;
    status::StatusChange* statusChange;

    dss::memset(groups, -1, sizeof(groups));
    int groupCount = 0;
    int i;

    j = 0;
    i = 0;
    p = groups;

    do {
        count = battleSelectTargetParam->getSourceCountForGroup(i);

        if (count != 0) {
            validCount = 0;

            if (j < count) {
                do {
                    source = battleSelectTargetParam->getSourceCharacterStatusForGroup(i, j);
                    statusChange = &source->haveStatusInfo_.statusChange_;
                    j++;

                    if (statusChange->isEnable(status::StatusChange::StatusMahokanta) == 0 &&
                        statusChange->isEnable(status::StatusChange::StatusMahosute) == 0)
                        validCount++;
                } while (j < count);
            }

            if (count * 10 / 3 > (count - validCount) * 10) {
                *p++ = i;
                groupCount++;
            }
        }

        i++;
    } while (i < 4);

    if (groupCount == 0)
        return false;

    selectedGroup = groups[dssrand::rand(groupCount)];
    count = battleSelectTargetParam->getSourceCountForGroup(selectedGroup);

    for (i = 0; i < count; i++) {
        battleSelectTargetParam->setTargetCharacterStatus(
            i,
            battleSelectTargetParam->getSourceCharacterStatusForGroup(selectedGroup, i));
    }

    battleSelectTargetParam->targetCount_ = count;
    battleSelectTargetParam->targetGroup_ = selectedGroup;

    return true;
}

THUMB bool btl::BattleSecondCheck::personalCheckRariho(BattleSelectTargetParam *battleSelectTargetParam)
{
    return personalCheckNormalMember(battleSelectTargetParam, status::StatusChange::StatusSleep);
}


THUMB bool btl::BattleSecondCheck::personalCheckManusa(BattleSelectTargetParam *battleSelectTargetParam)
{
    return personalCheckNormalMember(battleSelectTargetParam, status::StatusChange::StatusManusa);
}



THUMB bool btl::BattleSecondCheck::personalCheckMahoton(BattleSelectTargetParam *battleSelectTargetParam)
{
    return personalCheckNormalMember(battleSelectTargetParam, status::StatusChange::StatusMahoton);
}


THUMB bool btl::BattleSecondCheck::personalCheckDokunoiki(BattleSelectTargetParam *battleSelectTargetParam)
{
    return personalCheckNormalMember(battleSelectTargetParam, status::StatusChange::StatusPoison);
}


THUMB bool btl::BattleSecondCheck::personalCheckYaketukuiki(BattleSelectTargetParam *battleSelectTargetParam)
{
    return personalCheckNormalMember(battleSelectTargetParam, status::StatusChange::StatusSpazz);
}


THUMB bool btl::BattleSecondCheck::personalCheckMedapaniDance(BattleSelectTargetParam *battleSelectTargetParam)
{
    return personalCheckNormalMember(battleSelectTargetParam, status::StatusChange::StatusConfusion);
}


THUMB bool btl::BattleSecondCheck::personalCheckRukanan(BattleSelectTargetParam *battleSelectTargetParam)
{
    int groups[4];
    int groupCount;
    int found;
    int* p;

    int i;
    int j;
    int count;
    int selectedGroup;

    dss::memset(groups, -1, sizeof(groups));

    i = 0;
    groupCount = 0;
    p = groups;

    do {
        count = battleSelectTargetParam->getSourceCountForGroup(i);

        if (count != 0) {
            found = j = 0;

            if (j < count) {
                do {
                    if (battleSelectTargetParam->getSourceCharacterStatusForGroup(i, j)
                            ->haveStatusInfo_.getDefence(0) != 0)
                        found = 1;

                    j++;
                } while (j < count);
            }

            if (found) {
                *p++ = i;
                groupCount++;
            }
        }

        i++;
    } while (i < 4);

    if (groupCount == 0)
        return false;

    selectedGroup = groups[dssrand::rand(groupCount)];
    count = battleSelectTargetParam->getSourceCountForGroup(selectedGroup);

    i = 0;

    if (i < count) {
        do {
            battleSelectTargetParam->setTargetCharacterStatus(
                i,
                battleSelectTargetParam->getSourceCharacterStatusForGroup(selectedGroup, i));

            i++;
        } while (i < count);
    }

    battleSelectTargetParam->targetCount_ = count;
    battleSelectTargetParam->targetGroup_ = selectedGroup;

    return true;
}

THUMB bool btl::BattleSecondCheck::personalCheckSukuruto(BattleSelectTargetParam *battleSelectTargetParam)
{
    int groups[4];
    int groupCount;
    int found;
    int* p;

    int i;
    int j;
    int count;
    int selectedGroup;

    dss::memset(groups, -1, sizeof(groups));

    i = 0;
    groupCount = 0;
    p = groups;

    do {
        count = battleSelectTargetParam->getSourceCountForGroup(i);

        if (count != 0) {
            found = j = 0;

            if (j < count) {
                do {
                    if (battleSelectTargetParam->getSourceCharacterStatusForGroup(i, j)
                            ->haveStatusInfo_.getDefence(0) != 999)
                        found = 1;

                    j++;
                } while (j < count);
            }

            if (found) {
                *p++ = i;
                groupCount++;
            }
        }

        i++;
    } while (i < 4);

    if (groupCount == 0)
        return false;

    selectedGroup = groups[dssrand::rand(groupCount)];
    count = battleSelectTargetParam->getSourceCountForGroup(selectedGroup);

    i = 0;

    if (i < count) {
        do {
            battleSelectTargetParam->setTargetCharacterStatus(
                i,
                battleSelectTargetParam->getSourceCharacterStatusForGroup(selectedGroup, i));

            i++;
        } while (i < count);
    }

    battleSelectTargetParam->targetCount_ = count;
    battleSelectTargetParam->targetGroup_ = selectedGroup;

    return true;
}


THUMB bool btl::BattleSecondCheck::personalCheckPiorimu(BattleSelectTargetParam *battleSelectTargetParam)
{
    int groups[4];
    int groupCount;
    int found;
    int* p;

    int i;
    int j;
    int count;
    int selectedGroup;

    dss::memset(groups, -1, sizeof(groups));

    i = 0;
    groupCount = 0;
    p = groups;

    do {
        count = battleSelectTargetParam->getSourceCountForGroup(i);

        if (count != 0) {
            found = j = 0;

            if (j < count) {
                do {
                    if (battleSelectTargetParam->getSourceCharacterStatusForGroup(i, j)
                            ->haveStatusInfo_.getAgility(0) != 0xFF)
                        found = 1;

                    j++;
                } while (j < count);
            }

            if (found) {
                *p++ = i;
                groupCount++;
            }
        }

        i++;
    } while (i < 4);

    if (groupCount == 0)
        return false;

    selectedGroup = groups[dssrand::rand(groupCount)];
    count = battleSelectTargetParam->getSourceCountForGroup(selectedGroup);

    i = 0;

    if (i < count) {
        do {
            battleSelectTargetParam->setTargetCharacterStatus(
                i,
                battleSelectTargetParam->getSourceCharacterStatusForGroup(selectedGroup, i));

            i++;
        } while (i < count);
    }

    battleSelectTargetParam->targetCount_ = count;
    battleSelectTargetParam->targetGroup_ = selectedGroup;

    return true;
}


THUMB bool btl::BattleSecondCheck::personalCheckRarihoForGod(BattleSelectTargetParam *battleSelectTargetParam)
{
    int groups[4];
    int groupCount;
    int count;
    int i;
    int notSleeping;
    int* p;
    int j;
    int validCount;
    int selectedGroup;
    status::CharacterStatus* source;
    status::StatusChange* statusChange;

    dss::memset(groups, -1, sizeof(groups));

    groupCount = 0;
    i = 0;
    p = groups;

    do {
        count = battleSelectTargetParam->getSourceCountForGroup(i);

        if (count != 0) {
            validCount = 0;
            notSleeping = 0;
            j = 0;

            if (j < count) {
                do {
                    source = battleSelectTargetParam->getSourceCharacterStatusForGroup(i, j);
                    statusChange = &source->haveStatusInfo_.statusChange_;
                    j++;

                    if (statusChange->isEnable(status::StatusChange::StatusSleep) == 0)
                        notSleeping = 1;

                    if (statusChange->isEnable(status::StatusChange::StatusMahokanta) == 0 &&
                        statusChange->isEnable(status::StatusChange::StatusMahosute) == 0)
                        validCount++;
                } while (j < count);
            }

            if ((count - validCount) * 10 < count * 10 / 3 && notSleeping != 0) {
                *p++ = i;
                groupCount++;
            }
        }

        i++;
    } while (i < 4);

    if (groupCount == 0)
        return false;

    selectedGroup = groups[dssrand::rand(groupCount)];
    count = battleSelectTargetParam->getSourceCountForGroup(selectedGroup);

    for (i = 0; i < count; i++) {
        battleSelectTargetParam->setTargetCharacterStatus(
            i,
            battleSelectTargetParam->getSourceCharacterStatusForGroup(selectedGroup, i));
    }

    battleSelectTargetParam->targetCount_ = count;
    battleSelectTargetParam->targetGroup_ = selectedGroup;

    return true;
}

THUMB bool btl::BattleSecondCheck::personalCheckMahotonForGod(BattleSelectTargetParam *battleSelectTargetParam)
{
    int groups[4];
    int groupCount;
    int count;
    int i;
    int* p;
    int j;
    int validCount;
    int selectedGroup;
    status::CharacterStatus* source;
    status::HaveStatus* haveStatus;
    status::StatusChange* statusChange;

    dss::memset(groups, -1, sizeof(groups));

    groupCount = 0;
    i = 0;
    p = groups;

    do {
        count = battleSelectTargetParam->getSourceCountForGroup(i);

        if (count != 0) {
            j = 0;
            validCount = 0;

            if (j < count) {
                do {
                    source = battleSelectTargetParam->getSourceCharacterStatusForGroup(i, j);
                    j++;
                    haveStatus = &source->haveStatusInfo_.haveStatus_;
                    statusChange = &source->haveStatusInfo_.statusChange_;

                    if (statusChange->isEnable(status::StatusChange::StatusMahokanta) == 0 &&
                        statusChange->isEnable(status::StatusChange::StatusMahosute) == 0 &&
                        statusChange->isEnable(status::StatusChange::StatusMahoton) == 0 &&
                        haveStatus->getMp() != 0)
                        validCount++;
                } while (j < count);
            }

            if (validCount != 0) {
                *p++ = i;
                groupCount++;
            }
        }

        i++;
    } while (i < 4);

    if (groupCount == 0)
        return false;

    selectedGroup = groups[dssrand::rand(groupCount)];
    count = battleSelectTargetParam->getSourceCountForGroup(selectedGroup);

    for (i = 0; i < count; i++) {
        battleSelectTargetParam->setTargetCharacterStatus(
            i,
            battleSelectTargetParam->getSourceCharacterStatusForGroup(selectedGroup, i));
    }

    battleSelectTargetParam->targetCount_ = count;
    battleSelectTargetParam->targetGroup_ = selectedGroup;

    return true;
}

THUMB bool btl::BattleSecondCheck::personalCheckRukananForGod(BattleSelectTargetParam *battleSelectTargetParam)
{
    int groups[4];
    int groupCount;
    int count;
    int i;
    int hasDefence;
    int notDefended;
    int* p;
    int j;
    int validCount;
    int selectedGroup;
    status::CharacterStatus* source;
    status::HaveStatusInfo* info;
    status::StatusChange* statusChange;

    dss::memset(groups, -1, sizeof(groups));

    groupCount = 0;
    i = 0;
    p = groups;

    do {
        count = battleSelectTargetParam->getSourceCountForGroup(i);

        if (count != 0) {
            validCount = 0;
            hasDefence = 0;
            notDefended = 1;
            j = 0;

            if (j < count) {
                do {
                    source = battleSelectTargetParam->getSourceCharacterStatusForGroup(i, j);
                    info = &source->haveStatusInfo_;
                    statusChange = &source->haveStatusInfo_.statusChange_;
                    j++;

                    if (info->actionDefence_.exec(ACT_DEF_RUKANI, ACT_DEF_A) != 0)
                        notDefended = 0;

                    if (info->getDefence(0) != 0)
                        hasDefence = 1;

                    if (statusChange->isEnable(status::StatusChange::StatusMahokanta) == 0 &&
                        statusChange->isEnable(status::StatusChange::StatusMahosute) == 0)
                        validCount++;
                } while (j < count);
            }

            if ((count - validCount) * 10 < count * 10 / 3 &&
                hasDefence != 0 && notDefended == 0) {
                *p++ = i;
                groupCount++;
            }
        }

        i++;
    } while (i < 4);

    if (groupCount == 0)
        return false;

    selectedGroup = groups[dssrand::rand(groupCount)];
    count = battleSelectTargetParam->getSourceCountForGroup(selectedGroup);

    for (i = 0; i < count; i++) {
        battleSelectTargetParam->setTargetCharacterStatus(
            i,
            battleSelectTargetParam->getSourceCharacterStatusForGroup(selectedGroup, i));
    }

    battleSelectTargetParam->targetCount_ = count;
    battleSelectTargetParam->targetGroup_ = selectedGroup;

    return true;
}

THUMB bool btl::BattleSecondCheck::personalCheckNormalMember(BattleSelectTargetParam *battleSelectTargetParam, status::StatusChange::Status statusType)
{
    int groups[4];
    int groupCount;
    int found;
    int* p;

    int i;
    int j;
    int count;
    int selectedGroup;

    dss::memset(groups, -1, sizeof(groups));

    i = 0;
    groupCount = 0;
    p = groups;

    do {
        count = battleSelectTargetParam->getSourceCountForGroup(i);

        if (count != 0) {
            found = j = 0;

            if (j < count) {
                do {
                    if (battleSelectTargetParam->getSourceCharacterStatusForGroup(i, j)
                            ->haveStatusInfo_.statusChange_.isEnable(statusType) == 0)
                        found = 1;

                    j++;
                } while (j < count);
            }

            if (found) {
                *p++ = i;
                groupCount++;
            }
        }

        i++;
    } while (i < 4);

    if (groupCount == 0)
        return false;

    selectedGroup = groups[dssrand::rand(groupCount)];
    count = battleSelectTargetParam->getSourceCountForGroup(selectedGroup);

    i = 0;

    if (i < count) {
        do {
            battleSelectTargetParam->setTargetCharacterStatus(
                i,
                battleSelectTargetParam->getSourceCharacterStatusForGroup(selectedGroup, i));

            i++;
        } while (i < count);
    }

    battleSelectTargetParam->targetCount_ = count;
    battleSelectTargetParam->targetGroup_ = selectedGroup;

    return true;
}

THUMB bool btl::BattleSecondCheck::personalCheckMahokantaAll(BattleSelectTargetParam *battleSelectTargetParam)
{
    int sourceCount = battleSelectTargetParam->sourceCount_;

    int aliveCount = 0;
    int validCount = 0;
    int i;

    status::CharacterStatus* source;

    for (i = 0; i < sourceCount; i++) {
        source = battleSelectTargetParam->getSourceCharacterStatus(i);

        if (source->haveStatusInfo_.isDeath() == 0) {
            aliveCount++;

            if (source->haveStatusInfo_.statusChange_.isEnable(
                    status::StatusChange::StatusMahokanta) == 0 &&
                source->haveStatusInfo_.statusChange_.isEnable(
                    status::StatusChange::StatusMahosute) == 0)
                validCount++;
        }
    }

    if (aliveCount * 10 / 3 <= (aliveCount - validCount) * 10)
        return false;

    for (i = 0; i < sourceCount; i++) {
        battleSelectTargetParam->setTargetCharacterStatus(
            i,
            battleSelectTargetParam->getSourceCharacterStatus(i));
    }

    battleSelectTargetParam->targetCount_ = sourceCount;

    return true;
}


THUMB bool btl::BattleSecondCheck::personalCheckMegante(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::HaveStatus* haveStatus =
        &battleSelectTargetParam->actorCharacterStatus_->haveStatusInfo_.haveStatus_;

    int hpMax = haveStatus->getHpMax() * 10;
    int hp = (int)haveStatus->getHp() * 10;

    if (hpMax / 3 < hp)
        return false;

    int sourceCount = battleSelectTargetParam->sourceCount_;

    for (int i = 0; i < sourceCount; i++) {
        battleSelectTargetParam->setTargetCharacterStatus(
            i,
            battleSelectTargetParam->getSourceCharacterStatus(i));
    }

    battleSelectTargetParam->targetCount_ = sourceCount;

    return true;
}


THUMB bool btl::BattleSecondCheck::personalCheckMeganteForGod(BattleSelectTargetParam *battleSelectTargetParam)
{
    status::HaveStatus* haveStatus =
        &battleSelectTargetParam->actorCharacterStatus_->haveStatusInfo_.haveStatus_;

    int hpMax = haveStatus->getHpMax();

    if (hpMax < (int)haveStatus->getHp() * 2)
        return false;

    int sourceCount = battleSelectTargetParam->sourceCount_;

    for (int i = 0; i < sourceCount; i++) {
        battleSelectTargetParam->setTargetCharacterStatus(
            i,
            battleSelectTargetParam->getSourceCharacterStatus(i));
    }

    battleSelectTargetParam->targetCount_ = sourceCount;

    return true;
}

THUMB bool btl::BattleSecondCheck::personalCheckBehomara(BattleSelectTargetParam *battleSelectTargetParam)
{
    int sourceCount = battleSelectTargetParam->sourceCount_;

    int aliveCount = 0;
    int validCount = 0;
    int lowHpCount = 0;
    int i;

    status::CharacterStatus* source;
    status::HaveStatus* haveStatus;
    int hpMax;

    for (i = 0; i < sourceCount; i++) {
        source = battleSelectTargetParam->getSourceCharacterStatus(i);

        if (source->haveStatusInfo_.isDeath() == 0) {
            aliveCount++;

            haveStatus = &source->haveStatusInfo_.haveStatus_;

            hpMax = haveStatus->getHpMax();

            if ((hpMax >> 1) > (int)haveStatus->getHp())
                lowHpCount++;

            if (source->haveStatusInfo_.statusChange_.isEnable(
                    status::StatusChange::StatusMahokanta) == 0 &&
                source->haveStatusInfo_.statusChange_.isEnable(
                    status::StatusChange::StatusMahosute) == 0)
                validCount++;
        }
    }

    if (aliveCount * 10 / 3 <= (aliveCount - validCount) * 10)
        return false;

    if (aliveCount * 10 / 3 * 2 > lowHpCount * 10)
        return false;

    for (i = 0; i < sourceCount; i++) {
        battleSelectTargetParam->setTargetCharacterStatus(
            i,
            battleSelectTargetParam->getSourceCharacterStatus(i));
    }

    battleSelectTargetParam->targetCount_ = sourceCount;

    return true;
}




THUMB bool btl::BattleSecondCheck::personalCheckKiariku(BattleSelectTargetParam *battleSelectTargetParam)
{
    int sourceCount = battleSelectTargetParam->sourceCount_;
    int aliveCount = 0;
    int validCount = 0;
    int flag = 0;
    int i = 0;
    status::CharacterStatus* source;

    if (i < sourceCount) {
        do {
            source = battleSelectTargetParam->getSourceCharacterStatus(i);

            if (source->haveStatusInfo_.isDeath() == 0) {
                aliveCount++;

                if (source->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusMahokanta) == 0 &&
                    source->haveStatusInfo_.statusChange_.isEnable(
                        status::StatusChange::StatusMahosute) == 0) {

                    if (source->haveStatusInfo_.statusChange_.isEnable(
                            status::StatusChange::StatusSpazz) != 0)
                        flag = 1;

                    validCount++;
                }
            }

            i++;
        } while (i < sourceCount);
    }

    if (aliveCount * 10 / 3 <= validCount * 10)
        return false;

    if (flag == 0)
        return false;

    i = 0;

    if (i < sourceCount) {
        do {
            battleSelectTargetParam->setTargetCharacterStatus(
                i,
                battleSelectTargetParam->getSourceCharacterStatus(i));
        } while (++i < sourceCount);
    }

    battleSelectTargetParam->targetCount_ = sourceCount;

    return true;
}


THUMB bool btl::BattleSecondCheck::personalCheckZameha(BattleSelectTargetParam *battleSelectTargetParam)
{
    int sourceCount = battleSelectTargetParam->sourceCount_;
    int aliveCount = 0;
    int validCount = 0;
    int flag = 0;
    int i;

    status::CharacterStatus* source;

    for (i = 0; i < sourceCount; i++) {
        source = battleSelectTargetParam->getSourceCharacterStatus(i);

        if (source->haveStatusInfo_.isDeath() == 0) {
            aliveCount++;

            if (source->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusMahokanta) == 0 && source->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusMahosute) == 0) {
                if (source->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusSleep) != 0)
                    flag = 1;
                validCount++;
            }
        }
    }

    if (aliveCount * 10 / 3 <= validCount * 10)
        return false;

    if (flag == 0)
        return false;

    for (i = 0; i < sourceCount; i++) {
        battleSelectTargetParam->setTargetCharacterStatus(
            i,
            battleSelectTargetParam->getSourceCharacterStatus(i));
    }

    battleSelectTargetParam->targetCount_ = sourceCount;

    return true;
}




THUMB bool btl::BattleSecondCheck::personalCheckHubaha(BattleSelectTargetParam* battleSelectTargetParam)
{
    int count = 0;
    int i = 0;
    status::CharacterStatus* source;
    int sourceCount = battleSelectTargetParam->sourceCount_;

    if (i < sourceCount) {
        do {
            source = battleSelectTargetParam->getSourceCharacterStatus(i);

            if (source->haveStatusInfo_.isFubahaFlag() != 0) {
                if (source->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusFubaha) == 0) {
                    count++;
                }
            }

            i++;
            sourceCount = battleSelectTargetParam->sourceCount_;
        } while (i < sourceCount);
    }

    if (count == 0) {
        return false;
    }

    for (i = 0; i < sourceCount; i++) {
        battleSelectTargetParam->setTargetCharacterStatus(
            i, battleSelectTargetParam->getSourceCharacterStatus(i));
    }

    battleSelectTargetParam->targetCount_ = sourceCount;
    return true;
}


THUMB bool btl::BattleSecondCheck::personalCheckMegazaru(BattleSelectTargetParam* battleSelectTargetParam)
{
    int sourceCount;
    int validCount;
    int i;
    status::HaveStatusInfo* p_haveStatusInfo;

    sourceCount = battleSelectTargetParam->sourceCount_;
    validCount = 0;

    for (i = 0; i < sourceCount; i++) {
        p_haveStatusInfo = &battleSelectTargetParam->getSourceCharacterStatus(i)->haveStatusInfo_;
        if (p_haveStatusInfo->isDeath() != 0) {
            if (p_haveStatusInfo->isEscapeFlag() == 0) {
                if (p_haveStatusInfo->isDisappearFlag() == 0) {
                    validCount++;
                }
            }
        }
    }

    if (sourceCount > validCount * 2 || validCount == 0) {
        return false;
    }

    int targetCount;
    status::CharacterStatus* source;

    targetCount = 0;
    i = targetCount;

    if (i < sourceCount) {
        do {
            source = battleSelectTargetParam->getSourceCharacterStatus(i);
            p_haveStatusInfo = &source->haveStatusInfo_;

            if (p_haveStatusInfo->isEscapeFlag() == 0) {
                if (p_haveStatusInfo->isDisappearFlag() == 0) {
                    battleSelectTargetParam->setTargetCharacterStatus(targetCount, source);
                    targetCount++;
                }
            }
        } while (++i < sourceCount);
    }

    battleSelectTargetParam->targetCount_ = targetCount;
    return true;
}

THUMB bool btl::BattleSecondCheck::personalCheckOtakebi(BattleSelectTargetParam* battleSelectTargetParam)
{
    int sourceCount;
    int i;
    int found;
    status::CharacterStatus* source;

    sourceCount = battleSelectTargetParam->sourceCount_;

    i = 0;
    found = 0;

    if (i < sourceCount) {
        do {
            source = battleSelectTargetParam->getSourceCharacterStatus(i);
            i++;

            if (source->haveStatusInfo_.isDeath() == 0) {
                if (source->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusPath1) != 0) {
                    found = 1;
                    break;
                }
            }
        } while (i < sourceCount);
    }

    if (found != 0) {
        return false;
    }

    for (i = 0; i < sourceCount; i++) {
        battleSelectTargetParam->setTargetCharacterStatus(i, battleSelectTargetParam->getSourceCharacterStatus(i));
    }

    battleSelectTargetParam->targetCount_ = sourceCount;
    return true;
}


THUMB bool btl::BattleSecondCheck::personalCheckHadou(BattleSelectTargetParam* battleSelectTargetParam)
{
    int aliveCount;
    int validCount;
    int sourceCount;
    status::StatusChange* p_statusChange;
    status::CharacterStatus* source;

    sourceCount = battleSelectTargetParam->sourceCount_;
    aliveCount = 0;
    validCount = 0;

    for (int i = 0; i < sourceCount; i++) {
        source = battleSelectTargetParam->getSourceCharacterStatus(i);

        if (source->haveStatusInfo_.isDeath() == 0) {
            p_statusChange = &source->haveStatusInfo_.statusChange_;
            aliveCount++;

            if (p_statusChange->isEnable(status::StatusChange::StatusMahosute) != 0) goto checkEnd;
            if (p_statusChange->isEnable(status::StatusChange::StatusMahokanta) != 0) goto checkEnd;
            if (p_statusChange->isEnable(status::StatusChange::StatusDefenceChange) != 0) goto checkEnd;
            if (p_statusChange->isEnable(status::StatusChange::StatusFubaha) != 0) goto checkEnd;
            if (p_statusChange->isEnable(status::StatusChange::StatusBaikiruto) != 0) goto checkEnd;
            if (p_statusChange->isEnable(status::StatusChange::StatusMosyasu) != 0) goto checkEnd;
            if (p_statusChange->isEnable(status::StatusChange::StatusAstoron) != 0) goto checkEnd;
            if (p_statusChange->isEnable(status::StatusChange::StatusDragoram) != 0) goto checkEnd;
            if (p_statusChange->isEnable(status::StatusChange::StatusAgilityChange) != 0) goto checkEnd;
            validCount++;
        }
    }

checkEnd:
    if (validCount == aliveCount) {
        return 0;
    }

    for (int i = 0; i < sourceCount; i++) {
        battleSelectTargetParam->setTargetCharacterStatus(
            i, battleSelectTargetParam->getSourceCharacterStatus(i));
    }

    battleSelectTargetParam->targetCount_ = sourceCount;
    return 1;
}


THUMB bool btl::BattleSecondCheck::personalCheckFree(){
    return 1;
}