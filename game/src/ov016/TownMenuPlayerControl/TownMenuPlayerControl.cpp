#include "ov016/TownMenuPlayerControl/TownMenuPlayerControl.hpp"
#include "ov016/status/PlayerItemInfo.hpp"
#include "ov016/status/FukuroItemInfo.hpp"
#include "main/status/PartyStatus.hpp"

TownMenuPlayerControl gTownMenuPlayerControl;

THUMB TownMenuPlayerControl* TownMenuPlayerControl::getSingleton()
{
    return &gTownMenuPlayerControl;
}

THUMB void TownMenuPlayerControl::initialize()
{
    if (initializeLock_ == 1) {
        initializeLock_ = 0;
        return;
    }
    activeChara_ = 0;
    activeCharaIndex_ = 0;
    activeItem_ = 0;
    activeItemPage_ = 0;
    activeFukuro_ = 0;
    targetChara_ = 0;
    targetItem_ = 0;
    targetItemPage_ = 0;
    targetFukuro_ = 0;
    activeCommand_ = 0;
    activeMagic_ = 0;
    activeMagicID_ = 0;
    activeTactics_ = 0;
    actorIndex_ = 0;
    targetIndex_ = 0;
    takanomeX_ = 0;
    takanomeY_ = 0;
}

THUMB unsigned char TownMenuPlayerControl::getActiveItemIndexToAll()
{
    if (activeFukuro_) {
        return status::FukuroItemInfo::getIndexToAll(activeItem_, activeItemPage_);
    } else {
        int index = activeItem_ + activeItemPage_ * 6;
        int maxCount = status::PlayerItemInfo::getItemMaxCount(activeChara_);
        return index;
    }
}

THUMB unsigned char TownMenuPlayerControl::getTargetItemIndexToAll()
{
    if (targetFukuro_) {
        return status::FukuroItemInfo::getIndexToAll(targetItem_, targetItemPage_);
    } else {
        int index = targetItem_ + targetItemPage_ * 6;
        int maxCount = status::PlayerItemInfo::getItemMaxCount(targetChara_);
        return index;
    }
}

THUMB void TownMenuPlayerControl::setActiveCommand(unsigned char activeCommand)
{
    activeCommand_ = activeCommand;
    if (activeFukuro_) {
        if (status::PlayerItemInfo::ableToShow() && activeCommand > 2) {
            activeCommand_ += 1;
        } else if (activeCommand > 2) {
            activeCommand_ += 2;
        }
        return;
    }
    if (!status::PlayerItemInfo::ableToEqip(activeChara_, activeItem_ + activeItemPage_ * 6) && !status::PlayerItemInfo::ableToShow()) {
        if (activeCommand > 2) {
            activeCommand_ += 2;
        }
    } else if (!status::PlayerItemInfo::ableToEqip(activeChara_, activeItem_ + activeItemPage_ * 6)) {
        if (activeCommand > 2) {
            activeCommand_ += 1;
        }
    } else if (!status::PlayerItemInfo::ableToShow()) {
        if (activeCommand > 3) {
            activeCommand_ += 1;
        }
    }
}

THUMB unsigned char TownMenuPlayerControl::getActiveCommand()
{
    unsigned char command = activeCommand_;
    int show = 0;
    if (status::PlayerItemInfo::ableToShow()) {
        show = 1;
    }
    switch (command) {
    case 4:
        if (activeFukuro_ == 1 || !status::PlayerItemInfo::ableToEqip(activeChara_, getActiveItemIndexToAll())) {
            command -= 1;
        }
        break;
    case 5:
        if (activeFukuro_ == 1) {
            if (show == 1) {
                command -= 1;
            } else {
                command -= 2;
            }
        } else if (show == 0 && !status::PlayerItemInfo::ableToEqip(activeChara_, getActiveItemIndexToAll())) {
            command -= 2;
        } else if (show == 0 || !status::PlayerItemInfo::ableToEqip(activeChara_, getActiveItemIndexToAll())) {
            command -= 1;
        }
        break;
    }
    return command;
}

THUMB unsigned char TownMenuPlayerControl::getDrawCharaCount(TOWN_MENU menuType)
{
    if (menuType == TOWN_MENU_ITEM && status::g_Party.fukuro_) {
        return status::g_Party.getCount() + 1;
    }
    return status::g_Party.getCount();
}

THUMB void TownMenuPlayerControl::setPlayerActiveItemByChangeMax()
{
    status::g_Party.setPlayerMode();
    int maxCount = status::PlayerItemInfo::getItemMaxCount(activeChara_);
    if (activeItem_ + activeItemPage_ * 6 >= maxCount) {
        activeItem_--;
        if (activeItemPage_ == 1 && activeItem_ < 0) {
            activeItemPage_ = 0;
            activeItem_ = 5;
        }
        if (activeItem_ < 0) {
            activeItemPage_ = 0;
            activeItem_ = 0;
        }
    }
}

THUMB void TownMenuPlayerControl::setFukuroActiveItemByChangeMax()
{
    int pageMax = status::FukuroItemInfo::getPageMax();
    if (activeItemPage_ >= pageMax) {
        activeItemPage_ = pageMax - 1;
        activeItem_ = (unsigned char)(status::FukuroItemInfo::getPageItemCount(activeItemPage_) - 1);
        return;
    }
    if (activeItem_ >= status::FukuroItemInfo::getPageItemCount(activeItemPage_)) {
        activeItem_--;
        if (activeItem_ < 0) {
            activeItemPage_--;
            activeItem_ = 5;
            if (activeItemPage_ < 0) {
                activeItemPage_ = 0;
                activeItem_ = 0;
            }
        }
    }
}

THUMB void TownMenuPlayerControl::setPlayerTargetItemByChangeMax()
{
    status::g_Party.setPlayerMode();
    int maxCount = status::PlayerItemInfo::getItemMaxCount(targetChara_);
    if (targetItem_ + targetItemPage_ * 6 >= maxCount) {
        targetItem_--;
        if (targetItemPage_ == 1 && targetItem_ < 0) {
            targetItemPage_ = 0;
            targetItem_ = 5;
        }
        if (targetItem_ < 0) {
            targetItemPage_ = 0;
            targetItem_ = 0;
        }
    }
}

THUMB void TownMenuPlayerControl::setFukuroTargetItemByChangeMax()
{
    int pageMax = status::FukuroItemInfo::getPageMax();
    if (targetItemPage_ >= pageMax) {
        targetItemPage_ = pageMax - 1;
        targetItem_ = (unsigned char)(status::FukuroItemInfo::getPageItemCount(targetItemPage_) - 1);
        return;
    }
    if (targetItem_ >= status::FukuroItemInfo::getPageItemCount(targetItemPage_)) {
        targetItem_--;
        if (targetItem_ < 0) {
            targetItemPage_--;
            targetItem_ = 5;
            if (targetItemPage_ < 0) {
                targetItemPage_ = 0;
                targetItem_ = 0;
            }
        }
    }
}

THUMB void TownMenuPlayerControl::setTakanome(char x, char y)
{
    takanomeX_ = x;
    takanomeY_ = y;
}
