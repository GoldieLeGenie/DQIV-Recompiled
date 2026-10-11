#pragma once
#include "globaldefs.h"
#include "GameInfo.hpp"
#include "main/menu/MenuBase.hpp"
#include "main/menu/UiMsg.hpp"
#include "main/menu/UnkMenuDisplay.hpp"
#include "main/menu/MessageWindow.hpp"
#include "main/menu/UnkMenuFaceDisplay.hpp"

struct TownMenu_MESSAGE : menu::MenuBase
{
    int keyEnable_;
    int yesNo_;
    int ynExec_;
    int yesNoCursor_;
    int ynPosX_;
    int ynPosY_;
    int yesNoSuperCancel_;
    int suspendInput_;
    menu::MenuItem yesNoItem_;
    int noClose_;

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();

    void openMessageForTEST();
    void openMessageForTALK();
    void openMessageForMENU();
    void openMessageForENCOUNT();
    void openMessageForBATTLE();
    void openMessage(eMessageWindow type);
    void addMessageNOWAIT(int messageID);
    void addMessage(int messageID);
    void addMessage(int messageID1, int messageID2);
    void addMessage(int messageID1, int messageID2, int messageID3);
    void addMessage(int messageID1, int messageID2, int messageID3, int messageID4);
    void addMessageCount(int messageID, int count);
    void addMessageSerial(int messageID);
    void addMessage(const char* message);
    void setYesNo();
    void setYesNo(int cursor);
    void setYesNoPosition(int x, int y);
    void setYesNoSuperCancel(bool flag);
    void restartMessage();
    void addMessageWAITKEY();
    bool isMessageWAITPROG();
    void clearMessageWAITPROG();
    void setMessageCursor(bool flag);
    void setMessageIntervalCursor(bool flag);
    void setMessageLastCursor(bool flag);
    void SetNoClose(bool flag);
};

extern TownMenu_MESSAGE data_020ed1bc; //gTownMenu_MESSAGE 
