#pragma ipa file
#include "main/cmn/ExtraMapLink.hpp"
#include "main/dss/DssUtils.hpp"
#include "ov000/town/TownActionCalculate.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/global/Global.hpp"
#include "main/cmn/WorldLocation.hpp"
#include "main/cmn/PlayerManager.hpp"
#include "main/status/StageStatus.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/cmn/CommonRuraData.hpp"



char s_dpb8b[8] = "dpb8b";   // 0x020be960
char s_mpout2[8] = "mpout2";  // 0x020be958
char s_ev02[8] = "ev02";    // 0x020be950
char s_dkf1a[8] = "dkf1a";   // 0x020be948
char s_dpout[8] = "dpout";   // 0x020be940
char s_sqf4[8] = "sqf4";    // 0x020be938
char s_sif1[8] = "sif1";    // 0x020be930
char s_sjm2f1[8] = "sjm2f1";  // 0x020be928
char s_seout[8] = "seout";   // 0x020be920
char s_mrout[8] = "mrout";   // 0x020be918
char s_mcout2[8] = "mcout2";  // 0x020be910
char s_mcout1[8] = "mcout1";  // 0x020be908
char s_tcf5[8] = "tcf5";    // 0x020be900
char s_tcf4[8] = "tcf4";    // 0x020be8f8
char s_tcf3[8] = "tcf3";    // 0x020be8f0
char s_tcf2[8] = "tcf2";    // 0x020be8e8
char s_tbf3[8] = "tbf3";    // 0x020be8e0
char s_taf2a[8] = "taf2a";   // 0x020be8d8
char s_tatop[8] = "tatop";   // 0x020be8d0
char s_clf5b[8] = "clf5b";   // 0x020be8c8
char s_cif1[8] = "cif1";    // 0x020be8c0
char s_fk01[8] = "fk01";    // 0x020be8b8
char s_ckout[8] = "ckout";   // 0x020be8b0
char s_cccf1b[8] = "cccf1b";  // 0x020be8a8

cmn::ExtraMapLink cmn::g_extraMapLink;

