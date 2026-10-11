#pragma once
#include "globaldefs.h"
#include "main/param/Param.hpp"
#include "GameInfo.hpp"

namespace cmn
{
    struct PartyTalk {
        struct PARTY_TALK_MESSAGE 
        {                                      
            int mesCount;
            int message;
            int yesno;
            int yesMesCount;
            int noMesCount;
            int playerIndex;
            int addMesCount;
        };
        param::PartyTalk *partyTalk_;
        int size_;
        PARTY_TALK_MESSAGE partyTalkMessage_[10];
        int mesCount_;
        int partyTalkNo_;
        int saveMessage_[20];
        int saveMessageCount_;
        int preMessage_[10];
        int preMessageCount_;
        int noChangeObjectLimitCount_;
        int floor_;
        int objectNo_;
        int lastExit_;
        int prevItem_;
        int carriageOutCount_;
        int saveMessageFlag_;
        int findPriorityMessage_;
        int getChapter6Data_;
        static cmn::PartyTalk s_partyTalk; //data_020ef9d4
        static cmn::PartyTalk* getSingleton();
        void initialize();
        void setPartyTalkMessage();
        void resetPartyTalk();
        void setPreMessageNo(int mesNo);
        void setObjectNo(int objNo);
        void setExitNo(int exitNo);
        void setPreItem(int itemId);
        void setPreMessage(int count, int message);
#ifdef DQIV_NATIVE
        // The party talk selection the European release left out (from the
        // mobile version).
        bool getPartyTalkData(int table);
        void getPartyTalk();
        bool checkCondition(int count, int conditionNo);
        static bool checkPlayer(int playerIndex, int playerCondition);
        static bool checkHaveItem(int itemIndex);
        void setPickMessage(int pick, int document);
        void saveMessageUpdate();
#endif
    };
}
