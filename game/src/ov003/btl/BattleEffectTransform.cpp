#include "ov003/btl/BattleEffectTransform.hpp"
#include "ov003/btl/BattleTransform.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/data/FileLoader.hpp"
#include "main/script/ScriptSystem.hpp"
#include "main/cmn/CommonEffectData.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/param/MonsterAnim.hpp"
#include "main/sound/SoundManager.hpp"
#include "ov003/btl/BattleCamera.hpp"
#include <printf.h>

#pragma profile on

THUMB btl::BattleEffectTransform::BattleEffectTransform()
{
    enable_ = 0;
}

THUMB btl::BattleEffectTransform::~BattleEffectTransform()
{
}

THUMB void btl::BattleEffectTransform::setup(int index, int nearDist, int rev)
{
    char buf[0x80];

    index_ = index;
    rev_ = rev;
    dss::sprintf_s(buf, sizeof(buf), "data/trans/m%03d.tex", index_);
    dataObject_.setup(buf, 1, 0);
    texture_ = dataObject_.getAddr();
    ((TextureObject*)texture_)->unkfunc_02086798(0);

    sprintf(buf, "data/pam/m%03d_tai.pam", index);
    if (dss::g_File.isExist(buf)) {
        if (paletteData_.getAddr()) {
            paletteData_.cleanup();
        }
        paletteData_.setup(buf, 1, 0);
        paletteAnim_.anim_.setup(paletteData_.getAddr());
        paletteAnim_.texture_ = (TextureObject*)texture_;
        paletteAnim_.colorCount_ = paletteAnim_.anim_.getColorCount();
        paletteAnim_.unkfunc_0205b3d0();
    }

    int animIndex = param::MonsterAnim::getAnimData(status::excelParam.monsterAnim_, index, 0, 0x1e);
    if (animIndex >= 0) {
        param::MonsterAnim* anim = &status::excelParam.monsterAnim_[animIndex];
        unsigned char camera = anim->camera;
        if (camera) {
            BattleCamera::getSingleton()->setCameraAnimation(camera, anim->camera2, anim->wait);
        }
    }

    process_ = 0;
    m_near = nearDist;
    readNext();
    enable_ = 1;
    counter_ = 0;
}

THUMB void btl::BattleEffectTransform::draw()
{
    if (!enable_) {
        return;
    }

    dssaObject_.draw();
    dssaObject_.execute();
    if (paletteAnim_.texture_) {
        paletteAnim_.unkfunc_0205b44c();
    }
    if (dssaObject_.isEnd() && !readNext()) {
        enable_ = 0;
    }

    int animIndex = param::MonsterAnim::getAnimData(status::excelParam.monsterAnim_, index_, 0, 0x1e);
    if (animIndex >= 0) {
        param::MonsterAnim* anim = &status::excelParam.monsterAnim_[animIndex];
        if (counter_ == anim->hitframe && anim->sound) {
            SoundManager::playSe(anim->sound, 0);
        }
    }
    counter_++;
}

THUMB void btl::BattleEffectTransform::cleanup()
{
    if (texture_) {
        ((TextureObject*)texture_)->unkfunc_02086868();
        dataObject_.cleanup();
        texture_ = 0;
        paletteAnim_.unkfunc_0205b648();
        paletteData_.cleanup();
    }
}

THUMB int btl::BattleEffectTransform::readNext()
{
    char buf[0x80];

    if (animData_.getAddr()) {
        dssaObject_.cleanup();
        animData_.cleanup();
    }

    dss::sprintf_s(buf, sizeof(buf), "data/trans/m%03d_%02d.dssa", index_, process_);
    if (!dss::g_File.isExist(buf)) {
        return 0;
    }

    animData_.setup(buf, 0, 1);
    dssaObject_.setup(animData_.getAddr());
    dssaObject_.setTexture(texture_);
    dssaObject_.type_ = DSSAObjectWithCamera::Near;
    dssaObject_.setReverse(rev_);

    dss::Fix32Vector3 pos;
    pos.vz.value = m_near << 10;
    dssaObject_.setPosition(pos);
    process_++;
    return 1;
}

THUMB int btl::BattleEffectTransform::isEnd()
{
    return animData_.getAddr() == 0;
}

static const int indexOffset[14][3] = {
    { 250, 251, -1 },
    { 252, 10253, -1 },
    { 254, -1, -1 },
    { 255, -1, -1 },
    { 256, 258, -1 },
    { 10257, 10258, -1 },
    { 260, 10260, 259 },
    { 262, 261, 263 },
    { 270, 272, -1 },
    { 10271, 10272, -1 },
    { 274, 10274, 273 },
    { 276, 275, 277 },
    { 278, -1, -1 },
};

THUMB btl::BattleTransform::BattleTransform() : enable_(0)
{
}

THUMB btl::BattleTransform::~BattleTransform()
{
}

