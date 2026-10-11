#pragma once
#include "globaldefs.h"

namespace cmn {
    struct ResourceStorage {
        static int maxStorage_;

        int refCounter_[256];
        int index_[256];
        int counter_;

        ResourceStorage();
        ~ResourceStorage();
        virtual void initialize();
        virtual void terminate();
        virtual int loadResource(int id) = 0;
        virtual void releaseResource(int id) = 0;
        int getResource(int id);
        int getRefCounter(int id);
        void restoreResource(int id);
        int getEmptyArea();
        int getResourceArea(int id);
    };
}
