#include "ov015/btl/BattleMenu.hpp"
#include "ov015/btl/BattleMenuPlayerControl.hpp"
#include "ov003/btl/BattleMonsterMask.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"

BattleMenuJudge gBattleMenuJudge;

THUMB void BattleMenuJudge::baseSetup()
{
    unk_00 = 0;
    unk_04 = 0;
    playerMaxNum_ = 0;
    minadeinFlag_ = 0;
}

THUMB void BattleMenuJudge::turnSetup()
{
    btl::BattleMenuPlayerControl::getSingleton()->activeChara_ = -1;
    playerMaxNum_ = status::g_Party.getCount();
    for (int i = 0; i < playerMaxNum_; i++) {
        status::g_Party.getPlayerStatus(i)->haveBattleStatus_.clearSelectCommand();
    }
    minadeinFlag_ = 0;
}

THUMB BattleMenuJudge* BattleMenuJudge::getSingleton()
{
    return &gBattleMenuJudge;
}

THUMB bool BattleMenuJudge::judgeBackChara()
{
    bool result = true;
    int chara = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
    status::g_Party.getPlayerStatus(chara)->haveBattleStatus_.setSelectCommand(status::HaveBattleStatus::NoSelect, 0);
    for (;;) {
        if (--chara < 0) {
            result = false;
            break;
        }
        if (isCommandingPlayer(chara)) {
            status::g_Party.getPlayerStatus(chara)->haveBattleStatus_.setSelectCommand(status::HaveBattleStatus::NoSelect, 0);
            break;
        }
    }
    btl::BattleMenuPlayerControl::getSingleton()->activeChara_ = chara;
    return result;
}

THUMB bool BattleMenuJudge::judgeNextChara()
{
    int chara = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
    bool result = true;
    for (;;) {
        chara++;
        if (chara >= status::g_Party.getCarriageOutCount()) {
            result = false;
            break;
        }
        if (isCommandingPlayer(chara)) {
            break;
        }
    }
    btl::BattleMenuPlayerControl::getSingleton()->activeChara_ = chara;
    return result;
}

THUMB void BattleMenuJudge::backActionMenu(int page)
{
    BattleMonsterNamePlate::getSingleton().init();
    BattleMonsterNamePlate::getSingleton().setMonster();
    gBattleMenu_ACTIONMENU.open();
    gBattleMenu_ACTIONMENU.pageItem_.active_ = page;
}

THUMB void BattleMenuJudge::setAttack(int group)
{
    status::HaveBattleStatus* battleStatus = &status::g_Party.getPlayerStatus(btl::BattleMenuPlayerControl::getSingleton()->activeChara_)->haveBattleStatus_;
    battleStatus->setSelectCommand(status::HaveBattleStatus::Attack, 0);
    battleStatus->selectedGroup_ = group;
}

THUMB void BattleMenuJudge::setMagicEnemy(int magic, int group)
{
    int chara = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
    status::PlayerStatus* player = status::g_Party.getPlayerStatus(chara);
    status::HaveBattleStatus* battleStatus = &status::g_Party.getPlayerStatus(chara)->haveBattleStatus_;
    battleStatus->setSelectCommand(status::HaveBattleStatus::UseAction, player->haveStatusInfo_.haveAction_.getAction(magic));
    battleStatus->selectedGroup_ = group;
}

THUMB void BattleMenuJudge::setMagicParty(int magic, int target)
{
    int chara = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
    status::PlayerStatus* player = status::g_Party.getPlayerStatus(chara);
    status::HaveBattleStatus* battleStatus = &status::g_Party.getPlayerStatus(chara)->haveBattleStatus_;
    battleStatus->setSelectCommand(status::HaveBattleStatus::UseAction, player->haveStatusInfo_.haveAction_.getAction(magic));
    battleStatus->selectedGroup_ = 0;
    battleStatus->selectedTarget_ = target;
}

THUMB void BattleMenuJudge::setItemEnemy(int item, int group)
{
    status::HaveBattleStatus* battleStatus = &status::g_Party.getPlayerStatus(btl::BattleMenuPlayerControl::getSingleton()->activeChara_)->haveBattleStatus_;
    battleStatus->setSelectCommand(status::HaveBattleStatus::UseItem, item);
    battleStatus->selectedGroup_ = group;
}

