#pragma ipa file
#include "ov001/fld/FieldPlayerManager.hpp"
#include "ov001/fld/FieldStage.hpp"
#include "ov001/fld/FieldSystem.hpp"
#include "ov001/fld/FieldSymbolManager.hpp"
#include "ov001/fld/FieldPlayerDoku.hpp"
#include "ov001/fld/FieldActionCalculate.hpp"
#include "ov001/fld/FieldRectCollManager.hpp"
#include "ov001/window/FieldWindowSystem.hpp"
#include "main/dss/Pad.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/cmn/CommonRuraData.hpp"
#include "main/cmn/ExtraMapLink.hpp"
#include "main/cmn/HengeNoTsueManager.hpp"
#include "main/cmn/WorldLocation.hpp"
#include "main/encount/Encount.hpp"
#include "main/global/Global.hpp"
#include "main/global/StageLink.hpp"
#include "main/menu/UiMsg.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/StageStatus.hpp"

static const dss::Fix32 ruraFixY(0x64000);
static const dss::Fix32 ruraUpFixY(0xc8000);
static const dss::Fix32 symbolFixY(0x8000);
static const dss::Fix32 soraPosFixY(0x10000);
static const dss::Fix32 ruraPosFixY(0x8000);
static const dss::Fix32 fallSpeed(0x3000);
static const dss::Fix32 fieldRuraSpeed(0x4000);

ARM FieldPlayerManager::FieldPlayerManager()
{
}

ARM FieldPlayerManager* FieldPlayerManager::getSingleton()
{
    static FieldPlayerManager fieldPlayerManager;
    return &fieldPlayerManager;
}

ARM void FieldPlayerManager::initialize()
{
    locked_ = 0;
    party_.setPositionPointer(&position_);
    party_.setDirIdxPointer(&dirIdx_);
    dirIdx_ = 0;
    party_.setPositionArrayPointer(g_cmnPartyInfo.getPositionArrayPointer());
    party_.setDirIdxArrayPointer(g_cmnPartyInfo.getDirectionArrayPointer());
    player_.setPositionPointer(&position_);
    player_.setDirIdxPointer(&dirIdx_);
    eventEncount_ = 0;
    int index = StageLink::getFieldSymbolIndex();
    dss::Fix32Vector3 pos;
    if (index != -1) {
        pos = fld::FieldStage::getSingleton()->getSymbolPosition(index);
    }
    g_cmnPartyInfo.checkBallon(&pos);
    partyDraw_.setup();
    shipDraw_.setup();
    balloonDraw_.setup();
    shipDraw_.setPosition(g_Stage.shipPosition_);
    balloonDraw_.setPosition(g_Stage.balloonPosition_);
    position_.vx = 0x1a8L;
    position_.vy = 0x127L;
    scriptMoveFlag_ = 0;
    SpriteCharacter::setAllCharaAnim(1);
    g_cmnPartyInfo.partyTalk = 1;
}

ARM void FieldPlayerManager::terminate()
{
    if (player_.getMoveType() == FieldPlayer::MOVE_RURA_END) {
        int townId = g_Stage.getRuraTownID();
        if (townId != -1) {
            g_Stage.balloonPosition_ = cmn::CommonRuraData::getSingleton()->getBalloonTownPos(townId);
            g_Stage.shipPosition_ = cmn::CommonRuraData::getSingleton()->getShipTownPos(townId);
            g_cmnPartyInfo.setBalloonFieldTypeByTownId(townId);
        }
    } else {
        g_Stage.shipPosition_ = dss::Fix32Vector3(shipDraw_.getPosition());
        g_Stage.balloonPosition_ = dss::Fix32Vector3(balloonDraw_.getPosition());
    }
    partyDraw_.cleanup();
    shipDraw_.cleanup();
    balloonDraw_.cleanup();
}

