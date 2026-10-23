#include "ov016/TownMenu_OPERATION/TownMenu_OPERATION_ROOT.hpp"
#include "ov016/TownMenu_OPERATION/TownMenu_OPERATION_CAREER.hpp"
#include "ov016/TownMenu_OPERATION/TownMenu_OPERATION_EQUIP.hpp"
#include "ov016/TownMenu_OPERATION/TownMenu_OPERATION_SETTING.hpp"
#ifdef DQIV_NATIVE
#include "ov016/TownMenu_OPERATION/TownMenu_OPERATION_ACHIEVEMENTS.hpp"
#endif
#include "ov016/TownMenu_OPERATION/TownMenu_OPERATION_TACTICS.hpp"
#include "ov016/TownMenu_OPERATION/TownMenu_OPERATION_SHIFT_PARTY.hpp"
#include "ov016/TownMenu_ROOT/TownMenu_ROOT.hpp"
#include "ov016/TownMenuPlayerControl/TownMenuPlayerControl.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/CatalogView.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/OptionStatus.hpp"
#include "main/status/GameFlag.hpp"
#include "main/status/UseAction.hpp"
#include "main/text/TextAPI.hpp"
#include "ov016/MenuTemplate/MenuTemplate_town.hpp"
#include "ov016/UnkTownMenuDraw/TownMenuScreens.hpp"

THUMB void TownMenu_OPERATION_ROOT::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    sortItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    charaItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    mode_ = 0;
    abort_ = 0;
    sortDone_ = 0;
    saveResult_ = 0;
    isMantan_ = 0;
    useKiari_ = 0;
    saveCount_ = 0;
    seCount_ = 0;
    navigator_.setupBase();
    charaNavigator_.setupBase();
    commandCount_ = setupCommandList(command_);
    menuItem_.active_ = TownMenuPlayerControl::getSingleton()->activeTactics_;
}

THUMB void TownMenu_OPERATION_ROOT::menuExecute()
{
    if (g_TownMenuMessage.isOpen()) {
        return;
    }
    MenuTemplate_town::TOWN_CANCEL(&cancelItem_);
    if (mode_ == 1) {
        MenuTemplate_town::townMenuTacticsBoxUp(&sortItem_, sortItem_.active_);
        return;
    }
    if (mode_ == 2) {
        if (status::g_Party.getCount() < 5) {
            MenuTemplate_town::townMenuItemSelectHalfChara(&charaItem_, status::g_Party.getCount() + 1, charaItem_.active_);
        } else {
            MenuTemplate_town::townMenuTacticsSelectChara(&charaItem_, status::g_Party.getCount() + 1, charaItem_.active_, -1, -1);
        }
    }
    MenuTemplate_town::TOWN_OPERATION(&menuItem_, menuItem_.active_, commandCount_);
}

THUMB void TownMenu_OPERATION_ROOT::menuDraw()
{
    if (charaItem_.getActive() == status::g_Party.getCount()) {
        drawOperationRootScreen(mode_, command_, commandCount_, -2);
    } else {
        drawOperationRootScreen(mode_, command_, commandCount_, charaItem_.getActive());
    }
    if (g_TownMenuMessage.isOpen()) {
        return;
    }
    cancelItem_.drawActive();
    if (mode_ == 1) {
        sortItem_.drawActive();
        return;
    }
    if (mode_ == 2) {
        charaItem_.drawActive();
        return;
    }
    menuItem_.drawActive();
}

