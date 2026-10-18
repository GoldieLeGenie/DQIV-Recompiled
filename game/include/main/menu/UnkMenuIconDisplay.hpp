#pragma once
#include <globaldefs.h>
#include "main/menu/MenuDisplay.hpp"
#include "main/menu/MenuIconData.hpp"

struct UnkHoppingDigit {
    int frame_;                                 // 0x00  -99: free, < 0: waiting
    int number_;                                // 0x04
    int x_;                                     // 0x08
    int y_;                                     // 0x0C
    int drawY_;                                 // 0x10

    void clear();
    void start(int x, int y, int number, int delay);
    void update(UnkOamBuffer* oam);
    void draw(UnkOamBuffer* oam);
    int isFree();
};


struct UnkHoppingNumber : UnkMenuDisplay {
    UnkHoppingDigit digit_[40];                 // 0x30

    UnkHoppingNumber();
    virtual void setup(int id);
    virtual void update(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void execute(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void draw(UnkOamBuffer* main, UnkOamBuffer* sub);
    UnkHoppingDigit* getFreeDigit();
    void startNumber(int x, int y, int value, int delay);
};

// Four 48x48 icons on the main screen 
struct UnkMenuIconDisplay : UnkMenuDisplay {
    int posX_[4];                               // 0x30
    int posY_[4];                               // 0x40
    int value_[4];                              // 0x50
    int flag_[4];                               // 0x60

    UnkMenuIconDisplay();
    virtual void setup(int id);
    virtual void update(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void execute(UnkOamBuffer* main, UnkOamBuffer* sub);
    virtual void draw(UnkOamBuffer* main, UnkOamBuffer* sub);
    void drawIcon(UnkOamBuffer* oam, int x, int y, int index);
    void loadIcon(int index, int value);
    void setIconPosition(int index, int x, int y);
    void setIconVisible(int index, int flag);
    void transferIconTiles(int x, int y, void* src, int srcX, int srcY, int width);
};

