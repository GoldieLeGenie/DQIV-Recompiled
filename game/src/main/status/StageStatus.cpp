#include "main/status/StageStatus.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/cmn/ExtraMapLink.hpp"
#include "main/status/ActionExec.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/cmn/CommonRuraData.hpp"
#include "main/status/ExcelParam.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "main/cmn/ImmigrantTown.hpp"

extern char map_caf1[8];
extern char map_btl_pl_d[12];
extern char map_empty[4];
extern char map_casino[8];
extern char map_field[8];
extern char map_m[4];
extern char map_h[4];
extern char map_c[4];
extern char map_f[4];
extern char map_darkfield[12];
extern char map_d[4];
extern char map_t[4];
extern char map_s[4];
extern char map_e[4];
extern char map_btl_pl_dk[12];
extern char map_btl_se_dk[12];
extern char map_btl_sd_dk[12];
extern char map_btl_mt_dk[12];
extern char map_btl_po_dk[12];
extern char map_btl_wd_dk[12];
extern char map_btl_se_d[12];
extern char map_btl_sd_d[12];
extern char map_btl_mt_d[12];
extern char map_btl_po_d[12];
extern char map_btl_wd_d[12];
extern char map_btldougu_d[12];
extern char map_btlyado_d[12];



int status::StageStatus::toramana_;
status::StageStatus g_Stage; //data_020d08e0



THUMB status::StageStatus::StageStatus()
{
    this->flag_.flag_ = 0;
    for (int i = 0; i < 16; i++)
    {
        this->mapVeil_[i].flag_ |= 0xFFFF;
    }
    this->loadType_ = profile::SAVETYPE_INVALID;
}

THUMB status::StageStatus::~StageStatus()
{
  return;
}


THUMB void status::StageStatus::initialize()
{
    this->flag_.flag_ = 0;

    dss::strcpy_s(this->mapName_, 0x20, map_caf1);

    this->map_ = 0;

    dss::strcpy_s(this->btlMapName_, 0x20, map_btl_pl_d);

    this->btlMap_ = 0;

    dss::strcpy_s(this->evBtlMapName_, 0x20, map_btl_pl_d);

    this->evBtlMap_ = 0;

    int x; /* uninitialized */
    this->ruraEnable_.flag_ = x;

    this->maxKekai = 4;
    this->ruraFlag_ = 0;
    this->ropeLink_ = 0;
    this->worldTime_ = 0;
    this->chapterLoad_ = 0;

    this->shipPosition_.vx = 2536.0f;
    this->shipPosition_.vy = 560.0f;
    this->shipPosition_.vz = 0L;
    this->balloonPosition_.vx = 2608.0f;
    this->balloonPosition_.vy = 768.0f;
    this->balloonPosition_.vz = 0L;

    this->idoLink_.data_.link_.inFlag_ = 0;
    this->idoLink_.data_.link_.outFlag_  = 0;
    StageStatus::IdoLink* link = &g_Stage.idoLink_.data_.link_;
    link->shipEncount_ = 0;
    link->encount_ = 0;
    this->playerLockCount_ = 0;

    setTimeZone(TIME_ZONE_DAYTIME);
    initSymbolFlag();
    initFurnFlag();
    initDoorOpenFlag();
    setMapVeil(1);

    this->ruraEnable_.flag_ = 0;

    cmn::CommonRuraData::getSingleton()->initialize();

    dss::strcpy_s(this->lastRanaStageName_, 0xA, map_empty);

    this->lastFldSurface_ = -1;
    this->encountMapUid_ = 0;

    for (int i = 0; i < 9; i++)
    {
        this->lastParty_[i] = 0;
    }
}


