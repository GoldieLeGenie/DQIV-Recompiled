#include "main/param/ColorCorrect.hpp"
#include "main/dss/DssUtils.hpp"


THUMB int param::ColorCorrect::getCorrectIndex(param::ColorCorrect *data, char *name)
{
    unsigned int index = 0;

    switch (name[1] + name[0] * 100)
    {
    case 0x270D: index = 0x08;  break;  // "ca"
    case 0x270E: index = 0x14;  break;  // "cb"
    case 0x270F: index = 0x24;  break;  // "cc"
    case 0x2710: index = 0x2C;  break;  // "cd"
    case 0x2711: index = 0x34;  break;  // "ce"
    case 0x2713: index = 0x3C;  break;  // "cg"
    case 0x2714: index = 0x40;  break;  // "ch"
    case 0x2715: index = 0x44;  break;  // "ci"
    case 0x2716: index = 0x48;  break;  // "cj"
    case 0x2717: index = 0x4C;  break;  // "ck"
    case 0x27EA: index = 0x60;  break;  // "ev"
    case 0x2843: index = 0x6C;  break;  // "fk"
    case 0x2901: index = 0x70;  break;  // "ha"
    case 0x2903: index = 0x74;  break;  // "hc"
    case 0x2908: index = 0x7C;  break;  // "hh"
    case 0x2AF5: index = 0x80;  break;  // "ma"
    case 0x2AF6: index = 0x84;  break;  // "mb"
    case 0x2AF7: index = 0x88;  break;  // "mc"
    case 0x2AF8: index = 0x90;  break;  // "md"
    case 0x2AF9: index = 0x94;  break;  // "me"
    case 0x2AFA: index = 0x98;  break;  // "mf"
    case 0x2AFB: index = 0x9C;  break;  // "mg"
    case 0x2AFC: index = 0xA8;  break;  // "mh"
    case 0x2AFD: index = 0xB8;  break;  // "mi"
    case 0x2AFE: index = 0xBC;  break;  // "mj"
    case 0x2AFF: index = 0xCC;  break;  // "mk"
    case 0x2B00: index = 0xD0;  break;  // "ml"
    case 0x2B01: index = 0xD8;  break;  // "mm"
    case 0x2B02: index = 0xDC;  break;  // "mn"
    case 0x2B03: index = 0xF4;  break;  // "mo"
    case 0x2B04: index = 0xF8;  break;  // "mp"
    case 0x2B05: index = 0x100; break;  // "mq"
    case 0x2B06: index = 0x10C; break;  // "mr"
    case 0x2B07: index = 0x110; break;  // "ms"
    case 0x2D4E: index = 0x114; break;  // "sb"
    case 0x2D4F: index = 0x118; break;  // "sc"
    case 0x2D50: index = 0x11C; break;  // "sd"
    case 0x2D51: index = 0x120; break;  // "se"
    case 0x2D52: index = 0x124; break;  // "sf"
    case 0x2D54: index = 0x128; break;  // "sh"
    case 0x2D56: index = 0x12C; break;  // "sj"
    case 0x2D5F: index = 0x148; break;  // "ss"
    case 0x2DB1: index = 0x168; break;  // "ta"
    case 0x2DB2: index = 0x178; break;  // "tb"
    case 0x2DB3: index = 0x18C; break;  // "tc"
    case 0x2DB4: index = 0x1A0; break;  // "td"
    case 0x2DB5: index = 0x1B4; break;  // "te"
    case 0x2772: index = 0x204; break;  // "db"
    case 0x277D: index = 0x208; break;  // "dm"
    case 0x2780: index = 0x20C; break;  // "dp"
    }

    while (index < 0x21C)
    {
        if (dss::strcmp(data[index].floor, name) == 0)
            return index;
        index++;
    }
    return -1;
}

DataObject param::ColorCorrect::data_;
