#pragma ipa file
#include "ov016/TownMenu_OPERATION/TownMenu_OPERATION_SETTING.hpp"
#include "ov016/TownMenu_OPERATION/TownMenu_OPERATION_ROOT.hpp"
#include "ov016/TownMenu_ROOT/TownMenu_ROOT.hpp"
#include "main/menu/MenuAPI.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/OptionStatus.hpp"
#include "ov016/MenuTemplate/MenuTemplate_town.hpp"
#include "ov016/UnkTownMenuDraw/UnkTownMenuDraw_02173d7c.hpp"
#include "ov016/UnkTownMenuDraw/UnkTownMenuDraw_0217ad94.hpp"
#include "ov016/UnkTownMenuDraw/UnkTownMenuDraw_0217d560.hpp"
#include "main/menu/UnkMenuPartsDraw.hpp"
#include "main/menu/UnkMenuCommonDraw_0201e194.hpp"

static UnkMenuParts s_parts[] = {
    { 0x15, 0xb5, 0, 0, 0, 0x58, 0, 0 },
    { 0x15, 0xb5, 0, 0, 0, 0x74, 0, 0 },
    { 0x15, 0xb5, 0, 0, 0, 0x90, 0, 0 },
    { 0x15, 0xb4, 0, 0, 0, 0x24, 0, 0 },
    { 0xff, 0, 0, 0, 0, 0, 0, 0 },
};

THUMB void TownMenu_OPERATION_SETTING::menuSetup()
{
#ifdef DQIV_NATIVE
    status::g_Party.setPlayerMode();
    bgmItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    bgmItem_.active_ = 0;
    settings_.open(true);
    return;
#endif
    status::g_Party.setPlayerMode();
    bgmItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    seItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
    speedItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    bgmItem_.enableSE_ = 0;
    seItem_.enableSE_ = 0;
    speedItem_.enableSE_ = 0;
    cancelItem_.enableSE_ = 0;
    mode_ = 2;
    prevMode_ = 2;
    unkfunc_0217a8f8();
}

THUMB void TownMenu_OPERATION_SETTING::menuExecute()
{
#ifdef DQIV_NATIVE
    settings_.execute(bgmItem_);
    return;
#endif
    MenuTemplate_town::TOWN_OP_BGMVOL(&bgmItem_, bgmItem_.active_);
    MenuTemplate_town::TOWN_OP_EFFECTVOL(&seItem_, seItem_.active_);
    MenuTemplate_town::TOWN_OP_BATTLE(&speedItem_, speedItem_.active_);
    MenuTemplate_town::TOWN_CANCEL(&cancelItem_);
}

THUMB void TownMenu_OPERATION_SETTING::menuDraw()
{
#ifdef DQIV_NATIVE
    settings_.draw(bgmItem_, 0, 0x100, unkfunc_0201e194);
    return;
#endif
    unkfunc_02173d7c();
    int y[3] = { 0x50, 0x6c, 0x88 };
    for (int i = 0; i < 3; i++) {
        unkfunc_02173da4(0x68, y[i]);
    }
    unkfunc_0217cc8c();
    unkfunc_0201e194(0, 0x20, 0x100, 0x80, 0x40);
    unkfunc_0217ad94(5, -1);
    unkfunc_0217eb5c(0x40, 0xa0, 0);
    unkfunc_0217aa8c();
    unkfunc_0201e260();
    unkfunc_0201e350(-1, -1, 0);
    bgmItem_.drawActive();
    seItem_.drawActive();
    speedItem_.drawActive();
    cancelItem_.drawActive();
}

THUMB void TownMenu_OPERATION_SETTING::menuUpdate()
{
#ifdef DQIV_NATIVE
    if (settings_.update(bgmItem_, redraw_)) {
        close();
        gTownMenu_OPERATION_ROOT.open();
    }
    return;
#endif
    if (!unkfunc_0217aad4()) {
        unkfunc_0217a924();
        unkfunc_0217a994();
        unkfunc_0217aa14();
    }
    if (mode_ == prevMode_) {
        return;
    }
    switch (mode_) {
    case 2:
        bgmItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
        seItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
        speedItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
        break;
    case 3:
        bgmItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
        seItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
        speedItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
        break;
    case 4:
        bgmItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
        seItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
        speedItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
        break;
    }
    bgmItem_.enableSE_ = 0;
    seItem_.enableSE_ = 0;
    speedItem_.enableSE_ = 0;
    unkfunc_0217a8f8();
    prevMode_ = mode_;
}

THUMB void TownMenu_OPERATION_SETTING::unkfunc_0217a8f8()
{
    bgmItem_.active_ = g_Option.getBgmVolume();
    seItem_.active_ = g_Option.getSeVolume();
    speedItem_.active_ = g_Option.getBattleSpeed();
}