THUMB void status::StageStatus::setup(char *mapname)
{
    this->flag_.flag_ = 0;
    setMapName(mapname);
    param::FloorParam *floorParam = status::excelParam.floorParam_;
    int index = param::FloorParam::getFloorIndex(floorParam, mapname);
    param::FloorParam *entry = &floorParam[index];

    if (g_Global.isAreaChange() == 1 &&
        dss::strcmp(g_Global.getMapName(), map_casino) != 0 &&
        dss::strcmp(g_Global.getPrevMapName(), map_casino) != 0)
    {
        UnkImmigrantTown::getSingleton()->resetPositions();
    }

    g_Stage.setTimeZoneEnable((entry->byte_1 & 0x1E) << 0x17 >> 0x18);
    g_Stage.setMapIcon((char)(entry->byte_2 & 0xF));
    g_Stage.setShopIcon((entry->byte_2 & 0xF0) << 0x14 >> 0x18);
    g_Stage.setCameraIcon((char)(entry->byte_3 & 0xF));

    g_Stage.setRula(((entry->byte_3 & 0xF0) << 0x14 >> 0x18) == 1);
    g_Stage.setRulaDisable(((entry->byte_3 & 0xF0) << 0x14 >> 0x18) == 2);

    g_Stage.setRiremito(((char)(entry->byte_4 & 0xF)) == 1);
    g_Stage.setRiremitoDisable(((char)(entry->byte_4 & 0xF)) == 2);
    g_Stage.setLanaruta((entry->byte_4 & 0xF0) << 0x14 >> 0x18);
    g_Stage.setAbortSave((char)(entry->byte_1 & 1));

    char byte25lo = (char)(entry->byte_5 & 0xF);
    if (byte25lo != 0)
    {
        g_Stage.setBashaEnable(true);
        g_Stage.setBashaEnter(true);
    }
    else
    {
        g_Stage.setBashaEnable(false);
        g_Stage.setBashaEnter(false);
    }

    int byte25hi = (entry->byte_5 & 0xF0) << 0x14 >> 0x18;
    if (byte25hi != 0)
    {
        g_Stage.setBashaEnable(false);
    }
    else
    {
        g_Stage.setBashaEnable(true);
    }

    char byte26lo = (char)(entry->byte_6 & 0xF);
    if (byte26lo != 0)
    {
        g_Stage.setBashaEnable(true);
        g_Stage.setBashaEnter(false);
    }

    g_Stage.setEncount(((entry->byte_6 & 0xF0) << 0x14 >> 0x18) == 1);

    bool eventEncount = false;
    int val = (entry->byte_6 & 0xF0) >> 4;
    if ((char)val == 4)
        eventEncount = true;
    if ((char)val == 5)
        eventEncount = true;
    if ((char)val == 7)
        eventEncount = true;
    if ((char)val == 0xA)
        eventEncount = true;
    if ((char)val == 0xD)
        eventEncount = true;
    if ((char)val == 0xE)
        eventEncount = true;
    g_Stage.setEventEncount(eventEncount);

    setBtlMapName(entry->battlefloor);
    setEvBtlMapName(entry->eventfloor);
}


THUMB void status::StageStatus::setupField()
{
    this->flag_.flag_ = 0;

    if (g_Global.getFieldType() != 2)
    {
        g_Stage.setTimeZoneEnable(1);
    }
    else
    {
        g_Stage.setTimeZoneEnable(0);
    }

    if (g_Global.getFieldType() == 0)
    {
        g_Stage.setMapIcon(1);
    }
    else
    {
        g_Stage.setMapIcon(0);
    }

    g_Stage.setShopIcon(0);
    g_Stage.setCameraIcon(0);
    g_Stage.setRula(true);
    g_Stage.setRiremito(false);

    if (g_Global.getFieldType() != 2)
    {
        g_Stage.setLanaruta(1);
    }
    else
    {
        g_Stage.setLanaruta(0);
    }

    g_Stage.setBashaEnter(true);
    g_Stage.setBashaEnable(true);
    g_Stage.setEncount(true);
}

THUMB void status::StageStatus::cleanup()
{
    return;
}

THUMB void status::StageStatus::setTimeZoneEnable(int flag)
{
    if (flag)
    {
        this->flag_.flag_ = this->flag_.flag_ | 1;
        return;
    }
    else
    {
        this->flag_.flag_ = this->flag_.flag_ & 0xfffffffe;
        return;
    }
    return;
}

THUMB int status::StageStatus::isTimeZoneEnable()
{
    return (this->flag_.flag_ & 1) != 0;
}

THUMB void status::StageStatus::setTimeZone(TIME_ZONE time)
{
    this->timeZone_ = time;
    switch (time)
    {
    case TIME_ZONE_MORNING:
        this->worldTime_ = WORLD_TIME_MORNING;
        return;
    case TIME_ZONE_DAYTIME:
        this->worldTime_ = WORLD_TIME_DAYTIME;
        return;
    case TIME_ZONE_EVENING:
        this->worldTime_ = WORLD_TIME_EVENING;
        return;
    case TIME_ZONE_NIGHT:
        this->worldTime_ = WORLD_TIME_NIGHT;
    }
    return;
}