ARM void cmn::ExtraMapLink::setup()
{
    dss::Fix32Vector3 offset;

    mapLinkDataCount_ = 0;
    rectLinkCount_ = 0;

    if (g_GlobalDQ4.unkfunc_02058114(12)) {
        switch (g_Global.getMapName()[0]) {
            case 'c':
                switch (g_Global.getMapName()[1]) {
                    case 'c':
                        setLinkData(0x706b, 0x5c, LINK_TOWN_TO_FIELD, s_cccf1b, NULL);
                        break;
                    case 'k':
                        setLinkData(0x706d, 0x5d, LINK_TOWN_TO_FIELD, s_ckout, NULL);
                        setLinkData(0x106d, 0, NOT_LINK_THIS_TOWN, s_ckout, NULL);
                        setLinkData(0x106c, 0, NOT_LINK_THIS_TOWN, s_ckout, NULL);
                        setLinkData(0x106b, 0, NOT_LINK_THIS_TOWN, s_ckout, NULL);
                        offset.set(0, 0, 0x1000);
                        setLinkData(0x106a, 0, LINK_TOWN_OFFSET, s_ckout, NULL, offset);
                        setLinkData(0x1193, 0x106a, LINK_TOWN_TO_TOWN_OFFSET, s_ckout, s_fk01, offset);
                        setLinkData(0x1195, 0x106a, LINK_TOWN_TO_TOWN_OFFSET, s_ckout, s_fk01, offset);
                        break;
                    case 'i':
                        setLinkData(0x707b, 0x79, LINK_TOWN_TO_FIELD, s_cif1, NULL);
                        setLinkData(0x707b, 0x1e, LINK_TOWN_TO_FIELD, s_cif1, NULL);
                        break;
                    case 'l':
                        offset.set(0, -0x14000, 0);
                        setLinkData(0x7001, 0x6f, LINK_TOWN_TO_FIELD, s_clf5b, NULL, offset);
                        break;
                }
                break;
            case 't':
                switch (g_Global.getMapName()[1]) {
                    case 'a':
                        setLinkData(0x7065, 0x5a, LINK_TOWN_TO_FIELD, s_tatop, NULL);
                        setLinkData(0x7067, 0x5a, LINK_TOWN_TO_FIELD, s_taf2a, NULL);
                        break;
                    case 'b':
                        setLinkData(0x7069, 0x5b, LINK_TOWN_TO_FIELD, s_tbf3, NULL);
                        break;
                    case 'c':
                        setLinkData(0x7071, 0x5f, LINK_TOWN_TO_FIELD, s_tcf2, NULL);
                        setLinkData(0x7073, 0x60, LINK_TOWN_TO_FIELD, s_tcf3, NULL);
                        setLinkData(0x7075, 0x61, LINK_TOWN_TO_FIELD, s_tcf4, NULL);
                        setLinkData(0x7077, 0x62, LINK_TOWN_TO_FIELD, s_tcf5, NULL);
                        break;
                }
                break;
            case 'f':
                setLinkData(0x706f, 0x5e, LINK_TOWN_TO_FIELD, s_fk01, NULL);
                setLinkData(0x1194, 0, NOT_LINK_THIS_TOWN, s_fk01, NULL);
                setLinkData(0x1192, 0, NOT_LINK_THIS_TOWN, s_fk01, NULL);
                break;
            case 'm':
                switch (g_Global.getMapName()[1]) {
                    case 'c':
                        offset.set(0, 0xc000, 0);
                        setLinkData(0x7000, 0x15, LINK_TOWN_TO_FIELD, s_mcout1, NULL, offset);
                        setLinkData(0x7001, 0x15, LINK_TOWN_TO_FIELD, s_mcout1, NULL, offset);
                        offset.set(-0xa000, -0x10000, 0);
                        setLinkData(0x7000, 0x64, LINK_TOWN_TO_FIELD, s_mcout2, NULL, offset);
                        setLinkData(0x7001, 0x64, LINK_TOWN_TO_FIELD, s_mcout2, NULL, offset);
                        break;
                    case 'r':
                        offset.set(0, 0x18000, 0);
                        setLinkData(0x7002, 0x56, LINK_TOWN_TO_FIELD, s_mrout, NULL, offset);
                        offset.set(0, -0x12000, 0);
                        setLinkData(0x7001, 0x56, LINK_TOWN_TO_FIELD, s_mrout, NULL, offset);
                        break;
                }
                break;
            case 's':
                switch (g_Global.getMapName()[1]) {
                    case 'e':
                        offset.set(0, -0x10000, 0);
                        setLinkData(0x7001, 0x81, LINK_TOWN_TO_FIELD, s_seout, NULL, offset);
                        setLinkData(0x7000, 0x81, LINK_TOWN_TO_FIELD, s_seout, NULL, offset);
                        offset.set(0, 0xc000, 0);
                        setLinkData(0x7002, 0x33, LINK_TOWN_TO_FIELD, s_seout, NULL, offset);
                        break;
                    case 'j':
                        setLinkData(0x10cb, 0x10ca, LINK_TOWN_TO_TOWN, s_sjm2f1, s_sif1);
                        break;
                    case 'q':
                        setLinkData(0x707d, 0x6d, LINK_TOWN_TO_FIELD, s_sqf4, NULL);
                        break;
                }
                break;
            case 'd':
                setLinkData(0x7079, 0x78, LINK_TOWN_TO_FIELD, s_dpout, NULL);
                offset.set(0, 0x19000, 0);
                setLinkData(0x7001, 0x1c, LINK_TOWN_TO_FIELD, s_dkf1a, NULL, offset);
                break;
            case 'e':
                if (g_Global.getMapName()[3] == '2') {
                    offset.set(0, 0x10000, 0);
                    setLinkData(0x7001, 0x1d, LINK_TOWN_TO_FIELD, s_ev02, NULL, offset);
                    setLinkData(0x7000, 0x1d, LINK_TOWN_TO_FIELD, s_ev02, NULL, offset);
                    offset.set(0, -0x4a000, 0);
                    setLinkData(0x7002, 0x1d, LINK_TOWN_TO_FIELD, s_ev02, NULL, offset);
                }
                break;
        }
    } else {
        setLinkData(0x15, 0x7001, LINK_FIELD_TO_TOWN, NULL, s_mcout1);
        setLinkData(0x6f, 0x7001, LINK_FIELD_TO_TOWN, NULL, s_clf5b);
        for (int i = 0x59; i <= 0x62; i++) {
            setLinkData(i, 0, NOT_LINK_THIS_TOWN, NULL, NULL);
        }
        setLinkData(0x79, 0, NOT_LINK_THIS_TOWN, NULL, NULL);
        setLinkData(0x6d, 0, NOT_LINK_THIS_TOWN, NULL, NULL);

        dss::Fix32Vector3 topLeft;
        dss::Fix32Vector3 bottomRight;
        dss::Fix32Vector3 pos;

        topLeft.vx.value = 0x85c000;
        topLeft.vy.value = 0x84c000;
        bottomRight.vx.value = 0x8d4000;
        bottomRight.vy.value = 0x8e4000;
        pos.vx.value = 0x4ba000;
        pos.vy.value = 0x5ec000;
        setFieldRectLinkToField(topLeft, bottomRight, RECT_FIELD_TO_GOT, pos);

        topLeft.vx.value = 0x8cf000;
        topLeft.vy.value = 0xc4c000;
        bottomRight.vx.value = 0x8f1000;
        bottomRight.vy.value = 0xc58000;
        offset.vx.value = 0;
        offset.vy.value = 0;
        offset.vz.value = 0x2000;
        setFieldRectLinkToTown(topLeft, bottomRight, 0x56, 0x7001, s_mrout, offset);

        topLeft.set(0x8cf000, 0xc59000, 0);
        bottomRight.set(0x8f1000, 0xc6e000, 0);
        offset.set(0, 0, -0x2000);
        setFieldRectLinkToTown(topLeft, bottomRight, 0x56, 0x7002, s_mrout, offset);

        topLeft.set(0xe60000, 0x1b7000, 0);
        bottomRight.set(0xe89000, 0x1bf000, 0);
        setFieldRectLinkToTown(topLeft, bottomRight, 0x1c, 0x7001, s_dkf1a, offset);
    }
}

