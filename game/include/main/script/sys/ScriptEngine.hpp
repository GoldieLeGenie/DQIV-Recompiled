#pragma once
#include <globaldefs.h>
#include "main/data/DataObject.hpp"
#include "main/script/sys/ScriptGroup.hpp"

struct ScriptEngine {
    DataObject dataObject_;                     // 0x00000
    ScriptGroup scriptGroup_;                   // 0x00010
    int chapter_;                               // 0x199BC
    int enable_;                                // 0x199C0

    ScriptEngine();
    ~ScriptEngine();
    void setup(void* addr, int chapter);
    void setup();
    void cleanup();
    void initialize();
    void terminate();
    void execute();
};
