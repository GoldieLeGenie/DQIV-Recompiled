#pragma once
#include <globaldefs.h>
#include "nitro/pm.hpp"

extern int data_02120f90;                       // sleep when the lid is closed
extern int data_02120f94;
extern int data_02120f98;                       // LCD request (1: off, 2: on)
extern UnkPmSleepCallback data_02120f9c;
extern UnkPmSleepCallback data_02120fa8;

void unkfunc_02089414();                        // init
void unkfunc_02089470();                        // update (sleep, LCD power)
void unkfunc_02089558(int enable);              // lid-close sleep enable
int unkfunc_02089568();
void unkfunc_02089578(void* arg);               // pre-sleep callback
void unkfunc_0208957c(void* arg);               // post-sleep callback
