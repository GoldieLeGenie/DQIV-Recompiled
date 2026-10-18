#pragma ipa file
#include "ov003/btl/BattleMonsterMask.hpp"
#include "main/dss/RenderObject.hpp"
#include "ov003/btl/BattleActorAnimation.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"
#include "main/status/HaveEquipment.hpp"
#include "main/global/Global.hpp"
#include "main/dss/ScreenPosition.hpp"
#ifdef DQIV_NATIVE
#include "DualScreen.hpp"
#include "ov003/btl/BattleCamera.hpp"
#endif

static dss::Fix32 unkScale(1.0f);
int BattleMonsterMask::monsterRectTemp[12];

THUMB BattleMonsterMask::BattleMonsterMask() : select_(-1)
{
}

THUMB BattleMonsterMask::~BattleMonsterMask()
{
}

THUMB BattleMonsterMask* BattleMonsterMask::getSingleton()
{
    static BattleMonsterMask battleMonsterMask;
    return &battleMonsterMask;
}

THUMB void BattleMonsterMask::initialize()
{
    sprite_.screens_ = 3;
    sprite_.setPolygonID(0x3e);
    sprite_.depth_ = 0;
    sprite_.setAlpha(10);
    sprite_.unkfunc_02084534(0, 0);
    sprite_.unkfunc_0208456c(0x80, 0x80);
    sprite_.enable_ = 0;
    sprite_.setColor(0, 0, 0);
    mask_[0].loadFile("data/mask/en02.tex", 0);
    mask_[0].setAlpha(1);
    mask_[1].loadFile("data/mask/en01.tex", 0);
    mask_[1].setAlpha(1);
    for (int i = 0; i < 2; i++) {
        mask_[i].setBothScreens();
        mask_[i].setPolygonID(0x3e);
        mask_[i].setDepth(0);
        mask_[i].setPosition(0, 0);
        mask_[i].setEnabled(0);
    }
    scale_ = 0x1000;
}

THUMB void BattleMonsterMask::setup()
{
    select_ = -1;
    for (int i = 0; i < g_monster.getCount(); i++) {
        if (g_monster.getMonsterStatus(i)->isEnable()) {
            calcTargetPos(i);
        }
    }
}

THUMB void BattleMonsterMask::terminate()
{
    for (int i = 0; i < 2; i++) {
        mask_[i].unload();
    }
}

THUMB void BattleMonsterMask::execute()
{
}

THUMB void BattleMonsterMask::select(int groupId)
{
    select_ = groupId;
}

THUMB void BattleMonsterMask::draw()
{
#ifdef DQIV_NATIVE
    // Zoomed 16:9 battles: the zoom that keeps every monster clear of the
    // windows (bosses are drawn smaller than the setting), from their sizes.
    {
        double fit = 0, extent = 0, across = 0, spanTop = 0, spanBottom = 0;
        bool span = false;
        // Across: the monsters and their name plates stay within the band
        // around the DS bottom screen (the plates are drawn there). Their
        // places, unzoomed, from the last projection, while the camera rests
        // (it pans in attacks).
        const bool resting = !BattleCamera::getSingleton()->isCameraAnimation();
        const double zoomUsed = pc::battlePositionZoom(), shiftUsed = pc::battlePositionShift();
        for (int i = 0; i < g_monster.getCount(); i++) {
            if (!g_monster.getMonsterStatus(i)->isBattleEnable()) {
                continue;
            }
            short* rect = MonsterTaiData[g_monster.getMonsterIndex(i)];
            const int h = status::HaveEquipment::getAbsoluteValue(rect[1] - rect[3]);
            double monsterFit = pc::battleZoomToFit(h, rect[3] / 2);
            const dss::Vector2<int> screen = *getScreenPosition(i);
            if (resting && screen.vx == -1 && screen.vy == -1 && zoomUsed > 1) {
                // Beyond the band: smaller until it shows.
                const double smaller = zoomUsed * 0.95;
                across = across == 0 ? smaller : (smaller < across ? smaller : across);
            } else if (resting && zoomUsed > 0) {
                const double from = (screen.vx - 128) / zoomUsed;
                const double half = status::HaveEquipment::getAbsoluteValue(rect[2] - rect[4]) / 2.0;
                const double side = from < 0 ? -from : from;
                // z side + max(z half, plate half) <= the band's half width.
                constexpr double room = 124 + pc::battleBandSide, plateHalf = 36;
                double zoomAcross = side + half > 0 ? room / (side + half) : 4;
                if (zoomAcross * half < plateHalf && side > 0) {
                    zoomAcross = (room - plateHalf) / side;
                }
                across = across == 0 ? zoomAcross : (zoomAcross < across ? zoomAcross : across);
                // Its sprite's top and bottom, unzoomed (y' = zoom y + shift).
                const double feet = (screen.vy - shiftUsed) / zoomUsed, top = feet - rect[1], bottom = feet - rect[3];
                spanTop = !span || top < spanTop ? top : spanTop;
                spanBottom = !span || bottom > spanBottom ? bottom : spanBottom;
                span = true;
            }
            if (h + rect[3] / 2 > extent) {
                extent = h + rect[3] / 2;
            }
            fit = fit == 0 ? monsterFit : (monsterFit < fit ? monsterFit : fit);
        }
        if (fit > 0) {
            pc::setBattleZoomAcross(across);
            pc::setBattleZoomFit(fit); // kept once the monsters are gone (victory)
            pc::setBattleMonsterExtent(extent);
            if (span) {
                pc::setBattleMonsterSpan(spanTop, spanBottom);
            }
        }
    }
#endif
    if (select_ < 0) {
        sprite_.enable_ = 0;
        for (int i = 0; i < 2; i++) {
            mask_[i].setEnabled(0);
        }
        return;
    }
    sprite_.enable_ = 1;
    for (int i = 0; i < 2; i++) {
        mask_[i].setEnabled(1);
    }
    for (int i = 0; i < g_monster.getCount(); i++) {
        if (g_monster.getMonsterStatus(i)->isBattleEnable() && select_ == g_monster.getMonsterGroup(i)) {
            targetPos[i] = getTargetPos(i);
        }
    }
    setupOrtho2DFine();
    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 12; i++) {
            if (select_ == g_monster.getMonsterGroup(i) && g_monster.getMonsterStatus(i)->isBattleEnable()) {
                mask_[j].setPosition(targetPos[i].vx / 2, targetPos[i].vy * 2 / 3);
                mask_[j].setRotation(0);
                mask_[j].draw();
            }
        }
    }
    sprite_.draw();
}

