#include "ov003/btl/BattleTurn.hpp"


#pragma profile on

THUMB btl::BattleTurn::BattleTurn()
{
    flag_.flag_ = 0;
}

THUMB btl::BattleTurn::~BattleTurn()
{
}

THUMB void btl::BattleTurn::setBattleActor2(btl::BattleActor2* battleActor)
{
    battleActor_ = battleActor;
    flag_.flag_ = 0;
}

THUMB void btl::BattleTurn::execute()
{
    if (!(flag_.flag_ & 1)) {
        setup();
        flag_.flag_ |= 1;
    }

    if (!(flag_.flag_ & 2)) {
        exec();
        if (checkEnd()) {
            cleanup();
            flag_.flag_ |= 2;
        }
    }
}


THUMB int btl::BattleTurn::isEnd()
{
    if (flag_.flag_ & 2) {
        return 1;
    }
    return 0;
}

THUMB int btl::BattleTurn::checkEnd()
{
    if (battleActor_->isEnd() && isBattleActor2End()) {
        return 1;
    }
    return 0;
}

THUMB void btl::BattleTurn::setup()
{
    battleActor_->setup();
}

THUMB void btl::BattleTurn::cleanup()
{
    flag_.flag_ = 0;
}

THUMB void btl::BattleTurn::exec()
{
    battleActor_->execute();
}


THUMB int btl::BattleTurn::isBattleActor2End()
{
    if (battleActor_->isEnd()) {
        return 1;
    }
    if (!battleActor_->isActionEnable()) {
        return 1;
    }
    return 0;
}

THUMB void btl::BattleTurn::reattack()
{
    flag_.flag_ = 0;
    battleActor_->reattack();
}

THUMB void btl::BattleTurn::change()
{
    flag_.flag_ = 0;
    battleActor_->change();
}

THUMB void btl::BattleTurn::tarot()
{
    flag_.flag_ = 0;
    battleActor_->tarot();
}

THUMB void btl::BattleTurn::parupunte()
{
    flag_.flag_ = 0;
    battleActor_->parupunte();
}


