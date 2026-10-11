#pragma ipa file
#include "ov016/UnkTownMenuDraw/UnkTownMenuDraw_0217d560.hpp"
#include "ov016/UnkTownMenuDraw/UnkTownMenuDraw_0217ad94.hpp"
#include "main/menu/MenuBase.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/UseItem.hpp"
#include "main/status/UseAction.hpp"
#include "main/menu/MenuManager.hpp"
#include "ov016/TownMenuPlayerControl/TownMenuPlayerControl.hpp"
#include "ov016/status/FukuroItemInfo.hpp"
#include "ov016/status/PlayerItemInfo.hpp"
#include "main/menu/UnkMenuPartsDraw.hpp"
#include "main/menu/UnkMenuCommonDraw_0201e194.hpp"

// not in the ROM (dead-stripped), keeps the const locals of this file
THUMB void unkfunc_unused_11()
{
    const int unk0 = -1;
    const int unk1 = 1;
    const int unk2 = -1;
    const int unk3 = 2;
    const int unk4 = 0;
    const int unk5 = -1;
    const int unk6 = -1;
}

THUMB void unkfunc_0217d560()
{
    if (!data_020ed1bc.isOpen()) {
        unkfunc_0217ad94(-1, -1);
        unkfunc_0217eb5c(0, 0, 1);
    }
    unkfunc_0201e260();
    unkfunc_0201e350(-1, -1, 0);
}

THUMB void unkfunc_0217d598(int chara)
{
    if (!data_020ed1bc.isOpen()) {
        unkfunc_0217e060(chara, 0, 1, 0);
        unkfunc_0217cea8(1);
        unkfunc_0217e144(1, 0);
    }
    unkfunc_0201e260();
    if (!data_020ed1bc.isOpen()) {
        unkfunc_0217e34c(chara, 0);
        unkfunc_0201e194(0, 0x50, 0x100, 0x70, -1);
    }
}

THUMB void unkfunc_0217d5f4(int chara, int itemID, int page)
{
    if (!data_020ed1bc.isOpen()) {
        unkfunc_0217e060(chara, 0, 1, 0);
        unkfunc_0217e388(chara, page, 0);
        unkfunc_0201e194(0, 0, 0x100, 0xa0, 0x70);
        unkfunc_0217b3b0(itemID, 0, 0);
        if (TownMenuPlayerControl::getSingleton()->activeFukuro_ != 0) {
            if (status::FukuroItemInfo::getItemMaxCount() > 6) {
                TownMenuPlayerControl* ctrl = TownMenuPlayerControl::getSingleton();
                unkfunc_0217b300(0xb8, status::FukuroItemInfo::getPageMax() - 1, ctrl->activeItemPage_, 1);
            }
        } else if (status::PlayerItemInfo::getItemMaxCount(chara) > 6) {
            unkfunc_0217b300(0xd0, 1, TownMenuPlayerControl::getSingleton()->activeItemPage_, 0);
        }
    }
    unkfunc_0201e260();
    unkfunc_0201e194(0, 0x78, 0xb0, 0x48, 0x90);
    switch (status::UseItem::getItemType(itemID)) {
    case -1:
        break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        unkfunc_0217b488(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, itemID, 0, 0);
        break;
    default:
        unkfunc_0217b40c(itemID);
        break;
    }
}

