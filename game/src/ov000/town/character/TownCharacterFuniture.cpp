#include "ov000/town/TownCharacter.hpp"
#include "ov000/town/TownActionCalculate.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "ov000/town/TownActionWalk.hpp"
#include "ov000/Commands/TownCommand.hpp"
#include "main/cmn/CommonPartyInfo.hpp"

ARM void TownCharacterFuniture::setup(TOWN_CHARACTER& data)
{
    rgbFrame_ = -1;
    changeAlphaType_ = CHANGE_NONE;
}

ARM void TownCharacterFuniture::setDisplay(int flag)
{
}

ARM int TownCharacterFuniture::isDisplay()
{
    return false;
}

ARM void TownCharacterFuniture::execute()
{
    TownCharacterBase::execute();
}

ARM void TownCharacterFuniture::setAnimation(int flag)
{
}

ARM void TownCharacterFuniture::setNearCharacter(int flag)
{
}

ARM void TownCharacterFuniture::cleanup()
{
}

ARM void TownCharacterFuniture::setPosition(dss::Fix32Vector3& pos)
{
    dss::Fix32Vector3 diff = pos - data_.position;
    TownStageManager::getSingleton()->addMapUidPosFX32(mapUid_, diff);
    data_.position = pos;
}

ARM void TownCharacterFuniture::execMovePassive()
{
    int id = TownStageManager::getSingleton()->coll_.m_id;
    if (id == -1) {
        return;
    }
    int obj = coll_GetObjId(TownStageManager::getSingleton()->stage_.m_fld.m_coll, id);
    int uid = TownStageManager::getSingleton()->stage_.m_fld.GetMapObjUid(obj);
    if (uid != mapUid_) {
        return;
    }
    dss::Fix32Vector3 pos = data_.position;
    dss::Fix32Vector3 party = g_cmnPartyInfo.position_;
    dss::Fix32Vector3 dir = pos - party;
    if (TownActionWalk::getSingleton()->moveFlag_ == 0) {
        return;
    }
    if (moveData_.counter % 40 > 20 || moveData_.counter % 2 != 0) {
        moveData_.counter++;
        return;
    }
    short idx = 0;
    TownActionCalculate::getIdxByVec(idx, dir);
    unsigned char d = TownActionCalculate::getParamDir4ByIdx(idx);
    dir = TownActionCalculate::getParamVec(d);
    dss::Fix32Vector3 next = pos + dir * moveData_.speed;
    dss::Fix32Vector3 out;
    TownStageManager::getSingleton()->boxCompute(pos, next, collR_, &out);
    setPosition(out);
    moveData_.counter++;
}

ARM void TownCharacterFuniture::setMapUid(int uid)
{
    mapUid_ = uid;
    data_.position = TownStageManager::getSingleton()->getMapUidPos(uid);
    collR_ = 0x99a;
}

ARM void TownCharacterFuniture::draw()
{
}
