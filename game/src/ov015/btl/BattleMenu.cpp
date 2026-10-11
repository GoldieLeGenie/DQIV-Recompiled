#pragma ipa file
// ov016 (MaterielMenuInstances.cpp) has its own copy of the abstract CursorMoveBase vtable and the
// linker merges same-name weak vtables across overlays, so the copy of this overlay gets its own name
#define CursorMoveBase CursorMoveBase_ov015
#include "ov015/btl/BattleMenu.hpp"
#undef CursorMoveBase

// global instances of the battle menus
THUMB
UnkBattleMenu_0216e270 gUnkBattleMenu_0216e270;
BattleMenu_ROOT gBattleMenu_ROOT;
BattleMenu_ACTIONMENU gBattleMenu_ACTIONMENU;
BattleMenu_ATTACK gBattleMenu_ATTACK;
BattleMenu_MAGIC gBattleMenu_MAGIC;
BattleMenu_MAGIC2ENEMY gBattleMenu_MAGIC2ENEMY;
BattleMenu_MAGIC2PARTY gBattleMenu_MAGIC2PARTY;
BattleMenu_ITEM gBattleMenu_ITEM;
UnkBattleMenu_0216cf44 gUnkBattleMenu_0216cf44;
BattleMenu_ITEMUSE2ENEMY gBattleMenu_ITEMUSE2ENEMY;
BattleMenu_ITEMUSE2PARTY gBattleMenu_ITEMUSE2PARTY;
BattleMenu_TACTICSMENU gBattleMenu_TACTICSMENU;
BattleMenu_ARRAYMENU gBattleMenu_ARRAYMENU;
BattleMenu_ARRAY_CHANGE gBattleMenu_ARRAY_CHANGE;
BattleMenu_ARRAY_ALL gBattleMenu_ARRAY_ALL;
BattleMenu_NGMESSAGE gBattleMenu_NGMESSAGE;
BattleMenuSub_HISTORY gBattleMenuSub_HISTORY;
BattleMenu_StadiumAbort gBattleMenu_StadiumAbort;
