#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/status/HaveStatusInfo.hpp"

// town menu window drawing (root, item, magic, status, operation menus)
void unkfunc_0217ad94(int type, int x);
void unkfunc_0217aee0(status::HaveStatusInfo* info, int x, int y);
void unkfunc_0217af90(int index, int x, int y);
void unkfunc_0217afc4(status::HaveStatusInfo* info);
void unkfunc_0217b038();
void unkfunc_0217b0c4(status::HaveStatusInfo* info, int flag, int index, int fukuro);
void unkfunc_0217b184(status::HaveStatusInfo* info);
void unkfunc_0217b1ec(int item, int x, int y);
void unkfunc_0217b254(int item, int index, int x, int y);
void unkfunc_0217b2b4(int index);
void unkfunc_0217b300(int x, int pageMax, int page, int flag);
void unkfunc_0217b368(int x, int pageMax, int page);
void unkfunc_0217b3b0(int index, int flag, int mode);
void unkfunc_0217b40c(int item);
void unkfunc_0217b488(status::HaveStatusInfo* info, int item, int equipIndex, int flag);
void unkfunc_0217b5b0(int mode);
void unkfunc_0217b70c(int mode, int flag);
void unkfunc_0217b908(status::HaveStatusInfo* info, int index, int flag);
void unkfunc_0217ba8c(status::HaveStatusInfo* info, int pageMax, int page);
void unkfunc_0217bae0(status::HaveStatusInfo* info);
void unkfunc_0217bb74(int x, int y, int value, int flag, status::HaveStatusInfo* info);
void unkfunc_0217bbcc(int page, unsigned char* rura);
void unkfunc_0217bc9c(status::HaveStatusInfo* info);
void unkfunc_0217bdb8(status::HaveStatusInfo* info);
void unkfunc_0217c290(status::HaveStatusInfo* info, int flag);
void unkfunc_0217c3e0(status::HaveStatusInfo* info, int battle);
void unkfunc_0217c4f0();
void unkfunc_0217c658();
void unkfunc_0217c7f0();
void unkfunc_0217c8bc(int number, int x, int y, int index);
void unkfunc_0217c918(int x, int y, int value);
void unkfunc_0217c94c(int x, int y, int value);
void unkfunc_0217c974(status::HaveStatusInfo* info, int x, int y);
void unkfunc_0217c9ec();
void unkfunc_0217ca5c(char* order, char* list, int count);
void unkfunc_0217cb3c(char* order);
void unkfunc_0217cc4c();
void unkfunc_0217cc6c();
void unkfunc_0217cc8c();
void unkfunc_0217ccac();
void unkfunc_0217cce4(int x, int y);
void unkfunc_0217ccf8(status::HaveStatusInfo* info, int item, int flag);
void unkfunc_0217cea8(int mode);
void unkfunc_0217cf1c(int mode);
int unkfunc_0217cf70(status::HaveStatusInfo* info);
int unkfunc_0217cf78(status::HaveStatusInfo* info);
void unkfunc_0217cf80(int* param, int x, int y);
void unkfunc_0217cf98(int* param, int x, int y, int flag);
void unkfunc_0217cfe8(int* param, int mode, int flag, int drawFlag);
void unkfunc_0217d054(status::HaveStatusInfo* info, int index, int x, int y);
int unkfunc_0217d140(status::HaveStatusInfo* info, int item, int* param);
int unkfunc_0217d2e8(status::HaveStatusInfo* info, int equipIndex, int* param);
void unkfunc_0217d454(status::HaveStatusInfo* info, int* param, int flag);
void unkfunc_0217d4f0(status::HaveStatusInfo* info, int* param, int flag);