ARM void FieldPlayerManager::execute()
{
    if (encount::Encount::getSingleton()->isEncounted()) {
        return;
    }
    if (g_Global.partChangeFlag_ == 1) {
        return;
    }
    if (mapChangeCounter_ != -1) {
        if (mapChangeCounter_ == 0) {
            setLock(0);
        }
        flagMapLink_ = 1;
        mapChangeCounter_--;
    }
    dss::Fix32Vector3 prevPos(position_);
    switch (g_cmnPartyInfo.menuAction_) {
    case cmn::MENU_HENGE_NO_TSUE:
        setLock(1);
        g_HengeNoTsue.setNextAction(player_.getMoveType());
        g_cmnPartyInfo.setMenuAction(cmn::MENU_ACTION_NONE);
        player_.setMoveType(FieldPlayer::HENGE_START);
        hengeCounter_ = 0;
        SoundManager::playSe(0x169, 0);
        break;
    case cmn::BALLON_HORN:
        setBallonWhistle();
        player_.setMoveType(FieldPlayer::BALLON_HORN);
        g_cmnPartyInfo.setMenuAction(cmn::MENU_ACTION_NONE);
        break;
    }
    if (g_Stage.menuSearch_ == 1) {
        setPlayerCommand(START_SEARCH_COMMAND);
        if (!player_.playerSearch()) {
            if (player_.getMoveType() == FieldPlayer::MOVE_SHIP) {
                ui_MsgSndSet(0x30);
                FieldWindowSystem::getSingleton()->openCommonMessage();
                FieldWindowSystem::getSingleton()->addCommonMessage(0xc40c9);
                FieldWindowSystem::getSingleton()->addCommonMessage(0xc40cf);
            } else {
                ui_MsgSndSet(0x30);
                FieldWindowSystem::getSingleton()->openCommonMessage();
                FieldWindowSystem::getSingleton()->addCommonMessage(0xc40ba);
                FieldWindowSystem::getSingleton()->addCommonMessage(0xc40cf);
            }
        }
        setLock(1);
    }
    if (g_Stage.menuTalk_ == 1) {
        FieldWindowSystem::getSingleton()->cmdWindow_.changeNormalPhase();
        setPlayerCommand(START_TALK_COMMAND);
        ui_MsgSndSet(0x30);
        FieldWindowSystem::getSingleton()->openCommonMessage();
        FieldWindowSystem::getSingleton()->addCommonMessage(0xc3d97);
        setLock(1);
    }
    if (g_Stage.ruraFlag_ == 1) {
        setStartRura();
    }
    normalExec();
    everyExec();
    switch (player_.getMoveType()) {
    case FieldPlayer::MOVE_WALK:
        walkExec();
        break;
    case FieldPlayer::MOVE_SHIP:
        shipExec();
        break;
    case FieldPlayer::MOVE_SHIP_GET_ON:
        getOnShip();
        break;
    case FieldPlayer::MOVE_SHIP_GET_OUT:
        getDownShip();
        break;
    case FieldPlayer::MOVE_BALLOON_GET_ON_WALK:
        rideBalloonWalk();
        break;
    case FieldPlayer::MOVE_BALLOON_GET_ON:
        rideBalloon();
        break;
    case FieldPlayer::MOVE_BALLOON:
        balloonExec();
        break;
    case FieldPlayer::MOVE_BALLOON_GET_OUT:
        downBalloon();
        break;
    case FieldPlayer::MOVE_RURA_DOWN:
    case FieldPlayer::MOVE_SORATOBU:
    case FieldPlayer::MOVE_SORATOBU_END:
        ruraDownExec();
        break;
    case FieldPlayer::MOVE_RURA_UP:
        ruraUpExec();
        break;
    case FieldPlayer::HENGE_START:
        hengeStartExec();
        break;
    case FieldPlayer::HENGE_RESET:
        hengeResetExec();
        break;
    case FieldPlayer::BALLON_HORN:
        ballonWhistle();
        break;
    }
    if (g_Stage.menuSearch_ == 1) {
        g_Stage.menuSearch_ = 0;
        setLock(0);
    }
    if (g_Stage.menuTalk_ == 1) {
        setLock(0);
        g_Stage.menuTalk_ = 0;
    }
    if (isEncountLock() == true) {
        FieldPlayerDoku::getSingleton()->damageFlag_ = 0;
        FieldPlayerDoku::getSingleton()->effectFlag_ = 0;
        FieldPlayerDoku::getSingleton()->encountFlag_ = 0;
    } else {
        FieldPlayerDoku::getSingleton()->damageFlag_ = 1;
        FieldPlayerDoku::getSingleton()->effectFlag_ = 1;
        FieldPlayerDoku::getSingleton()->encountFlag_ = 1;
    }
    FieldPlayerDoku::getSingleton()->checkDokuDamage(prevPos, position_);
    if (!isLock()) {
        getPlayerCommand();
    }
    setPlayerCommand(PUSH_NONE);
    g_cmnPartyInfo.setPartyInfo(&position_, dirIdx_);
}

ARM void FieldPlayerManager::draw()
{
    dss::Fix32Vector3 pos = getDrawPosition();
    dss::Vector2<int> pos2 = fld::FieldStage::getSingleton()->calcDrawPosition(pos);
    partyDraw_.setDepth(0, pos.vy.value);
    partyDraw_.setPosition(0, fld::FieldStage::getSingleton()->calcDrawPosition(pos));
    partyDraw_.setRotate(0, dirIdx_);
    for (int i = 1; i < partyDraw_.countReal_; i++) {
        pos = party_.getMemberPosition(i);
        pos2 = fld::FieldStage::getSingleton()->calcDrawPosition(pos);
        partyDraw_.setPosition(i, pos2);
        partyDraw_.setDepth(i, pos.vy.value);
        partyDraw_.setRotate(i, (unsigned short)party_.getMemberDirIdx(i));
    }
    partyDraw_.draw();
}

