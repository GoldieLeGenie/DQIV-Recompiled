#include "main/dss/Sprite2D.hpp"
#include "main/cmn/CommonEffectData.hpp"
#include "main/global/Global.hpp"

ARM MenuSprite::MenuSprite()
{
    texture_ = 0;
}

ARM MenuSprite::~MenuSprite()
{
}

ARM void MenuSprite::freeVram()
{
    ((TextureObject*)texture_)->unkfunc_02086868();
}

ARM void MenuSprite::transferToVram()
{
    ((TextureObject*)texture_)->unkfunc_02086968(0);
}

ARM void MenuSprite::allocVram()
{
    ((TextureObject*)texture_)->unkfunc_020868dc();
}

ARM int MenuSprite::getTextureData()
{
    return ((TextureObject*)texture_)->unkfunc_02086a9c();
}

ARM void MenuSprite::loadFile(const char* filename, int a)
{
    data_.setup(filename, a, 0);
    setupTexture();
}

ARM void MenuSprite::loadFromMemory(void* addr)
{
    data_.setup(addr);
    setupTexture();
}

ARM void MenuSprite::setupTexture()
{
    texture_ = data_.getAddr();
    ((TextureObject*)texture_)->unkfunc_02086798(1);
    sprite_.texture_ = texture_;
    sprite_.unkfunc_02084534(0, 0);
    sprite_.unkfunc_0208456c(((TextureObject*)texture_)->unkfunc_02086c18(), ((TextureObject*)texture_)->unkfunc_02086c64());
    sprite_.unkfunc_02084578(0, 0, ((TextureObject*)texture_)->unkfunc_02086c18(), ((TextureObject*)texture_)->unkfunc_02086c64());
}

ARM void MenuSprite::unload()
{
    ((TextureObject*)texture_)->unkfunc_02086868();
    data_.cleanup();
    texture_ = 0;
}

ARM void MenuSprite::attachRender(Render* render)
{
    render_ = render;
    render->unkfunc_020852c8(&sprite_);
}

ARM int MenuSprite::isLoaded()
{
    return texture_ != 0;
}

ARM void MenuSprite::setPosition(int x, int y)
{
    sprite_.unkfunc_02084534(x, y);
}

ARM void MenuSprite::setSize(int w, int h)
{
    sprite_.unkfunc_0208456c(w, h);
}

ARM void MenuSprite::setTexCoords(int u0, int v0, int u1, int v1)
{
    sprite_.unkfunc_02084578(u0, v0, u1, v1);
}

ARM void MenuSprite::draw()
{
    sprite_.draw();
}

ARM void MenuSprite::setEnabled(int enable)
{
    sprite_.enable_ = enable;
}

ARM void MenuSprite::setMainScreen()
{
    sprite_.screens_ = 1;
}

ARM void MenuSprite::setSubScreen()
{
    sprite_.screens_ = 2;
}

ARM void MenuSprite::setBothScreens()
{
    sprite_.screens_ = 3;
}

ARM void MenuSprite::setAlpha(int a)
{
    sprite_.setAlpha((unsigned char)a);
}

ARM void MenuSprite::setPolygonID(int polygonID)
{
    sprite_.setPolygonID((unsigned char)polygonID);
}

ARM void MenuSprite::setDepth(int a)
{
    sprite_.depth_ = a;
}

ARM void MenuSprite::setRotation(int a)
{
    sprite_.rotation_ = a;
}

ARM void MenuSprite::setColor(unsigned char r, unsigned char g, unsigned char b)
{
    sprite_.setColor(r, g, b);
}
