#pragma ipa file
#include "ov003/btl/BattleRoot.hpp"
#include "main/dss/DssUtils.hpp"
#include "ov003/btl/BattleActorManager2.hpp"
#include "ov003/btl/BattleActorExec.hpp"
#include "ov003/btl/BattleMonsterMask.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"
#include "ov003/status/MonsterStatus.hpp"
#include "ov015/btl/BattleMenuPlayerControl.hpp"
#include "main/data/DataObject.hpp"
#include "main/dss/Random.hpp"
#include "main/encount/Encount.hpp"
#include "main/global/Global.hpp"
#include "main/profile/Profile.hpp"
#include "main/status/BaseAction.hpp"
#include "main/status/BaseActionStatus.hpp"
#include "main/status/BattleHistory.hpp"
#include "main/status/BattleResult.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/HaveAction.hpp"
#include "main/status/HaveBattleStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/task/PartTaskManager.hpp"

btl::BattleRound battleRound_;

THUMB btl::BattleRoot::BattleRoot()
{
}

THUMB btl::BattleRoot::~BattleRoot()
{
}

THUMB btl::BattleRoot* btl::BattleRoot::getSingleton()
{
    static BattleRoot m_singleton;
    return &m_singleton;
}

THUMB void btl::BattleRoot::initialize()
{
    status::HaveAction::setBattleMode();
    if (encount::Encount::getSingleton()->battleMode_ == encount::Encount::Normal) {
        setupBattle();
    }
    else {
        setupCrusingMenu();
    }
    unkfunc_0207e7e4();
}

THUMB void btl::BattleRoot::terminate()
{
    if (encount::Encount::getSingleton()->battleMode_ == encount::Encount::Normal) {
        cleanupBattle();
    }
    else {
        cleanupCrusingMenu();
    }

    switch (data_020f21f8.state_) {
    case GlobalFade::FADE_NONE:
        data_020f21f8.state_ = GlobalFade::FADE_OUT_BLACK;
        break;
    case GlobalFade::FADE_IN_BLACK:
        data_020f21f8.state_ = GlobalFade::FADE_OUT_WHITE;
        break;
    default:
        data_020f21f8.state_ = GlobalFade::FADE_OUT_BLACK;
        break;
    }
    data_020f21f8.count_ = 0;
    data_020f21f8.frames_ = 3;
    data_0210bc18.unkfunc_02058294(&data_020f21f8);
}

THUMB void btl::BattleRoot::execute()
{
    g_PartTaskManager.run();
    unkfunc_0207e7e4();
}

THUMB void btl::BattleRoot::draw()
{
}

THUMB void btl::BattleRoot::setupBattle()
{
    BattleActorManager2::getSingleton()->initialize();
    eventEncount_ = encount::Encount::getSingleton()->encountParam_.isEventEncount();
    setupMonster();

    g_PartTaskManager.registerTask(0, &encountTask_);
    g_PartTaskManager.registerTask(1, &statusTask_);
    g_PartTaskManager.registerTask(2, &firstAttackTask_);
    g_PartTaskManager.registerTask(3, &commandTask_);
    g_PartTaskManager.registerTask(4, &roundTask_);
    g_PartTaskManager.registerTask(5, &roundEndTask_);
    g_PartTaskManager.registerTask(6, &battleEndTask_);
    g_PartTaskManager.registerTask(7, &exitTask_);
    g_PartTaskManager.registerTask(8, &exitWaitTask_);
    g_PartTaskManager.registerTask(9, &partyReorderTask_);
    g_PartTaskManager.registerTask(10, &demolitionTask_);
    g_PartTaskManager.registerTask(13, &timeReverseTask_);
    g_PartTaskManager.registerTask(14, &timeReverseEndTask_);
    g_PartTaskManager.registerTask(15, &escapeTask_);
    g_PartTaskManager.registerTask(16, &eventTask_);
    g_PartTaskManager.registerTask(20, &firstReorderTask_);
    g_PartTaskManager.registerTask(21, &eventTask2_);
    if (g_Global.fightStadiumFlag_) {
        g_PartTaskManager.registerTask(17, &stadiumEndTask_);
        g_PartTaskManager.registerTask(18, &stadiumDrawTask_);
        g_PartTaskManager.registerTask(19, &stadiumResultTask_);
        stadiumEndTask_.messageCount_ = 0;
        stadiumDrawTask_.messageCount_ = 0;
        stadiumResultTask_.messageCount_ = 0;
        roundTask_.waitFlag_ = 0;
    }
    g_PartTaskManager.initialize();
    g_PartTaskManager.setNextTask(0);

    if (!g_Global.fightStadiumFlag_) {
        dss::memset(BattleMenuPlayerControl::getSingleton()->targetMonsterGroup_, 0, sizeof(BattleMenuPlayerControl::getSingleton()->targetMonsterGroup_));
        BattleMenuPlayerControl::getSingleton()->allClear();
        BattleMenuPlayerControl::getSingleton()->activeChara_ = -1;
        BattleMonsterMask::getSingleton()->setup();
    }
}

