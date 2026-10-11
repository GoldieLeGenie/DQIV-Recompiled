#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "main/menu/CatalogView.hpp"
#include "main/menu/MaterielMenu_NameEdit.hpp"
#ifdef DQIV_NATIVE
#include "ov016/MenuSettings/SettingsPanel.hpp"
#endif

struct MaterielMenu_LOAD : menu::MenuBase
{
    enum LOAD_STATUS {
        LOAD_MODESELECT = 0,
        LOAD_RESUME = 1,
        LOAD_DATACHECK = 2,
        LOAD_DATASELECT = 3,
        LOAD_ISOVERLOAD = 4,
        LOAD_NAMEEDIT = 5,
        LOAD_SEXUALITY = 6,
        LOAD_WRITECHECK = 7,
        LOAD_DELETECHECK = 8,
        LOAD_LOADCHECK = 9,
        LOAD_BLANK = 10,
        LOAD_END = 11,
#ifdef DQIV_NATIVE
        LOAD_SETTINGS = 12, // the PC version's settings, the last entry of the root menu
#endif
    };
    static const int ROOT_COUNT;
    static const int ROOT_RESUME_COUNT;
    static int activeDiaryNo_;

    menu::MenuItem rootItem_;           /* 0x1C */
    menu::MenuItem dataItem_;           /* 0x80 */
    menu::MenuItem sexualityItem_;      /* 0xE4 */
    CursorMoveGridLoop navigator_;      /* 0x148 */
    LOAD_STATUS status_;                /* 0x154 */
    int menuMode_;                      /* 0x158 */
    int messageCounter_;                /* 0x15C */
    int country_;                       /* 0x160 */
    int sexuality_;                     /* 0x164 */
    DiaryInfo diary_[3];                /* 0x168 */
    profile::CatalogView* catalogview_;          /* 0x1B0 */
    int killResult_;                    /* 0x1B4 */
    int makeResult_;                    /* 0x1B8 */
    int resume_;                        /* 0x1BC */
    int frame_;                         /* 0x1C0 */
#ifdef DQIV_NATIVE
    menu::MenuItem settingsItem_;
    SettingsPanel settings_;
    int rootCount() const;              // the original entries
#endif

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void changeStatus(LOAD_STATUS status);
    bool deleteDiary();
    bool makeDiary();
    bool updateActiveDiary();
    void closeMessage();
    void rootUpdate();
    void dataSelectUpdate();
    void sexualityUpdate();
    void sexualityDraw();
    bool messageUpdate();
};

extern MaterielMenu_LOAD gMaterielMenu_LOAD;

