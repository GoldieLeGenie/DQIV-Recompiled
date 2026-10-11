#include "main/btl/BattleAutoFeed.hpp"
#include "main/menu/MenuAPI.hpp"
#include "main/status/OptionStatus.hpp"

int BattleAutoFeed::waitCounter_;
int BattleAutoFeed::counter_;
int BattleAutoFeed::encountCounter_;
int BattleAutoFeed::executeCounter_;
int BattleAutoFeed::resultCounter_;
int BattleAutoFeed::afterCounter_;
int BattleAutoFeed::speed_;
int BattleAutoFeed::DEBUG_WAIT = 0x1e;

THUMB void BattleAutoFeed::setMessage()
{
    counter_ = 0;
}

THUMB bool BattleAutoFeed::isEndMessage()
{
    if (waitCounter_ == WAIT_FRAME_4) {
        if (++counter_ > WAIT_FRAME_0 && isEnd()) {
            return true;
        }
    } else if (isFinish() || isNext()) {
        if (++counter_ > waitCounter_) {
            if (isNext()) {
                sendNext();
                counter_ = 0;
            } else if (isFinish()) {
                return true;
            }
        }
    }
    return false;
}

THUMB void BattleAutoFeed::setMessageSend()
{
    counter_ = 0;
    MenuAPI::setMessageCursor(true);
    MenuAPI::suspendMessageKeyInput(false);
}

THUMB bool BattleAutoFeed::isEndMessageSend()
{
    if (isEndBattleEnd() != false) {
        return true;
    }
    return false;
}

THUMB void BattleAutoFeed::setEncountMessage()
{
    MenuAPI::setMessageCursor(false);
    encountCounter_ = 0;
    setCursor();
}

THUMB bool BattleAutoFeed::isEndEncountMessage()
{
    if (waitCounter_ == WAIT_FRAME_4) {
        if (encountCounter_ > WAIT_FRAME_0 && isEnd()) {
            setCursor();
            return true;
        }
    } else if (MenuAPI::isFinishMessage() || MenuAPI::isEndMessage()) {
        if (encountCounter_ > waitCounter_) {
            return true;
        }
    }
    encountCounter_++;
    return false;
}

THUMB void BattleAutoFeed::setExecuteMessage()
{
    executeCounter_ = 0;
    setCursor();
}

THUMB bool BattleAutoFeed::isEndExecuteMessage()
{
    if (waitCounter_ == WAIT_FRAME_4) {
        if (++executeCounter_ > WAIT_FRAME_0 && isEnd()) {
            return true;
        }
    } else if (isFinish() || isNext()) {
        if (++executeCounter_ > waitCounter_) {
            if (isNext()) {
                sendNext();
                executeCounter_ = 0;
            } else if (isFinish()) {
                return true;
            }
        }
    }
    return false;
}

THUMB void BattleAutoFeed::setResultMessage()
{
    resultCounter_ = 0;
    setCursor();
}

THUMB bool BattleAutoFeed::isEndResultMessage()
{
    if (waitCounter_ == WAIT_FRAME_4) {
        if (++resultCounter_ > WAIT_FRAME_0 && isEnd()) {
            return true;
        }
    } else if (isFinish() || isNext()) {
        if (++resultCounter_ > waitCounter_) {
            if (isNext()) {
                sendNext();
                resultCounter_ = 0;
            } else if (isFinish()) {
                return true;
            }
        }
    }
    return false;
}

THUMB void BattleAutoFeed::setAfterMessage()
{
    afterCounter_ = 0;
    setCursor();
}

THUMB bool BattleAutoFeed::isEndAfterMessage()
{
    if (waitCounter_ == WAIT_FRAME_4) {
        if (++afterCounter_ > WAIT_FRAME_0 && isEnd()) {
            return true;
        }
    } else if (isFinish() || isNext()) {
        if (++afterCounter_ > waitCounter_) {
            if (isNext()) {
                sendNext();
                afterCounter_ = 0;
            } else if (isFinish()) {
                return true;
            }
        }
    }
    return false;
}

