#include "ov037/PlayerTitle.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/BattleHistory.hpp"
#include "main/status/BattleResult.hpp"
#include "main/status/GameStatus.hpp"
#include "main/status/GameFlag.hpp"
#include <string.h>

THUMB int cmn::PlayerTitleChapter5::getPartyTitle()
{
    status::g_Party.setNormalMode();
    int title = 500;
    int hero;
    if (status::g_Story.sex_ == SEX_MALE) {
        hero = 1;
    } else {
        hero = 2;
        title += 1;
    }
    int level = status::PartyStatus::getPlayerStatusForPlayerIndex(hero)->haveStatusInfo_.haveStatus_.level_;
    int hour = (int)status::g_Game.getPlayTime() / 216000;
    int gold = status::g_Party.gold_;
    unsigned int victoryCount = (unsigned int)status::g_BattleHistory.getVictoryCount() % 1000;
    unsigned int monsterCount = (unsigned int)status::g_BattleHistory.getMonsterCount() % 1000;
    bool flag14e = g_AreaFlag.check(0x14e);
    bool flag15f = g_AreaFlag.check(0x15f);
    bool flag16d = g_AreaFlag.check(0x16d);
    bool flag172 = g_AreaFlag.check(0x172);
    bool flag1a4 = g_AreaFlag.check(0x1a4);
    bool flag1ab = g_AreaFlag.check(0x1ab);
    bool uragiriNoDoukutu = checkUragiriNoDoukutu();
    bool tenkuSeries = checkTenkuSeries();
    float escapeCount = status::g_BattleHistory.getChapterEscapeCount();
    float battleCount = status::g_BattleHistory.getChapterBattleCount();
    unsigned int wipeoutCount = status::g_BattleHistory.getChapterWipeoutCount();

    if (g_AreaFlag.check(0x12f) == false) {
    } else if (level < 2 && g_AreaFlag.check(0x12f) == true) {
        title += 2;
    } else if (level < 3) {
        title += 4;
    } else if (level < 7 && escapeCount / battleCount > 0.3) {
        title += 6;
    } else if (level < 7 && wipeoutCount > 10) {
        title += 8;
    } else if (level < 7 && wipeoutCount > 4) {
        title += 10;
    } else if (level < 4 && g_AreaFlag.check(0x131) == true) {
        title += 12;
    } else if (level > 6 && g_AreaFlag.check(0x133) == false) {
        title += 16;
    } else if (level > 3 && g_AreaFlag.check(0x132) == false) {
        title += 18;
    } else if (g_AreaFlag.check(0x132) == false) {
        title += 20;
    } else if (g_AreaFlag.check(0x133) == false) {
        title += 22;
    } else if (uragiriNoDoukutu == true && level < 4) {
        title += 24;
    } else if (uragiriNoDoukutu == true && level < 6) {
        title += 26;
    } else if (uragiriNoDoukutu == true && level > 7) {
        title += 28;
    } else if (level < 11 && wipeoutCount > 9) {
        title += 30;
    } else if (level < 11 && escapeCount / battleCount > 0.25) {
        title += 32;
    } else if (level < 11 && gold < 100) {
        title += 34;
    } else if (level < 10 && checkHaveTetunoKinko() == true) {
        title += 36;
    } else if (level < 11 && gold > 2000) {
        title += 38;
    } else if (level < 7) {
        title += 40;
    } else if (level < 9) {
        title += 42;
    } else if (level < 11) {
        title += 44;
    } else if (flag1ab == true && level > 90) {
        title += 48;
    } else if (flag1ab == true && level > 70) {
        title += 50;
    } else if (level > 35 && level < 42 && status::PartyStatus::isInsideCarriageForPlayerIndex(hero) == true) {
        title += 52;
    } else if (level > 30 && status::PartyStatus::isInsideCarriageForPlayerIndex(hero) == false &&
               status::g_Party.getCarriageOutCount() == 1) {
        title += 54;
    } else if (checkShirinishikaretai() == true) {
        title += 56;
    } else if (level > 60 && flag1ab == true) {
        title += 58;
    } else if (level > 50 && status::g_BattleResult.getEncountCount() == 210) {
        title += 60;
    } else if (level > 50 && 210 - status::g_BattleResult.getEncountCount() < 10) {
        title += 62;
    } else if (status::g_Party.haveItemSack_.getCount() >= 113) {
        title += 64;
    } else if (level > 50 && PlayerTitle::checkAllEquip(false) == true) {
        title += 66;
    } else if (flag1ab == true && level > 50) {
        title += 68;
    } else if (flag1ab == true && level > 40) {
        title += 70;
    } else if (level < 35 && status::g_BattleResult.getMonsterCount(0x60) > 20) {
        title += 72;
    } else if (level < 33 && PlayerTitle::getMetalSeriesExp() > 0.4) {
        title += 74;
    } else if (level < 33 && PlayerTitle::getMetalSeriesExp() > 0.3) {
        title += 76;
    } else if (flag1a4 == false && level > 85) {
        title += 78;
    } else if (flag1a4 == false && level > 70) {
        title += 80;
    } else if (flag1a4 == false && level > 50) {
        title += 82;
    } else if (flag1a4 == false && level > 45) {
        title += 84;
    } else if (flag1a4 == false && battleCount / wipeoutCount < 30.0f) {
        title += 86;
    } else if (flag1a4 == false && battleCount / escapeCount < 10.0f) {
        title += 88;
    } else if (flag1a4 == true && level > 45) {
        title += 90;
    } else if (flag1a4 == true && level > 41) {
        title += 92;
    } else if (flag1a4 == true && level > 39) {
        title += 94;
    } else if (flag1a4 == true && level > 37) {
        title += 96;
    } else if (flag1a4 == true && level > 35) {
        title += 98;
    } else if (flag1a4 == true && level > 29) {
        title += 100;
    } else if (tenkuSeries == false && level > 40) {
        title += 102;
    } else if (tenkuSeries == false && level > 37) {
        title += 104;
    } else if (tenkuSeries == true && g_AreaFlag.check(0x179) == false) {
        title += 106;
    } else if (flag172 == true && 210 - status::g_BattleResult.getEncountCount() < 25) {
        title += 108;
    } else if (flag172 == true && tenkuSeries == false) {
        title += 110;
    } else if (flag16d == true && flag172 == false && level < 27) {
        title += 112;
    } else if (flag16d == true && flag172 == false && level < 29) {
        title += 114;
    } else if (flag16d == true && flag172 == false && level < 31) {
        title += 116;
    } else if (flag16d == true && flag172 == false && level > 30) {
        title += 118;
    } else if (flag16d == false && PlayerTitle::checkAllEquip(false) == true) {
        title += 120;
    } else if (flag16d == false && level > 35) {
        title += 122;
    } else if (flag16d == false && gold > 65000) {
        title += 124;
    } else if (flag16d == false && g_AreaFlag.check(0x167) == true && g_AreaFlag.check(0x16a) == false) {
        title += 126;
    } else if (flag16d == false && g_AreaFlag.check(0x16a) == true) {
        title += 128;
    } else if (PlayerTitle::checkUhhunnpinkutai() == true) {
        title += 130;
    } else if (g_AreaFlag.check(0x155) == true && g_AreaFlag.check(0x15f) == false) {
        title += 132;
    } else if (flag14e == true && flag15f == false && level < 26) {
        title += 134;
    } else if (flag14e == true && flag15f == false && level < 28) {
        title += 136;
    } else if (flag14e == true && flag15f == false && level < 30) {
        title += 138;
    } else if (flag14e == true && flag15f == false && level > 29) {
        title += 140;
    } else if (level > 27 && g_AreaFlag.check(0x14d) == false) {
        title += 142;
    } else if (level > 25 && g_AreaFlag.check(0x161) == true) {
        title += 144;
    } else if (level > 24 && g_AreaFlag.check(0x14a) == false) {
        title += 146;
    } else if (level > 20 && g_AreaFlag.check(0x146) == false) {
        title += 148;
    } else if (level > 17 && g_AreaFlag.check(0x13d) == false) {
        title += 150;
    } else if (status::g_BattleResult.getMonsterCount(0x60) > 50) {
        title += 152;
    } else if (PlayerTitle::getMetalSeriesExp() > 0.4) {
        title += 154;
    } else if (PlayerTitle::getMetalSeriesExp() > 0.3) {
        title += 156;
    } else if (status::g_Party.playerMedalCoin_ >= 40) {
        title += 158;
    } else if (checkHaremuNaito() == true) {
        title += 160;
    } else if (checkOyajigonomi() == true) {
        title += 161;
    } else if (status::g_Party.casinoCoin_ > 100000) {
        title += 163;
    } else if (gold < 500) {
        title += 165;
    } else if (gold > 30000) {
        title += 167;
    } else if (victoryCount <= 100) {
        title += 169;
    } else if (monsterCount >= 300 && monsterCount <= 450) {
        title += 171;
    } else if (victoryCount >= 101 && victoryCount <= 200) {
        title += 173;
    } else if (monsterCount >= 700 && monsterCount <= 999) {
        title += 175;
    } else if (victoryCount >= 300 && victoryCount <= 400) {
        title += 177;
    } else if (monsterCount <= 200) {
        title += 179;
    } else if (victoryCount >= 500 && victoryCount <= 600) {
        title += 181;
    } else if (monsterCount >= 550 && monsterCount <= 680) {
        title += 183;
    } else if (victoryCount >= 600 && victoryCount <= 700 && status::g_Story.sex_ == SEX_FEMALE) {
        title += 184;
    } else if (victoryCount >= 700 && victoryCount <= 800) {
        title += 186;
    } else if (victoryCount >= 900 && victoryCount <= 999) {
        title += 188;
    } else {
        title += 190;
    }
    return title;
}

