#include "main/object/PaletteAnimationObject.hpp"
#include "main/dss/UnkVramTransfer.hpp"

ARM void PaletteAnimationObject::unkfunc_0205b3d0()
{
    wait_ = 0;
    frame_ = 0;
    dss::memcpy(color_, anim_.getColor(frame_), anim_.getColorCount() * 2);
    data_0211e450.unkfunc_02086378(1, color_, texture_->unkfunc_02086aa4(), texture_->unkfunc_02086ab4(), 0);
}

ARM void PaletteAnimationObject::unkfunc_0205b44c()
{
    wait_++;
    if (wait_ < anim_.getWait(frame_)) {
        return;
    }
    wait_ = 0;
    frame_++;
    if (frame_ >= anim_.getFrameCount()) {
        frame_ = 0;
    }
    dss::memcpy(color_, anim_.getColor(frame_), anim_.getColorCount() * 2);
    data_0211e450.unkfunc_02086378(1, color_, texture_->unkfunc_02086aa4(), texture_->unkfunc_02086ab4(), 0);
}

ARM void PaletteAnimationObject::unkfunc_0205b514(int address, int size)
{
    wait_ = 0;
    frame_ = 0;
    dss::memcpy(color_, anim_.getColor(frame_), anim_.getColorCount() * 2);
    data_0211e450.unkfunc_02086378(1, color_, address, size, 0);
}

ARM void PaletteAnimationObject::unkfunc_0205b584(int address, int size)
{
    wait_++;
    if (wait_ < anim_.getWait(frame_)) {
        return;
    }
    dss::memcpy(color_, anim_.getColor(frame_), anim_.getColorCount() * 2);
    data_0211e450.unkfunc_02086378(1, color_, address, size, 0);
    wait_ = 0;
    frame_++;
}

ARM int PaletteAnimationObject::unkfunc_0205b628()
{
    return frame_ >= anim_.getFrameCount();
}

ARM void PaletteAnimationObject::unkfunc_0205b648()
{
    colorCount_ = 0;
    texture_ = 0;
}
