#include "ov016/TownMenu_MAGIC/TownMenu_MAGIC_ROOT.hpp"
#include "ov016/TownMenu_MAGIC/TownMenu_MAGIC_CHARSELECT.hpp"
#include "ov016/TownMenu_MAGIC/TownMenu_MAGIC_MOVE.hpp"
#include "ov016/TownMenu_ROOT/TownMenu_ROOT.hpp"
#include "ov016/TownMenuPlayerControl/TownMenuPlayerControl.hpp"
#include "ov000/town/TownFurniture.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/cmn/CommonRuraData.hpp"
#include "main/cmn/NonBattleActionManager.hpp"
#include "main/global/Global.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/UseAction.hpp"
#include "main/status/UseActionParam.hpp"
#include "main/status/UseActionMacro.hpp"
#include "main/text/TextAPI.hpp"
#include "ov016/MenuTemplate/MenuTemplate_town.hpp"
#include "ov016/UnkTownMenuDraw/UnkTownMenuDraw_0217d560.hpp"

THUMB void TownMenu_MAGIC_ROOT::menuSetup()
{
    status::g_Party.setBattleMode();
    magicItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    charaItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    if (mode_ != 1) {
        mode_ = 0;
        activeChara_ = TownMenuPlayerControl::getSingleton()->activeChara_;
    } else {
        mode_ = 1;
        TownMenuPlayerControl::getSingleton()->setActiveChara(activeChara_);
    }
    activeMagic_ = TownMenuPlayerControl::getSingleton()->activeMagic_;
    riremitoOK_ = 0;
    useBehomara_ = 0;
    soundCount_ = 0;
    targetCount_ = 0;
    getUseAction();
    charaNavigator_.setupBase();
    magicNavigator_.setupBase();
}

THUMB void TownMenu_MAGIC_ROOT::menuExecute()
{
    status::g_Party.setBattleMode();
    int count = status::g_Party.getCount();
    if (mode_ == 0) {
        if (count < 6) {
            MenuTemplate_town::townMenuItemSelectHalfChara(&charaItem_, count, activeChara_);
        } else {
            MenuTemplate_town::townMenuItemSelectChara(&charaItem_, count, activeChara_);
        }
    } else {
        MenuTemplate_town::townMenuSelectMagic(&magicItem_, magicNumMax_, activeMagic_);
    }
    MenuTemplate_town::TOWN_CANCEL(&cancelItem_);
}

THUMB void TownMenu_MAGIC_ROOT::menuDraw()
{
    status::g_Party.setBattleMode();
    status::HaveStatusInfo& statusInfo = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_;
    int mp1;
    int mp2;
    if (mode_ == 0) {
        mp1 = statusInfo.getMpMax();
        mp2 = statusInfo.getMp();
    } else {
        mp1 = statusInfo.getMp();
        mp2 = status::UseAction::getUseMp(magicArray_[activeMagic_]);
    }
    unkfunc_0217d9cc(mp1, mp2, magicArray_[activeMagic_], activeChara_, mode_);
    if (!data_020ed1bc.isOpen()) {
        if (mode_ == 0) {
            charaItem_.drawActive();
        } else {
            magicItem_.drawActive();
        }
        cancelItem_.drawActive();
    }
}

