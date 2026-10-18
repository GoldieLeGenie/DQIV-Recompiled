#pragma ipa file
#include "ov000/town/TownCharacter.hpp"
#include "ov000/town/TownExtraCollManager.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "ov000/town/TownCamera.hpp"
#include "ov000/Commands/TownCommand.hpp"
#include "main/data/FileLoader.hpp"
#include "main/script/ScriptSystem.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/status/BaseStatus.hpp"
#include "nitro/os.hpp"
#include "main/dss/PaletteEffect.hpp"

static const dss::Fix32 bigRockSpeed(0x133);
static const dss::Fix32 collLine(0x1733);
static const short rotSpeed = 0x265;
static const int rotFrame = 0x19a;

static const char* modelNameTable[19] = {
    "dozo", "msdr", "oiwa", "estrk", "pisaro", "kago", "koiwa", "dpoe01", "dpoe02", "dpoe03",
    "dpoe04", "dpoe05", "dpoe06", "dpoe07", "dpoe08", "dpoe09", "dpoe10", "msdr_help", "msdr_opening",
};

ARM TownModelDraw::TownModelDraw()
{
}

ARM TownModelDraw::~TownModelDraw()
{
}

ARM void TownModelDraw::setup(TOWN_CHARACTER& data)
{
    TownCharacterBase::setup(data);
    loadModel(data.charaIndex);
    model_.start(1);
    model_.setPosition(data_.position);
    dirIdx_ = 0;
    display_ = 1;
    modelIdx3d_.set(0, 0, 0);
    defaultIndex_ = data_.charaIndex;
    loopSe_ = 0;
    basePalletRate_.value = 2000;
}

ARM void TownModelDraw::cleanup()
{
    model_.cleanup(1);
    if (loopSe_ == 1) {
        SoundManager::stopSeWithIndex(0x46c, 0);
    }
}

ARM void TownModelDraw::loadModel(int index)
{
    int anim = 0;
    OS_Wait();
    char name[128];
    dss::sprintf_s(name, sizeof(name), "data/chr/%s.nsbmd", modelNameTable[index]);
    model_.setup(name);
    int i = 0;
    bool found;
    do {
        found = false;
        dss::sprintf_s(name, sizeof(name), "data/chr/%s_%d.nsbma", modelNameTable[index], i);
        if (dss::g_File.isExist(name)) {
            found = true;
            model_.unkfunc_020587d4(name, anim);
            anim++;
        }
        dss::sprintf_s(name, sizeof(name), "data/chr/%s_%d.nsbta", modelNameTable[index], i);
        if (dss::g_File.isExist(name)) {
            found = true;
            model_.unkfunc_020587d4(name, anim);
            anim++;
        }
        dss::sprintf_s(name, sizeof(name), "data/chr/%s_%d.nsbca", modelNameTable[index], i);
        if (dss::g_File.isExist(name)) {
            found = true;
            model_.unkfunc_020587d4(name, anim);
            anim++;
        }
        i++;
    } while (found);
}

ARM void TownModelDraw::execute()
{
    TownCharacterBase::execute();
}

ARM void TownModelDraw::draw()
{
    if (display_) {
        model_.draw();
    }
}

ARM void TownModelDraw::setDisplay(int flag)
{
    display_ = flag;
}

ARM int TownModelDraw::isDisplay()
{
    return display_;
}

ARM void TownModelDraw::changePose(int pose)
{
    model_.cleanup(1);
    data_.charaIndex = pose;
    loadModel(pose);
    model_.start(1);
    model_.setPosition(data_.position);
    dirIdx_ = 0;
    display_ = 1;
    modelIdx3d_.set(0, 0, 0);
}

ARM void TownModelDraw::restorePose()
{
    model_.cleanup(1);
    data_.charaIndex = defaultIndex_;
    loadModel(data_.charaIndex);
    model_.start(1);
    model_.setPosition(data_.position);
    dirIdx_ = 0;
    display_ = 1;
    modelIdx3d_.set(0, 0, 0);
}

ARM void TownModelDraw::requestReload()
{
    int anim = model_.m_animation_index;
    model_.cleanup(1);
    loadModel(data_.charaIndex);
    model_.start(1);
    model_.setPosition(data_.position);
    dirIdx_ = 0;
    modelIdx3d_.set(0, 0, 0);
    model_.startAnimation(anim, 1);
}

ARM void TownModelDraw::setAnimation(int flag)
{
    model_.pause(flag == 0);
}

ARM void TownModelDraw::setPosition(dss::Fix32Vector3& pos)
{
    dss::Fix32Vector3 nowPos = getPosition();
    TownExtraCollManager::getSingleton()->addMoveColl(data_.ctrlNo, type_, nowPos, pos);
    data_.position = pos;
    model_.setPosition(pos);
}

ARM void TownModelDraw::setRotation(dss::Vector3<short>& rot)
{
    model_.setRotationIdx(rot);
}