THUMB void btl::BattleRoot::cleanupBattle()
{
    cleanupMonster();
    if (status::g_BattleResult.playerDemolition_ && status::g_BattleResult.isDisablePlayerDemolition()) {
        status::g_BattleResult.playerDemolitionMessage_ = 1;
        status::g_BattleResult.setDisablePlayerDemolition(false);
        status::g_Party.getPlayerStatus(0)->haveStatusInfo_.revival();
    }
    status::g_BattleResult.setDisablePlayerDemolition(false);
}

THUMB void btl::BattleRoot::setupMonster()
{
    backupPartyStatus_ = (status::PartyStatus*)unkfunc_0207f834(&data_0211a60c, sizeof(status::PartyStatus), 0x20);
    backupPlayerStatus_ = (status::PlayerStatus*)unkfunc_0207f834(&data_0211a60c, sizeof(status::PlayerStatus) * 26, 0x20);
    backupPlayerFlag_ = (status::PlayerFlag*)unkfunc_0207f834(&data_0211a60c, sizeof(status::PlayerFlag) * 26, 0x20);
    store();
    status::MonsterParty::initializeSortIndex();

    int i;
    int j;
    for (i = 0; i < 4; i++) {
        int monsterIndex = encount::Encount::getSingleton()->monsterIndex_[i];
        int monsterCount = encount::Encount::getSingleton()->monsterCount_[i];
        for (j = 0; j < monsterCount; j++) {
            if (monsterCount != 0) {
                g_monster.add(i, monsterIndex, 1);
                if (!g_Global.fightStadiumFlag_) {
                    for (int k = 0; k < 210; k++) {
                        if (monsterIndex == status::excelParam.bookData_[k].name) {
                            status::g_BattleResult.setEncount(k, 1);
                        }
                    }
                }
            }
        }
    }

    if (eventEncount_ == 0) {
        int count = g_monster.getCount();
        for (int i = 0; i < count; i++) {
            unsigned short hpMax = g_monster.getMonsterStatus(i)->haveStatusInfo_.getHpMax();
            unsigned short hp = hpMax * (unsigned short)(dssrand::rand(25) + 76) / 100;
            if (hp == 0) {
                hp = 1;
            }
            g_monster.getMonsterStatus(i)->haveStatusInfo_.setHp(hp);
        }
    }

    int count = g_monster.getCount();
    for (int i = 0; i < count; i++) {
        g_monster.getMonsterStatus(i)->setStartStatus();
    }

    status::g_Party.setBattleMode();
    count = status::g_Party.getCount();
    for (int i = 0; i < count; i++) {
        int noDamage = status::PartyStatus::noDamageEnable_;
        status::g_Party.getPlayerStatus(i)->haveStatusInfo_.noDamage_ = noDamage;
    }
    count = g_monster.getCount();
    for (int i = 0; i < count; i++) {
        int noDamage = status::PartyStatus::noDamageEnableForMonster_;
        g_monster.getMonsterStatus(i)->haveStatusInfo_.noDamage_ = noDamage;
    }

    int tileId = encount::Encount::getSingleton()->tileId_;
    status::HaveBattleStatus::eventFlag_ = eventEncount_;
    BattleActorManager2::getSingleton()->setEventBattle(eventEncount_, tileId);
    status::BaseAction::eventBattle_ = eventEncount_;
    status::BaseActionStatus::eventBattle_ = eventEncount_;
    status::g_Party.startBattle();

    BattleActorManager2::getSingleton()->setFirstAttack(encount::Encount::getSingleton()->getFirstAttack());
    if (status::g_BattleResult.monsterFirstAttack_) {
        BattleActorManager2::getSingleton()->setFirstAttack((FirstAttack)2);
    }
    if (status::g_BattleResult.playerFirstAttack_) {
        BattleActorManager2::getSingleton()->setFirstAttack((FirstAttack)1);
    }
    if ((eventEncount_ || g_Global.fightStadiumFlag_) && !status::g_BattleResult.monsterFirstAttack_) {
        BattleActorManager2::getSingleton()->setFirstAttack((FirstAttack)0);
    }

    status::g_Party.battleExp_ = 0;
    status::g_Party.battleMonsterCount_ = 0;
    status::g_Party.battleGold_ = 0;
    status::g_BattleResult.playerVictory_ = 0;
    status::g_BattleResult.playerDemolition_ = 0;
    status::g_BattleResult.battleTurnCount_ = 0;
    status::g_BattleResult.playerFirstAttack_ = 0;
    status::g_BattleResult.monsterFirstAttack_ = 0;
    status::BaseAction::clear();
    BattleActorManager2::getSingleton()->execStartOfBattle();
    if (!g_Global.fightStadiumFlag_) {
        status::g_BattleHistory.historyType_ = status::BattleHistory::RightNow;
        status::g_BattleHistory.regenesisChapterBattleCount();
    }
}