THUMB void unkfunc_0217d6e8(int chara, int itemID, int page, int sound)
{
    if (!data_020ed1bc.isOpen() && sound == 0) {
        int x = (TownMenuPlayerControl::getSingleton()->activeItem_ % 2) * 0x78 + 0x14;
        int y = (TownMenuPlayerControl::getSingleton()->activeItem_ / 2) * 32 + 0x14;
        int command;
        if (TownMenuPlayerControl::getSingleton()->activeFukuro_ != 0) {
            command = status::FukuroItemInfo::getItemFukuroCommandFlag();
        } else {
            command = status::PlayerItemInfo::getItemPlayerCommandFlag(chara, TownMenuPlayerControl::getSingleton()->getActiveItemIndexToAll());
        }
        unkfunc_0217e060(chara, 0, 1, 0);
        unkfunc_0217b5b0(command);
        unkfunc_0201e194(0, 0, 0x100, 0xa0, 0x68);
        unkfunc_0217e388(chara, page, 0);
        unkfunc_0217cce4(x, y + 4);
    }
    unkfunc_0201e260();
    if (!data_020ed1bc.isOpen() && sound == 0) {
        unkfunc_0201e194(0, 0x78, 0xb0, 0x48, 0x90);
        switch (status::UseItem::getItemType(itemID)) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            unkfunc_0217b488(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, itemID, 0, 0);
            break;
        default:
            unkfunc_0217b40c(itemID);
            break;
        }
    }
}

THUMB void unkfunc_0217d7f8(int active, int itemID, int page)
{
    status::g_Party.setBattleMode();
    if (!data_020ed1bc.isOpen()) {
        unkfunc_0217e060(active + page * 4, 0, 1, 1);
        unkfunc_0201e194(0, 0, 0x100, 0xa0, -1);
        unkfunc_0217e584(page);
        status::g_Party.setPlayerMode();
    }
    unkfunc_0201e260();
    if (!data_020ed1bc.isOpen()) {
        unkfunc_0217b40c(itemID);
        unkfunc_0201e194(0, 0x78, 0xb0, 0x48, 0x90);
    }
}

THUMB void unkfunc_0217d870(int activeChara, int targetChara, int itemID, int sound)
{
    if (!data_020ed1bc.isOpen() && sound == 0) {
        unkfunc_0217e060(activeChara, 0, 0, 0);
        unkfunc_0217eba4(0x88, 0xa0);
        unkfunc_0217b184(&status::g_Party.getPlayerStatus(targetChara)->haveStatusInfo_);
        unkfunc_0217ccf8(&status::g_Party.getPlayerStatus(targetChara)->haveStatusInfo_, itemID, 0);
        unkfunc_0217e144(1, 0);
        unkfunc_0201e194(0, 0, 0x100, 0x48, 0x28);
    }
    unkfunc_0217e34c(targetChara, 1);
    unkfunc_0201e260();
    unkfunc_0201e194(0, 0x50, 0x100, 0x70, -1);
}

THUMB void unkfunc_0217d900(int activeChara, int targetChara, int itemID, int page, int sound)
{
    if (!data_020ed1bc.isOpen() && sound == 0) {
        unkfunc_0217e060(activeChara, 0, 0, 0);
        unkfunc_0217eba4(0x88, 0xa0);
        unkfunc_0217b184(&status::g_Party.getPlayerStatus(targetChara)->haveStatusInfo_);
        unkfunc_0201e194(0, 0, 0x100, 0xa0, 0x70);
        unkfunc_0217e388(targetChara, page, 1);
        unkfunc_0217b3b0(0, 0, 1);
        if (status::PlayerItemInfo::getItemMaxCount(targetChara) + 1 > 6) {
            unkfunc_0217b300(0xd0, 1, TownMenuPlayerControl::getSingleton()->targetItemPage_, 0);
        }
    }
    unkfunc_0217ccf8(&status::g_Party.getPlayerStatus(targetChara)->haveStatusInfo_, itemID, 1);
    unkfunc_0201e194(0, 8, 0x100, 0x48, 0x30);
    unkfunc_0217e34c(targetChara, 1);
    unkfunc_0201e194(0, 0x50, 0x100, 0x70, -1);
}

THUMB void unkfunc_0217d9cc(int mp1, int mp2, int magicID, int chara, int mode)
{
    if (!data_020ed1bc.isOpen()) {
        status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(chara)->haveStatusInfo_;
        unkfunc_0217e0ac(chara);
        unkfunc_0217ba8c(info, mp1, mp2);
        if (mode == 0) {
            unkfunc_0217cea8(0);
            unkfunc_0217e144(0, 0);
            unkfunc_0217bae0(info);
            unkfunc_0201e194(0, 0x58, 0x100, 0x58, -1);
        } else {
            unkfunc_0217e5a0(chara);
            unkfunc_0217b3b0(magicID, 1, 0);
            unkfunc_0201e194(0, 0, 0x100, 0xa0, 0x70);
            unkfunc_0217cf1c(0);
        }
    }
    unkfunc_0201e260();
}

