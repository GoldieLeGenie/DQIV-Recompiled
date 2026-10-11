#include "main/param/MonsterMap.hpp"
#include "main/dss/DssUtils.hpp"


THUMB int param::MonsterMap::getFloorIndex(param::MonsterMap* data, int section, const char* name)
{
    unsigned int index;

    switch (name[1] + (section * 10000 + name[0] * 100))
    {
        case 0x4E81: index = 0; break;       // 1:da
        case 0x4E82: index = 1; break;       // 1:db
        case 0x54C1: index = 0xA; break;       // 1:ta
        case 0x7593: index = 0x11; break;      // 2:dc
        case 0x7BD2: index = 0x14; break;      // 2:tb
        case 0x9CA4: index = 0x1B; break;      // 3:dd
        case 0x9CA5: index = 0x22; break;      // 3:de
        case 0xC3B7: index = 0x28; break;      // 4:dg
        case 0xC3BF: index = 0x2C; break;      // 4:do
        case 0xF103: index = 0x2F; break;      // 5:tc
        case 0xEAC9: index = 0x34; break;      // 5:di
        case 0xEAC7: index = 0x38; break;      // 5:dg
        case 0xEA5E: index = 0x3D; break;      // 5:cb
        case 0xEAC2: index = 0x41; break;      // 5:db
        case 0xEACA: index = 0x4A; break;      // 5:dj
        case 0xEACB: index = 0x4D; break;      // 5:dk
        case 0xEACC: index = 0x51; break;      // 5:dl
        case 0xEACD: index = 0x57; break;      // 5:dm
        case 0xF0A6: index = 0x5D; break;      // 5:sj
        case 0xEACF: index = 0x68; break;      // 5:do
        case 0xEA69: index = 0x6D; break;      // 5:cm
        case 0xF104: index = 0x73; break;      // 5:td
        case 0xF105: index = 0x79; break;      // 5:te
        case 0xEACE: index = 0x8D; break;      // 5:dn
        case 0xF0A8: index = 0x9A; break;      // 5:sl
        case 0xEA68: index = 0x9F; break;      // 5:cl
        case 0xF0AE: index = 0xAB; break;      // 5:sr
        case 0x111E0: index = 0xAC; break;      // 6:dp
        default:
            index = 0;
            break;
    }
    for (; index < 0xC9; index++)
    {
        if (dss::strcmp(data[index].floorID, name) == 0)
        {
            return index;
        }
    }

    switch (name[1] + name[0] * 100)
    {
        case 0x2771: index = 0; break;       // da
        case 0x2772: index = 1; break;       // db
        case 0x2DB1: index = 0xA; break;       // ta
        case 0x2773: index = 0x11; break;      // dc
        case 0x2DB2: index = 0x14; break;      // tb
        case 0x2774: index = 0x1B; break;      // dd
        case 0x2775: index = 0x22; break;      // de
        case 0x2777: index = 0x28; break;      // dg
        case 0x277F: index = 0x2C; break;      // do
        case 0x2DB3: index = 0x2F; break;      // tc
        case 0x2779: index = 0x34; break;      // di
        case 0x270E: index = 0x3D; break;      // cb
        case 0x277A: index = 0x4A; break;      // dj
        case 0x277B: index = 0x4D; break;      // dk
        case 0x277C: index = 0x51; break;      // dl
        case 0x277D: index = 0x57; break;      // dm
        case 0x2D56: index = 0x5D; break;      // sj
        case 0x2719: index = 0x6D; break;      // cm
        case 0x2DB4: index = 0x73; break;      // td
        case 0x2DB5: index = 0x79; break;      // te
        case 0x277E: index = 0x8D; break;      // dn
        case 0x2D58: index = 0x9A; break;      // sl
        case 0x2718: index = 0x9F; break;      // cl
        case 0x2D5E: index = 0xAB; break;      // sr
        case 0x2780: index = 0xAC; break;      // dp
        default:
            return -1;
    }
    for (; index < 0xC9; index++)
    {
        if (dss::strcmp(data[index].floorID, name) == 0)
        {
            return index;
        }
    }
    return -1;
}

DataObject param::MonsterMap::data_;
