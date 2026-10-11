#pragma ipa file
#include "main/status/BaseAction.hpp"
#include "main/status/BaseActionStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/BasePartyStatus.hpp"
#include "ov003/status/MonsterParty.hpp"
#include "ov015/btl/BattleSelectTarget.hpp"


status::UseActionParam* status::BaseAction::useActionParam_;
int status::BaseAction::mirrorDamage_;
int status::BaseAction::doubleFlag_;
int status::BaseAction::splitFlag_;
int status::BaseAction::joukFlag_;
int status::BaseAction::timeReverseFlag_;
int status::BaseAction::allKaishinFlag_;
volatile int status::BaseAction::kaishinFlag_;
int status::BaseAction::tsukonFlag_;
int status::BaseAction::tsukon2Flag_;
int status::BaseAction::missFlag_;
int status::BaseAction::eventBattle_;
int status::BaseAction::multiFlag_;
int status::BaseAction::flag_;
param::ActionParam* status::BaseAction::actionParam_;
status::BaseActionWorkParam status::BaseAction::workParam_;
int status::BaseAction::callMonster_[4];

const SplitJoukTable splitJoukTable = { { 9, 5, 2 } };



#pragma profile on

THUMB status::BaseAction::BaseAction()
    : actionIndex_(0), resultFlag_(0), playerEffectValue_(0), monsterEffectValue_(0)
{
}

THUMB status::BaseAction::~BaseAction(){
    return;
}

THUMB void status::BaseAction::initialize()
{
    status::BaseAction::actionParam_ = excelParam.actionParam_;
}


THUMB void status::BaseAction::clear()
{
    status::BaseAction::doubleFlag_ = 0;
    status::BaseAction::splitFlag_ = 0;
    status::BaseAction::joukFlag_ = 0;
    status::BaseAction::timeReverseFlag_ = 0;
    status::BaseAction::allKaishinFlag_ = 0;
}

THUMB void status::BaseAction::exec(status::UseActionParam& useActionParam, int flag)
{
    int i;
    this->executeAction_ = 0;
    status::BaseAction::splitFlag_ = 0;
    status::BaseAction::joukFlag_ = 0;
    status::BaseAction::timeReverseFlag_ = 0;
    status::BaseAction::multiFlag_ = 0;
    this->rollingKick_ = 0;
    this->sleepTurn_ = 0;
    status::BaseAction::tsukonFlag_ = 0;
    status::BaseAction::tsukon2Flag_ = 0;
    status::BaseAction::flag_ = 0;
    status::BaseAction::workParam_.value_ = 0;

    for (int i = 0; i < 4; i++) {
        status::BaseAction::callMonster_[i] = 0;
    }
    status::BaseActionStatus::flag_.flag_ = 0;

    this->actionIndex_ = useActionParam.actionIndex_;
    status::BaseAction::useActionParam_ = &useActionParam;
    message_.setup(&useActionParam);

    status::CharacterStatus* actor = useActionParam.actorCharacterStatus_;
    if (actor) {
        actor->haveStatusInfo_.clearHpInBattle();
        actor->haveStatusInfo_.clearMpInBattle();
        actor->haveStatusInfo_.clearStatusChangeInBattle();
    }

    int clearCount = status::BaseAction::useActionParam_->targetCount_;
    for (int i = 0; i < clearCount; i++) {
        status::CharacterStatus* target = useActionParam.targetCharacterStatus_[i];
        if (target) {
            target->haveStatusInfo_.clearHpInBattle();
            target->haveStatusInfo_.clearMpInBattle();
            target->haveStatusInfo_.clearStatusChangeInBattle();
        }
    }

    if (status::BaseAction::useActionParam_->actorHaveItemSack_) {
        status::CharacterStatus* a = status::BaseAction::useActionParam_->actorCharacterStatus_;
        if (a) {
            a->haveStatusInfo_.clearHpInBattle();
            a->haveStatusInfo_.clearMpInBattle();
            a->haveStatusInfo_.clearStatusChangeInBattle();
        }
        int count = status::BaseAction::useActionParam_->targetCount_;
        for (i = 0; i < count; i++) {
            execMainRoot(0, status::BaseAction::useActionParam_->targetCharacterStatus_[i]);
            status::BaseAction::useActionParam_->result_ = resultFlag_;
            status::BaseAction::useActionParam_->targetResult_[i] = resultFlag_;
            message_.setExecMessage(&status::BaseAction::useActionParam_->message_[i]);
            message_.setResultMessage(status::BaseAction::useActionParam_->targetCharacterStatus_[i],
                                      &status::BaseAction::useActionParam_->message_[i]);
            message_.setSplitMessage(status::BaseAction::useActionParam_->actorCharacterStatus_,
                                     status::BaseAction::useActionParam_->targetCharacterStatus_[i],
                                     &status::BaseAction::useActionParam_->message_[0],
                                     actionIndex_);
        }
        this->execAfter(0);
        return;
    }

    int ok = 1;
    if (flag) {
        this->checkStatusRelease();
    }
    if (!this->checkActorStatusChangeRelease(status::BaseAction::useActionParam_->actorCharacterStatus_)) {
        ok = 0;
    } else if (!this->checkActorAstoron()) {
        ok = 0;
    } else if (!this->checkSpazz()) {
        ok = 0;
    } else if (!this->checkSleep()) {
        ok = 0;
    } else if (!this->checkPath1()) {
        ok = 0;
    } else if (!this->checkTimeStop()) {
        ok = 0;
    } else if (!this->checkUseMp()) {
        ok = 0;
    } else if (!this->checkMahoton()) {
        ok = 0;
    } else if (!this->checkFizzleZone()) {
        ok = 0;
    }

    if (!ok) {
        status::CharacterStatus* a = status::BaseAction::useActionParam_->actorCharacterStatus_;
        if (a) {
            a->haveBattleStatus_.setActionFailed(actionIndex_);
        }
        this->useMp();
        return;
    }

    this->checkConfusion();
    execBefore();

    int count = status::BaseAction::useActionParam_->targetCount_;
    status::CharacterStatus* a = status::BaseAction::useActionParam_->actorCharacterStatus_;

    if (count == 0) {
        if (a) {
            a->haveStatusInfo_.clearHpInBattle();
            a->haveStatusInfo_.clearMpInBattle();
            a->haveStatusInfo_.clearStatusChangeInBattle();
        }
        message_.setExecMessage(&status::BaseAction::useActionParam_->message_[0]);
        message_.setResultMessage(status::BaseAction::useActionParam_->actorCharacterStatus_,
                                  &status::BaseAction::useActionParam_->message_[0]);
        this->execMainRoot(status::BaseAction::useActionParam_->actorCharacterStatus_, 0);
        message_.setSplitMessage(status::BaseAction::useActionParam_->actorCharacterStatus_, 0,
                                 &status::BaseAction::useActionParam_->message_[0],
                                 actionIndex_);
    } else {
        unsigned short savedHp = 0;
        if ((unsigned int)(actionIndex_ - 513) <= 1) {
            savedHp = a->haveStatusInfo_.getHp();
            a->haveStatusInfo_.setHp(0);
        }
        if (a) {
            a->haveStatusInfo_.clearHpInBattle();
            a->haveStatusInfo_.clearMpInBattle();
            a->haveStatusInfo_.clearStatusChangeInBattle();
        }
        if ((unsigned int)(this->actionIndex_ - 513) <= 1) {
            a->haveStatusInfo_.setHp(savedHp);
        }

        for (int i = 0; i < count; i++) {
            status::BaseAction::useActionParam_->currentTargetIndex_ = i;
            message_.setExecMessage(&status::BaseAction::useActionParam_->message_[i]);
            message_.setExecMessageAdd(actionIndex_,
                                       &status::BaseAction::useActionParam_->message_[i]);
            mahokantaFlag_ = 0;

            if (!status::BaseAction::useActionParam_->targetCharacterStatus_[i]->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusMahosute)
             && !status::BaseAction::useActionParam_->targetCharacterStatus_[i]->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusAstoron)
             && this->checkTargetMahokanta(status::BaseAction::useActionParam_->actorCharacterStatus_,
                              status::BaseAction::useActionParam_->targetCharacterStatus_[i])) {
                targetType_ = status::BaseAction::useActionParam_->actorCharacterStatus_->characterType_;
                {
                    status::UseActionParam* p = status::BaseAction::useActionParam_;
                    status::CharacterStatus* act = p->actorCharacterStatus_;
                    p->originalTargetCharacterStatus_[i] = p->targetCharacterStatus_[i];
                    p->targetCharacterStatus_[i] = act;
                }
                status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.setMahokantaCounter(true);
            }

            if (this->execMainRoot(status::BaseAction::useActionParam_->actorCharacterStatus_,
                              status::BaseAction::useActionParam_->targetCharacterStatus_[i])) {
                status::BaseAction::useActionParam_->result_ = resultFlag_;
                status::BaseAction::useActionParam_->targetResult_[i] = resultFlag_;
                message_.setResultMessage(status::BaseAction::useActionParam_->targetCharacterStatus_[i],
                                          &status::BaseAction::useActionParam_->message_[i]);

                if (status::BaseAction::kaishinFlag_) {
                    status::BaseAction::kaishinFlag_ = 0;
                    status::BaseAction::useActionParam_->message_[i].spclMessage_[0] = 801052;
                    status::BaseAction::useActionParam_->actorCharacterStatus_->damageSound_ = status::CharacterStatus::KaishinSe;
                    if (!status::BaseAction::multiFlag_ || i != 1) {
                        status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.setFirstKaishin(true);
                    }
                    if (status::BaseAction::multiFlag_ && i == 1) {
                        status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.setSecondKaishin(true);
                    }
                }

                if ((unsigned int)(actionIndex_ - 231) <= 1) {
                    if (status::BaseAction::tsukonFlag_ || status::BaseAction::tsukon2Flag_) {
                        if (status::BaseAction::useActionParam_->actorCharacterStatus_->characterType_ == PLAYER) {
                            status::BaseAction::useActionParam_->message_[0].spclMessage_[0] = 801052;
                            status::BaseAction::useActionParam_->actorCharacterStatus_->damageSound_ = status::CharacterStatus::TsukonSe;
                        } else {
                            status::BaseAction::useActionParam_->message_[0].spclMessage_[1] = 801055;
                            status::BaseAction::useActionParam_->actorCharacterStatus_->damageSound_ = status::CharacterStatus::TsukonSe;
                        }
                    }
                }

                if (status::BaseAction::multiFlag_) {
                    status::BaseAction::useActionParam_->targetCharacterStatus_[i]->setMultiDamageAnimation();
                    if (i == 0) {
                        status::CharacterStatus* t0 = status::BaseAction::useActionParam_->targetCharacterStatus_[i];
                        if (t0) {
                            int v = t0->haveStatusInfo_.effectValue_;
                            t0->haveStatusInfo_.setUseActionEffectValueMulti(0, v);
                            if (v) {
                                status::BaseAction::useActionParam_->targetCharacterStatus_[i]->haveStatusInfo_.setMultiAttack(true);
                                if (status::BaseAction::useActionParam_->targetCharacterStatus_[i]->haveStatusInfo_.isDeath()) {
                                    status::BaseAction::useActionParam_->targetCharacterStatus_[i]->haveStatusInfo_.setMultiFirstDeath(true);
                                }
                                status::BaseAction::useActionParam_->targetCharacterStatus_[i]->setMultiDamageAnimation1();
                            }
                        }
                    }
                    if (i == 1) {
                        status::CharacterStatus* t1 = status::BaseAction::useActionParam_->targetCharacterStatus_[i];
                        if (t1) {
                            int v = t1->haveStatusInfo_.effectValue_;
                            t1->haveStatusInfo_.setUseActionEffectValueMulti(1, v);
                            if (v) {
                                if (status::BaseAction::useActionParam_->targetCharacterStatus_[i]->haveStatusInfo_.isDeath()) {
                                    status::BaseAction::useActionParam_->targetCharacterStatus_[i]->setMultiDamageAnimation2nd();
                                }
                                status::BaseAction::useActionParam_->targetCharacterStatus_[i]->setMultiDamageAnimation2();
                            }
                        }
                    }
                }

                if (mahokantaFlag_) {
                    mahokantaFlag_ = 0;
                }
                if (actionIndex_ == 475) {
                    status::BaseAction::useActionParam_->message_[0].resultMessage_[0] = 700017;
                }
            }

            {
                status::UseActionParam* p = status::BaseAction::useActionParam_;
                message_.setSplitMessage(p->actorCharacterStatus_,
                                         p->targetCharacterStatus_[i],
                                         &p->message_[i],
                                         actionIndex_);
            }
            this->execAfterOne(i);

            if (i == 0
             && this->checkActorDouble(status::BaseAction::useActionParam_->actorCharacterStatus_)
             && status::BaseAction::useActionParam_->targetCharacterStatus_[0]->haveStatusInfo_.isDeath()) {
                status::BaseAction::useActionParam_->targetCharacterStatus_[1] = 0;
                status::BaseAction::useActionParam_->targetCount_ = 1;
                break;
            }
        }
    }

    this->execAfter(1);
}

