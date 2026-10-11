#include "ov015/btl/BattleMonsterNamePlate.hpp"
#include "ov003/btl/BattleMonsterMask.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"
#include "main/status/HaveEquipment.hpp"
#include "main/menu/MenuBase.hpp"
#include "main/text/TextAPI.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/menu/UnkMenuPartsDraw.hpp"

BattleMonsterNamePlate gBattleMonsterNamePlate;

THUMB BattleMonsterNamePlate& BattleMonsterNamePlate::getSingleton()
{
    return gBattleMonsterNamePlate;
}

THUMB void BattleMonsterNamePlate::init()
{
    addCount_ = 0;
    dss::memset(monsterData_, 0, sizeof(monsterData_));
    dss::memset(sortList_, 0, sizeof(sortList_));
}

THUMB void BattleMonsterNamePlate::setMonster()
{
    addCount_ = 0;
    for (int i = 0; i < MonsterGroupMax; i++) {
        monsterData_[i].name = -1;
        monsterData_[i].num = -1;
        monsterData_[i].center = -1;
        monsterData_[i].leng = -1;
        monsterData_[i].height = 0;
        monsterData_[i].drawID = -1;
    }
    int groupList[MonsterGroupMax] = {0, 0, 0, 0};
    for (int i = 0; i < g_monster.getCount(); i++) {
        if (g_monster.getMonsterStatus(i)->isBattleEnable()) {
            groupList[g_monster.getMonsterStatus(i)->characterGroup_]++;
        }
    }
    int groupCount = 0;
    for (int i = 0; i < MonsterGroupMax; i++) {
        if (groupList[i] > 0) {
            groupCount++;
        }
    }
    for (int group = 0; addCount_ < groupCount; group++) {
        int debug = 0;
        for (int monster = 0; monster < g_monster.getCount(); monster++) {
            if (group == g_monster.getMonsterGroup(monster) && g_monster.getMonsterStatus(monster)->isBattleEnable()) {
                debug++;
            }
        }
        if (debug > 0) {
            int monsterCount = seekMonster(group);
            if (monsterCount != -1) {
                setMonsterParameter(group, monsterCount);
                addCount_++;
            }
        }
    }
    sortPosition();
}

THUMB int BattleMonsterNamePlate::seekMonster(int group)
{
    int monsterCount = g_monster.getCount();
    for (int i = 0; i < monsterCount; i++) {
        if (group == g_monster.getMonsterStatus(i)->characterGroup_ && g_monster.getMonsterStatus(i)->isBattleEnable()) {
            return i;
        }
    }
    return -1;
}

THUMB void BattleMonsterNamePlate::setMonsterParameter(int group, int monsterCount)
{
    char text[MAX_STRING_SIZE];
    int* rect = BattleMonsterMask::getSingleton()->getMonsterTouchRect(monsterCount);
    monsterData_[addCount_].name = g_monster.getMonsterIndex(monsterCount);
    monsterData_[addCount_].num = 0;
    monsterData_[addCount_].group = rect[0];
    for (int i = 0; i < g_monster.getCount(); i++) {
        if (rect[0] == g_monster.getMonsterGroup(i) && g_monster.getMonsterStatus(i)->isBattleEnable()) {
            monsterData_[addCount_].num++;
        }
    }
    if (g_monster.getMonsterStatus(monsterCount)->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusMosyasu)) {
        TextAPI::getPlayerNamePlateTextImitation(text, g_monster.getMonsterStatus(monsterCount)->mosyasIndex_, monsterData_[addCount_].num);
    } else {
        TextAPI::getMonsterNamePlateTextImitation(text, monsterData_[addCount_].name, monsterData_[addCount_].num);
    }
    monsterData_[addCount_].leng = unkfunc_02050e20(addCount_, text);
    monsterData_[addCount_].height = rect[2];
    monsterData_[addCount_].drawID = (addCount_ == 0) ? 1 : 2;
    adjustPosition(addCount_);
    monsterData_[addCount_].center = getGroupCenter(group) + 2;
    adjustPlateCenter(monsterData_[addCount_], true);
}

THUMB short BattleMonsterNamePlate::getGroupCenter(int group)
{
    int* rect;
    int max = 0;
    int min = MAX_DISPLAY_SIZE_X;
    int maxCount = g_monster.getCount();
    for (int i = 0; i < maxCount; i++) {
        if (g_monster.getMonsterStatus(i)->isBattleEnable()) {
            rect = BattleMonsterMask::getSingleton()->getMonsterTouchRect(i);
            if (rect[0] == group) {
                if (rect[3] > max) {
                    max = rect[3];
                }
                if (rect[1] < min) {
                    min = rect[1];
                }
            }
        }
    }
    int leng = max - min;
    return min + (leng >> 1);
}

