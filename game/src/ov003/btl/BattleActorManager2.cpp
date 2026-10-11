#pragma ipa file
#include "ov003/btl/BattleActorManager2.hpp"
#include "main/dss/DssUtils.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"
#include "main/status/BattleHistory.hpp"
#include "ov037/PlayerTitle.hpp"
#include "main/encount/Encount.hpp"
#include "main/encount/EncountParam.hpp"
#include "main/formation/FormationIdManager.hpp"
#include "main/SpecialParty/SpecialParty.hpp"
#include "main/CountDown/CountDown.hpp"
#include "main/menu/UnkMenuOverlay.hpp"


#pragma profile on
THUMB btl::BattleActorManager2::BattleActorManager2()
{
    escape_ = 0;
    escapeSuccess_ = 0;
    escapeCount_ = 0;
}

THUMB btl::BattleActorManager2::~BattleActorManager2(){

}

THUMB btl::BattleActorManager2* btl::BattleActorManager2::getSingleton()
{
    static BattleActorManager2 singleton;
    return &singleton;
}


THUMB void btl::BattleActorManager2::initialize()
{
    deathLog_ = 0;
    g_monster.setDropItem(0);
    g_monster.setDropItemMonster(-1);
}

THUMB void btl::BattleActorManager2::selectActor()
{
    actorCount_ = 0;
    selectActorPlayer();
    selectActorMonster();
    clearFirstAttack();
}


THUMB void btl::BattleActorManager2::selectActorPlayer()
{
    if (!g_Global.fightStadiumFlag_  && getFirstAttack() != 2 && escape_ == 0) {
        status::g_Party.setBattleMode();
        int count = status::g_Party.getCarriageOutCount();
        int found = 0;

        for (int i = 0; i < count; i++) {
            if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath())
                continue;
            if (status::g_Party.getPlayerStatus(i)->haveBattleStatus_.getSelectCommand() != 2)
                continue;
            if (status::g_Party.getPlayerStatus(i)->haveBattleStatus_.selectIndex_ != 0x13)
                continue;

            found = 1;
            actor_[actorCount_++].setup(status::g_Party.getPlayerStatus(i));
        }

        if (found == 0) {
            for (int i = 0; i < count; i++) {
                if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath())
                    continue;

                status::g_Party.getPlayerStatus(i)->print();
                actor_[actorCount_++].setup(status::g_Party.getPlayerStatus(i));
            }
        }
    }
}

THUMB void btl::BattleActorManager2::selectActorMonster()
{
    if (getFirstAttack() != FirstAttackPlayer || escape_ != 0) {
        if (eventFlag_ != 0) {
            if (eventType_ == Velorinman) {
                int count = g_monster.getCount();
                for (int i = 0; i < count; i++) {
                    if (g_monster.getMonsterStatus(i)->eventFlag_.flag_ & 1) {
                        actor_[actorCount_++].setup(g_monster.getMonsterStatus(i));
                    }
                }
            }
            else {
                int count = g_monster.getCount();
                for (int i = 0; i < count; i++) {
                    if (g_monster.getMonsterStatus(i)->haveStatusInfo_.isDeath())
                        continue;
                    g_monster.getMonsterStatus(i)->print();
                    actor_[actorCount_++].setup(g_monster.getMonsterStatus(i));
                }
            }
        }
        else {
            int count = g_monster.getCount();
            for (int i = 0; i < count; i++) {
                if (g_monster.getMonsterStatus(i)->haveStatusInfo_.isDeath())
                    continue;
                g_monster.getMonsterStatus(i)->print();
                actor_[actorCount_++].setup(g_monster.getMonsterStatus(i));
            }
        }
    }
}

THUMB btl::BattleActor2* btl::BattleActorManager2::add(status::CharacterStatus* chara)
{
    int actorCount = actorCount_++;
    actor_[actorCount].setup(chara);
    return &actor_[actorCount];
}

