#include "ov016/TownMenu_OPERATION/TownMenu_OPERATION_EQUIP.hpp"
#include "ov016/TownMenu_OPERATION/TownMenu_OPERATION_ROOT.hpp"
#include "ov016/TownMenu_ROOT/TownMenu_ROOT.hpp"
#include "ov016/TownMenuPlayerControl/TownMenuPlayerControl.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/sound/MenuSoundManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/UseItem.hpp"
#include "main/text/TextAPI.hpp"
#include "ov016/MenuTemplate/MenuTemplate_town.hpp"
#include "ov016/UnkTownMenuDraw/UnkTownMenuDraw_0217d560.hpp"
#include "main/menu/MenuTemplate_Common.hpp"

THUMB void TownMenu_OPERATION_EQUIP::menuSetup()
{
    status::g_Party.setPlayerMode();
    charaItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    itemItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    removeItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_ACTIVE);
    charaNavigator_.setupBase();
    itemNavigator_.setupBase();
    removeNavigator_.setupBase();
    itemType_ = -1;
    beginItemType_ = -1;
    curseType_ = 0;
    activeChara_ = 0;
    itemListActive_ = 0;
    msgEnd2NextEquipment_ = 1;
    playSound_ = 0;
    MenuSoundManager::getSingleton()->initialize();
    setItemTypeList();
}

THUMB void TownMenu_OPERATION_EQUIP::menuExecute()
{
    status::g_Party.setPlayerMode();
    int count = status::g_Party.getCount();
    if (itemType_ == -1) {
        if (count < 6) {
            MenuTemplate_town::townMenuItemSelectHalfChara(&charaItem_, count, charaItem_.active_);
        } else {
            MenuTemplate_town::townMenuItemSelectChara(&charaItem_, count, charaItem_.active_);
        }
    } else {
        int itemCount = itemCount_ - itemPageStart_ * 6;
        if (itemCount > 6) {
            itemCount = 6;
        }
        MenuTemplate_Common::COMMON_ITEM_ICON32_2x3(&itemItem_, itemCount, itemItem_.active_);
        if (itemType_ >= 0) {
            MenuTemplate_town::TOWN_OP_NOEQUIP(&removeItem_);
        }
    }
    MenuTemplate_town::TOWN_CANCEL(&cancelItem_);
}

THUMB void TownMenu_OPERATION_EQUIP::menuDraw()
{
    status::g_Party.setPlayerMode();
    if (itemType_ >= 0) {
        if (itemListActive_ != 0) {
            unkfunc_0217df54(itemType_, activeChara_, itemPageStart_, itemIdList_, itemCount_, itemPosList_[itemPageStart_ * 6 + itemItem_.getActive()]);
        } else {
            unkfunc_0217df54(itemType_, activeChara_, itemPageStart_, itemIdList_, itemCount_, -1);
        }
    } else {
        unkfunc_0217df54(itemType_, activeChara_, itemPageStart_, itemPosList_, itemCount_, -1);
    }
    if (!data_020ed1bc.isOpen()) {
        charaItem_.drawActive();
        cancelItem_.drawActive();
        if (itemType_ >= 0) {
            itemItem_.drawActive();
            removeItem_.drawActive();
        }
    }
}

THUMB void TownMenu_OPERATION_EQUIP::menuUpdate()
{
    status::g_Party.setPlayerMode();
    if (!MenuSoundManager::getSingleton()->isPlaySound() && playSound_ == 1) {
        if (curseType_ == 1) {
            int index = itemPageStart_ * 6 + itemItem_.getActive();
            data_020ed1bc.openMessageForMENU();
            TextAPI::setMACRO0(10, 0x40000000, itemIdList_[index]);
            data_020ed1bc.addMessage(0xc3d8b);
        } else if (curseType_ == 2) {
            data_020ed1bc.openMessageForMENU();
            TextAPI::setMACRO0(7, 0x40000000, itemIdList_[0]);
            data_020ed1bc.addMessage(0xc3d8d);
        }
        playSound_ = 0;
    }
    if (data_020ed1bc.isOpen()) {
        if (data_020ed1bc.stat_ == MENUBASE_STAT_OK || data_020ed1bc.stat_ == MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            if (msgEnd2NextEquipment_) {
                setItemTypeList();
                itemListActive_ = 1;
                itemType_++;
            }
            msgEnd2NextEquipment_ = 1;
        }
        return;
    }
    if (playSound_ == 1) {
        return;
    }
    charaNavigator_.setup(5, 2, status::g_Party.getCount());
    itemNavigator_.setup(2, 3, itemCount_);
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        if (itemType_ == -1) {
            close();
            gTownMenu_OPERATION_ROOT.open();
        } else {
            itemType_--;
            itemNavigator_.setPageNo(0);
            itemPageStart_ = 0;
            itemListActive_ = 1;
        }
        redraw_ = 1;
        setItemTypeList();
        return;
    }
    int result = MenuUpdate_Assist::menuSelect(charaItem_, charaNavigator_);
    if (result != 0) {
        if (result == 2) {
            itemType_ = 0;
            itemListActive_ = 1;
            setItemTypeList();
            redraw_ = 1;
            return;
        }
        itemType_ = -1;
        charaItem_.result_ = 0;
        charaItem_.lastresult_ = 0;
        activeChara_ = charaItem_.active_;
        TownMenuPlayerControl::getSingleton()->setActiveChara(activeChara_);
        setItemTypeList();
        redraw_ = 1;
        return;
    }
    result = MenuUpdate_Assist::menuSelect(itemItem_, itemNavigator_);
    if (result != 0) {
        if (result == 2) {
            if (itemType_ >= ITEM_WEAPON && itemType_ <= ITEM_ACCESSORY) {
                setEquip();
                setItemTypeList();
            }
            if (itemType_ > ITEM_ACCESSORY) {
                close();
                gTownMenu_ROOT.open();
                gTownMenu_ROOT.menuItem_.active_ = TownMenu_ROOT::ROOT_OPERATION;
            }
            redraw_ = 1;
            return;
        }
        if (result == 4 || result == 5) {
            itemItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
            removeItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
            itemListActive_ = 0;
            redraw_ = 1;
            return;
        }
        itemPageStart_ = itemNavigator_.getPageNo();
        redraw_ = 1;
        return;
    }
    removeNavigator_.setup(1, 1, 1);
    result = MenuUpdate_Assist::menuSelect(removeItem_, removeNavigator_);
    if (result != 0) {
        int active = 4;
        if (result == 2) {
            if (itemType_ >= ITEM_WEAPON && itemType_ <= ITEM_ACCESSORY) {
                unkfunc_0216c6ac();
            }
            if (itemType_ > ITEM_ACCESSORY) {
                close();
                gTownMenu_ROOT.open();
                gTownMenu_ROOT.menuItem_.active_ = TownMenu_ROOT::ROOT_OPERATION;
                return;
            }
        } else if (result == 4) {
            if (itemCount_ != 0) {
                itemItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
                removeItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_ACTIVE);
                int count = itemCount_ - itemPageStart_ * 6;
                while (active >= count) {
                    active -= 2;
                }
                itemItem_.active_ = active;
                itemListActive_ = 1;
            }
        } else if (result == 5) {
            if (itemCount_ != 0) {
                itemItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
                removeItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_ACTIVE);
                itemListActive_ = 1;
            }
        } else if (result == 6) {
            itemNavigator_.pageBack(0);
            itemPageStart_ = itemNavigator_.getPageNo();
        } else if (result == 7) {
            itemNavigator_.pageNext(0);
            itemPageStart_ = itemNavigator_.getPageNo();
        }
        redraw_ = 1;
    }
    if (itemType_ != beginItemType_) {
        setItemTypeList();
        if (itemType_ == -1) {
            charaItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
            itemItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
            charaItem_.active_ = activeChara_;
            removeItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
        } else {
            charaItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
            itemItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
            itemItem_.active_ = 0;
            charaItem_.active_ = activeChara_;
            removeItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_ACTIVE);
        }
        if (itemType_ > -1 && itemCount_ == 0) {
            itemItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
            removeItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
            removeItem_.active_ = 0;
        }
        beginItemType_ = itemType_;
        redraw_ = 1;
    }
}

