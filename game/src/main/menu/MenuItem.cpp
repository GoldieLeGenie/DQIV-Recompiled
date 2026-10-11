#include "globaldefs.h"
#include "main/dss/UnkDisplay.hpp"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/menu/UnkMenuArrowDisplay.hpp"
#include "main/menu/UnkMenuDisplays.hpp"
#include "main/dss/Camera.hpp"
#include "main/dss/Pad.hpp"
#include "main/sound/Sound.hpp"
#include "nitro/pad.h"
#include "main/menu/UnkMenuPartsDraw.hpp"

THUMB void unkfunc_020518f8(MENUITEM_DATA* data, int x, int y)
{
    data->x = x;
    data->y = y;
}

THUMB void menu::MenuItem::setup(MENUITEM_TYPE mtype, CURSORTYPE ctype)
{
    navMode_ = 1;
    mtype_ = mtype;
    ctype_ = ctype;
    result_ = MENUITEM_RESULT_NONE;
    lastresult_ = MENUITEM_RESULT_NONE;
    menuitem_data_ = NULL;
    unk_34 = -1;
    active_ = 0;
    unk_3C = -1;
    unk_44 = 0;
    unk_48 = 0;
    enableSE_ = 1;
    enableDirectButton_ = 0;
    enableLoopEdge_ = 0;
    switch (mtype_) {
    case MENUITEM_TYPE_TOUCH:
        flagTouch_ = 1;
        enablePad_ = 0;
        enableCancel_ = 0;
        break;
    case MENUITEM_TYPE_TOUCH_PAD:
        flagTouch_ = 1;
        enablePad_ = 1;
        enableCancel_ = 0;
        break;
    case MENUITEM_TYPE_TOUCH_CANCEL:
        flagTouch_ = 1;
        enablePad_ = 0;
        enableCancel_ = 1;
        break;
    case MENUITEM_TYPE_TOUCH_PAD_CANCEL:
        flagTouch_ = 1;
        enablePad_ = 1;
        enableCancel_ = 1;
        break;
    default:
        flagTouch_ = 0;
        enablePad_ = 0;
        enableCancel_ = 0;
        break;
    }
}

THUMB void menu::MenuItem::drawActive()
{
    MENUITEM_DATA* data = menuitem_data_;
    if (data == NULL) {
        return;
    }
    unkfunc_02081254();
    for (int i = 0; i <= menuitem_max_; data++, i++) {
        int view;
        switch (data->view) {
        case 0:
            view = 0;
            break;
        case 1:
            view = 1;
            break;
        case 2:
            view = 2;
            break;
        case 3:
            view = 3;
            break;
        default:
            view = -1;
            break;
        }
        if (view == -1) {
            continue;
        }
        int x = unk_44 + data->x;
        int y = unk_48 + data->y;
        int w = data->w;
        int h = data->h;
        if (enablePad_ && i == active_) {
            switch (ctype_) {
            case CURSORTYPE_UP:
                x += w / 2;
                y += h;
                break;
            case CURSORTYPE_DOWN:
                x += w / 2;
                y -= 8;
                break;
            case CURSORTYPE_LEFT:
                x += w;
                y += h / 2;
                break;
            case CURSORTYPE_RIGHT:
                y += h / 2;
                break;
            case CURSORTYPE_ACTIVE:
                y += h / 2;
                break;
            case CURSORTYPE_INACTIVE:
                y += h / 2;
                break;
            }
            data_020f530c.arrow_.unkfunc_02052658(x, y, ctype_, 0);
        }
    }
}

THUMB void menu::MenuItem::setMenuItem(MENUITEM_DATA* data, int width, int height, int count)
{
    menuitem_data_ = data;
    menuitem_width_ = width;
    menuitem_height_ = height;
    menuitem_min_ = 0;
    menuitem_max_ = count - 1;
}

THUMB void menu::MenuItem::setBaseXY(int x, int y)
{
    unk_44 = x;
    unk_48 = y;
}

