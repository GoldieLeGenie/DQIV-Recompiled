#pragma ipa file
#include "ov000/town/TownCharacter.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "ov000/town/TownActionCalculate.hpp"
#include "ov000/town/TownCharacterManager.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownPlayerAction.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "ov000/town/TownActionWalk.hpp"
#include "ov000/town/TownFurniture.hpp"
#include "ov000/Commands/TownCommand.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/cmn/CommonEffectLocation.hpp"
#include "main/dss/Camera.hpp"
#include "main/dss/Random.hpp"
#include "main/fld/FldStage.hpp"
#include "main/status/BaseStatus.hpp"
#include "ov000/town/riseup/TownRiseup.hpp"

static const dss::Fix32 fixLen(0x333);

int TownCharacterBase::areaCheck_;
int TownCharacterBase::allEventLock_;
int TownCharacterBase::monsterTalk_;

ARM TownCharacterBase::TownCharacterBase() : collFlag_(1)
{
}

ARM TownCharacterBase::~TownCharacterBase()
{
}

ARM void TownCharacterBase::setup(TOWN_CHARACTER& chara)
{
    data_.enable = 1;
    data_.index = chara.index;
    data_.charaIndex = chara.charaIndex;
    data_.dir = chara.dir;
    data_.position = chara.position;
    data_.flag.flag_ = 0;
    simpleMove_.setup();
    moveType_ = MOVE_TYPE_NONE;
    stageColl_ = 0;
    animCounter_ = 0;
    rgbFrame_ = -1;
    mapNo_ = -1;
    voice_ = cmn::TalkSoundManager::MESSAGESOUND_MIDDLE;
}

ARM void TownCharacterBase::setDir(int dir)
{
    data_.dir = dir;
}

ARM int TownCharacterBase::getDir()
{
    return 0;
}

ARM void TownCharacterBase::setRotation(dss::Vector3<short>& rot)
{
}

ARM void TownCharacterBase::setShadow(int flag)
{
}

ARM void TownCharacterBase::setAnimation(int flag)
{
}

ARM void TownCharacterBase::setNearCharacter(int flag)
{
}

ARM void TownCharacterBase::setDisplay(int flag)
{
}

ARM int TownCharacterBase::isDisplay()
{
    return 0;
}

ARM void TownCharacterBase::setSleepCharacter(int flag)
{
}

ARM void TownCharacterBase::setWriggleCharacter(int flag)
{
}

ARM void TownCharacterBase::setAlpha(unsigned char alpha)
{
}

ARM void TownCharacterBase::changePose(int set)
{
}

ARM void TownCharacterBase::restorePose()
{
}

ARM void TownCharacterBase::requestReload()
{
}

ARM void TownCharacterBase::setScriptData(TOWN_SCRIPT_DATA& script)
{
    script_ = script;
    script_.isEnd = 0;
}

ARM int TownCharacterBase::isScriptEnd()
{
    return script_.isEnd;
}

ARM void TownCharacterBase::execWait()
{
    if (++script_.counter >= script_.frame) {
        script_.isEnd = 1;
    }
}

ARM void TownCharacterBase::execMove()
{
    if (getTalked() == 1) {
        return;
    }
    dss::Fix32Vector3 oldPos = data_.position;
    dss::Fix32Vector3 newPos = oldPos;
    simpleMove_.execMove(newPos);
    if (TownPlayerManager::getSingleton()->charaColl_ == 1 && collFlag_ == 1) {
        if ((areaCheck_ == 1 && (bool)(data_.flag.flag_ & 1) == true) || areaCheck_ == 0) {
            dss::Fix32Vector3 dirVec = newPos - oldPos;
            dss::Fix32Vector3 playerPos = TownPlayerManager::getSingleton()->getPosition();
            playerPos.vy = newPos.vy;
            dss::Fix32Vector3 vec = playerPos - newPos;
            if (vec.lengthsq().value < ((TownPlayerAction::collR * TownPlayerAction::collR) * 4).value + 0x12c) {
                dirVec.normalize();
                vec.normalize();
                dss::Fix32 dot = vec * dirVec;
                if (dot.value > 0x165) {
                    return;
                }
            }
        }
    }
    int stageColl = stageColl_;
    if ((stageColl & 1) || (stageColl & 2)) {
        TownStageManager::getSingleton()->characoterColl(oldPos, newPos, TownPlayerAction::collR, &newPos, stageColl);
    }
    if ((bool)(data_.flag.flag_ & 8) == false) {
        setDir(moveIdx_);
    }
    setPosition(newPos);
    if (stageColl_ & 4) {
        if (TownCharacterManager::getSingleton()->charaToCharaColl(this) == 1) {
            setPosition(oldPos);
            return;
        }
    }
    if (simpleMove_.moveUpdate() != 1) {
        return;
    }
    if (moveType_ == MOVE_TYPE_TO_PARTY) {
        setNextMoveToParty();
        setSimpleMove();
        return;
    }
    moveType_ = MOVE_TYPE_NONE;
    script_.isEnd = 1;
}

