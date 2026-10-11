#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/CursorMoveGridLoop.hpp"
#include "main/status/UseActionParam.hpp"

// town operation menu root 
struct TownMenu_OPERATION_ROOT : menu::MenuBase
{
    menu::MenuItem menuItem_;               /* 0x1C */
    menu::MenuItem cancelItem_;             /* 0x80 */
    menu::MenuItem sortItem_;               /* 0xE4 */
    menu::MenuItem charaItem_;              /* 0x148 */
    int unk_1ac;                            /* 0x1AC */
    unsigned char mode_;                    /* 0x1B0 */
    char command_[9];                       /* 0x1B1 */
    unsigned char commandCount_;            /* 0x1BA */
    unsigned char abort_;                   /* 0x1BB */
    unsigned char saveCount_;               /* 0x1BC */
    int saveResult_;                        /* 0x1C0 */
    int isMantan_;                          /* 0x1C4 */
    int useKiari_;                          /* 0x1C8 */
    unsigned char seCount_;                 /* 0x1CC */
    unsigned short actionIndex_;            /* 0x1CE */
    CursorMoveGridLoop navigator_;          /* 0x1D0 */
    CursorMoveGridLoop sortNavigator_;      /* 0x1DC */
    CursorMoveGridLoop charaNavigator_;     /* 0x1E8 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void unkfunc_0217a278();
    void unkfunc_0217a2a0();
    void unkfunc_0217a318();
    void unkfunc_0217a3e0();
    void unkfunc_0217a438();
    int unkfunc_0217a4e4(char* command);
    void allRecoveryMessage(status::UseActionParam& useActionParam);
    void unkfunc_0217a67c();
};

extern TownMenu_OPERATION_ROOT gTownMenu_OPERATION_ROOT;

