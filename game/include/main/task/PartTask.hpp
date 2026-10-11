#pragma once
#include <globaldefs.h>

namespace task {

    struct PartTask {
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct Sample00Task : PartTask {
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct Sample01Task : PartTask {
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

    struct Sample02Task : PartTask {
        virtual void initialize();
        virtual void terminate();
        virtual void execute();
    };

}  