#pragma once
#include "globaldefs.h"
#include "main/param/Param.hpp"
#include "GameInfo.hpp"
#include "main/menu/UiMsg.hpp"

struct TownCharacterManager;

namespace cmn
{
    struct TalkSoundManager
    {
        enum MESSAGESOUND{
            MESSAGESOUND_NONE=48,
            MESSAGESOUND_HIGH=49,
            MESSAGESOUND_MIDDLE=50,
            MESSAGESOUND_LOW=51,
            MESSAGESOUND_STOP=57
        };
        MESSAGESOUND orderSound_[33];
        int orderDataCount_;
        int index_;
        int charaNo_;
        void setup();
        void setVoice(int charaNo);
        void setMessageSound(int count, int index);
        MESSAGESOUND getOrderMessageSound();
        MESSAGESOUND getDefaultMessageSound();
        void setOrderMessage(MESSAGESOUND sound);
        MESSAGESOUND getCharacterVoice(int charaNo);
        MESSAGESOUND getPlayerVoice();
        MESSAGESOUND getPlayerVoice(int charaNo);
        MESSAGESOUND getMostHeroicVoice();
    };

    extern TalkSoundManager g_talkSound;    // data_020efcc0
}

