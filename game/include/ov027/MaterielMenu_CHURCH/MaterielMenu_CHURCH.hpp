#pragma once
#include "globaldefs.h"
#include "GameInfo.hpp"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MaterielMenu_SlotEnter.hpp"
#include "main/menu/MaterielMenuPlayerControl.hpp"
#include "main/menu/MaterielMenu_SAVE.hpp"

struct MaterielMenu_CHURCH_MIRACLE : menu::MenuBase
{
    enum MIRACLE_STATUS {
        MIRACLE_FIRST = -2,
        MIRACLE_NONE = -1,
        MIRACLE_CHECK = 0,
        MIRACLE_CHECKOK = 1,
        MIRACLE_CHECKNG = 2,
        MIRACLE_NG = 3,
        MIRACLE_ISEND = 4,
        MIRACLE_SOUND = 5,
        MIRACLE_SOUNDEND = 6
    };

    menu::MenuItem menuItem_;           /* 0x1C */
    unsigned char unk_80[0x64];         /* 0x80 */
    CursorMoveGridLoop navigator_;     /* 0xE4 */
    int miracle_;                       /* 0xF0 */
    int activeChara_;                   /* 0xF4 */
    MIRACLE_STATUS miracleStatus_;      /* 0xF8 */
    int sexType_;                       /* 0xFC */
    int unk_100;                        /* 0x100 */
    int miracleAmount_[3];              /* 0x104 */
    int soundType_;                     /* 0x110 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    bool messageUpdate();
    bool listUpdate();
    void selectRevival();
    void selectRevivalEnd();            
    void selectAntidote();
    void selectAntiCurse();
    bool resistCurseEquip(ItemType equip);
    void selectGoldCheck();
    void selectCheckNG();
    void selectNG();
    bool isMiracle(int index, int miracle);
    void payOutMiracle();
    void openRootMenu();
};

struct MaterielMenu_CHURCH_ROOT : menu::MenuBase
{
    enum {
        MIRACLE_ORDER_REVIVAL = 0,
        MIRACLE_ORDER_ANTIDOTE = 1,
        MIRACLE_ORDER_ANTICURSE = 2
    };

    unsigned char unk_1c[0x64];         /* 0x1C */
    menu::MenuItem menuItem_;           /* 0x80 */
    menu::MenuItem menuItem2_;          /* 0xE4 */
    int activeCommand_;                 /* 0x148 */
    int expMessageCount_;               /* 0x14C */
    int timeType_;                      /* 0x150 */
    int sexType_;                       /* 0x154 */
    int churchType_;                    /* 0x158 */
    int commandNum_;                    /* 0x15C */
    int firstFlag_;                     /* 0x160 */
    int unk_164;                        /* 0x164 */
    CursorMoveGridLoop navigator_;     /* 0x168 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    bool rootUpdate();
    bool commandUpdate();
    void selectNextExp();
    void selectEnd();
    void firstMessage();
    void oneMessage(int mess);
};

extern MaterielMenu_CHURCH_MIRACLE gMaterielMenu_CHURCH_MIRACLE;
extern MaterielMenu_CHURCH_ROOT gMaterielMenu_CHURCH_ROOT;