ARM void FieldPlayerManager::walkExec()
{
    dss::Fix32Vector3 pos;
    party_.setPosition(player_.getPosition());
    party_.setDirIdx(dirIdx_);
    party_.execute();
    dirIdx_ = *player_.dirIdx_;
    partyDraw_.setRotate(0, dirIdx_);
    execMapLink();
}

ARM void FieldPlayerManager::getOnShip()
{
    if (!moveToTarget()) {
        player_.setPosition(position_);
        party_.setPosition(position_);
        party_.setDirIdx(dirIdx_);
        party_.execute();
    } else {
        int count = partyDraw_.countReal_;
        if (party_.moveAllPlayerToFirst(count) == true) {
            shipDraw_.setPosition(targetPos_);
            player_.setPosition(position_);
            player_.setMoveType(FieldPlayer::MOVE_SHIP);
            partyDraw_.setDrawNone();
            shipDraw_.ride_ = 1;
            shipDraw_.setRotate(dirIdx_);
            setLock(0);
            g_cmnPartyInfo.rideOnType_ = cmn::RIDE_ON_SHIP_IKADA;
            SoundManager::playBgm(0x19, 0);
            fld::FieldStage::getSingleton()->fieldData.pause_ = 0;
        }
    }
}

ARM void FieldPlayerManager::getDownShip()
{
    int count = partyDraw_.countReal_;
    if (!moveToTarget()) {
        player_.setPosition(position_);
        party_.setPosition(position_);
        party_.setDirIdx(dirIdx_);
        party_.moveAllPlayerToFirst(count);
    } else {
        shipDraw_.ride_ = 0;
        if (party_.moveAllPlayerToFirst(count) == true) {
            player_.setMoveType(FieldPlayer::MOVE_WALK);
            g_cmnPartyInfo.rideOnType_ = cmn::RIDE_ON_NONE;
            SoundManager::fieldPlay();
            setLock(0);
            fld::FieldStage::getSingleton()->fieldData.pause_ = 0;
        }
    }
}

ARM void FieldPlayerManager::ruraDownExec()
{
    int count = partyDraw_.countReal_;
    position_ += drawRuraOffset_;
    dss::Fix32Vector3 oldPos(position_);
    if (!moveToTarget()) {
        drawRuraOffset_ += position_ - oldPos;
        party_.setPosition(getDrawPosition());
        party_.setDirIdx(dirIdx_);
        party_.moveAllPlayerToFirst(count);
    } else {
        partyDraw_.partyCharacter_[0].setShadowFlag(1);
        if (player_.getMoveType() == FieldPlayer::MOVE_SORATOBU) {
            player_.setMoveType(FieldPlayer::MOVE_SORATOBU_END);
            g_Stage.setRuraFlag(3);
            StageLink::resetTownExitIndex();
            g_Stage.symbolID_ = 0x12;
            g_Global.startTown(cmn::CommonRuraData::getSingleton()->get_TATOP_Name());
            g_Stage.flagMapChange_ = 1;
            player_.setMoveType(FieldPlayer::MOVE_WALK);
            setLock(1);
        }
        if (party_.moveAllPlayerToFirst(count) == true && player_.getMoveType() == FieldPlayer::MOVE_RURA_DOWN) {
            for (int i = 0; i < partyDraw_.countReal_; i++) {
                partyDraw_.partyCharacter_[i].setShadowFlag(1);
            }
            player_.setMoveType(FieldPlayer::MOVE_WALK);
            player_.setWait(2);
            flagMapLink_ = 0;
            party_.setBashaArray(status::g_Party.bashaEnable_);
            FieldSystem::getSingleton()->cameraLock_ = 0;
            setLock(0);
        }
    }
    position_ = ruraPos_;
}

ARM void FieldPlayerManager::shipExec()
{
    position_ = player_.getPosition();
    dirIdx_ = *player_.dirIdx_;
    party_.setDirIdx(dirIdx_);
    party_.setPosition(position_);
    setPartyToFirst();
    shipDraw_.setPosition(position_);
    shipDraw_.setRotate(dirIdx_);
    g_Stage.shipPosition_ = dss::Fix32Vector3(position_);
    execMapLink();
}

