#include "ov000/town/TownCharacterManager.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/object/DisplayCharacter.hpp"
#include "ov000/town/TownCamera.hpp"
#include "ov000/town/TownSystem.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownPlayerAction.hpp"
#include "ov000/town/TownExtraCollManager.hpp"
#include "ov000/town/TownActionCalculate.hpp"
#include "ov000/Commands/TownCommand.hpp"
#include "main/dss/Camera.hpp"
#include "ov003/btl/BattleActorAnimation.hpp"
#include "ov003/btl/BattleMonsterMask.hpp"
#include <nitro/fx/fx_atan.h>
#include "main/dss/ScreenPosition.hpp"

static const int max_change = 36;
static int monsterObj[max_change][3] = {
    { 158, 310, TownCharacterBase::TOWN_CHARACTER_MONSTER },
    { 161, 311, TownCharacterBase::TOWN_CHARACTER_MONSTER },
    { 162, 312, TownCharacterBase::TOWN_CHARACTER_MONSTER },
    { 164, 313, TownCharacterBase::TOWN_CHARACTER_MONSTER },
    { 166, 314, TownCharacterBase::TOWN_CHARACTER_MONSTER },
    { 167, 315, TownCharacterBase::TOWN_CHARACTER_MONSTER },
    { 169, 316, TownCharacterBase::TOWN_CHARACTER_MONSTER },
    { 170, 317, TownCharacterBase::TOWN_CHARACTER_MONSTER },
    { 192, 318, TownCharacterBase::TOWN_CHARACTER_MONSTER },
    { 196, 319, TownCharacterBase::TOWN_CHARACTER_MONSTER },
    { 197, 320, TownCharacterBase::TOWN_CHARACTER_MONSTER },
    { 210, 321, TownCharacterBase::TOWN_CHARACTER_MONSTER },
    { 211, 322, TownCharacterBase::TOWN_CHARACTER_MONSTER },
    { 212, 323, TownCharacterBase::TOWN_CHARACTER_MONSTER },
    { 213, 324, TownCharacterBase::TOWN_CHARACTER_MONSTER },
    { 165, 0, TownCharacterBase::TOWN_CHARACTER_MODEL },
    { 199, 1, TownCharacterBase::TOWN_CHARACTER_MODEL },
    { 200, 2, TownCharacterBase::TOWN_CHARACTER_MODEL },
    { 201, 3, TownCharacterBase::TOWN_CHARACTER_MODEL },
    { 202, 4, TownCharacterBase::TOWN_CHARACTER_MODEL },
    { 203, 5, TownCharacterBase::TOWN_CHARACTER_MODEL },
    { 204, 6, TownCharacterBase::TOWN_CHARACTER_MODEL },
    { 221, 7, TownCharacterBase::TOWN_CHARACTER_MODEL },
    { 222, 8, TownCharacterBase::TOWN_CHARACTER_MODEL },
    { 223, 9, TownCharacterBase::TOWN_CHARACTER_MODEL },
    { 224, 10, TownCharacterBase::TOWN_CHARACTER_MODEL },
    { 225, 11, TownCharacterBase::TOWN_CHARACTER_MODEL },
    { 226, 12, TownCharacterBase::TOWN_CHARACTER_MODEL },
    { 227, 13, TownCharacterBase::TOWN_CHARACTER_MODEL },
    { 228, 14, TownCharacterBase::TOWN_CHARACTER_MODEL },
    { 229, 15, TownCharacterBase::TOWN_CHARACTER_MODEL },
    { 230, 16, TownCharacterBase::TOWN_CHARACTER_MODEL },
    { 298, 17, TownCharacterBase::TOWN_CHARACTER_MODEL },
    { 300, 18, TownCharacterBase::TOWN_CHARACTER_MODEL },
    { 299, 0, TownCharacterBase::TOWN_CHARACTER_FURNITURE },
    { 215, 325, TownCharacterBase::TOWN_CHARACTER_MONSTER },
};

ARM TownCharacterManager::TownCharacterManager()
{
}

ARM TownCharacterManager* TownCharacterManager::getSingleton()
{
    static TownCharacterManager m_singleton;
    return &m_singleton;
}

ARM TownCharacterManager::~TownCharacterManager()
{
}

