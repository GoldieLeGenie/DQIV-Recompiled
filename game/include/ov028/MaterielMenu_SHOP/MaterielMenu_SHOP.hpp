#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/dss/UnkBgBuffer.hpp"
#include "main/menu/MaterielMenu_SlotEnter.hpp"
#include "main/menu/MaterielMenuPlayerControl.hpp"

struct MaterielMenu_SHOP_MESSAGE_MANAGER
{
    int shopType_;                      /* 0x00 */

    static MaterielMenu_SHOP_MESSAGE_MANAGER* getSingleton();
    int idle();
    int buy();
    int sell();
    int yameru();
    int cancel();
    void noMoney(int* mes);
    void buyItem(bool plural, bool battleUse, int* mes);
    int haveItemMax();
    void buyToSack(bool haveMoney, int* mes);
    int getItem(bool isBasha, bool isDeath);
    int checkMoney(int overItem, bool haveNoMoney, int* mes);
    int haveOther();
    int haveWhose();
    int haveSomething();
    int sortEnd();
    int checkEquip(bool equip);
    int equipCurseItem(bool first);
    int equipItem();
    void haveNoItem(bool fukuro, int* mes);
    int sellOK();
    int sellHowMany();
    int sellPluralSellOK();
    void sellNG(bool sellYet, int* mes);
    int sellDifficult();
    void celectNo(bool sellYet, int* mes);
    void sellCurse(bool sellYet, int* mes);
    void sellEnd(bool sellYet, int* mes);
    void overMoney(int* mes);
};

struct MaterielMenu_SHOP_MANAGER
{
    int shopType_;                      /* 0x00 */
    int sellItemCount_;                 /* 0x04 */
    int shopAction_;                    /* 0x08 */
    int item_[6];                       /* 0x0C */
    int itemPrice_[6];                  /* 0x24 */
    int itemQuantity_[6];               /* 0x3C */
    int sellQuantity_;                  /* 0x54 */
    int extraShop_;                     /* 0x58 */

    static MaterielMenu_SHOP_MANAGER* getSingleton();
    void allClear();
    void openShopMenu(int type);
    void initializeShopItem();
    void setShopType(int type);
    int getShopType();
    void setExtraShop(int type);
    int getExtraShop();
    void setShopAction(int action);
    int getShopAction();
    int getItem(int index);
    int getItemPrice(int index);
    int getItemPriceSum(int index);
    int getItemQuantity(int index);
    int getSellItemCount();
    void setSellQuantity(int quantity);
    int getSellQuantity();
    bool buyItem(int index, int activeChara);
    bool sellItem(int activeItem, int activeChara, int price);
    void resetItemQuantity();
    void addItem(int index);
    void subItem(int index);
    void setItemQuantity(int index, int quantity);
    bool sellOK(int activeChara);
    void payOut(int index);
    bool sellOut(int price);
};

struct MaterielMenu_SHOP_ROOT : menu::MenuBase
{
    int mode_;                          /* 0x1C */
    menu::MenuItem menuItem_;           /* 0x20 */
    CursorMoveGridLoop navigator_;     /* 0x84 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void execConduct();
    void showMessage(int mes);
};

struct MaterielMenu_SHOP_BUYMENU : menu::MenuBase
{
    menu::MenuItem menuItem_;           /* 0x1C */
    menu::MenuItem menuItem2_;          /* 0x80 */
    int activeItem_;                    /* 0xE4 */
    int fukuroItemCount_[6];            /* 0xE8 */
    int message_;                       /* 0x100 */
    CursorMoveGridLoop navigator_;     /* 0x104 */
#ifdef DQIV_NATIVE
    // Category tabs of the list (L / R): rows_ are the shop indices shown.
    int filter_ = 0;
    int rows_[6] = {};
    int rowCount_ = 0;
    int tabs_[8] = {};
    int tabCount_ = 0;
    int tab_ = 0;
    int fresh_ = 0; // set by the shop's root menu when it opens this one
    void buildRows();
    void buildTabs();
    int rowOf(int item) const;
#endif

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void checkBuy();
    void changeQuantity(bool add);
};

