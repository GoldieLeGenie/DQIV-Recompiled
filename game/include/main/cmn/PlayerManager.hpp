#pragma once
#include "globaldefs.h"
#include "GameInfo.hpp"
#include "main/dss/DssUtils.hpp"


namespace cmn {
    struct PlayerManager{
        virtual void setPosition(dss::Fix32Vector3& pos) = 0;
        virtual dss::Fix32Vector3 getPosition() = 0;
        virtual short getDirection() = 0;
        virtual void resetParty() {}
        int flagMapLink_;  
        int charaColl_;

        static PLAYER_COMMAND checkCommand_;
        static PLAYER_COMMAND command_;
        static int locked_;

        PlayerManager();
        ~PlayerManager();
        static void initLock(void);
        static void setLock(int flag);
        static bool isLock();
        int getLockCount();
        static void setPlayerCommand(PLAYER_COMMAND command);
        static PLAYER_COMMAND getPlayerCommand();
        void checkCommandEnd();
    };
}

