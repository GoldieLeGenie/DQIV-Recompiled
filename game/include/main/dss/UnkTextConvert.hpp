#pragma once
#include <globaldefs.h>
#include "main/dss/Utf8Iterator.hpp"

// UTF-16 text (2 bytes per char)
struct Utf16Iterator : UnkTextIterator {
    Utf16Iterator();
    virtual unsigned short unkfunc_0208771c();
    virtual int unkfunc_02087734(int c);
    virtual int unkfunc_020877b8();
    virtual int unkfunc_02087800();
    virtual int unkfunc_02087850();
    virtual int unkfunc_020878b8();
};

// Unicode tables of the Shift-JIS chars (file data)
struct UnkSjisTable {
    unsigned short* tableE0_;                   // 0x00 lead bytes 0xE0-0xEF
    int sizeE0_;                                // 0x04
    int size81_;                                // 0x08
    unsigned short* table81_;                   // 0x0C lead bytes 0x81-0x9F
};

extern unsigned short data_020c455c[21];        // normal kana
extern unsigned short data_020c4586[21];        // small kana
extern unsigned short data_020c4618[52];        // lower case
extern unsigned short data_020c45b0[52];        // upper case
extern unsigned short data_020c4680[85];        // katakana
extern unsigned short data_020c472a[85];        // hiragana
extern unsigned short data_020c4894[96];        // ASCII
extern unsigned short data_020c47d4[96];        // full width
extern UnkSjisTable data_0211fd34;
extern unsigned char data_0211fd44[0x800];

void unkfunc_02087bd0(unsigned short* table81, int size81, unsigned short* tableE0, int sizeE0);
int unkfunc_02087c00(char* dst, int size, const char* src);                                     // UTF-16 -> UTF-8
unsigned short unkfunc_02087c98(unsigned short c);                                              // Unicode -> Shift-JIS
int unkfunc_02087d48(char* dst, unsigned int size, const char* src);                            // UTF-8 -> Shift-JIS
int unkfunc_02087e08(char* dst, int size, const char* src);                                     // Shift-JIS -> UTF-8
int unkfunc_02087ee4(unsigned short* from, unsigned short* to, int c);                          // to[i] -> from[i]
int unkfunc_02087f14(unsigned short* from, unsigned short* to, char* dst, int size, const char* src);
int unkfunc_02087fbc(unsigned short* from, unsigned short* to, char* dst, int size, const char* src, int count);
char* unkfunc_02088078(const char* src);                                                        // UTF-8 -> Shift-JIS work buffer