THUMB void TownMenu_OPERATION_SETTING::unkfunc_0217a924()
{
    bgmItem_.execInput();
    switch (bgmItem_.result_) {
    case menu::MenuItem::MENUITEM_RESULT_NONE:
        break;
    case menu::MenuItem::MENUITEM_RESULT_CHANGE:
        mode_ = 2;
        g_Option.setBgmVolume(bgmItem_.active_);
        redraw_ = 1;
        break;
    case menu::MenuItem::MENUITEM_RESULT_OK:
        bgmItem_.result_ = menu::MenuItem::MENUITEM_RESULT_NONE;
        bgmItem_.lastresult_ = menu::MenuItem::MENUITEM_RESULT_NONE;
        redraw_ = 1;
        break;
    case menu::MenuItem::MENUITEM_RESULT_CANCEL:
        break;
    case menu::MenuItem::MENUITEM_RESULT_SUPERCANCEL:
        gTownMenu_ROOT.stat_ = menu::MenuBase::MENUBASE_STAT_CANCEL;
        MenuAPI::clearMenuAll();
        break;
    case menu::MenuItem::MENUITEM_RESULT_UP:
        mode_ = 4;
        break;
    case menu::MenuItem::MENUITEM_RESULT_DOWN:
        mode_ = 3;
        break;
    case menu::MenuItem::MENUITEM_RESULT_LEFT:
        break;
    case menu::MenuItem::MENUITEM_RESULT_RIGHT:
        break;
    }
}

THUMB void TownMenu_OPERATION_SETTING::unkfunc_0217a994()
{
    seItem_.execInput();
    switch (seItem_.result_) {
    case menu::MenuItem::MENUITEM_RESULT_NONE:
        break;
    case menu::MenuItem::MENUITEM_RESULT_CHANGE:
        mode_ = 3;
        g_Option.setSeVolume(seItem_.active_);
        redraw_ = 1;
        break;
    case menu::MenuItem::MENUITEM_RESULT_OK:
        seItem_.result_ = menu::MenuItem::MENUITEM_RESULT_NONE;
        seItem_.lastresult_ = menu::MenuItem::MENUITEM_RESULT_NONE;
        redraw_ = 1;
        break;
    case menu::MenuItem::MENUITEM_RESULT_CANCEL:
        break;
    case menu::MenuItem::MENUITEM_RESULT_SUPERCANCEL:
        gTownMenu_ROOT.stat_ = menu::MenuBase::MENUBASE_STAT_CANCEL;
        MenuAPI::clearMenuAll();
        break;
    case menu::MenuItem::MENUITEM_RESULT_UP:
        mode_ = 2;
        break;
    case menu::MenuItem::MENUITEM_RESULT_DOWN:
        mode_ = 4;
        break;
    case menu::MenuItem::MENUITEM_RESULT_LEFT:
        break;
    case menu::MenuItem::MENUITEM_RESULT_RIGHT:
        break;
    }
}

THUMB void TownMenu_OPERATION_SETTING::unkfunc_0217aa14()
{
    speedItem_.execInput();
    switch (speedItem_.result_) {
    case menu::MenuItem::MENUITEM_RESULT_NONE:
        break;
    case menu::MenuItem::MENUITEM_RESULT_CHANGE:
        mode_ = 4;
        g_Option.setBattleSpeed(speedItem_.active_);
        redraw_ = 1;
        break;
    case menu::MenuItem::MENUITEM_RESULT_OK:
        speedItem_.result_ = menu::MenuItem::MENUITEM_RESULT_NONE;
        speedItem_.lastresult_ = menu::MenuItem::MENUITEM_RESULT_NONE;
        redraw_ = 1;
        break;
    case menu::MenuItem::MENUITEM_RESULT_CANCEL:
        break;
    case menu::MenuItem::MENUITEM_RESULT_SUPERCANCEL:
        gTownMenu_ROOT.stat_ = menu::MenuBase::MENUBASE_STAT_CANCEL;
        MenuAPI::clearMenuAll();
        break;
    case menu::MenuItem::MENUITEM_RESULT_DOWN:
        mode_ = 2;
        break;
    case menu::MenuItem::MENUITEM_RESULT_UP:
        mode_ = 3;
        break;
    case menu::MenuItem::MENUITEM_RESULT_LEFT:
        break;
    case menu::MenuItem::MENUITEM_RESULT_RIGHT:
        break;
    }
}

THUMB void TownMenu_OPERATION_SETTING::unkfunc_0217aa8c()
{
    s_parts[0].x_ = bgmItem_.active_ * 32 + 0x66;
    s_parts[1].x_ = seItem_.active_ * 32 + 0x66;
    s_parts[2].x_ = speedItem_.active_ * 32 + 0x66;
    s_parts[3].x_ = 0xc;
    s_parts[3].y_ = (mode_ - 2) * 0x1c + 0x58;
    drawMenuPartsPlain(s_parts, 0);
}

THUMB int TownMenu_OPERATION_SETTING::unkfunc_0217aad4()
{
    cancelItem_.execInput();
    if (cancelItem_.result_ == menu::MenuItem::MENUITEM_RESULT_CANCEL) {
        cancelItem_.result_ = menu::MenuItem::MENUITEM_RESULT_NONE;
        cancelItem_.lastresult_ = menu::MenuItem::MENUITEM_RESULT_NONE;
        close();
        gTownMenu_OPERATION_ROOT.open();
        return 1;
    }
    return 0;
}
