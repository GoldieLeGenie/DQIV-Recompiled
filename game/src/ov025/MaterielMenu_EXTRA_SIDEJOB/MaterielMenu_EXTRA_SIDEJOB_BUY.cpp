#include "ov025/MaterielMenu_EXTRA_SIDEJOB/MaterielMenu_EXTRA_SIDEJOB.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/dss/Random.hpp"
#include "main/status/ShopList.hpp"
#include "main/status/UseItem.hpp"

THUMB void MaterielMenu_EXTRA_SIDEJOB_BUY::menuSetup()
{
    mode_ = JOB_FIRST;
}

THUMB void MaterielMenu_EXTRA_SIDEJOB_BUY::menuExecute()
{
}

THUMB void MaterielMenu_EXTRA_SIDEJOB_BUY::menuDraw()
{
}

THUMB void MaterielMenu_EXTRA_SIDEJOB_BUY::menuUpdate()
{
    if (g_TownMenuMessage.isOpen()) {
        if (g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
            g_TownMenuMessage.close();
            selectYes();
        } else if (g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            g_TownMenuMessage.close();
            selectNo();
        }
    }
}

THUMB void MaterielMenu_EXTRA_SIDEJOB_BUY::selectYes()
{
    switch (mode_) {
    case JOB_FIRST:
        getSellItem();
        g_TownMenuMessage.openMessageForTALK();
        TextAPI::setMACRO0(0xa, 0x40000000, itemID_);
        TextAPI::setMACRO0(0x3e, 0xf0000000, status::UseItem::getSellPrice(itemID_));
        g_TownMenuMessage.addMessage(0x63ee);
        g_TownMenuMessage.setYesNo();
        mode_ = JOB_SELL;
        break;
    case JOB_SELL:
        if (status::g_Shop.sideJobItemFlag_[itemIndex_] == 0) {
            status::g_Shop.sideJobItemFlag_[itemIndex_] = 1;
        }
        g_TownMenuMessage.openMessageForTALK();
        g_TownMenuMessage.addMessage(0x63ef);
        mode_ = JOB_END;
        break;
    case JOB_END:
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        break;
    }
}

THUMB void MaterielMenu_EXTRA_SIDEJOB_BUY::selectNo()
{
    switch (mode_) {
    case JOB_FIRST:
        g_TownMenuMessage.openMessageForTALK();
        g_TownMenuMessage.addMessage(0x63f3);
        mode_ = JOB_END;
        break;
    case JOB_SELL:
        g_TownMenuMessage.openMessageForTALK();
        g_TownMenuMessage.addMessage(0x63f1);
        mode_ = JOB_END;
        break;
    case JOB_END:
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        break;
    }
}

THUMB void MaterielMenu_EXTRA_SIDEJOB_BUY::getSellItem()
{
    int odds = dssrand::rand(SELL_ITEM_ODDS);
    if (odds > SELL_HAJANOTURUGI) {
        itemID_ = 0x24;
        itemIndex_ = HAJANOTURUGI_INDEX;
    } else if (odds > SELL_KUSARIGAMA) {
        itemID_ = 5;
        itemIndex_ = KUSARIGAMA_INDEX;
    } else if (odds > SELL_KUROSUBOU) {
        itemID_ = 0xc;
        itemIndex_ = KUROSUBOU_INDEX;
    } else if (odds > SELL_SEINARUNAIHU) {
        itemID_ = 0xa;
        itemIndex_ = SEINARUNAIHU_INDEX;
    } else if (odds > SELL_DOUNOTURUGI) {
        itemID_ = 3;
        itemIndex_ = DOUNOTURUGI_INDEX;
    } else {
        itemID_ = 2;
        itemIndex_ = KONBOU_INDEX;
    }
}