THUMB void unkfunc_0217da64(int mp, int useMp, int chara, int page)
{
    if (!data_020ed1bc.isOpen()) {
        unkfunc_0217e0ac(chara);
        unkfunc_0217ba8c(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, mp, useMp);
        unkfunc_0201e194(0, 0, 0x100, 0xa0, -1);
        unkfunc_0217e520(page);
    }
    unkfunc_0201e260();
    unkfunc_0201e350(-1, -1, 0);
}

THUMB void unkfunc_0217dac4(int mp, int useMp, int chara, int page, unsigned char* town)
{
    if (!data_020ed1bc.isOpen()) {
        if (mp != -1 && useMp != -1) {
            unkfunc_0217ad94(1, -1);
            unkfunc_0217eb5c(0x40, 0xa0, 0);
            unkfunc_0217ba8c(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, mp, useMp);
            unkfunc_0217eba4(0, 0xa0);
            unkfunc_0217b0c4(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, 0, chara, 0);
        } else {
            unkfunc_0217ad94(2, -1);
            unkfunc_0217eb5c(0x40, 0xa0, 0);
            unkfunc_0217eba4(0x88, 0xa0);
            unkfunc_0217b0c4(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, 1, chara, 0);
        }
        unkfunc_0201e194(0, 0, 0x100, 0xa0, 0x70);
        unkfunc_0217bbcc(page, town);
        unkfunc_0217cf1c(1);
    }
    unkfunc_0201e260();
}

THUMB void unkfunc_0217db80(int index, int page)
{
    unkfunc_0217e0e4(index, page);
    unkfunc_0217bc9c(&status::g_Party.getPlayerStatus(index)->haveStatusInfo_);
    unkfunc_0217bdb8(&status::g_Party.getPlayerStatus(index)->haveStatusInfo_);
    unkfunc_0201e194(0, 0, 0x98, 0x70, 0x18);
    unkfunc_0201e194(0x98, 0, 0x68, 0x70, -1);
    unkfunc_0217c290(&status::g_Party.getPlayerStatus(index)->haveStatusInfo_, 0);
    unkfunc_0201e194(0, 0x60, 0x98, 0x60, -1);
    unkfunc_0201e194(0x98, 0xa0, 0x68, 0x20, -1);
    unkfunc_0201e194(0x98, 0x80, 0x68, 0x20, -1);
}

THUMB void unkfunc_0217dc18(int index, int page)
{
    unkfunc_0217e0e4(index, page);
    unkfunc_0217c3e0(&status::g_Party.getPlayerStatus(index)->haveStatusInfo_, 0);
    unkfunc_0201e194(0, 0, 0x100, 0x70, 0x28);
    unkfunc_0217c3e0(&status::g_Party.getPlayerStatus(index)->haveStatusInfo_, 1);
    unkfunc_0201e194(0, 0, 0x100, 0xc0, 0x28);
}

THUMB void unkfunc_0217dc6c(int index, int type, int page)
{
    unkfunc_0217e0e4(index, page);
    unkfunc_0217e628(type);
}

THUMB void unkfunc_0217dc80(int page)
{
    unkfunc_0217e0e4(-2, page);
    unkfunc_0217c7f0();
    unkfunc_0201e194(0, 0, 0x100, 0x70, 0x28);
    unkfunc_0201e1c4(0, 0x50, 0x100);
}

