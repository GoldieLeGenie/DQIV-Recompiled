#include "main/script/sys/ScriptObject.hpp"

void (*ScriptObject::setCtrlIdFunction_)(int);

ARM ScriptObject::ScriptObject()
{
}

ARM ScriptObject::~ScriptObject()
{
}

ARM void ScriptObject::setup(void* addr)
{
    dataObject_.setup(addr);
    setup();
}

ARM void ScriptObject::setup()
{
    placeParam_.setup(unkfunc_0207f8dc(dataObject_.getAddr(), 0));
    initializeScriptParam_.setup(unkfunc_0207f8dc(dataObject_.getAddr(), 1));
    executeScriptParam_.setup(unkfunc_0207f8dc(dataObject_.getAddr(), 2));
    terminateScriptParam_.setup(unkfunc_0207f8dc(dataObject_.getAddr(), 3));
}

ARM void ScriptObject::cleanup()
{
    placeParam_.cleanup();
    initializeScriptParam_.cleanup();
    executeScriptParam_.cleanup();
    terminateScriptParam_.cleanup();
    dataObject_.cleanup();
}

ARM int ScriptObject::place()
{
    return ctrlId_ = placeParam_.execute();
}

ARM void ScriptObject::initialize()
{
    setCtrlIdFunction_(ctrlId_);
    initializeScriptParam_.execute();
}

ARM void ScriptObject::terminate()
{
    setCtrlIdFunction_(ctrlId_);
    terminateScriptParam_.execute();
}

ARM void ScriptObject::execute()
{
    setCtrlIdFunction_(ctrlId_);
    executeScriptParam_.execute();
}

ARM void ScriptObject::setSetCtrlIdFunction(void (*fc)(int))
{
    setCtrlIdFunction_ = fc;
}
