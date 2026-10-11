#include "main/status/PlayerStatus.hpp"
#include "main/status/StageStatus.hpp"

THUMB status::PlayerStatus::PlayerStatus()
{
    clearWalkDamage();
    return;
}   

THUMB status::PlayerStatus::~PlayerStatus(){
    return;
}

THUMB void status::PlayerStatus::setup(int index)
{
    this->index_ = index;
    this->characterType_ = PLAYER;
    this->haveStatusInfo_.setup(this->index_, 1);
    this->haveStatusInfo_.haveBattleStatus_ = &this->haveBattleStatus_;
    this->haveBattleStatus_.setup(PLAYER, 0, this->index_);
    this->haveBattleStatus_.haveStatusInfo_ = &this->haveStatusInfo_;
}

THUMB void status::PlayerStatus::levelup(int level)
{
    if (this->haveStatusInfo_.haveStatus_.isPlayer_) {
        this->haveStatusInfo_.levelup(level);
    }
    return;
}

THUMB void status::PlayerStatus::setBestCondition()
{
    this->haveStatusInfo_.haveStatus_.addBaseHp(1000);
    this->haveStatusInfo_.haveStatus_.addBaseMp(1000);
    return;
}

THUMB bool status::PlayerStatus::walkDamage(StageDamage damage) {
    bool result = false;

    if (this->haveStatusInfo_.isDeath()) {
        return result;
    }

    switch (damage) {
    case Barrier:
        if (status::StageStatus::isToramana() == 0) {
            setNearDeath(0xf);
            result = true;
        }
        break;
    case Poison:
        if (status::StageStatus::isToramana() == 0) {
            setNearDeath(1);
            result = true;
        }
        break;
    case None:
        break;
    }

    return result;
}

THUMB bool status::PlayerStatus::walkNormal() {
    bool result = false;

    this->walkCount_++;
    if (this->walkCount_ < 0x10) {
        return result;
    }

    this->walkCount_ = 0;

    if (this->haveStatusInfo_.isDeath()) {
        return result;
    }

    if (g_Stage.isEncount()) {
        if (this->setWalkEffectPoison()) {
            result = true;
        }
        if (this->setWalkEffectZombiMail()) {
            result = true;
        }
        this->setWalkEffectLuckShoes();
    }

    this->setWalkEffectLuckHat();
    this->setWalkEffectGoddessRing();
    this->setWalkEffectLifeRing();

    return result;
}

THUMB bool status::PlayerStatus::setWalkEffectPoison() {
    bool result = false;

    if (this->haveStatusInfo_.getCondition() == 6) {
        if (this->walkCountPoison_ % 8 == 0) {
            this->setNearDeath(1);
            result = true;
        }
        this->walkCountPoison_++;
    }

    return result;
}

THUMB bool status::PlayerStatus::setWalkEffectZombiMail() {
    bool result = false;

    if (this->haveStatusInfo_.haveStatus_.playerIndex_ != 25 &&
        this->haveStatusInfo_.haveEquipment_.isEquipment(68)) {
        if (this->walkCountZombieMail_ % 8 == 0) {
            this->setNearDeath(1);
            result = true;
        }
        this->walkCountZombieMail_++;
    }

    return result;
}

THUMB void status::PlayerStatus::setWalkEffectLuckShoes()
{
    if (this->haveStatusInfo_.haveEquipment_.isEquipment(105))
    {
        this->haveStatusInfo_.addExp(1);
        ++this->walkCountLuckShoes_;
    }
    else
    {
        this->walkCountLuckShoes_ = 0;
    }
    return;
}

THUMB void status::PlayerStatus::setWalkEffectLuckHat() {
    if (this->haveStatusInfo_.haveEquipment_.isEquipment(92)) {
        if (this->walkCountLuckHat_ % 4 == 0) {
            this->haveStatusInfo_.addMp(1);
        }
        this->walkCountLuckHat_++;
    }
    else {
        this->walkCountLuckHat_ = 0;
    }
    return;
}

