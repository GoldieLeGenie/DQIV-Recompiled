#include "main/btl/BattleExecLevelup.hpp"
#include "main/dss/Pad.hpp"
#include "ov003/btl/BattleRootTask.hpp"
#include "ov003/btl/BattleRoot.hpp"
#include "ov003/btl/BattleRound.hpp"
#include "ov003/btl/BattleMessage.hpp"
#include "ov003/btl/BattleExecVictory.hpp"
#include "ov003/btl/BattleActorManager2.hpp"
#include "ov003/btl/AfterActionTask.hpp"
#include "ov003/btl/ExecMessageTask.hpp"
#include "ov003/status/MonsterParty.hpp"
#include "ov003/status/MonsterStatus.hpp"
#include "main/global/Global.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/BattleResult.hpp"
#include "main/status/BaseAction.hpp"
#include "main/status/StageStatus.hpp"
#include "main/task/PartTaskManager.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/sound/MenuSoundManager.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/MaterielMenu_SlotEnter.hpp"
#include "main/encount/Encount.hpp"
#include "main/text/TextAPI.hpp"
#include "main/cmn/CommonCounterInfo.hpp"

static int endTaskIndex;
static int levelupBgmCount;

THUMB void btl::EncountTask::initialize()
{
}

THUMB void btl::EncountTask::terminate()
{
}

THUMB void btl::EncountTask::execute()
{
    if (!g_BattleExecEncount.execute()) {
        g_PartTaskManager.setNextTask(2);
    }
}

THUMB void btl::FirstAttackTask::initialize()
{
}

THUMB void btl::FirstAttackTask::terminate()
{
}

THUMB void btl::FirstAttackTask::execute()
{
    if (!g_BattleExecFirstAttack.execute()) {
        g_PartTaskManager.setNextTask(1);
    }
}

THUMB void btl::StatusTask::initialize()
{
}

THUMB void btl::StatusTask::terminate()
{
}

THUMB void btl::StatusTask::execute()
{
    if (!g_BattleExecStatus.execute()) {
        if (status::g_Party.getAliveCountOutsideCarriagePlayerOnly() == 0) {
            g_PartTaskManager.setNextTaskWithSleep(9);
            return;
        }
        if (BattleActorManager2::getSingleton()->isImpEventBattle()) {
            g_PartTaskManager.setNextTask(0x10);
            return;
        }
        if (BattleActorManager2::getSingleton()->getFirstAttack() == 2) {
            g_PartTaskManager.setNextTask(0x14);
            return;
        }
        BattleActorManager2::getSingleton()->getFirstAttack();
        g_PartTaskManager.setNextTask(3);
    }
}

THUMB void btl::FirstReorderTask::initialize()
{
}

THUMB void btl::FirstReorderTask::terminate()
{
}

THUMB void btl::FirstReorderTask::execute()
{
    if (status::g_Party.getAliveCountOutsideCarriagePlayerOnly() == 0) {
        g_PartTaskManager.setNextTaskWithSleep(9);
        return;
    }
    g_PartTaskManager.setNextTask(4);
}

THUMB void btl::CommandTask::initialize()
{
    if (!status::g_Party.isPartyActionEnable()) {
        g_PartTaskManager.setNextTask(4);
        return;
    }
    MenuAPI::openBattleMenu();
}

THUMB void btl::CommandTask::terminate()
{
}

THUMB void btl::CommandTask::execute()
{
    if (g_Global.fightStadiumFlag_) {
        g_PartTaskManager.setNextTask(4);
        return;
    }
    if (status::g_Party.getAliveCountOutsideCarriagePlayerOnly() == 0) {
        g_PartTaskManager.setNextTaskWithSleep(9);
        return;
    }
    if (!status::g_Party.isPartyActionEnable()) {
        g_PartTaskManager.setNextTask(4);
        return;
    }
    if (MenuAPI::isFinishMenu()) {
        if (BattleActorManager2::getSingleton()->escape_) {
            g_PartTaskManager.setNextTask(0xf);
        } else {
            g_PartTaskManager.setNextTask(4);
        }
    }
}

THUMB void btl::RoundTask::initialize()
{
    battleRound_.initialize();
    if (!g_Global.fightStadiumFlag_) {
        MenuAPI::openBattleStadiumAbort();
    }
    if (status::g_Party.getAliveCountOutsideCarriagePlayerOnly() == 0) {
        g_PartTaskManager.setNextTaskWithSleep(9);
    }
}

