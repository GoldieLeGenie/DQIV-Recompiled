#pragma ipa file
#include "ov000/town/TownActionMoveToTarget.hpp"
#include "ov000/town/TownActionCalculate.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "main/dss/Camera.hpp"

TownActionMoveToTarget gMoveToTarget;
static const char addAlpha = 4;

ARM int TownActionMoveToTarget::setup()
{
    speed_.value = 0;
    target_.set(0, 0, 0);
    moveMode_ = MOVE_TO_TARGET;
    return -1;
}

ARM void TownActionMoveToTarget::execute()
{
    static const dss::Fix32 drawLen2(0xa3d);
    dss::Fix32Vector3 vec;
    if (moveMode_ == MOVE_TO_TARGET) {
        nowPos_ = nowPos_ + moveVec_;
        vec = target_ - nowPos_;
        if (vec.lengthsq() < speed_ * speed_) {
            if (partyMoveFlag_ == 1) {
                moveMode_ = MOVE_TO_FIRST;
                TownPlayerManager::getSingleton()->party_.moveFirstFlag_ = 1;
                partyMoveFlag_ = 0;
            } else {
                moveMode_ = MOVE_END;
            }
            nowPos_ = target_;
        } else if (vec.lengthsq() < drawLen2) {
            if (eraseFlag_ == 0) {
                eraseFlag_ = 1;
                count_ = 0;
            }
        }
    }
    switch (drawType_) {
    case 0:
        drawAllExec();
        break;
    case 1:
        drawEraseExec(1);
        break;
    case 2:
        drawEraseExec(0);
        break;
    }
    position_ = nowPos_;
}

ARM void TownActionMoveToTarget::setAction(dss::Fix32Vector3& startPos, dss::Fix32Vector3& target, dss::Fix32 speed, int moveFlag, int drawFlag, int nextAction)
{
    target_ = target;
    nowPos_ = startPos;
    speed_ = speed;
    moveVec_ = target_ - position_;
    moveVec_.normalize();
    moveVec_ = moveVec_ * speed;
    moveType_ = moveFlag;
    partyMoveFlag_ = moveFlag == 1;
    drawType_ = drawFlag;
    nextAction_ = nextAction;
    dss::Fix32Vector3 dir = target_ - position_;
    TownActionCalculate::getIdxByVec(dirIdx_, dir);
    moveMode_ = MOVE_TO_TARGET;
    eraseFlag_ = 0;
}

ARM int TownActionMoveToTarget::update()
{
    int ret = -1;
    if (moveMode_ == MOVE_END) {
        ret = nextAction_;
    } else if (moveMode_ == MOVE_TO_FIRST) {
        if (TownPlayerManager::getSingleton()->party_.moveFirstFlag_ == 0) {
            ret = nextAction_;
            moveMode_ = MOVE_END;
        }
    }
    if (ret != -1) {
        switch (drawType_) {
        case 0:
            TownPlayerManager::getSingleton()->partyDraw_.resetDrawPartyCount();
            break;
        case 1:
            TownPlayerManager::getSingleton()->partyDraw_.setDrawPartyOne();
            break;
        case 2:
            TownPlayerManager::getSingleton()->partyDraw_.setDrawPartyNone();
            break;
        }
    }
    count_++;
    return ret;
}

ARM void TownActionMoveToTarget::drawAllExec()
{
    int i;
    int count = TownPlayerManager::getSingleton()->partyDraw_.countReal_;
    for (i = 0; i < count; i++) {
        if (!TownPlayerManager::getSingleton()->party_.isEqalNextPos(i)) {
            if (TownPlayerManager::getSingleton()->partyDraw_.partyDispAlpha_[i] < 0x1f) {
                TownPlayerManager::getSingleton()->partyDraw_.addAlpha(i, addAlpha);
            }
        }
    }
}

ARM void TownActionMoveToTarget::drawEraseExec(int start)
{
    int count = TownPlayerManager::getSingleton()->partyDraw_.countReal_;
    if (eraseFlag_ != 1) {
        return;
    }
    for (int i = start; i < count; i++) {
        if (count_ - i * 8 >= 0) {
            TownPlayerManager::getSingleton()->partyDraw_.addAlpha(i, -addAlpha);
        }
    }
}