ARM void FieldPlayerManager::setup()
{
    flagMapLink_ = 0;
    int index = StageLink::getFieldSymbolIndex();
    if (index != -1) {
        dss::Fix32Vector3 pos;
        pos = fld::FieldStage::getSingleton()->getSymbolPosition(index);
        fld::FieldStage::getSingleton()->setSymbolFlag(index & 0xfff);
        position_.vx = pos.vx;
        position_.vy = pos.vy;
        player_.setup();
    }
    flagMapLink_ = 1;
    cmn::g_extraMapLink.setExtraFieldPos(position_, dirIdx_);
    dirIdx_ = DIR_8_DD;
    player_.setPosition(position_);
    if (g_Stage.idoLink_.data_.link_.shipEncount_ == 1 || g_cmnPartyInfo.rideOnType_ == cmn::RIDE_ON_SHIP_IKADA) {
        player_.setMoveType(FieldPlayer::MOVE_SHIP);
        shipDraw_.ride_ = 1;
        partyDraw_.setDrawNone();
    } else if (g_cmnPartyInfo.rideOnType_ == cmn::RIDE_ON_TOWN_BALLOON) {
        if (g_Global.getFieldType() != 1) {
            setBalloonPos(position_);
            setScriptBalloon(0);
            partyDraw_.setDrawNone();
        } else {
            setBalloonPos(position_);
            setScriptBalloon(2);
            partyDraw_.setDrawNone();
        }
    }
    dss::Fix32Vector3 target;
    int link_flag = g_Stage.ruraFlag_;
    if (link_flag != 0) {
        int symbol = StageLink::getFieldSymbolIndex();
        target = fld::FieldStage::getSingleton()->getSymbolPosition(symbol);
        if (link_flag == 2) {
            player_.setMoveType(FieldPlayer::MOVE_RURA_DOWN);
            target.vy += ruraPosFixY;
        } else if (link_flag == 3) {
            player_.setMoveType(FieldPlayer::MOVE_SORATOBU);
            target.vy -= soraPosFixY;
        }
        for (int i = 0; i < partyDraw_.countReal_; i++) {
            partyDraw_.partyCharacter_[i].setShadowFlag(0);
        }
        targetPos_ = target;
        ruraPos_ = target;
        drawRuraOffset_.set(0, 0, 0);
        drawRuraOffset_.vy -= ruraFixY;
        FieldSystem::getSingleton()->setLookAtPos(target);
        FieldSystem::getSingleton()->cameraLock_ = 1;
        target.vy -= ruraFixY;
        position_ = target;
        speedToTarget_ = dss::Fix32(fallSpeed);
        setLock(1);
        shipDraw_.setPosition(g_Stage.shipPosition_);
        balloonDraw_.setPosition(g_Stage.balloonPosition_);
    }
    party_.setup();
    if (g_Stage.idoLink_.data_.link_.encount_ == 1) {
        locked_ = g_Stage.playerLockCount_;
        setLock(0);
    } else {
        FieldPlayerDoku::getSingleton()->setup();
    }
    g_Stage.idoLink_.data_.link_.shipEncount_ = 0;
    g_Stage.idoLink_.data_.link_.encount_ = 0;
    g_Stage.setRuraFlag(0);
    setPlayerCommand(PUSH_NONE);
    g_cmnPartyInfo.setPartyInfo(&position_, dirIdx_);
    g_cmnPartyInfo.prevLocation_ = 0;
    mapChangeCounter_ = 10;
    setLock(1);
    if (g_Global.isAreaChange() == true) {
        g_cmnPartyInfo.resetShipIkadaMapName();
    }
}

ARM void FieldPlayerManager::cleanup()
{
    player_.cleanup();
    party_.cleanup();
}

ARM void FieldPlayerManager::everyExec()
{
}

ARM void FieldPlayerManager::normalExec()
{
    if (isLock()) {
        if (scriptMoveFlag_ == 1) {
            dss::Fix32Vector3 nextPos(position_);
            scriptMove_.execMove(nextPos);
            if (scriptMove_.moveUpdate() == 1) {
                scriptMoveFlag_ = 0;
            }
            if (position_ != nextPos) {
                dss::Fix32Vector3 vec = nextPos - position_;
                dirIdx_ = FieldActionCalculate::getDir8ByVector3(vec);
            }
            position_ = nextPos;
            cmn::WorldLocation::calcWorldPos(&position_.vx.value, &position_.vy.value);
        }
    } else {
        checkCommandEnd();
        if (player_.getMoveType() == FieldPlayer::MOVE_BALLOON) {
            if (dss::g_Pad.edge() & 2) {
                setPlayerCommand(PUSH_BALLOON_GETOUT_BUTTON);
            }
            if (dss::g_Pad.edge() & 0x400) {
                setPlayerCommand(PUSH_BALLOON_SEARCH_BUTTON);
            }
        }
        if (dss::g_Pad.edge() & 1) {
            setPlayerCommand(PUSH_BENRI_BUTTON);
        }
        dss::Fix32Vector3 nowPos(position_);
        player_.execute();
        position_ = player_.getPosition();
        if (nowPos != position_) {
            FieldSymbolManager::getSingleton()->resetFlag_ = 1;
        }
        if (g_Global.getFieldType() == 0) {
            int x = position_.vx.value / 16 / FX32_ONE;
            int y = position_.vy.value / 16 / FX32_ONE;
            g_Stage.setMapVeil(x, y, 0);
        }
    }
}

