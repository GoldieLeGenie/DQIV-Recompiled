#include "main/status/UseItem.hpp"
#include "main/status/UseAction.hpp"
#include "main/status/BaseAction.hpp"
#include "main/status/ExcelParam.hpp"



unsigned char IT_tbl[164] = {
    0x00, 0x24, 0x3A, 0x8D, 0x1C, 0x4B, 0x5A, 0x8A, 0x1E, 0x9A, 0x40, 0x5B, 0x10, 0x01, 0x9D, 0x94,
    0x70, 0x26, 0x23, 0x60, 0x92, 0x0A, 0x6A, 0x43, 0x8C, 0x4D, 0x1D, 0x98, 0x8B, 0x22, 0x25, 0x93,
    0x27, 0x90, 0x91, 0x63, 0x8F, 0x4C, 0x59, 0x56, 0x52, 0x1B, 0x8E, 0x06, 0x04, 0x17, 0x3E, 0x15,
    0x58, 0x11, 0x9F, 0x3D, 0x14, 0x19, 0x35, 0x02, 0x29, 0x13, 0x9E, 0x05, 0xA0, 0x3C, 0x1A, 0x12,
    0xA2, 0x28, 0x57, 0xA1, 0x3F, 0x03, 0x16, 0x2B, 0x18, 0x7B, 0x78, 0x7A, 0x79, 0x7D, 0x81, 0x80,
    0x7F, 0x7E, 0x7C, 0x33, 0x50, 0x45, 0x44, 0x61, 0x34, 0x48, 0x62, 0x42, 0x32, 0x47, 0x46, 0x2C,
    0x55, 0x2D, 0x2E, 0x2F, 0x31, 0x72, 0x0D, 0x0C, 0x0B, 0x77, 0x0F, 0x0E, 0x3B, 0x20, 0x4F, 0x4A,
    0x49, 0x08, 0x09, 0x30, 0x66, 0x84, 0x82, 0x86, 0x85, 0x87, 0x83, 0x37, 0x39, 0x38, 0x07, 0x95,
    0x36, 0x97, 0x54, 0x73, 0x64, 0x99, 0x4E, 0x67, 0x53, 0x75, 0x5C, 0x6B, 0x41, 0x5E, 0x89, 0x21,
    0x6C, 0x65, 0x9B, 0x5F, 0x88, 0x71, 0x96, 0x74, 0x1F, 0x6E, 0x2A, 0x9C, 0x51, 0x5D, 0x76, 0x68,
    0x6F, 0x6D, 0x69, 0x00
};  // data_020bd9c0

unsigned char FR_tbl[164] = {
    0x00, 0x16, 0x81, 0x53, 0x6C, 0x5A, 0x77, 0x58, 0x6E, 0x34, 0x4A, 0x78, 0x05, 0x2A, 0x59, 0x52,
    0x02, 0x15, 0x17, 0x7E, 0x55, 0x5E, 0x4B, 0x1A, 0x57, 0x90, 0x6D, 0x49, 0x4E, 0x14, 0x19, 0x56,
    0x1B, 0x4F, 0x50, 0x18, 0x51, 0x74, 0x76, 0x62, 0x5F, 0x6B, 0x54, 0xA2, 0xA0, 0x0B, 0x45, 0x0C,
    0x9F, 0x0A, 0x96, 0x44, 0x08, 0x09, 0x8A, 0x95, 0x1D, 0x48, 0x7B, 0x7C, 0x97, 0x01, 0x06, 0x07,
    0x94, 0x1C, 0x73, 0x7D, 0x47, 0x93, 0x0D, 0x32, 0x0E, 0x22, 0x20, 0x25, 0x24, 0x29, 0x28, 0x27,
    0x21, 0x23, 0x26, 0x38, 0x4C, 0x37, 0x6F, 0x7F, 0x3A, 0x35, 0x80, 0xA1, 0x3B, 0x36, 0x39, 0x30,
    0x4D, 0x2D, 0x2F, 0x31, 0x13, 0x99, 0x11, 0x10, 0x04, 0x3D, 0x12, 0x0F, 0x91, 0x63, 0x2E, 0x70,
    0x71, 0x5D, 0x03, 0x1F, 0x9B, 0x66, 0x64, 0x67, 0x68, 0x6A, 0x69, 0x3F, 0x40, 0x41, 0x5C, 0x83,
    0x88, 0x46, 0x5B, 0x98, 0x82, 0x3C, 0x2C, 0x85, 0x61, 0x42, 0x79, 0x89, 0x9D, 0x75, 0x9C, 0x72,
    0x8B, 0x9E, 0x2B, 0x33, 0x65, 0x92, 0x8F, 0x9A, 0x84, 0x8D, 0x1E, 0x3E, 0x60, 0x7A, 0x43, 0x86,
    0x8C, 0x8E, 0x87, 0x00
};  // data_020bd91c

