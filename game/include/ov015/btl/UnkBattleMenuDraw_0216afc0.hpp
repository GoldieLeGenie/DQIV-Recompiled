#pragma once
#include "globaldefs.h"
#include "main/status/HaveStatusInfo.hpp"

// battle menu drawing helpers: command icons, party status windows, monster name plates
void unkfunc_0216afc0(int command0, int command1, int command2, int command3);
void unkfunc_0216aff8(int x, int y, int hp, status::HaveStatusInfo* info);
void unkfunc_0216b07c(status::HaveStatusInfo* info, int hp, int flag);
void unkfunc_0216b0a0(int group);
void unkfunc_0216b134(int command, int x, int y, int flag);
void unkfunc_0216b254(int x, int y, int message);
void unkfunc_0216b27c(int x, int y, int max, int page);
void unkfunc_0216b2c4(int x, int y, int item, int flag);
void unkfunc_0216b314(int x, int y, int action);
void unkfunc_0216b344(status::HaveStatusInfo* info, int index);
void unkfunc_0216b460(int x, int y, status::HaveStatusInfo* info);
void unkfunc_0216b590(int* list, int count, int page);
void unkfunc_0216b638(status::HaveStatusInfo* info, int x, int y);
void unkfunc_0216b6cc();
void unkfunc_0216b6e0(status::HaveStatusInfo* info, int a, int b, int c, int index);
void unkfunc_0216b7d4(int index, int value, int message, int color);
void unkfunc_0216b858(int x, int y, int value, int type);
void unkfunc_0216b8d0(int mp, int useMp);
void unkfunc_0216b91c(int x, int y, int icon);
void unkfunc_0216b938(int x, int y, status::HaveStatusInfo* info, int flag);
int unkfunc_0216b980(status::HaveStatusInfo* info);
int unkfunc_0216b9a8(status::HaveStatusInfo* info, int hp);
void unkfunc_0216b9c8(int flag);
void unkfunc_0216b9e8(int flag);
