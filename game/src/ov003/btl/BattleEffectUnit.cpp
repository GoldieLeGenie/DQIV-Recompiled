#include "ov003/btl/BattleEffectUnit.hpp"
#include "ov003/btl/BattleCamera.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"
#include "main/object/DSSAObject.hpp"
#include "main/cmn/CommonEffectLocation.hpp"
#include "main/data/FileLoader.hpp"
#include "main/dss/Random.hpp"
#include "main/script/ScriptSystem.hpp"
#include "main/sound/SoundManager.hpp"
#include "nitro/os.hpp"

cmn::CommonEffectResource* btl::BattleEffectUnit::resource_;
btl::BattleEffectStorage* btl::BattleEffectUnit::storage_;

#pragma profile on
THUMB btl::BattleEffectUnit::BattleEffectUnit()
{
}

THUMB btl::BattleEffectUnit::~BattleEffectUnit()
{
}

THUMB void btl::BattleEffectUnit::setControlData(BattleEffectStorage* storage, cmn::CommonEffectResource* resource)
{
    storage_ = storage;
    resource_ = resource;
}

THUMB void btl::BattleEffectUnit::initialize()
{
    cameraWait_ = 0;
    effect_ = 0;
    start_ = 0;
    max_ = 0;
    process_ = 0;
    pass_ = 0;
    frame_ = 0;
    for (int i = 0; i < 12; i++) {
        group_[i] = 0;
    }
}

THUMB void btl::BattleEffectUnit::terminate()
{
    for (int i = 0; i < 12; i++) {
        if (group_[i] != 0 && group_[i]->isEnable()) {
            cleanupEffectGroup(i);
        }
    }
}

THUMB void btl::BattleEffectUnit::setTarget(status::UseActionParam& useActionParam, int flag)
{
    if (effect_ == 0) {
        return;
    }

    int i = 0;
    max_ = 0;
    if (effect_->interval != 0) {
        for (i = 0; i < useActionParam.targetCount_; i++) {
            if (flag == 1 && useActionParam.targetCharacterStatus_[i]->characterType_ == PLAYER) {
                continue;
            }
            if ((char)(effect_->byte_1 & 1) && useActionParam.targetCharacterStatus_[i]->haveStatusInfo_.effectValue_ == 0) {
                continue;
            }
            if (result_[i]) {
                setEffectPosition(max_, useActionParam.targetCharacterStatus_[i]->haveStatusInfo_.drawCtrlId_, 1);
                max_++;
            }
            else {
                targetPos_[max_] = dss::Fix32Vector3(0, -0x80000, 0);
                max_++;
            }
        }
    }
    else {
        max_ = 1;
        status::CharacterStatus* actor = useActionParam.actorCharacterStatus_;
        if (actor->characterType_ == MONSTER) {
            if (useActionParam.targetCharacterStatus_[0]->characterType_ == PLAYER) {
                setEffectPosition(i, actor->haveStatusInfo_.drawCtrlId_, 1);
            }
            else {
                setEffectPosition(i, useActionParam.targetCharacterStatus_[0]->haveStatusInfo_.drawCtrlId_, 1);
            }
        }
        else {
            setEffectPosition(i, useActionParam.targetCharacterStatus_[0]->haveStatusInfo_.drawCtrlId_, 1);
        }
    }

    if (useActionParam.actorCharacterStatus_->characterType_ == MONSTER) {
        cameraWait_ = 0;
    }
    else {
        cameraWait_ = 1;
    }
    int homing = effect_->homing;
    if (homing != 0 && max_ == 1) {
        BattleCamera::getSingleton()->homing_.setRotateTime(homing);
        BattleCamera::getSingleton()->setHomingWaitTime(effect_->hold);
        BattleCamera::getSingleton()->setHomingTarget(useActionParam.targetCharacterStatus_[0]->haveStatusInfo_.drawCtrlId_);
    }
    if ((char)((effect_->byte_1 & 6) >> 1) == DISPLAY_RANDOM) {
        shufflePosition();
    }
}

