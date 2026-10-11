#include "ov016/TownMenu_ITEM/TownMenu_ITEM_CHECKTARGET.hpp"
#include "ov016/TownMenu_ITEM/TownMenu_ITEM_EQUIPCHECK.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemSelectChara.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemSelectTargetChara.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemSelectTargetItem.hpp"
#include "ov016/TownMenuPlayerControl/TownMenuPlayerControl.hpp"
#include "ov016/status/PlayerItemInfo.hpp"
#include "ov016/status/FukuroItemInfo.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/MenuAPI.hpp"
#include "ov016/UnkTownMenu_02176fa0/UnkTownMenu_02176fa0.hpp"
#include "main/sound/MenuSoundManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/UseItem.hpp"
#include "main/text/TextAPI.hpp"

THUMB void TownMenu_ITEM_CHECKTARGET::menuSetup()
{
    status::g_Party.setPlayerMode();
    MenuSoundManager::getSingleton()->initialize();
    isPlaySound_ = 0;
    changeItemID_ = 0;
    fromActiveChara_ = TownMenuPlayerControl::getSingleton()->activeChara_;
    toActiveChara_ = TownMenuPlayerControl::getSingleton()->targetChara_;
    activeItem_ = TownMenuPlayerControl::getSingleton()->getActiveItemIndexToAll();
    if (TownMenuPlayerControl::getSingleton()->targetItem_ == -1) {
        isSpace_ = 1;
        targetItem_ = -1;
    } else {
        isSpace_ = 0;
        if (!TownMenuPlayerControl::getSingleton()->targetFukuro_) {
            targetItem_ = TownMenuPlayerControl::getSingleton()->getTargetItemIndexToAll();
        }
    }
}

THUMB void TownMenu_ITEM_CHECKTARGET::menuUpdate()
{
    if (isPlaySound_) {
        if (!MenuSoundManager::getSingleton()->isPlaySound()) {
            data_020ed1bc.openMessageForMENU();
            TextAPI::setMACRO0(7, 0x40000000, changeItemID_);
            data_020ed1bc.addMessage(0xc3d8d);
            isPlaySound_ = 0;
        }
        return;
    }
    int actor = -1;
    int fromIname = -1;
    int target = -1;
    int toMaxIname = -1;
    int fukuroIname = -1;
    int leadpc = 0;
    unsigned int flag;
    if (data_020ed1bc.isOpen()) {
        int stat = data_020ed1bc.stat_;
        if (stat == MENUBASE_STAT_OK || stat == MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            close();
            unkfunc_02172bdc();
        }
        return;
    }
    if (!TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        actor = status::g_Party.getPlayerStatus(fromActiveChara_)->haveStatusInfo_.haveStatus_.playerIndex_;
        curseItemID_ = status::PlayerItemInfo::getItemIndex(fromActiveChara_, activeItem_);
        fromIname = status::PlayerItemInfo::getItemIndex(fromActiveChara_, activeItem_);
    }
    if (!TownMenuPlayerControl::getSingleton()->targetFukuro_) {
        target = status::g_Party.getPlayerStatus(toActiveChara_)->haveStatusInfo_.haveStatus_.playerIndex_;
        if (isSpace_ == 0) {
            toMaxIname = status::PlayerItemInfo::getItemIndex(toActiveChara_, targetItem_);
        }
    }
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        short item = TownMenuPlayerControl::getSingleton()->activeItem_;
        char page = TownMenuPlayerControl::getSingleton()->activeItemPage_;
        curseItemID_ = status::FukuroItemInfo::getItemId(item, page);
        fukuroIname = status::FukuroItemInfo::getItemId(item, page);
    }
    while (status::g_Party.getPlayerStatus(leadpc)->haveStatusInfo_.isDeath()) {
        leadpc++;
    }
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_ && TownMenuPlayerControl::getSingleton()->targetFukuro_) {
        openMessage(-1, -1, fukuroIname, -1, -1, status::g_Party.getPlayerIndex(leadpc), 0xc3d40);
        return;
    }
    if (isCurse()) {
        return;
    }
    if (fromActiveChara_ == toActiveChara_ && !TownMenuPlayerControl::getSingleton()->activeFukuro_ && !TownMenuPlayerControl::getSingleton()->targetFukuro_) {
        if (status::g_Party.getPlayerStatus(fromActiveChara_)->haveStatusInfo_.isDeath()) {
            openMessage(actor, target, fromIname, -1, -1, status::g_Party.getPlayerIndex(leadpc), 0xc3d2a);
        } else {
            openMessage(actor, -1, fromIname, -1, -1, -1, 0xc3d3c);
        }
        changeItem(0x10);
        return;
    }
    flag = checkGiveFlag();
    switch (flag) {
    case 2:
        openMessage(actor, -1, fromIname, -1, -1, -1, 0xc3d30);
        break;
    case 6:
        openMessage(actor, -1, fromIname, -1, -1, status::g_Party.getPlayerIndex(leadpc), 0xc3d32);
        break;
    case 8:
        if (isSpace_) {
            openMessage(-1, target, fukuroIname, -1, -1, -1, 0xc3d34);
        } else {
            openMessage(-1, target, -1, fukuroIname, toMaxIname, -1, 0xc3d38);
        }
        break;
    case 9:
        if (isSpace_) {
            openMessage(-1, target, fukuroIname, -1, -1, status::g_Party.getPlayerIndex(leadpc), 0xc3d36);
        } else {
            openMessage(-1, target, -1, fukuroIname, toMaxIname, status::g_Party.getPlayerIndex(leadpc), 0xc3d3a);
        }
        break;
    case 1:
        if (isSpace_) {
            openMessage(actor, target, fromIname, -1, -1, -1, 0xc3d28);
        } else {
            openMessage(actor, target, -1, fromIname, toMaxIname, -1, 0xc3d2c);
        }
        break;
    case 4:
        if (isSpace_) {
            openMessage(actor, target, fromIname, -1, -1, -1, 0xc3d26);
        } else {
            openMessage(actor, target, -1, fromIname, toMaxIname, -1, 0xc3d2c);
        }
        break;
    case 5:
        if (isSpace_) {
            openMessage(actor, target, fromIname, -1, -1, status::g_Party.getPlayerIndex(leadpc), 0xc3d2a);
        } else {
            openMessage(actor, target, -1, fromIname, toMaxIname, status::g_Party.getPlayerIndex(leadpc), 0xc3d2e);
        }
        break;
    default:
        if (isSpace_) {
            openMessage(actor, target, fromIname, -1, -1, -1, 0xc3d24);
        } else {
            openMessage(actor, target, -1, fromIname, toMaxIname, -1, 0xc3d2c);
        }
        break;
    }
    changeItem(flag);
}

