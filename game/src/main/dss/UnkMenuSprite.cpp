#include "main/dss/UnkSprite2D.hpp"
#include "main/cmn/CommonEffectData.hpp"
#include "main/global/Global.hpp"

ARM UnkMenuSprite::UnkMenuSprite()
{
    texture_ = 0;
}

ARM UnkMenuSprite::~UnkMenuSprite()
{
}

ARM void UnkMenuSprite::unkfunc_02057d1c()
{
    ((TextureObject*)texture_)->unkfunc_02086868();
}

ARM void UnkMenuSprite::unkfunc_02057d2c()
{
    ((TextureObject*)texture_)->unkfunc_02086968(0);
}

ARM void UnkMenuSprite::unkfunc_02057d40()
{
    ((TextureObject*)texture_)->unkfunc_020868dc();
}

ARM int UnkMenuSprite::unkfunc_02057d50()
{
    return ((TextureObject*)texture_)->unkfunc_02086a9c();
}

ARM void UnkMenuSprite::unkfunc_02057d60(const char* filename, int a)
{
    data_.setup(filename, a, 0);
    unkfunc_02057dac();
}

ARM void UnkMenuSprite::unkfunc_02057d88(void* addr)
{
    data_.setup(addr);
    unkfunc_02057dac();
}

ARM void UnkMenuSprite::unkfunc_02057dac()
{
    texture_ = data_.getAddr();
    ((TextureObject*)texture_)->unkfunc_02086798(1);
    sprite_.texture_ = texture_;
    sprite_.unkfunc_02084534(0, 0);
    sprite_.unkfunc_0208456c(((TextureObject*)texture_)->unkfunc_02086c18(), ((TextureObject*)texture_)->unkfunc_02086c64());
    sprite_.unkfunc_02084578(0, 0, ((TextureObject*)texture_)->unkfunc_02086c18(), ((TextureObject*)texture_)->unkfunc_02086c64());
}

ARM void UnkMenuSprite::unkfunc_02057e34()
{
    ((TextureObject*)texture_)->unkfunc_02086868();
    data_.cleanup();
    texture_ = 0;
}

ARM void UnkMenuSprite::unkfunc_02057e58(Render* render)
{
    render_ = render;
    render->unkfunc_020852c8(&sprite_);
}

ARM int UnkMenuSprite::unkfunc_02057e74()
{
    return texture_ != 0;
}

ARM void UnkMenuSprite::unkfunc_02057e88(int x, int y)
{
    sprite_.unkfunc_02084534(x, y);
}

ARM void UnkMenuSprite::unkfunc_02057e98(int w, int h)
{
    sprite_.unkfunc_0208456c(w, h);
}

ARM void UnkMenuSprite::unkfunc_02057ea8(int u0, int v0, int u1, int v1)
{
    sprite_.unkfunc_02084578(u0, v0, u1, v1);
}

ARM void UnkMenuSprite::unkfunc_02057ec0()
{
    sprite_.draw();
}

ARM void UnkMenuSprite::unkfunc_02057ed4(int enable)
{
    sprite_.enable_ = enable;
}

ARM void UnkMenuSprite::unkfunc_02057edc()
{
    sprite_.unk_2c = 1;
}

ARM void UnkMenuSprite::unkfunc_02057ee8()
{
    sprite_.unk_2c = 2;
}

ARM void UnkMenuSprite::unkfunc_02057ef4()
{
    sprite_.unk_2c = 3;
}

ARM void UnkMenuSprite::unkfunc_02057f00(int a)
{
    sprite_.setAlpha((unsigned char)a);
}

ARM void UnkMenuSprite::unkfunc_02057f18(int polygonID)
{
    sprite_.setPolygonID((unsigned char)polygonID);
}

ARM void UnkMenuSprite::unkfunc_02057f30(int a)
{
    sprite_.unk_28 = a;
}

ARM void UnkMenuSprite::unkfunc_02057f38(int a)
{
    sprite_.unk_30 = a;
}

ARM void UnkMenuSprite::unkfunc_02057f40(unsigned char r, unsigned char g, unsigned char b)
{
    sprite_.setColor(r, g, b);
}
