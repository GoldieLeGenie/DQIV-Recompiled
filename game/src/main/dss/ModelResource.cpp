#include "main/dss/ModelResource.hpp"
#include "main/dss/VramManager.hpp"
#include "main/object/ModelObject.hpp"
#include "nitro/os.hpp"
#include "main/dss/PaletteEffect.hpp"

NNSG3dResTex* g_lastLoadedTex;

ARM ModelResource::ModelResource()
{
    modelFile_ = NULL;
    model_ = NULL;
}

ARM void ModelResource::unkfunc_02083354(void* data)
{
    unkfunc_02083384(data);
}

ARM void ModelResource::unkfunc_02083360()
{
    unkfunc_02083454();
}

ARM void ModelResource::unkfunc_0208336c()
{
    unkfunc_02083530();
    unkfunc_02083460();
}

ARM void ModelResource::unkfunc_02083384(void* data)
{
    modelFile_ = data;
    unkfunc_02083460();
    model_ = NNS_G3dGetMdlByIdx(func_0206e8c0(modelFile_), 0);
    NNSG3dResMdlSet* mdlSet = func_0206e8c0(modelFile_);
    for (int i = 0; i < mdlSet->dict.numEntry; i++) {
        model_ = NNS_G3dGetMdlByIdx(func_0206e8c0(modelFile_), i);
        func_0206aea8(0, 0, 2, 0, 0x1f, 0x8000);
        func_0206e484(model_, 0, 0x8000);
    }
}

ARM void ModelResource::unkfunc_02083454()
{
    unkfunc_02083530();
}

ARM void ModelResource::unkfunc_02083460()
{
    NNSG3dResMdlSet* mdlSet = func_0206e8c0(modelFile_);
    NNSG3dResTex* tex = func_0206e8d0(modelFile_);
    if (tex == NULL) {
        return;
    }
    g_lastLoadedTex = tex;
    texture_ = tex;
    int texSize = func_0206a600(tex);
    func_0206a60c(tex);
    int plttSize = func_0206a71c(tex);
    int texKey = g_VramTransfer.allocTexVram(texSize);
    int plttKey = g_VramTransfer.allocPlttVram(plttSize);
    setPaletteEffectTexture(tex);
    if (!(tex->texInfo.flag & 1)) {
        func_0206a618(tex, texKey, 0);
        func_0206a728(tex, plttKey);
        OS_Wait();
        func_0206a62c(tex, TRUE);
        func_0206a730(tex, TRUE);
    }
    func_0206abb4(mdlSet, tex);
}

ARM void ModelResource::unkfunc_02083530()
{
    NNSG3dResMdlSet* mdlSet = func_0206e8c0(modelFile_);
    NNSG3dResTex* tex = func_0206e8d0(modelFile_);
    if (tex == NULL) {
        return;
    }
    func_0206ac24(mdlSet);
    int plttKey = func_0206a780(tex);
    int texKey;
    int tex4x4Key;
    func_0206a6e4(tex, &texKey, &tex4x4Key);
    if (plttKey != 0) {
        g_VramTransfer.freePlttVram(plttKey);
    }
    if (texKey != 0) {
        g_VramTransfer.freeTexVram(texKey);
    }
    g_lastLoadedTex = NULL;
    texture_ = NULL;
}

ARM void* ModelResource::unkfunc_020835c8()
{
    return modelFile_;
}

ARM NNSG3dResMdl* ModelResource::unkfunc_020835d0()
{
    return model_;
}

ARM NNSG3dResTex* getLastModelTexture()
{
    return g_lastLoadedTex;
}
