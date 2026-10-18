#include "main/menu/MenuDisplay.hpp"

THUMB MenuDisplay::MenuDisplay()
{
    reset();
}

THUMB void MenuDisplay::reset()
{
    if (enable_ != 0) {
        enable_ = 0;
    }
    key_ = -1;
}

THUMB void MenuDisplay::updateMove()
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

THUMB int MenuDisplay::isDrawable(OamBuffer* main, OamBuffer* sub)
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

THUMB void MenuDisplay::setType(int type)
{
    type_ = type;
}

THUMB void MenuDisplay::setEnable(int enable)
{
    if (enable_ != enable) {
        enable_ = enable;
    }
}

THUMB void MenuDisplay::setPosition(int x, int y)
{
    x_ = x;
    y_ = y;
}

THUMB void MenuDisplay::setSize(int w, int h)
{
    w_ = w;
    h_ = h;
}

THUMB void MenuDisplay::setKey(int key)
{
    key_ = key;
}

THUMB int MenuDisplay::isKey(int key)
{
    if (key_ == key) {
        return 1;
    }
    return 0;
}
