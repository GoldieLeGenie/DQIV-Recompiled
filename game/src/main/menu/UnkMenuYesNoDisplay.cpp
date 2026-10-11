#include "main/menu/UnkMenuYesNoDisplay.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/menu/UnkMenuDisplays.hpp"
#include "main/text/TextAPI.hpp"
#include "nitro/g2.hpp"

THUMB UnkMenuYesNoDisplay::UnkMenuYesNoDisplay()
{
}

THUMB void UnkMenuYesNoDisplay::setup(int id)
{
    id_ = id;
    unkfunc_02080110(&yesChar_, yesData_, 6, 2);
    unkfunc_02080110(&noChar_, noData_, 6, 2);
    unkfunc_0204f1ac();
    unkfunc_0204f260(2);
    transfer_ = 0;
    cursor_ = 0;
    unkfunc_0204f270(192, 208);
    unkfunc_0204f278(64, 56);
}

THUMB void UnkMenuYesNoDisplay::update(UnkOamBuffer* main, UnkOamBuffer* sub)
{
}

THUMB void UnkMenuYesNoDisplay::execute(UnkOamBuffer* main, UnkOamBuffer* sub)
{
}

inline void UnkMenuYesNoDisplay::transferRow(int row)
{
    int chr = id_ + row * 32;
    int tile = row * 6;
    unkfunc_020827f0(0x13, chr * 32, yesData_ + tile * 32, 0xc0);
    unkfunc_020827f0(0x13, (chr + 6) * 32, noData_ + tile * 32, 0xc0);
}

THUMB void UnkMenuYesNoDisplay::draw(UnkOamBuffer* main, UnkOamBuffer* sub)
{
    if (!enable_) {
        unkfunc_0208175c(0);
    }
    if (!unkfunc_0204f214(main, sub)) {
        return;
    }
    int y = y_ % 192;
    UnkMenuWindowFrame* frame = &data_020f530c.windowFrame2_;
    frame->unkfunc_0204f800(0);
    frame->unkfunc_0204f270(x_, y_);
    frame->unkfunc_0204f278(w_, h_);
    frame->unkfunc_0204f820(x_ + 10, y + 16 + cursor_ * 24, 1);
    frame->unkfunc_0204f828(1);
    frame->unkfunc_0204f264(1);
    G2_SetOBJAttr(sub->unkfunc_02082694(), x_ + 14, y + 10, 0, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE,
                  GX_OAM_SHAPE_32x16, GX_OAM_COLORMODE_16, id_, 15, 0);
    G2_SetOBJAttr(sub->unkfunc_02082694(), x_ + 46, y + 10, 0, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE,
                  GX_OAM_SHAPE_8x16, GX_OAM_COLORMODE_16, id_ + 4, 15, 0);
    G2_SetOBJAttr(sub->unkfunc_02082694(), x_ + 14, y + 34, 0, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE,
                  GX_OAM_SHAPE_32x16, GX_OAM_COLORMODE_16, id_ + 6, 15, 0);
    G2_SetOBJAttr(sub->unkfunc_02082694(), x_ + 46, y + 34, 0, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE,
                  GX_OAM_SHAPE_8x16, GX_OAM_COLORMODE_16, id_ + 10, 15, 0);
    if (transfer_ == 1) {
        for (int i = 0; i < 2; i++) {
            transferRow(i);
        }
    }
    int type;
    switch (unkfunc_020817bc()) {
    case 1:
        type = 0x14;
        break;
    case 2:
        type = 0x11;
        break;
    case 3:
        type = 0x11;
        break;
    default:
        type = 0;
        break;
    }
    if (type) {
        unkfunc_0208175c(1);
        unkfunc_02081794(type, x_ + 2, y_ - 0xbe, w_ - 4, h_ - 4);
    }
}

THUMB void UnkMenuYesNoDisplay::unkfunc_02052a24(UnkOamBuffer* main, UnkOamBuffer* sub)
{
}

THUMB void UnkMenuYesNoDisplay::unkfunc_02052a28(int yesMessageId, int noMessageId)
{
    char yes[0x200];
    char no[0x200];
    unkfunc_02080038(10);
    unkfunc_02080130(&yesChar_, 0);
    unkfunc_02080130(&noChar_, 0);
    g_text_extractor.extractText(yes, sizeof(yes), yesMessageId);
    g_text_extractor.extractText(no, sizeof(no), noMessageId);
    unkfunc_0207f994(&yesChar_, 0, 0, 1, yes);
    unkfunc_0207f994(&noChar_, 0, 0, 1, no);
    transfer_ = 1;
}

THUMB void UnkMenuYesNoDisplay::unkfunc_02052aa4(int cursor)
{
    cursor_ = cursor;
}
