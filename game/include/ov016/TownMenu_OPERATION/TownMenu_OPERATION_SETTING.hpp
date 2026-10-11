#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#ifdef DQIV_NATIVE
#include "ov016/MenuSettings/SettingsPanel.hpp"
#endif

// settings page of the town operation menu (bgm volume, se volume, battle speed)
struct TownMenu_OPERATION_SETTING : menu::MenuBase
{
    unsigned char prevMode_;            /* 0x1C */
    unsigned char mode_;                /* 0x1D */
    menu::MenuItem bgmItem_;            /* 0x20 */
    menu::MenuItem seItem_;             /* 0x84 */
    menu::MenuItem speedItem_;          /* 0xE8 */
    menu::MenuItem unk_14c;             /* 0x14C */
    menu::MenuItem cancelItem_;         /* 0x1B0 */
#ifdef DQIV_NATIVE
    // The PC version shows all its settings here instead (bgmItem_ is the cursor).
    SettingsPanel settings_;
#endif

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void unkfunc_0217a8f8();
    void unkfunc_0217a924();
    void unkfunc_0217a994();
    void unkfunc_0217aa14();
    void unkfunc_0217aa8c();
    int unkfunc_0217aad4();
};

extern TownMenu_OPERATION_SETTING gTownMenu_OPERATION_SETTING;

