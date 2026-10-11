#include "main/menu/CursorMoveGridLoop.hpp"
#include "globaldefs.h"

THUMB void CursorMoveBase::setupBase()
{
    w_ = 0;
    h_ = 0;
    count_ = 0;
    pageNo_ = 0;
}

THUMB void CursorMoveBase::setXY(int w, int h)
{
    w_ = w;
    h_ = h;
}

THUMB void CursorMoveBase::setMaxCount(int count)
{
    count_ = count;
}

THUMB int CursorMoveBase::getPageNo()
{
    return pageNo_;
}

THUMB void CursorMoveBase::setPageNo(int pageNo)
{
    pageNo_ = pageNo;
}

THUMB int CursorMoveBase::getPageMaxCount()
{
    return (count_ - 1) / (w_ * h_) + 1;
}

THUMB int CursorMoveBase::pageNext(int index)
{
    int max = getPageMaxCount();
    pageNo_++;
    pageNo_ = (pageNo_ < max) ? pageNo_ : 0;
    int count = getCountInPage();
    if (index >= count) {
        index = (count - 1) / w_;
        index *= w_;
    }
    return index;
}

THUMB int CursorMoveBase::pageBack(int index)
{
    int max = getPageMaxCount();
    pageNo_--;
    pageNo_ = (pageNo_ < 0) ? (short)(max - 1) : pageNo_;
    int count = getCountInPage();
    if (index >= count) {
        index = count - 1;
    }
    return index;
}

THUMB int CursorMoveBase::getIndex(int index)
{
    return index + pageNo_ * w_ * h_;
}

THUMB int CursorMoveBase::getMaxCountInPage()
{
    return w_ * h_;
}

THUMB int CursorMoveBase::getCountInPage()
{
    int last = getPageMaxCount() - 1;
    if (pageNo_ != last) {
        return getMaxCountInPage();
    }
    last = getPageMaxCount() - 1;
    int n = getMaxCountInPage();
    return count_ - n * last;
}

THUMB int CursorMoveGridLoop::inputRight(int index)
{
    if (getCountInPage() == 0) {
        return 0;
    }
    pageNext(index);
    int result = index / w_;
    result *= w_;
    int count = getCountInPage();
    if (result >= count) {
        result = (count - 1) / w_;
        result *= w_;
    }
    return result;
}

THUMB int CursorMoveGridLoop::inputLeft(int index)
{
    if (getCountInPage() == 0) {
        return 0;
    }
    pageBack(index);
    int result = (index / w_ + 1) * w_ - 1;
    int count = getCountInPage();
    if (result >= count) {
        result = count - 1;
    }
    return result;
}

THUMB int CursorMoveGridLoop::inputDown(int index)
{
    if (getCountInPage() == 0) {
        return 0;
    }
    return index % w_;
}

THUMB int CursorMoveGridLoop::inputUp(int index)
{
    if (getCountInPage() == 0) {
        return 0;
    }
    int count = getCountInPage();
    int page = count / w_;
    int result = index % w_ + page * w_;
    if (result >= count) {
        result -= w_;
    }
    return result;
}

THUMB void CursorMoveGridLoop::setup(int w, int h, int count)
{
    setXY(w, h);
    setMaxCount(count);
}
