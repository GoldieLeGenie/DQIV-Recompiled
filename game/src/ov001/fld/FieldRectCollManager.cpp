#pragma ipa file
#include "ov001/fld/FieldRectCollManager.hpp"
#include "main/cmn/CommonCalculate.hpp"

ARM void FieldRectCollManager::setup()
{
    rectCollCount_ = 0;
    for (int i = 0; i < RECT_COLL_NUM; i++) {
        rectColl_[i].type = RECT_NONE;
    }
}

ARM void FieldRectCollManager::setRectColl(dss::Fix32Vector3& pos1, dss::Fix32Vector3& pos2, int type)
{
    for (int i = 0; i < rectCollCount_; i++) {
        if (rectColl_[i].type == RECT_NONE) {
            rectColl_[i].pos1 = pos1;
            rectColl_[i].pos2 = pos2;
            rectColl_[i].type = type;
            break;
        }
    }
    rectColl_[rectCollCount_].pos1 = pos1;
    rectColl_[rectCollCount_].pos2 = pos2;
    rectColl_[rectCollCount_].type = type;
    rectCollCount_++;
}

ARM int FieldRectCollManager::checkFieldColl(dss::Fix32Vector3& pos)
{
    for (int i = 0; i < rectCollCount_; i++) {
        if (rectColl_[i].type == RECT_NONE || rectColl_[i].type == RECT_BALLON) {
            continue;
        }
        if (cmn::CommonCalculate::simpleAreaInCheck(rectColl_[i].pos1, rectColl_[i].pos2, pos)) {
            return rectColl_[i].type;
        }
    }
    return RECT_NONE;
}

ARM bool FieldRectCollManager::checkTypeColl(dss::Fix32Vector3& pos, int type)
{
    for (int i = 0; i < rectCollCount_; i++) {
        if (type == rectColl_[i].type) {
            if (cmn::CommonCalculate::simpleAreaInCheck(rectColl_[i].pos1, rectColl_[i].pos2, pos)) {
                return true;
            }
        }
    }
    return false;
}

ARM FieldRectCollManager* FieldRectCollManager::getSingleton()
{
    static FieldRectCollManager fieldRectCollManager;
    return &fieldRectCollManager;
}
