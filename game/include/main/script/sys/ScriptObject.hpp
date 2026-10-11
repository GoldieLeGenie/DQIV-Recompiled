#pragma once
#include <globaldefs.h>
#include "main/data/DataObject.hpp"
#include "main/script/sys/PlacementParameter.hpp"
#include "main/script/sys/ScriptParam.hpp"

struct ScriptObject {
    DataObject dataObject_;                     // 0x000
    PlacementParameter placeParam_;             // 0x010
    ScriptParam initializeScriptParam_;         // 0x038
    ScriptParam executeScriptParam_;            // 0x2EC
    ScriptParam terminateScriptParam_;          // 0x5A0
    int ctrlId_;                                // 0x854

    ScriptObject();
    ~ScriptObject();
    void setup(void* addr);
    void setup();
    void cleanup();
    int place();
    void initialize();
    void terminate();
    void execute();
    static void setSetCtrlIdFunction(void (*fc)(int));

    static void (*setCtrlIdFunction_)(int);
};