unsigned char EN_tbl[164] = {
    0x00, 0x14, 0x64, 0x13, 0x3F, 0x0F, 0x42, 0x8B, 0x06, 0x83, 0x18, 0x38, 0x3B, 0x04, 0x40, 0x50,
    0x6D, 0x84, 0x4C, 0x35, 0x19, 0x1C, 0x20, 0x89, 0x1D, 0x6C, 0x24, 0x3C, 0x5E, 0x88, 0x56, 0x57,
    0x8A, 0x9F, 0xA0, 0x92, 0x0D, 0x1F, 0x17, 0x31, 0x26, 0x12, 0x69, 0x6B, 0x99, 0x48, 0x0E, 0x3E,
    0x3D, 0x29, 0x81, 0x15, 0x0B, 0x4D, 0x2A, 0x49, 0x6A, 0x1A, 0x11, 0x52, 0x27, 0x2C, 0x9D, 0x87,
    0x85, 0x01, 0x55, 0x74, 0xA2, 0x80, 0x36, 0x0A, 0x66, 0x4B, 0x79, 0x44, 0x6F, 0x5F, 0x91, 0x1B,
    0x9E, 0x4F, 0x68, 0x4A, 0x32, 0x34, 0x41, 0x43, 0x21, 0xA1, 0x16, 0x2F, 0x33, 0x4E, 0x67, 0x5A,
    0x51, 0x45, 0x47, 0x2E, 0x25, 0x76, 0x70, 0x2D, 0x30, 0x1E, 0x73, 0x8E, 0x0C, 0x59, 0x5B, 0x58,
    0x02, 0x39, 0x10, 0x61, 0x62, 0x7B, 0x7D, 0x7C, 0x7A, 0x7E, 0x7F, 0x94, 0x53, 0x96, 0x54, 0x72,
    0x65, 0x3A, 0x9C, 0x9B, 0x5D, 0x90, 0x03, 0x07, 0x28, 0x8C, 0x71, 0x75, 0x82, 0x63, 0x86, 0x6E,
    0x09, 0x8F, 0x37, 0x95, 0x23, 0x22, 0x46, 0x78, 0x60, 0x77, 0x2B, 0x05, 0x9A, 0x93, 0x8D, 0x97,
    0x5C, 0x08, 0x98, 0x00
};  // data_020bd878
unsigned char DE_tbl[164] = {
    0x00, 0xA2, 0x10, 0x4D, 0x14, 0x47, 0x15, 0x7B, 0x44, 0x72, 0x3E, 0x3C, 0x42, 0x01, 0x12, 0x28,
    0x30, 0x74, 0x05, 0x39, 0xA1, 0x0F, 0x1F, 0x35, 0x8A, 0x2F, 0x22, 0x1A, 0x93, 0x78, 0x56, 0x58,
    0x79, 0x9C, 0x9D, 0x88, 0x02, 0x1E, 0x0B, 0x37, 0x8E, 0x49, 0x85, 0x11, 0x67, 0x50, 0x46, 0x17,
    0x19, 0x68, 0x71, 0x83, 0x08, 0x29, 0x64, 0x52, 0x6A, 0x0D, 0x57, 0x98, 0x23, 0x48, 0x9B, 0x7A,
    0x32, 0x1B, 0x99, 0x69, 0xA0, 0x6E, 0x6B, 0x4E, 0x86, 0x53, 0x6F, 0x18, 0x4B, 0x76, 0x82, 0x0E,
    0x9F, 0x2A, 0x87, 0x51, 0x38, 0x3F, 0x13, 0x16, 0x21, 0x9E, 0x89, 0x34, 0x20, 0x27, 0x84, 0x5A,
    0x4C, 0x43, 0x63, 0x33, 0x62, 0x70, 0x2C, 0x36, 0x77, 0x1C, 0x2E, 0x7E, 0x3A, 0x59, 0x54, 0x3D,
    0x2D, 0x91, 0x0A, 0x5C, 0x5D, 0x7F, 0x24, 0x92, 0x03, 0x4F, 0x55, 0x0C, 0x97, 0x8D, 0x9A, 0x75,
    0x1D, 0x65, 0x94, 0x96, 0x5B, 0x26, 0x90, 0x8F, 0x25, 0x7C, 0x07, 0x4A, 0x73, 0x61, 0x81, 0x66,
    0x06, 0x31, 0x3B, 0x6D, 0x5F, 0x60, 0x45, 0x6C, 0x5E, 0x80, 0x2B, 0x40, 0x95, 0x09, 0x7D, 0x8B,
    0x41, 0x04, 0x8C, 0x00
};  // data_020bd7d4

