#include "ov016/MaterielMenu_PICTUREBOOK/MaterielMenu_PICTUREBOOK.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/sound/MenuSoundManager.hpp"
#include "main/cmn/ExtraMapLink.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/BattleResult.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"

THUMB void MaterielMenu_PICTUREBOOK_ROOT::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    m_bookData = status::excelParam.bookData_;
    m_activeMonster = MaterielMenuPlayerControl::getSingleton()->activeItem_;
    m_nowPage = MaterielMenuPlayerControl::getSingleton()->activeItemPage_;
    unk_98 = 1;
    m_state = 0;
    menuItem_.active_ = m_activeMonster;
    navigator_.setupBase();
    navigator_.setup(2, 8, MaterielMenu_PICTUREBOOK_DETAIL::MAX_MONSTER_NO + 1);
    int find = 0;
    gMaterielMenu_PICTUREBOOK_DETAIL.isOpen_ = 0;
    if (m_activeMonster == 0 && m_nowPage == 0) {
        for (int i = 0; i < MaterielMenu_PICTUREBOOK_DETAIL::MAX_MONSTER_NO + 1; i++) {
            for (int j = 0; j < 309; j++) {
                if (j == m_bookData[i].name) {
                    if (status::g_BattleResult.isEncount(i) == true) {
                        m_nowPage = i / 16;
                        m_activeMonster = i % 16;
                        find = 1;
                    }
                    break;
                }
            }
            if (find == 1) {
                break;
            }
        }
    }
    navigator_.setPageNo(m_nowPage);
    getMonsterFlag();
    int activeItem = m_activeMonster;
    MaterielMenuPlayerControl::getSingleton()->activeItem_ = activeItem;
    int activeItemPage = m_nowPage;
    MaterielMenuPlayerControl::getSingleton()->activeItemPage_ = activeItemPage;
}

THUMB void MaterielMenu_PICTUREBOOK_ROOT::menuExecute()
{
    if (m_nowPage == 13) {
        MenuTemplate_materiel::MATERIEL_PICTUREBOOK_SELECT(&menuItem_, m_activeMonster, 2);
    } else {
        MenuTemplate_materiel::MATERIEL_PICTUREBOOK_SELECT(&menuItem_, m_activeMonster, 16);
    }
}

THUMB void MaterielMenu_PICTUREBOOK_ROOT::menuDraw()
{
    if (status::g_BattleResult.getEncountCount() != 0) {
        unkfunc_0216fda0(monsterName_, monsterFlag_);
        if (m_state == 0 && !data_020ed1bc.isOpen()) {
            menuItem_.drawActive();
        }
    }
}