THUMB void unkfunc_0217dcb0(int mode, char* command, int count, int chara)
{
    if (!data_020ed1bc.isOpen()) {
        unkfunc_0217ad94(5, -1);
        unkfunc_0217eb5c(0x40, 0xa0, 0);
        if (mode == 2) {
            unkfunc_0217ad94(5, -1);
            unkfunc_0217eb5c(0x40, 0xa0, 0);
            unkfunc_0217eba4(0x88, 0xa0);
            if (chara < 0) {
                unkfunc_0217b0c4(&status::g_Party.getPlayerStatus(0)->haveStatusInfo_, 1, chara, 0);
            } else {
                unkfunc_0217b0c4(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, 1, chara, 0);
            }
            status::g_Party.setBattleMode();
            char order[12];
            char n = 0;
            for (char i = 0; i < status::g_Party.getCount(); i++) {
                status::PlayerStatus* player = status::g_Party.getPlayerStatus(i);
                if (player->haveStatusInfo_.haveStatus_.isPlayer() != false) {
                    order[n] = i;
                    n++;
                }
            }
            order[n] = -1;
            unkfunc_0217e7bc(order, mode);
        } else {
            unkfunc_0217e6e8(mode, command, count);
        }
    }
    unkfunc_0201e260();
    if (chara != -2 && mode == 2) {
        unkfunc_0217e34c(chara, 0);
        unkfunc_0201e194(0, 0x50, 0x100, 0x70, -1);
    }
}

THUMB void unkfunc_0217ddc8(char* chara, int index)
{
    unkfunc_0217ad94(5, -1);
    unkfunc_0217eb5c(0x40, 0xa0, 0);
    unkfunc_0217eba4(0x88, 0xa0);
    if (index < 0) {
        unkfunc_0217b0c4(&status::g_Party.getPlayerStatus(0)->haveStatusInfo_, 1, index, 0);
    } else {
        unkfunc_0217b0c4(&status::g_Party.getPlayerStatus(index)->haveStatusInfo_, 1, index, 0);
    }
    unkfunc_0217e7bc(chara, -1);
    unkfunc_0217e994(chara);
}

THUMB void unkfunc_0217de2c(char* chara, int index, int active)
{
    int x = (active % 5) * 0x28 + 0x14;
    int y = (active / 5) * 0x28 + 0x64;
    int count = status::g_Party.getCount();
    if (status::g_Party.getSortIndex(1) != -1 || status::g_Party.getSortIndex(2) != -1) {
        count--;
    }
    if (count < 5) {
        y = 0x8c;
    }
    unkfunc_0217c9ec();
    unkfunc_0201e194(0x10, 0, 0xd0, 0x48, -1);
    unkfunc_0217cce4(x, y);
    unkfunc_0217ddc8(chara, index);
}

THUMB void unkfunc_0217deb4(char* select, char* list, int active, int count)
{
    if (!data_020ed1bc.isOpen()) {
        unkfunc_0217ca5c(select, list, count);
        unkfunc_0201e194(0, 0x68, 0xe0, 0x58, -1);
        unkfunc_0217cb3c(select);
        unkfunc_0201e194(0, 0, 0x90, 0x68, -1);
        if (list[active] != -1) {
            unkfunc_0201e3f4(list[active], 0);
            if (status::g_Party.getCarriageEnableOnGame()) {
                unkfunc_0217c3e0(&status::g_Party.getPlayerStatus(list[active])->haveStatusInfo_, 1);
                unkfunc_0201e194(0, 0, 0x100, 0xc0, 0x28);
            } else {
                unkfunc_0201e260();
            }
        } else {
            unkfunc_0217ccac();
            unkfunc_0201e260();
        }
    }
}