THUMB void btl::BattleEffectUnit::setTarget(status::CharacterStatus* target, int nullType)
{
    if (effect_ == 0 || (char)(effect_->byte_1 & 1)) {
        return;
    }

    max_ = 1;
    setEffectPosition(0, target->haveStatusInfo_.drawCtrlId_, nullType);
    if (target->characterType_ == MONSTER) {
        cameraWait_ = 0;
    }
    else {
        cameraWait_ = 1;
    }
    int homing = effect_->homing;
    if (homing != 0 && max_ == 1) {
        BattleCamera::getSingleton()->homing_.setRotateTime(homing);
        BattleCamera::getSingleton()->setHomingWaitTime(effect_->hold);
        BattleCamera::getSingleton()->setHomingTarget(target->haveStatusInfo_.drawCtrlId_);
    }
    if ((char)((effect_->byte_1 & 6) >> 1) == DISPLAY_RANDOM) {
        shufflePosition();
    }
}

THUMB void btl::BattleEffectUnit::setSpecialTarget(int drawCtrlId, int nullType)
{
    if (effect_ != 0) {
        max_ = 1;
        setEffectPosition(0, drawCtrlId, nullType);
        cameraWait_ = 0;
    }
}

THUMB void btl::BattleEffectUnit::setup(param::EffectParam* effect)
{
    char file[0x80];

    effect_ = effect;
    dss::sprintf_s(file, sizeof(file), "data/effect/effect%03d.lz", effect_->index);
    if (dss::g_File.isExist(file) == 0) {
        effect_ = 0;
    }
    else {
        for (int i = 0; i < 12; i++) {
            result_[i] = 1;
        }
        start_ = 0;
        frame_ = 0;
        process_ = 0;
        pass_ = 0;
        hit_ = 0;
    }
}

THUMB void btl::BattleEffectUnit::cleanup()
{
    effect_ = 0;
    start_ = 0;
    for (int i = 0; i < 12; i++) {
        group_[i] = 0;
    }
}

THUMB void btl::BattleEffectUnit::setEffectPosition(int index, int drawCtrlId, int nullType)
{
    btl::BattleMonsterDraw2* monsterDraw = btl::BattleMonsterDraw2::getSingleton();
    btl::BattleMonster* monster = &monsterDraw->monsters_[drawCtrlId];
    int type = (char)((effect_->byte_1 & 0x78) >> 3);
    if (type == 0) {
        targetPos_[index] = dss::Fix32Vector3(0, 0, 0);
    }
    else if (type <= 3) {
        targetPos_[index] = (dss::Fix32Vector3(*((Position*)monster)->getPosition()) - monster->getNullPosition(1, type));
    }
    else {
        targetPos_[index] = (dss::Fix32Vector3(*((Position*)monster)->getPosition()) - monster->getNullPosition(nullType, type));
    }

    int range = (char)(effect_->byte_2 & 0xf);
    if (range != 0) {
        int x = dssrand::rand(range << 8) - (range << 7);
        range = (char)(effect_->byte_2 & 0xf);
        int y = dssrand::rand(range << 8) - (range << 7);
        targetPos_[index].vx.value += x;
        targetPos_[index].vy.value += y;
    }
    targetPos_[index].vz.value += 0x100;
}

THUMB void btl::BattleEffectUnit::shufflePosition()
{
    if (max_ > 1) {
        for (int i = 0; i < 16; i++) {
            int a = dssrand::rand(max_);
            int b = dssrand::rand(max_);
            dss::Fix32Vector3 temp = targetPos_[a];
            targetPos_[a] = targetPos_[b];
            targetPos_[b] = temp;
        }
    }
}

THUMB void btl::BattleEffectUnit::setupEffectGroup(int index)
{
    char file[0x80];

    dss::sprintf_s(file, sizeof(file), "data/effect/effect%03d.lz", effect_->index);
    if (dss::g_File.isExist(file) == 0) {
        return;
    }

    cmn::CommonEffectData* data = resource_->getResource(effect_->index);
    group_[index] = storage_->getContainer();
    BattleEffectGroup* group = group_[index];
    if (resource_->getRefCounter(effect_->index) == 1) {
        group->addEffect(data, 1);
    }
    else {
        group->addEffect(data, 0);
    }

    if (cmn::CommonEffectData::isSecondEffect(effect_->index)) {
        cmn::CommonEffectData* data2 = resource_->getResource(effect_->index + 10000);
        if (resource_->getRefCounter(effect_->index + 10000) == 0) {
            group->addEffect(data2, 1);
        }
        else {
            group->addEffect(data2, 0);
        }
    }

    group->setPosition(targetPos_[index]);
    group->setDisplayType((char)((effect_->byte_1 & 6) >> 1), 0);
    group->setDisplayType((char)((effect_->byte_1 & 6) >> 1), 1);
    group->setScale(dss::Fix32(effect_->scale));
}