unsigned char JP_tbl[164] = {
    0x00,0x75,0x2A,0x5C,0x50,0x22,0x52,0x67,
    0x70,0x1E,0x3C,0x7C,0x24,0x37,0x4C,0x69,
    0x60,0xA0,0x04,0x87,0x99,0x61,0x1C,0x5A,
    0x89,0x5E,0x7F,0x28,0x18,0x8A,0x85,0x93,
    0x31,0x56,0x57,0x80,0x6D,0x72,0x49,0x23,
    0x66,0x01,0x83,0x65,0x44,0x17,0x21,0x53,
    0x51,0x68,0x19,0x0E,0x3A,0x6C,0x25,0x15,
    0x76,0x63,0x91,0x8D,0x92,0x79,0x58,0x9B,
    0x36,0x59,0x86,0x9D,0x42,0x74,0x88,0x34,
    0x84,0x14,0x0A,0x4F,0x46,0x95,0x77,0x62,
    0x55,0x6A,0x82,0x16,0x7A,0x1A,0x4A,0x4B,
    0x71,0x54,0x30,0x1D,0x2E,0x6B,0x81,0x7E,
    0x7D,0x98,0x96,0x1F,0x02,0x8F,0x07,0x97,
    0x29,0x2D,0x06,0x48,0x08,0x2F,0x27,0x9C,
    0x5F,0x39,0x1B,0x90,0x64,0x47,0x35,0x10,
    0x8E,0x05,0x78,0x5B,0x8B,0x2B,0x8C,0x9F,
    0x0F,0x09,0x3E,0x3D,0x45,0x41,0x0B,0x2C,
    0x03,0x4D,0x0C,0x0D,0x20,0x9E,0x38,0x12,
    0x32,0x33,0x3B,0x43,0x6E,0x6F,0x13,0x5D,
    0x7B,0x26,0x11,0x73,0x3F,0x9A,0x4E,0xA1,
    0x94,0x40,0xA2,0x00
}; // data_020bd730
unsigned char ES_tbl[164]= {
    0x00, 0x7D, 0x57, 0x48, 0x55, 0x69, 0x5E, 0x47, 0x5B, 0x8D, 0x36, 0x5F, 0x21, 0x01, 0x02, 0x44,
    0x07, 0x1B, 0x20, 0x68, 0x4B, 0x37, 0x75, 0x1D, 0x4C, 0x7E, 0x56, 0x7F, 0x45, 0x1C, 0x1E, 0x6A,
    0x1A, 0x49, 0x4A, 0x1F, 0x2F, 0x4F, 0x5D, 0x60, 0x66, 0x54, 0x46, 0x9C, 0x9D, 0x13, 0x33, 0x15,
    0x38, 0x11, 0x9A, 0x98, 0x19, 0x16, 0x03, 0x99, 0x65, 0x32, 0x92, 0x04, 0x97, 0x91, 0x18, 0x12,
    0x96, 0x64, 0x9B, 0x94, 0x34, 0x95, 0x14, 0x2C, 0x17, 0x3D, 0x3F, 0x40, 0x41, 0x3A, 0x42, 0x3E,
    0x43, 0x3B, 0x3C, 0x5A, 0x30, 0x90, 0x2E, 0x9E, 0x59, 0xA1, 0x6B, 0x8E, 0x8C, 0x9F, 0xA0, 0x26,
    0x52, 0x25, 0x24, 0x27, 0x6C, 0x58, 0x0D, 0x0C, 0x0F, 0xA2, 0x0E, 0x0B, 0x31, 0x73, 0x23, 0x7C,
    0x7B, 0x05, 0x08, 0x29, 0x09, 0x87, 0x86, 0x8A, 0x89, 0x8B, 0x88, 0x61, 0x62, 0x63, 0x06, 0x4D,
    0x0A, 0x22, 0x5C, 0x82, 0x6D, 0x83, 0x28, 0x6E, 0x50, 0x2A, 0x2D, 0x76, 0x4E, 0x72, 0x71, 0x8F,
    0x74, 0x85, 0x53, 0x67, 0x84, 0x80, 0x79, 0x10, 0x93, 0x78, 0x81, 0x35, 0x51, 0x39, 0x2B, 0x6F,
    0x77, 0x7A, 0x70, 0x00
};  // data_020bda64

