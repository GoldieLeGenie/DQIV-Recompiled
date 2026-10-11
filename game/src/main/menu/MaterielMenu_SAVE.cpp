#pragma ipa file
#include "main/menu/MaterielMenu_SAVE.hpp"
#include "main/object/DisplayCharacter.hpp"
#include "main/menu/MaterielMenuWindowManager.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/global/Global.hpp"
#include "main/sound/MenuSoundManager.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/cmn/GameManager.hpp"
#include "main/cmn/PlayerManager.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/cmn/TalkSoundManager.hpp"
#include "main/cmn/NonBattleActionManager.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/StageStatus.hpp"
#include "ov000/town/TownSystem.hpp"
#include "ov026/MaterielMenu_SURECHIGAI/MaterielMenu_SURECHIGAI.hpp"
#include "main/menu/UiMsg.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_02177bac.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"

THUMB void MaterielMenu_SAVE::menuSetup()
{
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    menuItem_.enableSE_ = 1;
    m_focusDiary = 0;
    isSave_ = 0;
    messageCounter_ = 0;
    startSurechigai_ = 0;
    status_ = MENU_IS_SAVE;
    MenuSoundManager::getSingleton()->initialize();
}

THUMB void MaterielMenu_SAVE::menuExecute()
{
    MenuTemplate_materiel::MATERIEL_DIARY_SELECT(&menuItem_);
}

THUMB void MaterielMenu_SAVE::menuDraw()
{
    if (status_ != MENU_SELECT || messageCounter_ == 0) {
        if (saveType_ == TYPE_CHURCH) {
            unkfunc_0216ff18();
        }
        return;
    }
    for (int i = 0; i < 3; i++) {
        int savetype = diary_[i].savetype_;
        char* name = diary_[i].name_;
        if (savetype == 4) {
            unkfunc_0217800c(i, name, -1, -1, -1, diary_[i].time_, 0x5a, name != 0);
        } else {
            unkfunc_0217800c(i, name, diary_[i].chapter_, diary_[i].level_, diary_[i].town_, diary_[i].time_, 0x22, name != 0);
        }
    }
    unkfunc_02177c00(0, 0x10, 0x100, 0x68, 0);
    if (saveType_ == TYPE_CHURCH) {
        unkfunc_0216ff18();
    }
    menuItem_.drawActive();
}

THUMB void MaterielMenu_SAVE::menuUpdate()
{
    if (!messageUpdate()) {
        commandUpdate();
    }
}

