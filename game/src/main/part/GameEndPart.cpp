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
    if (!g_TownMenuMessage.isOpen()) {
        g_TownMenuMessage.openMessageForMENU();
        g_TownMenuMessage.addMessage(message_);
    }
    if (nextPart_ == -1) {
        return;
    }
    if (g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
        g_GlobalDQ4.unkfunc_020580fc(nextPart_);
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

ARM void GameEndPart::setType(int type)
{
    setTypeAndNextPart(type, -1);
}

ARM void GameEndPart::setTypeAndNextPart(int type, int nextPart)
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

ARM void GameEndPart::start(int type)
{
    setType(type);
    g_GlobalDQ4.unkfunc_020580fc(GAME_END_PART);
}

ARM void GameEndPart::startWithNextPart(int type, int nextPart)
{
    setTypeAndNextPart(type, nextPart);
    g_GlobalDQ4.unkfunc_020580fc(GAME_END_PART);
}
