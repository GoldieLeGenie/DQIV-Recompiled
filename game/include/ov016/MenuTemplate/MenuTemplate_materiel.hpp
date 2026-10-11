#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"

// MenuItem layouts of the materiel (shop/church/casino/diary/...) menus
namespace MenuTemplate_materiel {
    void MATERIEL_SHOP_ROOT(menu::MenuItem* menuitem, int active);
    void MATERIEL_CHURCH_ROOT(menu::MenuItem* menuitem, int maxCommand, int active);
    void MATERIEL_ICON32_5x2_CHURCH(menu::MenuItem* menuitem, int active, int max);
    void MATERIEL_BET_COIN(menu::MenuItem* menuitem, int active, int max, int x, int y);
    void MATERIEL_YESNO_BUTTON(menu::MenuItem* menuitem, int active, int y);
    void MATERIEL_POKER_SELECT_CARD(menu::MenuItem* menuitem, int active);
    void MATERIEL_POKER_SELECT_DEAL(menu::MenuItem* menuitem, int active);
    void MATERIEL_POKER_BET_COIN(menu::MenuItem* menuitem, int active);
    void MATERIEL_POKER_HI_AND_LOW(menu::MenuItem* menuitem, int active);
    void MATERIEL_MONSTER_LIST(menu::MenuItem* menuitem, int active, int max);
    void MATERIEL_GET_UPDOWN(menu::MenuItem* menuitem);
    void MATERIEL_DIARY_MENU(menu::MenuItem* menuitem, int num);
    void MATERIEL_DIARY_SELECT(menu::MenuItem* menuitem);
    void MATERIEL_DIARY_LOAD(menu::MenuItem* menuitem);
    void MATERIEL_KEYBOARD_JAP(menu::MenuItem* menuitem);
    void MATERIEL_KEYBOARD_JAP_MESSAGE(menu::MenuItem* menuitem);
    void MATERIEL_SEXUALITY(menu::MenuItem* menuitem);
#ifdef DQIV_NATIVE
    void MATERIEL_SETTINGS(menu::MenuItem* menuitem, int num, int step);
    void MATERIEL_CELECT_BUY_TABS(menu::MenuItem* menuitem, int active, int max);
#endif
    void MATERIEL_PAGE_2x1(menu::MenuItem* menuitem, int active, int x, int y);
    void MATERIEL_CELECT_BUY(menu::MenuItem* menuitem, int active, int max);
    void MATERIEL_CELECT_SELL(menu::MenuItem* menuitem, int active, int max);
    void MATERIEL_ICON32_5x2(menu::MenuItem* menuitem, int active, int max);
    void MATERIEL_MONEY_SELECT(menu::MenuItem* menuitem, int active, int digit);
    void MATERIEL_COIN_SELECT(menu::MenuItem* menuitem, int active, int digit);
    void MATERIEL_CHENGEGIFT_5x2(menu::MenuItem* menuitem, int active, int max);
    void MATERIEL_PICTUREBOOK_SELECT(menu::MenuItem* menuitem, int active, int max);
    void MATERIEL_PICTUREBOOK_MONSTERANIME(menu::MenuItem* menuitem);
    void MATERIEL_CANCEL(menu::MenuItem* menuitem);
    void shopPlayerArrow(menu::MenuItem* menuitem);
    void shopSackArrow(menu::MenuItem* menuitem, int active);
    void shopSellQuantity(menu::MenuItem* menuitem, int active);
    void shopSellArrow(menu::MenuItem* menuitem, int active);
    void surechigaiRoot(menu::MenuItem* menuitem, int active);
    void surechigaiViewCommand(menu::MenuItem* menuitem, int active);
    void suretigaiSelectChiaus(menu::MenuItem* menuitem, int active, int max);
    void surechigaiSelectObject(menu::MenuItem* menuitem, int active, int max);
    void surechigaiSelectSex(menu::MenuItem* menuitem, int active);
    void surechigaiSelectAetas(menu::MenuItem* menuitem, int active);
    void surechigaiSelectSkill(menu::MenuItem* menuitem, int active);
}
