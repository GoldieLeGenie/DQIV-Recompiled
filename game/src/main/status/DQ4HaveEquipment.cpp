#include "main/status/DQ4HaveEquipment.hpp"


THUMB status::DQ4HaveEquipment::DQ4HaveEquipment()
{
  return;
}


THUMB status::DQ4HaveEquipment::~DQ4HaveEquipment()
{
    return;
}


THUMB void status::DQ4HaveEquipment::beSpell(ItemType type, int itemIndex) {
    HaveEquipment::beSpell(type,itemIndex);
    if (this->playerIndex_ == 25 && status::HaveEquipment::isSpell(type)) {
         HaveEquipment::breakSpell(type);
        }
    return;
}

