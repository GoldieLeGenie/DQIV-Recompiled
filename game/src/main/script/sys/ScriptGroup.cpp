#include "main/script/sys/ScriptGroup.hpp"

bool (*ScriptGroup::scriptObjectEnableFunction_)(int);
void (*ScriptGroup::scriptObjectCtrlFunction_)(int, int);

ARM ScriptGroup::ScriptGroup()
{
}

ARM ScriptGroup::~ScriptGroup()
{
}

ARM void ScriptGroup::setup(void* addr)
{
    dataObject_.setup(addr);
    setup();
}

ARM void ScriptGroup::setup()
{
    mainScriptObject_.setup(unkfunc_0207f8dc(dataObject_.getAddr(), 0));
    scriptObjectCount_ = unkfunc_0207f8c4(dataObject_.getAddr()) - 1;
    for (int i = 0; i < scriptObjectCount_; i++) {
        scriptObject_[i].setup(unkfunc_0207f8dc(dataObject_.getAddr(), i + 1));
    }
    for (int i = 0; i < SCRIPT_OBJECT_MAX; i++) {
        scriptObjectEnableFlag_[i] = 0;
    }
}

ARM void ScriptGroup::cleanup()
{
    mainScriptObject_.cleanup();
    for (int i = 0; i < scriptObjectCount_; i++) {
        scriptObject_[i].cleanup();
    }
    scriptObjectCount_ = 0;
    dataObject_.cleanup();
}

ARM void ScriptGroup::initialize()
{
    mainScriptObject_.initialize();
    for (int i = 0; i < SCRIPT_OBJECT_MAX; i++) {
        if (scriptObjectEnableFunction_(i)) {
            scriptObjectEnableFlag_[i] = 1;
        } else {
            scriptObjectEnableFlag_[i] = 0;
        }
    }
    for (int i = 0; i < scriptObjectCount_; i++) {
        if (scriptObjectEnableFlag_[i]) {
            int ctrl = scriptObject_[i].place();
            scriptObjectCtrlFunction_(ctrl, i);
            scriptObject_[i].initialize();
        }
    }
}

ARM void ScriptGroup::terminate()
{
    mainScriptObject_.terminate();
    for (int i = 0; i < scriptObjectCount_; i++) {
        if (scriptObjectEnableFlag_[i]) {
            scriptObject_[i].terminate();
        }
    }
}

ARM void ScriptGroup::execute()
{
    mainScriptObject_.execute();
    for (int i = 0; i < scriptObjectCount_; i++) {
        if (scriptObjectEnableFlag_[i]) {
            scriptObject_[i].execute();
        }
    }
}

ARM void ScriptGroup::setScriptObjectEnableFunction(bool (*fc)(int))
{
    scriptObjectEnableFunction_ = fc;
}

ARM void ScriptGroup::setScriptObjectCtrlFunction(void (*fc)(int, int))
{
    scriptObjectCtrlFunction_ = fc;
}