ARM dss::Fix32Vector3 TownModelDraw::getPosition()
{
    return data_.position;
}

ARM void TownModelDraw::setPaletteRate(dss::Fix32 r, dss::Fix32 g, dss::Fix32 b)
{
    setPaletteEffectTexture(model_.unk_b5c.texture_);
    dss::Fix32Vector3 rate(r, g, b);
    applyModelPaletteRgbRate(model_.unk_b5c.texture_, rate);
}

ARM void TownModelDraw::setPaletteRate(unsigned char r, unsigned char g, unsigned char b, dss::Fix32 rate)
{
    int rr = status::BaseStatus::getClampValue(0, r, 0x1f);
    int gg = status::BaseStatus::getClampValue(0, g, 0x1f);
    int bb = status::BaseStatus::getClampValue(0, b, 0x1f);
    rate.value = dss::clamp<int>(0, rate.value, basePalletRate_.value);
    blendModelPaletteToColor(model_.unk_b5c.texture_, rr, gg, bb, rate);
}

ARM void TownModelDraw::setMotion(int motion, int loop)
{
    setAnimation(1);
    model_.startAnimation(motion, loop);
}

ARM bool TownModelDraw::isMotion()
{
    return model_.m_play_flag == 0;
}

ARM void TownModelDraw::setDir(int dir)
{
    dirIdx_ = dir;
    dss::Vector3<short> rot;
    rot.set(0, dirIdx_, 0);
    setRotation(rot);
}

static int count;
static int stop;
static int prev_stop = 1;

ARM void TownModelDraw::setMoveBigRock()
{
    moveType_ = MOVE_TYPE_BIG_ROCK;
    dss::Fix32Vector3 nowPos = getPosition();
    dss::Fix32Vector3 target;
    target.vx = -0x6000;
    target.vy = nowPos.vy;
    target.vz = 0x4000;
    simpleMove_.setActionMove(nowPos, target);
    simpleMove_.setMoveSpeed(bigRockSpeed);
    dss::Vector3<short> add;
    add.set(rotSpeed, 0, 0);
    simpleMove_.setSimpleRot(modelIdx3d_, add, rotFrame);
    script_.num[0] = Z_MOVE;
    script_.num[1] = ROOT0;
    count = 0;
}

ARM void TownModelDraw::execMoveBigRock()
{
    stop = 0;
    dss::Fix32Vector3 playerPos;
    dss::Fix32Vector3 pos = getPosition();
    dss::Fix32Vector3 target = TownPlayerManager::getSingleton()->getPosition();
    dss::Fix32 min = ((target - pos)).lengthsq();
    int drawCount = TownPlayerManager::getSingleton()->partyDraw_.countReal_;
    simpleMove_.execMove(pos);
    for (int i = 0; i < drawCount; i++) {
        playerPos = TownPlayerManager::getSingleton()->party_.getMemberPosition(i);
        if (script_.num[0] == Z_MOVE) {
            if (pos.vz > playerPos.vz - collLine && pos.vz < playerPos.vz + collLine &&
                playerPos.vx <= pos.vx + collLine && playerPos.vx >= pos.vx - collLine) {
                stop = 1;
            }
        } else if (script_.num[1] == ROOT4) {
            if (pos.vx < playerPos.vx + collLine && playerPos.vz <= pos.vz + collLine &&
                playerPos.vz >= pos.vz - collLine) {
                stop = 1;
            }
        } else {
            if (pos.vx > playerPos.vx - collLine && playerPos.vz <= pos.vz + collLine &&
                playerPos.vz >= pos.vz - collLine) {
                stop = 1;
            }
        }
    }
    if (TownPlayerManager::getSingleton()->player_.actionType_ == 3 || TownPlayerManager::getSingleton()->player_.actionType_ == 0xc) {
        if (loopSe_ == 1) {
            SoundManager::stopSeWithIndex(0x46c, 0);
            loopSe_ = 0;
        }
        return;
    }
    if (stop == 1) {
        if (prev_stop == 0) {
            loopSe_ = 0;
            SoundManager::stopSeWithIndex(0x46c, 0);
        }
        prev_stop = stop;
        return;
    }
    if (prev_stop == 1) {
        loopSe_ = 1;
        SoundManager::playSe(0x46c, 0);
    }
    prev_stop = stop;
    switch (script_.num[1]) {
    case ROOT_END1:
    case ROOT_END2:
        if (++count == 80) {
            loopSe_ = 0;
            SoundManager::stopSeWithIndex(0x46c, 0);
        }
        break;
    }
    simpleMove_.execRot(modelIdx3d_);
    if (simpleMove_.moveUpdate() == 1) {
        setRoot(script_.num[1], target, pos);
    }
    simpleMove_.rotUpdate();
    setPosition(pos);
    setRotation(modelIdx3d_);
}

