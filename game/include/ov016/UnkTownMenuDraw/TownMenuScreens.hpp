#pragma once
#include "globaldefs.h"
#include "main/status/HaveStatusInfo.hpp"

// town menu screen drawing (each one combines the window drawings of TownMenuDraw)
void drawRootMenuScreen();
void drawItemCharaSelectScreen(int chara);
void drawItemSelectScreen(int chara, int itemID, int page);
void drawItemCommandScreen(int chara, int itemID, int page, int sound);
void drawItemUseTargetScreen(int active, int itemID, int page);
void drawItemTargetCharaScreen(int activeChara, int targetChara, int itemID, int sound);
void drawItemTargetItemScreen(int activeChara, int targetChara, int itemID, int page, int sound);
void drawMagicRootScreen(int mp1, int mp2, int magicID, int chara, int mode);
void drawMagicCharaSelectScreen(int mp, int useMp, int chara, int page);
void drawWarpTownSelectScreen(int mp, int useMp, int chara, int page, unsigned char* town);
void drawStatusMainPage(int index, int page);
void drawStatusSpellPage(int index, int page);
void drawStatusPartySummaryScreen(int index, int type, int page);
void drawStatusMoneyScreen(int page);
void drawOperationRootScreen(int mode, char* command, int count, int chara);
void drawTacticsScreen(char* chara, int index);
void drawTacticsSelectScreen(char* chara, int index, int active);
void drawShiftPartyScreen(char* select, char* list, int active, int count);
void drawEquipScreen(int itemType, int chara, int page, unsigned char* list, int count, int active);
void drawItemMenuHeader(int chara, int flag, int mode, int fukuro);
void drawMagicMenuHeader(int chara);
void drawStatusMenuHeader(int index, int page);
void drawCharaSelectWindow(int mode, int flag);
void drawStatusPartyIcons(int page);
void drawCharaItemList(int chara, int target);
void drawItemListPage(int chara, int page, int target);
void drawPartyHpMpList(int page);
void drawPartyHpMpListBattle(int page);
void drawFieldSpellList(int chara);
void drawPartySummaryStats(int type);
void drawTacticsCommandList(int mode, char* command, int count);
void drawPartyOrderIcons(char* order, int mode);
void drawPartyTacticsList(char* order);
void drawEquipItemList(int chara, unsigned char* list, int count, int itemType, int page);
void drawEquipItemInfo(int chara, int index, int itemType);
void drawRootMenuButtonFrames(int x, int y, int flag);
void drawSmallButtonFrame(int x, int y);
