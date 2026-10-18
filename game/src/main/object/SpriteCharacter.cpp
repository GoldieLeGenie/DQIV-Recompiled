#pragma ipa file
#include "main/object/SpriteCharacter.hpp"
#include "main/cmn/CommonEffectData.hpp"
#include "main/script/sys/ScriptParam.hpp"

dss::BitFlag<unsigned char> SpriteCharacter::allFlag_(SpriteCharacter::FLAG_ANIM);
static void* s_shadowTexture;
static DataObject s_commonTex;
static int s_shadowDraw = 1;

ARM SpriteCharacter::SpriteCharacter()
{
}

ARM SpriteCharacter::~SpriteCharacter()
{
}

ARM void SpriteCharacter::setup(const char* name)
{
    data_.setup(name, 0, 0);
    unkfunc_0204b25c();
}

ARM void SpriteCharacter::unkfunc_0204b25c()
{
    textureNum_ = getArchiveFileCount(data_.getAddr());
    for (int i = 0; i < textureNum_; i++) {
        textures_[i] = getArchiveFile(data_.getAddr(), i);
    }
    dispTexture_ = textures_[0];
    ((TextureObject*)dispTexture_)->unkfunc_02086798(1);
    texture_ = dispTexture_;
    unkfunc_02084534(0, 0);
    anmIndex_ = 0;
    direction_ = -1;
    dispDirection_ = 0;
    frameWidth_ = 0x18;
    frameHeight_ = 0x20;
    unkfunc_0204b33c();
    shadow_.unkfunc_0204b768();
    flag_.flag_ |= FLAG_DEFAULT;
}

ARM void SpriteCharacter::cleanup()
{
    ((TextureObject*)dispTexture_)->unkfunc_02086868();
    data_.cleanup();
    shadow_.unkfunc_0204b7e4();
}

ARM void SpriteCharacter::unkfunc_0204b33c()
{
    unkfunc_0208456c(frameWidth_, frameHeight_);
    unkfunc_02084578(0, 0, frameWidth_, frameHeight_);
    screens_ = 1;
}

ARM void SpriteCharacter::setAlpha(int alpha)
{
    RenderObject::setAlpha(alpha);
    shadow_.setAlpha((unsigned char)((unsigned int)alpha >> 1));
}

ARM void SpriteCharacter::draw()
{
    if (flag_.check(FLAG_DISPLAY) == false) {
        return;
    }
    execute();
    Sprite2D::draw();
    if (flag_.check(FLAG_SHADOW)) {
        shadow_.draw();
    }
}

ARM void SpriteCharacter::execute()
{
    if (flag_.check(FLAG_ANIM_NEUTRAL)) {
        if (!(allFlag_.flag_ & FLAG_ANIM)) {
            return;
        }
    } else if (!flag_.check(FLAG_ANIM)) {
        return;
    }
    switch (anmIndex_ / 12) {
    case 0:
        unkfunc_02084578(0, 0, frameWidth_, frameHeight_);
        break;
    case 1:
        unkfunc_02084578(frameWidth_, 0, frameWidth_ * 2, frameHeight_);
        break;
    case 2:
        unkfunc_02084578(frameWidth_ * 2, 0, frameWidth_ * 3, frameHeight_);
        break;
    case 3:
        unkfunc_02084578(frameWidth_, 0, frameWidth_ * 2, frameHeight_);
        break;
    }
    anmIndex_++;
    anmIndex_ = dss::loop(anmIndex_, 0, 0x30);
}

ARM void SpriteCharacter::reload(int dir)
{
    if (dispDirection_ == dir) {
        return;
    }
    ((TextureObject*)dispTexture_)->unkfunc_020869ec((TextureObject*)textures_[dir], 0);
    dispDirection_ = dir;
}

ARM void SpriteCharacter::setPosition(int x, int y)
{
    unkfunc_02084534(x - frameWidth_ / 2, y - frameHeight_);
    if (flag_.check(FLAG_STAY)) {
        shadow_.setPosition(x, y);
    }
}

ARM void SpriteCharacter::setPosition(dss::Vector2<int> pos)
{
    unkfunc_02084534(pos.vx - frameWidth_ / 2, pos.vy - frameHeight_);
    if (flag_.check(FLAG_STAY)) {
        shadow_.setPosition(pos.vx, pos.vy);
    }
}

ARM void SpriteCharacter::setDirection(unsigned short dir)
{
    if (textureNum_ == 4) {
        direction_ = dir / 2;
        reload(direction_);
    } else {
        direction_ = dir;
        reload(direction_);
    }
}

ARM void SpriteCharacter::setDisplayEnable(int flag)
{
    if (flag) {
        flag_.flag_ |= FLAG_DISPLAY;
    } else {
        flag_.flag_ &= ~FLAG_DISPLAY;
    }
}

ARM void SpriteCharacter::setAnimFlag(int flag)
{
    if (flag == 1) {
        flag_.flag_ |= FLAG_ANIM;
        flag_.flag_ &= ~FLAG_ANIM_NEUTRAL;
    } else if (flag == 2) {
        flag_.flag_ |= FLAG_ANIM_NEUTRAL;
    } else {
        flag_.flag_ &= ~FLAG_ANIM;
        flag_.flag_ &= ~FLAG_ANIM_NEUTRAL;
    }
}

ARM void SpriteCharacter::setShadowFlag(int flag)
{
    if (flag) {
        flag_.flag_ |= FLAG_SHADOW;
    } else {
        flag_.flag_ &= ~FLAG_SHADOW;
    }
}

ARM void SpriteCharacter::setAllCharaAnim(int flag)
{
    if (flag) {
        allFlag_.flag_ |= FLAG_ANIM;
    } else {
        allFlag_.flag_ &= ~FLAG_ANIM;
    }
}

ARM int SpriteCharacter::getAllCharaAnim()
{
    return (allFlag_.flag_ & FLAG_ANIM) ? true : false;
}

ARM SpriteShadow::SpriteShadow()
{
}

ARM SpriteShadow::~SpriteShadow()
{
}

ARM void SpriteCharacter::unkfunc_0204b704()
{
    s_commonTex.setup("data/common.tex", 0, 0);
    s_shadowTexture = s_commonTex.getAddr();
    ((TextureObject*)s_shadowTexture)->unkfunc_02086798(1);
}

ARM void SpriteCharacter::unkfunc_0204b744()
{
    ((TextureObject*)s_shadowTexture)->unkfunc_02086868();
    s_commonTex.cleanup();
}

ARM void SpriteShadow::unkfunc_0204b768()
{
    texture_ = s_shadowTexture;
    unkfunc_02084534(0, 0);
    unkfunc_0208456c(0x12, 0xc);
    unkfunc_02084578(0, 0, 0x20, 0x20);
    screens_ = 1;
    depth_ = 4;
    setAlpha(15);
}

ARM void SpriteShadow::unkfunc_0204b7e4()
{
}

ARM void SpriteShadow::draw()
{
    if (s_shadowDraw) {
        Sprite2D::draw();
    }
}

ARM void SpriteShadow::setPosition(int x, int y)
{
    unkfunc_02084534(x - 9, y - 8);
}
