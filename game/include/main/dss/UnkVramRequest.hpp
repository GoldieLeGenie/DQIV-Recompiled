#pragma once
#include <globaldefs.h>
#include "nitro/gx.h"

// OAM shadow buffer of one screen
struct UnkOamBuffer {
    GXOamAttr oam_[128];                        // 0x000
    int count_;                                 // 0x400
    int prevCount_;                             // 0x404
    int unk_408;                                // 0x408

    void unkfunc_02082644();                    // init
    void unkfunc_02082648();                    // clear
    void unkfunc_02082678(int type);            // queue the transfer
    GXOamAttr* unkfunc_02082694();              // next free entry
};

// palette shadow buffer (16 palettes of 16 colors)
struct UnkPaletteBuffer {
    unsigned short color_[0x100];               // 0x000

    void unkfunc_020826ac(int type);            // queue the transfer
    void unkfunc_020826c8(int index, int color, unsigned short rgb);
    void unkfunc_020826d8(int index, int color, const void* src, int count);
};

// queued copy to the VRAM, OAM or palettes
struct UnkVramRequest {
    int active_;                                // 0x00
    int type_;                                  // 0x04
    int offset_;                                // 0x08
    void* base_;                                // 0x0C
    const void* src_;                           // 0x10
    int size_;                                  // 0x14
    void* buffer_;                              // 0x18 freed after the copy
};

struct UnkVramRequestQueue {
    UnkVramRequest request_[128];               // 0x000
    int size_;                                  // 0xE00 bytes copied by the last update
    int lines_;                                 // 0xE04 lines spent by the last update

    UnkVramRequestQueue() { unkfunc_020826fc(); }
    void unkfunc_020826fc();                    // init
    void* unkfunc_02082868(int type);           // destination of a type
    int unkfunc_020829e0(UnkVramRequest* request);  // copy
};

extern UnkVramRequestQueue data_0211c508;

void unkfunc_02082724();                        // copy all the queued requests
UnkVramRequest* unkfunc_02082794(int type, int offset, void* data, int size);   // queue compressed data
UnkVramRequest* unkfunc_020827f0(int type, int offset, const void* src, int size);  // queue
int unkfunc_02082a30(int bg);                   // BG character type (0-3 main, 4-7 sub)
int unkfunc_02082aa4(int bg);                   // BG screen type (0-3 main, 4-7 sub)

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
