#include "ov034/MaterielMenuExtraChapterTitle/MaterielMenuExtraChapterTitle.hpp"
#include "main/dss/RenderObject.hpp"
#include "ov001/window/FieldWindowSystem.hpp"
#include "main/cmn/PartyTalk.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/global/Global.hpp"
#include "main/dss/UnkSprite2D.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"


THUMB void MaterielMenuExtraChapterTitle::menuSetup()
{
    if (m_chapter_end == 1) {
        cmn::PartyTalk::getSingleton()->initialize();
        if (m_chapter == 2 || m_chapter == 4) {
            FieldWindowSystem::getSingleton()->clearAllMap();
        }
    }

    m_fade = 0;
    m_mode = 1;
    m_draw_count = 0;

    if (status::g_Story.chapter_ != 0 && m_chapter_end != 0) {
        MenuSoundManager::getSingleton()->initialize();
        MenuSoundManager::getSingleton()->setPlaySound((MenuSoundManager::MENU_SOUND)(m_chapter + 9));
    }

    g_Global.fadeInBlack(0x3C);
}

THUMB void MaterielMenuExtraChapterTitle::menuDraw()
{
    if (m_mode != 0) {
        if (m_chapter == 0 && m_chapter_end == 1) {
            unkfunc_0216fe50(m_chapter, m_chapter_end);
        } else {
            unkfunc_020848a8();
            unkfunc_0216fe50(m_chapter, m_chapter_end);
        }

        switch (m_fade) {
        case 0:
            if (g_GlobalFade.isFadeEnd() != 0) {
                m_fade = 1;
            }
            break;
        case 1:
            if (m_chapter_end == 1 && m_chapter != 0) {
                if (MenuSoundManager::getSingleton()->isPlaySound() == 0) {
                    goto fade;
                }
                break;
            }
            if (m_draw_count < 0x78) {
                m_draw_count++;
                break;
            }
        fade:
            m_fade = 2;
            g_Global.fadeOutBlack(0x2D);
            break;
        case 2:
            if (g_GlobalFade.isFadeEnd() != 0) {
                m_fade = 0;
                m_mode = 0;
            }
            break;
        }
    } else {
        switch (m_fade) {
        case 0:
            g_Global.fadeInBlack(0x1E);
            MaterielMenu_WINDOW_MANAGER::getSingleton()->chapterEnd_ = 1;
            m_fade = 3;
            break;
        case 3:
            if (g_GlobalFade.isFadeEnd() != 0) {
                MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
            }
            break;
        }
    }
}

THUMB void MaterielMenuExtraChapterTitle::menuUpdate()
{
    if (m_mode == 0 && m_fade == 0) {
        redraw_ = 1;
    }
}


THUMB void MaterielMenuExtraChapterTitle::setChapterTitleInfo(int chapter,int flag)
{
  this->m_chapter = chapter;
  this->m_chapter_end = flag;
}

ARM void MaterielMenuExtraChapterTitle::menuExecute()
{
  return;
}