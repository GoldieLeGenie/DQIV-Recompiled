#include "ov016/MaterielMenu_LOAD/MaterielMenu_LOAD.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/global/Global.hpp"
#include "main/status/Status.hpp"
#include "main/status/GameStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "ov016/UnkMaterielMenuDraw/MaterielMenuDrawExtra.hpp"
#ifdef DQIV_NATIVE
#include "main/menu/MenuPartsDraw.hpp"
#include "main/text/TextAPI.hpp"
#include "Settings.hpp"
#include "main/dss/Pad.hpp"
#include <nitro/pad.h>
#include <stdio.h>
#include <string.h>

namespace {
void copyText(char* destination, int size, const char* text)
{
    strncpy(destination, text, size - 1);
    destination[size - 1] = 0;
}
}

THUMB int MaterielMenu_LOAD::rootCount() const
{
    return resume_ != 0 ? ROOT_RESUME_COUNT : ROOT_COUNT;
}
#endif

const int MaterielMenu_LOAD::ROOT_RESUME_COUNT = 4;
const int MaterielMenu_LOAD::ROOT_COUNT = 3;
int MaterielMenu_LOAD::activeDiaryNo_ = -1;

THUMB void MaterielMenu_LOAD::menuSetup()
{
    rootItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    dataItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    sexualityItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
#ifdef DQIV_NATIVE
    settingsItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
#endif
    status_ = LOAD_MODESELECT;
    country_ = 0;
    sexuality_ = 0;
    messageCounter_ = -1;
    menuMode_ = 0;
    country_ = status::g_Game.language;
    killResult_ = 0;
    makeResult_ = 0;
    resume_ = 0;
    if (profile::SaveLoad::checkbank(3) == 1) {
        resume_ = 1;
    }
    frame_ = 0;
}

THUMB void MaterielMenu_LOAD::menuExecute()
{
#ifdef DQIV_NATIVE
    // The original entries, then the settings.
    MenuTemplate_materiel::MATERIEL_DIARY_MENU(&rootItem_, rootCount() + 1);
    settings_.execute(settingsItem_);
    MenuTemplate_materiel::MATERIEL_DIARY_LOAD(&dataItem_);
    MenuTemplate_materiel::MATERIEL_SEXUALITY(&sexualityItem_);
    return;
#endif
    if (resume_ != 0) {
        MenuTemplate_materiel::MATERIEL_DIARY_MENU(&rootItem_, ROOT_RESUME_COUNT);
    } else {
        MenuTemplate_materiel::MATERIEL_DIARY_MENU(&rootItem_, ROOT_COUNT);
    }
    MenuTemplate_materiel::MATERIEL_DIARY_LOAD(&dataItem_);
    MenuTemplate_materiel::MATERIEL_SEXUALITY(&sexualityItem_);
}

inline void MaterielMenu_LOAD::sexualityDraw()
{
    int message;
    int sexualityMessage[2] = { 0xa0000005, 0xa0000006 };
    drawNameEditField(country_, data_020f1878, data_020f17c0, data_020f17bc, 2, 1);
    drawFrameStyleB(0x30, 0x10, 0xa0, 0x28, -1);
    message = 0x800001a1;
    drawCenteredMessage(&message, 0x30, 0x38, 0xa0, 0x50);
    drawMessageColumn(sexualityMessage, 2, 0x70, 0x5a);
    drawFrameStyleB(0x30, 0x38, 0xa0, 0x48, 0x50);
    sexualityItem_.drawActive();
}

