#pragma once
#include "main/dss/DssUtils.hpp"
#include "main/data/DataObject.hpp"
#include "main/param/FloorParam.hpp"
#include <globaldefs.h>
#include "main/param/ColorCorrect.hpp"
#include "main/param/ShopDataFirst.hpp"
#include "main/param/ShopDataSecond.hpp"
#include "main/param/MonsterMap.hpp"
namespace param {
    template <typename T, int N>
    struct ExcelFile {
        unsigned int id;
        T data[N];
    };

    struct PartyTalk
    {
        unsigned int messageID;
        unsigned int value1;
        unsigned int value2;
        unsigned int value3;
        unsigned int pick1;
        unsigned short start;
        unsigned short end;
        unsigned short floor;
        unsigned short document1;
        unsigned char object1;
        unsigned char object2;
        unsigned char condition1;
        unsigned char condition2;
        unsigned char condition3;
        unsigned char horse;
        char byte_1;
        char byte_2;
        char byte_3;
        char byte_4;
        char byte_5;
        char byte_6;
        char byte_7;
        unsigned char dmmy0;
        unsigned char dmmy1;
        unsigned char dmmy2;
    };
#ifdef DQIV_NATIVE
    struct PartyTalkIndex {
        short floor;
        short seek;     // first record
        short size;     // record count
    };
    struct PartyTalkTable {
        const char* filename;
        const PartyTalkIndex* index;
        int count;
    };
    extern const PartyTalkTable partyTalkTables[6]; // chapters 1-4, 5 and 6
#endif
    struct AppriseItem {
        unsigned int message1;
        unsigned int message2;
        unsigned int message3;
        unsigned int message4;
        unsigned int message5;
        unsigned int message6;
        unsigned int message7;
    };

    struct AlterMessage {
        unsigned int message;
        unsigned short obj;
        unsigned char dmmy0;
        unsigned char dmmy1;

        static const unsigned int size_;
        static const unsigned int ID_;
        static DataObject data_;
    };

    struct CharaVoice {
        unsigned short index;
        unsigned short voice;

        static const unsigned int size_;
        static const unsigned int ID_;
        static DataObject data_;
    };
    
    struct ActionParam {
        unsigned int actionMes;
        unsigned int playerSuccessMes;
        unsigned int playerSuccessMesDie;
        unsigned int monsterSuccessMes;
        unsigned int monsterSuccessMesDie;
        unsigned int playerFailedMes;
        unsigned int monsterFailedMes;
        unsigned int endMes;
        unsigned int menuMes;
        unsigned short action;
        unsigned short effectFriend;
        unsigned short effectEnemy;
        unsigned short effectLap;
        unsigned short MonsterMin;
        unsigned short MonsterMax;
        unsigned short PlayerMin;
        unsigned short PlayerMax;
        unsigned short menuIndex;
        unsigned char type;
        unsigned char magictype;
        unsigned char canceltype;
        unsigned char kouka;
        unsigned char useMP;
        unsigned char fool;
        unsigned char human;
        unsigned char god;
        char byte_1;
        char byte_2;
        char byte_3;
        char byte_4;
        char byte_5;
        char byte_6;
        char byte_7;
        unsigned char dmmy0;
        unsigned char dmmy1;
        unsigned char dmmy2;
    };

    struct AbreactTurn {
        unsigned int msg1;
        unsigned int msg2;
        unsigned int msg3;
        unsigned int msg4;
        unsigned int msg5;
        unsigned int msg6;
        unsigned int msg7;
        unsigned short action;
        unsigned char turn;
        unsigned char pattern;
        char byte_1;
        char byte_2;
        unsigned char dmmy0;
        unsigned char dmmy1;
    };

