#pragma once

#define WORLD_TIME_MORNING  0xCC0
#define WORLD_TIME_DAYTIME  0x80
#define WORLD_TIME_EVENING  0x7C0
#define WORLD_TIME_NIGHT    0x8C0

enum PLAYER_COMMAND {
    PUSH_NONE=0,
    PUSH_BENRI_BUTTON=1,
    PUSH_BALLOON_GETOUT_BUTTON=2,
    PUSH_BALLOON_SEARCH_BUTTON=3,
    START_SEARCH_COMMAND=4,
    END_SEARCH_COMMAND=5,
    START_TALK_COMMAND=6,
    END_TALK_COMMAND=7,
    START_RIDE_BALLOON_COMMAND=8,
    END_RIDE_BALLOON_COMMAND=9,
    START_IDO_LINK_COMMAND=10,
    END_IDO_LINK_COMMAND=11
};

enum PartyModeState {
    PARTY_MODE_DISABLED = 0,
    PARTY_MODE_ENABLED  = 1,
};

enum BashaPosition {
    BASHA_OUTSIDE,   
    BASHA_INSIDE,   
};


enum BoolResult {
    RESULT_FALSE = 0,
    RESULT_TRUE  = 1,
};

enum CarriagePosition {
    OUTSIDE_CARRIAGE, 
    INSIDE_CARRIAGE
};

enum Sex {
    SEX_MALE = 0x0,
    SEX_FEMALE = 0x1,
    SEX_NONE = 0x2,
};
enum StageDamage {
    Barrier = 0,
    Poison = 1,
    None = 2
};

enum ActionDefenceKind {
    ACT_DEF_ALL = 0,
    ACT_DEF_IO = 1,
    ACT_DEF_GIRA = 2,
    ACT_DEF_MERA = 3,
    ACT_DEF_DEIN = 4,
    ACT_DEF_BAGI = 5,
    ACT_DEF_HYADO = 6,
    ACT_DEF_MANUSA = 7,
    ACT_DEF_RARIHO = 8,
    ACT_DEF_ZAKI = 9,
    ACT_DEF_NIFRAM = 10,
    ACT_DEF_MAHOTORA = 11,
    ACT_DEF_MAHOTONE = 12,
    ACT_DEF_MEDAPANI = 13,
    ACT_DEF_RUKANI = 14,
    ACT_DEF_MEGANTE = 15,
    ACT_DEF_DRAGON = 16,
    ACT_DEF_METAL = 17,
    ACT_DEF_ZONBI = 18,
    ACT_DEF_AIR = 19,
    ACT_DEF_SLIME = 20,
    ACT_DEF_POISON = 21,
    ACT_DEF_REST = 22,
    ACT_DEF_ANTIDANCE = 23,
    ACT_DEF_FIRE = 24,
    ACT_DEF_BLIZZARD = 25,
    ACT_DEF_CLAP = 26,
    ACT_DEF_SPAZZ = 27,
    ACT_DEF_ARMY = 28
};
enum ActionDefenceType {
    ACT_DEF_A = 0,
    ACT_DEF_B = 1,
    ACT_DEF_C = 2,
    ACT_DEF_D = 3,
    ACT_DEF_E = 4,
    ACT_DEF_F = 5,
    ACT_DEF_G = 6,
    ACT_DEF_H = 7
};


enum CommandType {
    COMMAND_GANGANIKOUZE = 0,
    COMMAND_BACCHIRIGANBARE = 1,
    COMMAND_ORENIMAKASERO = 2,
    COMMAND_JYUMONTUKAUNA = 3,
    COMMAND_INOCHIDAIZINI = 4,
    COMMAND_MEIREISASERO = 5,
    COMMAND_DEBUG = 6,
    COMMAND_NONE = 7
};

enum EncountPart {
    EncountDungeon = 0,
    EncountField = 1
};

enum LandType {
    Floor = 0,
    Sea = 1,
    Desert = 2,
    Mountain = 3,
    Pond = 4,
    Bush = 5,
    Field = 6,
    Forest = 7
};

enum JobType {
    JOB_NONE = 0,
    JOB_HERO = 1,
    JOB_WARRIOR = 2,
    JOB_PRINCESS = 3,
    JOB_PRIEST = 4,
    JOB_MAGE = 5,
    JOB_TRADER = 6,
    JOB_WARLOCK = 7,
    JOB_DANCER = 8,
    JOB_HOIMIN = 9,
    JOB_ENFORCER = 10,
    JOB_BARD = 11,
    JOB_ALCHEMIST = 12,
    JOB_INN_SON = 13,
    JOB_TINKER = 14,
    JOB_TENKU = 15,
    JOB_DRAGON_SON = 16,
    JOB_PISSARO = 17
};

enum Language {
    Japanese = 0x0,
    English = 0x1,
    French = 0x2,
    German = 0x3,
    Italian = 0x4,
    Spanish = 0x5,
};

enum TIME_ZONE
{
    TIME_ZONE_NONE = 0x0,
    TIME_ZONE_MORNING = 0x1,
    TIME_ZONE_DAYTIME = 0x2,
    TIME_ZONE_EVENING = 0x3,
    TIME_ZONE_NIGHT = 0x4,
};
enum CharacterType {
    PLAYER = 0x0,
    MONSTER = 0x1,
};

namespace btl {
    enum FirstAttack {
        FirstAttackNone = 0x0,
        FirstAttackPlayer = 0x1,
        FirstAttackMonster = 0x2,
    };
}

enum EncountType {
    TYPE_NONE=-1,
    TYPE_A=0,
    TYPE_B=1,
    TYPE_C=2,
    TYPE_D=3,
    TYPE_E=4,
    TYPE_F=5,
    TYPE_G=6,
    TYPE_H=7,
    TYPE_I=8,
    TYPE_J=9,
    TYPE_K=10,
    TYPE_L=11,
    TYPE_M=12,
    TYPE_N=13
};



enum ItemType
{
    ITEM_WEAPON = 0x0,
    ITEM_ARMOR = 0x1,
    ITEM_SHIELD = 0x2,
    ITEM_HELMET = 0x3,
    ITEM_ACCESSORY = 0x4,
    ITEM_ITEM = 0x5,
    ITEM_SEED = 0x6,
    ITEM_KEY = 0x7,
    ITEM_CASINO = 0x8,
    ITEM_OTHER = 0x9,
    ITEM_IMPORTANT = 0xA,
    ITEM_DEBUG = 0xB,
};
enum EquipType {
    Attack = 0,
    Defence = 1,
    Strength = 2,
    Agility = 3,
    Luck = 4,
    Wisdom = 5,
};

enum eMessageWindow 
{
    TEST_MESSAGE_WINDOW = 0x0,
    TALK_MESSAGE_WINDOW = 0x1,
    MENU_MESSAGE_WINDOW = 0x2,
    ENCOUNT_MESSAGE_WINDOW = 0x3,
    BATTLE_MESSAGE_WINDOW = 0x4,
    BATTLE_SHOP_MESSAGE_WINDOW = 0x5,
};