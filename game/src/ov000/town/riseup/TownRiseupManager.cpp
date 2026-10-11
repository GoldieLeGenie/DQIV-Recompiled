#include "ov000/town/riseup/TownRiseup.hpp"
#include "ov000/town/TownCamera.hpp"

int TownRiseupManager::riseupCounter_;

THUMB TownRiseupManager::TownRiseupManager()
{
}

THUMB TownRiseupManager::~TownRiseupManager()
{
}

THUMB TownRiseupManager* TownRiseupManager::getSingleton()
{
    static TownRiseupManager m_singleton;
    return &m_singleton;
}

THUMB void TownRiseupManager::initialize()
{
    riseupStorage_.initialize();
    riseupResourece_.initialize();
    effectResourece_.initialize();
    for (int i = 0; i < 16; i++) {
        riseup_[i] = NULL;
    }
    TownRiseupBase::setCamera(&TownCamera::getSingleton()->camera_.unk_004);
    riseupCounter_ = 0;
}

THUMB void TownRiseupManager::terminate()
{
    for (int i = 0; i < 16; i++) {
        if (riseup_[i] != NULL) {
            cleanup(i);
        }
    }
    riseupStorage_.terminate();
    riseupResourece_.terminate();
    effectResourece_.terminate();
}

THUMB int TownRiseupManager::setup(int type, dss::Fix32Vector3 pos)
{
    for (int i = 0; i < 16; i++) {
        if (riseup_[i] == NULL) {
            riseup_[i] = riseupStorage_.getContainer(0);
            riseup_[i]->setResource(riseupResourece_.getResource(type));
            riseup_[i]->setup(type);
            riseup_[i]->setPosition(pos);
            riseupCounter_++;
            return i;
        }
    }
    return 0;
}

THUMB int TownRiseupManager::setupMedal(dss::Fix32Vector3 pos)
{
    for (int i = 0; i < 16; i++) {
        if (riseup_[i] == NULL) {
            riseup_[i] = riseupStorage_.getContainer(3);
            riseup_[i]->setResource(effectResourece_.getResource(0x393));
            riseup_[i]->setup(0x393);
            riseup_[i]->setPosition(pos);
            riseupCounter_++;
            return i;
        }
    }
    return 0;
}

THUMB int TownRiseupManager::setupSprite(int type, dss::Fix32Vector3 pos, int flag, int wait)
{
    for (int i = 0; i < 16; i++) {
        if (riseup_[i] == NULL) {
            riseup_[i] = riseupStorage_.getContainer(1);
            riseup_[i]->setResource(effectResourece_.getResource(type));
            riseup_[i]->startCounter_ = wait;
            riseup_[i]->setup(type);
            riseup_[i]->setPosition(pos);
            riseup_[i]->setupNear(flag);
            riseupCounter_++;
            return i;
        }
    }
    return 0;
}

THUMB int TownRiseupManager::setupScript(int type, dss::Fix32Vector3 start, dss::Fix32Vector3 end, int frame)
{
    for (int i = 0; i < 16; i++) {
        if (riseup_[i] == NULL) {
            riseup_[i] = riseupStorage_.getContainer(2);
            riseup_[i]->setResource(riseupResourece_.getResource(type));
            riseup_[i]->setup(type);
            riseup_[i]->setScriptData(start, end, frame);
            riseupCounter_++;
            return i;
        }
    }
    return 0;
}

THUMB int TownRiseupManager::setupSpriteMove(int type, dss::Fix32Vector3 start, dss::Fix32Vector3 end, int frame)
{
    for (int i = 0; i < 16; i++) {
        if (riseup_[i] == NULL) {
            riseup_[i] = riseupStorage_.getContainer(1);
            riseup_[i]->setResource(effectResourece_.getResource(type));
            riseup_[i]->setup(type);
            riseup_[i]->setScriptData(start, end, frame);
            riseupCounter_++;
            return i;
        }
    }
    return 0;
}

THUMB int TownRiseupManager::setupSpriteFade(int type, dss::Fix32Vector3 pos, int frame, int flag)
{
    for (int i = 0; i < 16; i++) {
        if (riseup_[i] == NULL) {
            riseup_[i] = riseupStorage_.getContainer(1);
            riseup_[i]->setResource(effectResourece_.getResource(type));
            riseup_[i]->setup(type);
            riseup_[i]->setScriptFade(pos, frame, flag);
            riseupCounter_++;
            return i;
        }
    }
    return 0;
}

THUMB void TownRiseupManager::cleanup(int index)
{
    riseupStorage_.restoreContainer(riseup_[index]->getType());
    if (riseup_[index]->getResorceType() == 0) {
        riseupResourece_.restoreResource(riseup_[index]->index_);
    } else {
        effectResourece_.restoreResource(riseup_[index]->index_);
    }
    riseup_[index]->cleanup();
    riseup_[index] = NULL;
    riseupCounter_--;
}

THUMB void TownRiseupManager::draw()
{
    if (riseupCounter_ != 0) {
        for (int i = 0; i < 16; i++) {
            if (riseup_[i] != NULL) {
                riseup_[i]->draw();
            }
        }
    }
}

THUMB void TownRiseupManager::execute()
{
    if (riseupCounter_ != 0) {
        for (int i = 0; i < 16; i++) {
            if (riseup_[i] != NULL) {
                riseup_[i]->execute();
                if (riseup_[i]->enable_ == 0 && isGarbageCorrect(i)) {
                    cleanup(i);
                }
            }
        }
    }
}

THUMB bool TownRiseupManager::isFinish(int index)
{
    if (riseup_[index] == NULL) {
        return true;
    }
    return riseup_[index]->isFinish();
}

THUMB bool TownRiseupManager::isGarbageCorrect(int index)
{
    return riseup_[index]->flag_.flag_ & TownRiseupBase::FALG_GARBAGE_CORRECTION;
}