ARM void cmn::ExtraMapLink::setExtraFieldPos(dss::Fix32Vector3& pos, short& idx)
{
    switch (extraLink_) {
        case LINK_TOWN_TO_FIELD:
            pos += offset_;
            idx = extraIdx_;
            break;
        case RECT_FIELD_TO_WORLD:
        case RECT_FIELD_TO_YAMI:
        case RECT_FIELD_TO_GOT:
        case LINK_FIELD_ABS_POS:
            pos = offset_;
            idx = extraIdx_;
            break;
    }
    extraLink_ = 0;
}

ARM int cmn::ExtraMapLink::checkFieldLink(int id)
{
    if (id == 0x24 || id == 0x83) {
        switch (WorldLocation::getCurrentTimeZone()) {
            case TIME_ZONE_DAYTIME:
            case TIME_ZONE_EVENING:
                return id == 0x24 ? 0 : 4;
            case TIME_ZONE_MORNING:
            case TIME_ZONE_NIGHT:
                if (id != 0x24) {
                    return 4;
                }
                StageLink::setTownExitIndex(0x7001);
                g_Global.startTown(s_mpout2);
                FieldPlayerManager::getSingleton();
                PlayerManager::setLock(1);
                g_Stage.symbolID_ = id;
                return 3;
        }
    }

    for (int i = 0; i < mapLinkDataCount_; i++) {
        if (mapLinkData_[i].nowId == id) {
            switch (mapLinkData_[i].type) {
                case NOT_LINK_THIS_TOWN:
                    return 4;
                case LINK_FIELD_TO_TOWN:
                    StageLink::setTownExitIndex(mapLinkData_[i].nextId);
                    g_Global.startTown(mapLinkData_[i].nextMapName);
                    FieldPlayerManager::getSingleton();
                    PlayerManager::setLock(1);
                    g_Stage.symbolID_ = id;
                    return 3;
            }
        }
    }
    return 0;
}

