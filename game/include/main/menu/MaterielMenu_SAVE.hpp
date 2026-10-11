#pragma once
#include "main/menu/MenuBase.hpp"
#include "main/menu/CatalogView.hpp"

struct MaterielMenu_SAVE : menu::MenuBase
{
    enum SAVE_TYPE {
        TYPE_CHURCH = 0,
        TYPE_CHAPTER = 1,
        TYPE_LASTDUNGEON = 2,
        TYPE_CLEAR = 3,
        TYPE_SURECHIGAI = 4
    };
    enum SAVEMENU_MODE {
        MENU_IS_SAVE = 0,
        MENU_READING = 1,
        MENU_SELECT = 2,
        MENU_IS_OVERWRITE = 3,
        MENU_WRITING = 4,
        MENU_WRITINGEXEC = 5,
        MENU_WRITINGWAIT = 6,
        MENU_SOUNDWAIT = 7,
        MENU_FAILED = 8,
        MENU_SUCCESS = 9,
        MENU_IS_END = 10,
        MENU_GAME_END = 11,
        MENU_END = 12,
        MENU_BLANK = 13,
        MENU_SURECHIGAI_START = 14
    };

    DiaryInfo diary_[3];                /* 0x1C */
    SAVE_TYPE saveType_;                /* 0x64 */
    SAVEMENU_MODE status_;              /* 0x68 */
    short m_focusDiary;                 /* 0x6C */
    int isSave_;                        /* 0x70 */
    int mother_;                        /* 0x74 */
    int startSurechigai_;               /* 0x78 */
    int sexType_;                       /* 0x7C */
    menu::MenuItem menuItem_;           /* 0x80 */
    int messageCounter_;                /* 0xE4 */
    int saveResult_;                    /* 0xE8 */
    int waitFrame_;                     /* 0xEC */
    profile::CatalogView* catalogview_;          /* 0xF0 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void setMode(SAVEMENU_MODE mode);
    bool messageUpdate();
    void commandUpdate();
    bool getSaveData();
    void dataSelectUpdate();
    void firstCancelMessage();
    void setMessage(int messageID);
    void setWaitMessage(int messageID, bool sex);
    void setYesNoMessage(int messageID, bool yes);
    void setTalkMessage(int messageID);
    void setTalkMessage(int messageID1, int messageID2);
    void setTalkWaitMessage(int messageID);
    void setTalkYesNoMessage(int messageID, bool yes);
    void setTalkYesNoMessage2(int messageID1, int messageID2, bool yes);
};

extern MaterielMenu_SAVE gMaterielMenu_SAVE;

