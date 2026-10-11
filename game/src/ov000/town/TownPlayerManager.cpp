#pragma ipa file
#include "ov000/town/TownPlayerManager.hpp"
#include "main/dss/Pad.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/object/DisplayCharacter.hpp"
#include "ov000/town/TownActionBallonHorn.hpp"
#include "ov000/town/TownActionHenge.hpp"
#include "ov000/town/TownActionRuraFailed.hpp"
#include "ov000/town/TownActionCalculate.hpp"
#include "ov000/town/TownActionWalk.hpp"
#include "ov000/town/TownCamera.hpp"
#include "ov000/town/TownCharacterManager.hpp"
#include "ov000/town/TownDamageFloor.hpp"
#include "ov000/town/TownExtraCollManager.hpp"
#include "ov000/town/TownFurniture.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "ov000/town/TownSystem.hpp"
#include "ov000/town/TownWindowSystem.hpp"
#include "ov000/Commands/TownCommand.hpp"
#include "main/Commands/CommonCommand.hpp"
#include "main/btl/BattleScriptManager.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/cmn/CommonRuraData.hpp"
#include "main/cmn/CommonCounterInfo.hpp"
#include "main/cmn/ExtraMapLink.hpp"
#include "main/cmn/HengeNoTsueManager.hpp"
#include "main/cmn/NonBattleActionManager.hpp"
#include "main/cmn/TalkSoundManager.hpp"
#include "main/encount/Encount.hpp"
#include "main/global/Global.hpp"
#include "main/status/BattleResult.hpp"
#include "main/status/BaseStatus.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/HaveEquipment.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/text/TextAPI.hpp"
#include "ov000/town/riseup/TownRiseup.hpp"
#include "ov000/town/TownActionRura.hpp"
#include "ov000/town/TownIkadaAction2.hpp"
#include "ov000/town/TownKaidanAction2.hpp"
#include "ov000/town/TownPlayerAction.hpp"

static dss::Fix32Vector3 position(0, 0, 0);

ARM TownPlayerManager::TownPlayerManager()
{
}

ARM TownPlayerManager* TownPlayerManager::getSingleton()
{
    static TownPlayerManager m_singleton;
    return &m_singleton;
}

ARM void TownPlayerManager::initialize()
{
    g_cmnPartyInfo.checkBallon(&g_cmnPartyInfo.position_);
    flagEncount_ = 0;
    encountLock_ = 0;
    flagMapLink_ = 0;
    effectPosFlag_ = 0;
    charaColl_ = 1;
    locked_ = 0;
    remoteFlag_ = 0;
    eventEncount_ = 0;
    partyDraw_.setup();
    exitLock_ = 0;
    cameraRot_ = ROT_NONE;
    prev_cameraRot_ = ROT_NONE;
    txAction_ = ANIM_NONE;
    txCounter_ = 0;
    scriptColl_ = 0;
    unsigned int i;
    param::MapChurch* church = status::excelParam.mapChurch_;
    for (i = 0; i < param::MapChurch::size_; i++) {
        if (church[i].floor[0] == g_Global.getMapName()[0] && church[i].floor[1] == g_Global.getMapName()[1]) {
            church_ = (char)(church[i].byte_1 & 1);
            break;
        }
    }
    wait_ = 30;
    battleLose_ = 0;
    static dss::Fix32Vector3 returnIkadaPos(-4.0f, 0.0f, -1.18f);
    if (status::g_BattleResult.playerDemolitionMessage_ != 2) {
        battleLose_ = 1;
        getSingleton()->setLock(1);
        dss::Fix32Vector3 overview(g_Stage.overviewPosition_);
        g_Stage.overviewTempPosition_ = overview;
        g_Stage.lastFldSurface_ = -1;
        g_Stage.setRanaMapName("");
        param::VehicleData* vehicle = status::excelParam.vehicle_;
        if (g_Global.getMapName()[0] == 'h' && g_Global.getMapName()[1] == 'h') {
            g_cmnPartyInfo.setIkadaInfo("hhout", &returnIkadaPos);
        }
        for (unsigned int i = 0; i < param::VehicleData::size_; i++) {
            char name0 = vehicle[i].mapname[0];
            char name1 = vehicle[i].mapname[1];
            if (name0 == 'c' && name1 == 'b') {
                name0 = 'm';
            } else if (name0 == 'c' && name1 == 'c') {
                name0 = 'h';
            } else if (name0 == 'c' && name1 == 'h') {
                name0 = 'h';
            }
            if (name0 == g_Global.getMapName()[0] && name1 == g_Global.getMapName()[1]) {
                dss::Fix32Vector3 pos;
                pos.vx.value = vehicle[i].shipX;
                pos.vy.value = vehicle[i].shipY;
                g_Stage.shipPosition_ = dss::Fix32Vector3(pos);
                pos.vx.value = vehicle[i].balloonX;
                pos.vy.value = vehicle[i].balloonY;
                g_Stage.balloonPosition_ = dss::Fix32Vector3(pos);
                break;
            }
        }
        if ((g_Global.getMapName()[0] == 'm' && g_Global.getMapName()[1] == 's') ||
            (g_Global.getMapName()[0] == 'd' && g_Global.getMapName()[1] == 'p')) {
            g_Stage.balloonFieldType_ = 1;
            g_Stage.balloonPosition_ = cmn::CommonRuraData::getSingleton()->getBalloonTownPos(0x16);
        } else {
            g_Stage.balloonFieldType_ = 0;
        }
    } else if (g_Stage.load_ != 0) {
        getSingleton()->setLock(1);
    }
    defaultClip_ = 2;
    BillboardCharacter::setAllCharaAnim(1);
    static const dss::Fix32 CLIP120(0x78000);
    static const dss::Fix32 CLIP80(0x50000);
    static const dss::Fix32 CLIP70(0x46000);
    switch (g_Global.getMapName()[0]) {
    case 's':
        switch (g_Global.getMapName()[1]) {
        case 'j':
        case 'r':
            TownStageManager::getSingleton()->setClipDistance(CLIP80);
            defaultClip_ = 0;
            break;
        }
        return;
    case 'd':
        if (g_Global.getMapName()[1] == 'p') {
            if (g_Global.getMapName()[2] == 'o') {
                TownStageManager::getSingleton()->setClipDistance(CLIP120);
                defaultClip_ = 0;
            } else if (g_Global.getMapName()[3] == '6') {
                TownStageManager::getSingleton()->setClipDistance(CLIP70);
                defaultClip_ = 0;
            }
        }
    case 'm':
        if (g_Global.getMapName()[1] == 's') {
            defaultClip_ = 0;
            TownStageManager::getSingleton()->setClipDistance(CLIP70);
        }
        return;
    }
}

