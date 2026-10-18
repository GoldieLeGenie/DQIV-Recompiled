#pragma once
#include "globaldefs.h"

// town menu page drawing helpers 
void drawSettingScreenLabels();
void drawSettingLevelNumbers(int x, int y);
void drawCareerRecordLine(int value, int index, int bookOfBeasts, int heroLevel);
void drawCareerPlayTime(int time, int type);
void drawCareerTitle(int title);
void drawCareerHistoryType(int type);
void drawTownShopGoods(int index);
