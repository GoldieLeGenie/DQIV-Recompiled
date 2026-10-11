#pragma ipa file
#include "ov027/MaterielMenu_CHURCH/MaterielMenu_CHURCH.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/global/Global.hpp"
#include "main/param/Param.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/GameFlag.hpp"
#include "main/status/ShopList.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/cmn/TalkSoundManager.hpp"
#include "ov016/MenuTemplate/MenuTemplate_town.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"

THUMB void MaterielMenu_CHURCH_ROOT::menuSetup()
{
    status::g_Party.setBattleMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    menuItem2_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    MaterielMenuPlayerControl::getSingleton()->allClear();
    activeCommand_ = -1;
    expMessageCount_ = -1;
    firstFlag_ = 1;
    unk_164 = 1;
    churchType_ = status::g_Shop.getChurchType(0);
    int mother = false;
    const char* town = "mc";
    const char* motherMapName = "ms";
    const char* sure = "ssh";
    if (town[0] == g_Global.getMapName()[0] && town[1] == g_Global.getMapName()[1] && g_AreaFlag.check(0x57)) {
        churchType_ = 1;
    }
    if (motherMapName[0] == g_Global.getMapName()[0] && motherMapName[1] == g_Global.getMapName()[1]) {
        mother = true;
    }
    int sex;
    param::MapChurch* church = status::excelParam.mapChurch_;
    sex = false;
    int i = 0;
    unsigned int count = param::MapChurch::size_;
    for (; i < count; i++) {
        if (church[i].floor[0] == g_Global.getMapName()[0] &&
            church[i].floor[1] == g_Global.getMapName()[1] &&
            church[i].floor[2] == g_Global.getMapName()[2]) {
            sex = (char)(church[i].byte_1 & 1);
            break;
        }
    }
    if (sure[0] == g_Global.getMapName()[0] && sure[1] == g_Global.getMapName()[1] && sure[2] == g_Global.getMapName()[2]) {
        sex = 1000;
    }
    sexType_ = sex ? 1000 : 0;
    int type = 0x32;
    if (sexType_ == 1000 && mother == false) {
        type = 0x31;
    }
    ui_MsgSndSet(type);
    timeType_ = g_Stage.getTimeZone();
    navigator_.setupBase();
    menuItem_.active_ = 0;
    if (g_Global.bookingFlag_ != Global::BOOKING_NONE) {
        activeCommand_ = 7;
    }
}

THUMB void MaterielMenu_CHURCH_ROOT::menuExecute()
{
    commandNum_ = churchType_ == 1 ? 6 : 5;
    int commandNum = commandNum_;
    MaterielMenuPlayerControl::getSingleton()->churchCommandNum_ = commandNum;
    MenuTemplate_materiel::MATERIEL_CHURCH_ROOT(&menuItem_, commandNum_, menuItem_.active_);
    MenuTemplate_town::TOWN_CANCEL(&menuItem2_);
}

THUMB void MaterielMenu_CHURCH_ROOT::menuDraw()
{
    if (activeCommand_ != 7) {
        unkfunc_0216fc94(activeCommand_, firstFlag_);
        if (activeCommand_ == -1 && firstFlag_ == 0 && data_020ed1bc.isMessageWAITPROG()) {
            menuItem_.drawActive();
        }
    }
}

THUMB void MaterielMenu_CHURCH_ROOT::menuUpdate()
{
    if (firstFlag_) {
        firstFlag_ = 0;
        firstMessage();
    }
    if (rootUpdate() == false) {
        commandUpdate();
    }
}

THUMB bool MaterielMenu_CHURCH_ROOT::rootUpdate()
{
    int stat = data_020ed1bc.stat_;
    switch (activeCommand_) {
    case -1:
        if (stat == 1 || stat == 2) {
            data_020ed1bc.close();
            redraw_ = 1;
            return true;
        }
        break;
    case 7:
        return true;
    case 1:
        if (stat == 1 || stat == 2) {
            data_020ed1bc.restartMessage();
            selectNextExp();
            redraw_ = 1;
        }
        break;
    case 6:
        if (stat == 1) {
            data_020ed1bc.close();
            activeCommand_ = -1;
            redraw_ = 1;
        } else if (stat == 2) {
            data_020ed1bc.close();
            selectEnd();
        }
        break;
    case 5:
        if (stat == 1 || stat == 2) {
            data_020ed1bc.close();
            activeCommand_ = -1;
            MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        }
        break;
    }
    return false;
}

