#include "main/dss/SleepControl.hpp"
#include "nitro/gx.h"
#include "nitro/os.hpp"
#include "nitro/pad.h"
#include "nitro/pm.h"

int g_sleepEnabled;
int g_lcdRequest;
int data_02120f94;
UnkPmSleepCallback g_preSleepCallback;
UnkPmSleepCallback g_postSleepCallback;

ARM void initSleep()
{
    g_sleepEnabled = 1;
    g_preSleepCallback.func_ = onPreSleep;
    g_preSleepCallback.arg_ = &data_02120f94;
    g_postSleepCallback.func_ = onPostSleep;
    g_postSleepCallback.arg_ = NULL;
    func_0207c1c4(&g_preSleepCallback);
    func_0207c1f4(&g_postSleepCallback);
}

ARM void updateSleep()
{
    if (g_sleepEnabled == 1 && PAD_DetectFold() == TRUE) {
        g_lcdRequest = 0;
        data_02120f94 = 0;
        func_0207bdb4(PM_TRIGGER_COVER_OPEN | PM_TRIGGER_CARD, 0, 0);
    }
    if (g_lcdRequest == 0) {
        return;
    }
    if (g_lcdRequest == 1) {
        OS_Wait();
        GX_DispOff();
        REG_DISPCNT_SUB &= ~0x10000;
        OS_Wait();
        if (func_0207c0b8(FALSE) != 0) {
            g_lcdRequest = 0;
        }
    } else if (g_lcdRequest == 2) {
        if (func_0207c0b8(TRUE) != 0) {
            OS_Wait();
            GX_DispOn();
            REG_DISPCNT_SUB |= 0x10000;
            OS_Wait();
            g_lcdRequest = 0;
        }
    }
}

ARM void setSleepEnabled(int enable)
{
    g_sleepEnabled = enable;
}

ARM int isSleepEnabled()
{
    return g_sleepEnabled;
}

ARM void onPreSleep(void* arg)
{
}

ARM void onPostSleep(void* arg)
{
}
