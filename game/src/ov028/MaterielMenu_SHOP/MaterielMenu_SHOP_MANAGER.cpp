#include "ov028/MaterielMenu_SHOP/MaterielMenu_SHOP.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/ShopList.hpp"
#include "main/status/UseItem.hpp"
#include "main/profile/Profile.hpp"

THUMB MaterielMenu_SHOP_MANAGER* MaterielMenu_SHOP_MANAGER::getSingleton()
{
    static MaterielMenu_SHOP_MANAGER shopManager;
    return &shopManager;
}

THUMB void MaterielMenu_SHOP_MANAGER::allClear()
{
    for (int i = 0; i < 9; i++) {
        item_[i] = 0;
        itemPrice_[i] = 0;
        itemQuantity_[i] = 0;
    }
    shopType_ = 0;
    shopAction_ = 0;
    sellQuantity_ = 0;
    extraShop_ = 0;
}

THUMB void MaterielMenu_SHOP_MANAGER::openShopMenu(int type)
{
    setShopType(type);
    gMaterielMenu_SHOP_ROOT.open();
}

THUMB void MaterielMenu_SHOP_MANAGER::initializeShopItem()
{
    char mapName[3] = {0, 0, 0};
    mapName[0] = g_Stage.getMapName()[0];
    mapName[1] = g_Stage.getMapName()[1];
    int minusCount = 0;
    sellItemCount_ = status::g_Shop.getShopCount(shopType_);
    for (int i = 0; i < sellItemCount_; i++) {
        if (status::g_Story.chapter_ == 3) {
            if (dss::strcmp(mapName, "mf") == 0 && shopType_ == 2 && i > 2) {
                if (status::g_Shop.sideJobItemFlag_[i] == 1) {
                    item_[i - minusCount] = status::g_Shop.getShopItem(shopType_, i);
                    itemPrice_[i - minusCount] = status::g_Shop.getShopPrice(shopType_, i);
                    itemQuantity_[i - minusCount] = 1;
                } else {
                    minusCount++;
                }
            } else {
                item_[i] = status::g_Shop.getShopItem(shopType_, i);
                itemPrice_[i] = status::g_Shop.getShopPrice(shopType_, i);
                itemQuantity_[i] = 1;
            }
        } else {
            item_[i] = status::g_Shop.getShopItem(shopType_, i);
            itemPrice_[i] = status::g_Shop.getShopPrice(shopType_, i);
            itemQuantity_[i] = 1;
            if (dss::strcmp(g_Stage.getMapName(), "ss5b1b") == 0) {
                itemPrice_[i] *= 10;
            }
        }
    }
    sellItemCount_ -= minusCount;
}

THUMB void MaterielMenu_SHOP_MANAGER::setShopType(int type)
{
    shopType_ = type;
    initializeShopItem();
}

THUMB int MaterielMenu_SHOP_MANAGER::getShopType()
{
    return shopType_;
}

THUMB void MaterielMenu_SHOP_MANAGER::setExtraShop(int type)
{
    extraShop_ = type;
}

THUMB int MaterielMenu_SHOP_MANAGER::getExtraShop()
{
    return extraShop_;
}

THUMB void MaterielMenu_SHOP_MANAGER::setShopAction(int action)
{
    shopAction_ = action;
}

THUMB int MaterielMenu_SHOP_MANAGER::getShopAction()
{
    return shopAction_;
}

THUMB int MaterielMenu_SHOP_MANAGER::getItem(int index)
{
    return item_[index];
}

THUMB int MaterielMenu_SHOP_MANAGER::getItemPrice(int index)
{
    return itemPrice_[index];
}

THUMB int MaterielMenu_SHOP_MANAGER::getItemPriceSum(int index)
{
    return itemPrice_[index] * itemQuantity_[index];
}

THUMB int MaterielMenu_SHOP_MANAGER::getItemQuantity(int index)
{
    return itemQuantity_[index];
}

THUMB int MaterielMenu_SHOP_MANAGER::getSellItemCount()
{
    return sellItemCount_;
}

