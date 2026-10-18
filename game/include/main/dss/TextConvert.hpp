#pragma once
#include <globaldefs.h>
#include "main/dss/Utf8Iterator.hpp"

// UTF-16 text (2 bytes per char)
struct Utf16Iterator : TextIterator {
    Utf16Iterator();
    virtual unsigned short getChar();
    virtual int putChar(int c);
    virtual int nextChar();
    virtual int prevChar();
    virtual int countChars();
    virtual int getByteLength();
};

// Unicode tables of the Shift-JIS chars (file data)
struct SjisTables {
    unsigned short* tableE0_;                   // 0x00 lead bytes 0xE0-0xEF
    int sizeE0_;                                // 0x04
    int size81_;                                // 0x08
    unsigned short* table81_;                   // 0x0C lead bytes 0x81-0x9F
};

extern unsigned short g_normalKanaCharTable[21];        // normal kana
extern unsigned short g_smallKanaCharTable[21];        // small kana
extern unsigned short g_lowerCaseCharTable[52];        // lower case
extern unsigned short g_upperCaseCharTable[52];        // upper case
extern unsigned short g_katakanaCharTable[85];        // katakana
extern unsigned short g_hiraganaCharTable[85];        // hiragana
extern unsigned short g_asciiCharTable[96];        // ASCII
extern unsigned short g_fullWidthCharTable[96];        // full width
extern SjisTables g_sjisTables;
extern unsigned char g_sjisTextBuffer[0x800];

void initSjisTables(unsigned short* table81, int size81, unsigned short* tableE0, int sizeE0);
int convertUtf16ToUtf8(char* dst, int size, const char* src);                                     // UTF-16 -> UTF-8
unsigned short unicodeToSjis(unsigned short c);                                              // Unicode -> Shift-JIS
int convertUtf8ToSjis(char* dst, unsigned int size, const char* src);                            // UTF-8 -> Shift-JIS
int convertSjisToUtf8(char* dst, int size, const char* src);                                     // Shift-JIS -> UTF-8
int translateChar(unsigned short* from, unsigned short* to, int c);                          // to[i] -> from[i]
int translateText(unsigned short* from, unsigned short* to, char* dst, int size, const char* src);
int translateTextPrefix(unsigned short* from, unsigned short* to, char* dst, int size, const char* src, int count);
char* convertUtf8ToSjisWork(const char* src);                                                        // UTF-8 -> Shift-JIS work buffer
