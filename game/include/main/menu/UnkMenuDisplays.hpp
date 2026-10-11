#pragma once
#include <globaldefs.h>
#include "main/menu/UnkMenuFrameDisplay.hpp"
#include "main/menu/UnkMenuFaceDisplay.hpp"
#include "main/menu/UnkMenuIconDisplay.hpp"
#include "main/menu/UnkMenuDummyDisplay.hpp"
#include "main/menu/MessageWindow.hpp"
#include "main/menu/UnkMenuYesNoDisplay.hpp"
#include "main/menu/UnkMenuTextDisplay.hpp"
#include "main/menu/UnkMenuWindowFrame.hpp"
#include "main/menu/UnkMenuSpriteDisplay.hpp"
#include "main/menu/UnkMenuArrowDisplay.hpp"

// Menu display objects, set up, updated and drawn together by unkfunc_0204e6e4...unkfunc_0204ec38
struct UnkMenuDisplays {
    UnkMenuFrameDisplay frames_;                // 0x0000
    UnkMenuFaceDisplay face_;                   // 0x1034
    UnkMenuIconDisplay icon_;                   // 0x1070
    UnkMenuDummyDisplay dummy_;                 // 0x10E0
    MessageWindow message_;                     // 0x1110
    UnkMenuYesNoDisplay yesNo_;                 // 0x2B04
    UnkMenuTextDisplay texts_[4];               // 0x2E5C
    UnkMenuWindowFrame windowFrame1_;           // 0x540C
    UnkMenuWindowFrame windowFrame2_;           // 0x545C
    UnkMenuSpriteDisplay sprite_;               // 0x54AC
    UnkMenuArrowDisplay arrow_;                 // 0x551C
    UnkHoppingNumber hopping_;                  // 0x565C
};

extern UnkMenuDisplays data_020f530c;
