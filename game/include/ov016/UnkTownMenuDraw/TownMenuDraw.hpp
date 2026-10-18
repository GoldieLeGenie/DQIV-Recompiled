#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/status/HaveStatusInfo.hpp"

// town menu window drawing (root, item, magic, status, operation menus)
void drawRootMenuCommands(int type, int x);
void drawPartyMemberIcon(status::HaveStatusInfo* info, int x, int y);
void unkfunc_0217af90(int index, int x, int y);
void drawCharaItemNameList(status::HaveStatusInfo* info);
void drawBagItemNameList();
void drawActiveCharaNameBar(status::HaveStatusInfo* info, int flag, int index, int fukuro);
void drawTargetCharaNameBar(status::HaveStatusInfo* info);
void drawItemSlot(int item, int x, int y);
void drawBagItemSlot(int item, int index, int x, int y);
void drawItemEquippedMark(int index);
void drawPageIndicator(int x, int pageMax, int page, int flag);
void drawPageIndicatorWide(int x, int pageMax, int page);
void drawMenuDescription(int index, int flag, int mode);
void drawItemUsageInfo(int item);
void drawEquipCompareWindow(status::HaveStatusInfo* info, int item, int equipIndex, int flag);
void drawItemUseCommands(int mode);
void drawPartyIconRow(int mode, int flag);
void drawCharaSummaryPanel(status::HaveStatusInfo* info, int index, int flag);
void drawCasterMpBar(status::HaveStatusInfo* info, int pageMax, int page);
void drawTownSpellList(status::HaveStatusInfo* info);
void drawSpellName(int x, int y, int value, int flag, status::HaveStatusInfo* info);
void drawRuraTownList(int page, unsigned char* rura);
void drawStatusCharaProfile(status::HaveStatusInfo* info);
void drawStatusAbilities(status::HaveStatusInfo* info);
void drawStatusEquipment(status::HaveStatusInfo* info, int flag);
void drawStatusSpellList(status::HaveStatusInfo* info, int battle);
void drawPartyHpMpSummary();
void drawPartyAttackDefenceSummary();
void drawPartyMoneyAndPlayTime();
void drawCarriageOrderNumber(int number, int x, int y, int index);
void drawMenuHeaderText(int x, int y, int value);
void drawCommandLabel(int x, int y, int value);
void drawCharaTacticsRow(status::HaveStatusInfo* info, int x, int y);
void drawTacticsOptions();
void drawShiftPartyIconList(char* order, char* list, int count);
void drawShiftPartyFrontSlots(char* order);
void unkfunc_0217cc4c();
void unkfunc_0217cc6c();
void unkfunc_0217cc8c();
void unkfunc_0217ccac();
void drawTownCommandCursor(int x, int y);
void drawItemTargetInfo(status::HaveStatusInfo* info, int item, int flag);
void drawChooseCharaPrompt(int mode);
void unkfunc_0217cf1c(int mode);
int getCharaTextColorIndex(status::HaveStatusInfo* info);
int getCharaCondition(status::HaveStatusInfo* info);
void drawCharaFaceIcon(int* param, int x, int y);
void drawItemListRow(int* param, int x, int y, int flag);
void drawCharaNameBarParts(int* param, int mode, int flag, int drawFlag);
void drawCharaLevelOrCondition(status::HaveStatusInfo* info, int index, int x, int y);
int getEquipCompareValues(status::HaveStatusInfo* info, int item, int* param);
int getEquipSlotCompareValues(status::HaveStatusInfo* info, int equipIndex, int* param);
void setHpMpParams(status::HaveStatusInfo* info, int* param, int flag);
void setAttackDefenceParams(status::HaveStatusInfo* info, int* param, int flag);
