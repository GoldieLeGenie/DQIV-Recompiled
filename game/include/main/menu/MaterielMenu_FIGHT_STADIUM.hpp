#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "main/menu/MaterielMenuWindowManager.hpp"

struct MaterielMenu_FIGHT_STADIUM : menu::MenuBase
{
    enum {
        FIGHT_STADIUM_START = 0,
        FIGHT_STADIUM_CHOICE = 1,
        FIGHT_STADIUM_BET = 2,
        FIGHT_STADIUM_BATTLE = 3,
        FIGHT_STADIUM_RESULT = 4,
        FIGHT_STADIUM_RETRY = 5,
        FIGHT_STADIUM_END = 6
    };
    static const short RESULT_NONE = 0;
    static const short RESULT_WIN = 1;
    static const short RESULT_LOSE = 2;
    static const short RESULT_DRAW = 3;
    static const short RESULT_RETIRE = 4;
    static const int BET_FIGURE_MAX = 2;
    static const int BET_CURSOR_Y;
    static const int BET_COIN_MAX;
    static const int DOUBLEUP_COIN_MAX;
    static const int BET_CURSOR_X;

    menu::MenuItem betItem_;            /* 0x1C */
    menu::MenuItem monsterItem_;        /* 0x80 */
    int status_;                        /* 0xE4 */
    int messageCount_;                  /* 0xE8 */
    int haveCoin_;                      /* 0xEC */
    int result_;                        /* 0xF0 */
    unsigned char unk_f4;               /* 0xF4 */
    unsigned char blinkCount_;          /* 0xF5 */
    int blink_;                         /* 0xF8 */
    int waitProg_;                      /* 0xFC */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void playBackMenu(int result);
    bool messageUpdate();
    void statusUpdate();
    void coinUpdate();
    void monsterListUpdate();
    void resultUpdate();
    void battleStart();
    void showMessage(int messageID);
    void closeMessage();
};

extern MaterielMenu_FIGHT_STADIUM gMaterielMenu_FIGHT_STADIUM;

