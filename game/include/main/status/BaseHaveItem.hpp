#pragma once
#include <globaldefs.h>
#include "main/status/ItemData.hpp"

namespace status{
    struct BaseHaveItem {
        status::ItemData* item_;
        int itemMax_;
        BaseHaveItem();
        ~BaseHaveItem();
        virtual void clear();
        virtual int add(int itemIndex);
        virtual int del(int ctrlId);
        int addOne(int itemIndex);
        int addNum(int itemIndex, int count);
        int delOne(int ctrlId);
        bool delNum(int index);
        void sort();
        int getCount();
        int getMaxCount();
        int getItem(int index);
        int getItemSortIndex(int itemIndex);
        status::ItemData* getItemData(int index);
        int getItemCount(int index);
        int isItem(int itemIndex);
        int getItemMax();
    };
}
