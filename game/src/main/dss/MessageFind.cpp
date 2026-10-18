#include "main/dss/MessageFind.hpp"

int g_lastFoundMsgSize = -1;
int g_lastFoundMsgId = -1;
unsigned char* g_lastFoundMessage;

ARM char* findMessage(char* addr, unsigned int msg_id)
{
    if (addr == NULL) {
        return NULL;
    }
    UnkMessageFile* file = (UnkMessageFile*)addr;
    unsigned int first = file->firstId_;
    unsigned int last = file->lastId_;
    unsigned int count = file->count_;
    int offset1 = file->unk_14;
    int offset2 = file->unk_18;
    g_lastFoundMsgId = -1;
    g_lastFoundMessage = NULL;
    g_lastFoundMsgSize = -1;
    if (first > msg_id) {
        return NULL;
    }
    if (last < msg_id) {
        return NULL;
    }
    msg_id -= first;
    UnkMessageEntry* entry = file->entries_;
    for (unsigned int i = 0; i < count; i++) {
        if (entry->id_ == msg_id) {
            int size = entry->size_;
            char* msg = addr + (offset1 + offset2 + (entry->offset_ << 2));
            g_lastFoundMsgId = msg_id;
            g_lastFoundMessage = (unsigned char*)msg;
            g_lastFoundMsgSize = size;
            return msg;
        }
        entry++;
    }
    return NULL;
}
