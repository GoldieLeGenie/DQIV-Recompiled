#pragma ipa file
#include "ov001/window/UnkFieldMap.hpp"
#include "main/dss/DssUtils.hpp"

ARM UnkFieldMap_0212d558::UnkFieldMap_0212d558()
{
}

ARM UnkFieldMap_0212d558::~UnkFieldMap_0212d558()
{
}

ARM void UnkFieldMap_0212d558::setup()
{
    char path[0x80];
    dss::sprintf_s(path, sizeof(path), "data/field/2d/point_s.tex");
    unk_100.unkfunc_02057d60(path, 0);
    unk_100.unkfunc_02057e58(render_);
    unk_100.unkfunc_02057ee8();
    unk_100.unkfunc_02057f18(0x34);
    unk_100.unkfunc_02057ea8(8, 7, 0x18, 0x18);
    unk_100.unkfunc_02057f30(0xa);
    unk_100.unkfunc_02057f00(0);
    unk_100.unkfunc_02057ed4(1);
    dss::sprintf_s(path, sizeof(path), "data/field/2d/gw.tex");
    unk_08.unkfunc_02057d60(path, 0);
    unk_08.unkfunc_02057ee8();
    unk_08.unkfunc_02057f18(0x3e);
    unk_08.unkfunc_02057f30(0);
    unk_08.unkfunc_02057f00(0);
    unk_08.unkfunc_02057d1c();
    unk_100.unkfunc_02057d1c();
    unk_58.unkfunc_02057ee8();
    unk_58.unkfunc_02057f18(0x3e);
    unk_58.unkfunc_02057f30(4);
    unk_58.unkfunc_02057f00(0);
    unk_58.unkfunc_02057e98(3, 3);
    unk_150 = 1;
}
