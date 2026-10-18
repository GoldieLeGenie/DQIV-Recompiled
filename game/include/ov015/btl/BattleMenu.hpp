#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "ov015/btl/BattleMenuPlayerControl.hpp"
#include "ov015/btl/BattleMonsterNamePlate.hpp"
#include "ov015/btl/MenuTemplate_battle.hpp"
#include "ov015/btl/BattleMenuDrawCommon.hpp"
#include "ov015/btl/BattleMenuScreenDraw.hpp"

struct BattleMenu_NGMESSAGE : menu::MenuBase
{
    enum RETURN_MENU {
        MENU_ROOT = 0,
        MENU_ACTIONMENU = 1
    };

    int messageID_;                     /* 0x1C */
    int returnPos_;                     /* 0x20 */
    RETURN_MENU returnMenu_;            /* 0x24 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
};

struct BattleMenuSub_HISTORY : menu::MenuBase
{
    int select_;                        /* 0x1C */
    int history_;                       /* 0x20 */
    int update_;                        /* 0x24 */
    int isRedraw_;                      /* 0x28 */
    int commandChara_;                  /* 0x2C */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
};

struct BattleMenu_ROOT : menu::MenuBase
{
    static const short MEMBER_MAX = 4;
    static const short CANCEL_WAIT_TIME = 8;
    static const short EXIT_WAIT_TIME = 120;
    static const short LAST_QUEST = 5;

    menu::MenuItem menuItem_;           /* 0x1C */
    menu::MenuItem cancelItem_;         /* 0x80 */
    CursorMoveGridLoop navigator_;     /* 0xE4 */
    short cancelWait_;                       /* 0xF0 */
    int isCancelWait_;                         /* 0xF4 */
    int isSimpleMenu_;                         /* 0xF8 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
};

struct BattleMenu_ACTIONMENU : menu::MenuBase
{
    menu::MenuItem menuItem_;           /* 0x1C */
    menu::MenuItem pageItem_;           /* 0x80 */
    menu::MenuItem cancelItem_;         /* 0xE4 */
    int activeCharacter_;               /* 0x148 */
    int unk_14c;                        /* 0x14C */
    CursorMoveGridLoop navigator_;     /* 0x150 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    int getUseActionNum(int chara);
    void selectAttack();
    void selectMagic();
    void selectItem();
    void selectDefence();
};

struct BattleMenu_MAGIC2PARTY : menu::MenuBase
{
    menu::MenuItem menuItem_;           /* 0x1C */
    menu::MenuItem cancelItem_;         /* 0x80 */
    int partyMax_;                      /* 0xE4 */
    int unk_e8;                         /* 0xE8 */
    int magic_;                         /* 0xEC  */
    int activeMagicPos_;                /* 0xF0  */
    CursorMoveGridLoop navigator_;     /* 0xF4 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
};

struct BattleMenu_ITEMUSE2PARTY : menu::MenuBase
{
    menu::MenuItem menuItem_;           /* 0x1C */
    menu::MenuItem cancelItem_;         /* 0x80 */
    int activeChara_;                         /* 0xE4 */
    int activeItem_;                         /* 0xE8 */
    int partyMax_;                         /* 0xEC */
    CursorMoveGridLoop navigator_;     /* 0xF0 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
};

struct BattleMenu_ARRAYMENU : menu::MenuBase
{
    int cursorPos_;                         /* 0x1C */
    menu::MenuItem menuItem_;           /* 0x20 */
    menu::MenuItem cancelItem_;         /* 0x84 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
};

struct TOUCHRECT
{
    short x;
    short y;
    short width;
    short height;
    short group;
};

struct BattleMenu_ATTACK : menu::MenuBase
{
    menu::MenuItem menuItem_;           /* 0x1C */
    menu::MenuItem cancelItem_;         /* 0x80 */
    int activeCharacter_;               /* 0xE4 */
    int monsterMask_;                   /* 0xE8  */
    int enemyMaxNum_;                   /* 0xEC  */
    TOUCHRECT touchRect_[12];           /* 0xF0  */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void unkfunc_0216ea10();
};

struct BattleMenu_MAGIC2ENEMY : menu::MenuBase
{
    menu::MenuItem menuItem_;           /* 0x1C */
    menu::MenuItem cancelItem_;         /* 0x80 */
    int activeChara_;                   /* 0xE4 */
    int activeMagic_;                   /* 0xE8 */
    int activeMagicPos_;                /* 0xEC */
    int activeMagicIndex_;              /* 0xF0 */
    int enemyNumMax_;                   /* 0xF4 */
    int monsterMask_;                         /* 0xF8 */
    TOUCHRECT touchRect_[12];           /* 0xFC */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
};