THUMB void BattleMonsterNamePlate::maxPriority(int group)
{
    monsterData_[group].drawID = 1;
    for (int i = 0; i < addCount_; i++) {
        if (i != group) {
            monsterData_[i].drawID = 2;
        }
    }
}

THUMB void BattleMonsterNamePlate::sortPosition()
{
    makeSortList();
    for (int i = 0; i < addCount_; i++) {
        Monster_DATA& m_from = monsterData_[sortList_[i]];
        int move = true;
        int moreHight = true;
        while (move && moreHight) {
            move = false;
            moreHight = false;
            for (int j = 0; j < i; j++) {
                Monster_DATA& m_to = monsterData_[sortList_[j]];
                if (m_from.height > m_to.height) {
                    moreHight = true;
                } else if (changeHeight(m_from, m_to)) {
                    move = true;
                }
            }
        }
    }
}

THUMB void BattleMonsterNamePlate::makeSortList()
{
    int centerLengList[MonsterGroupMax];
    dss::memset(centerLengList, 0x80, sizeof(centerLengList));
    for (int i = 0; i < addCount_; i++) {
        int centerLeng = status::HaveEquipment::getAbsoluteValue(MAX_DISPLAY_SIZE_Y - monsterData_[i].center);
        int thisNum = i;
        for (int j = 0; j < i; j++) {
            if (centerLengList[j] > centerLeng) {
                int temp = centerLengList[j];
                centerLengList[j] = centerLeng;
                centerLeng = temp;
                int tempNum = sortList_[j];
                sortList_[j] = thisNum;
                thisNum = tempNum;
            }
        }
        centerLengList[i] = centerLeng;
        sortList_[i] = thisNum;
    }
}

THUMB bool BattleMonsterNamePlate::changeHeight(Monster_DATA& m_from, Monster_DATA& m_to)
{
    if (m_from.height == m_to.height) {
        int leng = m_from.center - m_to.center;
        int interval = (m_from.leng >> 1) + (m_to.leng >> 1);
        if (interval > status::HaveEquipment::getAbsoluteValue(leng)) {
            int firstSride;
            if (leng < 0) {
                firstSride = -(m_from.leng >> 3);
            } else {
                firstSride = m_from.leng >> 3;
            }
            m_from.center += firstSride;
            bool pumpUp = false;
            leng = m_from.center - m_to.center;
            if ((interval >> 2) > interval - status::HaveEquipment::getAbsoluteValue(leng)) {
                int sride = interval - status::HaveEquipment::getAbsoluteValue(leng);
                if (leng < 0) {
                    m_from.center -= sride;
                } else {
                    m_from.center += sride;
                }
            } else {
                m_from.height -= PLATE_SIZE;
                pumpUp = true;
            }
            adjustPlateCenter(m_from, pumpUp);
            return true;
        }
    }
    return false;
}

THUMB void BattleMonsterNamePlate::adjustPlateCenter(Monster_DATA& monster, bool pumpUp)
{
    int subCenter = monster.center - (monster.leng >> 1);
    int addCenter = monster.center + (monster.leng >> 1);
    if (subCenter < CURSOR_BLANK) {
        monster.center -= subCenter - CURSOR_BLANK;
        if (!pumpUp) {
            monster.height -= PLATE_SIZE;
        }
    }
    if (addCenter > MAX_DISPLAY_SIZE_X) {
        monster.center -= addCenter - MAX_DISPLAY_SIZE_X;
        if (!pumpUp) {
            monster.height -= PLATE_SIZE;
        }
    }
    if (monster.height < 0) {
        monster.height += PLATE_SIZE;
    }
}

THUMB void BattleMonsterNamePlate::adjustPosition(int no)
{
    int height = monsterData_[no].height;
    if (height < PLATE_SIZE + BLANK_SIZE) {
        monsterData_[no].height = BLANK_SIZE;
        return;
    }
    int rectSize = (height - BLANK_SIZE) / PLATE_SIZE;
    if ((height - BLANK_SIZE) % PLATE_SIZE < PLATE_SIZE / 2) {
        rectSize--;
    }
    monsterData_[no].height = rectSize * PLATE_SIZE + BLANK_SIZE;
}
