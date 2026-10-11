#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MaterielMenu_SlotEnter.hpp"
#include "main/menu/MaterielMenuPlayerControl.hpp"
#include "main/menu/MaterielMenu_SAVE.hpp"
#include "main/menu/MaterielMenu_NameEdit.hpp"
#include "main/profile/Profile.hpp"

// surechigai menus whose code (ov026) is not decompiled yet, named after their menuSetup address
struct UnkMaterielMenu_02189360 : menu::MenuBase
{
    int unk_1c;                         /* 0x1C */
    int unk_20;                         /* 0x20 */
    menu::MenuItem menuItem_;           /* 0x24 */
    CursorMoveGridLoop navigator_;     /* 0x88 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
};

struct UnkMaterielMenu_02189630 : menu::MenuBase
{
    unsigned char unk_1c;               /* 0x1C */
    menu::MenuItem menuItem_;           /* 0x20 */
    CursorMoveGridLoop navigator_;     /* 0x84 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
};

struct UnkMaterielMenu_02189a80 : menu::MenuBase
{
    int unk_1c;                         /* 0x1C */
    int unk_20;                         /* 0x20 */
    int unk_24;                         /* 0x24 */
    menu::MenuItem menuItem_;           /* 0x28 */
    CursorMoveGridLoop navigator_;     /* 0x8C */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void unkfunc_02189d70();
};

struct MaterielMenu_SURECHIGAI_MAKE_TAISHI : menu::MenuBase
{
    signed char mode_;                  /* 0x1C */
    int firstFlag_;                     /* 0x20 */
    int changeTaishi_;                  /* 0x24 */
    menu::MenuItem menuItem_;           /* 0x28 */
    CursorMoveGridLoop navigator_;     /* 0x8C */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
};

struct MaterielMenu_SURECHIGAI_ROOT : menu::MenuBase
{
    int mode_;                          /* 0x1C */
    int firstFlag_;                     /* 0x20 */
    menu::MenuItem menuItem_;           /* 0x24 */
    CursorMoveGridLoop navigator_;     /* 0x88 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void selectCommand();
};

extern UnkMaterielMenu_02189630 gUnkMaterielMenu_02189630;                /* surechigai list menu */
extern UnkMaterielMenu_02189360 gUnkMaterielMenu_02189360;                /* surechigai list menu */
extern MaterielMenu_SURECHIGAI_ROOT gMaterielMenu_SURECHIGAI_ROOT;
extern MaterielMenu_SURECHIGAI_MAKE_TAISHI gMaterielMenu_SURECHIGAI_SELECT_OBJECT;
extern UnkMaterielMenu_02189a80 gUnkMaterielMenu_02189a80;                /* surechigai menu opened by MAKE_TAISHI */

