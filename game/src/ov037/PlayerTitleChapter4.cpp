#include "ov037/PlayerTitle.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/BattleHistory.hpp"
#include "main/status/GameFlag.hpp"

THUMB int cmn::PlayerTitleChapter4::getPartyTitle()
{
    status::g_Party.setNormalMode();
    int title = 400;
    int index = status::g_Party.getSortIndex(9);
    int index8 = status::g_Party.getSortIndex(8);
    int inMap = false;
    const char* checkName = "mjf1n1";
    char* mapName = g_Stage.getMapName();
    if (dss::strcmp(mapName, checkName) == 0 || dss::strcmp(mapName, "mjf1b1") == 0) {
        inMap = true;
    }
    status::HaveEquipment& haveEquipment = status::g_Party.getPlayerStatus(index)->haveStatusInfo_.haveEquipment_;
    status::HaveEquipment& haveEquipment8 = status::g_Party.getPlayerStatus(index8)->haveStatusInfo_.haveEquipment_;
    int level = status::g_Party.getPlayerStatus(index)->haveStatusInfo_.haveStatus_.level_;
    bool flagc5 = g_AreaFlag.check(0xc5);
    bool flagc6 = g_AreaFlag.check(0xc6);
    float escapeCount = status::g_BattleHistory.getChapterEscapeCount();
    float battleCount = status::g_BattleHistory.getChapterBattleCount();

    if (level <= 3) {
    } else if (level <= 7 && escapeCount / battleCount > 0.25) {
        title += 1;
    } else if (level <= 10 && status::g_BattleHistory.getChapterWipeoutCount() > 9) {
        title += 2;
    } else if (level <= 5 && g_AreaFlag.check(0xc3) == true) {
        title += 3;
    } else if (level <= 5) {
        title += 4;
    } else if (level <= 15 && haveEquipment.getEquipment(ITEM_ARMOR) == 0x36 &&
               haveEquipment8.getEquipment(ITEM_ARMOR) == 0x36) {
        title += 5;
    } else if (level <= 12 && status::g_Party.gold_ < 500) {
        title += 6;
    } else if (level <= 11 && flagc5 == false) {
        title += 7;
    } else if (level <= 13 && flagc5 == false) {
        title += 8;
    } else if (level <= 14 && flagc5 == false) {
        title += 9;
    } else if (flagc5 == true && escapeCount / battleCount > 0.3) {
        title += 10;
    } else if (flagc5 == true && status::g_BattleHistory.getChapterWipeoutCount() > 11) {
        title += 11;
    } else if (level <= 15 && flagc6 == false && status::g_Party.gold_ > 5000) {
        title += 12;
    } else if (inMap == true && (unsigned int)status::g_BattleHistory.getMonsterCount() % 100 > 70) {
        title += 13;
    } else if (inMap == true && (unsigned int)status::g_BattleHistory.getMonsterCount() % 100 < 30) {
        title += 14;
    } else if (inMap == true) {
        title += 15;
    } else if (level <= 15 && flagc5 == true && flagc6 == false) {
        title += 16;
    } else if (level <= 15 && flagc6 == true) {
        title += 17;
    } else if (level >= 16 && flagc6 == false) {
        title += 18;
    } else if (level <= 15) {
        title += 19;
    } else if (level >= 16) {
        title += 20;
    }
    return title;
}