ARM void TownCharacterBase::setPosition(dss::Fix32Vector3& pos)
{
    data_.position = pos;
}

ARM void TownCharacterBase::execRiseup()
{
    if (script_.counter == 0) {
        script_.num[1] = TownRiseupManager::getSingleton()->setup(script_.num[0], data_.position);
    }
    if (TownRiseupManager::getSingleton()->isFinish(script_.num[1])) {
        script_.isEnd = 1;
    }
    script_.counter++;
}

ARM void TownCharacterBase::execVanish()
{
    int cycle = script_.num[0];
    if (script_.counter % cycle < cycle / 2) {
        setDisplay(0);
    } else {
        setDisplay(1);
    }
    if (++script_.counter >= script_.frame) {
        setDisplay(1);
        script_.isEnd = 1;
    }
}

ARM void TownCharacterBase::execTremble()
{
    int n;
    if (script_.num[2] == 0) {
        script_.num[2] = 16;
    }
    switch (script_.num[1]) {
    case 0:
        n = 1;
        break;
    case 1:
        n = 2;
        break;
    case 2:
        n = 4;
        break;
    case 3:
        n = 8;
        break;
    }
    if (script_.counter == 0) {
        script_.node[0] = data_.position;
    }
    dss::Fix32Vector3 pos = script_.node[0];
    int val;
    int direction = script_.num[0];
    switch (direction) {
    case 0:
        val = pos.vy.value;
        break;
    case 1:
        val = pos.vx.value;
        break;
    case 2:
        val = pos.vz.value;
        break;
    }
    int roop = script_.num[2];
    int current = script_.counter % roop;
    if (current < roop / 2) {
        if (current < roop / 4) {
            val += 0x100 / n;
        } else {
            val -= 0x100 / n;
        }
    } else {
        if (current < roop * 3 / 4) {
            val -= 0x100 / n;
        } else {
            val += 0x100 / n;
        }
    }
    switch (direction) {
    case 0:
        pos.vy.value = val;
        break;
    case 1:
        pos.vx.value = val;
        break;
    case 2:
        pos.vz.value = val;
        break;
    }
    setPosition(pos);
    if (++script_.counter < script_.frame) {
        return;
    }
    setPosition(script_.node[0]);
    script_.isEnd = 1;
}

ARM void TownCharacterBase::setCollFlag(int flag)
{
    collFlag_ = flag;
}

ARM int TownCharacterBase::getCollFlag()
{
    return collFlag_;
}

ARM void TownCharacterBase::execute()
{
    if ((bool)(data_.flag.flag_ & 0x400) == true) {
        return;
    }
    if ((bool)(data_.flag.flag_ & 0x80) == true) {
        return;
    }
    if ((bool)(data_.flag.flag_ & 0x20) == true && (bool)(data_.flag.flag_ & 0x40) == true) {
        data_.flag.flag_ &= ~0x40;
        setDir(swingIdx_);
    }
    switch (moveType_) {
    case MOVE_TYPE_AREA:
        execAreaMove();
        break;
    case MOVE_TYPE_ROOT:
        execRootMove();
        break;
    case MOVE_TYPE_PURSUE:
        execPursueMove();
        break;
    case MOVE_TYPE_TO_PARTY:
    case MOVE_TYPE_SIMPLE_MOVE:
        execMove();
        break;
    case MOVE_TYPE_JUMP:
        jumpMove();
        break;
    case MOVE_TYPE_PASSIVE:
        execMovePassive();
        break;
    case MOVE_TYPE_RANDOM:
        execMoveRandom();
        break;
    case MOVE_TYPE_REVESE:
        execMoveReverse();
        break;
    case MOVE_TYPE_BIG_ROCK:
        execMoveBigRock();
        break;
    case MOVE_TYPE_WAIT:
        execMoveWait();
        break;
    }
    if ((bool)(data_.flag.flag_ & 0x10) == true) {
        dss::Vector3<short> angle;
        angle.vy = getDir();
        simpleMove_.execRot(angle);
        setDir(angle.vy);
        if (simpleMove_.rotUpdate() == 1) {
            data_.flag.flag_ &= ~0x10;
        }
    }
    if (changeAlphaType_ != CHANGE_NONE) {
        changeAlpha();
    }
    if (rgbFrame_ == -1) {
        return;
    }
    changeRGB();
}

