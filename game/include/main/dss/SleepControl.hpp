#pragma once
#include <globaldefs.h>
#include "nitro/pm.hpp"

extern int g_sleepEnabled;                       // sleep when the lid is closed
extern int data_02120f94;
extern int g_lcdRequest;                       // LCD request (1: off, 2: on)
extern UnkPmSleepCallback g_preSleepCallback;
extern UnkPmSleepCallback g_postSleepCallback;

void initSleep();                        // init
void updateSleep();                        // update (sleep, LCD power)
void setSleepEnabled(int enable);              // lid-close sleep enable
int isSleepEnabled();
void onPreSleep(void* arg);               // pre-sleep callback
void onPostSleep(void* arg);               // post-sleep callback
