#include "main/param/Event.hpp"

char player_event3[36] = "data/param/param_player_event3.dat"; // 0x020bc5cc
char player_event2[36] = "data/param/param_player_event2.dat"; // 0x020bc5a8
char player_event1[36] = "data/param/param_player_event1.dat"; // 0x020bc584
char player_event4[36] = "data/param/param_player_event4.dat"; // 0x020bc5f0

THUMB param::Event* param::Event::getFileData(unsigned int id) {
    const char* filename = 0; 

    if (id < 0xB3U) {
        filename = player_event1;
    } else if (id < 0xF3U) {
        filename = player_event2;
    } else if (id < 0x132U) {
        filename = player_event3;
    } else if (id < 0x15CU) {
        filename = player_event4;
    }

    param::Event* result = (param::Event*)ExcelBinaryData::checkSum(
        ExcelBinaryData::readFileData(&param::Event::data_, filename),
        0xEA9C
    );

    for (unsigned int i = 0; i < 0x742U; i++) {
        if (id == result->index) { 
            break;
        }
        result++;
    }

    return result;
}

DataObject param::Event::data_;