THUMB void btl::BattleActorManager2::setActorOrder()
{
    for (int i = 0; i < actorCount_; i++) {
        if (actor_[i].characterStatus_->characterType_ == 0 &&
            actor_[i].characterStatus_->haveBattleStatus_.actionIndex_ == 0x30) {
            actor_[i].agility_ = 100000;
        }
    }

    for (int j = 0; j < actorCount_ - 1; j++) {
        for (int i = 0; i < actorCount_ - 1; i++) {
            if (actor_[i].agility_ < actor_[i + 1].agility_) {
                dss::swap(&actor_[i], &actor_[i + 1]);
            }
        }
    }
}



THUMB void btl::BattleActorManager2::setActorAction()
{
    for (int i = 0; i < actorCount_; i++) {
        actor_[i].setAction();
        actor_[i].setTarget();
    }
}


THUMB int btl::BattleActorManager2::getActorCount()
{
    return actorCount_;
}

THUMB btl::BattleActor2* btl::BattleActorManager2::getBattleActor(int index)
{
    return &actor_[index];
}

THUMB void btl::BattleActorManager2::execStartOfRound()
{
    for (int i = 0; i < status::g_Party.getCount(); i++) {
        status::g_Party.getPlayerStatus(i)->haveStatusInfo_.setUseActionEffectValue(0);
        status::g_Party.getPlayerStatus(i)->haveStatusInfo_.setUseActionEffectValueMulti(0, 0);
        status::g_Party.getPlayerStatus(i)->haveStatusInfo_.setUseActionEffectValueMulti(1, 0);
        status::g_Party.getPlayerStatus(i)->haveStatusInfo_.addDamage_ = 0;
        status::g_Party.getPlayerStatus(i)->haveStatusInfo_.setStatusChangeRelease(0);
        status::g_Party.getPlayerStatus(i)->haveBattleStatus_.execStartOfRound();
        status::g_Party.getPlayerStatus(i)->setNoneAnimation();
        status::g_Party.getPlayerStatus(i)->damageSound_ = status::CharacterStatus::NoSe;
        status::g_Party.getPlayerStatus(i)->execStartOfRound();
    }

    for (int i = 0; i < g_monster.getCount(); i++) {
        g_monster.getMonsterStatus(i)->haveStatusInfo_.setUseActionEffectValue(0);
        g_monster.getMonsterStatus(i)->haveStatusInfo_.setUseActionEffectValueMulti(0, 0);
        g_monster.getMonsterStatus(i)->haveStatusInfo_.setUseActionEffectValueMulti(1, 0);
        g_monster.getMonsterStatus(i)->haveStatusInfo_.addDamage_ = 0;
        g_monster.getMonsterStatus(i)->haveStatusInfo_.setStatusChangeRelease(0);
        g_monster.getMonsterStatus(i)->haveBattleStatus_.execStartOfRound();
        g_monster.getMonsterStatus(i)->setNoneAnimation();
        g_monster.getMonsterStatus(i)->damageSound_ = status::CharacterStatus::NoSe;
        g_monster.getMonsterStatus(i)->execStartOfRound();
    }
}



THUMB void btl::BattleActorManager2::execEndOfRound()
{
    status::g_Party.setBattleMode();

    int count = status::g_Party.getCount();
    for (int i = 0; i < count; i++) {
        status::g_Party.getPlayerStatus(i)->haveStatusInfo_.statusChange_.execEndOfRound();
        ((status::HaveBattleStatus*)status::g_Party.getPlayerStatus(i))->clearGroupControl();
    }

    for (int i = 0; i < g_monster.getCount(); i++) {
        g_monster.getMonsterStatus(i)->haveStatusInfo_.statusChange_.execEndOfRound();
        ((status::HaveBattleStatus*)g_monster.getMonsterStatus(i))->clearGroupControl();
        g_monster.getMonsterStatus(i)->naturalRecovery();
    }

    if (dummyPlayer_.haveStatusInfo_.isMeganteRing()) {
        dummyPlayer_.haveStatusInfo_.setMeganteRing(false);
    }

    if (dummyPlayer_.haveStatusInfo_.isMegazaruRing()) {
        dummyPlayer_.haveStatusInfo_.setMegazaruRing(false);
    }

    status::g_BattleResult.battleTurnCount_++;
    escape_ = 0;
    escapeSuccess_ = 0;
}

