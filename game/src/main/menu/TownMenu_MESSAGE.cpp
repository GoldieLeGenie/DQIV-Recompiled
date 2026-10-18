#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/UnkMenuDisplays.hpp"
#include "main/dss/Pad.hpp"
#include "main/menu/CommonMenu_YESNO.hpp"
#include "main/cmn/CommonCounterInfo.hpp"
#include "main/status/GameStatus.hpp"
#include "main/debug/UnkDebugInfo.hpp"

int g_menuTextColors[5] = { 1, 7, 9, 2, 4 };

static MENUITEM_DATA yesNoItemData[] = {
    {1, 2, 0, 0, 0x100, 0xc0},
    {-1, -1, 0, 0, 0, 0},
};

THUMB void TownMenu_MESSAGE::menuSetup()
{
    yesNoItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
    keyEnable_ = 0;
    yesNo_ = 0;
    ynExec_ = 0;
    ynPosX_ = 0xc0;
    ynPosY_ = 0x40;
    yesNoSuperCancel_ = 1;
    suspendInput_ = 0;
    noClose_ = 0;
    data_020f1d88.unkfunc_0203cc0c();
}

THUMB void TownMenu_MESSAGE::menuExecute()
{
    if (ynExec_ == 0) {
        yesNoItem_.setMenuItem(yesNoItemData, 1, 1, 1);
    }
    if (noClose_ != 0) {
        UnkMenuFaceDisplay* const busy = &data_020f530c.face_;
        busy->mode_ = 11;
        busy->setPriority(0);
        busy->setPosition(0xe0, 0x164);
        busy->setEnable(1);
    }
}

THUMB void TownMenu_MESSAGE::menuDraw()
{
    s_draw->setEnable(1);
}

THUMB void TownMenu_MESSAGE::menuUpdate()
{
    if (frame_ >= 2 && suspendInput_ == 0) {
        if (ynExec_ == 0) {
            yesNoItem_.result_ = 0;
            yesNoItem_.lastresult_ = 0;
            yesNoItem_.execInput();
            int key = 0;
            if ((dss::g_Pad.edge() & 0x1) || (dss::g_Pad.edge() & 0x2) ||
                (dss::g_Pad.edge() & 0x400) || (dss::g_Pad.edge() & 0x800) ||
                (dss::g_Pad.edge() & 0x40) || (dss::g_Pad.edge() & 0x10) ||
                (dss::g_Pad.edge() & 0x80) || (dss::g_Pad.edge() & 0x20) ||
                (dss::g_Pad.edge() & 0x200) || (dss::g_Pad.edge() & 0x100)) {
                key = 1;
            }
            if (keyEnable_ != 0) {
                if (yesNo_ != 0) {
                    if (s_draw->isWaitTrigger() && key == 1) {
                        s_draw->clearWaitTriggerSE();
                        keyEnable_ = 0;
                    }
                    if (s_draw->isFinish()) {
                        data_020ed094.open();
                        data_020ed094.setYesNo(yesNoCursor_);
                        data_020ed094.setPosition(ynPosX_, ynPosY_);
                        data_020ed094.setSuperCancel(yesNoSuperCancel_);
                        ynExec_ = 1;
                        keyEnable_ = 0;
                    }
                } else {
                    if (s_draw->isWaitTrigger() && key == 1) {
                        s_draw->clearWaitTriggerSE();
                        keyEnable_ = 0;
                    }
                    if (s_draw->isFinish() && key == 1) {
                        s_draw->clearWaitTriggerSE();
                        keyEnable_ = 0;
                    }
                }
            } else {
                if (s_draw->isWaitTrigger() && key == 0) {
                    keyEnable_ = 1;
                }
                if (s_draw->isFinish() && key == 0) {
                    keyEnable_ = 1;
                }
            }
            if (s_draw->isEnd()) {
                stat_ = MENUBASE_STAT_OK;
            }
        } else {
            if (data_020ed094.stat_ == MENUBASE_STAT_OK) {
                stat_ = MENUBASE_STAT_OK;
            }
            if (data_020ed094.stat_ == MENUBASE_STAT_CANCEL) {
                stat_ = MENUBASE_STAT_CANCEL;
            }
        }
    }
}

THUMB void TownMenu_MESSAGE::openMessageForTEST()
{
    openMessage(TEST_MESSAGE_WINDOW);
}

THUMB void TownMenu_MESSAGE::openMessageForTALK()
{
    openMessage(TALK_MESSAGE_WINDOW);
}

THUMB void TownMenu_MESSAGE::openMessageForMENU()
{
    openMessage(MENU_MESSAGE_WINDOW);
}

THUMB void TownMenu_MESSAGE::openMessageForENCOUNT()
{
    openMessage(ENCOUNT_MESSAGE_WINDOW);
}

