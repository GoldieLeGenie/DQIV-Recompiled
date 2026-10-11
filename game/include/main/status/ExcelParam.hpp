#pragma once
#include "main/param/Param.hpp"
#include "main/param/ShopDataFirst.hpp"
#include "main/param/ShopDataSecond.hpp"
#include "main/param/BattleMap.hpp"

#include <globaldefs.h>
#include "GameInfo.hpp"

namespace param {
    struct MonsterAnim;
}

namespace status {
    struct ExcelParam {
        param::CharInitData* charaInitData_;
        param::HeroData* heroData_;
        param::WarriorData* warriorData_;
        param::PrincessData* princessData_;
        param::PriestData* priestData_;
        param::MageData* mageData_;
        param::TraderData* traderData_;
        param::WarlockData* warlockData_;
        param::DancerData* dancerData_;
        param::PissaroData* pissaroData_;
        param::ItemData* itemData_;
        param::AppriseItem* appriseItem_;
        param::AlterMessage* alterMessage_;
        param::CharaVoice* charaVoice_;
        param::ActionParam* actionParam_;
        param::AbreactTurn* abreactTurn_;
        param::MonsterData* monsterData_;
        param::BookData* bookData_;
        param::MonsterAnim* monsterAnim_;
        param::SplitMsg* splitMsg_;
        param::EncountData* encountData_;
        param::EncountFormationID* encountFormationID_;
        param::EncountFormNum* encountFormNum_;
        param::EncountSpecial* encountSpecial_;
        param::MonsterMap* monsterMap_;
        param::EffectParam* effect_;
        param::EffectColorParam* effectColor_;
        param::ColorCorrect* colorCorrect_;
        param::FloorFog* floorFog_;
        param::CLUTCode* clutCode_;
        param::FloorBackColor* floorBackColor_;
        param::SurechigaiObjectData* surechigai_;
        param::FloorParam* floorParam_;
        param::ShopDataFirst* shopDataFirst_;
        param::ShopDataSecond* shopDataSecond_;
        param::CommonParam* commonParam_;
        param::FieldSymbol* fieldSymbol_;
        param::EncountSeaTile encountSeaTile_;
        param::EncountTile1 encountTile1_;
        param::EncountTile2 encountTile2_;
        param::EncountTile3 encountTile3_;
        param::EncountGotTile encountGotTile_;
        param::EncountYamiTile encountYamiTile_;
        param::BattleMap* battleMapData_;
        param::MapCamera* mapCamera_;
        param::VehicleData* vehicle_;
        param::MirrorMessage* mirrorMessage_;
        param::MapChurch* mapChurch_;
        param::SurechigaiTenant* surechigaiTenant_;

        ExcelParam();
        ~ExcelParam();
        void setup();
        param::CharInitData* getCharaInitData();
        param::HeroData* getHeroData();
        param::WarriorData* getWarriorData();
        param::PrincessData* getPrincessData();
        param::PriestData* getPriestData();
        param::MageData* getMageData();
        param::TraderData* getTraderData();
        param::WarlockData* getWarlockData();
        param::DancerData* getDancerData();
        param::PissaroData* getPissaroData();
        param::ItemData* getItemData();
        param::AppriseItem* getAppriseItem();
        param::AlterMessage* getAlterMessage();
        param::CharaVoice* getCharaVoice();
        void setupTown();
        void cleanupTown();
        void setupTownInitialize();
        void cleanupTownInitialize();
        void setupField();
        void cleanupField();
        void setupFieldInitialize();
        void cleanupFieldInitialize();

    };

    extern ExcelParam excelParam; //data_020c79b8

}

