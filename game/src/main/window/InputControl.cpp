#include "main/window/InputControl.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/menu/MenuAPI.hpp"
#include "main/cmn/PlayerManager.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov001/fld/FieldPlayerManager.hpp"

int window::InputControl::next_ = window::PHASE_NORMAL;
int window::InputControl::prev_ = window::PHASE_NORMAL;
dss::BitFlag<unsigned char>* window::InputControl::icon_;
dss::BitFlag<unsigned int>* window::InputControl::permit_;

ARM window::InputControl::InputControl()
{
}

ARM window::InputControl::~InputControl()
{
}

ARM void window::InputControl::initialize(dss::BitFlag<unsigned int>* permit, dss::BitFlag<unsigned char>* icon)
{
    permit_ = permit;
    icon_ = icon;
    next_ = PHASE_NORMAL;
}

ARM void window::InputControl::playerLock(bool flag)
{
    if (data_0210bb94.unkfunc_02058114(0xc)) {
        TownPlayerManager::getSingleton()->setLock(flag);
    }
    if (data_0210bb94.unkfunc_02058114(0xe)) {
        FieldPlayerManager::getSingleton()->setLock(flag);
    }
}

ARM bool window::InputControl::isPlayerLock()
{
    if (data_0210bb94.unkfunc_02058114(0xc)) {
        return TownPlayerManager::getSingleton()->isLock();
    }
    if (!data_0210bb94.unkfunc_02058114(0xe)) {
        return true;
    }
    return FieldPlayerManager::getSingleton()->isLock();
}

ARM void window::InputControl::setupIcon()
{
    data_020ed11c.unkfunc_020273a8(((icon_->flag_ & 1) ? true : false), ((icon_->flag_ & 2) ? true : false), ((icon_->flag_ & 4) ? true : false), ((icon_->flag_ & 8) ? true : false));
}

ARM void window::InputControl::setNextPhase(int phase)
{
    next_ = phase;
    prev_ = getPhase();
}

ARM bool window::InputControl::goNext(int phase)
{
    if (!permit_->check(phase)) {
        return false;
    }
    setNextPhase(phase);
    return true;
}

ARM bool window::InputControl::isNext()
{
    return next_ != getPhase();
}
