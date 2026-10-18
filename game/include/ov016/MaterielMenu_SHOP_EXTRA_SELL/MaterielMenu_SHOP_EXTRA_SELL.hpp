#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/CursorMoveGridLoop.hpp"

// extra shop: item sale at a random price (opened by MaterielMenu_SHOP_WHO_SELL)
struct MaterielMenu_SHOP_EXTRA_SELL : menu::MenuBase
{
    int activeChara_;                       /* 0x1C */
    int activeItem_;                        /* 0x20 */
    int activeItemPage_;                    /* 0x24 */
    int pageMax_;                           /* 0x28 */
    int mode_;                              /* 0x2C */
    int price_;                             /* 0x30 */
    menu::MenuItem menuItem_;               /* 0x34 */
    menu::MenuItem cancelItem_;             /* 0x98 */
    CursorMoveGridLoop navigator_;          /* 0xFC */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    int updateMessage();
    void offerPrice(int item);
    void resetCursor();
    void returnToWhoSell();
    void showMessages(int message, int message2);
};

extern MaterielMenu_SHOP_EXTRA_SELL gUnkMaterielMenu_0216b304;
