#pragma once
#include <globaldefs.h>
#include "main/data/DataObject.hpp"
#include "main/script/sys/ScriptTree.hpp"

struct ScriptParam {
    DataObject dataObject_;                     // 0x000
    ScriptTree scriptTree_;                     // 0x010
    int count_;                                 // 0x2A4
    char* tree_;                                // 0x2A8
    int* offset_;                               // 0x2AC
    unsigned char* command_;                    // 0x2B0

    ScriptParam();
    ~ScriptParam();
    void setup(void* addr);
    void setup();
    void cleanup();
    void execute();
    static void setExecuteCommandFunction(bool (*fc)(void*));
    bool execScriptCommand(int cmdIndex);
    bool checkScriptCommandStatus();
    void clearScriptCommandStatus(int cmdIndex);
    int getScriptCommandType(int cmdIndex);
    bool getScriptCommandClearFlag(int cmdIndex);

    static bool (*executeCommandFunction_)(void*);
};
