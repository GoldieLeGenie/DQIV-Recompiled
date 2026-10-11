#pragma ipa file
#include "ov016/TownMenu_ITEM/TownMenu_ITEM_USE.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemSelectChara.hpp"
#include "ov016/TownMenu_ROOT/TownMenu_ROOT.hpp"
#include "ov016/TownMenuPlayerControl/TownMenuPlayerControl.hpp"
#include "ov016/status/PlayerItemInfo.hpp"
#include "ov016/status/FukuroItemInfo.hpp"
#include "ov000/town/TownWindowSystem.hpp"
#include "ov001/window/FieldWindowSystem.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/MenuAPI.hpp"
#include "ov016/UnkTownMenu_02176fa0/UnkTownMenu_02176fa0.hpp"
#include "main/window/MenuControl.hpp"
#include "main/cmn/GameManager.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/UseActionParam.hpp"
#include "main/status/UseActionMacro.hpp"
#include "main/text/TextAPI.hpp"
#include "ov016/MenuTemplate/MenuTemplate_town.hpp"
#include "ov016/UnkTownMenuDraw/UnkTownMenuDraw_0217d560.hpp"

THUMB void TownMenu_ITEM_USE::menuSetup()
{
    status::g_Party.setBattleMode();
    charaItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    pageItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
    charaItem_.active_ = 0;
    unkfunc_02171e8c();
    navigator_.setupBase();
    page_ = 0;
    updateHP_ = 0;
    updateMP_ = 0;
    openMessage_ = 0;
    openCharaSelect_ = 0;
    conditionPoison_ = 0;
    for (int i = 0; i < 4; i++) {
        resultMes_[i] = 0;
    }
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        char page = TownMenuPlayerControl::getSingleton()->activeItemPage_;
        itemID_ = status::FukuroItemInfo::getItemId(TownMenuPlayerControl::getSingleton()->activeItem_, page);
    } else {
        status::g_Party.setPlayerMode();
        int itemIndex = TownMenuPlayerControl::getSingleton()->getActiveItemIndexToAll();
        status::g_Party.setBattleMode();
        itemID_ = status::PlayerItemInfo::getItemIndex(activeChara_, itemIndex);
    }
}

THUMB void TownMenu_ITEM_USE::menuExecute()
{
    status::g_Party.setBattleMode();
    int count = status::g_Party.getCount() - page_ * 4;
    if (count > 4) {
        count = 4;
    }
    MenuTemplate_town::townMenuPageTargetChara(&charaItem_, count, charaItem_.active_);
    if (status::g_Party.getCount() > 4) {
        MenuTemplate_town::townMenuPageCenter(&pageItem_, page_);
    }
    MenuTemplate_town::TOWN_CANCEL(&cancelItem_);
}

THUMB void TownMenu_ITEM_USE::menuDraw()
{
    unkfunc_0217d7f8(charaItem_.active_, itemID_, page_);
    if (!data_020ed1bc.isOpen()) {
        charaItem_.drawActive();
        cancelItem_.drawActive();
        pageItem_.drawActive();
    }
}

THUMB void TownMenu_ITEM_USE::menuUpdate()
{
    status::g_Party.setBattleMode();
    int active = charaItem_.active_;
    if (data_020ed1bc.isOpen()) {
        if (data_020ed1bc.stat_ == MENUBASE_STAT_OK) {
            if (useHealItem() == 1 && openMessage_ == 1) {
                if (itemID_ == 0x80) {
                    if (isResult_ == 1) {
                        openHealMessage();
                        return;
                    }
                } else {
                    openHealMessage();
                    return;
                }
            }
            data_020ed1bc.close();
            close();
            if (itemID_ == 0x82 && isResult_ == 1) {
                gTownMenu_ROOT.stat_ = MENUBASE_STAT_OK;
                if (data_0210bb94.unkfunc_02058114(0xc) == 1) {
                    TownWindowSystem::getSingleton()->cmdWindow_.menuRefresh();
                } else if (data_0210bb94.unkfunc_02058114(0xe) == 1) {
                    FieldWindowSystem::getSingleton()->cmdWindow_.menuRefresh();
                }
                cmn::GameManager::getSingleton()->resetParty();
                return;
            }
            if (openCharaSelect_ == 1) {
                gTownMenuItemSelectChara.open();
                return;
            }
            gUnkTownMenu_02176fa0.open();
        }
        return;
    }
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        close();
        gUnkTownMenu_02176fa0.open();
        redraw_ = 1;
        return;
    }
    navigator_.setup(2, 2, status::g_Party.getCount());
    int result = MenuUpdate_Assist::menuSelect(charaItem_, navigator_);
    if (result != 0) {
        if (result == 2) {
            itemUse();
            return;
        }
        page_ = navigator_.getPageNo();
        redraw_ = 1;
        return;
    }
    if (MenuUpdate_Assist::isPageFlip(pageItem_, navigator_, active)) {
        charaItem_.active_ = active;
        page_ = navigator_.getPageNo();
        redraw_ = 1;
    }
}

