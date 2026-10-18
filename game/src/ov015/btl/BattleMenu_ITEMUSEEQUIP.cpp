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

THUMB void UnkBattleMenu_0216cf44::menuSetup()
{
    status::g_Party.setBattleMode();
    cancelItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_CANCEL, menu::MenuItem::CURSORTYPE_NONE);
    unk_e4.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD, menu::MenuItem::CURSORTYPE_ACTIVE);
    BattleMonsterMask::getSingleton()->select(-1);
    unk_14c = btl::BattleMenuPlayerControl::getSingleton()->activeItem_;
    unk_148 = btl::BattleMenuPlayerControl::getSingleton()->activeChara_;
    MenuSoundManager::getSingleton()->initialize();
    unk_154 = 0;
}

THUMB void UnkBattleMenu_0216cf44::menuExecute()
{
    static MENUITEM_DATA menu[] = {
        {1, 2, 0x48, 0xa8, 0x38, 0x10},
        {1, 2, 0x90, 0xa8, 0x38, 0x10},
        {-1, -1, 0, 0, 0, 0},
    };
    MenuTemplate_Common::COMMON_ITEM_ICON32_2x3(&menuItem_, 6, (unk_14c - 1) % 6 + 1);
    unkfunc_0216d488();
    unk_e4.setMenuItem(menu, 2, 1, 2);
    MenuTemplate_battle::BATTLE_CANCEL(&cancelItem_);
}

THUMB void UnkBattleMenu_0216cf44::menuDraw()
{
    if (!data_020ed1bc.isOpen()) {
        status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(btl::BattleMenuPlayerControl::getSingleton()->activeChara_)->haveStatusInfo_;
        int count = info->haveItem_.getCount();
        int items[12];
        dss::memset(items, 0, sizeof(items));
        for (int i = 0; i < count; i++) {
            items[i] = info->haveItem_.getItem(i);
        }
        unkfunc_0216ba78(items, count, btl::BattleMenuPlayerControl::getSingleton()->activeItem_ / 6);
        unk_e4.drawActive();
    }
}

THUMB void UnkBattleMenu_0216cf44::menuUpdate()
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

THUMB int UnkBattleMenu_0216cf44::unkfunc_0216d0c0()
{
    if (MenuUpdate_Assist::isCancel(cancelItem_)) {
        redraw_ = 1;
        close();
        gBattleMenu_ITEM.open();
        return 1;
    }
    return 0;
}

THUMB void UnkBattleMenu_0216cf44::unkfunc_0216d0ec()
{
}

