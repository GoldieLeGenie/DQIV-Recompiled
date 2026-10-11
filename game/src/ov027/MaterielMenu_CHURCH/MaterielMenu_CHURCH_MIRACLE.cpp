#pragma ipa file
#include "ov027/MaterielMenu_CHURCH/MaterielMenu_CHURCH.hpp"
#include "main/object/DisplayCharacter.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/global/Global.hpp"
#include "main/param/Param.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/sound/MenuSoundManager.hpp"
#include "main/cmn/GameManager.hpp"
#include "main/cmn/TalkSoundManager.hpp"
#include "main/cmn/NonBattleActionManager.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"

static int miracleMessage[3][7] = {
    {0xc6fd0, 0xc6fd8, 0xc6fd9, 0xc6fdc, 0xc6fdf},
    {0xc6fe5, 0xc6feb, 0xc6fec, 0xc6fef, 0xc6ff2},
    {0xc6ff8, 0xc6ffe, 0xc6fff, 0xc7002, 0xc7005},
};

THUMB void MaterielMenu_CHURCH_MIRACLE::menuSetup()
{
    status::g_Party.setBattleMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    menuItem_.active_ = 0;
    navigator_.setupBase();
    navigator_.setup(5, 2, status::g_Party.getCount());
    miracle_ = -1;
    miracleStatus_ = MIRACLE_NONE;
    unk_100 = 1;
    int type_;
    int mother_;
    param::MapChurch* church = status::excelParam.mapChurch_;
    const char motherMapName[3] = "ms";
    type_ = false;
    mother_ = false;
    int i = 0;
    unsigned int count = param::MapChurch::size_;
    for (; i < count; i++) {
        if (church[i].floor[0] == g_Global.getMapName()[0] &&
            church[i].floor[1] == g_Global.getMapName()[1] &&
            church[i].floor[2] == g_Global.getMapName()[2]) {
            if (motherMapName[0] == g_Global.getMapName()[0] && motherMapName[1] == g_Global.getMapName()[1]) {
                mother_ = true;
            }
            type_ = (char)(church[i].byte_1 & 1);
            break;
        }
    }
    sexType_ = type_ ? 1000 : 0;
    soundType_ = 1;
    int type = 0x32;
    if (sexType_ == 1000 && mother_ == false) {
        type = 0x31;
        soundType_ = 0;
    }
    ui_MsgSndSet(type);
    for (int i = 0; i < 2; i++) {
        miracleAmount_[i] = 0;
    }
    MenuSoundManager::getSingleton()->initialize();
    if (g_Global.bookingFlag_ == Global::BOOKING_CHURCH) {
        miracleStatus_ = MIRACLE_ISEND;
    }
}

THUMB void MaterielMenu_CHURCH_MIRACLE::menuExecute()
{
    status::g_Party.setBattleMode();
    MenuTemplate_materiel::MATERIEL_ICON32_5x2_CHURCH(&menuItem_, menuItem_.active_, status::g_Party.getCount());
}

THUMB void MaterielMenu_CHURCH_MIRACLE::menuDraw()
{
    if (data_020ed1bc.isMessageWAITPROG() && miracleStatus_ != MIRACLE_ISEND && miracleStatus_ != MIRACLE_SOUND && miracleStatus_ != MIRACLE_SOUNDEND) {
        status::g_Party.setBattleMode();
        unkfunc_0216fcbc(status::g_Party.getCount());
        menuItem_.drawActive();
        return;
    }
    unkfunc_0216fc94(0, 0);
}

THUMB void MaterielMenu_CHURCH_MIRACLE::menuUpdate()
{
    if (MenuSoundManager::getSingleton()->isPlaySound() == false) {
        status::g_Party.setBattleMode();
        if (messageUpdate() == false) {
            listUpdate();
        }
    }
}

