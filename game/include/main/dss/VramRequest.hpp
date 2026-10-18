#pragma once
#include <globaldefs.h>
#include "nitro/gx.h"

// OAM shadow buffer of one screen
struct OamBuffer {
    GXOamAttr oam_[128];                        // 0x000
    int count_;                                 // 0x400
    int prevCount_;                             // 0x404
    int unk_408;                                // 0x408

    void init();                    // init
    void clear();                    // clear
    void requestTransfer(int type);            // queue the transfer
    GXOamAttr* allocEntry();              // next free entry
};

// palette shadow buffer (16 palettes of 16 colors)
struct PaletteBuffer {
    unsigned short color_[0x100];               // 0x000

    void requestTransfer(int type);            // queue the transfer
    void setColor(int index, int color, unsigned short rgb);
    void loadColors(int index, int color, const void* src, int count);
};

// queued copy to the VRAM, OAM or palettes
struct VramRequest {
    int active_;                                // 0x00
    int type_;                                  // 0x04
    int offset_;                                // 0x08
    void* base_;                                // 0x0C
    const void* src_;                           // 0x10
    int size_;                                  // 0x14
    void* buffer_;                              // 0x18 freed after the copy
};

struct VramRequestQueue {
    VramRequest request_[128];               // 0x000
    int size_;                                  // 0xE00 bytes copied by the last update
    int lines_;                                 // 0xE04 lines spent by the last update

    VramRequestQueue() { init(); }
    void init();                    // init
    void* getDestination(int type);           // destination of a type
    int executeRequest(VramRequest* request);  // copy
};

extern VramRequestQueue g_VramRequestQueue;

void flushVramRequests();                        // copy all the queued requests
VramRequest* requestCompressedVramCopy(int type, int offset, void* data, int size);   // queue compressed data
VramRequest* requestVramCopy(int type, int offset, const void* src, int size);  // queue
int getBgCharVramType(int bg);                   // BG character type (0-3 main, 4-7 sub)
int getBgScreenVramType(int bg);                   // BG screen type (0-3 main, 4-7 sub)

extern "C" {
    void func_0206785c(unsigned int value, void* dest, unsigned int size);  // fill
    void* func_02064d68(void);                  // BG0 character
    void* func_02064dbc(void);                  // BG1 character
    void* func_02064e10(void);                  // BG2 character
    void* func_02064ea0(void);                  // BG3 character
    void* func_02064d9c(void);                  // sub BG0 character
    void* func_02064df0(void);                  // sub BG1 character
    void* func_02064e60(void);                  // sub BG2 character
    void* func_02064ef8(void);                  // sub BG3 character
    void* func_02064ad0(void);                  // BG0 screen
    void* func_02064b24(void);                  // BG1 screen
    void* func_02064b78(void);                  // BG2 screen
    void* func_02064c70(void);                  // BG3 screen
    void* func_02064b04(void);                  // sub BG0 screen
    void* func_02064b58(void);                  // sub BG1 screen
    void* func_02064bfc(void);                  // sub BG2 screen
    void* func_02064cf4(void);                  // sub BG3 screen
}
