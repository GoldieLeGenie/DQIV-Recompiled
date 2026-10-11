#include "main/cmn/CommonRuraData.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/status/GameFlag.hpp"

char* p_tatop = tatop;
const RuraMapEntry storyFlagCompare[42] = {   // 0x020b5fbc
    { 0x0F, 1 }, { 0x10, 1 }, { 0x0B, 2 }, { 0x0C, 2 }, { 0x1A, 2 }, { 0x0D, 2 },
    { 0x01, 2 }, { 0x1B, 3 }, { 0x02, 3 }, { 0x01, 3 }, { 0x0A, 4 }, { 0x1C, 4 },
    { 0x08, 4 }, { 0x09, 4 }, { 0x15, 4 }, { 0x00, 5 }, { 0x01, 5 }, { 0x02, 5 },
    { 0x03, 5 }, { 0x04, 5 }, { 0x05, 5 }, { 0x06, 5 }, { 0x07, 5 }, { 0x08, 5 },
    { 0x09, 5 }, { 0x0A, 5 }, { 0x0B, 5 }, { 0x0C, 5 }, { 0x0D, 5 }, { 0x0D, 5 },
    { 0x0E, 5 }, { 0x0F, 5 }, { 0x10, 5 }, { 0x11, 5 }, { 0x12, 5 }, { 0x13, 5 },
    { 0x14, 5 }, { 0x15, 5 }, { 0x16, 5 }, { 0x17, 5 }, { 0x18, 5 }, { 0x19, 5 }
};
char  tatop[8]   = "tatop";
#pragma explicit_zero_data on
int   unusedRuraData = 0;
#pragma explicit_zero_data reset
int   unusedRuraBss;



ARM cmn::CommonRuraData* cmn::CommonRuraData::getSingleton()
{
    static CommonRuraData fielTownData;
    return &fielTownData;
}

ARM cmn::CommonRuraData::CommonRuraData(){
}

ARM cmn::CommonRuraData::~CommonRuraData(){

}

ARM void cmn::CommonRuraData::initialize()
{
  this->vehicle = status::excelParam.vehicle_;
  return;
}


ARM void cmn::CommonRuraData::setEnableRura(int townID)
{
  g_Stage.ruraEnable_.flag_ |= 1 << townID;
}



ARM void cmn::CommonRuraData::setEnableRuraMap(int flag)
{
    setEnableRura(storyFlagCompare[flag].mapId);
}

ARM int cmn::CommonRuraData::isEnableRuraBeforeChapter5(int count)
{
    int i;
    if (status::g_Story.chapter_ >= 5)
    {
        return 0;
    }
    for (i = 0; i < 0x2A; i++)
    {
        if (status::g_Story.chapter_  == storyFlagCompare[i].group )
        {
            int mapId = storyFlagCompare[i + count].mapId;
            if (isEnableRura(mapId) == 0)
            {
                return -1;
            }
            return mapId;
        }
    }
    return -1;
}

ARM int cmn::CommonRuraData::isEnableRura(int townID)
{
    if ((g_Stage.ruraEnable_.flag_ & (1 << townID)) == 0)
    {
        return 0;
    }
    return 1;
}

ARM int cmn::CommonRuraData::getRuraCount()
{
    int count = 0;
    for (int i = 0; i < 0x2A; i++)
    {
        int chapter = status::g_Story.chapter_;
        if (chapter >= 5)
        {
            chapter = 5;
        }
        if (chapter == storyFlagCompare[i].group || chapter == 0)
        {
            if (isEnableRura(storyFlagCompare[i].mapId) != 0 && i != 0x1D)
            {
                count++;
            }
        }
    }
    return count;
}

ARM int cmn::CommonRuraData::getChapterRuraCount()
{
    int count = 0;
    int chapter = status::g_Story.chapter_;
    if (chapter > 5)
    {
        chapter = 5;
    }
    for (int i = 0; i < 0x2A; i++)
    {
        if (chapter == storyFlagCompare[i].group)
        {
            count++;
        }
    }
    return count;
}

ARM void cmn::CommonRuraData::setRuraFlagAll()
{
    int chapter = status::g_Story.chapter_;
    if (chapter > 5)
    {
        chapter = 5;
    }
    for (int i = 0; i < 0x2A; i++)
    {
        if (g_AreaFlag.check(i) != 0 && chapter == storyFlagCompare[i].group)
        {
            setEnableRuraMap(i);
        }
    }
}

ARM void cmn::CommonRuraData::setGlobalRuraFlagAll()
{
    for (int i = 0; i < 0x2A; i++)
    {
        if (isEnableRura(storyFlagCompare[i].mapId) == 1 &&
            storyFlagCompare[i].group <= status::g_Story.chapter_)
        {
            g_AreaFlag.set(i);
        }
    }
}

ARM int cmn::CommonRuraData::getRenameFlag() {
    return g_AreaFlag.check(0x1D);
}


ARM int cmn::CommonRuraData::getSymbolID(int townID)
{
  return this->vehicle[townID].id;
}

ARM dss::Fix32Vector3 cmn::CommonRuraData::getShipTownPos(int townID)
{
    dss::Fix32Vector3 pos;
    pos.vx.value = vehicle[townID].shipX;
    pos.vy.value = vehicle[townID].shipY;
    return pos;
}

ARM dss::Fix32Vector3 cmn::CommonRuraData::getBalloonTownPos(int townID)
{
    dss::Fix32Vector3 pos;
    switch (townID)
    {
        case 0x16:
        case 0x17:
        case 0x18:
        case 0x19:
            pos.vx.value = (fx32)0x4B8000;
            pos.vy.value = (fx32)0x5E8000;
            break;
        default:
            pos.vx.value = vehicle[townID].balloonX;
            pos.vy.value = vehicle[townID].balloonY;
            break;
    }
    return pos;
}

ARM int cmn::CommonRuraData::getWorld(int townID)
{
    unsigned int world = vehicle[townID].world;
    if (world >= 3)
    {
        world = 0;
    }
    return world;
}

ARM char* cmn::CommonRuraData::get_TATOP_Name()
{
    return p_tatop;
}