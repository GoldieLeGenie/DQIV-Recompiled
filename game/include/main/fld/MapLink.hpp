#pragma once
#include <globaldefs.h>

struct MAP_LINK_IDX {
    unsigned short name;                        // 0x00
    unsigned short n_exit;                      // 0x02
};

struct MAP_LINK_TBL {
    unsigned short exit_id;                     // 0x00
    unsigned short target_id;                   // 0x02
    unsigned short name;                        // 0x04
};

struct CMapLink {
    struct chunk {
        unsigned int id;                        // 0x00
        unsigned int size;                      // 0x04
    };

    char* m_name;                               // 0x00
    void* m_data;                               // 0x04
    unsigned short* m_hash;                     // 0x08
    unsigned short m_data_num;                  // 0x0C
    int m_exit_id;                              // 0x10

    void setup(void* data);
    char* search(const char* map_name, int exit_id);
    int hash(const char* name);
};