ARM bool cmn::ExtraMapLink::checkExtraTownPos(dss::Fix32Vector3& pos, short& idx)
{
    bool result = false;
    switch (extraLink_) {
        case LINK_EXTRA_TOWN:
            pos = extraPos_;
            idx = extraIdx_;
            result = true;
            break;
        case LINK_TOWN_OFFSET:
            pos += offset_;
            result = true;
            break;
    }
    extraLink_ = 0;
    return result;
}

ARM void cmn::ExtraMapLink::startExitLoop()
{
    if (extraLink_ != 0) {
        return;
    }

    int surfaceId = TownStageManager::getSingleton()->getHitSurfaceIdByType(1);
    int group = TownStageManager::getSingleton()->coll_.m_surfacePolyNo[1];
    if (surfaceId % 2 == 0) {
        surfaceId++;
    } else {
        surfaceId--;
    }
    int id = TownStageManager::getSingleton()->getOtherPolyNoBySurfaceId(surfaceId, group);

    dss::Fix32Vector3 pos;
    dss::Fix32Vector3 dir;
    dss::Fix32Vector3 up(0, 1, 0);
    TownStageManager::getSingleton()->stage_.collGetPolygonPos(id, &pos);
    TownStageManager::getSingleton()->getPolyDirection(dir, id);

    dss::Fix32 d = up * dir;
    dss::Fix32 scale;
    scale.value = 0x8f2;
    if (d.value == 0) {
        extraPos_ = pos + dir * scale;
        TownActionCalculate::getIdxByVec(extraIdx_, dir);
    } else {
        extraPos_ = pos;
        extraPos_.vy.value += 0x28;
        extraIdx_ = g_cmnPartyInfo.dirIdx_;
    }

    extraLink_ = LINK_EXTRA_TOWN;
    g_Global.startTown(g_Global.getMapName());
    PlayerManager::setLock(1);
}

ARM void cmn::ExtraMapLink::setExtraExitTown(const char* mapName, int id)
{
    StageLink::setTownExitIndex(id);
    g_Global.startTown((char*)mapName);
    extraLink_ = EXIT_TOWN;
    PlayerManager::setLock(1);
}

ARM void cmn::ExtraMapLink::setExtraLinkTown(const char* mapName, dss::Fix32Vector3& pos, short dir)
{
    extraLink_ = LINK_EXTRA_TOWN;
    g_Global.startTown((char*)mapName);
    extraPos_ = pos;
    extraIdx_ = dir;
    PlayerManager::setLock(1);
}

ARM void cmn::ExtraMapLink::setLinkData(int nowId, int nextId, LINK_TYPE type, const char* nowMapName, const char* nextMapName, dss::Fix32Vector3& offset)
{
    if (nowMapName != NULL && dss::strcmp(g_Global.getMapName(), nowMapName) != 0) {
        return;
    }
    for (int i = 0; i < mapLinkDataCount_; i++) {
        if (mapLinkData_[i].nowId == nowId) {
            setData(i, nowId, nextId, type, nowMapName, nextMapName, offset);
            return;
        }
    }
    setData(mapLinkDataCount_, nowId, nextId, type, nowMapName, nextMapName, offset);
    mapLinkDataCount_++;
}