THUMB TIME_ZONE status::StageStatus::getTimeZone()
{
    return this->timeZone_;
}

THUMB void status::StageStatus::setWorldTime(int wt)
{
    this->worldTime_ = wt;
};

THUMB int status::StageStatus::getWorldTime()
{
    return this->worldTime_;
}

THUMB void status::StageStatus::setMapIcon(int flag)
{
    if (flag)
    {
        this->flag_.flag_ = this->flag_.flag_ | 2;
        return;
    }
    else
    {
        this->flag_.flag_ = this->flag_.flag_ & 0xfffffffd;
        return;
    }
    return;
}

THUMB bool status::StageStatus::isMapIcon()
{
    if (this->flag_.flag_ & 2)
    {
        return 1;
    }
    return 0;
}

THUMB void status::StageStatus::setShopIcon(int flag)
{
    if (flag)
    {
        this->flag_.flag_ = this->flag_.flag_ | 4;
        return;
    }
    else
    {
        this->flag_.flag_ = this->flag_.flag_ & 0xfffffffb;
        return;
    }
    return;
}

THUMB bool status::StageStatus::isShopIcon()
{
    if (this->flag_.flag_ & 4)
    {
        return 1;
    }
    return 0;
}

THUMB void status::StageStatus::setCameraIcon(int flag)
{
    if (flag)
    {
        this->flag_.flag_ = this->flag_.flag_ | 8;
        return;
    }
    else
    {
        this->flag_.flag_ = this->flag_.flag_ & 0xfffffff7;
        return;
    }
    return;
}

THUMB bool status::StageStatus::isCameraIcon()
{
    if (this->flag_.flag_ & 8)
    {
        return 1;
    }
    return 0;
}

THUMB void status::StageStatus::setRula(int flag)
{
    if (flag)
    {
        this->flag_.flag_ = this->flag_.flag_ | 0x10;
        return;
    }
    else
    {
        this->flag_.flag_ = this->flag_.flag_ & 0xffffffef;
        return;
    }
    return;
}

THUMB bool status::StageStatus::isRula()
{
    if (this->flag_.flag_ & 0x10)
    {
        return 1;
    }
    return 0;
}

THUMB void status::StageStatus::setRulaDisable(int flag)
{
    if (flag)
    {
        this->flag_.flag_ = this->flag_.flag_ | 0x800;
        return;
    }
    else
    {
        this->flag_.flag_ = this->flag_.flag_ & 0xfffff7ff;
        return;
    }
    return;
}

THUMB bool status::StageStatus::isRulaDisable()
{
    if (this->flag_.flag_ & 0x800)
    {
        return 1;
    }
    return 0;
}

THUMB void status::StageStatus::setRiremito(int flag)
{
    if (flag)
    {
        this->flag_.flag_ = this->flag_.flag_ | 0x20;
        return;
    }
    else
    {
        this->flag_.flag_ = this->flag_.flag_ & 0xffffffdf;
        return;
    }
    return;
}

THUMB bool status::StageStatus::isRiremito()
{
    if (this->flag_.flag_ & 32)
    {
        return 1;
    }
    return 0;
}

THUMB void status::StageStatus::setRiremitoDisable(int flag)
{
    if (flag)
    {
        this->flag_.flag_ = this->flag_.flag_ | 0x1000;
        return;
    }
    else
    {
        this->flag_.flag_ = this->flag_.flag_ & 0xffffefff;
        return;
    }
    return;
}

THUMB bool status::StageStatus::isRiremitoDisable()
{
    if (this->flag_.flag_ & 0x1000)
    {
        return 1;
    }
    return 0;
}

THUMB void status::StageStatus::setLanaruta(int flag)
{
    if (flag)
    {
        this->flag_.flag_ = this->flag_.flag_ | 0x40;
        return;
    }
    else
    {
        this->flag_.flag_ = this->flag_.flag_ & 0xffffffbf;
        return;
    }
    return;
}

THUMB bool status::StageStatus::isLanaruta()
{
    if (this->flag_.flag_ & 0x40)
    {
        return 1;
    }
    return 0;
}