THUMB void TownMenu_MAGIC_ROOT::menuUpdate()
{
    status::g_Party.setBattleMode();
    if (useBehomara_ == 1 && targetCount_ != 0 && soundCount_ != 0xff) {
        if (soundCount_ > 0x19) {
            SoundManager::playSe(0x1f5, 0);
            soundCount_ = 0;
            targetCount_--;
        } else {
            soundCount_++;
        }
    }
    if (data_020ed1bc.isOpen()) {
        if (data_020ed1bc.stat_ == MENUBASE_STAT_OK) {
            data_020ed1bc.close();
            if (isResult_ == 1 && unkfunc_02179ba8() == 1) {
                close();
                gTownMenu_ROOT.stat_ = MENUBASE_STAT_OK;
                mode_ = 0;
            }
        }
        return;
    }
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        cancelItem_.result_ = 0;
        cancelItem_.lastresult_ = 0;
        if (mode_ == 1) {
            mode_ = 0;
            activeMagic_ = 0;
            charaItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
            redraw_ = 1;
            return;
        }
        close();
        gTownMenu_ROOT.open();
        gTownMenu_ROOT.menuItem_.active_ = TownMenu_ROOT::ROOT_MAGIC;
        redraw_ = 1;
        return;
    }
    if (mode_ == 0) {
        charaNavigator_.setup(5, 2, status::g_Party.getCount());
        int result = MenuUpdate_Assist::menuSelect(charaItem_, charaNavigator_);
        if (result != 0) {
            if (result == 2) {
                if (magicNumMax_ == 0) {
                    data_020ed1bc.openMessageForMENU();
                    TextAPI::setMACRO0(1, 0x50000000, status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveStatus_.playerIndex_);
                    data_020ed1bc.addMessage(0xc3cd5);
                    return;
                }
                if (status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.isDeath()) {
                    data_020ed1bc.openMessageForMENU();
                    TextAPI::setMACRO0(1, 0x50000000, status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveStatus_.playerIndex_);
                    data_020ed1bc.addMessage(0xc3cd7);
                    return;
                }
                mode_ = 1;
                redraw_ = 1;
                return;
            }
            activeMagic_ = 0;
            charaNavigator_.getPageNo();
            activeChara_ = charaNavigator_.getIndex(charaItem_.active_);
            TownMenuPlayerControl::getSingleton()->setActiveChara(activeChara_);
            getUseAction();
            redraw_ = 1;
        }
        return;
    }
    magicNavigator_.setup(2, 4, magicNumMax_);
    int result = MenuUpdate_Assist::menuSelect(magicItem_, magicNavigator_);
    if (result != 0) {
        activeMagic_ = magicItem_.active_;
        TownMenuPlayerControl::getSingleton()->setActiveMagic(activeMagic_);
        if (result == 2) {
            judgeMagic();
            redraw_ = 1;
            return;
        }
        redraw_ = 1;
    }
}

THUMB void TownMenu_MAGIC_ROOT::unkfunc_021797a8()
{
    charaItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
    charaItem_.active_ = activeChara_;
}

THUMB void TownMenu_MAGIC_ROOT::getUseAction()
{
    status::g_Party.setBattleMode();
    unsigned char count = 0;
    int haveActionCount = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveAction_.getCount();
    for (int i = 0; i < haveActionCount; i++) {
        int magicID = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveAction_.getAction(i);
        if (status::UseAction::isUsuallyUse(magicID)) {
            magicArray_[count] = (unsigned char)magicID;
            count++;
        }
    }
    magicNumMax_ = count;
}

THUMB void TownMenu_MAGIC_ROOT::judgeMagic()
{
    status::g_Party.setBattleMode();
    int magicID = magicArray_[activeMagic_];
    TownMenuPlayerControl::getSingleton()->activeMagicID_ = magicID;
    status::PlayerStatus* playerStatus = status::g_Party.getPlayerStatus(activeChara_);
    if (status::UseAction::getUseMp(magicID) > playerStatus->haveStatusInfo_.getMp()) {
        data_020ed1bc.openMessageForMENU();
        data_020ed1bc.addMessage(0xc3c72);
        return;
    }
    switch (status::UseAction::getUseType(magicID)) {
    case status::UseItem::Friend:
        if (status::UseAction::getUseArea(magicID) == status::UseItem::One) {
            close();
            gTownMenu_MAGIC_CHARSELECT.open();
        } else {
            useBehomara_ = 1;
            useMagicNoTarget();
        }
        break;
    case status::UseItem::None:
        if (magicID == 0xcb) {
            unkfunc_02179b0c();
        } else {
            useBehomara_ = 0;
            useMagicNoTarget();
        }
        break;
    }
}

