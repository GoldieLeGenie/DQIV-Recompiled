#include "ov006/BookEffectGroup.hpp"

#pragma profile on

THUMB book::BookEffectGroup::BookEffectGroup()
{
    effectSimple_[FIRST] = NULL;
    effectSimple_[SECOND] = NULL;
    flag_.clear();
    state_.clear();
}

THUMB book::BookEffectGroup::~BookEffectGroup()
{
}

THUMB void book::BookEffectGroup::draw()
{
    if (((flag_.flag_ & FLAG_FIRST_DRAW) ? true : false) && !((state_.flag_ & FLAG_FIRST_CAMERA) ? true : false)) {
        effectSimple_[FIRST]->draw();
    }
    if (((flag_.flag_ & FLAG_SECOND_DRAW) ? true : false) && !((state_.flag_ & FLAG_SECOND_CAMERA) ? true : false)) {
        effectSimple_[SECOND]->draw();
    }
}

THUMB void book::BookEffectGroup::start()
{
    flag_.flag_ |= FLAG_FIRST_DRAW;
    effectSimple_[FIRST]->start();
    if (effectSimple_[SECOND] != NULL) {
        flag_.flag_ |= FLAG_SECOND_DRAW;
        effectSimple_[SECOND]->start();
    }
}

THUMB void book::BookEffectGroup::addEffect(cmn::CommonEffectData* data, int flag)
{
    cmn::CommonEffectSimple* effect;
    if (data->getEffectType() == 0) {
        effectFlat_.setup(data, 1);
        effect = &effectFlat_;
    } else {
        effectCubic_.setup(data, flag);
        effect = &effectCubic_;
    }
    effectSimple_[regist_] = effect;
    regist_++;
}

THUMB void book::BookEffectGroup::cleanup(int flag)
{
    effectFlat_.cleanup(1);
    effectCubic_.cleanup(flag);
    effectSimple_[FIRST] = NULL;
    effectSimple_[SECOND] = NULL;
    flag_.clear();
    state_.clear();
    regist_ = 0;
}

THUMB void book::BookEffectGroup::setScale(dss::Fix32 scale)
{
    if (effectSimple_[FIRST] != NULL) {
        effectSimple_[FIRST]->setScale(scale);
    }
    if (effectSimple_[SECOND] != NULL) {
        effectSimple_[SECOND]->setScale(scale);
    }
}

THUMB void book::BookEffectGroup::setDisplayType(int type, int index)
{
    cmn::CommonEffectSimple* effect = effectSimple_[index];
    if (effect == NULL) {
        return;
    }
    if (type == DISPLAY_CENTRAL) {
        effect->getType();
        effectSimple_[index]->setDisplayType(1);
    } else {
        effect->setDisplayType(3);
    }
    if (type == DISPLAY_CENTRAL) {
        if (index == FIRST) {
            state_.flag_ |= FLAG_FIRST_CAMERA;
        } else {
            state_.flag_ |= FLAG_SECOND_CAMERA;
        }
    } else {
        if (index == FIRST) {
            state_.flag_ &= ~FLAG_FIRST_CAMERA;
        } else {
            state_.flag_ &= ~FLAG_SECOND_CAMERA;
        }
    }
}

THUMB void book::BookEffectGroup::setPosition(dss::Fix32Vector3& position)
{
    if (effectSimple_[FIRST] != NULL) {
        effectSimple_[FIRST]->setPosition(position);
    }
    if (effectSimple_[SECOND] != NULL) {
        effectSimple_[SECOND]->setPosition(position);
    }
}

THUMB bool book::BookEffectGroup::isEnable()
{
    if (effectSimple_[FIRST] != NULL) {
        return true;
    }
    return false;
}

THUMB bool book::BookEffectGroup::isEnd()
{
    bool end = true;
    if (effectSimple_[FIRST] != NULL) {
        if (!effectSimple_[FIRST]->isEnd()) {
            end = false;
        } else {
            flag_.flag_ &= ~FLAG_FIRST_DRAW;
        }
    }
    if (effectSimple_[SECOND] != NULL) {
        if (!effectSimple_[SECOND]->isEnd()) {
            end = false;
        } else {
            flag_.flag_ &= ~FLAG_SECOND_DRAW;
        }
    }
    return end;
}