THUMB void MaterielMenu_PICTUREBOOK_ROOT::menuUpdate()
{
    if (!MenuSoundManager::getSingleton()->isPlaySound() && m_state == 1) {
        m_state = 0;
        data_020ed1bc.openMessageForMENU();
        data_020ed1bc.addMessage(0xc3d9c, 0xc3d9d);
    }
    if (data_020ed1bc.isOpen()) {
        if ((unsigned int)(data_020ed1bc.stat_ - 1) <= 1) {
            data_020ed1bc.close();
            close();
            cmn::g_extraMapLink.setMonstarBookLink();
            g_Stage.returnBookFlag_ = 1;
        }
        return;
    }
    if (m_state == 1) {
        return;
    }
    navigator_.setup(2, 8, MaterielMenu_PICTUREBOOK_DETAIL::MAX_MONSTER_NO + 1);
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result != 0) {
        m_activeMonster = menuItem_.active_;
        int activeItem = m_activeMonster;
        MaterielMenuPlayerControl::getSingleton()->activeItem_ = activeItem;
        if (m_nowPage != navigator_.getPageNo()) {
            m_nowPage = navigator_.getPageNo();
            int activeItemPage = m_nowPage;
            MaterielMenuPlayerControl::getSingleton()->activeItemPage_ = activeItemPage;
            getMonsterFlag();
        }
        if (result == 2 && monsterFlag_[m_activeMonster] == 1) {
            close();
            gMaterielMenu_PICTUREBOOK_DETAIL.isOpen_ = 1;
            gMaterielMenu_PICTUREBOOK_DETAIL.open();
        }
        if (result == 3 && !checkCompletePictureBook()) {
            MaterielMenuPlayerControl::getSingleton()->allClear();
            close();
            cmn::g_extraMapLink.setMonstarBookLink();
            g_Stage.returnBookFlag_ = 1;
        }
        if (result == 7) {
            if (status::g_BattleResult.getEncountCount() == 0) {
                m_activeMonster = navigator_.pageBack(m_activeMonster);
                m_nowPage = navigator_.getPageNo();
            } else {
                while (!checkPage()) {
                    m_activeMonster = navigator_.pageNext(m_activeMonster);
                    m_nowPage = navigator_.getPageNo();
                }
                getMonsterFlag();
            }
            int activeItem = m_activeMonster;
            MaterielMenuPlayerControl::getSingleton()->activeItem_ = activeItem;
            int activeItemPage = m_nowPage;
            MaterielMenuPlayerControl::getSingleton()->activeItemPage_ = activeItemPage;
        }
        if (result == 6) {
            if (status::g_BattleResult.getEncountCount() == 0) {
                m_activeMonster = navigator_.pageNext(m_activeMonster);
                m_nowPage = navigator_.getPageNo();
            } else {
                while (!checkPage()) {
                    m_activeMonster = navigator_.pageBack(m_activeMonster);
                    m_nowPage = navigator_.getPageNo();
                }
                getMonsterFlag();
            }
            int activeItem = m_activeMonster;
            MaterielMenuPlayerControl::getSingleton()->activeItem_ = activeItem;
            int activeItemPage = m_nowPage;
            MaterielMenuPlayerControl::getSingleton()->activeItemPage_ = activeItemPage;
        }
        redraw_ = 1;
    }
}

THUMB bool MaterielMenu_PICTUREBOOK_ROOT::checkPage()
{
    int start = m_nowPage * 16;
    for (int i = start; i < start + 16; i++) {
        if (status::g_BattleResult.isEncount(i) == true) {
            return true;
        }
    }
    return false;
}

THUMB void MaterielMenu_PICTUREBOOK_ROOT::getMonsterFlag()
{
    int start = m_nowPage * 16;
    int end = start + 16;
    for (int i = 0; i < 16; i++) {
        monsterName_[i] = -1;
        monsterFlag_[i] = 0;
    }
    if (m_nowPage == 13) {
        if (status::g_BattleResult.isEncount(m_nowPage * 16)) {
            monsterName_[0] = m_bookData[m_nowPage * 16].name;
            monsterName_[1] = m_bookData[m_nowPage * 16 + 1].name;
            monsterFlag_[0] = 1;
            monsterFlag_[1] = 1;
        }
        return;
    }
    int index = 0;
    for (int i = start; i < end; index++, i++) {
        if (status::g_BattleResult.isEncount(i) == true) {
            for (int j = 0; j < 309; j++) {
                if (j == m_bookData[i].name) {
                    monsterName_[index] = j;
                    monsterFlag_[index] = 1;
                    break;
                }
            }
        }
    }
}

THUMB bool MaterielMenu_PICTUREBOOK_ROOT::checkCompletePictureBook()
{
    if (status::g_Story.isCompleteCoin() == true) {
        return false;
    }
    if (status::g_BattleResult.getEncountCount() != MaterielMenu_PICTUREBOOK_DETAIL::MAX_MONSTER_NO + 1) {
        return false;
    }
    MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_FANFARE_L);
    status::g_Party.addCasinoCoin(300000);
    status::g_Story.setCompleteCoin(true);
    m_state = 1;
    return true;
}