THUMB bool MaterielMenu_SAVE::messageUpdate()
{
    if (!data_020ed1bc.isOpen()) {
        return false;
    }
    int stat = data_020ed1bc.stat_;
    switch (status_) {
    case MENU_IS_SAVE:
        if (stat == menu::MenuBase::MENUBASE_STAT_OK) {
            data_020ed1bc.close();
            setMode(MENU_READING);
        } else if (stat == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            firstCancelMessage();
        }
        break;
    case MENU_SELECT:
        if (messageCounter_ == 0) {
            switch (saveType_) {
            case TYPE_CHURCH:
            case TYPE_LASTDUNGEON:
            case TYPE_SURECHIGAI:
                data_020ed1bc.setMessageLastCursor(false);
                messageCounter_++;
                redraw_ = 1;
                break;
            case TYPE_CHAPTER:
            case TYPE_CLEAR:
                messageCounter_++;
                redraw_ = 1;
                break;
            }
        } else {
            return false;
        }
        break;
    case MENU_IS_OVERWRITE:
        if (stat == menu::MenuBase::MENUBASE_STAT_OK) {
            data_020ed1bc.close();
            setMode(MENU_WRITING);
        }
        if (stat == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            setMode(MENU_SELECT);
        }
        break;
    case MENU_WRITING:
        if (messageCounter_ == 1) {
            return false;
        }
        break;
    case MENU_WRITINGEXEC:
        return false;
    case MENU_WRITINGWAIT:
        return false;
    case MENU_SOUNDWAIT:
        return false;
    case MENU_IS_END:
        if (messageCounter_ == 0) {
            if (stat == menu::MenuBase::MENUBASE_STAT_OK) {
                data_020ed1bc.close();
                switch (saveType_) {
                case TYPE_CHURCH:
                    setTalkMessage(0xc700b);
                    break;
                case TYPE_CHAPTER:
                case TYPE_LASTDUNGEON:
                case TYPE_CLEAR:
                case TYPE_SURECHIGAI:
                    break;
                }
                setMode(MENU_END);
            }
            if (stat == menu::MenuBase::MENUBASE_STAT_CANCEL) {
                data_020ed1bc.close();
                switch (saveType_) {
                case TYPE_CHURCH:
                    if (isSave_ != 0) {
                        setTalkMessage(0xc6fbb);
                        setMode(MENU_GAME_END);
                    } else {
                        setTalkYesNoMessage2(0xc6fb7, 0xc6fb8, false);
                        messageCounter_++;
                    }
                    break;
                case TYPE_CHAPTER:
                    setMode(MENU_IS_SAVE);
                    break;
                case TYPE_LASTDUNGEON:
                    break;
                case TYPE_CLEAR:
                    setMode(MENU_IS_SAVE);
                    break;
                case TYPE_SURECHIGAI:
                    setMode(MENU_READING);
                    break;
                }
            }
        } else {
            if (stat == menu::MenuBase::MENUBASE_STAT_OK) {
                data_020ed1bc.close();
                setTalkMessage(0xc6fbb);
                setMode(MENU_GAME_END);
            }
            if (stat == menu::MenuBase::MENUBASE_STAT_CANCEL) {
                data_020ed1bc.close();
                setMode(MENU_READING);
            }
        }
        break;
    case MENU_GAME_END:
        if (messageCounter_ == 0 && (unsigned int)(stat - 1) <= 1) {
            data_020ed1bc.close();
        }
        break;
    case MENU_BLANK:
        if ((unsigned int)(stat - 1) <= 1) {
            data_020ed1bc.close();
        }
        return false;
    case MENU_READING:
    case MENU_FAILED:
    case MENU_SUCCESS:
    case MENU_END:
        if ((unsigned int)(stat - 1) <= 1) {
            data_020ed1bc.close();
        }
        break;
    case MENU_SURECHIGAI_START:
        if (stat == menu::MenuBase::MENUBASE_STAT_OK) {
            data_020ed1bc.close();
            setTalkMessage(0x929ff);
            setMode(MENU_END);
            MaterielMenu_WINDOW_MANAGER::getSingleton()->surechigaiStart_ = 1;
            startSurechigai_ = 1;
        }
        if (stat == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            setTalkMessage(0x92a2f);
            setMode(MENU_END);
            MaterielMenu_WINDOW_MANAGER::getSingleton()->surechigaiStart_ = 0;
        }
        break;
    }
    return true;
}

