#pragma once
#include <globaldefs.h>
#include "main/menu/MenuFrameDisplay.hpp"
#include "main/menu/MenuFaceDisplay.hpp"
#include "main/menu/MenuIconDisplay.hpp"
#include "main/menu/MenuDummyDisplay.hpp"
#include "main/menu/MessageWindow.hpp"
#include "main/menu/MenuYesNoDisplay.hpp"
#include "main/menu/MenuTextDisplay.hpp"
#include "main/menu/MenuWindowFrame.hpp"
#include "main/menu/MenuSpriteDisplay.hpp"
#include "main/menu/MenuArrowDisplay.hpp"

// Menu display objects, set up, updated and drawn together by setupMenuDisplays...resetMenuDisplays
struct MenuDisplays {
    MenuFrameDisplay frames_;                // 0x0000
    MenuFaceDisplay face_;                   // 0x1034
    MenuIconDisplay icon_;                   // 0x1070
    MenuDummyDisplay dummy_;                 // 0x10E0
    MessageWindow message_;                     // 0x1110
    MenuYesNoDisplay yesNo_;                 // 0x2B04
    MenuTextDisplay texts_[4];               // 0x2E5C
    MenuWindowFrame windowFrame1_;           // 0x540C
    MenuWindowFrame windowFrame2_;           // 0x545C
    MenuSpriteDisplay sprite_;               // 0x54AC
    MenuArrowDisplay arrow_;                 // 0x551C
    HoppingNumberDisplay hopping_;                  // 0x565C
};

extern MenuDisplays g_MenuDisplays;
