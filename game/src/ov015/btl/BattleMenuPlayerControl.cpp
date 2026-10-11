#include "ov015/btl/BattleMenuPlayerControl.hpp"
#include "main/dss/DssUtils.hpp"
#include "ov015/btl/BattleMenu.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/HaveBattleStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/status/UseItem.hpp"
#include "main/status/StatusChange.hpp"
#include "ov003/status/MonsterParty.hpp"
#include "ov003/status/MonsterStatus.hpp"
#include "main/menu/MenuDataCommon.hpp"


const CondMessageTable CONDITION_MESSAGE_TABLE = { {
    0xA000001A, 0xA0000019, 0xA0000018,
    0xA0000017, 0xA0000016, 0xA0000015,
    0,
} };
btl::BattleMenuPlayerControl gBattleMenuPlayerControl;   // = data_ov015_0217acd4

const CondCheckTable CONDITION_STATUS_TABLE = { { 1, 0, 5, 3, 2, 4 } };

const int AUTO_COMMAND_NAME_TABLE[6] = {
    0x90000001, 0x90000002, 0x90000005,
    0x90000003, 0x90000004, 0x90000006,
};


THUMB btl::BattleMenuPlayerControl * btl::BattleMenuPlayerControl::getSingleton()
{
  return &gBattleMenuPlayerControl;
}


THUMB int btl::BattleMenuPlayerControl::getPlayerItemId()   
{
    return status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_.getItem(activeItem_);
}

THUMB void btl::BattleMenuPlayerControl::clear()
{
    status::g_Party.setBattleMode();

    activeChara_ = -1;
    activeItem_ = -1;
    activeMagic_ = -1;
    targetChara_ = -1;

    dss::memset(secondHistory_, -1, sizeof(secondHistory_));
    dss::memset(firstHistory_, -1, sizeof(firstHistory_));
    dss::memset(memberHP_, 0, sizeof(memberHP_));
    dss::memset(memberMP_, 0, sizeof(memberMP_));
    dss::memset(memberLv_, 1, sizeof(memberLv_));
    dss::memset(memberHPColor_, 0, sizeof(memberHPColor_));
    dss::memset(memberCondition_, 0, sizeof(memberCondition_));
    dss::memset(conditionChange_, 0, sizeof(conditionChange_));

    tacticsSex_ = 0;
    if (status::g_Story.sex_ == 1) {
        tacticsSex_ = 6;
    }

    for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
        if (status::g_Story.chapter_ >= 5) {
            status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i)->haveStatusInfo_;

            if (info->haveStatus_.playerIndex_ != 1 && info->haveStatus_.playerIndex_ != 2
             && info->haveStatus_.isPlayer_ != 0 && info->isDeath() == 0) {
                firstHistory_[i] = AUTO_COMMAND_NAME_TABLE[status::g_Party.getPlayerStatus(i)->haveStatusInfo_.battleCommand_];
                firstHistory_[i] += tacticsSex_;
            }
        }
        flashStatus(i);
    }
}


THUMB void btl::BattleMenuPlayerControl::allClear()
{
    clear();
    dss::memset(magicPosition_, 0, sizeof(magicPosition_));
}

THUMB void btl::BattleMenuPlayerControl::setNoSelectHistory(int index)
{
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(index)->haveStatusInfo_;

    if (info->haveStatus_.playerIndex_ != 1 && info->haveStatus_.playerIndex_ != 2
     && info->haveStatus_.isPlayer_ != 0 && status::g_Story.chapter_ >= 5) {
        secondHistory_[index] = -1;
        firstHistory_[index] = tacticsSex_ + AUTO_COMMAND_NAME_TABLE[info->battleCommand_];
    } else {
        secondHistory_[index] = -1;
        firstHistory_[index] = -1;
    }
}

