#include "main/profile/Profile.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/status/ShopList.hpp"
#include "main/cmn/ExtraMapLink.hpp"
#include "main/global/Global.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/GameFlag.hpp"
#include "main/status/GameStatus.hpp"
#include "main/status/OptionStatus.hpp"
#include "main/status/BattleHistory.hpp"
#include "main/cmn/CommonCounterInfo.hpp"
#include "main/cmn/HengeNoTsueManager.hpp"
#include "main/cmn/PartyTalk.hpp"
#include "main/cmn/UnkEnvoyManager.hpp"

THUMB int profile::Profile::collectDATA(int bookNo, int saveType)
{
    presetMember();
    dss::memset(&this->profiledata_, 0xFF, 0x3300);
    dss::memset(this->profiledummy_, 0xFF, 0x900);
    this->pSYSTEM->MAGIC = 0x65747261;      // 'arte'
    this->pSYSTEM->VER = 0x0BF7FF5C;
    this->pSYSTEM->CHECKSUM = 0;
    if (bookNo != 3) {
        g_Stage.profileBank_ = bookNo;
        this->pSYSTEM->BOOKNO = bookNo;
    } else {
        this->pSYSTEM->BOOKNO = g_Stage.profileBank_;
    }
    this->pSYSTEM->SAVETYPE = saveType;
    collectDATA_PARTY();
    collectDATA_CHAPTER();
    collectDATA_PLAYER();
    collectDATA_MONSTER();
    collectDATA_ENVOY();
    calcCheckSum();
    return 1;
}

