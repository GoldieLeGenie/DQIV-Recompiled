#include "main/param/Fukuro.hpp"
#include "main/param/Event.hpp"

char player_fukuro1[36] = "data/param/param_player_fukuro1.dat"; // 0x020bc5cc

THUMB param::Fukuro *param::Fukuro::getFileData(unsigned int index)
{
    char *name;
    if (index < 0x15B)
        name = player_fukuro1;

    void* addr = ExcelBinaryData::readFileData(&param::Fukuro::data_, name);
    param::Fukuro *data = (param::Fukuro*)ExcelBinaryData::checkSum(addr, 0x02CE0284);

    unsigned int i = 0;
    do
    {
        if (index == data->index)
            return data;
        i++;
        data++;
    } while (i < 0x15B);
    return data;
}

DataObject param::Fukuro::data_;