THUMB bool MaterielMenu_CHURCH_MIRACLE::messageUpdate()
{
    int stat = data_020ed1bc.stat_;
    if (data_020ed1bc.isOpen() == false && miracleStatus_ != MIRACLE_ISEND) {
        return false;
    }
    switch (miracleStatus_) {
    case MIRACLE_NONE:
        if (stat == 1 || stat == 2) {
            data_020ed1bc.close();
            redraw_ = 1;
        }
        break;
    case MIRACLE_CHECK:
        if (stat == 1) {
            if (status::g_Party.gold_ >= miracleAmount_[miracle_]) {
                if (miracle_ == MaterielMenu_CHURCH_ROOT::MIRACLE_ORDER_REVIVAL) {
                    selectRevival();
                } else if (miracle_ == MaterielMenu_CHURCH_ROOT::MIRACLE_ORDER_ANTIDOTE) {
                    selectAntidote();
                } else if (miracle_ == MaterielMenu_CHURCH_ROOT::MIRACLE_ORDER_ANTICURSE) {
                    selectAntiCurse();
                }
            } else {
                data_020ed1bc.close();
                selectCheckNG();
            }
            redraw_ = 1;
        } else if (stat == 2) {
            data_020ed1bc.close();
            selectNG();
            redraw_ = 1;
        }
        break;
    case MIRACLE_SOUND:
        if (stat == 1 || stat == 2 || data_020ed1bc.isMessageWAITPROG()) {
            int type = 0x32;
            if (soundType_ == 0) {
                type = 0x31;
            }
            ui_MsgSndSet(type);
            data_020ed1bc.close();
            data_020ed1bc.openMessageForTALK();
            data_020ed1bc.addMessageNOWAIT(sexType_ + 0xc7008);
            data_020ed1bc.addMessageWAITKEY();
            openRootMenu();
        }
        return true;
    case MIRACLE_ISEND:
        if (stat == 1 || stat == 2 || data_020ed1bc.isMessageWAITPROG()) {
            data_020ed1bc.close();
            selectRevivalEnd();
            redraw_ = 1;
        }
        return true;
    case MIRACLE_SOUNDEND:
        if (data_020ed1bc.isMessageWAITPROG()) {
            MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_MIRACLE);
            if (miracle_ == MaterielMenu_CHURCH_ROOT::MIRACLE_ORDER_REVIVAL) {
                miracleStatus_ = MIRACLE_ISEND;
            } else {
                miracleStatus_ = MIRACLE_SOUND;
            }
            status::HaveStatusInfo& statusInfo = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_;
            if (miracle_ == MaterielMenu_CHURCH_ROOT::MIRACLE_ORDER_REVIVAL) {
                statusInfo.rebirth();
                cmn::GameManager::getSingleton()->resetParty();
            } else if (miracle_ == MaterielMenu_CHURCH_ROOT::MIRACLE_ORDER_ANTIDOTE) {
                statusInfo.detoxPoison();
            } else if (miracle_ == MaterielMenu_CHURCH_ROOT::MIRACLE_ORDER_ANTICURSE) {
                bool resist = false;
                ItemType equip[5] = {(ItemType)0, (ItemType)1, (ItemType)2, (ItemType)3, (ItemType)4};
                for (int i = 0; i < 5; i++) {
                    if (resist == false) {
                        resist = resistCurseEquip(equip[i]);
                    }
                }
            }
        }
        return true;
    }
    if (data_020ed1bc.isMessageWAITPROG() == false) {
        return true;
    }
    return false;
}

