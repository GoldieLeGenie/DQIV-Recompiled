#pragma ipa file
#include "ov015/btl/UnkBattleMenuDraw_0216ba54.hpp"
#include "ov015/btl/UnkBattleMenuDraw_0216afc0.hpp"
#include "ov015/btl/BattleMenuPlayerControl.hpp"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/UseAction.hpp"
#include "main/status/UseItem.hpp"
#include "main/text/TextAPI.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/menu/UnkMenuPartsDraw.hpp"

// parts lists (their definition order sets the .data layout)
static UnkMenuParts s_parts_02176f38[] = {
    { 0x01, 0x00, (short)0xf000, 0, 0, 0x68, 0xe0, 0x58 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02176e90[] = {
    { 0x01, 0x00, (short)0xf000, 0, 0x18, 0x78, 0xd0, 0x28 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02176f54[] = {
    { 0x01, 0x00, (short)0xf000, 0, 0x40, 0xa0, 0x98, 0x20 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02176f1c[] = {
    { 0x03, 0xd0, (short)0xf000, 0, 0, 8, 0x100, 0x40 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02176f70[] = {
    { 0x01, 0x00, (short)0xf000, 0, 0x18, 0x58, 0xd0, 0x48 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02176ee4[] = {
    { 0x01, 0x00, (short)0xf000, 0, 0x10, 0x70, 0xe8, 0x30 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02176eac[] = {
    { 0x01, 0x00, (short)0xf000, 0, 0, 0, 0x98, 0x68 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02176ec8[] = {
    { 0x0d, 0x09, (short)0xf000, 0, 0, 4, 0x70, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02176f00[] = {
    { 0x0e, 0x08, (short)0xf000, 0, 6, 0, 0x30, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0217705e[] = {
    { 0x14, 0xc2, 0, 0, 4, 0xe, 0x20, 0x18 },
    { 0x0c, 0xf1, 0, 0, 0, 0, 0x20, 0x20 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02177088[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x14, 0xf, 0x4c, 0xa },
    { 0x0d, 0x08, (short)0xf000, 1, 0x74, 0xf, 0x4c, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021770dc[] = {
    { 0x01, 0x00, (short)0xf000, 0, 0, 0, 0x100, 0xa0 },
    { 0x16, 0x00, (short)0xf000, 0, 0, 0x68, 0x100, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02177106[] = {
    { 0x0f, 0x08, (short)0xf000, 0, 0, 0, 0xa, 0xa },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 0xa, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02177130[] = {
    { 0x14, 0xc2, 0, 0, 4, 0xe, 0x20, 0x18 },
    { 0x0c, 0xf1, 0, 0, 0, 0, 0x20, 0x20 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0217715a[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x4c, 0xab, 0x3c, 0xa },
    { 0x0d, 0x08, (short)0xf000, 1, 0x94, 0xab, 0x3c, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02176f8c[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x10, 0, 0x30, 0xa },
    { 0x0d, 0x08, (short)0xf000, 1, 0x14, 0xc, 0x28, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02176fb6[] = {
    { 0x0d, 0x08, (short)0xf000, 2, 0x20, 0xc, 0xa, 0xa },
    { 0x0f, 0x0b, (short)0xf000, 3, 0, 0, 0x14, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02176fe0[] = {
    { 0x01, 0x00, (short)0xf000, 0, 0, 0, 0x48, 0x20 },
    { 0x0d, 0x09, (short)0xf000, 0, 8, 0xa, 0x38, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_0217700a[] = {
    { 0x01, 0x00, (short)0xf000, 0, 0, 0, 0x100, 0xa0 },
    { 0x16, 0x00, (short)0xf000, 0, 0, 0x68, 0x100, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02177034[] = {
    { 0x0d, 0x08, (short)0xf000, 2, 0x20, 0xc, 0xa, 0xa },
    { 0x0d, 0x0b, (short)0xf000, 3, 0, 0, 0x14, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021770b2[] = {
    { 0x14, 0xc2, 0, 0, 4, 0xe, 0x20, 0x18 },
    { 0x0c, 0xf1, 0, 0, 0, 0, 0x20, 0x20 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021771bc[] = {
    { 0x01, 0x00, (short)0xf000, 0, 0, 0, 0x100, 0xa0 },
    { 0x16, 0x00, (short)0xf000, 2, 0, 8, 0x100, 0 },
    { 0x16, 0x00, (short)0xf000, 3, 0, 0x90, 0x100, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_021771f4[] = {
    { 0x01, 0x00, (short)0xf000, 0, 0, 0, 0x100, 0xc0 },
    { 0x16, 0x00, (short)0xf000, 1, 0, 0x18, 0x100, 0 },
    { 0x0d, 0x09, (short)0xf000, 0, 0x10, 0xc, 0xe0, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static UnkMenuParts s_parts_02177184[] = {
    { 0x14, 0xc2, 0, 0, 4, 0xe, 0x20, 0x18 },
    { 0x0c, 0xf1, 0, 0, 0, 0, 0x20, 0x20 },
    { 0x0f, 0x0a, (short)0xf000, 1, -4, 8, 0xc, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static int s_param[2];

THUMB void unkfunc_0216ba54(int group)
{
    unkfunc_0216c08c(group);
    unkfunc_0216c1d4();
}

THUMB void unkfunc_0216ba60(int* items, int count, int page)
{
    unkfunc_0216c264();
    unkfunc_0216c10c(items, count, page);
}

THUMB void unkfunc_0216ba78(int* items, int count, int page)
{
    unkfunc_02050ea8(s_parts_02176f54, 0);
    int chara = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
    status::PlayerStatus* player = status::g_Party.getPlayerStatus(chara);
    unkfunc_0216b07c(&player->haveStatusInfo_, btl::BattleMenuPlayerControl::getSingleton()->memberHP_[chara], 0);
    s_param[0] = 0x80000069;
    s_param[1] = 0x8000006b;
    unkfunc_02050ed0(s_parts_0217715a, s_param, 1);
    unkfunc_0216c10c(items, count, page);
}

THUMB void unkfunc_0216bae8()
{
    unkfunc_0216c264();
    unkfunc_0216c0c8();
}

THUMB void unkfunc_0216baf4(int group)
{
    unkfunc_0216c08c(group);
    unkfunc_0216c264();
}

THUMB void unkfunc_0216bb00(int* actions, int count, int page)
{
    int pageMax;
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(btl::BattleMenuPlayerControl::getSingleton()->activeChara_)->haveStatusInfo_;
    unkfunc_0216c1e4();
    unkfunc_02050ea8(s_parts_0217700a, 0);
    pageMax = (count - 1) / 6;
    int num = (count - 1) % 6 + 1;
    if (pageMax > page) {
        num = 6;
    }
    int top = page * 6;
    for (int i = 0; i < num; i++) {
        int x = i % 2;
        int y = i / 2;
        x = x * 120 + 20;
        y = y * 32 + 12;
        unkfunc_0216b314(x, y, actions[top + i]);
    }
    unkfunc_0216b254(8, 0x74, status::UseAction::getMenuMessage(info->haveAction_.getAction(btl::BattleMenuPlayerControl::getSingleton()->activeMagic_)));
    unkfunc_0216b27c(0xd8, 0x78, pageMax, page);
    unkfunc_0216b6cc();
}

THUMB void unkfunc_0216bbc0(int group)
{
    unkfunc_0216c1e4();
    unkfunc_0216c08c(group);
}

THUMB void unkfunc_0216bbd0()
{
    unkfunc_0216c1e4();
    unkfunc_0216c0c8();
}

THUMB void unkfunc_0216bbdc()
{
    unkfunc_02050ea8(s_parts_02176ee4, 0);
    unkfunc_0216c2a0();
    int list[4];
    dss::memset(list, 0, sizeof(list));
    int n = 0;
    for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
        int index = status::g_Party.getPlayerIndex(i);
        status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i)->haveStatusInfo_;
        if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.isPlayer_ && index != 1 && index != 2) {
            int x = n * 0x28 + 0x48;
            unkfunc_0216c3a4(x, 0x70, index);
            unkfunc_0216b638(info, x, 0x70);
            n++;
        }
    }
    int param = 0x17;
    unkfunc_02050ebc(s_parts_0217705e, &param, 0x20, 0x70);
    unkfunc_0216c350(btl::BattleMenuPlayerControl::getSingleton()->activeChara_);
    unkfunc_0216b6cc();
}

THUMB void unkfunc_0216bc84()
{
    static const int tactics[6] = { 0x90000001, 0x90000002, 0x90000005, 0x90000003, 0x90000004, 0x90000006 };
    unkfunc_02050ea8(s_parts_02176f70, 0);
    unkfunc_0216c2a0();
    const int* p = tactics;
    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 3; i++) {
            int param = *p;
            param += btl::BattleMenuPlayerControl::getSingleton()->tacticsSex_;
            unkfunc_02050ee0(s_parts_02176f00, &param, i * 0x40 + 0x24, j * 0x1e + 0x60, 1);
            p++;
        }
    }
    int count = 0;
    int list[4];
    dss::memset(list, 0, sizeof(list));
    for (int i = 0; i < status::g_Party.getCarriageOutCount() && count < 4; i++) {
        int index = status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_;
        if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.isPlayer_ && index != 1 && index != 2) {
            list[count] = i;
            count++;
        }
    }
    int k;
    int member;
    int chara = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
    if (chara == -1) {
        for (k = 0; k < count; k++) {
            status::PlayerStatus* player = status::g_Party.getPlayerStatus(list[k]);
            member = list[k];
            unkfunc_0216aff8(k * 0x40, 0x38, btl::BattleMenuPlayerControl::getSingleton()->memberHP_[member], &player->haveStatusInfo_);
        }
    }
    unkfunc_0216c350(chara);
    unkfunc_0216b6cc();
}

THUMB void unkfunc_0216bd94()
{
    unkfunc_02050ea8(s_parts_02176e90, 0);
    int param[2] = { (int)0x80000008, (int)0x80000009 };
    unkfunc_02050ee0(s_parts_02177088, param, 0x18, 0x78, 1);
    unkfunc_0216c2b0();
    unkfunc_0216b6cc();
}

THUMB void unkfunc_0216bdd4()
{
    UnkMenuParts window[] = {
        { 0x01, 0x00, (short)0xf000, 0, 0x20, 0x70, 0xc0, 0x30 },
        { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
    };
    unkfunc_02050ea8(window, 0);
    for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
        int x = i * 0x28 + 0x30;
        int index = status::g_Party.getPlayerIndex(i);
        status::PlayerStatus* player = status::g_Party.getPlayerStatus(i);
        unkfunc_0216c3a4(x, 0x70, index);
        unkfunc_0216b638(&player->haveStatusInfo_, x, 0x70);
    }
    unkfunc_0216c2b0();
    unkfunc_0216c350(btl::BattleMenuPlayerControl::getSingleton()->activeChara_);
    unkfunc_0216b6cc();
}

THUMB void unkfunc_0216be58(int* list, int count, int page)
{
    status::g_Party.setMemberShiftMode();
    unkfunc_0216b590(list, count, page);
    unkfunc_0216c2b0();
    int chara = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
    status::PlayerStatus* player = status::g_Party.getPlayerStatus(chara);
    unkfunc_0216b07c(&player->haveStatusInfo_, btl::BattleMenuPlayerControl::getSingleton()->memberHP_[chara], 0);
    int target = btl::BattleMenuPlayerControl::getSingleton()->targetChara_;
    unkfunc_0216c350(target);
    unkfunc_0216b460(0x78, 8, &status::g_Party.getPlayerStatus(target)->haveStatusInfo_);
    unkfunc_0216b6cc();
}

THUMB void unkfunc_0216bec8(int* list, int count, int* order, int orderCount)
{
    status::g_Party.setMemberShiftMode();
    unkfunc_02050ea8(s_parts_02176eac, 0);
    for (int i = 0; i < 4; i++) {
        int x = (i % 2) * 0x48 + 8;
        int y = (i / 2) * 0x30 + 8;
        int number[3];
        number[0] = i + 1;
        number[1] = (int)":";
        number[2] = (int)"\xff\xfe\xa6\x24";
        int color = 0;
        if (orderCount > i) {
            status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(*order)->haveStatusInfo_;
            color = unkfunc_0216b980(info);
            int param[4] = { 0, (int)0xa0000013, (int)":", 0 };
            param[0] = info->haveStatus_.playerIndex_ + 0x50000000;
            param[3] = status::g_Party.getPlayerStatus(*order)->haveStatusInfo_.haveStatus_.level_;
            if (info->haveStatus_.isBattleNpc_) {
                param[3] = 0xa0000073;
                unkfunc_02050ee0(s_parts_02177034, param, x, y, data_020be244[color]);
            } else {
                unkfunc_02050ee0(s_parts_02176fb6, param, x, y, data_020be244[color]);
            }
            unkfunc_02050ee0(s_parts_02176f8c, param, x, y, data_020be244[color]);
        }
        unkfunc_02050ee0(s_parts_02177106, number, x, y, data_020be244[color]);
        order++;
    }
    int target = btl::BattleMenuPlayerControl::getSingleton()->targetChara_;
    status::g_Party.setBattleMode();
    if (target < status::g_Party.getCount()) {
        unkfunc_0216b460(0x98, 0, &status::g_Party.getPlayerStatus(target)->haveStatusInfo_);
    }
    unkfunc_0216c2c0(list, count);
    unkfunc_0216b6cc();
}

THUMB void unkfunc_0216c040()
{
    for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
        status::PlayerStatus* player = status::g_Party.getPlayerStatus(i);
        unkfunc_0216aff8(i * 0x40, 0xa0, btl::BattleMenuPlayerControl::getSingleton()->memberHP_[i], &player->haveStatusInfo_);
    }
}

THUMB void unkfunc_0216c08c(int group)
{
    int chara = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
    status::PlayerStatus* player = status::g_Party.getPlayerStatus(chara);
    unkfunc_0216b07c(&player->haveStatusInfo_, btl::BattleMenuPlayerControl::getSingleton()->memberHP_[chara], 0);
    unkfunc_0216b0a0(group);
    unkfunc_0216b6cc();
}

THUMB void unkfunc_0216c0c8()
{
    unkfunc_02050ea8(s_parts_021771bc, 0);
    for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
        unkfunc_0216b344(&status::g_Party.getPlayerStatus(i)->haveStatusInfo_, i);
    }
    unkfunc_0216b6cc();
}

THUMB void unkfunc_0216c10c(int* items, int count, int page)
{
    int pageMax;
    int top;
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(btl::BattleMenuPlayerControl::getSingleton()->activeChara_)->haveStatusInfo_;
    pageMax = (count - 1) / 6;
    int num = (count - 1) % 6 + 1;
    if (pageMax > page) {
        num = 6;
    }
    unkfunc_02050ea8(s_parts_021770dc, 0);
    top = page * 6;
    for (int i = 0; i < num; i++) {
        int x = i % 2;
        int y = i / 2;
        x = x * 120 + 20;
        y = y * 32;
        unkfunc_0216b2c4(x, y, items[top + i], info->haveItem_.isEquipment(i + top));
    }
    unkfunc_0216b27c(0xd8, 0x78, pageMax, page);
    unkfunc_0216b254(8, 0x74, status::UseItem::getMenuMessage(btl::BattleMenuPlayerControl::getSingleton()->getPlayerItemId()));
    unkfunc_0216b6cc();
}

THUMB void unkfunc_0216c1d4()
{
    unkfunc_0216b134(0x10, 0x40, 0xa0, 0);
}

THUMB void unkfunc_0216c1e4()
{
    int useMp;
    int chara = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(chara)->haveStatusInfo_;
    unkfunc_0216b07c(info, btl::BattleMenuPlayerControl::getSingleton()->memberHP_[chara], 0);
    unkfunc_0216b134(0x20, 0x40, 0xa0, 0);
    useMp = status::UseAction::getUseMp(info->haveAction_.getAction(btl::BattleMenuPlayerControl::getSingleton()->activeMagic_));
    if (useMp == 0xff) {
        useMp = info->getMp();
        if (useMp == 0) {
            useMp = 1;
        }
    } else if (useMp == 1000) {
        useMp = info->getMp();
    }
    unkfunc_0216b8d0(info->getMp(), useMp);
}

THUMB void unkfunc_0216c264()
{
    int chara = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
    status::PlayerStatus* player = status::g_Party.getPlayerStatus(chara);
    unkfunc_0216b07c(&player->haveStatusInfo_, btl::BattleMenuPlayerControl::getSingleton()->memberHP_[chara], 0);
    unkfunc_0216b134(0x40, 0x40, 0xa0, 0);
}

THUMB void unkfunc_0216c2a0()
{
    unkfunc_0216b134(4, 0x40, 0xa0, 0);
}

THUMB void unkfunc_0216c2b0()
{
    unkfunc_0216b134(2, 0x40, 0xa0, 0);
}

THUMB void unkfunc_0216c2c0(int* list, int count)
{
    unkfunc_02050ea8(s_parts_02176f38, 0);
    for (int i = 0; i < count; i++) {
        int x = (i % 5) * 0x28 + 0x10;
        int y = (i / 5) * 0x28 + 0x68;
        if (list[i] != -1) {
            status::PlayerStatus* player = status::g_Party.getPlayerStatus(list[i]);
            unkfunc_0216c3a4(x, y, status::g_Party.getPlayerIndex(list[i]));
            unkfunc_0216b638(&player->haveStatusInfo_, x, y);
        } else {
            unkfunc_0216c3a4(x, y, -1);
        }
    }
}

THUMB void unkfunc_0216c350(int chara)
{
    if (chara == -1) {
        int param = 0x8000006e;
        unkfunc_02050ee0(s_parts_02176fe0, &param, 0x88, 0xa0, 1);
        return;
    }
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(chara)->haveStatusInfo_;
    unkfunc_0216aff8(0x88, 0xa0, info->getHp(), info);
}

THUMB void unkfunc_0216c3a4(int x, int y, int index)
{
    int inParty = 0;
    int found = 0;
    status::g_Party.setBattleMode();
    for (int i = 0; i < status::g_Party.getCount(); i++) {
        if (index == status::g_Party.getPlayerIndex(i)) {
            if (i < status::g_Party.getCarriageOutCount()) {
                inParty = 1;
            }
            found = i;
        }
    }
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(found)->haveStatusInfo_;
    int param[2];
    param[0] = info->haveStatus_.iconIndex_;
    param[1] = found + 1;
    if (index == -1) {
        param[0] = 0x19;
        unkfunc_02050ebc(s_parts_02177130, param, x, y);
        return;
    }
    if (inParty) {
        unkfunc_02050ee0(s_parts_02177184, param, x, y, data_020be244[unkfunc_0216b980(info)]);
        return;
    }
    unkfunc_02050ebc(s_parts_021770b2, param, x, y);
}

THUMB void unkfunc_0216c468(int chara)
{
    int count = status::g_Party.getCarriageOutCount();
    for (int i = 0; i < count; i++) {
        status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i)->haveStatusInfo_;
        int hp = btl::BattleMenuPlayerControl::getSingleton()->memberHP_[i];
        unkfunc_0216b6e0(info, hp, btl::BattleMenuPlayerControl::getSingleton()->memberMP_[i], info->haveStatus_.level_, i);
        int first = btl::BattleMenuPlayerControl::getSingleton()->firstHistory_[i];
        int second = btl::BattleMenuPlayerControl::getSingleton()->secondHistory_[i];
        unkfunc_0216b7d4(i, first, second, btl::BattleMenuPlayerControl::getSingleton()->getHPColor(i));
        if (i != chara) {
            unkfunc_0216b858(i * 0x40 + 8, 0x4a, info->haveStatus_.iconIndex_, i);
        }
    }
    unkfunc_02050ea8(s_parts_02176f1c, 0);
}

THUMB void unkfunc_0216c524(int* actions, int count, int chara)
{
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(chara)->haveStatusInfo_;
    unkfunc_02050698(0, 0);
    TextAPI::setMACRO0(1, 0x50000000, info->haveStatus_.playerIndex_);
    int message = 0xa0000023;
    unkfunc_02050ed0(s_parts_021771f4, &message, data_020be244[0]);
    for (int i = 0; i < count; i++) {
        int param = actions[i] + 0x70000000;
        unkfunc_02050ee0(s_parts_02176ec8, &param, (i % 2) * 120 + 16, (i / 2) * 16 + 36, data_020be244[unkfunc_0216b980(info)]);
    }
}