ARM void TownCharacterManager::initialize()
{
    characterStorage.initialize();
    TownCharacterBase::areaCheck_ = 0;
}

ARM void TownCharacterManager::terminate()
{
    for (int i = 0; i < TOWN_CHARACTER_MAX; i++) {
        if (character_[i]) {
            cleanup(i);
        }
    }
    characterStorage.terminate();
}

ARM void TownCharacterManager::execute()
{
    int even = (unkfunc_02081254() & 1) == 0;
    for (int i = 0; i < TOWN_CHARACTER_MAX; i++) {
        if (character_[i]) {
            character_[i]->execute();
            character_[i]->setPosition2d(even, *unkfunc_02057128(i));
        }
    }
    TownCharacterBase::areaCheck_ = 0;
}

ARM int TownCharacterManager::setup(TOWN_CHARACTER& chara)
{
    int result = -1;
    int drawType = TownCharacterBase::TOWN_CHARACTER_NORMAL;
    for (int i = 0; i < TOWN_CHARACTER_MAX; i++) {
        if (character_[i] == 0) {
            for (int count = 0; count < max_change; count++) {
                if (chara.charaIndex == monsterObj[count][0]) {
                    chara.charaIndex = monsterObj[count][1];
                    drawType = monsterObj[count][2];
                    break;
                }
            }
            character_[i] = characterStorage.getContainer(drawType);
            character_[i]->render_ = &TownSystem::getSingleton()->render_;
            character_[i]->unk_d0 = &TownCamera::getSingleton()->camera_.unk_004;
            character_[i]->setup(chara);
            character_[i]->type_ = drawType;
            character_[i]->data_.ctrlNo = i;
            character_[i]->setMonsterSpeak(TownCharacterBase::monsterTalk_);
            unkfunc_0205710c(i, &character_[i]->data_.position);
            result = i;
            break;
        }
    }
    townCharacterCount_++;
    int type = character_[result]->type_;
    if (type == TownCharacterBase::TOWN_CHARACTER_MONSTER || type == TownCharacterBase::TOWN_CHARACTER_MODEL) {
        TownExtraCollManager::getSingleton()->addCharacterColl(result, type);
    }
    return result;
}

ARM void TownCharacterManager::draw()
{
    for (int i = 0; i < TOWN_CHARACTER_MAX; i++) {
        if (character_[i]) {
            character_[i]->draw();
        }
    }
}

ARM void TownCharacterManager::requestCharacterReload()
{
    for (int i = 0; i < TOWN_CHARACTER_MAX; i++) {
        if (character_[i]) {
            character_[i]->requestReload();
        }
    }
}

ARM void TownCharacterManager::setPosing(int index, int pose)
{
    for (int count = 0; count < max_change; count++) {
        if (pose == monsterObj[count][0]) {
            pose = monsterObj[count][1];
            break;
        }
    }
    character_[index]->setPosing(pose);
}

ARM int TownCharacterManager::getCharatType(int index)
{
    return character_[index]->type_;
}

ARM void TownCharacterManager::cleanup(int index)
{
    townCharacterCount_--;
    characterStorage.restoreContainer(character_[index]->type_);
    character_[index]->cleanup();
    character_[index]->data_.enable = 0;
    character_[index] = 0;
}

ARM void TownCharacterManager::setPlayerDirection(int index)
{
    dss::Fix32Vector3 pos = TownPlayerManager::getSingleton()->getPosition();
    dss::Fix32Vector3 target = getPosition(index);
    dss::Fix32Vector3 dir = pos - target;
    dir.normalize();
    character_[index]->setSwingRoundIdx();
    setRotate(index, FX_Atan2Idx(dir.vx.value, dir.vz.value));
}

ARM dss::Fix32Vector3& TownCharacterManager::getPosition(int index)
{
    return character_[index]->data_.position;
}

ARM void TownCharacterManager::setRotate(int index, int rot)
{
    character_[index]->setDir(rot);
}

ARM void TownCharacterManager::setRotate(int index, dss::Vector3<short>& rot)
{
    character_[index]->setRotation(rot);
}

ARM void TownCharacterManager::setTalkedArea(int index, int flag)
{
    character_[index]->setSpeak(flag);
}

ARM void TownCharacterManager::setTalked(int index, int flag)
{
    character_[index]->setTalked(flag);
}

