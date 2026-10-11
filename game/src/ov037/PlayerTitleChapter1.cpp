#include "ov037/PlayerTitle.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/BattleHistory.hpp"
#include "main/status/GameFlag.hpp"

THUMB int cmn::PlayerTitleChapter1::getPartyTitle()
{
    int title = 100;
    int index = status::g_Party.getSortIndex(3);
    if (index == -1) {
        index = 0;
    }
    int arena = status::g_Party.getSortIndex(12);
    int level = status::g_Party.getPlayerStatus(index)->haveStatusInfo_.haveStatus_.level_;
    int gold = status::g_Party.gold_;
    float escapeCount = status::g_BattleHistory.getChapterEscapeCount();
    float battleCount = status::g_BattleHistory.getChapterBattleCount();

    if (level <= 3) {
    } else if (level <= 4) {
        title += 1;
    } else if (level <= 10 && escapeCount / battleCount > 0.25) {
        title += 2;
    } else if (level <= 10 && status::g_BattleHistory.getChapterWipeoutCount() > 10) {
        title += 3;
    } else if (level <= 5) {
        title += 4;
    } else if (level <= 7 && g_AreaFlag.check(0x3c) == true) {
        title += 5;
    } else if (level <= 10 && gold < 30) {
        title += 6;
    } else if (level <= 10 && gold > 1000) {
        title += 7;
    } else if (level <= 8 && arena != -1) {
        title += 8;
    } else if (level <= 10 && arena != -1) {
        title += 9;
    } else if (level <= 10) {
        title += 10;
    } else if (g_AreaFlag.check(0x3c) == true) {
        title += 11;
    } else if (level >= 13) {
        title += 12;
    } else if (level <= 12) {
        title += 13;
    }
    return title;
}