THUMB bool MaterielMenu_CHURCH_ROOT::commandUpdate()
{
    if (data_020ed1bc.isOpen() && data_020ed1bc.isMessageWAITPROG()) {
        if (unk_164) {
            unk_164 = 0;
            redraw_ = 1;
            return false;
        }
        int offset = 6 - commandNum_;
        navigator_.setup(2, 3, commandNum_);
        int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
        if (result != 0) {
            if (result == 2) {
                int cmd = offset + menuItem_.active_;
                switch (cmd) {
                case 0:
                    close();
                    data_020ed1bc.close();
                    gMaterielMenu_SAVE.open();
                    gMaterielMenu_SAVE.saveType_ = MaterielMenu_SAVE::TYPE_CHURCH;
                    redraw_ = 1;
                    break;
                case 1:
                    selectNextExp();
                    break;
                case 2:
                    data_020ed1bc.openMessageForTALK();
                    data_020ed1bc.addMessage(sexType_ + 0xc6fcd);
                    data_020ed1bc.addMessageWAITKEY();
                    close();
                    gMaterielMenu_CHURCH_MIRACLE.open();
                    gMaterielMenu_CHURCH_MIRACLE.menuItem_.active_ = 0;
                    gMaterielMenu_CHURCH_MIRACLE.miracle_ = MIRACLE_ORDER_REVIVAL;
                    break;
                case 3:
                    data_020ed1bc.openMessageForTALK();
                    data_020ed1bc.addMessage(sexType_ + 0xc6fe2);
                    data_020ed1bc.addMessageWAITKEY();
                    close();
                    gMaterielMenu_CHURCH_MIRACLE.open();
                    gMaterielMenu_CHURCH_MIRACLE.menuItem_.active_ = 0;
                    gMaterielMenu_CHURCH_MIRACLE.miracle_ = MIRACLE_ORDER_ANTIDOTE;
                    break;
                case 4:
                    data_020ed1bc.openMessageForTALK();
                    data_020ed1bc.addMessage(sexType_ + 0xc6ff5);
                    data_020ed1bc.addMessageWAITKEY();
                    close();
                    gMaterielMenu_CHURCH_MIRACLE.open();
                    gMaterielMenu_CHURCH_MIRACLE.menuItem_.active_ = 0;
                    gMaterielMenu_CHURCH_MIRACLE.miracle_ = MIRACLE_ORDER_ANTICURSE;
                    break;
                case 5:
                    redraw_ = 1;
                    activeCommand_ = 5;
                    selectEnd();
                    return true;
                }
                redraw_ = 1;
                return true;
            }
            if (result == 3) {
                redraw_ = 1;
                if (activeCommand_ == -1) {
                    selectEnd();
                    return true;
                }
            }
        }
    }
    return false;
}

THUMB void MaterielMenu_CHURCH_ROOT::selectNextExp()
{
    status::g_Party.setPlayerMode();
    int count = expMessageCount_;
    if (count == -1) {
        oneMessage(0xc6fbf);
        data_020ed1bc.setMessageCursor(true);
        activeCommand_ = 1;
    } else if (count >= status::g_Party.getCount()) {
        activeCommand_ = -1;
        expMessageCount_ = -1;
        unk_164 = 1;
        data_020ed1bc.openMessageForTALK();
        data_020ed1bc.addMessageNOWAIT(sexType_ + 0xc7008);
        data_020ed1bc.addMessageWAITKEY();
        return;
    } else {
        status::HaveStatusInfo& statusInfo = status::g_Party.getPlayerStatus(count)->haveStatusInfo_;
        int exp = statusInfo.getLevelupExp();
        if (statusInfo.haveStatus_.isPlayer() == false) {
            expMessageCount_++;
            selectNextExp();
            return;
        }
        if (statusInfo.haveStatus_.level_ == 99) {
            int playerID = status::g_Party.getPlayerIndex(expMessageCount_);
            TextAPI::setMACRO0(6, 0x50000000, playerID);
            data_020ed1bc.addMessage(sexType_ + 0xc6fc3);
            data_020ed1bc.setMessageCursor(true);
        } else if (exp <= 0) {
            int playerID = status::g_Party.getPlayerIndex(expMessageCount_);
            TextAPI::setMACRO0(6, 0x50000000, playerID);
            data_020ed1bc.addMessage(sexType_ + 0xc6fc6);
            data_020ed1bc.addMessage(sexType_ + 0xc6fc7);
            data_020ed1bc.setMessageCursor(true);
        } else {
            int playerID = status::g_Party.getPlayerIndex(expMessageCount_);
            TextAPI::setMACRO0(6, 0x50000000, playerID);
            TextAPI::setMACRO0(8, 0xf0000000, exp);
            data_020ed1bc.addMessage(sexType_ + 0xc6fca);
            data_020ed1bc.setMessageCursor(true);
        }
    }
    expMessageCount_++;
}

THUMB void MaterielMenu_CHURCH_ROOT::selectEnd()
{
    oneMessage(0xc700b);
    activeCommand_ = 5;
}

THUMB void MaterielMenu_CHURCH_ROOT::firstMessage()
{
    data_020ed1bc.openMessageForTALK();
    if (activeCommand_ != 7) {
        if (timeType_ == 4 || timeType_ == 1) {
            data_020ed1bc.addMessageNOWAIT(sexType_ + 0xc6f9d);
            data_020ed1bc.addMessageWAITKEY();
        } else {
            data_020ed1bc.addMessageNOWAIT(sexType_ + 0xc6f9a);
            data_020ed1bc.addMessageWAITKEY();
        }
        return;
    }
    ui_MsgSndSet(0x30);
    data_020ed1bc.addMessageNOWAIT(sexType_ + 0xc6fbc);
    data_020ed1bc.addMessageWAITKEY();
}

THUMB void MaterielMenu_CHURCH_ROOT::oneMessage(int mess)
{
    data_020ed1bc.openMessageForTALK();
    data_020ed1bc.addMessage(mess + sexType_);
}
