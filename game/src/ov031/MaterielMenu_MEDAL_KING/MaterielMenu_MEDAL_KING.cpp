#pragma ipa file
#include "ov031/MaterielMenu_MEDAL_KING/MaterielMenu_MEDAL_KING.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/text/TextAPI.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"

THUMB void MaterielMenu_MEDAL_KING::menuSetup()
{
    status::g_Party.setPlayerMode();
    mode_ = -1;
    getReward_ = 0;
    systemMessage_ = 0;
    haveMedal_ = status::g_Party.playerMedalCoin_;
    status::g_Party.setPlayerMedalCoin(0);
    for (int i = 9; i >= 0; i--) {
        if (status::g_Party.medalCoin_ >= rewardCount_[i]) {
            nextRewardNo_ = i + 1;
            return;
        }
    }
}

THUMB void MaterielMenu_MEDAL_KING::menuUpdate()
{
    if (data_020ed1bc.isOpen()) {
        if ((unsigned int)(data_020ed1bc.stat_ - 1) <= 1) {
            if (systemMessage_ == 1) {
                TextAPI::setMACRO0(0xa, 0x40000000, rewardItem_[nextRewardNo_]);
                data_020ed1bc.restartMessage();
                data_020ed1bc.addMessage(0xc8329);
                if (status::g_Party.medalCoin_ >= 60) {
                    mode_ = 1;
                }
                nextRewardNo_++;
                systemMessage_ = 0;
            } else {
                data_020ed1bc.close();
                kingJudge();
            }
        }
    } else {
        selectMessage();
    }
}

THUMB void MaterielMenu_MEDAL_KING::selectMessage()
{
    bool depositMedal = false;
    if (status::g_Party.medalCoin_ != 0) {
        depositMedal = true;
    }
    if (status::g_Party.medalCoin_ >= 60) {
        haveAllReward();
        return;
    }
    data_020ed1bc.openMessageForTALK();
    if (depositMedal == false && haveMedal_ == 0) {
        TextAPI::setMACRO0(0x3c, 0xf0000000, rewardCount_[nextRewardNo_]);
        TextAPI::setMACRO0(0xa, 0x40000000, rewardItem_[nextRewardNo_]);
        data_020ed1bc.addMessage(0xc8322, 0xc8323, 0xc8336);
        mode_ = 2;
    } else if (depositMedal == false && haveMedal_ != 0) {
        TextAPI::setMACRO0(0x3a, 0xf0000000, haveMedal_);
        data_020ed1bc.addMessage(0xc8322, 0xc8323, 0xc8324, 0xc8325);
        mode_ = 0;
    } else if (depositMedal == true && haveMedal_ == 0) {
        TextAPI::setMACRO0(0x39, 0xf0000000, status::g_Party.medalCoin_);
        TextAPI::setMACRO0(0x3c, 0xf0000000, rewardCount_[nextRewardNo_]);
        TextAPI::setMACRO0(0xa, 0x40000000, rewardItem_[nextRewardNo_]);
        data_020ed1bc.addMessage(0xc8338, 0xc832d, 0xc8336);
        mode_ = 2;
    } else if (depositMedal == true && haveMedal_ != 0) {
        TextAPI::setMACRO0(0x39, 0xf0000000, haveMedal_ + status::g_Party.medalCoin_);
        data_020ed1bc.addMessage(0xc8338, 0xc833a, 0xc833b);
        mode_ = 0;
    }
}

THUMB void MaterielMenu_MEDAL_KING::kingJudge()
{
    switch (mode_) {
    case 0:
        if (judgeReward() == true) {
            getReward();
            return;
        }
        data_020ed1bc.openMessageForTALK();
        TextAPI::setMACRO0(0x39, 0xf0000000, status::g_Party.medalCoin_);
        TextAPI::setMACRO0(0x3c, 0xf0000000, rewardCount_[nextRewardNo_]);
        TextAPI::setMACRO0(0xa, 0x40000000, rewardItem_[nextRewardNo_]);
        if (getReward_) {
            data_020ed1bc.addMessage(0xc832d);
        }
        data_020ed1bc.addMessage(0xc8336);
        mode_ = 2;
        break;
    case 1:
        haveAllReward();
        break;
    case 2:
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        break;
    }
}

THUMB void MaterielMenu_MEDAL_KING::getReward()
{
    int nextCount = rewardCount_[nextRewardNo_];
    int itemID = rewardItem_[nextRewardNo_];
    bool havePlayer = false;
    getReward_ = 1;
    for (int i = 0; i < status::g_Party.getCount(); i++) {
        if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveItem_.getCount() != 12) {
            status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveItem_.add(itemID);
            havePlayer = true;
            break;
        }
    }
    if (havePlayer == false) {
        status::g_Party.haveItemSack_.add(itemID);
    }
    data_020ed1bc.openMessageForTALK();
    TextAPI::setMACRO0(0x3b, 0xf0000000, nextCount);
    TextAPI::setMACRO0(0xa, 0x40000000, itemID);
    data_020ed1bc.addMessage(0xc8328);
    systemMessage_ = 1;
}

THUMB void MaterielMenu_MEDAL_KING::haveAllReward()
{
    status::g_Party.addMedalCoin(haveMedal_);
    data_020ed1bc.openMessageForTALK();
    data_020ed1bc.addMessage(0xc8330, 0xc8331, 0xc8332, 0xc8333);
    mode_ = 2;
}

THUMB bool MaterielMenu_MEDAL_KING::judgeReward()
{
    int medal = status::g_Party.medalCoin_;
    int allMedal = medal + haveMedal_;
    if (allMedal >= rewardCount_[nextRewardNo_]) {
        haveMedal_ = haveMedal_ - (rewardCount_[nextRewardNo_] - medal);
        status::g_Party.setMedalCoin(rewardCount_[nextRewardNo_]);
        return true;
    }
    status::g_Party.addMedalCoin(haveMedal_);
    return false;
}

ARM void MaterielMenu_MEDAL_KING::menuDraw()
{
}

ARM void MaterielMenu_MEDAL_KING::menuExecute()
{
}

int MaterielMenu_MEDAL_KING::rewardCount_[11] = {15, 20, 25, 30, 34, 38, 43, 47, 52, 60, -1};
int MaterielMenu_MEDAL_KING::rewardItem_[11] = {0x6b, 0x65, 0x42, 0x17, 0x62, 0x1c, 0x5c, 0x6e, 0x51, 0x27, -1};