THUMB void status::UseItem::initialize()
{
    UseitemData_.itemData_ = status::excelParam.getItemData();
	return;
}

THUMB ItemType status::UseItem::getItemType(int itemIndex) {
    switch (UseitemData_.itemData_[itemIndex].type) {
    case 0:
        return ITEM_WEAPON;
    case 1:
        return ITEM_ARMOR;
    case 2:
        return ITEM_SHIELD;
    case 3:
        return ITEM_HELMET;
    case 4:
        return ITEM_ACCESSORY;
    case 5:
        return ITEM_ITEM;
    case 6:
        return ITEM_SEED;
    case 7:
        return ITEM_KEY;
    case 8:
        return ITEM_CASINO;
    case 9:
        return ITEM_OTHER;
    case 10:
        return ITEM_IMPORTANT;
    case 11:
        return ITEM_DEBUG;
    default:
        return ITEM_OTHER;
    }
}

THUMB status::UseItem::ThrowType status::UseItem::getThrowType(int itemIndex) {
    char val = UseitemData_.itemData_[itemIndex].byte_3;
    int result = (val & 0xc) << 0x16 >> 0x18;
    switch (result) {
    case 0:
        return THROW_NG;
    case 1:
        return THROW_OK;
    case 2:
        return THROW_DIFFICULT;
    default:
        return THROW_NG;
    }
}


THUMB status::UseItem::SellType status::UseItem::getSellType(int itemIndex) {
    char val = UseitemData_.itemData_[itemIndex].byte_3;
    char result = (val & 3);
    switch (result) {
    case 0:
        return SELL_NG;
    case 1:
        return SELL_OK;
    case 2:
        return SELL_DIFFICULT;
    default:
        return SELL_NG;
    }
}

THUMB bool status::UseItem::isCurse(int itemIndex)
{
    if ((UseitemData_.itemData_[itemIndex].byte_2 & 8) << 0x15 >> 0x18 != 0) {
        return 1;
    }
    return 0;
}

THUMB int status::UseItem::isLost(int itemIndex)
{
    char val = UseitemData_.itemData_[itemIndex].byte_2;
    int result = (val & 4) << 0x16 >> 0x18;
    switch (result) {
    case 0:
        return 0;
    case 1:
        return 1;
    default:
        return 1;
    }
}

THUMB bool status::UseItem::isOrder(int itemIndex)
{
    char val = UseitemData_.itemData_[itemIndex].byte_3;
    int result = ((val & 0x30) << 0x14) >> 0x18;

    if (result == 1) {
        return 1;
    }
    return 0;
}


THUMB int status::UseItem::getSellPrice(int itemIndex)
{
    return UseitemData_.itemData_[itemIndex].SellPrice;
}


THUMB int status::UseItem::getBuyPrice(int itemIndex)
{
	return UseitemData_.itemData_[itemIndex].BuyPrice;
}


