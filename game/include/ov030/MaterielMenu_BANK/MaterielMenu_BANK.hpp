#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MaterielMenu_SlotEnter.hpp"

struct MaterielMenu_BANK_PUTIN : menu::MenuBase
{
    menu::MenuItem menuItem_;           /* 0x1C */
    CursorMoveGridLoop navigator_;     /* 0x80 */
    int oldActive_;                     /* 0x8C */
    int unk_90;                         /* 0x90 */
    int putinMoney_;                    /* 0x94 */
    int first_;                         /* 0x98 */
    int end_;                           /* 0x9C */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    bool messageUpdate();
    void bankUpdate();
    void bankPutin();
    void cancelPutin();
};

struct MaterielMenu_BANK_ROOT : menu::MenuBase
{
    menu::MenuItem menuItem_;           /* 0x1C */
    CursorMoveGridLoop navigator_;     /* 0x80 */
    int oldActive_;                     /* 0x8C */
    int playerID_;                      /* 0x90 */
    int first_;                         /* 0x94 */
    int end_;                           /* 0x98 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void closeBank();
};

struct MaterielMenu_BANK_DRAW : menu::MenuBase
{
    menu::MenuItem menuItem_;           /* 0x1C */
    CursorMoveGridLoop navigator_;     /* 0x80 */
    int oldActive_;                     /* 0x8C */
    int unk_90;                         /* 0x90 */
    int drawMoney_;                     /* 0x94 */
    int alivePlayer_;                   /* 0x98 */
    int first_;                         /* 0x9C */
    int end_;                           /* 0xA0 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    bool messageUpdate();
    void bankUpdate();
    void bankDraw();
    void cancelDrawfrom();
};

extern MaterielMenu_BANK_PUTIN gMaterielMenu_BANK_PUTIN;
extern MaterielMenu_BANK_DRAW gMaterielMenu_BANK_DRAW;


extern MaterielMenu_BANK_ROOT gMaterielMenu_BANK_ROOT;
