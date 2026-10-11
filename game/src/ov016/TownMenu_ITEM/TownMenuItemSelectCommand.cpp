#pragma ipa file
#include "ov016/TownMenu_ITEM/TownMenuItemSelectCommand.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemMessage.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemSelectChara.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemSelectTargetChara.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemTarotMessage.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemUseManager.hpp"
#include "ov016/TownMenu_ITEM/TownMenu_ITEM_MOVE.hpp"
#include "ov016/TownMenu_ITEM/TownMenu_ITEM_USE.hpp"
#include "ov016/TownMenu_ROOT/TownMenu_ROOT.hpp"
#include "ov016/TownMenuPlayerControl/TownMenuPlayerControl.hpp"
#include "ov016/status/PlayerItemInfo.hpp"
#include "ov016/status/FukuroItemInfo.hpp"
#include "ov001/fld/FieldPlayerManager.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/MenuAPI.hpp"
#include "ov016/UnkTownMenu_02176fa0/UnkTownMenu_02176fa0.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/cmn/CommonRuraData.hpp"
#include "main/cmn/NonBattleActionManager.hpp"
#include "main/global/Global.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/sound/MenuSoundManager.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/status/UseItem.hpp"
#include "main/status/UseActionParam.hpp"
#include "main/text/TextAPI.hpp"
#include "main/text/TextHook.hpp"
#include "ov016/MenuTemplate/MenuTemplate_town.hpp"
#include "ov016/UnkTownMenuDraw/UnkTownMenuDraw_0217d560.hpp"

static int equipMessage_[27] = {
    0xc4c97, 0xc4cb7, 0xc4cda, 0xc4cf7, 0xc4d03, 0xc4d13, 0xc4d19, 0xc4d33, 0xc4d3b,
    0xc4d6d, 0xc4d74, 0xc4d81, 0xc4d8e, 0xc4d96, 0xc4db5, 0xc4dc1, 0xc4dce, 0xc4e10,
    0xc4e18, 0xc4e29, 0xc4e42, 0xc4e49, 0xc4e50, 0xc4e5d, 0xc4e8f, 0xc4ea5, 0xc4fba,
};

THUMB void TownMenuItemSelectCommand::menuSetup()
{
    status::g_Party.setPlayerMode();
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        char page = TownMenuPlayerControl::getSingleton()->activeItemPage_;
        itemID_ = status::FukuroItemInfo::getItemId(TownMenuPlayerControl::getSingleton()->activeItem_, page);
    } else {
        int chara = TownMenuPlayerControl::getSingleton()->activeChara_;
        itemID_ = status::PlayerItemInfo::getItemIndex(chara, TownMenuPlayerControl::getSingleton()->getActiveItemIndexToAll());
    }
    MenuSoundManager::getSingleton()->initialize();
    sound_ = 0;
    isLock_ = 0;
    resetLock_ = 0;
    boots_ = 0;
    isResult_ = 0;
    useItem_ = 0;
    useInoriNoyubiwa_ = 0;
    openMessage_ = 0;
    useItemPlayer_ = 0;
    updataMP_ = 0;
    for (int i = 0; i < 8; i++) {
        resultMes_[i] = -1;
    }
    for (int i = 0; i < 4; i++) {
        updataHP_[i] = -1;
    }
    navigator_.setupBase();
    menuItem_.active_ = TownMenuPlayerControl::getSingleton()->getActiveCommand();
}

THUMB void TownMenuItemSelectCommand::menuExecute()
{
    status::g_Party.setPlayerMode();
    if (resetLock_ == 1) {
        isLock_ = 0;
        resetLock_ = 0;
    }
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        commandFlag_ = status::FukuroItemInfo::getItemFukuroCommandFlag();
    } else {
        int chara = TownMenuPlayerControl::getSingleton()->activeChara_;
        commandFlag_ = status::PlayerItemInfo::getItemPlayerCommandFlag(chara, TownMenuPlayerControl::getSingleton()->getActiveItemIndexToAll());
    }
    MenuTemplate_town::TOWN_CANCEL(&cancelItem_);
    MenuTemplate_town::townMenuItemCommand(&menuItem_, commandFlag_, menuItem_.active_);
}

THUMB void TownMenuItemSelectCommand::menuDraw()
{
    char page = TownMenuPlayerControl::getSingleton()->activeItemPage_;
    unkfunc_0217d6e8(TownMenuPlayerControl::getSingleton()->activeChara_, itemID_, page, sound_);
    if (!data_020ed1bc.isOpen() && sound_ == 0) {
        menuItem_.drawActive();
    }
}

