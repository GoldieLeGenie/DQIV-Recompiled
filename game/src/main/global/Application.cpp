#include "main/global/Application.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/dss/Random.hpp"
#include "main/dss/SleepControl.hpp"

UnkApplication g_Application;

ARM void UnkApplication::unkfunc_02057f8c(int arg)
{
    initSystem(arg);
    clearVram();
    unkfunc_0207ecf4();
    unkfunc_0207e7e0();
    initSleep();
    dssrand::unkfunc_02080cec();
    vf00();
}

ARM void UnkApplication::unkfunc_02057fc4()
{
    g_GlobalDQ4.unkfunc_02058014(vf10());
}