THUMB void UnkBattleMenu_0216cf44::unkfunc_0216d0f0()
{
    if (data_020ed1bc.isOpen()) {
        if ((unsigned int)(data_020ed1bc.stat_ - 1) <= 1) {
            data_020ed1bc.close();
        }
        return;
    }
    unk_e4.execInput();
    switch (unk_e4.result_) {
    case menu::MenuItem::MENUITEM_RESULT_CHANGE:
        redraw_ = 1;
        return;
    case menu::MenuItem::MENUITEM_RESULT_OK:
        unk_e4.result_ = 0;
        unk_e4.lastresult_ = 0;
        redraw_ = 1;
        if (unk_e4.active_ == 0) {
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
        unk_e4.active_ = 1;
        return;
    case menu::MenuItem::MENUITEM_RESULT_DOWN:
        unk_e4.active_ = 0;
        return;
    }
}

THUMB void UnkBattleMenu_0216cf44::selectUseItem()
{
    int action = status::UseItem::getBattleUseAction(status::g_Party.getPlayerStatus(unk_148)->haveStatusInfo_.haveItem_.getItem(unk_14c));
    status::UseItem::UseArea area = status::UseAction::getUseArea(action);
    switch (status::UseAction::getUseType(action)) {
    case status::UseItem::Myself:
        BattleMenuJudge::getSingleton()->setItemParty(unk_14c, unk_148);
        BattleMenuJudge::getSingleton()->setNextPlayer();
        close();
        return;
    case status::UseItem::Friend:
        if (area == status::UseItem::One) {
            btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = 0;
            gBattleMenu_ITEMUSE2PARTY.open();
            int item = unk_14c;
            int chara = unk_148;
            gBattleMenu_ITEMUSE2PARTY.unk_e4 = chara;
            gBattleMenu_ITEMUSE2PARTY.unk_e8 = item;
            close();
            return;
        }
        BattleMenuJudge::getSingleton()->setItemPartyAll(unk_14c);
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
                BattleMenuJudge::getSingleton()->setItemEnemy(unk_14c, group);
                BattleMenuJudge::getSingleton()->setNextPlayer();
            } else {
                BattleMonsterNamePlate::getSingleton().init();
                BattleMonsterNamePlate::getSingleton().setMonster();
                int target = BattleMenuJudge::getSingleton()->getLiveMonsterID();
                btl::BattleMenuPlayerControl::getSingleton()->targetChara_ = target;
                gBattleMenu_ITEMUSE2ENEMY.open();
                int item = unk_14c;
                int chara = unk_148;
                gBattleMenu_ITEMUSE2ENEMY.unk_148 = chara;
                gBattleMenu_ITEMUSE2ENEMY.unk_14c = item;
            }
            close();
            return;
        }
        BattleMenuJudge::getSingleton()->setItemEnemyAll(unk_14c);
        BattleMenuJudge::getSingleton()->setNextPlayer();
        close();
        return;
    default:
        status::g_Party.getPlayerStatus(unk_148)->haveBattleStatus_.setSelectCommand(status::HaveBattleStatus::UseItem, unk_14c);
        BattleMenuJudge::getSingleton()->setNextPlayer();
        close();
        return;
    }
}

THUMB void UnkBattleMenu_0216cf44::selectEquip()
{
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(unk_148)->haveStatusInfo_;
    if (info->haveEquipment_.isSpell(status::UseItem::getItemType(info->haveItem_.getItem(unk_14c)))) {
        MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_NOROI);
        int equipment = info->haveEquipment_.getEquipment(status::UseItem::getItemType(info->haveItem_.getItem(unk_14c)));
        data_020ed1bc.openMessageForBATTLE();
        TextAPI::setMACRO0(7, 0x40000000, equipment);
        data_020ed1bc.addMessage(0xc3d8d);
        unk_154 = 1;
        return;
    }
    if (!info->haveItem_.isEquipment(unk_14c)) {
        if (status::UseItem::isCurse(info->haveItem_.getItem(unk_14c)) && info->haveStatus_.playerIndex_ != 25) {
            MenuSoundManager::getSingleton()->setPlaySound(MenuSoundManager::MENU_SOUND_NOROI);
            data_020ed1bc.openMessageForBATTLE();
            TextAPI::setMACRO0(10, 0x40000000, info->haveItem_.getItem(unk_14c));
            data_020ed1bc.addMessage(0xc3d8b);
            gBattleMenuSub_HISTORY.update_ = 0;
            unk_154 = 1;
            if (info->haveItem_.getItem(unk_14c) == 0x5a) {
                info->statusChange_.setup2(status::StatusChange::StatusConfusion, 0);
            }
        }
        status::g_Party.getPlayerStatus(unk_148)->haveStatusInfo_.setEquipment(unk_14c);
    }
}

THUMB void UnkBattleMenu_0216cf44::unkfunc_0216d488()
{
    int index = btl::BattleMenuPlayerControl::getSingleton()->activeItem_;
    int item = status::g_Party.getPlayerStatus(unk_148)->haveStatusInfo_.haveItem_.getItem(index);
    unk_150 = status::g_Party.getPlayerStatus(unk_148)->haveStatusInfo_.isEquipEnable(item);
}
