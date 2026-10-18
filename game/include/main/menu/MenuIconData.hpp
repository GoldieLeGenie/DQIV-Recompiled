#pragma once
#include <globaldefs.h>

// Icon graphics files of the menus (data/G2D/icon/*.mpt)
void loadMenuIconFiles();                        // load the icon files
void setItemIconType(int type);                // reload the item icons (0: items, 1: surechigai)
void* getMenuIconData(int type, int index);    // icon data of a file (type 0xf0000000...), index 9999: palette
