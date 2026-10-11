#include "main/param/BattleMap.hpp"
#include "main/dss/DssUtils.hpp"

THUMB int param::BattleMap::getBattleMap(param::BattleMap *data, char *name)
{
    int k;
    char c;
    int t;

    if (dss::strstr(name, btl_) != 0)
    {
        c = name[5];
        k = 4;
    }
    else
    {
        c = name[4];
        k = 3;
    }
    t = c + 100 * name[k];

    if (dss::strstr(name, btldougu) != 0)
        t = 0x2EE0;
    if (dss::strstr(name, btlyado) != 0)
        t = 0x2EE0;

    unsigned int index = 0;

    switch (t)
    {
    case 0x2C2C: index = 0x00; break;  // "pl" 
    case 0x2D50: index = 0x00; break;  // "sd" 
    case 0x2B08: index = 0x04; break;  // "mt"
    case 0x2D51: index = 0x07; break;  // "se"
    case 0x2C2F: index = 0x0A; break;  // "po"
    case 0x2EE0: index = 0x0D; break;  
    case 0x2580: index = 0x11; break;
    case 0x2785: index = 0x15; break;  // "du"
    case 0x2FA5: index = 0x16; break;  // "ya"
    case 0x2771: index = 0x18; break;  // "da"
    case 0x2772: index = 0x1B; break;  // "db"
    case 0x2773: index = 0x1C; break;  // "dc"
    case 0x2774: index = 0x1D; break;  // "dd"
    case 0x2775: index = 0x1F; break;  // "de"
    case 0x2777: index = 0x20; break;  // "dg"
    case 0x2778: index = 0x21; break;  // "dh"
    case 0x2779: index = 0x22; break;  // "di"
    case 0x277A: index = 0x23; break;  // "dj"
    case 0x277B: index = 0x24; break;  // "dk"
    case 0x277C: index = 0x25; break;  // "dl"
    case 0x277D: index = 0x26; break;  // "dm"
    case 0x277E: index = 0x28; break;  // "dn"
    case 0x277F: index = 0x29; break;  // "do"
    case 0x2780: index = 0x2A; break;  // "dp"
    case 0x2DB1: index = 0x2B; break;  // "ta"
    case 0x2DB2: index = 0x43; break;  // "tb"
    case 0x2DB3: index = 0x47; break;  // "tc"
    case 0x2DB4: index = 0x4A; break;  // "td"
    case 0x2DB5: index = 0x51; break;  // "te"
    case 0x2D56: index = 0x57; break;  // "sj"
    case 0x2D58: index = 0x5A; break;  // "sl"
    case 0x2D5A: index = 0x5D; break;  // "sn"
    case 0x2D5B: index = 0x5E; break;  // "so"
    case 0x2D5C: index = 0x5F; break;  // "sp"
    case 0x2D5D: index = 0x60; break;  // "sq"
    case 0x2D5E: index = 0x61; break;  // "sr"
    case 0x270E: index = 0x62; break;  // "cb"
    case 0x270F: index = 0x63; break;  // "cc"
    case 0x2711: index = 0x69; break;  // "ce"
    case 0x2716: index = 0x6C; break;  // "cj"
    case 0x2718: index = 0x6D; break;  // "cl"
    case 0x2719: index = 0x7C; break;  // "cm"
    case 0x2AF7: index = 0x7E; break;  // "mc"
    case 0x2B05: index = 0x81; break;  // "mq"
    }

    while (index < 0x8A)
    {
        if (dss::strcmp(data[index].map, name) == 0)
            return index;
        index++;
    }
    return -1;
}

char btl_[8] = "btl_";
char btldougu[12] = "btldougu";
char btlyado[8] = "btlyado";

DataObject param::BattleMap::data_;
