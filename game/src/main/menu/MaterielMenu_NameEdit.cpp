#pragma ipa file
#include "main/menu/MaterielMenu_NameEdit.hpp"
#include "main/menu/MaterielMenuWindowManager.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/UiMsg.hpp"
#include "main/text/TextAPI.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Pad.hpp"
#include "main/profile/Profile.hpp"
#include "main/status/GameStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "ov016/MaterielMenu_LOAD/MaterielMenu_LOAD.hpp"
#include "ov026/MaterielMenu_SURECHIGAI/MaterielMenu_SURECHIGAI.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_02177bac.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"
#include "main/cmn/UnkEnvoyManager.hpp"

int data_020f17bc;
int data_020f17c0;
int data_020f17c4[45];
int data_020f1878[45];
int data_020f192c[45];
int data_020f19e0[66];
char data_020f1ae8[0x8c];
char data_020f1b74[0xb8];
char data_020f1c2c[0x14c];

THUMB void MaterielMenu_NameEdit::menuSetup()
{
    data_020f0078.mode_ = 1;
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    keyboard_.unkfunc_0216a980();
    language_ = 0;
    unk_88 = 0;
    unk_98 = 0;
    bCloseEditTownName_ = 0;
    setNameEditMode();
}

THUMB void MaterielMenu_NameEdit::setNameEditMode()
{
    language_ = status::g_Game.language;
    inputType_ = TYPE_NAME_EDIT;
    data_020f17bc = 8;
}

THUMB void MaterielMenu_NameEdit::setTownNameMode()
{
    inputType_ = TYPE_TOWN_NAME;
    data_020f17bc = 9;
}

THUMB void MaterielMenu_NameEdit::unkfunc_0203b004()
{
    inputType_ = TYPE_MESSAGE;
    data_020f17bc = 45;
}

THUMB void MaterielMenu_NameEdit::menuExecute()
{
    if (inputType_ == TYPE_MESSAGE) {
        MenuTemplate_materiel::MATERIEL_KEYBOARD_JAP_MESSAGE(&menuItem_);
    } else {
        MenuTemplate_materiel::MATERIEL_KEYBOARD_JAP(&menuItem_);
    }
}

THUMB void MaterielMenu_NameEdit::menuDraw()
{
    if (data_020ed1bc.isOpen()) {
        return;
    }
    for (int i = 0; i < 66; i++) {
        data_020f19e0[i] = 0;
    }
    unkfunc_0203b0f0(data_020f19e0);
    unkfunc_0203b750(data_020f1878);
    int message = unkfunc_0203b158();
    if (inputType_ == TYPE_MESSAGE) {
        unkfunc_02178180(data_020f19e0, 1);
        unkfunc_02177c00(0, 0x48, 0x100, 0x78, 0x60);
        unkfunc_0203b184(message);
    } else {
        unkfunc_02178180(data_020f19e0, 0);
        unkfunc_02177c00(0x38, 0x10, 0xa0, 0x20, -1);
        unkfunc_0203b1c8(message);
    }
    menuItem_.drawActive();
    switch (returnMenu_) {
    case RETURN_MENU_SURECHIGAI:
        unkfunc_0216feb8(-1, data_020f0078.unkfunc_0203a5ec());
        break;
    case RETURN_MENU_SURECHIGAI_MESSAGE:
        break;
    }
}

THUMB void MaterielMenu_NameEdit::unkfunc_0203b0f0(int* label)
{
    int pos = 0;
    for (int i = 0; i < 66; i++) {
        char* buffer = &data_020f1c2c[pos];
        *label = (int)buffer;
        if (!keyboard_.unkfunc_0216ac54(i) || keyboard_.unkfunc_0216ac08(i) == 0x20) {
            pos += unkfunc_0203b798(keyboard_.unkfunc_0216ac08(i), buffer);
            data_020f1c2c[pos] = 0;
            pos++;
        } else {
            *label = keyboard_.unkfunc_0216ac08(i);
        }
        label++;
    }
}

THUMB int MaterielMenu_NameEdit::unkfunc_0203b158()
{
    switch (inputType_) {
    case TYPE_MESSAGE:
        return 0xa00002d3;
    case TYPE_NAME_EDIT:
        return 0x800001a0;
    case TYPE_TOWN_NAME:
        return 0xa00002d5;
    }
    return 0;
}

