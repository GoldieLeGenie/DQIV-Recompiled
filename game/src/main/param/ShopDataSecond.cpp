#include "main/param/ShopDataSecond.hpp"

THUMB int param::ShopDataSecond::getIndex(char *name)
{
    int index = 0;

    switch (100 * name[0] + name[1])
    {
    case 0x2901: index = 0x00; break;  // "ha"
    case 0x2AF5: index = 0x01; break;  // "ma"
    case 0x2AF6: index = 0x02; break;  // "mb"
    case 0x2AF7: index = 0x03; break;  // "mc"
    case 0x2AF8: index = 0x04; break;  // "md"
    case 0x2AF9: index = 0x05; break;  // "me"
    case 0x2D4E: index = 0x06; break;  // "sb"
    case 0x2903: index = 0x07; break;  // "hc"
    case 0x270F: index = 0x08; break;  // "cc"
    case 0x2DB2: index = 0x09; break;  // "tb"
    case 0x2AFA: index = 0x0A; break;  // "mf"
    case 0x2AFB: index = 0x0B; break;  // "mg"
    case 0x2710: index = 0x0C; break;  // "cd"
    case 0x2AFC: index = 0x0D; break;  // "mh"
    case 0x2AFD: index = 0x0E; break;  // "mi"
    case 0x2711: index = 0x0F; break;  // "ce"
    case 0x2AFE: index = 0x10; break;  // "mj"
    case 0x2AFF: index = 0x11; break;  // "mk"
    case 0x27E2: index = 0x12; break;  // "en"
    }

    return 0x34 * index;
}

DataObject param::ShopDataSecond::data_;
