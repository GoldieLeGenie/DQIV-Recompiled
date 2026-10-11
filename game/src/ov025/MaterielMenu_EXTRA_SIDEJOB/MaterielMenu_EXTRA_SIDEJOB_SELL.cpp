#include "ov025/MaterielMenu_EXTRA_SIDEJOB/MaterielMenu_EXTRA_SIDEJOB.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/dss/Random.hpp"
#include "main/status/ShopList.hpp"
#include "main/status/UseItem.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"

THUMB void MaterielMenu_EXTRA_SIDEJOB_SELL::menuSetup()
{
    mode_ = SELL_FIRST;
    sellItemID_ = 0;
    waitCount_ = 0;
    sellItemPrice_ = 0;
    getSellItem();
}

THUMB void MaterielMenu_EXTRA_SIDEJOB_SELL::menuExecute()
{
}

THUMB void MaterielMenu_EXTRA_SIDEJOB_SELL::menuDraw()
{
    if (mode_ >= SELL_WAIT) {
        unkfunc_0216ff10();
    }
}

THUMB void MaterielMenu_EXTRA_SIDEJOB_SELL::menuUpdate()
{
    if (data_020ed1bc.isOpen()) {
        if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
            data_020ed1bc.close();
            selectYes();
        } else if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            selectNo();
        }
    } else if (mode_ == SELL_WAIT) {
        if (waitCount_ > WAIT_COUNT) {
            data_020ed1bc.openMessageForTALK();
            TextAPI::setMACRO0(0xa, 0x40000000, sellItemID_);
            TextAPI::setMACRO0(0x3e, 0xf0000000, status::UseItem::getBuyPrice(sellItemID_));
            data_020ed1bc.addMessage(0x63c3);
            data_020ed1bc.setYesNo();
            mode_ = SELL_ITEM;
        } else {
            waitCount_++;
        }
    }
}

THUMB void MaterielMenu_EXTRA_SIDEJOB_SELL::selectYes()
{
    switch (mode_) {
    case SELL_FIRST:
        mode_ = SELL_WAIT;
        break;
    case SELL_ITEM:
        sellItemYesCheck();
        break;
    case SELL_CHECK:
        data_020ed1bc.openMessageForTALK();
        data_020ed1bc.addMessage(0x63e2);
        sellItemPrice_ /= 16;
        addPay();
        mode_ = SELL_END;
        break;
    case SELL_ABATEMENT:
        data_020ed1bc.openMessageForTALK();
        data_020ed1bc.addMessage(0x63c9);
        sellItemPrice_ = 1;
        addPay();
        mode_ = SELL_END;
        break;
    case SELL_ADVANCE:
        if (dssrand::rand(4) == 0) {
            data_020ed1bc.openMessageForTALK();
            data_020ed1bc.addMessage(0x63d3);
            sellItemPrice_ = (sellItemPrice_ + sellItemPrice_ / 10) / 8;
            addPay();
            mode_ = SELL_END;
        } else {
            data_020ed1bc.openMessageForTALK();
            data_020ed1bc.addMessage(0x63d6);
            mode_ = SELL_END;
        }
        break;
    case SELL_END:
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        break;
    }
}

THUMB void MaterielMenu_EXTRA_SIDEJOB_SELL::selectNo()
{
    switch (mode_) {
    case SELL_FIRST:
        data_020ed1bc.openMessageForTALK();
        data_020ed1bc.addMessage(0x63e6);
        data_020ed1bc.setYesNo();
        break;
    case SELL_ITEM:
        sellItemNoCheck();
        break;
    case SELL_CHECK:
        data_020ed1bc.openMessageForTALK();
        data_020ed1bc.addMessage(0x63e4);
        mode_ = SELL_END;
        break;
    case SELL_ABATEMENT:
    case SELL_ADVANCE:
        data_020ed1bc.openMessageForTALK();
        TextAPI::setMACRO0(0xa, 0x40000000, sellItemID_);
        TextAPI::setMACRO0(0x3e, 0xf0000000, status::UseItem::getBuyPrice(sellItemID_));
        data_020ed1bc.addMessage(0x63c6, 0x63c3);
        data_020ed1bc.setYesNo();
        mode_ = SELL_ITEM;
        break;
    case SELL_END:
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        break;
    }
}