THUMB bool cmn::PlayerTitleChapter5::checkHaveTetunoKinko()
{
    status::g_Party.setPlayerMode();
    bool ret = false;
    int partyCount = status::g_Party.getCount();
    for (int i = 0; i < partyCount; i++) {
        if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveItem_.isItem(0x9e) == true) {
            ret = true;
        }
    }
    if (status::g_Party.haveItemSack_.isItem(0x9e) == true) {
        ret = true;
    }
    status::g_Party.setNormalMode();
    return ret;
}

THUMB bool cmn::PlayerTitleChapter5::checkUragiriNoDoukutu()
{
    status::g_Party.setPlayerMode();
    bool ret = false;
    char mapName[3] = {0, 0, 0};
    const char* checkName = "dh";
    mapName[0] = g_Stage.getMapName()[0];
    mapName[1] = g_Stage.getMapName()[1];
    if (strcmp(mapName, checkName) == 0 && status::g_Party.getCount() == 1) {
        ret = true;
    }
    status::g_Party.setNormalMode();
    return ret;
}

THUMB bool cmn::PlayerTitleChapter5::checkShirinishikaretai()
{
    status::g_Party.setPlayerMode();
    bool ret = false;
    int checkChara[6];
    checkChara[0] = status::g_Party.getSortIndex(4);
    checkChara[1] = status::g_Party.getSortIndex(9);
    checkChara[2] = status::g_Party.getSortIndex(8);
    checkChara[3] = status::g_Party.getSortIndex(3);
    checkChara[4] = status::g_Party.getSortIndex(5);
    checkChara[5] = status::g_Party.getSortIndex(6);
    int level[6] = {0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 6; i++) {
        if (checkChara[i] == -1) {
            status::g_Party.setNormalMode();
            return false;
        }
    }
    for (int i = 0; i < 6; i++) {
        level[i] = status::g_Party.getPlayerStatus(checkChara[i])->haveStatusInfo_.haveStatus_.level_;
    }
    if ((level[0] + level[1] + level[2]) / 3 - (level[3] + level[4] + level[5]) / 3 >= 4) {
        ret = true;
    }
    status::g_Party.setNormalMode();
    return ret;
}