ARM void TownCharacterBase::changeRGB()
{
    if (rgbFrame_ >= 0) {
        switch (rgbChangeType_) {
        case RGB_CHANGE1:
            setRGB += addRGB;
            setPaletteRate(setRGB.vx, setRGB.vy, setRGB.vz);
            break;
        case RGB_CHANGE2: {
            unsigned char r = setRGB.vx.value / 4096;
            unsigned char g = setRGB.vy.value / 4096;
            unsigned char b = setRGB.vz.value / 4096;
            dss::Fix32 rate;
            rate.value = (rgbFrameMax_ - rgbFrame_) << 12;
            rate /= rgbFrameMax_;
            setPaletteRate(r, g, b, rate);
            break;
        }
        }
    }
    rgbFrame_--;
}

ARM void TownCharacterBase::setPaletteRate(dss::Fix32 r, dss::Fix32 g, dss::Fix32 b)
{
}

ARM void TownCharacterBase::changeAlpha()
{
    static int fadeData[10] = {10, 9, 9, 8, 8, 7, 6, 4, 2, 0};
    static int blinkFrame[6] = {7, 6, 6, 5, 4, 3};
    switch (changeAlphaType_) {
    case CHANGE_FADE_IN1:
    case CHANGE_FADE_OUT1: {
        int index = dss::clamp<int>(0, alphaCounter_ * 10 / alphaFrame_, 9);
        unsigned char fadeIn = changeAlphaType_ == CHANGE_FADE_IN1 ? 1 : 0;
        unsigned char alpha;
        if (fadeIn) {
            alpha = fadeData[9 - index] * 31 / 10;
        } else {
            alpha = fadeData[index] * 31 / 10;
        }
        remoteAlpha_ = alpha;
        remoteAlpha_ = status::BaseStatus::getClampValue(0, remoteAlpha_, 0x1f);
        setAlpha(remoteAlpha_);
        break;
    }
    case CHANGE_FADE_IN2:
    case CHANGE_FADE_OUT2: {
        int prevIndex = (alphaCounter_ - 1) * 6 / alphaFrame_;
        int index = dss::clamp<int>(0, alphaCounter_ * 6 / alphaFrame_, 5);
        unsigned char setA = changeAlphaType_ == CHANGE_FADE_IN2 ? 0x1f : 0;
        unsigned char resetA = changeAlphaType_ == CHANGE_FADE_IN2 ? 0 : 0x1f;
        if (blinkCounter_ == blinkFrame[index] || prevIndex != index) {
            blinkCounter_ = 0;
            setAlpha(setA);
        }
        if (blinkCounter_ == 0) {
            setAlpha(setA);
        } else if (blinkCounter_ == 3) {
            setAlpha(resetA);
        }
        blinkCounter_++;
        break;
    }
    case CHANGE_LINEARLY:
        remoteAlpha_ += alphaAdd_;
        remoteAlpha_ = status::BaseStatus::getClampValue(0, remoteAlpha_, 0x1f);
        setAlpha(remoteAlpha_);
        if (remoteAlpha_ == 0 || remoteAlpha_ == 0x1f) {
            script_.isEnd = 1;
            alphaAdd_ = 0;
        }
        break;
    }
    if (alphaCounter_ >= alphaFrame_) {
        switch (changeAlphaType_) {
        case CHANGE_FADE_OUT1:
        case CHANGE_FADE_OUT2:
            setDisplay(0);
            break;
        }
        changeAlphaType_ = CHANGE_NONE;
    }
    alphaCounter_++;
}

