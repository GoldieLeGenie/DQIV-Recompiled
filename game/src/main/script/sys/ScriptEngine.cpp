#include "main/script/sys/ScriptEngine.hpp"

ARM ScriptEngine::ScriptEngine()
{
    chapter_ = 0;
    enable_ = 0;
}

ARM ScriptEngine::~ScriptEngine()
{
}

ARM void ScriptEngine::setup(void* addr, int chapter)
{
    dataObject_.setup(addr);
    chapter_ = chapter;
    setup();
}

ARM void ScriptEngine::setup()
{
    scriptGroup_.setup(unkfunc_0207f8dc(dataObject_.getAddr(), chapter_));
    enable_ = 1;
}

ARM void ScriptEngine::cleanup()
{
    scriptGroup_.cleanup();
    dataObject_.cleanup();
}

ARM void ScriptEngine::initialize()
{
    if (enable_ == 0) {
        return;
    }
    scriptGroup_.initialize();
}

ARM void ScriptEngine::terminate()
{
    if (enable_ == 0) {
        return;
    }
    scriptGroup_.terminate();
}

ARM void ScriptEngine::execute()
{
    if (enable_ == 0) {
        return;
    }
    scriptGroup_.execute();
}
