#pragma once
#include <globaldefs.h>

// message entry of a message file
struct UnkMessageEntry {
    unsigned short id_;                         // 0x00 id - first id
    unsigned short size_;                       // 0x02
    unsigned short offset_;                     // 0x04 in words
};

// message file header
struct UnkMessageFile {
    int unk_00;                                 // 0x00
    int unk_04;                                 // 0x04
    unsigned int firstId_;                      // 0x08
    unsigned int lastId_;                       // 0x0C
    unsigned int count_;                        // 0x10
    int unk_14;                                 // 0x14
    int unk_18;                                 // 0x18
    int unk_1c;                                 // 0x1C
    UnkMessageEntry entries_[1];                // 0x20
};

extern int data_020c4974;                       // last found message id
extern int data_020c4978;                       // last found message size
extern unsigned char* data_02120544;            // last found message

char* unkfunc_02088484(char* addr, unsigned int msg_id);    // find a message (NULL: not found)
