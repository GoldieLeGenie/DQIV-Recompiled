#pragma once
#include "globaldefs.h"
#include "main/param/Param.hpp"
#include "GameInfo.hpp"

namespace cmn
{
    struct CommonRuraData {
        param::VehicleData *vehicle;
        static cmn::CommonRuraData* getSingleton();
        CommonRuraData();
        ~CommonRuraData();
        void initialize();
        void setEnableRura(int townID);
        void setEnableRuraMap(int flag);
        int isEnableRuraBeforeChapter5(int count);
        int isEnableRura(int townID);
        int getRuraCount();
        int getChapterRuraCount();
        void setRuraFlagAll();
        void setGlobalRuraFlagAll();
        int getRenameFlag();
        int getSymbolID(int townID);
        dss::Fix32Vector3 getShipTownPos(int townID);
        dss::Fix32Vector3 getBalloonTownPos(int townID);
        int getWorld(int townID);
        char* get_TATOP_Name();

    };
}

struct RuraMapEntry {
    int mapId;
    int group;    //chapter
};
extern const RuraMapEntry storyFlagCompare[42];   // 0x020b5fbc

extern char* p_tatop;
extern int   unusedRuraData;
extern char  tatop[8];
extern int   unusedRuraBss;
