#include "ov016/TownMenu_ITEM/TownMenuItemUseManager.hpp"

TownMenuItemUseManager gTownMenuItemUseManager;

THUMB TownMenuItemUseManager* TownMenuItemUseManager::getSingleton()
{
    return &gTownMenuItemUseManager;
}

THUMB bool TownMenuItemUseManager::getDefaultCloseMenuItem(int item)
{
    if (item == 0x74) {
        return true;
    }
    return false;
}
