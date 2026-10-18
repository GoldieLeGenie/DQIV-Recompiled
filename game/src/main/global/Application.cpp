#include "main/global/Application.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/dss/Random.hpp"
#include "main/dss/SleepControl.hpp"

DssApplication g_Application;

ARM void DssApplication::unkfunc_02057f8c(int arg)
{
    initSystem(arg);
    clearVram();
    unkfunc_0207ecf4();
    unkfunc_0207e7e0();
    initSleep();
    dssrand::unkfunc_02080cec();
    vf00();
}

ARM void DssApplication::unkfunc_02057fc4()
{
    g_GlobalDQ4.unkfunc_02058014(vf10());
}