THUMB void profile::Profile::collectDATA_PARTY()
{
    dss::memset(this->pPARTY->RESTART, 0, 0x10);
    dss::memset(this->pPARTY->CHURCH, 0, 0x10);

    char* church = ::g_Stage.getChurchMapName();
    char* map = g_Global.getMapName();
    int fieldType = 0;
    if (church == 0) {
        church = map;
    }
    if (*church == 0) {
        church = map;
    }
    if (this->pSYSTEM->SAVETYPE == profile::SAVETYPE_CHURCH) {
        map = church;
    }
    if (strcmp(map, "field") == 0) {
        fieldType = g_Global.getFieldType();
    }
    dss::strcpy_s((char*)this->pPARTY->CHURCH, 0x10, church);
    dss::strcpy_s((char*)this->pPARTY->RESTART, 0x10, map);
    this->pPARTY->FIELDTYPE = fieldType;

    dss::Fix32Vector3 pos;
    short dir;
    g_cmnPartyInfo.getPartyInfo(&pos, &dir);
    this->pPARTY->PARTY_X = pos.vx.value;
    this->pPARTY->PARTY_Y = pos.vy.value;
    this->pPARTY->PARTY_Z = pos.vz.value;
    this->pPARTY->PARTY_D = dir;

    dss::Fix32Vector3 ship(::g_Stage.shipPosition_);
    this->pPARTY->SHIP_X = ship.vx.value;
    this->pPARTY->SHIP_Y = ship.vy.value;
    this->pPARTY->SHIP_Z = ship.vz.value;

    dss::Fix32Vector3 balloon(::g_Stage.balloonPosition_);
    this->pPARTY->BALLOON_X = balloon.vx.value;
    this->pPARTY->BALLOON_Y = balloon.vy.value;
    this->pPARTY->BALLOON_Z = balloon.vz.value;

    dss::Fix32Vector3 ikada = g_cmnPartyInfo.getIkadaPos();
    this->pPARTY->RAFT_X = ikada.vx.value;
    this->pPARTY->RAFT_Y = ikada.vy.value;
    this->pPARTY->RAFT_Z = ikada.vz.value;

    dss::Fix32Vector3 overview(::g_Stage.overviewTempPosition_);
    this->pPARTY->OVERVIEW_X = overview.vx.value;
    this->pPARTY->OVERVIEW_Y = overview.vy.value;
    this->pPARTY->OVERVIEW_Z = overview.vz.value;

    this->pPARTY->RIDEON = g_cmnPartyInfo.rideOnType_;
    this->pPARTY->BALLOON_FIELD = ::g_Stage.balloonFieldType_;
    
    dss::strcpy_s((char*)this->pPARTY->RANALUTA_MAP, 0xA, ::g_Stage.lastRanaStageName_);
    this->pPARTY->RANALUTA_SURFACE = ::g_Stage.lastFldSurface_;
    this->pPARTY->CHAPTER = status::g_Story.chapter_;

    collectGameFlag(this->pPARTY->GLOBALFLAG, &g_AreaFlag);
    collectGameFlag(this->pPARTY->AREAFLAG, &g_LocalFlag);
    collectGameFlag(this->pPARTY->LOCALFLAG, &g_GlobalFlag);
    for (int i = 0; i < 0x40; i++) {
        this->pPARTY->LOCALFLAG[i] = 0;
    }

    ::g_Stage.collectMapFlag((profile::SAVETYPE)this->pSYSTEM->SAVETYPE, this->pPARTY);
    status::g_Party.setSaveData(this->pPARTY);
    this->pPARTY->WORLDTIME = ::g_Stage.getWorldTime();
    this->pPARTY->TIMEZONE = ::g_Stage.getTimeZone();
    this->pPARTY->TIMESTOP = ::g_Stage.timestop_;
    this->pPARTY->SEX = status::g_Story.sex_;

    dss::memset(this->pPARTY->NAME, 0, 0x20);
    dss::strcpy_s((char*)this->pPARTY->NAME, 0x20, status::g_Story.heroName);

    for (int i = 0; i < 0x10; i++) {
        this->pPARTY->DAY_COUNTER[i] = cmn::g_CommonCounterInfo.dayCounter_[i];
    }
    for (int i = 0; i < 4; i++) {
        this->pPARTY->FREE_COUNTER[i] = cmn::g_CommonCounterInfo.freeCounter_[i];
    }
    this->pPARTY->BONMOL[0] = status::g_Story.getEndorEventItemCount(status::StoryStatus::EVENT_HAGANENOTURUGI);
    this->pPARTY->BONMOL[1] = status::g_Story.getEndorEventItemCount(status::StoryStatus::EVENT_TETUNOYOROI);
    this->pPARTY->PLAYTIME = status::g_Game.getPlayTime();
    this->pPARTY->LANGUAGE = status::g_Game.language;
    this->pPARTY->BGM_VOLUME = ::g_Option.getBgmVolume();
    this->pPARTY->SE_VOLUME = ::g_Option.getSeVolume();
    this->pPARTY->BATTLE_SPEED = ::g_Option.getBattleSpeed();
    this->pPARTY->ENCOUNT = encount::Encount::getSingleton()->enable_;
    this->pPARTY->SYMBOLID = ::g_Stage.symbolID_;

    for (int k = 0; k < 0xA2; k++) {
        this->pPARTY->NENEITEM[k] = status::g_Shop.haveItemNene_.getItem(k);
        this->pPARTY->NENECOUNT[k] = status::g_Shop.haveItemNene_.getItemCount(k);
    }
    for (int i = 0; i < 6; i++) {
        this->pPARTY->DARTS_ITEM[i] = status::g_Shop.sideJobItemFlag_[i];
    }
    for (int i = 0; i < 0xA; i++) {
        this->pPARTY->SPEAKTO_MESSAGE[i] = cmn::PartyTalk::getSingleton()->preMessage_[i];
    }
    this->pPARTY->SPEAKTO_OBJECT = cmn::PartyTalk::getSingleton()->objectNo_;
    this->pPARTY->SPEAKTO_EXITNO = cmn::PartyTalk::getSingleton()->lastExit_;
    this->pPARTY->SPEAKTO_ITEMNO = cmn::PartyTalk::getSingleton()->prevItem_;

    status::g_BattleHistory.setSaveData(this->pPARTY, this->pHISTORY);
    this->pPARTY->BANKMONEY = status::g_Party.bankMoney_;
    this->pPARTY->MEDALCOIN = status::g_Party.medalCoin_;
    this->pPARTY->TAROT = status::g_Story.isTarot();
    this->pPARTY->USE_BANK = status::g_Story.isUseBank();
    this->pPARTY->COMP_PICTUREBOOK = status::g_Story.isCompleteCoin();

    this->pPARTY->HENGE_CHARANO = g_HengeNoTsue.charNo_;
    this->pPARTY->HENGE_COUNTER = g_HengeNoTsue.counter_;
    this->pPARTY->HENGE_CHANGE = g_HengeNoTsue.change_;
    this->pPARTY->HENGE_ENDLESS = g_HengeNoTsue.endLess_;
    this->pPARTY->HENGE_INDEX = g_HengeNoTsue.index_;
    this->pPARTY->PLAYERMEDAL = status::g_Party.playerMedalCoin_;

    dss::memset(this->pPARTY->IKADAMAP, 0, 0x10);
    dss::strcpy_s((char*)this->pPARTY->IKADAMAP, 0x10, g_cmnPartyInfo.getIkadaMapName());
    this->pPARTY->BALONFLAG = g_cmnPartyInfo.barron_;
    this->pPARTY->UNIQUEID = status::g_Game.getUniqueID();

    for (int i = 0; i < 0x32; i++) {
        this->pPARTY->SELECTTAISHI_FLAG[i] = data_020f0078.unkfunc_0203ab30(i);
    }
}

THUMB void profile::Profile::collectDATA_CHAPTER()
{
    profile::PROFILE_CHAPTER* pc = this->pCHAPTER;
    status::HaveItemSack* sack = status::g_Story.fukuro_;

    for (int i = 0; i < 4; i++) {
        for (int k = 0; k < 162; k++) {
            pc->SACKITEM[k] = sack->getItem(k);
            int maxItem = sack->getItemCount(k);
            if (maxItem > 99) {
                maxItem = 99;
            }
            pc->SACKCOUNT[k] = maxItem;
        }
        pc->GOLD = status::g_Story.gold_[i];
        pc->CASINOCOIN = status::g_Story.coin_[i];
        pc++;
        sack++;
    }

    for (int k = 0; k < 162; k++) {
        pc->SACKITEM[k] = status::g_Party.haveItemSack_.getItem(k);
        int maxItem = status::g_Party.haveItemSack_.getItemCount(k);
        if (maxItem > 99) {
            maxItem = 99;
        }
        pc->SACKCOUNT[k] = maxItem;
    }
    pc->GOLD = status::g_Party.gold_;
    pc->CASINOCOIN = status::g_Party.casinoCoin_;
}