ARM void TownCharacterBase::execPursueMove()
{
    if (moveData_.counter >= 60) {
        moveData_.counter = 0;
    }
    if (moveData_.counter >= 30) {
        moveData_.counter++;
        return;
    }
    static dss::Fix32 length2(0xb33);
    dss::Fix32Vector3 oldPos = data_.position;
    int drawCharaCount = TownPlayerManager::getSingleton()->partyDraw_.countReal_;
    dss::Fix32Vector3 vec = (TownPlayerManager::getSingleton()->party_.getMemberPosition(drawCharaCount) - oldPos);
    if (vec.lengthsq() < fixLen * fixLen) {
        return;
    }
    dss::Fix32Vector3 vec2 = (TownPlayerManager::getSingleton()->getPosition() - oldPos);
    if (collFlag_ == 1) {
        if (vec2.lengthsq() < length2 * length2) {
            return;
        }
    }
    short idx = getDir();
    if (moveData_.counter % 60 < 30 && (unkfunc_02081254() & 1)) {
        vec.normalize();
        dss::Fix32Vector3 newPos = oldPos + vec * moveData_.speed;
        TownStageManager::getSingleton()->characoterColl(oldPos, newPos, TownPlayerAction::collR, &newPos, 3);
        setPosition(newPos);
        vec = newPos - oldPos;
        TownActionCalculate::getIdxByVec(idx, vec);
        setDir(idx);
    }
    moveData_.counter++;
}

ARM void TownCharacterBase::checkMoveColl(dss::Fix32Vector3& pos1, dss::Fix32Vector3& pos2, dss::Fix32& ret, dss::Fix32& min)
{
    dss::Fix32 temp = ret;
    if (TownStageManager::getSingleton()->stage_.collCrossCheckPoly(pos1, pos2, &temp, 1) == -1) {
        return;
    }
    if (!(temp < min)) {
        return;
    }
    min = temp;
    ret = temp;
    if (ret <= TownPlayerAction::townCharaPreR) {
        moveData_.vector[3].vz.value = 0;
        moveData_.vector[3].vy.value = 0;
        return;
    }
    moveData_.vector[3].vy = (ret - TownPlayerAction::townCharaPreR) / moveData_.speed;
    moveData_.vector[3].vz.value = moveData_.vector[3].vy.value / 4096;
}

ARM void TownCharacterBase::execAreaMove()
{
    if (moveData_.counter == 80) {
        moveData_.counter = 0;
    }
    static const dss::Fix32 Len(0x2800);
    if (moveData_.counter == 0) {
        unsigned char param = dssrand::rand(4);
        int dir = TownActionCalculate::getIdxByParam(param);
        setDir(dir);
        moveData_.vector[3].vx.value = dir;
        moveData_.vector[2] = TownActionCalculate::getParamVec(param);
        data_.flag.flag_ &= ~0x200;
        if (stageColl_ & 1) {
            data_.flag.flag_ |= 0x200;
            moveData_.vector[3].vz.value = 35;
            dss::Fix32Vector3 add;
            add = TownActionCalculate::getParamVec(param - 1 < 0 ? 3 : param - 1);
            dss::Fix32Vector3 pos1 = data_.position + add * TownPlayerAction::collR;
            dss::Fix32Vector3 pos2 = pos1 + moveData_.vector[2] * (moveData_.speed * 40 + TownPlayerAction::townCharaPreR);
            pos1.vy += TownPlayerAction::collR;
            pos2.vy += TownPlayerAction::collR;
            dss::Fix32 retLen = Len;
            dss::Fix32 tempLen = Len;
            checkMoveColl(pos1, pos2, retLen, tempLen);
            add = TownActionCalculate::getParamVec(param + 1 > 3 ? 0 : param + 1);
            pos1 = data_.position + add * TownPlayerAction::collR;
            pos2 = pos1 + moveData_.vector[2] * (moveData_.speed * 40 + TownPlayerAction::townCharaPreR);
            pos1.vy += TownPlayerAction::collR;
            pos2.vy += pos1.vy;
            checkMoveColl(pos1, pos2, retLen, tempLen);
            pos1 = data_.position;
            pos2 = pos1 + moveData_.vector[2] * (moveData_.speed * 40 + TownPlayerAction::townCharaPreR);
            pos1.vy += TownPlayerAction::collR;
            pos2.vy = pos1.vy;
            checkMoveColl(pos1, pos2, retLen, tempLen);
        }
        moveData_.vector[2] *= moveData_.speed;
    }
    if (moveData_.counter < 35) {
        dss::Fix32Vector3 oldPos = data_.position;
        dss::Fix32Vector3 newPos = oldPos + moveData_.vector[2];
        if (moveData_.vector[0].vx > newPos.vx || moveData_.vector[0].vz > newPos.vz ||
            moveData_.vector[1].vx < newPos.vx || moveData_.vector[1].vz < newPos.vz) {
            moveData_.counter++;
            return;
        }
        if ((bool)(data_.flag.flag_ & 0x200) && moveData_.counter >= moveData_.vector[3].vz.value) {
            moveData_.counter++;
            return;
        }
        if ((areaCheck_ == 1 && (bool)(data_.flag.flag_ & 1) == true) || areaCheck_ == 0) {
            dss::Fix32Vector3 vec1 = (TownPlayerManager::getSingleton()->getPosition() - newPos);
            if (vec1.lengthsq().value < ((TownPlayerAction::townCharaR * TownPlayerAction::townCharaR) * 4).value + 0x190) {
                dss::Fix32Vector3 vec2 = moveData_.vector[2];
                dss::Fix32 dot = vec1 * vec2;
                if (dot > dss::Fix32(0L)) {
                    moveData_.counter++;
                    return;
                }
            }
        }
        if (stageColl_ & 2) {
            TownStageManager::getSingleton()->characoterColl(oldPos, newPos, TownPlayerAction::collR, &newPos, 2);
        }
        setPosition(newPos);
        if (stageColl_ & 4) {
            if (TownCharacterManager::getSingleton()->charaToCharaColl(this) == 1) {
                setPosition(oldPos);
                moveData_.counter++;
                return;
            }
        }
        if (!(bool)(data_.flag.flag_ & 8)) {
            setDir(moveData_.vector[3].vx.value);
        }
    }
    moveData_.counter++;
}