THUMB void TownMenu_OPERATION_EQUIP::setItemTypeList()
{
    int count = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_.getCount();
    status::g_Party.getCount();
    unsigned char n = 0;
    itemCount_ = 0;
    for (int i = 0; i < 12; i++) {
        itemIdList_[i] = 0;
        itemPosList_[i] = 0;
    }
    if (itemType_ < 0) {
        for (unsigned char i = 0; i < count; i++) {
            int item = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_.getItem(i);
            if (status::UseItem::getItemType(item) <= ITEM_ACCESSORY) {
                itemIdList_[n] = item;
                itemPosList_[n] = i;
                n++;
                itemCount_ = n;
            }
        }
    } else if (itemType_ >= 0) {
        for (unsigned char i = 0; i < count; i++) {
            itemIdList_[i] = 0;
            itemPosList_[i] = i;
            int item = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_.getItem(i);
            if (itemType_ == status::UseItem::getItemType(item)) {
                itemIdList_[n] = item;
                itemPosList_[n] = i;
                n++;
                itemCount_ = n;
            }
        }
    }
    if (itemCount_ == 0) {
        itemListActive_ = 0;
    } else {
        itemListActive_ = 1;
    }
    itemItem_.active_ = 0;
    itemNavigator_.setPageNo(0);
    itemPageStart_ = 0;
}

THUMB void TownMenu_OPERATION_EQUIP::setEquip()
{
    status::HaveStatusInfo& info = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_;
    int index = itemPageStart_ * 6 + itemItem_.getActive();
    int itemPos = itemPosList_[index];
    if (itemPos == -1) {
        return;
    }
    if (info.isEquipEnable(itemIdList_[index])) {
        if (!info.haveEquipment_.isEquipment(itemIdList_[index])) {
            if (status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveEquipment_.isSpell((ItemType)itemType_)) {
                MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_NOROI);
                curseType_ = 2;
                playSound_ = 1;
                return;
            }
            status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.setEquipment(itemPos);
            if (status::UseItem::isCurse(itemIdList_[index]) && info.haveStatus_.playerIndex_ != 0x19) {
                MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_NOROI);
                curseType_ = 1;
                playSound_ = 1;
                return;
            }
        }
        setItemTypeList();
        itemType_++;
    } else {
        data_020ed1bc.openMessageForMENU();
        TextAPI::setMACRO0(1, 0x50000000, info.haveStatus_.playerIndex_);
        TextAPI::setMACRO0(10, 0x40000000, itemIdList_[index]);
        data_020ed1bc.addMessage(0xc3d49);
        msgEnd2NextEquipment_ = 0;
    }
}

THUMB int TownMenu_OPERATION_EQUIP::unkfunc_0216c6ac()
{
    status::HaveStatusInfo& info = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_;
    if (itemCount_ != 0) {
        if (info.haveEquipment_.isSpell((ItemType)itemType_)) {
            data_020ed1bc.openMessageForMENU();
            TextAPI::setMACRO0(7, 0x40000000, itemIdList_[0]);
            data_020ed1bc.addMessage(0xc3d8d);
            return 0;
        }
        info.resetEquipment2((ItemType)itemType_);
        setItemTypeList();
    }
    itemType_++;
    return 1;
}