THUMB void MaterielMenu_LOAD::menuDraw()
{
    int rootMessage[3] = { 0x80000196, 0x80000197, 0x80000198 };
    int rootResumeMessage[4] = { 0x80000196, 0x8000019f, 0x80000197, 0x80000198 };
    if (status_ == LOAD_MODESELECT) {
#ifdef DQIV_NATIVE
        // One more line for the settings.
        static char s_settingsEntry[48];
        copyText(s_settingsEntry, sizeof(s_settingsEntry), pc::settingsTitle(pc::gameSettings().language));
        if (resume_ == 1) {
            drawMessageColumnSmall(rootResumeMessage, ROOT_RESUME_COUNT, 0x18, 0x10);
        } else {
            drawMessageColumnSmall(rootMessage, ROOT_COUNT, 0x18, 0x10);
        }
        int param = (int)s_settingsEntry;
        UnkMenuParts entry[] = {
            { 0x0d, 0x08, (short)0xf000, 0, 0x18, 0x10, 0x90, 0xe },
            { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
        };
        drawMenuParts(entry, &param, 0, rootCount() * 14, 1);
        drawFrameStyleA(8, 8, 0xa0, resume_ == 1 ? 0x48 + 16 : 0x38 + 16, -1);
        rootItem_.drawActive();
        return;
#endif
        if (resume_ == 1) {
            drawMessageColumnSmall(rootResumeMessage, ROOT_RESUME_COUNT, 0x18, 0x10);
            drawFrameStyleA(8, 8, 0xa0, 0x48, -1);
            rootItem_.drawActive();
        } else {
            drawMessageColumnSmall(rootMessage, ROOT_COUNT, 0x18, 0x10);
            drawFrameStyleA(8, 8, 0xa0, 0x38, -1);
            rootItem_.drawActive();
        }
    }
    if (status_ == LOAD_DATASELECT) {
        int message = rootMessage[0];
        int flag = 1;
        if (menuMode_ == 1) {
            message = rootMessage[1];
            flag = 0;
        } else if (menuMode_ == 2) {
            message = rootMessage[2];
        }
        drawCenteredMessage(&message, 8, 0x38, 0xf0, 0x50);
        for (int i = 0; i < 3; i++) {
            if (diary_[i].savetype_ == 4) {
                drawJournalSlot(i, diary_[i].name_, -1, -1, -1, diary_[i].time_, 0x5a, flag);
            } else {
                drawJournalSlot(i, diary_[i].name_, diary_[i].chapter_, diary_[i].level_, diary_[i].town_, diary_[i].time_, 0x5a, flag);
            }
        }
        drawFrameStyleB(8, 0x38, 0xf0, 0x78, 0x50);
        dataItem_.drawActive();
    }
    if (status_ == LOAD_SEXUALITY) {
        sexualityDraw();
    }
#ifdef DQIV_NATIVE
    if (status_ == LOAD_SETTINGS) {
        settings_.draw(settingsItem_, 0, 0x100, drawFrameStyleA);
    }
#endif
}

THUMB void MaterielMenu_LOAD::menuUpdate()
{
    if (frame_ < 30) {
        frame_++;
        return;
    }
    if (messageUpdate()) {
        return;
    }
    switch (status_) {
    case LOAD_MODESELECT:
        rootUpdate();
        break;
    case LOAD_DATACHECK:
        if (updateActiveDiary()) {
            changeStatus(LOAD_DATASELECT);
            activeDiaryNo_ = 0;
            redraw_ = 1;
        } else {
            data_020ed1bc.openMessageForMENU();
            data_020ed1bc.addMessageNOWAIT(0xcb60b);
            data_020ed1bc.addMessageWAITKEY();
            changeStatus(LOAD_BLANK);
        }
        break;
    case LOAD_DATASELECT:
        dataSelectUpdate();
        break;
    case LOAD_SEXUALITY:
        sexualityUpdate();
        break;
#ifdef DQIV_NATIVE
    case LOAD_SETTINGS:
        if (settings_.update(settingsItem_, redraw_)) {
            changeStatus(LOAD_MODESELECT);
            redraw_ = 1;
        }
        // The language may have changed.
        country_ = status::g_Game.language;
        break;
#endif
    case LOAD_WRITECHECK: {
        int result = profile::SaveLoad::isCardOK();
        if (result) {
            result = makeDiary();
        }
        if (result) {
            changeStatus(LOAD_END);
        } else {
            data_020ed1bc.close();
            data_020ed1bc.openMessageForMENU();
            data_020ed1bc.addMessageNOWAIT(0xcb5fa);
            data_020ed1bc.addMessageWAITKEY();
            changeStatus(LOAD_BLANK);
        }
        break;
    }
    case LOAD_DELETECHECK:
        if (data_020ed1bc.isOpen() && data_020ed1bc.isMessageWAITPROG()) {
            if (deleteDiary()) {
                data_020ed1bc.openMessageForMENU();
                data_020ed1bc.addMessage(0xcb605);
                messageCounter_++;
            } else {
                data_020ed1bc.openMessageForMENU();
                data_020ed1bc.addMessageNOWAIT(0xcb608);
                data_020ed1bc.addMessageWAITKEY();
                changeStatus(LOAD_BLANK);
            }
        }
        break;
    case LOAD_LOADCHECK:
        if (profile::SaveLoad::loadbank(activeDiaryNo_)) {
            changeStatus(LOAD_END);
        } else {
            data_020ed1bc.openMessageForMENU();
            data_020ed1bc.addMessageNOWAIT(0xcb60b);
            data_020ed1bc.addMessageWAITKEY();
            changeStatus(LOAD_BLANK);
        }
        break;
    case LOAD_BLANK:
        if (data_020ed1bc.isOpen()) {
            data_020ed1bc.isMessageWAITPROG();
        }
        break;
    case LOAD_END:
        close();
        break;
    case LOAD_RESUME:
        if (profile::SaveLoad::loadbank(3)) {
            changeStatus(LOAD_END);
        } else {
            data_020ed1bc.openMessageForMENU();
            data_020ed1bc.addMessageNOWAIT(0xcb60b);
            data_020ed1bc.addMessageWAITKEY();
            changeStatus(LOAD_BLANK);
        }
        break;
    }
}

THUMB bool MaterielMenu_LOAD::messageUpdate()
{
    if (!data_020ed1bc.isOpen()) {
        return false;
    }
    int stat = data_020ed1bc.stat_;
    switch (status_) {
    case LOAD_DATASELECT:
        switch (menuMode_) {
        case 0:
            if (stat == menu::MenuBase::MENUBASE_STAT_OK) {
                data_020ed1bc.close();
                changeStatus(LOAD_NAMEEDIT);
                redraw_ = 1;
            } else if (stat == menu::MenuBase::MENUBASE_STAT_CANCEL) {
                data_020ed1bc.close();
            }
            break;
        case 1:
            if (stat == menu::MenuBase::MENUBASE_STAT_OK) {
                data_020ed1bc.close();
                changeStatus(LOAD_NAMEEDIT);
                redraw_ = 1;
            } else if (stat == menu::MenuBase::MENUBASE_STAT_CANCEL) {
                data_020ed1bc.close();
            }
            break;
        case 2:
            if (stat == menu::MenuBase::MENUBASE_STAT_OK) {
                data_020ed1bc.close();
                data_020ed1bc.openMessageForMENU();
                data_020ed1bc.addMessageNOWAIT(0xcb604);
                data_020ed1bc.addMessageWAITKEY();
                changeStatus(LOAD_DELETECHECK);
            } else if (stat == menu::MenuBase::MENUBASE_STAT_CANCEL) {
                data_020ed1bc.close();
            }
            break;
        }
        break;
    case LOAD_DELETECHECK:
        if (messageCounter_ > 0) {
            if ((unsigned int)(stat - 1) <= 1) {
                redraw_ = 1;
                data_020ed1bc.close();
                changeStatus(LOAD_MODESELECT);
            }
        } else {
            return false;
        }
        break;
    case LOAD_WRITECHECK:
    case LOAD_LOADCHECK:
        if (messageCounter_ > 0) {
            if ((unsigned int)(stat - 1) <= 1) {
                redraw_ = 1;
                data_020ed1bc.close();
                changeStatus(LOAD_MODESELECT);
            }
        } else if (data_020ed1bc.isMessageWAITPROG()) {
            data_020ed1bc.close();
            return false;
        }
        break;
    case LOAD_BLANK:
        if ((unsigned int)(stat - 1) <= 1) {
            data_020ed1bc.close();
        }
        return false;
    case LOAD_END:
        closeMessage();
        break;
    }
    return true;
}

THUMB void MaterielMenu_LOAD::rootUpdate()
{
    rootItem_.execInput();
    switch (rootItem_.result_) {
    case 2: {
        rootItem_.result_ = 0;
        rootItem_.lastresult_ = 0;
        dataItem_.active_ = 0;
#ifdef DQIV_NATIVE
        if (rootItem_.active_ == rootCount()) {
            settingsItem_.active_ = 0;
            settings_.open(false);
            changeStatus(LOAD_SETTINGS);
            redraw_ = 1;
            break;
        }
#endif
        if (!profile::SaveLoad::isCardOK()) {
            stat_ = menu::MenuBase::MENUBASE_STAT_OK;
            changeStatus(LOAD_END);
            break;
        }
        if (resume_ != 0) {
            int active = rootItem_.active_;
            if (active == 0) {
                menuMode_ = 0;
            } else if (active == 1) {
                changeStatus(LOAD_RESUME);
                break;
            } else if (active == 2) {
                menuMode_ = 1;
            } else if (active == 3) {
                menuMode_ = 2;
            }
            changeStatus(LOAD_DATACHECK);
        } else {
            int active = rootItem_.active_;
            if (active == 0) {
                menuMode_ = 0;
            } else if (active == 1) {
                menuMode_ = 1;
            } else if (active == 2) {
                menuMode_ = 2;
            }
            changeStatus(LOAD_DATACHECK);
        }
        break;
    }
    case 3:
        rootItem_.result_ = 0;
        rootItem_.lastresult_ = 0;
        break;
    case 5:
#ifdef DQIV_NATIVE
        rootItem_.active_ = rootCount();
        break;
#endif
        if (resume_ != 0) {
            rootItem_.active_ = 3;
        } else {
            rootItem_.active_ = 2;
        }
        break;
    case 6:
        rootItem_.active_ = 0;
        break;
    }
}

THUMB void MaterielMenu_LOAD::dataSelectUpdate()
{
    dataItem_.execInput();
    switch (dataItem_.result_) {
    case 2: {
        dataItem_.result_ = 0;
        dataItem_.lastresult_ = 0;
        int active = dataItem_.active_;
        activeDiaryNo_ = active;
        switch (menuMode_) {
        case 0:
            if (diary_[active].name_ == 0) {
                data_020ed1bc.openMessageForMENU();
                data_020ed1bc.addMessage(0xcb5eb);
                data_020ed1bc.setYesNo();
            } else {
                changeStatus(LOAD_LOADCHECK);
            }
            break;
        case 1:
            if (diary_[active].name_ != 0) {
                data_020ed1bc.openMessageForMENU();
                data_020ed1bc.addMessage(0xcb5ef);
                data_020ed1bc.setYesNo();
            } else {
                changeStatus(LOAD_NAMEEDIT);
            }
            break;
        case 2:
            if (diary_[active].name_ != 0) {
                data_020ed1bc.openMessageForMENU();
                profile::SaveLoad::setCatalogMacro(&catalogview_[activeDiaryNo_]);
                if (diary_[activeDiaryNo_].savetype_ == 4) {
                    data_020ed1bc.addMessage(0xcb610);
                } else if (diary_[activeDiaryNo_].chapter_ >= 5) {
                    data_020ed1bc.addMessage(0xcb601);
                } else {
                    data_020ed1bc.addMessage(0xcb5fe);
                }
                data_020ed1bc.setYesNo(1);
            }
            break;
        }
        redraw_ = 1;
        break;
    }
    case 3:
        dataItem_.result_ = 0;
        dataItem_.lastresult_ = 0;
        changeStatus(LOAD_MODESELECT);
        redraw_ = 1;
        break;
    case 1:
        activeDiaryNo_ = dataItem_.active_;
        break;
    case 5:
        dataItem_.active_ = 2;
        activeDiaryNo_ = dataItem_.active_;
        break;
    case 6:
        dataItem_.active_ = 0;
        activeDiaryNo_ = dataItem_.active_;
        break;
    }
}

THUMB void MaterielMenu_LOAD::sexualityUpdate()
{
    sexualityItem_.execInput();
    switch (sexualityItem_.result_) {
    case 2:
        sexualityItem_.result_ = 0;
        sexualityItem_.lastresult_ = 0;
        exitCode_ = activeDiaryNo_;
        data_020ed1bc.openMessageForMENU();
        data_020ed1bc.addMessageNOWAIT(0xcb5f7);
        data_020ed1bc.addMessageWAITKEY();
        changeStatus(LOAD_WRITECHECK);
        redraw_ = 1;
        break;
    case 3:
        sexualityItem_.result_ = 0;
        sexualityItem_.lastresult_ = 0;
        changeStatus(LOAD_NAMEEDIT);
        redraw_ = 1;
        break;
    case 5:
        sexualityItem_.active_ = 1;
        break;
    case 6:
        sexualityItem_.active_ = 0;
        break;
    }
    sexuality_ = sexualityItem_.active_;
}

THUMB bool MaterielMenu_LOAD::updateActiveDiary()
{
    catalogview_ = profile::SaveLoad::getCatalogView();
    int active = profile::SaveLoad::getCatalogRecent();
    profile::CatalogView* view = catalogview_;
    DiaryInfo* diary = diary_;
    int useFlag[3] = { 0, 0, 0 };
    int i;
    int* use = useFlag;
    for (i = 0; i < 3; i++, view++, diary++, use++) {
        *use = view->useFlag_;
        if (*use != 0) {
            diary->name_ = view->name_;
            diary->chapter_ = view->chapter_;
            diary->level_ = view->level_;
            diary->town_ = view->town_;
            diary->time_ = view->time_;
            diary->savetype_ = view->savetype_;
        } else {
            diary->name_ = 0;
            diary->chapter_ = 0;
            diary->level_ = 0;
            diary->town_ = 0;
            diary->time_ = 0;
            diary->savetype_ = 0;
        }
    }
    if (useFlag[active] == 0) {
        active = 0;
        for (int k = 0; k < 3; k++) {
            if (useFlag[k] != 0) {
                active = k;
                break;
            }
        }
    }
    dataItem_.active_ = active;
    return true;
}

THUMB bool MaterielMenu_LOAD::deleteDiary()
{
    return profile::SaveLoad::killbank(activeDiaryNo_);
}

THUMB bool MaterielMenu_LOAD::makeDiary()
{
    status::g_Story.sex_ = (Sex)sexuality_;
    status::Status::setEventParty(0);
    status::g_Game.setPlayTime(0);
    status::g_Game.resetUniqueID();
    g_Global.startFirstTown();
    status::g_Story.setHeroName(gMaterielMenu_NameEdit.getNameUTF8());
    return profile::SaveLoad::savebank(activeDiaryNo_, profile::SAVETYPE_NEW);
}

THUMB void MaterielMenu_LOAD::changeStatus(LOAD_STATUS status)
{
    if (status == LOAD_NAMEEDIT) {
        close();
        gMaterielMenu_NameEdit.open();
        gMaterielMenu_NameEdit.returnMenu_ = MaterielMenu_NameEdit::RETURN_MENU_LOAD;
        if (status_ != LOAD_SEXUALITY) {
            gMaterielMenu_NameEdit.clearName();
        }
    }
    status_ = status;
    messageCounter_ = 0;
}

THUMB void MaterielMenu_LOAD::closeMessage()
{
    int stat = data_020ed1bc.stat_;
    redraw_ = 1;
    if ((unsigned int)(stat - 1) <= 1) {
        data_020ed1bc.close();
    }
}
