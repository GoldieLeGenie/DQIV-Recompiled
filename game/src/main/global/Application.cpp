#include "main/global/Application.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/dss/Random.hpp"
#include "main/dss/SleepControl.hpp"

UnkApplication data_0210bb78;

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
    data_0210bb94.unkfunc_02058014(vf10());
}
