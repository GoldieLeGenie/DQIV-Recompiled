#pragma once
#include <globaldefs.h>
#include "main/data/DataObject.hpp"

struct CsvRow {
    short column_[8];                           // 0x00

    void unkfunc_020571c4();
    void unkfunc_020571e4(int index, int offset);
    short unkfunc_020571f0(int index);
};

struct CsvData {
    char* text_;                                // 0x00
    unsigned int size_;                         // 0x04
    CsvRow* row_;                               // 0x08
    unsigned int rowCount_;                     // 0x0C
    DataObject data_;                           // 0x10

    CsvData();
    ~CsvData();
    void unkfunc_02057234(const char* filename, int a);
    void unkfunc_0205726c(const char* filename, int a);
    char* unkfunc_02057368(int row, int column);
};
