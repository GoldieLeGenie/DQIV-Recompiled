#pragma ipa file
#include "main/menu/CommonMenu_YESNO.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/TownMenu_PARTY_TALK.hpp"
#include "main/menu/MenuAPI.hpp"
#include "main/menu/MenuUpdateAssist.hpp"
#include "main/sound/SoundManager.hpp"
#include "globaldefs.h"
#include "main/sound/Sound.hpp"
#include "main/menu/UiMsg.hpp"
#include "main/menu/UnkMenuDisplays.hpp"

static MENUITEM_DATA yesNoItemData[] = {
    {1, 2, 0xd0, 0x08, 0x28, 0x10},
    {1, 2, 0xd0, 0x20, 0x28, 0x10},
    {-1, -1, 0, 0, 0, 0},
};

THUMB void CommonMenu_YESNO::menuSetup()
{
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    menuItem_.active_ = 0;
    menuItem_.enableLoopEdge_ = 1;
    UnkMenuYesNoDisplay* const window = &data_020f530c.yesNo_;
    window->setEnable(1);
    window->setTexts(0xa0000064, 0xa0000065);
    window->setPosition(0xc0, 0xc0);
    ui_MsgSetYesNoCursor(0);
    Sound::sePlay(0x130);
    superCancel_ = 1;
    wait_ = 0;
}

THUMB void CommonMenu_YESNO::menuExecute()
{
    menuItem_.setMenuItem(yesNoItemData, 1, 2, 2);
}

THUMB void CommonMenu_YESNO::menuDraw()
{
    ui_MsgDrawYesNo();
    menuItem_.drawActive();
}

THUMB void CommonMenu_YESNO::menuUpdate()
{
    if (wait_ < 4) {
        wait_++;
        return;
    }
    menuItem_.execInput();
    switch (menuItem_.result_) {
        case 1:
            ui_MsgSetYesNoCursor(menuItem_.active_);
            break;
        case 2:
            if (menuItem_.active_ == 0) {
                stat_ = MENUBASE_STAT_OK;
            } else {
                stat_ = MENUBASE_STAT_CANCEL;
            }
            close();
            break;
        case 3:
            stat_ = MENUBASE_STAT_CANCEL;
            close();
            break;
        case 4:
            if (superCancel_ != 0) {
                if (MenuAPI::isTownMenuRoot()) {
                    gTownMenu_ROOT.stat_ = MENUBASE_STAT_CANCEL;
                    MenuAPI::clearMenuAll();
                }
                stat_ = MENUBASE_STAT_CANCEL;
                close();
            }
            break;
        case 5:
            menuItem_.active_ = 1;
            ui_MsgSetYesNoCursor(1);
            break;
        case 6:
            menuItem_.active_ = 0;
            ui_MsgSetYesNoCursor(0);
            break;
        case 7:
        case 8:
            break;
    }
}

THUMB void CommonMenu_YESNO::setYesNo(int cursor)
{
    menuItem_.active_ = cursor;
    ui_MsgSetYesNoCursor(cursor);
}

THUMB void CommonMenu_YESNO::setPosition(int x, int y)
{
    data_020f530c.yesNo_.setPosition(x, y + 0xc0);
}

THUMB void CommonMenu_YESNO::setSuperCancel(int flag)
{
    superCancel_ = flag;
}
