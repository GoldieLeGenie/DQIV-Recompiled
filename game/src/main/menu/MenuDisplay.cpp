#include "main/menu/MenuDisplay.hpp"

THUMB UnkMenuDisplay::UnkMenuDisplay()
{
    reset();
}

THUMB void UnkMenuDisplay::reset()
{
    if (enable_ != 0) {
        enable_ = 0;
    }
    key_ = -1;
}

THUMB void UnkMenuDisplay::updateMove()
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

THUMB int UnkMenuDisplay::isDrawable(UnkOamBuffer* main, UnkOamBuffer* sub)
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

THUMB void UnkMenuDisplay::setType(int type)
{
    type_ = type;
}

THUMB void UnkMenuDisplay::setEnable(int enable)
{
    if (enable_ != enable) {
        enable_ = enable;
    }
}

THUMB void UnkMenuDisplay::setPosition(int x, int y)
{
    x_ = x;
    y_ = y;
}

THUMB void UnkMenuDisplay::setSize(int w, int h)
{
    w_ = w;
    h_ = h;
}

THUMB void UnkMenuDisplay::setKey(int key)
{
    key_ = key;
}

THUMB int UnkMenuDisplay::isKey(int key)
{
    if (key_ == key) {
        return 1;
    }
    return 0;
}
