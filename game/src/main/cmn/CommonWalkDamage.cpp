#include "main/cmn/CommonWalkDamage.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/encount/Encount.hpp"
#include "main/sound/SoundManager.hpp"

int cmn::CommonWalkDamage::memberDamage_;
int cmn::CommonWalkDamage::topStride_;
int cmn::CommonWalkDamage::partyStride_;
int cmn::CommonWalkDamage::walkCount_;
int cmn::CommonWalkDamage::effectFlag_;
int cmn::CommonWalkDamage::damageFlag_;
int cmn::CommonWalkDamage::encountFlag_;
cmn::MembaerDamage cmn::CommonWalkDamage::partyDamage_[4];
signed char cmn::CommonWalkDamage::damage_[82];

ARM void cmn::CommonWalkDamage::setup()
{
    for (int i = 0; i < 82; i++) {
        damage_[i] = None;
    }
    for (int i = 0; i < 4; i++) {
        partyDamage_[i].type = None;
    }
    memberDamage_ = 0;
    effectFlag_ = 1;
    damageFlag_ = 1;
    encountFlag_ = 1;
    seCounter_ = -1;
}

ARM void cmn::CommonWalkDamage::clear()
{
    for (int i = 0; i < 82; i++) {
        damage_[i] = None;
    }
    for (int i = 0; i < 4; i++) {
        partyDamage_[i].type = None;
    }
    memberDamage_ = 0;
    seCounter_ = -1;
}

ARM bool cmn::CommonWalkDamage::checkWalkStride()
{
    bool result = false;
    if (partyDamage_[0].frame % topStride_ == 0) {
        result = true;
    }

    int type = partyDamage_[0].type;
    damage_[0] = type;
    if (type != None) {
        partyDamage_[0].frame++;
    }

    status::g_Party.setBattleMode();
    int count = status::g_Party.getCarriageOutCount();
    for (int i = 1; i < count; i++) {
        partyDamage_[i].frame++;
        partyDamage_[i].type = damage_[partyStride_ * i - 1];
        if ((partyDamage_[i].type != None && damage_[partyStride_ * i] == None) || partyDamage_[i].frame >= topStride_) {
            partyDamage_[i].counter = 0;
            partyDamage_[i].frame = 0;
        }
    }

    int total = partyStride_ * status::g_Party.getCarriageOutCount();
    for (int i = 0; i < total; i++) {
        damage_[total - i] = damage_[total - i - 1];
    }

    walkCount_++;
    return result;
}

ARM void cmn::CommonWalkDamage::checkWalk(dss::Fix32Vector3& pos, dss::Fix32Vector3& prevPos)
{
    status::g_Party.setBattleMode();
    int count = status::g_Party.getCarriageOutCount();
    int damage = None;

    if (checkBarrier() == 1) {
        partyDamage_[0].type = Barrier;
        memberDamage_ = 1;
    } else if (checkPoison() == 1) {
        partyDamage_[0].type = Poison;
        memberDamage_ = 1;
    } else {
        partyDamage_[0].type = None;
        partyDamage_[0].frame = 0;
    }

    if (pos != prevPos) {
        if (encountFlag_ == 1) {
            encount::Encount::getSingleton()->execWalk();
        }
        if (memberDamage_ == 1 && checkWalkStride() == 1 && partyDamage_[0].type != None) {
            partyDamage_[0].counter = 0;
            walkCount_ = 0;
        }
        if (damageFlag_ == 1) {
            for (int i = 0; i < count; i++) {
                if (status::g_Party.getPlayerStatus(i)->walkNormal() == 1 && effectFlag_ == 1) {
                    setPartyMemberColor(status::g_Party.getPlayerIndex(i), 1);
                    partyDamage_[i].counter = 0;
                }
            }
        }
    }

    for (int i = 0; i < count; i++) {
        if (memberDamage_ == 1 && partyDamage_[i].type != None) {
            if (partyDamage_[i].counter == 0) {
                if (!status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath() == 1 && damageFlag_ == 1) {
                    if (status::g_Party.getPlayerStatus(i)->walkDamage((StageDamage)partyDamage_[i].type) == 1 && effectFlag_ == 1) {
                        setPartyMemberColor(status::g_Party.getPlayerIndex(i), partyDamage_[i].type);
                    }
                }
            }
            damage = partyDamage_[i].type;
        }
        if (partyDamage_[i].counter == 4) {
            setPartyMemberColor(status::g_Party.getPlayerIndex(i), 2);
        }
        partyDamage_[i].counter++;
    }

    if (memberDamage_ == 1 && damage == None && walkCount_ > partyStride_ * (count - 1)) {
        memberDamage_ = 0;
        for (int i = 0; i < count; i++) {
            setPartyMemberColor(status::g_Party.getPlayerIndex(i), 2);
        }
        for (int i = 0; i < 82; i++) {
            damage_[i] = None;
        }
        walkCount_ = 0;
    }

    if (seCounter_ != -1) {
        seCounter_++;
        if (seCounter_ >= 12) {
            seCounter_ = -1;
            if (nextSe_ == 1) {
                if (nextSeType_ == 0) {
                    SoundManager::playSe(0x13c, 0);
                } else if (nextSeType_ == 1) {
                    SoundManager::playSe(0x13b, 0);
                }
                nextSe_ = 0;
                seCounter_ = 0;
            }
        }
    }
}

ARM bool cmn::CommonWalkDamage::isPlaySe()
{
    if (seCounter_ == -1) {
        seCounter_ = 0;
        return true;
    }
    return false;
}

ARM void cmn::CommonWalkDamage::setNextSe(int type)
{
    if ((unsigned int)type > 1) {
        return;
    }
    if (seCounter_ > 0) {
        nextSe_ = 1;
        nextSeType_ = type;
    }
}