struct BattleMenu_ITEMUSE2ENEMY : menu::MenuBase
{
    menu::MenuItem menuItem_;           /* 0x1C */
    int unk_80[25];                     /* 0x80 */
    menu::MenuItem cancelItem_;         /* 0xE4 */
    int activeChara_;                        /* 0x148 */
    int activeItem_;                        /* 0x14C */
    int enemyNumMax_;                        /* 0x150 */
    int oldGroup_;                        /* 0x154 */
    int oldActive_;                        /* 0x158 */
    int monsterMask_;                        /* 0x15C */
    TOUCHRECT touchRect_[12];              /* 0x160 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void unkfunc_0216cd44();
    void unkfunc_0216cd80(int index);
    void unkfunc_0216cdc0(int index);
    void unkfunc_0216ce00();
    int unkfunc_0216cee0();
};

struct BattleMenu_ARRAY_CHANGE : menu::MenuBase
{
    int phase_;                         /* 0x1C */
    int prevPhase_;                         /* 0x20 */
    int targetPos_;                         /* 0x24 */
    menu::MenuItem menuItem_;           /* 0x28 */
    menu::MenuItem targetItem_;              /* 0x8C */
    menu::MenuItem pageItem_;              /* 0xF0 */
    menu::MenuItem cancelItem_;         /* 0x154 */
    CursorMoveGridLoop memberNavigator_;        /* 0x1B8 */
    CursorMoveGridLoop targetNavigator_;        /* 0x1C4 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    int unkfunc_0216f37c();
    int unkfunc_0216f3c4();
    int unkfunc_0216f4cc();
};

struct BattleMenu_ARRAY_ALL : menu::MenuBase
{
    int newOrder_[4];                      /* 0x1C */
    int selectNum_;                         /* 0x2C */
    int unk_30[25];                     /* 0x30 */
    menu::MenuItem menuItem_;           /* 0x94 */
    menu::MenuItem cancelItem_;         /* 0xF8 */
    CursorMoveGridLoop navigator_;     /* 0x15C */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
};

struct BattleMenu_TACTICSMENU : menu::MenuBase
{
    int phase_;                         /* 0x1C */
    int prevPhase_;                         /* 0x20 */
    int memberPos_;                         /* 0x24 */
    int tacticsPos_;                         /* 0x28 */
    int memberList_[10];                     /* 0x2C */
    int memberNum_;                         /* 0x54 */
    int unk_58[25];                     /* 0x58 */
    menu::MenuItem menuItem_;           /* 0xBC */
    menu::MenuItem tacticsItem_;             /* 0x120 */
    menu::MenuItem cancelItem_;         /* 0x184 */
    CursorMoveGridLoop memberNavigator_;        /* 0x1E8 */
    CursorMoveGridLoop tacticsNavigator_;        /* 0x1F4 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    int unkfunc_0216fb88();
    void unkfunc_0216fbdc();
    int unkfunc_0216fc4c();
    void unkfunc_0216fc98();
    void unkfunc_0216fcec();
};

struct BattleMenu_ITEM : menu::MenuBase
{
    int activeChara_;                         /* 0x1C */
    int unk_20;                         /* 0x20 */
    menu::MenuItem menuItem_;           /* 0x24 */
    menu::MenuItem cancelItem_;         /* 0x88 */
    menu::MenuItem pageItem_;              /* 0xEC */
    CursorMoveGridLoop navigator_;     /* 0x150 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void selectUseItem();
    bool isSelectEquipEnable();
};

struct BattleMenuJudge
{
    int unk_00;                         /* 0x00 */
    int unk_04;                         /* 0x04 */
    int playerMaxNum_;                  /* 0x08 */
    int minadeinFlag_;                  /* 0x0C */
    TOUCHRECT touchRect_[12];           /* 0x10 */

