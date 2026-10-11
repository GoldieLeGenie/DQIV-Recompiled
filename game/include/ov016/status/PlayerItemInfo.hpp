#pragma once
#include "globaldefs.h"
#include "GameInfo.hpp"
#include "main/status/UseActionParam.hpp"
#include "main/status/UseItem.hpp"

namespace status {
struct PlayerItemInfo
{
    static int getItemIndex(int player, int index);
    static int getItemMaxCount(int player);
    static bool ableToShow();
    static bool ableToEqip(int player, int index);
    static int getItemPlayerCommandFlag(int player, int index);
    static int getEquipItemIdByType(int player, ItemType type);
    static status::UseItem::ThrowType getItemThrowType(int player, int index);
    static void throwCharaItem(int player, int index);
    static bool checkCurse(int player, int itemIndex);
    static int usePlayerItem(status::UseActionParam& param, int player, int index, int* execMessage, int* resultMessage, int target);
};
}