THUMB void btl::BattleActorManager2::retireActor()
{
    if (eventFlag_ != 0) {
        if (eventType_ == Velorinman) {
            int count = g_monster.getCount();
            int flag = 0;
            for (int i = 0; i < count; i++) {
                if (g_monster.getMonsterStatus(i)->haveStatusInfo_.isDeath()) {
                    if (g_monster.getMonsterStatus(i)->eventFlag_.flag_ & 1) {
                        flag = 1;
                        break;
                    }
                    g_monster.getMonsterStatus(i)->haveStatusInfo_.setHp(200);
                }
            }
            if (flag) {
                for (int i = 0; i < count; i++) {
                    g_monster.getMonsterStatus(i)->haveStatusInfo_.setHp(0);
                }
                eventEnd_ = 1;
            } else {
                int min = 1000;
                for (int i = 0; i < count; i++) {
                    int hp = g_monster.getMonsterStatus(i)->haveStatusInfo_.getHp();
                    if (hp < min) {
                        min = hp;
                    }
                }
                for (int i = 0; i < count; i++) {
                    g_monster.getMonsterStatus(i)->haveStatusInfo_.setHp(min);
                }
                return;
            }
        }
        if (eventType_ == DeathPissaro) {
            if (!g_monster.getMonsterStatus(0)->haveStatusInfo_.isDeath()) {
                return;
            }
            if (!g_monster.getMonsterStatus(0)->haveStatusInfo_.isBossDeathFlag()) {
                return;
            }
        }
        if (eventType_ == EvilPriest) {
            if (!g_monster.getMonsterStatus(0)->haveStatusInfo_.isDeath()) {
                return;
            }
            if (!g_monster.getMonsterStatus(0)->haveStatusInfo_.isBossDeathFlag()) {
                return;
            }
        }
    }

    int count = g_monster.getCount();
    if (g_monster.getBattleCount() == 1) {
        checkDeathMonster();
    } else if (g_monster.getBattleCount() != 0) {
        checkDeathMonster();
    } else {
        execMonsterDeathForItem();
        checkDeathMonster();

        int ctrlIds[12];
        for (int i = 0; i < 12; i++) {
            ctrlIds[i] = -1;
        }
        for (int i = 0; i < count; i++) {
            ctrlIds[i] = g_monster.getCtrlId(i);
        }
        for (int i = 0; i < count; i++) {
            if (ctrlIds[i] != -1) {
                ((status::MonsterPartyWithDraw&)g_monster).status::MonsterPartyWithDraw::del(ctrlIds[i]);
            }
        }
    }
}

