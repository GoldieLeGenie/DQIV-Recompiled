#include "main/menu/MenuDataCommon.hpp"
#include "main/status/UseItem.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/cmn/CommonRuraData.hpp"

using namespace menu;

char takai[] = "@\x82\xbd\x82\xa9\x82\xa2";   // 0x020bebd8 "@たかい"
const SuretigaiSexTable       suretigaiSexTable_       = { { 0xa0000005, 0xa0000006, 0xa0000259 } };                                                                                                                                                                                                                                                                                                                                                                            // 0x020b5dbc
const OpponentTable           opponentTable_           = { { 0x000000af, 0x000000b0, 0x000000b1, 0x000000b2, 0x000000aa } };                                                                                                                                                                                                                                                                                                                                                    // 0x020b5dc8
const EquipKindTable          equipKindTable_          = { { 0xa0000024, 0xa0000025, 0xa0000026, 0xa0000027, 0xa0000028 } };                                                                                                                                                                                                                                                                                                                                                    // 0x020b5ddc
const SuretigaiRootMenuTable  suretigaiRootMenuTable_  = { { 0x800001f4, 0x800001f5, 0x800001f6, 0x800001f7, 0x800000cf } };                                                                                                                                                                                                                                                                                                                                                    // 0x020b5df0
const ItemUseCommandTable     ItemUseCommand_          = { { 0x80000069, 0x8000006a, 0x8000006d, 0x8000006b, 0x8000006c, 0x800000cf } };                                                                                                                                                                                                                                                                                                                                        // 0x020b5e04
const RootMenuKindTable       rootMenuKindTable_       = { { 0x00000008, 0x00000002, 0x00000003, 0x00000000, 0x00000004, 0x00000001 } };                                                                                                                                                                                                                                                                                                                                        // 0x020b5e1c
const RootMenuNameTable       rootMenuNameTable_       = { { 0x80000078, 0x80000064, 0x80000065, 0x80000068, 0x80000066, 0x80000067 } };                                                                                                                                                                                                                                                                                                                                        // 0x020b5e34
const AbilityKindTable        abilityKindTable_        = { { 0xa000000c, 0xa000000d, 0xa0000007, 0xa0000008, 0xa0000009, 0xa000000a, 0xa000000b, 0xa000000e, 0xa000000f } };                                                                                                                                                                                                                                                                                                    // 0x020b5e4c
const ItemMessageTable        itemMessageTable_        = { { 0xa0000259, 0x50000067, 0x50000068, 0x50000069, 0x5000006a, 0x5000006b, 0x5000006c, 0x5000006d, 0x5000006e, 0x5000006f, 0x50000070, 0x50000071, 0x50000072, 0x50000073, 0x50000074, 0x50000075, 0x50000076, 0x50000077, 0x50000078, 0x50000079, 0x5000007a, 0x5000007b, 0x5000007c, 0x5000007d } };                                                                                                                 // 0x020b5e70
const RuraMoveName            ruraMoveName_            = { { 0xc0000010, 0xc0000011, 0xc0000012, 0xc0000013, 0xc0000014, 0xc0000015, 0xc0000016, 0xc0000017, 0xc0000018, 0xc0000019, 0xc000001a, 0xc000001b, 0xc000001c, 0xc000001d, 0xc000001f, 0xc0000020, 0xc0000021, 0xc0000022, 0xc0000023, 0xc0000024, 0xc0000025, 0xc0000026, 0xc0000027, 0xc0000028, 0xc0000029, 0xc000002a, 0xc0000005, 0xc0000008, 0xc000000c, 0xc0000033, 0xc0000001, 0xc0000002, 0xc0000007, 0xc0000003, 0xc0000004, 0xc0000006, 0xc000000a, 0xc0000009, 0xc000000b, 0xc000000d, 0xc000000e, 0xc000000f } };  // 0x020b5ed0
char hikui[] = "@\x82\xd0\x82\xad\x82\xa2";   // 0x020bebe0 "@ひくい"