THUMB void TownMenu_OPERATION_ROOT::menuUpdate()
{
    if (seCount_ > 30 && useKiari_ == 0) {
        SoundManager::playSe(0x1f5, 0);
        seCount_ = 0;
        if (actionIndex_ != 0xc7) {
            isMantan_ = 0;
        }
    }
    if (g_TownMenuMessage.isOpen()) {
        int stat = g_TownMenuMessage.stat_;
        if (s_draw->isFinish()) {
            isMantan_ = 0;
            if (mode_ == 3) {
                status::UseActionParam useActionParam;
                useActionParam.clear();
                if (status::g_Party.isRecoveryForMantan()) {
                    if (status::g_Party.recoveryForMantan(useActionParam)) {
                        g_TownMenuMessage.restartMessage();
                        allRecoveryMessage(useActionParam);
                        redraw_ = 1;
                        isMantan_ = 1;
                        useKiari_ = 0;
                        seCount_ = 0;
                        return;
                    }
                } else if (status::g_Party.isPoisonForMantan()) {
                    if (status::g_Party.destroyPoisonForMantan(useActionParam)) {
                        g_TownMenuMessage.restartMessage();
                        allRecoveryMessage(useActionParam);
                        useKiari_ = 1;
                        redraw_ = 1;
                    } else {
                        mode_ = 0;
                    }
                    isMantan_ = 1;
                    seCount_ = 0;
                } else {
                    mode_ = 0;
                }
                status::g_Party.setPlayerMode();
                return;
            }
        }
        if (isMantan_ == 1) {
            seCount_++;
        }
        if (g_TownMenuMessage.isMessageWAITPROG()) {
            if (abort_ == 2) {
                saveResult_ = profile::SaveLoad::savebank(3, profile::SAVETYPE_RESUME);
                abort_ = 3;
            }
            updateAbortSaveMessage();
            return;
        }
        if (stat == menu::MenuBase::MENUBASE_STAT_OK) {
            if (mode_ == 8) {
                updateAbortSaveMessage();
                return;
            }
            g_TownMenuMessage.close();
            if (mode_ == 1 || mode_ == 2) {
                if (sortDone_ == 1) {
                    menuItem_.ctype_ = menu::MenuItem::CURSORTYPE_ACTIVE;
                    menuItem_.drawActive();
                    mode_ = 0;
                    sortDone_ = 0;
                    return;
                }
                sortItems();
                return;
            }
        } else if (stat == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            g_TownMenuMessage.close();
            if (mode_ != 1) {
                mode_ = 0;
                menuItem_.ctype_ = menu::MenuItem::CURSORTYPE_ACTIVE;
                menuItem_.drawActive();
                redraw_ = 1;
            }
        }
        return;
    }
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        if (mode_ == 1 || mode_ == 2) {
            mode_ = 0;
            menuItem_.ctype_ = menu::MenuItem::CURSORTYPE_ACTIVE;
            menuItem_.drawActive();
            redraw_ = 1;
            return;
        }
        close();
        gTownMenu_ROOT.open();
        gTownMenu_ROOT.menuItem_.active_ = TownMenu_ROOT::ROOT_OPERATION;
        return;
    }
    navigator_.setup(2, 5, commandCount_);
    sortNavigator_.setup(2, 1, 2);
    charaNavigator_.setup(5, 2, status::g_Party.getCount() + 1);
    if (mode_ == 1) {
        int result = MenuUpdate_Assist::menuSelect(sortItem_, sortNavigator_);
        if (result == 0) {
            return;
        }
        if (result == 2) {
            askSortSack();
        }
        redraw_ = 1;
        return;
    }
    if (mode_ == 2) {
        int result = MenuUpdate_Assist::menuSelect(charaItem_, charaNavigator_);
        if (result == 0) {
            return;
        }
        if (result == 2) {
            askSortCharaItems();
        }
        redraw_ = 1;
        return;
    }
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result == 0) {
        return;
    }
    if (result == 2) {
        TownMenuPlayerControl::getSingleton()->setActiveTactics(menuItem_.active_);
        switch (command_[menuItem_.active_]) {
        case 0:
            useFullRecovery();
            break;
        case 1:
            close();
            gTownMenu_OPERATION_EQUIP.open();
            break;
        case 2:
            openShiftParty();
            break;
        case 3:
            if (status::g_Party.getCount() > 2 ||
                (int)status::g_Party.getPlayerStatus(0)->haveStatusInfo_.haveStatus_.playerIndex_ > 2 ||
                (int)status::g_Party.getPlayerStatus(1)->haveStatusInfo_.haveStatus_.playerIndex_ > 2) {
                close();
                gTownMenu_OPERATION_TACTICS.open();
            }
            break;
        case 4:
            mode_ = 2;
            break;
        case 5:
            mode_ = 1;
            break;
        case 6:
            close();
            gTownMenu_OPERATION_CAREER.open();
            break;
        case 7:
            close();
            gTownMenu_OPERATION_SETTING.open();
            break;
#ifdef DQIV_NATIVE
        case COMMAND_ACHIEVEMENTS:
            close();
            gTownMenu_OPERATION_ACHIEVEMENTS.open();
            break;
#endif
        case 8:
            mode_ = 8;
            g_TownMenuMessage.openMessageForMENU();
            if (!g_Stage.isAbortSaveDungeon()) {
                g_TownMenuMessage.addMessage(0xcba22, 0xcba23);
            } else if (!g_Stage.isAbortSaveTown()) {
                g_TownMenuMessage.addMessage(0xcba25, 0xcba26, 0xcba2a);
            } else {
                g_TownMenuMessage.addMessage(0xcba12);
                g_TownMenuMessage.setYesNo();
                g_TownMenuMessage.setYesNoSuperCancel(false);
                abort_ = 2;
            }
            break;
        }
    }
    redraw_ = 1;
}

