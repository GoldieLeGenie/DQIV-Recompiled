#pragma once
#ifdef DQIV_NATIVE
#include "main/menu/MenuBase.hpp"

// The PC version's achievements in the game's menu style, pages turned by
// L / R (or left / right): those unlocked, those still missing. The list
// scrolls; the window below it describes the chosen one. The menu showing it
// owns the cursor's menu item.
struct AchievementsPanel
{
    enum Page {
        PAGE_UNLOCKED,
        PAGE_MISSING,
        PAGE_COUNT,
    };
    enum { VISIBLE_ROWS = 7, ROW_STEP = 14 };
    int page_ = 0;
    int first_ = 0;          // the first entry shown
    int entries_[128] = {};  // achievement indices on the page
    int entryCount_ = 0;

    void open();
    void execute(menu::MenuItem& item);
    // True when the player leaves the achievements.
    bool update(menu::MenuItem& item, int& redraw);
    // The page tabs and the window below them, across (x, w) of the screen;
    // `frame` draws a window (x, y, w, h, separator line).
    void draw(menu::MenuItem& item, int x, int w, void (*frame)(int, int, int, int, int));

private:
    void buildEntries();
    int rows() const;
};
#endif
