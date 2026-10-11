#include <stdarg.h>
#pragma ipa file
#include "main/menu/DebugMenu.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/dss/Pad.hpp"
#include "main/dss/DssUtils.hpp"
#include <stdio.h>

static DebugMenuPad s_pad;
DebugMenu data_0210bc40("Debug Menu");
static char s_buildDate[0x20] = "Build Date Time";

ARM int DebugMenuPad::unkfunc_020582e0()
{
    return dss::g_Pad.unkfunc_0207f290() & 0x10;
}

ARM int DebugMenuPad::unkfunc_020582f8()
{
    return dss::g_Pad.unkfunc_0207f290() & 0x20;
}

ARM int DebugMenuPad::unkfunc_02058310()
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
    unkfunc_02058448();
}

ARM void DebugMenu::draw()
{
    unkfunc_020584c4();
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
    data_02116ce0.unkfunc_0207e864(x_ + x, y_ + y, buf);
}

ARM void DebugMenu::unkfunc_02058430(const char* date)
{
    dss::strcpy(s_buildDate, date);
}

ARM void DebugMenu::unkfunc_02058448()
{
    DebugMenuItem* item = item_;
    if (item == 0 || item->type_ == 3) {
        return;
    }
    DebugMenuItem& cur = item[cursor_];
    if (s_pad.unkfunc_020582e0()) {
        cur.unkfunc_02058568(1);
    }
    if (s_pad.unkfunc_020582f8()) {
        cur.unkfunc_02058568(-1);
    }
    if (s_pad.unkfunc_02058310()) {
        cur.unkfunc_020585c0();
    }
}

ARM void DebugMenu::unkfunc_020584c4()
{
    if (item_ == 0 || item_->type_ == 3) {
        return;
    }
    print(0, 0, "%s", title_);
    int i = 0;
    for (DebugMenuItem* item = item_; item->type_ != 3; item++) {
        item->unkfunc_020585d8(this, 1, i + 1);
        i++;
    }
    item_[cursor_].unkfunc_02058640(this, 4, 15);
    unkfunc_020583a8(i);
}

ARM void DebugMenuItem::unkfunc_02058568(int delta)
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

ARM void DebugMenuItem::unkfunc_020585c0()
{
    if (onExec_ != 0) {
        onExec_();
    }
}

ARM void DebugMenuItem::unkfunc_020585d8(DebugMenu* menu, int x, int y)
{
    if (value_ == 0) {
        menu->print(x, y, name_);
        return;
    }
    menu->print(x, y, "%d:%-10s %5d", y, name_, *value_);
}

ARM void DebugMenuItem::unkfunc_02058640(DebugMenu* menu, int x, int y)
{
    if (value_ != 0 && valueName_ != 0) {
        menu->print(x, y, "%s", valueName_[*value_]);
    }
}