THUMB int status::BaseAction::execBefore()
{
    if (actionIndex_ == 137) {
        if (status::BaseAction::useActionParam_->actorCharacterStatus_->characterType_ == PLAYER) {
            moonSaltCount_ = g_monster.getCount();
        }
        if (status::BaseAction::useActionParam_->actorCharacterStatus_->characterType_ == MONSTER) {
            g_Party.setBattleModeWithCarriage();
            moonSaltCount_ = g_Party.getCount();
        }
    }

    if (status::BaseAction::useActionParam_->actorCharacterStatus_
     && !status::UseAction::isBaikiruto(actionIndex_)) {
        status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.setBaikirutoDisable(true);
    }

    if (this->checkActorDouble(status::BaseAction::useActionParam_->actorCharacterStatus_)) {
        status::BaseAction::useActionParam_->targetCharacterStatus_[1] =
            status::BaseAction::useActionParam_->targetCharacterStatus_[0];
        status::BaseAction::useActionParam_->targetCount_ = 2;
    }

    if (actionIndex_ == 459) {
        status::BaseAction::callMonster_[0] = g_monster.getMonsterCallIndex();
        status::BaseAction::callMonster_[1] = g_monster.getMonsterCallType();
    }
    if (actionIndex_ == 477) {
        status::BaseAction::callMonster_[0] = g_monster.getMonsterCallIndex();
        status::BaseAction::callMonster_[1] = g_monster.getMonsterCallType();
    }
    if (actionIndex_ == 480) {
        status::BaseAction::callMonster_[0] = g_monster.getMonsterCallIndex();
    }
    if ((unsigned int)(actionIndex_ - 528) <= 1) {
        if (g_monster.getMonsterCallType() == 0) {
            status::BaseAction::callMonster_[0] = 0;
        }
        if (g_monster.getMonsterCallType() == 1) {
            status::BaseAction::callMonster_[0] = 1;
        }
        if (g_monster.getMonsterCallType() == 2) {
            status::BaseAction::callMonster_[0] = 2;
        }
        status::BaseAction::callMonster_[1] = g_monster.getMonsterCallIndex();
    }

    if (actionIndex_ == 371 && status::BaseAction::useActionParam_->actorCharacterStatus_) {
        if (dssrand::rand(8) < 5) {
            unsigned int lv = status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.haveStatus_.level_;
            status::BaseAction::flag_ = (lv >> 1) + 20;
            status::BaseAction::flag_ = status::getRandomVariation(status::BaseAction::flag_, 20, 10);
        } else {
            status::BaseAction::flag_ = 0;
        }
    }

    if (actionIndex_ == 513) {
        status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.setHp(0);
        status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.setExecuteMeganteRing(true);
        status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.clearHpInBattle();
    }
    if (actionIndex_ == 514) {
        status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.setHp(0);
        status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.clearHpInBattle();
    }

    return 1;
}

THUMB int status::BaseAction::execAfterOne(int index)
{
    unsigned int f = status::BaseActionStatus::flag_.flag_;
    unsigned int* g = (unsigned int*)&status::BaseActionStatus::flag_.flag_;
    if (*g & 3) {
        if (f & 1) {
            status::BaseActionStatus::flag_.flag_ = *g & ~1;
            this->message_.setResultMessage(&status::BaseAction::useActionParam_->message_[index], 801455, 0);
        }
        if (status::BaseActionStatus::flag_.flag_ & 2) {
            status::BaseActionStatus::flag_.flag_ &= ~2;
            this->message_.setResultMessage(&status::BaseAction::useActionParam_->message_[index], 801451, 0);
        }
    }
    if (actionIndex_ == 371) {
        int t = status::BaseAction::useActionParam_->currentTargetIndex_;
        if (status::BaseAction::useActionParam_->targetResult_[t]) {
            if (status::BaseAction::useActionParam_->targetCharacterStatus_[t]->haveStatusInfo_.isDeath()) {
                status::BaseAction::useActionParam_->message_[t].resultMessage_[0] = 801060; // 0xC3924
            } else {
                status::BaseAction::useActionParam_->message_[t].resultMessage_[0] = 801058; // 0xC3922
            }
        }
    }
    if (status::BaseAction::useActionParam_->targetCharacterStatus_[index]
     && status::BaseAction::useActionParam_->targetCharacterStatus_[index]->haveStatusInfo_.isMahokantaCounter()) {
        status::CharacterStatus* tgt = status::BaseAction::useActionParam_->targetCharacterStatus_[index];
        tgt->haveStatusInfo_.mahokantaEffectValue_[index] = tgt->haveStatusInfo_.effectValue_;
    }
    if (actionIndex_ == 269) {
        status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.addHpInBattle(status::HaveStatusInfo::SpecialAction, -1023);
        status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.setKillMyself(true);
        this->message_.setAddMessage(&status::BaseAction::useActionParam_->message_[status::BaseAction::useActionParam_->targetCount_ - 1], 801257, 0); // 0xC39E9
    }
    if (status::BaseActionStatus::mahokantaMessFlag_) {
        status::BaseActionStatus::mahokantaMessFlag_ = 0;
        this->message_.setResultMessage(&status::BaseAction::useActionParam_->message_[status::BaseAction::useActionParam_->currentTargetIndex_], 0, 0);
    }
    if (status::BaseActionStatus::confusionMessFlag_) {
        status::BaseActionStatus::confusionMessFlag_ = 0;
        this->message_.setResultMessage(&status::BaseAction::useActionParam_->message_[status::BaseAction::useActionParam_->currentTargetIndex_], status::BaseActionStatus::work_, 0);
    }
    if (status::BaseActionStatus::sleepMessFlag_) {
        status::BaseActionStatus::sleepMessFlag_ = 0;
        this->message_.setResultMessage(&status::BaseAction::useActionParam_->message_[status::BaseAction::useActionParam_->currentTargetIndex_], status::BaseActionStatus::work_, 0);
    }
    if (status::BaseActionStatus::path1MessFlag_) {
        status::BaseActionStatus::path1MessFlag_ = 0;
        this->message_.setResultMessage(&status::BaseAction::useActionParam_->message_[status::BaseAction::useActionParam_->currentTargetIndex_], status::BaseActionStatus::work_, 0);
    }
    if (status::BaseActionStatus::baikirutoMessFlag_) {
        status::BaseActionStatus::baikirutoMessFlag_ = 0;
        this->message_.setResultMessage(&status::BaseAction::useActionParam_->message_[status::BaseAction::useActionParam_->currentTargetIndex_], status::BaseActionStatus::work_, 0);
    }
    if (status::UseAction::getDamageType(actionIndex_) == 10 && this->resultFlag_ == 0) {
        int t = status::BaseAction::useActionParam_->currentTargetIndex_;
        if (status::BaseAction::useActionParam_->targetCharacterStatus_[t]) {
            if (status::BaseAction::useActionParam_->targetCharacterStatus_[t]->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusSleep)) {
                message_.setResultMessage(&status::BaseAction::useActionParam_->message_[t], 801999, 0); // 0xC3CCF
            }
        }
    }
    return 1;
}

