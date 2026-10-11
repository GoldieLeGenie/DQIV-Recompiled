#pragma once
#include "main/dss/DssUtils.hpp"
#include "globaldefs.h"
#include "main/data/DataObject.hpp"
#include "main/data/FileLoader.hpp"
#include "main/script/sys/ScriptEngine.hpp"
#include "main/script/ScriptBaseCommand.hpp"

struct CommandParameter;

struct ScriptSystem {
    int flag_;                                  // 0x00000
    int chapter_;                               // 0x00004
    DataObject dataObject_;                     // 0x00008
    ScriptEngine scriptEngine_;                 // 0x00018
    int executeEnable_;                         // 0x199DC

    ScriptSystem();
    ~ScriptSystem();
    static ScriptSystem* getSingleton();
    void setup(char* fname);
    void setup();
    void cleanup();
    void initialize(int chapter);
    void terminate();
    void execute();
};

int CommandFunction(CommandParameter* param);

