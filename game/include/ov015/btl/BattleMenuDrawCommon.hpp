#pragma once
#include "globaldefs.h"
#include "main/status/HaveStatusInfo.hpp"

// battle menu drawing helpers: command icons, party status windows, monster name plates
void drawBattleCommandIcons(int command0, int command1, int command2, int command3);
void drawBattleMemberHpPercent(int x, int y, int hp, status::HaveStatusInfo* info);
void drawBattleActiveMember(status::HaveStatusInfo* info, int hp, int flag);
void drawMonsterNamePlates(int group);
void drawBattleCommandIcon(int command, int x, int y, int flag);
void drawBattleHelpMessage(int x, int y, int message);
void drawBattlePageNumber(int x, int y, int max, int page);
void drawBattleItemEntry(int x, int y, int item, int flag);
void drawBattleActionName(int x, int y, int action);
void drawBattleMemberStatusPanel(status::HaveStatusInfo* info, int index);
void drawBattleCharaStatus(int x, int y, status::HaveStatusInfo* info);
void drawBattleMemberSelectList(int* list, int count, int page);
void drawBattleMemberConditionMarks(status::HaveStatusInfo* info, int x, int y);
void drawBattleCancelButton();
void drawBattleTopStatusWindow(status::HaveStatusInfo* info, int a, int b, int c, int index);
void drawBattleActionHistory(int index, int value, int message, int color);
void drawBattleMemberIconInSlot(int x, int y, int value, int type);
void drawBattleMpCost(int mp, int useMp);
void drawBattleMemberIcon(int x, int y, int icon);
void drawBattleMemberFace(int x, int y, status::HaveStatusInfo* info, int flag);
int getBattleHpColorIndex(status::HaveStatusInfo* info);
int getBattleHpColorIndexByHp(status::HaveStatusInfo* info, int hp);
void drawBattleRootMenu(int flag);
void drawBattleActionMenu(int flag);