struct MaterielMenu_SHOP_EQUIPCHECK : menu::MenuBase
{
    int charAllocation_;                /* 0x1C */
    int itemAllocation_;                /* 0x20 */
    int unk_24;                         /* 0x24 */
    int mode_;                          /* 0x28 */
    int haveItemOver_;                  /* 0x2C */
    signed char ctrlID_;                /* 0x30 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void yesAdmin();
    void noAdmin();
    void giveItem();
    void messageSetup();
    void showMessage(int mes);
    void rerurnRoot();
    void checkMoneyMessage(int* mes, int mesCount, bool haveNoMoney);
};

struct MaterielMenu_SHOP_WHOSE : menu::MenuBase
{
    menu::MenuItem menuItem_;           /* 0x1C */
    menu::MenuItem menuItem2_;          /* 0x80 */
    int activeChara_;                   /* 0xE4 */
    int activeItem_;                    /* 0xE8 */
    int mode_;                          /* 0xEC */
    int noSort_;                        /* 0xF0 */
    int maxCharaCount_;                 /* 0xF4 */
    int yesno_;                         /* 0xF8 */
    int endMessage_;                    /* 0xFC */
    CursorMoveGridLoop navigator_;     /* 0x100 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void selectYes();
    void selectNo();
    void yesSort();
    void haveMaxCheck();
    void checkMoney();
    void giveBuyItem();
    void showMessage(int mes);
};

struct MaterielMenu_SHOP_WHO_SELL : menu::MenuBase
{
    int activeChara_;                   /* 0x1C */
    int unk_20;                         /* 0x20 */
    int maxCharaCount_;                 /* 0x24 */
    int messageCurse_;                  /* 0x28 */
    int return_;                        /* 0x2C */
    int extraShop_;                     /* 0x30 */
    int extraMode_;                     /* 0x34 */
    menu::MenuItem menuItem_;           /* 0x38 */
    menu::MenuItem menuItem2_;          /* 0x9C */
    CursorMoveGridLoop navigator_;     /* 0x100 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void cancel();
    bool extraMessageUpdata();
    void showMessage(int mes1, int mes2);
};

struct MaterielMenu_SHOP_VALUE : menu::MenuBase
{
    int activeItem_;                    /* 0x1C */
    int activeChara_;                   /* 0x20 */
    int sellType_;                      /* 0x24 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void selectYes();
    void selectNo();
    void checkHaveMoney();
    bool checkItemMoney();
    void showMessage(int mes1, int mes2, int mes3);
};

struct MaterielMenu_SHOP_SELL_ITEM : menu::MenuBase
{
    int activeChara_;                   /* 0x1C */
    int itemIndex_;                     /* 0x20 */
    int unk_24;                         /* 0x24 */
    int pageStart_;                     /* 0x28 */
    menu::MenuItem menuItem_;           /* 0x2C */
    menu::MenuItem menuItem2_;          /* 0x90 */
    menu::MenuItem menuItem3_;          /* 0xF4 */
    CursorMoveGridLoop navigator_;     /* 0x158 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
};

struct MaterielMenu_SHOP_SELL_SACK : menu::MenuBase
{
    int itemIndex_;                     /* 0x1C */
    int itemCount_;                     /* 0x20 */
    int pageStart_;                     /* 0x24 */
    menu::MenuItem menuItem_;           /* 0x28 */
    menu::MenuItem menuItem3_;          /* 0x8C */
    menu::MenuItem menuItem2_;          /* 0xF0 */
    CursorMoveGridLoop navigator_;     /* 0x154 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void selectItem();
    void changeItem();
};

struct MaterielMenu_SHOP_SELL_QUANTITY : menu::MenuBase
{
    int quantity_;                      /* 0x1C */
    int itemIndex_;                     /* 0x20 */
    int unk_24;                         /* 0x24 */
    menu::MenuItem menuItem_;           /* 0x28 */
    CursorMoveGridLoop navigator_;     /* 0x8C */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void changeQuantity(bool add);
};

extern MaterielMenu_SHOP_VALUE gMaterielMenu_SHOP_VALUE;
extern MaterielMenu_SHOP_EQUIPCHECK gMaterielMenu_SHOP_EQUIPCHECK;
extern MaterielMenu_SHOP_ROOT gMaterielMenu_SHOP_ROOT;
extern MaterielMenu_SHOP_WHOSE gMaterielMenu_SHOP_WHOSE;
extern MaterielMenu_SHOP_WHO_SELL gMaterielMenu_SHOP_WHO_SELL;
extern MaterielMenu_SHOP_SELL_SACK gMaterielMenu_SHOP_SELL_SACK;
extern MaterielMenu_SHOP_SELL_ITEM gMaterielMenu_SHOP_SELL_ITEM;
extern MaterielMenu_SHOP_SELL_QUANTITY gMaterielMenu_SHOP_SELL_QUANTITY;
extern MaterielMenu_SHOP_BUYMENU gMaterielMenu_SHOP_BUYMENU;

