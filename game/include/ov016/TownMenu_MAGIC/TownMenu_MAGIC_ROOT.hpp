#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/CursorMoveGridLoop.hpp"
#include "ov001/fld/FieldSymbolManager.hpp"

// town magic menu: caster and spell selection
struct TownMenu_MAGIC_ROOT : menu::MenuBase
{
    unsigned char mode_;                    /* 0x1C */
    unsigned char activeChara_;             /* 0x1D */
    unsigned char activeMagic_;             /* 0x1E */
    unsigned char magicNumMax_;             /* 0x1F */
    short magicArray_[16];                  /* 0x20 */
    int isResult_;                          /* 0x40 */
    int riremitoOK_;                        /* 0x44 */
    int useBehomara_;                       /* 0x48 */
    unsigned char soundCount_;              /* 0x4C */
    unsigned char targetCount_;             /* 0x4D */
    menu::MenuItem charaItem_;              /* 0x50 */
    menu::MenuItem magicItem_;              /* 0xB4 */
    menu::MenuItem cancelItem_;             /* 0x118 */
    CursorMoveGridLoop charaNavigator_;     /* 0x17C */
    CursorMoveGridLoop magicNavigator_;     /* 0x188 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void unkfunc_021797a8();
    void getUseAction();
    void judgeMagic();
    void useMagicNoTarget();
    void unkfunc_02179b0c();
    int unkfunc_02179ba8();
};

extern TownMenu_MAGIC_ROOT gTownMenu_MAGIC_ROOT;
