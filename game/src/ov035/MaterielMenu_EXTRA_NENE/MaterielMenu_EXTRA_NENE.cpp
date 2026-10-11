#include "ov035/MaterielMenu_EXTRA_NENE/MaterielMenu_EXTRA_NENE.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/HaveItemSack.hpp"
#include "main/status/ShopList.hpp"
#include "main/status/UseItem.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Random.hpp"
#include "main/cmn/CommonCounterInfo.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"
#include "main/menu/MenuTemplate_Common.hpp"

THUMB void MaterielMenu_EXTRA_NENE::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    navigator_.setupBase();
    MaterielMenuPlayerControl::getSingleton()->allClear();
    activeChara_ = 0;
    proceeds_ = 0;
    MaterielMenuPlayerControl::getSingleton()->activeChara_ = 0;
    mode_ = 0;
    drawMode_ = 0;
    neneItemCount_ = status::g_Shop.haveItemNene_.getCount();
}

THUMB void MaterielMenu_EXTRA_NENE::menuExecute()
{
    switch (drawMode_) {
    case 0:
        MenuTemplate_materiel::MATERIEL_ICON32_5x2(&menuItem_, menuItem_.active_, 2);
        break;
    case 1: {
        int count;
        if (MaterielMenuPlayerControl::getSingleton()->activeChara_ == 1) {
            count = status::g_Party.haveItemSack_.getCount();
        } else {
            count = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_.getCount();
        }
        if (navigator_.getPageNo() == 0 && count > 6) {
            count = 6;
        } else if (navigator_.getPageNo() == navigator_.getPageMaxCount() - 1) {
            count -= navigator_.getPageNo() * 6;
        }
        MenuTemplate_Common::COMMON_ITEM_ICON32_2x3(&menuItem_, count, menuItem_.active_);
        break;
    }
    case 2: {
        unsigned int count;
        int lastPage = navigator_.getPageMaxCount() - 1;
        int page = navigator_.getPageNo();
        if (page != lastPage && neneItemCount_ > 6) {
            count = 6;
        } else {
            lastPage = navigator_.getPageMaxCount() - 1;
            page = navigator_.getPageNo();
            if (page == lastPage) {
                count = neneItemCount_;
                if (count > 6) {
                    lastPage = navigator_.getPageMaxCount() - 1;
                    count = neneItemCount_ - lastPage * 6;
                }
            } else {
                count = neneItemCount_;
            }
        }
        MenuTemplate_Common::COMMON_ITEM_ICON32_2x3(&menuItem_, count, menuItem_.active_);
        break;
    }
    }
}

THUMB void MaterielMenu_EXTRA_NENE::menuDraw()
{
    if (data_020ed1bc.isOpen() == false && mode_ != 0) {
        switch (drawMode_) {
        case 0:
            unkfunc_0216fb98();
            break;
        case 1:
            unkfunc_0216fdcc();
            break;
        case 2: {
            int lastPage = navigator_.getPageMaxCount() - 1;
            unkfunc_0216fe10(neneItemCount_, navigator_.getPageNo(), lastPage);
            break;
        }
        }
        menuItem_.drawActive();
    }
}