THUMB int status::BaseAction::execAfter(int flag)
{
    status::BaseAction::useActionParam_->exec_ = executeAction_;

    if (actionIndex_ == 0xAD || actionIndex_ == 0x16D) {
        
        int i; 
        int count = status::BaseAction::useActionParam_->targetCount_;
        for (i = 0; i < count; i++) {
            if (status::BaseAction::useActionParam_->targetResult_[i] != 0) {
                this->resultFlag_ = 1;
            }
        }
        status::BaseAction::useActionParam_->result_ = this->resultFlag_;
    }

    if (actionIndex_ == 0x15C && resultFlag_ != 0 && dssrand::rand(0xA) == 0) {
        status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.execThrow(status::BaseAction::useActionParam_->actorCharacterStatus_->haveBattleStatus_.sortIndex_);
        this->message_.setAddMessage(&status::BaseAction::useActionParam_->message_[0], 0xC3A2C, 0);
    }

    if (actionIndex_ == 0xA0 && resultFlag_ != 0 && dssrand::rand(0xA) == 0) {
        status::BaseAction::setBreakPrayRing(true);
        this->message_.setAddMessage(&status::BaseAction::useActionParam_->message_[0], 0xC3A2C, 0);
    }

    if (flag == 0) {
        return 1;
    }
    this->useMp(); // useMp

    if (actionIndex_ == 0x3F && (status::BaseActionStatus::flag_.flag_ & 4) != 0) {
        status::BaseActionStatus::flag_.flag_ &= ~4;
        status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.addHpInBattle(status::HaveStatusInfo::SpecialAction, -0x3FF);
        this->message_.setAddMessage(&status::BaseAction::useActionParam_->message_[status::BaseAction::useActionParam_->targetCount_ - 1], 0xC39E9, 0);
    }

    if (actionIndex_ == 0x1B) {
        status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.addHpInBattle(status::HaveStatusInfo::SpecialAction, -0x3FF);
        status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.setKillMyself(true);
        this->message_.setAddMessage(&status::BaseAction::useActionParam_->message_[status::BaseAction::useActionParam_->targetCount_ - 1], 0xC39E9, 0);
    }

    if (actionIndex_ == 0x202) {
        status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.addHpInBattle(status::HaveStatusInfo::SpecialAction, -0x3FF);
        this->message_.setAddMessage(&status::BaseAction::useActionParam_->message_[status::BaseAction::useActionParam_->targetCount_ - 1], 0xC39E9, 0);
    }

    if (actionIndex_ == 0x201) {
        status::BaseAction::useActionParam_->message_[status::BaseAction::useActionParam_->targetCount_ - 1].addMessage_[0] = 0xC3A4B;
        status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.setExecuteMeganteRing(true);
    }
    if (actionIndex_ == 0x202) { // dup devs, 2e bloc 0x202 séparé — garder
        status::BaseAction::useActionParam_->message_[status::BaseAction::useActionParam_->targetCount_ - 1].addMessage_[1] = 0xC3A4B;
    }

    if (status::BaseAction::useActionParam_->actorCharacterStatus_) {
        status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.setBaikirutoDisable(false);
    }

    int msg;
    int msg2;
    int action = actionIndex_;

    if (action >= 0x21E && action <= 0x226) {
        msg = 0;
        if (action == 0x21E) msg = 0xC3BF4;
        if (action == 0x21F) msg = 0;
        if (action == 0x220) msg = 0xC3BF7;
        if (action == 0x221) msg = 0xC3BFA;
        if (action == 0x222) {
            msg = 0xC3BFD;
            status::BaseAction::doubleFlag_ = 1;
        }
        action = actionIndex_; 
        if (action == 0x223) msg = 0xC3BFF;
        if (action == 0x224) msg = 0xC3C02;
        if (action == 0x225) msg = 0xC3C05;
        if (action == 0x226) msg = 0xC3C08;
        if (msg != 0) {
            status::BaseAction::useActionParam_->message_[0].execMessage_[1] = msg;
        }
    }

    action = actionIndex_;
    if (action >= 0x1CB && action <= 0x1E5) {
        int msg = 0;   // 
        int msg2 = 0;
        if (action == 0x1CB) msg = 0xC3B59;
        if (action == 0x1CC) { msg = 0; msg2 = 0; }
        if (action == 0x1CD) { msg = 0; msg2 = 0; }
        if (action == 0x1CE) { msg = 0; msg2 = 0; }
        if (action == 0x1CF) { msg = 0; msg2 = 0; }
        if (action == 0x1D0) { msg = 0xC3965; msg2 = 0; }
        if (action == 0x1D1) { msg = 0xC3B6B; msg2 = 0; }
        if (action == 0x1D2) { msg = 0; msg2 = 0; }
        if (action == 0x1D3) { msg = 0xC3B6D; msg2 = 0; }
        if (action == 0x1D4) { msg = 0xC3B71; msg2 = 0; }
        if (action == 0x1D5) { msg = 0; msg2 = 0; }
        if (action == 0x1D6) { msg = 0xC3B80; msg2 = 0; }
        if (action == 0x1D7) { msg = 0xC3B85; msg2 = 0xC3B88; }
        if (action == 0x1D8) { msg = 0xC3B85; msg2 = 0xC3B8B; }
        if (action == 0x1D9) { msg = 0; msg2 = 0; }
        if (action == 0x1DA) { msg = 0xC3B8E; msg2 = 0; }
        if (action == 0x1DB) { msg = 0; msg2 = 0; }
        if (action == 0x1DC) { msg = 0xC3A31; msg2 = 0; }
        if (action == 0x1DD) { msg = 0xC3B98; msg2 = 0; }
        if (action == 0x1DE) { msg = 0xC3BFD; msg2 = 0; }
        if (action == 0x1DF) { msg = 0; msg2 = 0; }
        if (action == 0x1E0) { msg = 0; msg2 = 0; }
        if (action == 0x1E1) { msg = 0; msg2 = 0; }
        if (action == 0x1E2) { msg = 0xC3BAD; msg2 = 0; }
        if (action == 0x1E3) { msg = 0; msg2 = 0; }
        if (action == 0x1E4) { msg = 0; msg2 = 0; }
        if (action == 0x1E5) { msg = 0; msg2 = 0; }
        if (msg != 0) {
            status::BaseAction::useActionParam_->message_[0].execMessage_[0] = msg;
        }
        if (msg2 != 0) {
            status::BaseAction::useActionParam_->message_[0].execMessage_[1] = msg2;
        }
        if (actionIndex_ == 0x1D1) {
            g_Party.setPlayerMode();
            if (g_Party.getAlivePlayerCountOutsideCarriage() > 1) {
                status::BaseAction::useActionParam_->message_[0].execMessage_[0] = 0xC3CCD;
            }
        }
        if (actionIndex_ == 0x1CB) {
            switch (status::BaseAction::callMonster_[1]) {
            case 0: status::BaseAction::useActionParam_->message_[0].resultMessage_[0] = 0xC3B62; break;
            case 1: status::BaseAction::useActionParam_->message_[0].resultMessage_[0] = 0xC3B65; break;
            case 2: status::BaseAction::useActionParam_->message_[0].resultMessage_[0] = 0xC3B68; break;
            }
        }
        if (actionIndex_ == 0x1DD) {
            switch (status::BaseAction::callMonster_[1]) {
            case 0: status::BaseAction::useActionParam_->message_[0].resultMessage_[0] = 0xC3B9B; break;
            case 1: status::BaseAction::useActionParam_->message_[0].resultMessage_[0] = 0xC3B9E; break;
            case 2: status::BaseAction::useActionParam_->message_[0].resultMessage_[0] = 0xC3BA1; break;
            }
        }
        if (actionIndex_ == 0x1E2 && !status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.isDeath()) {
            status::BaseAction::useActionParam_->message_[0].addMessage_[0] = 0xC3BAF;
        }
        if (actionIndex_ == 0x1DA) {
            status::BaseAction::useActionParam_->message_[status::BaseAction::useActionParam_->targetCount_ - 1].addMessage_[0] = 0xC3C9C;
        }
        if (actionIndex_ == 0x1DB) {
            parupunteMetalSlime1();
        }
    }

    action = actionIndex_;
    if (action >= 0x1A1 && action <= 0x1CA) {
        if (action == 0x1AD) {
            status::BaseAction::useActionParam_->message_[0].execMessage_[1] = 0xC3CC5;
        }
        if (actionIndex_ == 0x1A7) {
            status::BaseAction::useActionParam_->message_[0].execMessage_[1] = 0xC3CAC;
        }
    }

    action = actionIndex_;
    if (action >= 0x210 && action <= 0x21D && action == 0x216) {
        status::BaseAction::useActionParam_->message_[0].spclMessage_[0] = 0xC391C;
    }

    if (actionIndex_ == 0x61) {
        status::BaseAction::useActionParam_->message_[0].execMessage_[1] = 0xC3CC1;
    }
    if (actionIndex_ == 0x1D0) {
        status::BaseAction::useActionParam_->message_[0].execMessage_[1] = 0xC3CC1;
    }

    if (actionIndex_ >= 0x10E && actionIndex_ <= 0x11A && resultFlag_ != 0) {
        int count = getCallMonsterCount();
        status::BaseAction::useActionParam_->targetCount_ = count;
        for (int i = 0; i < count; i++) {
            status::BaseAction::useActionParam_->targetCharacterStatus_[i] = getCallMonsterStatus(i);
            status::BaseAction::useActionParam_->message_[i].resultMessage_[0] = status::BaseAction::useActionParam_->message_[0].resultMessage_[0];
        }
    }

    if (actionIndex_ == 0x12F) {
        status::BaseAction::useActionParam_->message_[0].execMessage_[1] = 0xC3CC3;
    }

    if (actionIndex_ == 0x30 || actionIndex_ == 0x31) {
        int i; // R3
        int count = status::BaseAction::useActionParam_->targetCount_;
        for (i = 0; i < count; i++) {
            if (i != 0) {
                status::BaseAction::useActionParam_->message_[i].resultMessage_[0] = 0;
            }
        }
    }
    if (actionIndex_ == 0x2B) {
        int i; // R3
        int count = status::BaseAction::useActionParam_->targetCount_;
        for (i = 0; i < count; i++) {
            if (i != 0) {
                status::BaseAction::useActionParam_->message_[i].resultMessage_[0] = 0;
            }
        }
    }
    if (actionIndex_ == 0x210) {
        int i; // R3
        int count = status::BaseAction::useActionParam_->targetCount_;
        for (i = 0; i < count; i++) {
            if (i != 0) {
                status::BaseAction::useActionParam_->message_[i].resultMessage_[0] = 0;
            }
        }
        if (status::BaseAction::callMonster_[0] == 1) {
            status::BaseAction::useActionParam_->message_[0].resultMessage_[0] = 0xC3BB8;
        }
        if (status::BaseAction::callMonster_[0] == 2) {
            status::BaseAction::useActionParam_->message_[0].resultMessage_[0] = 0xC3BBB;
        }
    }
    if (actionIndex_ == 0x211) {
        int i; // R3
        int count = status::BaseAction::useActionParam_->targetCount_;
        for (i= 0; i < count; i++) {
            if (i != 0) {
                status::BaseAction::useActionParam_->message_[i].resultMessage_[0] = 0;
            }
        }
        if (status::BaseAction::callMonster_[0] == 1) {
            status::BaseAction::useActionParam_->message_[0].resultMessage_[0] = 0xC3BC0;
        }
        if (status::BaseAction::callMonster_[0] == 2) {
            status::BaseAction::useActionParam_->message_[0].resultMessage_[0] = 0xC3BC2;
        }
    }

    if (actionIndex_ == 0x173) {
        if (status::BaseAction::useActionParam_->result_) {
            status::BaseAction::useActionParam_->message_[0].execMessage_[1] = 0xC3ABF;
        } else {
            status::BaseAction::useActionParam_->message_[0].execMessage_[1] = 0xC3AC2;
        }
    }

    if (actionIndex_ == 0x1CB) {
        int firstPlayer;   
        int firstMonster;
        int count = status::BaseAction::useActionParam_->targetCount_;
        firstPlayer = 1;
        firstMonster = 1;
        for (int i = 0; i < count; i++) {
            if (status::BaseAction::useActionParam_->targetCharacterStatus_[i]->characterType_ == PLAYER) {
                if (firstPlayer) {
                    firstPlayer = 0;
                } else {
                    status::BaseAction::useActionParam_->message_[i].clear();
                }
            }
            if (status::BaseAction::useActionParam_->targetCharacterStatus_[i]->characterType_ == MONSTER) {
                if (firstMonster) {
                    firstMonster = 0;
                } else {
                    status::BaseAction::useActionParam_->message_[i].clear();
                }
            }
        }
    }

    if (actionIndex_ == 0x1DD) {
        int count = status::BaseAction::useActionParam_->targetCount_;
        for (int i = 0; i < count; i++) {
            if (i != 0) {
                status::BaseAction::useActionParam_->message_[i].clear();
            }
        }
    }

    if (actionIndex_ == 0x1B) {
        int count = status::BaseAction::useActionParam_->targetCount_;
        for (int i = 0; i < count; i++) {
            if (!status::BaseAction::useActionParam_->targetCharacterStatus_[i]->haveStatusInfo_.isDeath()) {
                status::BaseAction::useActionParam_->message_[i].resultMessage_[0] = 0xC3922;
            }
        }
    }

    return 1;
}

THUMB int status::BaseAction::execMainRoot(status::CharacterStatus* actor, status::CharacterStatus* target)
{
    resultFlag_ = 0;
    status::BaseAction::kaishinFlag_ = 0;
    status::BaseAction::missFlag_ = 0;
    status::BaseAction::splitFlag_ = 0;
    status::BaseAction::joukFlag_ = 0;
    if (target) {
        target->haveStatusInfo_.rebirthFlag_ = 0;
    }
    return execMain(actor, target);
}


THUMB int status::BaseAction::execMain(status::CharacterStatus* actor, status::CharacterStatus* target)
{
    if (target) {
        target->haveStatusInfo_.clearHpInBattle();
        target->haveStatusInfo_.clearMpInBattle();
        target->haveStatusInfo_.clearStatusChangeInBattle();
    }
    if (this->checkActorKaishin(actor) == 0) {
        return 0;
    }
    setEffectValue(target); // virtuel slot 0
    if (actor != 0 && target != 0) {
        originalEffectValue_ = status::getUsuallyAttackEffectValue(actor, target);
    }
    status::CharacterStatus* special = this->checkTargetSpecialSelect(actor, target);
    if (special) {
        target = special;
        special->haveStatusInfo_.clearHpInBattle();
        special->haveStatusInfo_.clearMpInBattle();
        special->haveStatusInfo_.clearStatusChangeInBattle();
        if (this->checkActorDouble(status::BaseAction::useActionParam_->actorCharacterStatus_)) { // checkActorDouble
            if (status::BaseAction::useActionParam_->targetCount_ != 2) {
                status::BaseAction::multiFlag_ = 1;
                status::BaseAction::useActionParam_->targetCharacterStatus_[1] = status::BaseAction::useActionParam_->targetCharacterStatus_[0];
                status::BaseAction::useActionParam_->targetCount_ = 2;
            }
        }
    }
    this->setEffectValueException(actor, target);
    if (this->checkTargetZaoraruZaoriku(target) == 0) return 0;
    if (this->checkPowerSave(actor) == 0) return 0;
    if (this->checkTargetBaikiruto(target) == 0) return 0;
    if (this->checkTargetAstoron(target) == 0) return 0;
    if (this->checkTargetMahosute(actor, target) == 0) return 0;
    if (this->checkTargetFeather(actor) == 0) return 0;
    if (this->checkTargetFubaha(target) == 0) return 0;
    if (this->checkTargetDefence(target) == 0) return 0;
    if (this->checkTargetJouk(target) == 0) return 0;
    if (this->checkTargetSplitJouk(target) == 0) return 1;  
    if (this->checkTargetEquipment(target) == 0) return 1;  
    if (this->checkActorManusa(actor, target) == 0) return 0;
    if (this->checkItemMissA(actor, target) == 0) return 0;
    if (this->checkItemMuchiB(actor, target) == 0) return 0;
    if (this->checkItemDamageC(actor, target) == 0) return 0;
    if (this->checkItemDamageD(actor, target) == 0) return 0;
    if (this->checkItemForceE(actor, target) == 0) return 0;
    if (this->checkActorBaikiruto(actor) == 0) return 0;
    this->execActionDefence(target);
    this->execActionDefenceException(target);
    if (effectValue_ == 0) {
        resultFlag_ = 0;
        if (target) {
            target->haveStatusInfo_.setUseActionEffectValue(0);
        }
    } else {
        resultFlag_ = 1;
        execActionEffect(actor, target);
    }
    this->execAddDamage(actor, target);
    this->execAddDamageItemForceE(actor, target);
    this->checkSleep(target);
    if (this->checkTargetReleaseConfusionSleep(target) == 0) return 0;
    if (this->checkTargetReleaseItetsukuhadou(target) == 0) return 0;
    if (this->checkItemRecoveryI(actor, target) == 0) return 0;
    if (this->checkItemRecoveryJ(actor, target) == 0) return 0;
    if (this->checkItemOther(actor, target) == 0) return 0;
    executeAction_ = 1;
    status::BaseAction::useActionParam_->result_ = resultFlag_;
    return 1;
}