THUMB void btl::BattleActorManager2::checkDeathMonster()
{
    short log = 0;

    for (int i = 0; i < g_monster.getCount(); i++) {
        if (!g_monster.getMonsterStatus(i)->haveStatusInfo_.isDeath()) {
            continue;
        }

        int bit = 1 << g_monster.getCtrlId(i);
        log |= bit;

        if (deathLog_ & bit) {
            continue;
        }

        if (g_monster.getMonsterStatus(i)->haveStatusInfo_.isEscapeFlag()) {
            addMonsterEscapeCount(1);
        } else if (g_monster.getMonsterStatus(i)->haveStatusInfo_.isDisappearFlag()) {
            addMonsterDisappearCount(1);
        } else if (g_monster.getMonsterStatus(i)->haveStatusInfo_.isDeath()) {
            addMonsterDeathCount(1);
            execMonsterDeath(i);

            int w = winningStatus_;
            unsigned short index = w;

            switch (w & 0xffff0000) {
            case 0x10000:
                if (index != g_monster.getMonsterIndex(i)) {
                    winningStatus_ = 0x20000 | g_monster.getMonsterIndex(i);
                }
                break;
            case 0x20000:
                break;
            case 0x40000:
                if (index != g_monster.getMonsterIndex(i)) {
                    winningStatus_ = 0x20000 | g_monster.getMonsterIndex(i);
                } else {
                    winningStatus_ = 0x10000 | g_monster.getMonsterIndex(i);
                }
                break;
            default:
                winningStatus_ = 0x40000 | g_monster.getMonsterIndex(i);
                break;
            }
        }
    }

    deathLog_ = log;

    if (status::isCallMonster()) {
        clearDeadMonster(1);
        status::setCallMonster(0);
    }
}

THUMB void btl::BattleActorManager2::clearDeadMonster(int all)
{
    int count = g_monster.getCount();

    int ctrlIds[12];
    for (int i = 0; i < 12; i++) {
        ctrlIds[i] = -1;
    }

    for (int i = 0; i < count; i++) {
        if (g_monster.getMonsterStatus(i)->haveStatusInfo_.isDeath()) {
            ctrlIds[i] = g_monster.getCtrlId(i);
            if (all == 0) {
                if (!g_monster.getMonsterStatus(i)->haveStatusInfo_.isEscapeFlag()) {
                    if (!g_monster.getMonsterStatus(i)->haveStatusInfo_.isDisappearFlag()) {
                        ctrlIds[i] = -1;
                    }
                }
            }
        }
    }

    for (int i = 0; i < count; i++) {
        if (ctrlIds[i] != -1) {
            ((status::MonsterPartyWithDraw&)g_monster).status::MonsterPartyWithDraw::del(ctrlIds[i]);
            if (deathLog_ & (1 << ctrlIds[i])) {
                deathLog_ ^= 1 << ctrlIds[i];
            }
        }
    }
}

THUMB void btl::BattleActorManager2::execStartOfBattle()
{
    status::g_Party.setMemberShiftMode();

    int count = status::g_Party.getCount();
    for (int i = 0; i < count; i++) {
        status::g_Party.getPlayerStatus(i)->haveStatusInfo_.execStartOfBattle();
    }

    monsterDeathCount_ = 0;      // 0x66a8
    monsterEscapeCount_ = 0;     // 0x66ac
    monsterDisappearCount_ = 0;  // 0x66b0
    winningStatus_ = 0;          // 0x66b4
}

THUMB void btl::BattleActorManager2::execEndOfBattle()
{
    status::g_Party.setMemberShiftMode();

    int count = status::g_Party.getCount();
    for (int i = 0; i < count; i++) {
        status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath();
        status::g_Party.getPlayerStatus(i)->haveBattleStatus_.changeMonsterReverse();
        status::g_Party.getPlayerStatus(i)->haveStatusInfo_.statusChange_.execEndOfBattle();
        status::g_Party.getPlayerStatus(i)->haveStatusInfo_.clearDefenceChange();
        status::g_Party.getPlayerStatus(i)->haveStatusInfo_.clearAgilityChange();
    }
}

THUMB int btl::BattleActorManager2::isBattleEnd()
{
    if (status::g_Party.isDemolition()) {
        status::g_BattleResult.playerDemolition_ = 1;
        status::g_BattleResult.playerVictory_ = 0;
        return 1;
    }

    int count = 0;
    for (int i = 0; i < g_monster.getCount(); i++) {
        if (g_monster.getMonsterStatus(i)->isBattleEnable()) {
            count++;
        }
    }

    if (count == 0) {
        status::g_BattleResult.playerDemolition_ = 0;
        status::g_BattleResult.playerVictory_ = 1;
        return 1;
    }

    return 0;
}

