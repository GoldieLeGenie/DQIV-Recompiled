#include "main/global/UnkApplication.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/dss/Random.hpp"
#include "main/dss/UnkSleep.hpp"

UnkApplication data_0210bb78;

ARM void UnkApplication::unkfunc_02057f8c(int arg)
{
    unkfunc_0207e934(arg);
    unkfunc_0207e9f4();
    unkfunc_0207ecf4();
    unkfunc_0207e7e0();
    unkfunc_02089414();
    dssrand::unkfunc_02080cec();
    vf00();
}

ARM void UnkApplication::unkfunc_02057fc4()
{
    data_0210bb94.unkfunc_02058014(vf10());
}