THUMB void status::BaseAction::execActionEffect(status::CharacterStatus* actor, status::CharacterStatus* target)
{
    if (target == 0) {
        return;
    }
    status::BaseActionStatus::actionIndex_ = actionIndex_;
    status::BaseActionStatus::setEffectValue(effectValue_, playerEffectValue_, monsterEffectValue_);
    if (target != 0) { 
        target->haveStatusInfo_.setUseActionEffectValue(0);
    }
    instantDeath_ = 0;
    message_.instantDeath_ = 0;
    switch (status::UseAction::getDamageType(actionIndex_)) {
    case status::UseAction::DamageTypeNone:
        resultFlag_ = status::BaseActionStatus::actionTypeNone(actor, target);
        break;
    case status::UseAction::DamageTypeDamage:
        resultFlag_ = status::BaseActionStatus::actionTypeDamage(target);
        message_.instantDeath_ = 1;
        instantDeath_ = 1;
        if (target->haveStatusInfo_.isDeath() && actor == target) {
            target->haveStatusInfo_.setDamageMyself(true);
        }
        break;
    case status::UseAction::DamageTypeRecovery:
        resultFlag_ = status::BaseActionStatus::actionTypeRecovery(target);
        break;
    case status::UseAction::DamageTypeInstantDeath:
        status::BaseActionStatus::actionTypeInstantDeath(target);
        break;
    case status::UseAction::DamageTypeRebirth:
        resultFlag_ = status::BaseActionStatus::actionTypeRebirth(target);
        break;
    case status::UseAction::DamageTypeAddMp:
        resultFlag_ = status::BaseActionStatus::actionTypeAddMP(target);
        break;
    case status::UseAction::DamageTypeSubMp:
        resultFlag_ = status::BaseActionStatus::actionTypeSubMP(actor, target);
        break;
    case status::UseAction::DamageTypeBaikiruto:
        status::BaseActionStatus::actionTypeBaikiruto(target);
        break;
    case status::UseAction::DamageTypeMahokanta:
        status::BaseActionStatus::actionTypeMahokanta(target);
        break;
    case status::UseAction::DamageTypeMahosute:
        status::BaseActionStatus::actionTypeMahosute(target);
        break;
    case status::UseAction::DamageTypeMahoton:
        status::BaseActionStatus::actionTypeMahoton(target);
        break;
    case status::UseAction::DamageTypeSleep:
        resultFlag_ = status::BaseActionStatus::actionTypeSleep(target);
        break;
    case status::UseAction::DamageTypePoison:
        status::BaseActionStatus::actionTypePoison(target);
        break;
    case status::UseAction::DamageTypeSpazz:
        status::BaseActionStatus::actionTypeSpazz(target); 
        break;
    case status::UseAction::DamageTypeDefenceChange:
        resultFlag_ = status::BaseActionStatus::actionTypeDefenceChange(target);
        break;
    case status::UseAction::DamageTypeStatusClear:
        resultFlag_ = status::BaseActionStatus::actionTypeStatusClear(target);
        break;
    case status::UseAction::DamageTypeAgilityChange:
        resultFlag_ = status::BaseActionStatus::actionTypeAgilityChange(target);
        break;
    case status::UseAction::DamageTypeAstoron:
        status::BaseActionStatus::actionTypeAstoron(target);
        break;
    case status::UseAction::DamageTypeConfusion:
        status::BaseActionStatus::actionTypeConfusion(target);
        break;
    case status::UseAction::DamageTypeDragoram:
        status::BaseActionStatus::actionTypeDragoram(target);
        break;
    case status::UseAction::DamageTypeManusa:
        status::BaseActionStatus::actionTypeManusa(target);
        break;
    case status::UseAction::DamageTypeFubaha:
        status::BaseActionStatus::actionTypeFubaha(target);
        break;
    case status::UseAction::DamageTypeFeather:
        status::BaseActionStatus::actionTypeFeather(target);
        break;
    case status::UseAction::DamageTypeLight:
        resultFlag_ = status::BaseActionStatus::actionTypeLight(target);
        break;
    case status::UseAction::DamageTypeDefence:
        status::BaseActionStatus::actionTypeDefence(target);
        break;
    case status::UseAction::DamageTypePowerSave:
        status::BaseActionStatus::actionTypePowerSave(target);
        break;
    case status::UseAction::DamageTypeMosyasu:
        resultFlag_ = status::BaseActionStatus::actionTypeMosyas(actor, target);
        break;
    }
}

THUMB void status::BaseAction::execAddDamage(status::CharacterStatus* actor, status::CharacterStatus* target)
{
    if (actor == 0) {
        return;
    }
    if (target == 0) {
        return;
    }
    if (actionIndex_ == 0xE9) {
        if (target->haveStatusInfo_.effectValue_ == 0) {
            return;
        }
        if (target->haveStatusInfo_.isDeath()) {
            return;
        }
        if (dssrand::rand(8) < 3) {
            if (target->haveStatusInfo_.actionDefence_.exec(status::UseAction::getActionDefenceKind(0x1C), status::UseAction::getActionDefenceType(0x1C))) {
                target->haveStatusInfo_.statusChange_.setup2(status::StatusChange::StatusSleep, 1);
                target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
                target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction); 
                actor->haveStatusInfo_.setAddEffectSleep(true);
                sleepTurn_ = 1;
                this->message_.setAddMessage(&status::BaseAction::useActionParam_->message_[0], 801399 , 0);
            }
        }
    }
    if (actionIndex_ == 0xEA) {
        if (target->haveStatusInfo_.effectValue_ == 0) {
            return;
        }
        if (target->haveStatusInfo_.isDeath()) {
            return;
        }
        if (dssrand::rand(8) < 3) {
            target->haveStatusInfo_.statusChange_.setup2(status::StatusChange::StatusPoison, 0);
            target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
            actor->haveStatusInfo_.setAddEffectPoison(true);
            this->message_.setAddMessage(&status::BaseAction::useActionParam_->message_[0], 801403, 0);
        }
    }
    if (actionIndex_ == 0xEB) {
        if (target->haveStatusInfo_.effectValue_ == 0) {
            return;
        }
        if (target->haveStatusInfo_.isDeath()) {
            return;
        }
        if (dssrand::rand(8) < 1) {
            target->haveStatusInfo_.statusChange_.setup2(status::StatusChange::StatusSpazz, 1);
            target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
            actor->haveStatusInfo_.setAddEffectSpazz(true);
            this->message_.setAddMessage(&status::BaseAction::useActionParam_->message_[0], 801405, 0);
        }
    }
    if (actionIndex_ == 0x233 ) {
        if (target->haveStatusInfo_.effectValue_ == 0) {
            return;
        }
        if (target->haveStatusInfo_.isDeath()) {
            return;
        }
        if (dssrand::rand(8) == 0) {
            target->haveStatusInfo_.statusChange_.setup2(status::StatusChange::StatusSpazz, 0);
            target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
            actor->haveStatusInfo_.setAddEffectSpazz(true);
            this->message_.setAddMessage(&status::BaseAction::useActionParam_->message_[0], 801405 , 0);
        }
    }
}

THUMB void status::BaseAction::execAddDamageItemForceE(status::CharacterStatus* actor, status::CharacterStatus* target)
{
    if (actor == 0) {
        return;
    }
    if (target == 0) {
        return;
    }
    if (status::UseAction::isForceE(actionIndex_) == 0) {
        return;
    }
    if (target->haveStatusInfo_.isDeath()) {
        return;
    }
    status::DQ4HaveEquipment* equip = &actor->haveStatusInfo_.haveEquipment_; 
    if (equip->isEquipment(0x19)) {
        if (status::BaseAction::eventBattle_ == 0) {
            if (monsterEffectValue_ != 0) {
                if (dssrand::rand(8) == 0) {
                    target->haveStatusInfo_.statusChange_.setup2(status::StatusChange::StatusSpazz, 0);
                    target->haveStatusInfo_.setStatusChangeInBattle(status::HaveStatusInfo::ResultAction);
                    actor->haveStatusInfo_.setAddEffectSpazz(true);
                    this->message_.setAddMessage(&status::BaseAction::useActionParam_->message_[0], 801405, 0);
                }
            }
        }
    }
    if (actor->haveStatusInfo_.haveEquipment_.isEquipment(0x23)) {
        if (target->haveStatusInfo_.effectValue_ == 0) {
            return;
        }
        this->message_.setAddMessage(&status::BaseAction::useActionParam_->message_[0], 801463, 0);
    }
    if (equip->isEquipment(0x18)) {
        if (monsterEffectValue_ != 0) {
            if (dssrand::rand(3) == 0) {
                if (target->haveStatusInfo_.actionDefence_.exec(status::UseAction::getActionDefenceKind(0x1C), status::UseAction::getActionDefenceType(0x1C))) {
                    target->haveStatusInfo_.statusChange_.setup2(status::StatusChange::StatusSleep, 1);
                    actor->haveStatusInfo_.setAddEffectSleep(true);
                    sleepTurn_ = 1;
                    this->message_.setAddMessage(&status::BaseAction::useActionParam_->message_[0], 801399, 0);
                }
            }
        }
    }
    equip->isEquipment(0x20); 
    if (equip->isEquipment(0x29)) {
        if (monsterEffectValue_ != 0) {
            if (dssrand::rand(6) == 0) {
                target->haveStatusInfo_.statusChange_.setup2(status::StatusChange::StatusPoison, 1);
                actor->haveStatusInfo_.setAddEffectPoison(true);
                this->message_.setAddMessage(&status::BaseAction::useActionParam_->message_[0], 801403, 0);
            }
        }
    }
}

THUMB int status::BaseAction::checkStatusRelease()
{
    status::StatusChange* sc = &status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.statusChange_;
    sc->execStartOfTurn();
    int release = sc->isRelease();
    if (release != 0 && release == status::StatusChange::StatusSleep) { 
        return 1;
    }
    return 0;
}


THUMB int status::BaseAction::checkActorStatusChangeRelease(status::CharacterStatus* actor)
{
    if (actor == 0) {
        return 1;
    }
    if (status::HaveAction::isTownMode()) {
        actor->haveStatusInfo_.setStatusChangeRelease(false);
        return 1;
    }
    if (actor->haveStatusInfo_.isStatusChangeRelease() == 0) {
        return 1;
    }
    return 0;
}


THUMB int status::BaseAction::checkActorAstoron()
{
    status::StatusChange* sc = &status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.statusChange_;
    if (sc->isEnable(status::StatusChange::StatusAstoron)) {
        resultFlag_ = 0;
        this->message_.setExecMessage(&status::BaseAction::useActionParam_->message_[0], sc->getExecMessage(status::StatusChange::StatusAstoron), 0, 0, 0);
        this->message_.setResultMessage(&status::BaseAction::useActionParam_->message_[0], 0, 0);
        return 0;
    }
    return 1;
}

THUMB int status::BaseAction::checkSpazz()
{
    status::StatusChange* sc = &status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.statusChange_;
    if (sc->isEnable(status::StatusChange::StatusSpazz)) {
        resultFlag_ = 0;
        this->message_.setExecMessage(&status::BaseAction::useActionParam_->message_[0], sc->getExecMessage(status::StatusChange::StatusSpazz), 0, 0, 0);
        this->message_.setResultMessage(&status::BaseAction::useActionParam_->message_[0], 0, 0);
        return 0;
    }
    if (sc->getExecMessage1(status::StatusChange::StatusSpazz) != 0) {
        resultFlag_ = 0;
        this->message_.setExecMessage(&status::BaseAction::useActionParam_->message_[0], sc->getReleaseMessage(), 0, 0, 0);
        this->message_.setResultMessage(&status::BaseAction::useActionParam_->message_[0], 0, 0);
        status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.setStatusChangeRelease(true);
        return 0;
    }
    return 1;
}

THUMB int status::BaseAction::checkSleep()
{
    status::CharacterStatus* actor = status::BaseAction::useActionParam_->actorCharacterStatus_;
    status::StatusChange* sc = &actor->haveStatusInfo_.statusChange_;
    if (sc->isEnable(status::StatusChange::StatusSleep)) {
        if (actionIndex_ == 0x144 || actionIndex_ == 0x145) {
            status::BaseAction::useActionParam_->message_[0].beforeMessage_[0] = 801485; 
            actor->haveStatusInfo_.setSleepAttack(true);
            return 1;
        }
        resultFlag_ = 0;
        this->message_.setExecMessage(&status::BaseAction::useActionParam_->message_[0], sc->getExecMessage(status::StatusChange::StatusSleep), 0, 0, 0);
        this->message_.setResultMessage(&status::BaseAction::useActionParam_->message_[0], 0, 0);
        return 0;
    }
    if (sc->getExecMessage1(status::StatusChange::StatusSleep) != 0) {
        resultFlag_ = 0;
        this->message_.setExecMessage(&status::BaseAction::useActionParam_->message_[0], sc->getReleaseMessage(), 0, 0, 0);
        this->message_.setResultMessage(&status::BaseAction::useActionParam_->message_[0], 0, 0);
        status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.setStatusChangeRelease(true);
        return 0;
    }
    return 1;
}

THUMB int status::BaseAction::checkPath1()
{
    status::StatusChange* sc = &status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.statusChange_;
    if (sc->isEnable(status::StatusChange::StatusPath1)) {
        resultFlag_ = 0;
        if (sc->getActionIndex(status::StatusChange::StatusPath1) == 0x10A) {
            this->message_.setExecMessage(&status::BaseAction::useActionParam_->message_[0], sc->getExecMessage(status::StatusChange::StatusPath1), 0, 0, 0);
        }
        if (sc->getActionIndex(status::StatusChange::StatusPath1) == 0x155) {
            this->message_.setExecMessage(&status::BaseAction::useActionParam_->message_[0], 801500, 0, 0, 0);
        }
        if (sc->getActionIndex(status::StatusChange::StatusPath1) == 0x1B8) {
            this->message_.setExecMessage(&status::BaseAction::useActionParam_->message_[0], 801502, 0, 0, 0);
        }
        if (sc->getActionIndex(status::StatusChange::StatusPath1) == 0x212) {
            this->message_.setExecMessage(&status::BaseAction::useActionParam_->message_[0], 801204 , 0, 0, 0);
        }
        if (sc->getActionIndex(status::StatusChange::StatusPath1) == 0x212) { 
            this->message_.setExecMessage(&status::BaseAction::useActionParam_->message_[0], 801514 , 0, 0, 0);
        }
        if (sc->getActionIndex(status::StatusChange::StatusPath1) == 0x218) {
            this->message_.setExecMessage(&status::BaseAction::useActionParam_->message_[0], 801514 , 0, 0, 0);
        }
        if (sc->getActionIndex(status::StatusChange::StatusPath1) == 0x219) {
            this->message_.setExecMessage(&status::BaseAction::useActionParam_->message_[0], 801516, 0, 0, 0);
        }
        if (sc->getActionIndex(status::StatusChange::StatusPath1) == 0x21A) {
            this->message_.setExecMessage(&status::BaseAction::useActionParam_->message_[0], 801498, 0, 0, 0);
        }
        return 0;
    }
    if (sc->getExecMessage1(status::StatusChange::StatusPath1) != 0) {
        resultFlag_ = 0;
        this->message_.setExecMessage(&status::BaseAction::useActionParam_->message_[0], sc->getReleaseMessage(), 0, 0, 0);
        this->message_.setResultMessage(&status::BaseAction::useActionParam_->message_[0], 0, 0);
        status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.setStatusChangeRelease(true);
        return 0;
    }
    return 1;
}