THUMB bool status::UseItem::isEquipEnable(JobType job, int itemIndex, int sex)
{
    signed char val;

    switch (job) {
    case JOB_HERO:
        if (sex == 0) {
            val = UseitemData_.itemData_[itemIndex].byte_1 & 1;
            if (val != 0) {
                return true;
            }
        }
        else if (sex == 1) {
            val = (UseitemData_.itemData_[itemIndex].byte_1 & 2) >> 1;
            if (val != 0) {
                return true;
            }
        }
        break;
    case JOB_WARRIOR:
        val = (UseitemData_.itemData_[itemIndex].byte_1 & 4) >> 2;
        if (val != 0) {
            return true;
        }
        break;
    case JOB_PRINCESS:
        val = (UseitemData_.itemData_[itemIndex].byte_1 & 8) >> 3;
        if (val != 0) {
            return true;
        }
        break;
    case JOB_PRIEST:
        val = (UseitemData_.itemData_[itemIndex].byte_1 & 0x10) >> 4;
        if (val != 0) {
            return true;
        }
        break;
    case JOB_MAGE:
        val = (UseitemData_.itemData_[itemIndex].byte_1 & 0x20) >> 5;
        if (val != 0) {
            return true;
        }
        break;
    case JOB_TRADER:
        val = (UseitemData_.itemData_[itemIndex].byte_1 & 0x40) >> 6;
        if (val != 0) {
            return true;
        }
        break;
    case JOB_WARLOCK:
        val = (UseitemData_.itemData_[itemIndex].byte_1 & 0x80) >> 7;
        if (val != 0) {
            return true;
        }
        break;
    case JOB_DANCER:
        val = UseitemData_.itemData_[itemIndex].byte_2 & 1;
        if (val != 0) {
            return true;
        }
        break;
    case JOB_PISSARO:
        val = (UseitemData_.itemData_[itemIndex].byte_2 & 2) >> 1;
        if (val != 0) {
            return true;
        }
        break;
    }
    return false;
}


THUMB EquipType status::UseItem::getEquipType(int itemIndex)
{
    switch (UseitemData_.itemData_[itemIndex].change) {
    case 3:
        return Strength;
    case 4:
        return Agility;
    case 5:
        return Wisdom;
    case 6:
        return Luck;
    case 1:
        return Attack;
    case 2:
        return Defence;
    default:
        return Attack;
    }
}

THUMB int status::UseItem::getEquipValue(int itemIndex)
{
    return UseitemData_.itemData_[itemIndex].effect;
}

THUMB status::UseItem::UseType status::UseItem::getUseType(int itemIndex)
{
    int result = getAction(itemIndex);
    result = status::UseAction::getUseType(result);
    switch (result) {
    case 0:
        return Myself;
    case 1:
        return Friend;
    case 2:
        return Enemy;
    case 3:
        return Both;
    case 4:
        return None;
    default:
        return None;
    }
}

THUMB status::UseItem::UseArea status::UseItem::getUseArea(int itemIndex)
{
    int result = getAction(itemIndex);
    result = status::UseAction::getUseArea(result);
    switch (result) {
    case 0:
        return Zero;
    case 1:
        return One;
    case 2:
        return Group;
    case 3:
        return All;
    case 4:
        return Horse;
    case 5:
        return UsuallyHorse;
    case 6:
        return BattleHorse;
    default:
        return Zero;
    }
}

THUMB int status::UseItem::getAction(int itemIndex)
{
    return UseitemData_.itemData_[itemIndex].action;
}

THUMB int status::UseItem::getBattleUseAction(int itemIndex)
{
	return UseitemData_.itemData_[itemIndex].battleAction;
}

THUMB int status::UseItem::getMenuMessage(int itemIndex)
{
    return UseitemData_.itemData_[itemIndex].menuMes;
}

THUMB bool status::UseItem::isUsuallyUse(int itemIndex) {
    char val = UseitemData_.itemData_[itemIndex].byte_2;
    int result = ((val & 0x40) << 0x12) >> 0x18;
    if (result) {
        return 1;
    }
    return 0;
}

THUMB int status::UseItem::getTypeSortIndex(int itemIndex)
{
    return UseitemData_.itemData_[itemIndex].typeSort;
}


