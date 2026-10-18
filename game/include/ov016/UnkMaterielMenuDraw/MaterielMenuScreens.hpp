#pragma once
#include "globaldefs.h"

// materiel menu screen drawing (each one combines the window drawings of MaterielMenuDraw)
void drawShopRootScreen();
void drawShopBuyScreen(int* fukuroItemCount, int flag);
void drawShopWhoseScreen(int flag);
void drawShopWhoSellScreen();
void drawShopSellItemScreen();
void drawShopSellSackScreen();
void drawShopSellQuantityScreen(int quantity);
void drawShopScreen();
void drawChurchScreen(int activeCommand, int firstFlag);
void drawChurchMiracleScreen(int count);
void drawChangeGiftScreen();
void drawBankRootScreen();
void drawBankAmountScreen(int a, int b, int money);
void drawCoinSaleroomScreen(int coin, int flag);
void drawPokerHighLowScreen(int coin, int flag);
void drawSlotCoins(int coin, int flag);
void drawMonsterBookScreen(int* monsterName, int* monsterFlag);
void drawMonsterBookDetailScreen(int monsterNo, int monsterName);
void drawPresentExpScreen(int activeChara, int extraExp);
void drawHostageScreen();
void drawColosseumScreen();
void drawNeneSellScreen();
void drawNeneStorageScreen(int count, int page, int pageMax);
void drawChapterTitleScreen(int chapter, int chapterEnd);
void drawTagModeRootScreen();
void drawTagModeTenantScreen(int count, int page, int pageMax, int index, int flag, int select);
void drawTagModeTenantListScreen(int mode, int count, int page, int pageMax, int index);
void drawTagModeAmbassadorScreen(int mode, int active, int page, int value);
void drawNameEditScreen(int mode, int active);
void drawSideJobScreen();
void drawMoneyAndPartyWindows();
