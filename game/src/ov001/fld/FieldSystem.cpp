#pragma ipa file
#include "ov001/fld/FieldSystem.hpp"
#ifdef DQIV_NATIVE
#include "DualScreen.hpp"
#endif
#include "ov001/fld/FieldStage.hpp"
#include "ov001/fld/FieldPlayerManager.hpp"
#include "ov001/fld/FieldSymbolManager.hpp"
#include "ov001/window/FieldWindowSystem.hpp"
#include "main/cmn/CommonChapterTitle.hpp"
#include "main/cmn/WorldLocation.hpp"
#include "main/cmn/GameManager.hpp"
#include "main/cmn/ExtraMapLink.hpp"
#include "main/cmn/TalkSoundManager.hpp"
#include "main/cmn/PartyTalk.hpp"
#include "main/cmn/NonBattleActionManager.hpp"
#include "main/btl/BattleScriptManager.hpp"
#include "main/script/ScriptSystem.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/GameFlag.hpp"
#include "main/status/Status.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/global/Global.hpp"
#include "main/global/StageLink.hpp"
#include "main/encount/Encount.hpp"

ARM FieldSystem::FieldSystem()
{
}

ARM FieldSystem* FieldSystem::getSingleton()
{
    static FieldSystem m_singleton;
    return &m_singleton;
}

ARM void FieldSystem::initialize()
{
    g_Global.fieldType_ = g_Global.nextFieldType_;
    status::excelParam.setupField();
    status::excelParam.setupFieldInitialize();
    render_.unkfunc_02084efc();
    cmn::WorldLocation::getSingleton()->initialize();
    fld::FieldStage::getSingleton()->initialize();
    FieldPlayerManager::getSingleton()->initialize();
    cmn::g_extraMapLink.setup();
    cmn::GameManager::getSingleton()->initialize();
    FieldPlayerManager* player = FieldPlayerManager::getSingleton();
    cmn::GameManager::getSingleton()->playerManager_ = player;
    cmn::g_talkSound.setup();
    FieldPlayerManager::getSingleton()->setup();
    encount::Encount::getSingleton()->setup(EncountDungeon, Floor);
    if (g_Stage.isEncount()) {
        btl::BattleScriptManager::getSingleton()->checkScriptBattleResult();
    }
    fld::FieldStage::getSingleton()->ChangeTime(1);
    g_Stage.setupField();
    ScriptSystem::getSingleton()->initialize(status::g_Story.chapter_);
    FieldSymbolManager::getSingleton()->initialize();
    encountStart_ = 0;
    scriptLock_ = 0;
    if (g_AreaFlag.check(0x13d)) {
        status::g_Party.ship_ = 1;
    } else {
        status::g_Party.ship_ = 0;
    }
    if (g_AreaFlag.check(0x172)) {
        status::g_Party.balloon_ = 1;
    } else {
        status::g_Party.balloon_ = 0;
    }
    SoundManager::fieldPlay();
    ScreenEffectManager::getSingleton()->initialize();
    int id;
    if (g_Stage.ruraFlag_ != 0) {
        id = -1;
    } else {
        id = StageLink::getTownExitIndex();
    }
    if (id != -1) {
        cmn::PartyTalk::getSingleton()->resetPartyTalk();
        cmn::PartyTalk::getSingleton()->setExitNo(id);
    }
    status::excelParam.cleanupFieldInitialize();
    exitSound_ = 0;
    g_Stage.loadType_ = profile::SAVETYPE_INVALID;
}

ARM void FieldSystem::terminate()
{
    cmn::CommonChapterTitle::getSingleton()->cleanup();
    ScriptSystem::getSingleton()->terminate();
    if (!g_Global.isNextPart(13)) {
        g_GlobalFlag.clear();
        g_Stage.playerLockCount_ = 0;
        g_Stage.idoLink_.data_.link_.encount_ = 0;
        if (exitSound_ == 1) {
            Sound::sePlayDirect(0x131);
        }
        if (g_Global.isNextPart(12) == true) {
            int exitIndex = StageLink::getTownExitIndex();
            g_Stage.setRanaMap(exitIndex);
        }
        if (!g_Global.isNextPart(16)) {
            status::StageStatus::setToramana(0);
        }
    } else {
        g_Stage.idoLink_.data_.link_.encount_ = 1;
        FieldPlayerManager::getSingleton()->resetLockByEventEncount();
        g_Stage.playerLockCount_ = FieldPlayerManager::getSingleton()->getLockCount();
    }
    FieldSymbolManager::getSingleton()->terminate();
    ScreenEffectManager::getSingleton()->terminate();
    FieldPlayerManager::getSingleton()->cleanup();
    cmn::GameManager::getSingleton()->terminate();
    FieldPlayerManager::getSingleton()->terminate();
    fld::FieldStage::getSingleton()->terminate();
    cmn::WorldLocation::getSingleton()->terminate();
    render_.unkfunc_02084f50();
    status::Status::setFlagShopExec();
    status::excelParam.cleanupField();
    g_Global.partChangeFlag_ = 0;
}

