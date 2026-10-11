#pragma once
#include "globaldefs.h"
#include "main/status/HaveStatusInfo.hpp"

// town menu screen drawing (each one combines the window drawings of UnkTownMenuDraw_0217ad94)
void unkfunc_0217d560();
void unkfunc_0217d598(int chara);
void unkfunc_0217d5f4(int chara, int itemID, int page);
void unkfunc_0217d6e8(int chara, int itemID, int page, int sound);
void unkfunc_0217d7f8(int active, int itemID, int page);
void unkfunc_0217d870(int activeChara, int targetChara, int itemID, int sound);
void unkfunc_0217d900(int activeChara, int targetChara, int itemID, int page, int sound);
void unkfunc_0217d9cc(int mp1, int mp2, int magicID, int chara, int mode);
void unkfunc_0217da64(int mp, int useMp, int chara, int page);
void unkfunc_0217dac4(int mp, int useMp, int chara, int page, unsigned char* town);
void unkfunc_0217db80(int index, int page);
void unkfunc_0217dc18(int index, int page);
void unkfunc_0217dc6c(int index, int type, int page);
void unkfunc_0217dc80(int page);
void unkfunc_0217dcb0(int mode, char* command, int count, int chara);
void unkfunc_0217ddc8(char* chara, int index);
void unkfunc_0217de2c(char* chara, int index, int active);
void unkfunc_0217deb4(char* select, char* list, int active, int count);
void unkfunc_0217df54(int itemType, int chara, int page, unsigned char* list, int count, int active);
void unkfunc_0217e060(int chara, int flag, int mode, int fukuro);
void unkfunc_0217e0ac(int chara);
void unkfunc_0217e0e4(int index, int page);
void unkfunc_0217e144(int mode, int flag);
void unkfunc_0217e1bc(int page);
void unkfunc_0217e34c(int chara, int target);
void unkfunc_0217e388(int chara, int page, int target);
void unkfunc_0217e520(int page);
void unkfunc_0217e584(int page);
void unkfunc_0217e5a0(int chara);
void unkfunc_0217e628(int type);
void unkfunc_0217e6e8(int mode, char* command, int count);
void unkfunc_0217e7bc(char* order, int mode);
void unkfunc_0217e994(char* order);
void unkfunc_0217e9ec(int chara, unsigned char* list, int count, int itemType, int page);
void unkfunc_0217eb10(int chara, int index, int itemType);
void unkfunc_0217eb5c(int x, int y, int flag);
void unkfunc_0217eba4(int x, int y);
