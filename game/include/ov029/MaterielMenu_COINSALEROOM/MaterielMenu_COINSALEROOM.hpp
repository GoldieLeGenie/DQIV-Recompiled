#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MaterielMenu_SlotEnter.hpp"

struct MaterielMenu_COINSALEROOM_ROOT : menu::MenuBase
{
    menu::MenuItem menuItem_;           /* 0x1C */
    CursorMoveGridLoop navigator_;     /* 0x80 */
    int coin_;                          /* 0x8C */
    int first_;                         /* 0x90 */
    int blink_;                         /* 0x94 */
    unsigned char blinkCount_;          /* 0x98 */
    signed char mode_;                  /* 0x99 */
    unsigned char oldActive_;           /* 0x9A */
    unsigned char coinPrice_;           /* 0x9B */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    bool messageUpdata();
    void buyCoinUpdata();
    void closeMessage();
    void cancelBuyCoin();
    void showMessage(int messageID1, int messageID2, int messageID3);
};

struct MaterielMenu_COINSALEROOM_BUY : menu::MenuBase
{
    int coin_;                          /* 0x1C */
    int coinPrice_;                     /* 0x20 */
    int mode_;                          /* 0x24 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    bool messageUpdata();
    void buyCoinMessage();
    void yesMessage();
    void noMessage();
    void getCasinoCoin();
    void showMessage(int messageID);
};

extern MaterielMenu_COINSALEROOM_BUY gMaterielMenu_COINSALEROOM_BUY;
extern MaterielMenu_COINSALEROOM_ROOT gMaterielMenu_COINSALEROOM_ROOT;