ARM void cmn::ExtraMapLink::setLinkData(int nowId, int nextId, LINK_TYPE type, const char* nowMapName, const char* nextMapName)
{
    dss::Fix32Vector3 offset(0, 0, 0);

    if (nowMapName != NULL && dss::strcmp(nowMapName, g_Global.getMapName()) != 0) {
        return;
    }
    for (int i = 0; i < mapLinkDataCount_; i++) {
        if (mapLinkData_[i].nowId == nowId) {
            setData(i, nowId, nextId, type, nowMapName, nextMapName, offset);
            return;
        }
    }
    setData(mapLinkDataCount_, nowId, nextId, type, nowMapName, nextMapName, offset);
    mapLinkDataCount_++;
}

ARM int cmn::ExtraMapLink::checkTownMapLink(int id)
{
    switch (extraLink_) {
        case EXIT_TOWN:
            return 6;
        case LINK_EXTRA_TOWN:
            return 5;
    }

    for (int i = 0; i < mapLinkDataCount_; i++) {
        if (mapLinkData_[i].nowId == id && dss::strcmp(mapLinkData_[i].nowMapName, g_Global.getMapName()) == 0) {
            switch (mapLinkData_[i].type) {
                case LINK_TOWN_TO_TOWN:
                    StageLink::setTownExitIndex(mapLinkData_[i].nextId);
                    g_Global.startTown(mapLinkData_[i].nextMapName);
                    TownPlayerManager::getSingleton()->setLock(1);
                    return 1;
                case LINK_TOWN_TO_FIELD:
                    extraLink_ = LINK_TOWN_TO_FIELD;
                    offset_ = mapLinkData_[i].offsetData;
                    extraIdx_ = 4;
                    StageLink::setFieldSymbolIndex(mapLinkData_[i].nextId);
                    g_Global.nextFieldType_ = getFieldTypeBySurface(mapLinkData_[i].nextId);
                    g_Global.startField();
                    TownPlayerManager::getSingleton()->setLock(1);
                    g_Stage.idoLink_.data_.link_.inFlag_ = 0;
                    g_Stage.idoLink_.data_.link_.outFlag_ = 0;
                    return 2;
                case NOT_LINK_THIS_TOWN:
                    return 4;
                case LINK_TOWN_OFFSET:
                    offset_ = mapLinkData_[i].offsetData;
                    extraLink_ = LINK_TOWN_OFFSET;
                    return 0;
                case LINK_TOWN_TO_TOWN_OFFSET:
                    offset_ = mapLinkData_[i].offsetData;
                    extraLink_ = LINK_TOWN_OFFSET;
                    StageLink::setTownExitIndex(mapLinkData_[i].nextId);
                    g_Global.startTown(mapLinkData_[i].nextMapName);
                    TownPlayerManager::getSingleton()->setLock(1);
                    return 1;
                case LINK_DEFAULT:
                    return 0;
            }
        }
    }
    return 0;
}

ARM void cmn::ExtraMapLink::setExtraExitField(int id, dss::Fix32Vector3& pos)
{
    extraLink_ = LINK_TOWN_TO_FIELD;
    offset_ = pos;
    extraIdx_ = 4;
    StageLink::setFieldSymbolIndex(id);
    g_Global.nextFieldType_ = getFieldTypeBySymbol(id);
    g_Global.startField();
    PlayerManager::setLock(1);
    g_Stage.idoLink_.data_.link_.inFlag_ = 0;
    g_Stage.idoLink_.data_.link_.outFlag_ = 0;
    PlayerManager::setLock(1);
}

ARM void cmn::ExtraMapLink::setTownINN()
{
    dss::Fix32Vector3 pos = TownPlayerManager::getSingleton()->getPosition();
    short dir = TownPlayerManager::getSingleton()->getDirection();
    setExtraLinkTown(g_Global.getMapName(), pos, dir);
}

ARM void cmn::ExtraMapLink::setMonstarBookLink()
{
    g_cmnPartyInfo.prevLocation_ = 1;
    if (g_Global.prevPartTown_ != 0) {
        dss::Fix32Vector3 pos = TownPlayerManager::getSingleton()->getPosition();
        short dir = TownPlayerManager::getSingleton()->getDirection();
        setExtraLinkTown(g_Global.getPrevMapName(), pos, dir);
    } else {
        dss::Fix32Vector3 pos = g_cmnPartyInfo.position_;
        setExtraLinkFieldAbsPos(g_Global.getFieldType(), pos, 4);
    }
}