THUMB void MaterielMenu_SAVE::commandUpdate()
{
    int playSound = MenuSoundManager::getSingleton()->isPlaySound();
    waitFrame_++;
    switch (status_) {
    case MENU_IS_SAVE:
        if (messageCounter_ == 0) {
            if (!profile::SaveLoad::isCardOK()) {
                setMode(MENU_BLANK);
                switch (saveType_) {
                case TYPE_CHURCH:
                    setWaitMessage(0xc7019, false);
                    break;
                case TYPE_CHAPTER:
                case TYPE_LASTDUNGEON:
                case TYPE_CLEAR:
                    setWaitMessage(0xcba0e, true);
                    break;
                case TYPE_SURECHIGAI:
                    setTalkWaitMessage(0x92a14);
                    break;
                }
                return;
            }
            switch (saveType_) {
            case TYPE_CHURCH: {
                int sex;
                param::MapChurch* church = status::excelParam.mapChurch_;
                char motherMapName[] = "ms";
                sex = 0;
                mother_ = 0;
                unsigned int i = 0;
                unsigned int count = param::MapChurch::size_;
                for (; i < count; i++) {
                    if (church[i].floor[0] == g_Global.getMapName()[0] &&
                        church[i].floor[1] == g_Global.getMapName()[1] &&
                        church[i].floor[2] == g_Global.getMapName()[2]) {
                        if (motherMapName[0] == g_Global.getMapName()[0] && motherMapName[1] == g_Global.getMapName()[1]) {
                            mother_ = 1;
                        }
                        sex = (char)(church[i].byte_1 & 1);
                        break;
                    }
                }
                sexType_ = sex ? 1000 : 0;
                setTalkYesNoMessage2(0xc6fa2, 0xc6fa3, true);
                break;
            }
            case TYPE_CHAPTER:
                setYesNoMessage(0xcb9d3, true);
                break;
            case TYPE_LASTDUNGEON:
                ui_MsgSndSet(0x31);
                setTalkYesNoMessage(0xcb9e7, true);
                break;
            case TYPE_CLEAR:
                setYesNoMessage(0xcb9fb, true);
                break;
            case TYPE_SURECHIGAI:
                setTalkYesNoMessage2(0x929e8, 0x929e9, true);
                break;
            }
        }
        break;
    case MENU_READING:
        if (getSaveData()) {
            setMode(MENU_SELECT);
            redraw_ = 1;
            int active = g_Stage.profileBank_;
            if (active == 3) {
                active = 0;
            }
            menuItem_.active_ = active;
        } else {
            setMode(MENU_BLANK);
            switch (saveType_) {
            case TYPE_CHURCH:
                setWaitMessage(0xc7019, false);
                break;
            case TYPE_CHAPTER:
            case TYPE_LASTDUNGEON:
            case TYPE_CLEAR:
                setWaitMessage(0xcba0e, true);
                break;
            case TYPE_SURECHIGAI:
                setTalkWaitMessage(0x92a14);
                break;
            }
        }
        break;
    case MENU_BLANK:
        if (messageCounter_ == 0) {
            if (data_020ed1bc.isOpen()) {
                data_020ed1bc.isMessageWAITPROG();
            }
        }
        break;
    case MENU_SELECT:
        if (messageCounter_ == 0) {
            switch (saveType_) {
            case TYPE_CHURCH:
                setTalkWaitMessage(0xc6fa7);
                break;
            case TYPE_LASTDUNGEON:
                setTalkWaitMessage(0xcb9ea);
                break;
            case TYPE_CHAPTER:
            case TYPE_CLEAR:
                messageCounter_++;
                redraw_ = 1;
                break;
            case TYPE_SURECHIGAI:
                setTalkWaitMessage(0x929ec);
                break;
            }
        } else {
            if (saveType_ == TYPE_CLEAR || saveType_ == TYPE_CHAPTER) {
                dataSelectUpdate();
            } else if (data_020ed1bc.isOpen() && data_020ed1bc.isMessageWAITPROG()) {
                dataSelectUpdate();
            }
        }
        break;
    case MENU_IS_OVERWRITE:
        profile::SaveLoad::setCatalogMacro(&catalogview_[menuItem_.active_]);
        switch (saveType_) {
        case TYPE_CHURCH:
            setTalkYesNoMessage(0xc6faa, true);
            break;
        case TYPE_CHAPTER:
            setYesNoMessage(0xcb9da, true);
            break;
        case TYPE_LASTDUNGEON:
            setTalkYesNoMessage(0xcb9ed, true);
            break;
        case TYPE_CLEAR:
            setYesNoMessage(0xcba02, true);
            break;
        case TYPE_SURECHIGAI:
            setTalkYesNoMessage(0x929ef, true);
            break;
        }
        break;
    case MENU_WRITING:
        if (messageCounter_ == 0) {
            waitFrame_ = 999;
            switch (saveType_) {
            case TYPE_CHURCH:
                MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_SAVE);
                setWaitMessage(0xc7016, true);
                waitFrame_ = 0;
                break;
            case TYPE_CHAPTER:
                setWaitMessage(0xcb9dd, true);
                break;
            case TYPE_LASTDUNGEON:
                MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_SAVE);
                setWaitMessage(0xcb9f0, true);
                waitFrame_ = 0;
                break;
            case TYPE_CLEAR:
                setWaitMessage(0xcba05, true);
                break;
            case TYPE_SURECHIGAI:
                setWaitMessage(0x929f2, true);
                break;
            }
            messageCounter_++;
        }
        data_020ed1bc.SetNoClose(true);
        setMode(MENU_WRITINGEXEC);
        break;
    case MENU_WRITINGEXEC:
        if (data_020ed1bc.isOpen() && data_020ed1bc.isMessageWAITPROG()) {
            saveResult_ = 0;
            dss::Fix32Vector3 pos;
            switch (saveType_) {
            case TYPE_CHURCH:
                g_Stage.setChurchMapName(g_Stage.getMapName());
                profile::SaveLoad::savebankAsync(menuItem_.active_, profile::SAVETYPE_CHURCH);
                break;
            case TYPE_CLEAR:
                g_Stage.lastFldSurface_ = -1;
                g_Stage.setRanaMapName("");
            case TYPE_LASTDUNGEON:
                g_Stage.setChurchMapName("hck1f1");
                g_cmnPartyInfo.setBalloonPosByExtraSave();
                pos.vx = dss::Fix32(-1.65f);
                pos.vy = dss::Fix32(0.0f);
                pos.vz = dss::Fix32(0.65f);
                g_Stage.overviewPosition_ = pos;
                g_Stage.overviewTempPosition_ = pos;
                profile::SaveLoad::savebankAsync(menuItem_.active_, profile::SAVETYPE_CHURCH);
                g_cmnPartyInfo.resetBalloonPosByExtraSave();
                break;
            case TYPE_SURECHIGAI:
                g_Stage.setChurchMapName("sshout");
                pos.vx = dss::Fix32(1.9f);
                pos.vy = dss::Fix32(0.0f);
                pos.vz = dss::Fix32(1.18f);
                g_Stage.overviewPosition_ = pos;
                g_Stage.overviewTempPosition_ = pos;
                profile::SaveLoad::savebankAsync(menuItem_.active_, profile::SAVETYPE_CHURCH);
                break;
            case TYPE_CHAPTER:
                g_Stage.lastFldSurface_ = -1;
                g_Stage.setRanaMapName("");
                profile::SaveLoad::savebankAsync(menuItem_.active_, profile::SAVETYPE_INTERVAL);
                break;
            }
            setMode(MENU_WRITINGWAIT);
        }
        break;
    case MENU_WRITINGWAIT:
        if (profile::SaveLoad::savebankAsyncWait()) {
            saveResult_ = profile::SaveLoad::savebankAsyncResult();
            setMode(MENU_SOUNDWAIT);
        }
        break;
    case MENU_SOUNDWAIT:
        if (!playSound) {
            data_020ed1bc.SetNoClose(false);
            if (saveResult_ == 1) {
                setMode(MENU_SUCCESS);
                data_020ed1bc.clearMessageWAITPROG();
                data_020ed1bc.close();
            } else if (saveResult_ == 0) {
                setMode(MENU_FAILED);
                data_020ed1bc.clearMessageWAITPROG();
                data_020ed1bc.close();
            }
        }
        break;
    case MENU_FAILED:
        data_020ed1bc.openMessageForMENU();
        switch (saveType_) {
        case TYPE_CHURCH:
            setWaitMessage(0xc6fad, false);
            setMode(MENU_BLANK);
            break;
        case TYPE_CHAPTER:
            setWaitMessage(0xcb9e3, true);
            setMode(MENU_BLANK);
            break;
        case TYPE_LASTDUNGEON:
            setTalkWaitMessage(0xcb9f7);
            setMode(MENU_BLANK);
            break;
        case TYPE_CLEAR:
            setWaitMessage(0xcba0b, true);
            setMode(MENU_BLANK);
            break;
        case TYPE_SURECHIGAI:
            setTalkWaitMessage(0x929f5);
            setMode(MENU_BLANK);
            break;
        }
        break;
    case MENU_SUCCESS:
        data_020ed1bc.openMessageForMENU();
        switch (saveType_) {
        case TYPE_CHURCH:
            setTalkYesNoMessage(0xc6fb0, true);
            setMode(MENU_IS_END);
            isSave_ = 1;
            break;
        case TYPE_CHAPTER:
            TextAPI::setMACRO0(0x42, 0xf0000000, menuItem_.active_ + 1);
            setMessage(0xcb9e0);
            setMode(MENU_END);
            break;
        case TYPE_LASTDUNGEON:
            setTalkMessage(0xcb9f3, 0xcb9f4);
            setMode(MENU_END);
            break;
        case TYPE_CLEAR:
            TextAPI::setMACRO0(0x42, 0xf0000000, menuItem_.active_ + 1);
            setMessage(0xcba08);
            setMode(MENU_END);
            break;
        case TYPE_SURECHIGAI:
            setTalkYesNoMessage(0x929f8, true);
            setMode(MENU_SURECHIGAI_START);
            break;
        }
        break;
    case MENU_GAME_END:
        if (messageCounter_ == 0) {
            data_020f21f8.state_ = GlobalFade::FADE_NONE;
            data_020f21f8.count_ = 0;
            data_020f21f8.frames_ = 120;
            data_020f21f8.sprite_[0].setColor(0, 0, 0);
            data_020f21f8.sprite_[1].setColor(0, 0, 0);
            data_0210bc18.unkfunc_02058294(&data_020f21f8);
            messageCounter_++;
            TownSystem::getSingleton()->fadeCount_ = 0;
            g_Global.bookingFlag_ = Global::BOOKING_GAMESET;
            SoundManager::stopBgm(120);
            cmn::GameManager::getSingleton();
            cmn::PlayerManager::setLock(1);
            cmn::GameManager::getSingleton()->playerManager_->charaColl_ = 0;
            BillboardCharacter::setAllCharaAnim(0);
            MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        } else if (messageCounter_ == 1) {
            if (!g_GlobalFade.isEnd()) {
                ui_MsgSndSet(0x30);
                setMessage(0xc6fbc);
                messageCounter_++;
            }
        }
        break;
    case MENU_END:
        close();
        switch (saveType_) {
        case TYPE_SURECHIGAI:
            if (startSurechigai_ == 0) {
                gMaterielMenu_SURECHIGAI_ROOT.open();
                return;
            }
            break;
        case TYPE_CHURCH:
        case TYPE_CHAPTER:
        case TYPE_LASTDUNGEON:
        case TYPE_CLEAR:
            break;
        }
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        break;
    }
}

