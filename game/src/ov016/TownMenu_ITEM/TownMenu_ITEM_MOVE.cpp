#include "ov016/TownMenu_ITEM/TownMenu_ITEM_MOVE.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemSelectChara.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemSelectCommand.hpp"
#include "ov016/TownMenu_MAGIC/TownMenu_MAGIC_MOVE.hpp"
#include "ov016/TownMenu_ROOT/TownMenu_ROOT.hpp"
#include "ov016/TownMenuPlayerControl/TownMenuPlayerControl.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/MenuAPI.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/cmn/CommonRuraData.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/status/UseItem.hpp"
#include "main/status/UseActionParam.hpp"
#include "main/status/UseActionMacro.hpp"
#include "main/text/TextAPI.hpp"
#include "ov016/MenuTemplate/MenuTemplate_town.hpp"
#include "ov016/UnkTownMenuDraw/UnkTownMenuDraw_0217d560.hpp"

THUMB void TownMenu_ITEM_MOVE::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    pageItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
    navigator_.setupBase();
    moveCount_ = cmn::CommonRuraData::getSingleton()->getRuraCount();
    unsigned char count = 0;
    rulaOK_ = 0;
    if (status::g_Story.chapter_ < 5) {
        int chapterRuraCount = cmn::CommonRuraData::getSingleton()->getChapterRuraCount();
        for (int i = 0; i < chapterRuraCount; i++) {
            int townNo = cmn::CommonRuraData::getSingleton()->isEnableRuraBeforeChapter5(i);
            if (townNo != -1) {
                moveTown_[count] = townNo;
                count++;
            }
        }
    } else {
        for (unsigned char i = 0; i < 29; i++) {
            if (cmn::CommonRuraData::getSingleton()->isEnableRura(i) == 1) {
                moveTown_[count] = i;
                count++;
            }
        }
    }
    if (moveCount_ == 0) {
        g_Stage.setRulaDisable(1);
    }
}

THUMB void TownMenu_ITEM_MOVE::menuExecute()
{
    int count = moveCount_ - navigator_.getPageNo() * 8;
    if (count > 8) {
        count = 8;
    }
    MenuTemplate_town::townMenuPageRightTwoArrow(&pageItem_, pageItem_.active_);
    MenuTemplate_town::townMenuSelectMagic(&menuItem_, count, menuItem_.active_);
    MenuTemplate_town::TOWN_CANCEL(&cancelItem_);
}

THUMB void TownMenu_ITEM_MOVE::menuDraw()
{
    int activeChara = TownMenuPlayerControl::getSingleton()->activeChara_;
    unkfunc_0217dac4(-1, -1, activeChara, navigator_.getPageNo(), moveTown_);
    if (!data_020ed1bc.isOpen()) {
        menuItem_.drawActive();
    }
}

THUMB void TownMenu_ITEM_MOVE::menuUpdate()
{
    if (data_020ed1bc.isOpen()) {
        if (data_020ed1bc.stat_ == MENUBASE_STAT_OK) {
            data_020ed1bc.close();
            close();
            gTownMenu_ROOT.stat_ = MENUBASE_STAT_OK;
            if (rulaOK_ != 0) {
                g_Stage.setRuraFlag(1);
                g_Stage.setRuraTownID(moveTown_[menuItem_.getActive() + navigator_.getPageNo() * 8]);
                return;
            }
            if (!g_Stage.isRulaDisable() && !g_Stage.isRula()) {
                g_cmnPartyInfo.setMenuAction((cmn::MENU_ACTION)2);
            }
        }
        return;
    }
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        unk_148.setPageNo(0);
        close();
        gTownMenuItemSelectCommand.open();
        redraw_ = 1;
        return;
    }
    navigator_.setup(2, 4, moveCount_);
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result != 0) {
        if (result == 2) {
            moveTown();
            return;
        }
        redraw_ = 1;
        return;
    }
    int active = menuItem_.active_;
    if (MenuUpdate_Assist::isPageFlip(pageItem_, navigator_, active)) {
        menuItem_.active_ = active;
        redraw_ = 1;
    }
}

THUMB void TownMenu_ITEM_MOVE::moveTown()
{
    int activeChara = TownMenuPlayerControl::getSingleton()->activeChara_;
    int itemIndex = TownMenuPlayerControl::getSingleton()->getActiveItemIndexToAll();
    int message[4] = { 0 };
    int resMessage[4] = { 0 };
    int index = 0;
    data_020ed1bc.openMessageForMENU();
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        int maxCharaCount = status::g_Party.getCount();
        while (status::g_Party.getPlayerStatus(index)->haveStatusInfo_.isDeath()) {
            index++;
            if (index > maxCharaCount) {
                close();
                gTownMenuItemSelectChara.open();
            }
        }
        TextAPI::setMACRO0(1, 0x50000000, status::g_Party.getPlayerIndex(index));
    } else {
        TextAPI::setMACRO0(1, 0x50000000, status::g_Party.getPlayerIndex(activeChara));
    }
    status::UseActionParam useActionParam;
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        useActionParam.actorHaveItemSack_ = &status::g_Party.haveItemSack_;
        useActionParam.actorCharacterStatus_ = status::g_Party.getPlayerStatus(index);
    } else {
        useActionParam.actorCharacterStatus_ = status::g_Party.getPlayerStatus(activeChara);
    }
    useActionParam.targetCount_ = 1;
    useActionParam.targetCharacterStatus_[0] = status::g_Party.getPlayerStatus(activeChara);
    useActionParam.itemSortIndex_ = itemIndex;
    status::UseItem::execUse(&useActionParam);
    status::UseActionMacro::setExecMacro(useActionParam.actorCharacterStatus_, useActionParam.targetCharacterStatus_[0], useActionParam.actionIndex_);
    status::UseActionMacro::setResultMacro(useActionParam.actorCharacterStatus_, useActionParam.targetCharacterStatus_[0], useActionParam.actionIndex_);
    TextAPI::setMACRO0(10, 0x40000000, 0x72);
    status::UseActionMessage& useActionMessage = useActionParam.message_[0];
    message[0] = useActionMessage.execMessage_[0];
    TownMenuPlayerControl::getSingleton()->setActiveCharaIndex(useActionParam.actorCharacterStatus_->haveStatusInfo_.haveStatus_.playerIndex_);
    int count = 0;
    while (message[count] != 0) {
        MenuAPI::addMessageSerial(message[count]);
        count++;
        message[count] = useActionMessage.execMessage_[count];
    }
    if (g_Stage.isRulaDisable()) {
        resMessage[0] = useActionMessage.resultMessage_[0];
        int messageCount = 0;
        while (resMessage[messageCount] != 0) {
            MenuAPI::addMessageSerial(resMessage[messageCount]);
            messageCount++;
            resMessage[messageCount] = useActionMessage.resultMessage_[messageCount];
        }
    }
    rulaOK_ = useActionParam.result_;
    g_cmnPartyInfo.actorIndex_ = useActionParam.actorCharacterStatus_->haveStatusInfo_.haveStatus_.playerIndex_;
    g_cmnPartyInfo.targetIndex_ = useActionParam.targetCharacterStatus_[0]->haveStatusInfo_.haveStatus_.playerIndex_;
}
