#pragma once
#include "globaldefs.h"
#include "GameInfo.hpp"

struct MessageWindow;

void ui_MsgSystemInit();
void ui_MsgSetup(eMessageWindow type, int lang);
void ui_MsgAdd(int strNo);
void ui_MsgAddSerial(int no);
void ui_MsgAdd(const char* str);
void unkfunc_0205614c();
void ui_MsgAddWait();
void unkfunc_02056174();
void unkfunc_02056184(int cursor);
void unkfunc_02056194(int strNo);
void unkfunc_02056300();
int unkfunc_02056328(int no);
void ui_MsgSndSet(int no);
void ui_MsgSndSet(int* pms);
int ui_MsgSndGet();

extern MessageWindow* s_draw;
extern int s_msgCount;
