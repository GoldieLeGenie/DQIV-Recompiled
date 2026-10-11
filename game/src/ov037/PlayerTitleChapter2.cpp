#include "ov037/PlayerTitle.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/BattleHistory.hpp"
#include "main/status/GameFlag.hpp"

THUMB int cmn::PlayerTitleChapter2::getPartyTitle()
{
    int title = 200;
    int level = status::g_Party.getPlayerStatus(status::g_Party.getSortIndex(4))->haveStatusInfo_.haveStatus_.level_;
    int gold = status::g_Party.gold_;
    float escapeCount = status::g_BattleHistory.getChapterEscapeCount();
    float battleCount = status::g_BattleHistory.getChapterBattleCount();
    int monsterCount = status::g_BattleHistory.getMonsterCount();

    if (level <= 3) {
    } else if (level <= 10 && escapeCount / battleCount > 0.25) {
        title += 1;
    } else if (level <= 4) {
        title += 2;
    } else if (level <= 6 && g_AreaFlag.check(0x57) == true) {
        title += 3;
    } else if (level <= 7 && g_AreaFlag.check(0x5a) == false) {
        title += 4;
    } else if (level <= 7 && g_AreaFlag.check(6) == true && g_AreaFlag.check(0x67) == false) {
        title += 5;
    } else if (level <= 8 && g_AreaFlag.check(0x57) == true) {
        title += 6;
    } else if (level <= 9 && monsterCount % 100 < 50) {
        title += 7;
    } else if (level <= 9) {
        title += 8;
    } else if (level <= 15 && gold < 300) {
        title += 9;
    } else if (level <= 10 && g_AreaFlag.check(0x65) == true) {
        title += 10;
    } else if (level <= 14 && g_AreaFlag.check(0x65) == false && monsterCount % 100 < 30) {
        title += 11;
    } else if (level <= 14 && g_AreaFlag.check(0x65) == false && monsterCount % 100 < 60) {
        title += 12;
    } else if (level <= 14 && g_AreaFlag.check(0x65) == false) {
        title += 13;
    } else if (g_AreaFlag.check(6) == true && status::g_BattleHistory.getChapterWipeoutCount() > 10) {
        title += 14;
    } else if (level <= 14 && g_AreaFlag.check(6) == true && g_AreaFlag.check(0x67) == false) {
        title += 15;
    } else if (g_AreaFlag.check(6) == true && g_AreaFlag.check(0x67) == false) {
        title += 16;
    } else if (level <= 16 && gold > 1000) {
        title += 17;
    } else if (level <= 17 && g_AreaFlag.check(0x67) == true) {
        title += 18;
    } else if (level <= 17) {
        title += 19;
    } else if (level >= 18) {
        title += 20;
    }
    return title;
}
