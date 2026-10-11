#include "main/status/BaseActionMessage.hpp"
#include "ov016/TownMenuPlayerControl/TownMenuPlayerControl.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/status/ActionExec.hpp"
#include "main/status/BaseAction.hpp"
#include "ov003/status/MonsterParty.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownFurniture.hpp"


THUMB void func_0202d014()
{
}

status::BaseActionMessageData status::messageData_;


THUMB status::BaseActionMessage::BaseActionMessage()

{
  return;
}



THUMB status::BaseActionMessage::~BaseActionMessage()

{
  return;
}


THUMB void status::BaseActionMessage::initialize() {
    messageData_.actionParam_ = excelParam.actionParam_;
    messageData_.splitMsg_ = excelParam.splitMsg_;
}


THUMB void status::BaseActionMessage::setup(status::UseActionParam* useActionParam) {
    messageData_.useActionParam_ = useActionParam;
    this->actionIndex_ = useActionParam->actionIndex_;
}


THUMB void status::BaseActionMessage::setExecMessage(status::UseActionMessage* message) {
    message->execMessage_[0] = getExecMessage(0);
    message->execMessage_[1] = getExecMessage(1);
    message->execMessage_[2] = getExecMessage(2);
    message->execMessage_[3] = getExecMessage(3);
}

THUMB void status::BaseActionMessage::setExecMessageAdd(int actionIndex, status::UseActionMessage* message) {
    if (actionIndex == 0x229) {
        message->execMessage_[1] = 0xC3AC4;
    }
}



THUMB void status::BaseActionMessage::setResultMessage(status::CharacterStatus* target, status::UseActionMessage* message) {
    message->resultMessage_[0] = getResultMessage();
    if (target->characterType_ == MONSTER
        && data_0210bb94.unkfunc_02058114(0xD) != 0
        && target->haveStatusInfo_.isDeath() != 0
        && target->haveStatusInfo_.isEscapeFlag() == 0
        && target->haveStatusInfo_.isDisappearFlag() == 0) {
        int i = 0;
        if (g_monster.getCount() > 0) {
            do {
                 if ((status::CharacterStatus*)g_monster.getMonsterStatus(i) == target) {
                    g_monster.checkDropItem(i);
                }
                i++;
            } while (i < g_monster.getCount());
        }
    }
    if (this->instantDeath_ != 0) {
        int deathMes = 0;
        if (target->haveStatusInfo_.isDeath() != 0) {
            if (target->characterType_ == 0) {
                deathMes = 0xC3928;
            }
            if (target->characterType_ == 1) {
                deathMes = 0xC3924;
                if (target->isDeathDisable() != 0) {
                    deathMes = 0;
                }
            }
        }
        if (deathMes != 0) {
            message->resultMessage_[0] = deathMes;
        }
    }
}


THUMB void status::BaseActionMessage::setExecMessage(UseActionMessage *useActionMessage,int mes0,int mes1,int mes2,int mes3)
{
  if (mes0 != 0) {
    useActionMessage->execMessage_[0] = mes0;
  }
  if (mes1 != 0) {
    useActionMessage->execMessage_[1] = mes1;
  }
  if (mes2 != 0) {
    useActionMessage->execMessage_[2] = mes2;
  }
  if (mes3 != 0) {
    useActionMessage->execMessage_[3] = mes3;
  }
  return;
}





THUMB void status::BaseActionMessage::setResultMessage(status::UseActionMessage* message, int result0, int result1) {
    message->resultMessage_[0] = result0;
    message->resultMessage_[1] = result1;
}

THUMB void status::BaseActionMessage::setAddMessage(status::UseActionMessage* message, int msg0, int msg1) {
    message->addMessage_[0] = msg0;
    if (msg1 != 0) {
        message->addMessage_[1] = msg1;
    }
}