ARM void TownCharacterBase::execRootMove()
{
}

ARM void TownCharacterBase::setMoveToParty()
{
    moveType_ = MOVE_TYPE_TO_PARTY;
    dss::Fix32Vector3 target1;
    switch (script_.num[0]) {
    case 0:
        moveType_ = MOVE_TYPE_NONE;
        return;
    case 1:
        script_.node[2] = script_.node[1];
        script_.node[1].vz = script_.node[0].vz;
        break;
    case 2:
        script_.node[2] = script_.node[1];
        script_.node[1].vx = script_.node[0].vx;
        break;
    default:
        moveType_ = MOVE_TYPE_NONE;
        return;
    }
    dss::Fix32 length0 = ((script_.node[1] - script_.node[0])).length();
    dss::Fix32 length1 = ((script_.node[2] - script_.node[1])).length();
    int tempFrame = script_.frame;
    script_.num[1] = tempFrame * (length1 / (length0 + length1)).value / 4096;
    script_.frame = tempFrame * (length0 / (length0 + length1)).value / 4096;
}

ARM void TownCharacterBase::setNextMoveToParty()
{
    script_.node[1] = script_.node[2];
    script_.node[0] = data_.position;
    script_.frame = script_.num[1];
    script_.counter = 0;
    moveType_ = MOVE_TYPE_NONE;
}

ARM void TownCharacterBase::setSimpleMove()
{
    if (moveType_ != MOVE_TYPE_TO_PARTY) {
        moveType_ = MOVE_TYPE_SIMPLE_MOVE;
    }
    simpleMove_.setActionMove(script_.node[0], script_.node[1]);
    simpleMove_.setMoveFrame(script_.frame);
    setPersonalEventLock(0);
    short idx = getDir();
    dss::Fix32Vector3 dirVec = script_.node[1] - script_.node[0];
    TownActionCalculate::getIdxByVec(idx, dirVec);
    moveIdx_ = idx;
}

ARM void TownCharacterBase::setSimpleRot(short idx, int frame, int mode)
{
    data_.flag.flag_ |= 0x10;
    dss::Vector3<short> start;
    dss::Vector3<short> end;
    start.set(0, 0, 0);
    end.set(0, 0, 0);
    start.vy = getDir();
    end.vy = idx;
    simpleMove_.setActionRot(start, end);
    simpleMove_.setRotFrame(frame, mode);
    setPersonalEventLock(0);
}

ARM void TownCharacterBase::setJumpMove(dss::Fix32Vector3 endPos, int frame)
{
    moveType_ = MOVE_TYPE_JUMP;
    dss::Fix32Vector3 pos = data_.position;
    simpleMove_.setJumpMove(pos, endPos, frame);
    script_.isEnd = 0;
    setPersonalEventLock(0);
}

ARM void TownCharacterBase::jumpMove()
{
    dss::Fix32Vector3 pos = data_.position;
    simpleMove_.execMove(pos);
    if (simpleMove_.moveUpdate() == 1) {
        moveType_ = MOVE_TYPE_NONE;
        script_.isEnd = 1;
    }
    setPosition(pos);
}

