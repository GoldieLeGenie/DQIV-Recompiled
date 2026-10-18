#pragma once
#include <globaldefs.h>
#include "main/menu/MenuBase.hpp"
#include "main/dss/BgBuffer.hpp"

extern int data_021098a4;                       // origin of the parts
extern int data_021098a8;
extern int data_021098ac;                       // 1: sub screen BGs set up
extern UnkScreenBuffer* data_021098b0;          // back BG screen
extern UnkScreenBuffer* data_021098b4;          // window BG screen
extern UnkCharBuffer* data_021098b8;            // window BG characters

// Menu parts drawn on the sub screen BGs (window frames, tiles, texts, numbers, gauges, icons, faces, arrows)
void setupMenuBgDraw(int enable);              // set up the sub screen BGs
void setMenuDrawOffset(int x, int y);            // origin of the parts
void fillMenuBackRect(int x, int y, int w, int h, int palette);
void drawMenuWindowBack(int x, int y, int w, int h, int palette);
void drawMenuWindowBorder(int x, int y, int w, int h, int palette);
void drawMenuWindowBox(int x, int y, int w, int h);  // clear a rect of both BGs
void drawMenuObjFrame(int x, int y, int w, int h, int xlu);
void drawMenuItemFrame(int x, int y, int w, int h, int priority, int flag);  // frame on data_020f530c.frames_
void drawMenuIcon(int x, int y, int w, int h, int type, int index);     // icon
void drawMenuSystemTiles(int x, int y, int w, int h, int palette, int flip, int bank, int chr);
int drawMenuText(int x, int y, int w, int h, int palette, int color, int align, int font, int value, int wrap);
int drawMenuNumber(int x, int y, int w, int h, int palette, int color, int align, int font, int value);
void drawMenuHpGauge(int x, int y, int percent);   // HP gauge
int setNamePlateText(int index, const char* text);  // name plate width
void showNamePlate(int index, int x, int y, int priority, int flag);    // show a name plate
#ifdef DQIV_NATIVE
void unkfunc_native_platePalette(int index, int palette);                 // a name plate letters' OBJ palette
#endif
void showHoppingNumber(int x, int y, int value, int delay);                 // hopping number
void drawMenuPartsPlain(UnkMenuParts* parts, int* param);
void drawMenuPartsAt(UnkMenuParts* parts, int* param, int x, int y);
void drawMenuPartsColored(UnkMenuParts* parts, int* param, int color);
void drawMenuParts(UnkMenuParts* parts, int* param, int x, int y, int color);
void drawMenuPartAt(UnkMenuParts* part, int* param, int x, int y);
void drawMenuPart(UnkMenuParts* part, int* param, int x, int y, int color);
