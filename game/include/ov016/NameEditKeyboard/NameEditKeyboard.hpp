#pragma once
#include "globaldefs.h"

// one key of the name entry keyboard: neighbour keys for the d-pad and what the key does
struct NameEditKey {
    int id_;            // 0x00
    int up_;            // 0x04
    int down_;          // 0x08
    int left_;          // 0x0C
    int right_;         // 0x10
    int type_;          // 0x14  0: character, 2: change page, 6: none
    int code_;          // 0x18
};

// character written by a key of the current keyboard page
struct NameEditChar {
    int id_;            // 0x00
    int code_;          // 0x04  UTF-8 bytes
};

// d-pad cursor of the name entry keyboard (MaterielMenu_NameEdit + 0xA0)
struct NameEditKeyboard {
    int index_;                     // 0x00
    NameEditKey* keyTable_;      // 0x04
    NameEditKey* key_;           // 0x08
    int type_;                      // 0x0C
    NameEditChar* charTable_;    // 0x10
    int keyCount_;                  // 0x14

    void init();
    int pressKey();
    int getCurrentChar();
    int togglePage();
    int moveUp();
    int moveDown();
    int moveLeft();
    int moveRight();
    int moveCursor(int direction);
    int isNeighbourKey(int direction, int index);
    int setCursorToKey(int code);
    int getKeyChar(int code);
    int getKeyType(int code);
};
