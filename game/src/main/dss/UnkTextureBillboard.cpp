#include "main/dss/Billboard.hpp"
#include "main/cmn/CommonEffectData.hpp"

ARM void UnkTextureBillboard::unkfunc_02058680(const BillboardVertex* vertex, const BillboardTexCoord* texCoord, void* texture)
{
    texture_ = texture;
    ((TextureObject*)texture)->unkfunc_02086798(1);
    unkfunc_020840c8(vertex);
    unkfunc_0208413c(texCoord);
    RenderObject::texture_ = texture_;
}

ARM void UnkTextureBillboard::unkfunc_020586c4()
{
    ((TextureObject*)texture_)->unkfunc_02086868();
}

ARM void UnkTextureBillboard::unkfunc_020586d4(int a)
{
    ((TextureObject*)texture_)->unkfunc_020869ec((TextureObject*)(void*)a, 0);
}