THUMB void status::StageStatus::setLanarutaDisable(int flag)
{
    if (flag)
    {
        this->flag_.flag_ = this->flag_.flag_ | 0x40000;
        return;
    }
    else
    {
        this->flag_.flag_ = this->flag_.flag_ & 0xfffbffff;
        return;
    }
    return;
}

THUMB bool status::StageStatus::isLanarutaDisable()
{
    if (this->flag_.flag_ & 0x40000)
    {
        return 1;
    }
    return 0;
}

THUMB void status::StageStatus::setToramana(int flag)
{
    toramana_ = flag;
    return;
}

THUMB int status::StageStatus::isToramana()
{
    return toramana_;
}

THUMB void status::StageStatus::setBashaEnter(int flag)
{
    if (flag)
    {
        this->flag_.flag_ = this->flag_.flag_ | 0x80;
        return;
    }
    else
    {
        this->flag_.flag_ = this->flag_.flag_ & 0xffffff7f;
        return;
    }
    return;
}

THUMB bool status::StageStatus::isBashaEnter()
{
    if (this->flag_.flag_ & 0x80)
    {
        return 1;
    }
    return 0;
}

THUMB void status::StageStatus::setBashaEnable(bool flag)
{
    if (flag)
    {
        this->flag_.flag_ = this->flag_.flag_ | 0x100;
        return;
    }
    else
    {
        this->flag_.flag_ = this->flag_.flag_ & 0xFFFFFEFF;
        return;
    }
    return;
}

THUMB bool status::StageStatus::isBashaEnable()
{
    if (this->flag_.flag_ & 256)
    {
        return 1;
    }
    return 0;
}

THUMB void status::StageStatus::setEncount(int flag)
{
    if (flag)
    {
        this->flag_.flag_ = this->flag_.flag_ | 0x200;
        return;
    }
    else
    {
        this->flag_.flag_ = this->flag_.flag_ & 0xfffffdff;
        return;
    }
    return;
}

THUMB bool status::StageStatus::isEncount()
{
    if (this->flag_.flag_ & 0x200)
    {
        return 1;
    }
    return 0;
}

THUMB void status::StageStatus::setEventEncount(bool flag)
{
    if (flag)
    {
        this->flag_.flag_ = this->flag_.flag_ | 0x400;
        return;
    }
    else
    {
        this->flag_.flag_ = this->flag_.flag_ & 0xfffffbff;
        return;
    }
    return;
}

THUMB void status::StageStatus::setAbortSave(int flag)
{
    if (flag)
    {
        this->flag_.flag_ = this->flag_.flag_ | 0x20000;
        return;
    }
    else
    {
        this->flag_.flag_ = this->flag_.flag_ & 0xfffdffff;
        return;
    }
    return;
}

THUMB bool status::StageStatus::isAbortSave()
{
    if (0x20000 & this->flag_.flag_)
    {
        return 1;
    }
    return 0;
}

THUMB int status::StageStatus::isAbortSaveTown()
{
    char local[2];

    if (data_0210bb94.unkfunc_02058114(12) == 1)
    {
        if (TownPlayerManager::getSingleton()->isSaveAndBattleOK() == 0)
        {
            return 0;
        }
    }
#ifdef DQIV_NATIVE
    // The PC version saves anywhere (dungeons too), except in the middle of
    // an action (rope, fall, stairs).
    return 1;
#endif

    char *const p = local;
    p[0] = '\0';
    p[1] = '\0';

    char *name = getMapName();
    local[0] = name[0];

    if (dss::strcmp(getMapName(), map_field) == 0)
    {
        return 1;
    }

    if (dss::strcmp(local, map_m) == 0 ||
        dss::strcmp(local, map_h) == 0 ||
        dss::strcmp(local, map_c) == 0 ||
        dss::strcmp(local, map_f) == 0)
    {

        if (this->isAbortSave() == 0)
        {
            return 0;
        }
    }

    return 1;
}