THUMB void TownMenu_OPERATION_ROOT::askSortCharaItems()
{
    g_TownMenuMessage.openMessageForMENU();
    g_TownMenuMessage.addMessage(0xc3d5b);
    g_TownMenuMessage.setYesNo();
    menuItem_.ctype_ = menu::MenuItem::CURSORTYPE_NONE;
}

THUMB void TownMenu_OPERATION_ROOT::askSortSack()
{
    if (status::g_Party.haveItemSack_.getItem(0) != 0) {
        g_TownMenuMessage.openMessageForMENU();
        if (sortItem_.active_ == 0) {
            g_Option.setSackSort(0);
            g_TownMenuMessage.addMessage(0xc3d60);
        } else {
            g_Option.setSackSort(1);
            g_TownMenuMessage.addMessage(0xc3d65);
        }
        g_TownMenuMessage.setYesNo();
        menuItem_.ctype_ = menu::MenuItem::CURSORTYPE_NONE;
        return;
    }
    g_TownMenuMessage.openMessageForMENU();
    g_TownMenuMessage.addMessage(0xc3d68);
}

THUMB void TownMenu_OPERATION_ROOT::useFullRecovery()
{
    status::g_Party.setBattleMode();
    int noRecovery = 1;
    status::UseActionParam useActionParam;
    useActionParam.clear();
    g_TownMenuMessage.openMessageForMENU();
    if (status::g_Party.isRecoveryForMantan()) {
        if (status::g_Party.recoveryForMantan(useActionParam)) {
            allRecoveryMessage(useActionParam);
            noRecovery = 0;
        }
        mode_ = 3;
        isMantan_ = 1;
    } else if (status::g_Party.isPoisonForMantan()) {
        if (status::g_Party.destroyPoisonForMantan(useActionParam)) {
            allRecoveryMessage(useActionParam);
            noRecovery = 0;
        }
        mode_ = 3;
        isMantan_ = 1;
    }
    if (noRecovery) {
        isMantan_ = 0;
        g_TownMenuMessage.addMessage(0xc3d6b);
    }
    status::g_Party.setPlayerMode();
}

THUMB void TownMenu_OPERATION_ROOT::openShiftParty()
{
    status::g_Party.setMemberShiftMode();
    if (status::g_Party.getCount() == 1 && !status::g_Party.getCarriageEnableOnGame()) {
        menuItem_.result_ = 0;
        menuItem_.lastresult_ = 0;
        g_TownMenuMessage.openMessageForMENU();
        g_TownMenuMessage.addMessage(0xc3d59);
        status::g_Party.setPlayerMode();
        return;
    }
    close();
    gTownMenu_OPERATION_SHIFT_PARTY.open();
}

THUMB void TownMenu_OPERATION_ROOT::sortItems()
{
    int count = status::g_Party.getCount();
    if (mode_ == 1) {
        if (status::g_Party.haveItemSack_.getItem(0) == 0) {
            return;
        }
        status::g_Party.haveItemSack_.sortOutSack((status::HaveItemSack::SortType)g_Option.getSackSort());
        g_TownMenuMessage.openMessageForMENU();
        g_TownMenuMessage.addMessage(0xc3d63);
    } else {
        if (count == charaItem_.active_) {
            for (int i = 0; i < count; i++) {
                status::g_Party.haveItemSack_.sortOutItem(&status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveItem_);
            }
        } else {
            status::g_Party.haveItemSack_.sortOutItem(&status::g_Party.getPlayerStatus(charaItem_.active_)->haveStatusInfo_.haveItem_);
        }
        g_TownMenuMessage.openMessageForMENU();
        g_TownMenuMessage.addMessage(0xc3d5e);
    }
    sortDone_ = 1;
}