THUMB void status::PlayerStatus::setWalkEffectGoddessRing() {
    if (this->haveStatusInfo_.haveEquipment_.isEquipment(103)) {
        if (this->walkCountGoddessRing_ % 8 == 0) {
            this->haveStatusInfo_.addMp(1);
        }
        this->walkCountGoddessRing_++;
    }
    else {
        this->walkCountGoddessRing_ = 0;
    }
    return;
}

THUMB void status::PlayerStatus::setWalkEffectLifeRing() {
    if (this->haveStatusInfo_.haveEquipment_.isEquipment(106)) {
        if (this->walkCountLifeRing_ % 4 == 0) {
            this->haveStatusInfo_.addBaseHp(1);
        }
        this->walkCountLifeRing_++;
    }
    else {
        this->walkCountLifeRing_ = 0;
    }
    return;
}

THUMB void status::PlayerStatus::clearWalkDamage() {
    this->walkCount_ = 0;
    this->walkCountBarrier_ = 0;
    this->walkCountPoison_ = 0;
    this->walkCountPoisonCondition_ = 0;
    this->walkCountZombieMail_ = 0;
    this->walkCountLuckShoes_ = 0;
    this->walkCountLuckHat_ = 0;
    this->walkCountGoddessRing_ = 0;
    this->walkCountLifeRing_ = 0;
}

THUMB void status::PlayerStatus::setNearDeath(int damage)
{
    int currentHp;
    currentHp = this->haveStatusInfo_.getHp();
    if (currentHp - damage >= 1) {
        this->haveStatusInfo_.addBaseHp(-damage);
    }
    else {
        this->haveStatusInfo_.addBaseHp(-(currentHp - 1));
    }
}

THUMB void status::PlayerStatus::execStartOfRound()
{
    if (this->haveStatusInfo_.battleCommand_ != COMMAND_MEIREISASERO) {
        this->haveBattleStatus_.setSelectCommand(status::HaveBattleStatus::AISelect, 0);
    }
    return;
}

THUMB void status::PlayerStatus::execOfRound()
{
    this->haveStatusInfo_.statusChange_.execStartOfTurn();
    this->haveStatusInfo_.statusChange_.execEndOfTurn();

    if (this->haveStatusInfo_.statusChange_.getExecMessage1(status::StatusChange::StatusDragoram)) {
        this->haveBattleStatus_.changeMonsterReverse();
    }

    if (this->haveStatusInfo_.statusChange_.isRelease()) {
        this->haveStatusInfo_.statusChange_.getReleaseMessage();
    }

    this->haveStatusInfo_.statusChange_.execEndOfRound();
    return;
}

THUMB void status::PlayerStatus::print(){
}

THUMB void status::PlayerStatus::printAction(){
}

THUMB void status::PlayerStatus::setSaveDataForPlayer(profile::PROFILE_PLAYER* dat) {
    dat->LEVEL = this->haveStatusInfo_.haveStatus_.level_;
    dat->STRENGTH = this->haveStatusInfo_.haveStatus_.baseStatus_.strength_;
    dat->AGILITY = this->haveStatusInfo_.haveStatus_.getAgility();
    dat->WISDAM = this->haveStatusInfo_.haveStatus_.getWisdom();
    dat->LUCK = this->haveStatusInfo_.haveStatus_.getLuck();
    dat->PROTECTION = this->haveStatusInfo_.haveStatus_.baseStatus_.protection_;
    dat->HP = this->haveStatusInfo_.haveStatus_.getHp();
    dat->HPMAX = this->haveStatusInfo_.haveStatus_.getHpMax();
    dat->MP = this->haveStatusInfo_.haveStatus_.getMp();
    dat->MPMAX = this->haveStatusInfo_.haveStatus_.getMpMax();
    dat->EXP = this->haveStatusInfo_.haveStatus_.exp_;
    dat->TACTICS = this->haveStatusInfo_.battleCommand_;

    if (this->haveStatusInfo_.haveStatus_.isPlayer_) {
        HaveItem* haveItem = &this->haveStatusInfo_.haveItem_;
        int mask = 1;
        dat->ITEMEQUIP = 0;
        for (int i = 0; i < 12; i++) {
            dat->HAVEITEM[i] = haveItem->getItem(i);
            if (haveItem->isEquipment(i)) {
                dat->ITEMEQUIP |= mask;
            }
            mask <<= 1;
        }

        int j = 0;
        Action* action = this->haveStatusInfo_.haveAction_.newAction_;
        do {
            dat->HAVEACTION_BATTLE[j] = action->battleAction_;
            dat->HAVEACTION_USUALLY[j] = action->usuallyAction_;
            dat->HAVEACTION_LEVEL[j] = action->actionLevel_;
            dat->HAVEACTION_FLAG[j] = action->actionFlag_;
            j++;
            action++;
        } while (j < 24);
    }

    dat->POISON = this->haveStatusInfo_.isPoison();
}