ARM bool TownCharacterManager::isTalked(int index)
{
    return character_[index]->getTalked();
}

ARM void TownCharacterManager::setShadow(int index, int flag)
{
    character_[index]->setShadow(flag);
}

ARM void TownCharacterManager::setAnimation(int index, int flag)
{
    character_[index]->setAnimation(flag);
}

ARM void TownCharacterManager::setWriggleCharacter(int index, int flag)
{
    character_[index]->setWriggleCharacter(flag);
}

ARM void TownCharacterManager::setNearCharacter(int index, int flag)
{
    character_[index]->setNearCharacter(flag);
}

ARM void TownCharacterManager::setDisplay(int index, int flag)
{
    character_[index]->setDisplay(flag);
    character_[index]->setCollFlag(flag);
    int type = character_[index]->type_;
    switch (type) {
    case TownCharacterBase::TOWN_CHARACTER_MONSTER:
    case TownCharacterBase::TOWN_CHARACTER_MODEL:
        if (flag == 1) {
            TownExtraCollManager::getSingleton()->addCharacterColl(index, type);
        } else {
            TownExtraCollManager::getSingleton()->resetCharaColl(index, TownCharacterBase::TOWN_CHARACTER_MONSTER);
        }
        break;
    case TownCharacterBase::TOWN_CHARACTER_SLEEP:
        if (flag == 1) {
            TownExtraCollManager::getSingleton()->addCharacterColl(index, TownCharacterBase::TOWN_CHARACTER_SLEEP);
        } else {
            TownExtraCollManager::getSingleton()->resetCharaColl(index, TownCharacterBase::TOWN_CHARACTER_NORMAL);
        }
        break;
    }
}

ARM void TownCharacterManager::setAlpha(int index, unsigned char alpha)
{
    character_[index]->setAlpha(alpha);
}

ARM void TownCharacterManager::setPosition(int index, dss::Fix32Vector3& pos)
{
    character_[index]->setPosition(pos);
}

ARM void TownCharacterManager::setSleepCharacter(int index, int flag)
{
    if (flag == 1) {
        if (character_[index]->type_ != TownCharacterBase::TOWN_CHARACTER_SLEEP && character_[index]->getCollFlag() == 1 && character_[index]->isDisplay() == 1) {
            TownExtraCollManager::getSingleton()->addSleepChara(index);
        }
    } else {
        if (character_[index]->type_ == TownCharacterBase::TOWN_CHARACTER_SLEEP && character_[index]->getCollFlag() == 1) {
            TownExtraCollManager::getSingleton()->resetCharaColl(index, TownCharacterBase::TOWN_CHARACTER_NORMAL);
        }
    }
    character_[index]->setMonsterSpeak(flag);
    character_[index]->setSleepCharacter(flag);
}

ARM void TownCharacterManager::setCollFlag(int index, int flag)
{
    if (flag == 1) {
        if (character_[index]->type_ == TownCharacterBase::TOWN_CHARACTER_SLEEP && character_[index]->getCollFlag() == 0) {
            TownExtraCollManager::getSingleton()->addSleepChara(index);
        } else {
            TownCharacterBase* p = character_[index];
            if (p->type_ == TownCharacterBase::TOWN_CHARACTER_MONSTER) {
                TownExtraCollManager::getSingleton()->addCharacterColl(index, p->data_.charaIndex);
            }
        }
    } else {
        if (character_[index]->type_ == TownCharacterBase::TOWN_CHARACTER_SLEEP && character_[index]->getCollFlag() == 1) {
            TownExtraCollManager::getSingleton()->resetCharaColl(index, TownCharacterBase::TOWN_CHARACTER_NORMAL);
        } else if (character_[index]->type_ == TownCharacterBase::TOWN_CHARACTER_MONSTER) {
            TownExtraCollManager::getSingleton()->resetCharaColl(index, TownCharacterBase::TOWN_CHARACTER_MONSTER);
        }
    }
    character_[index]->setCollFlag(flag);
}

ARM int TownCharacterManager::getDirection(int index)
{
    return character_[index]->getDir();
}

ARM void TownCharacterManager::resetCharaTalk()
{
    for (int i = 0; i < TOWN_CHARACTER_MAX; i++) {
        if (character_[i]) {
            character_[i]->resetTalk();
        }
    }
}