THUMB void MaterielMenu_EXTRA_NENE::menuUpdate()
{
    if (data_020ed1bc.isOpen()) {
        if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
            data_020ed1bc.close();
            selectYes();
        } else if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            selectNo();
        }
        return;
    }
    if (mode_ == 0) {
        showMessage(0x9473, -1, -1);
        data_020ed1bc.setYesNo();
        return;
    }
    int count = 0;
    switch (drawMode_) {
    case 0:
        count = 2;
        break;
    case 1:
        if (MaterielMenuPlayerControl::getSingleton()->activeChara_ == 1) {
            count = status::g_Party.haveItemSack_.getCount();
        } else {
            count = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_.getCount();
        }
        break;
    case 2:
        count = status::g_Shop.haveItemNene_.getCount();
        break;
    }
    navigator_.setup(2, 3, count);
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result == 0) {
        return;
    }
    if (drawMode_ == 0) {
        int active = menuItem_.active_;
        MaterielMenuPlayerControl::getSingleton()->activeChara_ = active;
    }
    if (result == 2) {
        switch (drawMode_) {
        case 0:
            if (menuItem_.active_ == 1) {
                if (checkHaveItem(true) == true) {
                    menuItem_.active_ = 0;
                    drawMode_ = 1;
                }
            } else {
                if (checkHaveItem(false) == true) {
                    drawMode_ = 1;
                }
            }
            navigator_.setPageNo(0);
            break;
        case 1:
            checkSellItem();
            break;
        case 2:
            showMessage(0x9490, -1, -1);
            mode_ = 5;
            break;
        }
    }
    if (result == 3) {
        switch (drawMode_) {
        case 0:
            showMessage(0x948b, -1, -1);
            data_020ed1bc.setYesNo();
            mode_ = 3;
            break;
        case 1:
            drawMode_ = 0;
            menuItem_.active_ = MaterielMenuPlayerControl::getSingleton()->activeChara_;
            break;
        case 2:
            showMessage(0x9490, -1, -1);
            mode_ = 5;
            break;
        }
    }
    int cursor = menuItem_.active_;
    MaterielMenuPlayerControl::getSingleton()->activeItem_ = cursor;
    int page = navigator_.getPageNo();
    MaterielMenuPlayerControl::getSingleton()->activeItemPage_ = page;
    redraw_ = 1;
}

THUMB void MaterielMenu_EXTRA_NENE::selectYes()
{
    switch (mode_) {
    case 0:
        if (cmn::g_CommonCounterInfo.freeCounter_[0] != 0) {
            calcProceeds();
            cmn::g_CommonCounterInfo.freeCounter_[0] = 0;
        }
        if (proceeds_ == 0) {
            if (checkHaveItem(true) == false && checkHaveItem(false) == false) {
                showMessage(0x9488, 0x948b, -1);
                data_020ed1bc.setYesNo();
                mode_ = 3;
                return;
            }
            showMessage(0x9476, -1, -1);
            mode_ = 4;
            return;
        }
        TextAPI::setMACRO0(0x36, 0xf0000000, proceeds_);
        showMessage(0x9474, 0x9475, -1);
        status::g_Party.addGold(proceeds_);
        mode_ = 1;
        break;
    case 1:
        if (checkHaveItem(true) == false && checkHaveItem(false) == false) {
            showMessage(0x9488, 0x948b, -1);
            data_020ed1bc.setYesNo();
            mode_ = 3;
            return;
        }
        showMessage(0x9476, -1, -1);
        mode_ = 4;
        break;
    case 2: {
        int index = navigator_.getIndex(menuItem_.active_);
        int item;
        int count;
        if (MaterielMenuPlayerControl::getSingleton()->activeChara_ == 1) {
            item = status::g_Party.haveItemSack_.getItem(index);
            status::g_Party.haveItemSack_.execThrow(index);
            if (checkHaveItem(true) == false) {
                drawMode_ = 0;
                MaterielMenuPlayerControl::getSingleton()->activeChara_ = 0;
            }
            count = status::g_Party.haveItemSack_.getCount();
        } else {
            status::HaveStatusInfo& statusInfo = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_;
            item = statusInfo.haveItem_.getItem(index);
            statusInfo.execThrow(index);
            count = statusInfo.haveItem_.getCount();
            if (checkHaveItem(false) == false) {
                drawMode_ = 0;
            }
        }
        if (navigator_.getIndex(menuItem_.active_) >= count) {
            menuItem_.active_ = 0;
            navigator_.setPageNo(0);
            MaterielMenuPlayerControl::getSingleton()->activeItem_ = 0;
            MaterielMenuPlayerControl::getSingleton()->activeItemPage_ = 0;
        }
        redraw_ = 1;
        status::HaveItemSack& neneItemSack = status::g_Shop.haveItemNene_;
        neneItemSack.add(item);
        neneItemCount_ = status::g_Shop.haveItemNene_.getCount();
        TextAPI::setMACRO0(0xa, 0x40000000, item);
        showMessage(0x947b, 0x947c, -1);
        data_020ed1bc.setYesNo();
        mode_ = 1;
        break;
    }
    case 3:
        if (status::g_Shop.haveItemNene_.getCount() == 0) {
            showMessage(0x9493, -1, -1);
            mode_ = 5;
            return;
        }
        menuItem_.active_ = 0;
        navigator_.setup(2, 3, neneItemCount_);
        navigator_.setPageNo(0);
        drawMode_ = 2;
        MaterielMenuPlayerControl::getSingleton()->activeItem_ = 0;
        MaterielMenuPlayerControl::getSingleton()->activeItemPage_ = 0;
        redraw_ = 1;
        break;
    case 5:
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        break;
    }
}

