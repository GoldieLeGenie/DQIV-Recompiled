#include "main/status/ShopList.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/status/GameFlag.hpp"
#include "main/param/ShopDataFirst.hpp"
#include "main/param/ShopDataSecond.hpp"

status::ShopList status::g_Shop;
status::ShopData status::ShopData_;

char check_map[4] = "en";   // 0x020bdc70
extern char s_mj[4];
extern char s_md[4];
extern char s_mb[4];
extern char s_cd[4];
extern char s_hc[4];


THUMB status::ShopList::ShopList(){
    return;
}
THUMB status::ShopList::~ShopList(){
    return;
}

THUMB void status::ShopList::initialize()
{
    ShopData_.shopDataFirst_ = excelParam.shopDataFirst_;
    ShopData_.shopDataSecond_ = excelParam.shopDataSecond_;
}


THUMB int status::ShopList::getDataIndex(int shop)
{
    char name[4];
    

    if (g_Stage.crusingPeopleEncount_ == 1) {
        strcpy(name, check_map);
        name[2] = '\0';
    } else {
        const char* check = "ss\0";
        name[0] = g_Stage.getMapName()[0];
        name[1] = g_Stage.getMapName()[1];
        name[2] = '\0';
        
        if (dss::strcmp(name, check) == 0) {
            name[2] = g_Stage.getMapName()[2];
            name[3] = '\0';
        }
    }

    int index;
    if (ShopData_.section_ == 0) {
        index = param::ShopDataSecond::getIndex(name);
    } else {
        index = param::ShopDataFirst::getIndex(name);
    }

    switch (shop) {
        case 1:  index += 1;  break;
        case 2:  index += 2;  break;
        case 3:  index += 8;  break;
        case 4:  index += 14; break;
        case 5:  index += 20; break;
        case 6:  index += 26; break;
        case 7:  index += 27; break;
        case 8:  index += 28; break;
        case 9:  index += 34; break;
        case 10: index += 40; break;
        case 11: index += 46; break;
    }
    return index;
}


THUMB int status::ShopList::getShopCount(int shop)
{
    int count = 0;
    int dataIndex = getDataIndex(shop);
    char buf[3] = { 0, 0, 0 };
    int limit;                                   
    buf[0] = g_Stage.getMapName()[0];
    buf[1] = g_Stage.getMapName()[1];
    switch (shop)
    {
        case 2:
        {
            char* town = s_md;
            if (dss::strcmp(buf, s_mj) == 0 && g_Story.chapter_ >= 5)
            {
                limit = 0;
            }
            else if (dss::strcmp(buf, town) == 0 && g_AreaFlag.check(0x5E) == 0)
            {
                limit = 0;
            }
            else
            {
                limit = 6;
            }
            break;
        }
        case 8:
            if (dss::strcmp(buf, s_mb) == 0 && g_AreaFlag.check(0x147) == 0)
            { limit = 0; } else { limit = 6; }
            break;
        case 3:
            if (dss::strcmp(buf, s_cd ) == 0 && g_Story.chapter_ < 5)
            { limit = 0; } else { limit = 6; }
            break;
        case 9:
            if (dss::strcmp(buf, s_hc) == 0 && g_AreaFlag.check(0x147) == 0)
            { limit = 0; } else { limit = 6; }
            break;
        case 4:
            limit = 6;
            break;
        case 10:
            limit = 6;
            break;
        case 5:
        case 11:
            limit = 6;
            break;
    }
    if (ShopData_.section_ == 0)
    {
        for (int i = 0; i < limit; i++)
        {
            if (ShopData_.shopDataFirst_[dataIndex + i].item != 0)
            {
                count++;
            }
        }
    }
    else
    {
        for (int i = 0; i < limit; i++)
        {
            if (ShopData_.shopDataSecond_[dataIndex + i].item != 0)
            {
                count++;
            }
        }
    }
    return count;
}

THUMB int status::ShopList::getShopItem(int shop, int index)
{
    int dataIndex = getDataIndex(shop);

    if (ShopData_.section_ == 0)
    {
        return ShopData_.shopDataFirst_[dataIndex + index].item;
    }
    else
    {
        return ShopData_.shopDataSecond_[dataIndex + index].item;
    }
}

THUMB int status::ShopList::getShopPrice(int shop, int index)
{
    int dataIndex = getDataIndex(shop);

    if (ShopData_.section_ == 0)
    {
        return ShopData_.shopDataFirst_[dataIndex + index].price;
    }
    else
    {
        return ShopData_.shopDataSecond_[dataIndex + index].price;
    }
}

THUMB int status::ShopList::getHotelPrice(int second)
{
    int dataIndex;

    if (second == 0)
    {
        dataIndex = getDataIndex(0);
    }
    else
    {
        dataIndex = getDataIndex(6);
    }

    if (ShopData_.section_ == 0)
    {
        return ShopData_.shopDataFirst_[dataIndex].price;
    }
    else
    {
        return ShopData_.shopDataSecond_[dataIndex].price;
    }
}

THUMB status::ShopList::ChurchType status::ShopList::getChurchType(int first)
{
    int dataIndex;

    if (first == 0)
    {
        dataIndex = getDataIndex(1);
    }
    else
    {
        dataIndex = getDataIndex(7);
    }

    if (ShopData_.section_ == 0)
    {
        switch (ShopData_.shopDataFirst_[dataIndex].price)
        {
        case NoExist: return NoExist;
        case Exist:   return Exist;
        case NoPray:  return NoPray;
        }
    }
    else
    {
        switch (ShopData_.shopDataSecond_[dataIndex].price)
        {
        case NoExist: return NoExist;
        case Exist:   return Exist;
        case NoPray:  return NoPray;
        }
    }
    return Exist;
}

char s_mj[4] = "mj";        // 0x020bdc78
char s_md[4] = "md";        // 0x020bdc7c
char s_mb[4] = "mb";        // 0x020bdc80
char s_cd[4] = "cd";        // 0x020bdc84
char s_hc[4] = "hc";        // 0x020bdc88
