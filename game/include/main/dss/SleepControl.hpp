#pragma once
#include <globaldefs.h>
#include "nitro/pm.hpp"

extern int data_02120f90;                       // sleep when the lid is closed
extern int data_02120f94;
extern int data_02120f98;                       // LCD request (1: off, 2: on)
extern UnkPmSleepCallback data_02120f9c;
extern UnkPmSleepCallback data_02120fa8;

void initSleep();                        // init
void updateSleep();                        // update (sleep, LCD power)
void setSleepEnabled(int enable);              // lid-close sleep enable
int isSleepEnabled();
void onPreSleep(void* arg);               // pre-sleep callback
void onPostSleep(void* arg);               // post-sleep callback