ARM void FieldPlayerManager::execMapLink()
{
    dss::Fix32Vector3 pos = getSingleton()->getPosition();
    int index = fld::FieldStage::getSingleton()->getSearchSymbolAttach(pos);
    if (index == -1) {
        if (flagMapLink_ == 1) {
            if (cmn::g_extraMapLink.checkFieldRectLinkNo(pos) == -1) {
                flagMapLink_ = 0;
            }
            return;
        }
        int id = cmn::g_extraMapLink.checkFieldRectLinkByType(pos, cmn::RECT_FIELD_TO_TOWN);
        if (id != 0) {
            FieldSystem::getSingleton()->exitSound_ = 1;
            g_Stage.symbolID_ = id;
            flagMapLink_ = 1;
        }
        return;
    }
    if (flagMapLink_ != 0) {
        return;
    }
    int ret = cmn::g_extraMapLink.checkFieldLink(index);
    if (ret == cmn::NOT_LINK_THIS_TOWN) {
        return;
    }
    if (ret == cmn::LINK_FIELD_TO_TOWN) {
        FieldSystem::getSingleton()->exitSound_ = 1;
        flagMapLink_ = 1;
        return;
    }
    char* name = StageLink::getName("world", index);
    if (name != NULL) {
        fld::FieldStage::getSingleton()->setSymbolFlag(index);
        int id = StageLink::getSymbolIndex();
        StageLink::setTownExitIndex(id);
        g_Stage.symbolID_ = index;
        g_Global.startTown(name);
        getSingleton()->setLock(1);
        FieldSystem::getSingleton()->exitSound_ = 1;
        flagMapLink_ = 1;
    }
}

ARM void FieldPlayerManager::setPosition(dss::Fix32Vector3& pos)
{
    position_ = pos;
    player_.setPosition(position_);
}

ARM void FieldPlayerManager::inputPad(int padDir)
{
    player_.inputPad(padDir);
}

ARM void FieldPlayerManager::inputClear()
{
    player_.inputClear();
}

ARM void FieldPlayerManager::resetParty()
{
    if (partyDraw_.countReal_ != 0) {
        partyDraw_.cleanup();
        partyDraw_.setup();
        int count = partyDraw_.countReal_;
        for (int i = 0; i < count; i++) {
            dss::Fix32Vector3 pos = party_.getMemberPosition(i);
            fld::FieldStage::getSingleton()->calcDrawPosition(pos);
            partyDraw_.setDepth(0, pos.vy.value);
            partyDraw_.setPosition(0, fld::FieldStage::getSingleton()->calcDrawPosition(pos));
            partyDraw_.setRotate(0, party_.getMemberDirIdx(i));
        }
    }
    if (player_.getMoveType() == FieldPlayer::MOVE_SHIP) {
        partyDraw_.setDrawNone();
    }
}

ARM LandType FieldPlayerManager::getLandType()
{
    switch (player_.getAttr()) {
    case WMAP_ATTR_NOT:
        return Floor;
    case WMAP_ATTR_SOUG:
        return Field;
    case WMAP_ATTR_SIGE:
        return Bush;
    case WMAP_ATTR_SUNA:
        return Desert;
    case WMAP_ATTR_MORI:
        return Forest;
    case WMAP_ATTR_YAMA1:
        return Mountain;
    case WMAP_ATTR_YAMA2:
        return Mountain;
    case WMAP_ATTR_KAIG:
        return Field;
    case WMAP_ATTR_UMI:
        return Sea;
    case WMAP_ATTR_ASAS:
        return Sea;
    case WMAP_ATTR_DOKU:
        return Pond;
    }
    return Floor;
}

ARM FieldCarrirerDraw* FieldPlayerManager::getCarrierPos(int type)
{
    if (type == FieldCarrirerDraw::CARRIER_SHIP) {
        return &shipDraw_;
    }
    return &balloonDraw_;
}

// unreferenced in the ROM: its only user was dead-stripped by the linker
static int s_unk_021614cc;
static int balloonAnim;

