#include "main/status/HaveItemSack.hpp"
#include "main/status/UseItem.hpp"

THUMB status::HaveItemSack::HaveItemSack() : sortType_(Kind) {
    item_ = itemArray_;
    itemMax_ = 162;
    clear();
}

THUMB status::HaveItemSack::~HaveItemSack() {

}

THUMB void status::HaveItemSack::operator=(status::HaveItemSack* rhs) {
    int count = rhs->getCount();
    for (int i = 0; i < count; i++) {
        int item = rhs->getItem(i);
        int itemCount = rhs->getItemCount(i);
        adds(item, itemCount);
    }
}

THUMB int status::HaveItemSack::adds(int itemIndex, int count) {
    int sortIndex = BaseHaveItem::getItemSortIndex(itemIndex);
    int currentCount = BaseHaveItem::getItemCount(sortIndex);
    if ((count + currentCount) <= 99) {
        return BaseHaveItem::addNum(itemIndex, count);
    }
    int newCount = 99 - BaseHaveItem::getItemCount(sortIndex);
    return BaseHaveItem::addNum(itemIndex, newCount);
}

THUMB int status::HaveItemSack::add(int itemIndex) {
    return adds(itemIndex, 1);
}

THUMB int status::HaveItemSack::del(int itemIndex) {
    return BaseHaveItem::delNum(itemIndex);
}

THUMB void status::HaveItemSack::execThrow(int itemIndex) {
    del(itemIndex);
}

THUMB void status::HaveItemSack::sortOutSack(status::HaveItemSack::SortType type) {
    int t;
    int rank;
    int src;
    int dst;

    if (type == Same) {
        t = this->sortType_;
    } else {
        t = type;
        this->sortType_ = type;
    }

    status::ItemData sp10[0xA2];   

    int i = 0;
    if (this->itemMax_ > 0) {
        status::ItemData* p = sp10;
        do {
            p->index_ = 0;
            p->equip_ = 0;
            p->count_ = 0;
            i++;
            p++;
        } while (i < this->itemMax_);
    }

    dst = 0;
    rank = 0;
    if (this->itemMax_ > 0) {
        do {
            int wantKey = rank + 1;
            src = 0;
            if (this->itemMax_ > 0) {
                do {
                    if (this->item_[src].index_ != 0) {
                        if (t == 0) {
                            if (wantKey == status::UseItem::getTypeSortIndex(this->item_[src].index_)) {
                                sp10[dst++] = this->item_[src];
                                this->item_[src].index_ = 0;
                                break;
                            }
                        } else if (t == 1) {
                            if (wantKey == status::UseItem::getNameSortIndex(this->item_[src].index_)) {
                                sp10[dst++] = this->item_[src];
                                this->item_[src].index_ = 0;
                                break;
                            }
                        }
                    }
                    src++;
                } while (src < this->itemMax_);
            }
            rank++;
        } while (rank < this->itemMax_);
    }
    int j = 0;
    if (this->itemMax_ > 0) {
        status::ItemData* sp = sp10;
        do {
            if (sp->index_ != 0) {
                this->item_[j] = *sp;
            }
            sp++;
            j++;
        } while (j < this->itemMax_);
    }
}

THUMB void status::HaveItemSack::sortOutItem(status::HaveItem *haveItem)
{
    for (int i = 0; i < haveItem->getMaxCount(); i++)
    {
        sortOutItemOne(haveItem);
    }
}

THUMB void status::HaveItemSack::sortOutItemOne(status::HaveItem *haveItem)
{
    for (int i = 0; i < haveItem->getCount(); i++)
    {
        if (haveItem->isEquipment(i) == 0)
        {
            if (UseItem::isOrder(haveItem->getItem(i)))
            {
                ((status::BaseHaveItem *)this)->add(haveItem->getItem(i));
                haveItem->del(i);
                return;
            }
        }
    }
}

