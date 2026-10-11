#include "main/dss/UnkFont.hpp"

ARM UnkFont::UnkFont()
{
    unk_08 = 0;
    height_ = 0;
}

ARM void UnkFont::unkfunc_02088554(void* data, int height)
{
    func_02069030(&font_, data);
    unsigned short index = func_02069060(&font_, '%');
    if (index != 0xffff) {
        font_.info_->alterCharIndex_ = index;
    }
    height_ = height;
}

ARM int UnkFont::unkfunc_0208858c(UnkG2dCanvas* canvas, int x, int y, int color, unsigned short c)
{
    if (canvas == NULL) {
        unsigned short index = func_02069060(&font_, c);
        if (index == 0xffff) {
            index = font_.info_->alterCharIndex_;
        }
        return func_020690a8(&font_, index)->charWidth_;
    }
    return func_02069d94(canvas, &font_, x, y, color, c);
}
