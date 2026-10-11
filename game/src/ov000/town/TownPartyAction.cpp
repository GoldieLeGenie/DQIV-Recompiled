#pragma ipa file
#include "ov000/town/TownPartyAction.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownPlayerAction.hpp"
#include "ov000/town/TownActionCalculate.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/status/StageStatus.hpp"

ARM TownPartyAction::TownPartyAction()
{
    changeAlpha_ = 0;
}

ARM TownPartyAction::~TownPartyAction()
{
}

ARM void TownPartyAction::setup()
{
    if (g_Stage.idoLink_.data_.link_.encount_ == 0 && g_cmnPartyInfo.prevLocation_ == 0) {
        for (int i = 0; i < 128; i++) {
            m_pos_array[i] = g_cmnPartyInfo.position_;
            m_dir_array[i] = g_cmnPartyInfo.dirIdx_;
        }
    }
    for (int i = 0; i < 8; i++) {
        partyAlphaFlag_[i] = 0;
    }
    setFormation_ = 0;
    moveType_ = 0;
    half_ = 0;
    counter_ = 0;
    fixFlag_ = 0;
    script_ = 0;
    distanceCount_ = 2;
}

ARM void TownPartyAction::cleanup()
{
}

ARM int TownPartyAction::moveAllPlayerToFirst(int count)
{
    static int start;
    int i;
    int move;
    int n;
    int ret;
    int idx;
    fixFlag_ = 0;
    ret = 1;
    move = 1;
    if (half_ == 1) {
        if (counter_ % distanceCount_ != 0) {
            move = 0;
        }
        counter_++;
    }
    if (move == 1) {
        n = (count - 1) * 8;
        i = 0;
        if (n > 0) {
            idx = n - 1;
            do {
                if (m_pos_array[n - i] != m_pos_array[idx - i] || m_dir_array[n - i] != m_dir_array[idx - i]) {
                    ret = 0;
                }
                m_pos_array[n - i] = m_pos_array[idx - i];
                m_dir_array[n - i] = m_dir_array[idx - i];
                i++;
            } while (i < n);
        }
        m_pos_array[0] = g_cmnPartyInfo.position_;
        m_dir_array[0] = g_cmnPartyInfo.dirIdx_;
    } else {
        ret = 0;
    }
    if (ret == 1) {
        half_ = 0;
        counter_ = 0;
    }
    if (start == 1 && ret == 1) {
        setAllPotition(g_cmnPartyInfo.position_);
        start = 0;
    }
    if (ret == 0) {
        start = 1;
    }
    return ret;
}

ARM void TownPartyAction::normalMove()
{
    static const dss::Fix32 unkLength2(-0.3f);
    fixFlag_ = 0;
    if (moveFirstFlag_ == 0) {
        if (script_ == 0) {
            if (!((*m_pos_array).length(g_cmnPartyInfo.position_) > dss::Fix32(1L) / 32)) {
                return;
            }
            if (setFormation_ == 1) {
                for (int i = 0; i < 128; i++) {
                    m_dir_array[i] = TownPlayerManager::getSingleton()->frmDirIdx_;
                }
                setFormation_ = 0;
            }
            for (int i = 0; i < 127; i++) {
                m_pos_array[127 - i] = m_pos_array[126 - i];
                m_dir_array[127 - i] = m_dir_array[126 - i];
            }
            m_pos_array[0] = g_cmnPartyInfo.position_;
            m_dir_array[0] = g_cmnPartyInfo.dirIdx_;
            return;
        }
        dss::Fix32Vector3 vec = g_cmnPartyInfo.position_ - *m_pos_array;
        if (!(g_cmnPartyInfo.position_ != g_cmnPartyInfo.prev_position_)) {
            return;
        }
        if (vec.length() >= TownPlayerAction::walkSpeed) {
            for (int i = 0; i < 127; i++) {
                m_pos_array[127 - i] = m_pos_array[126 - i];
                m_dir_array[127 - i] = m_dir_array[126 - i];
            }
            vec = g_cmnPartyInfo.position_ - g_cmnPartyInfo.prev_position_;
            vec.normalize();
            m_pos_array[0] = g_cmnPartyInfo.position_;
            m_dir_array[0] = g_cmnPartyInfo.dirIdx_;
            m_pos_array[1] = g_cmnPartyInfo.position_ - vec * TownPlayerAction::walkSpeed;
            return;
        }
        dss::Fix32 len = vec.length();
        for (int i = 1; i < 8; i++) {
            dss::Fix32Vector3 tv1 = m_pos_array[i * 8 - 1] - m_pos_array[i * 8];
            tv1.normalize();
            temp[i] = m_pos_array[i * 8];
            m_pos_array[i * 8] += tv1 * len;
        }
        fixFlag_ = 1;
    } else if (moveAllPlayerToFirst(getDrawCount()) == 1) {
        moveFirstFlag_ = 0;
    }
}

static const dss::Fix32 unkLength(0.3f);

ARM void TownPartyAction::resetFixPos()
{
    if (fixFlag_ != 1) {
        return;
    }
    for (int i = 1; i < 8; i++) {
        setMemberPosition(i, temp[i]);
    }
}