THUMB bool MaterielMenu_SAVE::getSaveData()
{
    if (!profile::SaveLoad::isCardOK()) {
        return false;
    }
    catalogview_ = profile::SaveLoad::getCatalogView();
    profile::CatalogView* view = catalogview_;
    DiaryInfo* diary = diary_;
    for (int i = 0; i < 3; i++, view++, diary++) {
        if (view->useFlag_ != 0) {
            diary->name_ = view->name_;
            diary->chapter_ = view->chapter_;
            diary->level_ = view->level_;
            diary->town_ = view->town_;
            diary->time_ = view->time_;
        } else {
            diary->name_ = 0;
            diary->chapter_ = 0;
            diary->level_ = 0;
            diary->town_ = 0;
            diary->time_ = 0;
        }
    }
    return true;
}

THUMB void MaterielMenu_SAVE::setMode(SAVEMENU_MODE mode)
{
    status_ = mode;
    messageCounter_ = 0;
}

THUMB void MaterielMenu_SAVE::dataSelectUpdate()
{
    menuItem_.execInput();
    switch (menuItem_.result_) {
    case 2:
        menuItem_.result_ = 0;
        menuItem_.lastresult_ = 0;
        if (diary_[m_focusDiary].name_ == 0) {
            setMode(MENU_WRITING);
        } else {
            setMode(MENU_IS_OVERWRITE);
        }
        if (data_020ed1bc.isOpen()) {
            data_020ed1bc.close();
        }
        redraw_ = 1;
        break;
    case 3:
        menuItem_.result_ = 0;
        menuItem_.lastresult_ = 0;
        if (data_020ed1bc.isOpen()) {
            data_020ed1bc.close();
        }
        firstCancelMessage();
        redraw_ = 1;
        break;
    case 5:
        menuItem_.active_ = 2;
        break;
    case 6:
        menuItem_.active_ = 0;
        break;
    }
    m_focusDiary = menuItem_.active_;
}

