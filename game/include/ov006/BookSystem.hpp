#pragma once
#include <globaldefs.h>
#include "main/dss/Render.hpp"
#include "main/dss/UnkSprite2D.hpp"

// monster book owns the render list and the background sprite)
struct BookSystem {
    Render render_;                             // 0x000 DS-only
    UnkMenuSprite sprite_;                      // 0x608 DS-only (encyclopedia background)
    int monsterNo_;                             // 0x658

    BookSystem();
    ~BookSystem();
    static void unkfunc_021219ac();             // empty, called by BookPart right after loading the overlay
    static BookSystem* getSingleton();
    void initialize();
    void terminate();
    void execute();
    void draw();
};