ARM void TownPartyAction::setPosition()
{
    switch (moveType_) {
    case 0:
        normalMove();
        break;
    case 1:
    case 2:
        formationMove();
        break;
    }
    bool endFlag = true;
    if (changeAlpha_ != 1) {
        return;
    }
    int drawCount = TownPlayerManager::getSingleton()->partyDraw_.countReal_;
    for (int i = 0; i < drawCount; i++) {
        if (isEqalNextPos(i) == false && partyAlphaFlag_[i] == 0) {
            partyAlphaFlag_[i] = 1;
        }
        if (partyAlphaFlag_[i] == 1) {
            TownPlayerManager::getSingleton()->partyDraw_.addAlpha(i, 4);
        }
        if (TownPlayerManager::getSingleton()->partyDraw_.partyDispAlpha_[i] == 31) {
            partyAlphaFlag_[i] = 2;
        }
        if (partyAlphaFlag_[i] != 2) {
            endFlag = false;
        }
    }
    if (endFlag == true) {
        changeAlpha_ = 0;
    }
}

ARM void TownPartyAction::setAllPotition(dss::Fix32Vector3& pos)
{
    for (int i = 0; i < 128; i++) {
        m_pos_array[i] = pos;
        m_dir_array[i] = g_cmnPartyInfo.dirIdx_;
    }
}

ARM dss::Fix32Vector3 TownPartyAction::getMemberPosition(int index)
{
    return m_pos_array[index * 8];
}

ARM void TownPartyAction::setMemberPosition(int index, dss::Fix32Vector3& pos)
{
    m_pos_array[index * 8] = pos;
}

ARM short TownPartyAction::getMemberDirIdx(int index)
{
    return m_dir_array[index * 8];
}

ARM void TownPartyAction::setMemberDirIdx(int index, short dirIdx)
{
    m_dir_array[index * 8] = dirIdx;
}

ARM int TownPartyAction::getDrawCount()
{
    return TownPlayerManager::getSingleton()->partyDraw_.countReal_;
}

ARM void TownPartyAction::setPositionArrayPointer(dss::Fix32Vector3* pos)
{
    m_pos_array = pos;
}

ARM void TownPartyAction::setDirIdxArrayPointer(short* dir)
{
    m_dir_array = dir;
}

ARM bool TownPartyAction::isEqalNextPos(int index)
{
    bool ret = 0;
    if (index < 1) {
        return false;
    }
    if (index > 8) {
        return false;
    }
    if ((m_pos_array[index * 8] == m_pos_array[index * 8 - 1])) {
        ret = 1;
    }
    return ret;
}

ARM void TownPartyAction::setFormation(dss::Fix32Vector3& dirVec, short dirIdx, dss::Fix32 speed)
{
    fixFlag_ = 0;
    int count = getDrawCount();
    dss::Fix32Vector3 start[8];
    for (int i = 1; i < count; i++) {
        start[i] = getMemberPosition(i);
    }
    for (int i = 0; i < 128; i++) {
        m_pos_array[i] = g_cmnPartyInfo.position_ + dirVec * i;
        m_dir_array[i] = dirIdx;
    }
    if (speed != dss::Fix32(0L)) {
        dss::Fix32Vector3 end;
        for (int i = 1; i < count; i++) {
            end = getMemberPosition(i);
            partyMove_[i].setActionMove(start[i], end);
            partyMove_[i].setMoveSpeed(speed);
        }
        moveType_ = 2;
    } else {
        moveType_ = 1;
    }
    setFormation_ = 1;
}

ARM void TownPartyAction::formationMove()
{
    if (moveType_ == 1) {
        moveType_ = 0;
        return;
    }
    int count = getDrawCount();
    dss::Fix32Vector3 nowPos;
    dss::Fix32Vector3 nextPos;
    int ret = 1;
    dss::Vector3<short> angle;
    for (int i = 1; i < count; i++) {
        nowPos = getMemberPosition(i);
        nextPos = nowPos;
        angle.vy = getMemberDirIdx(i);
        partyMove_[i].execMove(nextPos);
        partyMove_[i].execRot(angle);
        dss::Fix32Vector3 dir = nextPos - nowPos;
        short idx = getMemberDirIdx(i);
        TownActionCalculate::getIdxByVec(idx, dir);
        setMemberPosition(i, nextPos);
        setMemberDirIdx(i, idx);
        if (partyMove_[i].moveUpdate() == 0) {
            ret = 0;
        } else {
            setMemberDirIdx(i, m_dir_array[i * 8 - 1]);
        }
    }
    if (ret == 1) {
        moveType_ = 0;
    }
}

ARM bool TownPartyAction::isFormationEnd()
{
    return moveType_ == 0;
}

ARM void TownPartyAction::setupPartyDelNotMoveFirst()
{
    for (int i = 0; i < 120; i++) {
        m_pos_array[i] = m_pos_array[i + 8];
        m_dir_array[i] = m_dir_array[i + 8];
    }
}

ARM void TownPartyAction::setMoveToFirstHalfSpeed(SPEED_TYPE flag)
{
    half_ = flag;
    switch (flag) {
    case SPEED_TYPE1:
        distanceCount_ = 2;
        break;
    case SPEED_TYPE2:
        distanceCount_ = 4;
        break;
    default:
        distanceCount_ = 2;
        break;
    }
}
