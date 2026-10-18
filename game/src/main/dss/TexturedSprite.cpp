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

ARM void UnkMenuSprite::freeVram()
{
    ((TextureObject*)texture_)->unkfunc_02086868();
}

ARM void UnkMenuSprite::transferToVram()
{
    ((TextureObject*)texture_)->unkfunc_02086968(0);
}

ARM void UnkMenuSprite::allocVram()
{
    ((TextureObject*)texture_)->unkfunc_020868dc();
}

ARM int UnkMenuSprite::getTextureData()
{
    return ((TextureObject*)texture_)->unkfunc_02086a9c();
}

ARM void UnkMenuSprite::loadFile(const char* filename, int a)
{
    data_.setup(filename, a, 0);
    setupTexture();
}

ARM void UnkMenuSprite::loadFromMemory(void* addr)
{
    data_.setup(addr);
    setupTexture();
}

ARM void UnkMenuSprite::setupTexture()
{
    texture_ = data_.getAddr();
    ((TextureObject*)texture_)->unkfunc_02086798(1);
    sprite_.texture_ = texture_;
    sprite_.unkfunc_02084534(0, 0);
    sprite_.unkfunc_0208456c(((TextureObject*)texture_)->unkfunc_02086c18(), ((TextureObject*)texture_)->unkfunc_02086c64());
    sprite_.unkfunc_02084578(0, 0, ((TextureObject*)texture_)->unkfunc_02086c18(), ((TextureObject*)texture_)->unkfunc_02086c64());
}

ARM void UnkMenuSprite::unload()
{
    ((TextureObject*)texture_)->unkfunc_02086868();
    data_.cleanup();
    texture_ = 0;
}

ARM void UnkMenuSprite::attachRender(Render* render)
{
    render_ = render;
    render->unkfunc_020852c8(&sprite_);
}

ARM int UnkMenuSprite::isLoaded()
{
    return texture_ != 0;
}

ARM void UnkMenuSprite::setPosition(int x, int y)
{
    sprite_.unkfunc_02084534(x, y);
}

ARM void UnkMenuSprite::setSize(int w, int h)
{
    sprite_.unkfunc_0208456c(w, h);
}

ARM void UnkMenuSprite::setTexCoords(int u0, int v0, int u1, int v1)
{
    sprite_.unkfunc_02084578(u0, v0, u1, v1);
}

ARM void UnkMenuSprite::draw()
{
    sprite_.draw();
}

ARM void UnkMenuSprite::setEnabled(int enable)
{
    sprite_.enable_ = enable;
}

ARM void UnkMenuSprite::setMainScreen()
{
    sprite_.unk_2c = 1;
}

ARM void UnkMenuSprite::setSubScreen()
{
    sprite_.unk_2c = 2;
}

ARM void UnkMenuSprite::setBothScreens()
{
    sprite_.unk_2c = 3;
}

ARM void UnkMenuSprite::setAlpha(int a)
{
    sprite_.setAlpha((unsigned char)a);
}

ARM void UnkMenuSprite::setPolygonID(int polygonID)
{
    sprite_.setPolygonID((unsigned char)polygonID);
}

ARM void UnkMenuSprite::setDepth(int a)
{
    sprite_.unk_28 = a;
}

ARM void UnkMenuSprite::setRotation(int a)
{
    sprite_.unk_30 = a;
}

ARM void UnkMenuSprite::setColor(unsigned char r, unsigned char g, unsigned char b)
{
    sprite_.setColor(r, g, b);
}
