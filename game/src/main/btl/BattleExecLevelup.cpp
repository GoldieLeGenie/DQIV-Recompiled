#include "main/btl/BattleExecLevelup.hpp"
#include "main/global/Global.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/menu/MenuAPI.hpp"
#include "main/text/TextAPI.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/UseAction.hpp"
#include "ov003/btl/BattleMessage.hpp"
#include "main/btl/BattleAutoFeed.hpp"

BattleExecLevelup g_BattleExecLevelup;

THUMB void setMessage(int mes0, int mes1, int mes2, int mes3)
{
    if (data_0210bb94.unkfunc_0205810c() == 13) {
        btl::BattleMessage::setMessage(mes0, mes1, mes2, mes3);
        return;
    }
    MenuAPI::openMessageWindowMenu();
    if (mes0 != 0) {
        MenuAPI::addMessageSerial(mes0);
    }
    if (mes1 != 0) {
        MenuAPI::addMessageSerial(mes1);
    }
    if (mes2 != 0) {
        MenuAPI::addMessageSerial(mes2);
    }
    if (mes3 != 0) {
        MenuAPI::addMessageSerial(mes3);
    }
}

THUMB void BattleExecVictory10::setup()
{
    TextAPI::setMACRO0(0x12, 0x50000000, index_);
    TextAPI::setMACRO0(0x50, 0xf0000000, level_);
    setMessage(0xc3c42, 0, 0, 0);
    BattleAutoFeed::setMessageSend();
}

THUMB void BattleExecVictory11::setup()
{
    status::BaseStatus* diff = status::g_Party.getPlayerStatus(index_)->haveStatusInfo_.haveStatus_.getDiffStatus();
    if (diff->hpMax_ != 0) {
        TextAPI::setMACRO0(0x4d, 0xf0000000, diff->hpMax_);
        setMessage(0xc3c44, 0, 0, 0);
        BattleAutoFeed::setMessageSend();
    }
}

THUMB void BattleExecVictory12::setup()
{
    status::BaseStatus* diff = status::g_Party.getPlayerStatus(index_)->haveStatusInfo_.haveStatus_.getDiffStatus();
    if (diff->mpMax_ != 0) {
        TextAPI::setMACRO0(0x51, 0xf0000000, diff->mpMax_);
        setMessage(0xc3c46, 0, 0, 0);
        BattleAutoFeed::setMessageSend();
    }
}

THUMB void BattleExecVictory12a::setup()
{
    TextAPI::setMACRO0(0x12, 0x50000000, index_);
    setMessage(0xc3c48, 0, 0, 0);
    BattleAutoFeed::setMessageSend();
}

THUMB void BattleExecVictory13::setup()
{
    status::BaseStatus* diff = status::g_Party.getPlayerStatus(index_)->haveStatusInfo_.haveStatus_.getDiffStatus();
    TextAPI::setMACRO1(0x52, 0xf0000000, diff->strength_);
    TextAPI::setMACRO2(0x52, 0xf0000000, diff->agility_);
    TextAPI::setMACRO3(0x52, 0xf0000000, diff->protection_);
    TextAPI::setMACRO4(0x52, 0xf0000000, diff->wisdom_);
    TextAPI::setMACRO5(0x52, 0xf0000000, diff->luck_);
    if (status::HaveAction::isBattleMode()) {
        setMessage(0xc3c4a, 0, 0, 0);
        BattleAutoFeed::setMessageSend();
    } else {
        setMessage(0xc3da9, 0, 0, 0);
        BattleAutoFeed::setMessageSend();
        if (status::g_Party.getLevelupPlayer() == -1) {
            MenuAPI::setMessageCursor(false);
        }
    }
}

THUMB void BattleExecVictory15::setup()
{
    TextAPI::setMACRO0(0x12, 0x50000000, playerIndex_);
    int word = status::UseAction::getWordDBIndex(actionIndex_[0]);
    TextAPI::setMACRO0(0x11, 0x70000000, word);
    TextAPI::setMACRO0(0, 0x70000000, word);
    setMessage(0xc3c4c, 0, 0, 0);
    BattleAutoFeed::setMessageSend();
    index_ = 1;
}

