#pragma once
#include "globaldefs.h"
#include "main/window/CommandWindow.hpp"

struct FieldWindowSystem {
    window::CommandWindow cmdWindow_;           // 0x00
    int message_enable_;                        // 0x70

    FieldWindowSystem();
    static FieldWindowSystem* getSingleton();
    void initialize();
    void terminate();
    void execute();
    void draw();
    void openMessage(int index, int count);
    void addCommonMessage(int index);
    void openCommonMessage();
    bool isOpen();
    void setMenuPermit(bool flag);
    void clearAllMap();
};