ARM void TownPlayerManager::terminate()
{
    if (txAction_ != ANIM_NONE) {
        partyDraw_.restorePose();
    }
    partyDraw_.cleanup();
}

ARM void TownPlayerManager::execute()
{
    TownCharacterManager::getSingleton()->search_ = 0;
    g_cmnPartyInfo.playerTalk = 0;
    if (encount::Encount::getSingleton()->isEncounted()) {
        return;
    }
    if (g_Global.partChangeFlag_ == 1 && player_.actionType_ != ACTION_TYPE_FALL) {
        return;
    }
    if (mapChangeCounter_ != -1) {
        if (mapChangeCounter_ == 0 && player_.actionType_ != ACTION_TYPE_FALL) {
            setRemote(0);
        }
        flagMapLink_ = 1;
        mapChangeCounter_--;
    }
    searchMapUid_ = 0;
    TownCharacterManager::getSingleton()->resetCharaTalk();
    g_cmnPartyInfo.prev_position_ = g_cmnPartyInfo.position_;
    checkMenuAction();
    if (prev_cameraRot_ != ROT_NONE || cameraRot_ != ROT_NONE) {
        setCameraRot();
    }
    setShadow();
    normalExec();
    unkfunc_02133764();
    if (effectPosFlag_ == 0) {
        partyDraw_.setPosition(0, g_cmnPartyInfo.position_);
        partyDraw_.setRotate(0, g_cmnPartyInfo.dirIdx_);
        party_.setPosition();
    } else {
        partyDraw_.setPosition(0, effectPos_);
        dss::Fix32Vector3 tempPos(g_cmnPartyInfo.position_);
        g_cmnPartyInfo.position_ = effectPos_;
        partyDraw_.setRotate(0, g_cmnPartyInfo.dirIdx_);
        party_.setPosition();
        g_cmnPartyInfo.position_ = tempPos;
    }
    for (int i = 1; i < partyDraw_.countReal_; i++) {
        partyDraw_.setPosition(i, party_.getMemberPosition(i));
        partyDraw_.setRotate(i, party_.getMemberDirIdx(i));
    }
    party_.resetFixPos();
    execMapLink();
    if (!isLock()) {
        getPlayerCommand();
    }
    int menuAction = g_cmnPartyInfo.menuAction_;
    if (menuAction != cmn::MENU_ACTION_NONE) {
        if (menuAction != cmn::MENU_HENGE_NO_TSUE) {
            setLock(0);
        }
        g_cmnPartyInfo.setMenuAction(cmn::MENU_ACTION_NONE);
    }
    if (g_Stage.menuTalk_ == 1) {
        g_Stage.menuTalk_ = 0;
        setLock(0);
    }
    if (g_Stage.menuSearch_ == 1) {
        g_Stage.menuSearch_ = 0;
        setLock(0);
    }
    if (nextEncount_ == 1 && encountTile_ == 0) {
        setLock(0);
        nextEncount_ = 2;
    }
    setPlayerCommand(PUSH_NONE);
    textureAnimExecute();
    if (status::g_BattleResult.playerDemolitionMessage_ != 2) {
        if (status::g_BattleResult.playerDemolitionMessage_ != 0) {
            status::g_BattleResult.playerDemolition_ = 0;
            status::g_BattleResult.playerDemolitionMessage_ = 2;
        } else {
            demolitionChurch();
        }
    } else if (g_Stage.load_ != 0) {
        loadChurch();
    }
}

ARM void TownPlayerManager::draw()
{
    partyDraw_.execute();
}

ARM void TownPlayerManager::setup()
{
    partyDraw_.setExcute(1);
    party_.setPositionArrayPointer(g_cmnPartyInfo.getPositionArrayPointer());
    party_.setDirIdxArrayPointer(g_cmnPartyInfo.getDirectionArrayPointer());
    scriptType_ = 0;
    allShadowReset_ = 1;
    shadowSet_ = 0;
    mapChangeSE_ = 1;
    int exitIndex = StageLink::getTownExitIndex();
    dss::Fix32Vector3 pos;
    if (exitIndex != -1 && g_Stage.idoLink_.data_.link_.encount_ == 0 && g_cmnPartyInfo.prevLocation_ == 0) {
        TownStageManager::getSingleton()->setExitPosition(&pos, exitIndex);
        status::HaveEquipment::getAbsoluteValue(exitIndex);
        setPosition(pos);
        flagMapLink_ = 1;
        dss::Fix32Vector3 dirVec = TownStageManager::getSingleton()->getSurfaceDir(exitIndex);
        if (dirVec.vy <= dss::Fix32(0x333)) {
            short idx = 0;
            TownActionCalculate::getIdxByVec(idx, dirVec);
            g_cmnPartyInfo.setDirIdx(idx);
        }
    }
    if (g_Stage.flagMapChange_ == 1) {
        pos.set(0, 0, 0);
        setPosition(pos);
        g_Stage.flagMapChange_ = 0;
    }
    if (g_Stage.idoLink_.data_.link_.encount_ == 1) {
        locked_ = g_Stage.playerLockCount_;
        g_cmnPartyInfo.prevFrameBattle_ = 1;
        setLock(0);
    }
    player_.setup();
    party_.setup();
    TownStageManager::getSingleton()->coll_.searchClear();
    g_cmnPartyInfo.prevLocation_ = 0;
    g_Stage.idoLink_.data_.link_.encount_ = 0;
    g_Stage.menuTalk_ = 0;
    g_Stage.menuSearch_ = 0;
    g_Stage.ropeLink_ = 0;
    partyDraw_.setAnimation(2);
    idoMess_ = 0;
    tabiLink_ = 0;
    walkCounter_ = 0;
    nextEncount_ = 0;
    encountTile_ = 0;
    setPlayerCommand(PUSH_NONE);
    if (player_.actionType_ != ACTION_TYPE_FALL) {
        mapChangeCounter_ = 10;
    } else {
        mapChangeCounter_ = 0;
    }
    flagMapLink_ = 1;
    setRemote(1);
    searchAction_ = 0;
    mapFKLock_ = 0;
    if (dss::strcmp(g_Global.getMapName(), "fk01") == 0) {
        mapFKLock_ = 1;
    }
}

