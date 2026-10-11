#include "main/part/GameEndPart.hpp"
#include "main/global/Global.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"

GameEndPart g_GameEndPart;

ARM GameEndPart::GameEndPart()
{
    type_ = 1;
}

ARM void GameEndPart::initialize()
{
    g_Global.fadeIn(30);
}

ARM void GameEndPart::terminate()
{
}

ARM void GameEndPart::onExecutePart()
{
    if (type_ == 0) {
        return;
    }
    if (!data_020ed1bc.isOpen()) {
        data_020ed1bc.openMessageForMENU();
        data_020ed1bc.addMessage(message_);
    }
    if (nextPart_ == -1) {
        return;
    }
    if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
        data_0210bb94.unkfunc_020580fc(nextPart_);
        type_ = 0;
    }
}

ARM void GameEndPart::onDrawPart()
{
}

ARM void GameEndPart::onWindowPart()
{
}

ARM void GameEndPart::onDebugPart()
{
}

ARM void GameEndPart::unkfunc_0205d228(int type)
{
    unkfunc_0205d238(type, -1);
}

ARM void GameEndPart::unkfunc_0205d238(int type, int nextPart)
{
    type_ = type;
    nextPart_ = nextPart;
    switch (type_) {
    case 0:
        message_ = 0;
        break;
    case 1:
        message_ = 0xcb60b;
        break;
    case 2:
        message_ = 0xcb5fa;
        break;
    case 3:
        message_ = 0xcb608;
        break;
    case 4:
        message_ = 0xcb9e3;
        break;
    case 5:
        message_ = 0xcb60e;
        break;
    default:
        message_ = 0;
        break;
    }
}

ARM void GameEndPart::unkfunc_0205d2d0(int type)
{
    unkfunc_0205d228(type);
    data_0210bb94.unkfunc_020580fc(GAME_END_PART);
}

ARM void GameEndPart::unkfunc_0205d2ec(int type, int nextPart)
{
    unkfunc_0205d238(type, nextPart);
    data_0210bb94.unkfunc_020580fc(GAME_END_PART);
}
