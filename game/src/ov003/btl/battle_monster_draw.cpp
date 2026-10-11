#pragma ipa file
#include "ov003/btl/BattleMonster.hpp"
#include "ov003/btl/BattleCamera.hpp"
#include "ov003/status/MonsterParty.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/HaveEquipment.hpp"
#include "main/param/MonsterAnim.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/dss/Random.hpp"

static const char* animName[14] = { "tai", "at", "at2", "at3", "br", "dk", "na", "ju", "fo", "s1", "s2", "s3", "s4", "tu" };

#pragma profile on

THUMB btl::BattleMonster::BattleMonster() : monsterGroup_(0), monsterIndex_(-1)
{
}

THUMB void btl::BattleMonster::setup(int monsterGroup, int monsterIndex)
{
    monsterGroup_ = monsterGroup;
    monsterIndex_ = monsterIndex;
    monsterDraw_.setup(monsterIndex_);
    monsterDraw_.setCurrentFrame(0, dssrand::rand(30));
    screenPosition_ = 0;
    screenWidth_ = 0;
    animationFlag_ = 0;
    monsterDraw_.setCameraType(DSSAObjectWithCamera::Near2);
}

THUMB void btl::BattleMonster::cleanup()
{
    UnkCharacterPalette* palette = &monsterDraw_.palette_;
    if (palette->enable_) {
        paletteAnim_.unkfunc_0205b648();
        palette->enable_ = 0;
        paletteData_.cleanup();
    }
    monsterIndex_ = -1;
    monsterDraw_.cleanup();
    screenPosition_ = 0;
    screenWidth_ = 0;
}

THUMB bool btl::BattleMonster::isEnable()
{
    return monsterIndex_ != -1;
}

THUMB dss::Fix32 btl::BattleMonster::getWidth()
{
    return monsterDraw_.getWidth();
}

THUMB int btl::BattleMonster::getWidthInt()
{
    return monsterDraw_.getWidthInt();
}

THUMB void btl::BattleMonster::setPosition(const dss::Fix32Vector3& pos)
{
    monsterDraw_.setPosition(pos);
}

THUMB void btl::BattleMonster::draw()
{
    UnkCharacterPalette* palette = &monsterDraw_.palette_;
    if (palette->enable_) {
        paletteAnim_.unkfunc_0205b584(palette->unk_408, palette->unk_404);
        if (paletteAnim_.unkfunc_0205b628()) {
            palette->enable_ = 0;
            paletteData_.cleanup();
        }
    }
    if (monsterDraw_.currentAnimationIndex_ != 0) {
        dss::Fix32Vector3 pos = *monsterDraw_.getPosition();
        pos.vz.value += 0x100;
        monsterDraw_.setPosition(pos);
        monsterDraw_.draw();
        pos.vz.value -= 0x100;
        monsterDraw_.setPosition(pos);
    } else {
        monsterDraw_.draw();
    }
}

THUMB void btl::BattleMonster::startAnimation(int actionIndex, int animIndex)
{
    if (actionIndex == 0x1e9) {
        actionIndex = 0x47;
    }
    if (actionIndex == 0x144) {
        monsterDraw_.start(0xb, 0);
    } else if (animIndex == 0x22) {
        monsterDraw_.start(0x22, 0);
    } else if (animIndex == 0x23) {
        monsterDraw_.start(0x23, 0);
    } else if (animIndex == 0x20) {
        monsterDraw_.start(0x20, 0);
    } else if (animIndex == 0x1f) {
        if (actionIndex == 0x104) {
            monsterDraw_.start(0x1f, 0);
        }
        if (actionIndex == 0) {
            monsterDraw_.start(0x1f, 0);
        }
    } else {
        int anim = param::MonsterAnim::getAnimData(status::excelParam.monsterAnim_, monsterIndex_, actionIndex, animIndex);
        int animNo;
        SoundManager::playSe(status::excelParam.monsterAnim_[anim].sound, 0);
        if (anim < 0) {
            animNo = 0x1e;
        } else {
            setCameraAnimation(anim);
            animNo = status::excelParam.monsterAnim_[anim].animfile;
        }
        monsterDraw_.start(animNo, 0);
        setPaletteAnim(animNo);
    }
}

THUMB void btl::BattleMonster::setCameraAnimation(int index)
{
    param::MonsterAnim* anim = &status::excelParam.monsterAnim_[index];
    BattleCamera::getSingleton()->setCameraAnimation(anim->camera, anim->camera2, anim->wait);
}

THUMB void btl::BattleMonster::startAnimation(int animIndex)
{
    monsterDraw_.start(animIndex, 0);
}

