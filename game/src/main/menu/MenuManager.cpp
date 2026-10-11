#include "main/menu/MenuManager.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/menu/UnkMenuBg.hpp"
#include "main/menu/UnkMenuPartsDraw.hpp"

static int s_updateTime;
static MenuSubManager* s_currentMenu;
static int s_requestEnable;
static int s_unk24;
static MENUDISPLAY_MODE s_mode;
static MENUDISPLAY_MODE s_next;
static int s_change_count;
static int s_enable;
static int s_drawTimeRedraw;
static int s_drawTime;
static int s_redraw;
static int s_unk04;
static MenuSubManager s_normalMenu;
static MenuSubManager s_extraMenu;

THUMB void MenuManager::setup()
{
    s_normalMenu.setup();
    s_extraMenu.setup();
    s_currentMenu = &s_normalMenu;
    s_mode = MENUDISPLAY_OFF;
    s_next = MENUDISPLAY_OFF;
    s_change_count = 0;
    s_enable = 1;
    s_requestEnable = 1;
    s_redraw = 1;
}

THUMB void MenuManager::setMenuDisplay(int mode)
{
    if (mode == MENUDISPLAY_OFF) {
        unkfunc_02081728(4);
        data_020facb8.icon_.unkfunc_0204fea8(0);
        unkfunc_0208120c(&dss::g_DISPLAYPLUGIN_DOUBLE3D);
    }
    if (mode == MENUDISPLAY_NORMAL) {
        unkfunc_02081728(4);
        data_020facb8.icon_.unkfunc_0204fea8(0);
        unkfunc_0208120c(&dss::g_DISPLAYPLUGIN_DOUBLE3D);
    }
    if (mode == MENUDISPLAY_EXTRA) {
        unkfunc_02081728(4);
        unkfunc_0208120c(&dss::g_DISPLAYPLUGIN_CAPTURE);
        data_020facb8.icon_.unkfunc_0204fea8(1);
    }
    clearMenuAll();
    data_02116ce0.unkfunc_0207e810();
    s_currentMenu->m_update = 1;
}

THUMB void MenuManager::requestMenuMode(MENUDISPLAY_MODE mode)
{
    if (s_mode != mode) {
        s_next = mode;
        s_change_count = 0;
        setMenuDisplay(MENUDISPLAY_OFF);
    }
}

THUMB int MenuManager::isMenuMode(MENUDISPLAY_MODE mode)
{
    return s_mode == mode;
}

THUMB int MenuManager::isRequesting()
{
    return s_mode != s_next;
}

THUMB void MenuManager::execute()
{
    if (data_0210bc40.unkfunc_02058380() == 1) {
        s_redraw = 1;
        return;
    }
    int request = s_requestEnable;
    int enable = s_enable;
    if (enable != request) {
        s_enable = request;
        enable = request;
        s_redraw = 1;
        if (request == 0) {
            unkfunc_02050614(1);
            unkfunc_02050494();
            return;
        }
    }
    if (enable == 0) {
        return;
    }
    MENUDISPLAY_MODE mode = s_mode;
    MENUDISPLAY_MODE next = s_next;
    if (mode != next) {
        if (s_change_count == 2) {
            s_mode = next;
            s_change_count = 0;
            setMenuDisplay(next);
        }
        s_change_count++;
    }
    s_currentMenu->execute();
    if (s_currentMenu->m_update) {
        s_redraw = 1;
    }
    int start = unkfunc_0207e7e8();
    unkfunc_02050614(s_redraw);
    s_currentMenu->draw(0, 0xc0);
    s_currentMenu->m_update = 0;
    int drawTime = unkfunc_0207e7e8() - start;
    if (s_redraw) {
        unkfunc_02050494();
    }
    start = unkfunc_0207e7e8();
    s_currentMenu->update();
    int updateTime = unkfunc_0207e7e8() - start;
    if (s_redraw) {
        s_redraw = 0;
        s_drawTimeRedraw = drawTime;
    } else {
        s_drawTime = drawTime;
    }
    s_updateTime = updateTime;
}

THUMB void MenuManager::addMenu(menu::MenuBase* menu)
{
    s_currentMenu->addMenu(menu);
    menu->stat_ = menu::MenuBase::MENUBASE_STAT_ACTIVE;
}

THUMB void MenuManager::deleteMenu(menu::MenuBase* menu)
{
    s_currentMenu->deleteMenu(menu);
}

THUMB void MenuManager::clearMenuAll()
{
    s_currentMenu->clearMenuAll();
}

THUMB int MenuManager::isOpenMenu(menu::MenuBase* menu)
{
    return s_currentMenu->isOpenMenu(menu);
}

THUMB int MenuManager::getUpdateTime()
{
    return s_unk04;
}

THUMB void MenuManager::setMenuEnable(int enable)
{
    s_requestEnable = enable;
    s_redraw = 1;
}

THUMB int MenuManager::isExtraMenu()
{
    if (s_currentMenu == &s_extraMenu) {
        return 1;
    }
    return 0;
}
