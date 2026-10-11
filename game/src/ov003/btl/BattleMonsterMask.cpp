#pragma ipa file
#include "ov003/btl/BattleMonsterMask.hpp"
#include "main/dss/RenderObject.hpp"
#include "ov003/btl/BattleActorAnimation.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"
#include "main/status/HaveEquipment.hpp"
#include "main/global/Global.hpp"
#include "main/dss/ScreenPosition.hpp"

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
    sprite_.unk_2c = 3;
    sprite_.setPolygonID(0x3e);
    sprite_.unk_28 = 0;
    sprite_.setAlpha(10);
    sprite_.unkfunc_02084534(0, 0);
    sprite_.unkfunc_0208456c(0x80, 0x80);
    sprite_.enable_ = 0;
    sprite_.setColor(0, 0, 0);
    mask_[0].unkfunc_02057d60("data/mask/en02.tex", 0);
    mask_[0].unkfunc_02057f00(1);
    mask_[1].unkfunc_02057d60("data/mask/en01.tex", 0);
    mask_[1].unkfunc_02057f00(1);
    for (int i = 0; i < 2; i++) {
        mask_[i].unkfunc_02057ef4();
        mask_[i].unkfunc_02057f18(0x3e);
        mask_[i].unkfunc_02057f30(0);
        mask_[i].unkfunc_02057e88(0, 0);
        mask_[i].unkfunc_02057ed4(0);
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
        mask_[i].unkfunc_02057e34();
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
    if (select_ < 0) {
        sprite_.enable_ = 0;
        for (int i = 0; i < 2; i++) {
            mask_[i].unkfunc_02057ed4(0);
        }
        return;
    }
    sprite_.enable_ = 1;
    for (int i = 0; i < 2; i++) {
        mask_[i].unkfunc_02057ed4(1);
    }
    for (int i = 0; i < g_monster.getCount(); i++) {
        if (g_monster.getMonsterStatus(i)->isBattleEnable() && select_ == g_monster.getMonsterGroup(i)) {
            targetPos[i] = getTargetPos(i);
        }
    }
    unkfunc_020848a8();
    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 12; i++) {
            if (select_ == g_monster.getMonsterGroup(i) && g_monster.getMonsterStatus(i)->isBattleEnable()) {
                mask_[j].unkfunc_02057e88(targetPos[i].vx / 2, targetPos[i].vy * 2 / 3);
                mask_[j].unkfunc_02057f38(0);
                mask_[j].unkfunc_02057ec0();
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
    unkfunc_0205710c(actorindex, &pos);
}

THUMB dss::Vector2<int> BattleMonsterMask::getTargetPos(int actorindex)
{
    dss::Vector2<int> screen;
    int monster = g_monster.getMonsterIndex(actorindex);
    short* rect = MonsterTaiData[monster];
    int maskScaleX = status::HaveEquipment::getAbsoluteValue(rect[1] - rect[3]);
    int maskScaleY = ((scale_ * status::HaveEquipment::getAbsoluteValue(rect[2] - rect[4])).value / 4096);
    maskScaleX = (scale_ * maskScaleX).value / 4096;
    screen = *unkfunc_02057128(actorindex);
    maskScaleY = maskScaleY * 2 / 3;
    screen.vx -= maskScaleY;
    screen.vy -= maskScaleX;
    for (int i = 0; i < 2; i++) {
        mask_[i].unkfunc_02057e98(maskScaleY, maskScaleX);
    }
    return screen;
}

THUMB int* BattleMonsterMask::getMonsterTouchRect(int actorindex)
{
    int monster = g_monster.getMonsterIndex(actorindex);
    short* rect = MonsterTaiData[monster];
    int w = status::HaveEquipment::getAbsoluteValue(rect[2] - rect[4]);
    int h = status::HaveEquipment::getAbsoluteValue(rect[1] - rect[3]);
    dss::Vector2<int> screen = *unkfunc_02057128(actorindex);
    monsterRectTemp[0] = g_monster.getMonsterGroup(actorindex);
    monsterRectTemp[1] = screen.vx - w / 2;
    monsterRectTemp[2] = screen.vy - h - rect[3] / 2;
    monsterRectTemp[3] = screen.vx + w / 2;
    monsterRectTemp[4] = screen.vy;
    return monsterRectTemp;
}