THUMB int TownMenu_OPERATION_ROOT::setupCommandList(char* command)
{
    if (status::g_Story.chapter_ >= 5 && g_AreaFlag.check(0x132) == true) {
        for (char i = 0; i < 9; i++) {
            command[i] = i;
        }
#ifdef DQIV_NATIVE
        command[9] = COMMAND_ACHIEVEMENTS;
        return 10;
#endif
        return 9;
    }
    char normal[8] = { 0, 1, 2, 4, 5, 6, 8, 7 };
    for (char i = 0; i < 8; i++) {
        command[i] = normal[i];
    }
#ifdef DQIV_NATIVE
    // The PC version's achievements, after the settings.
    command[8] = COMMAND_ACHIEVEMENTS;
    command[9] = -1;
    return 9;
#endif
    command[8] = -1;
    return 8;
}

THUMB void TownMenu_OPERATION_ROOT::allRecoveryMessage(status::UseActionParam& useActionParam)
{
    status::g_Party.setBattleMode();
    int message[4];
    int messageCount;
    int spell;
    int targetCount;
    status::UseActionMessage* useActionMessage;
    for (int i = 0; i < 4; i++) {
        message[i] = 0;
    }
    useActionMessage = &useActionParam.message_[0];
    spell = useActionParam.actionIndex_;
    targetCount = useActionParam.targetCount_;
    TextAPI::setMACRO0(1, 0x50000000, useActionParam.actorCharacterStatus_->haveStatusInfo_.haveStatus_.playerIndex_);
    TextAPI::setMACRO0(0x11, 0x70000000, status::UseAction::getWordDBIndex(spell));
    messageCount = 0;
    message[messageCount] = useActionMessage->execMessage_[messageCount];
    while (message[messageCount] != 0) {
        g_TownMenuMessage.addMessageNOWAIT(message[messageCount]);
        messageCount++;
        message[messageCount] = useActionMessage->execMessage_[messageCount];
        SoundManager::playSe(0x132, 0);
    }
    for (int i = 0; i < targetCount; i++) {
        TextAPI::setMACRO0(0x12, 0x50000000, useActionParam.targetCharacterStatus_[i]->haveStatusInfo_.haveStatus_.playerIndex_);
        messageCount = 0;
        message[messageCount] = useActionMessage->resultMessage_[messageCount];
        while (message[messageCount] != 0) {
            if (!status::g_Party.isRecoveryForMantan() && !status::g_Party.isRecoveryForMantan()) {
                if (i != targetCount) {
                    g_TownMenuMessage.addMessageNOWAIT(message[messageCount]);
                } else {
                    g_TownMenuMessage.addMessage(message[messageCount]);
                }
            } else {
                g_TownMenuMessage.addMessageNOWAIT(message[messageCount]);
            }
            messageCount++;
            message[messageCount] = useActionMessage->resultMessage_[messageCount];
        }
    }
    actionIndex_ = useActionParam.actionIndex_;
}

THUMB void TownMenu_OPERATION_ROOT::updateAbortSaveMessage()
{
    switch (abort_) {
    case 0:
        g_TownMenuMessage.close();
        break;
    case 1:
        break;
    case 2:
        g_TownMenuMessage.close();
        g_TownMenuMessage.openMessageForMENU();
        g_TownMenuMessage.addMessageNOWAIT(0xcba15);
        g_TownMenuMessage.addMessageWAITKEY();
        break;
    case 3:
        g_TownMenuMessage.close();
        g_TownMenuMessage.openMessageForMENU();
        if (saveResult_ == 0) {
            g_TownMenuMessage.addMessage(0xcba1c);
            abort_ = 1;
        } else {
#ifdef DQIV_NATIVE
            // The PC version keeps playing after a quick save (the DS asks to
            // switch the console off).
            g_TownMenuMessage.addMessage(0xcba18);
            abort_ = 4;
#else
            g_TownMenuMessage.addMessage(0xcba18, 0xcba19);
            abort_ = 1;
#endif
        }
        break;
#ifdef DQIV_NATIVE
    case 4:
        g_TownMenuMessage.close();
        abort_ = 0;
        mode_ = 0;
        menuItem_.ctype_ = menu::MenuItem::CURSORTYPE_ACTIVE;
        redraw_ = 1;
        break;
#endif
    }
}
