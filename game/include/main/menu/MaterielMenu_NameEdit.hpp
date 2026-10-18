#pragma once
#include "main/menu/MenuBase.hpp"
#include "ov016/NameEditKeyboard/NameEditKeyboard.hpp"

struct MaterielMenu_NameEdit : menu::MenuBase
{
    enum RETURN_MENU_LIST {
        RETURN_MENU_LOAD = 0,
        RETURN_MENU_SURECHIGAI = 1,
        RETURN_MENU_SURECHIGAI_MESSAGE = 2,
        RETURN_MENU_SURECHIGAI_TOWNNAME = 3,
        RETURN_MENU_NULL = 4
    };
    enum INPUT_TYPE {
        TYPE_NAME_EDIT = 0,
        TYPE_TOWN_NAME = 1,
        TYPE_MESSAGE = 2,
        TYPE_NULL = 3
    };

    menu::MenuItem menuItem_;           /* 0x1C */
    int language_;                      /* 0x80 */
    INPUT_TYPE inputType_;              /* 0x84 */
    int unk_88;                         /* 0x88 */
    int unk_8c;                         /* 0x8C */
    RETURN_MENU_LIST returnMenu_;       /* 0x90 */
    int blinkCount_;                         /* 0x94 */
    int changeTaishi_;                         /* 0x98 */
    int bCloseEditTownName_;            /* 0x9C */
    NameEditKeyboard keyboard_;      /* 0xA0 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void setNameEditMode();
    void setTownNameMode();
    void setMessageMode();
    void buildKeyLabels(int* label);
    int getPromptMessage();
    void drawMessageInput(int message);
    void drawNameInput(int message);
    void appendChar(int index, int code);
    void deleteLastChar();
    int validateName();
    void onSpecialKey();
    void finishNameEdit(bool ok);
    void clearName();
    char* getNameUTF8();
    void buildNameLabels(int* label);
    int unpackCharCode(unsigned int code, char* buffer);
    int isDefaultTownName();
};

extern MaterielMenu_NameEdit gMaterielMenu_NameEdit;

extern int g_nameEditMaxLength;                                    /* name edit: max length */
extern int g_nameEditLength;                                    /* name edit: current length */
extern int g_nameEditLabels[];                                  /* name edit: characters */
