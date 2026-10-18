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
#include "ov016/UnkMaterielMenuDraw/MaterielMenuDrawExtra.hpp"
#include "ov016/UnkMaterielMenuDraw/MaterielMenuScreens.hpp"
#include "main/cmn/EnvoyManager.hpp"

int g_nameEditMaxLength;
int g_nameEditLength;
int s_nameEditCharCodes[45];
int g_nameEditLabels[45];
int data_020f192c[45];
int s_nameEditKeyLabels[66];
char s_nameEditUtf8Buffer[0x8c];
char s_nameEditLabelBuffer[0xb8];
char s_nameEditKeyLabelBuffer[0x14c];

THUMB void MaterielMenu_NameEdit::menuSetup()
{
    g_EnvoyManager.mode_ = 1;
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    keyboard_.init();
    language_ = 0;
    unk_88 = 0;
    changeTaishi_ = 0;
    bCloseEditTownName_ = 0;
    setNameEditMode();
}

THUMB void MaterielMenu_NameEdit::setNameEditMode()
{
    language_ = status::g_Game.language;
    inputType_ = TYPE_NAME_EDIT;
    g_nameEditMaxLength = 8;
}

THUMB void MaterielMenu_NameEdit::setTownNameMode()
{
    inputType_ = TYPE_TOWN_NAME;
    g_nameEditMaxLength = 9;
}

THUMB void MaterielMenu_NameEdit::setMessageMode()
{
    inputType_ = TYPE_MESSAGE;
    g_nameEditMaxLength = 45;
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
    if (g_TownMenuMessage.isOpen()) {
        return;
    }
    for (int i = 0; i < 66; i++) {
        s_nameEditKeyLabels[i] = 0;
    }
    buildKeyLabels(s_nameEditKeyLabels);
    buildNameLabels(g_nameEditLabels);
    int message = getPromptMessage();
    if (inputType_ == TYPE_MESSAGE) {
        drawNameEditCharTable(s_nameEditKeyLabels, 1);
        drawFrameStyleB(0, 0x48, 0x100, 0x78, 0x60);
        drawMessageInput(message);
    } else {
        drawNameEditCharTable(s_nameEditKeyLabels, 0);
        drawFrameStyleB(0x38, 0x10, 0xa0, 0x20, -1);
        drawNameInput(message);
    }
    menuItem_.drawActive();
    switch (returnMenu_) {
    case RETURN_MENU_SURECHIGAI:
        drawNameEditScreen(-1, g_EnvoyManager.getType());
        break;
    case RETURN_MENU_SURECHIGAI_MESSAGE:
        break;
    }
}

THUMB void MaterielMenu_NameEdit::buildKeyLabels(int* label)
{
    int pos = 0;
    for (int i = 0; i < 66; i++) {
        char* buffer = &s_nameEditKeyLabelBuffer[pos];
        *label = (int)buffer;
        if (!keyboard_.getKeyType(i) || keyboard_.getKeyChar(i) == 0x20) {
            pos += unpackCharCode(keyboard_.getKeyChar(i), buffer);
            s_nameEditKeyLabelBuffer[pos] = 0;
            pos++;
        } else {
            *label = keyboard_.getKeyChar(i);
        }
        label++;
    }
}

THUMB int MaterielMenu_NameEdit::getPromptMessage()
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

THUMB void MaterielMenu_NameEdit::drawMessageInput(int message)
{
    drawNameEditCommentField(g_nameEditLabels, g_nameEditLength);
    drawFrameStyleB(0, 0, 0x100, 0x48, -1);
    drawCenteredMessage(&message, 8, 0x4a, 0xf0, 0x58);
}

THUMB void MaterielMenu_NameEdit::drawNameInput(int message)
{
    int blink = 0;
    if (blinkCount_ == 15 && blinkCount_ == 30) {
        redraw_ = 1;
    }
    if (blinkCount_ > 30) {
        blinkCount_ = 0;
    }
    if (blinkCount_ >= 15) {
        blink = 1;
    }
    blinkCount_++;
    drawNameEditField(language_, g_nameEditLabels, g_nameEditLength, g_nameEditMaxLength, blink, 0);
    drawFrameStyleB(0, 0x30, 0x100, 0x78, 0x48);
    drawCenteredMessage(&message, 8, 0x4a, 0xf0, 0x2c);
}