THUMB void btl::BattleMonster::startAnimationWithLoop(int animIndex, int flag)
{
    monsterDraw_.start(animIndex, flag);
}

THUMB bool btl::BattleMonster::startGattai()
{
    if (monsterIndex_ == 0xa9 && monsterDraw_.currentAnimationIndex_ != 9) {
        monsterDraw_.start(9, 0);
        dss::Fix32Vector3 pos(0, 0, 0);
        monsterDraw_.setPosition(pos);
        return true;
    }
    return false;
}

THUMB void btl::BattleMonster::disappearGattaiSlime()
{
    if (monsterIndex_ == 0xa9 && monsterDraw_.currentAnimationIndex_ != 9) {
        monsterDraw_.start(0x24, 1);
    }
}

THUMB bool btl::BattleMonster::isAppearKingSlime2()
{
    if (monsterIndex_ == 0xa9 && monsterDraw_.currentAnimationIndex_ == 9 && monsterDraw_.getCurrentFrame() == monsterDraw_.getMaxFrame() - 1) {
        return true;
    }
    return false;
}

THUMB void btl::BattleMonster::setPaletteAnim(int animNo)
{
}

THUMB void btl::BattleMonster::setTransOfEnd()
{
    monsterDraw_.flag_.flag_ |= 0x20;
}

THUMB btl::BattleMonsterDraw2::BattleMonsterDraw2()
{
    enable_ = 1;
}

THUMB btl::BattleMonsterDraw2::~BattleMonsterDraw2()
{
}

THUMB btl::BattleMonsterDraw2* btl::BattleMonsterDraw2::getSingleton()
{
    static BattleMonsterDraw2 m_singleton;
    return &m_singleton;
}

static dss::Fix32 defaultWidth(1.5f);

THUMB void btl::BattleMonsterDraw2::setup()
{
    dss::Fix32 width(0L);
    dss::Fix32 length(0L);
    int widthInt = 0;
    arrayInt_ = 0;
    int lengthInt = 0;
    getCount();
    for (int i = 0; i < monsterCount_; i++) {
        if (monsters_[i].getWidth() == dss::Fix32(0L)) {
            width = defaultWidth;
        } else {
            width = monsters_[i].getWidth();
            widthInt = monsters_[i].getWidthInt();
        }
        length += width;
        lengthInt += widthInt;
    }
    length = dss::Fix32(0L) - length;
    length /= 2;
    lengthInt = -lengthInt / 2;
    arrayInt_ = lengthInt;
    for (int i = 0; i < monsterCount_; i++) {
        if (monsters_[i].getWidth() == dss::Fix32(0L)) {
            width = defaultWidth;
        } else {
            width = monsters_[i].getWidth();
            widthInt = monsters_[i].getWidthInt();
        }
        dss::Fix32Vector3 position(0, 0, 0);
        position.vx = length + width / 2;
        monsters_[i].setPosition(position);
        length += width;
        monsters_[i].screenPosition_ = lengthInt;
        monsters_[i].screenWidth_ = widthInt;
        lengthInt += widthInt;
    }
}

THUMB void btl::BattleMonsterDraw2::cleanup()
{
    for (int i = 0; i < 12; i++) {
        if (monsters_[i].isEnable()) {
            monsters_[i].cleanup();
        }
    }
}

THUMB int btl::BattleMonsterDraw2::setup(int monsterGroup, int monsterIndex)
{
    for (int i = 0; i < 12; i++) {
        if (!monsters_[i].isEnable()) {
            monsters_[i].setup(monsterGroup, monsterIndex);
            return i;
        }
    }
    return -1;
}

THUMB void btl::BattleMonsterDraw2::cleanup(int ctrl)
{
    monsters_[ctrl].cleanup();
}

THUMB int btl::BattleMonsterDraw2::getCount()
{
    monsterCount_ = 0;
    for (int i = 0; i < 12; i++) {
        if (monsters_[i].isEnable()) {
            monsterCount_++;
        }
    }
    return monsterCount_;
}

THUMB void btl::BattleMonsterDraw2::draw()
{
    if (enable_) {
        for (int i = 0; i < 12; i++) {
            if (monsters_[i].isEnable()) {
                monsters_[i].draw();
            }
        }
    }
}

