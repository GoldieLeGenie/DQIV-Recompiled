#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/param/Param.hpp"

namespace status
{
    struct HaveStatusInfo;
    struct PlayerStatus;
    struct PartyStatus;
    struct PlayerFlag;
    struct HaveAction;
    struct HaveItem;
    struct BaseHaveItem;

    struct HaveBattleStatus
    {
        enum SelectCommand
        {
            Attack = 0,
            Defence = 1,
            UseAction = 2,
            UseItem = 3,
            NoSelect = 4,
            AISelect = 5
        };

        enum CallStart
        {
            StartRound = 0,
            StartTurn = 1
        };
        static param::MonsterData* monsterData_;
        static param::CharInitData* charInitData_;
        static int tarotDebugIndex_;
        static int parupunteDebugIndex_;
        static int confusionDebugIndex_;
        static int torunekoDebugIndex_;
        static int eventFlag_;
        static PlayerFlag groupControl_[4];
        static int groupRoopIndex_[4];
        int groupIndex_;
        int index_;
        int originalIndex_;
        CharacterType originalType_;
        CharacterType type_;
        HaveStatusInfo *haveStatusInfo_; 
        int exp_;
        unsigned short hp_;
        unsigned short hpMax_;
        unsigned short mp_;
        unsigned short mpMax_;
        unsigned short strength_;
        unsigned short protection_;
        unsigned short agility_;
        unsigned short wisdom_;
        unsigned char luck_;
        int texp_;
        int tlevel_;
        int change_;
        int actionIndex_;
        int actionCount_;
        int patternIndex_;
        dss::BitFlaguint patternFailedFlag_;
        dss::BitFlaguint disablePattern_;
        dss::BitFlaguint disablePattern2nd_;
        SelectCommand selectCommand_;
        int selectIndex_;
        int selectedGroup_;
        int selectedTarget_;
        int sortIndex_;
        int monsterCtrlId_;
        int escape_;
        int tarotCount_;
        int tarotActionIndex_;
        unsigned char brains_;
        unsigned char group_[6];
        unsigned char crossFire_;
        int crossFireTarget_;
        unsigned char multi_;
        char multiCount_;
        char multiCount2_;
        char multiTurnCount_;
        char multiGlobalCount_;
        char multiExecCount_;
        unsigned char level_;
        unsigned char dragon_;
        unsigned char metal_;
        unsigned char zombi_;
        unsigned char air_;
        unsigned char slime_;
        unsigned char jouk_;
        int monsterIndexForNpc_;
        unsigned char mosyasAction_[17];
        int mosyasActionCount_;

       

        HaveBattleStatus();
        ~HaveBattleStatus();
        static void initialize();
        void setup(CharacterType type, int group, int index);
        void newBaseChangeMonster(int index);
        void newBaseChangeMonsterFromPlayer(int index);
        void newBaseChangeMonsterReverse2();
        void newBaseChangeMonsterWithHpMp(int index);
        void changeMonsterFromPlayer(int index);
        void changeMonsterReverse();
        void setupPlayer();
        void setupMonster();
        bool isJouk();
        void setActionIndex(int index);
        void setActionSelect(CallStart callStart);
        void setActionSelectForPlayer();
        void setActionSelectForMonster(CallStart callStart);
        void setActionPatternForMonster();
        static int getClampValue(int value, int min, int max);
        void setActionIndexForMonster();
        int getActionIndex(int index);
        int getActionAnimation();
        void setActionFailed(int index);
        bool isActionFailed();
        void setActionDisable(int index);
        bool isActionDisable();
        bool isActionAllDisable();
        void setActionDisable2nd();
        bool isActionDisable2nd();
        bool isActionAllDisable2nd();
        void clearActionDisable2nd();
        int isRestOneAction2nd();
        void setRestOne();
        int getRestNum();
        bool isUseMp();
        int isMahoton();
        int isFizzleZone();
        bool checkRestMP_Mahoton();
        void clearGroupControl();
        void checkGroupControl();
        int getGroupControl();
        void setSelectCommand(SelectCommand command, int index);
        SelectCommand getSelectCommand();
        void clearSelectCommand();
        void startBattle();
        void execStartOfRound();
        void setMultiAction();
        bool isMultiAction();
        void setupTarotAction();
        void setupParupunteAction();
        void setupConfuseAction();
        int setupTorunekoAction();
        void setupSpecialAction(int actionIndex);
        void updateSpecialSleepBehaviors();
        void setupRollOver();
        void setupSleepAction();
        void setupTurnBeforeAction();
        void setMosyasAction(int action);
        int getMosyasAction(int index);
        void clearMosyasAction();
        void print();
    };
}

struct TorunekoActionTable { int v[10]; };
struct RoopCopy { int v[4]; };
struct RestArray { int v[6]; };
struct TarotTable { int v[8]; };
struct ParupunteArraySmall { int v[13]; };
struct ParupunteArrayLarge { int v[23]; };
struct ConfuseArray3 { int v[3]; };
struct ConfuseArray4 { int v[4]; };
struct ConfuseArray6 { int v[6]; };
struct ConfuseArray8 { int v[8]; };

extern const ConfuseArray4       confusePlayer8;
extern const ConfuseArray4       confusePlayer5;
extern const ConfuseArray3       confusePlayer12;
extern const ConfuseArray3       confusePlayer7;
extern const ConfuseArray3       confusePlayer3;
extern const ConfuseArray4       confusePlayer9;
extern const ConfuseArray4       confusePlayer4;
extern const RoopCopy            roopPattern;
extern const ConfuseArray4       confusePlayer6;
extern const ConfuseArray6       confuseOtherParupunte;
extern const ConfuseArray6       confuseOtherNormal;
extern const RestArray           restOneTable;
extern const RestArray           restNumTable;
extern const ConfuseArray8       confuseMonsterNormal;
extern const TarotTable          tarotActionTable;
extern const ConfuseArray8       confuseMonsterParupunte;
extern const TorunekoActionTable torunekoParupunte;
extern const TorunekoActionTable torunekoNormal;
extern const ParupunteArraySmall parupunteSmallTable;
extern const ParupunteArrayLarge parupunteLargeTable;
