#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MaterielMenuPlayerControl.hpp"
#include "main/param/Param.hpp"

struct MaterielMenu_PICTUREBOOK_DETAIL : menu::MenuBase
{
    static const int MAX_MONSTER_NO = 209;
    static const int MONSTER_COUNT_IN_PAGE = 16;    

    menu::MenuItem menuItem_;           /* 0x1C */
    CursorMoveGridLoop navigator_;     /* 0x80 */
    int monsterNo_;                     /* 0x8C */
    int activeMonster_;                 /* 0x90 */
    int isOpen_;                        /* 0x94 */
    param::BookData* bookData_;         /* 0x98 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void checkPage(bool next);
};

struct MaterielMenu_PICTUREBOOK_ROOT : menu::MenuBase
{
    menu::MenuItem menuItem_;           /* 0x1C */
    CursorMoveGridLoop navigator_;     /* 0x80 */
    param::BookData* m_bookData;        /* 0x8C */
    int m_activeMonster;                /* 0x90 */
    int m_nowPage;                      /* 0x94 */
    int unk_98;                         /* 0x98 */
    int m_state;                        /* 0x9C */
    int monsterName_[16];               /* 0xA0 */
    int monsterFlag_[16];               /* 0xE0 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    bool checkPage();
    void getMonsterFlag();
    bool checkCompletePictureBook();
};

extern MaterielMenu_PICTUREBOOK_DETAIL gMaterielMenu_PICTUREBOOK_DETAIL;
extern MaterielMenu_PICTUREBOOK_ROOT gMaterielMenu_PICTUREBOOK_ROOT;

