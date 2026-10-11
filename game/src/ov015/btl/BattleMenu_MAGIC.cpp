#include "ov015/btl/BattleMenu.hpp"
#include "main/dss/DssUtils.hpp"
#include "ov015/btl/BattleMenuPlayerControl.hpp"
#include "ov003/btl/BattleMonsterMask.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/profile/Profile.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/UseAction.hpp"
#include "main/text/TextAPI.hpp"
#include "main/menu/MenuTemplate_Common.hpp"

THUMB void BattleMenu_MAGIC::menuSetup()
{
    status::g_Party.setBattleMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    unk_e4.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
    BattleMonsterMask::getSingleton()->select(-1);
    count_ = 0;
    dss::memset(haveAction_, -1, sizeof(haveAction_));
    dss::memset(haveActionIndex_, -1, sizeof(haveActionIndex_));
    unkfunc_0216ded8();
    navigator_.setupBase();
    navigator_.setup(2, 3, count_);
    int pos = btl::BattleMenuPlayerControl::getSingleton()->getMagicPosition();
    int page = pos / 6;
    pos = pos % 6;
    menuItem_.active_ = pos;
    navigator_.setPageNo(page);
    int magic = haveActionIndex_[pos];
    btl::BattleMenuPlayerControl::getSingleton()->activeMagic_ = magic;
}

THUMB void BattleMenu_MAGIC::setActiveMagicPos(int pos)
{
    int active = pos;
    active %= 6;
    menuItem_.active_ = active;
    navigator_.setPageNo(pos / 6);
}

THUMB void BattleMenu_MAGIC::menuExecute()
{
    navigator_.setup(2, 3, count_);
    MenuTemplate_battle::BATTLE_MAGIC_2x3(&menuItem_, navigator_.getCountInPage());
    MenuTemplate_battle::BATTLE_CANCEL(&cancelItem_);
    int max = navigator_.getPageMaxCount() - 1;
    MenuTemplate_Common::TOWN_PAGE_1x1(&unk_e4, navigator_.getIndex(unk_e4.active_), max, 0xd8, 0x78);
}

THUMB void BattleMenu_MAGIC::menuDraw()
{
    if (!data_020ed1bc.isOpen()) {
        unkfunc_0216bb00(haveAction_, count_, navigator_.getPageNo());
        menuItem_.drawActive();
        cancelItem_.drawActive();
    }
}

