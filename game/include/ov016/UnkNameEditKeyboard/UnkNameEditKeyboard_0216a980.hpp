#pragma once
#include "globaldefs.h"

// one key of the name entry keyboard: neighbour keys for the d-pad and what the key does
struct UnkNameEditKey {
    int id_;            // 0x00
    int up_;            // 0x04
    int down_;          // 0x08
    int left_;          // 0x0C
    int right_;         // 0x10
    int type_;          // 0x14  0: character, 2: change page, 6: none
    int code_;          // 0x18
};

// character written by a key of the current keyboard page
struct UnkNameEditChar {
    int id_;            // 0x00
    int code_;          // 0x04  UTF-8 bytes
};

// d-pad cursor of the name entry keyboard (MaterielMenu_NameEdit + 0xA0)
struct UnkNameEditKeyboard {
    int index_;                     // 0x00
    UnkNameEditKey* keyTable_;      // 0x04
    UnkNameEditKey* key_;           // 0x08
    int type_;                      // 0x0C
    UnkNameEditChar* charTable_;    // 0x10
    int keyCount_;                  // 0x14

    void unkfunc_0216a980();
    int unkfunc_0216a9b4();
    int unkfunc_0216a9f8();
    int unkfunc_0216aa0c();
    int unkfunc_0216aa5c();
    int unkfunc_0216aa6c();
    int unkfunc_0216aa7c();
    int unkfunc_0216aa8c();
    int unkfunc_0216aa9c(int direction);
    int unkfunc_0216aaf8(int direction, int index);
    int unkfunc_0216abbc(int code);
    int unkfunc_0216ac08(int code);
    int unkfunc_0216ac54(int code);
};
