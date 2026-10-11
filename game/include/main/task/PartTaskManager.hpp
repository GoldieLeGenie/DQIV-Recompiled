#pragma once
#include <globaldefs.h>
#include "main/dss/DssUtils.hpp"
#include "main/task/PartTask.hpp"

namespace task {

    struct PartTaskManager {
        PartTask *parts_[24];
        PartTask *currentTask_;
        int currentTaskID_;
        int previousTaskID_;
        int nextTaskID_;
        int sleepTaskID_;
        PartTaskManager();
        ~PartTaskManager();
        void run();
        void registerTask(int id, PartTask *task);
        void setNextTask(int id);
        int getCurrentTask();
        bool checkTask(int id);
        void setNextTaskWithSleep(int id);
        void wakeup();
        void initialize();
    };

}  


extern task::PartTaskManager partTaskManager;   // data_ov003_021492dc
extern task::PartTaskManager g_PartTaskManager; // 0x020ef7e4
extern task::Sample00Task g_Sample00Task;
extern task::Sample01Task g_Sample01Task;
extern task::Sample02Task g_Sample02Task;