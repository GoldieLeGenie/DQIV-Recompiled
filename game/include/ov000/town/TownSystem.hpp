#pragma once
#include "main/sound/SoundManager.hpp"
#include "globaldefs.h"
#include "main/global/StageLink.hpp"
#include "main/dss/Render.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/cmn/NonBattleActionManager.hpp"

struct TownSystem;
struct TownStageManager;
struct TownFurnitureManager;
struct TownPlayerManager;
namespace encount { struct Encount; }

struct TownSystem {
    Render render_;                                                                 // 0x000
    int playExitSE_;                                                                // 0x608
    int defaultSELock_;                                                             // 0x60C
    int scriptLock_;                                                                // 0x610
    int trigger_;                                                                   // 0x614
    int fadeCount_;                                                                 // 0x618

    TownSystem();
    static TownSystem* getSingleton();
    static void unkfunc_02132210();
    void initialize();
    void terminate();
    void execute();
    void draw();
    void bookingMenu();
    void playTownExitSE()
    {
        if (playExitSE_ && !defaultSELock_) {
            Sound::sePlayDirect(0x131);
        }
    }
};
