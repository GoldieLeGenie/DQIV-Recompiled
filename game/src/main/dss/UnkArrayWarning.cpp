#include "main/dss/UnkArrayWarning.hpp"
#include "main/dss/DssUtils.hpp"
#ifdef DQIV_NATIVE
#include <stdexcept>
#include <string>
#endif

int data_020c499c = 1;
int data_02120fb4;
UnkArrayWarning data_02120fb8[16];

ARM void unkfunc_02089580(int enable)
{
    data_020c499c = enable;
}

ARM void unkfunc_02089590(int index, int size, unsigned int caller)
{
    if (data_02120fb4 < 16) {
        data_02120fb8[data_02120fb4].index_ = index;
        data_02120fb8[data_02120fb4].size_ = size;
        data_02120fb8[data_02120fb4].caller_ = caller;
    }
    data_02120fb4++;
}

ARM UnkArrayWarning* unkfunc_020895e8(int index)
{
    return &data_02120fb8[index];
}

ARM int unkfunc_020895fc()
{
    return data_02120fb4;
}

ARM void unkfunc_0208960c(int index, int size)
{
#ifdef DQIV_NATIVE
    throw std::out_of_range("Game array index " + std::to_string(index) + " / " + std::to_string(size));
#else
    register unsigned int caller = 0;
    asm { mov caller, lr }
    if (data_020c499c != 0) {
        unkfunc_02089590(index, size, caller);
        char buf[0x40];
        dss::sprintf_s(buf, sizeof(buf), "ARRAY ERROR %d/%d %08x !!!!", index, size, caller);
        if (data_020c499c == 1) {
            (void)buf;                          // printed in the debug build
        }
    }
#endif
}
