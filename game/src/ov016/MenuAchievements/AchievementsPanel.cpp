#ifdef DQIV_NATIVE
#include "ov016/MenuAchievements/AchievementsPanel.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/MaterielMenuDraw.hpp"
#include "main/menu/MenuPartsDraw.hpp"
#include "main/dss/BgText.hpp"
#include "main/dss/Pad.hpp"
#include "Achievements.hpp"
#include "HdPictures.hpp"
#include "Settings.hpp"
#include <nitro/pad.h>
#include <stdio.h>
#include <string.h>

namespace {
// Texts in the game's font: the menu API takes pointers to strings in the
// program's memory.
// The list above the window's line (each name after its badge), the chosen
// one's badge and description below.
enum {
    DESCRIPTION_LINES = 3,
    TEXT_LEFT = 0x18,
    TEXT_WIDTH = 0xd0,
    ICON_SIZE = 12,
    TITLE_LEFT = TEXT_LEFT + ICON_SIZE + 4,
    TITLE_WIDTH = 0xe8 - TITLE_LEFT,
    LIST_TOP = 0x18,
    LINE_Y = 0x8c,
    BADGE_LEFT = 0x14,
    BADGE_SIZE = 40,
    DETAIL_LEFT = BADGE_LEFT + BADGE_SIZE + 8,
    DETAIL_WIDTH = 0xe8 - DETAIL_LEFT,
};
char s_title[AchievementsPanel::VISIBLE_ROWS][96];
char s_lines[DESCRIPTION_LINES][128];
char s_pages[AchievementsPanel::PAGE_COUNT][48];
char s_empty[48];
MenuPartsEntry s_left[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0, 0, TEXT_WIDTH, 0xe },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
MenuPartsEntry s_detail[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0, 0, DETAIL_WIDTH, 0xe },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
void text(MenuPartsEntry* parts, char* line, int x, int y)
{
    int param = (int)line;
    parts[0].x_ = x;
    parts[0].y_ = y;
    drawMenuParts(parts, &param, 0, 0, 1);
}
// An achievement's badge at high resolution over (x, y, size, size) of the
// menu, whose screen buffer holds both DS screens (the top one first).
void badge(int index, bool locked, int x, int y, int size)
{
    std::shared_ptr<const std::vector<std::uint32_t>> pixels = pc::achievementBadge(index, locked);
    if (pixels) {
        const int screenY = g_menuPartsOriginY + y;
        pc::placeHdPicture(screenY >= 192 ? 1 : 0, g_menuPartsOriginX + x, screenY % 192, size, size,
                           ((std::uint64_t)index << 1) | (locked ? 1 : 0), pixels);
    }
}
// Width of a text in the menus' small font.
int textWidth(const char* text)
{
    setBgTextFontSize(10);
    drawBgTextWrapped(NULL, 0, 0, 0, text, 0);
    return getBgTextWidth();
}
// Length of the next UTF-8 character.
int charLength(const char* text)
{
    const unsigned char c = (unsigned char)*text;
    return c < 0x80 ? 1 : c < 0xe0 ? 2 : c < 0xf0 ? 3 : 4;
}
// The text cut to `width` pixels, ending with "..." when it was cut.
void fit(char* destination, int size, const char* source, int width)
{
    snprintf(destination, size, "%s", source);
    if (textWidth(destination) <= width) {
        return;
    }
    int length = 0;
    char trial[128];
    for (int next = 0; source[next] != 0 && next < size - 4; next += charLength(source + next)) {
        snprintf(trial, sizeof(trial), "%.*s...", next, source);
        if (textWidth(trial) > width) {
            break;
        }
        length = next;
    }
    snprintf(destination, size, "%.*s...", length, source);
}
// The text split in lines of `width` pixels at its spaces; the last one is cut.
void wrap(const char* source, int width)
{
    for (int line = 0; line < DESCRIPTION_LINES; line++) {
        s_lines[line][0] = 0;
    }
    char trial[128];
    for (int line = 0; line < DESCRIPTION_LINES && *source; line++) {
        if (line == DESCRIPTION_LINES - 1) {
            fit(s_lines[line], sizeof(s_lines[line]), source, width);
            return;
        }
        // The longest run of words that fits.
        int length = 0;
        for (int next = 0;; next++) {
            if (source[next] == ' ' || source[next] == 0) {
                snprintf(trial, sizeof(trial), "%.*s", next, source);
                if (length > 0 && textWidth(trial) > width) {
                    break;
                }
                length = next;
                if (source[next] == 0) {
                    break;
                }
            }
        }
        if (length == 0) {
            length = (int)strlen(source);
        }
        snprintf(s_lines[line], sizeof(s_lines[line]), "%.*s", length, source);
        source += length;
        while (*source == ' ') {
            source++;
        }
    }
}
}

void AchievementsPanel::open()
{
    page_ = PAGE_UNLOCKED;
    buildEntries();
}

