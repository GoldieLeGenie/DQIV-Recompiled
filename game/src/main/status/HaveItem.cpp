#include "main/status/HaveItem.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/param/Param.hpp"
#include "main/status/UseItem.hpp"

THUMB status::HaveItem::HaveItem() {
    this->item_ = this->itemArray_;
    this->itemMax_ = 12;
    this->clear();
}

THUMB status::HaveItem::~HaveItem() {
    return;
}

THUMB void status::HaveItem::setup(int index) {
    const param::CharInitData* CharaInitData;
    CharaInitData = excelParam.getCharaInitData();
    this->clear();
    sort(this->add(CharaInitData[index].weapon));
    sort(this->add(CharaInitData[index].armor));
    sort(this->add(CharaInitData[index].shield));
    sort(this->add(CharaInitData[index].hat));
}

THUMB void status::HaveItem::clear()
{
  BaseHaveItem::clear();
  for(int i = 0; i < 12; i++){
      this->item_[i].equip_ = 0;
  }
 return;
}

THUMB int status::HaveItem::add(int index){
    return BaseHaveItem::addOne(index);
}

THUMB int status::HaveItem::del(int ctrlId)
{
  int iVar1 = delOne(ctrlId);
  sortEquipment();
  return iVar1;
}

THUMB void status::HaveItem::sort(int index) {
    if (index != -1) {
        this->item_[index].equip_ = 1;
        sortEquipment();
    }
}

THUMB void status::HaveItem::setEquipmentForTest(int index)
{
    if (index != -1)
        this->item_[index].equip_ = 1;
}

THUMB void status::HaveItem::sortEquipmentForTest(int index) {
    item_[index].equip_ = 0;
    sortEquipment();
}

THUMB void status::HaveItem::resetEquipmentWithItemIndex(int itemIndex)
{
    int offset = 0;

    for (int i = 0; i < getCount(); i++, offset += 4) {
        ItemData* items = this->item_;
        if (items[i].equip_ != 0) {
            if (items[i].index_ == itemIndex) {
                sortEquipmentForTest(i);
                return;
            }
        }
    }
    return;
}

THUMB int status::HaveItem::isEquipment(int index)
{
    return this->item_[index].equip_;
}

THUMB void status::HaveItem::sortEquipment() {
    ItemData sp18[12];
    unsigned int targetIdx;

    for (int i = 0; i < 12; i++) {
        sp18[i].index_ = 0;
    }

    targetIdx = 0;
    for (int j = 0; j < 12; j++) {
        if (this->item_[j].index_ != 0 && this->item_[j].equip_ != 0) {
            if (status::UseItem::getItemType(this->item_[j].index_) == 0) {
                sp18[targetIdx++] = this->item_[j];
                this->item_[j].index_ = 0;
            }
        }
    }
    for (int j = 0; j < 12; j++) {
        if (this->item_[j].index_ != 0 && this->item_[j].equip_ != 0) {
            if (status::UseItem::getItemType(this->item_[j].index_) == 1) {
                sp18[targetIdx++] = this->item_[j];
                this->item_[j].index_ = 0;
            }
        }
    }
    for (int j = 0; j < 12; j++) {
        if (this->item_[j].index_ != 0 && this->item_[j].equip_ != 0) {
            if (status::UseItem::getItemType(this->item_[j].index_) == 2) {
                sp18[targetIdx++] = this->item_[j];
                this->item_[j].index_ = 0;
            }
        }
    }
    for (int j = 0; j < 12; j++) {
        if (this->item_[j].index_ != 0 && this->item_[j].equip_ != 0) {
            if (status::UseItem::getItemType(this->item_[j].index_) == 3) {
                sp18[targetIdx++] = this->item_[j];
                this->item_[j].index_ = 0;
            }
        }
    }
    for (int j = 0; j < 12; j++) {
        if (this->item_[j].index_ != 0 && this->item_[j].equip_ != 0) {
            if (status::UseItem::getItemType(this->item_[j].index_) == 4) {
                sp18[targetIdx++] = this->item_[j];
                this->item_[j].index_ = 0;
            }
        }
    }
    for (int j = 0; j < 12; j++) {
        if (this->item_[j].index_ != 0) {
            sp18[targetIdx++] = this->item_[j];
            this->item_[j].index_ = 0;
        }
    }
    for (int i = 0; i < 12; i++) {
        if (sp18[i].index_ != 0) {
            this->item_[i] = sp18[i];
        }
    }

}

THUMB int status::HaveItem::isSpace() {
    if (getCount() < 0xC) {
        return 1;
    }
    return 0;
}