THUMB void TownMenuItemSelectCommand::menuUpdate()
{
    if (isLock_ == 1) {
        return;
    }
    if (data_020ed1bc.isOpen()) {
        if (data_020ed1bc.stat_ == MENUBASE_STAT_OK || data_020ed1bc.stat_ == MENUBASE_STAT_CANCEL) {
            if (isResult_ == 1 && openMessage_ == 1) {
                data_020ed1bc.restartMessage();
                if (itemID_ == 0x83) {
                    SoundManager::playSe(0x1f5, 0);
                    data_020ed1bc.addMessage(0xc3d05);
                    status::g_Party.setBattleMode();
                    for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
                        status::g_Party.getPlayerStatus(i)->haveStatusInfo_.setHp(updataHP_[i]);
                    }
                    data_020ed1bc.setMessageLastCursor(false);
                    status::g_Party.setPlayerMode();
                    openMessage_ = 0;
                    redraw_ = 1;
                    return;
                }
                if (itemID_ == 0x66) {
                    SoundManager::playSe(0x1f5, 0);
                    int activeChara = TownMenuPlayerControl::getSingleton()->activeChara_;
                    TextAPI::setMACRO0(0x12, 0x50000000, status::g_Party.getPlayerIndex(activeChara));
                    TextAPI::setMACRO0(0x51, 0xf0000000, status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.effectValue_);
                    data_020ed1bc.addMessage(0xc3d73);
                    if (resultMes_[2] != -1) {
                        data_020ed1bc.addMessage(resultMes_[2]);
                    }
                    status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.setMp(updataMP_);
                    openMessage_ = 0;
                    redraw_ = 1;
                    return;
                }
            }
            data_020ed1bc.close();
            if (itemID_ == 0x88) {
                if (useItem_ == 1) {
                    MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_AYAKASHI);
                    sound_ = 1;
                }
            } else if (useItem_ == 1) {
                resultItem();
            }
            useItem_ = 0;
        }
        return;
    }
    if (MenuSoundManager::getSingleton()->isPlaySound()) {
        return;
    }
    if (!MenuSoundManager::getSingleton()->isPlaySound() && sound_ != 0) {
        int count = 0;
        sound_ = 0;
        data_020ed1bc.openMessageForMENU();
        while (resultMes_[count] != -1) {
            if (count > 4) {
                break;
            }
            data_020ed1bc.addMessage(resultMes_[count]);
            count++;
        }
        if (count == 0) {
            close();
            gTownMenu_ROOT.stat_ = MENUBASE_STAT_OK;
        }
    }
    if (useItem_ != 0) {
        openUseItemMessage();
        openMessage_ = 1;
    }
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        close();
        gUnkTownMenu_02176fa0.open();
        redraw_ = 1;
        return;
    }
    navigator_.setup(3, 2, unkfunc_02176b90());
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result != 0) {
        if (result == 2) {
            TownMenuPlayerControl::getSingleton()->setActiveCommand(menuItem_.active_);
            unkfunc_0217638c();
        }
        redraw_ = 1;
    }
}

THUMB void TownMenuItemSelectCommand::unkfunc_0217638c()
{
    switch (TownMenuPlayerControl::getSingleton()->activeCommand_) {
        case 0:
            judgeUseItem();
            return;
        case 1:
            close();
            gTownMenuItemSelectTargetChara.open();
            return;
        case 2:
            judgeThrowItem();
            return;
        case 3:
            judgeEquipItem();
            return;
        case 4:
            setItemShowAction();
            return;
        case 5:
            close();
            gUnkTownMenu_02176fa0.open();
            redraw_ = 1;
            return;
    }
}