THUMB int menu::MenuItem::execInput()
{
    lastresult_ = result_;
    if (menuitem_data_ != NULL) {
        unkfunc_02051b60();
        if (result_ != MENUITEM_RESULT_OK && result_ != MENUITEM_RESULT_CANCEL) {
            result_ = MENUITEM_RESULT_NONE;
            reason_ = MENUITEM_REASON_NONE;
            if (!unkfunc_02051be0() && !unkfunc_02051be4() && !check11_PAD_DirectButton() && !check20_PAD_CancelButton()
                && !check30_PAD_Noactive() && !check40_PAD_OkButton()) {
                if (navMode_ == 0) {
                    if (!unkfunc_02051d40() && !unkfunc_02051dcc() && !unkfunc_02051e5c()) {
                        unkfunc_02051ed0();
                    }
                } else {
                    if (!check50_NEW_PAD_UP() && !check60_NEW_PAD_DOWN() && !check70_NEW_PAD_LEFT()) {
                        check80_NEW_PAD_RIGHT();
                    }
                }
            }
        }
    }
    if (result_ == MENUITEM_RESULT_CHANGE || (unsigned int)(result_ - MENUITEM_RESULT_UP) <= MENUITEM_RESULT_RIGHT - MENUITEM_RESULT_UP) {
        data_020f530c.arrow_.unkfunc_02052694();
    }
    if (result_ == MENUITEM_RESULT_OK && lastresult_ != result_ && enableSE_) {
        Sound::sePlay(300);
    }
    return result_;
}

// draws the item frames on the sub screen: the active one and a quarter of the others each frame
THUMB void menu::MenuItem::unkfunc_02051b60()
{
    if (!flagTouch_) {
        return;
    }
    unk_34 = -1;
    if (!MenuManager::getUpdateTime()) {
        return;
    }
    MENUITEM_DATA* data = menuitem_data_;
    int frame = unkfunc_02081254();
    for (int i = 0; i <= menuitem_max_; i++, data++) {
        int x = unk_44 + data->x;
        int y = unk_48 + data->y;
        int w = data->w;
        int h = data->h;
        if (i == active_ || (i & 3) == (frame & 3)) {
            unkfunc_020507ec(x, y + 192, w, h, 0, 1);
        }
    }
}

THUMB int menu::MenuItem::unkfunc_02051be0()
{
    return 0;
}

THUMB int menu::MenuItem::unkfunc_02051be4()
{
    return 0;
}

THUMB int menu::MenuItem::check11_PAD_DirectButton()
{
    if (!enableDirectButton_) {
        return 0;
    }
    if (dss::g_Pad.edge() & PAD_BUTTON_A) {
        result_ = MENUITEM_RESULT_DA;
        reason_ = MENUITEM_REASON_PAD;
        return 1;
    }
    if (dss::g_Pad.edge() & PAD_BUTTON_B) {
        result_ = MENUITEM_RESULT_DB;
        reason_ = MENUITEM_REASON_PAD;
        return 1;
    }
    if (dss::g_Pad.edge() & PAD_BUTTON_X) {
        result_ = MENUITEM_RESULT_DX;
        reason_ = MENUITEM_REASON_PAD;
        return 1;
    }
    if (dss::g_Pad.edge() & PAD_BUTTON_Y) {
        result_ = MENUITEM_RESULT_DY;
        reason_ = MENUITEM_REASON_PAD;
        return 1;
    }
    return 0;
}

THUMB int menu::MenuItem::check20_PAD_CancelButton()
{
    if (!enableCancel_) {
        return 0;
    }
    if (dss::g_Pad.edge() & PAD_BUTTON_Y) {
        result_ = MENUITEM_RESULT_SUPERCANCEL;
        reason_ = MENUITEM_REASON_PAD;
        return 1;
    }
    if (dss::g_Pad.edge() & PAD_BUTTON_B) {
        result_ = MENUITEM_RESULT_CANCEL;
        reason_ = MENUITEM_REASON_PAD;
        return 1;
    }
    return 0;
}

THUMB int menu::MenuItem::check30_PAD_Noactive()
{
    if (active_ != -1) {
        menuitem_temp_x_ = active_ % menuitem_width_;
        menuitem_temp_y_ = active_ / menuitem_width_;
        return 0;
    }
    if (dss::g_Pad.unkfunc_0207f290() & PAD_KEY_ALL) {
        active_ = 0;
        result_ = MENUITEM_RESULT_CHANGE;
        reason_ = MENUITEM_REASON_PAD;
        return 1;
    }
    return 1;
}

THUMB int menu::MenuItem::check40_PAD_OkButton()
{
    if (!enablePad_) {
        return 0;
    }
    if (active_ == -1) {
        return 0;
    }
    MENUITEM_DATA* data = &menuitem_data_[active_];
    if ((dss::g_Pad.edge() & PAD_BUTTON_A) || (dss::g_Pad.edge() & PAD_BUTTON_X)) {
        if (data->code == 1) {
            result_ = MENUITEM_RESULT_OK;
            reason_ = MENUITEM_REASON_PAD;
            return 1;
        }
        return 1;
    }
    return 0;
}

