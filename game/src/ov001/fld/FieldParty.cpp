#pragma ipa file
#include "ov001/fld/FieldPlayerManager.hpp"
#include "ov001/fld/FieldStage.hpp"
#include "ov001/fld/FieldActionCalculate.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/StageStatus.hpp"

ARM FieldParty::FieldParty()
{
}

ARM FieldParty::~FieldParty()
{
}

ARM void FieldParty::setup()
{
    m_pos_array = g_cmnPartyInfo.getPositionArrayPointer();
    m_dir_array = g_cmnPartyInfo.getDirectionArrayPointer();
    if (g_Stage.ruraFlag_ == 0) {
        flagBashaArray_ = status::g_Party.basha_;
    } else {
        flagBashaArray_ = 0;
    }
    if (g_Stage.idoLink_.data_.link_.encount_ == 0 && g_cmnPartyInfo.prevLocation_ == 0) {
        for (int i = 0; i < POSITION_ARRAY_MAX; i++) {
            m_pos_array[i] = *position_;
            m_dir_array[i] = 4;
        }
        bashaLPos_ = getMemberPosition(2);
        bashaRPos_ = bashaLPos_;
        bashaLIdx_ = getMemberDirIdx(2);
        bashaRIdx_ = bashaLIdx_;
        flagMoveToFirst_ = 0;
        countPartyArray_ = 0;
        countLFix_ = 0;
        countRFix_ = 0;
    } else {
        g_cmnPartyInfo.getPartyInfo(position_, dirIdx_);
        g_cmnPartyInfo.getBashaInfo(&bashaLPos_, &bashaRPos_, &bashaLIdx_, &bashaRIdx_, &countPartyArray_, &countLFix_, &countRFix_);
    }
}

ARM void FieldParty::cleanup()
{
}

ARM void FieldParty::execute()
{
    prevDirIdx_ = getMemberDirIdx(2);
    if (isBashaEnable() == 1) {
        bashaArray();
    } else {
        normalArray();
    }
    countPartyArray_++;
}

ARM int FieldParty::isBashaEnable()
{
    return status::g_Party.basha_;
}

ARM void FieldParty::setBashaArray(int flag)
{
    flagBashaArray_ = flag;
}

ARM void FieldParty::normalArray()
{
    if (m_pos_array[0] != *position_) {
        for (int i = 0; i < POSITION_ARRAY_MAX - 1; i++) {
            m_pos_array[POSITION_ARRAY_MAX - 1 - i] = m_pos_array[POSITION_ARRAY_MAX - 2 - i];
            m_dir_array[POSITION_ARRAY_MAX - 1 - i] = m_dir_array[POSITION_ARRAY_MAX - 2 - i];
        }
        m_pos_array[0] = *position_;
        m_dir_array[0] = *dirIdx_;
    }
}

ARM void FieldParty::bashaArray()
{
    static const dss::Fix32 speed(1.2f);
    if (m_pos_array[0] != *position_) {
        for (int i = 0; i < POSITION_ARRAY_MAX - 1; i++) {
            m_pos_array[POSITION_ARRAY_MAX - 1 - i] = m_pos_array[POSITION_ARRAY_MAX - 2 - i];
            m_dir_array[POSITION_ARRAY_MAX - 1 - i] = m_dir_array[POSITION_ARRAY_MAX - 2 - i];
        }
        m_pos_array[0] = *position_;
        m_dir_array[0] = *dirIdx_;
    }
    if (prevDirIdx_ != getMemberDirIdx(2)) {
        countPartyArray_ = 0;
    }
    getSidePos(3, bashaLPos_, &bashaLPos_, &bashaLIdx_);
    getSidePos(1, bashaRPos_, &bashaRPos_, &bashaRIdx_);
}

ARM void FieldParty::getSidePos(int side, dss::Fix32Vector3& nowPos, dss::Fix32Vector3* nextPos, short* retIdx)
{
    int bashaIdx;
    int idx = getMemberDirIdx(2);
    dss::Fix32Vector3 bashaPos = getMemberPosition(2);
    dss::Fix32Vector3 vec;
    dss::Fix32Vector3 pos;
    dss::Fix32Vector3 unk_78;
    dss::Fix32Vector3 unk_6c;
    if (side == 3) {
        bashaIdx = FieldActionCalculate::getDir8RotIdx(idx, -2);
    } else if (side == 1) {
        bashaIdx = FieldActionCalculate::getDir8RotIdx(idx, 2);
    }
    vec = FieldActionCalculate::getVector3ByDir8(bashaIdx);
    vec.normalize();
    pos = bashaPos + vec * 13 * FieldPlayer::Speed;
    collisionSide(side, &pos, idx);
    if (countPartyArray_ > COUNT_MAX) {
        countPartyArray_ = COUNT_MAX;
    }
    *nextPos = nowPos * (COUNT_MAX - countPartyArray_) / COUNT_MAX + pos * countPartyArray_ / COUNT_MAX;
    *retIdx = idx;
}

ARM void FieldParty::fixSidePos(int side, int fix)
{
    dss::Fix32Vector3* nextPos;
    int* cont;
    if (side == 3) {
        cont = &countLFix_;
        nextPos = &bashaLPos_;
    } else {
        cont = &countRFix_;
        nextPos = &bashaRPos_;
    }
    *cont += fix;
    *cont = (*cont > COUNT_MAX) ? COUNT_MAX : ((*cont < 0) ? 0 : *cont);
    *nextPos = getMemberPosition(2) * *cont / COUNT_MAX + *nextPos * (COUNT_MAX - *cont) / COUNT_MAX;
}

