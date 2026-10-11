#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MaterielMenu_SlotEnter.hpp"

struct MaterielMenu_EXTRA_SIDEJOB_BUY : menu::MenuBase
{
    enum {
        JOB_FIRST = 0,
        JOB_SELL = 1,
        JOB_END = 2
    };
    enum {
        KONBOU_INDEX = 0,
        DOUNOTURUGI_INDEX = 1,
        SEINARUNAIHU_INDEX = 2,
        KUROSUBOU_INDEX = 3,
        KUSARIGAMA_INDEX = 4,
        HAJANOTURUGI_INDEX = 5
    };
    static const int SELL_ITEM_ODDS = 32;
    static const int SELL_DOUNOTURUGI = 9;
    static const int SELL_SEINARUNAIHU = 18;
    static const int SELL_KUROSUBOU = 23;
    static const int SELL_KUSARIGAMA = 27;
    static const int SELL_HAJANOTURUGI = 29;

    int mode_;                          /* 0x1C */
    int itemID_;                        /* 0x20 */
    int itemIndex_;                     /* 0x24 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void selectYes();
    void selectNo();
    void getSellItem();
};

struct MaterielMenu_EXTRA_SIDEJOB_ROOT : menu::MenuBase
{
    enum {
        SIDEJOB_BUY = 0,
        SIDEJOB_SELL = 1,
        SIDEJOB_END = 2
    };
    static const int NEXT_MENU_ODDS = 8;

    int mode_;                          /* 0x1C */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
};

struct MaterielMenu_EXTRA_SIDEJOB_SELL : menu::MenuBase
{
    enum {
        SELL_FIRST = 0,
        SELL_WAIT = 1,
        SELL_ITEM = 2,
        SELL_CHECK = 3,
        SELL_ABATEMENT = 4,
        SELL_ADVANCE = 5,
        SELL_END = 6
    };
    enum {
        NO_MONEY = 0,
        HAVE_MAX_ITEM = 1,
        NOT_EQUIPMENT = 2,
        ABATEMENT_PRICE = 3,
        ADVANCE_PRICE = 4,
        CANCEL_SELL = 5
    };
    static const int SIDEJOB_SHOP_NO = 2;
    static const int SELL_ITEM_COUNT = 6;
    static const int ODDS_SELECT = 12;
    static const int WAIT_COUNT = 60;   

    int mode_;                          /* 0x1C */
    int sellItemID_;                    /* 0x20 */
    int sellItemPrice_;                 /* 0x24 */
    int waitCount_;                     /* 0x28 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void selectYes();
    void selectNo();
    void getSellItem();
    void sellItemYesCheck();
    void sellItemNoCheck();
    void addPay();
};

extern MaterielMenu_EXTRA_SIDEJOB_ROOT gMaterielMenu_EXTRA_SIDEJOB_ROOT;
extern MaterielMenu_EXTRA_SIDEJOB_BUY gMaterielMenu_EXTRA_SIDEJOB_BUY;
extern MaterielMenu_EXTRA_SIDEJOB_SELL gMaterielMenu_EXTRA_SIDEJOB_SELL;

