#pragma once
#include "globaldefs.h"
#include "main/status/HaveStatusInfo.hpp"

// Sub screen windows drawn with the menu parts
void drawMenuFrame(int x, int y, int w, int h, int lineY);              // window frame (lineY > 0: separator)
void drawMenuFrameLine(int x, int y, int w);                                // separator line
void drawPartyStatusFrames();                                                    // party member frames
void unkfunc_0201e234();
void drawPartyStatusWindows();                                                    // party members, dead ones last
int getHpTextColorIndex(status::HaveStatusInfo* info);                        // color: 0 normal, 1 near death, 2 dead
int getStatusCondition(status::HaveStatusInfo* info);                        // condition: 2 dead, 3 poison, 4 spell
void drawMoneyWindow(int x, int y, int coin);                             // money window (-1, -1: default position)
void drawCharaStatusWindow(int index, int mode);                                // status window of a party member
void drawPartyMemberWindow(status::HaveStatusInfo* info, int pos);              // small status window of a party member
int getActionNameMessage(int action);                                          // action name message
void unkfunc_unused_13();
