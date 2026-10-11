#pragma ipa file
#include "main/debug/UnkDebugBattleInfo.hpp"
#include "main/data/DataObject.hpp"
#include "main/menu/UnkMenuDisplays.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "ov003/btl/BattleMonsterMask.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"
#include "ov003/status/MonsterStatus.hpp"

UnkDebugBattleInfo data_020f2008;

THUMB void UnkDebugBattleInfo::initialize()
{
    type_ = 0;
    index_ = 0;
    for (int i = 0; i < g_monster.getCount(); i++) {
        status::HaveStatusInfo* info = &g_monster.getMonsterStatus(i)->haveStatusInfo_;
        info->getHp();
        info->getHpMax();
        info->getMp();
        info->getMpMax();
        info->getStrength(0);
        info->getAgility(0);
        info->getProtection(0);
        info->getWisdom(0);
        info->getLuck(0);
        info->getAttack(0);
        info->getDefence(0);
        BattleMonsterMask::getSingleton()->getMonsterTouchRect(i);
    }
}

THUMB void UnkDebugBattleInfo::draw()
{
    if (showHeap_ != 0) {
        unkfunc_0203d224(1, 1);
    }
    if (type_ == 0) {
        return;
    }
    if (type_ == 1 && index_ >= status::g_Party.getCount()) {
        return;
    }
    if (type_ == 2 && index_ >= g_monster.getCount()) {
        return;
    }
    int index = index_;
    if (type_ == 1) {
        int command = status::g_Party.getPlayerStatus(index)->haveBattleStatus_.actionIndex_;
        unkfunc_0202c17c(13, 1, "PLAYER NO      %3d", index);
        unkfunc_0202c17c(13, 2, "                  ");
        unkfunc_0202c17c(13, 3, "COMMAND        %3d", command);
        unkfunc_0203d264(13, 5, &status::g_Party.getPlayerStatus(index)->haveStatusInfo_);
    }
    if (type_ == 2) {
        int command = g_monster.getMonsterStatus(index)->haveBattleStatus_.actionIndex_;
        unkfunc_0202c17c(13, 1, "MONSTER NO     %3d", index);
        unkfunc_0202c17c(13, 2, "GROUP NO       %3d", g_monster.getMonsterGroup(index));
        unkfunc_0202c17c(13, 3, "COMMAND        %3d", command);
        unkfunc_0203d264(13, 5, &g_monster.getMonsterStatus(index)->haveStatusInfo_);
    }
    if (type_ == 2) {
        int* rect = BattleMonsterMask::getSingleton()->getMonsterTouchRect(index);
        if (blink_ > 0) {
            if (blink_ & 1) {
                data_020f530c.arrow_.unkfunc_02052658(rect[1] + (rect[3] - rect[1]) / 2, rect[2], 2, 0);
            }
            blink_--;
        }
    }
    unkfunc_0202c20c(13, 25, 18, 19);
    unkfunc_0202c20c(13, 1, 18, 15);
}

THUMB void UnkDebugBattleInfo::unkfunc_0203d224(int x, int y)
{
    unkfunc_0202c17c(x, y, "HEAP");
    unkfunc_0202c17c(x, y + 1, "%3dK", unkfunc_0207f86c(&data_0211a60c) >> 10);
    unkfunc_0202c20c(x, y, 4, 2);
}

