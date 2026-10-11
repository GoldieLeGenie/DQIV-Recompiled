#pragma once
#include "string.h"
#include <globaldefs.h>
#include "main/param/Param.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/HaveItemSack.hpp"
#include "main/param/ShopDataFirst.hpp"
#include "main/param/ShopDataSecond.hpp"
namespace status{
    struct ShopData {                              // data_020d0bcc
        int section_;                              // +0
        param::ShopDataFirst* shopDataFirst_;      // +4
        param::ShopDataSecond* shopDataSecond_;    // +8
    };

    struct ShopList {
        enum ChurchType{
            NoExist = 0,
            Exist   = 1,
            NoPray  = 2,
        };
        int sideJobItemFlag_[6];                   // +0x00 
        int pay_;                                  // +0x18
        HaveItemSack haveItemNene_;                // +0x1c
        ShopList();
        ~ShopList();
        static void initialize();
        int getDataIndex(int shop);
        int getShopCount(int shop);
        int getShopItem(int shop, int index);
        int getShopPrice(int shop, int index);
        int getHotelPrice(int second);
        ChurchType getChurchType(int first);

    };
    extern ShopData ShopData_;                 // data_020d0bcc
    extern ShopList g_Shop;                    // data_020d0be4
}