THUMB void TownMenu_ITEM_CHECKTARGET::openMessage(int actor, int target, int iname, int iname1, int iname2, int leadpc, int mes)
{
    data_020ed1bc.openMessageForMENU();
    if (actor != -1) {
        TextAPI::setMACRO0(1, 0x50000000, actor);
    }
    if (target != -1) {
        TextAPI::setMACRO0(0x12, 0x50000000, target);
    }
    if (iname != -1) {
        TextAPI::setMACRO0(10, 0x40000000, iname);
    }
    if (iname1 != -1) {
        TextAPI::setMACRO1(10, 0x40000000, iname1);
    }
    if (iname2 != -1) {
        TextAPI::setMACRO2(10, 0x40000000, iname2);
    }
    if (leadpc != -1) {
        TextAPI::setMACRO0(11, 0x50000000, leadpc);
    }
    data_020ed1bc.addMessage(mes);
}

THUMB unsigned int TownMenu_ITEM_CHECKTARGET::checkGiveFlag()
{
    unsigned int giveFlag = 0;
    if (!TownMenuPlayerControl::getSingleton()->targetFukuro_ && status::g_Party.getPlayerStatus(toActiveChara_)->haveStatusInfo_.isDeath()) {
        giveFlag |= 1;
    }
    if (!TownMenuPlayerControl::getSingleton()->activeFukuro_ && status::g_Party.getPlayerStatus(fromActiveChara_)->haveStatusInfo_.isDeath()) {
        giveFlag |= 4;
    }
    if (TownMenuPlayerControl::getSingleton()->targetFukuro_) {
        giveFlag |= 2;
    }
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        giveFlag |= 8;
    }
    return giveFlag;
}