ARM void FieldPlayerManager::rideBalloon()
{
    if (balloonAnim == 0) {
        balloonDraw_.setDepth(6);
    }
    balloonDraw_.setHigh(balloonAnim / 2);
    if (balloonAnim >= 64) {
        if (g_Global.getFieldType() == 1) {
            dss::Fix32Vector3 pos;
            pos.vx.value = 0x898000;
            pos.vy.value = 0x898000;
            cmn::g_extraMapLink.setExtraLinkFieldAbsPos(0, pos, 4);
            g_cmnPartyInfo.rideOnType_ = cmn::RIDE_ON_TOWN_BALLOON;
            g_Stage.balloonFieldType_ = 0;
            player_.setMoveType(FieldPlayer::MOVE_BALLOON);
        } else {
            player_.setMoveType(FieldPlayer::MOVE_BALLOON);
            g_cmnPartyInfo.rideOnType_ = cmn::RIDE_ON_TOWN_BALLOON;
            fld::FieldStage::getSingleton()->fieldData.pause_ = 0;
            setLock(0);
        }
    } else {
        fld::FieldStage::getSingleton()->setOffset(balloonAnim / 2);
        balloonAnim++;
    }
}

ARM void FieldPlayerManager::downBalloon()
{
    if (balloonAnim >= 64) {
        setLock(1);
    }
    if (balloonAnim == 0) {
        partyDraw_.resetDrawCount();
        g_cmnPartyInfo.rideOnType_ = cmn::RIDE_ON_NONE;
        dirIdx_ = DIR_8_DD;
        *player_.dirIdx_ = DIR_8_DD;
        party_.setPosition(player_.getPosition());
        party_.setDirIdx(DIR_8_DD);
        party_.setAllPlayerAtFirst();
        balloonDraw_.setDepth(3);
    } else if (balloonAnim == -2) {
        SoundManager::fieldPlay();
        player_.setMoveType(FieldPlayer::MOVE_WALK);
        FieldWindowSystem::getSingleton()->setMenuPermit(true);
        g_Stage.balloonPosition_ = dss::Fix32Vector3(position_);
        SoundManager::fieldPlay();
        setLock(0);
        balloonAnim = 0;
        fld::FieldStage::getSingleton()->fieldData.pause_ = 0;
        return;
    } else if (balloonAnim == 30) {
        SoundManager::stopBgm(30);
    }
    if (balloonAnim >= 0) {
        balloonDraw_.setHigh(balloonAnim / 2);
        fld::FieldStage::getSingleton()->setOffset(balloonAnim / 2);
    }
    balloonAnim--;
}

ARM void FieldPlayerManager::balloonExec()
{
    position_ = player_.getPosition();
    dirIdx_ = *player_.dirIdx_;
    balloonDraw_.setPosition(position_);
    fld::FieldStage::getSingleton()->setOffset(balloonAnim / 2);
}

ARM void FieldPlayerManager::rideBalloonWalk()
{
    static int frame;
    static dss::Fix32Vector3 vec;
    static dss::Fix32 speed(1.2f);
    dss::Fix32Vector3 balPos(balloonDraw_.getPosition());
    dss::Fix32Vector3 pos;
    if (frame == 0) {
        SoundManager::stopBgm(30);
        pos.vx = balPos.vx;
        pos.vy = balPos.vy;
        pos.vz = position_.vz;
        vec = pos - position_;
        if (vec.length() >= speed * 3) {
            short idx = FieldActionCalculate::getDir8ByVector3(vec);
            *player_.dirIdx_ = idx;
            dirIdx_ = idx;
            partyDraw_.setRotate(0, dirIdx_);
            targetPos_ = pos;
        } else {
            targetPos_ = position_;
        }
        speedToTarget_ = dss::Fix32(speed);
        frame++;
    }
    if (!moveToTarget()) {
        player_.setPosition(position_);
        party_.setPosition(position_);
        party_.setDirIdx(dirIdx_);
        party_.execute();
    } else {
        int count = partyDraw_.countReal_;
        if (party_.moveAllPlayerToFirst(count) == true) {
            position_ = balloonDraw_.getPosition();
            SoundManager::playBgm(0x1c, 0);
            player_.setMoveType(FieldPlayer::MOVE_BALLOON_GET_ON);
            frame = 0;
            partyDraw_.setDrawNone();
        }
    }
}

ARM bool FieldPlayerManager::moveToTarget()
{
    bool ret = false;
    dss::Fix32Vector3 vec;
    vec = targetPos_ - position_;
    if (vec.length().value > unkfunc_02031e84(speedToTarget_.value)) {
        vec.normalize();
        vec *= speedToTarget_;
        position_.vx += vec.vx;
        position_.vy += vec.vy;
    } else {
        position_.vx = targetPos_.vx;
        position_.vy = targetPos_.vy;
        ret = true;
    }
    return ret;
}

