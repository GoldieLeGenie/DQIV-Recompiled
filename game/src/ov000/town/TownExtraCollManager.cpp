#pragma ipa file
#include "ov000/town/TownExtraCollManager.hpp"
#include "ov000/town/TownActionCalculate.hpp"
#include "ov000/town/TownCharacterManager.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "main/cmn/CommonPartyInfo.hpp"

dss::Fix32 TownExtraCollManager::sleepCharaW;
dss::Fix32 TownExtraCollManager::sleepCharaH;
dss::Fix32 TownExtraCollManager::sleepCharaY;
static const dss::Fix32 unusedR1(0x333);
static const dss::Fix32 unusedR2(0x99a);

ARM TownExtraCollManager* TownExtraCollManager::getSingleton()
{
    static TownExtraCollManager townExtraCollManager;
    return &townExtraCollManager;
}

ARM void TownExtraCollManager::setup()
{
    for (int i = 0; i < EXTRA_COLL_COUNT_MAX; i++) {
        extraCollData_[i].id = -1;
    }
    sleepCharaW.value = 0x604;
    sleepCharaH.value = 0x85c;
    sleepCharaY.value = 0x800;
    extraCollCount_ = 0;
}

ARM void TownExtraCollManager::resetCharaColl(int charaNo, int type)
{
    for (int i = 0; i < EXTRA_COLL_COUNT_MAX; i++) {
        if (extraCollData_[i].type == type && extraCollData_[i].id == charaNo) {
            extraCollData_[i].flag = 0;
            int objectId = extraCollData_[i].objectId;
            TownStageManager* stage = TownStageManager::getSingleton();
            coll_EraseObjId(stage->stage_.m_fld.m_coll, objectId);
        }
    }
}

ARM void TownExtraCollManager::addCharacterColl(int ctrl, int type)
{
    dss::Fix32Vector3 vec;
    vec.vx.value = vec.vy.value = vec.vz.value = 0x1000;
    static const dss::Fix32 bigRockR(0x119a);
    int charaNo = TownCharacterManager::getSingleton()->getCharaIndex(ctrl);
    dss::Fix32Vector3 pos(TownCharacterManager::getSingleton()->getPosition(ctrl));
    switch (type) {
    case TownCharacterBase::TOWN_CHARACTER_MODEL:
        switch (charaNo) {
        case 1:
            vec.vx.value = 0x9c4;
            vec.vz.value = 0xbb8;
            pos.vz.value += 0x1b58;
            break;
        case 2:
            vec.vx = vec.vz = bigRockR;
            break;
        case 0:
        case 5:
        case 6:
            return;
        }
        break;
    case TownCharacterBase::TOWN_CHARACTER_MONSTER:
        switch (charaNo) {
        case 311:
            vec.vx.value = 0xfa0;
            vec.vz.value = 0x157c;
            break;
        case 313:
            vec.vx.value = 0x1194;
            vec.vz.value = 0x15e0;
            break;
        case 314:
            vec.vx.value = 0x12c0;
            vec.vz.value = 0x12c0;
            break;
        case 315:
            vec.vx.value = 0x1388;
            vec.vz.value = 0x1450;
            break;
        case 316:
        case 317:
            vec.vx.value = 0x7d0;
            vec.vz.value = 0x1388;
            break;
        }
        pos.vz.value -= 0x800;
        break;
    }
    if (g_cmnPartyInfo.collFlag_ == 1) {
        vec = dss::Fix32Vector3(g_cmnPartyInfo.collVec_);
    }
    for (int i = 0; i < EXTRA_COLL_COUNT_MAX; i++) {
        if (extraCollData_[i].type == TownCharacterBase::TOWN_CHARACTER_MONSTER && extraCollData_[i].id == ctrl && extraCollData_[i].flag == 0) {
            extraCollData_[i].flag = 1;
            TownStageManager::getSingleton()->addBoxCollision(pos, vec, extraCollData_[i].objectId);
            return;
        }
    }
    int no = extraCollCount_;
    extraCollData_[no].objectId = -1;
    extraCollData_[no].flag = 1;
    extraCollData_[no].type = TownCharacterBase::TOWN_CHARACTER_MONSTER;
    extraCollData_[no].id = ctrl;
    TownStageManager::getSingleton()->addBoxCollision(pos, vec, extraCollData_[no].objectId);
    extraCollCount_++;
}

ARM int TownExtraCollManager::isExtraCollChara(int objectNo, int& charaNo)
{
    for (int i = 0; i < extraCollCount_; i++) {
        if (objectNo == extraCollData_[i].objectId) {
            charaNo = extraCollData_[i].id;
            return extraCollData_[i].type;
        }
    }
    return -1;
}

ARM void TownExtraCollManager::addSleepChara(int charaNo)
{
    dss::Fix32Vector3 vec;
    dss::Fix32Vector3 pos;
    pos = TownCharacterManager::getSingleton()->getPosition(charaNo);
    int direction = TownCharacterManager::getSingleton()->getDirection(charaNo);
    char dir4 = TownActionCalculate::getParamDir4ByIdx(direction);
    vec.vy = sleepCharaY;
    switch (dir4) {
    case 0:
        vec.vx = sleepCharaW;
        vec.vz = sleepCharaH;
        break;
    case 1:
        vec.vx = sleepCharaH;
        vec.vz = sleepCharaW;
        break;
    case 2:
        vec.vx = sleepCharaW;
        vec.vz = sleepCharaH;
        break;
    case 3:
        vec.vx = sleepCharaH;
        vec.vz = sleepCharaW;
        break;
    }
    if (g_cmnPartyInfo.collFlag_ == 1) {
        vec = dss::Fix32Vector3(g_cmnPartyInfo.collVec_);
    }
    for (int i = 0; i < EXTRA_COLL_COUNT_MAX; i++) {
        if (extraCollData_[i].type == TownCharacterBase::TOWN_CHARACTER_NORMAL && extraCollData_[i].id == charaNo && extraCollData_[i].flag == 0) {
            extraCollData_[i].flag = 1;
            TownStageManager::getSingleton()->addBoxCollision(pos, vec, extraCollData_[i].objectId);
            return;
        }
    }
    extraCollCount_++;
    int no = extraCollCount_ - 1;
    extraCollData_[no].objectId = -1;
    extraCollData_[no].flag = 1;
    extraCollData_[no].type = TownCharacterBase::TOWN_CHARACTER_NORMAL;
    extraCollData_[no].id = charaNo;
    TownStageManager::getSingleton()->addBoxCollision(pos, vec, extraCollData_[no].objectId);
}

ARM void TownExtraCollManager::addMoveColl(int charNo, int type, dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos)
{
    switch (type) {
    case TownCharacterBase::TOWN_CHARACTER_SLEEP:
        type = TownCharacterBase::TOWN_CHARACTER_NORMAL;
        break;
    case TownCharacterBase::TOWN_CHARACTER_MONSTER:
    case TownCharacterBase::TOWN_CHARACTER_MODEL:
        type = TownCharacterBase::TOWN_CHARACTER_MONSTER;
        break;
    }
    for (int i = 0; i < extraCollCount_; i++) {
        if (extraCollData_[i].type == type && extraCollData_[i].id == charNo) {
            TownStageManager::getSingleton()->addMovePosByObjNo(extraCollData_[i].objectId, nowPos, nextPos);
        }
    }
}
