#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/dss/UnkFog.hpp"
#include "main/data/DataObject.hpp"
#include "main/param/Param.hpp"
#include "main/param/ColorCorrect.hpp"

struct TownDataManager {
    int correctTime_;                           // 0x00
    dss::Fix32Vector3 rate_;                    // 0x04
    int nextIndex_;                             // 0x10
    DataObject stageData_;                      // 0x14
    param::ColorCorrect* pCorrect_;             // 0x24
    param::FloorFog* pFloorFog_;                // 0x28
    param::CLUTCode* pCLUTCode_;                // 0x2C
    param::FloorBackColor* pBackColor_;         // 0x30

    TownDataManager();
    ~TownDataManager();
    void initialize();
    void terminate();
    void loadStage(char* filename);
    void setFloorfog(int index);
    void setTimezone(int index);
    dss::Fix32Vector3 getDefaultPaletteRate();
    void setBackcolor(int index);
    int getCurrentBackColor();

    void setNextBackColor(int index) { nextIndex_ = index; }
    int getNextBackColor() { return nextIndex_; }
};
