#pragma once
#include "globaldefs.h"

struct CNK_CHANGE_INFO {
    unsigned short id;                          // 0x00
    short sx;                                   // 0x02
    short sy;                                   // 0x04
    short ex;                                   // 0x06
    short ey;                                   // 0x08
    int offset;                                 // 0x0C
};

struct CNK_TEX {
    unsigned char attr;                         // 0x00
    unsigned char aflag;                        // 0x01
    unsigned short ano;                         // 0x02
    unsigned short no;                          // 0x04
};

struct WorldSymbolData {
    unsigned char flags;                        // 0x00
    unsigned char id;                           // 0x01
    unsigned short uid;                         // 0x02
    short x;                                    // 0x04
    short y;                                    // 0x06
    unsigned char w;                            // 0x08
    unsigned char h;                            // 0x09
};

// field map file (data/field/bin/*map.bin): WSYN, WCHA, WMAP, WCMA, WTEX and SDAT chunks
struct CWorldMap {
    struct header {
        unsigned int id;                        // 0x00
        unsigned int version;                   // 0x04
        unsigned char reserve[8];               // 0x08
    };

    struct chunk {
        unsigned int id;                        // 0x00
        unsigned int size;                      // 0x04
    };

    unsigned char* m_size;                      // 0x00 WSYN: block count x/y, block size x/y
    CNK_CHANGE_INFO* m_change_info;             // 0x04
    short* m_map;                               // 0x08
    short* m_change_map;                        // 0x0C
    CNK_TEX* m_tex;                             // 0x10
    chunk* m_symbol_chunk;                      // 0x14 SDAT, read by CWorldSymbol::setup_symbol
    unsigned short m_change_num;                // 0x18
    unsigned short m_tex_num;                   // 0x1A

    CWorldMap();
    ~CWorldMap();
    void setup(void* data);
    int getAttr(int x, int y);
    int getWorldNo(int x, int y);
    void worldChange(int no);
};

// symbols of the field map (SDAT chunk), stored right after the CWorldMap in FieldData
struct CWorldSymbol {
    WorldSymbolData* m_symbol_data;             // 0x00
    volatile unsigned short m_symbol_num;       // 0x04

    void setup_symbol(void* data);
};
