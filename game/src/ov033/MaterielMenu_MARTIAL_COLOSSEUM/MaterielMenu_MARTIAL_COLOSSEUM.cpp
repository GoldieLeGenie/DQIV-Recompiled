#include "ov033/MaterielMenu_MARTIAL_COLOSSEUM/MaterielMenu_MARTIAL_COLOSSEUM.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/MenuDataCommon.hpp"
#include "main/text/TextAPI.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/UseActionParam.hpp"
#include "main/status/UseItem.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"

THUMB void MaterielMenu_MARTIAL_COLOSSEUM::menuSetup()
{
    status::g_Party.setPlayerMode();
    activeChara_ = status::g_Party.getSortIndex(4);
    int sortIndex = status::g_Party.getSortIndex(4);
    MaterielMenuPlayerControl::getSingleton()->activeChara_ = sortIndex;
    itemIndex_ = 0;
    mode_ = 0;
    wins_ = MaterielMenuPlayerControl::getSingleton()->wins_;
    showMessage(0x4296, -1);
    data_020ed1bc.setYesNo();
}

THUMB void MaterielMenu_MARTIAL_COLOSSEUM::menuDraw()
{
    unkfunc_0216fdc4();
}

THUMB void MaterielMenu_MARTIAL_COLOSSEUM::menuUpdate()
{
    if (data_020ed1bc.isOpen()) {
        if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
            data_020ed1bc.close();
            selectYes();
        } else if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            data_020ed1bc.close();
            selectNo();
        }
    }
}

THUMB void MaterielMenu_MARTIAL_COLOSSEUM::selectYes()
{
    switch (mode_) {
    case 0:
        checkHaveYakusou();
        break;
    case 1:
        selectNo();
        break;
    case 2:
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
        break;
    }
}

THUMB void MaterielMenu_MARTIAL_COLOSSEUM::selectNo()
{
    if (wins_ < 5) {
        TextAPI::setMACRO0(0xf, 0x60000000, menu::MenuDataCommon::getOpponent(wins_));
        showMessage(0x42a2, -1);
    } else {
        MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
    }
    mode_ = 2;
}

THUMB void MaterielMenu_MARTIAL_COLOSSEUM::checkHaveYakusou()
{
    int i;
    status::HaveItem& haveItem = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_;
    for (i = 0; i < haveItem.getCount(); i++) {
        if (haveItem.getItem(i) == 0x6f) {
            itemIndex_ = i;
            checkUseYakusou();
            return;
        }
    }
    showMessage(0x429c, -1);
    mode_ = 1;
}

THUMB void MaterielMenu_MARTIAL_COLOSSEUM::checkUseYakusou()
{
    status::HaveStatusInfo& statusInfo = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_;
    if (statusInfo.getHpMax() != statusInfo.getHp()) {
        useYakusou();
        return;
    }
    showMessage(0x4299, -1);
    mode_ = 1;
}

THUMB void MaterielMenu_MARTIAL_COLOSSEUM::useYakusou()
{
    int playerIndex = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveStatus_.playerIndex_;
    data_020ed1bc.openMessageForTALK();
    status::UseActionParam useActionParam;
    useActionParam.clear();
    useActionParam.actorCharacterStatus_ = status::g_Party.getPlayerStatus(activeChara_);
    useActionParam.targetCount_ = 1;
    useActionParam.targetCharacterStatus_[0] = status::g_Party.getPlayerStatus(activeChara_);
    useActionParam.itemSortIndex_ = itemIndex_;
    status::UseItem::execUse(&useActionParam);
    status::UseActionMessage& message = useActionParam.message_[0];
    TextAPI::setMACRO0(0xa, 0x40000000, 0x6f);
    TextAPI::setMACRO0(1, 0x50000000, playerIndex);
    TextAPI::setMACRO0(0x12, 0x50000000, playerIndex);
    data_020ed1bc.addMessage(message.execMessage_[0], message.resultMessage_[0], 0x429f);
    data_020ed1bc.setYesNo();
}

THUMB void MaterielMenu_MARTIAL_COLOSSEUM::showMessage(int messageID1, int messageID2)
{
    data_020ed1bc.openMessageForTALK();
    data_020ed1bc.addMessage(messageID1);
    if (messageID2 != -1) {
        data_020ed1bc.addMessage(messageID2);
    }
}

ARM void MaterielMenu_MARTIAL_COLOSSEUM::menuExecute()
{
}
