#include "main/task/ExecTask.hpp"
#include "main/btl/BattleAutoFeed.hpp"
#include "main/menu/MenuAPI.hpp"
#include "main/global/Global.hpp"
#include "main/global/GlobalDQ4.hpp"

THUMB ExecTask::ExecTask()
{
    flag_.clear();
}

THUMB ExecTask::~ExecTask()
{
    flag_.clear();
}

THUMB bool ExecTask::execute()
{
    if (!flag_.check(1)) {
        flag_.flag_ |= 1;
        setup();
    } else {
        exec();
        if (flag_.check(2)) {
            cleanup();
            flag_.clear();
            return false;
        }
    }
    return true;
}

THUMB void ExecTask::setup()
{
    MenuAPI::openBattleMessage();
    MenuAPI::addMessage(0);
}

THUMB void ExecTask::exec()
{
    if (isEnd()) {
        flag_.flag_ |= 2;
    }
}

THUMB void ExecTask::terminate()
{
    flag_.flag_ |= 2;
}

THUMB void ExecTask::cleanup()
{
    flag_.clear();
}

THUMB bool ExecTask::isEnd()
{
    if (data_0210bb94.unkfunc_0205810c() == 13) {
        if (BattleAutoFeed::isEndMessageSend()) {
            return true;
        }
    } else if (MenuAPI::isFinishMessageWindow()) {
        return true;
    }
    return false;
}

THUMB void ExecTaskManager::initialize()
{
    unkfunc_02035bb8();
}

THUMB void ExecTaskManager::terminate()
{
    flag_.clear();
}

THUMB bool ExecTaskManager::execute()
{
    if (!flag_.check(1)) {
        flag_.flag_ |= 1;
        initialize();
    } else if (!flag_.check(2)) {
        if (pExecTask_[currentId_] != 0 && !pExecTask_[currentId_]->execute()) {
            currentId_++;
            if (pExecTask_[currentId_] == 0) {
                flag_.flag_ |= 2;
            }
        }
    } else {
        terminate();
    }
    if (flag_.check(2)) {
        return false;
    }
    return true;
}

THUMB void ExecTaskManager::unkfunc_02035bb8()
{
    currentId_ = 0;
    for (int i = 0; i < 16; i++) {
        pExecTask_[i] = 0;
    }
}

THUMB void ExecTaskManager::resister(int index, ExecTask* execTask)
{
    pExecTask_[index] = execTask;
}
