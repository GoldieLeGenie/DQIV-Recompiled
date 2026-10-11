#include "main/dss/UnkOverlaySlot.hpp"
#include "nitro/fs.hpp"

ARM UnkOverlaySlot::UnkOverlaySlot()
{
    loaded_ = 0;
    id_ = 0;
}

ARM UnkOverlaySlot::~UnkOverlaySlot()
{
}

ARM void UnkOverlaySlot::unkfunc_0208753c(int id)
{
    unkfunc_02087564();
    FS_LoadOverlay(NULL, id);
    loaded_ = 1;
    id_ = id;
}

ARM void UnkOverlaySlot::unkfunc_02087564()
{
    if (loaded_ == 0) {
        return;
    }
    FS_UnloadOverlay(NULL, id_);
    loaded_ = 0;
}

ARM void unkfunc_02087590(int id)
{
    FS_LoadOverlay(NULL, id);
}

ARM void unkfunc_020875a4(int id)
{
    FS_UnloadOverlay(NULL, id);
}