ARM void TownPlayerManager::cleanup()
{
    player_.cleanup();
    party_.cleanup();
}

ARM void TownPlayerManager::unkfunc_02133764()
{
}

ARM void TownPlayerManager::normalExec()
{
    if (!cmn::PlayerManager::isLock()) {
        checkCommandEnd();
        g_cmnPartyInfo.partyTalk = 1;
        if (g_Global.partChangeFlag_ == 0 && encount::Encount::getSingleton()->isEncounted() == 0 && remoteFlag_ == 0) {
            if (dss::g_Pad.edge() & 1) {
                setPlayerCommand(PUSH_BENRI_BUTTON);
                TownCharacterManager::getSingleton()->search_ = 1;
                g_cmnPartyInfo.partyTalk = 0;
            }
        }
        player_.execute();
        dss::Fix32Vector3 prevPos(g_cmnPartyInfo.prev_position_);
        dss::Fix32Vector3 nextPos(g_cmnPartyInfo.position_);
        TownDamageFloor::getSingleton()->checkDamageFloor(prevPos, nextPos);
        if (prevPos != nextPos) {
            switch (player_.actionType_) {
            case ACTION_TYPE_WALK:
            case ACTION_TYPE_ROPE:
            case ACTION_TYPE_KAIDAN:
            case ACTION_TYPE_FRAME_MOVE:
                partyDraw_.setWriggleCharaAll(1);
                break;
            default:
                partyDraw_.setWriggleCharaAll(0);
                break;
            }
            if (isLock()) {
                return;
            }
            walkCounter_++;
        } else {
            partyDraw_.setWriggleCharaAll(0);
        }
        return;
    }
    if (scriptType_ != 0) {
        scriptExecute();
    }
    if (scriptRotFlag_ == 1) {
        dss::Vector3<short> angle;
        angle.set(0, getDirection(), 0);
        scriptMove_.execRot(angle);
        setDirection(angle.vy);
        if (scriptMove_.rotUpdate() == 1) {
            scriptRotFlag_ = 0;
        }
    }
    if (player_.actionType_ == ACTION_TYPE_FALL) {
        TownFallAction::getSingleton()->exitFall();
    }
}

ARM void TownPlayerManager::mormalMapLink()
{
    char* name = TownStageManager::getSingleton()->getLinkMapName();
    if (name == NULL) {
        flagMapLink_ = 0;
        flagIdoLink_ = 0;
        return;
    }
    if (idoMess_ == 0) {
        if (flagIdoLink_ == 1) {
            ui_MsgSndSet(0x30);
            TownWindowSystem::getSingleton()->openCommonMessage();
            TownWindowSystem::getSingleton()->addCommonMessage(0xc40c6);
            idoMess_ = 1;
            g_Stage.idoLink_.data_.link_.inFlag_ = 1;
            g_Stage.idoLink_.pos_ = dss::Fix32Vector3(g_cmnPartyInfo.position_);
            g_Stage.idoLink_.data_.link_.dirIdx_ = g_cmnPartyInfo.dirIdx_;
            setLock(1);
            flagMapLink_ = 0;
            flagIdoLink_ = 0;
            getSingleton()->setPlayerCommand(START_IDO_LINK_COMMAND);
            return;
        }
        if (TownStageManager::getSingleton()->getHitSurfaceIdByType(5) != -1 &&
            TownStageManager::getSingleton()->getHitSurfaceIdByType(6) == -1) {
            return;
        }
    }
    if (flagMapLink_ != 0) {
        return;
    }
    flagMapLink_ = 1;
    if (mapChangeSE_ == 1) {
        TownSystem::getSingleton()->playExitSE_ = 1;
    }
    if (dss::strcmp(name, "world") == 0) {
        int id = StageLink::getSymbolIndex();
        StageLink::setFieldSymbolIndex(id);
        g_Global.nextFieldType_ = cmn::g_extraMapLink.getFieldTypeBySurface(id);
        g_Global.startField();
        getSingleton()->setLock(1);
        g_Stage.idoLink_.data_.link_.inFlag_ = 0;
        g_Stage.idoLink_.data_.link_.outFlag_ = 0;
    } else if (TownStageManager::getSingleton()->isStageExist(name)) {
        StageLink::setTownExitIndex(TownStageManager::getSingleton()->getExitIndex());
        g_Global.startTown(name);
        getSingleton()->setLock(1);
    }
}

