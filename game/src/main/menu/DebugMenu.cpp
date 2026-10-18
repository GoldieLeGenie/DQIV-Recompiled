#include <stdarg.h>
#pragma ipa file
#include "main/menu/DebugMenu.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/dss/Pad.hpp"
#include "main/dss/DssUtils.hpp"
#include <stdio.h>

static DebugMenuPad s_pad;
DebugMenu g_DebugMenu("Debug Menu");
static char s_buildDate[0x20] = "Build Date Time";

ARM int DebugMenuPad::isRightRepeat()
{
    return dss::g_Pad.repeat() & 0x10;
}

ARM int DebugMenuPad::isLeftRepeat()
{
    return dss::g_Pad.repeat() & 0x20;
}

ARM int DebugMenuPad::isAPressed()
{
    return dss::g_Pad.edge() & 1;
}

ARM DebugMenu::DebugMenu(const char* name)
{
    unk_68 = 0;
    unk_6c = 0;
    unk_70 = 1;
    dss::strcpy(name_, name);
    cursor_ = 0;
    x_ = 1;
    y_ = 1;
    item_ = 0;
}

ARM int DebugMenu::unkfunc_02058378()
{
    return unk_68;
}

ARM int DebugMenu::unkfunc_02058380()
{
    return unk_6c;
}

ARM int DebugMenu::isEnd()
{
    return true;
}

ARM void DebugMenu::update()
{
    updateItems();
}

ARM void DebugMenu::draw()
{
    drawItems();
}

ARM void DebugMenu::unkfunc_020583a8(int count)
{
    for (int i = 0; i < count; i++) {
        unk_20[i] = 1;
    }
}

ARM void DebugMenu::print(int x, int y, const char* fmt, ...)
{
    char buf[0x21];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, 0x21, fmt, args);
    g_DebugConsole.unkfunc_0207e864(x_ + x, y_ + y, buf);
}

ARM void DebugMenu::setBuildDate(const char* date)
{
    dss::strcpy(s_buildDate, date);
}

ARM void DebugMenu::updateItems()
{
    DebugMenuItem* item = item_;
    if (item == 0 || item->type_ == 3) {
        return;
    }
    DebugMenuItem& cur = item[cursor_];
    if (s_pad.isRightRepeat()) {
        cur.addValue(1);
    }
    if (s_pad.isLeftRepeat()) {
        cur.addValue(-1);
    }
    if (s_pad.isAPressed()) {
        cur.execute();
    }
}

ARM void DebugMenu::drawItems()
{
    if (item_ == 0 || item_->type_ == 3) {
        return;
    }
    print(0, 0, "%s", title_);
    int i = 0;
    for (DebugMenuItem* item = item_; item->type_ != 3; item++) {
        item->drawLine(this, 1, i + 1);
        i++;
    }
    item_[cursor_].drawValueName(this, 4, 15);
    unkfunc_020583a8(i);
}

ARM void DebugMenuItem::addValue(int delta)
{
    if (enable_ != 1) {
        return;
    }
    int next;
    if (value_ == 0) {
        return;
    }
    int old = *value_;
    next = old + delta;
    if (next < min_) {
        next = min_;
    }
    if (next >= max_) {
        next = max_;
    }
    if (next != old) {
        *value_ = next;
        if (onChange_ != 0) {
            onChange_();
        }
    }
}

ARM void DebugMenuItem::execute()
{
    if (onExec_ != 0) {
        onExec_();
    }
}

ARM void DebugMenuItem::drawLine(DebugMenu* menu, int x, int y)
{
    if (value_ == 0) {
        menu->print(x, y, name_);
        return;
    }
    menu->print(x, y, "%d:%-10s %5d", y, name_, *value_);
}

ARM void DebugMenuItem::drawValueName(DebugMenu* menu, int x, int y)
{
    if (value_ != 0 && valueName_ != 0) {
        menu->print(x, y, "%s", valueName_[*value_]);
    }
}

