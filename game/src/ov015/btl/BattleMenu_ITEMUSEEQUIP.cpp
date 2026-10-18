#pragma ipa file
#include "ov015/btl/BattleMenu.hpp"
#include "main/dss/DssUtils.hpp"
#include "ov015/btl/BattleMenuPlayerControl.hpp"
#include "ov003/btl/BattleMonsterMask.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/sound/MenuSoundManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/UseAction.hpp"
#include "main/status/UseItem.hpp"
#include "main/text/TextAPI.hpp"
#include "main/menu/MenuTemplate_Common.hpp"

THUMB void BattleMenu_ITEMUSEEQUIP::menuSetup()
{
    status::g_Party.setBattleMode();
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    useEquipItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    BattleMonsterMask::getSingleton()->select(-1);
    activeItem_ = btl::BattleMenuPlayerControl::getSingleton()->activeItem_;
    activeChara_ = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
    MenuSoundManager::getSingleton()->initialize();
    unk_154 = 0;
}

THUMB void BattleMenu_ITEMUSEEQUIP::menuExecute()
{
    static MENUITEM_DATA menu[] = {
        {1, 2, 0x48, 0xa8, 0x38, 0x10},
        {1, 2, 0x90, 0xa8, 0x38, 0x10},
        {-1, -1, 0, 0, 0, 0},
    };
    MenuTemplate_Common::COMMON_ITEM_ICON32_2x3(&menuItem_, 6, (activeItem_ - 1) % 6 + 1);
    unkfunc_0216d488();
    useEquipItem_.setMenuItem(menu, 2, 1, 2);
    MenuTemplate_battle::BATTLE_CANCEL(&cancelItem_);
}

THUMB void BattleMenu_ITEMUSEEQUIP::menuDraw()
{
    if (!g_TownMenuMessage.isOpen()) {
        status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(btl::BattleMenuPlayerControl::getSingleton()->activeChara_)->haveStatusInfo_;
        int count = info->haveItem_.getCount();
        int items[12];
        dss::memset(items, 0, sizeof(items));
        for (int i = 0; i < count; i++) {
            items[i] = info->haveItem_.getItem(i);
        }
        drawItemUseEquipMenu(items, count, btl::BattleMenuPlayerControl::getSingleton()->activeItem_ / 6);
        useEquipItem_.drawActive();
    }
}

THUMB void BattleMenu_ITEMUSEEQUIP::menuUpdate()
{
    if (MenuSoundManager::getSingleton()->isPlaySound()) {
        unk_154 = 1;
        return;
    }
    if (unk_154 == 1) {
        close();
        gBattleMenu_ITEM.open();
        gBattleMenuSub_HISTORY.update_ = 1;
    }
    if (unkfunc_0216d0c0() == 0) {
        unkfunc_0216d0ec();
        unkfunc_0216d0f0();
    }
    unkfunc_0216d488();
}

THUMB int BattleMenu_ITEMUSEEQUIP::unkfunc_0216d0c0()
{
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        redraw_ = 1;
        close();
        gBattleMenu_ITEM.open();
        return 1;
    }
    return 0;
}

THUMB void BattleMenu_ITEMUSEEQUIP::unkfunc_0216d0ec()
{
}

THUMB void BattleMenu_ITEMUSEEQUIP::unkfunc_0216d0f0()
{
    if (g_TownMenuMessage.isOpen()) {
        if ((unsigned int)(g_TownMenuMessage.stat_ - 1) <= 1) {
            g_TownMenuMessage.close();
        }
        return;
    }
    useEquipItem_.execInput();
    switch (useEquipItem_.result_) {
    case menu::MenuItem::MENUITEM_RESULT_CHANGE:
        redraw_ = 1;
        return;
    case menu::MenuItem::MENUITEM_RESULT_OK:
        useEquipItem_.result_ = 0;
        useEquipItem_.lastresult_ = 0;
        redraw_ = 1;
        if (useEquipItem_.active_ == 0) {
            selectUseItem();
            return;
        }
        selectEquip();
        redraw_ = 1;
        if (unk_154 == 0) {
            close();
            gBattleMenu_ITEM.open();
        }
        return;
    case menu::MenuItem::MENUITEM_RESULT_UP:
        useEquipItem_.active_ = 1;
        return;
    case menu::MenuItem::MENUITEM_RESULT_DOWN:
        useEquipItem_.active_ = 0;
        return;
    }
}