THUMB int status::StageStatus::isAbortSaveDungeon()
{
    char local[2];

    if (data_0210bb94.unkfunc_02058114(12) == 1)
    {
        if (TownPlayerManager::getSingleton()->isSaveAndBattleOK() == 0)
        {
            return 0;
        }
    }
#ifdef DQIV_NATIVE
    // The PC version saves anywhere (dungeons too), except in the middle of
    // an action (rope, fall, stairs).
    return 1;
#endif

    char *const p = local;
    p[0] = '\0';
    p[1] = '\0';

    char *name = getMapName();
    local[0] = name[0];

    if (dss::strcmp(this->getMapName(), map_darkfield) == 0)
    {
        return 1;
    }

    if (dss::strcmp(local, map_d) == 0 ||
        dss::strcmp(local, map_t) == 0 ||
        dss::strcmp(local, map_s) == 0 ||
        dss::strcmp(local, map_e) == 0)
    {

        if (this->isAbortSave() == 0)
        {
            return 0;
        }
    }

    return 1;
}

THUMB void status::StageStatus::setBtlMapName(const char*name)
{
    dss::strcpy_s(this->btlMapName_, 0x20, name);
    this->btlMap_ = this->btlMapName_;
    return;
}

THUMB void status::StageStatus::setBtlMapNameOnField(LandType land)
{
    if (g_Global.getFieldType() == 2)
    {
        switch (land)
        {
        case Floor:
            dss::strcpy_s(this->btlMapName_, 0x20, map_btl_pl_dk);
            break;
        case Sea:
            dss::strcpy_s(this->btlMapName_, 0x20, map_btl_se_dk);
            break;
        case Desert:
            dss::strcpy_s(this->btlMapName_, 0x20, map_btl_sd_dk);
            break;
        case Mountain:
            dss::strcpy_s(this->btlMapName_, 0x20, map_btl_mt_dk);
            break;
        case Pond:
            dss::strcpy_s(this->btlMapName_, 0x20, map_btl_po_dk);
            break;
        case Bush:
        case Field:
            dss::strcpy_s(this->btlMapName_, 0x20, map_btl_pl_dk);
            break;
        case Forest:
            dss::strcpy_s(this->btlMapName_, 0x20, map_btl_wd_dk);
            break;
        }
    }
    else
    {
        switch (land)
        {
        case Floor:
            dss::strcpy_s(this->btlMapName_, 0x20, map_btl_pl_d);
            break;
        case Sea:
            dss::strcpy_s(this->btlMapName_, 0x20, map_btl_se_d);
            break;
        case Desert:
            dss::strcpy_s(this->btlMapName_, 0x20, map_btl_sd_d);
            break;
        case Mountain:
            dss::strcpy_s(this->btlMapName_, 0x20, map_btl_mt_d);
            break;
        case Pond:
            dss::strcpy_s(this->btlMapName_, 0x20, map_btl_po_d);
            break;
        case Bush:
        case Field:
            dss::strcpy_s(this->btlMapName_, 0x20, map_btl_pl_d);
            break;
        case Forest:
            dss::strcpy_s(this->btlMapName_, 0x20, map_btl_wd_d);
            break;
        }
    }
    this->btlMap_ = this->btlMapName_;
}

THUMB char *status::StageStatus::getTraderMapName()
{
    TIME_ZONE timezone;
    dss::strcpy_s(this->btlMapName_, 0x20, map_btldougu_d);
    this->btlMap_ = this->btlMapName_;
    if (isTimeZoneEnable() != 0)
    {
        timezone = getTimeZone();
        switch (timezone)
        {
        case 0:
        case 2:
            this->btlMapName_[9] = 'd'; // day
            break;
        case 1:
        case 4:
            this->btlMapName_[9] = 'n'; // night
            break;
        case 3:
            this->btlMapName_[9] = 'e'; // evening
        }
        this->btlMapName_[10] = '\0';
    }
    return this->btlMap_;
}

THUMB char *status::StageStatus::getInnKeeperMapName()
{
    TIME_ZONE timezone;

    dss::strcpy_s(this->btlMapName_, 0x20, map_btlyado_d);
    this->btlMap_ = this->btlMapName_;
    if (isTimeZoneEnable() != 0)
    {
        timezone = getTimeZone();
        switch (timezone)
        {
        case 0:
        case 2:
            this->btlMapName_[8] = 'd'; // day
            break;
        case 1:
        case 4:
            this->btlMapName_[8] = 'n'; // night
            break;
        case 3:
            this->btlMapName_[8] = 'e'; // evening
        }
        this->btlMapName_[9] = 0;
    }
    return this->btlMap_;
}

