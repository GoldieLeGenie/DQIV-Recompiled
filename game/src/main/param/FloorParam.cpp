#include "main/param/FloorParam.hpp"
#include "main/dss/DssUtils.hpp"


THUMB int param::FloorParam::getFloorIndex(param::FloorParam* data, char* name)
{
    unsigned int index = 0;
    
     switch (name[1] + name[0] * 100)
    {
        default:
            break;
        case 0x270D: index = 0;   break;  // 
        case 0x270E: index = 0x3;   break;  // cb
        case 0x270F: index = 0x7;   break;  // cc
        case 0x2710: index = 0x14;  break;  // cd
        case 0x2711: index = 0x1B;  break;  // ce
        case 0x2712: index = 0x22;  break;  // cf
        case 0x2713: index = 0x25;  break;  // cg
        case 0x2714: index = 0x28;  break;  // ch
        case 0x2715: index = 0x2D;  break;  // ci
        case 0x2716: index = 0x34;  break;  // cj
        case 0x2719: index = 0x3F;  break;  // cm
        case 0x2717: index = 0x45;  break;  // ck
        case 0x2718: index = 0x4A;  break;  // cl
        case 0x2901: index = 0x56;  break;  // ha
        case 0x2903: index = 0x5F;  break;  // hc
        case 0x2908: index = 0x6B;  break;  // hh
        case 0x2AF5: index = 0x71;  break;  // ma
        case 0x2AF6: index = 0x7B;  break;  // mb
        case 0x2AF7: index = 0x83;  break;  // mc
        case 0x2AF8: index = 0x8B;  break;  // md
        case 0x2AF9: index = 0x92;  break;  // me
        case 0x2AFA: index = 0x95;  break;  // mf
        case 0x2AFB: index = 0x9E;  break;  // mg
        case 0x2AFC: index = 0xA4;  break;  // mh
        case 0x2AFD: index = 0xB2;  break;  // mi
        case 0x2AFE: index = 0xB9;  break;  // mj
        case 0x2AFF: index = 0xC3;  break;  // mk
        case 0x2B00: index = 0xC9;  break;  // ml
        case 0x2B01: index = 0xD3;  break;  // mm
        case 0x2B02: index = 0xDA;  break;  // mn
        case 0x2B03: index = 0xE5;  break;  // mo
        case 0x2B04: index = 0xEB;  break;  // mp
        case 0x2B05: index = 0xF1;  break;  // mq
        case 0x2B06: index = 0xFA;  break;  // mr
        case 0x2B07: index = 0x100; break;  // ms
        case 0x2771: index = 0x106; break;  // da
        case 0x2772: index = 0x107; break;  // db
        case 0x2773: index = 0x110; break;  // dc
        case 0x2774: index = 0x113; break;  // dd
        case 0x2775: index = 0x11A; break;  // de
        case 0x2776: index = 0x120; break;  // df
        case 0x2777: index = 0x122; break;  // dg
        case 0x277F: index = 0x127; break;  // do
        case 0x2778: index = 0x12C; break;  // dh
        case 0x2779: index = 0x132; break;  // di
        case 0x277A: index = 0x136; break;  // dj
        case 0x277B: index = 0x139; break;  // dk
        case 0x277C: index = 0x13C; break;  // dl
        case 0x277D: index = 0x142; break;  // dm
        case 0x277E: index = 0x148; break;  // dn
        case 0x2780: index = 0x153; break;  // dp
        case 0x2DB1: index = 0x170; break;  // ta
        case 0x2DB2: index = 0x177; break;  // tb
        case 0x2DB3: index = 0x17E; break;  // tc
        case 0x2DB4: index = 0x183; break;  // td
        case 0x2DB5: index = 0x189; break;  // te
        case 0x2D4D: index = 0x19D; break;  // sa
        case 0x2D4E: index = 0x19E; break;  // sb
        case 0x2D4F: index = 0x1A1; break;  // sc
        case 0x2D50: index = 0x1A2; break;  // sd
        case 0x2D51: index = 0x1A4; break;  // se
        case 0x2D52: index = 0x1A7; break;  // sf
        case 0x2D53: index = 0x1A8; break;  // sg
        case 0x2D54: index = 0x1A9; break;  // sh
        case 0x2D55: index = 0x1AD; break;  // si
        case 0x2D56: index = 0x1AE; break;  // sj
        case 0x2D57: index = 0x1B9; break;  // sk
        case 0x2D58: index = 0x1C0; break;  // sl
        case 0x2D59: index = 0x1C4; break;  // sm
        case 0x2D5A: index = 0x1C5; break;  // sn
        case 0x2D5B: index = 0x1C8; break;  // so
        case 0x2D5C: index = 0x1CA; break;  // sp
        case 0x2D5D: index = 0x1CB; break;  // sq
        case 0x2D5E: index = 0x1CF; break;  // sr
        case 0x2D5F: index = 0x1D0; break;  // ss
        case 0x2843: index = 0x1FA; break;  // fk
        case 0x27EA: index = 0x1FB; break;  // ev
        case 0x3009: index = 0x204; break;  // za

    }
    for (; index < 0x220; index++)
    {
        if (dss::strcmp(data[index].floor, name) == 0)
        {
            return index;
        }
    }
    return -1;
}

DataObject param::FloorParam::data_;