THUMB bool cmn::PlayerTitleChapter5::checkHaremuNaito()
{
    status::g_Party.setPlayerMode();
    if (status::g_Story.sex_ != SEX_MALE) {
        status::g_Party.setNormalMode();
        return false;
    }
    int checkChara[4];
    checkChara[0] = status::g_Party.getSortIndex(1);
    checkChara[1] = status::g_Party.getSortIndex(4);
    checkChara[2] = status::g_Party.getSortIndex(8);
    checkChara[3] = status::g_Party.getSortIndex(9);
    for (int i = 0; i < 4; i++) {
        if (checkChara[i] == -1) {
            status::g_Party.setNormalMode();
            return false;
        }
        if (status::g_Party.isOutsideCarriage(checkChara[i]) == false) {
            status::g_Party.setNormalMode();
            return false;
        }
    }
    status::g_Party.setNormalMode();
    return true;
}

THUMB bool cmn::PlayerTitleChapter5::checkOyajigonomi()
{
    status::g_Party.setBattleMode();
    int checkChara[3];
    checkChara[0] = status::g_Party.getSortIndex(3);
    checkChara[1] = status::g_Party.getSortIndex(5);
    checkChara[2] = status::g_Party.getSortIndex(6);
    for (int i = 0; i < 3; i++) {
        if (checkChara[i] == -1) {
            status::g_Party.setNormalMode();
            return false;
        }
        if (status::g_Party.isOutsideCarriage(checkChara[i]) == false) {
            status::g_Party.setNormalMode();
            return false;
        }
    }
    status::g_Party.setNormalMode();
    return true;
}

THUMB bool cmn::PlayerTitleChapter5::checkTenkuSeries()
{
    int haveTenku[4] = {false, false, false, false};
    int partyCount = status::g_Party.getCount();
    for (int i = 0; i < partyCount; i++) {
        status::HaveItem& haveItem = status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveItem_;
        int itemCount = haveItem.getCount();
        if (haveItem.isItem(0x21) == true || haveItem.isItem(0x22) == true) {
            haveTenku[0] = true;
        }
        if (haveItem.isItem(0x3e) == true) {
            haveTenku[1] = true;
        }
        if (haveItem.isItem(0x50) == true) {
            haveTenku[2] = true;
        }
        if (haveItem.isItem(0x59) == true) {
            haveTenku[3] = true;
        }
    }
    if (status::g_Party.haveItemSack_.isItem(0x21) == true || status::g_Party.haveItemSack_.isItem(0x22) == true) {
        haveTenku[0] = true;
    }
    if (status::g_Party.haveItemSack_.isItem(0x3e) == true) {
        haveTenku[1] = true;
    }
    if (status::g_Party.haveItemSack_.isItem(0x50) == true) {
        haveTenku[2] = true;
    }
    if (status::g_Party.haveItemSack_.isItem(0x59) == true) {
        haveTenku[3] = true;
    }
    for (int i = 0; i < 4; i++) {
        if (haveTenku[i] == false) {
            return false;
        }
    }
    return true;
}
