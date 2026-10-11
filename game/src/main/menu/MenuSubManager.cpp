#include "main/menu/MenuManager.hpp"
#include "main/menu/UnkMenuPartsDraw.hpp"

THUMB void MenuSubManager::setup()
{
    for (int i = 0; i < 8; i++) {
        m_menu[i] = 0;
        m_next[i] = 0;
    }
    m_update = 1;
}

THUMB void MenuSubManager::execute()
{
    for (int i = 0; i < 8; i++) {
        menu::MenuBase* menu = m_menu[i];
        if (menu) {
            menu->menuBaseExecute();
            if (menu->redraw_) {
                m_update = 1;
            }
        }
    }
}

THUMB void MenuSubManager::draw(int x, int y)
{
    for (int i = 0; i < 8; i++) {
        menu::MenuBase* menu = m_menu[i];
        if (menu) {
            unkfunc_02050698(x, y);
            menu->menuBaseDraw();
        }
    }
}

THUMB void MenuSubManager::update()
{
    for (int i = 0; i < 8; i++) {
        if (m_menu[i]) {
            m_menu[i]->menuBaseUpdate();
        }
    }
    for (int i = 0; i < 8; i++) {
        if (m_menu[i] != m_next[i]) {
            m_update = 1;
        }
        m_menu[i] = m_next[i];
    }
}

THUMB void MenuSubManager::addMenu(menu::MenuBase* menu)
{
    if (getMenuIndex(menu) == -1) {
        int index = getMenuIndex(0);
        if (index != -1) {
            m_next[index] = menu;
            menu->menuBaseSetup();
        }
    }
}

THUMB void MenuSubManager::deleteMenu(menu::MenuBase* menu)
{
    int index = getMenuIndex(menu);
    if (index != -1) {
        m_next[index]->menuClose();
        m_next[index] = 0;
    }
}

THUMB void MenuSubManager::clearMenuAll()
{
    for (int i = 0; i < 8; i++) {
        if (m_next[i]) {
            m_next[i]->menuClose();
            m_next[i] = 0;
        }
    }
    m_update = 1;
}

THUMB int MenuSubManager::isOpenMenu(menu::MenuBase* menu)
{
    if (getMenuIndex(menu) != -1) {
        return 1;
    }
    return 0;
}

THUMB int MenuSubManager::getMenuIndex(menu::MenuBase* menu)
{
    for (int i = 0; i < 8; i++) {
        if (m_next[i] == menu) {
            return i;
        }
    }
    return -1;
}
