#pragma ipa file
#include "main/dss/Render.hpp"
#include "main/fld/FldStage.hpp"
#include "main/object/ModelObject.hpp"
#include "main/data/DataObject.hpp"

namespace dss {
    template <typename T>
    T max(T a, T b)
    {
        return a > b ? a : b;
    }

    template <typename T>
    T min(T a, T b)
    {
        return a < b ? a : b;
    }

    template <typename T>
    T clamp(T a, T b, T c)
    {
        return min(max(a, b), c);
    }
}

// Unreferenced global objects
struct UnkGlobalStage {
    Render render_;                             // 0x000
    FldStage stage_;                            // 0x608
};

UnkGlobalStage data_020d1cd4;
ModelObject data_020d10a0;
DataObject data_020d1080;
int data_020d1044[3];
DataObject data_020d1090;

// Never called (dead-stripped): instantiates clamp<Fix32> and keeps data_020d1044 in the pooled .bss
ARM dss::Fix32 unkfunc_unused_28(dss::Fix32 value)
{
    const int lo = 0;
    const int hi = 1;
    data_020d1044[0] = 0;
    return dss::clamp(value, dss::Fix32(lo), dss::Fix32(hi));
}
