#pragma ipa file
#include "main/part/MessageDebugPart.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/global/Global.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Pad.hpp"
#include "main/dss/UnkVramTransfer.hpp"
#include "main/menu/MenuAPI.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/UnkMenuIconDisplay.hpp"
#include "main/status/GameStatus.hpp"
#include "nitro/os.hpp"
#include "main/menu/UnkMenuPartsDraw.hpp"

MessageDebugPart g_MessageDebugPart;

THUMB void MessageDebugPart::initialize()
{
    unkfunc_02080e90(&dss::g_DISPLAYPLUGIN_DOUBLE3D);
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
    data_0211e450.unkfunc_020861c4(0x40000, 0x4000);
    OS_Wait();
    g_Global.fadeIn(30);
}

THUMB void MessageDebugPart::terminate()
{
    unkfunc_02080e90(&dss::g_DISPLAYPLUGIN_STOP);
    data_0211e450.unkfunc_02086278();
}

THUMB void MessageDebugPart::onExecutePart()
{
    char path[128];

    if (MenuManager::isOpenMenu(&data_020ed1bc) == 0) {
        if (dss::g_Pad.edge() & 1) {
            isChanged_ = 1;
        }
    } else if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
        data_020ed1bc.close();
    }
    if (unkfunc_0200a644(4)) {
        MenuManager::setMenuEnable(1);
        isDisp_ = (isDisp_ == 0) ? 1 : 0;
        data_02116ce0.unkfunc_0207e810();
    }
    if (unkfunc_0200a644(0)) {
        step_ = 1;
    }
    if (unkfunc_0200a644(2)) {
        step_ = 10;
    }
    if (unkfunc_0200a644(0x100)) {
        step_ = 100;
    }
    if (unkfunc_0200a644(0x102)) {
        step_ = 1000;
    }
    if (unkfunc_0200a644(0x200)) {
        step_ = 10000;
    }
    if (unkfunc_0200a644(0x202)) {
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
            data_020ed1bc.openMessageForTEST();
            break;
        case 1:
            data_020ed1bc.openMessageForTALK();
            break;
        case 2:
            data_020ed1bc.openMessageForMENU();
            break;
        case 3:
            data_020ed1bc.openMessageForENCOUNT();
            break;
        case 4:
            data_020ed1bc.openMessageForBATTLE();
            break;
        }
        data_020ed1bc.addMessage(messageNo_);
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
        unkfunc_020827f0(0x23, 0, (unsigned char*)snap_.getAddr() + 0x14, 0x18000);
        snapState_++;
        break;
    case 4:
        unkfunc_020827f0(10, 0, (unsigned char*)snap_.getAddr() + 0x18014, 0x18000);
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
    unkfunc_0205077c(0, 0, 0x100, 0x78);
    data_02116ce0.unkfunc_0207e88c(1, 1, "Size(X)       %-10s", type[messageType_]);
    data_02116ce0.unkfunc_0207e88c(1, 2, "Lang(Y)       %-10s", lang[language_]);
    data_02116ce0.unkfunc_0207e88c(1, 3, "BACK(UP/DOWN) %-10s", back[back_]);
    data_02116ce0.unkfunc_0207e88c(1, 6, "Message %8d", messageNo_);
    data_02116ce0.unkfunc_0207e88c(1, 7, "*PAD Left/Right  +-%d      ", step_);
    data_02116ce0.unkfunc_0207e88c(1, 8, "  <PAD>           ... 1");
    data_02116ce0.unkfunc_0207e88c(1, 9, "  <PAD>  + B      ... 10");
    data_02116ce0.unkfunc_0207e88c(1, 10, "  <PAD>  + R      ... 100");
    data_02116ce0.unkfunc_0207e88c(1, 11, "  <PAD>  + B + R  ... 1000");
    data_02116ce0.unkfunc_0207e88c(1, 12, "  <PAD>  + L      ... 10000");
    data_02116ce0.unkfunc_0207e88c(1, 13, "  <PAD>  + B + L  ... 100000");
}

THUMB int MessageDebugPart::unkfunc_0200a644(int key)
{
    if (key == 0 && dss::g_Pad.pad() == 0) {
        return 1;
    }
    if (key == (dss::g_Pad.pad() & key)) {
        return 1;
    }
    return 0;
}
