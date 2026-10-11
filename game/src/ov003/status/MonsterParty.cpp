#include "ov003/status/MonsterParty.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"
#include "main/status/BaseActionStatus.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/dss/Random.hpp"

const MonsterFormation monsterFormationTable = { { -1, -1, -1, -1 } };
const GroupIndices monsterCallTypeTable = { { -1, -1, -1, -1 } };
#pragma explicit_zero_data on
dss::Vector3int g_monsterDrawPos = { 0, 0, 0 };
#pragma explicit_zero_data reset
int status::MonsterParty::sortIndex_[4] = { -1, -1, -1, -1 };
SpecialItem specialItem[26] = {
    { 0x02, 0x40 }, { 0x03, 0x40 }, { 0x05, 0x80 }, { 0x06, 0x80 },
    { 0x07, 0x50 }, { 0x2c, 0x40 }, { 0x2d, 0x40 }, { 0x2e, 0x80 },
    { 0x2f, 0x50 }, { 0x49, 0x40 }, { 0x4a, 0x40 }, { 0x53, 0x40 },
    { 0x55, 0x40 }, { 0x6f, 0x40 }, { 0x70, 0x80 }, { 0x72, 0x40 },
    { 0x01, 0xff }, { 0x01, 0xff }, { 0x01, 0xff }, { 0x01, 0xff },
    { 0x01, 0xff }, { 0x01, 0xff }, { 0x01, 0xff }, { 0x01, 0xff },
    { 0x01, 0xff }, { 0x00, 0x00 },
};

THUMB status::MonsterParty::MonsterParty(){
}

THUMB status::MonsterParty::~MonsterParty(){
    return;
}

THUMB int status::MonsterParty::add(int monsterGroup, int monsterIndex, int flag)
{
    int sortIndex;
    int i;

    sortIndex = getSortIndexInGroup(monsterIndex);

    for (i = 0; i < 12; i++) {
        if (!monster_[i].isEnable()) {
            monster_[i].setup(monsterGroup, monsterIndex, sortIndex);

            monster_[i].arrayIndex_ = i;
            monster_[i].ctrlId_ = i;
            monster_[i].haveStatusInfo_.drawCtrlId_ = i;

            return i;
        }
    }

    return -1;
}

THUMB void status::MonsterParty::del(int ctrl)
{
    if (monster_[ctrl].isEnable()) {
        monster_[ctrl].cleanup();
    }
}

THUMB void status::MonsterParty::clear()
{
    int i;

    for (i = 0; i < 12; i++) {
        if (monster_[i].isEnable()) {
            del(i);          
        }
    }

    dropItem_ = 0;
}

THUMB int status::MonsterParty::getCount() {
    this->monsterCount_ = 0;
    for (int i = 0; i < 12; i++) {
        if (this->monster_[i].isEnable()) {
            this->monsterCount_++;
        }
    }
    return this->monsterCount_;
}

THUMB int status::MonsterParty::getBattleCount() {
    monsterCount_ = 0;
    for (int i = 0; i < 12; i++) {
        if (monster_[i].isBattleEnable()) {
            monsterCount_++;
        }
    }
    return monsterCount_;
}

THUMB int status::MonsterParty::getMonsterCountDeadOrAlive(int monsterIndex) {
    int count = 0;
    for (int i = 0; i < 12; i++) {
        if (this->monster_[i].isBeingEnable() && this->monster_[i].characterIndex_ == monsterIndex) {
            count++;
        }
    }
    return count;
}

THUMB int status::MonsterParty::getMonsterCountInGroup(int groupIndex) {
    int count = 0;
    for (int i = 0; i < 12; i++) {
        if (this->monster_[i].isBattleEnable() && this->monster_[i].characterGroup_ == groupIndex) {
            count++;
        }
    }
    return count;
}

THUMB int status::MonsterParty::getMonsterCountInGroupExist(int groupIndex) {
    int count = 0;
    for (int i = 0; i < 12; i++) {
        if (this->monster_[i].isEnable() && this->monster_[i].characterGroup_ == groupIndex) {
            count++;
        }
    }
    return count;
}

THUMB status::MonsterStatus* status::MonsterParty::getMonsterInGroup(int groupIndex, int index) {
    int count = 0;
    for (int i = 0; i < 12; i++) {
        if (this->monster_[i].isBattleEnable() && this->monster_[i].characterGroup_ == groupIndex) {
            if (count == index) {
                return &this->monster_[i];
            }
            count++;
        }
    }
    return &this->monster_[0];
}

THUMB int status::MonsterParty::getMonsterIndexInGroup(int groupIndex) {
    for (int i = 0; i < 12; i++) {
        if (this->monster_[i].isBattleEnable() && this->monster_[i].characterGroup_ == groupIndex) {
            return this->monster_[i].characterIndex_;
        }
    }
    return -1;
}

THUMB int status::MonsterParty::getMonsterGroup(int index) {
    int count = 0;
    for (int i = 0; i < 12; i++) {
        if (this->monster_[i].isEnable()) {
            if (count == index) {
                return this->monster_[i].characterGroup_;
            }
            count++;
        }
    }
    return -1;
}

THUMB int status::MonsterParty::getMonsterIndex(int index) {
    int count = 0;
    for (int i = 0; i < 12; i++) {
        if (this->monster_[i].isEnable()) {
            if (count == index) {
                return this->monster_[i].characterIndex_;
            }
            count++;
        }
    }
    return -1;
}

THUMB status::MonsterStatus* status::MonsterParty::getMonsterStatus(int index) {
    int count = 0;
    for (int i = 0; i < 12; i++) {
        if (monster_[i].isEnable()) {
            if (count == index) {
                return &monster_[i];
            }
            count++;
        }
    }
    return &monster_[0];
}

