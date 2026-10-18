#pragma ipa file
#include "main/menu/UnkMenuCommonDraw_0201e194.hpp"
#ifdef DQIV_NATIVE
#include "main/menu/UnkMenuIconData.hpp"
#include "main/dss/UnkMemory.hpp"
#include "HdPictures.hpp"
#endif
#include "main/menu/MenuDataCommon.hpp"
#include "main/menu/UnkMenuPartsDraw.hpp"
#include "main/status/GameStatus.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/UseAction.hpp"

static UnkMenuParts s_frameParts[] = {
    { 0x01, 0x00, (short)0xf000, 0, 0x00, 0x00, 0x00, 0x00 },
    { 0xff, 0x00, 0x0000, 0, 0x00, 0x00, 0x00, 0x00 },
};

static UnkMenuParts s_unusedParts[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0xd0, 0x08, 0x38, 0x0a },
    { 0xff, 0x00, 0x0000, 0, 0x00, 0x00, 0x00, 0x00 },
};

static UnkMenuParts s_coinParts[] = {
    { 0x0f, 0x0a, (short)0xf000, 0, 0x00, 0xaa, 0x40, 0x0c },
    { 0xff, 0x00, 0x0000, 0, 0x00, 0x00, 0x00, 0x00 },
};

static UnkMenuParts s_goldParts[] = {
    { 0x0f, 0x0a, (short)0xf000, 0, 0x00, 0xaa, 0x40, 0x0c },
    { 0x0d, 0x0b, (short)0xf000, 1, 0x00, 0x00, 0x10, 0x0c },
    { 0xff, 0x00, 0x0000, 0, 0x00, 0x00, 0x00, 0x00 },
};

static UnkMenuParts s_statusAltParts[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0xd0, 0x08, 0x10, 0x0a },
    { 0x0d, 0x08, (short)0xf000, 1, 0xdc, 0x08, 0x10, 0x0a },
    { 0x0d, 0x0a, (short)0xf000, 2, 0xdc, 0x08, 0x18, 0x0a },
    { 0xff, 0x00, 0x0000, 0, 0x00, 0x00, 0x00, 0x00 },
};

static UnkMenuParts s_statusParts[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0xd0, 0x08, 0x10, 0x0a },
    { 0x0d, 0x08, (short)0xf000, 1, 0xdc, 0x08, 0x10, 0x0a },
    { 0x0f, 0x0a, (short)0xf000, 2, 0xdc, 0x08, 0x18, 0x0a },
    { 0xff, 0x00, 0x0000, 0, 0x00, 0x00, 0x00, 0x00 },
};

static UnkMenuParts s_memberParts[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x08, 0x0a, 0x30, 0x0a },
    { 0x0d, 0x08, (short)0xf000, 1, 0x07, 0x20, 0x10, 0x0a },
    { 0x0f, 0x0a, (short)0xf000, 2, 0x10, 0x20, 0x28, 0x0a },
    { 0x0d, 0x08, (short)0xf000, 3, 0x07, 0x2e, 0x10, 0x0a },
    { 0x0f, 0x0a, (short)0xf000, 4, 0x10, 0x2e, 0x28, 0x0a },
    { 0x0d, 0x08, (short)0xf000, 5, 0x08, 0x3c, 0x10, 0x0a },
    { 0x0d, 0x0b, (short)0xf000, 6, 0x00, 0x00, 0x08, 0x0a },
    { 0x0f, 0x0a, (short)0xf000, 7, 0x10, 0x3c, 0x28, 0x0a },
    { 0xff, 0x00, 0x0000, 0, 0x00, 0x00, 0x00, 0x00 },
};

static UnkMenuParts s_statusBigParts[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x9c, 0x08, 0x30, 0x0a },
    { 0x0d, 0x08, (short)0xf000, 1, 0x9c, 0x20, 0x18, 0x0a },
    { 0x0f, 0x0a, (short)0xf000, 2, 0xb4, 0x20, 0x20, 0x0a },
    { 0x0d, 0x08, (short)0xf000, 3, 0xd4, 0x20, 0x08, 0x0a },
    { 0x0f, 0x0a, (short)0xf000, 4, 0xd4, 0x20, 0x20, 0x0a },
    { 0x0d, 0x08, (short)0xf000, 5, 0x9c, 0x2e, 0x18, 0x0a },
    { 0x0f, 0x0a, (short)0xf000, 6, 0xb4, 0x2e, 0x20, 0x0a },
    { 0x0d, 0x08, (short)0xf000, 7, 0xd4, 0x2e, 0x08, 0x0a },
    { 0x0f, 0x0a, (short)0xf000, 8, 0xd4, 0x2e, 0x20, 0x0a },
    { 0x0d, 0x08, (short)0xf000, 9, 0x9c, 0x42, 0x38, 0x0a },
    { 0x0d, 0x08, (short)0xf000, 10, 0xd4, 0x42, 0x08, 0x0a },
    { 0x0f, 0x0a, (short)0xf000, 11, 0xd4, 0x42, 0x20, 0x0a },
    { 0x0d, 0x08, (short)0xf000, 12, 0x9c, 0x50, 0x38, 0x0a },
    { 0x0d, 0x08, (short)0xf000, 13, 0xd4, 0x50, 0x08, 0x0a },
    { 0x0f, 0x0a, (short)0xf000, 14, 0xd4, 0x50, 0x20, 0x0a },
    { 0xff, 0x00, 0x0000, 0, 0x00, 0x00, 0x00, 0x00 },
};

