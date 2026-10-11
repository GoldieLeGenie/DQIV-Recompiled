#include "ov016/status/FukuroItemInfo.hpp"
#include "ov016/status/PlayerItemInfo.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StageStatus.hpp"

THUMB int status::FukuroItemInfo::getPageMax()
{
    return (status::FukuroItemInfo::getItemMaxCount() - 1) / 6 + 1;
}

THUMB int status::FukuroItemInfo::getItemMaxCount()
{
    return status::g_Party.haveItemSack_.getCount();
}

THUMB int status::FukuroItemInfo::getPageItemCount(int page)
{
    getPageMax();
    return dss::max<int>(dss::min<int>(status::FukuroItemInfo::getItemMaxCount() - page * 6, 6), 0);
}

THUMB int status::FukuroItemInfo::getItemId(int page, int index)
{
    return status::g_Party.haveItemSack_.getItem(getIndexToAll(page, index));
}

THUMB int status::FukuroItemInfo::getItemFukuroCommandFlag()
{
    int flag = 0;
    if (status::PlayerItemInfo::ableToShow()) {
        flag |= 2;
    }
    return flag;
}

THUMB int status::FukuroItemInfo::getIndexToAll(int page, int index)
{
    getItemMaxCount();
    return page + index * 6;
}

THUMB status::UseItem::ThrowType status::FukuroItemInfo::getItemThrowType(int page, int index)
{
    return status::UseItem::getThrowType(getItemId(page, index));
}

THUMB void status::FukuroItemInfo::throwFukuroItem(int index)
{
    status::g_Party.haveItemSack_.execThrow(index);
}

THUMB int status::FukuroItemInfo::useFukuroItem(status::UseActionParam& param, int index, int* execMessage, int* resultMessage, int target)
{
    int actor = 0;
    int count = 0;
    int item = status::g_Party.haveItemSack_.getItem(index);

    while (status::g_Party.getPlayerStatus(actor)->haveStatusInfo_.isDeath() == 1) {
        actor++;
        if (actor == status::g_Party.getCount()) {
            break;
        }
    }

    param.clear();
    param.actorHaveItemSack_ = &status::g_Party.haveItemSack_;
    param.actorCharacterStatus_ = status::g_Party.getPlayerStatus(actor);
    if (target == -1) {
        status::g_Party.setBattleMode();
        int targetCount = 0;
        for (int i = 0; i < status::g_Party.getCount(); i++) {
            if (!status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath()) {
                param.targetCharacterStatus_[targetCount] = status::g_Party.getPlayerStatus(i);
                targetCount++;
            }
        }
        param.targetCount_ = targetCount;
        status::g_Party.setPlayerMode();
    } else {
        param.targetCount_ = 1;
        param.targetCharacterStatus_[0] = status::g_Party.getPlayerStatus(target);
    }
    param.itemSortIndex_ = index;
    status::UseItem::execUse(&param);

    status::UseActionMessage* message = &param.message_[0];
    for (int j = 0; message->execMessage_[j] != 0; j++) {
        *execMessage++ = message->execMessage_[j];
        count++;
    }

    if (item != 0x85 || (item == 0x85 && g_Stage.isRulaDisable() == 1)) {
        for (count = 0; message->resultMessage_[count] != 0; count++) {
            resultMessage[count] = message->resultMessage_[count];
            if (item == 0x7e && param.result_ == 1) {
                resultMessage[count] = 0xc3d73;
            }
        }
        for (int j = 0; message->addMessage_[j] != 0; j++) {
            resultMessage[count] = message->addMessage_[j];
            count++;
        }
    }
    return param.result_;
}