THUMB void btl::BattleRoot::cleanupMonster()
{
    BattleActorManager2::getSingleton()->execEndOfBattle();
    status::BaseAction::doubleFlag_ = 0;
    status::BaseAction::allKaishinFlag_ = 0;
    g_monster.clear();
    unkfunc_0207f840(&data_0211a60c, backupPartyStatus_);
    unkfunc_0207f840(&data_0211a60c, backupPlayerStatus_);
    unkfunc_0207f840(&data_0211a60c, backupPlayerFlag_);
}

THUMB void btl::BattleRoot::setupCrusingMenu()
{
    g_PartTaskManager.registerTask(11, &crusingTask_);
    g_PartTaskManager.registerTask(12, &crusingEndTask_);
    g_PartTaskManager.registerTask(7, &exitTask_);
    g_PartTaskManager.registerTask(8, &exitWaitTask_);
    g_PartTaskManager.setNextTask(11);
    if (encount::Encount::getSingleton()->battleMode_ == encount::Encount::CrusingTrader) {
        g_monster.add(0, 0xad, 1);
    }
    if (encount::Encount::getSingleton()->battleMode_ == encount::Encount::CrusingInnKeeper) {
        g_monster.add(0, 0xf7, 1);
    }
}

THUMB void btl::BattleRoot::cleanupCrusingMenu()
{
    g_monster.clear();
}

THUMB void btl::BattleRoot::store()
{
    dss::memcpy(backupPartyStatus_, &status::g_Party, sizeof(status::PartyStatus));
    for (int i = 0; i < 26; i++) {
        dss::memcpy(&backupPlayerStatus_[i], &originalPlayer_[i], sizeof(status::PlayerStatus));
        dss::memcpy(&backupPlayerFlag_[i], &originalPlayerFlag_[i], sizeof(status::PlayerFlag));
    }
}

THUMB void btl::BattleRoot::restore()
{
    dss::memcpy(&status::g_Party, backupPartyStatus_, sizeof(status::PartyStatus));
    for (int i = 0; i < 26; i++) {
        dss::memcpy(&originalPlayer_[i], &backupPlayerStatus_[i], sizeof(status::PlayerStatus));
        dss::memcpy(&originalPlayerFlag_[i], &backupPlayerFlag_[i], sizeof(status::PlayerFlag));
    }
}