ARM bool TownCharacterManager::charaToCharaColl(TownCharacterBase* chara)
{
    dss::Fix32Vector3 vec;
    dss::Fix32 RR = TownPlayerAction::collR * TownPlayerAction::collR * 4;
    for (int i = 0; i < TOWN_CHARACTER_MAX; i++) {
        TownCharacterBase* p = character_[i];
        if (p && (p->stageColl_ & 4) && p != chara && p->isDisplay() == 1) {
            vec = character_[i]->data_.position - chara->data_.position;
            if (vec.lengthsq() <= RR) {
                return true;
            }
        }
    }
    return false;
}

ARM void TownCharacterManager::characterColl(dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos, dss::Fix32Vector3& vecN, dss::Fix32 r, dss::Fix32 ctrLen, int polyNo, int farTalk, int menuFlag)
{
    dss::Fix32 len(0x800);
    dss::Fix32Vector3 pos(nowPos);
    dss::Fix32Vector3 vec;
    if (menuFlag == 0) {
        pos.vy -= TownPlayerAction::collR;
    }
    for (int i = 0; i < TOWN_CHARACTER_MAX; i++) {
        if (character_[i] && character_[i]->isDisplay()) {
            dss::Fix32Vector3& target = character_[i]->data_.position;
            vec = target - pos;
            if (unkfunc_02031e84(vec.vy.value) < 0x1000) {
                vec.vy = 0L;
                if (vec.length() <= TownPlayerAction::talkR) {
                    character_[i]->setSpeak(1);
                }
                if (search_ == 1) {
                    if (polyNo != -1) {
                        if (TownStageManager::getSingleton()->isPolyFacePosition(polyNo, target, len) == 1) {
                            dss::Fix32Vector3 normal;
                            TownStageManager::getSingleton()->getPolyDirection(normal, polyNo);
                            vec.normalize();
                            if ((vec * normal).value > 0xe42) {
                                character_[i]->setSpeak(1);
                                character_[i]->setCounterTalk(1);
                            }
                        }
                    }
                    if (farTalk == 1) {
                        if (vec.lengthsq() <= ctrLen * ctrLen) {
                            vec.normalize();
                            if ((vec * vecN).value > 0xe42) {
                                character_[i]->setSpeak(1);
                                character_[i]->setCounterTalk(1);
                            }
                        }
                    }
                }
            }
            if (character_[i]->getCollFlag() && character_[i]->getSpeak() && character_[i]->type_ != TownCharacterBase::TOWN_CHARACTER_SLEEP) {
                TownActionCalculate::crossCheck(nowPos, nextPos, target, r);
            }
        }
    }
}

ARM bool TownCharacterManager::checkAbortPlayerPos(dss::Fix32Vector3 pos)
{
    for (int i = 0; i < TOWN_CHARACTER_MAX; i++) {
        if (character_[i]) {
            if (((character_[i]->data_.position - pos)).lengthsq() < TownPlayerAction::coll2RR) {
                return false;
            }
        }
    }
    return true;
}

ARM bool TownCharacterManager::checkTalkingNearCharacter(dss::Fix32Vector3& pos, short dirIdx, int searchObject)
{
    dss::Fix32Vector3 playerDir;
    dss::Fix32Vector3 vec;
    dss::Fix32Vector3 target;
    dss::Fix32Vector3 tempPos;
    dss::Fix32 tempDot;
    dss::Fix32 dot;
    TownActionCalculate::getDirByIdx(dirIdx, playerDir);
    dot.value = 0xb50;
    int charNo = -1;
    dss::Fix32 tempLen2;
    tempLen2.value = 0x7fffffff;
    for (int i = 0; i < TOWN_CHARACTER_MAX; i++) {
        if (character_[i] && character_[i]->getSpeak()) {
            target = character_[i]->data_.position;
            vec = target - pos;
            vec.vy = 0L;
            if (vec.lengthsq() < tempLen2) {
                dss::Fix32Vector3 tvec(vec);
                tvec.normalize();
                tempDot = tvec * playerDir;
                if (dot < tempDot) {
                    charNo = i;
                    tempLen2 = vec.lengthsq();
                    tempPos = target;
                }
            }
        }
    }
    if (charNo != -1) {
        dss::Fix32 unused;
        if (character_[charNo]->getCounterTalk() == 1 || character_[charNo]->type_ == TownCharacterBase::TOWN_CHARACTER_SLEEP) {
            if (TownStageManager::getSingleton()->checkCrossNum(pos, tempPos, 1) == 0) {
                character_[charNo]->setCounterTalk(0);
            }
            character_[charNo]->setTalked(1);
            return true;
        } else if (TownStageManager::getSingleton()->checkCrossNumCheckUnder(pos, tempPos, 1) == 0) {
            character_[charNo]->setTalked(1);
            return true;
        }
    }
    return false;
}

