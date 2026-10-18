#include "main/cmn/CommonChapterTitle.hpp"
#include "main/dss/DssUtils.hpp"

ARM cmn::CommonChapterTitle* cmn::CommonChapterTitle::getSingleton()
{
    static CommonChapterTitle m_singleton;
    return &m_singleton;
}

ARM cmn::CommonChapterTitle::CommonChapterTitle()
{
}

ARM cmn::CommonChapterTitle::~CommonChapterTitle()
{
}

ARM void cmn::CommonChapterTitle::setup(int chapter, int flag)
{
    char filename[0x80];
    int x;
    int y;
    int w;
    int h;
    int texCoord;
    if (flag == 0) {
        switch (chapter) {
            case 1:
                dss::sprintf_s(filename, sizeof(filename), "data/2d/map/hyodai1_1.tex");
                x = 0;
                y = 0;
                w = 0x80;
                h = 0x80;
                texCoord = 1;
                break;
            case 2:
                dss::sprintf_s(filename, sizeof(filename), "data/2d/map/hyodai1_2.tex");
                x = 0;
                y = 0;
                w = 0x80;
                h = 0x80;
                texCoord = 1;
                break;
            case 3:
                dss::sprintf_s(filename, sizeof(filename), "data/2d/map/hyodai1_3.tex");
                x = 0;
                y = 0;
                w = 0x80;
                h = 0x80;
                texCoord = 1;
                break;
            case 4:
                dss::sprintf_s(filename, sizeof(filename), "data/2d/map/hyodai1_4.tex");
                x = 0;
                y = 0;
                w = 0x80;
                h = 0x80;
                texCoord = 1;
                break;
            case 5:
                dss::sprintf_s(filename, sizeof(filename), "data/2d/map/hyodai1_5.tex");
                x = 0;
                y = 0;
                w = 0x80;
                h = 0x80;
                texCoord = 1;
                break;
            case 6:
                dss::sprintf_s(filename, sizeof(filename), "data/2d/map/hyodai1_5.tex");
                x = 0;
                y = 0;
                w = 0x80;
                h = 0x80;
                texCoord = 1;
                break;
            default:
                dss::sprintf_s(filename, sizeof(filename), "data/2d/map/hyodai1.tex");
                x = 0;
                texCoord = 0;
                y = 0x10;
                w = 0x80;
                h = 0x40;
                break;
        }
    } else {
        dss::sprintf_s(filename, sizeof(filename), "data/2d/map/hyodai1.tex");
        x = 0;
        texCoord = 0;
        y = 0x10;
        w = 0x80;
        h = 0x40;
    }
    sprite_.loadFile(filename, 0);
    sprite_.setMainScreen();
    sprite_.setAlpha(0x1f);
    sprite_.setPolygonID(0x3f);
    sprite_.setDepth(0x80);
    sprite_.sprite_.unk_32 = 1;
    sprite_.setPosition(x, y);
    sprite_.setSize(w, h);
    if (texCoord) {
        sprite_.setTexCoords(0, 0, 0x100, 0xc0);
    }
}

ARM void cmn::CommonChapterTitle::cleanup()
{
    if (sprite_.isLoaded()) {
        sprite_.unload();
    }
}

ARM void cmn::CommonChapterTitle::draw()
{
    if (sprite_.isLoaded()) {
        sprite_.draw();
    }
}
