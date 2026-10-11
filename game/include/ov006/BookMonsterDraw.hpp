#pragma once
#include <globaldefs.h>
#include "main/cmn/CommonEffectResource.hpp"
#include "main/data/DataObject.hpp"
#include "main/object/GameMonster.hpp"
#include "main/object/PaletteAnimationObject.hpp"
#include "main/param/MonsterAnim.hpp"
#include "main/param/Param.hpp"
#include "ov006/BookEffectGroup.hpp"

struct BookSystem;

namespace param {
    MonsterAnim* getBookMonsterAnim();
    EffectParam* getBookEffectParam();
}

// monster shown in the monster book, with its attack animations/effects
struct BookMonsterDraw {
    param::MonsterData* monster_;               // 0x0000
    param::MonsterAnim* animation_;             // 0x0004
    int actionIndex_;                           // 0x0008
    int defaultAnimation_;                      // 0x000C
    int counter_;                               // 0x0010
    BookSystem* system_;                        // 0x0014 DS-only
    GameMonster character_;                     // 0x0018
    DataObject dataObject_;                     // 0x0D50 palette animation file
    PaletteAnimationObject paletteAnim_;        // 0x0D60 DS-only
    book::BookEffectGroup effect_;              // 0x0F80
    cmn::CommonEffectResource resource_;        // 0x1EA4
    int wait_;                                  // 0x2710
    int effectID_;                              // 0x2714

    BookMonsterDraw();
    ~BookMonsterDraw();
    static BookMonsterDraw* getSingleton();
    void initialize();
    void terminate();
    void setup(int index);
    void cleanup();
    void execute();
    void draw();
    void startAnimation(int animIndex);
    bool isActivate();
    void setPaletteAnim(int animNo);
    void setupEffect(int index);
    void cleanupEffect();
};