THUMB void TownMenu_MESSAGE::openMessageForBATTLE()
{
    openMessage(BATTLE_MESSAGE_WINDOW);
}

THUMB void TownMenu_MESSAGE::openMessage(eMessageWindow type)
{
    int language = 0;
    if (status::g_Game.language == Japanese) {
        language = 0;
    }
    if (status::g_Game.language == English) {
        language = 1;
    }
    if (status::g_Game.language == French) {
        language = 2;
    }
    if (status::g_Game.language == German) {
        language = 3;
    }
    if (status::g_Game.language == Italian) {
        language = 4;
    }
    if (status::g_Game.language == Spanish) {
        language = 5;
    }
    ui_MsgSetup(type, language);
    open();
    suspendInput_ = 0;
}

THUMB void TownMenu_MESSAGE::addMessageNOWAIT(int messageID)
{
    ui_MsgAdd(messageID);
    data_020f1d88.unkfunc_0203cc20(messageID);
}

THUMB void TownMenu_MESSAGE::addMessage(int messageID)
{
    if (messageID > 2000000) {
        addMessage((const char*)messageID);
        return;
    }
    ui_MsgAdd(messageID);
    ui_MsgAddWaitTrigger();
    data_020f1d88.unkfunc_0203cc20(messageID);
}

THUMB void TownMenu_MESSAGE::addMessage(int messageID1, int messageID2)
{
    addMessage(messageID1);
    addMessage(messageID2);
}

THUMB void TownMenu_MESSAGE::addMessage(int messageID1, int messageID2, int messageID3)
{
    addMessage(messageID1);
    addMessage(messageID2);
    addMessage(messageID3);
}

THUMB void TownMenu_MESSAGE::addMessage(int messageID1, int messageID2, int messageID3, int messageID4)
{
    addMessage(messageID1);
    addMessage(messageID2);
    addMessage(messageID3);
    addMessage(messageID4);
}

THUMB void TownMenu_MESSAGE::addMessageCount(int messageID, int count)
{
    for (int i = 0; i < count; i++) {
        addMessage(messageID + i);
    }
}

THUMB void TownMenu_MESSAGE::addMessageSerial(int messageID)
{
    ui_MsgAddSerial(messageID);
}

THUMB void TownMenu_MESSAGE::addMessage(const char* message)
{
    ui_MsgAdd(message);
    ui_MsgAddWaitTrigger();
    data_020f1d88.unkfunc_0203cc20(99999999);
}

THUMB void TownMenu_MESSAGE::setYesNo()
{
    yesNo_ = 1;
    yesNoCursor_ = 0;
}

THUMB void TownMenu_MESSAGE::setYesNo(int cursor)
{
    yesNo_ = 1;
    yesNoCursor_ = cursor;
}

THUMB void TownMenu_MESSAGE::setYesNoPosition(int x, int y)
{
    ynPosX_ = x;
    ynPosY_ = y;
}

THUMB void TownMenu_MESSAGE::setYesNoSuperCancel(bool flag)
{
    yesNoSuperCancel_ = flag;
}

THUMB void TownMenu_MESSAGE::restartMessage()
{
    stat_ = MENUBASE_STAT_ACTIVE;
    keyEnable_ = 0;
    yesNo_ = 0;
    ynExec_ = 0;
    ynPosX_ = 0xc0;
    ynPosY_ = 0x40;
    s_draw->restart();
    s_msgCount = 0;
    data_020f1d88.unkfunc_0203cc0c();
}

THUMB void TownMenu_MESSAGE::addMessageWAITKEY()
{
    ui_MsgAddWait();
}

THUMB bool TownMenu_MESSAGE::isMessageWAITPROG()
{
    return s_draw->isWaitProg();
}

THUMB void TownMenu_MESSAGE::clearMessageWAITPROG()
{
    s_draw->setWaitProg(0);
}

THUMB void TownMenu_MESSAGE::setMessageCursor(bool flag)
{
    s_draw->intervalCursor_ = flag;
    s_draw->lastCursor_ = flag;
}

THUMB void TownMenu_MESSAGE::setMessageIntervalCursor(bool flag)
{
    s_draw->intervalCursor_ = flag;
}

THUMB void TownMenu_MESSAGE::setMessageLastCursor(bool flag)
{
    s_draw->lastCursor_ = flag;
}

THUMB void TownMenu_MESSAGE::SetNoClose(bool flag)
{
    noClose_ = flag;
    UnkMenuFaceDisplay* obj = &data_020f530c.face_;
    if (noClose_ != 0) {
        obj->loadLoadingAnim();
        obj->mode_ = 11;
        return;
    }
    obj->mode_ = 0;
}