THUMB void MaterielMenu_NameEdit::unkfunc_0203b184(int message)
{
    unkfunc_02178278(data_020f1878, data_020f17c0);
    unkfunc_02177c00(0, 0, 0x100, 0x48, -1);
    unkfunc_02177fe0(&message, 8, 0x4a, 0xf0, 0x58);
}

THUMB void MaterielMenu_NameEdit::unkfunc_0203b1c8(int message)
{
    int blink = 0;
    if (unk_94 == 15 && unk_94 == 30) {
        redraw_ = 1;
    }
    if (unk_94 > 30) {
        unk_94 = 0;
    }
    if (unk_94 >= 15) {
        blink = 1;
    }
    unk_94++;
    unkfunc_021781c4(language_, data_020f1878, data_020f17c0, data_020f17bc, blink, 0);
    unkfunc_02177c00(0, 0x30, 0x100, 0x78, 0x48);
    unkfunc_02177fe0(&message, 8, 0x4a, 0xf0, 0x2c);
}

THUMB void MaterielMenu_NameEdit::menuUpdate()
{
    if (data_020ed1bc.isOpen()) {
        if (data_020ed1bc.stat_ == MENUBASE_STAT_OK) {
            data_020ed1bc.close();
            if (bCloseEditTownName_ == 1) {
                MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
            }
        } else if (data_020ed1bc.stat_ == MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            if (bCloseEditTownName_ == 1) {
                MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
            }
        }
        return;
    }
    int active = menuItem_.active_;
    int result = menuItem_.execInput();
    if (result != menu::MenuItem::MENUITEM_RESULT_NONE) {
        if (result == menu::MenuItem::MENUITEM_RESULT_OK) {
            menuItem_.result_ = 0;
            menuItem_.lastresult_ = 0;
            int code = keyboard_.unkfunc_0216a9b4();
            int index = keyboard_.index_;
            if (code & 0x80000000) {
                unkfunc_0203b4e4();
            } else {
                unkfunc_0203b398(index, code);
            }
            redraw_ = 1;
        }
        if (result == menu::MenuItem::MENUITEM_RESULT_CANCEL) {
            menuItem_.result_ = 0;
            menuItem_.lastresult_ = 0;
            if (data_020f17c0 == 0) {
                unkfunc_0203b514(false);
            } else {
                unkfunc_0203b3d8();
            }
            redraw_ = 1;
        }
        if (result == menu::MenuItem::MENUITEM_RESULT_CHANGE) {
            int move = active - menuItem_.active_;
            if (move == -1) {
                result = menu::MenuItem::MENUITEM_RESULT_RIGHT;
            }
            if (move == 1) {
                result = menu::MenuItem::MENUITEM_RESULT_LEFT;
            }
            if (move < -1) {
                result = menu::MenuItem::MENUITEM_RESULT_DOWN;
            }
            if (move > 1) {
                result = menu::MenuItem::MENUITEM_RESULT_UP;
            }
        }
        if (result == menu::MenuItem::MENUITEM_RESULT_UP) {
            menuItem_.active_ = keyboard_.unkfunc_0216aa5c();
        }
        if (result == menu::MenuItem::MENUITEM_RESULT_DOWN) {
            menuItem_.active_ = keyboard_.unkfunc_0216aa6c();
        }
        if (result == menu::MenuItem::MENUITEM_RESULT_LEFT) {
            menuItem_.active_ = keyboard_.unkfunc_0216aa7c();
        }
        if (result == menu::MenuItem::MENUITEM_RESULT_RIGHT) {
            menuItem_.active_ = keyboard_.unkfunc_0216aa8c();
        }
    } else if (dss::g_Pad.edge() & 8) {
        if (data_020f17c0 > 0) {
            menuItem_.active_ = keyboard_.unkfunc_0216abbc(0x41);
        }
    }
}

THUMB void MaterielMenu_NameEdit::unkfunc_0203b398(int index, int code)
{
    int max = data_020f17bc;
    if (data_020f17c0 < max) {
        data_020f17c0++;
    }
    int length = data_020f17c0;
    data_020f17c4[length - 1] = code;
    data_020f192c[length - 1] = index;
    if (length == max) {
        menuItem_.active_ = keyboard_.unkfunc_0216abbc(0x41);
    }
}

