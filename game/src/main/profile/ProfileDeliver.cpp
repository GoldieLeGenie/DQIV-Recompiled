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
#include "main/sound/Sound.hpp"
#include "main/cmn/UnkEnvoyManager.hpp"
#include "main/cmn/UnkImmigrantTown.hpp"

THUMB int profile::Profile::deliverDATA(int)
{
    presetMember();
    if (isValidData() == 0) {
        return 0;
    }
    if (calcCheckSum() == 0) {
        return 0;
    }
    deliverDATA_PARTY();
    deliverDATA_CHAPTER();
    deliverDATA_PLAYER();
    deliverDATA_MONSTER();
    deliverDATA_ENVOY();
    return 1;
}


THUMB void profile::Profile::deliverDATA_PARTY()
{
    g_Global.initialize();
    Sound::unkfunc_0205594c(15);

    g_Stage.profileBank_ = pSYSTEM->BOOKNO;                       
    g_Stage.loadType_    = (profile::SAVETYPE)pSYSTEM->SAVETYPE;  

    g_Stage.initialize();
    g_Stage.setup((char *)pPARTY->RESTART);
    g_Stage.setChurchMapName((char *)pPARTY->CHURCH);

    dss::Fix32Vector3 pos;
    pos.vx.value = pPARTY->PARTY_X;
    pos.vy.value = pPARTY->PARTY_Y;
    pos.vz.value = pPARTY->PARTY_Z;
    deliverRESTART_MAP(&pos, pPARTY->PARTY_D);

    dss::Fix32Vector3 ship;
    ship.vx.value = pPARTY->SHIP_X;
    ship.vy.value = pPARTY->SHIP_Y;
    ship.vz.value = pPARTY->SHIP_Z;
    g_Stage.shipPosition_ = dss::Fix32Vector3(ship);

    dss::Fix32Vector3 balloon;
    balloon.vx.value = pPARTY->BALLOON_X;
    balloon.vy.value = pPARTY->BALLOON_Y;
    balloon.vz.value = pPARTY->BALLOON_Z;
    g_Stage.balloonPosition_ = dss::Fix32Vector3(balloon);

    dss::Fix32Vector3 ikada;
    ikada.vx.value = pPARTY->RAFT_X;
    ikada.vy.value = pPARTY->RAFT_Y;
    ikada.vz.value = pPARTY->RAFT_Z;
    g_cmnPartyInfo.townIkadaPos_ = ikada;                         // +0x944

    dss::Fix32Vector3 overview;
    overview.vx.value = pPARTY->OVERVIEW_X;
    overview.vy.value = pPARTY->OVERVIEW_Y;
    overview.vz.value = pPARTY->OVERVIEW_Z;
    g_Stage.overviewPosition_     = overview;
    g_Stage.overviewTempPosition_ = overview;

    g_cmnPartyInfo.rideOnType_ = (cmn::PARTY_RIDE_ON_TYPE)pPARTY->RIDEON;
    g_Stage.balloonFieldType_  = pPARTY->BALLOON_FIELD;
    g_Stage.setRanaMapName((char *)pPARTY->RANALUTA_MAP);
    g_Stage.lastFldSurface_    = pPARTY->RANALUTA_SURFACE;

    status::g_Story.setChapter(pPARTY->CHAPTER);

    deliverGameFlag(&g_AreaFlag,   pPARTY->GLOBALFLAG);   // 0xDC
    deliverGameFlag(&g_LocalFlag,  pPARTY->AREAFLAG);     // 0x11C
    deliverGameFlag(&g_GlobalFlag, pPARTY->LOCALFLAG);    // 0x15C

    g_Stage.deliverMapFlag((profile::SAVETYPE)pSYSTEM->SAVETYPE, pPARTY);


    status::g_Party.setLoadData(pPARTY);
    

    g_Stage.setTimeZone((TIME_ZONE)pPARTY->TIMEZONE);
    g_Stage.setWorldTime(pPARTY->WORLDTIME);
    g_Stage.timestop_ = pPARTY->TIMESTOP;                         

    status::g_Story.sex_ = (Sex)pPARTY->SEX;                      // +8
    status::g_Story.setHeroName((char *)pPARTY->NAME);

    for (int i = 0; i < 16; i++)
        cmn::g_CommonCounterInfo.dayCounter_[i] = pPARTY->DAY_COUNTER[i];

    for (int i = 0; i < 4; i++)
        cmn::g_CommonCounterInfo.freeCounter_[i] = pPARTY->FREE_COUNTER[i];

    status::g_Story.setEndorEventItemCount(status::StoryStatus::EVENT_HAGANENOTURUGI, pPARTY->BONMOL[0]);
    status::g_Story.setEndorEventItemCount(status::StoryStatus::EVENT_TETUNOYOROI,    pPARTY->BONMOL[1]);

    status::g_Game.setPlayTime(pPARTY->PLAYTIME);

    g_Option.setBgmVolume(pPARTY->BGM_VOLUME);
    g_Option.setSeVolume(pPARTY->SE_VOLUME);
    g_Option.setBattleSpeed(pPARTY->BATTLE_SPEED);

    int encount = pPARTY->ENCOUNT;
    encount::Encount::getSingleton()->enable_ = encount;
    g_Stage.symbolID_ = pPARTY->SYMBOLID;                         // +0xBC

    for (int i = 0; i < 162; i++)
        status::g_Shop.haveItemNene_.adds(pPARTY->NENEITEM[i], pPARTY->NENECOUNT[i]);

    int *dart = status::g_Shop.sideJobItemFlag_;
    for (int i = 0; i < 6; i++)
    status::g_Shop.sideJobItemFlag_[i] = pPARTY->DARTS_ITEM[i];


    for (int i = 0; i < 10; i++)
        cmn::PartyTalk::getSingleton()->setPreMessage(i, pPARTY->SPEAKTO_MESSAGE[i]);

    int objectNo = pPARTY->SPEAKTO_OBJECT;
    cmn::PartyTalk::getSingleton()->objectNo_ = objectNo;          // +0x1B0
    int exitNo = pPARTY->SPEAKTO_EXITNO;
    cmn::PartyTalk::getSingleton()->lastExit_ = exitNo;            // +0x1B4
    int itemNo = pPARTY->SPEAKTO_ITEMNO;
    cmn::PartyTalk::getSingleton()->prevItem_ = itemNo;            // +0x1B8

    status::g_BattleHistory.setLoadData(pPARTY, pHISTORY);

    status::g_Party.setBankMoney(pPARTY->BANKMONEY);
    status::g_Party.setMedalCoin(pPARTY->MEDALCOIN);

    status::g_Story.setTarot(pPARTY->TAROT);
    status::g_Story.setUseBank(pPARTY->USE_BANK);
    status::g_Story.setCompleteCoin(pPARTY->COMP_PICTUREBOOK);

    g_HengeNoTsue.charNo_  = pPARTY->HENGE_CHARANO;               // +4
    g_HengeNoTsue.counter_ = pPARTY->HENGE_COUNTER;               // +8
    g_HengeNoTsue.change_  = pPARTY->HENGE_CHANGE;                // +0xC
    g_HengeNoTsue.endLess_ = pPARTY->HENGE_ENDLESS;               // 
    g_HengeNoTsue.index_   = pPARTY->HENGE_INDEX;                 // +0

    status::g_Party.setPlayerMedalCoin(pPARTY->PLAYERMEDAL);
    g_cmnPartyInfo.setIkadaMapName((char *)pPARTY->IKADAMAP);
    g_cmnPartyInfo.barron_ = pPARTY->BALONFLAG;                   // +0x964
    status::g_Game.setUniqueID(pPARTY->UNIQUEID);

    for (int i = 0; i < 50; i++)
        data_020f0078.unkfunc_0203ab20(i, pPARTY->SELECTTAISHI_FLAG[i]);
}

