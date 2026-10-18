#pragma once
#include <globaldefs.h>

// Menu system hooks called by the game part base (0x0203ed64...)
void loadMenuSystemFiles();                        // load menu graphics
void initMenuSystem();                        // init
void startMenuSystem();                        // start
void endMenuSystem();                        // end
void updateMenuSystem();
void executeMenuSystem();                        // execute
void drawMenuSystem();                        // draw

void setupMenuDisplays();                        // setup all menu displays
void disableMenuDisplays();                        // disable all menu displays
void updateMenuDisplays();
void executeMenuDisplays();
void drawMenuDisplays();
void moveMenuDisplays();                        // move all menu displays
void resetMenuDisplays();
void initMenuBg();
void updateMenuBg();
void executeMenuBg();
void transferMenuBg();
void initMenuMsgSystem();                        // ui_MsgSystemInit