    static BattleMenuJudge* getSingleton();
    void turnSetup();
    void baseSetup();
    void setAttack(int group);
    void setMagicEnemy(int magic, int group);
    void setMagicParty(int magic, int target);
    void setItemEnemy(int item, int group);
    void setItemEnemyAll(int item);
    void setItemParty(int item, int target);
    void setItemPartyAll(int item);
    bool judgeNextChara();
    bool judgeBackChara();
    void backActionMenu(int page);
    bool judgeBattleArrayChange();
    int getPlayerIndex() { return btl::BattleMenuPlayerControl::getSingleton()->activeChara_; }
    void setNextPlayer();
    void setPrevPlayer();
    int getMonsterTouchRect(TOUCHRECT* rect);
    int getLiveMonsterID();
    bool isCommandingPlayer(int index);
};

struct BattleMenu_MAGIC : menu::MenuBase
{
    static const int BTL_ACT_MAX = 30;

    menu::MenuItem menuItem_;           /* 0x1C */
    menu::MenuItem cancelItem_;         /* 0x80 */
    menu::MenuItem pageItem_;              /* 0xE4 */
    CursorMoveGridLoop navigator_;     /* 0x148 */
    int count_;                         /* 0x154 */
    int haveAction_[BTL_ACT_MAX];       /* 0x158 */
    int haveActionIndex_[BTL_ACT_MAX];  /* 0x1D0 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void setActiveMagicPos(int pos);
    void setMagicTargetFree(int magic);
    void unkfunc_0216ded8();
};

struct BattleMenu_StadiumAbort : menu::MenuBase
{
    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
};

// use / equip choice for an equipment item, opened from the item menu
struct BattleMenu_ITEMUSEEQUIP : menu::MenuBase
{
    menu::MenuItem menuItem_;           /* 0x1C */
    menu::MenuItem cancelItem_;         /* 0x80 */
    menu::MenuItem useEquipItem_;              /* 0xE4 */
    int activeChara_;                        /* 0x148 */
    int activeItem_;                        /* 0x14C */
    int equipEnable_;                        /* 0x150 */
    int unk_154;                        /* 0x154 */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    int unkfunc_0216d0c0();
    void unkfunc_0216d0ec();
    void unkfunc_0216d0f0();
    void selectUseItem();
    void selectEquip();
    void unkfunc_0216d488();
};

// test menu with one item
struct BattleMenu_TEST : menu::MenuBase
{
    menu::MenuItem menuItem_;           /* 0x1C */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
};

void ChangeParty(BattleMenu_ARRAY_ALL* self);

extern BattleMenu_NGMESSAGE gBattleMenu_NGMESSAGE;          /* 0x02179758 */
extern BattleMenuSub_HISTORY gBattleMenuSub_HISTORY;        /* 0x02179780 */
extern BattleMenu_ARRAYMENU gBattleMenu_ARRAYMENU;          /* 0x02179830 */
extern BattleMenu_ITEMUSE2PARTY gBattleMenu_ITEMUSE2PARTY;  /* 0x02179918 */
extern BattleMenu_ROOT gBattleMenu_ROOT;                    /* 0x02179a14 */
extern BattleMenu_MAGIC2PARTY gBattleMenu_MAGIC2PARTY;      /* 0x02179b10 */
extern BattleMenu_ACTIONMENU gBattleMenu_ACTIONMENU;        /* 0x02179ec4 */
extern BattleMenu_ARRAY_ALL gBattleMenu_ARRAY_ALL;          /* 0x0217a020 */
extern BattleMenu_ATTACK gBattleMenu_ATTACK;                /* 0x0217a188 */
extern BattleMenu_MAGIC2ENEMY gBattleMenu_MAGIC2ENEMY;      /* 0x0217a2f0 */
extern BattleMenu_ITEMUSE2ENEMY gBattleMenu_ITEMUSE2ENEMY;  /* 0x0217a634 */
extern BattleMenu_ARRAY_CHANGE gBattleMenu_ARRAY_CHANGE;    /* 0x0217a464 */
extern BattleMenu_TACTICSMENU gBattleMenu_TACTICSMENU;      /* 0x0217a80c */
extern BattleMenu_MAGIC gBattleMenu_MAGIC;                  /* 0x0217aa0c */
extern BattleMenuJudge gBattleMenuJudge;                    /* 0x021796b4 */
extern BattleMenu_ITEM gBattleMenu_ITEM;                    /* 0x02179d68 */
extern BattleMenu_StadiumAbort gBattleMenu_StadiumAbort;    /* 0x0217973c */
extern BattleMenu_ITEMUSEEQUIP gUnkBattleMenu_0216cf44;      /* 0x02179c10 */
extern BattleMenu_TEST gUnkBattleMenu_0216e270;      /* 0x021797b0 */