THUMB bool MaterielMenu_CHURCH_MIRACLE::listUpdate()
{
    if (data_020ed1bc.isMessageWAITPROG()) {
        if (unk_100) {
            unk_100 = 0;
            redraw_ = 1;
            return false;
        }
        int active = menuItem_.active_;
        int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
        navigator_.setup(5, 2, status::g_Party.getCount());
        if (result != 0) {
            if (result == 2) {
                redraw_ = 1;
                int lv = 0;
                status::PlayerStatus* player = status::g_Party.getPlayerStatus(activeChara_);
                if (player->haveStatusInfo_.haveStatus_.isPlayer() == false) {
                    miracleAmount_[0] = 10;
                } else {
                    lv = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveStatus_.level_;
                    miracleAmount_[0] = (lv * lv + 10) / 10 * 10;
                }
                miracleAmount_[1] = 5;
                miracleAmount_[2] = lv * 30;
                if (isMiracle(active, miracle_)) {
                    activeChara_ = active;
                    miracleStatus_ = MIRACLE_CHECK;
                    selectGoldCheck();
                } else {
                    int messageID = sexType_ + miracleMessage[miracle_][4];
                    int playerID = status::g_Party.getPlayerIndex(active);
                    data_020ed1bc.openMessageForTALK();
                    TextAPI::setMACRO0(0x12, 0x50000000, playerID);
                    data_020ed1bc.addMessage(messageID);
                    data_020ed1bc.setMessageLastCursor(true);
                    miracleStatus_ = MIRACLE_SOUND;
                }
                return true;
            }
            if (result == 3) {
                data_020ed1bc.openMessageForTALK();
                data_020ed1bc.addMessage(sexType_ + 0xc700e);
                miracleStatus_ = MIRACLE_SOUND;
                redraw_ = 1;
                return true;
            }
            activeChara_ = menuItem_.active_;
            int activeChara = activeChara_;
            MaterielMenuPlayerControl::getSingleton()->activeChara_ = activeChara;
            redraw_ = 1;
            return true;
        }
    }
    return false;
}

THUMB void MaterielMenu_CHURCH_MIRACLE::selectRevival()
{
    data_020ed1bc.close();
    int playerID = status::g_Party.getPlayerIndex(activeChara_);
    data_020ed1bc.openMessageForTALK();
    TextAPI::setMACRO0(0x12, 0x50000000, playerID);
    data_020ed1bc.addMessage(sexType_ + 0xc6fd3, sexType_ + 0xc6fd4);
    data_020ed1bc.addMessageWAITKEY();
    miracleStatus_ = MIRACLE_SOUNDEND;
}

THUMB void MaterielMenu_CHURCH_MIRACLE::selectRevivalEnd()
{
    status::HaveStatusInfo& statusInfo = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_;
    data_020ed1bc.close();
    if (g_Global.bookingFlag_ == Global::BOOKING_NONE) {
        redraw_ = 1;
        payOutMiracle();
        g_Global.bookingFlag_ = Global::BOOKING_CHURCH;
        close();
        cmn::GameManager::getSingleton()->playerManager_->setLock(1);
        cmn::GameManager::getSingleton()->playerManager_->charaColl_ = 0;
        BillboardCharacter::setAllCharaAnim(0);
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        return;
    }
    cmn::GameManager::getSingleton()->playerManager_->setLock(0);
    cmn::GameManager::getSingleton()->playerManager_->charaColl_ = 1;
    BillboardCharacter::setAllCharaAnim(1);
    int playerID = status::g_Party.getPlayerIndex(activeChara_);
    data_020ed1bc.openMessageForMENU();
    TextAPI::setMACRO0(0x12, 0x50000000, playerID);
    data_020ed1bc.addMessage(sexType_ + 0xc6fd5);
    g_Global.bookingFlag_ = Global::BOOKING_NONE;
    miracleStatus_ = MIRACLE_SOUND;
}

THUMB void MaterielMenu_CHURCH_MIRACLE::selectAntidote()
{
    status::HaveStatusInfo& statusInfo = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_;
    data_020ed1bc.close();
    int playerID = status::g_Party.getPlayerIndex(activeChara_);
    data_020ed1bc.openMessageForTALK();
    TextAPI::setMACRO0(0x12, 0x50000000, playerID);
    data_020ed1bc.addMessage(sexType_ + 0xc6fe8);
    data_020ed1bc.addMessageWAITKEY();
    payOutMiracle();
    miracleStatus_ = MIRACLE_SOUNDEND;
}

