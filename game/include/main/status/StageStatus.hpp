#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/profile/Profile.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/param/Param.hpp"
#include "main/global/Global.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/profile/Profile.hpp"
#include "main/cmn/PlayerManager.hpp"

namespace status{
    struct StageFlaguint : dss::BitFlaguint {
        StageFlaguint() { flag_ = 0; }
    };
    struct StageFlagushort : dss::BitFlagushort {
        StageFlagushort() { flag_ = 0; }
    };
    struct StageFlaguchar : dss::BitFlaguchar {
        StageFlaguchar() { flag_ = 0; }
    };
    struct StageVector3short : dss::Vector3short {
        StageVector3short() { vx = 0; vy = 0; vz = 0; }
    };

    struct StageStatus {
        static int toramana_;  //data_020d08d0 
        StageFlaguint flag_;
        char mapName_[32];
        char* map_;
        char btlMapName_[32];
        char* btlMap_;
        char evBtlMapName_[32];
        char* evBtlMap_;
        char evChapelMapName_[32];
        char churchMapName_[32];
        char* churchMap_;
        enum TIME_ZONE timeZone_;
        int worldTime_;
        int symbolID_;
        int lastSave_;
        StageVector3short backupCameraAngle_;
        dss::Fix32Vector3 shipPosition_;
        dss::Fix32Vector3 balloonPosition_;
        int balloonFieldType_;
        dss::Fix32Vector3 overviewPosition_;
        dss::Fix32Vector3 overviewTempPosition_;
        int flagMapChange_;
        StageFlaguint ruraEnable_;
        int ruraFlag_;
        int ruraSymbol_;
        int maxKekai;
        int crusingPeopleEncount_;
        StageFlaguchar symbolFlag_[16];
        StageFlagushort mapVeil_[16];
        StageFlaguchar furnFlag_[256];
        StageFlaguchar openFlag_[16];
        StageFlaguchar breakFlag_[8];
        int timestop_;
        int fallFlag_;
        struct IdoLink {
            int posZ_;                      // idoLink_.pos_.vz
            short dirIdx_;
            int inFlag_;
            int outFlag_;
            int shipEncount_;
            int encount_;
        };
        struct IdoLinkData {
            int posX_;                      // idoLink_.pos_.vx
            int posY_;                      // idoLink_.pos_.vy
            IdoLink link_;
        };
        union IdoLinkUnion {
#ifdef DQIV_NATIVE
            IdoLinkUnion() : pos_() {}
#endif
            dss::Fix32Vector3 pos_;
            IdoLinkData data_;
        } idoLink_;                         // 0x268

        int playerLockCount_;
        int lastFldSurface_;
        char lastRanaStageName_[10];
        int menuTalk_;
        int menuSearch_;
        int encountMapUid_;
        int ropeLink_;
        int load_;
        int chapterLoad_;
        profile::SAVETYPE loadType_;
        int returnBookFlag_;
        int profileBank_;
        short lastParty_[9];
        StageStatus();
        ~StageStatus();
        void initialize();
        void setup(char *mapname);
        void setupField();
        void cleanup();
        void setTimeZoneEnable(int flag);
        int isTimeZoneEnable();
        void setTimeZone(TIME_ZONE time);
        TIME_ZONE getTimeZone();
        void setWorldTime(int wt);
        int getWorldTime();
        void setMapIcon(int flag);
        bool isMapIcon();
        void setShopIcon(int flag);
        bool isShopIcon();
        void setCameraIcon(int flag);
        bool isCameraIcon();
        void setRula(int flag);
        bool isRula();
        void setRulaDisable(int flag);
        bool isRulaDisable();
        void setRiremito(int flag);
        bool isRiremito();
        void setRiremitoDisable(int flag);
        bool isRiremitoDisable();
        void setLanaruta(int flag);
        bool isLanaruta();
        void setLanarutaDisable(int flag);
        bool isLanarutaDisable();
        static void setToramana(int flag);
        static int isToramana();
        void setBashaEnter(int flag);
        bool isBashaEnter();
        void setBashaEnable(bool flag);
        bool isBashaEnable();
        void setEncount(int flag);
        bool isEncount();
        void setEventEncount(bool flag);
        void setAbortSave(int flag);
        bool isAbortSave();
        int isAbortSaveTown();
        int isAbortSaveDungeon();
        void setBtlMapName(const char* name);
        void setBtlMapNameOnField(LandType land);
        char* getTraderMapName();
        char* getInnKeeperMapName();
        char* getBtlMapName();
        void setEvBtlMapName(char* name);
        char* getEvBtlMapName();
        void updateBattleMap();
        void setMapName(const char* name);
        char* getMapName();
        void setChurchMapName(const char*name);
        char* getChurchMapName();
        void pushCameraAngle(dss::Vector3short pos);
        dss::Vector3short * popCameraAngle();
        void setMapVeil(int flag);
        void setMapVeil(int x, int y, int flag);
        int isMapVeil(int x, int y);
        void setSymbolFlag(int index);
        int getSymbolFlag(int index);
        void initSymbolFlag();
        void setFurnFlag(int index);
        bool getFurnFlag(int index);
        void removeFurnFlag(int index);
        void initFurnFlag();
        void setDoorOpenFlag(int index);
        void removeDoorOpenFlag(int index);
        bool getDoorOpenFlag(int index);
        void initDoorOpenFlag();
        void setFurnBreakFlag(int index);
        int getFurnBreakFlag(int index);
        void initFurnBreakFlag();
        void playerClearFlag();
        void playerFlagClear();
        void setRuraFlag(int rura);
        void setRuraTownID(int id);
        int getRuraTownID();
        void setRanaMap(int id);
        void setRanaMapName(const char* name);
        void collectMapFlag(profile::SAVETYPE savetype, profile::PROFILE_PARTY* pparty);
        void deliverMapFlag(profile::SAVETYPE savetype, profile::PROFILE_PARTY *pparty);
        void setFallFlag(int flag);
        int getFallFlag();
        int restartChurch();
    };

}
extern status::StageStatus g_Stage; //data_020d08e0

struct FieldPlayerManager;


