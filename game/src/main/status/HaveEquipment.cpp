#include "main/status/HaveEquipment.hpp"
#include "main/status/UseItem.hpp"
#include "main/param/Param.hpp"
#include "main/status/ExcelParam.hpp"

THUMB status::HaveEquipment::HaveEquipment() {
    this->weapon_ = 0;
    this->armor_ = 0;
    this->shield_ = 0;
    this->helmet_ = 0;
    this->accessory_ = 0;
    this->medicine_.flag_ = 0;
    this->strength_ = 0;
    this->agility_ = 0;
    this->protection_ = 0;
    this->wisdom_ = 0;
    this->luck_ = 0;
    this->attack_ = 0;
    this->defence_ = 0;
}


THUMB status::HaveEquipment::~HaveEquipment()
{
  return;
}

THUMB void status::HaveEquipment::setup(unsigned int index)
{
  clear();
  this->playerIndex_ = index;

  const param::CharInitData *charaInit = status::excelParam.getCharaInitData();

  setEquipment2(ITEM_WEAPON, charaInit[index].weapon);
  setEquipment2(ITEM_ARMOR, charaInit[index].armor);
  setEquipment2(ITEM_SHIELD, charaInit[index].shield);
  setEquipment2(ITEM_HELMET, charaInit[index].hat);
  setEquipment2(ITEM_ACCESSORY, 0);
}

THUMB void status::HaveEquipment::clear()
{
  this->weapon_ = 0;
  this->armor_ = 0;
  this->shield_ = 0;
  this->helmet_ = 0;
  this->accessory_ = 0;
  this->medicine_.flag_ = 0;
  this->strength_ = 0;
  this->agility_ = 0;
  this->protection_ = 0;
  this->wisdom_ = 0;
  this->luck_ = 0;
  this->attack_ = 0;
  this->defence_ = 0;
}

THUMB void status::HaveEquipment::setEquipment2(ItemType type, unsigned int itemIndex)
{
  if (itemIndex != 0)
  {
    switch (type)
    {
    case ITEM_WEAPON:
      this->weapon_ = itemIndex;
      this->beSpell(ITEM_WEAPON, itemIndex);
      return;
    case ITEM_ARMOR:
      this->armor_ = itemIndex;
      this->beSpell(ITEM_ARMOR, itemIndex);
      return;
    case ITEM_SHIELD:
      this->shield_ = itemIndex;
      this->beSpell(ITEM_SHIELD, itemIndex);
      return;
    case ITEM_HELMET:
      this->helmet_ = itemIndex;
      this->beSpell(ITEM_HELMET, itemIndex);
      return;
    case ITEM_ACCESSORY:
      this->accessory_ = itemIndex;
      this->beSpell(ITEM_ACCESSORY, itemIndex);
      return;
    }
  }
  return;
}

THUMB void status::HaveEquipment::setEquipment(unsigned int itemIndex)
{
  switch (UseItem::getItemType(itemIndex))
  {
  case ITEM_WEAPON:
    resetEquipment(ITEM_WEAPON);
    setEquipment2(ITEM_WEAPON, itemIndex);
    return;
  case ITEM_ARMOR:
    resetEquipment(ITEM_ARMOR);
    setEquipment2(ITEM_ARMOR, itemIndex);
    return;
  case ITEM_SHIELD:
    resetEquipment(ITEM_SHIELD);
    setEquipment2(ITEM_SHIELD, itemIndex);
    return;
  case ITEM_HELMET:
    resetEquipment(ITEM_HELMET);
    setEquipment2(ITEM_HELMET, itemIndex);
    return;
  case ITEM_ACCESSORY:
    resetEquipment(ITEM_ACCESSORY);
    setEquipment2(ITEM_ACCESSORY, itemIndex);
    return;
  }
  return;
}

THUMB void status::HaveEquipment::resetEquipment(ItemType type)
{
  switch (type)
  {
  case ITEM_WEAPON:
    this->weapon_ = 0;
    return;
  case ITEM_ARMOR:
    this->armor_ = 0;
    return;
  case ITEM_SHIELD:
    this->shield_ = 0;
    return;
  case ITEM_HELMET:
    this->helmet_ = 0;
    return;
  case ITEM_ACCESSORY:
    this->accessory_ = 0;
    return;
  }
  return;
}

THUMB void status::HaveEquipment::removeItem(unsigned int ItemIndex)
{
  if (this->weapon_ == ItemIndex)
  {
    resetEquipment(ITEM_WEAPON);
  }
  if (this->armor_ == ItemIndex)
  {
    resetEquipment(ITEM_ARMOR);
  }
  if (this->shield_ == ItemIndex)
  {
    resetEquipment(ITEM_SHIELD);
  }
  if (this->helmet_ == ItemIndex)
  {
    resetEquipment(ITEM_HELMET);
  }
  if (this->accessory_ == ItemIndex)
  {
    resetEquipment(ITEM_ACCESSORY);
  }
  return;
}

THUMB unsigned char status::HaveEquipment::getEquipment(ItemType type)
{
  switch (type)
  {
  case ITEM_WEAPON:
    return this->weapon_;
  case ITEM_ARMOR:
    return this->armor_;
  case ITEM_SHIELD:
    return this->shield_;
  case ITEM_HELMET:
    return this->helmet_;
  case ITEM_ACCESSORY:
    return this->accessory_;
  default:
    return 0;
  }
}

