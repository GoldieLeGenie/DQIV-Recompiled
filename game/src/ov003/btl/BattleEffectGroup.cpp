#include "ov003/btl/BattleEffectGroup.hpp"

#pragma profile on
THUMB btl::BattleEffectGroup::BattleEffectGroup()
{
    effectSimple_[0] = 0;
    effectSimple_[1] = 0;
    flag_.flag_ = 0;
    state_.flag_ = 0;
}

THUMB btl::BattleEffectGroup::~BattleEffectGroup()
{
}

THUMB void btl::BattleEffectGroup::draw()
{
    if ((flag_.flag_ & 1) && (state_.flag_ & 1) == 0) {
        effectSimple_[0]->draw();
    }
    if ((flag_.flag_ & 2) && (state_.flag_ & 2) == 0) {
        effectSimple_[1]->draw();
    }
}

THUMB void btl::BattleEffectGroup::extraDraw()
{
    if ((flag_.flag_ & 1) && (state_.flag_ & 1)) {
        effectSimple_[0]->draw();
    }
    if ((flag_.flag_ & 2) && (state_.flag_ & 2)) {
        effectSimple_[1]->draw();
    }
}

THUMB void btl::BattleEffectGroup::start()
{
    flag_.flag_ |= 1;
    effectSimple_[0]->start();
    if (effectSimple_[1] != 0) {
        flag_.flag_ |= 2;
        effectSimple_[1]->start();
    }
}

THUMB void btl::BattleEffectGroup::addEffect(cmn::CommonEffectData* data, int flag)
{
    cmn::CommonEffectSimple* effect;
    if (data->getEffectType() == 0) {
        effectFlat_.setup(data, 1);
        effect = &effectFlat_;
    }
    else {
        effectCubic_.setup(data, flag);
        effect = &effectCubic_;
    }
    effectSimple_[regist_] = effect;
    regist_++;
}

THUMB void btl::BattleEffectGroup::cleanup(int flag)
{
    effectFlat_.cleanup(1);
    effectCubic_.cleanup(flag);
    effectSimple_[0] = 0;
    effectSimple_[1] = 0;
    flag_.flag_ = 0;
    state_.flag_ = 0;
    regist_ = 0;
}

THUMB void btl::BattleEffectGroup::setScale(dss::Fix32 scale)
{
    if (effectSimple_[0] != 0) {
        effectSimple_[0]->setScale(scale);
    }
    if (effectSimple_[1] != 0) {
        effectSimple_[1]->setScale(scale);
    }
}

THUMB void btl::BattleEffectGroup::setDisplayType(int type, int index)
{
    cmn::CommonEffectSimple** effect = &effectSimple_[index];
    if (*effect == 0) {
        return;
    }

    if (type == 1) {
        (*effect)->getType();
        (*effect)->setDisplayType(1);
    }
    else {
        (*effect)->setDisplayType(3);
    }

    if (type == 1) {
        if (index == 0) {
            state_.flag_ |= 1;
        }
        else {
            state_.flag_ |= 2;
        }
    }
    else {
        if (index == 0) {
            state_.flag_ &= ~1;
        }
        else {
            state_.flag_ &= ~2;
        }
    }
}

THUMB void btl::BattleEffectGroup::setPosition(dss::Fix32Vector3& position)
{
    if (effectSimple_[0] != 0) {
        effectSimple_[0]->setPosition(position);
    }
    if (effectSimple_[1] != 0) {
        effectSimple_[1]->setPosition(position);
    }
}

THUMB bool btl::BattleEffectGroup::isEnable()
{
    return effectSimple_[0] != 0;
}

THUMB int btl::BattleEffectGroup::isEnd()
{
    int end = 1;
    if (effectSimple_[0] != 0) {
        if (!effectSimple_[0]->isEnd()) {
            end = 0;
        }
        else {
            flag_.flag_ &= ~1;
        }
    }
    if (effectSimple_[1] != 0) {
        if (!effectSimple_[1]->isEnd()) {
            end = 0;
        }
        else {
            flag_.flag_ &= ~2;
        }
    }
    return end;
}
