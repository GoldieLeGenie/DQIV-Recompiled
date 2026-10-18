#pragma ipa file
#include "main/part/MessageDebugPart.hpp"
#include "main/dss/Display.hpp"
#include "main/global/Global.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Pad.hpp"
#include "main/dss/VramManager.hpp"
#include "main/menu/MenuAPI.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/UnkMenuIconDisplay.hpp"
#include "main/status/GameStatus.hpp"
#include "nitro/os.hpp"
#include "main/menu/MenuPartsDraw.hpp"

MessageDebugPart g_MessageDebugPart;

THUMB void MessageDebugPart::initialize()
{
    changeDisplayPlugin(&dss::g_DISPLAYPLUGIN_DOUBLE3D);
    unk_20 = 0;
    snapState_ = 0;
    isDisp_ = 0;
    messageNo_ = 0;
    messageType_ = 1;
    language_ = 0;
    if (status::g_Game.language == Japanese) {
        language_ = 0;
    }
    if (status::g_Game.language == English) {
        language_ = 1;
    }
    if (status::g_Game.language == French) {
        language_ = 2;
    }
    if (status::g_Game.language == German) {
        language_ = 3;
    }
    if (status::g_Game.language == Italian) {
        language_ = 4;
    }
    if (status::g_Game.language == Spanish) {
        language_ = 5;
    }
    step_ = 1;
    back_ = 1;
    isChanged_ = 0;
    g_VramTransfer.initVramManagers(0x40000, 0x4000);
    OS_Wait();
    g_Global.fadeIn(30);
}

THUMB void MessageDebugPart::terminate()
{
    changeDisplayPlugin(&dss::g_DISPLAYPLUGIN_STOP);
    g_VramTransfer.unkfunc_02086278();
}

THUMB void MessageDebugPart::onExecutePart()
{
    char path[128];

    if (MenuManager::isOpenMenu(&g_TownMenuMessage) == 0) {
        if (dss::g_Pad.edge() & 1) {
            isChanged_ = 1;
        }
    } else if (g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
        g_TownMenuMessage.close();
    }
    if (isKeyHeld(4)) {
        MenuManager::setMenuEnable(1);
        isDisp_ = (isDisp_ == 0) ? 1 : 0;
        g_DebugConsole.unkfunc_0207e810();
    }
    if (isKeyHeld(0)) {
        step_ = 1;
    }
    if (isKeyHeld(2)) {
        step_ = 10;
    }
    if (isKeyHeld(0x100)) {
        step_ = 100;
    }
    if (isKeyHeld(0x102)) {
        step_ = 1000;
    }
    if (isKeyHeld(0x200)) {
        step_ = 10000;
    }
    if (isKeyHeld(0x202)) {
        step_ = 100000;
    }
    if (dss::g_Pad.edge() & 0x20) {
        isChanged_ = 1;
        messageNo_ -= step_;
    }
    if (dss::g_Pad.edge() & 0x10) {
        isChanged_ = 1;
        messageNo_ += step_;
    }
    if (dss::g_Pad.edge() & 0x400) {
        isChanged_ = 1;
        messageType_++;
        if (messageType_ == 5) {
            messageType_ = 1;
        }
    }
    if (dss::g_Pad.edge() & 0x800) {
        isChanged_ = 1;
        language_++;
        if (language_ == 6) {
            language_ = 0;
        }
        if (language_ == 0) {
            status::g_Game.language = Japanese;
        }
        if (language_ == 1) {
            status::g_Game.language = English;
        }
        if (language_ == 2) {
            status::g_Game.language = French;
        }
        if (language_ == 3) {
            status::g_Game.language = German;
        }
        if (language_ == 4) {
            status::g_Game.language = Italian;
        }
        if (language_ == 5) {
            status::g_Game.language = Spanish;
        }
    }
    if (snapState_ == 6) {
        if (dss::g_Pad.edge() & 0x40) {
            back_++;
            snapState_ = 2;
        }
        if (dss::g_Pad.edge() & 0x80) {
            back_--;
            snapState_ = 2;
        }
        if (back_ < 1) {
            back_ = 1;
        }
        if (back_ > 6) {
            back_ = 6;
        }
    }
    if (messageNo_ < 0) {
        messageNo_ = 0;
    }
    if (messageNo_ > 2000000) {
        messageNo_ = 2000000;
    }
    if (isChanged_ == 1) {
        switch (messageType_) {
        case 0:
            g_TownMenuMessage.openMessageForTEST();
            break;
        case 1:
            g_TownMenuMessage.openMessageForTALK();
            break;
        case 2:
            g_TownMenuMessage.openMessageForMENU();
            break;
        case 3:
            g_TownMenuMessage.openMessageForENCOUNT();
            break;
        case 4:
            g_TownMenuMessage.openMessageForBATTLE();
            break;
        }
        g_TownMenuMessage.addMessage(messageNo_);
        isChanged_ = 0;
    }
    switch (snapState_) {
    case 0:
        MenuAPI::changeMenuModeExtra();
        snapState_++;
        break;
    case 1:
        if (MenuAPI::isMenuModeExtra()) {
            snapState_++;
        }
        break;
    case 2: {
        dss::sprintf_s(path, sizeof(path), "data/G2D/SNAP/snap%03d.tim", back_);
        snap_.setup(path, 0, 0);
        unsigned short* p = (unsigned short*)snap_.getAddr();
        for (int i = 0; i < 0x18000; i++) {
            *p++ |= 0x8000;
        }
        snapState_++;
        break;
    }
    case 3:
        requestVramCopy(0x23, 0, (unsigned char*)snap_.getAddr() + 0x14, 0x18000);
        snapState_++;
        break;
    case 4:
        requestVramCopy(10, 0, (unsigned char*)snap_.getAddr() + 0x18014, 0x18000);
        snapState_++;
        break;
    case 5:
        snap_.cleanup();
        snapState_++;
        break;
    }
}

