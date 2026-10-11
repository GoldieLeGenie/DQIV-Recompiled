#pragma once
#ifdef DQIV_NATIVE
#include "main/menu/MenuBase.hpp"

// The PC version's settings in the game's menu style, pages turned by L / R:
// game, display, keyboard, gamepad. In a game, the game page also holds the
// game's own options (battle messages, music and sound volumes). The menu
// showing it owns the cursor's menu item.
struct SettingsPanel
{
    enum Row {
        ROW_BATTLE_SPEED = 100,
        ROW_MUSIC,
        ROW_SOUNDS,
        ROW_RESET,
        ROW_BACK,
    };
    int page_ = 0;
    bool inGame_ = false;
    int rows_[16] = {};
    int rowCount_ = 0;

    void open(bool inGame);
    void execute(menu::MenuItem& item);
    // True when the player leaves the settings.
    bool update(menu::MenuItem& item, int& redraw);
    // The page tabs and the window below them, across (x, w) of the screen;
    // `frame` draws a window (x, y, w, h, separator line).
    void draw(menu::MenuItem& item, int x, int w, void (*frame)(int, int, int, int, int));

private:
    void buildRows();
    int step() const;
    void change(int row, int step);
};
#endif