THUMB void TownMenu_ITEM_USE::unkfunc_02171e8c()
{
    status::g_Party.setPlayerMode();
    int chara = TownMenuPlayerControl::getSingleton()->activeChara_;
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        status::g_Party.setBattleMode();
        activeChara_ = status::g_Party.getCount();
        return;
    }
    unsigned short playerIndex = status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveStatus_.playerIndex_;
    status::g_Party.setBattleMode();
    for (unsigned char i = 0; i < status::g_Party.getCount(); i++) {
        if (!status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.isBattleNpc_) {
            if (playerIndex == status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_) {
                activeChara_ = i;
                return;
            }
        }
    }
}

THUMB void TownMenu_ITEM_USE::itemUse()
{
    status::g_Party.setPlayerMode();
    int itemIndex = TownMenuPlayerControl::getSingleton()->getActiveItemIndexToAll();
    status::g_Party.setBattleMode();
    int playerIndex = navigator_.getIndex(charaItem_.active_);
    int useMes[4] = { -1, -1, -1, -1 };
    int resultMes[4] = { -1, -1, -1, -1 };
    int mesCount = 0;
    status::UseActionParam useActionParam;
    updateHP_ = status::g_Party.getPlayerStatus(playerIndex)->haveStatusInfo_.getHp();
    updateMP_ = status::g_Party.getPlayerStatus(playerIndex)->haveStatusInfo_.getMp();
    if (status::g_Party.getPlayerStatus(playerIndex)->haveStatusInfo_.getCondition() == 6) {
        conditionPoison_ = 1;
    }
    data_020ed1bc.openMessageForMENU();
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        int count = 0;
        while (status::g_Party.getPlayerStatus(count)->haveStatusInfo_.haveStatus_.isBattleNpc_ || status::g_Party.getPlayerStatus(count)->haveStatusInfo_.isDeath()) {
            count++;
            if (count > status::g_Party.getCount()) {
                close();
                gTownMenuItemSelectChara.open();
                return;
            }
        }
        TextAPI::setMACRO0(1, 0x50000000, itemID_ == 0x66 ? status::g_Party.getPlayerStatus(playerIndex)->haveStatusInfo_.haveStatus_.playerIndex_ : status::g_Party.getPlayerStatus(count)->haveStatusInfo_.haveStatus_.playerIndex_);
        if (checkUseCharaAlive(playerIndex) == 1) {
            return;
        }
        isResult_ = status::FukuroItemInfo::useFukuroItem(useActionParam, itemIndex, useMes, resultMes, playerIndex);
        TownMenuPlayerControl::getSingleton()->setFukuroActiveItemByChangeMax();
        if (status::FukuroItemInfo::getItemMaxCount() == 0) {
            openCharaSelect_ = 1;
        }
    } else {
        TextAPI::setMACRO0(1, 0x50000000, itemID_ == 0x66 ? status::g_Party.getPlayerStatus(playerIndex)->haveStatusInfo_.haveStatus_.playerIndex_ : status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveStatus_.playerIndex_);
        if (checkUseCharaAlive(playerIndex) == 1) {
            return;
        }
        isResult_ = status::PlayerItemInfo::usePlayerItem(useActionParam, activeChara_, itemIndex, useMes, resultMes, playerIndex);
        status::g_Party.setPlayerMode();
        TownMenuPlayerControl::getSingleton()->setPlayerActiveItemByChangeMax();
        status::g_Party.setBattleMode();
        if (status::PlayerItemInfo::getItemMaxCount(activeChara_) == 0) {
            openCharaSelect_ = 1;
        }
    }
    short nowHP = status::g_Party.getPlayerStatus(playerIndex)->haveStatusInfo_.getHp();
    short nowMP = status::g_Party.getPlayerStatus(playerIndex)->haveStatusInfo_.getMp();
    status::g_Party.getPlayerStatus(playerIndex)->haveStatusInfo_.setHp(updateHP_);
    status::g_Party.getPlayerStatus(playerIndex)->haveStatusInfo_.setMp(updateMP_);
    updateHP_ = nowHP;
    updateMP_ = nowMP;
    if (conditionPoison_ == 1) {
        status::g_Party.getPlayerStatus(playerIndex)->haveStatusInfo_.setCondition((status::HaveStatusInfo::Condition)6);
    }
    status::UseActionMacro::setExecMacro(useActionParam.actorCharacterStatus_, useActionParam.targetCharacterStatus_[0], useActionParam.actionIndex_);
    status::UseActionMacro::setResultMacro(useActionParam.actorCharacterStatus_, useActionParam.targetCharacterStatus_[0], useActionParam.actionIndex_);
    status::g_Party.getPlayerStatus(playerIndex);
    TextAPI::setMACRO0(10, 0x40000000, itemID_);
    data_020ed1bc.addMessage(useMes[0]);
    openMessage_ = 1;
    while (resultMes[mesCount] != -1) {
        if (mesCount > 4) {
            break;
        }
        if (useHealItem() == 0) {
            data_020ed1bc.addMessage(resultMes[mesCount]);
        } else {
            resultMes_[mesCount] = resultMes[mesCount];
            data_020ed1bc.setMessageCursor(true);
        }
        mesCount++;
    }
    if (itemID_ == 0x82 && isResult_ == 1) {
        if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
            if (status::g_Party.haveItemSack_.getCount() == 0) {
                window::MenuControl::menu_ = 3;
            } else {
                window::MenuControl::menu_ = 2;
            }
        } else {
            if (status::PlayerItemInfo::getItemMaxCount(activeChara_) == 0) {
                window::MenuControl::menu_ = 3;
            } else {
                window::MenuControl::menu_ = 2;
            }
        }
        TownMenuPlayerControl::getSingleton()->initializeLock_ = 1;
    }
    g_cmnPartyInfo.actorIndex_ = useActionParam.actorCharacterStatus_->haveStatusInfo_.haveStatus_.playerIndex_;
    g_cmnPartyInfo.targetIndex_ = useActionParam.targetCharacterStatus_[0]->haveStatusInfo_.haveStatus_.playerIndex_;
}

