#include "ov000/town/TownMapEffect.hpp"
#include "main/dss/RenderObject.hpp"
#include "main/data/FileLoader.hpp"
#include "main/script/ScriptSystem.hpp"

ARM void TownMapEffect::setup(EFFECT_TYPE type)
{
    char path[128];
    type_ = type;
    checkTextureExist();
    m_enable = 0;
    if (loaded_) {
        sprite0_.unload();
    }
    loaded_ = 1;
    if (exist_) {
        switch (type_) {
        case EFFECT_TYPE_DREAM1:
            dss::sprintf_s(path, sizeof(path), "data/mask/siro1a1.tex");
            sprite0_.loadFile(path, 0);
            sprite0_.setMainScreen();
            sprite0_.setAlpha(30);
            sprite0_.setPolygonID(60);
            sprite1_.setSubScreen();
            sprite1_.setPolygonID(61);
            break;
        case EFFECT_TYPE_DREAM2:
            dss::sprintf_s(path, sizeof(path), "data/mask/siro1a2.tex");
            sprite0_.loadFile(path, 0);
            sprite0_.setMainScreen();
            sprite0_.setAlpha(30);
            sprite0_.setPolygonID(60);
            sprite1_.setupTexture();
            sprite1_.setSubScreen();
            sprite1_.setPolygonID(61);
            break;
        case EFFECT_TYPE_DEATHPISARO:
            dss::sprintf_s(path, sizeof(path), "data/mask/pisaro.tex");
            sprite0_.loadFile(path, 0);
            sprite0_.setMainScreen();
            sprite0_.setAlpha(30);
            sprite0_.setPolygonID(60);
            x_ = 128;
            y_ = 96;
            width_ = 256;
            height_ = 192;
            break;
        }
    }
    red_ = 31;
    green_ = 31;
    blue_ = 31;
    m_counter = -1;
}

ARM void TownMapEffect::execute()
{
    if (m_enable == 0) {
        return;
    }
    if (exist_) {
        sprite0_.setEnabled(1);
    } else {
        loaded_ = 0;
        sprite0_.setEnabled(0);
    }
}

ARM void TownMapEffect::draw()
{
    if (m_enable == 0) {
        return;
    }
    if (exist_) {
        setupOrtho2D();
        switch (type_) {
        case EFFECT_TYPE_DREAM1:
        case EFFECT_TYPE_DREAM2:
            sprite0_.setTexCoords(0, 0, 127, 95);
            sprite0_.setSize(128, 96);
            sprite0_.setPosition(0, 0);
            sprite0_.setRotation(0);
            sprite0_.setColor(red_, green_, blue_);
            sprite0_.draw();
            sprite0_.setPosition(128, 96);
            sprite0_.setRotation(0x8000);
            sprite0_.setColor(red_, green_, blue_);
            sprite0_.draw();
            sprite0_.setTexCoords(127, 0, 0, 95);
            sprite0_.setSize(128, 96);
            sprite0_.setPosition(128, 0);
            sprite0_.setRotation(0);
            sprite0_.setColor(red_, green_, blue_);
            sprite0_.draw();
            sprite0_.setPosition(0, 96);
            sprite0_.setRotation(0x8000);
            sprite0_.setColor(red_, green_, blue_);
            sprite0_.draw();
            sprite1_.setPosition(0, 0);
            sprite1_.setSize(256, 192);
            sprite1_.setColor(red_, green_, blue_);
            sprite1_.draw();
            break;
        case EFFECT_TYPE_DEATHPISARO:
            sprite0_.setSize(width_, height_);
            sprite0_.setPosition(x_ - (width_ >> 1), y_ - (height_ >> 1));
            sprite0_.draw();
            break;
        }
    }
    if (m_counter > -1) {
        green_ = 3100 / m_base_counter * m_counter / 100;
        blue_ = 3100 / m_base_counter * m_counter / 100;
        m_counter--;
    }
}

ARM void TownMapEffect::setPisaroEvent(int count)
{
    m_base_counter = count;
    m_counter = count;
}

ARM void TownMapEffect::cleanup()
{
    if (loaded_) {
        sprite0_.unload();
        exist_ = 0;
        m_enable = 0;
    }
    loaded_ = 0;
}

ARM void TownMapEffect::checkTextureExist()
{
    char path[128];
    exist_ = 0;
    switch (type_) {
    case EFFECT_TYPE_DREAM1:
        dss::sprintf_s(path, sizeof(path), "data/mask/siro1a1.tex");
        exist_ = dss::g_File.isExist(path);
        break;
    case EFFECT_TYPE_DREAM2:
        dss::sprintf_s(path, sizeof(path), "data/mask/siro1a2.tex");
        exist_ = dss::g_File.isExist(path);
        break;
    case EFFECT_TYPE_DEATHPISARO:
        dss::sprintf_s(path, sizeof(path), "data/mask/pisaro.tex");
        exist_ = dss::g_File.isExist(path);
        break;
    }
}

ARM void TownMapEffect::setScaleX(fx32 scaleX)
{
}

ARM void TownMapEffect::setScaleY(fx32 scaleY)
{
}
