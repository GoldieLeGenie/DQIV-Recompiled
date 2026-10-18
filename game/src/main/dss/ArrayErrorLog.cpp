#include "main/dss/ArrayErrorLog.hpp"
#include "main/dss/DssUtils.hpp"
#ifdef DQIV_NATIVE
#include <stdexcept>
#include <string>
#endif

int g_arrayErrorEnabled = 1;
int g_arrayWarningCount;
UnkArrayWarning g_arrayWarnings[16];

ARM void setArrayErrorEnabled(int enable)
{
    g_arrayErrorEnabled = enable;
}

ARM void addArrayError(int index, int size, unsigned int caller)
{
    if (g_arrayWarningCount < 16) {
        g_arrayWarnings[g_arrayWarningCount].index_ = index;
        g_arrayWarnings[g_arrayWarningCount].size_ = size;
        g_arrayWarnings[g_arrayWarningCount].caller_ = caller;
    }
    g_arrayWarningCount++;
}

ARM UnkArrayWarning* getArrayError(int index)
{
    return &g_arrayWarnings[index];
}

ARM int getArrayErrorCount()
{
    return g_arrayWarningCount;
}

ARM void reportArrayError(int index, int size)
{
#ifdef DQIV_NATIVE
    throw std::out_of_range("Game array index " + std::to_string(index) + " / " + std::to_string(size));
#else
    register unsigned int caller = 0;
    asm { mov caller, lr }
    if (g_arrayErrorEnabled != 0) {
        addArrayError(index, size, caller);
        char buf[0x40];
        dss::sprintf_s(buf, sizeof(buf), "ARRAY ERROR %d/%d %08x !!!!", index, size, caller);
        if (g_arrayErrorEnabled == 1) {
            (void)buf;                          // printed in the debug build
        }
    }
#endif
}