THUMB void TownMenu_MAGIC_ROOT::useMagicNoTarget()
{
    status::g_Party.setBattleMode();
    status::UseActionParam useActionParam;
    int isTaka = 0;
    if (data_0210bb94.unkfunc_02058114(0xe)) {
        int walkX = 0;
        int walkY = 0;
        isTaka = FieldSymbolManager::getSingleton()->searchSymbol(walkX, walkY);
        TownMenuPlayerControl::getSingleton()->setTakanome(walkX, walkY);
    }
    useActionParam.clear();
    useActionParam.actorCharacterStatus_ = status::g_Party.getPlayerStatus(activeChara_);
    if (useBehomara_ == 1) {
        int count = 0;
        for (int i = 0; i < status::g_Party.getCount(); i++) {
            if (!status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath()) {
                useActionParam.targetCharacterStatus_[count] = status::g_Party.getPlayerStatus(i);
                count++;
            }
        }
        targetCount_ = count;
        soundCount_ = 10;
    } else {
        useActionParam.targetCharacterStatus_[0] = status::g_Party.getPlayerStatus(activeChara_);
        targetCount_ = 1;
    }
    useActionParam.targetCount_ = targetCount_;
    useActionParam.actionIndex_ = magicArray_[activeMagic_];
    status::UseAction::execUse(&useActionParam);
    data_020ed1bc.openMessageForMENU();
    TextAPI::setMACRO0(1, 0x50000000, status::g_Party.getPlayerIndex(activeChara_));
    status::UseActionMacro::setExecMacro(useActionParam.actorCharacterStatus_, useActionParam.targetCharacterStatus_[0], useActionParam.actionIndex_);
    isResult_ = useActionParam.result_;
    status::UseActionMessage& useActionMessage = useActionParam.message_[0];
    if (useBehomara_ == 1) {
        data_020ed1bc.addMessageNOWAIT(useActionMessage.execMessage_[0]);
    } else {
        data_020ed1bc.addMessage(useActionMessage.execMessage_[0]);
    }
    if (magicArray_[activeMagic_] == 0xcc && isResult_ == 1) {
        riremitoOK_ = 1;
        SoundManager::playSe(0x132, 0);
        return;
    }
    for (int i = 0; i < targetCount_; i++) {
        status::UseActionMacro::setResultMacro(useActionParam.actorCharacterStatus_, useActionParam.targetCharacterStatus_[i], useActionParam.actionIndex_);
        if (magicArray_[activeMagic_] == 0xd1) {
            if (data_0210bb94.unkfunc_02058114(0xc)) {
                data_020ed1bc.addMessage(0xc3cde);
                break;
            }
            if (isTaka == 0) {
                data_020ed1bc.addMessage(0xc3cde);
                break;
            }
        }
        for (int messageCount = 0; useActionMessage.resultMessage_[messageCount] != 0; messageCount++) {
            if (useBehomara_ != 0) {
                data_020ed1bc.addMessageNOWAIT(useActionMessage.resultMessage_[messageCount]);
            } else {
                data_020ed1bc.addMessage(useActionMessage.resultMessage_[messageCount]);
            }
        }
    }
    g_cmnPartyInfo.actorIndex_ = useActionParam.actorCharacterStatus_->haveStatusInfo_.haveStatus_.playerIndex_;
    g_cmnPartyInfo.targetIndex_ = useActionParam.targetCharacterStatus_[0]->haveStatusInfo_.haveStatus_.playerIndex_;
    if (magicArray_[activeMagic_] == 0xd8) {
        SoundManager::playSe(0x23c, 0);
        return;
    }
    SoundManager::playSe(0x132, 0);
}

THUMB void TownMenu_MAGIC_ROOT::unkfunc_02179b0c()
{
    status::g_Party.setBattleMode();
    if (cmn::CommonRuraData::getSingleton()->getRuraCount() == 0) {
        data_020ed1bc.openMessageForMENU();
        TextAPI::setMACRO0(1, 0x50000000, status::g_Party.getPlayerIndex(activeChara_));
        TextAPI::setMACRO0(0x11, 0x70000000, status::UseAction::getWordDBIndex(0xcb));
        data_020ed1bc.addMessage(0xc3cd2, 0xc3d6f);
        return;
    }
    close();
    gTownMenu_MAGIC_MOVE.open();
    gTownMenu_MAGIC_MOVE.setActiveChara(activeChara_, status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveAction_.getCount());
    gTownMenu_MAGIC_MOVE.setActiveMagic(magicArray_[activeMagic_], activeMagic_);
}

THUMB int TownMenu_MAGIC_ROOT::unkfunc_02179ba8()
{
    int ret = 0;
    switch (magicArray_[activeMagic_]) {
    case 0xcc:
        if (riremitoOK_ != 0) {
            g_Global.setRanarutaFlag(true);
            cmn::NonBattleActionManager::getSingleton()->setAction(cmn::ACTION_RIREMITO);
        }
        ret = 1;
        break;
    case 0xd5:
        if (data_0210bb94.unkfunc_02058114(0xc) == 1) {
            TownFurnitureManager::getSingleton()->searchItem();
        }
        ret = 1;
        break;
    case 0xd0:
        if (isResult_ == 1) {
            g_Global.setRanarutaFlag(true);
            cmn::NonBattleActionManager::getSingleton()->setAction(cmn::ACTION_RANARUTA);
        }
        ret = 1;
        break;
    case 0xce:
        status::StageStatus::setToramana(1);
        ret = 1;
        break;
    case 0xcb:
    case 0xcd:
    case 0xcf:
    case 0xd1:
    case 0xd4:
    case 0xd7:
    case 0xd8:
        ret = 1;
        break;
    }
    return ret;
}