THUMB void BattleMonsterMask::calcTargetPos(int actorindex)
{
    dss::Fix32 x;
    dss::Fix32 y;
    dss::Fix32Vector3 pos;
    int index = g_monster.getMonsterStatus(actorindex)->haveStatusInfo_.drawCtrlId_;
    dss::Fix32Vector3* position = btl::BattleMonsterDraw2::getSingleton()->monsters_[index].monsterDraw_.getPosition();
    pos = dss::Fix32Vector3(*position);
    setWorldPosition(actorindex, &pos);
}

THUMB dss::Vector2<int> BattleMonsterMask::getTargetPos(int actorindex)
{
    dss::Vector2<int> screen;
    int monster = g_monster.getMonsterIndex(actorindex);
    short* rect = MonsterTaiData[monster];
    int maskScaleX = status::HaveEquipment::getAbsoluteValue(rect[1] - rect[3]);
    int maskScaleY = ((scale_ * status::HaveEquipment::getAbsoluteValue(rect[2] - rect[4])).value / 4096);
    maskScaleX = (scale_ * maskScaleX).value / 4096;
    screen = *getScreenPosition(actorindex);
    maskScaleY = maskScaleY * 2 / 3;
#ifdef DQIV_NATIVE
    // Zoomed 16:9 battles: the light around the target grows with the monster.
    maskScaleX = (int)(maskScaleX * pc::battleSpriteScale() + 0.5);
    maskScaleY = (int)(maskScaleY * pc::battleSpriteScale() + 0.5);
#endif
    screen.vx -= maskScaleY;
    screen.vy -= maskScaleX;
    for (int i = 0; i < 2; i++) {
        mask_[i].setSize(maskScaleY, maskScaleX);
    }
    return screen;
}

THUMB int* BattleMonsterMask::getMonsterTouchRect(int actorindex)
{
    int monster = g_monster.getMonsterIndex(actorindex);
    short* rect = MonsterTaiData[monster];
    int w = status::HaveEquipment::getAbsoluteValue(rect[2] - rect[4]);
    int h = status::HaveEquipment::getAbsoluteValue(rect[1] - rect[3]);
#ifdef DQIV_NATIVE
    // Zoomed 16:9 battles draw the monsters larger: their name and touch area
    // follow their size on screen.
    const double scale = pc::battleSpriteScale();
    w = (int)(w * scale + 0.5);
    h = (int)(h * scale + 0.5);
#endif
    dss::Vector2<int> screen = *getScreenPosition(actorindex);
    monsterRectTemp[0] = g_monster.getMonsterGroup(actorindex);
    monsterRectTemp[1] = screen.vx - w / 2;
    monsterRectTemp[2] = screen.vy - h - rect[3] / 2;
    monsterRectTemp[3] = screen.vx + w / 2;
    monsterRectTemp[4] = screen.vy;
    return monsterRectTemp;
}
