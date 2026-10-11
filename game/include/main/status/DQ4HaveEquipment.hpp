#pragma once
#include "main/status/HaveEquipment.hpp"
#include <globaldefs.h>


namespace status{
    struct DQ4HaveEquipment : HaveEquipment {
        DQ4HaveEquipment();
        ~DQ4HaveEquipment();
        virtual void beSpell(ItemType type, int itemIndex);
    };
}