THUMB int btl::BattleMenuPlayerControl::makePlayerHistory()
{
    int chara = activeChara_;

    if (chara < 0 || chara >= status::g_Party.getCarriageOutCount()) {
        return 0;
    }

    status::PlayerStatus* player;
    int* second;
    int oldFirst;
    int oldSecond;

    int* first = firstHistory_;
    oldFirst = first[chara];
    second = secondHistory_;
    oldSecond = second[chara];
    player = status::g_Party.getPlayerStatus(chara);
    status::HaveBattleStatus* hbs = &player->haveBattleStatus_;

    if (status::g_Party.getPlayerStatus(chara)->haveStatusInfo_.battleCommand_ == COMMAND_DEBUG) {
        second[chara] = -1;
        first[chara] = -1;
        return 0;
    }


    switch (hbs->getSelectCommand()) {
    case status::HaveBattleStatus::NoSelect:
        setNoSelectHistory(chara);
        break;
    case status::HaveBattleStatus::Attack:
        setAttackHistory();
        break;
    case status::HaveBattleStatus::Defence:
        setDefenceHistory();
        break;
    case status::HaveBattleStatus::UseAction:
        setUseActionHistory();
        break;
    case status::HaveBattleStatus::UseItem:
        setUseItemHistory();
        break;
    }

    if (oldFirst == first[chara] && oldSecond == second[chara]) {
        return 0;
    }
    return 1;
}

THUMB int btl::BattleMenuPlayerControl::resetPlayerHistory(int playerNum)
{
    int i;
    int* first;
    int oldFirst;
    int oldSecond;
    status::HaveBattleStatus* hbs;
    int playerIndex;
    int v;

    hbs = &status::g_Party.getPlayerStatus(playerNum)->haveBattleStatus_;
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(playerNum)->haveStatusInfo_;
    int* second = secondHistory_;
    int group;
    int monsterIndex;
    int sel;

    first = firstHistory_;
    oldFirst = first[playerNum];
    oldSecond = second[playerNum];
    playerIndex = status::g_Party.getPlayerStatus(playerNum)->haveStatusInfo_.haveStatus_.playerIndex_;

    if (info->isDeath() != 0 || info->haveStatus_.isPlayer_ == 0 || info->battleCommand_ == COMMAND_DEBUG) {
        second[playerNum] = -1;
        firstHistory_[playerNum] = -1;          
        if (oldFirst == firstHistory_[playerNum] && oldSecond == second[playerNum]) {
            return 0;
        }
        return 1;
    }

    if ((info->battleCommand_ != COMMAND_MEIREISASERO || BattleMenuJudge::getSingleton()->isCommandingPlayer(playerNum) == 0)
     && status::g_Story.chapter_ >= 5) {
        second[playerNum] = -1;
        firstHistory_[playerNum] = tacticsSex_ + AUTO_COMMAND_NAME_TABLE[info->battleCommand_];
        if (oldFirst == firstHistory_[playerNum] && oldSecond == second[playerNum]) {
            return 0;
        }
        return 1;
    }

    switch (hbs->getSelectCommand()) {
    case status::HaveBattleStatus::NoSelect:
        if (playerIndex != 1 && playerIndex != 2 && status::g_Story.chapter_ >= 5) {
            second[playerNum] = -1;
            first[playerNum] = tacticsSex_ + AUTO_COMMAND_NAME_TABLE[info->battleCommand_];
        } else {
            second[playerNum] = -1;
            first[playerNum] = -1;
        }
        break;

    case status::HaveBattleStatus::Attack: {
        group = hbs->selectedGroup_;
        for (i = 0; i < g_monster.getCount(); i++) {
            if (group == g_monster.getMonsterGroup(i))
                break;
        }
        monsterIndex = g_monster.getMonsterIndex(i);
        if (i < g_monster.getCount()) {
            second[playerNum] = monsterIndex + 0x60000000;
            first[playerNum] = 0x80000004;
            status::MonsterStatus* ms = g_monster.getMonsterStatus(i);
            if (ms->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusMosyasu)) {
                int base = 0x60000000;
                second[playerNum] = (int)((char*)(ms->mosyasIndex_ + 0xD6) + base);//weird cast, but it matches the original code
                if (ms->mosyasIndex_ == 0x19) {
                    second[playerNum] = base + 0xE0;
                }
            }
        }
        break;
    }

    case status::HaveBattleStatus::Defence:
        second[playerNum] = -1;
        first[playerNum] = 0x80000007;
        break;

    case status::HaveBattleStatus::UseAction:
        if (hbs->selectIndex_ > 0) {
            second[playerNum] = hbs->selectIndex_ + 0x70000000;
            first[playerNum] = 0x80000005;
        } else {
            setNoSelectHistory(playerNum);
        }
        break;

    case status::HaveBattleStatus::UseItem: {
        sel = hbs->selectIndex_;
        status::UseItem::getAction(info->haveItem_.getItem(sel));
        second[playerNum] = info->haveItem_.getItem(sel) + 0x40000000;
        first[playerNum] = 0x80000006;
        break;
    }
    }

    if (oldFirst == first[playerNum] && oldSecond == second[playerNum]) {
        return 0;
    }
    return 1;
}