THUMB void BattleMenu_MAGIC::menuUpdate()
{
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus((short)btl::BattleMenuPlayerControl::getSingleton()->activeChara_)->haveStatusInfo_;
    short pos = navigator_.getIndex(menuItem_.active_);
    int action = haveAction_[pos];
    int magic = haveActionIndex_[navigator_.getIndex(menuItem_.active_)];
    if (data_020ed1bc.isOpen()) {
        if ((unsigned int)(data_020ed1bc.stat_ - 1) <= 1) {
            data_020ed1bc.close();
            return;
        }
    } else {
        if (MenuUpdate_Assist::isCancel(cancelItem_)) {
            BattleMenuJudge::getSingleton()->backActionMenu(1);
            close();
            return;
        }
        int active = menuItem_.active_;
        if (MenuUpdate_Assist::isPageFlipOne(unk_e4, navigator_, active)) {
            menuItem_.active_ = active;
            redraw_ = 1;
            return;
        }
        int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
        if (result != 0) {
            redraw_ = 1;
            if (result == 2) {
                menuItem_.result_ = 0;
                menuItem_.lastresult_ = 0;
                if (!status::UseAction::isUse(action, info)) {
                    TextAPI::setMACRO0(1, 0x50000000, info->haveStatus_.playerIndex_);
                    data_020ed1bc.openMessageForBATTLE();
                    data_020ed1bc.addMessage(0xc3c6f);
                    return;
                }
                if (action == 0x13 && !status::g_Party.isExecMinadein()) {
                    data_020ed1bc.openMessageForBATTLE();
                    data_020ed1bc.addMessage(0xc394d);
                    return;
                }
                btl::BattleMenuPlayerControl::getSingleton()->setMagicPosition(pos);
                switch (status::UseAction::getUseType(action)) {
                case status::UseItem::Enemy:
                    if (status::UseAction::getUseArea(action) == status::UseItem::Group || status::UseAction::getUseArea(action) == status::UseItem::One) {
                        if (g_monster.getGroupCount() > 1) {
                            BattleMenuJudge::getSingleton()->setMagicEnemy(magic, -1);
                            int target = BattleMenuJudge::getSingleton()->getLiveMonsterID();
                            btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = target;
                            close();
                            BattleMonsterNamePlate::getSingleton().init();
                            BattleMonsterNamePlate::getSingleton().setMonster();
                            btl::BattleMenuPlayerControl::getSingleton()->activeMagic_ = magic;
                            gBattleMenu_MAGIC2ENEMY.open();
                            gBattleMenu_MAGIC2ENEMY.activeMagicPos_ = navigator_.getIndex(menuItem_.active_);
                            gBattleMenu_MAGIC2ENEMY.activeMagicIndex_ = action;
                        } else {
                            if (action == 0x13) {
                                BattleMenuJudge::getSingleton()->minadeinFlag_ = 1;
                            }
                            close();
                            int group = 0;
                            for (int i = 0; i < g_monster.getCount(); i++) {
                                if (g_monster.getMonsterStatus(i)->isEnable()) {
                                    group = g_monster.getMonsterGroup(i);
                                    break;
                                }
                            }
                            BattleMenuJudge::getSingleton()->setMagicEnemy(magic, group);
                            BattleMenuJudge::getSingleton()->setNextPlayer();
                        }
                    } else {
                        setMagicTargetFree(magic);
                    }
                    break;
                case status::UseItem::Friend:
                    if (status::UseAction::getUseArea(action) == status::UseItem::One) {
                        BattleMenuJudge::getSingleton()->setMagicParty(magic, -1);
                        btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = 0;
                        close();
                        if (status::g_Party.getCarriageOutCount() > 1) {
                            btl::BattleMenuPlayerControl::getSingleton()->activeMagic_ = magic;
                            gBattleMenu_MAGIC2PARTY.open();
                            gBattleMenu_MAGIC2PARTY.activeMagicPos_ = navigator_.getIndex(menuItem_.active_);
                        } else {
                            BattleMenuJudge::getSingleton()->setMagicParty(magic, 0);
                            BattleMenuJudge::getSingleton()->setNextPlayer();
                        }
                    } else {
                        setMagicTargetFree(magic);
                    }
                    break;
                default:
                    setMagicTargetFree(magic);
                    break;
                }
            }
        }
        btl::BattleMenuPlayerControl::getSingleton()->activeMagic_ = magic;
    }
}

THUMB void BattleMenu_MAGIC::unkfunc_0216ded8()
{
    int count;
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(btl::BattleMenuPlayerControl::getSingleton()->activeChara_)->haveStatusInfo_;
    count = info->haveAction_.getCount();
    int num = 0;
    count_ = num;
    dss::memset(haveAction_, -1, sizeof(haveAction_));
    dss::memset(haveActionIndex_, -1, sizeof(haveActionIndex_));
    for (int i = 0; i < count; i++) {
        int action = info->haveAction_.getAction(i);
        if (status::UseAction::isBattleUse(action)) {
            haveAction_[num] = action;
            haveActionIndex_[num] = i;
            num++;
        }
    }
    count_ = num;
}

THUMB void BattleMenu_MAGIC::setMagicTargetFree(int magic)
{
    close();
    BattleMenuJudge::getSingleton()->setMagicParty(magic, -1);
    BattleMenuJudge::getSingleton()->setNextPlayer();
}
