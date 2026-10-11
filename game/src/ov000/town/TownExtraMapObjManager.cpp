#include "ov000/town/TownExtraMapObjManager.hpp"
#include "ov000/town/TownPlayerAction.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "main/fld/FldStage.hpp"

ARM TownExtraMapObjManager* TownExtraMapObjManager::getSingleton()
{
    static TownExtraMapObjManager m_singleton;
    return &m_singleton;
}

ARM void TownExtraMapObjManager::setup()
{
    floorMapObjCount_ = 0;
}

ARM void TownExtraMapObjManager::setData(int mapUid, dss::Fix32Vector3 pos)
{
    for (int i = 0; i < floorMapObjCount_; i++) {
        if (mapUid == floorMapObj_[i].mapUid || floorMapObj_[i].mapUid == 0) {
            floorMapObj_[floorMapObjCount_].position = pos;
            return;
        }
    }
    floorMapObj_[floorMapObjCount_].mapUid = mapUid;
    floorMapObj_[floorMapObjCount_].position = pos;
    floorMapObjCount_++;
}

ARM int TownExtraMapObjManager::checkFloorMapUid(dss::Fix32Vector3& pos)
{
    dss::Fix32 RR = TownPlayerAction::collR * TownPlayerAction::collR * 4;
    dss::Fix32 R2 = TownPlayerAction::collR * 2;
    int retUid = 0;
    dss::Fix32Vector3 vec;
    dss::Fix32 tempRR;
    dss::Fix32 height;
    for (int i = 0; i < floorMapObjCount_; i++) {
        vec = (pos - floorMapObj_[i].position);
        height.value = unkfunc_02031e84(vec.vy.value);
        vec.vy = 0L;
        if ((tempRR = vec.lengthsq()) < RR && height < R2) {
            retUid = floorMapObj_[i].mapUid;
            RR = tempRR;
        }
    }
    return retUid;
}

ARM bool TownExtraMapObjManager::getPosition(int mapUid, dss::Fix32Vector3& position)
{
    switch (mapUid) {
    case 0x1843:
    case 0x1845:
    case 0x1848:
    case 0x184e:
        position = TownPlayerManager::getSingleton()->getPosition();
        return true;
    }
    for (int i = 0; i < floorMapObjCount_; i++) {
        if (mapUid == floorMapObj_[i].mapUid) {
            position = floorMapObj_[i].position;
            return true;
        }
    }
    return false;
}
