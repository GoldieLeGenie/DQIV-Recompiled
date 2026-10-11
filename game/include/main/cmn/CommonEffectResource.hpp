#pragma once
#include <globaldefs.h>
#include "main/cmn/ResourceStorage.hpp"
#include "main/cmn/CommonEffectData.hpp"

namespace cmn {
    struct CommonEffectResource : ResourceStorage {
        CommonEffectData storage_[5];       /* 0x808 */

        CommonEffectResource();
        ~CommonEffectResource();
        virtual void initialize();
        virtual void terminate();
        CommonEffectData* getResource(int id);
        virtual int loadResource(int id);
        virtual void releaseResource(int id);
        int getResourceStock();
    };
}
