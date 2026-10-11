#pragma once
#include <globaldefs.h>
#include "main/data/DataObject.hpp"
#include "main/object/DSSAObject.hpp"
#include "main/object/PaletteAnimationObject.hpp"

namespace btl {
    struct BattleEffectTransform {
        DataObject dataObject_;             /* 0x000 texture file */
        void* texture_;                     /* 0x010 */
        DataObject animData_;               /* 0x014 dssa file */
        DSSAObjectWithCamera dssaObject_;   /* 0x024 */
        DataObject paletteData_;            /* 0x0C4 pam file */
        PaletteAnimationObject paletteAnim_; /* 0x0D4 */
        int index_;                         /* 0x2F4 */
        int counter_;                       /* 0x2F8 */
        int process_;                       /* 0x2FC */
        int m_near;                         /* 0x300 */
        int enable_;                        /* 0x304 */
        int rev_;                           /* 0x308 */

        BattleEffectTransform();
        ~BattleEffectTransform();
        void setup(int index, int nearDist, int rev);
        void draw();
        void cleanup();
        int readNext();
        int isEnd();
    };
}