THUMB void TownMenuItemSelectCommand::judgeUseItem()
{
    int activeChara = TownMenuPlayerControl::getSingleton()->activeChara_;
    if (!TownMenuPlayerControl::getSingleton()->activeFukuro_ && status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.isDeath()) {
        data_020ed1bc.openMessageForMENU();
        TextAPI::setMACRO0(1, 0x50000000, status::g_Party.getPlayerIndex(activeChara));
        TextAPI::setMACRO0(10, 0x40000000, itemID_);
        data_020ed1bc.addMessage(0xc3dd9);
        return;
    }
    if (itemID_ == 0x72) {
        if (cmn::CommonRuraData::getSingleton()->getRuraCount() == 0) {
            if (TownMenuPlayerControl::getSingleton()->activeFukuro_ == 1) {
                activeChara = 0;
                int maxCharaCount = status::g_Party.getCount();
                while (status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.isDeath() == 1) {
                    activeChara++;
                    if (activeChara > maxCharaCount) {
                        activeChara = 0;
                        break;
                    }
                }
            }
            data_020ed1bc.openMessageForMENU();
            TextAPI::setMACRO0(1, 0x50000000, status::g_Party.getPlayerIndex(activeChara));
            TextAPI::setMACRO0(10, 0x40000000, itemID_);
            data_020ed1bc.addMessage(0xc3cf2, 0xc3d6f);
            return;
        }
        close();
        gTownMenu_ITEM_MOVE.open();
        return;
    }
    if (status::UseItem::getUseArea(itemID_) == status::UseItem::One && status::UseItem::getUseType(itemID_) != status::UseItem::Myself) {
        close();
        gTownMenu_ITEM_USE.open();
        return;
    }
    if (itemID_ == 9) {
        unkfunc_02176bfc();
        return;
    }
    if (itemID_ == 0x66 && TownMenuPlayerControl::getSingleton()->activeFukuro_ == 1) {
        close();
        gTownMenu_ITEM_USE.open();
        return;
    }
    useItemNoTarget();
}

THUMB void TownMenuItemSelectCommand::useItemNoTarget()
{
    int mesCount = 0;
    int activeChara = TownMenuPlayerControl::getSingleton()->activeChara_;
    int itemIndex = TownMenuPlayerControl::getSingleton()->getActiveItemIndexToAll();
    status::UseActionParam useActionParam;
    int useMes[4], resultMes[4], nowHP[4];
    for (int i = 0; i < 4; i++) {
        useMes[i] = -1;
        resultMes[i] = -1;
        nowHP[i] = -1;
    }
    status::g_Party.setBattleMode();
    for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
        updataHP_[i] = status::g_Party.getPlayerStatus(i)->haveStatusInfo_.getHp();
    }
    status::g_Party.setPlayerMode();
    updataMP_ = status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.getMp();
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        int count = 0;
        int maxCharaCount = status::g_Party.getCount();
        while (status::g_Party.getPlayerStatus(count)->haveStatusInfo_.isDeath()) {
            count++;
            if (count > maxCharaCount) {
                return;
            }
        }
        useItemPlayer_ = status::g_Party.getPlayerStatus(count)->haveStatusInfo_.haveStatus_.playerIndex_;
        isResult_ = status::FukuroItemInfo::useFukuroItem(useActionParam, itemIndex, useMes, resultMes, -1);
        TownMenuPlayerControl::getSingleton()->setFukuroActiveItemByChangeMax();
    } else {
        useItemPlayer_ = status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.haveStatus_.playerIndex_;
        if (status::PlayerItemInfo::getItemIndex(activeChara, itemIndex) == 0x66) {
            useInoriNoyubiwa_ = 1;
        }
        isResult_ = status::PlayerItemInfo::usePlayerItem(useActionParam, activeChara, itemIndex, useMes, resultMes, -1);
        TownMenuPlayerControl::getSingleton()->setPlayerActiveItemByChangeMax();
    }
    status::g_Party.setBattleMode();
    for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
        if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath() == 1) {
            nowHP[i] = 0;
        } else {
            nowHP[i] = (short)status::g_Party.getPlayerStatus(i)->haveStatusInfo_.getHpMax();
        }
        status::g_Party.getPlayerStatus(i)->haveStatusInfo_.setHp(updataHP_[i]);
        updataHP_[i] = nowHP[i];
    }
    status::g_Party.setPlayerMode();
    int nowMP = status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.getMp();
    status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.setMp(updataMP_);
    updataMP_ = nowMP;
    g_cmnPartyInfo.actorIndex_ = useActionParam.actorCharacterStatus_->haveStatusInfo_.haveStatus_.playerIndex_;
    g_cmnPartyInfo.targetIndex_ = useActionParam.targetCharacterStatus_[0]->haveStatusInfo_.haveStatus_.playerIndex_;
    if (itemID_ == TownMenuItemUseManager::getSingleton()->eventItem_) {
        TownMenuItemUseManager::getSingleton()->eventItemFlag_ = 1;
        close();
        gTownMenu_ROOT.stat_ = MENUBASE_STAT_OK;
        return;
    }
    useItem_ = 1;
    if (itemID_ == 0x85) {
        boots_ = 1;
    }
    if (itemID_ == 0x88 || itemID_ == 0x9b) {
        data_020ed1bc.openMessageForMENU();
        TextAPI::setMACRO0(1, 0x50000000, useItemPlayer_);
        TextAPI::setMACRO0(10, 0x40000000, itemID_);
        MenuAPI::addMessageSerial(useMes[0]);
        resultMes_[0] = 0xc3d6b;
    } else {
        while (useMes[mesCount] != -1) {
            if (mesCount > 4) {
                break;
            }
            resultMes_[mesCount] = useMes[mesCount];
            mesCount++;
        }
    }
    int resCount = 0;
    while (resultMes[resCount] != -1) {
        if (resCount > 4) {
            break;
        }
        resultMes_[mesCount] = resultMes[resCount];
        mesCount++;
        resCount++;
    }
    unkfunc_02176e14();
    if (itemID_ == 0x66 && isResult_ == 1) {
        resultMes_[1] = -1;
    }
    if (!TownMenuPlayerControl::getSingleton()->activeFukuro_ && status::PlayerItemInfo::getItemMaxCount(activeChara) == 0) {
        openUseItemMessage();
        close();
        gTownMenuItemSelectChara.open();
        if (TownMenuItemUseManager::getSingleton()->getDefaultCloseMenuItem(itemID_)) {
            gTownMenuItemSelectChara.closeMenuMessage_ = 1;
        }
    }
}