THUMB void TownMenu_ITEM_CHECKTARGET::changeItem(unsigned int flag)
{
    switch (flag) {
    case 2:
    case 6:
        status::UseItem::give(&status::g_Party.getPlayerStatus(fromActiveChara_)->haveStatusInfo_, activeItem_, &status::g_Party.haveItemSack_, -1);
        TownMenuPlayerControl::getSingleton()->setPlayerActiveItemByChangeMax();
        TownMenuPlayerControl::getSingleton()->setFukuroTargetItemByChangeMax();
        break;
    case 8:
    case 9:
        status::UseItem::give2(&status::g_Party.haveItemSack_, activeItem_, &status::g_Party.getPlayerStatus(toActiveChara_)->haveStatusInfo_, targetItem_);
        TownMenuPlayerControl::getSingleton()->setFukuroActiveItemByChangeMax();
        TownMenuPlayerControl::getSingleton()->setPlayerTargetItemByChangeMax();
        status::g_Party.getPlayerStatus(toActiveChara_)->haveStatusInfo_.haveItem_.sortEquipment();
        setTargetItem();
        close();
        gTownMenu_ITEM_EQUIPCHECK.open();
        break;
    default:
        status::PlayerStatus* targetStatus = status::g_Party.getPlayerStatus(toActiveChara_);
        status::UseItem::give2(&status::g_Party.getPlayerStatus(fromActiveChara_)->haveStatusInfo_, activeItem_, &targetStatus->haveStatusInfo_, targetItem_);
        TownMenuPlayerControl::getSingleton()->setPlayerActiveItemByChangeMax();
        TownMenuPlayerControl::getSingleton()->setPlayerTargetItemByChangeMax();
        status::g_Party.getPlayerStatus(fromActiveChara_)->haveStatusInfo_.haveItem_.sortEquipment();
        status::g_Party.getPlayerStatus(toActiveChara_)->haveStatusInfo_.haveItem_.sortEquipment();
        setTargetItem();
        if (flag != 0x10) {
            close();
            gTownMenu_ITEM_EQUIPCHECK.open();
        }
        break;
    }
}

THUMB void TownMenu_ITEM_CHECKTARGET::setTargetItem()
{
    int itemCount = status::PlayerItemInfo::getItemMaxCount(toActiveChara_);
    for (unsigned char i = 0; i < itemCount; i++) {
        if (curseItemID_ == status::PlayerItemInfo::getItemIndex(toActiveChara_, i) &&
            !status::g_Party.getPlayerStatus(toActiveChara_)->haveStatusInfo_.haveItem_.isEquipment(i)) {
            if (i > 6) {
                TownMenuPlayerControl::getSingleton()->targetItem_ = (unsigned char)(i - 6);
                TownMenuPlayerControl::getSingleton()->targetItemPage_ = 1;
            } else {
                TownMenuPlayerControl::getSingleton()->targetItem_ = i;
                TownMenuPlayerControl::getSingleton()->targetItemPage_ = 0;
            }
            return;
        }
    }
}

THUMB int TownMenu_ITEM_CHECKTARGET::isCurse()
{
    if (!TownMenuPlayerControl::getSingleton()->activeFukuro_ &&
        status::g_Party.getPlayerStatus(fromActiveChara_)->haveStatusInfo_.haveItem_.isEquipment(activeItem_) == 1 &&
        status::PlayerItemInfo::checkCurse(fromActiveChara_, curseItemID_)) {
        MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_NOROI);
        changeItemID_ = curseItemID_;
        isPlaySound_ = 1;
        gTownMenuItemSelectTargetItem.sound_ = 1;
        gTownMenuItemSelectTargetChara.sound_ = 1;
        redraw_ = 1;
        return 1;
    }
    if (isSpace_ == 0 && !TownMenuPlayerControl::getSingleton()->targetFukuro_) {
        changeItemID_ = status::PlayerItemInfo::getItemIndex(toActiveChara_, targetItem_);
        if (status::PlayerItemInfo::checkCurse(toActiveChara_, changeItemID_)) {
            MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_NOROI);
            isPlaySound_ = 1;
            return 1;
        }
    }
    return 0;
}

THUMB void TownMenu_ITEM_CHECKTARGET::unkfunc_02172bdc()
{
    gTownMenuItemSelectTargetChara.close();
    gTownMenuItemSelectTargetItem.close();
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_) {
        if (status::FukuroItemInfo::getItemMaxCount() == 0) {
            gTownMenuItemSelectChara.open();
        } else {
            gUnkTownMenu_02176fa0.open();
        }
    } else if (status::PlayerItemInfo::getItemMaxCount(fromActiveChara_) == 0) {
        gTownMenuItemSelectChara.open();
    } else {
        gUnkTownMenu_02176fa0.open();
    }
}
