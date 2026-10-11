#pragma once
#include "globaldefs.h"

namespace cmn {
    struct PlayerAction {
        virtual void unkfunc_02030f80();
        virtual void unkfunc_02030f84();
        virtual void unkfunc_0213c9b4() = 0;
        virtual void unkfunc_0213c9b0() = 0;

        int padInput_;                          // 0x04
        int dirInput_;                          // 0x08

        PlayerAction();
        ~PlayerAction();
        void inputPad(int padDir);
        void inputClear();
    };
}
