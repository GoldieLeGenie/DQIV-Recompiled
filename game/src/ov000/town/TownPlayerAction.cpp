#pragma ipa file
#include "ov000/town/TownPlayerAction.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownActionBallonHorn.hpp"
#include "ov000/town/TownActionHenge.hpp"
#include "ov000/town/TownActionRuraFailed.hpp"
#include "ov000/town/TownActionWalk.hpp"
#include "ov000/town/TownDoorAction.hpp"
#include "ov000/town/TownCamera.hpp"
#include "ov000/town/TownCharacterManager.hpp"
#include "ov000/town/TownDamageFloor.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "ov000/Commands/TownCommand.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/cmn/ExtraMapLink.hpp"
#include "main/status/StageStatus.hpp"
#include "ov000/town/TownActionRura.hpp"
#include "ov000/town/TownIkadaAction2.hpp"
#include "ov000/town/TownKaidanAction2.hpp"
#include "ov000/town/TownRopeAction2.hpp"
#include "ov000/town/TownShipAction2.hpp"
#include "ov000/town/TownSubeAction.hpp"

const dss::Fix32 TownPlayerAction::collR(0x59a);
const dss::Fix32 TownPlayerAction::collRR = collR * collR;
const dss::Fix32 TownPlayerAction::coll2RR = collR * collR * 4;
const dss::Fix32 TownPlayerAction::walkSpeed(0x19a);
const dss::Fix32 TownPlayerAction::surfaceR(0x666);
const dss::Fix32 TownPlayerAction::rageSurfaceR(0x1800);
const dss::Fix32 TownPlayerAction::objectR(0x59a);
const dss::Fix32 TownPlayerAction::fixR(0x571);
const dss::Fix32 TownPlayerAction::ropeSpeed(0x19a);
const dss::Fix32 TownPlayerAction::fallH(0x1000);
const dss::Fix32 TownPlayerAction::bumpH(0x800);
const dss::Fix32 TownPlayerAction::kaidanR(0x99a);
const dss::Fix32 TownPlayerAction::shipR(0x159a);
const dss::Fix32 TownPlayerAction::shipCollR(0x1400);
const dss::Fix32 TownPlayerAction::getDownL(0x785);
const dss::Fix32 TownPlayerAction::shipSpeed(0x171);
const dss::Fix32 TownPlayerAction::ikadaR(0x948);
const dss::Fix32 TownPlayerAction::getOnOffSpeed(0x11f);
const dss::Fix32 TownPlayerAction::shipCtrLen(0x1ccd);
const dss::Fix32 TownPlayerAction::walkCtrLen(0x199a);
const dss::Fix32 TownPlayerAction::ruraSpeed(0x333);
const dss::Fix32 TownPlayerAction::talkR(0x1000);
const dss::Fix32 TownPlayerAction::townCharaR(0x666);
const dss::Fix32 TownPlayerAction::townCharaPreR(0x800);
const dss::Fix32 TownPlayerAction::changePreR(0x948);

ARM TownPlayerAction::TownPlayerAction()
{
}

ARM TownPlayerAction::~TownPlayerAction()
{
}