THUMB void btl::RoundTask::terminate()
{
    battleRound_.terminate();
    MenuAPI::closeBattleStadiumAbort();
}

THUMB void btl::RoundTask::execute()
{
    if (g_Global.fightStadiumFlag_ && (dss::g_Pad.edge() & 2)) {
        waitFlag_ = 1;
    }
    if (battleRound_.isEnd()) {
        g_PartTaskManager.setNextTask(5);
        return;
    }
    battleRound_.execute();
    if (battleRound_.getCurrentTurn()->battleActor_->battleActorExec_.isActionEnd()) {
        if (status::g_Party.isDemolition()) {
            if (status::HaveStatusInfo::isGlbMeganteRing() || battleRound_.isMegazaruRingEnable()) {
                return;
            }
            g_PartTaskManager.setNextTask(5);
            return;
        }
        if (status::g_Party.getAliveCountOutsideCarriagePlayerOnly() == 0) {
            if (status::HaveStatusInfo::isGlbMeganteRing() || battleRound_.isMegazaruRingEnable()) {
                return;
            }
            g_PartTaskManager.setNextTaskWithSleep(9);
            return;
        }
    }
    if (battleRound_.isEnd()) {
        g_PartTaskManager.setNextTask(5);
        return;
    }
    if (battleRound_.getCurrentTurn()->battleActor_->battleActorExec_.isActionEnd()) {
        if (status::BaseAction::timeReverseFlag_) {
            g_PartTaskManager.setNextTask(0xd);
            return;
        }
        if (waitFlag_) {
            g_PartTaskManager.setNextTaskWithSleep(0x11);
            waitFlag_ = 0;
        }
    }
}

THUMB void btl::RoundEndTask::initialize()
{
}

THUMB void btl::RoundEndTask::terminate()
{
}

THUMB void btl::RoundEndTask::execute()
{
    if (g_Global.fightStadiumFlag_) {
        if (g_monster.getBattleCount() == 1) {
            g_PartTaskManager.setNextTask(0x13);
            return;
        }
        if (status::g_BattleResult.battleTurnCount_ >= 10) {
            g_PartTaskManager.setNextTask(0x12);
            return;
        }
    }
    if (BattleActorManager2::getSingleton()->isBattleEnd()) {
        if (status::g_BattleResult.playerDemolition_) {
            if (status::g_BattleResult.isDisablePlayerDemolition()) {
                g_PartTaskManager.setNextTask(7);
                return;
            }
            g_PartTaskManager.setNextTask(0xa);
            return;
        }
        g_PartTaskManager.setNextTask(6);
        if (g_Global.fightStadiumFlag_) {
            g_PartTaskManager.setNextTask(7);
        }
    } else if (BattleActorManager2::getSingleton()->isActionEnable()) {
        if (BattleActorManager2::getSingleton()->isImpEventBattle()) {
            g_PartTaskManager.setNextTask(0x10);
        } else {
            g_PartTaskManager.setNextTask(3);
        }
    } else {
        g_PartTaskManager.setNextTask(4);
    }
    if (BattleActorManager2::getSingleton()->isImpEventBattle()) {
        g_PartTaskManager.setNextTask(0x10);
    }
}

THUMB void btl::BattleEndTask::initialize()
{
    SoundManager::battleStop();
    SoundManager::playSe(0x199, 0);
    BattleActorManager2::getSingleton()->execEndOfBattle();
    int escapeCount = BattleActorManager2::getSingleton()->getMonsterEscapeCount();
    int deathCount = BattleActorManager2::getSingleton()->getMonsterDeathCount();
    int disappearCount = BattleActorManager2::getSingleton()->getMonsterDisappearCount();
    if (escapeCount != 0 && deathCount == 0) {
        endTaskIndex = 0;
        return;
    }
    if (disappearCount != 0 && deathCount == 0) {
        endTaskIndex = 7;
        return;
    }
    endTaskIndex = 1;
}

THUMB void btl::BattleEndTask::terminate()
{
    endTaskIndex = 0;
}