THUMB void btl::BattleActorManager2::execMonsterDeath(int index)
{
    status::g_Party.battleMonsterCount_++;

    status::MonsterStatus* monster = g_monster.getMonsterStatus(index);
    status::g_Party.battleExp_ += monster->haveStatusInfo_.getExp();
    int gold = monster->haveStatusInfo_.getGold();
    status::g_Party.battleGold_ += gold;

    if (g_Global.fightStadiumFlag_ == 1) {
        return;
    }
    if (monster->haveStatusInfo_.isEscapeFlag() == 1) {
        return;
    }
    if (monster->haveStatusInfo_.isDisappearFlag() == 1) {
        return;
    }

    int characterIndex = monster->characterIndex_;
    int found = -1;
    for (int i = 0; i < 210; i++) {
        if (characterIndex == status::excelParam.bookData_[i].name) {
            found = i;
            break;
        }
    }

    if (found != -1) {
        status::g_BattleResult.setEncount(found, 1);
    }

    int wipeout = 0;
    int escape = 0;
    for (int h = 0; h < 3; h++) {
        int exec = 1;
        if (h == 0) {
            status::g_BattleHistory.historyType_ = status::BattleHistory::RightNow;
            wipeout = status::g_BattleHistory.getWipeoutCount();
            escape = status::g_BattleHistory.getEscapeCount();
        }
        if (h == 1) {
            status::g_BattleHistory.historyType_ = status::BattleHistory::WinDeathPissaro;
            if (eventType_ != DeathPissaro) {
                exec = 0;
            }
        }
        if (h == 2) {
            status::g_BattleHistory.historyType_ = status::BattleHistory::WinEvilPrist;
            if (eventType_ != EvilPriest) {
                exec = 0;
            }
        }
        if (exec == 1) {
            if (found != -1) {
                status::g_BattleResult.addMonsterCount(found);
                status::g_BattleResult.getMonsterCount(found);
                status::g_BattleResult.regenesisLevel(found, status::g_Party.getMaxLevel());
            }
            status::g_BattleHistory.regenesisTotalGold(gold);
            status::g_BattleHistory.regenesisMonsterCount();
            status::g_BattleHistory.regenesisMaxDamage(monster->haveStatusInfo_.effectValueMax_);
            if (h > 0) {
                status::g_BattleHistory.setWipeoutCount(wipeout);
                status::g_BattleHistory.setEscapeCount(escape);
                status::g_BattleHistory.regenesisChapterBattleCount();
            }
        }
    }
}

THUMB void btl::BattleActorManager2::execMonsterDeathForItem()
{
    if (g_Global.fightStadiumFlag_ == 1) {
        return;
    }

    int dropMonster = g_monster.getDropItemMonster();
    int found = -1;
    for (int i = 0; i < 210; i++) {
        if (dropMonster == status::excelParam.bookData_[i].name) {
            found = i;
            break;
        }
    }

    for (int h = 0; h < 3; h++) {
        int exec = 1;
        if (h == 0) {
            status::g_BattleHistory.historyType_ = status::BattleHistory::RightNow;
        }
        if (h == 1) {
            status::g_BattleHistory.historyType_ = status::BattleHistory::WinDeathPissaro;
            if (eventType_ != DeathPissaro) {
                exec = 0;
            }
        }
        if (h == 2) {
            status::g_BattleHistory.historyType_ = status::BattleHistory::WinEvilPrist;
            if (eventType_ != EvilPriest) {
                exec = 0;
            }
        }
        if (exec == 1) {
            status::g_BattleHistory.setAdventureTime(status::g_Game.getPlayTime());
            status::g_BattleResult.getEncountCount();
            status::g_Party.setAllPlayerMode();

            int heroIndex;
            if (status::g_Story.sex_ == 0) {
                heroIndex = status::g_Party.getSortIndex(1);
            } else {
                heroIndex = status::g_Party.getSortIndex(2);
            }

            status::g_BattleHistory.regenesisVictoryCount();

            if (heroIndex != -1) {
                status::g_BattleHistory.regenesisHeroLevel(
                    status::g_Party.getPlayerStatus(heroIndex)->haveStatusInfo_.haveStatus_.level_);
            }

            status::g_Party.setBattleMode();

            if (h > 0) {
                status::g_BattleHistory.regenesisChapterEscapeCount();
                status::g_BattleHistory.regenesisChapterWipeoutCount();
                unkfunc_02039460(14);
                cmn::PlayerTitle::setPlayerTitle(h);
            }
        }
    }

    if (g_monster.getDropItem() != 0) {
        if (found != -1) {
            status::g_BattleResult.addItemCount(found);
        }
    }
}