ARM void TownPlayerAction::setup()
{
    allShadowReset_ = 1;
    for (int i = 0; i < 16; i++) {
        action_[i] = NULL;
    }
    action_[ACTION_TYPE_WALK] = TownActionWalk::getSingleton();
    action_[ACTION_TYPE_KAIDAN] = TownKaidanAction2::getSingleton();
    action_[ACTION_TYPE_FALL] = TownFallAction::getSingleton();
    action_[ACTION_TYPE_SUBE] = TownSubeAction::getSingleton();
    action_[ACTION_TYPE_ROPE] = TownRopeAction2::getSingleton();
    action_[ACTION_TYPE_SHIP] = TownShipAction2::getSingleton();
    action_[ACTION_TYPE_IKADA] = TownIkadaAction2::getSingleton();
    action_[ACTION_TYPE_DOOR] = TownDoorAction::getSingleton();
    action_[ACTION_TYPE_RURA] = TownActionRura::getSingleton();
    action_[ACTION_TYPE_HENGE] = TownActionHenge::getSingleton();
    action_[ACTION_TYPE_RURA_FAILED] = TownActionRuraFailed::getSingleton();
    action_[ACTION_TYPE_BALLON_HORN] = TownActionBallonHorn::getSingleton();
    actionType_ = ACTION_TYPE_WALK;
    if (g_Stage.idoLink_.data_.link_.outFlag_ == 1 && g_Stage.idoLink_.data_.link_.inFlag_ == 1) {
        g_cmnPartyInfo.position_ = g_Stage.idoLink_.pos_;
        g_cmnPartyInfo.setDirIdx(g_Stage.idoLink_.data_.link_.dirIdx_);
        g_Stage.idoLink_.data_.link_.outFlag_ = 0;
        g_Stage.idoLink_.data_.link_.inFlag_ = 0;
    }
    int ret = action_[ACTION_TYPE_FALL]->setup();
    if (ret != -1) {
        actionType_ = (TOWN_PLAYER_ACTION_TYPE)ret;
    }
    ret = action_[ACTION_TYPE_ROPE]->setup();
    if (ret != -1) {
        actionType_ = (TOWN_PLAYER_ACTION_TYPE)ret;
    }
    if (g_Stage.ruraFlag_ == 3) {
        dss::Fix32Vector3 position;
        position.vx.value = 0;
        position.vy.value = dss::Fix32Vector3(g_cmnPartyInfo.startPos_).vy.value;
        position.vz.value = 0x4000;
        TownCamera::getSingleton()->camera_.unk_004.setTarget(position);
        position.vy.value = 0x5000;
        g_cmnPartyInfo.startPos_ = position;
        g_cmnPartyInfo.setDirIdx(0);
        TownCamera::getSingleton()->setCameraLock(true);
        TownPlayerManager::getSingleton()->setRemote(1);
        actionType_ = ACTION_TYPE_FALL;
        TownFallAction::getSingleton()->setCollFall();
        TownFallAction::getSingleton()->count_ = 3;
    }
    dss::Fix32Vector3 pos(g_cmnPartyInfo.startPos_);
    short dirIdx;
    if (cmn::g_extraMapLink.checkExtraTownPos(pos, dirIdx) == true) {
        g_cmnPartyInfo.startPos_ = pos;
        g_cmnPartyInfo.setDirIdx(dirIdx);
        actionType_ = ACTION_TYPE_WALK;
    }
    if (g_Stage.idoLink_.data_.link_.encount_ == 0) {
        g_cmnPartyInfo.setStartPosition();
        TownDamageFloor::getSingleton()->setup();
    }
    for (int i = 0; i < 16; i++) {
        if (i != ACTION_TYPE_FALL && i != ACTION_TYPE_ROPE && action_[i] != NULL) {
            int ret = action_[i]->setup();
            if (ret != -1) {
                actionType_ = (TOWN_PLAYER_ACTION_TYPE)ret;
            }
        }
    }
    switch (actionType_) {
    case ACTION_TYPE_SHIP:
        TownPlayerManager::getSingleton()->partyDraw_.setDrawPartyNone();
        break;
    case ACTION_TYPE_IKADA:
        TownPlayerManager::getSingleton()->partyDraw_.setDrawPartyOne();
        break;
    default:
        if (g_Stage.idoLink_.data_.link_.encount_ == 0 && g_cmnPartyInfo.prevLocation_ == 0) {
            TownPlayerManager::getSingleton()->setStartEraseParty();
        }
        break;
    }
    if (g_Stage.ruraFlag_ != 0) {
        TownFallAction::getSingleton()->sePlay_ = 0;
    }
    g_Stage.setRuraFlag(0);
    checkAbortPos();
}

ARM void TownPlayerAction::cleanup()
{
}

ARM void TownPlayerAction::checkAbortPos()
{
    static dss::Fix32Vector3 vec[4] = {
        dss::Fix32Vector3(0.4f, 0.0f, 0.0f),
        dss::Fix32Vector3(0.0f, 0.0f, 0.4f),
        dss::Fix32Vector3(-0.4f, 0.0f, 0.0f),
        dss::Fix32Vector3(0.0f, 0.0f, -0.4f),
    };
    dss::Fix32Vector3 pos = TownPlayerManager::getSingleton()->getPosition();
    if (TownCharacterManager::getSingleton()->checkAbortPlayerPos(pos) == true) {
        return;
    }
    dss::Fix32Vector3 target = pos;
    for (int i = 0; i < 4; i++) {
        target = pos + vec[i];
        dss::Fix32 retLen(0x2000);
        dss::Fix32 height;
        if (TownCharacterManager::getSingleton()->checkAbortPlayerPos(target) == true) {
            dss::Fix32Vector3 ret = TownStageManager::getSingleton()->compute(pos, target, collR, collR, collR, height);
            if (ret.vx == target.vx && ret.vz == target.vz) {
                ret.vy += height;
                TownPlayerManager::getSingleton()->setPosition(ret);
                return;
            }
        }
    }
}

ARM void TownPlayerAction::execute()
{
    action_[actionType_]->execute();
    int type = action_[actionType_]->update();
    if (type != -1) {
        actionType_ = (TOWN_PLAYER_ACTION_TYPE)type;
    }
}

ARM void TownPlayerAction::unkfunc_0213c9b0()
{
}

ARM void TownPlayerAction::unkfunc_0213c9b4()
{
}