THUMB void BattleExecVictory15::exec()
{
    if (isEnd()) {
        if (actionIndex_[index_] != 0) {
            int word = status::UseAction::getWordDBIndex(actionIndex_[index_]);
            index_++;
            TextAPI::setMACRO0(0x12, 0x50000000, playerIndex_);
            TextAPI::setMACRO0(0x11, 0x70000000, word);
            TextAPI::setMACRO0(0, 0x70000000, word);
            setMessage(0xc3c4c, 0, 0, 0);
        } else {
            terminate();
        }
    }
}

THUMB void BattleExecVictory16::setup()
{
    if (status::g_Party.getLevelupPlayer() != -1) {
        counter_ = 1;
        SoundManager::stopBgm(0);
        SoundManager::playBgm(0x2f, 0);
    } else {
        counter_ = 0xb4;
    }
}

THUMB bool BattleExecVictory16::isEnd()
{
    if (counter_ > 0xb4) {
        return true;
    }
    counter_++;
    return false;
}

THUMB void BattleExecLevelup::initialize()
{
    ExecTaskManager::initialize();
    int index = status::g_Party.getLevelupPlayer();
    status::g_Party.getPlayerStatus(index)->haveStatusInfo_.levelup(0);
    status::g_Party.getPlayerStatus(index)->haveStatusInfo_.haveStatus_.getDiffStatus();
    battleExecVictory10.index_ = status::g_Party.getPlayerStatus(index)->haveStatusInfo_.haveStatus_.playerIndex_;
    battleExecVictory10.level_ = status::g_Party.getPlayerStatus(index)->haveStatusInfo_.haveStatus_.level_;
    int action = status::g_Party.getPlayerStatus(index)->haveStatusInfo_.haveAction_.getRememberingAction();
    if (action != 0) {
        resister(0, &battleExecVictory10);
        resister(1, &battleExecVictory11);
        resister(2, &battleExecVictory12);
        resister(3, &battleExecVictory12a);
        resister(4, &battleExecVictory13);
        resister(5, &battleExecVictory15);
        resister(6, &battleExecVictory16);
        int playerIndex = status::g_Party.getPlayerStatus(index)->haveStatusInfo_.haveStatus_.playerIndex_;
        battleExecVictory11.index_ = playerIndex;
        battleExecVictory12.index_ = playerIndex;
        battleExecVictory12a.index_ = playerIndex;
        battleExecVictory13.index_ = playerIndex;
        battleExecVictory15.playerIndex_ = playerIndex;
        battleExecVictory15.setActionIndex(0, action);
        battleExecVictory15.setActionIndex(1, status::g_Party.getPlayerStatus(index)->haveStatusInfo_.haveAction_.getRememberingAction());
        battleExecVictory15.setActionIndex(2, status::g_Party.getPlayerStatus(index)->haveStatusInfo_.haveAction_.getRememberingAction());
        battleExecVictory15.setActionIndex(3, status::g_Party.getPlayerStatus(index)->haveStatusInfo_.haveAction_.getRememberingAction());
        battleExecVictory15.setActionIndex(4, 0);
    } else {
        resister(0, &battleExecVictory10);
        resister(1, &battleExecVictory11);
        resister(2, &battleExecVictory12);
        resister(3, &battleExecVictory12a);
        resister(4, &battleExecVictory13);
        resister(5, &battleExecVictory16);
        int playerIndex = status::g_Party.getPlayerStatus(index)->haveStatusInfo_.haveStatus_.playerIndex_;
        battleExecVictory11.index_ = playerIndex;
        battleExecVictory12.index_ = playerIndex;
        battleExecVictory12a.index_ = playerIndex;
        battleExecVictory13.index_ = playerIndex;
    }
}

THUMB void BattleExecLevelup::terminate()
{
    ExecTaskManager::terminate();
}

THUMB BattleExecLevelup::~BattleExecLevelup()
{
}