THUMB int btl::BattleActorManager2::isActionEnable()
{
    int count = 0;
    status::g_Party.setBattleMode();

    int partyCount = status::g_Party.getCount();
    for (int i = 0; i < partyCount; i++) {
        if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isActionEnable()) {
            count++;
        }
    }

    if (count != 0) {
        return 1;
    }
    return 0;
}


THUMB void btl::BattleActorManager2::setFirstAttack(FirstAttack firstAttack)
{
    firstAttack_ = firstAttack;
}

THUMB btl::FirstAttack btl::BattleActorManager2::getFirstAttack()
{
    return firstAttack_;
}

THUMB void btl::BattleActorManager2::clearFirstAttack()
{
    firstAttack_ = FirstAttackNone;
}

THUMB void btl::BattleActorManager2::setEscape(int escape)
{
    escape_ = escape;

    if (eventFlag_ != 0) {
        escapeSuccess_ = 0;
        return;
    }

    if (getFirstAttack() == FirstAttackPlayer) {
        escapeSuccess_ = 1;
        return;
    }

    int count = g_monster.getCount();
    int disableCount = 0;
    for (int i = 0; i < count; i++) {
        if (!g_monster.getMonsterStatus(i)->isActionEnable()) {
            disableCount++;
        }
    }
    if (disableCount == count) {
        escapeSuccess_ = 1;
        return;
    }

    int tileLevel = encount::Encount::getSingleton()->encountParam_.tileLevel_;
    int level = status::g_Party.getStoryPlayerStatus()->haveStatusInfo_.haveStatus_.level_;
    
        switch (status::g_Story.chapter_) {
    case 1:
        level = status::PartyStatus::getPlayerStatusForPlayerIndex(3)->haveStatusInfo_.haveStatus_.level_;
        break;
    case 2:
        level = status::PartyStatus::getPlayerStatusForPlayerIndex(4)->haveStatusInfo_.haveStatus_.level_;
        {
            int lv = status::PartyStatus::getPlayerStatusForPlayerIndex(5)->haveStatusInfo_.haveStatus_.level_;
            if (level < lv) {
                level = lv;
            }
        }
        {
            int lv = status::PartyStatus::getPlayerStatusForPlayerIndex(6)->haveStatusInfo_.haveStatus_.level_;
            if (level < lv) {
                level = lv;
            }
        }
        break;
    case 3:
        level = status::PartyStatus::getPlayerStatusForPlayerIndex(7)->haveStatusInfo_.haveStatus_.level_;
        break;
    case 4:
        level = status::PartyStatus::getPlayerStatusForPlayerIndex(8)->haveStatusInfo_.haveStatus_.level_;
        {
            int lv = status::PartyStatus::getPlayerStatusForPlayerIndex(9)->haveStatusInfo_.haveStatus_.level_;
            if (level < lv) {
                level = lv;
            }
        }
        break;
    case 5:
    case 6:
        level = status::PartyStatus::getPlayerStatusForPlayerIndex(1)->haveStatusInfo_.haveStatus_.level_;
        {
            int lv = status::PartyStatus::getPlayerStatusForPlayerIndex(2)->haveStatusInfo_.haveStatus_.level_;
            if (level < lv) {
                level = lv;
            }
        }
        break;
    }

    if (level - tileLevel >= 5) {
        escapeSuccess_ = 1;
        return;
    }

    switch (escapeCount_) {
    case 0:
        if (dssrand::rand(2) == 0) {
            escapeSuccess_ = 1;
        }
        break;
    case 1:
        if (dssrand::rand(2) == 0) {
            escapeSuccess_ = 1;
        }
        break;
    case 2:
        if (dssrand::rand(4) != 3) {
            escapeSuccess_ = 1;
        }
        break;
    case 3:
        escapeSuccess_ = 1;
        break;
    }

    escapeCount_++;
}