THUMB void unkfunc_0217df54(int itemType, int chara, int page, unsigned char* list, int count, int active)
{
    if (!data_020ed1bc.isOpen()) {
        unkfunc_0217ad94(5, 0);
        unkfunc_0217eb5c(0, 0xa0, 0);
        unkfunc_0217cc4c();
        unkfunc_0201e194(0x48, 0xa0, 0x40, 0x20, -1);
        unkfunc_0217eba4(0x88, 0xa0);
        unkfunc_0217b0c4(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, 1, chara, 0);
        if (itemType == -1) {
            unkfunc_0217e144(0, 1);
        } else {
            unkfunc_0217e9ec(chara, list, count, itemType, page);
            if (count > 6) {
                unkfunc_0217b368(0x50, (count - 1) / 6, page);
            }
            unkfunc_0217cc6c();
            unkfunc_0201e194(0, 0, 0x100, 0xa0, -1);
        }
    }
    status::g_Party.setPlayerMode();
    if (itemType != -1) {
        if (active == -1) {
            unkfunc_0217eb10(chara, -1, itemType);
        } else {
            unkfunc_0217eb10(chara, active, itemType);
        }
        unkfunc_0201e194(0, 0x78, 0x68, 0x48, 0x90);
    }
    unkfunc_0217c290(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, 1);
    unkfunc_0201e194(0x68, 0x60, 0x98, 0x60, -1);
}

THUMB void unkfunc_0217e060(int chara, int flag, int mode, int fukuro)
{
    unkfunc_0217ad94(2, -1);
    unkfunc_0217eb5c(0x40, 0xa0, flag);
    if (mode != 0) {
        unkfunc_0217eba4(0x88, 0xa0);
    } else {
        unkfunc_0217eba4(0, 0xa0);
    }
    unkfunc_0217b0c4(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, mode, chara, fukuro);
}

THUMB void unkfunc_0217e0ac(int chara)
{
    unkfunc_0217ad94(1, -1);
    unkfunc_0217eb5c(0x40, 0xa0, 0);
    unkfunc_0217eba4(0, 0xa0);
    unkfunc_0217b0c4(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, 0, chara, 0);
}

THUMB void unkfunc_0217e0e4(int index, int page)
{
    unkfunc_0217ad94(4, -1);
    unkfunc_0217eb5c(0x40, 0xa0, 0);
    unkfunc_0217eba4(0x88, 0xa0);
    if (index == -2) {
        unkfunc_0217b0c4(&status::g_Party.getPlayerStatus(0)->haveStatusInfo_, 1, index, 0);
    } else {
        unkfunc_0217b0c4(&status::g_Party.getPlayerStatus(index)->haveStatusInfo_, 1, index, 0);
    }
    unkfunc_0217e1bc(page);
}

THUMB void unkfunc_0217e144(int mode, int flag)
{
    int count;
    if (mode != 0 && status::g_Party.fukuro_ != 0) {
        count = status::g_Party.getCount() + 1;
    } else {
        count = status::g_Party.getCount();
    }
    unkfunc_0217b70c(mode, flag);
    if (count < 5) {
        unkfunc_0201e194(0, 0x70, 0xb8, 0x30, -1);
    } else if (count == 5) {
        unkfunc_0201e194(0, 0x70, 0xe0, 0x30, -1);
    } else {
        unkfunc_0201e194(0, 0x48, 0xe0, 0x58, -1);
    }
}

THUMB void unkfunc_0217e1bc(int page)
{
    status::g_Party.setBattleMode();
    int count = status::g_Party.getCount();
    if (page == 0) {
        if (count > 4) {
            count = 5;
            unkfunc_0217b300(0xd0, status::g_Party.getCount() / 5, page, 0);
        }
        for (int i = 0; i < count; i++) {
            unkfunc_0217aee0(&status::g_Party.getPlayerStatus(i)->haveStatusInfo_, i * 0x28 + 0x10, 0x70);
        }
        for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
            unkfunc_0217c8bc(i + 1, i * 0x28 + 8, 0x78, i);
        }
        if (status::g_Party.getCount() < 4) {
            int x = status::g_Party.getCount() * 0x28 + 0x10;
            unkfunc_0217af90(-2, x, 0x70);
            unkfunc_0201e194(0, 0x70, 0xb8, 0x30, -1);
        } else if (status::g_Party.getCount() == 4) {
            unkfunc_0217af90(-2, 0xb0, 0x70);
            unkfunc_0201e194(0, 0x70, 0xe0, 0x30, -1);
        } else {
            unkfunc_0201e194(0, 0x70, 0x100, 0x30, -1);
        }
    } else {
        if (page == 1) {
            if (count > 10) {
                count = 10;
            }
            for (int i = 5; i < count; i++) {
                unkfunc_0217aee0(&status::g_Party.getPlayerStatus(i)->haveStatusInfo_, (i - page * 5) * 0x28 + 0x10, 0x70);
            }
            if (status::g_Party.getCount() < 10) {
                int x = (status::g_Party.getCount() - 5) * 0x28 + 0x10;
                unkfunc_0217af90(-2, x, 0x70);
            }
        } else {
            unkfunc_0217af90(-2, 0x10, 0x70);
        }
        unkfunc_0217b300(0xd0, status::g_Party.getCount() / 5, page, 0);
        unkfunc_0201e194(0, 0x70, 0x100, 0x30, -1);
    }
}

