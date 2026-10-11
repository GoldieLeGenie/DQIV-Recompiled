#include "main/status/BaseItem.hpp"

param::ItemData* dataItem_;

THUMB void status::BaseItem::initialize()
{
  dataItem_ = status::excelParam.getItemData();
}