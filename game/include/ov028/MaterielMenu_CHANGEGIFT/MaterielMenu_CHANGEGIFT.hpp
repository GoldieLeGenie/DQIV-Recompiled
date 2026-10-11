#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MaterielMenu_SlotEnter.hpp"
#include "main/menu/MaterielMenuPlayerControl.hpp"
#include "ov028/MaterielMenu_SHOP/MaterielMenu_SHOP.hpp"

struct MaterielMenu_CHANGEGIFT_EQUIPCHECK : menu::MenuBase
{
    int itemID_;                        /* 0x1C */
    int activeChara_;                   /* 0x20 */
    int equipmode_;                     /* 0x24 */
    int fukuro_;                        /* 0x28 */
    int yesnoFlag_;                     /* 0x2C */
    int playSound_;                     /* 0x30 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void equipCheck();
    void setMessage();
    void equipYesMessage();
    void equipNoMessage();
    void aliveCheck();
    void getGift();
};

struct MaterielMenu_CHANGEGIFT_ROOT : menu::MenuBase
{
    int mode_;                          /* 0x1C */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void selectYes();
    void selectNo();
    void checkCoin();
};

struct MaterielMenu_CHANGEGIFT_SELECTCHARA : menu::MenuBase
{
    menu::MenuItem menuItem_;           /* 0x1C */
    menu::MenuItem menuItem2_;          /* 0x80 */
    CursorMoveGridLoop navigator_;     /* 0xE4 */
    int activeChara_;                   /* 0xF0 */
    int maxCharaCount_;                 /* 0xF4 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void cancelChange();
};

struct MaterielMenu_CHANGEGIFT_SELECTGIFT : menu::MenuBase
{
    menu::MenuItem menuItem_;           /* 0x1C */
    menu::MenuItem menuItem2_;          /* 0x80 */
    CursorMoveGridLoop navigator_;     /* 0xE4 */
    int activeItem_;                    /* 0xF0 */
    int itemCount_;                     /* 0xF4 */
    int fukuroItemCount_[6];            /* 0xF8 */
    int message_;                       /* 0x110 */
    int selectChara_;                   /* 0x114 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void checkAmount();
    void cancelChange();
};

extern MaterielMenu_CHANGEGIFT_ROOT gMaterielMenu_CHANGEGIFT_ROOT;
extern MaterielMenu_CHANGEGIFT_EQUIPCHECK gMaterielMenu_CHANGEGIFT_EQUIPCHECK;
extern MaterielMenu_CHANGEGIFT_SELECTCHARA gMaterielMenu_CHANGEGIFT_SELECTCHARA;
extern MaterielMenu_CHANGEGIFT_SELECTGIFT gMaterielMenu_CHANGEGIFT_SELECTGIFT;