ARM void TownPlayerManager::execMapLink()
{
    static int dId = -1;
    int exitIndex = TownStageManager::getSingleton()->getExitIndex();
    if (exitIndex == -1 || exitLock_ != 0) {
        flagMapLink_ = 0;
        flagIdoLink_ = 0;
        return;
    }
    if (dId != exitIndex) {
        dId = exitIndex;
    }
    if (flagMapLink_ == 0) {
        int id = exitIndex & 0xfff;
        if (id < 400) {
            if (id >= 300) {
                cmn::g_extraMapLink.startExitLoop();
                getSingleton()->setLock(1);
                flagMapLink_ = 1;
                if (mapChangeSE_ == 1) {
                    TownSystem::getSingleton()->playExitSE_ = 1;
                }
                return;
            }
            if (id >= 200 && tabiLink_ == 0) {
                if (g_cmnPartyInfo.position_ != g_cmnPartyInfo.prev_position_) {
                    cmn::NonBattleActionManager::getSingleton()->setAction(cmn::ACTION_TRAVELDOOR);
                    g_Global.setRanarutaFlag(true);
                    setLock(1);
                    lockMapLink(EXIT_LOCK_TABI);
                    tabiLink_ = 1;
                }
                return;
            }
        }
        int ret = cmn::g_extraMapLink.checkTownMapLink(exitIndex);
        if (ret == 4) {
            return;
        }
        if (ret != 0) {
            if (mapChangeSE_ == 1) {
                TownSystem::getSingleton()->playExitSE_ = 1;
            }
            getSingleton()->setLock(1);
            flagMapLink_ = 1;
            return;
        }
    }
    mormalMapLink();
}

ARM void TownPlayerManager::inputPad(int padDir)
{
    player_.inputPad(padDir);
}

ARM void TownPlayerManager::inputClear()
{
    player_.inputClear();
}

ARM void TownPlayerManager::resetParty()
{
    status::g_Party.setDisplayMode();
    if (partyDraw_.countReal_ != 0) {
        partyDraw_.cleanup();
        partyDraw_.setup();
        int count = partyDraw_.countReal_;
        for (int i = 0; i < count; i++) {
            partyDraw_.setPosition(i, party_.getMemberPosition(i));
            partyDraw_.setRotate(i, party_.getMemberDirIdx(i));
        }
    }
    partyDraw_.resetAlpha();
    switch (player_.actionType_) {
    case ACTION_TYPE_SHIP:
        partyDraw_.setDrawPartyNone();
        break;
    case ACTION_TYPE_IKADA:
        partyDraw_.setDrawPartyOne();
        break;
    }
}

ARM void TownPlayerManager::lockMapLink(EXIT_LOCK_TYPE type)
{
    exitLock_ |= type;
}

ARM void TownPlayerManager::resetMapLink(RESET_EXIT_LOCK_TYPE type)
{
    exitLock_ &= ~type;
}

ARM void TownPlayerManager::setPosition(dss::Fix32Vector3& pos)
{
    g_cmnPartyInfo.position_ = pos;
}

ARM dss::Fix32Vector3 TownPlayerManager::getPosition()
{
    return g_cmnPartyInfo.position_;
}

ARM void TownPlayerManager::setPartyToFirst(dss::Fix32Vector3& pos)
{
    g_cmnPartyInfo.position_ = pos;
    party_.setAllPotition(pos);
}

ARM void TownPlayerManager::setDirection(short dirIdx)
{
    g_cmnPartyInfo.setDirIdx(dirIdx);
}

ARM short TownPlayerManager::getDirection()
{
    return g_cmnPartyInfo.dirIdx_;
}

ARM void TownPlayerManager::unkfunc_02133f60()
{
    short dirIdx = getDirection();
    dss::Fix32Vector3 pos = getPosition();
    TownActionWalk::getSingleton()->townCharColl(pos, pos, 1);
    int objectId = TownActionWalk::getSingleton()->searchObjectId_;
    int charaNo;
    if (objectId != -1 && TownExtraCollManager::getSingleton()->isExtraCollChara(objectId, charaNo) == 2) {
        TownCharacterManager::getSingleton()->setTalked(charaNo, 1);
        TownCharacterManager::getSingleton()->setTalkedArea(charaNo, 1);
        return;
    }
    if (!TownCharacterManager::getSingleton()->checkTalkingNearCharacter(pos, dirIdx, -1)) {
        TownWindowSystem::getSingleton()->openCommonMessage();
        TextAPI::setMACRO0(0x12, 0x50000000, status::g_Party.getPlayerStatus(0)->haveStatusInfo_.haveStatus_.playerIndex_);
        TownWindowSystem::getSingleton()->addCommonMessage(0xc3d97);
    }
}

ARM void TownPlayerManager::unkfunc_02134058()
{
    searchAction_ = TownActionWalk::getSingleton()->searchObject(1);
}

ARM void TownPlayerManager::setFormation(int frmDir, int charaDir, dss::Fix32 speed)
{
    dss::Fix32Vector3 frmVec;
    short dirIdx = TownActionCalculate::getIdxByParam(charaDir);
    frmVec = TownActionCalculate::getParamVec(frmDir);
    frmVec.normalize();
    frmVec *= TownPlayerAction::walkSpeed;
    setDirection(dirIdx);
    party_.setFormation(frmVec, dirIdx, speed);
    frmVec *= -1;
    TownActionCalculate::getIdxByVec(dirIdx, frmVec);
    frmDirIdx_ = dirIdx;
}

ARM void TownPlayerManager::setRemote(int flag)
{
    remoteFlag_ = flag;
    if (isEncountLock() == 0) {
        TownDamageFloor::getSingleton()->damageFlag_ = 1;
        TownDamageFloor::getSingleton()->effectFlag_ = 1;
        TownDamageFloor::getSingleton()->encountFlag_ = 1;
    } else {
        TownDamageFloor::getSingleton()->damageFlag_ = 0;
        TownDamageFloor::getSingleton()->effectFlag_ = 0;
        TownDamageFloor::getSingleton()->encountFlag_ = 0;
    }
}

ARM bool TownPlayerManager::isLock()
{
    if (remoteFlag_ == 1 || cmn::PlayerManager::isLock() == 1) {
        return true;
    }
    return false;
}

