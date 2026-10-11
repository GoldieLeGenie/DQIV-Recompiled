#pragma ipa file
#include "main/cmn/CommonChapterTitle.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/object/BillboardCharacter.hpp"
#include "ov000/town/TownSystem.hpp"
#include "ov000/town/TownEndrollManager.hpp"
#include "ov000/town/TownCamera.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownCharacterManager.hpp"
#include "ov000/Commands/TownCommand.hpp"
#include "ov000/town/TownOpeningManager.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemUseManager.hpp"
#include "main/cmn/GameManager.hpp"
#include "main/cmn/ExtraMapLink.hpp"
#include "main/cmn/TalkSoundManager.hpp"
#include "main/cmn/PartyTalk.hpp"
#include "main/cmn/CommonEffectLocation.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/cmn/NonBattleActionManager.hpp"
#include "main/btl/BattleScriptManager.hpp"
#include "main/script/ScriptSystem.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/GameFlag.hpp"
#include "main/status/Status.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/global/Global.hpp"
#include "main/encount/Encount.hpp"
#include "main/object/DSSAObject.hpp"
#include "main/object/DisplayCharacter.hpp"
#include "ov036/MaterielMenuExtraChangeHostage/MaterielMenuExtraChangeHostage.hpp"
#include "ov000/town/TownWindowSystem.hpp"
#include "ov000/town/TownFurniture.hpp"
#include "ov000/town/riseup/TownRiseup.hpp"
#include "ov000/town/TownExtraMapObjManager.hpp"

ARM void TownSystem::unkfunc_02132210()
{
}

ARM TownSystem::TownSystem()
{
}

ARM TownSystem* TownSystem::getSingleton()
{
    static TownSystem m_singleton;
    return &m_singleton;
}

ARM void TownSystem::initialize()
{
    BillboardCharacter::allAnimLock = 0;
    status::excelParam.setupTown();
    status::excelParam.setupTownInitialize();
    CharacterShadow::unkfunc_02049ba4();
    render_.unkfunc_02084efc();
    TownCamera::getSingleton()->initialize();
    TownStageManager::getSingleton()->initialize();
    TownFurnitureManager::getSingleton()->initialize();
    TownPlayerManager::getSingleton()->initialize();
    TownCharacterManager::getSingleton()->initialize();
    cmn::g_extraMapLink.setup();
    TownExtraMapObjManager::getSingleton()->setup();
    cmn::GameManager::getSingleton()->initialize();
    TownPlayerManager* player = TownPlayerManager::getSingleton();
    cmn::GameManager::getSingleton()->playerManager_ = player;
    cmn::g_talkSound.setup();
    TownPlayerManager::getSingleton()->setup();
    encount::Encount::getSingleton()->setup(EncountDungeon, Floor);
    btl::BattleScriptManager::getSingleton()->checkScriptBattleResult();
    TownRiseupManager::getSingleton()->initialize();
    ScriptSystem::getSingleton()->initialize(status::g_Story.chapter_);
    UnkScreenEffectManager::getSingleton()->unkfunc_0202ace4();
    playExitSE_ = 0;
    defaultSELock_ = 0;
    scriptLock_ = 0;
    fadeCount_ = 0;
    SoundManager::townPlay();
    int exitNo;
    if (g_Stage.ruraFlag_ != 0) {
        exitNo = -1;
    } else {
        exitNo = StageLink::getTownExitIndex();
    }
    if (exitNo != -1) {
        cmn::PartyTalk::getSingleton()->resetPartyTalk();
        cmn::PartyTalk::getSingleton()->setExitNo(exitNo);
    }
    cmn::CommonEffectLocation::getSingleton()->initialize();
    status::excelParam.cleanupTownInitialize();
    dss::Fix32 scale;
    scale.value = 0x87;
    DSSAObject::setDefaultScale(scale);
    DSSAObject::setPriority(8);
    g_Stage.loadType_ = profile::SAVETYPE_INVALID;
    if (dss::strcmp(g_Global.getMapName(), "ev01") == 0) {
        TownOpeningManager::getSingleton()->setup();
    }
    trigger_ = 1;
}

ARM void TownSystem::terminate()
{
    TownOpeningManager::getSingleton()->cleanup();
    TownEndrollManager::getSingleton()->cleanup();
    cmn::CommonEffectLocation::getSingleton()->terminate();
    cmn::CommonChapterTitle::getSingleton()->cleanup();
    ScriptSystem::getSingleton()->terminate();
    if (dss::strcmp(g_Global.getMapName(), "field") == 0) {
        TownCamera::getSingleton()->resetAngle();
        g_Stage.initDoorOpenFlag();
        g_Stage.setFallFlag(0);
    }
    if (!g_Global.isNextPart(13) && !g_Global.isNextPart(15) && !g_Global.isNextPart(16)) {
        g_GlobalFlag.clear();
        if (g_Global.isAreaChange()) {
            g_LocalFlag.clear();
            g_Stage.initDoorOpenFlag();
            g_cmnPartyInfo.resetShipIkadaMapName();
        }
        g_Stage.playerLockCount_ = 0;
        playTownExitSE();
        g_Stage.initFurnBreakFlag();
        status::StageStatus::setToramana(0);
    } else {
        g_Stage.idoLink_.data_.link_.encount_ = 1;
        TownPlayerManager::getSingleton()->resetLockByEventEncount();
        g_Stage.playerLockCount_ = TownPlayerManager::getSingleton()->getLockCount();
        SoundManager::setTownPlayEnable();
        g_cmnPartyInfo.beforeBattlePos_ = g_cmnPartyInfo.prev_position_;
    }
    TownPlayerManager::getSingleton()->cleanup();
    BillboardCharacter::allAnimLock = 0;
    DisplayCharacter::sleepBodyOffset_ = dss::Fix32(0.27f);
    DisplayCharacter::sleepHeadOffset_ = dss::Fix32(0.173f);
    DisplayCharacter::sleepHeight_ = dss::Fix32(0.194f);
    cmn::GameManager::getSingleton()->terminate();
    TownFurnitureManager::getSingleton()->terminate();
    TownRiseupManager::getSingleton()->terminate();
    TownCharacterManager::getSingleton()->terminate();
    TownPlayerManager::getSingleton()->terminate();
    TownStageManager::getSingleton()->terminate();
    TownCamera::getSingleton()->terminate();
    render_.unkfunc_02084f50();
    CharacterShadow::unkfunc_02049eb4();
    UnkScreenEffectManager::getSingleton()->unkfunc_0202adb4();
    status::Status::setFlagShopExec();
    status::excelParam.cleanupTown();
    g_Global.partChangeFlag_ = 0;
}

