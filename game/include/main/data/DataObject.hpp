#pragma once
#include <globaldefs.h>
#include "main/dss/UnkMemory.hpp"

struct DataObject {
    // vtable                                   // 0x00
    int m_flag;                                 // 0x04
    void* m_addr;                               // 0x08
    long m_size;                                // 0x0C

    DataObject();
    ~DataObject();
    virtual void setup(const char* filename, int a, int b);
    virtual void setup(void* addr);
    void cleanup();
    void* getAddr();
    long getSize();
};

struct LZDataObject : DataObject {
    LZDataObject() {}
    ~LZDataObject() {}
    virtual void setup(const char* filename, int a, int b);
    virtual void setup(void* addr);
};
