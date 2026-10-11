#pragma once
#include "main/dss/DssUtils.hpp"
#include <globaldefs.h>
#include "main/data/DataObject.hpp"


namespace param{
    struct MonsterAnim    
    {
        fx32 scale;
        unsigned short monster;
        unsigned short action;
        unsigned short sound;
        unsigned short hitframe;
        unsigned short startframe;
        unsigned short wait;
        unsigned char anim;
        unsigned char effect;
        unsigned char animfile;
        unsigned char camera;
        unsigned char camera2;
        char byte_1;
        unsigned char dmmy0;
        unsigned char dmmy1;
        static const char* filename_;

        static unsigned short getDataIndex(unsigned int index);
        static int getAnimData(MonsterAnim* anim, unsigned int monsterId, unsigned short actionId, unsigned short animId);
        static DataObject data_;
    };
}