THUMB void MaterielMenu_NameEdit::unkfunc_0203b3d8()
{
    if (data_020f17c0 > 0) {
        data_020f17c0--;
        data_020f17c4[data_020f17c0] = 0;
        data_020f192c[data_020f17c0] = 0;
    }
}

THUMB int MaterielMenu_NameEdit::unkfunc_0203b400()
{
    int space = 1;
    for (int i = 0; i < data_020f17c0; i++) {
        if (getNameUTF8()[i] != ' ') {
            space = 0;
        }
    }
    if (space) {
        clearName();
        menuItem_.active_ = 0;
        keyboard_.unkfunc_0216abbc(0);
        return 0;
    }
    int ng = 0;
    if (CheckBadWord(getNameUTF8())) {
        ng = 1;
    }
    if (returnMenu_ == RETURN_MENU_LOAD || returnMenu_ == RETURN_MENU_SURECHIGAI) {
        if (unkfunc_020549f0(getNameUTF8())) {
            ng = 1;
        }
    }
    if (ng) {
        data_020ed1bc.openMessageForMENU();
        switch (returnMenu_) {
        case RETURN_MENU_LOAD:
            data_020ed1bc.addMessage(0xcb5f3);
            break;
        case RETURN_MENU_SURECHIGAI:
            data_020ed1bc.addMessage(0x92a76);
            break;
        case RETURN_MENU_SURECHIGAI_MESSAGE:
            data_020ed1bc.addMessage(0x92a91);
            break;
        case RETURN_MENU_SURECHIGAI_TOWNNAME:
            data_020ed1bc.addMessage(0x92a51);
            break;
        }
        return 0;
    }
    if (data_020f17c0 > 0) {
        return 1;
    }
    return 0;
}

THUMB void MaterielMenu_NameEdit::unkfunc_0203b4e4()
{
    switch (keyboard_.key_->type_) {
    case 1:
        unkfunc_0203b3d8();
        break;
    case 3:
        if (unkfunc_0203b400()) {
            unkfunc_0203b514(true);
        }
        break;
    }
}

THUMB void MaterielMenu_NameEdit::unkfunc_0203b514(bool ok)
{
    if (returnMenu_ == RETURN_MENU_LOAD || returnMenu_ == RETURN_MENU_SURECHIGAI) {
        close();
    }
    if (ok) {
        switch (returnMenu_) {
        case RETURN_MENU_LOAD:
            gMaterielMenu_LOAD.open();
            gMaterielMenu_LOAD.changeStatus(MaterielMenu_LOAD::LOAD_SEXUALITY);
            break;
        case RETURN_MENU_SURECHIGAI:
            data_020f0078.unkfunc_0203a604(getNameUTF8());
            gUnkMaterielMenu_02189a80.open();
            if (unk_98 == 1) {
                gUnkMaterielMenu_02189a80.unk_24 = 1;
            }
            break;
        case RETURN_MENU_SURECHIGAI_MESSAGE:
            data_020f0078.unkfunc_0203a83c(getNameUTF8());
            data_020ed1bc.openMessageForMENU();
            if (MaterielMenu_WINDOW_MANAGER::getSingleton()->editMessageForScript_ == 1) {
                data_020ed1bc.addMessage(0x92a93);
            } else {
                ui_MsgSndSet(0x32);
                data_020ed1bc.addMessage(0x92a80);
            }
            data_020ed1bc.setYesNo();
            data_020ed1bc.setYesNoPosition(0xc0, 0x40);
            close();
            gUnkMaterielMenu_02189a80.open();
            gUnkMaterielMenu_02189a80.unk_1c = 6;
            if (unk_98 == 1) {
                gUnkMaterielMenu_02189a80.unk_24 = 1;
            }
            break;
        case RETURN_MENU_SURECHIGAI_TOWNNAME:
            data_020f0078.unkfunc_0203a7a8(getNameUTF8());
            bCloseEditTownName_ = 1;
            data_020ed1bc.openMessageForMENU();
            if (unkfunc_0203b7b8() == 1) {
                ui_MsgSndSet(0x32);
                data_020ed1bc.addMessage(0x92a54, 0x92a55);
            } else {
                ui_MsgSndSet(0x32);
                data_020ed1bc.addMessage(0x92a59);
            }
            break;
        }
    } else {
        switch (returnMenu_) {
        case RETURN_MENU_LOAD:
            gMaterielMenu_LOAD.open();
            gMaterielMenu_LOAD.changeStatus(MaterielMenu_LOAD::LOAD_MODESELECT);
            break;
        case RETURN_MENU_SURECHIGAI:
            gMaterielMenu_SURECHIGAI_SELECT_OBJECT.open();
            if (unk_98 == 1) {
                gMaterielMenu_SURECHIGAI_SELECT_OBJECT.changeTaishi_ = 1;
            }
            break;
        case RETURN_MENU_SURECHIGAI_MESSAGE:
            data_020ed1bc.openMessageForMENU();
            if (MaterielMenu_WINDOW_MANAGER::getSingleton()->editMessageForScript_ == 1) {
                data_020ed1bc.addMessage(0x92a8e);
            } else {
                ui_MsgSndSet(0x32);
                data_020ed1bc.addMessage(0x92a7b);
            }
            break;
        case RETURN_MENU_SURECHIGAI_TOWNNAME:
            break;
        }
    }
}

