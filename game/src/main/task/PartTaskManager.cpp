#pragma ipa file
#include "main/task/PartTaskManager.hpp"
#include "main/btl/BattleAutoFeed.hpp"
#include "main/dss/UnkVramTransfer.hpp"

task::PartTaskManager g_PartTaskManager;
task::Sample00Task g_Sample00Task;
task::Sample01Task g_Sample01Task;
task::Sample02Task g_Sample02Task;

THUMB task::PartTaskManager::PartTaskManager() {
    currentTask_ = 0;
    currentTaskID_ = 0;
    previousTaskID_ = -1;
    nextTaskID_ = -1;
    sleepTaskID_ = -1;
}

THUMB task::PartTaskManager::~PartTaskManager() {
}

THUMB void task::PartTaskManager::run() {
    if (currentTask_) {
        currentTask_->execute();
    }

    if (nextTaskID_ != -1) {
        if (currentTask_) {
            currentTask_->terminate();
        }

        previousTaskID_ = currentTaskID_;
        currentTaskID_ = nextTaskID_;
        nextTaskID_ = -1;

        currentTask_ = parts_[currentTaskID_];
        currentTask_->initialize();

        data_0211e450.unk_814 = 0;
    }

    BattleAutoFeed::printCounter();
}


THUMB void task::PartTaskManager::registerTask(int id, PartTask *task)
{
  this->parts_[id] = task;
}

THUMB void task::PartTaskManager::setNextTask(int id)
{
  this->nextTaskID_ = id;
  return;
}

THUMB int task::PartTaskManager::getCurrentTask()
{
  return this->currentTaskID_;
}


THUMB bool task::PartTaskManager::checkTask(int id) {
    return this->currentTaskID_ == id;
}


THUMB void task::PartTaskManager::setNextTaskWithSleep(int id) {
    sleepTaskID_ = currentTaskID_;
    setNextTask(id);
}


THUMB void task::PartTaskManager::wakeup() {
    currentTaskID_ = sleepTaskID_;
    sleepTaskID_ = -1;
    currentTask_ = parts_[currentTaskID_];
}


THUMB void task::PartTaskManager::initialize() {
    this->currentTask_ = 0;
    this->currentTaskID_ = 0;
    this->previousTaskID_ = -1;
    this->nextTaskID_ = -1;
    this->sleepTaskID_ = -1;
}

THUMB void task::PartTask::initialize() {
}

THUMB void task::PartTask::terminate() {
}

THUMB void task::PartTask::execute() {
}

THUMB void task::Sample00Task::initialize() {
}

THUMB void task::Sample00Task::terminate() {
}

THUMB void task::Sample00Task::execute() {
}

THUMB void task::Sample01Task::initialize() {
}

THUMB void task::Sample01Task::terminate() {
}

THUMB void task::Sample01Task::execute() {
}

THUMB void task::Sample02Task::initialize() {
}

THUMB void task::Sample02Task::terminate() {
}

THUMB void task::Sample02Task::execute() {
}
