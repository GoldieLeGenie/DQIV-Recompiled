#pragma ipa file
#include "ov016/UnkTownMenu_02178a58/UnkTownMenu_02178a58.hpp"
#include "main/menu/UnkMenuPartsDraw.hpp"

static UnkMenuParts s_parts[] = {
    { 0x01, 0x00, (short)0xf000, 0, 0x40, 0x48, 0x78, 0x28 },
    { 0x0d, 0x09, (short)0xf100, 0, 0x53, 0x50, 0x58, 0x10 },
    { 0x0d, 0x09, (short)0xf100, 1, 0x48, 0x5a, 0x68, 0x0e },
    { 0xff, 0, 0, 0, 0, 0, 0, 0 },
};

THUMB void UnkTownMenu_02178a58::menuSetup()
{
    unk_1c = 0;
}

THUMB void UnkTownMenu_02178a58::menuExecute()
{
}

THUMB void UnkTownMenu_02178a58::menuDraw()
{
    int param[2];
    if (unk_1c != 0) {
        param[0] = (int)L"\xfeff\x24ac";
        if (unk_1c == 1) {
            param[1] = 0x80000087;
        }
        if (unk_1c == 2) {
            param[1] = 0x80000088;
        }
        unkfunc_02050ea8(s_parts, param);
    }
}

THUMB void UnkTownMenu_02178a58::menuUpdate()
{
}

THUMB void UnkTownMenu_02178a58::unkfunc_02178aa8(int flag)
{
    if (flag == 0) {
        close();
    } else {
        open();
        redraw_ = 1;
    }
    unk_1c = flag;
}
