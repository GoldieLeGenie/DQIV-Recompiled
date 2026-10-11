#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/BaseHaveItem.hpp"
#include "main/status/ItemData.hpp"

namespace status {
    struct HaveItem;
}

namespace status{
    struct HaveItemSack : BaseHaveItem {
        enum SortType {
            Kind = 0x0,
            Name = 0x1,
            Same = 0x2,
        };
        SortType sortType_;
        ItemData itemArray_[162];
        HaveItemSack();
        ~HaveItemSack();
        void operator=(status::HaveItemSack* rhs);
        int adds(int itemIndex, int count);
        virtual int add(int itemIndex);
        virtual int del(int itemIndex);
        void execThrow(int itemIndex);
        void sortOutSack(status::HaveItemSack::SortType type);
        void sortOutItem(status::HaveItem *haveItem);
        void sortOutItemOne(status::HaveItem *haveItem);
    };
}