THUMB void TownMenuItemSelectCommand::judgeEquipItem()
{
    int activeChara = TownMenuPlayerControl::getSingleton()->activeChara_;
    int activeItem = TownMenuPlayerControl::getSingleton()->getActiveItemIndexToAll();
    int equipItemId = status::PlayerItemInfo::getEquipItemIdByType(activeChara, status::UseItem::getItemType(itemID_));
    isLock_ = 1;
    if (equipItemId != 0 && status::UseItem::isCurse(equipItemId) && status::g_Party.getPlayerIndex(activeChara) != 0x19) {
        gTownMenuItemMessage.open();
        gTownMenuItemMessage.setMessageMenu(TownMenuItemMessage::MESS_EQIP_NOROI);
        sound_ = 1;
        return;
    }
    gTownMenuItemMessage.open();
    gTownMenuItemMessage.setMessageMenu(TownMenuItemMessage::MESS_EQIP_NORMAL);
    if (!status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.haveEquipment_.isEquipment(itemID_)) {
        status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.setEquipment(activeItem);
    }
    if (status::UseItem::getItemType(itemID_) > 4) {
        menuItem_.active_ = TownMenuPlayerControl::getSingleton()->getActiveCommand();
    }
}

THUMB void TownMenuItemSelectCommand::setItemShowAction()
{
    int messageCount = status::UseItem::getJudgeMessageCount(itemID_);
    int itemValue = status::UseItem::getSellPrice(itemID_);
    int equipMemberCount = 0;
    data_020ed1bc.openMessageForMENU();
    TextAPI::setMACRO0(10, 0x40000000, itemID_);
    TextAPI::setMACRO0(0x4b, 0xf0000000, itemValue);
    if (status::g_Party.getPlayerStatus(status::g_Party.getSortIndex(7))->haveStatusInfo_.isDeath() == 1) {
        data_020ed1bc.addMessage(0xc4fc3);
        return;
    }
    if (status::g_Story.chapter_ == 5) {
        int heroIndex;
        if (status::g_Story.sex_ == SEX_MALE) {
            heroIndex = status::g_Party.getSortIndex(1);
        } else {
            heroIndex = status::g_Party.getSortIndex(2);
        }
        if (heroIndex != -1) {
            TextAPI::setMACRO0(9, 0x50000000, status::g_Party.getPlayerStatus(heroIndex)->haveStatusInfo_.haveStatus_.playerIndex_);
        }
    }
    if (status::UseItem::getItemType(itemID_) <= 4) {
        gTextHook.resetEQUIPABLE_PC();
        for (int i = 0; i < status::g_Party.getCount(); i++) {
            if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isEquipEnable(itemID_) && status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_ != 7) {
                gTextHook.setEQUIPABLE_PC(status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_);
                equipMemberCount++;
            }
        }
    }
    for (int i = 0; i < messageCount; i++) {
        if (status::g_Story.chapter_ == 3 || equipMemberCount == 0) {
            for (int j = 0; j < 27; j++) {
                if (equipMessage_[j] == status::UseItem::getJudgeMessage(itemID_, i)) {
                    i++;
                }
            }
        }
        data_020ed1bc.addMessage(status::UseItem::getJudgeMessage(itemID_, i));
    }
}