THUMB void MaterielMenu_NameEdit::clearName()
{
    dss::memset(data_020f17c4, (int)"@ ", sizeof(data_020f17c4));
    dss::memset(data_020f192c, 0, sizeof(data_020f192c));
    dss::memset(data_020f1878, 0, sizeof(data_020f1878));
    data_020f17c0 = 0;
}

THUMB char* MaterielMenu_NameEdit::getNameUTF8()
{
    int pos = 0;
    for (int i = 0; i < data_020f17c0; i++) {
        pos += unkfunc_0203b798(data_020f17c4[i], &data_020f1ae8[pos]);
    }
    data_020f1ae8[pos] = 0;
    unkfunc_02088078(data_020f1ae8);
    return data_020f1ae8;
}

THUMB void MaterielMenu_NameEdit::unkfunc_0203b750(int* label)
{
    int pos = 0;
    for (int i = 0; i < data_020f17c0; i++) {
        label[i] = (int)&data_020f1b74[pos];
        pos += unkfunc_0203b798(data_020f17c4[i], &data_020f1b74[pos]);
        data_020f1b74[pos] = 0;
        pos++;
    }
}

THUMB int MaterielMenu_NameEdit::unkfunc_0203b798(unsigned int code, char* buffer)
{
    int length = 0;
    for (int i = 3; i >= 0; i--) {
        unsigned char c = code >> (i * 8);
        if (c != 0) {
            buffer[length] = c;
            length++;
        }
    }
    return length;
}

THUMB int MaterielMenu_NameEdit::unkfunc_0203b7b8()
{
    char townName[0x2a];
    char landName[0x2a];
    char townSuffix[0x2a];
    char landSuffix[0x2a];
    char town[0x2a] = "\x83\x5e\x83\x45\x83\x93";
    char land[0x2a] = "\x83\x89\x83\x93\x83\x68";
    for (int i = 0; i < 0x2a; i++) {
        townName[i] = 0;
    }
    for (int i = 0; i < 0x2a; i++) {
        landName[i] = 0;
    }
    for (int i = 0; i < 0x2a; i++) {
        townSuffix[i] = 0;
    }
    for (int i = 0; i < 0x2a; i++) {
        landSuffix[i] = 0;
    }
    dss::strcpy_s(townName, 0x2a, status::g_Story.heroName);
    dss::strcpy_s(landName, 0x2a, status::g_Story.heroName);
    unkfunc_02087e08(townSuffix, 0x2a, town);
    unkfunc_02087e08(landSuffix, 0x2a, land);
    dss::strcat_s(townName, 0x2a, townSuffix);
    dss::strcat_s(landName, 0x2a, landSuffix);
    if (dss::strcmp((char*)data_020f0078.unkfunc_0203a820(), townName) == 0 || dss::strcmp((char*)data_020f0078.unkfunc_0203a820(), landName) == 0) {
        return 1;
    }
    return 0;
}
