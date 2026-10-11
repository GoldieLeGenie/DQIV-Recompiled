#pragma once
#include "globaldefs.h"
#include "main/window/CommandWindow.hpp"

struct TownWindowSystem {
    window::CommandWindow cmdWindow_;           // 0x00
    int unk_70;                                 // 0x70
    int town_message_;                          // 0x74

    TownWindowSystem();
    static TownWindowSystem* getSingleton();
    void initialize();
    void terminate();
    void execute();
    void draw();
    void openMessage(int index, int count);
    void addCommonMessage(int index);
    void openCommonMessage();
    void serialCommonMessage(int index);
    void waitCommonMessage();
    void clearCommonMessage();
    bool isWait();
    bool isOpen();

    void changeShopMenuPhase(int type) { cmdWindow_.changeShopMenuPhase(type); }
};