THUMB void TownMenuItemSelectCommand::judgeThrowItem()
{
    isLock_ = 1;
    int activeChara = TownMenuPlayerControl::getSingleton()->activeChara_;
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        short activeItem = TownMenuPlayerControl::getSingleton()->activeItem_;
        switch (status::FukuroItemInfo::getItemThrowType(activeItem, TownMenuPlayerControl::getSingleton()->activeItemPage_)) {
            case status::UseItem::THROW_OK:
                gTownMenuItemMessage.open();
                gTownMenuItemMessage.setMessageMenu(TownMenuItemMessage::MESS_THROW_OK);
                return;
            case status::UseItem::THROW_NG:
                gTownMenuItemMessage.open();
                gTownMenuItemMessage.setMessageMenu(TownMenuItemMessage::MESS_THROW_NG);
                return;
            case status::UseItem::THROW_DIFFICULT:
                gTownMenuItemMessage.open();
                gTownMenuItemMessage.setMessageMenu(TownMenuItemMessage::MESS_THROW_DIFFICULT);
                return;
        }
        return;
    }
    switch (status::PlayerItemInfo::getItemThrowType(activeChara, TownMenuPlayerControl::getSingleton()->getActiveItemIndexToAll())) {
        case status::UseItem::THROW_OK:
            gTownMenuItemMessage.open();
            gTownMenuItemMessage.setMessageMenu(TownMenuItemMessage::MESS_THROW_OK);
            return;
        case status::UseItem::THROW_NG:
            gTownMenuItemMessage.open();
            gTownMenuItemMessage.setMessageMenu(TownMenuItemMessage::MESS_THROW_NG);
            return;
        case status::UseItem::THROW_DIFFICULT:
            gTownMenuItemMessage.open();
            gTownMenuItemMessage.setMessageMenu(TownMenuItemMessage::MESS_THROW_DIFFICULT);
            return;
    }
}

THUMB int TownMenuItemSelectCommand::unkfunc_02176b90()
{
    switch (commandFlag_) {
        case 1:
            return 5;
        case 2:
            return 5;
        case 3:
            return 6;
    }
    return 4;
}

THUMB void TownMenuItemSelectCommand::unkfunc_02176bb0()
{
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        if (status::FukuroItemInfo::getItemMaxCount() == 0) {
            gTownMenuItemSelectChara.open();
            return;
        }
        gUnkTownMenu_02176fa0.open();
        return;
    }
    if (status::PlayerItemInfo::getItemMaxCount(TownMenuPlayerControl::getSingleton()->activeChara_) == 0) {
        gTownMenuItemSelectChara.open();
        return;
    }
    gUnkTownMenu_02176fa0.open();
}

THUMB void TownMenuItemSelectCommand::unkfunc_02176bfc()
{
    int index = status::g_Party.getSortIndex(8);
    if (index == -1) {
        useItemNoTarget();
        return;
    }
    data_020ed1bc.openMessageForMENU();
    if (status::g_Party.getPlayerStatus(index)->haveStatusInfo_.isDeath() || status::g_Party.isInsideCarriage(index)) {
        data_020ed1bc.addMessage(0xc5156);
        return;
    }
    if (status::g_Story.isTarot()) {
        data_020ed1bc.addMessage(0xc5159, 0xc515a);
        return;
    }
    int mes[5] = { -1, -1, -1, -1, -1 };
    int messageCount = 0;
    status::g_Story.setTarot(1);
    data_020ed1bc.addMessage(0xc505a, 0xc505b, 0xc505c, 0xc505d);
    TownMenuItemTarotMessage::getTarotMessage(mes, -1);
    while (mes[messageCount] != -1) {
        if (messageCount == 5) {
            break;
        }
        data_020ed1bc.addMessage(mes[messageCount]);
        messageCount++;
    }
    data_020ed1bc.addMessage(0xc5152, 0xc5153);
}

