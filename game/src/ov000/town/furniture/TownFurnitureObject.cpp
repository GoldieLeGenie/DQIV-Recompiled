#include "ov000/town/TownFurniture.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "main/cmn/TalkSoundManager.hpp"
#include "main/sound/SoundManager.hpp"
#include "ov000/town/TownWindowSystem.hpp"

THUMB TownFurnitureObject::TownFurnitureObject()
{
}

THUMB TownFurnitureObject::~TownFurnitureObject()
{
}

THUMB void TownFurnitureObject::setup(int uid, int data, param::CommonParam* common, int flag)
{
    uid_ = uid;
    data_ = data;
    common_ = common;
    phase_ = PHASE_FIRST_MESSAGE;
    index_ = -1;
    furniture_.flag_ = 0;
    furniture_.flag_ |= DEFAULT_SETTING;
    if ((char)(common_->byte_1 & 1)) {
        if (flag) {
            furniture_.flag_ &= ~OPEN_ANIM_ENABLE;
        } else {
            furniture_.flag_ &= ~CHECK_MESSAGE_FLAG;
        }
    }
}

THUMB void TownFurnitureObject::setupExtend(int data)
{
}

THUMB void TownFurnitureObject::cleanup()
{
}

THUMB void TownFurnitureObject::execute()
{
    switch (phase_) {
    case PHASE_FIRST_MESSAGE:
        if (checkMsg() != 0 || common_->normalMsg != 0) {
            phase_ = PHASE_WAIT_OPEN;
            setMessage();
        } else {
            phase_ = PHASE_OPEN;
        }
        break;
    case PHASE_WAIT_OPEN:
        if (TownWindowSystem::getSingleton()->isWait()) {
            phase_ = PHASE_OPEN;
        }
        if (!TownWindowSystem::getSingleton()->cmdWindow_.isMessage()) {
            cleanup();
            phase_ = PHASE_NONE;
        }
        break;
    case PHASE_OPEN:
        openObject();
        startRiseup();
        phase_ = PHASE_WAIT_CLOSE;
        break;
    case PHASE_WAIT_CLOSE:
        if (isRiseupEnd()) {
            endRiseup();
            phase_ = PHASE_CLOSE;
        }
        break;
    case PHASE_CLOSE:
        if (TownWindowSystem::getSingleton()->isWait()) {
            phase_ = PHASE_EXTEND_CLOSE;
        }
        if (!TownWindowSystem::getSingleton()->cmdWindow_.isMessage() && TownStageManager::getSingleton()->isCommonAnimationEnd(uid_)) {
            closeObject();
            cleanup();
            phase_ = PHASE_NONE;
        }
        break;
    case PHASE_EXTEND_CLOSE:
        if (soundStart()) {
            phase_ = PHASE_CLOSE;
        }
        break;
    }
    if (furniture_.flag_ & CHECK_ANIM_FLAG) {
        if (TownStageManager::getSingleton()->isCommonAnimationEnd(uid_)) {
            TownStageManager::getSingleton()->setObjectDraw(uid_, ANIM_NONE, 1);
            furniture_.flag_ &= ~CHECK_ANIM_FLAG;
        }
    }
}

THUMB void TownFurnitureObject::setFirstMessage()
{
    if (checkMsg() != 0 || common_->normalMsg != 0) {
        if (common_->checkMsg != 0) {
            addMessage(common_->checkMsg, false);
        }
        if (common_->normalMsg != 0) {
            addMessage(common_->normalMsg, false);
        }
        if (data_ != 0) {
            TownWindowSystem::getSingleton()->waitCommonMessage();
        }
    }
}

THUMB void TownFurnitureObject::setMessage()
{
    openWindow_ = 1;
    setFirstMessage();
    setSecondMessage();
}

THUMB void TownFurnitureObject::openObject()
{
    if (furniture_.flag_ & OPEN_ANIM_ENABLE) {
        if ((char)((common_->byte_1 & 2) >> 1)) {
            TownStageManager::getSingleton()->setObjectDraw(uid_, ANIM_OPEN, 1);
            SoundManager::playSe(common_->sound, 0);
        }
        if ((char)(common_->byte_1 & 1)) {
            furniture_.flag_ |= CHECK_ANIM_FLAG;
        }
    }
}

THUMB void TownFurnitureObject::closeObject()
{
    if ((furniture_.flag_ & CLOSE_ANIM_ENABLE) && (char)((common_->byte_1 & 2) >> 1) && !(char)(common_->byte_1 & 1)) {
        TownStageManager::getSingleton()->setObjectDraw(uid_, ANIM_CLOSE, 1);
        SoundManager::playSe(common_->sound, 0);
    }
}

THUMB void TownFurnitureObject::startRiseup()
{
    index_ = -1;
}

THUMB bool TownFurnitureObject::endRiseup()
{
    if (checkMsg() == 0 && common_->normalMsg == 0) {
        setMessage();
    }
    TownWindowSystem::getSingleton()->clearCommonMessage();
    return false;
}

THUMB bool TownFurnitureObject::isRiseupEnd()
{
    return true;
}

THUMB dss::Fix32Vector3 TownFurnitureObject::getFurnPosition()
{
    return TownStageManager::getSingleton()->getRiseupPos(uid_, common_->type);
}

THUMB unsigned int TownFurnitureObject::checkMsg()
{
    if (furniture_.flag_ & CHECK_MESSAGE_FLAG) {
        return common_->checkMsg;
    }
    if ((char)(common_->byte_1 & 1)) {
        return 0;
    }
    return common_->checkMsg;
}

THUMB void TownFurnitureObject::addMessage(int message, bool serial)
{
    if (openWindow_ != 0) {
        openWindow_ = 0;
        ui_MsgSndSet(0x30);
        TownWindowSystem::getSingleton()->openCommonMessage();
    }
    if (serial) {
        TownWindowSystem::getSingleton()->serialCommonMessage(message);
    } else {
        TownWindowSystem::getSingleton()->addCommonMessage(message);
    }
}

THUMB bool TownFurnitureObject::isFinish()
{
    return phase_ == PHASE_NONE;
}

THUMB bool TownFurnitureObject::soundStart()
{
    return true;
}