THUMB void unkfunc_0217e34c(int chara, int target)
{
    if ((target != 0 && TownMenuPlayerControl::getSingleton()->targetFukuro_ != 0)
        || (target == 0 && TownMenuPlayerControl::getSingleton()->activeFukuro_ != 0)) {
        unkfunc_0217b038();
    } else {
        unkfunc_0217afc4(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_);
    }
}

THUMB void unkfunc_0217e388(int chara, int page, int target)
{
    TownMenuPlayerControl::getSingleton();
    if (TownMenuPlayerControl::getSingleton()->activeFukuro_ == 0) {
        TownMenuPlayerControl::getSingleton();
    }
    if ((target != 0 && TownMenuPlayerControl::getSingleton()->targetFukuro_ != 0)
        || (target == 0 && TownMenuPlayerControl::getSingleton()->activeFukuro_ != 0)) {
        int count = status::FukuroItemInfo::getPageItemCount(page);
        for (int i = 0; i < count; i++) {
            int item = status::FukuroItemInfo::getItemId(i, page);
            int index = status::FukuroItemInfo::getIndexToAll(i, page);
            unkfunc_0217b254(item, index, (i % 2 + 1) * 16 + (i % 2) * 0x68 + 4, (i / 2) * 8 + (i / 2) * 0x18 + 4);
        }
        return;
    }
    int count = status::PlayerItemInfo::getItemMaxCount(chara) - page * 6;
    if (count > 6) {
        count = 6;
    }
    for (int i = 0; i < count; i++) {
        if (page == 0) {
            int item = status::PlayerItemInfo::getItemIndex(chara, i);
            unkfunc_0217b1ec(item, (i % 2 + 1) * 16 + (i % 2) * 0x68 + 4, (i / 2) * 8 + (i / 2) * 0x18 + 4);
            if (status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveItem_.isEquipment(i)) {
                unkfunc_0217b2b4(i);
            }
        } else {
            int item = status::PlayerItemInfo::getItemIndex(chara, i + 6);
            unkfunc_0217b1ec(item, (i % 2 + 1) * 16 + (i % 2) * 0x68 + 4, (i / 2) * 8 + (i / 2) * 0x18 + 4);
        }
    }
    if (!data_020ed1bc.isOpen() && target != 0 && count < 6) {
        unkfunc_0217b1ec(-1, (count % 2 + 1) * 16 + (count % 2) * 0x68 + 4, (count / 2) * 8 + (count / 2) * 0x18 + 4);
    }
}

THUMB void unkfunc_0217e520(int page)
{
    int i;
    int end = (page + 1) * 4;
    if (end >= status::g_Party.getCount()) {
        end = status::g_Party.getCount();
    }
    for (i = page * 4; i < end; i++) {
        unkfunc_0217b908(&status::g_Party.getPlayerStatus(i)->haveStatusInfo_, i - page * 4, page);
    }
    if (status::g_Party.getCount() > 4) {
        unkfunc_0217b368(0x50, (status::g_Party.getCount() - 1) / 4, page);
    }
}