THUMB int menu::MenuItem::unkfunc_02051d40()
{
    int x;
    int y;
    int index;
    int height;
    if (!enablePad_) {
        return 0;
    }
    if (!(dss::g_Pad.unkfunc_0207f290() & PAD_KEY_UP)) {
        return 0;
    }
    height = menuitem_height_;
    if (height == 1) {
        if (dss::g_Pad.edge() & PAD_KEY_UP) {
            result_ = MENUITEM_RESULT_UP;
            reason_ = MENUITEM_REASON_PAD;
        }
        return 1;
    }
    x = menuitem_temp_x_;
    y = menuitem_temp_y_;
    do {
        if (--y < 0) {
            x--;
            y = height - 1;
            if (x < 0) {
                if (dss::g_Pad.edge() & PAD_KEY_UP) {
                    result_ = MENUITEM_RESULT_UP;
                    reason_ = MENUITEM_REASON_PAD;
                }
                return 1;
            }
        }
        index = x + y * menuitem_width_;
    } while (index > menuitem_max_);
    if (active_ == index) {
        return 0;
    }
    active_ = index;
    result_ = MENUITEM_RESULT_CHANGE;
    reason_ = MENUITEM_REASON_PAD;
    return 1;
}

THUMB int menu::MenuItem::unkfunc_02051dcc()
{
    int x;
    int y;
    int index;
    int height;
    if (!enablePad_) {
        return 0;
    }
    if (!(dss::g_Pad.unkfunc_0207f290() & PAD_KEY_DOWN)) {
        return 0;
    }
    height = menuitem_height_;
    if (height == 1) {
        if (dss::g_Pad.edge() & PAD_KEY_DOWN) {
            result_ = MENUITEM_RESULT_DOWN;
            reason_ = MENUITEM_REASON_PAD;
        }
        return 1;
    }
    x = menuitem_temp_x_;
    y = menuitem_temp_y_;
    do {
        if (++y >= height) {
            x++;
            y = 0;
            if (x >= menuitem_width_) {
                if (dss::g_Pad.edge() & PAD_KEY_DOWN) {
                    result_ = MENUITEM_RESULT_DOWN;
                    reason_ = MENUITEM_REASON_PAD;
                }
                return 1;
            }
        }
        index = x + y * menuitem_width_;
    } while (index > menuitem_max_);
    if (active_ == index) {
        return 0;
    }
    active_ = index;
    result_ = MENUITEM_RESULT_CHANGE;
    reason_ = MENUITEM_REASON_PAD;
    return 1;
}

THUMB int menu::MenuItem::unkfunc_02051e5c()
{
    if (!enablePad_) {
        return 0;
    }
    if (!(dss::g_Pad.unkfunc_0207f290() & PAD_KEY_LEFT)) {
        return 0;
    }
    if (menuitem_width_ == 1) {
        if (dss::g_Pad.edge() & PAD_KEY_LEFT) {
            result_ = MENUITEM_RESULT_LEFT;
            reason_ = MENUITEM_REASON_PAD;
        }
        return 1;
    }
    int index = active_ - 1;
    if (index < menuitem_min_) {
        if (dss::g_Pad.edge() & PAD_KEY_LEFT) {
            result_ = MENUITEM_RESULT_LEFT;
            reason_ = MENUITEM_REASON_PAD;
        }
        return 1;
    }
    if (active_ == index) {
        return 0;
    }
    active_ = index;
    result_ = MENUITEM_RESULT_CHANGE;
    reason_ = MENUITEM_REASON_PAD;
    return 1;
}

THUMB int menu::MenuItem::unkfunc_02051ed0()
{
    if (!enablePad_) {
        return 0;
    }
    if (!(dss::g_Pad.unkfunc_0207f290() & PAD_KEY_RIGHT)) {
        return 0;
    }
    if (menuitem_width_ == 1) {
        if (dss::g_Pad.edge() & PAD_KEY_RIGHT) {
            result_ = MENUITEM_RESULT_RIGHT;
            reason_ = MENUITEM_REASON_PAD;
        }
        return 1;
    }
    int index = active_ + 1;
    if (index > menuitem_max_) {
        if (dss::g_Pad.edge() & PAD_KEY_RIGHT) {
            result_ = MENUITEM_RESULT_RIGHT;
            reason_ = MENUITEM_REASON_PAD;
        }
        return 1;
    }
    if (active_ == index) {
        return 0;
    }
    active_ = index;
    result_ = MENUITEM_RESULT_CHANGE;
    reason_ = MENUITEM_REASON_PAD;
    return 1;
}

