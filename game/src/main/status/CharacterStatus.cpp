#include "main/status/CharacterStatus.hpp"


THUMB  status::CharacterStatus::CharacterStatus(){
    this->characterIndex_ = -1;
    this->menuStatusFlag_.flag_ = 0;
    this->damageAnimation_ = NoAnim;
    this->damageAnimationMulti_ = NoAnim;
    this->damageAnimationFlag_.flag_ = 0;
    this->type_.flag_ = 0;
}


THUMB status::CharacterStatus::~CharacterStatus(){

}

THUMB int status::CharacterStatus::isActionEnable()
{
    return haveStatusInfo_.isActionEnable();
}

THUMB void status::CharacterStatus::setup(CharacterType type, int group, int index)
{
    if (type == MONSTER)
    {
        this->haveStatusInfo_.setup(index, 0);
        this->haveStatusInfo_.haveBattleStatus_ = &this->haveBattleStatus_;
        this->haveBattleStatus_.setup(MONSTER, group, index);
        this->haveBattleStatus_.haveStatusInfo_ = &this->haveStatusInfo_;
    }
    this->menuStatusFlag_.flag_ = 0;
}

THUMB void status::CharacterStatus::setMenuStatusFlag(status::HaveStatusInfo::DiffStatus status)
{
    this->menuStatusFlag_.flag_ = 0;
    this->menuStatusFlag_.flag_ |= 1 << status;
}

THUMB bool status::CharacterStatus::isMenuStatusFlag(status::HaveStatusInfo::DiffStatus status)
{
    return this->menuStatusFlag_.flag_ & (1 << status);
}

THUMB void status::CharacterStatus::clearMenuStatusFlag()
{
    this->menuStatusFlag_.flag_ = 0;
    return;
}


THUMB void status::CharacterStatus::setNoneAnimation() {
    this->damageAnimationFlag_.flag_ = 0;
    this->damageAnimation_ = NoAnim;
    this->damageAnimationMulti_ = NoAnim;
    return;
}

THUMB void status::CharacterStatus::setDamageAnimation()
{
    this->damageAnimationFlag_.flag_ = this->damageAnimationFlag_.flag_ | 1;
    return;
}


THUMB void status::CharacterStatus::clearDamageAnimation()
{
    this->damageAnimationFlag_.flag_ = this->damageAnimationFlag_.flag_ & ~1;
    return;
}

THUMB bool status::CharacterStatus::isDamageAnimation()
{
    return (this->damageAnimationFlag_.flag_ & 1);
}

THUMB void status::CharacterStatus::setAddDamageAnimation()
{
    this->damageAnimationFlag_.flag_ = this->damageAnimationFlag_.flag_ | 256;
    return;
}

THUMB void status::CharacterStatus::setDeathAnimation()
{
    this->damageAnimationFlag_.flag_ = this->damageAnimationFlag_.flag_ | 2;
    return;
}


THUMB void status::CharacterStatus::clearDeathAnimation()
{
    this->damageAnimationFlag_.flag_ = this->damageAnimationFlag_.flag_ & ~2;
    return;
}


THUMB bool status::CharacterStatus::isDeathAnimation()
{
    if (this->damageAnimationFlag_.flag_ & 2) {
        return 1;
    }
    return 0;
}

THUMB void status::CharacterStatus::setDisappearAnimation()

{
    this->damageAnimationFlag_.flag_ = this->damageAnimationFlag_.flag_ | 4;
    return;
}

THUMB void status::CharacterStatus::clearDisappearAnimation()

{
    this->damageAnimationFlag_.flag_ &= ~4;
    return;
}


THUMB bool status::CharacterStatus::isDisappearAnimation()

{
    if (this->damageAnimationFlag_.flag_ & 4) {
        return 1;
    }
    return 0;
}

THUMB void status::CharacterStatus::setMultiDamageAnimation1()
{
    this->damageAnimationFlag_.flag_ = this->damageAnimationFlag_.flag_ | 8;
    return;
}

THUMB void status::CharacterStatus::clearMultiDamageAnimation1() {
    this->damageAnimationFlag_.flag_ = this->damageAnimationFlag_.flag_ & ~8;
    return;
}

THUMB bool status::CharacterStatus::isMultiDamageAnimation1()

{
    if (this->damageAnimationFlag_.flag_ & 8) {
        return 1;
    }
    return 0;
}


THUMB void status::CharacterStatus::setMultiDamageAnimation2()

{
    this->damageAnimationFlag_.flag_ = this->damageAnimationFlag_.flag_ | 16;
    return;
}


THUMB void status::CharacterStatus::clearMultiDamageAnimation2()

{
    this->damageAnimationFlag_.flag_ = this->damageAnimationFlag_.flag_ & ~16;
    return;
}


THUMB bool status::CharacterStatus::isMultiDamageAnimation2()

{
    if (this->damageAnimationFlag_.flag_ & 16) {
        return 1;
    }
    return 0;
}


THUMB void status::CharacterStatus::setMultiDamageAnimation()

{
    this->damageAnimationFlag_.flag_ = this->damageAnimationFlag_.flag_ | 32;
    return;
}


THUMB void status::CharacterStatus::clearMultiDamageAnimation()

{
    this->damageAnimationFlag_.flag_ = this->damageAnimationFlag_.flag_ & ~32;
    return;
}


THUMB bool status::CharacterStatus::isMultiDamageAnimation()

{
    if (this->damageAnimationFlag_.flag_ & 32) {
        return 1;
    }
    return 0;
}


THUMB void status::CharacterStatus::setMultiDamageAnimation2nd()

{
    this->damageAnimationFlag_.flag_ = this->damageAnimationFlag_.flag_ | 64;
    return;
}


THUMB bool status::CharacterStatus::isMultiDamageAnimation2nd()

{
    if (this->damageAnimationFlag_.flag_ & 64) {
        return 1;
    }
    return 0;
}


THUMB void status::CharacterStatus::setEscapeAnimation()

{
    this->damageAnimationFlag_.flag_ = this->damageAnimationFlag_.flag_ | 128;
    return;
}


THUMB void status::CharacterStatus::clearEscapeAnimation()

{
    this->damageAnimationFlag_.flag_ = this->damageAnimationFlag_.flag_ & ~128;
    return;
}


THUMB bool status::CharacterStatus::isEscapeAnimation()

{
    if (this->damageAnimationFlag_.flag_ & 128) {
        return 1;
    }
    return 0;
}


THUMB void status::CharacterStatus::setRebirthAnimation()

{
    this->damageAnimationFlag_.flag_ = this->damageAnimationFlag_.flag_ | 512;
    return;
}


THUMB void status::CharacterStatus::clearRebirthAnimation()

{
    this->damageAnimationFlag_.flag_ = this->damageAnimationFlag_.flag_ & ~512;
    return;
}


THUMB bool status::CharacterStatus::isRebirthAnimation()

{
    if (this->damageAnimationFlag_.flag_ & 512){
        return 1;
    }
    return 0;
}

ARM void status::CharacterStatus::printAction()
{
    return;
}

ARM void status::CharacterStatus::print()
{
    return;
}









