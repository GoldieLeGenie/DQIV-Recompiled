#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MenuManager.hpp"

THUMB menu::MenuBase::MenuBase()
{
    menuBaseSetup();
}

THUMB void menu::MenuBase::menuBaseSetup()
{
    frame_ = 0;
    redraw_ = 1;
    stat_ = MENUBASE_STAT_ACTIVE;
    exitCode_ = -1;
    menuSetup();
    lock_ = 0;
    lockRequest_ = 0;
}

ARM void menu::MenuBase::menuSetup()
{
}

THUMB void menu::MenuBase::menuBaseExecute()
{
    if (lock_ != lockRequest_) {
        lock_ = lockRequest_;
    }
    menuExecute();
}

ARM void menu::MenuBase::menuExecute()
{
}

THUMB void menu::MenuBase::menuBaseDraw()
{
    menuDraw();
    redraw_ = 0;
}

ARM void menu::MenuBase::menuDraw()
{
}

THUMB void menu::MenuBase::menuBaseUpdate()
{
    if (lock_ == 0) {
        menuUpdate();
        frame_++;
    }
}

ARM void menu::MenuBase::menuUpdate()
{
}

THUMB void menu::MenuBase::open()
{
    MenuManager::addMenu(this);
}

THUMB void menu::MenuBase::close()
{
    MenuManager::deleteMenu(this);
    menuClose();
}

THUMB int menu::MenuBase::isOpen()
{
    return MenuManager::isOpenMenu(this);
}