ARM void FieldPlayerManager::savePartyDrawInfo()
{
    party_.savePartyDrawInfo();
}

ARM void FieldPlayerManager::setStartRura()
{
    g_Stage.setRuraFlag(2);
    short prevDir_ = dirIdx_;
    dss::Fix32Vector3 target;
    target = position_;
    target.vy -= ruraUpFixY;
    targetPos_ = target;
    FieldSystem::getSingleton()->setLookAtPos(position_);
    FieldSystem::getSingleton()->cameraLock_ = 1;
    player_.setMoveType(FieldPlayer::MOVE_RURA_UP);
    speedToTarget_ = dss::Fix32(fieldRuraSpeed);
    g_cmnPartyInfo.rideOnType_ = cmn::RIDE_ON_NONE;
    dirIdx_ = prevDir_;
    drawRuraOffset_.set(0, 0, 0);
    ruraPos_ = position_;
    SoundManager::playSe(0x23b, 0);
}

ARM void FieldPlayerManager::ruraUpExec()
{
    int i;
    int count = partyDraw_.countReal_;
    position_ += drawRuraOffset_;
    dss::Fix32Vector3 oldPos(position_);
    if (!moveToTarget()) {
        drawRuraOffset_ += position_ - oldPos;
        party_.setPosition(getDrawPosition());
        party_.setDirIdx(dirIdx_);
        party_.moveAllPlayerToFirst(count);
        for (i = 0; i < partyDraw_.countReal_; i++) {
            if (position_.vx == party_.getMemberPosition(i).vx && position_.vy == party_.getMemberPosition(i).vy) {
                partyDraw_.partyCharacter_[i].setShadowFlag(0);
            }
        }
    } else if (party_.moveAllPlayerToFirst(count) == true) {
        cmn::g_extraMapLink.setRuraLink();
        g_cmnPartyInfo.rideOnType_ = cmn::RIDE_ON_NONE;
        player_.setMoveType(FieldPlayer::MOVE_RURA_END);
    }
    for (int i = 0; i < count; i++) {
        dss::Fix32Vector3 pos = party_.getMemberPosition(i);
        if (position_.vx == pos.vx) {
            partyDraw_.partyCharacter_[i].setShadowFlag(0);
        }
    }
    position_ = ruraPos_;
}

ARM int FieldPlayerManager::getDamageColor(int type)
{
    switch (type) {
    case 1:
        return FieldPartyDraw::colorDoku;
    case 0:
        return FieldPartyDraw::colorBarrier;
    case 2:
        return 0x7fff;
    }
    return 0x7fff;
}

ARM bool FieldPlayerManager::isEncountLock()
{
    if (isLock() == true || player_.getMoveType() == FieldPlayer::MOVE_BALLOON) {
        return true;
    }
    return false;
}

ARM void FieldPlayerManager::setScriptBalloon(int flag)
{
    switch (flag) {
    case 0:
        balloonAnim = 64;
        player_.setMoveType(FieldPlayer::MOVE_BALLOON);
        balloonDraw_.setHigh(balloonAnim / 2);
        fld::FieldStage::getSingleton()->setOffset(balloonAnim / 2);
        break;
    case 1:
        setLock(1);
        balloonAnim = 0;
        player_.setMoveType(FieldPlayer::MOVE_BALLOON_GET_ON);
        break;
    case 2:
        balloonAnim = 64;
        balloonDraw_.setHigh(balloonAnim / 2);
        fld::FieldStage::getSingleton()->setOffset(balloonAnim / 2);
        player_.setMoveType(FieldPlayer::MOVE_BALLOON_GET_OUT);
        break;
    }
}

ARM void FieldPlayerManager::setSimpleMove(dss::Fix32Vector3 target, dss::Fix32 rate, int flag)
{
    target = (flag == 1) ? target + position_ : target;
    scriptMove_.setActionMove(position_, target);
    dss::Fix32 speed = rate * FieldPlayer::Speed;
    scriptMove_.setMoveSpeed(speed);
    scriptMoveFlag_ = 1;
}

ARM void FieldPlayerManager::setDirectionMove(dss::Fix32 target, int dir)
{
    scriptMoveFlag_ = 1;
    static const dss::Fix32 speed(0x1800);
    scriptMove_.setDirMove(target, dir, speed);
}

ARM void FieldPlayerManager::setScriptGetDownShip(int dir)
{
    dss::Fix32Vector3 vec;
    FieldActionCalculate::getVecByScriptParam4(vec, dir);
    dss::Fix32Vector3 target = position_ + vec * 20;
    getSingleton()->targetPos_ = target;
    getSingleton()->speedToTarget_ = dss::Fix32(FieldPlayer::Speed);
    player_.setMoveType(FieldPlayer::MOVE_SHIP_GET_OUT);
    getSingleton()->setLock(1);
    getSingleton()->partyDraw_.resetDrawCount();
    dirIdx_ = FieldActionCalculate::getIdxByParam4((short)dir);
    getSingleton()->setPartyToFirst();
}