THUMB char *status::StageStatus::getBtlMapName()
{
    TIME_ZONE timeZone;
    if (isTimeZoneEnable() != 0)
    {
        this->btlMapName_[6] = '_';
        timeZone = getTimeZone();
        switch (timeZone)
        {
        case 0:
        case 2:
            this->btlMapName_[7] = 'd';
            break;
        case 1:
        case 4:
            this->btlMapName_[7] = 'n';
            break;
        case 3:
            this->btlMapName_[7] = 'e';
            break;
        }
        this->btlMapName_[8] = 0;
    }
    return this->btlMap_;
}

THUMB void status::StageStatus::setEvBtlMapName(char *name)
{
    dss::strcpy_s(this->evBtlMapName_, 0x20, name);
    this->evBtlMap_ = this->evBtlMapName_;
}

THUMB char *status::StageStatus::getEvBtlMapName()
{
    TIME_ZONE timeZone;
    if (isTimeZoneEnable() != 0)
    {
        this->evBtlMap_[6] = '_';
        timeZone = getTimeZone();
        switch (timeZone)
        {
        case 0:
        case 2:
            this->evBtlMap_[7] = 'd'; // DAY
            break;
        case 1:
        case 4:
            this->evBtlMap_[7] = 'n'; // NIGHT
            break;
        case 3:
            this->evBtlMap_[7] = 'e'; // EVENING
            break;
        }
    }
    return this->evBtlMap_;
}

THUMB void status::StageStatus::updateBattleMap()
{
    char *EvBtlMapName;

    EvBtlMapName = getEvBtlMapName();
    this->btlMap_ = EvBtlMapName;
    return;
}

THUMB void status::StageStatus::setMapName(const char*name)
{
    dss::strcpy_s(this->mapName_, 0x20, name);
    this->map_ = this->mapName_;
    return;
}

THUMB char *status::StageStatus::getMapName()
{
    return this->map_;
}

THUMB void status::StageStatus::setChurchMapName(const char*name)

{
    dss::strcpy_s(this->churchMapName_, 0x20, name);
    this->churchMap_ = this->churchMapName_;
    return;
}

THUMB char *status::StageStatus::getChurchMapName()
{
    return this->churchMap_;
}

THUMB void status::StageStatus::pushCameraAngle(dss::Vector3short pos)
{
    this->backupCameraAngle_.vx = pos.vx;
    this->backupCameraAngle_.vy = pos.vy;
    this->backupCameraAngle_.vz = pos.vz;
    return;
}

THUMB dss::Vector3short *status::StageStatus::popCameraAngle()
{
    return &this->backupCameraAngle_;
}

THUMB void status::StageStatus::setMapVeil(int flag)
{
    if (flag != 0)
    {
        for (int i = 0; i < 16; i++)
        {
            this->mapVeil_[i].flag_ = this->mapVeil_[i].flag_ | 0xFFFF;
        }
    }
    else
    {
        for (int i = 0; i < 16; i++)
        {
            this->mapVeil_[i].flag_ = 0;
        }
    }
    return;
}

THUMB void status::StageStatus::setMapVeil(int x, int y, int flag)
{
    int xBit = (x << 4) / 256;
    int yIndex = (y << 4) / 256;

    this->mapVeil_[yIndex].flag_ &= ~(1 << xBit);
    return;
}

THUMB int status::StageStatus::isMapVeil(int x, int y)
{
    unsigned short val = this->mapVeil_[y].flag_;
    int mask = 1 << x;
    return (val & mask) != 0;
}

THUMB void status::StageStatus::setSymbolFlag(int index)
{
    this->symbolFlag_[index / 8].flag_ |= (1 << (index % 8));
    return;
}

THUMB int status::StageStatus::getSymbolFlag(int index)
{
    unsigned char val = this->symbolFlag_[index / 8].flag_;
    return (val & (1 << (index % 8))) != 0;
}

THUMB void status::StageStatus::initSymbolFlag()
{
    for (int i = 0; i < 16; i++)
    {
        this->symbolFlag_[i].flag_ = 0;
    }
    return;
}

THUMB void status::StageStatus::setFurnFlag(int index)
{
    this->furnFlag_[index / 8].flag_ |= (1 << (index % 8));
    return;
}

THUMB bool status::StageStatus::getFurnFlag(int index)
{
    int byteIndex = index / 8;
    unsigned char flag = this->furnFlag_[byteIndex].flag_;
    int bitPos = index % 8;
    return (flag & (1 << bitPos)) != 0;
}