THUMB int menu::MenuDataCommon::getRootMenuKind(int index)
{
    RootMenuKindTable table = rootMenuKindTable_;
    return table.v[index];
}

THUMB int menu::MenuDataCommon::getRootMenuName(int index)
{
    RootMenuNameTable table = rootMenuNameTable_;
    return table.v[index];
}

THUMB int menu::MenuDataCommon::getEquipKind(int index)
{
    EquipKindTable table = equipKindTable_;
    return table.v[index];
}

THUMB int menu::MenuDataCommon::getAbilityKind(int index)
{
    AbilityKindTable table = abilityKindTable_;
    return table.v[index];
}

THUMB int menu::MenuDataCommon::getStatus(int index, status::HaveStatusInfo statusInfo)
{
    bool isBattleNpc = statusInfo.haveStatus_.isBattleNpc_;
    if (isBattleNpc == true)
    {
        unsigned short playerIndex = statusInfo.haveStatus_.playerIndex_;
        if (index == 0) return status::excelParam.getCharaInitData()[playerIndex].attack;
        if (index == 1) return status::excelParam.getCharaInitData()[playerIndex].defence;
        if (index == 2) return status::excelParam.getCharaInitData()[playerIndex].strength;
        if (index == 3) return status::excelParam.getCharaInitData()[playerIndex].agility;
        if (index == 4) return status::excelParam.getCharaInitData()[playerIndex].vitality;
        if (index == 5) return status::excelParam.getCharaInitData()[playerIndex].intelligence;
        if (index == 6) return status::excelParam.getCharaInitData()[playerIndex].luck;
        if (index == 7) return status::excelParam.getCharaInitData()[playerIndex].HP;
        if (index == 8) return status::excelParam.getCharaInitData()[playerIndex].MP;
    }
    else
    {
        if (index == 0) return statusInfo.getAttack(0);
        if (index == 1) return statusInfo.getDefence(0);
        if (index == 2) return statusInfo.getStrength(0);
        if (index == 3) return statusInfo.getAgility(0);
        if (index == 4) return statusInfo.getProtection(0);
        if (index == 5) return statusInfo.getWisdom(0);
        if (index == 6) return statusInfo.getLuck(0);
        if (index == 7) return statusInfo.getHpMax();
        if (index == 8) return statusInfo.getMpMax();
    }
    return 0;
}


THUMB int menu::MenuDataCommon::getItemUseCommand(int index)
{
    ItemUseCommandTable commands = ItemUseCommand_; 
    return commands.cmd[index];
}


THUMB int menu::MenuDataCommon::getRuraName(int index) 
{
    RuraMoveName names = ruraMoveName_; 

    switch (status::g_Story.chapter_) { 
    case 1:
        if (index == 0xF)  index = 0x1E;
        if (index == 0x10) index = 0x1F;
        break;
    case 2:
        if (index == 0x1) index = 0x20;
        if (index == 0xB) index = 0x21;
        if (index == 0xC) index = 0x22;
        if (index == 0xD) index = 0x23;
        break;
    case 3:
        if (index == 0x1) index = 0x24;
        if (index == 0x1) index = 0x25; 
        break;
    case 4:
        if (index == 0xA)  index = 0x26;
        if (index == 0x8)  index = 0x27;
        if (index == 0x9)  index = 0x28;
        if (index == 0x15) index = 0x29;
        break;
    case 5:
    case 6:
        if (index == 0xD) {
            if (cmn::CommonRuraData::getSingleton()->getRenameFlag() == 1) {
                index = 0x1D;
            }
        }
        break;
    }
    return names.name[index];
}


THUMB void menu::MenuDataCommon::getShopCommand(int* command, bool bank)
{
    int base;
    if (bank) {
        base = 0x8000012C;
    } else {
        base = 0x800000C8;
    }
    command[0] = base;
    command[1] = base + 1;
    command[2] = 0x800000CF;
}



