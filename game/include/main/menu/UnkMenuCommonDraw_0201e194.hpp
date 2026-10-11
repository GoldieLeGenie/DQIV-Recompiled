#pragma once
#include "globaldefs.h"
#include "main/status/HaveStatusInfo.hpp"

// Sub screen windows drawn with the menu parts
void unkfunc_0201e194(int x, int y, int w, int h, int lineY);              // window frame (lineY > 0: separator)
void unkfunc_0201e1c4(int x, int y, int w);                                // separator line
void unkfunc_0201e1f4();                                                    // party member frames
void unkfunc_0201e234();
void unkfunc_0201e260();                                                    // party members, dead ones last
int unkfunc_0201e2f4(status::HaveStatusInfo* info);                        // color: 0 normal, 1 near death, 2 dead
int unkfunc_0201e318(status::HaveStatusInfo* info);                        // condition: 2 dead, 3 poison, 4 spell
void unkfunc_0201e350(int x, int y, int coin);                             // money window (-1, -1: default position)
void unkfunc_0201e3f4(int index, int mode);                                // status window of a party member
void unkfunc_0201e554(status::HaveStatusInfo* info, int pos);              // small status window of a party member
int unkfunc_0201e674(int action);                                          // action name message
void unkfunc_unused_13();