THUMB int status::MonsterParty::getCtrlId(int index) {
    int count = 0;
    for (int i = 0; i < 12; i++) {
        if (this->monster_[i].isEnable()) {
            if (count == index) {
                return this->monster_[i].ctrlId_;
            }
            count++;
        }
    }
    return -1;
}

THUMB void status::MonsterParty::setCtrlId(int index, int ctrlId)
{
    monster_[index].ctrlId_ = ctrlId;
    monster_[index].haveStatusInfo_.drawCtrlId_ = ctrlId;
}

THUMB status::MonsterStatus* status::MonsterParty::getMonsterStatusWithCtrlId(int ctrlId) {
    return &this->monster_[ctrlId];
}

THUMB int status::MonsterParty::getGroupCount() {
    int groups[4];
    int *p = (int*)((int)&groups[0]);
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    int i = 0;
    int count = this->getCount();
    if (count > 0) {
        do {
            int group = this->getMonsterGroup(i);
            if (group != -1) {
                groups[group]++;
            }
            i++;
            count = this->getCount();
        } while (i < count);
    }
    int result = 0;
    for (int j = 0; j < 4; j++) {
        if (groups[j] != 0) {
            result++;
        }
    }
    return result;
}

THUMB int status::MonsterParty::getMonsterGroupForMonsterIndex(int monsterIndex) {
    int count = this->getCount();
    for (int i = 0; i < count; i++) {
        if (this->getMonsterIndex(i) == monsterIndex) {
            return this->getMonsterGroup(i);
        }
    }
    return -1;
}


THUMB int status::MonsterParty::getMonsterCallType() {
    GroupIndices indices;
    int counts[4];
    indices = monsterCallTypeTable;
    int *cp = (int*)((int)&counts[0]);
    cp[0] = 0;
    cp[1] = 0;
    cp[2] = 0;
    cp[3] = 0;

    for (int i = 0; i < 4; i++) {
        counts[i] = this->getMonsterCountInGroup(i);
        if (counts[i] != 0) {
            indices.v[i] = this->getMonsterIndexInGroup(i);
        }
    }

    int idx1 = indices.v[1];
    int idx0 = indices.v[0];

    if (idx0 != idx1 && idx1 != -1) {
        return 2;
    }
    if (idx1 != indices.v[2] && indices.v[2] != -1) {
        return 2;
    }
    if (idx1 != indices.v[3] && indices.v[3] != -1) {
        return 2;
    }

    if ((idx0 == idx1 || idx0 == indices.v[2] || idx0 == indices.v[3]) &&
        counts[0] + counts[1] + counts[2] + counts[3] >= 1) {
        return 1;
    }
    if (idx1 == -1 && counts[0] >= 2) {
        return 1;
    }
    return 0;
}



THUMB int status::MonsterParty::getMonsterCallIndex() {
    MonsterFormation indices;
    int counts[4];
    indices = monsterFormationTable;
    int *cp = (int*)((int)&counts[0]);
    cp[0] = 0;
    cp[1] = 0;
    cp[2] = 0;
    cp[3] = 0;

    for (int i = 0; i < 4; i++) {
        counts[i] = this->getMonsterCountInGroup(i);
        if (counts[i] != 0) {
            indices.v[i] = this->getMonsterIndexInGroup(i);
        }
    }

    int idx0 = indices.v[0];
    int idx1 = indices.v[1];

    if (idx0 != idx1 && idx1 != -1) {
        return 0x136;
    }

    if ((idx0 == idx1 || idx0 == indices.v[2] || idx0 == indices.v[3]) &&
        counts[0] + counts[1] + counts[2] + counts[3] >= 1) {
        if (counts[0] != 0) {
            return idx0;
        }
        if (counts[1] != 0) {
            return idx1;
        }
        if (counts[2] != 0) {
            return indices.v[2];
        }
        if (counts[3] != 0) {
            return indices.v[3];
        }
        goto zero;
    }
    if (idx1 == -1 && counts[0] >= 2) {
        return idx0;
    }
    if (idx1 == -1 && counts[0] == 1) {
        return idx0;
    }
zero:
    return 0;
}


THUMB void status::MonsterParty::initializeSortIndex()
{
    int i;

    for (i = 0; i < 4; i++) {
        sortIndex_[i] = -1;
        sortCount_[i] = 0;
    }
}

 
THUMB int status::MonsterParty::getSortIndexInGroup(int index)
{
    int result = 0;
    int i;

    for (i = 0; i < 4; i++) {
        if (index == sortIndex_[i]) {
            result = isSortIndexInGroup(i);
            break;
        }

        if (sortIndex_[i] == -1) {
            sortIndex_[i] = index;
            result = isSortIndexInGroup(i);
            break;
        }
    }

    return result;
}


THUMB int status::MonsterParty::isSortIndexInGroup(int index)
{
    int i;
    int v;

    for (i = 0; i < 16; i++) {
        v = sortCount_[index];

        if (isSortIndex(v, sortIndex_[index]) != 0) {
            sortCount_[index]++;
            if (sortCount_[index] > 15)
                sortCount_[index] = 0;
        } else {
            sortCount_[index]++;
            if (sortCount_[index] > 15)
                sortCount_[index] = 0;
            break;
        }
    }

    return v;
}



THUMB int status::MonsterParty::isSortIndex(int sortIndex, int monsterIndex)
{
    int count;
    int i;

    count = g_monster.getCount();

    for (i = 0; i < count; i++) {
        if (monsterIndex == g_monster.getMonsterStatus(i)->characterIndex_ &&
            sortIndex     == g_monster.getMonsterStatus(i)->sortIndex_) {
            return 1;
        }
    }

    return 0;
}
