#include "main/data/ExcelBinaryData.hpp"

ARM void* ExcelBinaryData::checkSum(void* data, long id)
{
    return (char*)data + 4;
}

ARM void* ExcelBinaryData::readFileData(DataObject* data, const char* filename)
{
    data->setup(filename, 0, 0);
    return data->getAddr();
}

ARM void ExcelBinaryData::clearData(DataObject* data)
{
    data->cleanup();
}