THUMB void BattleAutoFeed::setMessageSpeed()
{
    switch (speed_) {
    case MESSAGE_SPEED_1:
        waitCounter_ = WAIT_FRAME_0;
        break;
    case MESSAGE_SPEED_2:
        waitCounter_ = WAIT_FRAME_1;
        break;
    case MESSAGE_SPEED_3:
        waitCounter_ = WAIT_FRAME_2;
        break;
    case MESSAGE_SPEED_4:
        waitCounter_ = WAIT_FRAME_3;
        break;
    case MESSAGE_SPEED_5:
        waitCounter_ = WAIT_FRAME_4;
        break;
    case MESSAGE_SPEED_DEBUG:
        waitCounter_ = DEBUG_WAIT;
        break;
    }
}

THUMB int BattleAutoFeed::getMessageSpeed()
{
    switch (speed_) {
    case MESSAGE_SPEED_1:
        return WAIT_FRAME_0;
    case MESSAGE_SPEED_2:
        return WAIT_FRAME_1;
    case MESSAGE_SPEED_3:
        return WAIT_FRAME_2;
    case MESSAGE_SPEED_4:
        return WAIT_FRAME_3;
    case MESSAGE_SPEED_5:
        return WAIT_FRAME_4;
    case MESSAGE_SPEED_DEBUG:
        return DEBUG_WAIT;
    }
    return 0;
}

THUMB void BattleAutoFeed::setCursor()
{
    speed_ = g_Option.getBattleSpeed();
    MenuAPI::setMessageCursor(false);
    MenuAPI::suspendMessageKeyInput(true);
    setMessageSpeed();
}

THUMB void BattleAutoFeed::setCursorWaiting()
{
    speed_ = g_Option.getBattleSpeed();
    if (speed_ != MESSAGE_SPEED_5) {
        MenuAPI::setMessageCursor(false);
        MenuAPI::suspendMessageKeyInput(true);
    } else {
        MenuAPI::setMessageCursor(true);
        MenuAPI::suspendMessageKeyInput(false);
    }
    setMessageSpeed();
}

THUMB void BattleAutoFeed::setCursorBattleEnd()
{
    MenuAPI::suspendMessageKeyInput(false);
}

THUMB bool BattleAutoFeed::isFinish()
{
    if (MenuAPI::isFinishMessage()) {
        return true;
    }
    if (MenuAPI::isEndMessage()) {
        return true;
    }
    return false;
}

THUMB bool BattleAutoFeed::isNext()
{
    if (MenuAPI::isMessageWaitTrigger()) {
        return true;
    }
    return false;
}

THUMB bool BattleAutoFeed::isEnd()
{
    if (MenuAPI::isFinishMessage() || MenuAPI::isMessageWaitTrigger()) {
        setCursorWaiting();
    }
    if (MenuAPI::isEndMessage()) {
        return true;
    }
    return false;
}

THUMB bool BattleAutoFeed::isEndBattleEnd()
{
    if (MenuAPI::isFinishMessage() || MenuAPI::isMessageWaitTrigger()) {
        setCursorBattleEnd();
    }
    if (MenuAPI::isEndMessage()) {
        return true;
    }
    return false;
}

THUMB void BattleAutoFeed::sendNext()
{
    speed_ = g_Option.getBattleSpeed();
    if (speed_ == MESSAGE_SPEED_5) {
        MenuAPI::clearMessageWaitTriggerSE();
    } else {
        MenuAPI::clearMessageWaitTriggerNOSE();
    }
}

THUMB void BattleAutoFeed::printCounter()
{
}

THUMB void BattleAutoFeed::disableAutoFeed()
{
    waitCounter_ = WAIT_FRAME_4;
}

THUMB void BattleAutoFeed::setDisableCursor(bool flag)
{
    if (flag) {
        MenuAPI::setMessageCursor(false);
    } else {
        MenuAPI::setMessageCursor(true);
    }
}