ARM void TownPlayerManager::setLock(int lock)
{
    if (locked_ == 0) {
        partyDraw_.setExcute(0);
        TownCharacterManager::getSingleton()->setAllEventLock(1);
        TownCharacterManager::getSingleton()->eventLockAllChraraAnim();
        partyDraw_.setAnimation(2);
        getSingleton()->partyDraw_.setWriggleCharaAll(0);
        TownStageManager::getSingleton()->setBoxTest(0);
    }
    cmn::PlayerManager::setLock(lock);
    player_.inputClear();
    if (locked_ == 0) {
        partyDraw_.setExcute(1);
        charaColl_ = 1;
        TownCharacterManager::getSingleton()->setAllEventLock(0);
        TownCharacterManager::getSingleton()->restoreCharacterAnim();
        partyDraw_.setAnimation(1);
        TownStageManager::getSingleton()->setBoxTest(defaultClip_);
    }
    if (isEncountLock() == 1) {
        TownDamageFloor::getSingleton()->damageFlag_ = 0;
        TownDamageFloor::getSingleton()->effectFlag_ = 0;
        TownDamageFloor::getSingleton()->encountFlag_ = 0;
    } else {
        TownDamageFloor::getSingleton()->damageFlag_ = 1;
        TownDamageFloor::getSingleton()->effectFlag_ = 1;
        TownDamageFloor::getSingleton()->encountFlag_ = 1;
    }
}

ARM void TownPlayerManager::setCureFloor()
{
    status::g_Party.allRecovery();
    UnkScreenEffectManager::getSingleton()->unkfunc_0202aec4(1);
}

ARM void TownPlayerManager::setCameraRotToNorth()
{
    switch (cameraRot_) {
    case ROT_TO_NORTH:
    case ROT_TO_NORTH_END:
    case ROT_TO_NORTH_FREE:
        return;
    }
    cameraRot_ = ROT_TO_NORTH;
    setLock(1);
}

ARM void TownPlayerManager::setCameraRot()
{
    short dirIdx = getDirection();
    switch (cameraRot_) {
    case ROT_NONE:
    case ROT_TO_NORTH_END:
    case ROT_TO_NORTH_FREE:
        break;
    case ROT_TO_NORTH: {
        short addAngle = 0;
        if (TownCamera::getSingleton()->setAngleNorth(addAngle) == true) {
            cameraRot_ = ROT_TO_NORTH_END;
            setLock(0);
        }
        if (player_.actionType_ != ACTION_TYPE_ROPE) {
            setDirection(addAngle + dirIdx);
        }
        break;
    }
    case ROT_TO_R:
        TownCamera::getSingleton()->rotateR();
        if (TownCamera::getSingleton()->flagRotateR == 1 && player_.actionType_ != ACTION_TYPE_ROPE) {
            int rot = (short)(dirIdx - 0x100);
            setDirection(rot);
        }
        break;
    case ROT_TO_L:
        TownCamera::getSingleton()->rotateL();
        if (TownCamera::getSingleton()->flagRotateL == 1 && player_.actionType_ != ACTION_TYPE_ROPE) {
            int rot = (short)(dirIdx + 0x100);
            setDirection(rot);
        }
        break;
    }
    prev_cameraRot_ = cameraRot_;
    if (cameraRot_ != ROT_TO_NORTH && cameraRot_ != ROT_TO_NORTH_END) {
        cameraRot_ = ROT_NONE;
    }
}

ARM void TownPlayerManager::setSimpleMove(dss::Fix32Vector3& prev, dss::Fix32Vector3& next, int frame)
{
    setLock(1);
    scriptMove_.setActionMove(prev, next);
    scriptMove_.setMoveFrame(frame);
    scriptType_ = 1;
    party_.script_ = 1;
    party_.fixFlag_ = 0;
}

ARM void TownPlayerManager::setSpeedMove(dss::Fix32Vector3& prev, dss::Fix32Vector3& next, dss::Fix32 speed)
{
    setLock(1);
    static const dss::Fix32 defaultSpeed(0x66);
    speed = defaultSpeed * speed;
    scriptMove_.setActionMove(prev, next);
    scriptMove_.setMoveSpeed(speed);
    scriptType_ = 1;
    party_.script_ = 1;
    party_.fixFlag_ = 0;
}

ARM void TownPlayerManager::setJumpMove(dss::Fix32Vector3& endPos, int frame)
{
    dss::Fix32Vector3 pos = getPosition();
    scriptMove_.setJumpMove(pos, endPos, frame);
    scriptType_ = 2;
    for (int i = 0; i < partyDraw_.countReal_; i++) {
        partyDraw_.partyCharacter_[i].setShadowFlag(0);
    }
    setLock(1);
}

ARM bool TownPlayerManager::isFinish()
{
    return scriptType_ == 0;
}

ARM void TownPlayerManager::setCameraRotType(CAMERA_ROT_TYPE type)
{
    if (prev_cameraRot_ != ROT_TO_NORTH && prev_cameraRot_ != ROT_TO_NORTH_END) {
        cameraRot_ = type;
        return;
    }
    if (prev_cameraRot_ == ROT_TO_NORTH_END && type == ROT_NONE) {
        cameraRot_ = ROT_NONE;
    }
}

ARM bool TownPlayerManager::isEncountLock()
{
    if (isLock() == true || encountLock_ == 1) {
        return true;
    }
    return false;
}

ARM void TownPlayerManager::setEncountLock(int flag)
{
    encountLock_ = flag;
    if (isEncountLock() == 1) {
        TownDamageFloor::getSingleton()->damageFlag_ = 0;
        TownDamageFloor::getSingleton()->effectFlag_ = 0;
        TownDamageFloor::getSingleton()->encountFlag_ = 0;
    } else {
        TownDamageFloor::getSingleton()->damageFlag_ = 1;
        TownDamageFloor::getSingleton()->effectFlag_ = 1;
        TownDamageFloor::getSingleton()->encountFlag_ = 1;
    }
}

