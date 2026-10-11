#include "ov003/btl/BattleRound.hpp"
#include "ov003/btl/BattleActorManager2.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"
#include "main/status/ActionExec.hpp"
#include "main/status/BaseActionStatus.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/global/Global.hpp"

#pragma profile on
THUMB btl::BattleRound::BattleRound()
{
}

THUMB btl::BattleRound::~BattleRound()
{
}

THUMB void btl::BattleRound::initialize()
{
    BattleActorManager2::getSingleton()->selectActor();
    BattleActorManager2::getSingleton()->setActorAction();
    BattleActorManager2::getSingleton()->setActorOrder();
    countBattleTurn_ = BattleActorManager2::getSingleton()->getActorCount();
    for (int i = 0; i < countBattleTurn_; i++) {
        battleTurn_[i].setBattleActor2(BattleActorManager2::getSingleton()->getBattleActor(i));
    }
    BattleActorManager2::getSingleton()->execStartOfRound();
    currentBattleTurn_ = 0;
    turnEndFlag_ = 0;
}

THUMB void btl::BattleRound::terminate()
{
    BattleActorManager2::getSingleton()->execEndOfRound();
    BattleActorManager2::getSingleton()->retireActor();
    status::g_Party.execOfRoundInCarriage();
}

THUMB void btl::BattleRound::execute()
{
    battleTurn_[currentBattleTurn_].execute();
}

THUMB int btl::BattleRound::isEnd()
{
    turnEndFlag_ = 0;
    if (battleTurn_[currentBattleTurn_].isEnd()) {
        turnEndFlag_ = 1;
        if (status::BaseActionStatus::execCallFriend_) {
            status::BaseActionStatus::execCallFriend_ = 0;
            if (status::getCallMonsterCount()) {
                add(status::getCallMonsterStatus(0));
            }
        }
        if (g_Global.fightStadiumFlag_ && g_monster.getBattleCount() == 1) {
            return 1;
        }
        if (!isMegazaruRingEnable() && BattleActorManager2::getSingleton()->isBattleEnd()) {
            return 1;
        }
        if (execMeganteRing()) {
            currentBattleTurn_++;
            return 0;
        }
        if (execMegazaruRing()) {
            currentBattleTurn_++;
            return 0;
        }

        battleTurn_[currentBattleTurn_].battleActor_->characterStatus_->haveBattleStatus_.setMultiAction();
        if (BattleActorManager2::getSingleton()->isBattleEnd()) {
            return 1;
        }
        if (battleTurn_[currentBattleTurn_].battleActor_->characterStatus_->haveBattleStatus_.isMultiAction()) {
            battleTurn_[currentBattleTurn_].battleActor_->characterStatus_->haveBattleStatus_.multiExecCount_++;
            if (battleTurn_[currentBattleTurn_].battleActor_->characterStatus_->haveBattleStatus_.multiExecCount_ < 2) {
                if (battleTurn_[currentBattleTurn_].battleActor_->isActionEnable()) {
                    battleTurn_[currentBattleTurn_].battleActor_->characterStatus_->haveBattleStatus_.setActionSelect(status::HaveBattleStatus::CallStart(0));
                    battleTurn_[currentBattleTurn_].reattack();
                    return 0;
                }
            }
            else {
                battleTurn_[currentBattleTurn_].battleActor_->characterStatus_->haveBattleStatus_.multiExecCount_ = 0;
            }
        }

        if (battleTurn_[currentBattleTurn_].battleActor_->characterStatus_->haveStatusInfo_.isSilverTarot()) {
            battleTurn_[currentBattleTurn_].battleActor_->characterStatus_->haveStatusInfo_.setSilverTarot(false);
            battleTurn_[currentBattleTurn_].battleActor_->characterStatus_->haveBattleStatus_.setupTarotAction();
            battleTurn_[currentBattleTurn_].tarot();
            return 0;
        }

        if (battleTurn_[currentBattleTurn_].battleActor_->characterStatus_->haveBattleStatus_.actionIndex_ == 0x42) {
            battleTurn_[currentBattleTurn_].battleActor_->characterStatus_->haveBattleStatus_.setupParupunteAction();
            battleTurn_[currentBattleTurn_].parupunte();
            return 0;
        }
        if (battleTurn_[currentBattleTurn_].battleActor_->characterStatus_->haveBattleStatus_.actionIndex_ == 0x2f &&
            battleTurn_[currentBattleTurn_].battleActor_->useActionParam_.result_) {
            battleTurn_[currentBattleTurn_].reattack();
            return 0;
        }
        if (battleTurn_[currentBattleTurn_].battleActor_->characterStatus_->haveBattleStatus_.actionIndex_ == 0x1e2 &&
            !battleTurn_[currentBattleTurn_].battleActor_->characterStatus_->haveStatusInfo_.isDeath()) {
            battleTurn_[currentBattleTurn_].reattack();
            return 0;
        }
        if (battleTurn_[currentBattleTurn_].battleActor_->characterStatus_->haveBattleStatus_.actionIndex_ == 0x1d9) {
            battleTurn_[currentBattleTurn_].reattack();
            return 0;
        }
        if (battleTurn_[currentBattleTurn_].battleActor_->characterStatus_->haveBattleStatus_.actionIndex_ == 0x11e) {
            battleTurn_[currentBattleTurn_].change();
            return 0;
        }
        if (battleTurn_[currentBattleTurn_].battleActor_->characterStatus_->haveBattleStatus_.actionIndex_ == 0x11f) {
            battleTurn_[currentBattleTurn_].change();
            return 0;
        }
        if (battleTurn_[currentBattleTurn_].battleActor_->characterStatus_->haveBattleStatus_.actionIndex_ == 0x120) {
            battleTurn_[currentBattleTurn_].change();
            return 0;
        }
        if (status::BaseActionStatus::isMonsterChange()) {
            status::BaseActionStatus::setMonsterChange(0);
            battleTurn_[currentBattleTurn_].change();
            return 0;
        }

        if (battleTurn_[currentBattleTurn_].battleActor_->characterStatus_->haveBattleStatus_.actionIndex_ == 0x1db) {
            currentBattleTurn_ = countBattleTurn_ - 1;
        }
        do {
            currentBattleTurn_++;
            if (currentBattleTurn_ == countBattleTurn_ - 1) {
                battleTurn_[currentBattleTurn_].battleActor_->characterStatus_->haveStatusInfo_.setLastActor(true);
            }
            if (currentBattleTurn_ >= countBattleTurn_) {
                return 1;
            }
        } while (!battleTurn_[currentBattleTurn_].battleActor_->isActionEnable());
    }

    if (currentBattleTurn_ >= countBattleTurn_) {
        return 1;
    }
    return 0;
}

