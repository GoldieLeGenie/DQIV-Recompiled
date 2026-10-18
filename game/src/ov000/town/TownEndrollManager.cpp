#include "ov000/town/TownEndrollManager.hpp"
#ifdef DQIV_NATIVE
#include "DualScreen.hpp"
#endif
#include "main/dss/RenderObject.hpp"
#include "main/dss/Display.hpp"
#include "main/cmn/CommonEffectData.hpp"
#include "main/dss/Camera.hpp"
#include "main/dss/UnkLanguage.hpp"
#include "main/profile/Profile.hpp"

THUMB TownEndrollManager::TownEndrollManager()
{
    staffCount_ = 0x80;
    enable_ = 0;
}

THUMB TownEndrollManager::~TownEndrollManager()
{
}

THUMB TownEndrollManager* TownEndrollManager::getSingleton()
{
    static TownEndrollManager m_singleton;
    return &m_singleton;
}

THUMB void TownEndrollManager::setup()
{
    char filename[0x80];
    enable_ = 1;
    frame_ = 0;
    unk_180 = 0;
    for (int i = 0; i < STAFF_MAX; i++) {
        staffPos_[i] = -(i * 0xc0);
        staffIndex_[i] = i;
        if (getGameRegion() == 2) {
            dss::sprintf_s(filename, 0x80, "data/endroll_na/endroll%02d.tex", i);
            staffCount_ = 0x40;
        } else {
            dss::sprintf_s(filename, 0x80, "data/endroll_eu/endroll%02d.tex", i);
            staffCount_ = 0x40;
        }
        staff_[i].loadFile(filename, 1);
        staff_[i].setAlpha(0x1f);
        staff_[i].setPolygonID(0x3f);
        staff_[i].setBothScreens();
        staff_[i].sprite_.unk_32 = 1;
        staff_[i].setSize(0x100, 0xc0);
        staff_[i].setTexCoords(0, 0, 0x100, 0xc0);
    }
    theEndFrame_ = 0;
    enableTheEnd_ = 0;
    enableScroll_ = 1;
    staffDone_ = 0;
    unk_18c = -1;
}

THUMB void TownEndrollManager::cleanup()
{
    if (enable_) {
        for (int i = 0; i < STAFF_MAX; i++) {
            if (staff_[i].isLoaded()) {
                staff_[i].unload();
            }
        }
        theEndData_.cleanup();
#ifdef DQIV_NATIVE
        // The DS reads its low memory when "THE END" never started; the PC cannot.
        if (texture_)
#endif
        ((TextureObject*)texture_)->unkfunc_02086868();
        textureData_.cleanup();
        enable_ = 0;
    }
}

THUMB void TownEndrollManager::animTheEnd()
{
    char filename[0x80];
    if (enableTheEnd_ && (getDisplayFrame() & 1)) {
        if (theEndData_.getAddr()) {
            theEndData_.cleanup();
        }
        dss::sprintf_s(filename, 0x80, "data/fin/fin_%03d.tex", theEndFrame_);
        theEndData_.setup(filename, 0, 0);
        theEndTexture_ = theEndData_.getAddr();
        dss::memcpy((void*)((TextureObject*)texture_)->unkfunc_02086a9c(), (char*)theEndTexture_ + ((TextureObject*)theEndTexture_)->unkfunc_02086a9c(), 0x1800);
        ((TextureObject*)texture_)->unkfunc_02086968(0);
        theEndFrame_++;
        if (theEndFrame_ == 0xb5) {
            enableTheEnd_ = 0;
        }
    }
}

THUMB void TownEndrollManager::drawStaffRoll()
{
    char filename[0x80];
    if (enableScroll_ == 0) {
        return;
    }
    for (int i = 0; i < STAFF_MAX; i++) {
        if (!staff_[i].isLoaded()) {
            continue;
        }
        if (getDisplayFrame() & 1) {
            staff_[i].setPosition(0, 0xc0 - staffPos_[i]);
        } else {
            staff_[i].setPosition(0, 0x180 - staffPos_[i]);
        }
        if ((staffCount_ - 1 == staffIndex_[i] && staffPos_[i] >= 0xc0) ||
            (staffCount_ - 2 == staffIndex_[i] && staffPos_[i] >= 0x180)) {
            if (staffCount_ - 1 == staffIndex_[i]) {
                staff_[i].setMainScreen();
            } else {
                staff_[i].setSubScreen();
            }
            staff_[i].setPosition(0, 0);
            staff_[i].draw();
            staffDone_ = 1;
            continue;
        }
        if ((unsigned int)frame_ % 3 < 2) {
            staffPos_[i]++;
        }
#ifdef DQIV_NATIVE
        if (pc::unifiedFrame()) {
            // The credits lie on black covering the DS screen's width: the
            // black continues across the wide picture, under the panel.
            // It samples the panel's own black corner, for the same shade.
            Sprite2D& sprite = staff_[i].sprite_;
            const short u0 = sprite.unk_20, v0 = sprite.unk_22, u1 = sprite.unk_24, v1 = sprite.unk_26;
            sprite.unkfunc_02084578(0, 0, 1, 1);
            sprite.draw();
            sprite.unkfunc_02084578(u0, v0, u1, v1);
            pc::setSpriteDecal(true);
            staff_[i].draw();
            pc::setSpriteDecal(false);
        } else
#endif
        staff_[i].draw();
        if (staffPos_[i] >= 0x240) {
            staffPos_[i] = 0;
            staffIndex_[i] = staffIndex_[i] + 3;
            staff_[i].unload();
            if (staffIndex_[i] < staffCount_) {
                if (getGameRegion() == 2) {
                    dss::sprintf_s(filename, 0x80, "data/endroll_na/endroll%02d.tex", staffIndex_[i]);
                } else {
                    dss::sprintf_s(filename, 0x80, "data/endroll_eu/endroll%02d.tex", staffIndex_[i]);
                }
                staff_[i].loadFile(filename, 1);
                staff_[i].setSize(0x100, 0xc0);
                staff_[i].setTexCoords(0, 0, 0x100, 0xc0);
            }
        }
    }
    frame_++;
}

THUMB void TownEndrollManager::startTheEnd()
{
    char filename[0x80];
    enableTheEnd_ = 1;
    enableScroll_ = 0;
    dss::sprintf_s(filename, 0x80, "data/fin/fin_000.tex");
    textureData_.setup(filename, 0, 0);
    texture_ = textureData_.getAddr();
    ((TextureObject*)texture_)->unkfunc_02086798(1);
    theEnd_.texture_ = texture_;
    theEnd_.unkfunc_02084534(0x48, 0x20);
    theEnd_.unkfunc_02084578(0, 0, 0x80, 0x60);
    theEnd_.unkfunc_0208456c(0x80, 0x60);
    theEnd_.unk_2c = 1;
}

THUMB bool TownEndrollManager::isEndTheEnd()
{
    return theEndFrame_ == 0xb5;
}

THUMB void TownEndrollManager::draw()
{
    if (enable_) {
        setupOrtho2D();
        Sprite2D& theEnd = theEnd_;
        theEnd.draw();
        drawStaffRoll();
    }
}

THUMB void TownEndrollManager::execute()
{
    if (enable_) {
        animTheEnd();
    }
}

THUMB int TownEndrollManager::isStaffRollEnd()
{
    return staffDone_;
}
