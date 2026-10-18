#pragma ipa file
#include "ov015/btl/BattleMenuDrawCommon.hpp"
#ifdef DQIV_NATIVE
#include "ov003/status/MonsterPartyWithDraw.hpp"
#include "main/dss/Display.hpp"
#include "main/dss/VramRequest.hpp"
#include <stdlib.h>
#endif
#include "ov015/btl/BattleMenuPlayerControl.hpp"
#include "ov015/btl/BattleMonsterNamePlate.hpp"
#include "ov003/btl/BattleMenuFace.hpp"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/menu/MenuPartsDraw.hpp"
#include "main/menu/MenuCommonDraw.hpp"

// parts lists (their definition order sets the .data layout)
static MenuPartsEntry s_partsBattleHpPercent[] = {
    { 0x01, 0x00, (short)0xf000, 0, 0, 0, 0x40, 0x20 },
    { 0x0d, 0x08, (short)0xf000, 0, 8, 6, 0x30, 0x10 },
    { 0x07, 0x00, (short)0xf000, 0x11, 8, 0, 0x30, 8 },
    { 0x11, 0x00, 0, 1, 0, 0x10, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleMemberIcon[] = {
    { 0x14, 0xc2, 0, 0, 4, 0xe, 0x20, 0x18 },
    { 0x0c, 0xf1, 0, 0, 0, 0, 0x20, 0x20 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleMpCostFrame[] = {
    { 0x01, 0x00, (short)0xf000, 0, 0, 0, 0x58, 0x20 },
    { 0x07, 0x00, (short)0xf000, 0x19, 0x50, 8, 8, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleMemberSelectArrow[] = {
    { 0x0c, 0xf4, 0, 0, 0, 0, 0x18, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleCharaPower[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0xa, 0x44, 0x38, 0xa },
    { 0x0d, 0x08, (short)0xf000, 4, 0x3c, 0x44, 0xa, 0xa },
    { 0x0f, 0x0b, (short)0xf000, 1, 0, 0, 0x1a, 0xa },
    { 0x0d, 0x08, (short)0xf000, 2, 0xa, 0x54, 0x38, 0xa },
    { 0x0d, 0x08, (short)0xf000, 4, 0x3c, 0x54, 0xa, 0xa },
    { 0x0f, 0x0b, (short)0xf000, 3, 0, 0, 0x1a, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleActionName[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 6, 7, 0x60, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleTopStatusFrame[] = {
    { 0x01, 0x00, (short)0xf000, 0, 0, 0, 0x40, 0x48 },
    { 0x17, 0x00, (short)0xf000, 1, 0, 0x10, 0x40, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleMemberIconSlot0[] = {
    { 0x09, 0x00, 0, 0, 0, 0, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleMemberIconSlot3[] = {
    { 0x09, 0x00, 3, 0, 0, 0, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleCommandIcon[] = {
    { 0x0c, 0xf4, 0, 0, 0x28, 0, 0x18, 0x18 },
    { 0x0d, 0x08, (short)0xf000, 1, 8, 0xa, 0x28, 0x10 },
    { 0x01, 0x00, (short)0xf000, 0, 0, 0, 0x48, 0x20 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleStatusPanelLevel[] = {
    { 0x0d, 0x08, (short)0xf000, 0xd, 0x2c, 0x27, 0x10, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 7, 0x30, 0x27, 0x18, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleCommandDisabled[] = {
    { 0x15, 0xb5, 0, 0, 0, 0x10, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleStatusPanelLevelNpc[] = {
    { 0x0d, 0x08, (short)0xf000, 0xd, 0x2c, 0x27, 0x10, 0x10 },
    { 0x0d, 0x0a, (short)0xf000, 7, 0x30, 0x27, 0x18, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleCharaStatusFrame[] = {
    { 0x01, 0x00, (short)0xf000, 0, 0, 0, 0x68, 0x68 },
    { 0x16, 0x00, (short)0xf000, 0, 0, 0x18, 0x68, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleMemberFace[] = {
    { 0x0a, 0x00, 0, 0, 0, 0, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleItemEntry[] = {
    { 0x14, 0xc0, 0, -1, 0, 0xc, 0x68, 0x18 },
    { 0x0e, 0x08, (short)0xf000, 0, 8, 0xc, 0x38, 0x18 },
    { 0x0c, 0xf0, 0, 1, 0x48, 0, 0x20, 0x20 },
    { 0x0d, 0x08, (short)0xf000, 2, 0x48, 0x18, 0x10, 0xc },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleMemberSelectFrame[] = {
    { 0x01, 0x00, (short)0xf000, 0, 0, 0x70, 0xe0, 0x30 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleTopStatusLevel[] = {
    { 0x0d, 0x08, (short)0xf000, 4, 0x14, 0x34, 0x10, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 7, 0x18, 0x34, 0x20, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleCharaLevel[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x46, 0xa, 0x10, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 1, 0x44, 0xa, 0x18, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleActionHistoryValue[] = {
    { 0x0e, 0x09, (short)0xf000, 0, 2, 8, 0x3e, 0x30 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleCharaHpMp[] = {
    { 0x0d, 0x09, (short)0xf000, 0, 8, 0xa, 0x30, 0xa },
    { 0x0d, 0x08, (short)0xf000, 1, 0x38, 0xa, 0x28, 0xa },
    { 0x0d, 0x08, (short)0xf000, 2, 0xc, 0x24, 0x18, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 3, 0x1c, 0x24, 0x20, 0xa },
    { 0x0d, 0x08, (short)0xf000, 8, 0x3c, 0x24, 0xa, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 4, 0x3c, 0x24, 0x20, 0xa },
    { 0x0d, 0x08, (short)0xf000, 5, 0xc, 0x34, 0x18, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 6, 0x1c, 0x34, 0x20, 0xa },
    { 0x0d, 0x08, (short)0xf000, 8, 0x3c, 0x34, 0xa, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 7, 0x3c, 0x34, 0x20, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleCharaLevelNpc[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x46, 0xa, 0x10, 0xa },
    { 0x0d, 0x0a, (short)0xf000, 1, 0x44, 0xa, 0x18, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleTopStatusLevelNpc[] = {
    { 0x0d, 0x08, (short)0xf000, 4, 0x14, 0x34, 0x10, 0xa },
    { 0x0d, 0x0a, (short)0xf000, 7, 0x18, 0x34, 0x20, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleActionHistory[] = {
    { 0x0e, 0x09, (short)0xf000, 0, 2, 0, 0x3e, 0x14 },
    { 0x0d, 0x09, (short)0xf000, 1, 2, 0x14, 0x3e, 0xa },
    { 0x0e, 0x09, (short)0xf000, 2, 4, 0x20, 0x38, 0x20 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleTargetCursor[] = {
    { 0x15, 0xb4, 0, 0, 0, 0, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleHelpMessage[] = {
    { 0x0e, 0x08, (short)0xf000, 0, 0, 0, 0xc8, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleMemberIconSlot2[] = {
    { 0x09, 0x00, 2, 0, 0, 0, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleConditionMarks[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 0x1c, 8, 0xa, 0xa },
    { 0x0d, 0x08, (short)0xf000, 1, 0x1c, 0x13, 0xa, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleTopStatus[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 8, 0x30, 0xa },
    { 0x0d, 0x08, (short)0xf000, 1, 8, 0x1a, 0x18, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 2, 8, 0x26, 0x18, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 3, 8, 0x34, 0x30, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 5, 0x18, 0x1a, 0x20, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 6, 0x18, 0x26, 0x20, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleCancelButton[] = {
    { 0x0c, 0xf4, 0, 0, 0xe0, 0xa0, 0x18, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattlePageNumber[] = {
    { 0x0f, 0x08, (short)0xf000, 0, 6, 4, 0xa, 0x10 },
    { 0x0d, 0x0b, (short)0xf000, 1, 0, 0, 0xa, 0x10 },
    { 0x0f, 0x0b, (short)0xf000, 2, 0, 0, 0xa, 0x10 },
    { 0x0c, 0xf4, 0, 3, 8, 0x10, 0x18, 0x18 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleMemberIconSlot1[] = {
    { 0x09, 0x00, 1, 0, 0, 0, 0, 0 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleMpCost[] = {
    { 0x0d, 0x08, (short)0xf000, 0, 8, 0xa, 0x10, 0xa },
    { 0x0f, 0x0a, (short)0xf000, 1, 0x10, 0xa, 0x20, 0xa },
    { 0x0d, 0x08, (short)0xf000, 2, 0x30, 0xa, 0x10, 0xa },
    { 0x0f, 0x0b, (short)0xf000, 3, 0, 0, 0x1e, 0xa },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};
static MenuPartsEntry s_partsBattleStatusPanel[] = {
    { 0x14, 0xc0, 0, -1, 0, 0, 0x70, 0x38 },
    { 0x0c, 0xf1, 0, 0, -4, 0x10, 0x20, 0x20 },
    { 0x0d, 0x08, (short)0xf000, 1, 0x1c, 3, 0x50, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 2, 0x20, 0x27, 0x50, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 3, 0x20, 0x10, 0x10, 0x10 },
    { 0x0d, 0x09, (short)0xf000, 4, 0x44, 0x10, 0x10, 0x10 },
    { 0x0d, 0x08, (short)0xf000, 5, 0x20, 0x1a, 0x10, 0x10 },
    { 0x0d, 0x09, (short)0xf000, 6, 0x44, 0x1a, 0x10, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 8, 0x30, 0x10, 0x18, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 9, 0x50, 0x10, 0x18, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 0xa, 0x30, 0x1a, 0x18, 0x10 },
    { 0x0f, 0x0a, (short)0xf000, 0xb, 0x50, 0x1a, 0x18, 0x10 },
    { 0x0f, 0x09, (short)0xf000, 0xc, 0, 3, 0x20, 0x10 },
    { 0xff, 0x00, 0, 0, 0, 0, 0, 0 },
};

THUMB void drawBattleCommandIcons(int command0, int command1, int command2, int command3)
{
    drawBattleCommandIcon(command0, 0x40, 0x80, 1);
    drawBattleCommandIcon(command1, 0x88, 0x80, 1);
    drawBattleCommandIcon(command2, 0x40, 0xa0, 1);
    drawBattleCommandIcon(command3, 0x88, 0xa0, 1);
}

THUMB void drawBattleMemberHpPercent(int x, int y, int hp, status::HaveStatusInfo* info)
{
    int index = info->haveStatus_.playerIndex_;
    status::g_Party.getSortIndex(index);
    int hpMax = info->getHpMax();
    int param[2];
    param[0] = index + 0x50000000;
    param[1] = hp * 100 / hpMax;
    if (param[1] <= 0 && hp > 0) {
        param[1] = 1;
    }
    if (hp > hpMax / 4 - 1 && param[1] < 26) {
        param[1] = 26;
    }
    drawMenuParts(s_partsBattleHpPercent, param, x, y, g_menuTextColors[getBattleHpColorIndexByHp(info, hp)]);
}

THUMB void drawBattleActiveMember(status::HaveStatusInfo* info, int hp, int flag)
{
    drawBattleMemberHpPercent(0, 0xa0, hp, info);
    drawBattleMemberFace(8, 0x72, info, flag);
}

#ifdef DQIV_NATIVE
// A group's health, like the HD-2D remakes: 1 when one of its monsters is at
// a quarter of its HP or less, 2 under a tenth.
static int unkfunc_native_groupHealth(int group)
{
    int health = 0;
    for (int i = 0; i < g_monster.getCount(); i++) {
        if (g_monster.getMonsterGroup(i) != group || !g_monster.getMonsterStatus(i)->isBattleEnable()) {
            continue;
        }
        status::HaveStatusInfo& info = g_monster.getMonsterStatus(i)->haveStatusInfo_;
        const int hp = info.getHp(), hpMax = info.getHpMax();
        if (hpMax <= 0) {
            continue;
        }
        if (hp * 10 < hpMax) {
            return 2;
        }
        if (hp * 4 <= hpMax) {
            health = 1;
        }
    }
    return health;
}

// OBJ palettes 10-13 (free in battle): the name plates' bright (15) and dim
// (14) palettes with the letters in yellow, then in red (the plates' frame
// keeps its palette).
static void unkfunc_native_platePalettes()
{
    static const unsigned short letters[2][2] = {
        { GX_RGB(31, 29, 6), GX_RGB(24, 20, 2) },
        { GX_RGB(31, 6, 6), GX_RGB(22, 3, 3) },
    };
    for (int screen = 0; screen < 2; screen++) {
        PaletteBuffer* obj = getObjPaletteBuffer(screen);
        for (int color = 0; color < 2; color++) {
            for (int dim = 0; dim < 2; dim++) {
                const int index = 10 + color * 2 + dim;
                obj->loadColors(index, 0, &obj->color_[(dim ? 14 : 15) * 16], 16);
                // The letters are colour 1 (2 is their edge).
                for (int c = 0; c < 2; c++) {
                    unsigned short rgb = letters[color][c];
                    if (dim) {
                        // Half as bright, like palette 14.
                        rgb = (unsigned short)(((rgb >> 1) & 0x3def));
                    }
                    obj->setColor(index, 1 + c, rgb);
                }
            }
        }
    }
}
#endif

THUMB void drawMonsterNamePlates(int group)
{
    int plateGroup;
    int count = BattleMonsterNamePlate::getSingleton().addCount_;
    if (count == 0) {
        return;
    }
#ifdef DQIV_NATIVE
    unkfunc_native_platePalettes();
#endif
    for (int i = 0; i < count; i++) {
        BattleMonsterNamePlate::Monster_DATA& data = BattleMonsterNamePlate::getSingleton().monsterData_[i];
        int drawID = data.drawID;
        int y = data.height;
        plateGroup = data.group;
        int x = data.center - (data.leng >> 1);
        if (group == plateGroup || group == -1) {
            showNamePlate(i, x, y, drawID, 1);
            if (group == plateGroup) {
                drawMenuPartsAt(s_partsBattleTargetCursor, 0, x, y + 8);
            }
        } else {
            showNamePlate(i, x, y, drawID, 0);
        }
#ifdef DQIV_NATIVE
        if (const int health = unkfunc_native_groupHealth(plateGroup)) {
            const bool bright = group == plateGroup || group == -1;
            unkfunc_native_platePalette(i, 10 + (health - 1) * 2 + (bright ? 0 : 1));
        }
#endif
    }
}

THUMB void drawBattleCommandIcon(int command, int x, int y, int flag)
{
    int param[2] = { -1, 0 };
    switch (command) {
    case 0x01:
        param[0] = 6;
        param[1] = 0x80000000;
        break;
    case 0x02:
        param[0] = 1;
        param[1] = 0x80000002;
        break;
    case 0x04:
        param[0] = 9;
        param[1] = 0x80000001;
        break;
    case 0x08:
        param[0] = 10;
        param[1] = 0x80000003;
        break;
    case 0x10:
        param[0] = 6;
        param[1] = 0x80000004;
        break;
    case 0x20:
        param[0] = 2;
        param[1] = 0x80000005;
        break;
    case 0x40:
        param[0] = 3;
        param[1] = 0x80000006;
        break;
    case 0x80:
        param[0] = 7;
        param[1] = 0x80000007;
        break;
    default:
        return;
    }
    if (flag == 0 && param[0] != -1) {
        drawMenuParts(s_partsBattleCommandIcon, param, x, y, g_menuTextColors[0]);
        drawMenuPartsAt(s_partsBattleCommandDisabled, 0, x, y);
    } else {
        drawMenuParts(s_partsBattleCommandIcon, param, x, y, g_menuTextColors[0]);
    }
}

THUMB void drawBattleHelpMessage(int x, int y, int message)
{
    drawMenuParts(s_partsBattleHelpMessage, &message, x, y, g_menuTextColors[0]);
}

THUMB void drawBattlePageNumber(int x, int y, int max, int page)
{
    if (max != 0) {
        int param[4] = { 0, (int)"\xff\xfe\xa7\x24", 0, 0xd };
        param[0] = page + 1;
        param[2] = max + 1;
        drawMenuParts(s_partsBattlePageNumber, param, x, y - 8, g_menuTextColors[0]);
    }
}

THUMB void drawBattleItemEntry(int x, int y, int item, int flag)
{
    int param[3] = { 0, 0, (int)"\xff\xfe\xa6\x24" };
    param[0] = item + 0x40000000;
    param[1] = item;
    if (flag) {
        param[2] = 0xa0000041;
    }
    drawMenuParts(s_partsBattleItemEntry, param, x, y, g_menuTextColors[0]);
}

THUMB void drawBattleActionName(int x, int y, int action)
{
    int param = getActionNameMessage(action);
    drawMenuParts(s_partsBattleActionName, &param, x, y, g_menuTextColors[0]);
}

THUMB void drawBattleMemberStatusPanel(status::HaveStatusInfo* info, int index)
{
    int playerIndex = info->haveStatus_.playerIndex_;
    int param[14];
    param[0] = info->haveStatus_.iconIndex_;
    param[1] = playerIndex + 0x50000000;
    param[2] = 0xa0000013;
    param[3] = 0xa0000011;
    param[4] = (int)"\xff\xfe\xa7\x24";
    param[5] = 0xa0000012;
    param[6] = (int)"\xff\xfe\xa7\x24";
    param[7] = info->haveStatus_.level_;
    param[8] = info->haveStatus_.getHp();
    param[9] = info->haveStatus_.getHpMax();
    param[10] = info->haveStatus_.getMp();
    param[11] = info->haveStatus_.getMpMax();
    param[12] = index + 1;
    param[13] = (int)":";
    param[2] = btl::BattleMenuPlayerControl::getSingleton()->getCondition(index);
    int color = btl::BattleMenuPlayerControl::getSingleton()->getHPColor(index);
    int x = (index % 2) * 120 + 12;
    int y = (index / 2) * 64 + 20;
    drawMenuParts(s_partsBattleStatusPanel, param, x, y, g_menuTextColors[color]);
    if (!btl::BattleMenuPlayerControl::getSingleton()->isConditionChange(index)) {
        if (info->haveStatus_.isBattleNpc_) {
            param[7] = 0xa0000073;
            drawMenuParts(s_partsBattleStatusPanelLevelNpc, param, x, y, g_menuTextColors[color]);
        } else {
            drawMenuParts(s_partsBattleStatusPanelLevel, param, x, y, g_menuTextColors[color]);
        }
    }
}

THUMB void drawBattleCharaStatus(int x, int y, status::HaveStatusInfo* info)
{
    status::g_Party.getSortIndex(info->haveStatus_.playerIndex_);
    int color = getBattleHpColorIndex(info);
    drawMenuPartsAt(s_partsBattleCharaStatusFrame, 0, x, y);
    int level[2] = { (int)":", 0 };
    level[1] = info->haveStatus_.level_;
    int status[9] = { 0, 0xa0000013, 0xa0000001, 0, 0, 0xa0000002, 0, 0, (int)"\xff\xfe\xa7\x24" };
    status[0] = info->haveStatus_.playerIndex_ + 0x50000000;
    status[3] = info->getHp();
    status[4] = info->getHpMax();
    status[6] = info->getMp();
    status[7] = info->getMpMax();
    int power[5] = { 0xa000000c, 0, 0xa000000d, 0, (int)":" };
    power[1] = info->getAttack(0);
    power[3] = info->getDefence(0);
    if (info->haveStatus_.isBattleNpc_) {
        level[1] = 0xa0000073;
        drawMenuParts(s_partsBattleCharaLevelNpc, level, x, y, g_menuTextColors[color]);
    } else {
        drawMenuParts(s_partsBattleCharaLevel, level, x, y, g_menuTextColors[color]);
    }
    drawMenuParts(s_partsBattleCharaHpMp, status, x, y, g_menuTextColors[color]);
    drawMenuParts(s_partsBattleCharaPower, power, x, y, g_menuTextColors[color]);
}

THUMB void drawBattleMemberSelectList(int* list, int count, int page)
{
    drawMenuPartsPlain(s_partsBattleMemberSelectFrame, 0);
    int pageMax = (count - 1) / 4;
    int num = (count - 1) % 4 + 1;
    if (pageMax != page) {
        num = 4;
    }
    for (int i = 0; i < num; i++) {
        int x = i * 40 + 16;
        status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(list[page * 4 + i])->haveStatusInfo_;
        drawBattleMemberIcon(x, 0x70, info->haveStatus_.iconIndex_);
        drawBattleMemberConditionMarks(info, x, 0x70);
    }
    int param = 0xd;
    if (pageMax > 0) {
        drawMenuParts(s_partsBattleMemberSelectArrow, &param, 0xc4, 0x80, g_menuTextColors[0]);
    }
}

THUMB void drawBattleMemberConditionMarks(status::HaveStatusInfo* info, int x, int y)
{
    int param[3] = { (int)" ", (int)" ", (int)" " };
    int i = 0;
    status::g_Party.getSortIndex(info->haveStatus_.playerIndex_);
    if (info->isDeath()) {
        param[i++] = 0xa0000042;
    } else if (info->isPoison()) {
        param[i++] = 0xa0000043;
    }
    if (info->isSpell()) {
        param[i] = 0xa0000044;
    }
    drawMenuParts(s_partsBattleConditionMarks, param, x, y, g_menuTextColors[getBattleHpColorIndex(info)]);
}

THUMB void drawBattleCancelButton()
{
    int param = 5;
    drawMenuPartsPlain(s_partsBattleCancelButton, &param);
}

THUMB void drawBattleTopStatusWindow(status::HaveStatusInfo* info, int a, int b, int c, int index)
{
    setMenuDrawOffset(0, 0);
    int x = index * 64;
    drawMenuPartsAt(s_partsBattleTopStatusFrame, 0, x, 0x78);
    int color = btl::BattleMenuPlayerControl::getSingleton()->getHPColor(index);
    int param[8];
    dss::memset(param, 0, sizeof(param));
    param[0] = info->haveStatus_.playerIndex_ + 0x50000000;
    param[1] = 0xa0000001;
    param[2] = 0xa0000002;
    param[3] = 0xa0000013;
    param[4] = (int)":";
    param[5] = a;
    param[6] = b;
    param[7] = c;
    param[3] = btl::BattleMenuPlayerControl::getSingleton()->getCondition(index);
    if (!btl::BattleMenuPlayerControl::getSingleton()->isConditionChange(index)) {
        if (info->haveStatus_.isBattleNpc_) {
            param[7] = 0xa0000073;
            drawMenuParts(s_partsBattleTopStatusLevelNpc, param, x, 0x78, g_menuTextColors[color]);
        } else {
            drawMenuParts(s_partsBattleTopStatusLevel, param, x, 0x78, g_menuTextColors[color]);
        }
    }
    drawMenuParts(s_partsBattleTopStatus, param, x, 0x78, g_menuTextColors[color]);
}

THUMB void drawBattleActionHistory(int index, int value, int message, int color)
{
    setMenuDrawOffset(0, 0);
    int x = index * 64;
    if (message != -1) {
        int param[3] = { 0, (int)"\xff\xfe\xbe\x24", 0 };
        param[2] = message;
        param[0] = value;
        drawMenuParts(s_partsBattleActionHistory, param, x, 0x10, g_menuTextColors[color]);
    } else if (value != -1) {
        drawMenuParts(s_partsBattleActionHistoryValue, &value, x, 0x10, g_menuTextColors[color]);
    }
}

THUMB void drawBattleMemberIconInSlot(int x, int y, int value, int type)
{
    switch (type) {
    case 0:
        drawMenuPartsAt(s_partsBattleMemberIconSlot0, &value, x, y);
        break;
    case 1:
        drawMenuPartsAt(s_partsBattleMemberIconSlot1, &value, x, y);
        break;
    case 2:
        drawMenuPartsAt(s_partsBattleMemberIconSlot2, &value, x, y);
        break;
    case 3:
        drawMenuPartsAt(s_partsBattleMemberIconSlot3, &value, x, y);
        break;
    }
}

THUMB void drawBattleMpCost(int mp, int useMp)
{
    drawMenuPartsAt(s_partsBattleMpCostFrame, 0, 0x88, 0xa0);
    int param[4] = { 0xa000003e, 0, (int)"\xff\xfe\xa7\x24", 0 };
    param[1] = useMp;
    param[3] = mp;
    drawMenuParts(s_partsBattleMpCost, param, 0x88, 0xa0, g_menuTextColors[0]);
}

THUMB void drawBattleMemberIcon(int x, int y, int icon)
{
    drawMenuPartsAt(s_partsBattleMemberIcon, &icon, x, y);
}

THUMB void drawBattleMemberFace(int x, int y, status::HaveStatusInfo* info, int flag)
{
    int index = info->haveStatus_.playerIndex_;
    if (flag == 1) {
        int icon = info->haveStatus_.iconIndex_;
        drawMenuPartsAt(s_partsBattleMemberFace, &icon, x, y);
        BattleMenuFace::getSingleton()->setDisplayOn(index, x, y);
    } else {
        BattleMenuFace::getSingleton()->setDisplayOff(index);
    }
}

THUMB int getBattleHpColorIndex(status::HaveStatusInfo* info)
{
    info->getHp();
    if (info->isDeath()) {
        return 2;
    }
    if (info->setNearDeath()) {
        return 1;
    }
    return 0;
}

THUMB int getBattleHpColorIndexByHp(status::HaveStatusInfo* info, int hp)
{
    if (hp == 0) {
        return 2;
    }
    if (hp <= info->getHpMax() / 4 - 1) {
        return 1;
    }
    return 0;
}

THUMB void drawBattleRootMenu(int flag)
{
    int command0;
    int command1;
    int command2;
    int command3;
    if (flag == 0) {
        command0 = 1;
        command1 = 2;
        command2 = 4;
        command3 = 8;
    } else {
        command0 = 1;
        command1 = 8;
        command2 = 0;
        command3 = 0;
    }
    drawBattleCommandIcons(command0, command1, command2, command3);
}

THUMB void drawBattleActionMenu(int flag)
{
    int chara = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
    status::PlayerStatus* player = status::g_Party.getPlayerStatus(chara);
    drawBattleActiveMember(&player->haveStatusInfo_, btl::BattleMenuPlayerControl::getSingleton()->memberHP_[chara], 1);
    int command0 = (flag & 0x10) ? 0x10 : 0;
    int command1 = (flag & 0x20) ? 0x20 : 0;
    int command2 = (flag & 0x40) ? 0x40 : 0;
    int command3 = (flag & 0x80) ? 0x80 : 0;
    drawBattleCommandIcons(command0, command1, command2, command3);
    drawMonsterNamePlates(-1);
    drawBattleCancelButton();
}

// not in the ROM (dead-stripped), its local array initializer is still in .rodata
THUMB void unkfunc_unused_12(int index)
{
    int message[8] = { 0xa0000019, 0xa000001a, 0xa0000016, 0xa0000017, 0xa0000015, 0xa0000018, 0xa0000014, 0 };
    drawMenuParts(s_partsBattleHelpMessage, &message[index], 0, 0, g_menuTextColors[0]);
}
