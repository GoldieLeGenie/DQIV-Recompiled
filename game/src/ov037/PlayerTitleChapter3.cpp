#include "ov037/PlayerTitle.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/status/BattleHistory.hpp"
#include "main/status/GameFlag.hpp"

THUMB int cmn::PlayerTitleChapter3::getPartyTitle()
{
    status::g_Party.setNormalMode();
    int title = 300;
    int index = status::g_Party.getSortIndex(7);
    int index16 = status::g_Party.getSortIndex(0x10);
    int index17 = status::g_Party.getSortIndex(0x11);
    int index15 = status::g_Party.getSortIndex(0xf);
    int flag9e = false;
    int haveItem8c = false;
    int haveItem8b = false;
    bool flag92;
    bool flag95;
    float escapeCount;
    float battleCount;
    status::HaveStatusInfo* statusInfo = &status::g_Party.getPlayerStatus(index)->haveStatusInfo_;
    int level = statusInfo->haveStatus_.level_;
    int gold = status::g_Party.gold_;
    if (g_AreaFlag.check(0x9e) == true) {
        flag9e = true;
    }
    if (checkHaveItem(*statusInfo, 0x8c) == true || g_AreaFlag.check(0x94) == true) {
        haveItem8c = true;
    }
    if (checkHaveItem(*statusInfo, 0x8b) == true || g_AreaFlag.check(0x91) == true) {
        haveItem8b = true;
    }
    flag92 = g_AreaFlag.check(0x92);
    flag95 = g_AreaFlag.check(0x95);
    escapeCount = status::g_BattleHistory.getChapterEscapeCount();
    battleCount = status::g_BattleHistory.getChapterBattleCount();

    if (g_AreaFlag.check(0x86) == false) {
    } else if (level == 1 && g_AreaFlag.check(0x86) == true && gold > 400) {
        title += 1;
    } else if (level <= 10 && escapeCount / battleCount > 0.25) {
        title += 2;
    } else if (level < 3 && g_AreaFlag.check(0x86) == true) {
        title += 3;
    } else if (g_AreaFlag.check(0x86) == true && flag9e == false) {
        title += 4;
    } else if (level > 6 && g_AreaFlag.check(0x89) == false) {
        title += 5;
    } else if (level < 7 && statusInfo->haveEquipment_.getEquipment(ITEM_WEAPON) == 0x24) {
        title += 6;
    } else if (level < 5 && flag9e == true && g_AreaFlag.check(0x89) == false) {
        title += 7;
    } else if (g_AreaFlag.check(0x89) == false) {
        title += 8;
    } else if (level <= 10 && index15 != -1) {
        title += 9;
    } else if (g_AreaFlag.check(0x89) == true && g_AreaFlag.check(0x8a) == false && g_AreaFlag.check(0x8b) == false) {
        title += 10;
    } else if (g_AreaFlag.check(0x8a) == true && haveItem8b == false) {
        title += 11;
    } else if (haveItem8b == true && flag92 == false && g_AreaFlag.check(0x91) == false) {
        title += 12;
    } else if (flag92 == true && flag95 == false && status::g_BattleHistory.getChapterWipeoutCount() > 8) {
        title += 13;
    } else if (flag92 == true && flag95 == false && gold < 500) {
        title += 14;
    } else if (flag92 == true && flag95 == false && gold > 20000) {
        title += 15;
    } else if (level < 10 && flag92 == true && flag95 == false) {
        title += 16;
    } else if (level < 13 && flag92 == true && flag95 == false) {
        title += 17;
    } else if (flag95 == true && haveItem8c == false) {
        title += 18;
    } else if (level > 12 && flag95 == false) {
        title += 19;
    } else if (level < 9 && flag95 == true) {
        title += 20;
    } else if (level < 10 && flag95 == true && (index16 != -1 || index17 != -1)) {
        title += 21;
    } else if (flag95 == true && gold > 59999) {
        title += 22;
    } else if (flag95 == true && gold > 20000) {
        title += 23;
    } else if (flag95 == true && status::g_Party.casinoCoin_ > 1000 && status::g_Story.getChapterCasinoCoin(2) < 500) {
        title += 24;
    } else if (flag95 == true && gold < 500) {
        title += 25;
    } else if (flag95 == true && status::g_BattleHistory.getChapterWipeoutCount() > 10) {
        title += 26;
    } else if (flag95 == true && escapeCount / battleCount > 0.2) {
        title += 27;
    } else if (flag95 == true && g_AreaFlag.check(0x98) == true && g_AreaFlag.check(0x9c) == false) {
        title += 28;
    } else if (g_AreaFlag.check(0x9c) == true) {
        title += 29;
    } else if (level > 15) {
        title += 30;
    } else if (level < 16) {
        title += 31;
    }
    return title;
}

THUMB bool cmn::PlayerTitleChapter3::checkHaveItem(status::HaveStatusInfo& statusInfo, int itemIndex)
{
    if (statusInfo.haveItem_.isItem(itemIndex) == true || status::g_Party.haveItemSack_.isItem(itemIndex) == true) {
        return true;
    }
    return false;
}