THUMB void btl::BattleEndTask::execute()
{
    if (endTaskIndex == 0 && !g_BattleExecMonsterEscape.execute()) {
        g_BattleExecMonsterEscape.terminate();
        g_PartTaskManager.setNextTask(7);
    }
    if (endTaskIndex == 7 && !g_BattleExecMonsterDisappear.execute()) {
        g_BattleExecMonsterDisappear.terminate();
        g_PartTaskManager.setNextTask(7);
    }
    if (endTaskIndex == 1 && !g_BattleExecDefeatMonster.execute()) {
        g_BattleExecDefeatMonster.terminate();
        if (status::g_Party.getLevelupPlayer() != -1) {
            endTaskIndex = 2;
            levelupBgmCount = 0;
            SoundManager::stopBgm(0);
            SoundManager::playBgm(0x2f, 0);
        } else {
            endTaskIndex = 4;
        }
    }
    if (endTaskIndex == 2) {
        if (++levelupBgmCount == 180) {
            endTaskIndex = 3;
        }
    }
    if (endTaskIndex == 3 && !g_BattleExecLevelup.execute()) {
        g_BattleExecLevelup.terminate();
        if (status::g_Party.getLevelupPlayer() != -1) {
            endTaskIndex = 3;
        } else {
            endTaskIndex = 4;
        }
    }
    if (endTaskIndex == 4 && !g_BattleExecGold.execute()) {
        g_BattleExecGold.terminate();
        endTaskIndex = 5;
    }
    if (endTaskIndex == 5) {
        if (g_monster.getDropItem()) {
            if (!g_BattleExecItem.execute()) {
                g_BattleExecItem.terminate();
                endTaskIndex = 0;
                g_PartTaskManager.setNextTask(7);
            }
        } else {
            endTaskIndex = 6;
            g_PartTaskManager.setNextTask(7);
        }
    }
}

THUMB void btl::ExitTask::initialize()
{
}

THUMB void btl::ExitTask::terminate()
{
    if (status::g_BattleResult.playerVictory_) {
        g_Global.endBattle(false);
    } else if (status::g_BattleResult.playerDemolition_) {
        if (status::g_BattleResult.isDisablePlayerDemolition()) {
            status::g_Party.recoveryDisableDemolition();
            g_Global.endBattle(false);
        } else {
            g_Global.endBattle(true);
        }
    } else {
        g_Global.endBattle(false);
    }
    SoundManager::finalFormBGM_ = 0;
}

THUMB void btl::ExitTask::execute()
{
    g_PartTaskManager.setNextTask(8);
}

THUMB void btl::ExitWaitTask::initialize()
{
    BattleActorManager2::getSingleton()->setEscape(0);
}

THUMB void btl::ExitWaitTask::terminate()
{
}

THUMB void btl::ExitWaitTask::execute()
{
}

THUMB void btl::PartyReorderTask::initialize()
{
    gBattleMenuSub_HISTORY.update_ = 1;
}

THUMB void btl::PartyReorderTask::terminate()
{
    gBattleMenuSub_HISTORY.update_ = 0;
}

THUMB void btl::PartyReorderTask::execute()
{
    if (!g_BattleExecReorder.execute()) {
        g_BattleExecReorder.terminate();
        g_PartTaskManager.wakeup();
        if (g_PartTaskManager.getCurrentTask() == 3) {
            MenuAPI::openBattleMenu();
        }
    }
}

THUMB void btl::DemolitionTask::initialize()
{
    SoundManager::battleStop();
}

THUMB void btl::DemolitionTask::terminate()
{
}

THUMB void btl::DemolitionTask::execute()
{
    if (!g_BattleExecDemolition.execute()) {
        g_BattleExecDemolition.terminate();
        g_PartTaskManager.setNextTask(7);
    }
}

THUMB void btl::CrusingTask::initialize()
{
    if (encount::Encount::getSingleton()->battleMode_ == 1) {
        g_Stage.crusingPeopleEncount_ = 1;
        MaterielMenu_WINDOW_MANAGER::getSingleton()->extraInnType_ = 2;
        MaterielMenu_WINDOW_MANAGER::getSingleton()->openMaterielWindow(4);
    }
    if (encount::Encount::getSingleton()->battleMode_ == 2) {
        g_Stage.crusingPeopleEncount_ = 1;
        MaterielMenu_WINDOW_MANAGER::getSingleton()->extraInnType_ = 2;
        MaterielMenu_WINDOW_MANAGER::getSingleton()->openMaterielWindow(0);
    }
}

THUMB void btl::CrusingTask::terminate()
{
}

THUMB void btl::CrusingTask::execute()
{
    if (MaterielMenu_WINDOW_MANAGER::getSingleton()->endWindow_) {
        g_PartTaskManager.setNextTask(0xc);
    }
}

