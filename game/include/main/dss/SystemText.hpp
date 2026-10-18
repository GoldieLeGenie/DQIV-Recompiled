#pragma once
#include <globaldefs.h>

extern void* data_0211fd20[5];                  // system text resources (character, font, palettes)

void initSystemText(void* res0, void* res1, void* res2, void* res3, void* res4);    // set the resources
void* getSystemTextData(int index);              // character resource
void* getSystemTextPalette(int index);              // palette resource
void loadSystemTextChars(int type, int screen1, int screen2);  // transfer the characters
void loadSystemTextPalettes(int type, int screen1, int screen2);  // transfer the palettes