    struct MonsterData {
        unsigned short name;
        unsigned short level;
        unsigned short exp;
        unsigned short MP;
        unsigned short HP;
        unsigned short attack;
        unsigned short defence;
        unsigned short money;
        unsigned short item;
        unsigned short action1;
        unsigned short action2;
        unsigned short action3;
        unsigned short action4;
        unsigned short action5;
        unsigned short action6;
        unsigned char agility;
        unsigned char itemRatio;
        unsigned char integer;
        unsigned char focus;
        unsigned char times;
        unsigned char heal;
        unsigned char avoid;
        unsigned char init;
        unsigned char initRatio;
        unsigned char pattern;
        unsigned char animation1;
        unsigned char animation2;
        unsigned char animation3;
        unsigned char animation4;
        unsigned char animation5;
        unsigned char animation6;
        unsigned char animationMuliti;
        char byte_1;
        char byte_2;
        char byte_3;
        char byte_4;
        char byte_5;
        char byte_6;
        char byte_7;
        char byte_8;
        char byte_9;
        char byte_10;
        unsigned char dmmy0;
        unsigned char dmmy1;
        unsigned char dmmy2;
    };

    struct BookData {
        unsigned short name;
        unsigned char ID;
        unsigned char type;
        char byte_1;
        unsigned char dmmy0;
        unsigned char dmmy1;
        unsigned char dmmy2;
    };

    

    struct SplitMsg {
        unsigned int branch;
        unsigned int aliveOne;
        unsigned int aliveTwo;
        unsigned int monsterOne;
        unsigned int monsterTwo;
        unsigned int monsterMore;
        unsigned int activate;
        unsigned int deactivate;
        unsigned int alive;
        unsigned int dead;
        unsigned int astoron;
        unsigned int mosyasu;
        unsigned int splitAvoid;
        unsigned int split;
        unsigned int avoid;
        unsigned int equip;
        unsigned int notEquit;
        unsigned int male;
        unsigned int female;
        unsigned int wastePlace;
        unsigned int rura;
        unsigned int riremito;
        unsigned int wasteTime;
        unsigned int cofferItem;
        unsigned int cofferMonster;
        unsigned int cofferGold;
        unsigned int cofferNothing;
        unsigned int tuboItem;
        unsigned int tuboMonster;
        unsigned int tuboGold;
        unsigned int tuboNothing;
        unsigned int noTarget;
        unsigned int northEast;
        unsigned int southEast;
        unsigned int northWest;
        unsigned int southWest;
        unsigned int nothing;
        unsigned int special;

        static const unsigned int size_;
        static const unsigned int ID_;
        static DataObject data_;
    };

    struct EncountData {
        unsigned short sound;
        unsigned short monsterA;
        unsigned short monsterB;
        unsigned short monsterC;
        unsigned short monsterD;
        unsigned short monsterE;
        unsigned short monsterF;
        unsigned short monsterG;
        unsigned short monsterH;
        unsigned short monsterI;
        unsigned short monsterJ;
        unsigned short monsterK;
        unsigned short monsterL;
        unsigned short specialM;
        unsigned short specialN;
        unsigned char tileLevel;
        unsigned char ratio;
        unsigned char formation;
        unsigned char firstattack;
        unsigned char invite;
        unsigned char escape;
        unsigned char event;
        unsigned char dmmy0;
        unsigned char dmmy1;
        unsigned char dmmy2;
    };

    struct EncountFormationID {
        unsigned char next2;
        unsigned char next3;
        unsigned char typeA;
        unsigned char typeB;
        unsigned char typeC;
        unsigned char typeD;
        unsigned char typeE;
        unsigned char typeF_rand;
        unsigned char typeF_min;
        unsigned char typeF_max;
        unsigned char typeG_rand;
        unsigned char typeG_min;
        unsigned char typeG_max;
        unsigned char typeH_rand;
        unsigned char typeH_min;
        unsigned char typeH_max;
        unsigned char typeI_rand;
        unsigned char typeI_min;
        unsigned char typeI_max;
        unsigned char typeJ_rand;
        unsigned char typeJ_min;
        unsigned char typeJ_max;
        unsigned char typeK;
        unsigned char typeL;
        unsigned char typeM;
        unsigned char typeN;
        unsigned char dmmy0;
        unsigned char dmmy1;
    };
    struct EncountFormNum {
        unsigned short groupraito;
        unsigned short ratio;
        unsigned char section;
        unsigned char party;
        unsigned char groupmax;
        unsigned char dmmy0;
    };