THUMB void status::StageStatus::removeFurnFlag(int index)
{
    this->furnFlag_[index / 8].flag_ &= ~(1 << (index % 8));
    return;
}

THUMB void status::StageStatus::initFurnFlag()
{
    for (int i = 0; i < 256; i++)
    {
        this->furnFlag_[i].flag_ = 0;
    }
    return;
}

THUMB void status::StageStatus::setDoorOpenFlag(int index)
{
    this->openFlag_[index / 8].flag_ |= (1 << (index % 8));
    return;
}

THUMB void status::StageStatus::removeDoorOpenFlag(int index)
{
    this->openFlag_[index / 8].flag_ &= ~(1 << (index % 8));
}

THUMB bool status::StageStatus::getDoorOpenFlag(int index)
{
    unsigned char val = this->openFlag_[index / 8].flag_;
    return (val & (1 << (index % 8))) != 0;
}

THUMB void status::StageStatus::initDoorOpenFlag()
{
    for (int i = 0; i < 16; i++)
    {
        this->openFlag_[i].flag_ = 0;
    }
    return;
}

THUMB void status::StageStatus::setFurnBreakFlag(int index)
{
    this->breakFlag_[index / 8].flag_ |= (1 << (index % 8));
    return;
}

THUMB int status::StageStatus::getFurnBreakFlag(int index)
{
    unsigned char val = this->breakFlag_[index / 8].flag_;
    return (val & (1 << (index % 8))) != 0;
}

THUMB void status::StageStatus::initFurnBreakFlag()
{
    for (int i = 0; i < 8; i++)
    {
        this->breakFlag_[i].flag_ = 0;
    }
    return;
}

THUMB void status::StageStatus::playerClearFlag()
{
    playerFlagClear();
}

THUMB void status::StageStatus::playerFlagClear()
{
    this->idoLink_.data_.link_.shipEncount_ = 0;
    this->idoLink_.data_.link_.outFlag_ = 0;
    this->idoLink_.data_.link_.inFlag_ = 0;
    this->idoLink_.data_.link_.encount_ = 0;
    this->playerLockCount_ = 0;

    initDoorOpenFlag();
    g_cmnPartyInfo.rideOnType_ = cmn::RIDE_ON_NONE;
    return;
}

THUMB void status::StageStatus::setRuraFlag(int rura)
{
    this->ruraFlag_ = rura;
    if (rura == 1)
    {
        if (data_0210bb94.unkfunc_02058114(0xE) != 0)
        {
            FieldPlayerManager::getSingleton();
            cmn::PlayerManager::setLock(1);
            return;
        }
        TownPlayerManager::getSingleton()->setLock(1);
        TownSystem::getSingleton()->scriptLock_ = 1;
    }
    return;
}

THUMB void status::StageStatus::setRuraTownID(int id)
{
    this->ruraSymbol_ = id;
    return;
}

THUMB int status::StageStatus::getRuraTownID()
{
    return this->ruraSymbol_;
}

THUMB void status::StageStatus::setRanaMap(int id)
{
    char *lastRanaStageName;
    char *MapName;

    lastRanaStageName = this->lastRanaStageName_;
    MapName = g_Global.getMapName();

    dss::strcpy_s(this->lastRanaStageName_, 10, MapName);
    this->lastFldSurface_ = id;
    return;
}

THUMB void status::StageStatus::setRanaMapName(const char*name)
{
    dss::strcpy_s(this->lastRanaStageName_, 10, name);
    return;
}

THUMB void status::StageStatus::collectMapFlag(profile::SAVETYPE savetype, profile::PROFILE_PARTY *pparty)
{
    int i;

    for (i = 0; i < 16; i++)
    {
        pparty->MAPVEIL[i] = this->mapVeil_[i].flag_;
    }

    for (i = 0; i < 128; i++)
    {
        pparty->FURNFLAG[i] = this->furnFlag_[i].flag_;
    }

    for (i = 0; i < 16; i++)
    {
        pparty->NEW_SYMBOLFLAG[i] = this->symbolFlag_[i].flag_;
    }

    for (i = 0; i < 8; i++)
    {
        if (savetype == profile::SAVETYPE_RESUME)
        {
            pparty->BREAKFLAG[i] = this->breakFlag_[i].flag_;
        }
        else
        {
            pparty->BREAKFLAG[i] = 0;
        }
    }
    return;
}