ARM void FieldSystem::execute()
{
    if (g_Global.getRanarutaFlag()) {
        ScreenEffectManager::getSingleton()->execute();
        cmn::NonBattleActionManager::getSingleton()->execute();
    }
    if (FieldWindowSystem::getSingleton()->isOpen()) {
        return;
    }
    if (!FieldPlayerManager::getSingleton()->isLock()) {
        execEncount();
    }
    FieldPlayerManager::getSingleton()->execute();
    fld::FieldStage::getSingleton()->execute();
    dss::Fix32Vector3 pos = FieldPlayerManager::getSingleton()->getPosition();
    if (!cameraLock_) {
        setLookAtPos(pos);
    }
    cmn::GameManager::getSingleton()->execute();
    if (encount::Encount::getSingleton()->isEncounted()) {
        return;
    }
    ScriptSystem::getSingleton()->execute();
}

ARM void FieldSystem::draw()
{
    cmn::CommonChapterTitle::getSingleton()->draw();
    ScreenEffectManager::getSingleton()->draw();
    if (ScreenEffectManager::getSingleton()->isEffectDrawing()) {
        return;
    }
#ifdef DQIV_NATIVE
    // Wide field: its bottom-screen sprites (symbols, party, ships) are drawn
    // every frame.
    pc::setSpritesOnBothScreens(pc::wideFieldFrame());
#endif
    fld::FieldStage::getSingleton()->draw();
    fld::FieldStage::getSingleton()->drawPlayer();
    FieldPlayerManager::getSingleton()->draw();
#ifdef DQIV_NATIVE
    pc::setSpritesOnBothScreens(false);
    pc::setDrawLayer(1); // the top screen's map: an inset in the wide field
#endif
    FieldWindowSystem::getSingleton()->draw();
#ifdef DQIV_NATIVE
    pc::setDrawLayer(0);
#endif
    render_.unkfunc_02084fa4();
}

ARM void FieldSystem::execEncount()
{
    dss::Fix32Vector3 posEncount = FieldPlayerManager::getSingleton()->getPosition();
    int bx = posEncount.vx.value / 0x100000;
    int by = posEncount.vy.value / 0x100000;
    LandType land = FieldPlayerManager::getSingleton()->getLandType();
    g_Stage.setBtlMapNameOnField(land);
    encount::Encount::getSingleton()->setStage("field", land);
    switch (g_Global.getFieldType()) {
    case 1:
        encount::Encount::getSingleton()->setBlockGot(0, 0);
        break;
    case 2:
        encount::Encount::getSingleton()->setBlockYami(0, 0);
        break;
    default:
        encount::Encount::getSingleton()->setBlock(bx, by);
        break;
    }
    if (encount::Encount::getSingleton()->isEncounted()) {
        encount::Encount::getSingleton()->exec();
        if (encount::Encount::getSingleton()->isEncountedNext() && encount::Encount::getSingleton()->brew()) {
            encount::Encount::getSingleton()->setCrusingPeople();
            g_Global.startBattle();
            g_Stage.idoLink_.data_.link_.encount_ = 1;
            if (FieldPlayerManager::getSingleton()->player_.getMoveType() == FieldPlayer::MOVE_SHIP) {
                g_Stage.idoLink_.data_.link_.shipEncount_ = 1;
            }
            FieldPlayerManager::getSingleton()->savePartyDrawInfo();
        }
    }
    FieldPlayerManager::getSingleton()->getPosition();
}

ARM void FieldSystem::setLookAtPos(dss::Fix32Vector3 pos)
{
    pos.vx -= dss::Fix32(128L);
    pos.vy -= dss::Fix32(96L);
    fld::FieldStage::getSingleton()->setPosition(pos);
}
