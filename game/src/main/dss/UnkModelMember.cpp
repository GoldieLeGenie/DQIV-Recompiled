#include "main/dss/UnkModelMember.hpp"
#include "main/dss/UnkVramTransfer.hpp"
#include "main/object/ModelObject.hpp"
#include "nitro/os.hpp"
#include "main/dss/UnkPaletteEffect.hpp"

NNSG3dResTex* data_0211d364;

ARM UnkModelMember::UnkModelMember()
{
    unk_00 = NULL;
    unk_04 = NULL;
}

ARM void UnkModelMember::unkfunc_02083354(void* data)
{
    unkfunc_02083384(data);
}

ARM void UnkModelMember::unkfunc_02083360()
{
    unkfunc_02083454();
}

ARM void UnkModelMember::unkfunc_0208336c()
{
    unkfunc_02083530();
    unkfunc_02083460();
}

ARM void UnkModelMember::unkfunc_02083384(void* data)
{
    unk_00 = data;
    unkfunc_02083460();
    unk_04 = NNS_G3dGetMdlByIdx(func_0206e8c0(unk_00), 0);
    NNSG3dResMdlSet* mdlSet = func_0206e8c0(unk_00);
    for (int i = 0; i < mdlSet->dict.numEntry; i++) {
        unk_04 = NNS_G3dGetMdlByIdx(func_0206e8c0(unk_00), i);
        func_0206aea8(0, 0, 2, 0, 0x1f, 0x8000);
        func_0206e484(unk_04, 0, 0x8000);
    }
}

ARM void UnkModelMember::unkfunc_02083454()
{
    unkfunc_02083530();
}

ARM void UnkModelMember::unkfunc_02083460()
{
    NNSG3dResMdlSet* mdlSet = func_0206e8c0(unk_00);
    NNSG3dResTex* tex = func_0206e8d0(unk_00);
    if (tex == NULL) {
        return;
    }
    data_0211d364 = tex;
    unk_08 = tex;
    int texSize = func_0206a600(tex);
    func_0206a60c(tex);
    int plttSize = func_0206a71c(tex);
    int texKey = data_0211e450.unkfunc_0208627c(texSize);
    int plttKey = data_0211e450.unkfunc_020862e0(plttSize);
    unkfunc_02085798(tex);
    if (!(tex->texInfo.flag & 1)) {
        func_0206a618(tex, texKey, 0);
        func_0206a728(tex, plttKey);
        OS_Wait();
        func_0206a62c(tex, TRUE);
        func_0206a730(tex, TRUE);
    }
    func_0206abb4(mdlSet, tex);
}

ARM void UnkModelMember::unkfunc_02083530()
{
    NNSG3dResMdlSet* mdlSet = func_0206e8c0(unk_00);
    NNSG3dResTex* tex = func_0206e8d0(unk_00);
    if (tex == NULL) {
        return;
    }
    func_0206ac24(mdlSet);
    int plttKey = func_0206a780(tex);
    int texKey;
    int tex4x4Key;
    func_0206a6e4(tex, &texKey, &tex4x4Key);
    if (plttKey != 0) {
        data_0211e450.unkfunc_02086318(plttKey);
    }
    if (texKey != 0) {
        data_0211e450.unkfunc_020862a0(texKey);
    }
    data_0211d364 = NULL;
    unk_08 = NULL;
}

ARM void* UnkModelMember::unkfunc_020835c8()
{
    return unk_00;
}

ARM NNSG3dResMdl* UnkModelMember::unkfunc_020835d0()
{
    return unk_04;
}

ARM NNSG3dResTex* unkfunc_020835d8()
{
    return data_0211d364;
}