THUMB void menu::MenuDataCommon::getHighAndLowCommand(int* command, int win) 
{
    if (win) {
        int base = 0xA0000064;
        command[0] = base;
        command[1] = base + 1;
        return;
    }
    command[0] = (int)takai;
    command[1] = (int)hikui;
}

THUMB void menu::MenuDataCommon::getChurchCommand(int* command, bool extra) 
{
    if (extra) {
        int base = 0x800000CB;
        command[0] = base;
        command[1] = base + 1;
        command[2] = base + 2;
        command[3] = base + 3;
        command[4] = base + 4;
        return;
    }
    int base = 0x800000CA;
    command[0] = base;
    command[1] = base + 1;
    command[2] = base + 2;
    command[3] = base + 3;
    command[4] = base + 4;
    command[5] = base + 5;
}

THUMB int menu::MenuDataCommon::getOpponent(int index)
{
    OpponentTable table = opponentTable_;
    return table.v[index];
}

THUMB int menu::MenuDataCommon::getSuretigaiRootMenu(int index)
{
    SuretigaiRootMenuTable table = suretigaiRootMenuTable_;
    return table.v[index];
}

THUMB int menu::MenuDataCommon::getSuretigaiSex(int index)
{
    SuretigaiSexTable table = suretigaiSexTable_;
    return table.v[index];
}

THUMB int menu::MenuDataCommon::getSurechigaiAetas(int index)
{
    return index - 0x5FFFFD3F;
}

THUMB int menu::MenuDataCommon::getSurechigaiSkill(int index)
{
    return index - 0x1FFFFFFF;
}


THUMB int menu::MenuDataCommon::getItemMessage(int index)
{
    ItemMessageTable table = itemMessageTable_;
    return table.v[index];
}

THUMB int menu::MenuDataCommon::getItemMessageNo(int itemID)
{
    if (status::UseItem::getItemType(itemID) == ITEM_KEY ||
        itemID == 0x7F ||
        (unsigned int)(itemID - 0x82) <= 2)
    {
        return 0x8000008A;
    }
    if (itemID == 0x81)
    {
        return 0xA00002C0;
    }
    if (status::UseItem::getItemType(itemID) == ITEM_IMPORTANT)
    {
        if (itemID == 0x85 || itemID == 0x86 || itemID == 0x88 ||
            itemID == 0x8D || itemID == 0x8E ||
            (unsigned int)(itemID - 0x96) <= 3)
        {
            return 0x80000092;
        }
        if (itemID == 0x9C)
        {
            return 0x80000093;
        }
    }
    return 0;
}

THUMB int menu::MenuDataCommon::getItemMessageNo2(int itemID) {
    if (status::UseItem::getItemType(itemID) == ITEM_KEY)
    {
        return 0xCAF66;
    }
    if (itemID == 0x7F)
    {
        return 0xCAF66;
    }
    if (itemID == 0x84)
    {
        return 0xCAF66;
    }
    if (itemID == 0x81)
    {
        return 0xA00002C0;
    }
    if (status::UseItem::getItemType(itemID) == ITEM_IMPORTANT)
    {
        if (itemID == 0x89)
        {
            return 0xCAF68;
        }
        if (itemID == 0x9E)
        {
            return 0xCAF6A;
        }
        return 0xCAF66;
    }
    return 0;
}

THUMB int menu::MenuDataCommon::convMessage(status::HaveStatusInfo* info, int messageId)
{
    if (info->haveStatus_.sex_  == 1)   
    {
        switch (messageId)
        {
            case 0xA0000014: messageId = 0xA000001B; break;
            case 0xA0000015: messageId = 0xA000001C; break;
            case 0xA0000016: messageId = 0xA000001D; break;
            case 0xA0000017: messageId = 0xA000001E; break;
            case 0xA0000018: messageId = 0xA000001F; break;
            case 0xA0000019: messageId = 0xA0000020; break;
            case 0xA000001A: messageId = 0xA0000021; break;
        }
    }
    return messageId;
}



