#pragma once

struct CursorMoveBase {
    virtual int inputLeft(int index) = 0;
    virtual int inputRight(int index) = 0;
    virtual int inputUp(int index) = 0;
    virtual int inputDown(int index) = 0;

    short w_;           /* 0x4 */
    short h_;           /* 0x6 */
    short count_;       /* 0x8 */
    short pageNo_;      /* 0xA */

    void setupBase();
    void setXY(int w, int h);
    void setMaxCount(int count);
    int getPageNo();
    void setPageNo(int pageNo);
    int getPageMaxCount();
    int pageNext(int index);
    int pageBack(int index);
    int getIndex(int index);
    int getMaxCountInPage();
    int getCountInPage();
};

struct CursorMoveGridLoop : CursorMoveBase {
    virtual int inputLeft(int index);
    virtual int inputRight(int index);
    virtual int inputUp(int index);
    virtual int inputDown(int index);

    void setup(int w, int h, int count);
};