// unused on DS (dead-stripped by the linker), only its data remains
THUMB void btl::BattleMonsterDraw2::setArrayPos()
{
    arrayInt_ = 0;
    for (int i = 0; i < 256; i++) {
        array_[i] = 0;
    }
    for (int i = 0; i < monsterCount_; i++) {
        if (monsters_[i].isEnable()) {
            if (monsters_[i].getWidth() == dss::Fix32(0L)) {
                arrayInt_ += 50;
            } else {
                arrayInt_ += monsters_[i].getWidthInt();
            }
        }
    }
    arrayInt_ = -arrayInt_ / 2;
    for (int i = 0; i < monsterCount_; i++) {
        if (monsters_[i].isEnable()) {
            int width;
            if (monsters_[i].getWidth() == dss::Fix32(0L)) {
                width = 50;
            } else {
                width = monsters_[i].getWidthInt();
            }
            for (int j = 0; j < width; j++) {
                array_[arrayInt_ + j + 128] = 1;
            }
            arrayInt_ += width;
        }
    }
}

THUMB void btl::BattleMonsterDraw2::resetArrayPos()
{
    arrayInt_ = 0;
    for (int i = 0; i < 256; i++) {
        array_[i] = 0;
    }
    getCount();
    int count = g_monster.getCount();
    for (int i = 0; i < count; i++) {
        if (!g_monster.getMonsterStatus(i)->haveStatusInfo_.isDeath()) {
            int drawCtrlId = g_monster.getMonsterStatus(i)->haveStatusInfo_.drawCtrlId_;
            BattleMonster* monster = &monsters_[drawCtrlId];
            if (monster->screenWidth_ != 0) {
                int width;
                if (monster->getWidth() == dss::Fix32(0L)) {
                    width = 50;
                } else {
                    width = monster->getWidthInt();
                }
                arrayInt_ = monster->screenPosition_;
                for (int j = 0; j < width; j++) {
                    array_[arrayInt_ + j + 128] = 1;
                }
            }
        }
    }
}

THUMB void btl::BattleMonsterDraw2::searchArrayPos(int monsterIndex)
{
    signed char temp_[270];
    resetArrayPos();
    int width = getMonsterWidthInt(monsterIndex);
    spaceCount_ = 0;
    bool start = false;
    for (int i = 128 - width; i < 256 - width; i++) {
        if (array_[i] == 0) {
            start = true;
        }
        if (start && array_[i] == 0) {
            bool flag = true;
            for (int j = 0; j < width; j++) {
                if (array_[i + j] != 0) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                for (int j = 0; j < width; j++) {
                    array_[i + j] = 5;
                }
                spaceArray_[spaceCount_] = i;
                spaceCount_++;
            }
        }
    }
    for (int i = 0; i < 256; i++) {
        temp_[255 - i] = array_[i];
    }
    for (int i = 0; i < 256; i++) {
        array_[i] = temp_[i];
    }
    start = false;
    for (int i = 128 - width; i < 256 - width; i++) {
        if (array_[i] != 0) {
            start = true;
        }
        if (start && array_[i] == 0) {
            bool flag = true;
            for (int j = 0; j < width; j++) {
                if (array_[i + j] != 0) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                for (int j = 0; j < width; j++) {
                    array_[i + j] = 5;
                }
                spaceArray_[spaceCount_] = 256 - i - width;
                spaceCount_++;
            }
        }
    }
    for (int i = 0; i < spaceCount_; i++) {
        if (spaceArray_[i] != 0) {
            spaceArray_[i] += width / 2;
            spaceArray_[i] -= 128;
        }
    }
    int min = 1000;
    int minIndex = -1;
    for (int i = 0; i < spaceCount_; i++) {
        if (status::HaveEquipment::getAbsoluteValue(spaceArray_[i]) < min) {
            min = status::HaveEquipment::getAbsoluteValue(spaceArray_[i]);
            minIndex = i;
        }
    }
    if (minIndex != -1) {
        spacePos_ = spaceArray_[minIndex];
        spaceWidth_ = width;
    } else {
        spacePos_ = 0;
        spaceWidth_ = 0;
    }
}

// unused on DS (dead-stripped by the linker), only its data remains
THUMB const char* btl::BattleMonsterDraw2::printArrayPos(int index)
{
    return animName[index];
}

THUMB bool btl::BattleMonsterDraw2::isCallFriend(int monsterIndex)
{
    searchArrayPos(monsterIndex);
    if (spaceWidth_) {
        return true;
    }
    return false;
}

THUMB bool btl::BattleMonsterDraw2::isAppearKingSlime2()
{
    bool ret = false;
    for (int i = 0; i < 12; i++) {
        if (monsters_[i].isAppearKingSlime2()) {
            ret = true;
        }
    }
    return ret;
}

THUMB void btl::BattleMonsterDraw2::startAnimationWithLoop(int ctrl, int index, int loop)
{
    monsters_[ctrl].startAnimationWithLoop(index, loop);
}