ARM int cmn::ExtraMapLink::getFieldTypeBySurface(int surfaceId)
{
    int id = surfaceId & 0xfff;
    int i;
    param::FieldSymbol* table = status::excelParam.fieldSymbol_;
    for (i = 0; i < 0x72; i++) {
        if (id == table[i].uid) {
            return table[i].world;
        }
    }
    return 0;
}

ARM void cmn::ExtraMapLink::eraseSurface(int surfaceId, LINK_TYPE type, const char* mapName)
{
    if (mapName == NULL) {
        return;
    }
    if (type == NOT_LINK_THIS_TOWN) {
        TownStageManager::getSingleton()->coll_.setEraseSurface(surfaceId, true);
    } else {
        TownStageManager::getSingleton()->coll_.setEraseSurface(surfaceId, false);
    }
}

ARM int cmn::ExtraMapLink::getFieldTypeBySymbol(int symbolId)
{
    int i;
    param::FieldSymbol* table = status::excelParam.fieldSymbol_;
    for (i = 0; i < 0x72; i++) {
        if (symbolId == table[i].uid) {
            return table[i].world;
        }
    }
    return 0;
}

ARM void cmn::ExtraMapLink::setFieldRectLinkToTown(dss::Fix32Vector3& topLeft, dss::Fix32Vector3& bottomRight, int nowId, int nextId, const char* mapName, dss::Fix32Vector3 offset)
{
    rectLinkData_[rectLinkCount_].type = RECT_FIELD_TO_TOWN;
    rectLinkData_[rectLinkCount_].pos[0] = topLeft;
    rectLinkData_[rectLinkCount_].pos[1] = bottomRight;
    rectLinkData_[rectLinkCount_].nowId = nowId;
    rectLinkData_[rectLinkCount_].nextId = nextId;
    rectLinkData_[rectLinkCount_].pos[3] = offset;  
    dss::strcpy_s(rectLinkData_[rectLinkCount_].nextMapName, 10, (char*)mapName);
    rectLinkCount_++;
}

ARM void cmn::ExtraMapLink::setFieldRectLinkToField(dss::Fix32Vector3& topLeft, dss::Fix32Vector3& bottomRight, int type, dss::Fix32Vector3 pos)
{
    rectLinkData_[rectLinkCount_].type = type;
    rectLinkData_[rectLinkCount_].nowId = 1;
    rectLinkData_[rectLinkCount_].pos[0] = topLeft;
    rectLinkData_[rectLinkCount_].pos[1] = bottomRight;
    rectLinkData_[rectLinkCount_].pos[2] = pos;
    rectLinkCount_++;
}

ARM int cmn::ExtraMapLink::checkFieldRectLinkNo(dss::Fix32Vector3& pos)
{
    for (int i = 0; i < rectLinkCount_; i++) {
        if (cmn::CommonCalculate::simpleAreaInCheck(rectLinkData_[i].pos[0], rectLinkData_[i].pos[1], pos) == 1) {
            return i;
        }
    }
    return -1;
}

ARM int cmn::ExtraMapLink::checkFieldRectLinkByType(dss::Fix32Vector3& pos, int type)
{
    int no = checkFieldRectLinkNo(pos);
    if (no != -1 && type == rectLinkData_[no].type) {
        if (rectLinkData_[no].type == RECT_FIELD_TO_TOWN) {
            StageLink::setTownExitIndex(rectLinkData_[no].nextId);
            g_Global.startTown(rectLinkData_[no].nextMapName);
            FieldPlayerManager::getSingleton();
            PlayerManager::setLock(1);
            offset_ = rectLinkData_[no].pos[3];
            return rectLinkData_[no].nowId;
        }

        extraLink_ = LINK_FIELD_ABS_POS;
        offset_ = rectLinkData_[no].pos[2];
        extraIdx_ = 4;
        switch (rectLinkData_[no].type) {
            case RECT_FIELD_TO_WORLD:
                g_Global.nextFieldType_ = 0;
                break;
            case RECT_FIELD_TO_YAMI:
                g_Global.nextFieldType_ = 2;
                break;
            case RECT_FIELD_TO_GOT:
                g_Global.nextFieldType_ = 1;
                break;
        }
        g_Global.startField();
        return 1;
    }
    return 0;
}