THUMB bool status::HaveEquipment::isEquipment(int itemIndex)
{
  if (this->weapon_ == itemIndex)
    return true;
  if (this->armor_ == itemIndex)
    return true;
  if (this->shield_ == itemIndex)
    return true;
  if (this->helmet_ == itemIndex)
    return true;
  if (this->accessory_ == itemIndex)
    return true;
  return false;
}

THUMB void status::HaveEquipment::recalculateStats()
{
  this->strength_ = 0;
  this->agility_ = 0;
  this->wisdom_ = 0;
  this->luck_ = 0;
  this->attack_ = 0;
  this->defence_ = 0;
  calcEffect(this->weapon_);
  calcEffect(this->armor_);
  calcEffect(this->shield_);
  calcEffect(this->helmet_);
  calcEffect(this->accessory_);
  return;
}

THUMB void status::HaveEquipment::calcEffect(int itemIndex)
{
  EquipType equipType = UseItem::getEquipType(itemIndex);
  int equipValue = UseItem::getEquipValue(itemIndex);

  switch (equipType)
  {
  case Strength:
    this->strength_ += equipValue;
    break;
  case Agility:
    if (getAbsoluteValue(equipValue) != 10000)
      this->agility_ += equipValue;
    break;
  case Wisdom:
    this->wisdom_ += equipValue;
    break;
  case Luck:
    this->luck_ += equipValue;
    break;
  case Attack:
    this->attack_ += equipValue;
    break;
  case Defence:
    this->defence_ += equipValue;
    break;
  }
  return;
}

THUMB int status::HaveEquipment::getAbsoluteValue(int ItemIndex)
{
  if (ItemIndex < 0)
  {
    ItemIndex = -ItemIndex;
  }
  return ItemIndex;
}

THUMB void status::HaveEquipment::beSpell(ItemType type, int itemIndex)
{
  if (UseItem::isCurse(itemIndex) != 0)
  {
    switch (type)
    {
    case ITEM_WEAPON:
      this->medicine_.flag_ = this->medicine_.flag_ | 1;
      break;
    case ITEM_ARMOR:
      this->medicine_.flag_ = this->medicine_.flag_ | 2;
      break;
    case ITEM_SHIELD:
      this->medicine_.flag_ = this->medicine_.flag_ | 4;
      break;
    case ITEM_HELMET:
      this->medicine_.flag_ = this->medicine_.flag_ | 8;
      break;
    case ITEM_ACCESSORY:
      this->medicine_.flag_ = this->medicine_.flag_ | 0x10;
      break;
    }
  }
  return;
}

THUMB void status::HaveEquipment::breakSpell(ItemType type)
{
  switch (type)
  {
  case ITEM_WEAPON:
    this->medicine_.flag_ = this->medicine_.flag_ & ~1;
    break;
  case ITEM_ARMOR:
    this->medicine_.flag_ = this->medicine_.flag_ & ~2;
    break;
  case ITEM_SHIELD:
    this->medicine_.flag_ = this->medicine_.flag_ & ~4;
    break;
  case ITEM_HELMET:
    this->medicine_.flag_ = this->medicine_.flag_ & ~8;
    break;
  case ITEM_ACCESSORY:
    this->medicine_.flag_ = this->medicine_.flag_ & ~0x10;
  default:
    break;
  }
}

THUMB int status::HaveEquipment::isSpell(ItemType type)
{
  switch (type)
  {
  case ITEM_WEAPON:
    if (this->medicine_.flag_ & 1)
    {
      return 1;
    }
    break;
  case ITEM_ARMOR:
    if (this->medicine_.flag_ & 2)
    {
      return 1;
    }
    break;
  case ITEM_SHIELD:
    if (this->medicine_.flag_ & 4)
    {
      return 1;
    }
    break;
  case ITEM_HELMET:
    if (this->medicine_.flag_ & 8)
    {
      return 1;
    }
    break;
  case ITEM_ACCESSORY:
    if (this->medicine_.flag_ & 0x10)
    {
      return 1;
    }
  }
  return 0;
}

THUMB int status::HaveEquipment::hasSpellEffect()
{
  if (isSpell(ITEM_WEAPON))
    return 1;
  if (isSpell(ITEM_ARMOR))
    return 1;
  if (isSpell(ITEM_SHIELD))
    return 1;
  if (isSpell(ITEM_HELMET))
    return 1;
  if (isSpell(ITEM_ACCESSORY))
    return 1;
  return 0;
}

THUMB void status::HaveEquipment::setSaveDataForPlayer(profile::PROFILE_PLAYER *data)
{
  data->EQUIP_WEAPON = this->weapon_;
  data->EQUIP_ARMOR = this->armor_;
  data->EQUIP_SHIELD = this->shield_;
  data->EQUIP_HELMET = this->helmet_;
  data->EQUIP_ACCESSORY = this->accessory_;
  data->EQUIP_MEDICINE = this->medicine_.flag_;
  return;
}

THUMB void status::HaveEquipment::setLoadDataForPlayer(profile::PROFILE_PLAYER *data)
{
  this->weapon_ = data->EQUIP_WEAPON;
  this->armor_ = data->EQUIP_ARMOR;
  this->shield_ = data->EQUIP_SHIELD;
  this->helmet_ = data->EQUIP_HELMET;
  this->accessory_ = data->EQUIP_ACCESSORY;
  this->medicine_.flag_ = data->EQUIP_MEDICINE;
  return;
}