#include "ov037/PlayerTitle.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/status/BattleHistory.hpp"
#include "main/status/BattleResult.hpp"
#include "main/status/GameStatus.hpp"

THUMB void cmn::PlayerTitle::setPlayerTitle(int clearFlag)
{
    status::g_Party.setNormalMode();
    int offsetTitle = 0;
    if (status::g_Story.sex_ == SEX_FEMALE) {
        offsetTitle = 1;
    }
    switch (clearFlag) {
    case 0: {
        status::g_BattleHistory.historyType_ = status::BattleHistory::RightNow;
        int titleNo = 0;
        switch (status::g_Story.chapter_) {
        case 1:
            titleNo += PlayerTitleChapter1::getPartyTitle();
            break;
        case 2:
            titleNo += PlayerTitleChapter2::getPartyTitle();
            break;
        case 3:
            titleNo += PlayerTitleChapter3::getPartyTitle();
            break;
        case 4:
            titleNo += PlayerTitleChapter4::getPartyTitle();
            break;
        case 5:
        case 6:
            titleNo += PlayerTitleChapter5::getPartyTitle();
            break;
        }
        status::g_BattleHistory.setTitle(titleNo);
        break;
    }
    case 1:
        status::g_BattleHistory.historyType_ = status::BattleHistory::WinDeathPissaro;
        status::g_BattleHistory.setTitle(getDefaultPlayerTitle() + offsetTitle);
        break;
    case 2:
        status::g_BattleHistory.historyType_ = status::BattleHistory::WinEvilPrist;
        status::g_BattleHistory.setTitle(0x320 + offsetTitle);
        break;
    }
}

THUMB int cmn::PlayerTitle::getDefaultPlayerTitle()
{
    int title = 700;
    int hero = 1;
    if (status::g_Story.sex_ == SEX_FEMALE) {
        hero = 2;
    }
    int hour = (int)status::g_Game.getPlayTime() / 216000;
    int heroLevel = status::PartyStatus::getPlayerStatusForPlayerIndex(hero)->haveStatusInfo_.haveStatus_.level_;
    int partyLevelAve = status::g_Party.getAverageLevel();
    float metalExp = getMetalSeriesExp();
    unsigned int wipoutCount = status::g_BattleHistory.getWipeoutCount();
    unsigned int escapeCount = status::g_BattleHistory.getEscapeCount();

    if (hour < 16) {
        title += 2;
    } else if (heroLevel <= 20 && partyLevelAve > 40) {
        title += 4;
    } else if (status::g_Party.getMaxLevel() <= 33) {
        title += 6;
    } else if (wipoutCount == 0 && escapeCount == 0) {
        title += 8;
    } else if (escapeCount == 0) {
        title += 10;
    } else if (wipoutCount == 0) {
        title += 12;
    } else if (heroLevel >= 65) {
        title += 14;
    } else if (heroLevel >= 50 && status::g_BattleResult.getMonsterCount(0x60) < 51) {
        title += 16;
    } else if (hour >= 100) {
        title += 18;
    } else if (hour >= 60 && status::g_BattleResult.getMonsterCount(0x60) < 51) {
        title += 20;
    } else if (metalExp > 0.4) {
        title += 24;
    } else if (210 - status::g_BattleResult.getEncountCount() < 23) {
        title += 26;
    } else if ((unsigned int)status::g_BattleHistory.getMonsterCount() >= 5000) {
        title += 28;
    } else if (status::g_BattleResult.getMonsterCount(0x60) > 50) {
        title += 30;
    } else if (wipoutCount <= 3) {
        title += 32;
    } else if (status::g_Party.haveItemSack_.getCount() >= 113) {
        title += 34;
    } else if (wipoutCount >= 30) {
        title += 36;
    } else if (escapeCount >= 100) {
        title += 38;
    } else if (status::g_Party.bankMoney_ + status::g_Party.gold_ >= 200000) {
        title += 40;
    } else if (status::g_Party.gold_ + status::g_Party.bankMoney_ < 1000) {
        title += 42;
    } else if (checkAllEquip(true) == true) {
        title += 44;
    } else if (checkEquip(true) == true) {
        title += 46;
    } else if (checkEquip(false) == true) {
        title += 48;
    } else if (checkAllEquip(false) == true) {
        title += 50;
    } else if (status::g_Party.casinoCoin_ > 100000) {
        title += 52;
    } else if (checkUhhunnpinkutai() == true) {
        title += 54;
    } else if (partyLevelAve <= 35) {
        title += 56;
    } else if (partyLevelAve <= 37) {
        title += 58;
    } else if (partyLevelAve <= 39) {
        title += 60;
    } else if (partyLevelAve <= 41) {
        title += 62;
    } else if (partyLevelAve <= 42) {
        title += 64;
    } else if (partyLevelAve <= 43) {
        title += 66;
    } else if (partyLevelAve >= 48) {
        title += 68;
    } else if (partyLevelAve >= 46) {
        title += 70;
    } else if (partyLevelAve >= 44) {
        title += 72;
    } else {
        title += 74;
    }
    return title;
}

