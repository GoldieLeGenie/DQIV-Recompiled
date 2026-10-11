#ifdef DQIV_NATIVE
#include "ov016/MenuSettings/SettingsPanel.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216ce4c.hpp"
#include "main/menu/UnkMenuPartsDraw.hpp"
#include "main/status/GameStatus.hpp"
#include "main/status/OptionStatus.hpp"
#include "main/text/TextAPI.hpp"
#include "main/dss/Pad.hpp"
#include "Settings.hpp"
#include <nitro/pad.h>
#include <stdio.h>
#include <string.h>

namespace {
// Texts in the game's font: the menu API takes pointers to strings in the
// program's memory.
char s_label[16][48];
char s_value[16][40];
UnkMenuParts s_left[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0, 0, 0xd0, 0xe },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
UnkMenuParts s_right[] = {
    { 0x0d, 0x0a, (short)0xf000, 0, 0, 0, 0xd0, 0xe },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
void text(UnkMenuParts* parts, char* line, int x, int y)
{
    int param = (int)line;
    parts[0].x_ = x;
    parts[0].y_ = y;
    unkfunc_02050ee0(parts, &param, 0, 0, 1);
}
void copy(char* destination, int size, const char* source)
{
    strncpy(destination, source, size - 1);
    destination[size - 1] = 0;
}
}

void SettingsPanel::open(bool inGame)
{
    inGame_ = inGame;
    page_ = pc::pageGame;
    buildRows();
}

void SettingsPanel::buildRows()
{
    rowCount_ = 0;
    switch (page_) {
    case pc::pageGame:
        rows_[rowCount_++] = pc::settingTextSpeed;
        rows_[rowCount_++] = pc::settingBattleBoost;
        if (inGame_) {
            rows_[rowCount_++] = ROW_BATTLE_SPEED;
            rows_[rowCount_++] = ROW_MUSIC;
            rows_[rowCount_++] = ROW_SOUNDS;
        }
        rows_[rowCount_++] = pc::settingVolume;
        rows_[rowCount_++] = pc::settingMobileSound;
        rows_[rowCount_++] = pc::settingLanguage;
        rows_[rowCount_++] = pc::settingFps;
        break;
    case pc::pageDisplay:
        rows_[rowCount_++] = pc::settingFullscreen;
        rows_[rowCount_++] = pc::settingWindowSize;
        rows_[rowCount_++] = pc::settingFrameRate;
        rows_[rowCount_++] = pc::settingCameraZoom;
        rows_[rowCount_++] = pc::settingPicture;
        rows_[rowCount_++] = pc::settingStretch;
        rows_[rowCount_++] = pc::settingVsync;
        rows_[rowCount_++] = pc::settingQuality;
        rows_[rowCount_++] = pc::settingTextures;
        break;
    default:
        for (int i = 0; i < pc::controlCount; i++) {
            rows_[rowCount_++] = i;
        }
        rows_[rowCount_++] = ROW_RESET;
        break;
    }
    rows_[rowCount_++] = ROW_BACK;
}

int SettingsPanel::step() const
{
    return rowCount_ <= 8 ? 16 : rowCount_ <= 12 ? 11 : 10;
}

void SettingsPanel::execute(menu::MenuItem& item)
{
    MenuTemplate_materiel::MATERIEL_SETTINGS(&item, rowCount_, step());
}

void SettingsPanel::change(int row, int step)
{
    switch (row) {
    case ROW_BATTLE_SPEED: {
        // Right: faster messages.
        int speed = g_Option.getBattleSpeed() - step;
        g_Option.setBattleSpeed(speed < 0 ? 0 : speed > 4 ? 4 : speed);
        return;
    }
    case ROW_MUSIC:
    case ROW_SOUNDS: {
        int volume = (row == ROW_MUSIC ? g_Option.getBgmVolume() : g_Option.getSeVolume()) + step;
        volume = volume < 0 ? 0 : volume > 4 ? 4 : volume;
        if (row == ROW_MUSIC) {
            g_Option.setBgmVolume(volume);
        } else {
            g_Option.setSeVolume(volume);
        }
        return;
    }
    }
    if (row < pc::settingCount && pc::changeSetting(row, step)) {
        if (row == pc::settingLanguage) {
            // The game's texts follow at once.
            status::g_Game.language = (Language)pc::gameSettings().language;
            TextAPI::setLanguage(pc::gameSettings().language);
        }
        pc::requestSettingsApply();
    }
}

bool SettingsPanel::update(menu::MenuItem& item, int& redraw)
{
    if (pc::rebindWaiting()) {
        // The window binds the next key or button.
        redraw = 1;
        return false;
    }
    const int before = item.active_;
    item.execInput();
    if (item.active_ != before) {
        redraw = 1; // the chosen line shows its value between arrows
    }
    // L and R turn the pages.
    int page = page_;
    if (dss::g_Pad.edge() & PAD_BUTTON_R) {
        page = (page + 1) % pc::pageCount;
    }
    if (dss::g_Pad.edge() & PAD_BUTTON_L) {
        page = (page + pc::pageCount - 1) % pc::pageCount;
    }
    if (page != page_) {
        page_ = page;
        buildRows();
        item.active_ = 0;
        item.result_ = 0;
        item.lastresult_ = 0;
        redraw = 1;
        return false;
    }
    const int row = rows_[item.active_];
    int step = 0;
    if (dss::g_Pad.edge() & PAD_KEY_LEFT) {
        step = -1;
    }
    if (dss::g_Pad.edge() & PAD_KEY_RIGHT) {
        step = 1;
    }
    bool activate = false;
    bool leave = false;
    switch (item.result_) {
    case 2:
        item.result_ = 0;
        item.lastresult_ = 0;
        activate = true;
        break;
    case 3:
        item.result_ = 0;
        item.lastresult_ = 0;
        leave = true;
        break;
    case 5:
        item.active_ = rowCount_ - 1;
        break;
    case 6:
        item.active_ = 0;
        break;
    }
    if (activate) {
        if (row == ROW_BACK) {
            leave = true;
        } else if (row == ROW_RESET) {
            pc::resetControls(page_);
            pc::saveSettings();
        } else if (page_ == pc::pageKeyboard || page_ == pc::pageGamepad) {
            pc::startRebind(page_, row);
        } else {
            step = 1;
        }
        redraw = 1;
    }
    if (step != 0 && (page_ == pc::pageGame || page_ == pc::pageDisplay) && row != ROW_BACK) {
        change(row, step);
        redraw = 1;
    }
    if (leave) {
        pc::saveSettings();
        page_ = pc::pageGame;
        buildRows();
        return true;
    }
    return false;
}

void SettingsPanel::draw(menu::MenuItem& item, int x, int w, void (*frame)(int, int, int, int, int))
{
    const int language = pc::gameSettings().language;
    const char* pages[pc::pageCount];
    for (int page = 0; page < pc::pageCount; page++) {
        pages[page] = pc::pageTitle(page, language);
    }
    unkfunc_native_tabs(pages, pc::pageCount, page_, x, w, frame);
    frame(x, 0x18, w, 0xc0 - 0x18, -1);
    for (int i = 0; i < rowCount_; i++) {
        const int row = rows_[i];
        const int y = 32 + i * step();
        const char* label = "";
        char value[40] = "";
        if (row == ROW_BACK) {
            label = pc::settingsBack(language);
        } else if (row == ROW_RESET) {
            label = pc::resetLabel(language);
        } else if (row == ROW_BATTLE_SPEED) {
            label = pc::battleSpeedLabel(language);
            copy(value, sizeof(value), pc::battleSpeedValue(g_Option.getBattleSpeed(), language));
        } else if (row == ROW_MUSIC || row == ROW_SOUNDS) {
            label = row == ROW_MUSIC ? pc::musicLabel(language) : pc::soundsLabel(language);
            snprintf(value, sizeof(value), "%d%%",
                     ((row == ROW_MUSIC ? g_Option.getBgmVolume() : g_Option.getSeVolume()) + 1) * 20);
        } else if (page_ == pc::pageKeyboard || page_ == pc::pageGamepad) {
            label = pc::controlName(row, language);
            const bool waiting = pc::rebindWaiting() && pc::rebindAction() == row;
            copy(value, sizeof(value), waiting ? pc::rebindPrompt(language) : pc::bindingName(page_, row, language).c_str());
        } else {
            label = pc::settingLabel(row, language);
            copy(value, sizeof(value), pc::settingValue(row, language).c_str());
        }
        copy(s_label[i], sizeof(s_label[i]), label);
        text(s_left, s_label[i], 0x18, y);
        if (value[0]) {
            // The chosen line's value can be changed with left and right.
            const bool adjustable = i == item.active_ && (page_ == pc::pageGame || page_ == pc::pageDisplay);
            snprintf(s_value[i], sizeof(s_value[i]), adjustable ? "< %s >" : "%s", value);
            text(s_right, s_value[i], 0x18, y);
        }
    }
    item.drawActive();
}
#endif