ARM void TownCharacterBase::setLockRot(int lockRot)
{
    if (lockRot == 1) {
        data_.flag.flag_ |= 8;
    } else {
        data_.flag.flag_ &= ~8;
    }
}

ARM void TownCharacterBase::setPosing(int index)
{
    if (index == data_.charaIndex) {
        restorePose();
    } else {
        changePose(index);
    }
}

ARM void TownCharacterBase::setMotion(int index, int flag)
{
}

ARM bool TownCharacterBase::isMotion()
{
    return false;
}

ARM void TownCharacterBase::setMovePassive()
{
    static const dss::Fix32 passiveSpeed(0x19a);
    moveType_ = MOVE_TYPE_PASSIVE;
    moveData_.frame = 0;
    moveData_.counter = 0;
    moveData_.speed = passiveSpeed;
    setPersonalEventLock(0);
}

ARM void TownCharacterBase::execMovePassive()
{
    if (!((areaCheck_ == 1 && (bool)(data_.flag.flag_ & 1) == true) || areaCheck_ == 0)) {
        return;
    }
    dss::Fix32Vector3 nowPos = data_.position;
    dss::Fix32Vector3 playerPos = g_cmnPartyInfo.position_;
    dss::Fix32Vector3 vec = nowPos - playerPos;
    vec.vy = 0L;
    if (unkfunc_02031e84(vec.vy.value) > TownPlayerAction::townCharaR.value) {
        return;
    }
    if (!(vec.lengthsq() <= (TownPlayerAction::townCharaR * TownPlayerAction::townCharaR) * 4)) {
        return;
    }
    moveData_.counter++;
    if (moveData_.counter % 40 > 20 || moveData_.counter % 2 != 0) {
        return;
    }
    if (TownActionWalk::getSingleton()->moveFlag_ == 0) {
        return;
    }
    dss::Fix32Vector3 prevPlayerPos = g_cmnPartyInfo.prev_position_;
    short idx = getDir();
    TownActionCalculate::getIdxByVec(idx, vec);
    unsigned char dir = TownActionCalculate::getParamDir4ByIdx(idx);
    vec = TownActionCalculate::getParamVec(dir);
    if (type_ == TOWN_CHARACTER_NORMAL) {
        setDir(idx);
    }
    dss::Fix32Vector3 nextPos = nowPos + vec * moveData_.speed;
    dss::Fix32Vector3 retPos;
    TownStageManager::getSingleton()->characoterColl(nowPos, nextPos, TownPlayerAction::collR, &retPos, 3);
    if (retPos.vx != nextPos.vx || retPos.vz != nextPos.vz) {
    }
    setPosition(retPos);
    if (TownCharacterManager::getSingleton()->charaToCharaColl(this) != 1) {
        return;
    }
    setPosition(nowPos);
}

ARM void TownCharacterBase::setMoveReverse()
{
    moveType_ = MOVE_TYPE_REVESE;
    moveData_.speed = TownPlayerAction::walkSpeed / 4;
    setPersonalEventLock(0);
}

ARM void TownCharacterBase::execMoveReverse()
{
    dss::Fix32Vector3 playerPos = g_cmnPartyInfo.position_;
    dss::Fix32Vector3 prevPlayerPos = g_cmnPartyInfo.prev_position_;
    if ((playerPos == prevPlayerPos)) {
        return;
    }
    dss::Fix32Vector3 vec = playerPos - prevPlayerPos;
    vec.normalize();
    switch (TownActionCalculate::getParamDir4ByIdx(getDir())) {
    case 0:
    case 2:
        vec.vz *= -1;
        break;
    case 1:
    case 3:
        vec.vx *= -1;
        break;
    }
    dss::Fix32Vector3 nowPos = data_.position;
    dss::Fix32Vector3 nextPos = nowPos + vec * moveData_.speed;
    dss::Fix32Vector3 retPos;
    moveData_.counter++;
    if (checkPlayerColl(nextPos) == 1) {
        return;
    }
    TownStageManager::getSingleton()->characoterColl(nowPos, nextPos, TownPlayerAction::collR, &retPos, 3);
    if (TownCharacterManager::getSingleton()->charaToCharaColl(this) == 1 || retPos.vx != nextPos.vx || retPos.vz != nextPos.vz) {
        setPosition(nowPos);
        return;
    }
    setPosition(retPos);
}

ARM void TownCharacterBase::setMoveRandom()
{
    setPersonalEventLock(0);
    moveType_ = MOVE_TYPE_RANDOM;
    moveData_.frame = 0;
    moveData_.counter = 0;
    moveData_.speed = TownPlayerAction::walkSpeed / 4;
}