THUMB void UnkDebugBattleInfo::unkfunc_0203d264(int x, int y, status::HaveStatusInfo* info)
{
    int hp = info->getHp();
    int hpMax = info->getHpMax();
    int mp = info->getMp();
    int mpMax = info->getMpMax();
    int strength = info->getStrength(0);
    int agility = info->getAgility(0);
    int protection = info->getProtection(0);
    int wisdom = info->getWisdom(0);
    int luck = info->getLuck(0);
    int attack = info->getAttack(0);
    int defence = info->getDefence(0);
    unkfunc_0202c17c(x, y, "HP/HPMAX %4d/%4d", hp, hpMax);
    unkfunc_0202c17c(x, y + 1, "MP/MPMAX %4d/%4d", mp, mpMax);
    unkfunc_0202c17c(x, y + 2, "                  ");
    unkfunc_0202c17c(x, y + 3, "STRENGTH       %3d", strength);
    unkfunc_0202c17c(x, y + 4, "AGILITY        %3d", agility);
    unkfunc_0202c17c(x, y + 5, "PROTECTION     %3d", protection);
    unkfunc_0202c17c(x, y + 6, "WISDOM         %3d", wisdom);
    unkfunc_0202c17c(x, y + 7, "LUCK           %3d", luck);
    unkfunc_0202c17c(x, y + 8, "                  ");
    unkfunc_0202c17c(x, y + 9, "ATTACK         %3d", attack);
    unkfunc_0202c17c(x, y + 10, "DEFENCE        %3d", defence);
    char mark[2] = { 'X', 'O' };
    status::StatusChange& change = info->statusChange_;
    unkfunc_0202c17c(x, 25, "ASTORON          %c", mark[change.isEnable(status::StatusChange::StatusAstoron) ? 1 : 0]);
    unkfunc_0202c17c(x, 26, "SPAZZ            %c", mark[change.isEnable(status::StatusChange::StatusSpazz) ? 1 : 0]);
    unkfunc_0202c17c(x, 27, "SLEEP            %c", mark[change.isEnable(status::StatusChange::StatusSleep) ? 1 : 0]);
    unkfunc_0202c17c(x, 28, "MANUSA           %c", mark[change.isEnable(status::StatusChange::StatusManusa) ? 1 : 0]);
    unkfunc_0202c17c(x, 29, "BAIKIRUTO        %c", mark[change.isEnable(status::StatusChange::StatusBaikiruto) ? 1 : 0]);
    unkfunc_0202c17c(x, 30, "FUBAHA           %c", mark[change.isEnable(status::StatusChange::StatusFubaha) ? 1 : 0]);
    unkfunc_0202c17c(x, 31, "MAHOKANTA        %c", mark[change.isEnable(status::StatusChange::StatusMahokanta) ? 1 : 0]);
    unkfunc_0202c17c(x, 32, "MOSYASU          %c", mark[change.isEnable(status::StatusChange::StatusMosyasu) ? 1 : 0]);
    unkfunc_0202c17c(x, 33, "POWER SAVE       %c", mark[change.isEnable(status::StatusChange::StatusPowerSave) ? 1 : 0]);
    unkfunc_0202c17c(x, 34, "MAHOTON          %c", mark[change.isEnable(status::StatusChange::StatusMahoton) ? 1 : 0]);
    unkfunc_0202c17c(x, 35, "MAHOSUTE         %c", mark[change.isEnable(status::StatusChange::StatusMahosute) ? 1 : 0]);
    unkfunc_0202c17c(x, 36, "DRAGORAM         %c", mark[change.isEnable(status::StatusChange::StatusDragoram) ? 1 : 0]);
    unkfunc_0202c17c(x, 37, "CONFUSION        %c", mark[change.isEnable(status::StatusChange::StatusConfusion) ? 1 : 0]);
    unkfunc_0202c17c(x, 38, "PATH 1           %c", mark[change.isEnable(status::StatusChange::StatusPath1) ? 1 : 0]);
    unkfunc_0202c17c(x, 39, "POISON           %c", mark[change.isEnable(status::StatusChange::StatusPoison) ? 1 : 0]);
    unkfunc_0202c17c(x, 40, "DEFENCE CHANGE   %c", mark[change.isEnable(status::StatusChange::StatusDefenceChange) ? 1 : 0]);
    unkfunc_0202c17c(x, 41, "AGILITY CHANGE   %c", mark[change.isEnable(status::StatusChange::StatusAgilityChange) ? 1 : 0]);
    unkfunc_0202c17c(x, 42, "DEFENCE          %c", mark[change.isEnable(status::StatusChange::StatusDefence) ? 1 : 0]);
    unkfunc_0202c17c(x, 43, "TIME STOP        %c", mark[change.isEnable(status::StatusChange::StatusTimeStop) ? 1 : 0]);
    unkfunc_0202c17c(x, 44, "FIZZLE ZONE      %c", mark[change.isEnable(status::StatusChange::StatusFizzleZone) ? 1 : 0]);
}