THUMB void unkfunc_0217e584(int page)
{
    status::g_Party.setBattleMode();
    unkfunc_0217e520(page);
    status::g_Party.setPlayerMode();
}

THUMB void unkfunc_0217e5a0(int chara)
{
    int count = status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveAction_.getCount();
    int n = 0;
    for (int i = 0; i < count; i++) {
        int action = status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveAction_.getAction(i);
        if (status::UseAction::isUsuallyUse(action)) {
            int message = unkfunc_0201e674(action);
            status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(chara)->haveStatusInfo_;
            unkfunc_0217bb74((n % 2) * 0x78 + 0x1c, (n / 2) * 16 + 0xc + (n / 2) * 8, message, 0, info);
            n++;
        }
    }
}

THUMB void unkfunc_0217e628(int type)
{
    if (status::g_Party.getCount() < 5) {
        if (type != 0) {
            unkfunc_0217c4f0();
        } else {
            unkfunc_0217c658();
        }
        unkfunc_0201e194(0, 0, status::g_Party.getCount() * 64, 0x70, -1);
        return;
    }
    if (type != 0) {
        unkfunc_0217c4f0();
    } else {
        unkfunc_0217c658();
    }
    unkfunc_02050698(0, 0xc1);
    unkfunc_0201e194(0, 0, 0x100, 0x70, -1);
    unkfunc_02050698(0, 0);
    int count = status::g_Party.getCount() - 4;
    if (count > 4) {
        unkfunc_0201e194(0, 0x60, 0x100, 0x60, -1);
        unkfunc_0201e194(0, 0, (count - 4) * 64, 0x60, -1);
    } else {
        unkfunc_0201e194(0, 0x60, count * 64, 0x60, -1);
    }
}

THUMB void unkfunc_0217e6e8(int mode, char* command, int count)
{
    int tactics[9] = { 0x8000006f, 0x8000006b, 0x80000071, 0x80000070, 0x80000072, 0x80000073, 0x80000082, 0x80000074, 0x80000083 };
    int setting[2] = { 0xa000006e, 0xa000006f };
    if (mode == 1) {
        for (int i = 0; i < 2; i++) {
            unkfunc_0217c94c((i % 2) * 0x50 + 0x10 + (i % 2) * 16, 0x88, setting[i]);
        }
        unkfunc_0201e194(0, 0x80, 0xd8, 0x20, -1);
        return;
    }
    for (int i = 0; i < count; i++) {
        unkfunc_0217c94c((i % 2) * 0x50 + 0x20 + (i % 2) * 16, (i / 2) * 16 + 0x24 + (i / 2) * 8, tactics[command[i]]);
    }
    unkfunc_0201e194(0x10, 0x18, 0xd0, ((count - 1) / 2) * 0x18 + 0x28, -1);
}

THUMB void unkfunc_0217e7bc(char* order, int mode)
{
    status::g_Party.setBattleMode();
    int carriageOut = status::g_Party.getCarriageOutCount();
    int count = status::g_Party.getCount();
    if (mode != 2 && (status::g_Party.getSortIndex(1) != -1 || status::g_Party.getSortIndex(2) != -1)) {
        count--;
    }
    for (int index = 0; index < status::g_Party.getCount(); index++) {
        status::PlayerStatus* player = status::g_Party.getPlayerStatus(index);
        if (player->haveStatusInfo_.haveStatus_.isPlayer() == false) {
            count--;
        }
    }
    int y = 0x48;
    int i = 0;
    if (count < 5) {
        y = 0x70;
    }
    if (count < 5) {
        unkfunc_0201e194(0, 0x70, 0xe0, 0x30, -1);
    } else {
        unkfunc_0201e194(0, 0x48, 0xe0, 0x58, -1);
    }
    for (; order[i] != -1 && i < 5; i++) {
        unkfunc_0217aee0(&status::g_Party.getPlayerStatus(order[i])->haveStatusInfo_, i * 32 + 16 + i * 8, y);
    }
    int n = 0;
    for (int j = 0; j < carriageOut; j++) {
        if (mode != 2) {
            if (status::g_Party.getPlayerStatus(j)->haveStatusInfo_.haveStatus_.playerIndex_ == 1) {
                continue;
            }
            if (status::g_Party.getPlayerStatus(j)->haveStatusInfo_.haveStatus_.playerIndex_ == 2) {
                continue;
            }
        }
        status::PlayerStatus* player = status::g_Party.getPlayerStatus(j);
        if (player->haveStatusInfo_.haveStatus_.isPlayer() != false) {
            int x = n * 32 + 16 + n * 8;
            unkfunc_0217c8bc(j + 1, x - 8, y + 8, order[n]);
            n++;
        }
    }
    if (order[i] == -1 || i < 5) {
        int x = i * 32 + 16 + i * 8;
        if (i == 5) {
            x = 16;
        }
        unkfunc_0217af90(-2, x, 0x70);
        return;
    }
    for (; order[i] != -1 && i < 9; i++) {
        unkfunc_0217aee0(&status::g_Party.getPlayerStatus(order[i])->haveStatusInfo_, (i - 5) * 32 + 16 + (i - 5) * 8, 0x70);
    }
    unkfunc_0217af90(-2, (i - 5) * 32 + 16 + (i - 5) * 8, 0x70);
}