THUMB btl::BattleTransform* btl::BattleTransform::getSingleton()
{
    static BattleTransform m_singleton;
    return &m_singleton;
}

THUMB void btl::BattleTransform::setup(int index)
{
    if (data_020ed1bc.isOpen()) {
        data_020ed1bc.close();
    }

    switch (index) {
    case 0xae:
        transIndex_ = 0;
        data_020ed1bc.openMessageForBATTLE();
        data_020ed1bc.addMessage(0x1f7f4);
        data_020ed1bc.setMessageLastCursor(false);
        break;
    case 0xcd:
        transIndex_ = 1;
        data_020ed1bc.openMessageForBATTLE();
        data_020ed1bc.addMessage(0x1f7f4);
        data_020ed1bc.setMessageLastCursor(false);
        break;
    case 0xce:
        transIndex_ = 2;
        data_020ed1bc.openMessageForBATTLE();
        data_020ed1bc.addMessage(0x1f7f6);
        data_020ed1bc.setMessageLastCursor(false);
        break;
    case 0xcf:
        transIndex_ = 4;
        break;
    case 0xd0:
        transIndex_ = 6;
        break;
    case 0xd1:
        transIndex_ = 7;
        break;
    case 0xd2:
        return;
    case 0x130:
        transIndex_ = 8;
        break;
    case 0x131:
        transIndex_ = 0xb;
        break;
    case 0x132:
    case 0x133:
        transIndex_ = 0xc;
        break;
    case 0x134:
    case 0x135:
        break;
    default:
        return;
    }

    for (int i = 0; i < 3; i++) {
        int monster = indexOffset[transIndex_][i];
        if (monster != -1) {
            int animIndex = param::MonsterAnim::getAnimData(status::excelParam.monsterAnim_, monster, 0, 0x1e);
            if (animIndex >= 0) {
                param::MonsterAnim* anim = &status::excelParam.monsterAnim_[animIndex];
                unsigned char camera = anim->camera;
                if (camera) {
                    BattleCamera::getSingleton()->setCameraAnimation(camera, anim->camera2, anim->wait);
                }
            }
        }
    }
    setTransform();
}

THUMB void btl::BattleTransform::setTransform()
{
    enable_ = 1;
    max_ = 0;
    for (int i = 0; i < 3; i++) {
        int monster = indexOffset[transIndex_][i];
        if (monster != -1) {
            if (monster > 10000) {
                trans_[i].setup(monster - 10000, i, 1);
            } else {
                trans_[i].setup(monster, i, 0);
            }
            max_++;
        }
    }
}

THUMB void btl::BattleTransform::draw()
{
    if (enable_) {
        for (int i = 0; i < max_; i++) {
            trans_[i].draw();
        }
    }
}

THUMB void btl::BattleTransform::cleanup()
{
    if (data_020ed1bc.isOpen()) {
        data_020ed1bc.close();
    }
    for (int i = 0; i < max_; i++) {
        trans_[i].cleanup();
    }
    enable_ = 0;
}

THUMB int btl::BattleTransform::isEnd()
{
    for (int i = 0; i < max_; i++) {
        if (!trans_[i].isEnd()) {
            return 0;
        }
    }
    return 1;
}

THUMB int btl::BattleTransform::startNext()
{
    switch (transIndex_) {
    case 0:
    case 1:
    case 3:
    case 5:
    case 6:
    case 7:
    case 10:
    case 11:
    case 12:
        return 0;
    case 4:
    case 8:
    case 9:
        transIndex_++;
        setTransform();
        return 1;
    case 2:
        if (data_020ed1bc.isOpen()) {
            data_020ed1bc.close();
        }
        data_020ed1bc.openMessageForBATTLE();
        data_020ed1bc.addMessage(0x1f7f8);
        data_020ed1bc.setMessageLastCursor(false);
        transIndex_++;
        setTransform();
        return 1;
    }
    return 0;
}

THUMB int btl::BattleTransform::getDummyFromMonster(int index)
{
    switch (index) {
    case 0xae:  return 0x118;
    case 0xcd:  return 0x119;
    case 0xce:  return 0x11a;
    case 0xcf:  return 0x11c;
    case 0xd0:  return 0x11e;
    case 0xd1:  return 0x11f;
    case 0x130: return 0x121;
    case 0x131: return 0x124;
    case 0x132:
    case 0x133: return 0x125;
    default:    return 0;
    }
}

THUMB int btl::BattleTransform::getDummyFromTrans()
{
    switch (getSingleton()->transIndex_) {
    case 0:  return 0x118;
    case 1:  return 0x119;
    case 2:  return 0x11a;
    case 3:  return 0x11b;
    case 4:  return 0x11c;
    case 5:  return 0x11d;
    case 6:  return 0x11e;
    case 7:  return 0x11f;
    case 8:  return 0x121;
    case 9:  return 0x122;
    case 10: return 0x123;
    case 11: return 0x124;
    case 12: return 0x125;
    default: return 0;
    }
}