THUMB int status::UseItem::getNameSortIndex(int itemIndex) {
    unsigned char* table = JP_tbl;
    Language lang = g_Game.language;
    if (lang == 0) {
        table = JP_tbl;
    }
    if (lang == 1) {
        table = EN_tbl;
    }
    if (lang == 2) {
        table = FR_tbl;
    }
    if (lang == 3) {
        table = DE_tbl;
    }
    if (lang == 4) {
        table = IT_tbl;
    }
    if (lang == 5) {
        table = ES_tbl;
    }
    return table[itemIndex];
}

THUMB int status::UseItem::getJudgeMessageCount(int itemIndex)
{
    int count;
    param::AppriseItem* item;
    UseitemData_.judgeItem_ = status::excelParam.getAppriseItem();
    count = 0;
    item = &UseitemData_.judgeItem_[itemIndex];
    if (item->message1 != 0) {
        count++;
    }
    if (item->message2 != 0) {
        count++;
    }
    if (item->message3 != 0) {
        count++;
    }
    if (item->message4 != 0) {
        count++;
    }
    if (item->message5 != 0) {
        count++;
    }
    if (item->message6 != 0) {
        count++;
    }
    if (item->message7 != 0) {
        count++;
    }

    return count;
}

THUMB int status::UseItem::getJudgeMessage(int itemIndex, int index)
{
    UseitemData_.judgeItem_ = status::excelParam.getAppriseItem();

    switch (index) {
    case 0:
        return UseitemData_.judgeItem_[itemIndex].message1;
    case 1:
        return UseitemData_.judgeItem_[itemIndex].message2;
    case 2:
        return UseitemData_.judgeItem_[itemIndex].message3;
    case 3:
        return UseitemData_.judgeItem_[itemIndex].message4;
    case 4:
        return UseitemData_.judgeItem_[itemIndex].message5;
    case 5:
        return UseitemData_.judgeItem_[itemIndex].message6;
    case 6:
        return UseitemData_.judgeItem_[itemIndex].message7;
    default:
        return 0;
    }
}

 

THUMB void status::UseItem::execUse(status::UseActionParam* useActionParam)
{
    status::HaveItemSack* actorHaveItemSack;
    int index = useActionParam->itemSortIndex_;
    actorHaveItemSack = useActionParam->actorHaveItemSack_;
    int Item;

    if (actorHaveItemSack) {
        Item = actorHaveItemSack->getItem(index);
        UseitemData_.itemIndex_ = Item;
    } else if (useActionParam->actorCharacterStatus_ != 0) {
        Item = useActionParam->actorCharacterStatus_->haveStatusInfo_.haveItem_.getItem(index);
        UseitemData_.itemIndex_ = Item;
        useActionParam->itemIndex_ = Item;
    }

    UseitemData_.actionIndex_ = getAction(UseitemData_.itemIndex_);
    useActionParam->actionIndex_ = UseitemData_.actionIndex_;

    UseAction::execUse(useActionParam);

    if (useActionParam->result_ != 0 || UseitemData_.itemIndex_ == 0x80) {
        if (isLost(UseitemData_.itemIndex_) != 0) {
            if (useActionParam->actorHaveItemSack_ != 0) {
                UseitemData_.itemIndex_ = useActionParam->actorHaveItemSack_->del(index);
            } else if (useActionParam->actorCharacterStatus_ != 0) {
                useActionParam->actorCharacterStatus_->haveStatusInfo_.haveItem_.del(index);
            }
        }

        if (status::BaseAction::isBreakPrayRing() != 0) {
            status::BaseAction::setBreakPrayRing(0);
            if (useActionParam->actorHaveItemSack_ != 0) {
                UseitemData_.itemIndex_ = useActionParam->actorHaveItemSack_->del(index);
                return;
            }
            if (useActionParam->actorCharacterStatus_ != 0) {
                useActionParam->actorCharacterStatus_->haveStatusInfo_.resetEquipment2(ITEM_ACCESSORY);
                useActionParam->actorCharacterStatus_->haveStatusInfo_.haveItem_.del(index);
            }
        }
    }
}


