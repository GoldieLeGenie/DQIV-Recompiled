#include "ov000/town/TownMapEffect.hpp"
#include "main/dss/RenderObject.hpp"
#include "main/data/FileLoader.hpp"
#include "main/script/ScriptSystem.hpp"

ARM void TownMapEffect::setup(EFFECT_TYPE type)
{
    char path[128];
    type_ = type;
    unkfunc_02142790();
    m_enable = 0;
    if (loaded_) {
        sprite0_.unkfunc_02057e34();
    }
    loaded_ = 1;
    if (exist_) {
        switch (type_) {
        case EFFECT_TYPE_DREAM1:
            dss::sprintf_s(path, sizeof(path), "data/mask/siro1a1.tex");
            sprite0_.unkfunc_02057d60(path, 0);
            sprite0_.unkfunc_02057edc();
            sprite0_.unkfunc_02057f00(30);
            sprite0_.unkfunc_02057f18(60);
            sprite1_.unkfunc_02057ee8();
            sprite1_.unkfunc_02057f18(61);
            break;
        case EFFECT_TYPE_DREAM2:
            dss::sprintf_s(path, sizeof(path), "data/mask/siro1a2.tex");
            sprite0_.unkfunc_02057d60(path, 0);
            sprite0_.unkfunc_02057edc();
            sprite0_.unkfunc_02057f00(30);
            sprite0_.unkfunc_02057f18(60);
            sprite1_.unkfunc_02057dac();
            sprite1_.unkfunc_02057ee8();
            sprite1_.unkfunc_02057f18(61);
            break;
        case EFFECT_TYPE_DEATHPISARO:
            dss::sprintf_s(path, sizeof(path), "data/mask/pisaro.tex");
            sprite0_.unkfunc_02057d60(path, 0);
            sprite0_.unkfunc_02057edc();
            sprite0_.unkfunc_02057f00(30);
            sprite0_.unkfunc_02057f18(60);
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
        sprite0_.unkfunc_02057ed4(1);
    } else {
        loaded_ = 0;
        sprite0_.unkfunc_02057ed4(0);
    }
}

ARM void TownMapEffect::draw()
{
    if (m_enable == 0) {
        return;
    }
    if (exist_) {
        unkfunc_020847e8();
        switch (type_) {
        case EFFECT_TYPE_DREAM1:
        case EFFECT_TYPE_DREAM2:
            sprite0_.unkfunc_02057ea8(0, 0, 127, 95);
            sprite0_.unkfunc_02057e98(128, 96);
            sprite0_.unkfunc_02057e88(0, 0);
            sprite0_.unkfunc_02057f38(0);
            sprite0_.unkfunc_02057f40(red_, green_, blue_);
            sprite0_.unkfunc_02057ec0();
            sprite0_.unkfunc_02057e88(128, 96);
            sprite0_.unkfunc_02057f38(0x8000);
            sprite0_.unkfunc_02057f40(red_, green_, blue_);
            sprite0_.unkfunc_02057ec0();
            sprite0_.unkfunc_02057ea8(127, 0, 0, 95);
            sprite0_.unkfunc_02057e98(128, 96);
            sprite0_.unkfunc_02057e88(128, 0);
            sprite0_.unkfunc_02057f38(0);
            sprite0_.unkfunc_02057f40(red_, green_, blue_);
            sprite0_.unkfunc_02057ec0();
            sprite0_.unkfunc_02057e88(0, 96);
            sprite0_.unkfunc_02057f38(0x8000);
            sprite0_.unkfunc_02057f40(red_, green_, blue_);
            sprite0_.unkfunc_02057ec0();
            sprite1_.unkfunc_02057e88(0, 0);
            sprite1_.unkfunc_02057e98(256, 192);
            sprite1_.unkfunc_02057f40(red_, green_, blue_);
            sprite1_.unkfunc_02057ec0();
            break;
        case EFFECT_TYPE_DEATHPISARO:
            sprite0_.unkfunc_02057e98(width_, height_);
            sprite0_.unkfunc_02057e88(x_ - (width_ >> 1), y_ - (height_ >> 1));
            sprite0_.unkfunc_02057ec0();
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
        sprite0_.unkfunc_02057e34();
        exist_ = 0;
        m_enable = 0;
    }
    loaded_ = 0;
}

ARM void TownMapEffect::unkfunc_02142790()
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

ARM void TownMapEffect::unkfunc_02142850(fx32 scaleX)
{
}

ARM void TownMapEffect::unkfunc_02142854(fx32 scaleY)
{
}