ARM void TownModelDraw::setPalletRate(dss::Fix32 rate)
{
    basePalletRate_ = rate;
}

ARM void TownModelDraw::setRoot(int root, dss::Fix32Vector3& target, dss::Fix32Vector3& pos)
{
    static const dss::Fix32 length(0x8000);
    static const dss::Fix32 unkLength(0x1b33);
    dss::Fix32Vector3 next = pos;
    dss::Vector3<short> add;
    dss::Fix32 speed = bigRockSpeed;
    switch (root) {
    case ROOT0:
        if (target.vx > pos.vx + collLine) {
            script_.num[1] = ROOT1;
            next.vx += length;
            add.set(0, 0, -rotSpeed);
            script_.num[0] = X_MOVE;
        } else {
            script_.num[1] = ROOT2;
            script_.num[0] = Z_MOVE;
            add.set(rotSpeed, 0, 0);
            next.vz += length;
        }
        break;
    case ROOT1:
        next.vz += length;
        add.set(rotSpeed, 0, 0);
        script_.num[1] = ROOT3;
        script_.num[0] = Z_MOVE;
        break;
    case ROOT3:
        if (target.vx < pos.vx - collLine) {
            script_.num[1] = ROOT4;
            next.vx -= length;
            add.set(0, 0, rotSpeed);
            script_.num[0] = X_MOVE;
        } else {
            script_.num[1] = ROOT_END2;
            script_.num[0] = Z_MOVE;
            next.vz += length * 3 / 4;
            add.set(rotSpeed, 0, 0);
        }
        break;
    case ROOT2:
        if (target.vx > pos.vx + collLine) {
            script_.num[1] = ROOT5;
            script_.num[0] = X_MOVE;
            next.vx += length;
            add.set(0, 0, -rotSpeed);
        } else {
            script_.num[1] = ROOT_END1;
            next.vz += length * 3 / 4;
            add.set(rotSpeed, 0, 0);
            script_.num[0] = Z_MOVE;
        }
        break;
    case ROOT4:
        script_.num[1] = ROOT_END1;
        next.vz += length * 7 / 10;
        add.set(rotSpeed, 0, 0);
        script_.num[0] = Z_MOVE;
        break;
    case ROOT5:
        script_.num[1] = ROOT_END2;
        next.vz += length * 7 / 10;
        add.set(rotSpeed, 0, 0);
        script_.num[0] = Z_MOVE;
        break;
    case ROOT_END1:
        add.set(rotSpeed / 3, 0, 0);
        script_.num[0] = Z_MOVE;
        script_.num[1] = FALL1;
        next.vx = -0x6000;
        next.vy = -0xb33;
        next.vz = 0x13000;
        speed /= 3;
        TownStageManager::getSingleton()->setCollisionObject(0x1f8);
        cmn::PlayerManager::setLock(1);
        TownCamera::getSingleton()->setMoveTo(next, 80, true);
        break;
    case ROOT_END2:
        TownStageManager::getSingleton()->setCollisionObject(0x1f9);
        add.set(rotSpeed / 3, 0, 0);
        script_.num[0] = Z_MOVE;
        script_.num[1] = FALL2;
        next.vx = 0x2000;
        next.vy = -0xb33;
        next.vz = 0x13000;
        speed /= 3;
        cmn::PlayerManager::setLock(1);
        TownPlayerManager::getSingleton()->partyDraw_.setExcute(0);
        TownCamera::getSingleton()->setMoveTo(next, 80, true);
        break;
    case FALL1:
        loopSe_ = 0;
        SoundManager::stopSeWithIndex(0x46c, 0);
        TownCamera::getSingleton()->setShake(3, 8);
        SoundManager::playSe(0x46d, 0);
        script_.num[1] = END_FALL;
        count = 0;
        break;
    case FALL2:
        loopSe_ = 0;
        SoundManager::stopSeWithIndex(0x46c, 0);
        TownCamera::getSingleton()->setShake(3, 8);
        script_.num[1] = END_FALL;
        SoundManager::playSe(0x46d, 0);
        count = 0;
        break;
    case END_FALL:
        if (count == 45) {
            TownCamera::getSingleton()->setMoveTargetPlayer(80);
        }
        if (count > 45 && TownCamera::getSingleton()->cameraMove_.isEnd()) {
            TownCamera::getSingleton()->setCameraLock(false);
            cmn::PlayerManager::setLock(0);
            TownPlayerManager::getSingleton()->partyDraw_.setExcute(1);
            moveType_ = MOVE_TYPE_NONE;
        }
        count++;
        break;
    }
    simpleMove_.setActionMove(pos, next);
    simpleMove_.setMoveSpeed(speed);
    simpleMove_.setSimpleRot(modelIdx3d_, add, rotFrame);
}

ARM int TownModelDraw::getDir()
{
    return dirIdx_;
}