ARM void TownCharacterBase::execMoveRandom()
{
    switch (moveData_.counter) {
    case -1: {
        dss::Fix32Vector3 playerPos = g_cmnPartyInfo.position_;
        dss::Fix32Vector3 prevPlayerPos = g_cmnPartyInfo.prev_position_;
        if (playerPos != prevPlayerPos) {
            moveData_.counter++;
        }
        return;
    }
    case 30:
        moveData_.counter = -1;
        return;
    }
    if (moveData_.counter == 0) {
        short dir = dssrand::rand(4) << 14;
        if ((bool)(data_.flag.flag_ & 8) == false) {
            setDir(dir);
        }
        moveData_.vector[3].vx.value = dir;
        TownActionCalculate::getDirByIdx(dir, moveData_.vector[2]);
        moveData_.vector[2].normalize();
        moveData_.vector[2] *= moveData_.speed;
    }
    dss::Fix32Vector3 nowPos = data_.position;
    dss::Fix32Vector3 nextPos = nowPos + moveData_.vector[2];
    dss::Fix32Vector3 retPos;
    moveData_.counter++;
    if (checkPlayerColl(nextPos) == 1) {
        return;
    }
    TownStageManager::getSingleton()->characoterColl(nowPos, nextPos, TownPlayerAction::collR, &retPos, 3);
    if (TownCharacterManager::getSingleton()->charaToCharaColl(this) == 1 || retPos.vx != nextPos.vx || retPos.vz != nextPos.vz) {
        setPosition(nowPos);
        return;
    }
    setPosition(retPos);
}

ARM bool TownCharacterBase::checkPlayerColl(dss::Fix32Vector3& newPos)
{
    dss::Fix32Vector3 vec1 = (TownPlayerManager::getSingleton()->getPosition() - newPos);
    if (vec1.lengthsq() < (TownPlayerAction::townCharaR * TownPlayerAction::townCharaR) * 4) {
        dss::Fix32Vector3 vec2 = moveData_.vector[2];
        vec2.normalize();
        vec1.normalize();
        dss::Fix32 dot = vec1 * vec2;
        if (dot > dss::Fix32(0L)) {
            return true;
        }
    }
    return false;
}

ARM void TownCharacterBase::setSwingRoundIdx()
{
    swingIdx_ = getDir();
    data_.flag.flag_ |= 0x40;
}

ARM void TownCharacterBase::setFadeType(int type, int frame)
{
    alphaCounter_ = 0;
    alphaFrame_ = frame;
    changeAlphaType_ = type;
    blinkCounter_ = 0;
    setPersonalEventLock(0);
    switch (changeAlphaType_) {
    case CHANGE_FADE_IN1:
    case CHANGE_FADE_IN2:
        setDisplay(1);
        setAlpha(0);
        break;
    }
}

ARM bool TownCharacterBase::isEndFade()
{
    return changeAlphaType_ == CHANGE_NONE;
}

ARM void TownCharacterBase::setChangePaletteRate(dss::Fix32Vector3& rgb, int rgbFrame)
{
    rgbChangeType_ = RGB_CHANGE1;
    addRGB = (rgb - setRGB) / rgbFrame;
    rgbFrame_ = rgbFrame;
    setPersonalEventLock(0);
}

ARM void TownCharacterBase::setChangePaletteRate(unsigned char r, unsigned char g, unsigned char b, int rgbFrame)
{
    rgbChangeType_ = RGB_CHANGE2;
    setRGB.vx.value = r << 12;
    setRGB.vy.value = g << 12;
    setRGB.vz.value = b << 12;
    rgbFrame_ = rgbFrame;
    setPersonalEventLock(0);
    rgbFrameMax_ = rgbFrame_;
}

ARM bool TownCharacterBase::isEndPalletRate()
{
    return rgbFrame_ == -1;
}

ARM void TownCharacterBase::setPersonalEventLock(int flag)
{
    if (flag == 1) {
        data_.flag.flag_ |= 0x80;
    } else {
        data_.flag.flag_ &= ~0x80;
    }
}

ARM void TownCharacterBase::setMonsterSpeak(int flag)
{
    if (flag == 1) {
        data_.flag.flag_ |= 0x100;
    } else {
        data_.flag.flag_ &= ~0x100;
    }
}

ARM bool TownCharacterBase::checkMonsterSpeak()
{
    return (bool)(data_.flag.flag_ & 0x100);
}