THUMB btl::BattleActor2* btl::BattleRound::add(status::CharacterStatus* chara)
{
    for (int i = countBattleTurn_ - 1; i > currentBattleTurn_; i--) {
        battleTurn_[i + 1] = battleTurn_[i];
    }
    BattleActor2* actor = BattleActorManager2::getSingleton()->add(chara);
    battleTurn_[currentBattleTurn_ + 1].setBattleActor2(actor);
    countBattleTurn_++;
    return actor;
}

THUMB int btl::BattleRound::isMegazaruRingEnable()
{
    if (!status::HaveStatusInfo::isGlbMegazaruRing()) {
        return 0;
    }
    if (!status::g_Party.isMegazaruRingEnable()) {
        status::HaveStatusInfo::setGlbMegazaruRing(false);
        return 0;
    }
    return 1;
}

THUMB int btl::BattleRound::execMegazaruRing()
{
    if (isMegazaruRingEnable()) {
        status::HaveStatusInfo::setGlbMegazaruRing(false);
        status::g_Party.setPlayerMode();
        int count = status::g_Party.getCount();
        for (int i = 0; i < count; i++) {
            status::PlayerStatus* player = status::g_Party.getPlayerStatus(i);
            if (player->haveStatusInfo_.isMegazaruRing()) {
                player->haveStatusInfo_.setMegazaruRing(false);
                player->haveStatusInfo_.setStatusChangeRelease(false);
                BattleActor2* actor = add(player);
                player->haveStatusInfo_.setHp(1);
                actor->megazaruRing();
                status::HaveStatusInfo* info = &player->haveStatusInfo_;
                int itemCount = info->haveItem_.getCount();
                for (int j = 0; j < itemCount; j++) {
                    if (info->haveItem_.isEquipment(j) && info->haveItem_.getItem(j) == 0x62) {
                        info->execThrow(j);
                        break;
                    }
                }
                return 1;
            }
        }
    }
    return 0;
}

THUMB int btl::BattleRound::execMeganteRing()
{
    if (status::HaveStatusInfo::isGlbMeganteRing()) {
        status::HaveStatusInfo::setGlbMeganteRing(false);
        status::g_Party.setPlayerMode();
        int count = status::g_Party.getCount();
        for (int i = 0; i < count; i++) {
            status::PlayerStatus* player = status::g_Party.getPlayerStatus(i);
            if (player->haveStatusInfo_.isMeganteRing()) {
                player->haveStatusInfo_.setMeganteRing(false);
                player->haveStatusInfo_.setStatusChangeRelease(false);
                BattleActor2* actor = add(player);
                player->haveStatusInfo_.setHp(1);
                actor->meganteRing();
                status::HaveStatusInfo* info = &player->haveStatusInfo_;
                int itemCount = info->haveItem_.getCount();
                for (int j = 0; j < itemCount; j++) {
                    if (info->haveItem_.isEquipment(j) && info->haveItem_.getItem(j) == 0x61) {
                        info->execThrow(j);
                        break;
                    }
                }
                return 1;
            }
        }
    }
    return 0;
}