ARM void TownCharacterManager::setCharaAnim(int index, short count)
{
    character_[index]->setAnimation(1);
    character_[index]->animCounter_ = count;
}

ARM void TownCharacterManager::eventLockAllChraraAnim()
{
    BillboardCharacter::setAllCharaAnim(0);
    for (int i = 0; i < TOWN_CHARACTER_MAX; i++) {
        if (character_[i] && !character_[i]->isMotionLock()) {
            character_[i]->setAnimation(2);
        }
    }
}

ARM int TownCharacterManager::getCharaIndex(int index)
{
    if (character_[index]) {
        return character_[index]->data_.charaIndex;
    }
    return -1;
}

ARM void TownCharacterManager::restoreCharacterAnim()
{
    BillboardCharacter::setAllCharaAnim(1);
    for (int i = 0; i < TOWN_CHARACTER_MAX; i++) {
        if (character_[i] && !character_[i]->isMotionLock()) {
            character_[i]->setAnimation(1);
        }
    }
}

ARM void TownCharacterManager::setAllEventLock(int flag)
{
    TownCharacterBase::allEventLock_ = flag;
    for (int i = 0; i < TOWN_CHARACTER_MAX; i++) {
        if (character_[i]) {
            character_[i]->setPersonalEventLock(flag);
        }
    }
}

ARM void TownCharacterManager::setCopyPlayerChara(int index, dss::Fix32Vector3& pos, short idx, int charNo)
{
    if (character_[index] == 0) {
        return;
    }
    character_[index]->setPosition(pos);
    character_[index]->setDir(idx);
    character_[index]->changePose(charNo);
    character_[index]->setDisplay(1);
}

ARM bool TownCharacterManager::checkIkadaTalk(dss::Fix32Vector3& target, dss::Fix32Vector3& playerDir)
{
    int charNo = -1;
    dss::Fix32Vector3 vec;
    dss::Fix32 farTargetsq = TownPlayerAction::collR * TownPlayerAction::collR * 3;
    for (int i = 0; i < TOWN_CHARACTER_MAX; i++) {
        if (character_[i] && character_[i]->isDisplay()) {
            vec = target - character_[i]->data_.position;
            vec.vy = 0L;
            dss::Fix32Vector3 tvec(vec);
            tvec.normalize();
            if (vec.lengthsq() < farTargetsq) {
                farTargetsq = vec.lengthsq();
                charNo = i;
            }
        }
    }
    if (charNo == -1) {
        return false;
    }
    character_[charNo]->setSpeak(1);
    character_[charNo]->setCounterTalk(1);
    character_[charNo]->setTalked(1);
    return true;
}

ARM void TownCharacterManager::setMonsterSpeakAll(int flag)
{
    for (int i = 0; i < TOWN_CHARACTER_MAX; i++) {
        if (character_[i]) {
            character_[i]->setMonsterSpeak(flag);
        }
    }
}

ARM void TownCharacterManager::checkObjectInTalk(int objectId)
{
    if (search_ != 1) {
        return;
    }
    for (int i = 0; i < TOWN_CHARACTER_MAX; i++) {
        if (character_[i] && character_[i]->isDisplay()) {
            TownCharacterBase* p = character_[i];
            TownStageManager* stage = TownStageManager::getSingleton();
            if (stage->stage_.getObjectIn(objectId, p->data_.position) == 1) {
                character_[i]->setSpeak(1);
                character_[i]->setCounterTalk(1);
            }
        }
    }
}

ARM void TownCharacterManager::setAllMotionLock(int flag)
{
    for (int i = 0; i < TOWN_CHARACTER_MAX; i++) {
        if (character_[i]) {
            character_[i]->setMotionLock(flag);
        }
    }
}