ARM bool TownPlayerManager::isIdoLinkPos()
{
    dss::Fix32Vector3 pos = TownStageManager::getSingleton()->getHitSurfacePosByType(5);
    dss::Fix32Vector3 vec = pos - g_cmnPartyInfo.position_;
    vec.vy = 0L;
    vec.normalize();
    dss::Fix32Vector3 dir;
    TownActionCalculate::getDirByIdx(g_cmnPartyInfo.dirIdx_, dir);
    dss::Fix32 dot = vec * dir;
    return dot.value >= 0x424;
}

ARM int TownPlayerManager::getDamageColor(int type)
{
    switch (type) {
    case 1:
        return TownPartyDraw::colorDoku;
    case 0:
        return TownPartyDraw::colorBarrier;
    }
    return 0x7fff;
}

ARM void TownPlayerManager::setShadow()
{
    static const dss::Fix32 fixMax(0x2cd);
    static const dss::Fix32 shadowHeight(0x5000);
    if (shadowSet_ == 1) {
        int i;
        int floorPoly = TownStageManager::getSingleton()->coll_.m_floorPolygonNo;
        dss::Fix32Vector3 floorPos;
        TownStageManager::getSingleton()->stage_.collGetPolygonPos(floorPoly, &floorPos);
        allShadowReset_ = 0;
        dss::Fix32Vector3 position(g_cmnPartyInfo.position_);
        for (i = 0; i < getSingleton()->partyDraw_.countReal_; i++) {
            dss::Fix32Vector3 pos = getSingleton()->party_.getMemberPosition(i);
            if (position.vx == pos.vx && position.vz == pos.vz) {
                if (TownStageManager::getSingleton()->getHitSurfaceIdByType(0) == -1 || floorPoly == -1) {
                    getSingleton()->partyDraw_.partyCharacter_[i].setShadowAlpha(0);
                } else {
                    floorPos.vx = pos.vx;
                    floorPos.vz = pos.vz;
                    getSingleton()->partyDraw_.partyCharacter_[i].setShadowPos(&floorPos);
                    dss::Fix32 vol = (shadowHeight - (pos.vy - floorPos.vy)) / shadowHeight * 12;
                    if (vol < dss::Fix32(0L)) {
                        vol = 0L;
                    }
                    int alpha = vol.value / 4096;
                    alpha = status::BaseStatus::getClampValue(0, (unsigned char)alpha, 12);
                    getSingleton()->partyDraw_.partyCharacter_[i].setShadowAlpha(alpha);
                    getSingleton()->partyDraw_.partyCharacter_[i].setShadowStay(0);
                }
            }
        }
    } else if (allShadowReset_ == 0) {
        getSingleton()->partyDraw_.partyCharacter_[0].setShadowStay(1);
        getSingleton()->partyDraw_.partyCharacter_[0].setShadowAlpha(12);
        int floorPoly = TownStageManager::getSingleton()->coll_.m_floorPolygonNo;
        dss::Fix32Vector3 floorPos;
        TownStageManager::getSingleton()->stage_.collGetPolygonPos(floorPoly, &floorPos);
        allShadowReset_ = 1;
        dss::Fix32Vector3 position(g_cmnPartyInfo.position_);
        for (int i = 1; i < getSingleton()->partyDraw_.countReal_; i++) {
            dss::Fix32Vector3 pos = getSingleton()->party_.getMemberPosition(i);
            int value = pos.vy.value - floorPos.vy.value;
            if (value >= 0 && value < 0x3c) {
                getSingleton()->partyDraw_.partyCharacter_[i].setShadowStay(1);
                getSingleton()->partyDraw_.partyCharacter_[i].setShadowAlpha(12);
            } else {
                allShadowReset_ = 0;
            }
        }
    }
}

ARM void TownPlayerManager::checkMenuAction()
{
    searchAction_ = 0;
    menuSearch_ = 0;
    if (nextEncount_ == 1) {
        btl::BattleScriptManager::getSingleton()->setEncountMap(encountTile_);
        encount::Encount::getSingleton()->forceEventBrew(encountTile_);
        encountTile_ = 0;
        return;
    }
    switch (g_cmnPartyInfo.menuAction_) {
    case cmn::MENU_RURA_FAILED:
        TownActionRuraFailed::getSingleton()->startCheck();
        player_.actionType_ = ACTION_TYPE_RURA_FAILED;
        TownSystem::getSingleton()->scriptLock_ = 1;
        TownCharacterManager::getSingleton()->setAllMotionLock(1);
        setLock(1);
        break;
    case cmn::MENU_RIREMIT: {
        int index = g_Stage.symbolID_;
        dss::Fix32Vector3 offsetdata(0, 0, 0);
        g_cmnPartyInfo.rideOnType_ = cmn::RIDE_ON_NONE;
        switch (g_Global.getMapName()[0]) {
        case 'd':
            switch (g_Global.getMapName()[1]) {
            case 'k':
                g_cmnPartyInfo.rideOnType_ = cmn::RIDE_ON_SHIP_IKADA;
                offsetdata.vy = 0x14000;
                break;
            case 'o':
                index = 0x41;
                break;
            case 'p':
                index = 0x52;
                break;
            }
            break;
        case 's':
            if (g_Global.getMapName()[1] == 'j') {
                index = 0x4d;
            }
            break;
        }
        cmn::g_extraMapLink.setExtraExitField(index, offsetdata);
        setLock(1);
        break;
    }
    case cmn::MENU_HENGE_NO_TSUE:
        g_HengeNoTsue.setNextAction(player_.actionType_);
        player_.actionType_ = ACTION_TYPE_HENGE;
        TownActionHenge::getSingleton()->setChangeAction();
        break;
    case cmn::BALLON_HORN:
        TownActionBallonHorn::getSingleton()->startAction(player_.actionType_);
        player_.actionType_ = ACTION_TYPE_BALLON_HORN;
        getSingleton()->setRemote(1);
        setLock(1);
        break;
    }
    if (g_Stage.menuTalk_ == 1) {
        setPlayerCommand(START_TALK_COMMAND);
        TownCharacterManager::getSingleton()->search_ = 1;
        switch (player_.actionType_) {
        case ACTION_TYPE_WALK:
            unkfunc_02133f60();
            break;
        case ACTION_TYPE_IKADA:
            TownIkadaAction2::getSingleton()->checkIkadaTalk(1);
            break;
        }
        setLock(1);
    }
    if (g_Stage.menuSearch_ == 1) {
        unkfunc_02134058();
        menuSearch_ = 1;
        setLock(1);
    }
    if (g_Stage.ruraFlag_ == 1) {
        getSingleton()->setLock(0);
        player_.actionType_ = ACTION_TYPE_RURA;
        TownActionRura::getSingleton()->startCheck();
        partyDraw_.setExcute(0);
    }
}