    struct EncountSpecial {
        unsigned short name;
        unsigned short monsterID1;
        unsigned short group1_min;
        unsigned short group1_max;
        unsigned short monsterID2;
        unsigned short group2_min;
        unsigned short group2_max;
        unsigned short monsterID3;
        unsigned short group3_min;
        unsigned short group3_max;
        unsigned short monsterID4;
        unsigned short group4_min;
        unsigned short group4_max;
        unsigned char daytile1;
        unsigned char daytile2;
        unsigned char daytile3;
        unsigned char daytile4;
        unsigned char nighttile;
        unsigned char dmmy0;
    };

    struct EffectParam {
        fx32 scale;
        unsigned short index;
        unsigned short frame;
        unsigned short homing;
        unsigned short sound;
        unsigned char interval;
        unsigned char camera;
        unsigned char color;
        unsigned char hold;
        unsigned char camera2;
        unsigned char wait;
        char byte_1;
        char byte_2;

        static void getCameraFile(int camera, char* file);

        static const unsigned int ID_;
        static const unsigned int size_;
        static DataObject data_;
    };

    struct EffectColorParam {
        fx32 rPoint;
        fx32 gPoint;
        fx32 bPoint;
        unsigned short frame;
        unsigned char dmmy0;
        unsigned char dmmy1;
    };

    


    struct FloorFog {
        fx32 side00_rate;
        fx32 side01_rate;
        unsigned char side00_R;
        unsigned char side00_G;
        unsigned char side00_B;
        unsigned char side00_offset;
        unsigned char side01_R;
        unsigned char side01_G;
        unsigned char side01_B;
        unsigned char side01_offset;
        char byte_1;
        unsigned char dmmy0;
        unsigned char dmmy1;
        unsigned char dmmy2;
    };

    struct CLUTCode {
        fx32 rPoint;
        fx32 gPoint;
        fx32 bPoint;
        unsigned char code;
        unsigned char dmmy0;
        unsigned char dmmy1;
        unsigned char dmmy2;
    };

    struct FloorBackColor {
        unsigned char backcolor;
        union {
            struct {
                unsigned char topUpLeftR;
                unsigned char topUpLeftG;
                unsigned char topUpLeftB;
                unsigned char topUpRightR;
                unsigned char topUpRightG;
                unsigned char topUpRightB;
                unsigned char topDownLeftR;
                unsigned char topDownLeftG;
                unsigned char topDownLeftB;
                unsigned char topDownRightR;
                unsigned char topDownRightG;
                unsigned char topDownRightB;
                unsigned char bottomUpLeftR;
                unsigned char bottomUpLeftG;
                unsigned char bottomUpLeftB;
                unsigned char bottomUpRightR;
                unsigned char bottomUpRightG;
                unsigned char bottomUpRightB;
                unsigned char bottomDownLeftR;
                unsigned char bottomDownLeftG;
                unsigned char bottomDownLeftB;
                unsigned char bottomDownRightR;
                unsigned char bottomDownRightG;
                unsigned char bottomDownRightB;
            };
            char color[24];
        };
        unsigned char dmmy0;
        unsigned char dmmy1;
        unsigned char dmmy2;
    };


    struct SurechigaiObjectData {
        unsigned char index;
        unsigned char level;
        char byte_1;
        unsigned char dmmy0;
    };

    

    

    struct CommonList {                                 // data/param/param_item_%s.dat
        unsigned int message;                           // 0x00
        unsigned short uid;                             // 0x04
        unsigned short item;                            // 0x06
        unsigned short gold;                            // 0x08
        unsigned short monster;                         // 0x0A
        unsigned short encount;                         // 0x0C
        unsigned short uidReplace;                      // 0x0E
        unsigned short openIndex;                       // 0x10
        unsigned short flagIndex;                       // 0x12
        unsigned char type;                             // 0x14
        unsigned char furnIndex;                        // 0x15
        unsigned char ListSize;                         // 0x16
        char byte_1;                                    // 0x17

