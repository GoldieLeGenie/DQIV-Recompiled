#pragma once
#include "globaldefs.h"

// battle menu page drawing: action / magic / item lists, tactics, formation and party pages
void drawAttackTargetMenu(int group);
void drawItemMenu(int* items, int count, int page);
void drawItemUseEquipMenu(int* items, int count, int page);
void drawItemUseOnPartyMenu();
void drawItemUseOnEnemyMenu(int group);
void drawMagicMenu(int* actions, int count, int page);
void drawMagicOnEnemyMenu(int group);
void drawMagicOnPartyMenu();
void drawTacticsMemberMenu();
void drawTacticsSelectMenu();
void drawFormationMenu();
void drawFormationChangeMemberMenu();
void drawFormationChangeTargetMenu(int* list, int count, int page);
void drawFormationAllMenu(int* list, int count, int* order, int orderCount);
void drawPartyHpPercents();
void drawEnemyTargetSelect(int group);
void drawPartyTargetSelect();
void drawItemList(int* items, int count, int page);
void drawAttackCommandButton();
void drawMagicCommandHeader();
void drawItemCommandHeader();
void drawTacticsCommandButton();
void drawFormationCommandButton();
void drawFormationMemberIcons(int* list, int count);
void drawSelectedCharaHp(int chara);
void drawMemberIcon(int x, int y, int index);
void drawBattleHistory(int chara);
void drawCharaBattleSpellList(int* actions, int count, int chara);