ARM void TownPlayerManager::scriptExecute()
{
    dss::Fix32Vector3 nowPos = getPosition();
    dss::Fix32Vector3 nextPos = nowPos;
    short dirIdx = getDirection();
    scriptMove_.execMove(nextPos);
    if (rotLock_ == 1) {
        setDirection(dirIdx);
    } else if (nextPos != nowPos) {
        dss::Fix32Vector3 vec = nextPos - nowPos;
        short idx = getDirection();
        TownActionCalculate::getIdxByVec(idx, vec);
        setDirection(idx);
    }
    int coll = scriptColl_;
    if ((coll & 2) || (coll & 1)) {
        TownStageManager::getSingleton()->characoterColl(nowPos, nextPos, TownPlayerAction::collR, &nextPos, coll);
    }
    setPosition(nextPos);
    if (player_.actionType_ == ACTION_TYPE_IKADA) {
        TownIkadaAction2::getSingleton()->setIkadaPosition(nextPos);
    }
    if (scriptType_ == 4) {
        TownIkadaAction2::getSingleton()->setIkadaPosition(nextPos);
    }
    if (scriptMove_.moveUpdate() == 1) {
        party_.script_ = 0;
        party_.fixFlag_ = 0;
        switch (scriptType_) {
        case 2:
            partyDraw_.partyCharacter_[0].setShadowFlag(1);
            party_.moveFirstFlag_ = 1;
            scriptType_ = 3;
            break;
        case 3:
            if (party_.moveFirstFlag_ == 0) {
                scriptType_ = 0;
                setLock(0);
                for (int i = 0; i < partyDraw_.countReal_; i++) {
                    partyDraw_.partyCharacter_[i].setShadowFlag(1);
                }
            }
            break;
        default:
            scriptType_ = 0;
            shadowSet_ = 0;
            setLock(0);
            break;
        }
    }
}

ARM void TownPlayerManager::setManyaDance(int flag)
{
    if (flag == 1) {
        partyDraw_.changePose(0x7d);
        txCounter_ = 0;
        txAction_ = MANYA_DANCE1;
    } else if (txAction_ != ANIM_NONE) {
        partyDraw_.changePose(0x7c);
        txAction_ = ANIM_END;
    }
}

ARM void TownPlayerManager::textureAnimExecute()
{
    switch (txAction_) {
    case MANYA_DANCE1:
        if (txCounter_ == 12) {
            partyDraw_.restorePose();
            partyDraw_.changePose(0x7e);
            txAction_ = MANYA_DANCE2;
        }
        break;
    }
    txCounter_++;
}

ARM void TownPlayerManager::setScriptRot(int frame, short idx, int flag)
{
    scriptRotFlag_ = 1;
    dss::Vector3<short> start(0, getDirection(), 0);
    short add;
    if (flag == 0) {
        add = -idx;
    } else {
        add = idx;
    }
    dss::Vector3<short> rot(0, add, 0);
    scriptMove_.setSimpleRot(start, rot, frame);
}

ARM void TownPlayerManager::setStartEraseParty()
{
    party_.changeAlpha_ = 1;
    for (int i = 1; i < partyDraw_.countReal_; i++) {
        partyDraw_.setAlpha(i, 0);
    }
}

ARM void TownPlayerManager::rizeupSet(int icon)
{
    riseupIndex_ = TownRiseupManager::getSingleton()->setup(icon, getPosition());
}

ARM bool TownPlayerManager::rizeupEnd()
{
    return TownRiseupManager::getSingleton()->isFinish(riseupIndex_);
}

ARM int TownPlayerManager::getInpasMapObj()
{
    return TownActionWalk::getSingleton()->getMapUid();
}

ARM bool TownPlayerManager::checkTalkToCharacter()
{
    dss::Fix32Vector3 pos = getPosition();
    switch (player_.actionType_) {
    case ACTION_TYPE_WALK: {
        TownCharacterManager::getSingleton()->search_ = 1;
        TownActionWalk::getSingleton()->townCharColl(pos, pos, 1);
        int objectId = TownActionWalk::getSingleton()->searchObjectId_;
        int charaNo;
        if (objectId != -1 && TownExtraCollManager::getSingleton()->isExtraCollChara(objectId, charaNo) == 2) {
            TownCharacterManager::getSingleton()->setTalked(charaNo, 1);
            TownCharacterManager::getSingleton()->setTalkedArea(charaNo, 1);
            return true;
        }
        return TownCharacterManager::getSingleton()->checkTalkingNearCharacter(pos, getDirection(), -1);
    }
    case ACTION_TYPE_IKADA:
        getDirection();
        TownCharacterManager::getSingleton()->search_ = 1;
        return TownIkadaAction2::getSingleton()->checkIkadaTalk(1);
    }
    return false;
}

ARM bool TownPlayerManager::getPlayerCopyInfo(int playerNo, dss::Fix32Vector3& pos, short& idx, int& charNo)
{
    status::g_Party.setDisplayMode();
    int count = status::g_Party.getCarriageOutCount();
    for (int i = 0; i < count; i++) {
        if (playerNo == status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_) {
            charNo = status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.charaIndex_;
            pos = party_.getMemberPosition(i);
            idx = party_.getMemberDirIdx(i);
            partyDraw_.setAlpha(i, 0);
            return true;
        }
    }
    return false;
}