THUMB void btl::BattleMenuPlayerControl::setAttackHistory()
{
    int i;
    int monsterIndex;
    status::MonsterStatus* ms;

    status::CharacterStatus* player = status::g_Party.getPlayerStatus(activeChara_);
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_; 
    int group = player->haveBattleStatus_.selectedGroup_;

    if (group < 0) {
        ms = g_monster.getMonsterInGroup(targetChara_, 0);
        secondHistory_[activeChara_] = ms->characterIndex_ + 0x60000000;
        firstHistory_[activeChara_] = 0x80000004;
        if (ms->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusMosyasu)) {
            int base = 0x60000000;
            secondHistory_[activeChara_] = (int)((char*)(ms->mosyasIndex_ + 0xD6) + base);//weird cast, but it matches the original code
            if (ms->mosyasIndex_ == 0x19) {
                secondHistory_[activeChara_] = base + 0xE0;
            }
        }
        return;
    }

    for (i = 0; i < g_monster.getCount(); i++) {
        if (group == g_monster.getMonsterGroup(i)) {
            break;
        }
    }
    monsterIndex = g_monster.getMonsterIndex(i);
    if (i < g_monster.getCount()) {
        secondHistory_[activeChara_] = monsterIndex + 0x60000000;
        firstHistory_[activeChara_] = 0x80000004;
        ms = g_monster.getMonsterStatus(i);
        if (ms->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusMosyasu)) {
            int base = 0x60000000;
            secondHistory_[activeChara_] = (int)((char*)(ms->mosyasIndex_ + 0xD6) + base);//same here
            if (ms->mosyasIndex_ == 0x19) {
                secondHistory_[activeChara_] = base + 0xE0;
            }
        }
    }
}


THUMB void btl::BattleMenuPlayerControl::setDefenceHistory()
{
    status::CharacterStatus* player = status::g_Party.getPlayerStatus(activeChara_);
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_; // morts tous les deux — garder
    secondHistory_[activeChara_] = -1;
    firstHistory_[activeChara_] = 0x80000007;
}

THUMB void btl::BattleMenuPlayerControl::setUseItemHistory()
{
    status::CharacterStatus* player = status::g_Party.getPlayerStatus(activeChara_);
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_;
    int sel = player->haveBattleStatus_.selectIndex_;

    if (sel < 0) {
        secondHistory_[activeChara_] = info->haveItem_.getItem(activeItem_) + 0x40000000;
        firstHistory_[activeChara_] = 0x80000006;
    } else {
        secondHistory_[activeChara_] = info->haveItem_.getItem(sel) + 0x40000000;
        firstHistory_[activeChara_] = 0x80000006;
    }
}


THUMB void btl::BattleMenuPlayerControl::setUseActionHistory()
{
    status::CharacterStatus* player = status::g_Party.getPlayerStatus(activeChara_);
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_;
    int sel = player->haveBattleStatus_.selectIndex_;

    if (sel < 0) {
        secondHistory_[activeChara_] = info->haveAction_.getAction(activeMagic_) + 0x70000000;
        firstHistory_[activeChara_] = 0x80000005;
    } else if (sel != 0) {
        secondHistory_[activeChara_] = sel + 0x70000000;
        firstHistory_[activeChara_] = 0x80000005;
    } else {
        setNoSelectHistory(activeChara_);
    }
}

THUMB bool btl::BattleMenuPlayerControl::flashStatus(int memberNum)
{
    bool result = false;
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(memberNum)->haveStatusInfo_;

    if (flashHP(memberNum) != 0) {
        result = true;
    }
    if (flashMP(memberNum) != 0) {
        result = true;
    }
    if (flashCondition(memberNum) != 0) {
        result = true;
    }
    if (flashHPColor(memberNum) != 0) {
        result = true;
    }
    memberLv_[memberNum] = info->haveStatus_.level_;

    return result;
}


