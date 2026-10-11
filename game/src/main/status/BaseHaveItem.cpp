#include "main/status/BaseHaveItem.hpp"


THUMB status::BaseHaveItem::BaseHaveItem() {
    
}

THUMB status::BaseHaveItem::~BaseHaveItem() {
}

THUMB void status::BaseHaveItem::clear()
{
    for (int i = 0; i < this->itemMax_; i++) {
        this->item_[i].index_ = 0;
        this->item_[i].equip_ = 0;
        this->item_[i].count_ = 0;
    }
    return;
}


THUMB int status::BaseHaveItem::add(int itemIndex) {
    return addOne(itemIndex);
}

THUMB int status::BaseHaveItem::del(int ctrlid)
{
    return delOne(ctrlid);
}

THUMB int status::BaseHaveItem::addOne(int itemIndex)
{
    int max;
    int result;
    int i;
    ItemData* items;
    ItemData* current;

    if (itemIndex == 0) {
        return -1;
    }

    result = 0;
    max = this->itemMax_;
    i = result;

    if (this->itemMax_ > 0) {
        current = this->item_;
        items = current;

        do {
            if (items->index_ == 0) {
                current[i].index_ = itemIndex;
                this->item_[i].count_ = 1;
                result = i;
                break;
            }
            i++;
            items++;
        } while (i < max);
    }

    return result;
}


THUMB int status::BaseHaveItem::addNum(int itemIndex, int count) {
    int result = 0;
    int max = this->itemMax_;
    int i = 0;

    if (max > 0) {
        do {
            if (item_[i].index_ == itemIndex) {   
                item_[i].index_ = itemIndex;
                item_[i].count_ += count;         
                result = i;
                break;
            }
            if (item_[i].index_ == 0) {           
                item_[i].index_ = itemIndex;
                item_[i].count_ = count;      
                result = i;
                break;
            }
            i++;
        } while (i < max);
    }
    return result;
}

THUMB int status::BaseHaveItem::delOne(int ctrlId)
{
  this->item_[ctrlId].clear();
  sort();
  return 1;
}


THUMB bool status::BaseHaveItem::delNum(int index)
{
  unsigned short count;
  
  count = this->item_[index].count_;
  if (count == 0) {
    return 0;
  }
  this->item_[index].count_ = count - 1;
  if (this->item_[index].count_ == 0) {
    this->item_[index].index_ = 0;
    sort();
  }
  return 1;
}

// fake volatile??
static inline void itemDataCopy(volatile status::ItemData& src, volatile status::ItemData& dst) {
    dst.index_ = src.index_;
    dst.equip_ = src.equip_;
    dst.count_ = src.count_;
}

THUMB void status::BaseHaveItem::sort() {
    ItemData sp4[0xA2];   

    for (int i = 0; i < this->itemMax_; i++) {
        sp4[i].index_ = 0;
        sp4[i].equip_ = 0;
        sp4[i].count_ = 0;
    }

    int j = 0;
    for (int i = 0; i < this->itemMax_; i++) {
        if (this->item_[i].index_ != 0) {
            itemDataCopy(this->item_[i], sp4[j]);
            j++;
        }
    }

    BaseHaveItem::clear();

    for (int i = 0; i < this->itemMax_; i++) {
        itemDataCopy(sp4[i], this->item_[i]);
    }
}



THUMB int status::BaseHaveItem::getCount()
{
    int count = 0;
    int index = 0;

    volatile BaseHaveItem* vThis = this;

    if (vThis->itemMax_ > 0) {
        ItemData* item = this->item_;

        do {
            if (item->index_ != 0) {
                count++;
            }

            index++;
            item++;

        } while (index < vThis->itemMax_);
    }

    return count;
}

THUMB int status::BaseHaveItem::getMaxCount()

{
  return this->itemMax_;
}

THUMB int status::BaseHaveItem::getItem(int index)

{
  return this->item_[index].index_;
}

THUMB int status::BaseHaveItem::getItemSortIndex(int itemIndex)
{
    int result = -1; 
    int currentMax;    
    int index = 0;
    volatile BaseHaveItem* vThis = this;
    if (this->itemMax_ > 0) {
        
        ItemData* item = this->item_;

        do {
            
            if (itemIndex == item->index_ ) {
                result = index; 
                break;          
            }

            currentMax = vThis->itemMax_;

            index++; 
            item++;  

        } while (index < currentMax);
    }

    return result;
}

THUMB status::ItemData * status::BaseHaveItem::getItemData(int index)
{
  return &this->item_[index];
}

THUMB int status::BaseHaveItem::getItemCount(int index)
{
  return this->item_[index].count_;
}


THUMB int status::BaseHaveItem::isItem(int itemIndex)
{
  int i;
  volatile BaseHaveItem* vThis = this;

  i = 0;
  if (this->itemMax_ > 0  ) {
    ItemData* item = this->item_;
    do {
      if (itemIndex == item->index_) {
        return 1;
      }
      i++;
      item++;
    } while (i < vThis->itemMax_);
  }
  return 0;
}

THUMB int status::BaseHaveItem::getItemMax()

{
  return this->itemMax_;
}

