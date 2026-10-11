#pragma once
#include "globaldefs.h"
#include "main/status/PartyStatus.hpp"
#include "main/cmn/GameManager.hpp"
#include "main/cmn/PlayerManager.hpp"
#include "main/global/Global.hpp"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MaterielMenuWindowManager.hpp"
#include "main/sound/MenuSoundManager.hpp"


struct MaterielMenuExtraChapterTitle : menu::MenuBase
{                                       
    int m_fade;
    int m_mode;
    int m_draw_count;
    int m_chapter;
    int m_chapter_end;
    virtual void menuSetup();
    virtual void menuDraw();
    virtual void menuUpdate();
    void setChapterTitleInfo(int chapter,int flag);
    virtual void menuExecute();
};



extern MaterielMenuExtraChapterTitle gMaterielMenu_EXTRA_CHAPTER_TITLE;
