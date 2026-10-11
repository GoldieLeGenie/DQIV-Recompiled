#include "ov016/TownMenu_MAGIC/TownMenu_MAGIC_CHARSELECT.hpp"
#include "ov016/TownMenu_MAGIC/TownMenu_MAGIC_ROOT.hpp"
#include "ov016/TownMenu_ROOT/TownMenu_ROOT.hpp"
#include "ov016/TownMenuPlayerControl/TownMenuPlayerControl.hpp"
#include "ov000/town/TownWindowSystem.hpp"
#include "ov001/window/FieldWindowSystem.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/window/MenuControl.hpp"
#include "main/cmn/GameManager.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/UseAction.hpp"
#include "main/status/UseActionParam.hpp"
#include "main/status/UseActionMacro.hpp"
#include "main/text/TextAPI.hpp"
#include "ov016/MenuTemplate/MenuTemplate_town.hpp"
#include "ov016/UnkTownMenuDraw/UnkTownMenuDraw_0217d560.hpp"

THUMB void TownMenu_MAGIC_CHARSELECT::menuSetup()
{
    status::g_Party.setBattleMode();
    charaItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    fromActiveChara_ = TownMenuPlayerControl::getSingleton()->activeChara_;
    toActiveChara_ = TownMenuPlayerControl::getSingleton()->targetChara_;
    page_ = TownMenuPlayerControl::getSingleton()->targetItemPage_;
    magicID_ = TownMenuPlayerControl::getSingleton()->activeMagicID_;
    activeMagic_ = TownMenuPlayerControl::getSingleton()->activeMagic_;
    haveMagicNum_ = 0;
    isResult_ = 0;
    openMessage_ = 0;
    conditionPoison_ = 0;
    for (int i = 0; i < 4; i++) {
        resultMes_[i] = 0;
    }
    charaItem_.active_ = toActiveChara_;
    navigator_.setupBase();
    navigator_.setup(2, 2, status::g_Party.getCount());
    navigator_.setPageNo(page_);
}

THUMB void TownMenu_MAGIC_CHARSELECT::menuExecute()
{
    status::g_Party.setBattleMode();
    int count = status::g_Party.getCount() - page_ * 4;
    if (count > 4) {
        count = 4;
    }
    MenuTemplate_town::townMenuPageTargetChara(&charaItem_, count, toActiveChara_);
    MenuTemplate_town::TOWN_CANCEL(&cancelItem_);
}

THUMB void TownMenu_MAGIC_CHARSELECT::menuDraw()
{
    unkfunc_0217da64(status::g_Party.getPlayerStatus(fromActiveChara_)->haveStatusInfo_.getMp(), status::UseAction::getUseMp(magicID_), fromActiveChara_, page_);
    if (!data_020ed1bc.isOpen()) {
        charaItem_.drawActive();
        cancelItem_.drawActive();
    }
}

THUMB void TownMenu_MAGIC_CHARSELECT::menuUpdate()
{
    status::g_Party.setBattleMode();
    if (data_020ed1bc.isOpen()) {
        if (data_020ed1bc.stat_ == MENUBASE_STAT_OK) {
            if (openMessage_ == 1) {
                int count = 0;
                unsigned int targetIndex = navigator_.getIndex(toActiveChara_);
                data_020ed1bc.restartMessage();
                for (; resultMes_[count] != 0; count++) {
                    TextAPI::setMACRO0(0x12, 0x50000000, status::g_Party.getPlayerIndex(targetIndex));
                    data_020ed1bc.addMessage(resultMes_[count]);
                }
                if (isResult_ == 1 && magicID_ != 0xca) {
                    SoundManager::playSe(0x1f5, 0);
                }
                if (magicID_ == 0xc4 || magicID_ == 0xc5 || magicID_ == 0xc6) {
                    SoundManager::playSe(0x1f5, 0);
                }
                status::g_Party.getPlayerStatus(targetIndex)->haveStatusInfo_.setHp(updateHP_);
                if (conditionPoison_ == 1 && magicID_ == 0xca) {
                    status::g_Party.getPlayerStatus(targetIndex)->haveStatusInfo_.detoxPoison();
                }
                openMessage_ = 0;
                redraw_ = 1;
                return;
            }
            redraw_ = 1;
            if ((magicID_ == 0xc8 || magicID_ == 0xc9) && isResult_ == 1) {
                close();
                gTownMenu_ROOT.stat_ = MENUBASE_STAT_OK;
                if (data_0210bb94.unkfunc_02058114(0xc) == 1) {
                    TownWindowSystem::getSingleton()->cmdWindow_.menuRefresh();
                } else if (data_0210bb94.unkfunc_02058114(0xe) == 1) {
                    FieldWindowSystem::getSingleton()->cmdWindow_.menuRefresh();
                }
                cmn::GameManager::getSingleton()->resetParty();
                return;
            }
            TownMenuPlayerControl::getSingleton()->setActiveChara(fromActiveChara_);
            TownMenuPlayerControl::getSingleton()->setActiveMagic(activeMagic_);
            data_020ed1bc.close();
            close();
            gTownMenu_MAGIC_ROOT.open();
            gTownMenu_MAGIC_ROOT.unkfunc_021797a8();
            gTownMenu_MAGIC_ROOT.mode_ = 1;
            gTownMenu_MAGIC_ROOT.getUseAction();
        }
        return;
    }
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        TownMenuPlayerControl::getSingleton()->setActiveChara(fromActiveChara_);
        TownMenuPlayerControl::getSingleton()->setActiveMagic(activeMagic_);
        close();
        gTownMenu_MAGIC_ROOT.open();
        gTownMenu_MAGIC_ROOT.unkfunc_021797a8();
        gTownMenu_MAGIC_ROOT.mode_ = 1;
        gTownMenu_MAGIC_ROOT.getUseAction();
        redraw_ = 1;
        return;
    }
    navigator_.setup(2, 2, status::g_Party.getCount());
    int result = MenuUpdate_Assist::menuSelect(charaItem_, navigator_);
    if (result != 0) {
        if (result == 2) {
            useMagic();
            return;
        }
        if (result == 1 || result >= 4) {
            toActiveChara_ = charaItem_.active_;
        }
        page_ = navigator_.getPageNo();
        redraw_ = 1;
    }
}