THUMB void btl::CrusingEndTask::initialize()
{
    g_Global.endBattle(false);
}

THUMB void btl::CrusingEndTask::terminate()
{
}

THUMB void btl::CrusingEndTask::execute()
{
}

THUMB void btl::TimeReverseTask::initialize()
{
    int count = g_monster.getCount();
    for (int i = 0; i < count; i++) {
        int ctrlId = g_monster.getMonsterStatus(i)->haveStatusInfo_.drawCtrlId_;
        btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].startAnimation(0, 0x1f);
    }
    counter_ = 0;
}

THUMB void btl::TimeReverseTask::terminate()
{
    status::MonsterParty* monster = &g_monster;
    monster->clear();
    SoundManager::resetLastBossPlay();
    SoundManager::playStart(SoundManager::bgmIndex_, 0xf);
}

THUMB void btl::TimeReverseTask::execute()
{
    if (++counter_ > 30) {
        g_PartTaskManager.setNextTask(0xe);
    }
}

THUMB void btl::TimeReverseEndTask::initialize()
{
}

THUMB void btl::TimeReverseEndTask::terminate()
{
    BattleRoot::getSingleton()->cleanupMonster();
    BattleRoot::getSingleton()->restore();
    BattleRoot::getSingleton()->setupMonster();
    
    btl::BattleMonsterDraw2::getSingleton()->setup();
    BattleActorManager2::getSingleton()->initialize();
}

THUMB void btl::TimeReverseEndTask::execute()
{
    g_PartTaskManager.setNextTask(0);
}

THUMB void btl::EscapeTask::initialize()
{
}

THUMB void btl::EscapeTask::terminate()
{
}

THUMB void btl::EscapeTask::execute()
{
    if (!g_BattleExecEscape.execute()) {
        if (BattleActorManager2::getSingleton()->escapeSuccess_) {
            g_PartTaskManager.setNextTask(7);
        } else {
            g_PartTaskManager.setNextTask(4);
        }
    }
}

THUMB void btl::EventTask::initialize()
{
}

THUMB void btl::EventTask::terminate()
{
}

THUMB void btl::EventTask::execute()
{
    if (BattleActorManager2::getSingleton()->eventType_ == BattleActorManager2::Velorinman) {
        if (!g_BattleExecVelorinman.execute()) {
            g_PartTaskManager.setNextTask(3);
        }
        return;
    }
    if (BattleActorManager2::getSingleton()->eventType_ == BattleActorManager2::DeathPissaro) {
        if (!g_BattleExecDeathPissaro.execute()) {
            g_BattleExecDeathPissaro.terminate();
            if (g_BattleExecDeathPissaroMahokanta.flag_) {
                g_PartTaskManager.setNextTask(0x15);
                return;
            }
            g_PartTaskManager.setNextTask(5);
        }
        return;
    }
    if (BattleActorManager2::getSingleton()->eventType_ == BattleActorManager2::EvilPriest) {
        if (!g_BattleExecEvilPriest.execute()) {
            g_BattleExecEvilPriest.terminate();
            g_PartTaskManager.setNextTask(5);
        }
        return;
    }
    g_PartTaskManager.setNextTask(3);
}

THUMB void btl::EventTask2::initialize()
{
}

THUMB void btl::EventTask2::terminate()
{
}

THUMB void btl::EventTask2::execute()
{
    if (BattleActorManager2::getSingleton()->eventType_ == BattleActorManager2::DeathPissaro) {
        if (g_BattleExecDeathPissaroMahokanta.flag_ && !g_BattleExecDeathPissaroMahokanta.execute()) {
            g_BattleExecDeathPissaroMahokanta.terminate();
            g_BattleExecDeathPissaroMahokanta.flag_ = 0;
            g_PartTaskManager.setNextTask(5);
        }
        return;
    }
    g_PartTaskManager.setNextTask(3);
}

THUMB void btl::StadiumEndTask::initialize()
{
}

THUMB void btl::StadiumEndTask::terminate()
{
}

