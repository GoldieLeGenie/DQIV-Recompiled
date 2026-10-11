#include "ov016/UnkMaterielMenu_0216b304/UnkMaterielMenu_0216b304.hpp"
#include "ov016/MaterielMenu_WINDOW_MANAGER/MaterielMenu_WINDOW_MANAGER.hpp"
#include "ov028/MaterielMenu_SHOP/MaterielMenu_SHOP.hpp"
#include "main/menu/MaterielMenuPlayerControl.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/MenuUpdateAssist.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/UseItem.hpp"
#include "main/text/TextAPI.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Random.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"
#include "main/menu/MenuTemplate_Common.hpp"

THUMB void UnkMaterielMenu_0216b304::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    mode_ = 0;
    price_ = 0;
    activeChara_ = MaterielMenuPlayerControl::getSingleton()->activeChara_;
    unkfunc_0216b6bc();
}

THUMB void UnkMaterielMenu_0216b304::menuExecute()
{
    status::HaveItem* haveItem = &status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_;
    int count;
    if (activeChara_ == status::g_Party.getCount()) {
        if (status::g_Party.haveItemSack_.getCount() < 6) {
            count = status::g_Party.haveItemSack_.getCount();
        } else if ((activeItemPage_ + 1) * 6 > status::g_Party.haveItemSack_.getCount()) {
            count = status::g_Party.haveItemSack_.getCount() - activeItemPage_ * 6;
        } else {
            count = 6;
        }
    } else if (activeItemPage_ == 0 && haveItem->getCount() > 6) {
        count = 6;
    } else if (activeItemPage_ == 1) {
        count = haveItem->getCount() - 6;
    } else {
        count = haveItem->getCount();
    }
    MenuTemplate_Common::COMMON_ITEM_ICON32_2x3(&menuItem_, count, activeItem_);
    MenuTemplate_materiel::MATERIEL_CANCEL(&cancelItem_);
}

THUMB void UnkMaterielMenu_0216b304::menuDraw()
{
    if (!data_020ed1bc.isOpen()) {
        if (activeChara_ == status::g_Party.getCount()) {
            unkfunc_0216fbf4();
        } else {
            unkfunc_0216fbbc();
        }
        menuItem_.drawActive();
    }
}

THUMB void UnkMaterielMenu_0216b304::menuUpdate()
{
    if (unkfunc_0216b4f0()) {
        return;
    }
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        unkfunc_0216b760();
    }
    int count;
    if (activeChara_ == status::g_Party.getCount()) {
        count = status::g_Party.haveItemSack_.getCount();
    } else {
        count = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_.getCount();
    }
    navigator_.setup(2, 3, count);
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result == 0) {
        return;
    }
    if (result == 2) {
        int item;
        if (activeChara_ == status::g_Party.getCount()) {
            item = status::g_Party.haveItemSack_.getItem(activeItem_ + activeItemPage_ * 6);
        } else {
            item = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_.getItem(activeItem_ + activeItemPage_ * 6);
        }
        MaterielMenu_SHOP_MANAGER::getSingleton()->setSellQuantity(1);
        unkfunc_0216b604(item);
        return;
    }
    activeItem_ = menuItem_.active_;
    activeItemPage_ = navigator_.getPageNo();
    MaterielMenuPlayerControl::getSingleton()->setActiveItem(activeItem_);
    MaterielMenuPlayerControl::getSingleton()->setActiveItemPage(activeItemPage_);
    redraw_ = 1;
}

THUMB int UnkMaterielMenu_0216b304::unkfunc_0216b4f0()
{
    if (data_020ed1bc.isOpen()) {
        if (data_020ed1bc.stat_ == MENUBASE_STAT_OK) {
            data_020ed1bc.close();
            switch (mode_) {
                case 2:
                    if (activeChara_ == status::g_Party.getCount()) {
                        if (status::g_Party.haveItemSack_.getCount() == 0) {
                            unkfunc_0216b760();
                            break;
                        }
                    } else if (status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_.getCount() == 0) {
                        unkfunc_0216b760();
                        break;
                    }
                    unkfunc_0216b778(0x6d8a, -1);
                    mode_ = 0;
                    break;
                case 1:
                    MaterielMenu_SHOP_MANAGER::getSingleton()->sellItem(activeItem_ + activeItemPage_ * 6, activeChara_, price_);
                    unkfunc_0216b6bc();
                    unkfunc_0216b778(0x6d8e, 0x6d8f);
                    data_020ed1bc.setYesNo();
                    mode_ = 2;
                    break;
                case 3:
                    MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
                    break;
            }
        } else if (data_020ed1bc.stat_ == MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            switch (mode_) {
                case 0:
                case 2:
                    unkfunc_0216b778(0x6d9a, -1);
                    mode_ = 3;
                    break;
                case 1:
                    unkfunc_0216b778(0x6d9c, 0x6d8f);
                    data_020ed1bc.setYesNo();
                    mode_ = 0;
                    break;
            }
        }
        return 1;
    }
    return 0;
}

THUMB void UnkMaterielMenu_0216b304::unkfunc_0216b604(int item)
{
    ItemType type = status::UseItem::getItemType(item);
    if (status::UseItem::getSellType(item) == status::UseItem::SELL_NG) {
        unkfunc_0216b778(0x6d97, 0x6d8f);
        mode_ = 2;
    } else if (type == 0 || type >= 4) {
        unkfunc_0216b778(0x6d94, 0x6d8f);
        mode_ = 2;
    } else {
        int price = status::UseItem::getBuyPrice(item);
        if (price == 0) {
            price = status::UseItem::getSellPrice(item);
        }
        if (dssrand::rand(10) == 0) {
            price_ = dss::getRandomVariation(price, -50, 100);
        } else {
            price_ = dss::getRandomVariation(price, 15, 25);
        }
        TextAPI::setMACRO0(10, 0x40000000, item);
        TextAPI::setMACRO0(0x4b, 0xf0000000, price_);
        unkfunc_0216b778(0x6d8d, -1);
        mode_ = 1;
    }
    data_020ed1bc.setYesNo();
}

THUMB void UnkMaterielMenu_0216b304::unkfunc_0216b6bc()
{
    navigator_.setupBase();
    activeItemPage_ = 0;
    MaterielMenuPlayerControl::getSingleton()->activeItemPage_ = 0;
    if (activeChara_ == status::g_Party.getCount()) {
        activeItem_ = 0;
        MaterielMenuPlayerControl::getSingleton()->setActiveItem(activeItem_);
        pageMax_ = (status::g_Party.haveItemSack_.getCount() - 1) / 6;
        return;
    }
    status::HaveItem* haveItem = &status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_;
    int i = 0;
    while (haveItem->isEquipment(i) == 1) {
        i++;
        if (i > haveItem->getCount()) {
            break;
        }
    }
    activeItem_ = i;
    if (i == haveItem->getCount()) {
        activeItem_--;
    }
    MaterielMenuPlayerControl::getSingleton()->setActiveItem(activeItem_);
    pageMax_ = (haveItem->getCount() - 1) / 6;
}

THUMB void UnkMaterielMenu_0216b304::unkfunc_0216b760()
{
    close();
    gMaterielMenu_SHOP_WHO_SELL.open();
    gMaterielMenu_SHOP_WHO_SELL.extraMode_ = 1;
}

THUMB void UnkMaterielMenu_0216b304::unkfunc_0216b778(int message, int message2)
{
    data_020ed1bc.openMessageForTALK();
    data_020ed1bc.addMessage(message);
    if (message2 != -1) {
        data_020ed1bc.addMessage(message2);
    }
}