THUMB void MaterielMenu_NameEdit::menuUpdate()
{
    if (g_TownMenuMessage.isOpen()) {
        if (g_TownMenuMessage.stat_ == MENUBASE_STAT_OK) {
            g_TownMenuMessage.close();
            if (bCloseEditTownName_ == 1) {
                MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
            }
        } else if (g_TownMenuMessage.stat_ == MENUBASE_STAT_CANCEL) {
            g_TownMenuMessage.close();
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
            int code = keyboard_.pressKey();
            int index = keyboard_.index_;
            if (code & 0x80000000) {
                onSpecialKey();
            } else {
                appendChar(index, code);
            }
            redraw_ = 1;
        }
        if (result == menu::MenuItem::MENUITEM_RESULT_CANCEL) {
            menuItem_.result_ = 0;
            menuItem_.lastresult_ = 0;
            if (g_nameEditLength == 0) {
                finishNameEdit(false);
            } else {
                deleteLastChar();
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
            menuItem_.active_ = keyboard_.moveUp();
        }
        if (result == menu::MenuItem::MENUITEM_RESULT_DOWN) {
            menuItem_.active_ = keyboard_.moveDown();
        }
        if (result == menu::MenuItem::MENUITEM_RESULT_LEFT) {
            menuItem_.active_ = keyboard_.moveLeft();
        }
        if (result == menu::MenuItem::MENUITEM_RESULT_RIGHT) {
            menuItem_.active_ = keyboard_.moveRight();
        }
    } else if (dss::g_Pad.edge() & 8) {
        if (g_nameEditLength > 0) {
            menuItem_.active_ = keyboard_.setCursorToKey(0x41);
        }
    }
}

THUMB void MaterielMenu_NameEdit::appendChar(int index, int code)
{
    int max = g_nameEditMaxLength;
    if (g_nameEditLength < max) {
        g_nameEditLength++;
    }
    int length = g_nameEditLength;
    s_nameEditCharCodes[length - 1] = code;
    data_020f192c[length - 1] = index;
    if (length == max) {
        menuItem_.active_ = keyboard_.setCursorToKey(0x41);
    }
}

THUMB void MaterielMenu_NameEdit::deleteLastChar()
{
    if (g_nameEditLength > 0) {
        g_nameEditLength--;
        s_nameEditCharCodes[g_nameEditLength] = 0;
        data_020f192c[g_nameEditLength] = 0;
    }
}

THUMB int MaterielMenu_NameEdit::validateName()
{
    int space = 1;
    for (int i = 0; i < g_nameEditLength; i++) {
        if (getNameUTF8()[i] != ' ') {
            space = 0;
        }
    }
    if (space) {
        clearName();
        menuItem_.active_ = 0;
        keyboard_.setCursorToKey(0);
        return 0;
    }
    int ng = 0;
    if (CheckBadWord(getNameUTF8())) {
        ng = 1;
    }
    if (returnMenu_ == RETURN_MENU_LOAD || returnMenu_ == RETURN_MENU_SURECHIGAI) {
        if (isNameTooWide(getNameUTF8())) {
            ng = 1;
        }
    }
    if (ng) {
        g_TownMenuMessage.openMessageForMENU();
        switch (returnMenu_) {
        case RETURN_MENU_LOAD:
            g_TownMenuMessage.addMessage(0xcb5f3);
            break;
        case RETURN_MENU_SURECHIGAI:
            g_TownMenuMessage.addMessage(0x92a76);
            break;
        case RETURN_MENU_SURECHIGAI_MESSAGE:
            g_TownMenuMessage.addMessage(0x92a91);
            break;
        case RETURN_MENU_SURECHIGAI_TOWNNAME:
            g_TownMenuMessage.addMessage(0x92a51);
            break;
        }
        return 0;
    }
    if (g_nameEditLength > 0) {
        return 1;
    }
    return 0;
}

THUMB void MaterielMenu_NameEdit::onSpecialKey()
{
    switch (keyboard_.key_->type_) {
    case 1:
        deleteLastChar();
        break;
    case 3:
        if (validateName()) {
            finishNameEdit(true);
        }
        break;
    }
}

THUMB void MaterielMenu_NameEdit::finishNameEdit(bool ok)
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
            g_EnvoyManager.setName(getNameUTF8());
            gUnkMaterielMenu_02189a80.open();
            if (changeTaishi_ == 1) {
                gUnkMaterielMenu_02189a80.changeTaishi_ = 1;
            }
            break;
        case RETURN_MENU_SURECHIGAI_MESSAGE:
            g_EnvoyManager.setComment(getNameUTF8());
            g_TownMenuMessage.openMessageForMENU();
            if (MaterielMenu_WINDOW_MANAGER::getSingleton()->editMessageForScript_ == 1) {
                g_TownMenuMessage.addMessage(0x92a93);
            } else {
                ui_MsgSndSet(0x32);
                g_TownMenuMessage.addMessage(0x92a80);
            }
            g_TownMenuMessage.setYesNo();
            g_TownMenuMessage.setYesNoPosition(0xc0, 0x40);
            close();
            gUnkMaterielMenu_02189a80.open();
            gUnkMaterielMenu_02189a80.step_ = 6;
            if (changeTaishi_ == 1) {
                gUnkMaterielMenu_02189a80.changeTaishi_ = 1;
            }
            break;
        case RETURN_MENU_SURECHIGAI_TOWNNAME:
            g_EnvoyManager.setTownName(getNameUTF8());
            bCloseEditTownName_ = 1;
            g_TownMenuMessage.openMessageForMENU();
            if (isDefaultTownName() == 1) {
                ui_MsgSndSet(0x32);
                g_TownMenuMessage.addMessage(0x92a54, 0x92a55);
            } else {
                ui_MsgSndSet(0x32);
                g_TownMenuMessage.addMessage(0x92a59);
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
            if (changeTaishi_ == 1) {
                gMaterielMenu_SURECHIGAI_SELECT_OBJECT.changeTaishi_ = 1;
            }
            break;
        case RETURN_MENU_SURECHIGAI_MESSAGE:
            g_TownMenuMessage.openMessageForMENU();
            if (MaterielMenu_WINDOW_MANAGER::getSingleton()->editMessageForScript_ == 1) {
                g_TownMenuMessage.addMessage(0x92a8e);
            } else {
                ui_MsgSndSet(0x32);
                g_TownMenuMessage.addMessage(0x92a7b);
            }
            break;
        case RETURN_MENU_SURECHIGAI_TOWNNAME:
            break;
        }
    }
}

