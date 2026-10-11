#include "main/dss/UnkSleep.hpp"
#include "nitro/gx.h"
#include "nitro/os.hpp"
#include "nitro/pad.h"
#include "nitro/pm.h"

int data_02120f90;
int data_02120f98;
int data_02120f94;
UnkPmSleepCallback data_02120f9c;
UnkPmSleepCallback data_02120fa8;

ARM void unkfunc_02089414()
{
    data_02120f90 = 1;
    data_02120f9c.func_ = unkfunc_02089578;
    data_02120f9c.arg_ = &data_02120f94;
    data_02120fa8.func_ = unkfunc_0208957c;
    data_02120fa8.arg_ = NULL;
    func_0207c1c4(&data_02120f9c);
    func_0207c1f4(&data_02120fa8);
}

ARM void unkfunc_02089470()
{
    if (data_02120f90 == 1 && PAD_DetectFold() == TRUE) {
        data_02120f98 = 0;
        data_02120f94 = 0;
        func_0207bdb4(PM_TRIGGER_COVER_OPEN | PM_TRIGGER_CARD, 0, 0);
    }
    if (data_02120f98 == 0) {
        return;
    }
    if (data_02120f98 == 1) {
        OS_Wait();
        GX_DispOff();
        REG_DISPCNT_SUB &= ~0x10000;
        OS_Wait();
        if (func_0207c0b8(FALSE) != 0) {
            data_02120f98 = 0;
        }
    } else if (data_02120f98 == 2) {
        if (func_0207c0b8(TRUE) != 0) {
            OS_Wait();
            GX_DispOn();
            REG_DISPCNT_SUB |= 0x10000;
            OS_Wait();
            data_02120f98 = 0;
        }
    }
}

ARM void unkfunc_02089558(int enable)
{
    data_02120f90 = enable;
}

ARM int unkfunc_02089568()
{
    return data_02120f90;
}

ARM void unkfunc_02089578(void* arg)
{
}

ARM void unkfunc_0208957c(void* arg)
{
}
