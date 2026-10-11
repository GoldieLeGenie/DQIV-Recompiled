#include "ov028/MaterielMenu_SHOP/MaterielMenu_SHOP.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/UseItem.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"

THUMB void MaterielMenu_SHOP_SELL_QUANTITY::menuSetup()
{
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    menuItem_.active_ = 0;
    navigator_.setupBase();
    quantity_ = 1;
    MaterielMenuPlayerControl* control = MaterielMenuPlayerControl::getSingleton();
    itemIndex_ = control->activeItem_ + control->activeItemPage_ * 6;
    unk_24 = 0;
    TextAPI::setMACRO0(0xa, 0x40000000, status::g_Party.haveItemSack_.getItem(itemIndex_));
    data_020ed1bc.openMessageForTALK();
    data_020ed1bc.addMessageNOWAIT(MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->sellHowMany());
    data_020ed1bc.addMessageWAITKEY();
}

THUMB void MaterielMenu_SHOP_SELL_QUANTITY::menuExecute()
{
    MenuTemplate_materiel::shopSellQuantity(&menuItem_, menuItem_.active_);
}

THUMB void MaterielMenu_SHOP_SELL_QUANTITY::menuDraw()
{
    if (data_020ed1bc.isMessageWAITPROG()) {
        unkfunc_0216fc2c(quantity_);
        menuItem_.drawActive();
    } else if (data_020ed1bc.isOpen()) {
        unkfunc_0216fc58();
    }
}

THUMB void MaterielMenu_SHOP_SELL_QUANTITY::menuUpdate()
{
    if (data_020ed1bc.isMessageWAITPROG() && unk_24 == 0) {
        redraw_ = 1;
        unk_24 = 1;
    }
    navigator_.setup(1, 1, 1);
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result != 0) {
        if (result == 2) {
            data_020ed1bc.clearMessageWAITPROG();
            MaterielMenu_SHOP_MANAGER::getSingleton()->setSellQuantity(quantity_);
            close();
            gMaterielMenu_SHOP_VALUE.sellType_ = status::UseItem::getSellType(status::g_Party.haveItemSack_.getItem(itemIndex_));
            gMaterielMenu_SHOP_VALUE.open();
        }
        if (result == 7) {
            changeQuantity(true);
        }
        if (result == 6) {
            changeQuantity(false);
        }
        if (result == 3) {
            int mes[2];
            data_020ed1bc.clearMessageWAITPROG();
            close();
            MaterielMenu_SHOP_MESSAGE_MANAGER::getSingleton()->celectNo(false, mes);
            data_020ed1bc.openMessageForTALK();
            data_020ed1bc.addMessage(mes[0], mes[1]);
            data_020ed1bc.setMessageLastCursor(true);
            gMaterielMenu_SHOP_SELL_SACK.open();
        }
        redraw_ = 1;
    }
    if (data_020ed1bc.isOpen() && (unsigned int)(data_020ed1bc.stat_ - 1) <= 1) {
        data_020ed1bc.close();
    }
}

THUMB void MaterielMenu_SHOP_SELL_QUANTITY::changeQuantity(bool add)
{
    if (add) {
        quantity_++;
        if (quantity_ > status::g_Party.haveItemSack_.getItemCount(itemIndex_)) {
            quantity_ = 1;
        }
    } else {
        quantity_--;
        if (quantity_ < 1) {
            quantity_ = status::g_Party.haveItemSack_.getItemCount(itemIndex_);
        }
    }
}
