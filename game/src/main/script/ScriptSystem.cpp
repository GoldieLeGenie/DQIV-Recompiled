#include "main/script/ScriptSystem.hpp"
#include "main/status/StageStatus.hpp"

ARM ScriptSystem::ScriptSystem()
{
}

ARM ScriptSystem::~ScriptSystem()
{
}

ARM ScriptSystem* ScriptSystem::getSingleton()
{
    static ScriptSystem scriptSystem;
    return &scriptSystem;
}

ARM void ScriptSystem::setup(char* fname)
{
    dataObject_.setup(fname, 1, 0);
    setup();
}

ARM void ScriptSystem::setup()
{
    PlacementParameter::setStartFunction(StartFunction);
    ScriptParam::setExecuteCommandFunction((bool (*)(void*))CommandFunction);
    ScriptObject::setSetCtrlIdFunction(setPlacementCtrlId);
    initializeScriptObjectEnable();
    ScriptGroup::setScriptObjectEnableFunction((bool (*)(int))isScriptObjectEnable);
    initializeScriptObjectCtrl();
    ScriptGroup::setScriptObjectCtrlFunction(setScriptObjectCtrl);
    scriptEngine_.setup(dataObject_.getAddr(), chapter_);
    executeEnable_ = 1;
}

ARM void ScriptSystem::cleanup()
{
    scriptEngine_.cleanup();
    dataObject_.cleanup();
}

ARM void ScriptSystem::initialize(int chapter)
{
    char fname[128];
    chapter_ = chapter;
    if (chapter == 6) {
        chapter_ = 5;
    }
    dss::sprintf_s(fname, sizeof(fname), "data/script/%s.bin", g_Stage.getMapName());
    if (dss::g_File.isExist(fname)) {
        setup(fname);
        flag_ = 1;
    } else {
        flag_ = 0;
        return;
    }
    scriptEngine_.initialize();
}

ARM void ScriptSystem::terminate()
{
    if (flag_ == 0) {
        return;
    }
    scriptEngine_.terminate();
    cleanup();
}

ARM void ScriptSystem::execute()
{
    if (flag_ == 0 || executeEnable_ == 0) {
        return;
    }
    scriptEngine_.execute();
}