THUMB void BattleMenuJudge::setItemEnemyAll(int item)
{
    setItemEnemy(item, 0);
}

THUMB void BattleMenuJudge::setItemParty(int item, int target)
{
    status::HaveBattleStatus* battleStatus = &status::g_Party.getPlayerStatus(btl::BattleMenuPlayerControl::getSingleton()->activeChara_)->haveBattleStatus_;
    battleStatus->setSelectCommand(status::HaveBattleStatus::UseItem, item);
    battleStatus->selectedGroup_ = 0;
    battleStatus->selectedTarget_ = target;
}

THUMB void BattleMenuJudge::setItemPartyAll(int item)
{
    setItemEnemy(item, 0);
}

THUMB int BattleMenuJudge::getMonsterTouchRect(TOUCHRECT* rect)
{
    int count = g_monster.getBattleCount();
    int num = 0;
    for (int i = 0; num < count; i++) {
        if (g_monster.getMonsterStatus(i)->isBattleEnable()) {
            int* monsterRect = BattleMonsterMask::getSingleton()->getMonsterTouchRect(i);
            touchRect_[num].group = monsterRect[0];
            touchRect_[num].x = monsterRect[1];
            touchRect_[num].y = monsterRect[2];
            touchRect_[num].width = monsterRect[3] - monsterRect[1];
            touchRect_[num].height = monsterRect[4] - monsterRect[2];
            rect[num].x = touchRect_[num].x;
            rect[num].y = touchRect_[num].y;
            rect[num].width = touchRect_[num].width;
            rect[num].height = touchRect_[num].height;
            rect[num].group = touchRect_[num].group;
            num++;
        }
    }
    return count;
}

THUMB int BattleMenuJudge::getLiveMonsterID()
{
    int i = 0;
    int count = g_monster.getCount();
    for (; i < count; i++) {
        if (g_monster.getMonsterStatus(i)->isEnable()) {
            break;
        }
    }
    return i;
}

THUMB bool BattleMenuJudge::judgeBattleArrayChange()
{
    return status::g_Party.getCarriageCount() != 0;
}

THUMB void BattleMenuJudge::setNextPlayer()
{
    btl::BattleMenuPlayerControl::getSingleton()->makePlayerHistory();
    if (getSingleton()->judgeNextChara()) {
        getSingleton()->backActionMenu(0);
        gBattleMenu_ACTIONMENU.activeCharacter_ = getSingleton()->getPlayerIndex();
        return;
    }
    btl::BattleMenuPlayerControl::getSingleton()->activeChara_ = -1;
    BattleMonsterMask::getSingleton()->select(-1);
    gBattleMenuSub_HISTORY.commandChara_ = -1;
    gBattleMenuSub_HISTORY.update_ = 0;
    gBattleMenu_ROOT.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
}

THUMB void BattleMenuJudge::setPrevPlayer()
{
    if (getSingleton()->judgeBackChara()) {
        getSingleton()->backActionMenu(0);
        gBattleMenu_ACTIONMENU.activeCharacter_ = getSingleton()->getPlayerIndex();
        gBattleMenuSub_HISTORY.select_ = getSingleton()->getPlayerIndex();
        return;
    }
    gBattleMenu_ROOT.open();
    gBattleMenuSub_HISTORY.commandChara_ = -1;
}

THUMB bool BattleMenuJudge::isCommandingPlayer(int index)
{
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(index)->haveStatusInfo_;
    if (info->battleCommand_ == COMMAND_MEIREISASERO
        && info->haveStatus_.isPlayer_
        && !info->isDeath()
        && !info->statusChange_.isEnable(status::StatusChange::StatusSleep)
        && !info->statusChange_.isEnable(status::StatusChange::StatusSpazz)
        && !info->statusChange_.isEnable(status::StatusChange::StatusPath1)
        && !info->statusChange_.isEnable(status::StatusChange::StatusAstoron)
        && !info->statusChange_.isEnable(status::StatusChange::StatusTimeStop)
        && !info->statusChange_.isEnable(status::StatusChange::StatusDragoram)) {
        if (minadeinFlag_ != 0 && (int)info->haveStatus_.playerIndex_ > 2) {
            return false;
        }
        return true;
    }
    return false;
}
