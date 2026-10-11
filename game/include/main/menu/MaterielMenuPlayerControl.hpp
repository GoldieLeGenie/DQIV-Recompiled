#pragma once
#include "globaldefs.h"

struct MaterielMenuPlayerControl {
    int activeChara_;               /* 0x00 */
    int leadpc_;                    /* 0x04 */
    int activeItem_;                /* 0x08 */
    int activeItemPage_;            /* 0x0C */
    int churchCommandNum_;          /* 0x10 */
    int extraExp_;                  /* 0x14 */
    int wins_;                      /* 0x18 */
    int activeChiaus_;              /* 0x1C */
    int activeChiausPage_;          /* 0x20 */
    int activeChiausSex_;           /* 0x24 */
    int activeChiausAetas_;         /* 0x28 */
    int activeChiausSkill_;         /* 0x2C */
    int activeChiausSkillPage_;     /* 0x30 */
    int nameCount_;                 /* 0x34 */

    static MaterielMenuPlayerControl* getSingleton();
    void allClear();
    void setActiveItem(int item) { activeItem_ = item; }
    void setActiveItemPage(int page) { activeItemPage_ = page; }
    void setExtraExp(int exp) { extraExp_ = exp; }
    void setWins(int wins) { wins_ = wins; }
};