THUMB int TownMenu_ITEM_USE::checkUseCharaAlive(int target)
{
    if (itemID_ == 0x6f || itemID_ == 0x66 || itemID_ == 0x7e || itemID_ == 0x80) {
        if (status::g_Party.getPlayerStatus(target)->haveStatusInfo_.isDeath()) {
            int mesNo = 0xc3ced;
            if (itemID_ == 0x66) {
                mesNo += 0xf;
            }
            if (itemID_ == 0x80) {
                mesNo = 0xc3ddd;
            }
            TextAPI::setMACRO0(10, 0x40000000, itemID_);
            TextAPI::setMACRO0(0x12, 0x50000000, status::g_Party.getPlayerIndex(target));
            data_020ed1bc.addMessage(mesNo, 0xc3dd4);
            return 1;
        }
    }
    return 0;
}

THUMB int TownMenu_ITEM_USE::useHealItem()
{
    if (itemID_ == 0x80 && resultMes_[0] == 0) {
        return 1;
    }
    if (itemID_ == 0x6f || itemID_ == 0x80 || itemID_ == 0x66 || itemID_ == 0x7e || itemID_ == 0x82 || itemID_ == 0x70) {
        return 1;
    }
    return 0;
}

THUMB void TownMenu_ITEM_USE::openHealMessage()
{
    int target = navigator_.getIndex(charaItem_.active_);
    data_020ed1bc.restartMessage();
    for (int count = 0; resultMes_[count] != 0; count++) {
        TextAPI::setMACRO0(0x12, 0x50000000, status::g_Party.getPlayerIndex(target));
        TextAPI::setMACRO0(0x51, 0xf0000000, status::g_Party.getPlayerStatus(target)->haveStatusInfo_.effectValue_);
        data_020ed1bc.addMessage(resultMes_[count]);
    }
    if (isResult_ == 1 && itemID_ != 0x70) {
        SoundManager::playSe(0x1f5, 0);
    }
    status::g_Party.getPlayerStatus(target)->haveStatusInfo_.setHp(updateHP_);
    status::g_Party.getPlayerStatus(target)->haveStatusInfo_.setMp(updateMP_);
    if (conditionPoison_ == 1 && itemID_ == 0x70) {
        status::g_Party.getPlayerStatus(target)->haveStatusInfo_.detoxPoison();
    }
    openMessage_ = 0;
    redraw_ = 1;
}
