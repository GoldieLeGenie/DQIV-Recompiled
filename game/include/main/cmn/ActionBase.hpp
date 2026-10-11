#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"

namespace cmn {
    struct ActionBase {
        static dss::Fix32Vector3& position_;
        static short& dirIdx_;

        virtual int setup() = 0;
        virtual void execute() = 0;
        virtual int update() = 0;
    };
}
