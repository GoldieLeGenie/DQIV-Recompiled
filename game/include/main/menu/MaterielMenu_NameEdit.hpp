#pragma once
#include "main/menu/MenuBase.hpp"
#include "ov016/UnkNameEditKeyboard/UnkNameEditKeyboard_0216a980.hpp"

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
    int unk_94;                         /* 0x94 */
    int unk_98;                         /* 0x98 */
    int bCloseEditTownName_;            /* 0x9C */
    UnkNameEditKeyboard keyboard_;      /* 0xA0 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void setNameEditMode();
    void setTownNameMode();
    void unkfunc_0203b004();
    void unkfunc_0203b0f0(int* label);
    int unkfunc_0203b158();
    void unkfunc_0203b184(int message);
    void unkfunc_0203b1c8(int message);
    void unkfunc_0203b398(int index, int code);
    void unkfunc_0203b3d8();
    int unkfunc_0203b400();
    void unkfunc_0203b4e4();
    void unkfunc_0203b514(bool ok);
    void clearName();
    char* getNameUTF8();
    void unkfunc_0203b750(int* label);
    int unkfunc_0203b798(unsigned int code, char* buffer);
    int unkfunc_0203b7b8();
};

extern MaterielMenu_NameEdit gMaterielMenu_NameEdit;

extern int data_020f17bc;                                    /* name edit: max length */
extern int data_020f17c0;                                    /* name edit: current length */
extern int data_020f1878[];                                  /* name edit: characters */
