#include "main/dss/OverlaySlot.hpp"
#include "nitro/fs.hpp"

ARM OverlaySlot::OverlaySlot()
{
    loaded_ = 0;
    id_ = 0;
}

ARM OverlaySlot::~OverlaySlot()
{
}

ARM void OverlaySlot::unkfunc_0208753c(int id)
{
    unkfunc_02087564();
    FS_LoadOverlay(NULL, id);
    loaded_ = 1;
    id_ = id;
}

ARM void OverlaySlot::unkfunc_02087564()
{
    if (loaded_ == 0) {
        return;
    }
    FS_UnloadOverlay(NULL, id_);
    loaded_ = 0;
}

ARM void loadOverlay(int id)
{
    FS_LoadOverlay(NULL, id);
}

ARM void unloadOverlay(int id)
{
    FS_UnloadOverlay(NULL, id);
}