THUMB void profile::Profile::deliverDATA_CHAPTER()
{
    profile::PROFILE_CHAPTER* pc = this->pCHAPTER;
    status::HaveItemSack* sack = status::g_Story.fukuro_;

    for (int i = 0; i < 4; i++) {
        for (int k = 0; k < 0xA2; k++) {
            sack->adds(pc->SACKITEM[k], pc->SACKCOUNT[k]);
        }
        status::g_Story.gold_[i] = pc->GOLD;
        status::g_Story.coin_[i] = pc->CASINOCOIN;
        pc++;
        sack++;
    }

    for (int k = 0; k < 0xA2; k++) {
        status::g_Party.haveItemSack_.adds(pc->SACKITEM[k], pc->SACKCOUNT[k]);
    }
    status::g_Party.setGold(pc->GOLD);
    status::g_Party.setCasinoCoin(pc->CASINOCOIN);
}

THUMB void profile::Profile::deliverDATA_PLAYER()
{
    for (int j = 0; j < 14; j++) {
        int pi = this->pPARTY->PARTY[j];
        if (pi != 0) {
            status::PlayerStatus* p = status::PartyStatus::getPlayerStatusForPlayerIndex(pi);
            p->setLoadDataForPlayer(&this->pPLAYER[j]);
            p->haveStatusInfo_.haveEquipment_.setLoadDataForPlayer(&this->pPLAYER[j]);
        }
    }
}

