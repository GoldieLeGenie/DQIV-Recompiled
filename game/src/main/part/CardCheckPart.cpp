#include "main/part/CardCheckPart.hpp"
#include "main/part/GameEndPart.hpp"
#include "main/menu/CatalogView.hpp"

CardCheckPart g_CardCheckPart;

ARM void CardCheckPart::initialize()
{
    switch (profile::SaveLoad::unkfunc_0202c058()) {
    case 1:
        data_0210bb94.unkfunc_020580fc(MENU_PART);
        break;
    case -1:
        data_0210bb94.unkfunc_020580fc(MENU_PART);
        break;
    case -2:
        g_GameEndPart.unkfunc_0205d2ec(5, MENU_PART);
        break;
    case -3:
        g_GameEndPart.unkfunc_0205d2d0(1);
        break;
    case -4:
        g_GameEndPart.unkfunc_0205d2d0(4);
        break;
    case -5:
        g_GameEndPart.unkfunc_0205d2d0(3);
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