ARM bool FieldPlayerManager::isEndScriptGetDownShip()
{
    return player_.getMoveType() == FieldPlayer::MOVE_WALK;
}

ARM void FieldPlayerManager::hengeStartExec()
{
    if (g_cmnPartyInfo.rideOnType_ != cmn::RIDE_ON_SHIP_IKADA) {
        if (hengeCounter_ % 4 < 2) {
            partyDraw_.setHengeDrawNone();
        } else {
            partyDraw_.resetDrawCount();
        }
    }
    if (hengeCounter_ >= 60) {
        player_.setMoveType(g_HengeNoTsue.getNextAction());
        setLock(0);
        g_HengeNoTsue.getChangeCharaNo();
        g_HengeNoTsue.setCounter();
        resetParty();
        hengeCounter_ = 0;
    }
    hengeCounter_++;
}

ARM void FieldPlayerManager::hengeResetExec()
{
    if (hengeCounter_ % 4 < 2) {
        partyDraw_.setHengeDrawNone();
    } else {
        partyDraw_.resetDrawCount();
    }
    if (hengeCounter_ >= 60) {
        player_.setMoveType(FieldPlayer::MOVE_WALK);
        setLock(0);
        partyDraw_.cleanup();
        partyDraw_.setup();
        hengeCounter_ = 0;
    }
    hengeCounter_++;
}

ARM dss::Fix32Vector3 FieldPlayerManager::getDrawPosition()
{
    switch (player_.getMoveType()) {
    case FieldPlayer::MOVE_RURA_UP:
    case FieldPlayer::MOVE_RURA_DOWN:
    case FieldPlayer::MOVE_RURA_END:
    case FieldPlayer::MOVE_SORATOBU:
    case FieldPlayer::MOVE_SORATOBU_END:
        return ruraPos_ + drawRuraOffset_;
    }
    return position_;
}

ARM bool FieldPlayerManager::checkBarronArea(dss::Fix32Vector3& pos)
{
    return FieldRectCollManager::getSingleton()->checkTypeColl(pos, RECT_BALLON);
}

ARM void FieldPlayerManager::ballonWhistle()
{
    if (ballonCounter_ == 360) {
        if (g_cmnPartyInfo.isBarronArea(&position_) && status::g_Party.basha_ == 0) {
            g_Global.fadeOutWhite(20);
            ballonType_ = WHITE_OUT;
        } else {
            setLock(0);
            player_.setMoveType(prevAction_);
            ui_MsgSndSet(0x30);
            FieldWindowSystem::getSingleton()->openCommonMessage();
            FieldWindowSystem::getSingleton()->addCommonMessage(0xc3d6d);
            SoundManager::fieldPlay();
        }
    } else if (ballonCounter_ > 360) {
        if (g_GlobalFade.isFadeEnd() == 1) {
            if (ballonType_ == WHITE_OUT) {
                ballonType_ = WHITE_IN;
                g_Global.fadeInWhite(20);
                g_cmnPartyInfo.callCarriage();
                setPartyToFirst();
            } else {
                setLock(0);
                player_.setMoveType(prevAction_);
                SoundManager::fieldPlay();
            }
        }
    }
    ballonCounter_++;
}

ARM void FieldPlayerManager::setBallonWhistle()
{
    ballonCounter_ = 0;
    SoundManager::play(0x32, 0xf);
    setLock(1);
    prevAction_ = player_.getMoveType();
}

ARM void FieldPlayerManager::setLockByEventEncount(int flag)
{
    eventEncount_ = flag;
    setLock(flag);
}

ARM void FieldPlayerManager::resetLockByEventEncount()
{
    if (eventEncount_ == 1) {
        setLock(0);
        eventEncount_ = 0;
    }
}

ARM void FieldPlayerManager::setCarrierDepth()
{
    if (balloonDraw_.getPosition().vy < shipDraw_.getPosition().vy) {
        balloonDraw_.setDepth(2);
        balloonDraw_.carrier_->shadow_.unk_28 = 2;
        shipDraw_.setDepth(3);
    } else {
        balloonDraw_.setDepth(3);
        balloonDraw_.carrier_->shadow_.unk_28 = 3;
        shipDraw_.setDepth(2);
    }
}

// stand-in for the dead-stripped user of s_unk_021614cc (keeps it in the .bss layout)
ARM int unkfunc_021614cc()
{
    return s_unk_021614cc;
}