THUMB void profile::Profile::deliverDATA_MONSTER()
{
    for (unsigned int i = 0; i < 0xD2; i++) {
        status::g_BattleResult.setMonsterCount(i, this->pMONSTER->KILL);
        status::g_BattleResult.setItemCount(i, this->pMONSTER->ITEMCOUNT);
        status::g_BattleResult.setLevel(i, this->pMONSTER->LEVEL);
        status::g_BattleResult.setEncount(i, this->pMONSTER->ENCOUNT);
        this->pMONSTER++;
    }
}

THUMB void profile::Profile::deliverDATA_ENVOY()
{
    for (int i = 0; i < 0x18; i++) {
        data_020f0078.unkfunc_0203a34c(i);
        if (this->pENVOY->TYPE != 0xFF) {
            data_020f0078.unkfunc_0203a574(1);
            data_020f0078.unkfunc_0203a58c(this->pENVOY->UNIQUE);
            data_020f0078.unkfunc_0203a5bc(this->pENVOY->TYPE);
            data_020f0078.unkfunc_0203a6f4(this->pENVOY->SEX);
            data_020f0078.unkfunc_0203a730(this->pENVOY->AGE);
            data_020f0078.unkfunc_0203a76c(this->pENVOY->SKILL);
            dss::memcpy(data_020f0078.unkfunc_0203a65c(), this->pENVOY->NAME, 0x1A);
            dss::memcpy(data_020f0078.unkfunc_0203a6d8(), this->pENVOY->HERONAME, 0x1A);
            dss::memcpy(data_020f0078.unkfunc_0203a820(), this->pENVOY->TOWNNAME, 0x2A);
            dss::memcpy(data_020f0078.unkfunc_0203a938(), this->pENVOY->COMMENT, 0x5C);
        } else {
            data_020f0078.unkfunc_0203a574(0);
        }
        this->pENVOY++;
    }
    UnkImmigrantTown::getSingleton()->unkfunc_02037ca4();
    data_020f0078.mode_ = 1;
    if (this->pENVOY->TYPE != 0xFF) {
        data_020f0078.unkfunc_0203a574(1);
        data_020f0078.unkfunc_0203a58c(this->pENVOY->UNIQUE);
        data_020f0078.unkfunc_0203a5bc(this->pENVOY->TYPE);
        data_020f0078.unkfunc_0203a6f4(this->pENVOY->SEX);
        data_020f0078.unkfunc_0203a730(this->pENVOY->AGE);
        data_020f0078.unkfunc_0203a76c(this->pENVOY->SKILL);
        dss::memcpy(data_020f0078.unkfunc_0203a65c(), this->pENVOY->NAME, 0x1A);
        dss::memcpy(data_020f0078.unkfunc_0203a6d8(), this->pENVOY->HERONAME, 0x1A);
        dss::memcpy(data_020f0078.unkfunc_0203a820(), this->pENVOY->TOWNNAME, 0x2A);
        dss::memcpy(data_020f0078.unkfunc_0203a938(), this->pENVOY->COMMENT, 0x5C);
    } else {
        data_020f0078.unkfunc_0203a574(0);
    }
}


THUMB void profile::Profile::deliverRESTART_MAP(dss::Fix32Vector3* pos, short dir)
{
    if (this->pSYSTEM->SAVETYPE == profile::SAVETYPE_CHURCH && g_Stage.restartChurch() == 1) {
        g_Stage.load_ = 1;
        return;
    }
    if (dss::strcmp((const char*)this->pPARTY->RESTART, "field") == 0) {
        cmn::g_extraMapLink.setExtraLinkFieldAbsPos(this->pPARTY->FIELDTYPE, *pos, 4);
        return;
    }
    cmn::g_extraMapLink.setExtraLinkTown((const char*)this->pPARTY->RESTART, *pos, dir);
}