THUMB void TownMenuItemSelectCommand::resultItem()
{
    if (TownMenuItemUseManager::getSingleton()->getDefaultCloseMenuItem(itemID_)) {
        close();
        gTownMenu_ROOT.stat_ = MENUBASE_STAT_OK;
        return;
    }
    if (boots_ == 1) {
        close();
        gTownMenu_ROOT.stat_ = MENUBASE_STAT_OK;
        if (!g_Stage.isRulaDisable()) {
            if (isResult_ == 1) {
                g_Stage.setRuraFlag(1);
                g_Stage.setRuraTownID(-1);
                return;
            }
            g_cmnPartyInfo.setMenuAction(cmn::MENU_RURA_FAILED);
        }
        return;
    }
    switch (itemID_) {
        case 0x9d:
            if (data_0210bb94.unkfunc_02058114(0xe) == 1) {
                close();
                FieldPlayerManager::getSingleton()->savePartyDrawInfo();
                g_Global.prevPartTown_ = 0;
                g_Global.startBook();
            }
            return;
        case 0x8d:
            if (isResult_ == 1) {
                close();
                gTownMenu_ROOT.stat_ = MENUBASE_STAT_OK;
                g_Global.setRanarutaFlag(true);
                cmn::NonBattleActionManager::getSingleton()->setAction(cmn::ACTION_RANARUTA);
            }
            return;
        case 0x98:
            close();
            gTownMenu_ROOT.stat_ = MENUBASE_STAT_OK;
            g_cmnPartyInfo.setMenuAction(cmn::MENU_HENGE_NO_TSUE);
            return;
        case 0x9b:
            close();
            gTownMenu_ROOT.stat_ = MENUBASE_STAT_OK;
            g_cmnPartyInfo.setMenuAction(cmn::BALLON_HORN);
            return;
        default:
            if (TownMenuPlayerControl::getSingleton()->activeCommand_ == 0 && sound_ == 0) {
                close();
                unkfunc_02176bb0();
            }
            return;
    }
}

THUMB void TownMenuItemSelectCommand::unkfunc_02176e14()
{
    if (itemID_ == 0x83 && isResult_ == 1) {
        resultMes_[1] = -1;
    }
    if (itemID_ == 0x8a) {
        resultMes_[0] = 0x6def;
        for (int i = 1; i < 5; i++) {
            resultMes_[i] = 0x6ddc + i;
        }
    }
    if (itemID_ == 0x8b) {
        for (int i = 0; i < 5; i++) {
            resultMes_[i] = 0x7b44 + i;
        }
    }
    if (itemID_ == 0x90) {
        resultMes_[0] = 0xc3de8;
        resultMes_[1] = -1;
    }
    if (itemID_ == 0x93) {
        resultMes_[1] = 0xc3de6;
    }
    if (status::UseItem::getItemType(itemID_) == 7) {
        resultMes_[1] = 0xc3d21;
    }
    if (itemID_ == 0x9d && data_0210bb94.unkfunc_02058114(0xe) == 0) {
        resultMes_[1] = 0xc3dea;
    }
}

THUMB void TownMenuItemSelectCommand::openUseItemMessage()
{
    int count = 0;
    int active = TownMenuPlayerControl::getSingleton()->activeChara_;
    data_020ed1bc.openMessageForMENU();
    TextAPI::setMACRO0(1, 0x50000000, useItemPlayer_);
    TextAPI::setMACRO0(10, 0x40000000, itemID_);
    if (useInoriNoyubiwa_ == 1) {
        int target = status::g_Party.getSortIndex(useItemPlayer_);
        TextAPI::setMACRO0(0x12, 0x50000000, useItemPlayer_);
        TextAPI::setMACRO0(0x51, 0xf0000000, status::g_Party.getPlayerStatus(target)->haveStatusInfo_.effectValue_);
    } else {
        TextAPI::setMACRO0(0x12, 0x50000000, status::g_Party.getPlayerIndex(active));
        TextAPI::setMACRO0(0x51, 0xf0000000, status::g_Party.getPlayerStatus(active)->haveStatusInfo_.effectValue_);
    }
    while (resultMes_[count] != -1) {
        if (count > 8) {
            break;
        }
        data_020ed1bc.addMessage(resultMes_[count]);
        count++;
    }
    if ((itemID_ == 0x83 || itemID_ == 0x66) && isResult_ == 1) {
        data_020ed1bc.setMessageCursor(true);
    }
}
