#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"

struct UnkTownMenu_02178a58 : menu::MenuBase {
    int unk_1c;     /* 0 = hidden, 1/2 = shown message */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void unkfunc_02178aa8(int flag);
};

extern UnkTownMenu_02178a58 gUnkTownMenu_02178a58;
