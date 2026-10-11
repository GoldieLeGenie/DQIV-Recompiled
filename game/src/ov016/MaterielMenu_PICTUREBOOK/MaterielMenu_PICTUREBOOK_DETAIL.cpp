#include "ov016/MaterielMenu_PICTUREBOOK/MaterielMenu_PICTUREBOOK.hpp"
#include "ov006/BookMonsterDraw.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/BattleResult.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"

THUMB void MaterielMenu_PICTUREBOOK_DETAIL::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    menuItem_.enableSE_ = 0;
    bookData_ = status::excelParam.bookData_;
    int activeItem = MaterielMenuPlayerControl::getSingleton()->activeItem_;
    activeMonster_ = activeItem + MaterielMenuPlayerControl::getSingleton()->activeItemPage_ * MONSTER_COUNT_IN_PAGE;
    navigator_.setupBase();
    navigator_.setup(1, 1, MAX_MONSTER_NO + 1);
    navigator_.setPageNo(activeMonster_);
    BookMonsterDraw::getSingleton()->setup(bookData_[activeMonster_].name);
}

THUMB void MaterielMenu_PICTUREBOOK_DETAIL::menuExecute()
{
    MenuTemplate_materiel::MATERIEL_PICTUREBOOK_MONSTERANIME(&menuItem_);
}

THUMB void MaterielMenu_PICTUREBOOK_DETAIL::menuDraw()
{
    unkfunc_0216fda8(activeMonster_, bookData_[activeMonster_].name);
    menuItem_.drawActive();
}

THUMB void MaterielMenu_PICTUREBOOK_DETAIL::menuUpdate()
{
    navigator_.setup(1, 1, MAX_MONSTER_NO + 1);
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result != 0) {
        if (result == 3) {
            int activeItem = activeMonster_ % MONSTER_COUNT_IN_PAGE;
            MaterielMenuPlayerControl::getSingleton()->activeItem_ = activeItem;
            int activeItemPage = activeMonster_ / MONSTER_COUNT_IN_PAGE;
            MaterielMenuPlayerControl::getSingleton()->activeItemPage_ = activeItemPage;
            close();
            gMaterielMenu_PICTUREBOOK_ROOT.open();
        }
        if (result == 7) {
            activeMonster_ = navigator_.getIndex(0);
            checkPage(true);
            BookMonsterDraw::getSingleton()->setup(bookData_[activeMonster_].name);
        }
        if (result == 6) {
            activeMonster_ = navigator_.getIndex(0);
            checkPage(false);
            BookMonsterDraw::getSingleton()->setup(bookData_[activeMonster_].name);
        }
        redraw_ = 1;
    }
}

THUMB void MaterielMenu_PICTUREBOOK_DETAIL::checkPage(bool next)
{
    while (!status::g_BattleResult.isEncount(activeMonster_)) {
        if (next) {
            activeMonster_++;
            if (activeMonster_ > MAX_MONSTER_NO) {
                activeMonster_ = 0;
            }
        } else {
            activeMonster_--;
            if (activeMonster_ < 0) {
                activeMonster_ = MAX_MONSTER_NO;
            }
        }
    }
    navigator_.setPageNo(activeMonster_);
}
