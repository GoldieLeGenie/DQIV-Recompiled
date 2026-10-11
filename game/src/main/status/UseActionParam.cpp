#include "main/status/UseActionParam.hpp"


THUMB void status::UseActionParam::clear()
{
    this->actorCharacterStatus_ = 0;
    this->actorHaveItemSack_ = 0;

    for (int i = 0; i < 18; i++) {
        this->targetCharacterStatus_[i] = 0;
    }

    for (int i = 0; i < 18; i++) {
        this->bakTargetCharacterStatus_[i] = 0;
    }

    for (int i = 0; i < 18; i++) {
        this->originalTargetCharacterStatus_[i] = 0;
    }

    for (int i = 0; i < 18; i++) {
        this->targetResult_[i] = 0;
    }

    this->targetCount_ = 0;
    this->damage_ = 0;
    this->currentTargetIndex_ = 0;

    clearMessage();

    this->itemIndex_ = 0;
    this->itemSortIndex_ = 0;
    this->result_ = 0;
    this->exec_ = 0;
}

THUMB void status::UseActionParam::clearMessage() {
    for (int i = 0; i < 0x12; i++) {
        this->message_[i].clear();
    }
    return;
}