THUMB void status::StageStatus::deliverMapFlag(profile::SAVETYPE savetype, profile::PROFILE_PARTY *pparty)
{
    int i;

    for (i = 0; i < 0x10; i++)
    {
        this->mapVeil_[i].flag_ = pparty->MAPVEIL[i];
    }

    for (i = 0; i < 0x80; i++)
    {
        this->furnFlag_[i].flag_ = pparty->FURNFLAG[i];
    }

    for (i = 0; i < 0x10; i++)
    {
        this->symbolFlag_[i].flag_ = pparty->NEW_SYMBOLFLAG[i];
    }

    for (i = 0; i < 8; i++)
    {
        if (savetype == 3)
        {
            this->breakFlag_[i].flag_ = pparty->BREAKFLAG[i];
        }
        else
        {
            this->breakFlag_[i].flag_ = 0;
        }
    }

    cmn::CommonRuraData::getSingleton()->setRuraFlagAll();
}

THUMB void status::StageStatus::setFallFlag(int flag)
{
    this->fallFlag_ = flag;
    return;
}

THUMB int status::StageStatus::getFallFlag()
{
    return this->fallFlag_;
}


THUMB int status::StageStatus::restartChurch()
{
    unsigned int found = 0xFFFFFFFF;
    dss::Fix32Vector3 pos;

    param::MapChurch* tbl = status::excelParam.mapChurch_;
    for (unsigned int i = 0; i < param::MapChurch::size_; i++) {
        if (this->churchMap_[0] == tbl[i].floor[0] && this->churchMap_[1] == tbl[i].floor[1]) {
            if (tbl[i].direction != 0xFFFF) {
                found = i;
            }
            break;
        }
    }
    if (found == 0xFFFFFFFF) {
        return 0;
    }
    pos.vx = tbl[found].playerX;
    pos.vy = tbl[found].playerY;
    pos.vz = tbl[found].playerZ;
    cmn::g_extraMapLink.setExtraLinkTown(this->churchMap_, pos, (short)tbl[found].direction);
    setTimeZone(TIME_ZONE_DAYTIME);
    return 1;
}

char map_caf1[8] = "caf1";  // 0x020bdb80
char map_btl_pl_d[12] = "btl_pl_d";  // 0x020bdb88
#pragma explicit_zero_data on
char map_empty[4] = "";  // 0x020bdb94
#pragma explicit_zero_data reset
char map_casino[8] = "casino";  // 0x020bdb98
char map_field[8] = "field";  // 0x020bdba0
char map_m[4] = "m";  // 0x020bdba8
char map_h[4] = "h";  // 0x020bdbac
char map_c[4] = "c";  // 0x020bdbb0
char map_f[4] = "f";  // 0x020bdbb4
char map_darkfield[12] = "darkfield";  // 0x020bdbb8
char map_d[4] = "d";  // 0x020bdbc4
char map_t[4] = "t";  // 0x020bdbc8
char map_s[4] = "s";  // 0x020bdbcc
char map_e[4] = "e";  // 0x020bdbd0
char map_btl_pl_dk[12] = "btl_pl_dk";  // 0x020bdbd4
char map_btl_se_dk[12] = "btl_se_dk";  // 0x020bdbe0
char map_btl_sd_dk[12] = "btl_sd_dk";  // 0x020bdbec
char map_btl_mt_dk[12] = "btl_mt_dk";  // 0x020bdbf8
char map_btl_po_dk[12] = "btl_po_dk";  // 0x020bdc04
char map_btl_wd_dk[12] = "btl_wd_dk";  // 0x020bdc10
char map_btl_se_d[12] = "btl_se_d";  // 0x020bdc1c
char map_btl_sd_d[12] = "btl_sd_d";  // 0x020bdc28
char map_btl_mt_d[12] = "btl_mt_d";  // 0x020bdc34
char map_btl_po_d[12] = "btl_po_d";  // 0x020bdc40
char map_btl_wd_d[12] = "btl_wd_d";  // 0x020bdc4c
char map_btldougu_d[12] = "btldougu_d";  // 0x020bdc58
char map_btlyado_d[12] = "btlyado_d";  // 0x020bdc64
