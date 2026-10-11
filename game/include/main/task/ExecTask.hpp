#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"

struct ExecTask {
    // vtable                           // 0x00
    dss::Flag flag_;                    // 0x04

    ExecTask();
    ~ExecTask();
    bool execute();
    virtual void setup();
    virtual void exec();
    void terminate();
    virtual void cleanup();
    virtual bool isEnd();
};

struct ExecTaskManager {
    // vtable                           // 0x00
    dss::Flag flag_;                    // 0x04
    ExecTask* pExecTask_[16];           // 0x08
    int currentId_;                     // 0x48

    ExecTaskManager() { flag_.clear(); }
    ~ExecTaskManager() {}
    virtual void initialize();
    void terminate();
    bool execute();
    void unkfunc_02035bb8();
    void resister(int id, ExecTask* task);
};

