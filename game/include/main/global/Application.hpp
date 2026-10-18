#pragma once
#include <globaldefs.h>
#include "main/dss/DssCore.hpp"

// Base of the DS application object (main() runs data_020c4fe4, a UnkGameApplication).
// The default virtuals (0x0200885c-0x02008874) and the vtable (0x020bb970) are defined in UnkGameApplication.cpp.
struct UnkApplication {
    virtual void vf00();                        // called by unkfunc_02057f8c after the system init
    virtual void vf04();  
    virtual void vf08();  
    virtual void vf0c();  
    virtual int vf10();                          // first task given to GlobalDQ4
    virtual void vf14();  

    void unkfunc_02057f8c(int arg);
    void unkfunc_02057fc4();
};

extern UnkApplication data_0210bb78;

