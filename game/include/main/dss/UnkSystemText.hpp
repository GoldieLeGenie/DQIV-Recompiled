#pragma once
#include <globaldefs.h>

extern void* data_0211fd20[5];                  // system text resources (character, font, palettes)

void unkfunc_02086de4(void* res0, void* res1, void* res2, void* res3, void* res4);    // set the resources
void* unkfunc_02086e50(int index);              // character resource
void* unkfunc_02086e60(int index);              // palette resource
void unkfunc_02086e70(int type, int screen1, int screen2);  // transfer the characters
void unkfunc_02086f6c(int type, int screen1, int screen2);  // transfer the palettes