        static const unsigned int size_;
        static const unsigned int ID_;                  // data_0208ca60
        static DataObject data_;                        // data_020c7964
    };

    struct CommonParam {
        unsigned int checkMsg;
        unsigned int normalMsg;
        unsigned int NothingMsg;
        unsigned int MonsterMsg;
        unsigned int BackMsg;
        unsigned short sound;
        unsigned char type;
        char byte_1;
    };

    struct FieldSymbol {
        unsigned int message;
        unsigned char uid;
        unsigned char world;
        unsigned char type;
        unsigned char dispX;
        unsigned char dispY;
        unsigned char color;
        unsigned char dmmy0;
        unsigned char dmmy1;

        unsigned char getWorld() { return world; }
    };

    struct EncountSeaTile {
        unsigned char* tile_;
        static const unsigned int ID_;
        static const unsigned int width_;
    };

    struct EncountTile1 {
        unsigned char* tile_;
        static const unsigned int ID_;
        static const unsigned int width_;
        static const unsigned int height_;
    };

    struct EncountTile2 {
        unsigned char* tile_;
        static const unsigned int ID_;
        static const unsigned int width_;
        static const unsigned int height_;
    };

    struct EncountTile3 {
        unsigned char* tile_;
        static const unsigned int ID_;
        static const unsigned int width_;
        static const unsigned int height_;
    };

    struct EncountGotTile {
        unsigned char* tile_;
        static const unsigned int ID_;
        static const unsigned int width_;
        static const unsigned int height_;
    };

    struct EncountYamiTile {
        unsigned char* tile_;
        static const unsigned int ID_;
        static const unsigned int width_;
        static const unsigned int height_;
    };

    

    struct MapCamera {
        fx32 distance;
        fx32 angleX;
        fx32 angleY;
        fx32 angleZ;
        fx32 targetX;
        fx32 targetY;
        fx32 targetZ;
        char floor[8];
        char file[16];

        static const unsigned int size_;
        static const unsigned int ID_;
        static DataObject data_;
    };

    struct VehicleData {
        fx32 shipX;
        fx32 shipY;
        fx32 balloonX;
        fx32 balloonY;
        unsigned char world;
        unsigned char id;
        char mapname[4];
        unsigned char dmmy0;
        unsigned char dmmy1;

        static const unsigned int size_;                // data_0208ca54
        static const unsigned int ID_;
    };

    struct MirrorMessage {
        unsigned int message;
        unsigned char leader;
        unsigned char dmmy0;
        unsigned char dmmy1;
        unsigned char dmmy2;

        static const unsigned int size_;
        static const unsigned int ID_;
        static DataObject data_;
    };

    struct MapChurch {
        fx32 playerX;
        fx32 playerY;
        fx32 playerZ;
        unsigned short direction;
        char floor[8];
        char byte_1;
        unsigned char dmmy0;

        static const unsigned int size_;
        static const unsigned int ID_;
        static DataObject data_;
    };
    struct SurechigaiTenant {
        unsigned char index;
        unsigned char icon;
        unsigned char name;
        unsigned char tokugi;
        unsigned char from;
        char byte_1;
        unsigned char dmmy0;
        unsigned char dmmy1;