THUMB void unkfunc_0201e194(int x, int y, int w, int h, int lineY)
{
    s_frameParts[0].x_ = x;
    s_frameParts[0].y_ = y;
    s_frameParts[0].w_ = w;
    s_frameParts[0].h_ = h;
    unkfunc_02050ea8(s_frameParts, NULL);
    if (lineY > 0) {
        unkfunc_0201e1c4(x, lineY, w);
    }
}

// not in the ROM (dead-stripped), its parts list is still in .data
THUMB void unkfunc_unused_13()
{
    unkfunc_02050ea8(s_unusedParts, NULL);
}

THUMB void unkfunc_0201e1c4(int x, int y, int w)
{
    UnkMenuParts parts[] = {
        { 0x16, 0x00, (short)0xf000, 0, 0x00, 0x00, 0x00, 0x00 },
        { 0xff, 0x00, 0x0000, 0, 0x00, 0x00, 0x00, 0x00 },
    };
    parts[0].x_ = x;
    parts[0].y_ = y;
    parts[0].w_ = w;
    unkfunc_02050ea8(parts, NULL);
}

THUMB void unkfunc_0201e1f4()
{
    unkfunc_02050698(0, 0);
    for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
#ifdef DQIV_NATIVE
        // A taller name band, for the portrait.
        unkfunc_0201e194(i * 0x40, 0, 0x40, 0x58, 0x1c);
        continue;
#endif
        unkfunc_0201e194(i * 0x40, 0, 0x40, 0x50, 0x18);
    }
}

THUMB void unkfunc_0201e234()
{
    UnkMenuParts parts[] = {
        { 0x0c, 0xf4, 0x0000, 0, 0xe0, 0xa0, 0x18, 0x18 },
        { 0xff, 0x00, 0x0000, 0, 0x00, 0x00, 0x00, 0x00 },
    };
    int param = 5;
    unkfunc_02050ea8(parts, &param);
}

THUMB void unkfunc_0201e260()
{
    status::g_Party.setBattleMode();
    int dead[4] = { -1, -1, -1, -1 };
    int deadCount = 0;
    int pos = 0;
    for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
        if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath()) {
            dead[deadCount] = i;
            deadCount++;
        } else {
            unkfunc_0201e554(&status::g_Party.getPlayerStatus(i)->haveStatusInfo_, pos++);
        }
    }
    for (int j = 0; j < deadCount; j++) {
        unkfunc_0201e554(&status::g_Party.getPlayerStatus(dead[j])->haveStatusInfo_, j + pos);
    }
    unkfunc_0201e1f4();
    status::g_Party.setPlayerMode();
}

THUMB int unkfunc_0201e2f4(status::HaveStatusInfo* info)
{
    if (info->isDeath()) {
        return 2;
    }
    if (info->setNearDeath()) {
        return 1;
    }
    return 0;
}

THUMB int unkfunc_0201e318(status::HaveStatusInfo* info)
{
    if (info->isDeath()) {
        return 2;
    }
    if (info->isPoison()) {
        return 3;
    }
    if (info->isSpell() && info->haveStatus_.playerIndex_ != 25) {
        return 4;
    }
    return 0;
}

THUMB void unkfunc_0201e350(int x, int y, int coin)
{
    UnkMenuParts* parts;
    int param[2];
    if (coin) {
        parts = s_coinParts;
        param[0] = status::g_Party.casinoCoin_;
        param[1] = 0;
    } else {
        parts = s_goldParts;
        param[0] = status::g_Party.gold_;
        param[1] = 0xa000003c;
    }
    if (x == -1 && y == -1) {
        parts[0].x_ = 0xb6;
        parts[0].w_ = 0x38;
        unkfunc_02050698(0, 0);
        unkfunc_02050ed0(parts, param, g_menuTextColors[0]);
        unkfunc_0201e194(0xb0, 0xa0, 0x50, 0x20, -1);
    } else {
        parts[0].x_ = x + 6;
        parts[0].w_ = 0x38;
        unkfunc_02050ed0(parts, param, g_menuTextColors[0]);
        unkfunc_0201e194(x, y, 0x50, 0x20, -1);
    }
}

