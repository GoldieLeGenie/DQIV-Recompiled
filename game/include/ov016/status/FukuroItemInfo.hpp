#pragma once
#include "globaldefs.h"
#include "GameInfo.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/status/UseActionParam.hpp"
#include "main/status/UseItem.hpp"

namespace status {
struct FukuroItemInfo
{
    static int getPageMax();
    static int getItemMaxCount();
    static int getPageItemCount(int page);
    static int getItemId(int page, int index);
    static int getItemFukuroCommandFlag();
    static int getIndexToAll(int page, int index);
    static status::UseItem::ThrowType getItemThrowType(int page, int index);
    static void throwFukuroItem(int index);
    static int useFukuroItem(status::UseActionParam& param, int index, int* execMessage, int* resultMessage, int target);
};
}

