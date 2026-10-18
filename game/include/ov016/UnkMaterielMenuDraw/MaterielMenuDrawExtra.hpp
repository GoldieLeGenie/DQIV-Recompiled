#pragma once
#include "globaldefs.h"

// materiel menu page drawing helpers (casino coin window, save/load diary, fight stadium, name entry)
void drawFrameStyleA(int x, int y, int w, int h, int lineY);
void drawFrameStyleB(int x, int y, int w, int h, int lineY);
void drawFightStadiumLayout(int flag);
void drawFightStadiumPanels(bool flag);
void drawMessageColumn(int* message, int count, int x, int y);
void drawMessageColumnSmall(int* message, int count, int x, int y);
void drawPokerCoinsAndBet(int coin, int bet, int a, int b);
void drawPokerHoldLabels(int* cards, int a);
void drawPokerPayoutTable(int coin, int a, int b);
void drawFightStadiumEntry(int monsterID, int diameter, int index, int orderCount);
void drawFightStadiumCursor(int index);
void drawFightStadiumBetDigits(int coin, int x, int y, int flag);
void drawCenteredMessage(int* message, int x, int y, int w, int h);
void drawJournalSlot(int index, char* name, int chapter, int level, int town, int time, int y, int flag);
void drawNameEditCharTable(int* name, int type);
void drawNameEditField(int country, int* name, int count, int max, int mode, int flag);
void drawNameEditCommentField(int* name, int count);
