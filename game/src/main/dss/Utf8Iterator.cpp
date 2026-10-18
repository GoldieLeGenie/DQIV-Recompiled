#pragma ipa file

#include "main/dss/Utf8Iterator.hpp"

ARM TextIterator::TextIterator()
{
    encoding_ = 0;
    buf_ = NULL;
    pos_ = 0;
    count_ = 0;
    length_ = 0;
    size_ = 0;
    countValid_ = 0;
    lengthValid_ = 0;
}

ARM void TextIterator::setText(const char* text)
{
    buf_ = (char*)text;
    pos_ = 0;
    count_ = 0;
    countValid_ = 0;
    length_ = 0;
    lengthValid_ = 0;
    size_ = 0;
}

ARM void TextIterator::setBuffer(char* text, int size)
{
    buf_ = text;
    pos_ = 0;
    count_ = 0;
    countValid_ = 0;
    length_ = 0;
    lengthValid_ = 0;
    size_ = size;
}

ARM void TextIterator::initBuffer(char* buf, int size)
{
    buf_ = buf;
    pos_ = 0;
    count_ = 0;
    countValid_ = 0;
    length_ = 0;
    lengthValid_ = 0;
    size_ = size;
    buf_[pos_] = 0;
    buf_[pos_ + 1] = 0;
}

ARM int TextIterator::getCount()
{
    if (countValid_ == 0) {
        count_ = countChars();
        countValid_ = 1;
    }
    return count_;
}

ARM int TextIterator::getLength()
{
    if (lengthValid_ == 0) {
        length_ = getByteLength();
        lengthValid_ = 1;
    }
    return length_;
}

ARM void TextIterator::truncate(int pos)
{
    buf_[pos] = 0;
    countValid_ = 0;
    lengthValid_ = 0;
}

ARM Utf8Iterator::Utf8Iterator()
{
    encoding_ = 1;
}

ARM unsigned short Utf8Iterator::getChar()
{
    return decodeChar((unsigned char*)buf_ + pos_);
}

ARM int Utf8Iterator::putChar(int c)
{
    getCount();
    int length = getLength();
    if ((unsigned int)(length + 4) > (unsigned int)size_) {
        return 0;
    }
    int size = encodeChar((unsigned char*)buf_ + length, c);
    if ((unsigned int)(length_ + size) < (unsigned int)size_) {
        count_++;
        length_ += size;
        return 1;
    }
    buf_[length] = 0;
    return 0;
}

ARM int Utf8Iterator::nextChar()
{
    int size = getCharSize((unsigned char*)buf_ + pos_);
    if (size == -1) {
        return 0;
    }
    if (size == 0) {
        return 0;
    }
    pos_ += size;
    return 1;
}

ARM int Utf8Iterator::prevChar()
{
    if (pos_ == 0) {
        return 0;
    }
    while (pos_ != 0) {
        pos_--;
        if (isLeadByte((unsigned char*)buf_ + pos_) == 1) {
            break;
        }
    }
    return 1;
}

ARM int Utf8Iterator::countChars()
{
    if (buf_ == NULL) {
        return 0;
    }
    int pos = pos_;
    getCharSize((unsigned char*)buf_);
    int count = 0;
    pos_ = 0;
    while (nextChar()) {
        count++;
    }
    pos_ = pos;
    return count;
}

ARM int Utf8Iterator::getByteLength()
{
    int length = 0;
    if (buf_ == NULL) {
        return length;
    }
    for (const unsigned char* p = (unsigned char*)buf_; *p != 0; p++) {
        length++;
    }
    return length;
}

ARM int Utf8Iterator::isLeadByte(const unsigned char* p)
{
    return (*p & 0xc0) != 0x80;
}

ARM int Utf8Iterator::getCharSize(const unsigned char* p)
{
    unsigned char c = *p;
    if (c == 0) {
        return 0;
    }
    if ((c & 0x80) == 0) {
        return 1;
    }
    if ((c & 0xe0) == 0xc0) {
        return 2;
    }
    if ((c & 0xf0) == 0xe0) {
        return 3;
    }
    return -1;
}

ARM unsigned short Utf8Iterator::decodeChar(const unsigned char* p)
{
    int size = getCharSize(p);
    if (size == 0) {
        return 0;
    }
    if (size == 1) {
        return p[0];
    }
    if (size == 2) {
        unsigned short c0 = p[0] & 0x1f;
        unsigned short c1 = p[1] & 0x3f;
        return (unsigned short)((unsigned short)(c0 << 6) | c1);
    }
    if (size == 3) {
        unsigned short c0 = p[0] & 0xf;
        unsigned short c1 = p[1] & 0x3f;
        unsigned short c2 = p[2] & 0x3f;
        c0 = c0 << 12;
        c1 = c1 << 6;
        return (unsigned short)(c0 | c1 | c2);
    }
    return 0xffff;
}

ARM int Utf8Iterator::encodeChar(unsigned char* p, int c)
{
    if ((unsigned int)c <= 0x7f) {
        p[0] = c & 0x7f;
        p[1] = 0;
        return 1;
    }
    if ((unsigned int)c <= 0x7ff) {
        p[0] = ((c >> 6) & 0x1f) | 0xc0;
        p[1] = (c & 0x3f) | 0x80;
        p[2] = 0;
        return 2;
    }
    p[0] = ((c >> 12) & 0xf) | 0xe0;
    p[1] = ((c >> 6) & 0x3f) | 0x80;
    p[2] = (c & 0x3f) | 0x80;
    p[3] = 0;
    return 3;
}

static inline int isSeparator(int c)
{
    int separator = 0;
    if (c == 9) {
        separator = 1;
    }
    if (c == 0x20) {
        separator = 1;
    }
    if (c == 0xd) {
        separator = 1;
    }
    if (c == 0xa) {
        separator = 1;
    }
    if (c == 0x3000) {
        separator = 1;
    }
    if (c == 0xff0e) {
        separator = 1;
    }
    if (c == 0x2026) {
        separator = 1;
    }
    if (c == 0xff1f) {
        separator = 1;
    }
    if (c == 0xff01) {
        separator = 1;
    }
    return separator;
}

ARM int readNextWord(TextIterator* src, char* buf, int size)
{
    int found = 0;
    Utf8Iterator dst;
    dst.initBuffer(buf, size);
    while (true) {
        int c = src->getChar();
        if (c == 0 || c == 0xffff) {
            break;
        }
        if (!isSeparator(c)) {
            break;
        }
        src->nextChar();
    }
    while (true) {
        int c = src->getChar();
        src->nextChar();
        if (c == 0 || c == 0xffff || c == 9 || c == 0x20 || c == 0xd || c == 0xa || c == 0x3000 || c == 0xff0e ||
            c == 0x2026 || c == 0xff1f || c == 0xff01) {
            break;
        }
        dst.putChar(c);
        found = 1;
    }
    return found;
}