THUMB int status::BaseAction::checkTimeStop()
{
    if (status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusTimeStop)) {
        resultFlag_ = 0;
        return 0;
    }
    return 1;
}

THUMB void status::BaseAction::checkConfusion()
{
    if (status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusConfusion)) {
        status::BaseAction::useActionParam_->message_[0].beforeMessage_[0] = 801519;
    }
}


THUMB void status::BaseAction::execActionDefence(status::CharacterStatus* target)
{
    if (target == 0) {
        return;
    }
    ActionDefenceKind kind = status::UseAction::getActionDefenceKind(actionIndex_);
    effectValue_ = target->haveStatusInfo_.actionDefence_.exec(kind, status::UseAction::getActionDefenceType(actionIndex_));
    if (target->characterType_ == PLAYER) {
        if (target->haveStatusInfo_.haveStatus_.playerIndex_ == 0x19 && kind == ACT_DEF_MEDAPANI) {
            effectValue_ = 0;
        }
        if (target->haveStatusInfo_.haveStatus_.isPlayer_ != 0 && kind == ACT_DEF_RARIHO && actionIndex_ == 0x1D) {
            effectValue_ = dssrand::rand(1000) < 750 ? 1000 : 0;
        }
    }
    playerEffectValue_ = playerEffectValue_ * effectValue_ / 1000;
    monsterEffectValue_ = monsterEffectValue_ * effectValue_ / 1000;
}

THUMB void status::BaseAction::execActionDefenceException(status::CharacterStatus *target)
{
    if (target == 0) {
        return;
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x3E)) {
        ActionDefenceKind kind = UseAction::getActionDefenceKind(actionIndex_);
        if (kind == ACT_DEF_ZAKI || kind == ACT_DEF_MEDAPANI) {
            setActionDefenceValueAdd(0x1F4);          
        }
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x59)) {
        ActionDefenceKind kind = UseAction::getActionDefenceKind(actionIndex_);
        if (kind == ACT_DEF_RARIHO || kind == ACT_DEF_MEDAPANI || kind == ACT_DEF_SPAZZ) {
            setActionDefenceValueAdd(0x14D);     
        }
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x5B)) {
        ActionDefenceKind kind = UseAction::getActionDefenceKind(actionIndex_);
        if (kind == ACT_DEF_MEDAPANI) {
            setActionDefenceValueAdd(0x14D);
        }
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x5D)) {
        ActionDefenceKind kind = UseAction::getActionDefenceKind(actionIndex_);
        if (kind == ACT_DEF_RARIHO || kind == ACT_DEF_MEDAPANI || kind == ACT_DEF_SPAZZ) {
            setActionDefenceValueAdd(0x14D);
        }
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x5E)) {
        ActionDefenceKind kind = UseAction::getActionDefenceKind(actionIndex_);
        if (kind == ACT_DEF_RARIHO || kind == ACT_DEF_ZAKI || kind == ACT_DEF_SPAZZ) {
            setActionDefenceValueAdd(0x14D);
        }
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x67)) {
        ActionDefenceKind kind = UseAction::getActionDefenceKind(actionIndex_);
        if (kind == ACT_DEF_RARIHO || kind == ACT_DEF_MEDAPANI || kind == ACT_DEF_SPAZZ) {
            setActionDefenceValueAdd(0x14D);
        }
    }
}

THUMB void status::BaseAction::setActionDefenceValueAdd(int ratio)
{
    if (dssrand::rand(1000) >= ratio) {
        effectValue_ = 0;
    }

    playerEffectValue_  = playerEffectValue_  * effectValue_ / 1000;
    monsterEffectValue_ = monsterEffectValue_ * effectValue_ / 1000;
}

THUMB int status::BaseAction::checkUseMp()
{
    if (isUseMp() != 0) {
        return 1;
    }

    resultFlag_ = 0;

    if (status::BaseAction::useActionParam_->actorCharacterStatus_ != NULL) {
        status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.setMpFailure(true);
    }

    message_.setMessageNotEnoughMp(&status::BaseAction::useActionParam_->message_[0]);

    return 0;
}

THUMB int status::BaseAction::isUseMp()
{
    int useMp = UseAction::getUseMp(actionIndex_);

    if (useMp == 255 &&
        status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.getMp() != 0) {
        return 1;
    }

    if (status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.getMp() >= useMp) {
        return 1;
    }
    return 0;
}

THUMB int status::BaseAction::checkMahoton()
{
    if (UseAction::isMahoton(actionIndex_) != 0) {

        StatusChange* sc =
            &status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.statusChange_;

        if (sc->isEnable(StatusChange::StatusMahoton) != 0) {

            resultFlag_ = 0;
            message_.setExecMessage(&status::BaseAction::useActionParam_->message_[0]);
            message_.setResultMessage(&status::BaseAction::useActionParam_->message_[0],
                                      sc->getResultMessage1(StatusChange::StatusMahoton),
                                      0);
            executeAction_ = 1;
            status::BaseAction::useActionParam_->actorCharacterStatus_
                ->haveStatusInfo_.setMahotoneFailure(true);
            return 0;
        }
    }
    return 1;
}

THUMB int status::BaseAction::checkFizzleZone()
{
    if (UseAction::isMahoton(actionIndex_) != 0) {

        StatusChange* sc =
            &status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.statusChange_;

        if (sc->isEnable(StatusChange::StatusFizzleZone) != 0) {

            resultFlag_ = 0;
            message_.setExecMessage(&status::BaseAction::useActionParam_->message_[0]);
            message_.setResultMessage(&status::BaseAction::useActionParam_->message_[0],
                                      sc->getResultMessage1(StatusChange::StatusFizzleZone),
                                      0);
            executeAction_ = 1;
            status::BaseAction::useActionParam_->actorCharacterStatus_
                ->haveStatusInfo_.setMahotoneFailure(true);
            return 0;
        }
    }
    return 1;
}

THUMB int status::BaseAction::checkPowerSave(status::CharacterStatus* actor)
{
    if (UseAction::isPowerSave(actionIndex_) != 0) {

        StatusChange* sc =
            &status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_.statusChange_;

        if (sc->isEnable(StatusChange::StatusPowerSave) != 0) {

            sc->release(StatusChange::StatusPowerSave);

            if (status::BaseAction::kaishinFlag_ == 0) {
                effectValue_        = effectValue_        * 20 / 10;
                playerEffectValue_  = playerEffectValue_  * 20 / 10;
                monsterEffectValue_ = monsterEffectValue_ * 20 / 10;
            }
        }
    }
    return 1;
}

THUMB int status::BaseAction::checkActorBaikiruto(status::CharacterStatus* actor)
{
    if (UseAction::isBaikiruto(actionIndex_) != 0) {

        if (actor->haveStatusInfo_.statusChange_.isEnable(StatusChange::StatusBaikiruto) != 0) {

            if (status::BaseAction::kaishinFlag_ == 0 &&
                status::BaseAction::useActionParam_->currentTargetIndex_ == 0) {

                effectValue_        *= 2;
                playerEffectValue_  *= 2;
                monsterEffectValue_ *= 2;
            }
        }
    }
    return 1;
}

THUMB status::CharacterStatus* status::BaseAction::checkTargetSpecialSelect(
        status::CharacterStatus *actor,
        status::CharacterStatus *target)
{
    if (actor == NULL) {
        return NULL;
    }
    if (target == NULL) {
        return NULL;
    }

    if (g_Global.fightStadiumFlag_ != 0) {
        if (!(actor->haveBattleStatus_.actionIndex_ != 0xE7 &&
              actor->haveBattleStatus_.actionIndex_ != 0xE8)) {
            monsterEffectValue_ = originalEffectValue_;
        }
        return NULL;
    }

    if (actor->haveStatusInfo_.haveEquipment_.isEquipment(0x27)) {
        return NULL;
    }
    if (actor->haveStatusInfo_.haveEquipment_.isEquipment(0x28)) {
        return NULL;
    }
    if (status::BaseAction::multiFlag_ != 0) {
        return NULL;
    }

    if (UseAction::isSpecialSelectTarget(actionIndex_) != 0) {

        if (target->characterType_ == PLAYER) {
            btl::BattleSelectTarget::setTargetSpecial(status::BaseAction::useActionParam_);
            playerEffectValue_ = originalEffectValue_ = status::BaseAction::useActionParam_->damage_;
        }
        if (target->characterType_ == MONSTER) {
            btl::BattleSelectTarget::setTargetSpecial(status::BaseAction::useActionParam_);
            monsterEffectValue_ = originalEffectValue_ = status::BaseAction::useActionParam_->damage_;
        }
        return status::BaseAction::useActionParam_->targetCharacterStatus_[0];
    }

    if (UseAction::isCrossFire(actionIndex_) != 0) {
        btl::BattleSelectTarget::setTargetCrossFire(status::BaseAction::useActionParam_);
        return status::BaseAction::useActionParam_->targetCharacterStatus_[0];
    }

    return NULL;
}

THUMB int status::BaseAction::checkTargetZaoraruZaoriku(status::CharacterStatus *target)
{
    if (HaveAction::isTownMode() != 0) {
        if (actionIndex_ == 0xC8 || actionIndex_ == 0xC9) {
            if (target->haveStatusInfo_.isDeath() == 0) {
                status::BaseAction::workParam_.value_ |= 2;
            }
        }
    }
    return 1;
}


THUMB int status::BaseAction::checkTargetBaikiruto(status::CharacterStatus *target)
{
    if (target == NULL) {
        return 1;
    }

    if (actionIndex_ == 0x2C) {

        StatusChange* sc = &target->haveStatusInfo_.statusChange_;

        if (sc->isEnable(StatusChange::StatusBaikiruto) != 0) {
            message_.setExecMessage(&status::BaseAction::useActionParam_->message_[0]);
            message_.setResultMessage(&status::BaseAction::useActionParam_->message_[0],
                                      sc->getAgainDisableMessage(StatusChange::StatusBaikiruto),
                                      0);
        }
    }
    return 1;
}

THUMB int status::BaseAction::checkTargetMahosute(status::CharacterStatus *actor, status::CharacterStatus *target)
{
    {
    if (actor == NULL) {
        return 1;
    }
    if (target == NULL) {
        return 1;
    }
    if (actor == target) {
        return 1;
    }

    if (UseAction::isMahosute(actionIndex_) != 0) {

        StatusChange* sc = &target->haveStatusInfo_.statusChange_;

        if (sc->isEnable(StatusChange::StatusMahosute) != 0) {

            int index = status::BaseAction::useActionParam_->currentTargetIndex_;

            message_.setExecMessage(&status::BaseAction::useActionParam_->message_[index]);

            message_.setResultMessage(
                &status::BaseAction::useActionParam_->message_[index],
                sc->getResultMessage2(
                    StatusChange::StatusMahosute,
                    status::BaseAction::useActionParam_->actorCharacterStatus_->characterType_),
                0);

            resultFlag_ = 0;
            executeAction_ = 1;
            return 0;
        }
    }
    return 1;
}
}

THUMB int status::BaseAction::checkTargetAstoron(status::CharacterStatus *target)
{
    if (target == NULL) {
        return 1;
    }

    if (UseAction::isAstoron(actionIndex_) != 0) {

        StatusChange* sc = &target->haveStatusInfo_.statusChange_;

        if (sc->isEnable(StatusChange::StatusAstoron) != 0) {

            int index = status::BaseAction::useActionParam_->currentTargetIndex_;

            message_.setExecMessage(&status::BaseAction::useActionParam_->message_[index]);

            message_.setResultMessage(
                &status::BaseAction::useActionParam_->message_[index],
                sc->getResultMessage2(
                    StatusChange::StatusAstoron,
                    status::BaseAction::useActionParam_->actorCharacterStatus_->characterType_),
                0);

            resultFlag_ = 0;
            executeAction_ = 1;
            return 0;
        }
    }
    return 1;
}

THUMB int status::BaseAction::checkTargetFubaha(status::CharacterStatus *target)
{
    if (target == NULL) {
        return 1;
    }

    if (UseAction::isFubaha(actionIndex_) != 0) {

        target->haveStatusInfo_.setFubahaFlag(true);

        if (target->haveStatusInfo_.statusChange_.isEnable(StatusChange::StatusFubaha) != 0) {
            playerEffectValue_  = playerEffectValue_  / 2;
            monsterEffectValue_ = monsterEffectValue_ / 2;
        }
    }
    return 1;
}