THUMB void btl::BattleActorManager2::setEventBattle(int flag, int tile)
{
    eventFlag_ = flag;
    eventTile_ = tile;
    eventEnd_ = 0;

    switch (eventTile_) {
    case 0xa3:
        eventType_ = Velorinman;
        g_monster.getMonsterStatus(0)->eventFlag_.flag_ |= 1;
        break;
    case 0xb9:
        eventType_ = DeathPissaro;
        g_monster.getMonsterStatus(0)->eventFlag_.flag_ |= 1;
        break;
    case 0xf1:
        eventType_ = EvilPriest;
        g_monster.getMonsterStatus(0)->eventFlag_.flag_ |= 1;
        break;
    default:
        eventType_ = EventNone;
        break;
    }
}

THUMB int btl::BattleActorManager2::isImpEventBattle()
{
    if (eventFlag_ == 0) {
        return 0;
    }
    if (eventEnd_ != 0) {
        return 0;
    }

    int result = 0;
    switch (eventType_) {
    case Velorinman:
        result = 1;
        break;
    case EvilPriest:
    case DeathPissaro:
        if (g_monster.getCount() == 1) {
            if (g_monster.getMonsterStatus(0)->haveStatusInfo_.isDeath()) {
                if (!g_monster.getMonsterStatus(0)->haveStatusInfo_.isBossDeathFlag()) {
                    result = 1;
                }
            }
        }
        break;
    }

    return result;
}

THUMB void btl::BattleActorManager2::addMonsterDeathCount(int count)
{
    monsterDeathCount_ += count;
}

THUMB int btl::BattleActorManager2::getMonsterDeathCount()
{
    return monsterDeathCount_;
}

THUMB void btl::BattleActorManager2::addMonsterEscapeCount(int count)
{
    monsterEscapeCount_ += count;
}


THUMB int btl::BattleActorManager2::getMonsterEscapeCount()
{
    return monsterEscapeCount_;
}

THUMB void btl::BattleActorManager2::addMonsterDisappearCount(int count)
{
    monsterDisappearCount_ += count;
}

THUMB int btl::BattleActorManager2::getMonsterDisappearCount()
{
    return monsterDisappearCount_;
}

THUMB void btl::BattleActorManager2::setMegazaruRing(status::UseActionParam* useActionParam)
{
    if (useActionParam->actionIndex_ != 514) {
        return;
    }

    status::CharacterStatus* targets[18];
    int count = 0;
    int targetCount = useActionParam->targetCount_;
    for (int i = 0; i < targetCount; i++) {
        targets[count++] = useActionParam->targetCharacterStatus_[i];
    }

    int newCount = 0;
    for (int i = 0; i < count; i++) {
        if (targets[i]->haveStatusInfo_.isMegazaruRing()) {
            targets[i]->haveStatusInfo_.setMegazaruRing(false);
        } else {
            useActionParam->targetCharacterStatus_[newCount] = targets[i];
            newCount++;
        }
    }

    useActionParam->targetCount_ = newCount;
}