THUMB void MaterielMenu_NameEdit::clearName()
{
    dss::memset(s_nameEditCharCodes, (int)"@ ", sizeof(s_nameEditCharCodes));
    dss::memset(data_020f192c, 0, sizeof(data_020f192c));
    dss::memset(g_nameEditLabels, 0, sizeof(g_nameEditLabels));
    g_nameEditLength = 0;
}

THUMB char* MaterielMenu_NameEdit::getNameUTF8()
{
    int pos = 0;
    for (int i = 0; i < g_nameEditLength; i++) {
        pos += unpackCharCode(s_nameEditCharCodes[i], &s_nameEditUtf8Buffer[pos]);
    }
    s_nameEditUtf8Buffer[pos] = 0;
    convertUtf8ToSjisWork(s_nameEditUtf8Buffer);
    return s_nameEditUtf8Buffer;
}

THUMB void MaterielMenu_NameEdit::buildNameLabels(int* label)
{
    int pos = 0;
    for (int i = 0; i < g_nameEditLength; i++) {
        label[i] = (int)&s_nameEditLabelBuffer[pos];
        pos += unpackCharCode(s_nameEditCharCodes[i], &s_nameEditLabelBuffer[pos]);
        s_nameEditLabelBuffer[pos] = 0;
        pos++;
    }
}

THUMB int MaterielMenu_NameEdit::unpackCharCode(unsigned int code, char* buffer)
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

THUMB int MaterielMenu_NameEdit::isDefaultTownName()
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
    convertSjisToUtf8(townSuffix, 0x2a, town);
    convertSjisToUtf8(landSuffix, 0x2a, land);
    dss::strcat_s(townName, 0x2a, townSuffix);
    dss::strcat_s(landName, 0x2a, landSuffix);
    if (dss::strcmp((char*)g_EnvoyManager.getTownName(), townName) == 0 || dss::strcmp((char*)g_EnvoyManager.getTownName(), landName) == 0) {
        return 1;
    }
    return 0;
}
