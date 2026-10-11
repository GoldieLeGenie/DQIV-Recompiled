#include "main/menu/UnkMenuDisplay.hpp"

THUMB UnkMenuDisplay::UnkMenuDisplay()
{
    unkfunc_0204f1ac();
}

THUMB void UnkMenuDisplay::unkfunc_0204f1ac()
{
    if (enable_ != 0) {
        enable_ = 0;
    }
    key_ = -1;
}

THUMB void UnkMenuDisplay::unkfunc_0204f1c0()
{
    if (moveCount_ == 0) {
        return;
    }
    if (--moveCount_ == 0) {
        x_ = targetX_;
        y_ = targetY_;
        return;
    }
    x_ += ((targetX_ - x_) * 4096 / moveCount_) / 4096;
    y_ += ((targetY_ - y_) * 4096 / moveCount_) / 4096;
}

THUMB int UnkMenuDisplay::unkfunc_0204f214(UnkOamBuffer* main, UnkOamBuffer* sub)
{
    if (enable_ == 0) {
        return 0;
    }
    switch (type_) {
    case 0:
        return 0;
    case 1:
        if (main == NULL) {
            return 0;
        }
        break;
    case 2:
        if (sub == NULL) {
            return 0;
        }
        break;
    case 3:
        if (main == NULL) {
            return 0;
        }
        if (sub == NULL) {
            return 0;
        }
        break;
    }
    return 1;
}

THUMB void UnkMenuDisplay::unkfunc_0204f260(int type)
{
    type_ = type;
}

THUMB void UnkMenuDisplay::unkfunc_0204f264(int enable)
{
    if (enable_ != enable) {
        enable_ = enable;
    }
}

THUMB void UnkMenuDisplay::unkfunc_0204f270(int x, int y)
{
    x_ = x;
    y_ = y;
}

THUMB void UnkMenuDisplay::unkfunc_0204f278(int w, int h)
{
    w_ = w;
    h_ = h;
}

THUMB void UnkMenuDisplay::unkfunc_0204f280(int key)
{
    key_ = key;
}

THUMB int UnkMenuDisplay::unkfunc_0204f284(int key)
{
    if (key_ == key) {
        return 1;
    }
    return 0;
}
