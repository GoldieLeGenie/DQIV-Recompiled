#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"

// town item menu: equip and throw messages of the item command menu
struct TownMenuItemMessage : menu::MenuBase
{
    enum TOWN_MENU_ITEM_MESSAGE_TYPE {
        MESS_NONE = 0,
        MESS_EQIP_NORMAL = 1,
        MESS_EQIP_NOROI = 2,
        MESS_THROW_OK = 3,
        MESS_THROW_NG = 4,
        MESS_THROW_DIFFICULT = 5,
        MESS_THROW_END = 6,
    };

    unsigned char messageMode_;             /* 0x1C */
    short cursedItem_;                      /* 0x1E */

    virtual void menuSetup();
    virtual void menuDraw() {}
    virtual void menuExecute() {}
    virtual void menuUpdate();
    void setMessageMenu(TOWN_MENU_ITEM_MESSAGE_TYPE type);
    void messageUpdate(int stat);
    void setItemEqipNormal();
    void setItemEqipNoroi();
    void setItemThrowAffi();
    void setItemThrowNG();
    void setItemThrowDifficult();
    void setItemThrow();
    void endMessageToCommandSelect();
    void endMessageToItemSelect();
    void endMessageToCharaSelect();
    void throwEndToReturnMenu();
};

extern TownMenuItemMessage gTownMenuItemMessage;