THUMB int status::BaseAction::checkTargetDefence(status::CharacterStatus *target)
{
    if (target == NULL) {
        return 1;
    }

    if (UseAction::getDamageType(actionIndex_) == UseAction::DamageTypeDamage) {

        StatusChange* sc = &target->haveStatusInfo_.statusChange_;

        if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x43)) {

            if (sc->isEnable(StatusChange::StatusSleep) != 0) {
                sc->setup(0x9B, true);
            }
            if (sc->isEnable(StatusChange::StatusSpazz) != 0) {
                sc->setup(0x9B, true);
            }
        }

        if (sc->isEnable(StatusChange::StatusDefence) != 0) {

            if (sc->getActionIndex(StatusChange::StatusDefence) == 0x9B) {
                playerEffectValue_  = playerEffectValue_  / 2;
                monsterEffectValue_ = monsterEffectValue_ / 2;
            }

            if (sc->getActionIndex(StatusChange::StatusDefence) == 0x9C) {
                if (actionIndex_ == 0x47) {
                    playerEffectValue_  = playerEffectValue_  / 5;
                    monsterEffectValue_ = monsterEffectValue_ / 5;
                } else {
                    playerEffectValue_  = playerEffectValue_  / 2;
                    monsterEffectValue_ = monsterEffectValue_ / 2;
                }
            }

            if (sc->getActionIndex(StatusChange::StatusDefence) == 0x4B) {
                playerEffectValue_  = playerEffectValue_  / 10;
                monsterEffectValue_ = monsterEffectValue_ / 10;
            }
        }
    }
    return 1;
}

THUMB int status::BaseAction::checkTargetJouk(status::CharacterStatus *target)
{
    if (target == NULL) {
        return 1;
    }
    if (status::BaseAction::kaishinFlag_ != 0) {
        return 1;
    }
    if (status::BaseAction::tsukonFlag_ != 0) {
        return 1;
    }
    if (status::BaseAction::tsukon2Flag_ != 0) {
        return 1;
    }

    if (UseAction::isJouk(actionIndex_) != 0) {

        StatusChange* sc = &target->haveStatusInfo_.statusChange_;

        if (sc->isEnable(StatusChange::StatusSleep) != 0) {
            return 1;
        }
        if (sc->isEnable(StatusChange::StatusSpazz) != 0) {
            return 1;
        }

        if (target->haveBattleStatus_.isJouk() != 0) {

            int index = status::BaseAction::useActionParam_->currentTargetIndex_;

            message_.setExecMessage(&status::BaseAction::useActionParam_->message_[index]);
            message_.setResultMessage(&status::BaseAction::useActionParam_->message_[index],
                                      0xC392E, 0);
            resultFlag_ = 0;
            executeAction_ = 1;
            target->haveStatusInfo_.setTargetJouk(true);
            return 0;
        }

        if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x3A) &&
            dssrand::rand(8) == 0) {

            int index = status::BaseAction::useActionParam_->currentTargetIndex_;

            message_.setExecMessage(&status::BaseAction::useActionParam_->message_[index]);
            message_.setResultMessage(&status::BaseAction::useActionParam_->message_[index],
                                      0xC392E, 0);
            resultFlag_ = 0;
            executeAction_ = 1;
            target->haveStatusInfo_.setTargetJouk(true);
            return 0;
        }
    }
    return 1;
}

THUMB int status::BaseAction::checkTargetSplitJouk(status::CharacterStatus *target)
{
    if (target == NULL) {
        return 1;
    }
    if (target->characterType_ == PLAYER) {
        return 1;
    }
    if (g_Global.fightStadiumFlag_ != 0) {
        return 1;
    }
    if (target->characterIndex_ != 0xB && target->characterIndex_ != 0x83) {
        return 1;
    }
    if (status::BaseAction::useActionParam_->currentTargetIndex_ != 0) {
        return 1;
    }

    executeAction_ = 1;

    if (status::BaseAction::kaishinFlag_ != 0) {
        return 1;
    }

    if (UseAction::isSplitJouk(actionIndex_) != 0) {

        SplitJoukTable tbl = splitJoukTable;
        int ret = 0;

        int r = dss::arrayToIndex(tbl.v, dssrand::rand(16), 3);

        if (r == 0) {
            ret = 1;
        }
        if (r == 1) {
            status::BaseAction::joukFlag_ = 1;
            ret = 0;
        }
        if (r == 2) {
            int monsterIndex = target->characterIndex_;
            int group = target->characterGroup_;

            initCallMonster();

            if (callDifferentMonster(group, monsterIndex) != 0) {
                status::BaseAction::splitFlag_ = 1;
                status::BaseAction::joukFlag_ = 0;
                ret = 0;
            } else {
                status::BaseAction::splitFlag_ = 0;
                status::BaseAction::joukFlag_ = 1;
                ret = 0;
            }
            resultFlag_ = 0;
        }
        return ret;
    }
    return 1;
}

THUMB int status::BaseAction::checkTargetReleaseConfusionSleep(status::CharacterStatus *target)
{
    if (target == NULL) {
        return 1;
    }
    if (resultFlag_ == 0) {
        return 1;
    }
    if (sleepTurn_ != 0) {
        return 1;
    }

    if (UseAction::isReleaseConfusion(actionIndex_) != 0) {

        if (target->haveStatusInfo_.isDeath() != 0) {
            return 1;
        }

        StatusChange* sc = &target->haveStatusInfo_.statusChange_;

        if (sc->isEnable(StatusChange::StatusConfusion) != 0 &&
            target->haveStatusInfo_.haveEquipment_.isEquipment(0x5A) == 0) {

            int flag = 0;
            if (target->characterType_ == PLAYER) {
                flag = (dssrand::rand(2) == 0) ? 1 : 0;
            }
            if (target->characterType_ == MONSTER) {
                flag = (dssrand::rand(4) == 0) ? 1 : 0;
            }

            if (flag != 0) {
                sc->release(StatusChange::StatusConfusion);
                target->haveStatusInfo_.setStatusChangeInBattle(
                    status::HaveStatusInfo::ResultAction);
                target->haveStatusInfo_.setStatusChangeRelease(true);
                message_.setAddMessage(
                    &status::BaseAction::useActionParam_->message_
                        [status::BaseAction::useActionParam_->currentTargetIndex_],
                    0xC3C14, 0);
            }
        }

        if (sc->isEnable(StatusChange::StatusSleep) != 0) {

            int flag = 0;
            if (target->characterType_ == PLAYER) {
                flag = (dssrand::rand(2) == 0) ? 1 : 0;
            }
            if (target->characterType_ == MONSTER) {
                flag = (dssrand::rand(4) == 0) ? 1 : 0;
            }

            if (flag != 0) {
                sc->release(StatusChange::StatusSleep);
                target->haveStatusInfo_.setStatusChangeInBattle(
                    status::HaveStatusInfo::ResultAction);
                target->haveStatusInfo_.setStatusChangeRelease(true);
                message_.setAddMessage(
                    &status::BaseAction::useActionParam_->message_
                        [status::BaseAction::useActionParam_->currentTargetIndex_],
                    0xC3C0E, 0);
            }
        }
    }
    return 1;
}

THUMB int status::BaseAction::checkTargetReleaseItetsukuhadou(status::CharacterStatus *target)
{
    if (target == NULL) {
        return 1;
    }

    if (actionIndex_ == 0x6A || actionIndex_ == 0x154) {
        message_.setExecMessage(
            &status::BaseAction::useActionParam_->message_
                [status::BaseAction::useActionParam_->currentTargetIndex_],
            0, 0xAAE66, 0, 0);
    }
    return 1;
}

THUMB int status::BaseAction::checkTargetMahokanta(status::CharacterStatus *actor,
                                             status::CharacterStatus *target)
{
    if (actor == NULL) {
        return 0;
    }
    if (target == NULL) {
        return 0;
    }
    if (actor == target) {
        return 0;
    }

    if (UseAction::isMahokanta(actionIndex_) != 0) {

        StatusChange* sc = &target->haveStatusInfo_.statusChange_;

        if (sc->isEnable(StatusChange::StatusMahokanta) != 0) {

            mahokantaFlag_ = 1;

            int index = status::BaseAction::useActionParam_->currentTargetIndex_;

            status::BaseAction::useActionParam_->message_[index].spclMessage_[0] =
                sc->getResultMessage2(StatusChange::StatusMahokanta,
                                      target->characterType_);

            executeAction_ = 1;
            return 1;
        }

        if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x45) &&
            dssrand::rand(3) == 0) {

            mahokantaFlag_ = 1;

            int index = status::BaseAction::useActionParam_->currentTargetIndex_;

            sc->getResultMessage3(StatusChange::StatusMahokanta, target->characterType_);

            status::BaseAction::useActionParam_->message_[index].spclMessage_[0] = 0xC3941;

            executeAction_ = 1;
            return 1;
        }
    }
    return 0;
}



THUMB int status::BaseAction::checkTargetEquipment(status::CharacterStatus *target)
{
    if (target == NULL) {
        return 1;
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x35)) {
        switch (UseAction::getActionDefenceKind(actionIndex_)) {
        case ACT_DEF_IO:
        case ACT_DEF_GIRA:
        case ACT_DEF_MERA:
        case ACT_DEF_BAGI:
        case ACT_DEF_HYADO:
            setEffectValueAdd(0x29A, 0);
            break;
        }
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x39)) {
        switch (UseAction::getActionDefenceKind(actionIndex_)) {
        case ACT_DEF_FIRE:
        case ACT_DEF_BLIZZARD:
            setEffectValueAdd(1000, -20);
            break;
        }
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x3B)) {
        switch (UseAction::getActionDefenceKind(actionIndex_)) {
        case ACT_DEF_IO:
        case ACT_DEF_GIRA:
        case ACT_DEF_MERA:
        case ACT_DEF_BAGI:
        case ACT_DEF_HYADO:
            setEffectValueAdd(0x29A, 0);
            break;
        }
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x3C)) {
        switch (UseAction::getActionDefenceKind(actionIndex_)) {
        case ACT_DEF_IO:
        case ACT_DEF_GIRA:
        case ACT_DEF_MERA:
        case ACT_DEF_FIRE:
        case ACT_DEF_BLIZZARD:
            setEffectValueAdd(0x29A, 0);
            break;
        }
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x3E)) {
        switch (UseAction::getActionDefenceKind(actionIndex_)) {
        case ACT_DEF_IO:
        case ACT_DEF_GIRA:
        case ACT_DEF_MERA:
        case ACT_DEF_BAGI:
        case ACT_DEF_HYADO:
            setEffectValueAdd(0x29A, 0);
            break;
        }
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x41)) {
        switch (UseAction::getActionDefenceKind(actionIndex_)) {
        case ACT_DEF_IO:
        case ACT_DEF_GIRA:
        case ACT_DEF_MERA:
        case ACT_DEF_FIRE:
        case ACT_DEF_BLIZZARD:
            setEffectValueAdd(0x29A, 0);
            break;
        }
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x42)) {
        switch (UseAction::getActionDefenceKind(actionIndex_)) {
        case ACT_DEF_IO:
        case ACT_DEF_GIRA:
        case ACT_DEF_MERA:
        case ACT_DEF_BAGI:
        case ACT_DEF_HYADO:
            setEffectValueAdd(0x29A, 0);
            break;
        }
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x46)) {
        switch (UseAction::getActionDefenceKind(actionIndex_)) {
        case ACT_DEF_IO:
        case ACT_DEF_GIRA:
        case ACT_DEF_MERA:
            setEffectValueAdd(0x29A, 0);
            break;
        }
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x48)) {
        switch (UseAction::getActionDefenceKind(actionIndex_)) {
        case ACT_DEF_IO:
        case ACT_DEF_GIRA:
        case ACT_DEF_MERA:
        case ACT_DEF_BAGI:
        case ACT_DEF_HYADO:
            setEffectValueAdd(0x29A, 0);
            break;
        }
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x4B)) {
        switch (UseAction::getActionDefenceKind(actionIndex_)) {
        case ACT_DEF_FIRE:
        case ACT_DEF_BLIZZARD:
            setEffectValueAdd(1000, -5);
            break;
        }
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x4C)) {
        switch (UseAction::getActionDefenceKind(actionIndex_)) {
        case ACT_DEF_FIRE:
        case ACT_DEF_BLIZZARD:
            setEffectValueAdd(1000, -15);
            break;
        }
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x4D)) {

        status::BaseAction::mirrorDamage_  = 0;

        switch (UseAction::getActionDefenceKind(actionIndex_)) {
        case ACT_DEF_IO:
        case ACT_DEF_GIRA:
        case ACT_DEF_MERA:
        case ACT_DEF_BAGI:
        case ACT_DEF_HYADO:
            status::BaseAction::mirrorDamage_  = playerEffectValue_;
            setEffectValueAdd(0x29A, 0);
            break;
        }
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x4F)) {
        switch (UseAction::getActionDefenceKind(actionIndex_)) {
        case ACT_DEF_FIRE:
        case ACT_DEF_BLIZZARD:
            setEffectValueAdd(1000, -25);
            break;
        }
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x50)) {
        switch (UseAction::getActionDefenceKind(actionIndex_)) {
        case ACT_DEF_FIRE:
        case ACT_DEF_BLIZZARD:
            setEffectValueAdd(1000, -30);
            break;
        }
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x51)) {
        switch (UseAction::getActionDefenceKind(actionIndex_)) {
        case ACT_DEF_FIRE:
        case ACT_DEF_BLIZZARD:
            setEffectValueAdd(1000, -40);
            break;
        }
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x52)) {
        switch (UseAction::getActionDefenceKind(actionIndex_)) {
        case ACT_DEF_FIRE:
        case ACT_DEF_BLIZZARD:
            setEffectValueAdd(1000, -30);
            break;
        }
    }

    return 1;
}