        char getLevel() { return (byte_1 & 0x1c) >> 2; }
    };
    struct CharInitData {
        unsigned short monsterID;
        unsigned short strength;
        unsigned short agility;
        unsigned short vitality;
        unsigned short intelligence;
        unsigned short luck;
        unsigned short HP;
        unsigned short MP;
        unsigned short attack;
        unsigned short defence;
        unsigned short gold;
        unsigned short weapon;
        unsigned short armor;
        unsigned short shield;
        unsigned short hat;
        unsigned short battle1;
        unsigned short battle2;
        unsigned short battle3;
        unsigned short battle4;
        unsigned short battle5;
        unsigned short battle6;
        unsigned short battle7;
        unsigned short battle8;
        unsigned short battle9;
        unsigned short battle10;
        unsigned short battle11;
        unsigned short battle12;
        unsigned short battle13;
        unsigned short magic1;
        unsigned short magic2;
        unsigned short magic3;
        unsigned short magic4;
        unsigned short magic5;
        unsigned short magic6;
        unsigned short magic7;
        unsigned short magic8;
        unsigned short magic9;
        unsigned short magic10;
        unsigned short magic11;
        unsigned short magic12;
        unsigned short magic13;
        unsigned char character;
        unsigned char icon;
        unsigned char job;
        unsigned char level;
        char byte_1;
        unsigned char dmmy0;

    };
    struct HeroData {
        unsigned int exp;
        unsigned short strength;
        unsigned short agility;
        unsigned short intelligence;
        unsigned short luck;
        unsigned short HP;
        unsigned short MP;
        unsigned short battleAction;
        unsigned short action;
        unsigned char level;
        unsigned char learn;
        unsigned char dmmy0;
        unsigned char dmmy1;
    };
    struct WarriorData {
        unsigned int exp;
        unsigned short strength;
        unsigned short agility;
        unsigned short intelligence;
        unsigned short luck;
        unsigned short HP;
        unsigned short MP;
        unsigned short battleAction;
        unsigned short action;
        unsigned char level;
        unsigned char learn;
        unsigned char dmmy0;
        unsigned char dmmy1;
    };
    struct PrincessData {
        unsigned int exp;
        unsigned short strength;
        unsigned short agility;
        unsigned short intelligence;
        unsigned short luck;
        unsigned short HP;
        unsigned short MP;
        unsigned short battleAction;
        unsigned short action;
        unsigned char level;
        unsigned char learn;
        unsigned char dmmy0;
        unsigned char dmmy1;
    };
    struct PriestData {
        unsigned int exp;
        unsigned short strength;
        unsigned short agility;
        unsigned short intelligence;
        unsigned short luck;
        unsigned short HP;
        unsigned short MP;
        unsigned short battleAction;
        unsigned short action;
        unsigned char level;
        unsigned char learn;
        unsigned char dmmy0;
        unsigned char dmmy1;
    };
    struct MageData {
        unsigned int exp;
        unsigned short strength;
        unsigned short agility;
        unsigned short intelligence;
        unsigned short luck;
        unsigned short HP;
        unsigned short MP;
        unsigned short battleAction;
        unsigned short action;
        unsigned char level;
        unsigned char learn;
        unsigned char dmmy0;
        unsigned char dmmy1;
    };
    struct TraderData {
        unsigned int exp;
        unsigned short strength;
        unsigned short agility;
        unsigned short intelligence;
        unsigned short luck;
        unsigned short HP;
        unsigned short MP;
        unsigned short battleAction;
        unsigned short action;
        unsigned char level;
        unsigned char learn;
        unsigned char dmmy0;
        unsigned char dmmy1;
    };
    struct WarlockData {
        unsigned int exp;
        unsigned short strength;
        unsigned short agility;
        unsigned short intelligence;
        unsigned short luck;
        unsigned short HP;
        unsigned short MP;
        unsigned short battleAction;
        unsigned short action;
        unsigned char level;
        unsigned char learn;
        unsigned char dmmy0;
        unsigned char dmmy1;
    };
    struct DancerData {
        unsigned int exp;
        unsigned short strength;
        unsigned short agility;
        unsigned short intelligence;
        unsigned short luck;
        unsigned short HP;
        unsigned short MP;
        unsigned short battleAction;
        unsigned short action;
        unsigned char level;
        unsigned char learn;
        unsigned char dmmy0;
        unsigned char dmmy1;
    };
    struct PissaroData {
        unsigned int exp;
        unsigned short strength;
        unsigned short agility;
        unsigned short intelligence;
        unsigned short luck;
        unsigned short HP;
        unsigned short MP;
        unsigned short battleAction;
        unsigned short action;
        unsigned char level;
        unsigned char learn;
        unsigned char dmmy0;
        unsigned char dmmy1;
    };
    struct ItemData {
        unsigned int casino;
        unsigned int menuMes;
        unsigned short BuyPrice;
        unsigned short SellPrice;
        unsigned short samall;
        unsigned short effect;
        unsigned short action;
        unsigned short battleAction;
        unsigned short typeSort;
        unsigned short nameSort;
        unsigned char display;
        unsigned char type;
        unsigned char change;
        unsigned char raiseUP;
        char byte_1;
        char byte_2;
        char byte_3;
        unsigned char dmmy0;

