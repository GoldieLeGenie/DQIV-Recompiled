#pragma ipa file
#include "ov016/TownMenuMapCaption/TownMenuMapCaption.hpp"
#include "main/menu/MenuPartsDraw.hpp"

static MenuPartsEntry s_parts[] = {
    { 0x01, 0x00, (short)0xf000, 0, 0x40, 0x48, 0x78, 0x28 },
    { 0x0d, 0x09, (short)0xf100, 0, 0x53, 0x50, 0x58, 0x10 },
    { 0x0d, 0x09, (short)0xf100, 1, 0x48, 0x5a, 0x68, 0x0e },
    { 0xff, 0, 0, 0, 0, 0, 0, 0 },
};

THUMB void TownMenuMapCaption::menuSetup()
{
    captionType_ = 0;
}

THUMB void TownMenuMapCaption::menuExecute()
{
}

THUMB void TownMenuMapCaption::menuDraw()
{
    int param[2];
    if (captionType_ != 0) {
        param[0] = (int)L"\xfeff\x24ac";
        if (captionType_ == 1) {
            param[1] = 0x80000087;
        }
        if (captionType_ == 2) {
            param[1] = 0x80000088;
        }
        drawMenuPartsPlain(s_parts, param);
    }
}

THUMB void TownMenuMapCaption::menuUpdate()
{
}

THUMB void TownMenuMapCaption::unkfunc_02178aa8(int flag)
{
    if (flag == 0) {
        close();
    } else {
        open();
        redraw_ = 1;
    }
    captionType_ = flag;
}