THUMB int menu::MenuItem::check50_NEW_PAD_UP()
{
    if (!enablePad_) {
        return 0;
    }
    if (!(dss::g_Pad.unkfunc_0207f290() & PAD_KEY_UP)) {
        return 0;
    }
    if (menuitem_height_ == 1) {
        if (dss::g_Pad.edge() & PAD_KEY_UP) {
            result_ = MENUITEM_RESULT_UP;
            reason_ = MENUITEM_REASON_PAD;
        }
        return 1;
    }
    int x = menuitem_temp_x_;
    int y = menuitem_temp_y_ - 1;
    if (y < 0) {
        if (enableLoopEdge_ && !(dss::g_Pad.edge() & PAD_KEY_UP)) {
            return 0;
        }
        result_ = MENUITEM_RESULT_UP;
        reason_ = MENUITEM_REASON_PAD;
        return 1;
    }
    int index = x + y * menuitem_width_;
    if (active_ == index) {
        return 0;
    }
    active_ = index;
    result_ = MENUITEM_RESULT_CHANGE;
    reason_ = MENUITEM_REASON_PAD;
    return 1;
}

THUMB int menu::MenuItem::check60_NEW_PAD_DOWN()
{
    if (!enablePad_) {
        return 0;
    }
    if (!(dss::g_Pad.unkfunc_0207f290() & PAD_KEY_DOWN)) {
        return 0;
    }
    int height = menuitem_height_;
    if (height == 1) {
        if (dss::g_Pad.edge() & PAD_KEY_DOWN) {
            result_ = MENUITEM_RESULT_DOWN;
            reason_ = MENUITEM_REASON_PAD;
        }
        return 1;
    }
    int width = menuitem_width_;
    int active = active_;
    int y = menuitem_temp_y_ + 1;
    int x = menuitem_temp_x_;
    if (active + width > menuitem_max_ || y >= height) {
        if (enableLoopEdge_ && !(dss::g_Pad.edge() & PAD_KEY_DOWN)) {
            return 0;
        }
        result_ = MENUITEM_RESULT_DOWN;
        reason_ = MENUITEM_REASON_PAD;
        return 1;
    }
    int index = x + y * width;
    if (active == index) {
        return 0;
    }
    active_ = index;
    result_ = MENUITEM_RESULT_CHANGE;
    reason_ = MENUITEM_REASON_PAD;
    return 1;
}

THUMB int menu::MenuItem::check70_NEW_PAD_LEFT()
{
    if (!enablePad_) {
        return 0;
    }
    if (!(dss::g_Pad.unkfunc_0207f290() & PAD_KEY_LEFT)) {
        return 0;
    }
    if (menuitem_width_ == 1) {
        if (dss::g_Pad.edge() & PAD_KEY_LEFT) {
            result_ = MENUITEM_RESULT_LEFT;
            reason_ = MENUITEM_REASON_PAD;
        }
        return 1;
    }
    int index = active_ - 1;
    if (menuitem_temp_x_ - 1 < 0) {
        result_ = MENUITEM_RESULT_LEFT;
        reason_ = MENUITEM_REASON_PAD;
        return 1;
    }
    if (active_ == index) {
        return 0;
    }
    active_ = index;
    result_ = MENUITEM_RESULT_CHANGE;
    reason_ = MENUITEM_REASON_PAD;
    return 1;
}

THUMB int menu::MenuItem::check80_NEW_PAD_RIGHT()
{
    if (!enablePad_) {
        return 0;
    }
    if (!(dss::g_Pad.unkfunc_0207f290() & PAD_KEY_RIGHT)) {
        return 0;
    }
    int width = menuitem_width_;
    if (width == 1) {
        if (dss::g_Pad.edge() & PAD_KEY_RIGHT) {
            result_ = MENUITEM_RESULT_RIGHT;
            reason_ = MENUITEM_REASON_PAD;
        }
        return 1;
    }
    int index = active_ + 1;
    int x = menuitem_temp_x_ + 1;
    if (index > menuitem_max_ || x >= width) {
        result_ = MENUITEM_RESULT_RIGHT;
        reason_ = MENUITEM_REASON_PAD;
        return 1;
    }
    if (active_ == index) {
        return 0;
    }
    active_ = index;
    result_ = MENUITEM_RESULT_CHANGE;
    reason_ = MENUITEM_REASON_PAD;
    return 1;
}
