#include "main/menu/TownMenu_PARTY_TALK.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/cmn/PartyTalk.hpp"
#include "main/cmn/TalkSoundManager.hpp"
#include "main/cmn/HengeNoTsueManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/dss/Random.hpp"
#include "ov000/town/TownStageManager.hpp"


static inline cmn::PartyTalk::PARTY_TALK_MESSAGE* getTalkMessage()
{
    cmn::PartyTalk* partyTalk = cmn::PartyTalk::getSingleton();
    return &partyTalk->partyTalkMessage_[partyTalk->partyTalkNo_];
}

static inline int getMesCount()
{
    return getTalkMessage()->mesCount;
}

static inline int getMessageNo()
{
    return getTalkMessage()->message;
}

THUMB void TownMenu_PARTY_TALK::menuSetup()
{
    status::g_Party.setNormalMode();
    cmn::PartyTalk::getSingleton()->setPartyTalkMessage();

    int count = 0;
    int sortIndex = -1;
    int playerIndex = 0;
    int partyCount = status::g_Party.getCount();
    int mesCount = getTalkMessage()->mesCount;
    int talkPlayer = getTalkMessage()->playerIndex;
    int list[12] = { -1 };
    for (int i = 0; i < 12; i++) {
        list[i] = -1;
    }

    switch (status::g_Story.chapter_) {
        case 0:
            if (status::g_Story.sex_ == 0) {
                playerIndex = 1;
                sortIndex = status::g_Party.getSortIndex(playerIndex);
            } else {
                playerIndex = 2;
                sortIndex = status::g_Party.getSortIndex(playerIndex);
            }
            break;
        case 1:
            playerIndex = 3;
            break;
        case 2:
            playerIndex = 4;
            break;
        case 3:
            playerIndex = 7;
            break;
        case 4:
            playerIndex = 9;
            break;
        case 5:
        case 6:
            if (status::g_Story.sex_ == 0) {
                playerIndex = 1;
                sortIndex = status::g_Party.getSortIndex(playerIndex);
            } else {
                playerIndex = 2;
                sortIndex = status::g_Party.getSortIndex(playerIndex);
            }
            break;
    }

    for (int i = 0; i < partyCount; i++) {
        if (status::g_Party.isOutsideCarriage(i) == 1) {
            if (playerIndex != status::g_Party.getPlayerIndex(i) && !status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath()) {
                list[count] = i;
                count++;
            }
        } else {
            bool npc = status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.isNoBattleNpc_;
            if (npc == true) {
                list[count] = i;
                count++;
            }
        }
    }

    if (data_0210bb94.unkfunc_02058114(12) == 1 && TownStageManager::getSingleton()->isRozariStage() == 1) {
        int last = status::g_Party.getCount() - 1;
        if (status::g_Party.getPlayerStatus(last)->haveStatusInfo_.haveStatus_.playerIndex_ == 0x18) {
            list[count] = last;
            count++;
        }
    }

    unk_1c = 2;
    int select;
    if (count == 0) {
        select = 0;
    } else {
        select = dssrand::rand(count);
    }

    if (mesCount != 0 || talkPlayer != 0) {
        int i = 0;
        if (status::g_Party.getCount() > 1 && list[select] != -1) {
            TextAPI::setMACRO0(1, 0x50000000, status::g_Party.getPlayerStatus(list[select])->haveStatusInfo_.haveStatus_.playerIndex_);
        }
        setLeaderMacro(sortIndex);
        if (talkPlayer != 0) {
            int index = status::g_Party.getSortIndex(talkPlayer);
            if (index != -1 && !status::g_Party.getPlayerStatus(index)->haveStatusInfo_.isDeath() && status::g_Party.isOutsideCarriage(index) == 1) {
                mesCount += getTalkMessage()->addMesCount;
            }
        }
        int message = getTalkMessage()->message;
        ui_MsgSndSet(cmn::TalkSoundManager::MESSAGESOUND_STOP);
        if (message == 0xc3ddb) {
            ui_MsgSndSet(cmn::g_talkSound.getPlayerVoice(status::g_Party.getPlayerStatus(list[select])->haveStatusInfo_.haveStatus_.charaIndex_));
        }
        if (message != 0xc3df2 && message != 0xc3d97 && message != 0xc3d99 && message != 0x80092 && g_HengeNoTsue.change_ == 1) {
            ui_MsgSndSet(cmn::g_talkSound.getPlayerVoice(g_HengeNoTsue.charNo_));
        }
        data_020ed1bc.openMessageForTALK();
        for (; i < mesCount; i++) {
            data_020ed1bc.addMessage(message + i);
        }
        if (getTalkMessage()->yesno != 0) {
            data_020ed1bc.setYesNo();
            unk_1c = 1;
        }
    } else {
        close();
        gTownMenu_ROOT.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
    }
}

THUMB void TownMenu_PARTY_TALK::menuUpdate()
{
    if (data_020ed1bc.isOpen() != 0) {
        int i = 0;
        if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
            data_020ed1bc.close();
            if (unk_1c == 2) {
                close();
                gTownMenu_ROOT.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
                return;
            }
            unk_1c = 2;
            int message = getMessageNo() + getMesCount();
            data_020ed1bc.openMessageForTALK();
            for (; i < getTalkMessage()->yesMesCount; i++) {
                data_020ed1bc.addMessage(message + i);
            }
        } else if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            data_020ed1bc.openMessageForTALK();
            if (getTalkMessage()->noMesCount == 0) {
                int message = getMessageNo() + getMesCount();
                for (; i < getTalkMessage()->yesMesCount; i++) {
                    data_020ed1bc.addMessage(message + i);
                }
            } else {
                int yesMesCount = getTalkMessage()->yesMesCount;
                int mesCount = getTalkMessage()->mesCount;
                int message = mesCount + yesMesCount + getTalkMessage()->message;
                for (; i < getTalkMessage()->noMesCount; i++) {
                    data_020ed1bc.addMessage(message + i);
                }
            }
            unk_1c = 2;
        }
    }
}

THUMB void TownMenu_PARTY_TALK::setLeaderMacro(int sortIndex)
{
    if (sortIndex == -1) {
        return;
    }
    if (status::g_Party.isInsideCarriage(sortIndex) == 1 || status::g_Party.getPlayerStatus(sortIndex)->haveStatusInfo_.isDeath() == true) {
        int i = 0;
        int count = status::g_Party.getCarriageOutCount();
        for (; i < count; i++) {
            if (!status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath()) {
                break;
            }
        }
        if (status::g_Party.getPlayerIndex(i) == 7) {
            int found = 0;
            for (i++; i < count; i++) {
                if (!status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath()) {
                    found = i;
                    break;
                }
            }
            if (found != 0) {
                TextAPI::setMACRO0(14, 0x50000000, status::g_Party.getPlayerIndex(found));
            }
        } else {
            TextAPI::setMACRO0(14, 0x50000000, status::g_Party.getPlayerIndex(i));
        }
    }
}
