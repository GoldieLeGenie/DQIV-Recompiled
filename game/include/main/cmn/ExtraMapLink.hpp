#pragma once
#include "main/cmn/CommonCalculate.hpp"
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/global/StageLink.hpp"

namespace cmn
{
    enum LINK_TYPE
    {
        LINK_DEFAULT             = 0x0,
        LINK_TOWN_TO_TOWN        = 0x1,
        LINK_TOWN_TO_FIELD       = 0x2,
        LINK_FIELD_TO_TOWN       = 0x3,
        NOT_LINK_THIS_TOWN       = 0x4,
        LINK_EXTRA_TOWN          = 0x5,
        EXIT_TOWN                = 0x6,
        RECT_FIELD_TO_TOWN       = 0x7,
        RECT_FIELD_TO_WORLD      = 0x8,
        RECT_FIELD_TO_YAMI       = 0x9,
        RECT_FIELD_TO_GOT        = 0xA,
        LINK_FIELD_ABS_POS       = 0xB,
        BLOCK_FIELD_SYMBOL       = 0xC,
        LINK_TOWN_OFFSET         = 0xD,
        LINK_TOWN_TO_TOWN_OFFSET = 0xE,
    };

    struct MAP_LINK_DATA                    //
    {
        int nowId;                          // 0x00
        int nextId;                         // 0x04
        dss::Fix32Vector3 offsetData;        // 0x08
        int fieldType;                      // 0x14
        LINK_TYPE type;                     // 0x18
        char nowMapName[10];                // 0x1C
        char nextMapName[10];               // 0x26
    };

    struct RECT_LINK_DATA                   // 
    {
        int type;                           // 0x00
        int nowId;                          // 0x04
        int nextId;                         // 0x08
        char nextMapName[10];               // 0x0C
        dss::Fix32Vector3 pos[3];            // 
    };

    struct ExtraMapLink                     // 
    {
        int mapLinkDataCount_;              // 
        int rectLinkCount_;                 // 
        MAP_LINK_DATA mapLinkData_[40];     // 
        RECT_LINK_DATA rectLinkData_[5];    // 
        int extraLinkNo_;                   // 
        int extraLink_;                     // 
        int nextId_;                        // 
        dss::Fix32Vector3 offset_;           // 
        dss::Fix32Vector3 extraPos_;         // 
        short extraIdx_;                    // 
        int offsetFlag_;                  // 
        dss::Fix32Vector3 posTopLeft_;       // 
        dss::Fix32Vector3 posBottomRight_;   // 

        void setup();
        void setExtraFieldPos(dss::Fix32Vector3& pos, short& idx);
        int checkFieldLink(int id);
        bool checkExtraTownPos(dss::Fix32Vector3& pos, short& idx);
        void startExitLoop();
        void setExtraExitTown(const char* mapName, int id);
        void setExtraLinkTown(const char* mapName, dss::Fix32Vector3& pos, short dir);
        int checkTownMapLink(int id);
        int getFieldTypeBySurface(int surfaceId);
        int getFieldTypeBySymbol(int symbolId);
        void eraseSurface(int surfaceId, LINK_TYPE type, const char* mapName);
        int checkFieldRectLinkNo(dss::Fix32Vector3& pos);
        int checkFieldRectLinkByType(dss::Fix32Vector3& pos, int type);
        bool checkEraseSymbolNo(int id);
        void setRuraLink();
        void setRanaLink();
        void setFieldRectLinkToField(dss::Fix32Vector3& topLeft, dss::Fix32Vector3& bottomRight, int type, dss::Fix32Vector3 pos);
        void setFieldRectLinkToTown(dss::Fix32Vector3& topLeft, dss::Fix32Vector3& bottomRight, int nowId, int nextId, const char* mapName, dss::Fix32Vector3 offset);
        void setExtraExitField(int id, dss::Fix32Vector3& pos);
        void setTownINN();
        void setMonstarBookLink();
        void setExtraLinkFieldAbsPos(int fieldType, dss::Fix32Vector3& pos, short dir);
        void setLinkData(int nowId, int nextId, LINK_TYPE type, const char* nowMapName, const char* nextMapName);
        void setLinkData(int nowId, int nextId, LINK_TYPE type, const char* nowMapName, const char* nextMapName,dss::Fix32Vector3& offset);
        void setData(int index, int nowId, int nextId, LINK_TYPE type, const char* nowMapName, const char* nextMapName,
                     dss::Fix32Vector3& offset);
    };

    extern ExtraMapLink g_extraMapLink;    // data_020ed28c
}

struct Global;


