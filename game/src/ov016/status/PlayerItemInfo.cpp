#include "ov016/status/PlayerItemInfo.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StageStatus.hpp"

THUMB int status::PlayerItemInfo::getItemIndex(int player, int index)
{
    return status::g_Party.getPlayerStatus(player)->haveStatusInfo_.haveItem_.getItem(index);
}

THUMB int status::PlayerItemInfo::getItemMaxCount(int player)
{
    return status::g_Party.getPlayerStatus(player)->haveStatusInfo_.haveItem_.getCount();
}

THUMB bool status::PlayerItemInfo::ableToShow()
{
    int count = status::g_Party.getCount();
    for (int i = 0; i < count; i++) {
        if (status::g_Party.getPlayerIndex(i) == 7) {
            return true;
        }
    }
    return false;
}

THUMB bool status::PlayerItemInfo::ableToEqip(int player, int index)
{
    int item = status::g_Party.getPlayerStatus(player)->haveStatusInfo_.haveItem_.getItem(index);
    return status::g_Party.getPlayerStatus(player)->haveStatusInfo_.isEquipEnable(item);
}

THUMB int status::PlayerItemInfo::getItemPlayerCommandFlag(int player, int index)
{
    int flag = 0;
    if (ableToEqip(player, index)) {
        flag |= 1;
    }
    if (ableToShow()) {
        flag |= 2;
    }
    return flag;
}

THUMB int status::PlayerItemInfo::getEquipItemIdByType(int player, ItemType type)
{
    return status::g_Party.getPlayerStatus(player)->haveStatusInfo_.haveEquipment_.getEquipment(type);
}

THUMB status::UseItem::ThrowType status::PlayerItemInfo::getItemThrowType(int player, int index)
{
    return status::UseItem::getThrowType(status::g_Party.getPlayerStatus(player)->haveStatusInfo_.haveItem_.getItem(index));
}

THUMB void status::PlayerItemInfo::throwCharaItem(int player, int index)
{
    status::g_Party.getPlayerStatus(player)->haveStatusInfo_.execThrow(index);
}

THUMB bool status::PlayerItemInfo::checkCurse(int player, int itemIndex)
{
    ItemType type = status::UseItem::getItemType(itemIndex);
    if (type <= 4) {
        int equip = getEquipItemIdByType(player, type);
        if (status::UseItem::isCurse(itemIndex) && itemIndex == equip && status::g_Party.getPlayerIndex(player) != 0x19) {
            return true;
        }
    }
    return false;
}

THUMB int status::PlayerItemInfo::usePlayerItem(status::UseActionParam& param, int player, int index, int* execMessage, int* resultMessage, int target)
{
    int count = 0;
    int item = getItemIndex(player, index);

    param.clear();
    param.actorCharacterStatus_ = status::g_Party.getPlayerStatus(player);
    if (target == -1) {
        if (item == 0x66) {
            param.targetCount_ = 1;
            param.targetCharacterStatus_[0] = status::g_Party.getPlayerStatus(player);
        } else {
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
        }
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
