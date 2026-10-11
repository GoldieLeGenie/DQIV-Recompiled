#pragma once
#include <globaldefs.h>

struct Utf8Iterator;

// Character access interface of the text iterators
struct UnkTextInterface {
    virtual unsigned short unkfunc_0208771c() = 0;         // current char
    virtual int unkfunc_02087734(int c) = 0;    // append a char
    virtual int unkfunc_020877b8() = 0;         // next char
    virtual int unkfunc_02087800() = 0;         // previous char
    virtual int unkfunc_02087850() = 0;         // char count
    virtual int unkfunc_020878b8() = 0;         // byte length
};

// Text cursor over a char buffer: read (unkfunc_020875ec + unkfunc_0208771c/unkfunc_020877b8) or write (unkfunc_02087634 +
// unkfunc_02087734)
struct UnkTextIterator : UnkTextInterface {
    int unk_04;                                 // 0x04
    char* buf_;                                 // 0x08
    int pos_;                                   // 0x0C
    int count_;                                 // 0x10
    int length_;                                // 0x14
    int size_;                                  // 0x18
    int countValid_;                            // 0x1C
    int lengthValid_;                           // 0x20

    UnkTextIterator();
    void unkfunc_020875ec(const char* text);
    void unkfunc_02087610(char* text, int size);
    void unkfunc_02087634(char* buf, int size);
    int unkfunc_02087674();                     // cached char count
    int unkfunc_020876a8();                     // cached byte length
    void unkfunc_020876dc(int pos);             // cut the text
};

// UTF-8 text (1 to 3 bytes per char)
struct Utf8Iterator : UnkTextIterator {
    Utf8Iterator();
    virtual unsigned short unkfunc_0208771c();
    virtual int unkfunc_02087734(int c);
    virtual int unkfunc_020877b8();
    virtual int unkfunc_02087800();
    virtual int unkfunc_02087850();
    virtual int unkfunc_020878b8();
    int unkfunc_020878e8(const unsigned char* p);   // first byte of a char
    int unkfunc_02087900(const unsigned char* p);   // char size (-1: invalid)
    unsigned short unkfunc_02087940(const unsigned char* p);    // decode
    int unkfunc_020879dc(unsigned char* p, int c);  // encode
};

int unkfunc_02087a74(UnkTextIterator* src, char* buf, int size);   // read the next word
