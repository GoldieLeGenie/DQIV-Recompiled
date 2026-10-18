#pragma once
#include <globaldefs.h>

struct Utf8Iterator;

// Character access interface of the text iterators
struct TextInterface {
    virtual unsigned short getChar() = 0;         // current char
    virtual int putChar(int c) = 0;    // append a char
    virtual int nextChar() = 0;         // next char
    virtual int prevChar() = 0;         // previous char
    virtual int countChars() = 0;         // char count
    virtual int getByteLength() = 0;         // byte length
};

// Text cursor over a char buffer: read (setText + getChar/nextChar) or write (initBuffer +
// putChar)
struct TextIterator : TextInterface {
    int unk_04;                                 // 0x04
    char* buf_;                                 // 0x08
    int pos_;                                   // 0x0C
    int count_;                                 // 0x10
    int length_;                                // 0x14
    int size_;                                  // 0x18
    int countValid_;                            // 0x1C
    int lengthValid_;                           // 0x20

    TextIterator();
    void setText(const char* text);
    void setBuffer(char* text, int size);
    void initBuffer(char* buf, int size);
    int getCount();                     // cached char count
    int getLength();                     // cached byte length
    void truncate(int pos);             // cut the text
};

// UTF-8 text (1 to 3 bytes per char)
struct Utf8Iterator : TextIterator {
    Utf8Iterator();
    virtual unsigned short getChar();
    virtual int putChar(int c);
    virtual int nextChar();
    virtual int prevChar();
    virtual int countChars();
    virtual int getByteLength();
    int isLeadByte(const unsigned char* p);   // first byte of a char
    int getCharSize(const unsigned char* p);   // char size (-1: invalid)
    unsigned short decodeChar(const unsigned char* p);    // decode
    int encodeChar(unsigned char* p, int c);  // encode
};

int readNextWord(TextIterator* src, char* buf, int size);   // read the next word
