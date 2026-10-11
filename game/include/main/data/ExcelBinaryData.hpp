#pragma once
#include <globaldefs.h>
#include "main/data/DataObject.hpp"

struct ExcelBinaryData {
    static void* readFileData(DataObject* data, const char* filename);
    static void clearData(DataObject* data);
    static void* checkSum(void* data, long id);
};
