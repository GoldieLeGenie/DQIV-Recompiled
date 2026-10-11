#include "ov003/btl/BattleMessage.hpp"
#include "ov003/btl/BattleActorManager2.hpp"
#include "ov003/btl/BattleExecVictory.hpp"



THUMB void btl::BattleMessage::setMessage(int message1, int message2, int message3, int message4)
{
    if ((message1 | message2 | message3 | message4) == 0) {
        return;
    }

    MenuAPI::openBattleMessage();

    if (message1 != 0) {
        MenuAPI::addMessageSerial(message1);
    }
    if (message2 != 0) {
        MenuAPI::addMessageSerial(message2);
    }
    if (message3 != 0) {
        MenuAPI::addMessageSerial(message3);
    }
    if (message4 != 0) {
        MenuAPI::addMessageSerial(message4);
    }

    BattleAutoFeed::setCursor();
    BattleAutoFeed::setMessage();
}

THUMB void btl::BattleMessage::setMessageInTown(int message1, int message2, int message3, int message4)
{
    MenuAPI::openCommonMessage();

    if (message1 != 0) {
        MenuAPI::addMessageSerial(message1);
    }
    if (message2 != 0) {
        MenuAPI::addMessageSerial(message2);
    }
    if (message3 != 0) {
        MenuAPI::addMessageSerial(message3);
    }
    if (message4 != 0) {
        MenuAPI::addMessageSerial(message4);
    }

    BattleAutoFeed::setCursor();
    BattleAutoFeed::setMessage();
}

THUMB int btl::BattleMessage::setBeforeMessage(status::UseActionParam* useActionParam)
{
    int message[4] = {0, 0, 0, 0};
    status::UseActionMessage* msg = &useActionParam->message_[0];

    message[0] = msg->beforeMessage_[0];
    message[1] = msg->beforeMessage_[1];

    if (message[0] + message[1] == 0) {
        return 0;
    }

    setMessage(message[0], message[1], 0, 0);
    return 1;
}

THUMB int btl::BattleMessage::setExecMessage(status::UseActionParam* useActionParam)
{
    int message[4] = {0, 0, 0, 0};
    status::UseActionMessage* msg = &useActionParam->message_[0];

    message[0] = msg->execMessage_[0];
    message[1] = msg->execMessage_[1];
    message[2] = msg->execMessage_[2];
    message[3] = msg->execMessage_[3];

    if (message[0] + message[1] + message[2] + message[3] == 0) {
        return 0;
    }

    setMessage(message[0], message[1], message[2], message[3]);
    return 1;
}

THUMB int btl::BattleMessage::setSpecialMessage(status::UseActionParam* useActionParam, int currentTarget)
{
    int message[4] = {0, 0, 0, 0};
    status::UseActionMessage* msg = &useActionParam->message_[currentTarget];

    message[0] = msg->spclMessage_[0];
    message[1] = msg->spclMessage_[1];

    if (message[0] + message[1] + message[2] + message[3] == 0) {
        return 0;
    }

    setMessage(message[0], message[1], message[2], message[3]);
    return 1;
}

THUMB int btl::BattleMessage::setResultMessage(status::UseActionParam* useActionParam, int index)
{
    int message[3] = {0, 0, 0};
    status::UseActionMessage* msg = &useActionParam->message_[index];

    message[0] = msg->resultMessage_[0];
    message[1] = msg->resultMessage_[1];

    if (message[0] + message[1] == 0) {
        return 0;
    }

    setMessage(message[0], message[1], 0, 0);
    return 1;
}

THUMB int btl::BattleMessage::setAfterMessage(status::UseActionParam* useActionParam, int index)
{
    int message[4] = {0, 0, 0, 0};

    if (btl::BattleActorManager2::getSingleton()->eventType_ == btl::BattleActorManager2::Velorinman) {
        int ctrlId = useActionParam->targetCharacterStatus_[0]->haveStatusInfo_.drawCtrlId_;
        if (useActionParam->actorCharacterStatus_->characterType_ == PLAYER) {
            if (ctrlId != btl::BattleExecEvent00::getRealVelorinman()) {
                if (status::UseAction::getDamageType(useActionParam->actionIndex_) == status::UseAction::DamageTypeDamage) {
                    TextAPI::setMACRO0(18, 0x60000000, 170);
                    setMessage(0xc3936, 0, 0, 0);
                    return 1;
                }
            }
        }
    }

    status::UseActionMessage* msg = &useActionParam->message_[index];

    message[0] = msg->addMessage_[0];
    message[1] = msg->addMessage_[1];

    if (message[0] + message[1] == 0) {
        return 0;
    }

    setMessage(message[0], message[1], 0, 0);
    return 1;
}

THUMB void btl::BattleMessage::setShakeMessage(status::UseActionParam* useActionParam, int currentTarget)
{
    status::CharacterStatus* target = useActionParam->targetCharacterStatus_[currentTarget];

    if (target == 0) {
        return;
    }
    if (useActionParam->result_ == 0) {
        return;
    }
    if (status::UseAction::getDamageType(useActionParam->actionIndex_) != status::UseAction::DamageTypeDamage) {
        return;
    }
    if (target->characterType_ != PLAYER) {
        return;
    }
    if (target->haveStatusInfo_.effectValue_ != 0) {
        MenuAPI::shakeMessage();
    }
}

THUMB void btl::BattleMessage::openEncountMessage()
{
    MenuAPI::openEncountMessage();
}

THUMB void btl::BattleMessage::addEncountMessage(int message)
{
    MenuAPI::catMessage(message);
    BattleAutoFeed::setMessage();
}