void AchievementsPanel::buildEntries()
{
    entryCount_ = 0;
    first_ = 0;
    for (int i = 0; i < pc::achievementCount() && entryCount_ < 128; i++) {
        if (pc::achievementUnlocked(i) == (page_ == PAGE_UNLOCKED)) {
            entries_[entryCount_++] = i;
        }
    }
}

int AchievementsPanel::rows() const
{
    const int rows = entryCount_ - first_;
    return rows < 1 ? 1 : rows > VISIBLE_ROWS ? VISIBLE_ROWS : rows;
}

void AchievementsPanel::execute(menu::MenuItem& item)
{
    MenuTemplate_materiel::MATERIEL_SETTINGS(&item, rows(), ROW_STEP);
}

bool AchievementsPanel::update(menu::MenuItem& item, int& redraw)
{
    const int before = item.active_;
    item.execInput();
    if (item.active_ != before) {
        redraw = 1; // the window below describes the chosen one
    }
    // L and R (or left and right) turn the pages.
    int page = page_;
    const int edge = dss::g_Pad.edge();
    if (edge & (PAD_BUTTON_R | PAD_KEY_RIGHT)) {
        page = (page + 1) % PAGE_COUNT;
    }
    if (edge & (PAD_BUTTON_L | PAD_KEY_LEFT)) {
        page = (page + PAGE_COUNT - 1) % PAGE_COUNT;
    }
    if (page != page_) {
        page_ = page;
        buildEntries();
        item.active_ = 0;
        item.result_ = 0;
        item.lastresult_ = 0;
        redraw = 1;
        return false;
    }
    switch (item.result_) {
    case 2:
        item.result_ = 0;
        item.lastresult_ = 0;
        break;
    case 3:
        item.result_ = 0;
        item.lastresult_ = 0;
        page_ = PAGE_UNLOCKED;
        return true;
    case 5:
        // Up from the first line: the list scrolls, or wraps to its end.
        if (first_ > 0) {
            first_--;
            item.active_ = 0;
        } else {
            first_ = entryCount_ > VISIBLE_ROWS ? entryCount_ - VISIBLE_ROWS : 0;
            item.active_ = rows() - 1;
        }
        redraw = 1;
        break;
    case 6:
        // Down from the last line: the list scrolls, or wraps to its start.
        if (first_ + rows() < entryCount_) {
            first_++;
            item.active_ = rows() - 1;
        } else {
            first_ = 0;
            item.active_ = 0;
        }
        redraw = 1;
        break;
    }
    return false;
}

void AchievementsPanel::draw(menu::MenuItem& item, int x, int w, void (*frame)(int, int, int, int, int))
{
    const int language = pc::gameSettings().language;
    const int unlocked = pc::achievementsUnlocked();
    snprintf(s_pages[PAGE_UNLOCKED], sizeof(s_pages[0]), "%s %d/%d", pc::achievementsUnlockedPage(language), unlocked,
             pc::achievementCount());
    snprintf(s_pages[PAGE_MISSING], sizeof(s_pages[0]), "%s %d", pc::achievementsLockedPage(language),
             pc::achievementCount() - unlocked);
    const char* pages[PAGE_COUNT] = { s_pages[PAGE_UNLOCKED], s_pages[PAGE_MISSING] };
    unkfunc_native_tabs(pages, PAGE_COUNT, page_, x, w, frame);

    // One window: the list, then below its line the chosen one's badge and description.
    frame(x, LIST_TOP, w, 0xc0 - LIST_TOP, LINE_Y);
    const bool locked = page_ == PAGE_MISSING;
    if (entryCount_ == 0) {
        snprintf(s_empty, sizeof(s_empty), "%s", pc::achievementsNone(language));
        text(s_left, s_empty, TEXT_LEFT, 32);
    }
    for (int i = 0; i < rows() && first_ + i < entryCount_; i++) {
        const int index = entries_[first_ + i];
        const int y = 32 + i * ROW_STEP;
        badge(index, locked, TEXT_LEFT, y, ICON_SIZE);
        fit(s_title[i], sizeof(s_title[i]), pc::achievementTitle(index, language), TITLE_WIDTH);
        text(s_left, s_title[i], TITLE_LEFT, y);
    }
    if (entryCount_ > 0 && first_ + item.active_ < entryCount_) {
        const int index = entries_[first_ + item.active_];
        badge(index, locked, BADGE_LEFT, LINE_Y + 6, BADGE_SIZE);
        wrap(pc::achievementDescription(index, language), DETAIL_WIDTH);
        for (int line = 0; line < DESCRIPTION_LINES; line++) {
            if (s_lines[line][0]) {
                // Three lines clear of the window's bottom edge (from 0xb8).
                text(s_detail, s_lines[line], DETAIL_LEFT, LINE_Y + 4 + line * 12);
            }
        }
    }
    item.drawActive();
}
#endif
