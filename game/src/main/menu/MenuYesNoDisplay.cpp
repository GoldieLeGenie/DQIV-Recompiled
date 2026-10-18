#include "main/menu/MenuYesNoDisplay.hpp"
#include "main/dss/Display.hpp"
#include "main/menu/MenuDisplays.hpp"
#include "main/text/TextAPI.hpp"
#include "nitro/g2.hpp"

THUMB MenuYesNoDisplay::MenuYesNoDisplay()
{
}

THUMB void MenuYesNoDisplay::setup(int id)
{
    id_ = id;
    initCharBuffer(&yesChar_, yesData_, 6, 2);
    initCharBuffer(&noChar_, noData_, 6, 2);
    reset();
    setType(2);
    transfer_ = 0;
    cursor_ = 0;
    setPosition(192, 208);
    setSize(64, 56);
}

THUMB void MenuYesNoDisplay::update(OamBuffer* main, OamBuffer* sub)
{
}

THUMB void MenuYesNoDisplay::execute(OamBuffer* main, OamBuffer* sub)
{
}

inline void MenuYesNoDisplay::transferRow(int row)
{
    int chr = id_ + row * 32;
    int tile = row * 6;
    requestVramCopy(0x13, chr * 32, yesData_ + tile * 32, 0xc0);
    requestVramCopy(0x13, (chr + 6) * 32, noData_ + tile * 32, 0xc0);
}

THUMB void MenuYesNoDisplay::draw(OamBuffer* main, OamBuffer* sub)
{
    if (!enable_) {
        setWindow1Enable(0);
    }
    if (!isDrawable(main, sub)) {
        return;
    }
    int y = y_ % 192;
    MenuWindowFrame* frame = &g_MenuDisplays.windowFrame2_;
    frame->setNameWidth(0);
    frame->setPosition(x_, y_);
    frame->setSize(w_, h_);
    frame->setArrow(x_ + 10, y + 16 + cursor_ * 24, 1);
    frame->setFrameType(1);
    frame->setEnable(1);
    G2_SetOBJAttr(sub->allocEntry(), x_ + 14, y + 10, 0, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE,
                  GX_OAM_SHAPE_32x16, GX_OAM_COLORMODE_16, id_, 15, 0);
    G2_SetOBJAttr(sub->allocEntry(), x_ + 46, y + 10, 0, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE,
                  GX_OAM_SHAPE_8x16, GX_OAM_COLORMODE_16, id_ + 4, 15, 0);
    G2_SetOBJAttr(sub->allocEntry(), x_ + 14, y + 34, 0, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE,
                  GX_OAM_SHAPE_32x16, GX_OAM_COLORMODE_16, id_ + 6, 15, 0);
    G2_SetOBJAttr(sub->allocEntry(), x_ + 46, y + 34, 0, GX_OAM_MODE_NORMAL, 0, GX_OAM_EFFECT_NONE,
                  GX_OAM_SHAPE_8x16, GX_OAM_COLORMODE_16, id_ + 10, 15, 0);
    if (transfer_ == 1) {
        for (int i = 0; i < 2; i++) {
            transferRow(i);
        }
    }
    int type;
    switch (getDisplayPluginType()) {
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
        setWindow1Enable(1);
        setWindow1(type, x_ + 2, y_ - 0xbe, w_ - 4, h_ - 4);
    }
}

THUMB void MenuYesNoDisplay::unkfunc_02052a24(OamBuffer* main, OamBuffer* sub)
{
}

THUMB void MenuYesNoDisplay::setTexts(int yesMessageId, int noMessageId)
{
    char yes[0x200];
    char no[0x200];
    setBgTextFontSize(10);
    fillCharBuffer(&yesChar_, 0);
    fillCharBuffer(&noChar_, 0);
    g_text_extractor.extractText(yes, sizeof(yes), yesMessageId);
    g_text_extractor.extractText(no, sizeof(no), noMessageId);
    drawBgText(&yesChar_, 0, 0, 1, yes);
    drawBgText(&noChar_, 0, 0, 1, no);
    transfer_ = 1;
}

THUMB void MenuYesNoDisplay::setCursor(int cursor)
{
    cursor_ = cursor;
}