THUMB void unkfunc_0217e994(char* order)
{
    int i = 0;
    unkfunc_0217c918(0, 0xe, 0x80000091);
    for (; order[i] != -1; i++) {
        unkfunc_0217c974(&status::g_Party.getPlayerStatus(order[i])->haveStatusInfo_, 0, i * 16 + 0x30);
    }
    unkfunc_0201e194(0, 0, 0x100, 0xc0, 0x20);
}

THUMB void unkfunc_0217e9ec(int chara, unsigned char* list, int count, int itemType, int page)
{
    if (itemType == -1) {
        for (int i = 0; i < count; i++) {
            if (status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveItem_.isEquipment(list[i])) {
                int item = status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveItem_.getItem(list[i]);
                unkfunc_0217b1ec(item, (i % 2 + 1) * 16 + 4 + (i % 2) * 0x68, (i / 2) * 8 + (i / 2) * 0x18 + 4);
                unkfunc_0217b2b4(i);
            }
        }
        return;
    }
    if (page == 0) {
        if (count > 6) {
            count = 6;
        }
        for (int i = 0; i < count; i++) {
            unkfunc_0217b1ec(list[i], (i % 2 + 1) * 16 + 4 + (i % 2) * 0x68, (i / 2) * 8 + (i / 2) * 0x18 + 4);
        }
        if (count > 0 && status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.haveEquipment_.isEquipment(list[0])) {
            unkfunc_0217b2b4(0);
        }
        return;
    }
    for (int i = 6; i < count; i++) {
        unkfunc_0217b1ec(list[i], ((i - 6) % 2 + 1) * 16 + 4 + ((i - 6) % 2) * 0x68, ((i - 6) / 2) * 8 + ((i - 6) / 2) * 0x18 + 4);
    }
}

THUMB void unkfunc_0217eb10(int chara, int index, int itemType)
{
    if (itemType != -1) {
        if (index == -1) {
            unkfunc_0217b488(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, -1, itemType, 0);
        } else {
            int item = status::PlayerItemInfo::getItemIndex(chara, index);
            unkfunc_0217b488(&status::g_Party.getPlayerStatus(chara)->haveStatusInfo_, item, itemType, 0);
        }
    }
}

THUMB void unkfunc_0217eb5c(int x, int y, int flag)
{
    if (flag != 0) {
        for (int i = 0; i < 6; i++) {
            unkfunc_0201e194((i % 2) * 0x48 + 0x40, (i / 2) * 32 + 0x60, 0x48, 0x20, -1);
        }
        return;
    }
    unkfunc_0201e194(x, y, 0x48, 0x20, -1);
}

THUMB void unkfunc_0217eba4(int x, int y)
{
    unkfunc_0201e194(x, y, 0x40, 0x20, -1);
}
