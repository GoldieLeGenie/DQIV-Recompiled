#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MaterielMenu_SlotEnter.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"

struct MaterielMenu_SLOT : menu::MenuBase
{
    enum {
        SLOT_START = 0,
        SLOT_GAME = 1,
        SLOT_RESULT = 2,
        SLOT_RESULT_EFFECT = 3,
        SLOT_RETRY = 4,
        SLOT_END = 5
    };
    static const short FANFARE_NUM = 500;
    static const short GREAT_FANFARE_NUM = 1000;

    char unk_1c[0x68];
    menu::MenuItem menuItem_;           /* 0x84 */
    int status_;                        /* 0xE8 */
    int messageCount_;                  /* 0xEC */
    int betCoin_;                       /* 0xF0 */
    int haveCoin_;                      /* 0xF4 */
    int resultCoin_;                    /* 0xF8 */
    int slotType_;                      /* 0xFC */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void setSlotType(int type);
    bool messageUpdate();
    void statusUpdate();
    void inputUpdate();
    void resultEffectUpdate();
    void gameUpdate();
    void showMessage(int messageID);
    void closeMessage();
};

extern MaterielMenu_SLOT gMaterielMenu_SLOT;