THUMB void btl::BattleEffectUnit::cleanupEffectGroup(int index)
{
    if (resource_->getRefCounter(effect_->index) == 1) {
        group_[index]->cleanup(1);
    }
    else {
        group_[index]->cleanup(0);
    }
    group_[index] = 0;
    pass_++;
    storage_->restoreContainer();
    resource_->restoreResource(effect_->index);
    if (cmn::CommonEffectData::isSecondEffect(effect_->index)) {
        resource_->restoreResource(effect_->index + 10000);
    }
}

THUMB void btl::BattleEffectUnit::draw()
{
    if (start_ != 0) {
        for (int i = pass_; i < max_; i++) {
            group_[i]->draw();
        }
    }
}

THUMB void btl::BattleEffectUnit::extraDraw()
{
    if (start_ != 0) {
        for (int i = pass_; i < max_; i++) {
            group_[i]->extraDraw();
        }
    }
}

THUMB void btl::BattleEffectUnit::waitStart()
{
    char file[0x80];
    char name2[0x80];

    if (effect_ == 0) {
        return;
    }
    if (start_ != 0) {
        return;
    }
    if (frame_ == 0) {
        if (storage_->getContainerStock() < max_) {
            return;
        }
        if (resource_->getResourceStock() < 2) {
            return;
        }
        if (BattleCamera::getSingleton()->isCameraAnimation() && cameraWait_ != 0) {
            return;
        }

        start_ = 1;
        param::EffectParam* effect = effect_;
        if (effect->color != 0xff) {
            cmn::CommonEffectLocation::getSingleton()->start(effect->color, (max_ - 1) * effect->interval);
        }
        OS_Wait();
        for (int i = 0; i < max_; i++) {
            setupEffectGroup(i);
        }

        if (effect_->camera != 0 || effect_->camera2 != 0) {
            param::EffectParam::getCameraFile(effect_->camera, file);
            if (effect_->camera2 == 0) {
                BattleCamera::getSingleton()->setFilename(file, 0);
            }
            else {
                param::EffectParam::getCameraFile(effect_->camera2, name2);
                BattleCamera::getSingleton()->setFilename(file, name2);
                BattleCamera::getSingleton()->setWait(effect_->wait);
            }
            BattleCamera::getSingleton()->initCamera();
        }

        if (effect_->homing != 0 && max_ == 1) {
            BattleCamera::getSingleton()->homing_.step_ = 1;
        }
    }
    else {
        frame_++;
    }
}

THUMB void btl::BattleEffectUnit::execute()
{
    waitStart();
    if (start_ == 0) {
        return;
    }

    // The original ARM runtime (_s32_div_f in Runtime/src/ll_div.s) returns
    // remainder zero for divisor zero. Single-target effects use interval 0.
#ifdef DQIV_NATIVE
    if ((effect_->interval == 0 || frame_ % effect_->interval == 0) && process_ < max_) {
#else
    if (frame_ % effect_->interval == 0 && process_ < max_) {
#endif
        group_[process_]->start();
        if (result_[process_]) {
            SoundManager::playSe(effect_->sound, 0);
        }
        process_++;
    }

    int hit = hit_;
#ifdef DQIV_NATIVE
    if ((effect_->frame == 0 || (frame_ - hit * effect_->interval) % effect_->frame == 0) && hit < max_) {
#else
    if ((frame_ - hit * effect_->interval) % effect_->frame == 0 && hit < max_) {
#endif
        hit_++;
    }

    for (int i = pass_; i < process_; i++) {
        if (group_[i]->isEnable() && group_[i]->isEnd()) {
            cleanupEffectGroup(i);
        }
    }

    if (process_ == max_ && pass_ == max_ && frame_ >= effect_->frame + max_ * effect_->interval) {
        cleanup();
    }
    frame_++;
}

THUMB int btl::BattleEffectUnit::getHitFrame()
{
    return effect_->frame;
}

THUMB bool btl::BattleEffectUnit::isEnable()
{
    return effect_ != 0;
}

THUMB void btl::BattleEffectUnit::setWaitTime(int wait)
{
    if (wait < 0) {
        wait = 0;
    }
    frame_ = -wait;
}

THUMB void btl::BattleEffectUnit::setFaildTarget(int index, int flag)
{
    result_[index] = flag;
}
