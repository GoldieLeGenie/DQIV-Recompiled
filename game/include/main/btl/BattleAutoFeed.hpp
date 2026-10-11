#pragma once
#include <globaldefs.h>

struct BattleAutoFeed {
    enum {
        MESSAGE_SPEED_1     = 0,
        MESSAGE_SPEED_2     = 1,
        MESSAGE_SPEED_3     = 2,
        MESSAGE_SPEED_4     = 3,
        MESSAGE_SPEED_5     = 4,
        MESSAGE_SPEED_DEBUG = 5,
    };
    enum {
        WAIT_FRAME_0 = 0x18,
        WAIT_FRAME_1 = 0x2c,
        WAIT_FRAME_2 = 0x40,
        WAIT_FRAME_3 = 0x54,
        WAIT_FRAME_4 = -1,
    };

    static int counter_;
    static int encountCounter_;
    static int executeCounter_;
    static int resultCounter_;
    static int afterCounter_;
    static int speed_;
    static int waitCounter_;
    static int DEBUG_WAIT;

    static void setMessage();
    static void setMessageSend();
    static void setEncountMessage();
    static void setExecuteMessage();
    static void setResultMessage();
    static void setAfterMessage();
    static void setMessageSpeed();
    static bool isEndMessage();
    static bool isEndMessageSend();
    static bool isEndEncountMessage();
    static bool isEndExecuteMessage();
    static bool isEndResultMessage();
    static bool isEndAfterMessage();
    static int getMessageSpeed();
    static void setCursor();
    static void setCursorWaiting();
    static void setCursorBattleEnd();
    static bool isFinish();
    static bool isNext();
    static bool isEnd();
    static bool isEndBattleEnd();
    static void sendNext();
    static void printCounter();
    static void disableAutoFeed();
    static void setDisableCursor(bool flag);
};