THUMB void status::UseItem::execBattleUse(status::UseActionParam* useActionParam)
{
    int itemSortIndex = useActionParam->itemSortIndex_;
    int item = useActionParam->actorCharacterStatus_->haveStatusInfo_.haveItem_.getItem(itemSortIndex);

    UseitemData_.itemIndex_ = item;
    useActionParam->itemIndex_ = item;

    UseitemData_.actionIndex_ = getBattleUseAction(UseitemData_.itemIndex_);
    useActionParam->actionIndex_ = UseitemData_.actionIndex_;

    UseAction::execUse(useActionParam);

    if ((useActionParam->result_ || UseitemData_.itemIndex_ == 0x80) && isLost(UseitemData_.itemIndex_)) {
        useActionParam->actorCharacterStatus_->haveStatusInfo_.haveItem_.del(itemSortIndex);
    }
}

THUMB void status::UseItem::give(status::BaseHaveItem* src, int index, status::BaseHaveItem* dst)
{
    int count = dst->getCount();
    int max = dst->getItemMax();

    if (count == max) {
        if (dst->getItemMax() != 0xa2) {
            int lastIndex = dst->getCount();
            lastIndex--;
            int lastItem = dst->getItem(lastIndex);
            lastIndex = dst->getCount();
            lastIndex--;
            dst->del(lastIndex);                    // vtable +8

            int srcItem = src->getItem(index);
            dst->add(srcItem);                      // vtable +4

            src->del(index);                        // vtable +8
            src->add(lastItem);                     // vtable +4
        }
    }
    else {
        int srcItem = src->getItem(index);
        dst->add(srcItem);                          // vtable +4
        src->del(index);                            // vtable +8
    }
}

THUMB void status::UseItem::give(status::HaveStatusInfo *status, int index,
                           status::BaseHaveItem *sack, int sackIndex)
{
    int itemIndex = 0;

    if (sackIndex == -1) {

        if (status->haveItem_.isEquipment(index)) {
            itemIndex = status->haveItem_.getItem(index);
        }

        give(&status->haveItem_, index, sack);
        status->haveEquipment_.removeItem(itemIndex);

    } else {

        if (status->haveItem_.isEquipment(index)) {
            status->resetEquipment1(index);
        }

        status::ItemData* data = status->haveItem_.getItemData(index);
        int other = sack->getItem(sackIndex);

        sack->add(data->index_);
        sack->del(sackIndex);

        data->index_ = other;
    }
}

THUMB void status::UseItem::give2(status::BaseHaveItem *sack, int sackIndex,
                            status::HaveStatusInfo *status, int index)
{
    if (index == -1) {
        give(sack, sackIndex, &status->haveItem_);
        return;
    }

    int itemIndex = 0;

    if (status->haveItem_.isEquipment(index)) {
        itemIndex = status->haveItem_.getItem(index);
    }
    status->haveEquipment_.removeItem(itemIndex);

    status::ItemData* src = sack->getItemData(sackIndex);
    status::ItemData* dst = status->haveItem_.getItemData(index);

    int old = dst->index_;

    dst->index_ = src->index_;
    dst->equip_ = 0;

    sack->del(sackIndex);
    sack->add(old);
}

THUMB void status::UseItem::give2(status::HaveStatusInfo *status, int index,
                            status::HaveStatusInfo *target, int targetIndex)
{
    int itemIndex = 0;

    if (targetIndex == -1) {

        if (status->haveItem_.isEquipment(index)) {
            itemIndex = status->haveItem_.getItem(index);
        }

        give(&status->haveItem_, index, &target->haveItem_);
        status->haveEquipment_.removeItem(itemIndex);
        return;
    }

    if (status->haveItem_.isEquipment(index)) {
        status->haveEquipment_.removeItem(status->haveItem_.getItem(index));
    }
    if (target->haveItem_.isEquipment(targetIndex)) {
        target->haveEquipment_.removeItem(target->haveItem_.getItem(targetIndex));
    }

    status::ItemData* src = status->haveItem_.getItemData(index);
    status::ItemData* dst = target->haveItem_.getItemData(targetIndex);

    src->equip_ = 0;
    dst->equip_ = 0;

    int tmp = src->index_;
    src->index_ = dst->index_;
    dst->index_ = tmp;

    status->haveItem_.sortEquipment();
    target->haveItem_.sortEquipment();
}

status::UseitemData UseitemData_;