THUMB void status::BaseActionMessage::setMessageNotEnoughMp(status::UseActionMessage* message) {
    message->execMessage_[0] = getExecMessage(0);
    message->execMessage_[1] = getExecMessage(1);
    message->execMessage_[2] = getExecMessage(2);
    message->execMessage_[3] = getExecMessage(3);
    message->resultMessage_[0] = 0xC3C72;
    message->resultMessage_[1] = 0;
}

THUMB int status::BaseActionMessage::getExecMessage(int index) {
    if (index == 0) {
        return messageData_.actionParam_[this->actionIndex_].actionMes;
    }
    else if (index == 1) {
        return 0;
    }
    else if (index == 2) {
        return 0;
    }
    else if (index == 2) {
        return 0;
    }
    return 0;
}


THUMB int status::BaseActionMessage::getResultMessage() {
    if (messageData_.useActionParam_->result_) {
        return getResultSuccessMessage();
    }
    return getResultFailedMessage();
}

THUMB int status::BaseActionMessage::getResultSuccessMessage() {
    int message = 0;
    status::CharacterStatus* target = messageData_.useActionParam_->targetCharacterStatus_[messageData_.useActionParam_->currentTargetIndex_];
    if (target != 0) {
        if (target->characterType_ == PLAYER) {
            message = messageData_.actionParam_[this->actionIndex_].playerSuccessMes;
        } else {
            message = messageData_.actionParam_[this->actionIndex_].monsterSuccessMes;
        }
    }
    return message;
}

THUMB int status::BaseActionMessage::getResultFailedMessage() {
    status::CharacterStatus* target = messageData_.useActionParam_->targetCharacterStatus_[0];
    if (target != 0) {
        if (target->characterType_ == PLAYER) {
            return messageData_.actionParam_[this->actionIndex_].playerFailedMes;
        }
        return messageData_.actionParam_[this->actionIndex_].monsterFailedMes;
    }
    return messageData_.actionParam_[this->actionIndex_].playerFailedMes;
}

 
THUMB int status::BaseActionMessage::setSplitMessage(status::CharacterStatus* actor, status::CharacterStatus* target, status::UseActionMessage* message, int actionIndex) {
    int execMes = message->execMessage_[0];
    this->actionIndex_ = actionIndex;
    if (execMes == 0xAAE6B) {
        message->execMessage_[0] = 0xC3A5F;
        return 1;
    }
    this->splitFlag_ = 0;
    message->execMessage_[0] = setSplitMessage(actor, target, message->execMessage_[0]);
    message->execMessage_[1] = setSplitMessage(actor, target, message->execMessage_[1]);
    message->resultMessage_[0] = setSplitMessage(actor, target, message->resultMessage_[0]);
    return this->splitFlag_;
}