ARM bool cmn::ExtraMapLink::checkEraseSymbolNo(int id)
{
    for (int i = 0; i < mapLinkDataCount_; i++) {
        if (mapLinkData_[i].type == NOT_LINK_THIS_TOWN && mapLinkData_[i].nowId == id) {
            return true;
        }
    }
    return false;
}

ARM void cmn::ExtraMapLink::setExtraLinkFieldAbsPos(int fieldType, dss::Fix32Vector3& pos, short dir)
{
    PlayerManager::setLock(1);
    extraLink_ = LINK_FIELD_ABS_POS;
    offset_ = pos;
    g_Global.nextFieldType_ = fieldType;
    g_Global.startField();
    PlayerManager::setLock(1);
    extraIdx_ = dir;
}

ARM void cmn::ExtraMapLink::setRuraLink()
{
    int toField = 0;
    int symbolId;
    int townId = g_Stage.getRuraTownID();

    switch (townId) {
        case 0x17:
            g_Global.startTown(s_fk01);
            break;
        case 0x19:
            g_Global.startTown(s_dpb8b);
            break;
        case -1:
            symbolId = 0x12;
            g_Stage.setRuraFlag(3);
            toField = 1;
            break;
        case 0x13:
            symbolId = 0x58;
            toField = 1;
            break;
        default:
            symbolId = CommonRuraData::getSingleton()->getSymbolID(townId);
            toField = 1;
            break;
    }

    if (toField == 1) {
        StageLink::setFieldSymbolIndex(symbolId);
        g_Global.nextFieldType_ = CommonRuraData::getSingleton()->getWorld(townId);
        g_Global.startField();
    } else {
        g_Stage.flagMapChange_ = 1;
        g_Stage.setRuraFlag(3);
        StageLink::resetTownExitIndex();
    }
}

ARM void cmn::ExtraMapLink::setRanaLink()
{
    if (g_GlobalDQ4.unkfunc_02058114(12)) {
        setExtraExitTown(g_Stage.lastRanaStageName_, g_Stage.lastFldSurface_);
    } else if (g_GlobalDQ4.unkfunc_02058114(14)) {
        g_cmnPartyInfo.prevLocation_ = 1;
        dss::Fix32Vector3 pos = FieldPlayerManager::getSingleton()->getPosition();
        setExtraLinkFieldAbsPos(g_Global.getFieldType(), pos, 4);
    }
}

#ifndef DQIV_NATIVE
inline
#endif
dss::Fix32Vector3 FieldPlayerManager::getPosition()
{
    return position_;
}

ARM void cmn::ExtraMapLink::setData(int index, int nowId, int nextId, LINK_TYPE type, const char* nowMapName, const char* nextMapName,
                                     dss::Fix32Vector3& offset)
{
    mapLinkData_[index].nowId = nowId;
    mapLinkData_[index].nextId = nextId;
    mapLinkData_[index].type = type;
    mapLinkData_[index].offsetData = offset;
    if (nowMapName != NULL) {
        dss::strcpy_s(mapLinkData_[index].nowMapName, sizeof(mapLinkData_[index].nowMapName), nowMapName);
    }
    if (nextMapName != NULL) {
        dss::strcpy_s(mapLinkData_[index].nextMapName, sizeof(mapLinkData_[index].nextMapName), nextMapName);
    }
    if (g_GlobalDQ4.unkfunc_02058114(12)) {
        eraseSurface(nowId, type, nowMapName);
    }
}