THUMB void MaterielMenu_EXTRA_SIDEJOB_SELL::getSellItem()
{
    int item[SELL_ITEM_COUNT] = { 0, 0, 0, 0, 0, 0 };
    int count = 0;
    for (int i = 0; i < SELL_ITEM_COUNT; i++) {
        if (status::g_Shop.sideJobItemFlag_[i] == 1) {
            item[count] = status::g_Shop.getShopItem(SIDEJOB_SHOP_NO, i);
            count++;
        }
    }
    sellItemID_ = item[dssrand::rand(count)];
    sellItemPrice_ = status::UseItem::getBuyPrice(sellItemID_);
}

THUMB void MaterielMenu_EXTRA_SIDEJOB_SELL::sellItemYesCheck()
{
    int odds = dssrand::rand(ODDS_SELECT);
    if (odds == NO_MONEY) {
        data_020ed1bc.openMessageForTALK();
        data_020ed1bc.addMessage(0x63d9, 0x63da);
        mode_ = SELL_END;
    } else if (odds == HAVE_MAX_ITEM) {
        data_020ed1bc.openMessageForTALK();
        data_020ed1bc.addMessage(0x63dd, 0x63de);
        mode_ = SELL_END;
    } else if (odds == NOT_EQUIPMENT) {
        TextAPI::setMACRO0(0xa, 0x40000000, sellItemID_);
        data_020ed1bc.openMessageForTALK();
        data_020ed1bc.addMessage(0x63e1);
        data_020ed1bc.setYesNo();
        mode_ = SELL_CHECK;
    } else {
        data_020ed1bc.openMessageForTALK();
        data_020ed1bc.addMessage(0x63c9);
        sellItemPrice_ /= 16;
        addPay();
        mode_ = SELL_END;
    }
}

THUMB void MaterielMenu_EXTRA_SIDEJOB_SELL::sellItemNoCheck()
{
    int odds = dssrand::rand(ODDS_SELECT);
    TextAPI::setMACRO0(0xa, 0x40000000, sellItemID_);
    if (odds == ABATEMENT_PRICE) {
        int price = sellItemPrice_ * 9 / 10;
        data_020ed1bc.openMessageForTALK();
        TextAPI::setMACRO0(0x4b, 0xf0000000, price);
        data_020ed1bc.addMessage(0x63cc);
        data_020ed1bc.setYesNo();
        mode_ = SELL_ABATEMENT;
    } else if (odds == ADVANCE_PRICE) {
        int price = sellItemPrice_;
        data_020ed1bc.openMessageForTALK();
        TextAPI::setMACRO0(0x4b, 0xf0000000, price + price / 10);
        data_020ed1bc.addMessage(0x63d0);
        data_020ed1bc.setYesNo();
        mode_ = SELL_ADVANCE;
    } else if (odds == CANCEL_SELL) {
        data_020ed1bc.openMessageForTALK();
        data_020ed1bc.addMessage(0x63d6);
        mode_ = SELL_END;
    } else {
        data_020ed1bc.openMessageForTALK();
        TextAPI::setMACRO0(0xa, 0x40000000, sellItemID_);
        TextAPI::setMACRO0(0x3e, 0xf0000000, status::UseItem::getBuyPrice(sellItemID_));
        data_020ed1bc.addMessage(0x63c6, 0x63c3);
        data_020ed1bc.setYesNo();
        mode_ = SELL_ITEM;
    }
}

THUMB void MaterielMenu_EXTRA_SIDEJOB_SELL::addPay()
{
    status::g_Shop.pay_ += sellItemPrice_;
    for (int i = 3; i < SELL_ITEM_COUNT; i++) {
        if (sellItemID_ == status::g_Shop.getShopItem(SIDEJOB_SHOP_NO, i)) {
            status::g_Shop.sideJobItemFlag_[i] = 0;
            return;
        }
    }
}