THUMB bool btl::BattleMenuPlayerControl::flashHP(int memberNum)
{
    status::CharacterStatus* player = status::g_Party.getPlayerStatus(memberNum);
    status::HaveStatusInfo* info = &player->haveStatusInfo_;
    short old = memberHP_[memberNum];

    if (player->isMenuStatusFlag(status::HaveStatusInfo::BeforeAction) != 0) {
        memberHP_[memberNum] = info->getHpInBattle(status::HaveStatusInfo::BeforeAction);
    } else if (player->isMenuStatusFlag(status::HaveStatusInfo::ExecuteAction) != 0) {
        memberHP_[memberNum] = info->getHpInBattle(status::HaveStatusInfo::ExecuteAction);
    } else if (player->isMenuStatusFlag(status::HaveStatusInfo::ResultAction) != 0) {
        memberHP_[memberNum] = info->getHpInBattle(status::HaveStatusInfo::ResultAction);
    } else if (player->isMenuStatusFlag(status::HaveStatusInfo::SpecialAction) != 0) {
        memberHP_[memberNum] = info->getHpInBattle(status::HaveStatusInfo::SpecialAction);
    } else {
        memberHP_[memberNum] = info->getHp();
    }

    return old != memberHP_[memberNum];
}

THUMB bool btl::BattleMenuPlayerControl::flashMP(int memberNum)
{
    status::CharacterStatus* player = status::g_Party.getPlayerStatus(memberNum);
    status::HaveStatusInfo* info = &player->haveStatusInfo_;
    short old = memberMP_[memberNum];

    if (player->isMenuStatusFlag(status::HaveStatusInfo::BeforeAction) != 0) {
        memberMP_[memberNum] = info->getMpInBattle(status::HaveStatusInfo::BeforeAction);
    } else if (player->isMenuStatusFlag(status::HaveStatusInfo::ExecuteAction) != 0) {
        memberMP_[memberNum] = info->getMpInBattle(status::HaveStatusInfo::ExecuteAction);
    } else if (player->isMenuStatusFlag(status::HaveStatusInfo::ResultAction) != 0) {
        memberMP_[memberNum] = info->getMpInBattle(status::HaveStatusInfo::ResultAction);
    } else if (player->isMenuStatusFlag(status::HaveStatusInfo::SpecialAction) != 0) {
        memberMP_[memberNum] = info->getMpInBattle(status::HaveStatusInfo::SpecialAction);
    } else {
        memberMP_[memberNum] = info->getMp();
    }

    return old != memberMP_[memberNum];
}

THUMB bool btl::BattleMenuPlayerControl::flashHPColor(int index)
{
    status::CharacterStatus* player = status::g_Party.getPlayerStatus(index);
    status::HaveStatusInfo* info;
    info = (status::HaveStatusInfo*)player;  
    info = &player->haveStatusInfo_;
    short old = memberHPColor_[index];

    if (player->isMenuStatusFlag(status::HaveStatusInfo::BeforeAction) != 0) {
        memberHPColor_[index] = isFlashHPColor(index, status::HaveStatusInfo::BeforeAction);
    } else if (player->isMenuStatusFlag(status::HaveStatusInfo::ExecuteAction) != 0) {
        memberHPColor_[index] = isFlashHPColor(index, status::HaveStatusInfo::ExecuteAction);
    } else if (player->isMenuStatusFlag(status::HaveStatusInfo::ResultAction) != 0) {
        memberHPColor_[index] = isFlashHPColor(index, status::HaveStatusInfo::ResultAction);
    } else if (player->isMenuStatusFlag(status::HaveStatusInfo::SpecialAction) != 0) {
        memberHPColor_[index] = isFlashHPColor(index, status::HaveStatusInfo::SpecialAction);
    } else {
        memberHPColor_[index] = unkfunc_0216b980(info);
    }

    return old != memberHPColor_[index];
}

THUMB int btl::BattleMenuPlayerControl::isFlashHPColor(int index, status::HaveStatusInfo::DiffStatus timing)
{
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(index)->haveStatusInfo_;

    if (info->isDeathInBattle(timing) != 0) {
        return 2;
    }
    if (info->isNearDeathInBattle(timing) != 0) {
        return 1;
    }
    return 0;
}