THUMB void MaterielMenu_SAVE::setMessage(int messageID)
{
    if (saveType_ == TYPE_CHURCH) {
        messageID += sexType_;
    }
    data_020ed1bc.openMessageForMENU();
    data_020ed1bc.addMessage(messageID);
}

THUMB void MaterielMenu_SAVE::setWaitMessage(int messageID, bool sex)
{
    if (saveType_ == TYPE_CHURCH && sex) {
        messageID += sexType_;
    }
    data_020ed1bc.openMessageForMENU();
    data_020ed1bc.addMessageNOWAIT(messageID);
    data_020ed1bc.addMessageWAITKEY();
}

THUMB void MaterielMenu_SAVE::setYesNoMessage(int messageID, bool yes)
{
    if (saveType_ == TYPE_CHURCH) {
        messageID += sexType_;
    }
    data_020ed1bc.openMessageForMENU();
    data_020ed1bc.addMessage(messageID);
    data_020ed1bc.setYesNo(yes ? 0 : 1);
}

THUMB void MaterielMenu_SAVE::setTalkMessage(int messageID)
{
    if (saveType_ == TYPE_CHURCH) {
        messageID += sexType_;
        int sound = 0x32;
        if (sexType_ == 1000 && mother_ == 0) {
            sound = 0x31;
        }
        ui_MsgSndSet(sound);
    } else if (saveType_ == TYPE_LASTDUNGEON) {
        ui_MsgSndSet(0x31);
    }
    data_020ed1bc.openMessageForTALK();
    data_020ed1bc.addMessage(messageID);
}

