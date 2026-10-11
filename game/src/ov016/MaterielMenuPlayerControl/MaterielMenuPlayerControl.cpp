#include "main/menu/MaterielMenuPlayerControl.hpp"

THUMB MaterielMenuPlayerControl* MaterielMenuPlayerControl::getSingleton()
{
    static MaterielMenuPlayerControl playerControl;
    return &playerControl;
}

THUMB void MaterielMenuPlayerControl::allClear()
{
    activeChara_ = 0;
    leadpc_ = 0;
    activeItem_ = 0;
    activeItemPage_ = 0;
    churchCommandNum_ = 0;
    wins_ = 0;
}