THUMB void btl::StadiumEndTask::execute()
{
    if (messageCount_ == 0) {
        data_020ed1bc.openMessageForBATTLE();
        data_020ed1bc.addMessage(0xc8f00);
        data_020ed1bc.setMessageCursor(false);
        data_020ed1bc.setYesNo(1);
        messageCount_++;
    } else if (messageCount_ == 1) {
        if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
            data_020ed1bc.close();
            data_020ed1bc.openMessageForBATTLE();
            data_020ed1bc.addMessage(0xc8f03);
            data_020ed1bc.setMessageCursor(false);
            messageCount_++;
            messageCount_++;
        } else if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            data_020ed1bc.openMessageForBATTLE();
            data_020ed1bc.addMessage(0xc8f07);
            data_020ed1bc.setMessageCursor(false);
            messageCount_++;
        }
    } else if (messageCount_ == 2) {
        if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK || data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            status::g_BattleResult.battleTurnCount_--;
            g_PartTaskManager.wakeup();
            messageCount_ = 0;
            return;
        }
    } else if (messageCount_ == 3) {
        if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK || data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            g_Global.fightStadiumResult_ = 4;
            g_PartTaskManager.setNextTask(7);
            return;
        }
    }
    g_PartTaskManager.setNextTask(0x11);
}

THUMB void btl::StadiumDrawTask::initialize()
{
}

THUMB void btl::StadiumDrawTask::terminate()
{
}

THUMB void btl::StadiumDrawTask::execute()
{
    if (messageCount_ == 0) {
        int betOnIndex = g_Global.betOnIndex_;
        int draw = 1;
        for (int i = 0; i < g_monster.getCount(); i++) {
            if (betOnIndex == g_monster.getMonsterStatus(i)->characterGroup_ && g_monster.getMonsterStatus(i)->isBattleEnable()) {
                draw = 0;
            }
        }

        int playerIndex = 0;
        status::g_Party.setBattleMode();
        for (int i = 0; i < status::g_Party.getCount(); i++) {
            if (!status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath()) {
                playerIndex = status::g_Party.getPlayerIndex(i);
                break;
            }
        }

        data_020ed1bc.openMessageForBATTLE();
        TextAPI::setMACRO0(0xc, 0x50000000, playerIndex);
        if (draw) {
            g_Global.fightStadiumResult_ = 2;
            data_020ed1bc.addMessage(0xc8f0b, 0xc8f0c, 0xc8f12, 0xc8f13);
        } else {
            g_Global.fightStadiumResult_ = 3;
            data_020ed1bc.addMessage(0xc8f0b, 0xc8f0c, 0xc8f0f);
        }
        data_020ed1bc.setMessageLastCursor(false);
        messageCount_++;
    } else if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK || data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
        data_020ed1bc.close();
        g_PartTaskManager.setNextTask(7);
        return;
    }
    g_PartTaskManager.setNextTask(0x12);
}

THUMB void btl::StadiumResultTask::initialize()
{
}

THUMB void btl::StadiumResultTask::terminate()
{
}

THUMB void btl::StadiumResultTask::execute()
{
    if (messageCount_ == 0) {
        MenuSoundManager::getSingleton()->initialize();
        int betOnIndex = g_Global.betOnIndex_;
        int monsterID = g_Global.betMonsterID_;
        int monsterSymbol = g_Global.betMonsterSymbol_;
        int lose = 1;
        for (int i = 0; i < g_monster.getCount(); i++) {
            if (g_monster.getMonsterStatus(i)->isBattleEnable() && betOnIndex == g_monster.getMonsterStatus(i)->characterGroup_) {
                lose = 0;
            }
        }

        data_020ed1bc.openMessageForBATTLE();
        TextAPI::setMACRO0(3, 0x60000000, monsterID, monsterSymbol);
        if (lose) {
            data_020ed1bc.addMessage(0xc8efb);
            g_Global.fightStadiumResult_ = 2;
        } else {
            data_020ed1bc.addMessage(0xc8eec);
            int coin = g_Global.betCoin_ * g_Global.diameter_;
            if (coin % 10 > 4) {
                coin += 10;
            }
            coin /= 10;
            if (coin >= 1000) {
                MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_FANFARE_L);
            } else if (coin >= 500) {
                MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_FANFARE_M);
            } else {
                MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_FANFARE_S);
            }
            g_Global.fightStadiumResult_ = 1;
        }
        data_020ed1bc.setMessageLastCursor(false);
        messageCount_++;
    } else if (!MenuSoundManager::getSingleton()->isPlaySound()
               && (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK || data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL)) {
        data_020ed1bc.close();
        g_PartTaskManager.setNextTask(7);
        return;
    }
    g_PartTaskManager.setNextTask(0x13);
}