THUMB void MaterielMenu_CHURCH_MIRACLE::selectAntiCurse()
{
    status::HaveStatusInfo& statusInfo = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_;
    data_020ed1bc.close();
    data_020ed1bc.openMessageForTALK();
    TextAPI::setMACRO0(0x12, 0x50000000, status::g_Party.getPlayerIndex(activeChara_));
    data_020ed1bc.addMessage(sexType_ + 0xc6ffb);
    data_020ed1bc.addMessageWAITKEY();
    payOutMiracle();
    miracleStatus_ = MIRACLE_SOUNDEND;
}

THUMB bool MaterielMenu_CHURCH_MIRACLE::resistCurseEquip(ItemType equip)
{
    status::HaveStatusInfo& statusInfo = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_;
    if (statusInfo.haveEquipment_.isSpell(equip)) {
        statusInfo.haveEquipment_.breakSpell(equip);
        statusInfo.resetEquipment2(equip);
        return true;
    }
    return false;
}

THUMB void MaterielMenu_CHURCH_MIRACLE::selectGoldCheck()
{
    int messageID = sexType_ + miracleMessage[miracle_][0];
    int playerID = status::g_Party.getPlayerIndex(activeChara_);
    data_020ed1bc.openMessageForTALK();
    TextAPI::setMACRO0(0x12, 0x50000000, playerID);
    TextAPI::setMACRO0(0x41, 0xf0000000, miracleAmount_[0]);
    TextAPI::setMACRO0(0x40, 0xf0000000, miracleAmount_[1]);
    TextAPI::setMACRO0(0x28, 0xf0000000, miracleAmount_[2]);
    data_020ed1bc.addMessage(messageID);
    data_020ed1bc.setYesNo();
}

THUMB void MaterielMenu_CHURCH_MIRACLE::selectCheckNG()
{
    int messageID[2];
    messageID[0] = sexType_ + miracleMessage[miracle_][1];
    messageID[1] = sexType_ + miracleMessage[miracle_][2];
    data_020ed1bc.openMessageForTALK();
    data_020ed1bc.addMessage(messageID[0], messageID[1]);
    miracleStatus_ = MIRACLE_SOUND;
}

THUMB void MaterielMenu_CHURCH_MIRACLE::selectNG()
{
    int messageID = sexType_ + miracleMessage[miracle_][3];
    data_020ed1bc.openMessageForTALK();
    data_020ed1bc.addMessage(messageID);
    miracleStatus_ = MIRACLE_SOUND;
}

THUMB bool MaterielMenu_CHURCH_MIRACLE::isMiracle(int index, int miracle)
{
    status::HaveStatusInfo& statusInfo = status::g_Party.getPlayerStatus(index)->haveStatusInfo_;
    switch (miracle) {
    case MaterielMenu_CHURCH_ROOT::MIRACLE_ORDER_REVIVAL:
        if (statusInfo.isDeath() != 0) {
            return true;
        }
        return false;
    case MaterielMenu_CHURCH_ROOT::MIRACLE_ORDER_ANTIDOTE:
        if (statusInfo.statusChange_.isEnable(status::StatusChange::StatusPoison) != 0) {
            return true;
        }
        return false;
    case MaterielMenu_CHURCH_ROOT::MIRACLE_ORDER_ANTICURSE:
        if (statusInfo.isSpell() != 0) {
            return true;
        }
        return false;
    }
    return false;
}

THUMB void MaterielMenu_CHURCH_MIRACLE::payOutMiracle()
{
    int gold = status::g_Party.gold_;
    int setGold = gold - miracleAmount_[miracle_];
    status::g_Party.setGold(setGold);
}

THUMB void MaterielMenu_CHURCH_MIRACLE::openRootMenu()
{
    close();
    gMaterielMenu_CHURCH_ROOT.open();
    gMaterielMenu_CHURCH_ROOT.menuItem_.active_ = 0;
    gMaterielMenu_CHURCH_ROOT.firstFlag_ = 0;
}