THUMB void status::PlayerStatus::setLoadDataForPlayer(profile::PROFILE_PLAYER* dat) {
    this->haveStatusInfo_.haveStatus_.setLevel(dat->LEVEL);
    this->haveStatusInfo_.haveStatus_.baseStatus_.strength_ = dat->STRENGTH;
    this->haveStatusInfo_.haveStatus_.baseStatus_.agility_ = dat->AGILITY;
    this->haveStatusInfo_.haveStatus_.baseStatus_.wisdom_ = dat->WISDAM;
    this->haveStatusInfo_.haveStatus_.baseStatus_.luck_ = dat->LUCK;
    this->haveStatusInfo_.haveStatus_.baseStatus_.protection_ = dat->PROTECTION;
    this->haveStatusInfo_.haveStatus_.setHp(dat->HP);
    this->haveStatusInfo_.haveStatus_.baseStatus_.setHpMax(dat->HPMAX);
    this->haveStatusInfo_.haveStatus_.setMp(dat->MP);
    this->haveStatusInfo_.haveStatus_.baseStatus_.setMpMax(dat->MPMAX);
    this->haveStatusInfo_.haveStatus_.setExp(dat->EXP);
    this->haveStatusInfo_.battleCommand_ = (CommandType)dat->TACTICS;

    if (this->haveStatusInfo_.haveStatus_.isPlayer_) {
        HaveItem* haveItem = &this->haveStatusInfo_.haveItem_;
        haveItem->clear();
        int mask = 1;

        for (int i = 0; i < 12; i++) {
            unsigned char itemIndex = dat->HAVEITEM[i];
            if (itemIndex != 0) {
                haveItem->addOne(itemIndex);
                if (dat->ITEMEQUIP & mask) {
                    haveItem->setEquipmentForTest(i);
                }
            }
            mask <<= 1;
        }

        HaveAction* haveAction = &this->haveStatusInfo_.haveAction_;
        haveAction->clear();
        int j = 0;
        do {
            haveAction->newAction_[j].battleAction_ = dat->HAVEACTION_BATTLE[j];
            haveAction->newAction_[j].usuallyAction_ = dat->HAVEACTION_USUALLY[j];
            haveAction->newAction_[j].actionLevel_ = dat->HAVEACTION_LEVEL[j];
            haveAction->newAction_[j].actionFlag_ = dat->HAVEACTION_FLAG[j];
            j++;
        } while (j < 24);
    }

    if (dat->POISON) {
        this->haveStatusInfo_.setPoison();
    }
}


#ifdef DQIV_NATIVE
// The matching DS build exports these methods using explicit ARM ABI names.
// Native C++ vtables need definitions with the host compiler's ABI instead.
bool status::CharacterStatus::isDeathDisable() { return false; }
void status::CharacterStatus::setMosyasRelease() {}
void status::CharacterStatus::setMosyasChange(int) {}
#else
extern "C" ARM bool _ZN6status15CharacterStatus14isDeathDisableEv(status::CharacterStatus* self) {
    return 0;
}

extern "C" ARM void _ZN6status15CharacterStatus16setMosyasReleaseEv(status::CharacterStatus* self) {
}

extern "C" ARM void _ZN6status15CharacterStatus15setMosyasChangeEi(status::CharacterStatus* self, int monsterIndex) {
}
#endif