THUMB void MaterielMenu_SHOP_MANAGER::setSellQuantity(int quantity)
{
    sellQuantity_ = quantity;
}

THUMB int MaterielMenu_SHOP_MANAGER::getSellQuantity()
{
    return sellQuantity_;
}

THUMB bool MaterielMenu_SHOP_MANAGER::buyItem(int index, int activeChara)
{
    status::g_Party.setPlayerMode();
    payOut(index);
    if (status::g_Party.fukuro_ != 0 && activeChara == status::g_Party.getCount()) {
        status::g_Party.haveItemSack_.adds(item_[index], itemQuantity_[index]);
        return true;
    }
    if (activeChara < status::g_Party.getCount()) {
        for (int i = 0; i < itemQuantity_[index]; i++) {
            if (status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.haveItem_.getCount() == 12) {
                itemQuantity_[index] -= i;
                status::g_Party.haveItemSack_.adds(item_[index], itemQuantity_[index]);
                itemQuantity_[index] = 1;
                return false;
            }
            status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.haveItem_.add(item_[index]);
        }
        itemQuantity_[index] = 1;
        return true;
    }
    return false;
}

THUMB bool MaterielMenu_SHOP_MANAGER::sellItem(int activeItem, int activeChara, int price)
{
    status::g_Party.setPlayerMode();
    int itemID;
    if (activeChara == status::g_Party.getCount()) {
        itemID = status::g_Party.haveItemSack_.getItem(activeItem);
    } else {
        itemID = status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.haveItem_.getItem(activeItem);
    }
    if (price == -1) {
        price = status::UseItem::getSellPrice(itemID);
    }
    if (activeChara == status::g_Party.getCount()) {
        if (sellOut(price)) {
            for (int i = 0; i < sellQuantity_; i++) {
                status::g_Party.haveItemSack_.execThrow(activeItem);
            }
            return true;
        }
    } else {
        if (sellOut(price)) {
            for (int i = 0; i < sellQuantity_; i++) {
                status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.execThrow(activeItem);
            }
            return true;
        }
    }
    return false;
}

THUMB void MaterielMenu_SHOP_MANAGER::resetItemQuantity()
{
    for (int i = 0; i < sellItemCount_; i++) {
        itemQuantity_[i] = 1;
    }
}

THUMB void MaterielMenu_SHOP_MANAGER::addItem(int index)
{
    itemQuantity_[index]++;
    if (itemQuantity_[index] > 9) {
        itemQuantity_[index] = 1;
    }
}

THUMB void MaterielMenu_SHOP_MANAGER::subItem(int index)
{
    itemQuantity_[index]--;
    if (itemQuantity_[index] < 1) {
        itemQuantity_[index] = 9;
    }
}

THUMB void MaterielMenu_SHOP_MANAGER::setItemQuantity(int index, int quantity)
{
    itemQuantity_[index] = quantity;
}

THUMB bool MaterielMenu_SHOP_MANAGER::sellOK(int activeChara)
{
    if (status::g_Party.gold_ == 999999) {
        return false;
    }
    for (int i = 0; i < status::g_Party.getCount(); i++) {
        if (status::g_Party.getPlayerStatus(activeChara)->haveStatusInfo_.haveItem_.getCount() != 0) {
            return true;
        }
    }
    if (status::g_Party.fukuro_ != 0 && status::g_Party.haveItemSack_.getCount() != 0) {
        return true;
    }
    return false;
}

THUMB void MaterielMenu_SHOP_MANAGER::payOut(int index)
{
    unsigned int gold = status::g_Party.gold_;
    gold -= itemPrice_[index] * itemQuantity_[index];
    status::g_Party.setGold(gold);
}

THUMB bool MaterielMenu_SHOP_MANAGER::sellOut(int price)
{
    unsigned int gold = status::g_Party.gold_ + price * sellQuantity_;
    if (gold > 999999) {
        if (extraShop_ == 2) {
            status::g_Party.setGold(999999);
            return true;
        }
        return false;
    }
    status::g_Party.setGold(gold);
    return true;
}