THUMB void profile::Profile::collectDATA_PLAYER()
{

    dss::memset(this->pPARTY->PARTY, 0, 14);
    this->pPARTY->PARTY[0] = 1;
    this->pPARTY->PARTY[1] = 2;
    this->pPARTY->PARTY[2] = 3;
    this->pPARTY->PARTY[3] = 4;
    this->pPARTY->PARTY[4] = 5;
    this->pPARTY->PARTY[5] = 6;
    this->pPARTY->PARTY[6] = 7;
    this->pPARTY->PARTY[7] = 8;
    this->pPARTY->PARTY[8] = 9;
    this->pPARTY->PARTY[9] = 25;

    for (int i = 0; i < 25; i++) {
        int k = i + 1;
        if (originalPlayerFlag_[k].flag_ != 0) {
            status::PlayerStatus* op = &originalPlayer_[k];
            int j = 0;
            int pi = op->haveStatusInfo_.haveStatus_.playerIndex_;
            for (; j < 14; j++) {
                if (pi == this->pPARTY->PARTY[j]) {
                    break;
                }
                if (this->pPARTY->PARTY[j] == 0) {
                    this->pPARTY->PARTY[j] = pi;
                    break;
                }
            }
        }
    }
    for (int j = 0; j < 14; j++) {
        int pi = this->pPARTY->PARTY[j];
        if (pi != 0) {
            status::PlayerStatus* p = status::PartyStatus::getPlayerStatusForPlayerIndex(pi);
            p->setSaveDataForPlayer(&this->pPLAYER[j]);
            p->haveStatusInfo_.haveEquipment_.setSaveDataForPlayer(&this->pPLAYER[j]);
        }
    }
}

THUMB void profile::Profile::collectDATA_MONSTER()
{
    for (unsigned int i = 0; i < 0xD2; i++) {
        this->pMONSTER->KILL = status::g_BattleResult.getMonsterCount(i);
        this->pMONSTER->ITEMCOUNT = status::g_BattleResult.getItemCount(i);
        this->pMONSTER->LEVEL = status::g_BattleResult.getLevel(i);
        this->pMONSTER->ENCOUNT = status::g_BattleResult.isEncount(i);
        this->pMONSTER++;
    }
}

THUMB void profile::Profile::collectDATA_ENVOY()
{
    for (int i = 0; i < 0x18; i++) {
        if (data_020f0078.unkfunc_0203a358(i) != 0) {
            data_020f0078.unkfunc_0203a34c(i);
            this->pENVOY->UNIQUE = data_020f0078.unkfunc_0203a5a4();
            this->pENVOY->TYPE = data_020f0078.unkfunc_0203a5ec();
            this->pENVOY->SEX = data_020f0078.unkfunc_0203a714();
            this->pENVOY->AGE = data_020f0078.unkfunc_0203a750();
            this->pENVOY->SKILL = data_020f0078.unkfunc_0203a78c();
            dss::memcpy(this->pENVOY->NAME, data_020f0078.unkfunc_0203a65c(), 0x1A);
            dss::memcpy(this->pENVOY->HERONAME, data_020f0078.unkfunc_0203a6d8(), 0x1A);
            dss::memcpy(this->pENVOY->TOWNNAME, data_020f0078.unkfunc_0203a820(), 0x2A);
            dss::memcpy(this->pENVOY->COMMENT, data_020f0078.unkfunc_0203a938(), 0x5C);
        } else {
            this->pENVOY->TYPE = 0xFF;
        }
        this->pENVOY++;
    }
    if (data_020f0078.unkfunc_0203a354() != 0) {
        data_020f0078.mode_ = 1;
        this->pENVOY->UNIQUE = data_020f0078.unkfunc_0203a5a4();
        this->pENVOY->TYPE = data_020f0078.unkfunc_0203a5ec();
        this->pENVOY->SEX = data_020f0078.unkfunc_0203a714();
        this->pENVOY->AGE = data_020f0078.unkfunc_0203a750();
        this->pENVOY->SKILL = data_020f0078.unkfunc_0203a78c();
        dss::memcpy(this->pENVOY->NAME, data_020f0078.unkfunc_0203a65c(), 0x1A);
        dss::memcpy(this->pENVOY->HERONAME, data_020f0078.unkfunc_0203a6d8(), 0x1A);
        dss::memcpy(this->pENVOY->TOWNNAME, data_020f0078.unkfunc_0203a820(), 0x2A);
        dss::memcpy(this->pENVOY->COMMENT, data_020f0078.unkfunc_0203a938(), 0x5C);
    } else {
        this->pENVOY->TYPE = 0xFF;
    }
}