ARM bool TownPlayerManager::setupDelPartyNotMoveFirst(int playerNo)
{
    status::g_Party.setDisplayMode();
    int count = status::g_Party.getCount();
    for (int i = 0; i < count; i++) {
        if (playerNo == status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_) {
            if (i == 0) {
                dss::Fix32Vector3 pos = party_.getMemberPosition(1);
                setPosition(pos);
                setDirection(party_.getMemberDirIdx(1));
                party_.setupPartyDelNotMoveFirst();
            }
            return true;
        }
    }
    return false;
}

ARM void TownPlayerManager::setIkadaSpeedMove(dss::Fix32Vector3& prev, dss::Fix32Vector3& next, dss::Fix32 speed)
{
    setSpeedMove(prev, next, speed);
    scriptType_ = 4;
}

ARM void TownPlayerManager::setIkadaFrameMove(dss::Fix32Vector3& prev, dss::Fix32Vector3& next, int frame)
{
    setSimpleMove(prev, next, frame);
    scriptType_ = 4;
}

ARM void TownPlayerManager::setEncount(int tile)
{
    encountTile_ = tile;
    nextEncount_ = 1;
}

ARM void TownPlayerManager::setMessage()
{
    if (TownWindowSystem::getSingleton()->town_message_ == 1) {
        TownWindowSystem::getSingleton()->town_message_ = 0;
        g_cmnPartyInfo.partyTalk = 0;
        return;
    }
    if (TownWindowSystem::getSingleton()->isOpen() == 1) {
        g_cmnPartyInfo.partyTalk = 0;
        return;
    }
    if (encount::Encount::getSingleton()->isEncounted() == 1) {
        g_cmnPartyInfo.partyTalk = 0;
        return;
    }
    switch (searchAction_) {
    case 6: {
        int uid = TownActionWalk::getSingleton()->getSekaijyuUid();
        if (uid == 0) {
            return;
        }
        ui_MsgSndSet(0x30);
        TownFurnitureManager::getSingleton()->checkObject(uid, 0, 1, 1);
        TownFurnitureManager::getSingleton()->setFurnFlag(uid, 0);
        break;
    }
    case 7:
        ui_MsgSndSet(0x30);
        TownWindowSystem::getSingleton()->openCommonMessage();
        TownWindowSystem::getSingleton()->addCommonMessage(0x1dca2);
        TownWindowSystem::getSingleton()->addCommonMessage(0x1d8c6);
        break;
    case 1:
        if (menuSearch_ != 1) {
            return;
        }
        if (TownActionWalk::getSingleton()->menu_ == 0) {
            return;
        }
        switch (player_.actionType_) {
        case ACTION_TYPE_WALK:
            TownFurnitureManager::getSingleton()->nothingGround();
            break;
        case ACTION_TYPE_SHIP:
        case ACTION_TYPE_IKADA:
            TownFurnitureManager::getSingleton()->nothingWater();
            break;
        }
        break;
    }
}

ARM bool TownPlayerManager::isSearch()
{
    return searchAction_ != 0;
}

ARM void TownPlayerManager::demolitionChurch()
{
    if (wait_ == 0) {
        if (church_ == 0) {
            ui_MsgSndSet(0x32);
            TownWindowSystem::getSingleton()->openMessage(0xc7013, 1);
        } else {
            const char* mapname = g_Global.getMapName();
            if (mapname[0] == 'm' && mapname[1] == 's') {
                ui_MsgSndSet(0x32);
            } else {
                ui_MsgSndSet(0x31);
            }
            TownWindowSystem::getSingleton()->openMessage(0xc73fb, 1);
        }
        getSingleton()->setLock(0);
        status::g_BattleResult.playerDemolitionMessage_ = 2;
    }
    wait_--;
}

ARM void TownPlayerManager::loadChurch()
{
    if (wait_ == 0) {
        if (g_Stage.chapterLoad_ == 0) {
            if (church_ == 0) {
                ui_MsgSndSet(0x32);
                TownWindowSystem::getSingleton()->openMessage(0xc7012, 2);
            } else {
                const char* mapname = g_Global.getMapName();
                if (mapname[0] == 'm' && mapname[1] == 's') {
                    ui_MsgSndSet(0x32);
                } else {
                    ui_MsgSndSet(0x31);
                }
                TownWindowSystem::getSingleton()->openMessage(0xc73fa, 2);
            }
        }
        g_Stage.chapterLoad_ = 0;
        g_Stage.load_ = 0;
        getSingleton()->setLock(0);
    }
    wait_--;
}

ARM void TownPlayerManager::rizeupSetParty(int charaNo, int markNo)
{
    status::g_Party.setDisplayMode();
    int count = status::g_Party.getCarriageOutCount();
    for (int i = 0; i < count; i++) {
        if (charaNo == status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_) {
            if (!status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath() == true) {
                TownRiseupManager::getSingleton()->setup(markNo, party_.getMemberPosition(i));
            }
        }
    }
}

ARM void TownPlayerManager::setVanAndBasha()
{
    partyDraw_.setVanAndBasha();
}

ARM void TownPlayerManager::setLockByEventEncount(int flag)
{
    eventEncount_ = flag;
    setLock(flag);
}

ARM void TownPlayerManager::resetLockByEventEncount()
{
    if (eventEncount_ == 1) {
        setLock(0);
        eventEncount_ = 0;
    }
}

ARM bool TownPlayerManager::isSaveAndBattleOK()
{
    bool ret = true;
    if (player_.actionType_ == ACTION_TYPE_ROPE || player_.actionType_ == ACTION_TYPE_FALL) {
        ret = false;
    }
    if (!TownKaidanAction2::getSingleton()->isSaveOK()) {
        ret = false;
    }
    return ret;
}