THUMB int status::BaseAction::checkActorManusa(status::CharacterStatus *actor,
                                         status::CharacterStatus *target)
{
    if (actor == NULL) {
        return 1;
    }
    if (target == NULL) {
        return 1;
    }
    if (status::BaseAction::kaishinFlag_ != 0) {
        return 1;
    }
    if (status::BaseAction::tsukonFlag_ != 0) {
        return 1;
    }
    if (status::BaseAction::tsukon2Flag_ != 0) {
        return 1;
    }

    if (UseAction::isManusa(actionIndex_) != 0) {

        if (actor->haveStatusInfo_.statusChange_.isEnable(StatusChange::StatusManusa) != 0) {

            if (target->characterType_ == PLAYER) {
                if (dssrand::rand(8) <= 2) {
                    playerEffectValue_ = 0;
                    monsterEffectValue_ = 0;
                }
            }
            if (target->characterType_ == MONSTER) {
                if (dssrand::rand(8) <= 4) {
                    playerEffectValue_ = 0;
                    monsterEffectValue_ = 0;
                }
            }
        }
    }
    return 1;
}

THUMB int status::BaseAction::checkTargetFeather(status::CharacterStatus *target)
{
    if (target == NULL) {
        return 1;
    }

    if (UseAction::isManusa(actionIndex_) != 0) {

        StatusChange* sc = &target->haveStatusInfo_.statusChange_;

        if (sc->isEnable(StatusChange::StatusFeather) != 0) {

            int index = status::BaseAction::useActionParam_->currentTargetIndex_;

            message_.setResultMessage(
                &status::BaseAction::useActionParam_->message_[index],
                sc->getResultMessage2(StatusChange::StatusFeather, MONSTER),
                0);

            resultFlag_ = 0;
            executeAction_ = 1;
            return 0;
        }
    }
    return 1;
}


THUMB int status::BaseAction::checkActorKaishin(status::CharacterStatus *target)
{
    if (target == NULL) {
        return 1;
    }
    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x27)) {
        return 1;
    }
    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x28)) {
        return 1;
    }
    if (target->haveStatusInfo_.isConfuseMissAttack()) {
        return 1;
    }
    if (target != NULL &&
        target->haveStatusInfo_.statusChange_.isEnable(StatusChange::StatusBaikiruto)) {
        return 1;
    }

    if (UseAction::isKaishin(actionIndex_) != 0) {

        if (target->characterType_ == PLAYER) {

            if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x13)) {
                if (dssrand::rand(3) == 0) {
                    status::BaseAction::kaishinFlag_ = 1;
                } else {
                    status::BaseAction::missFlag_ = 1;
                }
            } else if (target->haveStatusInfo_.haveStatus_.playerIndex_ == 4) {

                int level = target->haveStatusInfo_.haveStatus_.level_;

                if (dssrand::rand(512) < level + 15) {
                    status::BaseAction::kaishinFlag_ = 1;
                }
            } else {
                if (dssrand::rand(32) == 0) {
                    status::BaseAction::kaishinFlag_ = 1;
                }
            }
        }

        if (target->haveStatusInfo_.isAllKaishin()) {
            status::BaseAction::kaishinFlag_ = 1;
        }
        if (actionIndex_ == 0xE7 && dssrand::rand(4) == 0) {
            status::BaseAction::tsukonFlag_ = 1;
        }
        if (actionIndex_ == 0xE8) {
            status::BaseAction::tsukon2Flag_ = 1;
        }
    }
    return 1;
}


THUMB int status::BaseAction::checkActorDouble(status::CharacterStatus *target)
{
    if (target == NULL) {
        return 0;
    }

    if (UseAction::isMultiF(actionIndex_) != 0) {
        if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x16)) {
            return 1;
        }
        if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x25)) {
            return 1;
        }
    }
    return 0;
}

THUMB int status::BaseAction::checkItemMissA(status::CharacterStatus *actor, status::CharacterStatus *target)
{
    if (actor == NULL) {
        return 1;
    }

    if (UseAction::isErrorA(actionIndex_) != 0) {

        if (actor->haveStatusInfo_.haveEquipment_.isEquipment(0x13)) {

            if (status::BaseAction::kaishinFlag_ == 0 &&
                status::BaseAction::missFlag_ == 0 &&
                dssrand::rand(3) == 0) {
                status::BaseAction::missFlag_ = 1;
            }
        }
    }

    if (status::BaseAction::missFlag_ != 0) {
        status::BaseAction::missFlag_ = 0;
        playerEffectValue_ = 0;
        monsterEffectValue_ = 0;
    }

    if (actor->haveStatusInfo_.isConfuseMissAttack()) {
        actor->haveStatusInfo_.setConfuseMissAttack(false);
        playerEffectValue_ = 0;
        monsterEffectValue_ = 0;
    }
    return 1;
}

THUMB int status::BaseAction::checkItemMuchiB(status::CharacterStatus *actor,
                                              status::CharacterStatus *target)
{
    if (actor == NULL) {
        return 1;
    }
    if (target == NULL) {
        return 1;
    }

    if (UseAction::isErrorB(actionIndex_) != 0) {

        int a = actor->haveStatusInfo_.haveEquipment_.isEquipment(0x27);
        int b = actor->haveStatusInfo_.haveEquipment_.isEquipment(0x28);

        if (a == 0 && b == 0) {
            return 1;
        }

        switch (status::BaseAction::useActionParam_->currentTargetIndex_) {
        case 0:
            break;
        case 1:
            playerEffectValue_  = playerEffectValue_  * 80 / 100;
            monsterEffectValue_ = monsterEffectValue_ * 80 / 100;
            break;
        case 2:
            playerEffectValue_  = playerEffectValue_  * 70 / 100;
            monsterEffectValue_ = monsterEffectValue_ * 70 / 100;
            break;
        case 3:
            playerEffectValue_  = playerEffectValue_  * 50 / 100;
            monsterEffectValue_ = monsterEffectValue_ * 50 / 100;
            break;
        case 4:
            playerEffectValue_  = playerEffectValue_  * 30 / 100;
            monsterEffectValue_ = monsterEffectValue_ * 30 / 100;
            break;
        default:
            playerEffectValue_  = playerEffectValue_  * 20 / 100;
            monsterEffectValue_ = monsterEffectValue_ * 20 / 100;
            break;
        }
    }
    return 1;
}


THUMB int status::BaseAction::checkItemDamageC(status::CharacterStatus *actor,
                                               status::CharacterStatus *target)
{
    if (target == NULL) {
        return 1;
    }

    if (UseAction::isDamageC(actionIndex_) != 0) {

        HaveEquipment* equip = &actor->haveStatusInfo_.haveEquipment_;

        if (equip->isEquipment(0xF) &&
            target->haveBattleStatus_.metal_ != 0 &&
            status::BaseAction::kaishinFlag_ == 0) {
            playerEffectValue_  = 2;
            monsterEffectValue_ = 2;
        }

        if (equip->isEquipment(0x15) &&
            target->haveBattleStatus_.dragon_ != 0 &&
            status::BaseAction::kaishinFlag_ == 0) {
            playerEffectValue_  = playerEffectValue_  * 3 / 2;
            monsterEffectValue_ = monsterEffectValue_ * 3 / 2;
        }
    }
    return 1;
}

THUMB int status::BaseAction::checkItemDamageD(status::CharacterStatus *actor,
                                               status::CharacterStatus *target)
{
    if (target == NULL) {
        return 1;
    }

    if (UseAction::isDamageD(actionIndex_) != 0) {

        if (actor->haveStatusInfo_.haveEquipment_.isEquipment(0xF) &&
            target->haveBattleStatus_.metal_ != 0 &&
            status::BaseAction::kaishinFlag_ == 0) {
            playerEffectValue_  = 2;
            monsterEffectValue_ = 2;
        }
    }
    return 1;
}

THUMB int status::BaseAction::checkItemForceE(status::CharacterStatus *actor,
                                              status::CharacterStatus *target)
{
    if (target == NULL) {
        return 1;
    }

    if (UseAction::isForceE(actionIndex_) != 0) {

        HaveEquipment* equip = &actor->haveStatusInfo_.haveEquipment_;

        if (equip->isEquipment(0x10)) {

            if (dssrand::rand(8) == 0 && status::BaseAction::eventBattle_ == 0) {

                target->haveStatusInfo_.addHpInBattle(
                    status::HaveStatusInfo::ResultAction, -1023);
                target->haveStatusInfo_.setUseActionEffectValue(0);
                target->setDeathAnimation();
                target->haveStatusInfo_.setImmidiateDeath(true);

                message_.setExecMessage(&status::BaseAction::useActionParam_->message_[0]);
                message_.setResultMessage(&status::BaseAction::useActionParam_->message_[0],
                                          0xC3AC9, 0);
                message_.setAddMessage(&status::BaseAction::useActionParam_->message_[0],
                                       0xC3A6D, 0);
                return 0;
            } else {
                playerEffectValue_  = 1;
                monsterEffectValue_ = 1;
            }
        }

        if (equip->isEquipment(0x26) &&
            status::BaseAction::eventBattle_ == 0 &&
            dssrand::rand(8) == 0) {

            actor->haveStatusInfo_.setWeaponAddDamage(true);

            target->haveStatusInfo_.addHpInBattle(
                status::HaveStatusInfo::ResultAction, -1023);
            target->haveStatusInfo_.setUseActionEffectValue(0);
            target->setDeathAnimation();
            target->haveStatusInfo_.setImmidiateDeath(true);

            message_.setExecMessage(&status::BaseAction::useActionParam_->message_[0]);
            message_.setResultMessage(&status::BaseAction::useActionParam_->message_[0],
                                      0xC3AC9, 0);
            message_.setAddMessage(&status::BaseAction::useActionParam_->message_[0],
                                   0xC3A6D, 0);
            return 0;
        }
    }
    return 1;
}


THUMB int status::BaseAction::checkItemRecoveryI(status::CharacterStatus *actor,
                                                 status::CharacterStatus *target)
{
    if (target == NULL) {
        return 1;
    }

    if (UseAction::isAddHp(actionIndex_) != 0) {

        HaveEquipment* equip = &actor->haveStatusInfo_.haveEquipment_;

        if (equip->isEquipment(0x1C)) {
            if (target->haveStatusInfo_.effectValue_ / 4 != 0) {
                actor->haveStatusInfo_.addHpInBattle(
                    status::HaveStatusInfo::ResultAction,
                    target->haveStatusInfo_.effectValue_ / 4);
                target->haveStatusInfo_.setAddEffectRecovery(true);
                message_.setAddMessage(&status::BaseAction::useActionParam_->message_[0],
                                       0xC3CA6, 0);
            }
        }

        if (equip->isEquipment(0x2A)) {
            if (target->haveStatusInfo_.effectValue_ / 4 != 0) {
                actor->haveStatusInfo_.addHpInBattle(
                    status::HaveStatusInfo::ResultAction,
                    target->haveStatusInfo_.effectValue_ / 4);
                target->haveStatusInfo_.setAddEffectRecovery(true);
                message_.setAddMessage(&status::BaseAction::useActionParam_->message_[0],
                                       0xC3CA6, 0);
            }
        }
    }
    return 1;
}

THUMB int status::BaseAction::checkItemRecoveryJ(status::CharacterStatus *actor,
                                                 status::CharacterStatus *target)
{
    return 1;
}


