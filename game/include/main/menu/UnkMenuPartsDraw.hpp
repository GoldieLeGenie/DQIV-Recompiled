#pragma once
#include <globaldefs.h>
#include "main/menu/MenuBase.hpp"
#include "main/dss/UnkBgBuffer.hpp"

extern int data_021098a4;                       // origin of the parts
extern int data_021098a8;
extern int data_021098ac;                       // 1: sub screen BGs set up
extern UnkScreenBuffer* data_021098b0;          // back BG screen
extern UnkScreenBuffer* data_021098b4;          // window BG screen
extern UnkCharBuffer* data_021098b8;            // window BG characters

// Menu parts drawn on the sub screen BGs (window frames, tiles, texts, numbers, gauges, icons, faces, arrows)
void unkfunc_02050614(int enable);              // set up the sub screen BGs
void unkfunc_02050698(int x, int y);            // origin of the parts
void unkfunc_020506a4(int x, int y, int w, int h, int palette);
void unkfunc_020506ec(int x, int y, int w, int h, int palette);
void unkfunc_02050734(int x, int y, int w, int h, int palette);
void unkfunc_0205077c(int x, int y, int w, int h);  // clear a rect of both BGs
void unkfunc_020507a8(int x, int y, int w, int h, int xlu);
void unkfunc_020507ec(int x, int y, int w, int h, int priority, int flag);  // frame on data_020f530c.frames_
void unkfunc_02050820(int x, int y, int w, int h, int type, int index);     // icon
void unkfunc_02050860(int x, int y, int w, int h, int palette, int flip, int bank, int chr);
int unkfunc_02050900(int x, int y, int w, int h, int palette, int color, int align, int font, int value, int wrap);
int unkfunc_02050c70(int x, int y, int w, int h, int palette, int color, int align, int font, int value);
void unkfunc_02050d00(int x, int y, int percent);   // HP gauge
int unkfunc_02050e20(int index, const char* text);  // name plate width
void unkfunc_02050e44(int index, int x, int y, int priority, int flag);    // show a name plate
#ifdef DQIV_NATIVE
void unkfunc_native_platePalette(int index, int palette);                 // a name plate letters' OBJ palette
#endif
void unkfunc_02050e88(int x, int y, int value, int delay);                 // hopping number
void unkfunc_02050ea8(UnkMenuParts* parts, int* param);
void unkfunc_02050ebc(UnkMenuParts* parts, int* param, int x, int y);
void unkfunc_02050ed0(UnkMenuParts* parts, int* param, int color);
void unkfunc_02050ee0(UnkMenuParts* parts, int* param, int x, int y, int color);
void unkfunc_02050f1c(UnkMenuParts* part, int* param, int x, int y);
void unkfunc_02050f30(UnkMenuParts* part, int* param, int x, int y, int color);
