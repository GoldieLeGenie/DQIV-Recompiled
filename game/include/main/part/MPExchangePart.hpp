#pragma once
#include <globaldefs.h>
#include "main/global/GlobalGamePart.hpp"
#include "ov002/mp/MPExchange.hpp"
#include "nitro/pm.hpp"

// Surechigai: sends my envoy and receives the other player's one, then goes back to the town
struct MPExchangePart : GlobalGamePart {
    int state_;                                 // 0x04
    int nextState_;                             // 0x08
    int counter_;                               // 0x0C frames in the state
    MPExchange exchange_;                    // 0x10
    int lcdCounter_;                            // 0x2D8
    int known_;                                 // 0x2DC received envoy already in the town
    int message_;                               // 0x2E0

    MPExchangePart();
    virtual void initialize();
    virtual void terminate();
    virtual void onExecutePart();
    virtual void onDrawPart();
    virtual void onWindowPart();
    virtual void onDebugPart();
    void setState(int state);
    void updateState();
    void updateLcdPower();                    // LCD off while the lid is closed
    void playWaitSe();
    void restoreLcdAndSleep();
};

extern MPExchangePart g_MPExchangePart;
