#include "main/part/CardCheckPart.hpp"
#include "main/part/GameEndPart.hpp"
#include "main/menu/CatalogView.hpp"

CardCheckPart g_CardCheckPart;

ARM void CardCheckPart::initialize()
{
    switch (profile::SaveLoad::checkBackup()) {
    case 1:
        data_0210bb94.unkfunc_020580fc(MENU_PART);
        break;
    case -1:
        data_0210bb94.unkfunc_020580fc(MENU_PART);
        break;
    case -2:
        g_GameEndPart.startWithNextPart(5, MENU_PART);
        break;
    case -3:
        g_GameEndPart.start(1);
        break;
    case -4:
        g_GameEndPart.start(4);
        break;
    case -5:
        g_GameEndPart.start(3);
        break;
    }
}

ARM void CardCheckPart::terminate()
{
}

ARM void CardCheckPart::onExecutePart()
{
}

ARM void CardCheckPart::onDrawPart()
{
}

ARM void CardCheckPart::onWindowPart()
{
}

ARM void CardCheckPart::onDebugPart()
{
}