THUMB int status::BaseActionMessage::setSplitMessage(status::CharacterStatus* actor, status::CharacterStatus* target, int message) {
    int index = -1;
    if (message == 0) {
        return 0;
    }
    if (message == 0xAAE6B) {
        return 0xC3A5F;
    }
    if (message > 0xC3500) {
        return message;
    }
    unsigned int i = 0;
    if (i < param::SplitMsg::size_) {
        param::SplitMsg* p = messageData_.splitMsg_;
        do {
            if (message == (int)p->branch) {
                index = i;
                break;
            }
            i++;
            p++;
        } while (i < param::SplitMsg::size_);
    }
    if (index == -1) {
        return message;
    }
    int result = getMessagePlayerOne(target, index);
    if (result == 0) result = getMessagePlayerMany(target, index);
    if (result == 0) result = getMessageMonster1G(target, index);
    if (result == 0) result = getMessageMonster2G(target, index);
    if (result == 0) result = getMessageMonsterD(target, index);
    if (result == 0) result = getMessageTargetNoSleepNoSpazz(target, index);
    if (result == 0) result = getMessageTargetSleepSpazz(target, index);
    if (result == 0) result = getMessageTargetAlive(target, index);
    if (result == 0) result = getMessageTargetDead(target, index);
    if (result == 0) result = getMessageTargetAstoron(target, index);
    if (result == 0) result = getMessageTargetNoMosyasu(target, index);
    if (result == 0) result = getMessageTargetNoSplitNoJouk(target, index);
    if (result == 0) result = getMessageTargetSplit(target, index);
    if (result == 0) result = getMessageTargetJouk(target, index);
    if (result == 0) result = getMessageWeaponEquipment(actor, index);
    if (result == 0) result = getMessageNoWeaponEquipment(actor, index);
    if (result == 0) result = getMessageActorMale(actor, index);
    if (result == 0) result = getMessageActorFemale(actor, index);
    if (result == 0) result = getMessageRulaOff(index);
    if (result == 0) result = getMessageRiremitoOff(index);
    if (result == 0) result = getMessageNoUse(actor, index);
    if (result == 0) result = getMessageVainTimeZone(index);
    if (result == 0) result = getMessageItemInBox(index);
    if (result == 0) result = getMessageMonsterInBox(index);
    if (result == 0) result = getMessageGoldInBox(index);
    if (result == 0) result = getMessageZeroInBox(index);
    if (result == 0) result = getMessageItemInPot(index);
    if (result == 0) result = getMessageMonsterInPot(index);
    if (result == 0) result = getMessageGoldInPot(index);
    if (result == 0) result = getMessageZeroInPot(index);
    if (result == 0) result = getMessageNoTarget(index);
    if (result == 0) result = getMessageZero(index);
    if (result == 0) result = getMessageNorthEast(index);
    if (result == 0) result = getMessageSouthEast(index);
    if (result == 0) result = getMessageNorthWest(index);
    if (result == 0) result = getMessageSouthWest(index);
    return result;
}