ARM void TownSystem::execute()
{
    TownOpeningManager::getSingleton()->execute();
    TownEndrollManager::getSingleton()->execute();
    if (g_Global.getRanarutaFlag()) {
        UnkScreenEffectManager::getSingleton()->unkfunc_0202ad28();
        cmn::NonBattleActionManager::getSingleton()->execute();
    }
    TownFurnitureManager::getSingleton()->execute();
    TownRiseupManager::getSingleton()->execute();
    if (TownWindowSystem::getSingleton()->isOpen()) {
        return;
    }
    TownCharacterManager::getSingleton()->execute();
    TownPlayerManager::getSingleton()->execute();
    TownStageManager::getSingleton()->execute();
    TownCamera::getSingleton()->execute();
    cmn::GameManager::getSingleton()->execute();
    TownMenuItemUseManager::getSingleton()->eventItem_ = 0;
    if (encount::Encount::getSingleton()->isEncounted() == 0 && scriptLock_ == 0) {
        ScriptSystem::getSingleton()->execute();
    }
    TownPlayerManager::getSingleton()->setMessage();
    if (g_Global.bookingFlag_ != Global::BOOKING_NONE) {
        bookingMenu();
        return;
    }
    if (TownPlayerManager::getSingleton()->isLock() == 0) {
        encount::Encount::getSingleton()->execDungeon();
    }
    g_cmnPartyInfo.prevFrameBattle_ = 0;
    trigger_ = 1;
}

ARM void TownSystem::draw()
{
    cmn::CommonChapterTitle::getSingleton()->draw();
    UnkScreenEffectManager::getSingleton()->unkfunc_0202ad98();
    if (UnkScreenEffectManager::getSingleton()->unkfunc_0202af54()) {
        return;
    }
    TownCamera::getSingleton()->draw();
    TownOpeningManager::getSingleton()->draw();
    TownEndrollManager::getSingleton()->draw();
    bool cameraNo = TownCamera::getSingleton()->camera_.m_cameraNo == 0;
    if (cameraNo == true) {
        TownStageManager::getSingleton()->stage_.m_fld.mainCameraFlag_ = 1;
    } else {
        TownStageManager::getSingleton()->stage_.m_fld.mainCameraFlag_ = 0;
    }
    TownPlayerManager::getSingleton()->draw();
    render_.unkfunc_02084fa4();
    TownStageManager::getSingleton()->draw();
    TownRiseupManager::getSingleton()->draw();
    TownCharacterManager::getSingleton()->draw();
    TownFurnitureManager::getSingleton()->draw();
}

ARM void TownSystem::bookingMenu()
{
    switch (g_Global.bookingFlag_) {
    case Global::BOOKING_CHURCH:
        TownWindowSystem::getSingleton()->changeShopMenuPhase(0x20);
        return;
    case Global::BOOKING_INN:
        if (fadeCount_ == 0) {
            cmn::GameManager::getSingleton()->playerManager_->setLock(1);
            cmn::GameManager::getSingleton()->playerManager_->charaColl_ = 0;
            BillboardCharacter::setAllCharaAnim(0);
            fadeCount_++;
            return;
        }
        if (fadeCount_ <= 30) {
            fadeCount_++;
            return;
        }
        TownWindowSystem::getSingleton()->changeShopMenuPhase(0);
        return;
    case Global::BOOKING_HOSTAGE:
        if (fadeCount_ == 0) {
            cmn::GameManager::getSingleton()->playerManager_->setLock(1);
            cmn::GameManager::getSingleton()->playerManager_->charaColl_ = 0;
            BillboardCharacter::setAllCharaAnim(0);
            TownCharacterManager::getSingleton()->setPlayerDirection(gMaterielMenuExtra_ChangeHostage.ctrlID_);
            SoundManager::playSe(0x136, 0);
            g_Global.fadeOutBlack(1);
            fadeCount_++;
        }
        if (fadeCount_ == 60) {
            g_Global.fadeInBlack(30);
            fadeCount_++;
        }
        if (fadeCount_ <= 90) {
            fadeCount_++;
            return;
        }
        TownWindowSystem::getSingleton()->changeShopMenuPhase(0x17);
        return;
    case Global::BOOKING_GAMESET:
        if (fadeCount_ == 0) {
            cmn::GameManager::getSingleton()->playerManager_->setLock(1);
            cmn::GameManager::getSingleton()->playerManager_->charaColl_ = 0;
            BillboardCharacter::setAllCharaAnim(0);
            fadeCount_++;
        }
        if (data_020f21f8.count_ != data_020f21f8.frames_) {
            return;
        }
        TownWindowSystem::getSingleton()->changeShopMenuPhase(1);
        return;
    }
}