THUMB void MaterielMenu_EXTRA_NENE::selectNo()
{
    switch (mode_) {
    case 0:
        showMessage(0x9478, -1, -1);
        mode_ = 5;
        break;
    case 1:
        showMessage(0x948b, -1, -1);
        data_020ed1bc.setYesNo();
        mode_ = 3;
        break;
    case 2:
        TextAPI::setMACRO0(0xa, 0x40000000, sellItem_);
        showMessage(0x947e, 0x947c, -1);
        data_020ed1bc.setYesNo();
        mode_ = 1;
        break;
    case 3:
        showMessage(0x9490, -1, -1);
        mode_ = 5;
        break;
    }
}

THUMB void MaterielMenu_EXTRA_NENE::checkSellItem()
{
    int index = navigator_.getIndex(menuItem_.active_);
    int itemID;
    switch (MaterielMenuPlayerControl::getSingleton()->activeChara_) {
    case 1:
        itemID = status::g_Party.haveItemSack_.getItem(index);
        break;
    default:
        itemID = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_.getItem(index);
        break;
    }
    for (int i = 0; i < 162; i++) {
        if (itemID == status::g_Shop.haveItemNene_.getItem(i) && status::g_Shop.haveItemNene_.getItemCount(i) == 99) {
            showMessage(0x9484, 0x9485, 0x948b);
            data_020ed1bc.setYesNo();
            mode_ = 3;
            return;
        }
    }
    if (status::UseItem::getSellType(itemID) == 1) {
        TextAPI::setMACRO0(0xa, 0x40000000, itemID);
        showMessage(0x9481, 0x947c, -1);
        mode_ = 1;
    } else {
        int value = status::UseItem::getBuyPrice(itemID);
        if (value == 0) {
            value = status::UseItem::getSellPrice(itemID);
        }
        value += value / 2;
        sellItem_ = itemID;
        TextAPI::setMACRO0(0xa, 0x40000000, itemID);
        TextAPI::setMACRO0(0x4b, 0xf0000000, value);
        showMessage(0x947a, -1, -1);
        mode_ = 2;
    }
    data_020ed1bc.setYesNo();
}

THUMB void MaterielMenu_EXTRA_NENE::calcProceeds()
{
    proceeds_ = 0;
    for (int i = 0; i < status::g_Shop.haveItemNene_.getCount();) {
        int count = status::g_Shop.haveItemNene_.getItemCount(i);
        int item = status::g_Shop.haveItemNene_.getItem(i);
        int sellCount = 0;
        int price = status::UseItem::getBuyPrice(item);
        if (price == 0) {
            price = status::UseItem::getSellPrice(item);
        }
        for (int j = 0; j < count; j++) {
            if (dssrand::rand(4) != 0) {
                proceeds_ += dss::getRandomVariation(price, -50, 100);
                sellCount++;
            }
        }
        for (int j = 0; j < sellCount; j++) {
            status::g_Shop.haveItemNene_.execThrow(i);
        }
        if (sellCount != count) {
            i++;
        }
    }
    neneItemCount_ = status::g_Shop.haveItemNene_.getCount();
}

THUMB bool MaterielMenu_EXTRA_NENE::checkHaveItem(bool sack)
{
    if (sack == true) {
        if (status::g_Party.haveItemSack_.getCount() == 0) {
            return false;
        }
    } else {
        if (status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_.getCount() == 0) {
            return false;
        }
    }
    return true;
}

THUMB void MaterielMenu_EXTRA_NENE::showMessage(int messageID1, int messageID2, int messageID3)
{
    data_020ed1bc.openMessageForTALK();
    data_020ed1bc.addMessage(messageID1);
    if (messageID2 != -1) {
        data_020ed1bc.addMessage(messageID2);
    }
    if (messageID3 != -1) {
        data_020ed1bc.addMessage(messageID3);
    }
}