THUMB int btl::BattleMenuPlayerControl::flashCondition(int memberNum)
{
    int* flag;          
    int* msg;           
    int old;            
    int found;          
    int i;              

    status::PlayerStatus* player = status::g_Party.getPlayerStatus(memberNum);
    status::HaveStatusInfo* info = &player->haveStatusInfo_;

    msg = memberCondition_;
    old = msg[memberNum];

    CondCheckTable statusTable = CONDITION_STATUS_TABLE;      
    CondMessageTable messageTable = CONDITION_MESSAGE_TABLE;  

    i = 0;
    msg[memberNum] = 0xA0000013;
    flag = conditionChange_;
    flag[memberNum] = 0;
    found = -1;

    for (; messageTable.v[i] != 0; i++) {
        if (isFlashCondition(info, (status::HaveStatusInfo::MenuStatusChange)statusTable.v[i]) != 0) {
            conditionChange_[memberNum] = 1;        
            memberCondition_[memberNum] = menu::MenuDataCommon::convMessage(info, messageTable.v[i]);
            found = i;
        }
    }

    if (memberHP_[memberNum] == 0) {
        flag[memberNum] = 1;                       
        msg[memberNum] = menu::MenuDataCommon::convMessage(info, 0xA0000014);
        found = 6;
    }

    if (found < 3 && info->haveEquipment_.isEquipment(0x5A) != 0) {
        flag[memberNum] = 1;
        msg[memberNum] = menu::MenuDataCommon::convMessage(info, 0xA0000017);
    }

    if (old != msg[memberNum]) {
        return 1;
    }
    return 0;
}

THUMB int btl::BattleMenuPlayerControl::isFlashCondition(status::HaveStatusInfo* info, status::HaveStatusInfo::MenuStatusChange menuStatus)
{
    status::CharacterStatus* chr = status::PartyStatus::getPlayerStatusForPlayerIndex(info->haveStatus_.playerIndex_);

    if (chr->isMenuStatusFlag(status::HaveStatusInfo::BeforeAction) != 0) {
        return info->isStatusChangeInBattle(status::HaveStatusInfo::BeforeAction, menuStatus);
    }
    if (chr->isMenuStatusFlag(status::HaveStatusInfo::ExecuteAction) != 0) {
        return info->isStatusChangeInBattle(status::HaveStatusInfo::ExecuteAction, menuStatus);
    }
    if (chr->isMenuStatusFlag(status::HaveStatusInfo::ResultAction) != 0) {
        return info->isStatusChangeInBattle(status::HaveStatusInfo::ResultAction, menuStatus);
    }
    if (chr->isMenuStatusFlag(status::HaveStatusInfo::SpecialAction) != 0) {
        return info->isStatusChangeInBattle(status::HaveStatusInfo::SpecialAction, menuStatus);
    }

    switch (menuStatus) {
    case status::HaveStatusInfo::Manusa:
        return info->statusChange_.isEnable(status::StatusChange::StatusManusa);
    case status::HaveStatusInfo::Mahoton: {
        int enabled = info->statusChange_.isEnable(status::StatusChange::StatusMahoton);
        if (enabled == 0) {
            return enabled;
        }
        if (info->haveStatus_.getMpMax() != 0) {
            return 1;
        }
        return 0;
    }
    case status::HaveStatusInfo::Sleep:
        return info->statusChange_.isEnable(status::StatusChange::StatusSleep);
    case status::HaveStatusInfo::Confusion:
        return info->statusChange_.isEnable(status::StatusChange::StatusConfusion);
    case status::HaveStatusInfo::Spazz:
        return info->statusChange_.isEnable(status::StatusChange::StatusSpazz);
    case status::HaveStatusInfo::Spazz|status::HaveStatusInfo::Mahoton:
        return info->statusChange_.isEnable(status::StatusChange::StatusPoison);
    }
    return 0;
}

THUMB int btl::BattleMenuPlayerControl::getTargetGroup()
{
    
    int count;
    int group = targetMonsterGroup_[activeChara_];
    count = g_monster.getCount();
    for (int i = 0; i < count; i++) {
        if (group == g_monster.getMonsterStatus(i)->characterGroup_   
         && g_monster.getMonsterStatus(i)->isBattleEnable() != 0) {
            return group;
        }
    }
    return 0;
}

THUMB void btl::BattleMenuPlayerControl::setTargetGroup(int monsterNum)
{
  targetMonsterGroup_[activeChara_] = monsterNum;
}


THUMB void btl::BattleMenuPlayerControl::setMagicPosition(int position)
{
    magicPosition_[status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveStatus_.playerIndex_] = position;
}

THUMB int btl::BattleMenuPlayerControl::getMagicPosition()
{
    return magicPosition_[status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveStatus_.playerIndex_];
}