THUMB int status::BaseAction::checkItemOther(status::CharacterStatus *actor,status::CharacterStatus *target)
{
    if (actor == NULL) {
        return 1;
    }
    if (target == NULL) {
        return 1;
    }
    if (HaveAction::isTownMode() != 0) {
        return 1;
    }

    if (UseAction::isForceE(actionIndex_) != 0 &&
        actor->haveStatusInfo_.haveEquipment_.isEquipment(0x1D) &&
        monsterEffectValue_ != 0 &&
        dssrand::rand(8) == 0) {

        int mp = UseAction::getEffectValueToMonster(0x22);

        if (target->haveStatusInfo_.getMp() != 0 && mp > 0) {

            if (target->haveStatusInfo_.getMp() < mp) {
                mp = target->haveStatusInfo_.getMp();
            }

            target->haveStatusInfo_.addMpInBattle(
                status::HaveStatusInfo::ResultAction, -mp);
            actor->haveStatusInfo_.addMpInBattle(
                status::HaveStatusInfo::ResultAction, mp);
            target->haveStatusInfo_.setAddEffectMahotora(true);
            target->haveStatusInfo_.addDamage_ = mp;

            message_.setAddMessage(&status::BaseAction::useActionParam_->message_[0],
                                   0xC3A8B, 0);
        }
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x3D) &&
        playerEffectValue_ != 0 &&
        dssrand::rand(8) == 0 &&
        actor != target) {

        int mp = UseAction::getUseMp(actionIndex_);

        if (mp != 0) {
            target->haveStatusInfo_.addMpInBattle(
                status::HaveStatusInfo::ResultAction, mp);
            target->haveStatusInfo_.setAddMahotoraExecute(true);
            target->haveStatusInfo_.addDamage_ = mp;

            message_.setAddMessage(
                &status::BaseAction::useActionParam_->message_
                    [status::BaseAction::useActionParam_->currentTargetIndex_],
                0, 0xC3AC7);
        }
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x3F) &&
        playerEffectValue_ != 0 &&
        dssrand::rand(2) == 0 &&
        UseAction::isManusa(actionIndex_) != 0 &&
        UseAction::getDamageType(actionIndex_) == UseAction::DamageTypeDamage &&
        actor->haveStatusInfo_.isDeath() == 0) {

        actor->haveStatusInfo_.addHpInBattle(
            status::HaveStatusInfo::ResultAction, -playerEffectValue_ / 4);
        target->haveStatusInfo_.setAddEffectDamage(true);
        target->setAddDamageAnimation();

        if (playerEffectValue_ / 4 != 0) {

            actor->haveStatusInfo_.setCounterDamage(true);
            actor->haveStatusInfo_.addDamage_ = playerEffectValue_ / 4;

            int index = status::BaseAction::useActionParam_->currentTargetIndex_;

            message_.setAddMessage(&status::BaseAction::useActionParam_->message_[index],
                                   0xC3CB4, 0);

            if (actor->haveStatusInfo_.isDeath()) {
                message_.setAddMessage(&status::BaseAction::useActionParam_->message_[index],
                                       0xC3CB6, 0);
            }
        }
    }

    if (actor->haveStatusInfo_.haveEquipment_.isEquipment(0x14) &&
        UseAction::isDamageC(actionIndex_) != 0 &&
        monsterEffectValue_ != 0 &&
        UseAction::getDamageType(actionIndex_) == UseAction::DamageTypeDamage) {

        actor->haveStatusInfo_.addHpInBattle(
            status::HaveStatusInfo::ResultAction, -monsterEffectValue_ / 4);
        target->haveStatusInfo_.setAddEffectDamage(true);
        target->setAddDamageAnimation();

        if (monsterEffectValue_ / 4 != 0) {

            actor->haveStatusInfo_.setDamageMyself(true);
            actor->haveStatusInfo_.addDamage_ = monsterEffectValue_ / 4;

            int index = status::BaseAction::useActionParam_->currentTargetIndex_;

            if (actor->haveStatusInfo_.isDeath()) {
                message_.setAddMessage(&status::BaseAction::useActionParam_->message_[index],
                                       0xC3CBA, 0);
            } else {
                message_.setAddMessage(&status::BaseAction::useActionParam_->message_[index],
                                       0xC3CB8, 0);
            }
        }
    }

    if (target->haveStatusInfo_.haveEquipment_.isEquipment(0x4D) &&
        UseAction::getActionType(actionIndex_) == UseAction::ActionTypeMagic &&
        UseAction::getDamageType(actionIndex_) == UseAction::DamageTypeDamage &&
        playerEffectValue_ != 0 &&
        actor->haveStatusInfo_.isDeath() == 0) {

        ActionDefenceKind kind = UseAction::getActionDefenceKind(actionIndex_);
        ActionDefenceType type = UseAction::getActionDefenceType(actionIndex_);

        int value = actor->haveStatusInfo_.actionDefence_.exec(kind, type)
                        * status::BaseAction::mirrorDamage_ / 1000;

        actor->haveStatusInfo_.addHpInBattle(
            status::HaveStatusInfo::ResultAction, -value / 4);
        target->haveStatusInfo_.setAddEffectDamage(true);
        target->setAddDamageAnimation();

        if (value / 4 != 0) {

            actor->haveStatusInfo_.setCounterDamage(true);
            actor->haveStatusInfo_.addDamage_ = value / 4;

            int index = status::BaseAction::useActionParam_->currentTargetIndex_;

            if (actor->haveStatusInfo_.isDeath()) {
                message_.setAddMessage(&status::BaseAction::useActionParam_->message_[index],
                                       0xC3CB6, 0);
            } else {
                message_.setAddMessage(&status::BaseAction::useActionParam_->message_[index],
                                       0xC3CB4, 0);
            }
        }
    }
    return 1;
}

THUMB void status::BaseAction::checkSleep(status::CharacterStatus *target)
{
    if (target == NULL) {
        return;
    }
}

THUMB void status::BaseAction::useMp()
{
    if (executeAction_ != 0) {

        if (actionIndex_ == 0xC4 || actionIndex_ == 0xC5 ||
            actionIndex_ == 0xC6 || actionIndex_ == 0xC7) {

            int total = 0;
            int i;
            int count = status::BaseAction::useActionParam_->targetCount_;

            for (i = 0; i < count; i++) {
                total += status::BaseAction::useActionParam_->targetCharacterStatus_[i]
                             ->haveStatusInfo_.effectValue_;
            }
            if (total == 0) {
                return;
            }
        }

        if (HaveAction::isTownMode() != 0) {
            if ((actionIndex_ == 0xCA || actionIndex_ == 0xCB ||
                 actionIndex_ == 0xCC || actionIndex_ == 0xCD ||
                 actionIndex_ == 0xD0) && resultFlag_ == 0) {
                return;
            }
        }

        if (status::BaseAction::workParam_.value_ & 2) {
            int work = status::BaseAction::workParam_.value_;
            status::BaseAction::workParam_.value_ = work & ~2;
            return;
        }

        int mp = UseAction::getUseMp(actionIndex_);

        if (status::BaseAction::useActionParam_->actorCharacterStatus_
                ->haveStatusInfo_.haveEquipment_.isEquipment(0x11)) {
            if (actionIndex_ == 0x47 || actionIndex_ == 0x1A1 || actionIndex_ == 0x1A2) {
                mp = 3;
            }
        }

        if (mp == 0xFF) {
            status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_
                .addMpInBattle(status::HaveStatusInfo::ExecuteAction, -1024);
            status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_
                .addMpInBattle(status::HaveStatusInfo::ResultAction, 0);
            status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_
                .addMpInBattle(status::HaveStatusInfo::SpecialAction, 0);
        } else if (mp != 0) {
            status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_
                .addMpInBattle(status::HaveStatusInfo::ExecuteAction, -mp);
            status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_
                .addMpInBattle(status::HaveStatusInfo::ResultAction, 0);
            status::BaseAction::useActionParam_->actorCharacterStatus_->haveStatusInfo_
                .addMpInBattle(status::HaveStatusInfo::SpecialAction, 0);
        }

        if (actionIndex_ == 0x13) {

            g_Party.setBattleMode();

            int count = g_Party.getCount();

            for (int i = 0; i < count; i++) {

                if (g_Party.isInsideCarriage(i)) {
                    continue;
                }
                if (g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_ == 1) {
                    continue;
                }
                if (g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_ == 2) {
                    continue;
                }

                g_Party.getPlayerStatus(i)->haveStatusInfo_
                    .addMpInBattle(status::HaveStatusInfo::ExecuteAction, -mp);
                g_Party.getPlayerStatus(i)->haveStatusInfo_
                    .addMpInBattle(status::HaveStatusInfo::ResultAction, 0);
                g_Party.getPlayerStatus(i)->haveStatusInfo_
                    .addMpInBattle(status::HaveStatusInfo::SpecialAction, 0);
            }
        }
    }
}

THUMB void status::BaseAction::setEffectValue(status::CharacterStatus *target)
{
    if (target != NULL) {

        playerEffectValue_  = UseAction::getEffectValueToPlayer(actionIndex_);
        monsterEffectValue_ = UseAction::getEffectValueToMonster(actionIndex_);

        if (target->characterType_ == PLAYER) {
            originalEffectValue_ = playerEffectValue_;
        } else {
            originalEffectValue_ = monsterEffectValue_;
        }
    }
}

THUMB void status::BaseAction::setEffectValueAdd(int ratio, int offset)
{
    playerEffectValue_  = playerEffectValue_  * ratio / 1000 + offset;
    monsterEffectValue_ = monsterEffectValue_ * ratio / 1000 + offset;

    if (playerEffectValue_ < 0) {
        playerEffectValue_ = 0;
    }
    if (monsterEffectValue_ < 0) {
        monsterEffectValue_ = 0;
    }
}

THUMB void status::BaseAction::setEffectValueException(status::CharacterStatus *actor,status::CharacterStatus *target)
{
    int value;

    if (actionIndex_ == 0x47 || actionIndex_ == 0x1A1 || actionIndex_ == 0x1A2) {
        value = originalEffectValue_;
        if (status::BaseAction::kaishinFlag_ == 0) {
            value = originalEffectValue_;
        } 
        
        if (actor->haveStatusInfo_.haveEquipment_.isEquipment(0x25)) {
            value = value * 75 / 100;
        }
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0x6C) {
        value = status::getRandomVariation(
            dss::clamp<int>(actor->haveStatusInfo_.haveStatus_.level_ * 3 + 10, 0, 180), 15, 15);
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0x6D) {
        value = status::getRandomVariation(
            dss::clamp<int>(actor->haveStatusInfo_.haveStatus_.level_ * 2 + 30, 0, 95), 10, 10);
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0x7F) {
        value = actor->haveStatusInfo_.getMp() * 3;
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0x82) {
        value = target->haveStatusInfo_.getHp() * 95 / 100 + 1;
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0x85) {
        value = originalEffectValue_ / 2;
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0x86) {
        if (dssrand::rand(8) < 3) {
            value = status::getRandomVariation(actor->haveStatusInfo_.getAttack(0), 5, 5);
            playerEffectValue_  = value;
            monsterEffectValue_ = value;
        } else {
            playerEffectValue_  = 0;
            monsterEffectValue_ = 0;
        }
    }

    if (actionIndex_ == 0x89) {
        value = originalEffectValue_ * 3 / (moonSaltCount_ + 1);
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0x8C) {
        value = originalEffectValue_;
        if (target->type_.flag_ & 1) {
            value = value * 3 / 2;
        }
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0x8E) {
        value = originalEffectValue_;
        if (target->type_.flag_ & 2) {
            value = value * 3 / 2 + 1;
        }
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0x94) {

        value = originalEffectValue_;

        switch (rollingKick_) {
        case 0:
            break;
        case 1:
            value = value * 80 / 100;
            break;
        case 2:
            value = value * 70 / 100;
            break;
        case 3:
            value = value * 50 / 100;
            break;
        case 4:
            value = value * 30 / 100;
            break;
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            value = value * 20 / 100;
            break;
        }

        rollingKick_ = rollingKick_ + 1;
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0x95) {
        value = originalEffectValue_ * 3 / 2;
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0xE9) {
        value = originalEffectValue_;
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0xEA) {
        value = originalEffectValue_;
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0xEB) {
        value = originalEffectValue_;
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0xED) {
        value = originalEffectValue_ * 125 / 100;
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0xEE) {
        value = originalEffectValue_ * 125 / 100;
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0xEF) {
        value = originalEffectValue_ * 125 / 100;
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0x12F) {
        value = originalEffectValue_ / 2 + 150;
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0x132) {
        value = target->haveStatusInfo_.getHp() * 95 / 100 + 1;
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0x13C) {
        value = originalEffectValue_;
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0xF0) {
        value = originalEffectValue_;
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0x173) {
        if (status::BaseAction::flag_ != 0) {
            if ((target->type_.flag_ & 2) == 0) {
                playerEffectValue_  = status::BaseAction::flag_;
                monsterEffectValue_ = status::BaseAction::flag_;
            } else {
                playerEffectValue_  = 1;
                monsterEffectValue_ = 1;
            }
        } else {
            playerEffectValue_  = 0;
            monsterEffectValue_ = 0;
        }
    }

    if (actionIndex_ == 0x1E9) {
        value = originalEffectValue_;
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0x216) {
        value = status::getKaishinAttackEffectValue(actor, target);
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0x21D) {
        value = actor->haveStatusInfo_.getAttack(0);
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0x221) {
        value = status::getRandomVariation(
            actor->haveStatusInfo_.haveStatus_.level_, 50, 100);
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0x227) {
        value = originalEffectValue_;
        if ((target->type_.flag_ & 2) == 0) {
            value = value + 60;
        }
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0x233) {
        value = originalEffectValue_ * 3 / 2;
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0x234) {
        value = originalEffectValue_ + 50;
        playerEffectValue_  = value;
        monsterEffectValue_ = value;
    }

    if (actionIndex_ == 0x1DE) {
        status::BaseAction::doubleFlag_ = 1;
    }

    if (actionIndex_ == 0x171) {
        status::BaseAction::timeReverseFlag_ = 1;
    }

    if (actionIndex_ == 0x1DC) {
        status::BaseAction::timeReverseFlag_ = 1;
    }

    if (actionIndex_ == 0x1D1) {
        status::BaseAction::allKaishinFlag_ = 1;
    }
}

THUMB void status::BaseAction::setBreakPrayRing(int flag)
{
    if (flag) {
        status::BaseAction::workParam_.value_ |= 1;
    } else {
        status::BaseAction::workParam_.value_ &= ~1;
    }
}

THUMB bool status::BaseAction::isBreakPrayRing()
{
    return (status::BaseAction::workParam_.value_ & 1) != 0;
}
