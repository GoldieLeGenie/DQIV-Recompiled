#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/status/HaveStatusInfo.hpp"
#include "main/status/PlayerStatus.hpp"

// materiel menu window drawing (shop, bank, church, picture book, extra menus)
void drawShopRootCommands(bool bank, int type);
void drawPartsInTextColorAt(UnkMenuParts* parts, int* param, int x, int y, int mode);
void drawShopEquipCompare();
status::HaveStatusInfo* getPlayerHaveStatusInfo(status::PlayerStatus* player);
void drawShopHeldCounts();
void drawShopSackCount(int* count);
void drawPartsInTextColor(UnkMenuParts* parts, int* param, int mode);
void drawShopItemKind();
void drawShopBuyList(int flag);
void drawCharaItemNames();
void drawShopGiveCompare();
void drawPartyIcons(int y, int flag);
void drawSellPriceList();
void drawItemPage(int flag);
void drawSellPrice(int flag);
void drawItemPageNumber(int mode, int a, int b, int c);
void drawItemDetail(int item);
void drawSellQuantity(int quantity);
void drawPokerCoins(int coin, int flag);
void drawHighLowCommands(int win);
void drawMonsterBookPage(int* monsterName, int* monsterFlag);
void drawMonsterBookDetail(int monsterNo, int monsterName);
void drawChurchCommands();
void drawBankDigits(int money, int flag);
void drawActiveCharaHp(int flag);
void drawCommandCursor(int x, int y);
void drawCharaStatusList(int chara);
void drawActiveCharaHpMp();
void drawNeneItemPage(int count, int page, int pageMax);
void drawChapterTitleText(int chapter, int chapterEnd);
void drawTagModeRootMenu();
void drawTagModeTenantList(int a, int b, int c, int d);
void drawTagModeTenantDetail(int a, int b, int c);
void drawTagModeProfile(int a, int b);
void drawTagModeIconGrid(int page, int value);
void drawNameEditIcon(int active, int y);
void drawNameEditSex();
void drawNameEditAge();
void drawNameEditSkill();
void drawNameEditComment();
void drawSideJobItems();
void drawShopItemHeader(int chara);
int getShopEquipCompare(status::HaveStatusInfo* status, int* param, int item);
int getShopHeldCount(status::HaveStatusInfo* status, int* param, int item);
int activeCharaHasItem(int itemID);
void getSellListRow(int* param, int chara, int index);
void drawNameEditName(unsigned char* name, int flag);
void splitCommentLines(char* line1, char* line2, char* line3, char* comment);
#ifdef DQIV_NATIVE
// The shop's buy list with item icons, for the shop indices `rows` (a filter of the list).
void unkfunc_native_shopBuyList(const int* rows, int count);
int unkfunc_native_shopRowY(int row);
// The categories of the shop's buy list.
enum ShopFilter {
    SHOP_FILTER_ALL, SHOP_FILTER_WEAPON, SHOP_FILTER_ARMOR, SHOP_FILTER_SHIELD, SHOP_FILTER_HELMET,
    SHOP_FILTER_ACCESSORY, SHOP_FILTER_ITEM, SHOP_FILTER_COUNT
};
bool unkfunc_native_shopFilterMatch(int filter, int itemID);
const char* unkfunc_native_shopFilterName(int filter);
// A row of tabs above a menu window (x, w; 0x18 high at the top of the
// screen), the `active` one highlighted, with L and R at the ends. `frame`
// draws a window: drawMenuFrame or the like.
void unkfunc_native_tabs(const char* const* names, int count, int active, int x, int w, void (*frame)(int, int, int, int, int));
// The shop's category tabs.
void unkfunc_native_shopTabs(const int* filters, int count, int active);
#endif