ARM void TownCharacterBase::setMoveBigRock()
{
}

ARM void TownCharacterBase::execMoveBigRock()
{
}

ARM void TownCharacterBase::setMapUid(int uid)
{
}

ARM void TownCharacterBase::resetTalk()
{
    data_.flag.flag_ &= ~2;
    data_.flag.flag_ &= ~1;
    data_.flag.flag_ &= ~4;
}

ARM bool TownCharacterBase::getSpeak()
{
    return (bool)(data_.flag.flag_ & 1);
}

ARM void TownCharacterBase::setSpeak(int flag)
{
    if (flag == 1) {
        data_.flag.flag_ |= 1;
    } else {
        data_.flag.flag_ &= ~1;
    }
}

ARM bool TownCharacterBase::getTalked()
{
    return (bool)(data_.flag.flag_ & 2);
}

ARM void TownCharacterBase::setTalked(int flag)
{
    if (flag == 1) {
        data_.flag.flag_ |= 2;
    } else {
        data_.flag.flag_ &= ~2;
    }
}

ARM void TownCharacterBase::setCounterTalk(int flag)
{
    if (flag == 1) {
        data_.flag.flag_ |= 4;
    } else {
        data_.flag.flag_ &= ~4;
    }
}

ARM bool TownCharacterBase::getCounterTalk()
{
    return (bool)(data_.flag.flag_ & 4);
}

ARM bool TownCharacterBase::isRotEnd()
{
    return !(bool)(data_.flag.flag_ & 0x10);
}

ARM void TownCharacterBase::setSwingRound(int flag)
{
    if (flag == 1) {
        data_.flag.flag_ |= 0x20;
    } else {
        data_.flag.flag_ &= ~0x20;
    }
}

ARM void TownCharacterBase::setEnableLockWait(int frame)
{
    waitCounter_ = frame;
    moveType_ = MOVE_TYPE_WAIT;
}

ARM void TownCharacterBase::execMoveWait()
{
    waitCounter_--;
}

ARM bool TownCharacterBase::isMoveWaitEnd()
{
    return waitCounter_ <= 0;
}

ARM void TownCharacterBase::setMotionLock(int flag)
{
    if (flag == 1) {
        data_.flag.flag_ |= 0x400;
        setAnimation(0);
    } else {
        data_.flag.flag_ &= ~0x400;
        setAnimation(1);
    }
}

ARM bool TownCharacterBase::isMotionLock()
{
    return (bool)(data_.flag.flag_ & 0x400);
}

ARM void TownCharacterBase::setRotFrame(int frame, short idx, int flag, int typeA)
{
    data_.flag.flag_ |= 0x10;
    dss::Vector3<short> start(0, getDir(), 0);
    short r;
    if (flag == 0) {
        r = -idx;
    } else {
        r = idx;
    }
    dss::Vector3<short> rot(0, r, 0);
    simpleMove_.setSimpleRot(start, rot, frame);
    if (typeA == 1) {
        data_.flag.flag_ |= 0x800;
    }
}

ARM bool TownCharacterBase::isRotFrameEnd()
{
    if ((bool)(data_.flag.flag_ & 0x800) == true) {
        data_.flag.flag_ &= ~0x800;
        return true;
    }
    return isRotEnd();
}

ARM void TownCharacterBase::setSurechigaiMapNo(int no)
{
    mapNo_ = no;
}

ARM int TownCharacterBase::getSurechigaiMapNo()
{
    return mapNo_;
}

ARM void TownCharacterBase::setVoice(int voice)
{
    if (voice == cmn::TalkSoundManager::MESSAGESOUND_STOP) {
        data_.flag.flag_ &= ~0x1000;
        return;
    }
    data_.flag.flag_ |= 0x1000;
    voice_ = (cmn::TalkSoundManager::MESSAGESOUND)voice;
}

ARM bool TownCharacterBase::checkVoice()
{
    return (bool)(data_.flag.flag_ & 0x1000);
}

ARM cmn::TalkSoundManager::MESSAGESOUND TownCharacterBase::getVoice()
{
    return voice_;
}

ARM void TownCharacterBase::setPaletteRate(unsigned char r, unsigned char g, unsigned char b, dss::Fix32 rate)
{
}

ARM void TownCharacterBase::setPalletRate(dss::Fix32 rate)
{
}

static const int resetValue = 0;

void TownCharacterBase::unkfunc_resetStatics()
{
    const int& value = resetValue;
    allEventLock_ = value;
    monsterTalk_ = value;
}
