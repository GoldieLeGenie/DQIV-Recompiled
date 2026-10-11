#pragma once
#include <globaldefs.h>
#include "main/object/DSSAObject.hpp"
#include "main/dss/TextureObject.hpp"

struct PaletteAnimationObject {
    PaletteAnimation anim_;             /* 0x00 */
    TextureObject* texture_;            /* 0x10 */
    int colorCount_;                    /* 0x14 */
    int frame_;                         /* 0x18 */
    int wait_;                          /* 0x1C */
    unsigned short color_[256];         /* 0x20 */

    PaletteAnimationObject() { texture_ = 0; }
    void unkfunc_0205b3d0();                            // start
    void unkfunc_0205b44c();                            // execute
    void unkfunc_0205b514(int address, int size);       // start
    void unkfunc_0205b584(int address, int size);       // execute
    int unkfunc_0205b628();                             // isEnd
    void unkfunc_0205b648();                            // cleanup
};