ARM void FieldParty::collisionSide(int side, dss::Fix32Vector3* nextPos, int dirIdx)
{
    dss::Fix32Vector3 colPos;
    dss::Fix32Vector3 unk_54;
    dss::Fix32Vector3 fixVec;
    int fixIdx;
    int* cont;
    if (side == 3) {
        fixIdx = FieldActionCalculate::getDir8RotIdx(dirIdx, 2);
        cont = &countLFix_;
    } else {
        fixIdx = FieldActionCalculate::getDir8RotIdx(dirIdx, -2);
        cont = &countRFix_;
    }
    fixVec = FieldActionCalculate::getVector3ByDir8(fixIdx);
    fixVec.normalize();
    colPos = m_pos_array[19] + fixVec * -13;
    int blk1X = colPos.vx.value / 0x10000;
    int blk1Y = colPos.vy.value / 0x10000;
    int blk2X = nextPos->vx.value / 0x10000;
    int blk2Y = nextPos->vy.value / 0x10000;
    bool col1 = fld::FieldStage::getSingleton()->getBlockAttr(blk1X, blk1Y);
    bool col2 = fld::FieldStage::getSingleton()->getBlockAttr(blk2X, blk2Y);
    if (!col1 || !col2) {
        (*cont)++;
    } else {
        (*cont)--;
    }
    *cont = (*cont > COUNT_MAX) ? COUNT_MAX : ((*cont < 0) ? 0 : *cont);
    *nextPos += fixVec * *cont * 2 / 3;
}

ARM bool FieldParty::moveAllPlayerToFirst(int count)
{
    static int start;
    bool ret;
    count = dss::min<int>(count, 4);
    ret = true;
    for (int i = 0, n = count * 13; i < n; i++) {
        if (m_pos_array[n - i] != m_pos_array[n - i - 1] || m_dir_array[n - i] != m_dir_array[n - i - 1]) {
            ret = false;
        }
        m_pos_array[n - i] = m_pos_array[n - i - 1];
        m_dir_array[n - i] = m_dir_array[n - i - 1];
    }
    m_pos_array[0] = *position_;
    m_dir_array[0] = *dirIdx_;
    fixSidePos(3, 1);
    fixSidePos(1, 1);
    if (bashaLPos_ != bashaRPos_ && isBashaEnable() == 1) {
        ret = false;
    }
    if (count == 1) {
        ret = true;
    }
    if (start == 1 && ret == true) {
        setAllPlayerAtFirst();
        start = 0;
    }
    if (ret == false) {
        start = 1;
    }
    return ret;
}

ARM void FieldParty::setPosition(dss::Fix32Vector3 pos)
{
    *position_ = pos;
}

ARM void FieldParty::setDirIdx(int dirIdx)
{
    *dirIdx_ = dirIdx;
}

ARM dss::Fix32Vector3 FieldParty::getMemberPosition(int index)
{
    dss::Fix32Vector3 pos;
    if (isBashaEnable() == 1) {
        switch (index) {
        case 4:
            pos = bashaLPos_;
            return pos;
        case 5:
            pos = bashaRPos_;
            return pos;
        }
        pos = m_pos_array[index * 13];
        return pos;
    }
    pos = m_pos_array[index * 13];
    return pos;
}

ARM int FieldParty::getMemberDirIdx(int index)
{
    if (isBashaEnable() == 1) {
        switch (index) {
        case 4:
            return bashaLIdx_;
        case 5:
            return bashaRIdx_;
        }
        return m_dir_array[index * 13];
    }
    return m_dir_array[index * 13];
}

ARM void FieldParty::setAllPlayerAtFirst()
{
    for (int i = 0; i < POSITION_ARRAY_MAX; i++) {
        m_dir_array[i] = *dirIdx_;
        m_pos_array[i] = *position_;
    }
    m_dir_array[0] = *dirIdx_;
    m_pos_array[0] = *position_;
    bashaLPos_ = *position_;
    bashaRPos_ = *position_;
    bashaLIdx_ = *dirIdx_;
    bashaRIdx_ = *dirIdx_;
    resetBashaCount();
}

ARM void FieldParty::resetBashaCount()
{
    countPartyArray_ = 0;
    countRFix_ = 0;
    countLFix_ = 0;
}

ARM void FieldParty::setPositionArrayPointer(dss::Fix32Vector3* pos)
{
    m_pos_array = pos;
}

ARM void FieldParty::setDirIdxArrayPointer(short* dirIdx)
{
    m_dir_array = dirIdx;
}

ARM void FieldParty::setPositionPointer(dss::Fix32Vector3* pos)
{
    position_ = pos;
}

ARM void FieldParty::setDirIdxPointer(short* dirIdx)
{
    dirIdx_ = dirIdx;
}

ARM void FieldParty::savePartyDrawInfo()
{
    g_cmnPartyInfo.setPartyInfo(position_, *dirIdx_);
    g_cmnPartyInfo.setBashaInfo(&bashaLPos_, &bashaRPos_, bashaLIdx_, bashaRIdx_, countPartyArray_, countLFix_, countRFix_);
}
