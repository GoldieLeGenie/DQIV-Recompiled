#pragma once
#include <globaldefs.h>
#include "main/data/DataObject.hpp"
#include "main/script/sys/ScriptObject.hpp"

#define SCRIPT_OBJECT_MAX 48

struct ScriptGroup {
    DataObject dataObject_;                                     // 0x00000
    ScriptObject mainScriptObject_;                             // 0x00010
    ScriptObject scriptObject_[SCRIPT_OBJECT_MAX];              // 0x00868
    int scriptObjectCount_;                                     // 0x198E8
    int scriptObjectEnableFlag_[SCRIPT_OBJECT_MAX];             // 0x198EC

    ScriptGroup();
    ~ScriptGroup();
    void setup(void* addr);
    void setup();
    void cleanup();
    void initialize();
    void terminate();
    void execute();
    static void setScriptObjectEnableFunction(bool (*fc)(int));
    static void setScriptObjectCtrlFunction(void (*fc)(int, int));

    static bool (*scriptObjectEnableFunction_)(int);
    static void (*scriptObjectCtrlFunction_)(int, int);
};
