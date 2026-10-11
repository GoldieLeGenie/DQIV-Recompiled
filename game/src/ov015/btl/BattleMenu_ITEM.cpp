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
#include "main/status/UseItem.hpp"
#include "main/menu/MenuTemplate_Common.hpp"

THUMB void BattleMenu_ITEM::menuSetup()
{
    status::g_Party.setBattleMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    unk_ec.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH, menu::MenuItem::CURSORTYPE_NONE);
    int i;
    unk_1c = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(btl::BattleMenuPlayerControl::getSingleton()->activeChara_)->haveStatusInfo_;
    navigator_.setupBase();
    navigator_.setup(2, 3, info->haveItem_.getCount());
    int pos = 0;
    for (i = 0; i < info->haveItem_.getCount(); i++) {
        if (info->haveItem_.isEquipment(i) == 0) {
            pos = i;
            break;
        }
    }
    if (btl::BattleMenuPlayerControl::getSingleton()->activeItem_ != -1) {
        pos = btl::BattleMenuPlayerControl::getSingleton()->activeItem_;
    } else {
        btl::BattleMenuPlayerControl::getSingleton()->activeItem_ = pos;
    }
    int page = pos / 6;
    pos = pos % 6;
    menuItem_.active_ = pos;
    navigator_.setPageNo(page);
    BattleMonsterMask::getSingleton()->select(-1);
}

THUMB void BattleMenu_ITEM::menuExecute()
{
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(btl::BattleMenuPlayerControl::getSingleton()->activeChara_)->haveStatusInfo_;
    int count = info->haveItem_.getCount();
    navigator_.setup(2, 3, info->haveItem_.getCount());
    int num = (count - 1) % 6 + 1;
    int page;
    int last = navigator_.getPageMaxCount() - 1;
    page = navigator_.getPageNo();
    if (page != last) {
        num = 6;
    }
    MenuTemplate_Common::COMMON_ITEM_ICON32_2x3(&menuItem_, num, menuItem_.active_);
    MenuTemplate_battle::BATTLE_CANCEL(&cancelItem_);
    int max = navigator_.getPageMaxCount() - 1;
    MenuTemplate_Common::TOWN_PAGE_1x1(&unk_ec, unk_ec.active_, max, 0xd8, 0x78);
}

THUMB void BattleMenu_ITEM::menuDraw()
{
    if (!data_020ed1bc.isOpen()) {
        status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(btl::BattleMenuPlayerControl::getSingleton()->activeChara_)->haveStatusInfo_;
        int count = info->haveItem_.getCount();
        int items[12];
        dss::memset(items, 0, sizeof(items));
        for (int i = 0; i < count; i++) {
            items[i] = info->haveItem_.getItem(i);
        }
        unkfunc_0216ba60(items, count, navigator_.getPageNo());
        menuItem_.drawActive();
    }
}

THUMB void BattleMenu_ITEM::menuUpdate()
{
    if (data_020ed1bc.isOpen()) {
        if ((unsigned int)(data_020ed1bc.stat_ - 1) <= 1) {
            data_020ed1bc.close();
        }
        return;
    }
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        close();
        BattleMenuJudge::getSingleton()->backActionMenu(2);
        return;
    }
    int result = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);
    if (result != 0) {
        redraw_ = 1;
        if (result == 2) {
            if (isSelectEquipEnable()) {
                close();
                gUnkBattleMenu_0216cf44.open();
            } else {
                selectUseItem();
            }
            int item = navigator_.getIndex(menuItem_.active_);
            btl::BattleMenuPlayerControl::getSingleton()->activeItem_ = item;
            return;
        }
        int item = navigator_.getIndex(menuItem_.active_);
        btl::BattleMenuPlayerControl::getSingleton()->activeItem_ = item;
    }
    int active = menuItem_.active_;
    if (MenuUpdate_Assist::isPageFlipOne(unk_ec, navigator_, active) == 2) {
        menuItem_.active_ = active;
        int item = navigator_.getIndex(menuItem_.active_);
        btl::BattleMenuPlayerControl::getSingleton()->activeItem_ = item;
        redraw_ = 1;
    }
}

THUMB void BattleMenu_ITEM::selectUseItem()
{
    int index = navigator_.getIndex(menuItem_.active_);
    int action = status::UseItem::getBattleUseAction(status::g_Party.getPlayerStatus(unk_1c)->haveStatusInfo_.haveItem_.getItem(index));
    status::UseItem::UseArea area = status::UseAction::getUseArea(action);
    switch (status::UseAction::getUseType(action)) {
    case status::UseItem::Myself:
        BattleMenuJudge::getSingleton()->setItemParty(index, unk_1c);
        BattleMenuJudge::getSingleton()->setNextPlayer();
        close();
        return;
    case status::UseItem::Friend:
        if (area == status::UseItem::One) {
            btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = 0;
            gBattleMenu_ITEMUSE2PARTY.open();
            gBattleMenu_ITEMUSE2PARTY.unk_e4 = unk_1c;
            gBattleMenu_ITEMUSE2PARTY.unk_e8 = index;
            BattleMenuJudge::getSingleton()->setItemParty(index, -1);
            close();
            return;
        }
        BattleMenuJudge::getSingleton()->setItemPartyAll(index);
        BattleMenuJudge::getSingleton()->setNextPlayer();
        close();
        return;
    case status::UseItem::Enemy:
        if (area != status::UseItem::All) {
            if (g_monster.getGroupCount() == 1) {
                int group = 0;
                for (int i = 0; i < g_monster.getCount(); i++) {
                    if (g_monster.getMonsterStatus(i)->isBattleEnable()) {
                        group = g_monster.getMonsterGroup(i);
                        break;
                    }
                }
                BattleMenuJudge::getSingleton()->setItemEnemy(index, group);
                BattleMenuJudge::getSingleton()->setNextPlayer();
            } else {
                BattleMonsterNamePlate::getSingleton().init();
                BattleMonsterNamePlate::getSingleton().setMonster();
                int target = BattleMenuJudge::getSingleton()->getLiveMonsterID();
                btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = target;
                gBattleMenu_ITEMUSE2ENEMY.open();
                gBattleMenu_ITEMUSE2ENEMY.unk_148 = unk_1c;
                gBattleMenu_ITEMUSE2ENEMY.unk_14c = index;
            }
            close();
            return;
        }
        BattleMenuJudge::getSingleton()->setItemEnemyAll(index);
        BattleMenuJudge::getSingleton()->setNextPlayer();
        close();
        return;
    default:
        status::g_Party.getPlayerStatus(unk_1c);
        BattleMenuJudge::getSingleton()->setItemPartyAll(index);
        BattleMenuJudge::getSingleton()->setNextPlayer();
        close();
        return;
    }
}

THUMB bool BattleMenu_ITEM::isSelectEquipEnable()
{
    int index = btl::BattleMenuPlayerControl::getSingleton()->activeItem_;
    int item = status::g_Party.getPlayerStatus(unk_1c)->haveStatusInfo_.haveItem_.getItem(index);
    return status::g_Party.getPlayerStatus(unk_1c)->haveStatusInfo_.isEquipEnable(item);
}
