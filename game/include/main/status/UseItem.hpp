#pragma once

#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/UseActionParam.hpp"
#include "main/status/BaseHaveItem.hpp"
#include "main/param/Param.hpp"

namespace status{
    struct UseitemData {
        int unk_0;
        param::ItemData* itemData_;
        int unk_8;
        param::AppriseItem* judgeItem_;     // 0x0C  
        int unk_10;                         // 0x10
        int actionIndex_;                   // 0x14
        int itemIndex_;                     // 0x18
        char unk_1c[0x54];
    };
    struct UseItem {
        enum UseArea {
            Zero = 0,
            One = 1,
            Group = 2,
            All = 3,
            Horse = 4,
            UsuallyHorse = 5,
            BattleHorse = 6
        };
        enum UseType {
            Myself = 0,
            Friend = 1,
            Enemy = 2,
            Both = 3,
            None = 4
        };
        enum ThrowType {
            THROW_OK = 0,
            THROW_NG = 1,
            THROW_DIFFICULT = 2,
            THROW_MAX = 3
        };
        enum SellType {
            SELL_OK = 0,
            SELL_NG = 1,
            SELL_DIFFICULT = 2,
            SELL_MAX = 3
        };
        static void initialize();
        static ItemType getItemType(int itemIndex);
        static ThrowType getThrowType(int itemIndex);
        static SellType getSellType(int itemIndex);
        static bool isCurse(int itemIndex);
        static int isLost(int itemIndex);
        static bool isOrder(int itemIndex);
        static int getSellPrice(int itemIndex);
        static int getBuyPrice(int itemIndex);
        static bool isEquipEnable(JobType job, int itemIndex, int sex);
        static EquipType getEquipType(int itemIndex);
        static int getEquipValue(int itemIndex);
        static UseType getUseType(int itemIndex);
        static UseArea getUseArea(int itemIndex);
        static int getAction(int itemIndex);
        static int getBattleUseAction(int index);
        static int getMenuMessage(int itemIndex);
        static bool isUsuallyUse(int itemIndex);
        static int getTypeSortIndex(int itemIndex);
        static int getNameSortIndex(int itemIndex);
        static int getJudgeMessageCount(int itemIndex);
        static int getJudgeMessage(int itemIndex, int index);
        static void execUse(status::UseActionParam* useActionParam);
        static void execBattleUse(status::UseActionParam* useActionParam);
        static void give(status::BaseHaveItem* src, int index, status::BaseHaveItem* dst);
        static void give(status::HaveStatusInfo *status, int index,
                           status::BaseHaveItem *sack, int sackIndex);
        static void give2(status::BaseHaveItem *sack, int sackIndex,
                            status::HaveStatusInfo *status, int index);
        static void give2(status::HaveStatusInfo *status, int index,
                            status::HaveStatusInfo *target, int targetIndex);

    };
}

extern status::UseitemData UseitemData_; //data_020d0820
