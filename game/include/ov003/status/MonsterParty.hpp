#pragma once
#include "globaldefs.h"
#include "GameInfo.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/status/CharacterStatus.hpp"
#include "ov003/status/MonsterStatus.hpp"

namespace status{
    struct MonsterParty {
        static int sortIndex_[4]; //data_ov003_0213c608
        static int sortCount_[4];//data_ov003_0216638c
        MonsterStatus monster_[12];
        int monsterCount_;
        int dropItem_;
        int dropItemMonster_;
        MonsterParty();
        ~MonsterParty();
        virtual int add(int monsterGroup, int monsterIndex,int flag);
        virtual void del(int ctrl);
        virtual void clear();
        int getCount();
        int getBattleCount();
        int getMonsterCountDeadOrAlive(int monsterIndex);
        int getMonsterCountInGroup(int groupIndex);
        int getMonsterCountInGroupExist(int groupIndex);
        status::MonsterStatus* getMonsterInGroup(int groupIndex, int index);
        int getMonsterIndexInGroup(int groupIndex);
        int getMonsterGroup(int index);
        int getMonsterIndex(int index);
        status::MonsterStatus* getMonsterStatus(int index);
        int getCtrlId(int index);
        void setCtrlId(int index, int ctrlId);
        status::MonsterStatus* getMonsterStatusWithCtrlId(int ctrlId);
        int getGroupCount();
        int getMonsterGroupForMonsterIndex(int monsterIndex);
        int getMonsterCallType();
        int getMonsterCallIndex();
        static void initializeSortIndex();
        static int getSortIndexInGroup(int index);
        static int isSortIndexInGroup(int index);
        int getSortIndex(int monsterGroup, int monsterIndex);
        static int isSortIndex(int sortIndex, int monsterIndex);
        void checkDropItem(int ctrl);
        void setDropItem(int itemIndex_);
        int getDropItem();
        void setDropItemMonster(int monsterIndex);
        int getDropItemMonster();
        int getSpecialDropItem();
        

    };
}

struct GroupIndices {
    int v[4];
};
struct MonsterFormation {
    int v[4];
};

struct SpecialItem
{
    unsigned char item;                 // +0
    unsigned char rate;                 // +1
};

extern SpecialItem specialItem[26];//data_ov003_0213c618
extern const GroupIndices monsterCallTypeTable;
extern const MonsterFormation monsterFormationTable;

#include "ov003/status/MonsterPartyWithDraw.hpp"