THUMB void TownMenu_MAGIC_CHARSELECT::useMagic()
{
    status::g_Party.setBattleMode();
    status::UseActionParam useActionParam;
    int chara = toActiveChara_;
    int pageStart = page_ * 4;
    TownMenuPlayerControl::getSingleton()->targetChara_ = chara;
    TownMenuPlayerControl::getSingleton()->setTargetItemPage(page_);
    data_020ed1bc.openMessageForMENU();
    TextAPI::setMACRO0(1, 0x50000000, status::g_Party.getPlayerIndex(fromActiveChara_));
    if (magicID_ == 0xc4 || magicID_ == 0xc5 || magicID_ == 0xc6 || magicID_ == 0xca) {
        if (status::g_Party.getPlayerStatus(chara + pageStart)->haveStatusInfo_.isDeath()) {
            TextAPI::setMACRO0(0x11, 0x70000000, status::UseAction::getWordDBIndex(magicID_));
            TextAPI::setMACRO0(0x12, 0x50000000, status::g_Party.getPlayerIndex(chara + pageStart));
            data_020ed1bc.addMessage(0xc3cd2, 0xc3dd4);
            SoundManager::playSe(0x132, 0);
            return;
        }
    }
    useActionParam.clear();
    useActionParam.actorCharacterStatus_ = status::g_Party.getPlayerStatus(fromActiveChara_);
    useActionParam.targetCount_ = 1;
    useActionParam.targetCharacterStatus_[0] = status::g_Party.getPlayerStatus(chara + pageStart);
    useActionParam.actionIndex_ = magicID_;
    updateHP_ = useActionParam.targetCharacterStatus_[0]->haveStatusInfo_.getHp();
    if (status::g_Party.getPlayerStatus(chara + pageStart)->haveStatusInfo_.getCondition() == 6) {
        conditionPoison_ = 1;
    }
    status::UseAction::execUse(&useActionParam);
    short nowHP = useActionParam.targetCharacterStatus_[0]->haveStatusInfo_.getHp();
    useActionParam.targetCharacterStatus_[0]->haveStatusInfo_.setHp(updateHP_);
    updateHP_ = nowHP;
    if (conditionPoison_ == 1) {
        status::g_Party.getPlayerStatus(chara + pageStart)->haveStatusInfo_.setCondition((status::HaveStatusInfo::Condition)6);
    }
    isResult_ = useActionParam.result_;
    status::UseActionMacro::setExecMacro(useActionParam.actorCharacterStatus_, useActionParam.targetCharacterStatus_[0], useActionParam.actionIndex_);
    status::UseActionMacro::setResultMacro(useActionParam.actorCharacterStatus_, useActionParam.targetCharacterStatus_[0], useActionParam.actionIndex_);
    status::UseActionMessage& useActionMessage = useActionParam.message_[0];
    data_020ed1bc.addMessage(useActionMessage.execMessage_[0]);
    data_020ed1bc.setMessageCursor(true);
    openMessage_ = 1;
    for (int count = 0; useActionMessage.resultMessage_[count] != 0; count++) {
        resultMes_[count] = useActionMessage.resultMessage_[count];
    }
    if ((magicID_ == 0xc8 || magicID_ == 0xc9) && isResult_ == 1) {
        window::MenuControl::menu_ = 1;
        gTownMenu_MAGIC_ROOT.mode_ = 1;
        gTownMenu_MAGIC_ROOT.getUseAction();
        TownMenuPlayerControl::getSingleton()->setActiveChara(fromActiveChara_);
        TownMenuPlayerControl::getSingleton()->initializeLock_ = 1;
    }
    g_cmnPartyInfo.actorIndex_ = useActionParam.actorCharacterStatus_->haveStatusInfo_.haveStatus_.playerIndex_;
    g_cmnPartyInfo.targetIndex_ = useActionParam.targetCharacterStatus_[0]->haveStatusInfo_.haveStatus_.playerIndex_;
    SoundManager::playSe(0x132, 0);
}