THUMB void MaterielMenu_SAVE::setTalkMessage(int messageID1, int messageID2)
{
    if (saveType_ == TYPE_CHURCH) {
        messageID1 += sexType_;
        messageID2 += sexType_;
        int sound = 0x32;
        if (sexType_ == 1000 && mother_ == 0) {
            sound = 0x31;
        }
        ui_MsgSndSet(sound);
    } else if (saveType_ == TYPE_LASTDUNGEON) {
        ui_MsgSndSet(0x31);
    }
    data_020ed1bc.openMessageForTALK();
    data_020ed1bc.addMessage(messageID1, messageID2);
}

THUMB void MaterielMenu_SAVE::setTalkWaitMessage(int messageID)
{
    if (saveType_ == TYPE_CHURCH) {
        messageID += sexType_;
        int sound = 0x32;
        if (sexType_ == 1000 && mother_ == 0) {
            sound = 0x31;
        }
        ui_MsgSndSet(sound);
    } else if (saveType_ == TYPE_LASTDUNGEON) {
        ui_MsgSndSet(0x31);
    }
    data_020ed1bc.openMessageForTALK();
    data_020ed1bc.addMessageNOWAIT(messageID);
    data_020ed1bc.addMessageWAITKEY();
}

THUMB void MaterielMenu_SAVE::setTalkYesNoMessage(int messageID, bool yes)
{
    if (saveType_ == TYPE_CHURCH) {
        messageID += sexType_;
        int sound = 0x32;
        if (sexType_ == 1000 && mother_ == 0) {
            sound = 0x31;
        }
        ui_MsgSndSet(sound);
    } else if (saveType_ == TYPE_LASTDUNGEON) {
        ui_MsgSndSet(0x31);
    }
    data_020ed1bc.openMessageForTALK();
    data_020ed1bc.addMessage(messageID);
    data_020ed1bc.setYesNo(yes ? 0 : 1);
}

THUMB void MaterielMenu_SAVE::setTalkYesNoMessage2(int messageID1, int messageID2, bool yes)
{
    if (saveType_ == TYPE_CHURCH) {
        messageID1 += sexType_;
        messageID2 += sexType_;
        int sound = 0x32;
        if (sexType_ == 1000 && mother_ == 0) {
            sound = 0x31;
        }
        ui_MsgSndSet(sound);
    } else if (saveType_ == TYPE_LASTDUNGEON) {
        ui_MsgSndSet(0x31);
    }
    int cursor = yes ? 0 : 1;
    data_020ed1bc.openMessageForTALK();
    data_020ed1bc.addMessage(messageID1, messageID2);
    data_020ed1bc.setYesNo(cursor);
}

THUMB void MaterielMenu_SAVE::firstCancelMessage()
{
    switch (saveType_) {
    case TYPE_CHURCH:
        setTalkYesNoMessage2(0xc6fb3, 0xc6fb4, true);
        setMode(MENU_IS_END);
        break;
    case TYPE_CHAPTER:
        setYesNoMessage(0xcb9d6, false);
        setMode(MENU_IS_END);
        break;
    case TYPE_LASTDUNGEON:
        setTalkMessage(0xcb9f4);
        setMode(MENU_END);
        break;
    case TYPE_CLEAR:
        setYesNoMessage(0xcb9fe, false);
        setMode(MENU_IS_END);
        break;
    case TYPE_SURECHIGAI:
        setTalkYesNoMessage2(0x929fb, 0x929fc, false);
        setMode(MENU_IS_END);
        break;
    }
}
