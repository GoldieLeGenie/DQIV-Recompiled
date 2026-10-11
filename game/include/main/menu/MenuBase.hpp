#pragma once
#include "main/menu/CursorMoveGridLoop.hpp"
#include "main/menu/MenuUpdateAssist.hpp"

struct MENUITEM_DATA;

namespace menu {

struct MenuBase {
    enum MENUBASE_STAT {
        MENUBASE_STAT_ACTIVE=0,
        MENUBASE_STAT_OK=1,
        MENUBASE_STAT_CANCEL=2
    };
    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    virtual void menuClose() {}
    int redraw_;
    int frame_;
    MENUBASE_STAT stat_;
    int exitCode_;
    int lock_;
    int lockRequest_;

    MenuBase();
    void menuBaseSetup();
    void menuBaseExecute();
    void menuBaseDraw();
    void menuBaseUpdate();
    void open();
    void close();
    int isOpen();
};

struct MenuItem {
    enum MENUITEM_TYPE {
        MENUITEM_TYPE_TOUCH,
        MENUITEM_TYPE_TOUCH_PAD,
        MENUITEM_TYPE_TOUCH_CANCEL,
        MENUITEM_TYPE_TOUCH_PAD_CANCEL,
    };
    enum CURSORTYPE {
        CURSORTYPE_NONE,
        CURSORTYPE_UP,
        CURSORTYPE_DOWN,
        CURSORTYPE_LEFT,
        CURSORTYPE_RIGHT,
        CURSORTYPE_ACTIVE,
        CURSORTYPE_INACTIVE,
        CURSORTYPE_WIRELESS,
    };
    enum MENUITEM_RESULT {
        MENUITEM_RESULT_NONE,
        MENUITEM_RESULT_CHANGE,
        MENUITEM_RESULT_OK,
        MENUITEM_RESULT_CANCEL,
        MENUITEM_RESULT_SUPERCANCEL,
        MENUITEM_RESULT_UP,
        MENUITEM_RESULT_DOWN,
        MENUITEM_RESULT_LEFT,
        MENUITEM_RESULT_RIGHT,
        MENUITEM_RESULT_DA,
        MENUITEM_RESULT_DB,
        MENUITEM_RESULT_DX,
        MENUITEM_RESULT_DY,
    };
    enum MENUITEM_REASON {
        MENUITEM_REASON_NONE,
        MENUITEM_REASON_PAD,
        MENUITEM_REASON_TOUCH,
    };

    int menuitem_temp_x_;           /* 0x00 */
    int menuitem_temp_y_;           /* 0x04 */
    int menuitem_width_;            /* 0x08 */
    int menuitem_height_;           /* 0x0C */
    int menuitem_min_;              /* 0x10 */
    int menuitem_max_;              /* 0x14 */
    int flagTouch_;                 /* 0x18 */
    int enablePad_;                 /* 0x1C */
    int enableCancel_;              /* 0x20 */
    int enableDirectButton_;        /* 0x24 */
    int enableSE_;                  /* 0x28 */
    int enableLoopEdge_;            /* 0x2C */
    MENUITEM_DATA* menuitem_data_;  /* 0x30 */
    int unk_34;                     /* 0x34 */
    int active_;                    /* 0x38 */
    int unk_3C;                     /* 0x3C */
    int unk_40;                     /* 0x40 */
    int unk_44;                     /* 0x44 base x */
    int unk_48;                     /* 0x48 base y */
    int lastresult_;                /* 0x4C */
    int result_;                    /* 0x50 */
    int reason_;                    /* 0x54 */
    int mtype_;                     /* 0x58 */
    int ctype_;                     /* 0x5C */
    int navMode_;                   /* 0x60 */

    void setup(MENUITEM_TYPE mtype, CURSORTYPE ctype);
    void drawActive();
    void setMenuItem(MENUITEM_DATA* data, int width, int height, int count);
    void setBaseXY(int x, int y);
    int execInput();
    void unkfunc_02051b60();
    int unkfunc_02051be0();
    int unkfunc_02051be4();
    int check11_PAD_DirectButton();
    int check20_PAD_CancelButton();
    int check30_PAD_Noactive();
    int check40_PAD_OkButton();
    int unkfunc_02051d40();
    int unkfunc_02051dcc();
    int unkfunc_02051e5c();
    int unkfunc_02051ed0();
    int check50_NEW_PAD_UP();
    int check60_NEW_PAD_DOWN();
    int check70_NEW_PAD_LEFT();
    int check80_NEW_PAD_RIGHT();
    int getActive() { return active_; }
};

}  // namespace menu

// DS menu parts draw list entry (list terminated by type_ 0xff)
struct UnkMenuParts {
    unsigned char type_;    /* 0x0 */  // 0xff: end of list
    unsigned char subType_; /* 0x1 */
    short attr_;            /* 0x2 */  // palette << 12 | color << 8 | icon slot
    short index_;           /* 0x4 */  // index in the value table
    short x_;               /* 0x6 */
    short y_;               /* 0x8 */
    short w_;               /* 0xA */  // 0: 256
    short h_;               /* 0xC */  // 0: 256

    int unkfunc_02051708(int* param);           // value of the part
    int unkfunc_0205171c();                     // palette
    int unkfunc_02051728();                     // text color
    int unkfunc_02051734();                     // icon slot
};

struct MENUITEM_DATA {
    unsigned char code;     /* 0x0 */
    unsigned char view;     /* 0x1 */
    short x;        /* 0x2 */
    short y;        /* 0x4 */
    short w;        /* 0x6 */
    short h;        /* 0x8 */
};

void unkfunc_020518f8(MENUITEM_DATA* data, int x, int y);     // sets data->x/y

extern "C" {
    extern int data_020be244[];      /* draw flags passed as the last argument of unkfunc_02050ee0 */
}

