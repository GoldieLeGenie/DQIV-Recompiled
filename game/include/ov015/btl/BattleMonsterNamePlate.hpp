#pragma once
#include "globaldefs.h"

// name plates of the monster groups shown above the battle menu
struct BattleMonsterNamePlate {
    static const short BASE_LINE = 74;
    static const short PLATE_SIZE = 20;
    static const short BLANK_SIZE = 8;
    static const short MAX_STRING_SIZE = 256;
    static const short MAX_DISPLAY_SIZE_X = 256;
    static const short MAX_DISPLAY_SIZE_Y = 128;
    static const short PLATE_LENG = 4;
    static const short CURSOR_BLANK = 8;
    static const int MonsterGroupMax = 4;

    struct Monster_DATA {
        int name;                               // 0x00
        short num;                              // 0x04
        short drawID;                           // 0x06
        short group;                            // 0x08
        short center;                           // 0x0A
        short leng;                             // 0x0C
        short height;                           // 0x0E
    };

    int addCount_;                              // 0x00
    Monster_DATA monsterData_[MonsterGroupMax]; // 0x04
    short sortList_[MonsterGroupMax];           // 0x44

    static BattleMonsterNamePlate& getSingleton();
    void init();
    void setMonster();
    int seekMonster(int group);
    void setMonsterParameter(int group, int monsterCount);
    short getGroupCenter(int group);
    void maxPriority(int group);
    void sortPosition();
    void makeSortList();
    bool changeHeight(Monster_DATA& m_from, Monster_DATA& m_to);
    void adjustPlateCenter(Monster_DATA& monster, bool pumpUp);
    void adjustPosition(int no);
};

extern BattleMonsterNamePlate gBattleMonsterNamePlate;