THUMB void BattleMenu_ITEMUSEEQUIP::selectUseItem()
{
    int action = status::UseItem::getBattleUseAction(status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_.getItem(activeItem_));
    status::UseItem::UseArea area = status::UseAction::getUseArea(action);
    switch (status::UseAction::getUseType(action)) {
    case status::UseItem::Myself:
        BattleMenuJudge::getSingleton()->setItemParty(activeItem_, activeChara_);
        BattleMenuJudge::getSingleton()->setNextPlayer();
        close();
        return;
    case status::UseItem::Friend:
        if (area == status::UseItem::One) {
            btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = 0;
            gBattleMenu_ITEMUSE2PARTY.open();
            int item = activeItem_;
            int chara = activeChara_;
            gBattleMenu_ITEMUSE2PARTY.activeChara_ = chara;
            gBattleMenu_ITEMUSE2PARTY.activeItem_ = item;
            close();
            return;
        }
        BattleMenuJudge::getSingleton()->setItemPartyAll(activeItem_);
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
                BattleMenuJudge::getSingleton()->setItemEnemy(activeItem_, group);
                BattleMenuJudge::getSingleton()->setNextPlayer();
            } else {
                BattleMonsterNamePlate::getSingleton().init();
                BattleMonsterNamePlate::getSingleton().setMonster();
                int target = BattleMenuJudge::getSingleton()->getLiveMonsterID();
                btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = target;
                gBattleMenu_ITEMUSE2ENEMY.open();
                int item = activeItem_;
                int chara = activeChara_;
                gBattleMenu_ITEMUSE2ENEMY.activeChara_ = chara;
                gBattleMenu_ITEMUSE2ENEMY.activeItem_ = item;
            }
            close();
            return;
        }
        BattleMenuJudge::getSingleton()->setItemEnemyAll(activeItem_);
        BattleMenuJudge::getSingleton()->setNextPlayer();
        close();
        return;
    default:
        status::g_Party.getPlayerStatus(activeChara_)->haveBattleStatus_.setSelectCommand(status::HaveBattleStatus::UseItem, activeItem_);
        BattleMenuJudge::getSingleton()->setNextPlayer();
        close();
        return;
    }
}

THUMB void BattleMenu_ITEMUSEEQUIP::selectEquip()
{
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_;
    if (info->haveEquipment_.isSpell(status::UseItem::getItemType(info->haveItem_.getItem(activeItem_)))) {
        MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_NOROI);
        int equipment = info->haveEquipment_.getEquipment(status::UseItem::getItemType(info->haveItem_.getItem(activeItem_)));
        g_TownMenuMessage.openMessageForBATTLE();
        TextAPI::setMACRO0(7, 0x40000000, equipment);
        g_TownMenuMessage.addMessage(0xc3d8d);
        unk_154 = 1;
        return;
    }
    if (!info->haveItem_.isEquipment(activeItem_)) {
        if (status::UseItem::isCurse(info->haveItem_.getItem(activeItem_)) && info->haveStatus_.playerIndex_ != 25) {
            MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_NOROI);
            g_TownMenuMessage.openMessageForBATTLE();
            TextAPI::setMACRO0(10, 0x40000000, info->haveItem_.getItem(activeItem_));
            g_TownMenuMessage.addMessage(0xc3d8b);
            gBattleMenuSub_HISTORY.update_ = 0;
            unk_154 = 1;
            if (info->haveItem_.getItem(activeItem_) == 0x5a) {
                info->statusChange_.setup2(status::StatusChange::StatusConfusion, 0);
            }
        }
        status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.setEquipment(activeItem_);
    }
}

THUMB void BattleMenu_ITEMUSEEQUIP::unkfunc_0216d488()
{
    int index = btl::BattleMenuPlayerControl::getSingleton()->activeItem_;
    int item = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.haveItem_.getItem(index);
    equipEnable_ = status::g_Party.getPlayerStatus(activeChara_)->haveStatusInfo_.isEquipEnable(item);
}