THUMB bool cmn::PlayerTitle::checkUhhunnpinkutai()
{
    int checkChara[3];
    checkChara[0] = status::g_Party.getSortIndex(4);
    checkChara[1] = status::g_Party.getSortIndex(9);
    checkChara[2] = status::g_Party.getSortIndex(8);
    for (int i = 0; i < 3; i++) {
        if (checkChara[i] == -1) {
            return false;
        }
    }
    for (int i = 0; i < 3; i++) {
        status::HaveEquipment& haveEquipment = status::g_Party.getPlayerStatus(checkChara[i])->haveStatusInfo_.haveEquipment_;
        if (haveEquipment.getEquipment(ITEM_ARMOR) != 0x38 && haveEquipment.getEquipment(ITEM_ARMOR) != 0x41) {
            return false;
        }
    }
    return true;
}

THUMB bool cmn::PlayerTitle::checkAllEquip(bool allCheck)
{
    status::g_Party.setPlayerMode();
    bool ret = true;
    int heroIndex = 1;
    if (allCheck == true) {
        int partyCount = status::g_Party.getCount();
        for (int i = 0; i < partyCount; i++) {
            status::HaveEquipment& haveEquipment = status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveEquipment_;
            if (haveEquipment.getEquipment(ITEM_WEAPON) != 0 || haveEquipment.getEquipment(ITEM_ARMOR) != 0 ||
                haveEquipment.getEquipment(ITEM_SHIELD) != 0 || haveEquipment.getEquipment(ITEM_HELMET) != 0 ||
                haveEquipment.getEquipment(ITEM_ACCESSORY) != 0) {
                ret = false;
                break;
            }
        }
    } else {
        if (status::g_Story.sex_ == SEX_FEMALE) {
            heroIndex = 2;
        }
        status::HaveEquipment& haveEquipment = status::PartyStatus::getPlayerStatusForPlayerIndex(heroIndex)->haveStatusInfo_.haveEquipment_;
        if (haveEquipment.getEquipment(ITEM_WEAPON) != 0 || haveEquipment.getEquipment(ITEM_ARMOR) != 0 ||
            haveEquipment.getEquipment(ITEM_SHIELD) != 0 || haveEquipment.getEquipment(ITEM_HELMET) != 0 ||
            haveEquipment.getEquipment(ITEM_ACCESSORY) != 0) {
            ret = false;
        }
    }
    status::g_Party.setNormalMode();
    return ret;
}

THUMB bool cmn::PlayerTitle::checkEquip(bool weapon)
{
    status::g_Party.setPlayerMode();
    bool ret = true;
    int partyCount = status::g_Party.getCarriageOutCount();
    for (int i = 0; i < partyCount; i++) {
        status::HaveEquipment& haveEquipment = status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveEquipment_;
        if (weapon == true) {
            if (haveEquipment.getEquipment(ITEM_WEAPON) != 0) {
                ret = false;
                break;
            }
        } else {
            if (haveEquipment.getEquipment(ITEM_ARMOR) != 0 || haveEquipment.getEquipment(ITEM_SHIELD) != 0 ||
                haveEquipment.getEquipment(ITEM_HELMET) != 0) {
                ret = false;
                break;
            }
        }
    }
    status::g_Party.setNormalMode();
    return ret;
}

THUMB float cmn::PlayerTitle::getMetalSeriesExp()
{
    float hagureExp = status::g_BattleResult.getExpTotal(0x60);
    float kinguExp = status::g_BattleResult.getExpTotal(0x7d);
    float puratinaExp = status::g_BattleResult.getExpTotal(0xa8);
    float metalExp = hagureExp + kinguExp + puratinaExp;
    float totalExp = status::g_BattleResult.getAllMonsterTotalExp();
    return metalExp / totalExp;
}
