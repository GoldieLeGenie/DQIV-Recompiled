#pragma once
#include <globaldefs.h>

// Icon graphics files of the menus (data/G2D/icon/*.mpt)
void unkfunc_02051740();                        // load the icon files
void unkfunc_020517dc(int type);                // reload the item icons (0: items, 1: surechigai)
void* unkfunc_0205182c(int type, int index);    // icon data of a file (type 0xf0000000...), index 9999: palette