THUMB void unkfunc_0201e3f4(int index, int mode)
{
    UnkMenuParts* parts = s_statusParts;
    int x = 0;
    int y = 0;
    int frameX = 0x90;
    int frameY = 0;
    int lineY = 0x18;
    if (mode == 1) {
        unkfunc_02050698(0, 0);
        x -= 0x90;
        y += 0x58;
        frameX = 0;
        frameY = 0x58;
        lineY = 0x70;
    }
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(index)->haveStatusInfo_;
    int param[15];
    param[0] = info->haveStatus_.playerIndex_ + 0x50000000;
    param[1] = 0xa0000001;
    param[2] = info->haveStatus_.getHp();
    param[3] = (int)L"\xfeff\x24a7";
    param[4] = info->haveStatus_.getHpMax();
    param[5] = 0xa0000002;
    param[6] = info->haveStatus_.getMp();
    param[7] = (int)L"\xfeff\x24a7";
    param[8] = info->haveStatus_.getMpMax();
    param[9] = 0xa000000c;
    param[10] = (int)":";
    param[11] = info->getAttack(0);
    param[12] = 0xa000000d;
    param[13] = (int)":";
    param[14] = info->getDefence(0);
    int level[3];
    level[0] = 0xa0000013;
    level[1] = (int)":";
    level[2] = info->haveStatus_.level_;
    if (status::g_Game.language == French) {
        level[1] = (int)" :";
    }
    if (info->haveStatus_.isPlayer() == false) {
        level[2] = (int)"@?";
        parts = s_statusAltParts;
    }
    unkfunc_02050ee0(s_statusBigParts, param, x, y, g_menuTextColors[unkfunc_0201e2f4(info)]);
    unkfunc_02050ee0(parts, level, x, y, g_menuTextColors[unkfunc_0201e2f4(info)]);
    unkfunc_0201e194(frameX, frameY, 0x70, 0x68, lineY);
}

THUMB void unkfunc_0201e554(status::HaveStatusInfo* info, int pos)
{
    unkfunc_02050698(0, 0);
    int param[8];
    param[0] = info->haveStatus_.playerIndex_ + 0x50000000;
    param[1] = 0xa0000001;
    param[2] = info->getHp();
    param[3] = 0xa0000002;
    param[4] = info->getMp();
    switch (unkfunc_0201e318(info)) {
    case 2:
        param[5] = menu::MenuDataCommon::convMessage(info, 0xa0000014);
        s_memberParts[6].type_ = 0;
        s_memberParts[7].type_ = 0;
        s_memberParts[5].x_ = 7;
        s_memberParts[5].w_ = 0x30;
        break;
    case 3:
        param[5] = menu::MenuDataCommon::convMessage(info, 0xa0000018);
        s_memberParts[6].type_ = 0;
        s_memberParts[7].type_ = 0;
        s_memberParts[5].x_ = 7;
        s_memberParts[5].w_ = 0x30;
        break;
    default:
        if (info->haveStatus_.isPlayer() == false) {
            param[5] = 0xa000003f;
            param[6] = (int)":";
            s_memberParts[6].type_ = 0xd;
            s_memberParts[7].type_ = 0xd;
            param[7] = (int)"@?";
            s_memberParts[5].x_ = 8;
            s_memberParts[5].w_ = 0x10;
        } else {
            param[5] = 0xa000003f;
            param[6] = (int)":";
            param[7] = info->haveStatus_.level_;
            s_memberParts[6].type_ = 0xd;
            s_memberParts[7].type_ = 0xf;
            s_memberParts[5].x_ = 8;
            s_memberParts[5].w_ = 0x10;
        }
        break;
    }
#ifdef DQIV_NATIVE
    // The member's portrait left of the name, drawn at high resolution, in a
    // name band 4 pixels taller.
    static const short s_nativeY[] = { 0x0c, 0x24, 0x24, 0x32, 0x32, 0x40, 0, 0x40 };
    for (int i = 0; i < 8; i++) {
        s_memberParts[i].y_ = s_nativeY[i];
    }
    // Japanese names (four full-width characters) need the whole band.
    const bool portrait = status::g_Game.language != Japanese;
    s_memberParts[0].x_ = portrait ? 0x1a : 0x08;
    s_memberParts[0].w_ = portrait ? 0x24 : 0x30;
    void* icon = portrait ? unkfunc_0205182c(0xf1000000, info->haveStatus_.iconIndex_) : NULL;
    if (icon != NULL && unkfunc_0207f52c(icon) == 32 * 32) {
        unsigned char* picture = (unsigned char*)unkfunc_0207f590(icon);
        pc::placeHdPicture(0, (pos << 6) + data_021098a4 + 4, data_021098a8 + 5, 20, 20,
                           std::span<const std::uint8_t>(picture, 32 * 32));
        unkfunc_0207f840(&data_0211a60c, picture);
    }
#endif
    unkfunc_02050ee0(s_memberParts, param, pos << 6, 0, g_menuTextColors[unkfunc_0201e2f4(info)]);
}

THUMB int unkfunc_0201e674(int action)
{
    return status::UseAction::getWordDBIndex(action) + 0x70000000;
}
