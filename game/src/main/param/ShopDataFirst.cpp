#include "main/param/ShopDataFirst.hpp"


THUMB int param::ShopDataFirst::getIndex(char *name)
{
    int index = 0;

    switch (100 * name[0] + name[1])
    {
    case 0x2712: index = 0x0;  break;  // "cf" 
    case 0x2903: index = 0x01; break;  // "hc"
    case 0x2D4E: index = 0x02; break;  // "sb"
    case 0x2710: index = 0x03; break;  // "cd"
    case 0x2AFA: index = 0x04; break;  // "mf"
    case 0x2D51: index = 0x05; break;  // "se"
    case 0x2B01: index = 0x06; break;  // "mm"
    case 0x2B02: index = 0x07; break;  // "mn"
    case 0x2B03: index = 0x08; break;  // "mo"
    case 0x2713: index = 0x09; break;  // "cg"
    case 0x2AFE: index = 0x0A; break;  // "mj"
    case 0x2AFF: index = 0x0B; break;  // "mk"
    case 0x2AFD: index = 0x0C; break;  // "mi"
    case 0x2AFC: index = 0x0D; break;  // "mh"
    case 0x2908: index = 0x0E; break;  // "hh"
    case 0x2AF6: index = 0x0F; break;  // "mb"
    case 0x2AF7: index = 0x10; break;  // "mc"
    case 0x2AF8: index = 0x11; break;  // "md"
    case 0x2DB2: index = 0x12; break;  // "tb"
    case 0x2AF5: index = 0x13; break;  // "ma"
    case 0x2901: index = 0x14; break;  // "ha"
    case 0x2B05: index = 0x15; break;  // "mq"
    case 0x2B04: index = 0x16; break;  // "mp"
    case 0x2715: index = 0x17; break;  // "ci"
    case 0x2DB4: index = 0x18; break;  // "td"
    case 0x2D54: index = 0x19; break;  // "sh"
    case 0x2B07: index = 0x1A; break;  // "ms"
    case 0x2717: index = 0x1B; break;  // "ck"
    case 0x2D59: index = 0x1C; break;  // "sm"
    case 0x2780: index = 0x1D; break;  // "dp"
    case 0x2B06: index = 0x1E; break;  // "mr"
    case 0x2D5F:                       // "ss" -> ss1..ss5 selon le 3e caractere
        switch (name[2])
        {
        case '1': index = 0x1F; break;
        case '2': index = 0x20; break;
        case '3': index = 0x21; break;
        case '4': index = 0x22; break;
        case '5': index = 0x23; break;
        }
        break;
    }

    return 0x34 * index;
}

DataObject param::ShopDataFirst::data_;