THUMB int status::BaseActionMessage::getMessagePlayerOne(status::CharacterStatus* target, int splitIndex)
{
    int result = 0;

    if (messageData_.splitMsg_[splitIndex].aliveOne == 0) {
        return 0;
    }
    if (target == 0) {
        return 0;
    }
    if (target->characterType_ == PLAYER) {
        g_Party.setBattleMode();
        if (g_Party.getCarriageOutAliveCount() == 1) {
            result = messageData_.splitMsg_[splitIndex].aliveOne;
            this->splitFlag_ = 1;
        }
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessagePlayerMany(status::CharacterStatus* target, int splitIndex)
{
    int result = 0;

    if (messageData_.splitMsg_[splitIndex].aliveTwo == 0) {
        return 0;
    }
    if (target == 0) {
        return 0;
    }
    if (target->characterType_ == PLAYER) {
        g_Party.setBattleMode();
        if (g_Party.getCarriageOutAliveCount() >= 2) {
            result = messageData_.splitMsg_[splitIndex].aliveTwo;
            this->splitFlag_ = 1;
        }
    }
    return result;
}


THUMB int status::BaseActionMessage::getMessageMonster1G(status::CharacterStatus* target, int splitIndex)
{
    int result = 0;

    if (messageData_.splitMsg_[splitIndex].monsterOne == 0) {
        return 0;
    }
    if (g_monster.getMonsterCallType() == 0) {
        result = messageData_.splitMsg_[splitIndex].monsterOne;
        this->splitFlag_ = 1;
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageMonster2G(status::CharacterStatus* target, int splitIndex)
{
    int result = 0;

    if (messageData_.splitMsg_[splitIndex].monsterTwo == 0) {
        return 0;
    }
    if (g_monster.getMonsterCallType() == 1) {
        result = messageData_.splitMsg_[splitIndex].monsterTwo;
        this->splitFlag_ = 1;
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageMonsterD(status::CharacterStatus* target, int splitIndex)
{
    int result = 0;

    if (messageData_.splitMsg_[splitIndex].monsterMore == 0) {
        return 0;
    }
    if (g_monster.getMonsterCallType() == 2) {
        result = messageData_.splitMsg_[splitIndex].monsterMore;
        this->splitFlag_ = 1;
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageTargetNoSleepNoSpazz(status::CharacterStatus* target, int splitIndex)
{
    if (target == 0) {
        return 0;
    }
    if (target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusSleep)) {
        return 0;
    }
    if (target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusSpazz)) {
        return 0;
    }
    int result = messageData_.splitMsg_[splitIndex].activate;
    this->splitFlag_ = 0;
    return result;
}

THUMB int status::BaseActionMessage::getMessageTargetSleepSpazz(status::CharacterStatus* target, int splitIndex)
{
    int result = 0;

    if (target == 0) {
        return 0;
    }
    if (target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusSleep)
     || target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusSpazz)) {
        result = messageData_.splitMsg_[splitIndex].deactivate;
        this->splitFlag_ = 0;
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageTargetAlive(status::CharacterStatus* target, int splitIndex)
{
    int result = 0;

    if (target == 0) {
        return 0;
    }
    if (!target->haveStatusInfo_.isDeath()) {
        result = messageData_.splitMsg_[splitIndex].alive;
        this->splitFlag_ = 0;
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageTargetDead(status::CharacterStatus* target, int splitIndex)
{
    int result = 0;

    if (target == 0) {
        return 0;
    }
    if (target->haveStatusInfo_.isDeath()) {
        result = messageData_.splitMsg_[splitIndex].dead;
        this->splitFlag_ = 0;
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageTargetAstoron(status::CharacterStatus* target, int splitIndex)
{
    int result = 0;

    if (target == 0) {
        return 0;
    }
    if (target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusAstoron)) {
        result = messageData_.splitMsg_[splitIndex].astoron;
        splitFlag_ = 0;
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageTargetNoMosyasu(status::CharacterStatus* target, int splitIndex)
{
    int result = 0;

    if (target == 0) {
        return 0;
    }
    if (!target->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusMosyasu)) {
        result = messageData_.splitMsg_[splitIndex].mosyasu;
        splitFlag_ = 0;
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageTargetNoSplitNoJouk(status::CharacterStatus* target, int splitIndex)
{
    int result = 0;

    if (target == 0) {
        return 0;
    }
    int msg = messageData_.splitMsg_[splitIndex].splitAvoid;
    if (msg != 0 && status::BaseAction::splitFlag_ == 0 && status::BaseAction::joukFlag_ == 0) {
        result = msg;
        this->splitFlag_ = 0;   
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageTargetSplit(status::CharacterStatus* target, int splitIndex)
{
    int result = 0;

    if (target == 0) {
        return 0;
    }
    int msg = messageData_.splitMsg_[splitIndex].split;
    if (msg != 0 && status::BaseAction::splitFlag_ != 0) {
        result = msg;
        splitFlag_ = 0;
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageTargetJouk(status::CharacterStatus* target, int splitIndex)
{
    int result = 0;

    if (target == 0) {
        return 0;
    }
    int msg = messageData_.splitMsg_[splitIndex].avoid;
    if (msg && status::BaseAction::joukFlag_) {
        result = msg;
        splitFlag_ = 0;
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageWeaponEquipment(status::CharacterStatus* actor, int splitIndex)
{
    int result = 0;

    if (actor == 0) {
        return 0;
    }
    if (messageData_.splitMsg_[splitIndex].equip
     && actor->haveStatusInfo_.haveEquipment_.getEquipment(ITEM_WEAPON)) {
        result = messageData_.splitMsg_[splitIndex].equip;
        splitFlag_ = 0;
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageNoWeaponEquipment(status::CharacterStatus* actor, int splitIndex)
{
    int result = 0;

    if (actor == 0) {
        return 0;
    }
    if (messageData_.splitMsg_[splitIndex].notEquit 
     && actor->haveStatusInfo_.haveEquipment_.getEquipment(ITEM_WEAPON) == 0) {
        result = messageData_.splitMsg_[splitIndex].notEquit;
        splitFlag_ = 0;
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageActorMale(status::CharacterStatus* actor, int splitIndex)
{
    int result = 0;

    if (actor == 0) {
        return 0;
    }
    if (actor->haveStatusInfo_.haveStatus_.sex_  == 0) {
        result = messageData_.splitMsg_[splitIndex].male;
        splitFlag_ = 0;
    }
    return result;
}


THUMB int status::BaseActionMessage::getMessageActorFemale(status::CharacterStatus* actor, int splitIndex)
{
    int result = 0;

    if (actor == 0) {
        return 0;
    }
    if (actor->haveStatusInfo_.haveStatus_.sex_ == 1) {
        result = messageData_.splitMsg_[splitIndex].female;
        splitFlag_ = 0;
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageNoUse(status::CharacterStatus* target, int splitIndex)
{
    int result = 0;

    if (messageData_.splitMsg_[splitIndex].wastePlace != 0) {
        if (actionIndex_ == 0xD0 || actionIndex_ == 0xB2) {
            if (data_0210bb94.unkfunc_02058114(0xC) == 1 && g_Stage.lastFldSurface_ == -1) {
                result = messageData_.splitMsg_[splitIndex].wastePlace;
                splitFlag_ = 0;
            }
            if (g_Stage.isLanaruta() == 0) {
                result = messageData_.splitMsg_[splitIndex].wastePlace;
                splitFlag_ = 0;
            }
        }
        if (actionIndex_ == 0xCB || actionIndex_ == 0xA4 || actionIndex_ == 0xAF) {
            if (g_Stage.isRula() == 0) {
                result = messageData_.splitMsg_[splitIndex].wastePlace;
                splitFlag_ = 0;
            }
        }
        if (actionIndex_ == 0xCC) {
            if (g_Stage.isRiremito() == 0) {
                result = messageData_.splitMsg_[splitIndex].wastePlace;
                splitFlag_ = 0;
            }
        }
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageRulaOff(int splitIndex)
{
    int result = 0;
    int action = actionIndex_;

    if (action != 0xCB && action != 0xA4 && action != 0xAF
     && action != 0xB2 && action != 0xD0) {
        return 0;
    }

    if (messageData_.splitMsg_[splitIndex].rura ) {
        if (action == 0xB2 || action == 0xD0) {
            if (g_Stage.isLanarutaDisable()) {
                result = messageData_.splitMsg_[splitIndex].rura;
                splitFlag_ = 0;
            }
            if (g_Stage.timestop_  ) {     
                result = messageData_.splitMsg_[splitIndex].rura;
                splitFlag_ = 0;
            }
        } else {
            if (g_Stage.isRulaDisable()) {
                result = messageData_.splitMsg_[splitIndex].rura;
                splitFlag_ = 0;
            }
        }
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageRiremitoOff(int splitIndex)
{
    int result = 0;

    if (actionIndex_ != 0xCC) {
        return 0;
    }
    if (messageData_.splitMsg_[splitIndex].riremito 
     && g_Stage.isRiremitoDisable() ) {
        result = messageData_.splitMsg_[splitIndex].riremito;
        splitFlag_ = 0;
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageVainTimeZone(int splitIndex)
{
    int result = 0;

    if (messageData_.splitMsg_[splitIndex].wasteTime != 0) {
        if (actionIndex_ == 0xB2
         && (g_Stage.getTimeZone() == 1 || g_Stage.getTimeZone() == 4)) {
            result = messageData_.splitMsg_[splitIndex].wasteTime;
            splitFlag_ = 0;
        }
        if (actionIndex_ == 0xD4
         && data_0210bb94.unkfunc_02058114(0xC) == 0) {
            result = messageData_.splitMsg_[splitIndex].wasteTime;
            splitFlag_ = 0;
        }
    }
    return result;
}


THUMB int status::BaseActionMessage::getMessageItemInBox(int splitIndex)
{
    int result = 0;

    if (messageData_.splitMsg_[splitIndex].cofferItem != 0
     && data_0210bb94.unkfunc_02058114(0xC) == 1) {
        int uid = TownPlayerManager::getSingleton()->getInpasMapObj();
        if (TownFurnitureManager::getSingleton()->getCofferType(uid) == 7
         && TownFurnitureManager::getSingleton()->checkCoffer(uid) == 2) {
            result = messageData_.splitMsg_[splitIndex].cofferItem;
            splitFlag_ = 0;
        }
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageMonsterInBox(int splitIndex)
{
    int result = 0;

    if (messageData_.splitMsg_[splitIndex].cofferMonster != 0
     && data_0210bb94.unkfunc_02058114(0xC) == 1) {
        int uid = TownPlayerManager::getSingleton()->getInpasMapObj();
        if (TownFurnitureManager::getSingleton()->getCofferType(uid) == 7
         && TownFurnitureManager::getSingleton()->checkCoffer(uid) == 4) {
            result = messageData_.splitMsg_[splitIndex].cofferMonster;
            splitFlag_ = 0;
        }
    }
    return result;
}


THUMB int status::BaseActionMessage::getMessageGoldInBox(int splitIndex)
{
    int result = 0;

    if (messageData_.splitMsg_[splitIndex].cofferGold != 0
     && data_0210bb94.unkfunc_02058114(0xC) == 1) {
        int uid = TownPlayerManager::getSingleton()->getInpasMapObj();
        if (TownFurnitureManager::getSingleton()->getCofferType(uid) == 7
         && TownFurnitureManager::getSingleton()->checkCoffer(uid) == 3) {
            result = messageData_.splitMsg_[splitIndex].cofferGold;
            splitFlag_ = 0;
        }
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageZeroInBox(int splitIndex)
{
    int result = 0;

    if (messageData_.splitMsg_[splitIndex].cofferNothing != 0
     && data_0210bb94.unkfunc_02058114(0xC) == 1) {
        int uid = TownPlayerManager::getSingleton()->getInpasMapObj();
        if (TownFurnitureManager::getSingleton()->getCofferType(uid) == 7
         && TownFurnitureManager::getSingleton()->checkCoffer(uid) == 1) {
            result = messageData_.splitMsg_[splitIndex].cofferNothing;
            splitFlag_ = 0;
        }
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageItemInPot(int splitIndex)
{
    int result = 0;

    if (messageData_.splitMsg_[splitIndex].tuboItem != 0
     && data_0210bb94.unkfunc_02058114(0xC) == 1) {
        int uid = TownPlayerManager::getSingleton()->getInpasMapObj();
        if (TownFurnitureManager::getSingleton()->getCofferType(uid) == 0x27
         && TownFurnitureManager::getSingleton()->checkCoffer(uid) == 2) {
            result = messageData_.splitMsg_[splitIndex].tuboItem;
            splitFlag_ = 0;
        }
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageMonsterInPot(int splitIndex)
{
    int result = 0;

    if (messageData_.splitMsg_[splitIndex].tuboMonster != 0
     && data_0210bb94.unkfunc_02058114(0xC) == 1) {
        int uid = TownPlayerManager::getSingleton()->getInpasMapObj();
        if (TownFurnitureManager::getSingleton()->getCofferType(uid) == 0x27
         && TownFurnitureManager::getSingleton()->checkCoffer(uid) == 4) {
            result = messageData_.splitMsg_[splitIndex].tuboMonster;
            splitFlag_ = 0;
        }
    }
    return result;
}


THUMB int status::BaseActionMessage::getMessageGoldInPot(int splitIndex)
{
    int result = 0;

    if (messageData_.splitMsg_[splitIndex].tuboGold != 0
     && data_0210bb94.unkfunc_02058114(0xC) == 1) {
        int uid = TownPlayerManager::getSingleton()->getInpasMapObj();
        if (TownFurnitureManager::getSingleton()->getCofferType(uid) == 0x27
         && TownFurnitureManager::getSingleton()->checkCoffer(uid) == 3) {
            result = messageData_.splitMsg_[splitIndex].tuboGold;
            splitFlag_ = 0;
        }
    }
    return result;
}



THUMB int status::BaseActionMessage::getMessageZeroInPot(int splitIndex)
{
    int result = 0;

    if (messageData_.splitMsg_[splitIndex].tuboNothing != 0
     && data_0210bb94.unkfunc_02058114(0xC) == 1) {
        int uid = TownPlayerManager::getSingleton()->getInpasMapObj();
        if (TownFurnitureManager::getSingleton()->getCofferType(uid) == 0x27
         && TownFurnitureManager::getSingleton()->checkCoffer(uid) == 1) {
            result = messageData_.splitMsg_[splitIndex].tuboNothing;
            splitFlag_ = 0;
        }
    }
    return result;
}


THUMB int status::BaseActionMessage::getMessageNoTarget(int splitIndex)
{
    int result = 0;

    if (messageData_.splitMsg_[splitIndex].noTarget != 0) {
        if (data_0210bb94.unkfunc_02058114(0xC) == 1) {
            int uid = TownPlayerManager::getSingleton()->getInpasMapObj();
            int type = TownFurnitureManager::getSingleton()->getCofferType(uid);
            if (uid == 0 || (type != 0x27 && type != 7)) {
                result = messageData_.splitMsg_[splitIndex].noTarget;
                splitFlag_ = 0;
            }
        } else {
            result = messageData_.splitMsg_[splitIndex].noTarget;
            splitFlag_ = 0;
        }
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageNorthEast(int splitIndex)
{
    int result = 0;

    if (messageData_.splitMsg_[splitIndex].northEast != 0
     && data_0210bb94.unkfunc_02058114(0xE) != 0) {
        char dx = TownMenuPlayerControl::getSingleton()->takanomeX_;
        char dy = TownMenuPlayerControl::getSingleton()->takanomeY_;
        if (dx <= 0 && dy >= 0) {
            result = messageData_.splitMsg_[splitIndex].northEast;
            splitFlag_ = 0;
        }
    }
    return result;
}


THUMB int status::BaseActionMessage::getMessageSouthEast(int splitIndex)
{
    int result = 0;

    if (messageData_.splitMsg_[splitIndex].northEast != 0     
     && data_0210bb94.unkfunc_02058114(0xE) != 0) {
        char dx = TownMenuPlayerControl::getSingleton()->takanomeX_;
        char dy = TownMenuPlayerControl::getSingleton()->takanomeY_;
        if (dx <= 0 && dy < 0) {
            result = messageData_.splitMsg_[splitIndex].southEast;
            splitFlag_ = 0;
        }
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageNorthWest(int splitIndex)
{
    int result = 0;

    if (messageData_.splitMsg_[splitIndex].northWest != 0
     && data_0210bb94.unkfunc_02058114(0xE) != 0) {
        char dx = TownMenuPlayerControl::getSingleton()->takanomeX_;
        char dy = TownMenuPlayerControl::getSingleton()->takanomeY_;
        if (dx > 0 && dy >= 0) {
            result = messageData_.splitMsg_[splitIndex].northWest;
            splitFlag_ = 0;
        }
    }
    return result;
}

THUMB int status::BaseActionMessage::getMessageSouthWest(int splitIndex)
{
    int result = 0;

    if (messageData_.splitMsg_[splitIndex].southWest != 0
     && data_0210bb94.unkfunc_02058114(0xE) != 0) {
        char dx = TownMenuPlayerControl::getSingleton()->takanomeX_;
        char dy = TownMenuPlayerControl::getSingleton()->takanomeY_;
        if (dx > 0 && dy < 0) {
            result = messageData_.splitMsg_[splitIndex].southWest;
            splitFlag_ = 0;
        }
    }
    return result;
}


THUMB int status::BaseActionMessage::getMessageZero(int splitIndex)  
{
    int result = 0;

    if (messageData_.splitMsg_[splitIndex].nothing != 0
     && data_0210bb94.unkfunc_02058114(0xC) == 1) {
        if (TownFurnitureManager::getSingleton()->searchFloorItem() == 0) {
            result = messageData_.splitMsg_[splitIndex].nothing;
            splitFlag_ = 0;
        }
    }
    return result;
}