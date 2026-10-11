#include "ov016/TownMenu_MAGIC/TownMenu_MAGIC_MOVE.hpp"
#include "ov016/TownMenu_MAGIC/TownMenu_MAGIC_ROOT.hpp"
#include "ov016/TownMenu_ROOT/TownMenu_ROOT.hpp"
#include "ov016/TownMenuPlayerControl/TownMenuPlayerControl.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/cmn/CommonRuraData.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/status/UseAction.hpp"
#include "main/status/UseActionParam.hpp"
#include "main/status/UseActionMacro.hpp"
#include "main/text/TextAPI.hpp"
#include "ov016/MenuTemplate/MenuTemplate_town.hpp"
#include "ov016/UnkTownMenuDraw/UnkTownMenuDraw_0217d560.hpp"

THUMB void TownMenu_MAGIC_MOVE::menuSetup()
{
    status::g_Party.setBattleMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    moveAllocation_ = 0;
    navigator_.setupBase();
    moveCount_ = cmn::CommonRuraData::getSingleton()->getRuraCount();
    pageStart_ = 0;
    isResult_ = 0;
    pageMax_ = moveCount_ / 8;
    for (int i = 0; i < 29; i++) {
        moveTown_[i] = 0;
    }
    int moveMaxNum = 0;
    if (status::g_Story.chapter_ < 5) {
        int chapterRuraCount = cmn::CommonRuraData::getSingleton()->getChapterRuraCount();
        for (int i = 0; i < chapterRuraCount; i++) {
            int townNo = cmn::CommonRuraData::getSingleton()->isEnableRuraBeforeChapter5(i);
            if (townNo != -1) {
                moveTown_[moveMaxNum] = townNo;
                moveMaxNum++;
            }
        }
    } else {
        for (int i = 0; i < 29; i++) {
            if (cmn::CommonRuraData::getSingleton()->isEnableRura(i) == 1) {
                moveTown_[moveMaxNum] = i;
                moveMaxNum++;
            }
        }
    }
    if (moveCount_ == 0) {
        g_Stage.setRulaDisable(1);
    }
}

THUMB void TownMenu_MAGIC_MOVE::menuExecute()
{
    status::g_Party.setBattleMode();
    int count;
    if (pageMax_ == pageStart_) {
        count = moveCount_ % 8;
    } else {
        count = 8;
    }
    MenuTemplate_town::townMenuSelectMagic(&menuItem_, count, moveAllocation_);
    MenuTemplate_town::TOWN_CANCEL(&cancelItem_);
}

THUMB void TownMenu_MAGIC_MOVE::menuDraw()
{
    status::g_Party.setBattleMode();
    unkfunc_0217dac4(status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.getMp(), status::UseAction::getUseMp(0xcb), activeChara_, pageStart_, moveTown_);
    if (!data_020ed1bc.isOpen()) {
        menuItem_.drawActive();
        cancelItem_.drawActive();
    }
}

THUMB void TownMenu_MAGIC_MOVE::menuUpdate()
{
    status::g_Party.setBattleMode();
    if (data_020ed1bc.isOpen()) {
        if (data_020ed1bc.stat_ == MENUBASE_STAT_OK) {
            data_020ed1bc.close();
            gTownMenu_MAGIC_ROOT.mode_ = 0;
            close();
            gTownMenu_ROOT.stat_ = MENUBASE_STAT_OK;
            if (isResult_ != 0) {
                g_Stage.setRuraFlag(1);
                g_Stage.setRuraTownID(moveTown_[moveAllocation_ + pageStart_ * 8]);
                return;
            }
            if (!g_Stage.isRula() && !g_Stage.isRulaDisable()) {
                g_cmnPartyInfo.setMenuAction((cmn::MENU_ACTION)2);
            }
        }
        return;
    }
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        TownMenuPlayerControl::getSingleton()->setActiveChara(activeChara_);
        TownMenuPlayerControl::getSingleton()->setActiveMagic(activeMagic_ % 8);
        close();
        gTownMenu_MAGIC_ROOT.open();
        gTownMenu_MAGIC_ROOT.unkfunc_021797a8();
        gTownMenu_MAGIC_ROOT.mode_ = 1;
        gTownMenu_MAGIC_ROOT.getUseAction();
        redraw_ = 1;
        return;
    }
    navigator_.setup(2, 4, moveCount_);
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result != 0) {
        if (result == 2) {
            moveTown();
            menuItem_.result_ = 0;
            menuItem_.lastresult_ = 0;
        }
        moveAllocation_ = menuItem_.active_;
        pageStart_ = navigator_.getPageNo();
        redraw_ = 1;
    }
}

THUMB void TownMenu_MAGIC_MOVE::moveTown()
{
    status::g_Party.setBattleMode();
    status::UseActionParam useActionParam;
    int message[4] = { 0 };
    int resMessage[4] = { 0 };
    useActionParam.clear();
    useActionParam.actorCharacterStatus_ = status::g_Party.getPlayerStatus(activeChara_);
    useActionParam.targetCount_ = 1;
    useActionParam.targetCharacterStatus_[0] = status::g_Party.getPlayerStatus(activeChara_);
    useActionParam.actionIndex_ = magicID_;
    status::UseAction::execUse(&useActionParam);
    isResult_ = useActionParam.result_;
    g_cmnPartyInfo.actorIndex_ = useActionParam.actorCharacterStatus_->haveStatusInfo_.haveStatus_.playerIndex_;
    g_cmnPartyInfo.targetIndex_ = useActionParam.targetCharacterStatus_[0]->haveStatusInfo_.haveStatus_.playerIndex_;
    SoundManager::playSe(0x132, 0);
    data_020ed1bc.openMessageForMENU();
    status::UseActionMacro::setExecMacro(useActionParam.actorCharacterStatus_, useActionParam.targetCharacterStatus_[0], useActionParam.actionIndex_);
    status::UseActionMacro::setResultMacro(useActionParam.actorCharacterStatus_, useActionParam.targetCharacterStatus_[0], useActionParam.actionIndex_);
    TownMenuPlayerControl::getSingleton()->setActiveCharaIndex(useActionParam.actorCharacterStatus_->haveStatusInfo_.haveStatus_.playerIndex_);
    TextAPI::setMACRO0(1, 0x50000000, status::g_Party.getPlayerIndex(activeChara_));
    status::UseActionMessage& useActionMessage = useActionParam.message_[0];
    for (int i = 0; useActionMessage.execMessage_[i] != 0; i++) {
        data_020ed1bc.addMessage(useActionMessage.execMessage_[i]);
    }
    if (g_Stage.isRulaDisable() == 1) {
        for (int i = 0; useActionMessage.resultMessage_[i] != 0; i++) {
            data_020ed1bc.addMessage(useActionMessage.resultMessage_[i]);
        }
    }
}