THUMB void MessageDebugPart::onDrawPart()
{
}

THUMB void MessageDebugPart::onWindowPart()
{
}

THUMB void MessageDebugPart::onDebugPart()
{
    if (isDisp_ == 0) {
        return;
    }
    const char* type[] = { "TEST", "12 TALK", "12 MENU", "10 ENCOUNT", "10 BATTLE " };
    const char* lang[] = { "Japanese  ", "English   ", "French    ", "German    ", "Italian   ", "Spanish   " };
    const char* font[] = { "Default   ", "10L       ", "10L b     ", "10S       " };
    const char* back[] = { "------    ", "MAP 1     ", "MAP 2     ", "BATTLE    " };
    drawMenuWindowBox(0, 0, 0x100, 0x78);
    g_DebugConsole.unkfunc_0207e88c(1, 1, "Size(X)       %-10s", type[messageType_]);
    g_DebugConsole.unkfunc_0207e88c(1, 2, "Lang(Y)       %-10s", lang[language_]);
    g_DebugConsole.unkfunc_0207e88c(1, 3, "BACK(UP/DOWN) %-10s", back[back_]);
    g_DebugConsole.unkfunc_0207e88c(1, 6, "Message %8d", messageNo_);
    g_DebugConsole.unkfunc_0207e88c(1, 7, "*PAD Left/Right  +-%d      ", step_);
    g_DebugConsole.unkfunc_0207e88c(1, 8, "  <PAD>           ... 1");
    g_DebugConsole.unkfunc_0207e88c(1, 9, "  <PAD>  + B      ... 10");
    g_DebugConsole.unkfunc_0207e88c(1, 10, "  <PAD>  + R      ... 100");
    g_DebugConsole.unkfunc_0207e88c(1, 11, "  <PAD>  + B + R  ... 1000");
    g_DebugConsole.unkfunc_0207e88c(1, 12, "  <PAD>  + L      ... 10000");
    g_DebugConsole.unkfunc_0207e88c(1, 13, "  <PAD>  + B + L  ... 100000");
}

THUMB int MessageDebugPart::isKeyHeld(int key)
{
    if (key == 0 && dss::g_Pad.pad() == 0) {
        return 1;
    }
    if (key == (dss::g_Pad.pad() & key)) {
        return 1;
    }
    return 0;
}