        static const unsigned int size_;
        static const unsigned int ID_;
    };
}

extern const param::ExcelFile<unsigned char, 9> encountYamiTileTable;                 // EncountYamiTile tiles (3x3)
extern const param::ExcelFile<unsigned char, 16> encountGotTileTable;                // EncountGotTile tiles (4x4)
extern const param::ExcelFile<param::FloorFog, 2> floorFogTable;
extern const param::ExcelFile<param::EncountFormNum, 18> encountFormNumTable;
extern const param::ExcelFile<param::SurechigaiTenant, 23> surechigaiTenantTable;
extern const param::ExcelFile<param::SurechigaiObjectData, 50> surechigaiObjectDataTable;
extern const param::ExcelFile<param::CLUTCode, 15> clutCodeTable;
extern const param::ExcelFile<param::EncountFormationID, 9> encountFormationIdTable;
extern const param::ExcelFile<unsigned char, 256> encountTile3Table;               // EncountTile3 tiles (16x16)
extern const param::ExcelFile<unsigned char, 256> encountTile1Table;               // EncountTile1 tiles (16x16)
extern const param::ExcelFile<unsigned char, 256> encountSeaTileTable;               // EncountSeaTile tiles (16x16)
extern const param::ExcelFile<unsigned char, 256> encountTile2Table;               // EncountTile2 tiles (16x16)
extern const param::ExcelFile<param::FloorBackColor, 15> floorBackColorTable;
extern const param::ExcelFile<param::MapChurch, 26> mapChurchTable;
extern const param::ExcelFile<param::VehicleData, 29> vehicleDataTable;
extern const param::ExcelFile<param::CharaVoice, 217> charaVoiceTable;
extern const param::ExcelFile<param::BookData, 210> bookDataTable;
extern const param::ExcelFile<param::AbreactTurn, 52> abreactTurnTable;
extern const param::ExcelFile<param::CharInitData, 27> charInitDataTable;
extern const param::ExcelFile<param::HeroData, 100> heroDataTable;
extern const param::ExcelFile<param::WarriorData, 100> warriorDataTable;
extern const param::ExcelFile<param::PrincessData, 100> princessDataTable;
extern const param::ExcelFile<param::PriestData, 100> priestDataTable;
extern const param::ExcelFile<param::MageData, 100> mageDataTable;
extern const param::ExcelFile<param::TraderData, 100> traderDataTable;
extern const param::ExcelFile<param::WarlockData, 100> warlockDataTable;
extern const param::ExcelFile<param::DancerData, 100> dancerDataTable;
extern const param::ExcelFile<param::PissaroData, 100> pissaroDataTable;
extern const param::ExcelFile<param::MonsterMap, 201> monsterMapTable;
extern const param::ExcelFile<param::EffectColorParam, 162> effectColorTable;
extern const param::ExcelFile<param::EncountSpecial, 104> encountSpecialTable;
extern const param::ExcelFile<param::ShopDataFirst, 988> shopDataFirstTable;
extern const param::ExcelFile<param::ItemData, 162> itemDataTable;
extern const param::ExcelFile<param::SplitMsg, 39> splitMsgTable;
extern const param::ExcelFile<param::ColorCorrect, 540> colorCorrectTable;
extern const param::ExcelFile<param::ShopDataSecond, 1872> shopDataSecondTable;
extern const param::ExcelFile<param::EncountData, 256> encountDataTable;
extern const param::ExcelFile<param::MonsterData, 315> monsterDataTable;
extern const param::ExcelFile<param::FloorParam, 544> floorParamTable;
extern const param::ExcelFile<param::ActionParam, 595> actionParamTable;
