#pragma once
#include <globaldefs.h>

// message entry of a message file
struct MessageEntry {
    unsigned short id_;                         // 0x00 id - first id
    unsigned short size_;                       // 0x02
    unsigned short offset_;                     // 0x04 in words
};

// message file header
struct MessageFileHeader {
    int unk_00;                                 // 0x00
    int unk_04;                                 // 0x04
    unsigned int firstId_;                      // 0x08
    unsigned int lastId_;                       // 0x0C
    unsigned int count_;                        // 0x10
    int entriesOffset_;                                 // 0x14
    int entriesSize_;                                 // 0x18
    int unk_1c;                                 // 0x1C
    MessageEntry entries_[1];                // 0x20
};

extern int g_lastFoundMsgId;                       // last found message id
extern int g_lastFoundMsgSize;                       // last found message size
extern unsigned char* g_lastFoundMessage;            // last found message

char* findMessage(char* addr, unsigned int msg_id);    // find a message (NULL: not found)
