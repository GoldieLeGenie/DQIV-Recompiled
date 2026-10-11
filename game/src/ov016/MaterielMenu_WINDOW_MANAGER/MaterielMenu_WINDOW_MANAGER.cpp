#include "ov016/MaterielMenu_WINDOW_MANAGER/MaterielMenu_WINDOW_MANAGER.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/cmn/TalkSoundManager.hpp"
#include "main/global/Global.hpp"
#include "main/status/StageStatus.hpp"
#include "ov003/btl/BattleActorManager2.hpp"
#include "ov016/UnkMaterielMenu_0216b304/UnkMaterielMenu_0216b304.hpp"
#include "main/menu/UnkMenuOverlay.hpp"

THUMB MaterielMenu_WINDOW_MANAGER* MaterielMenu_WINDOW_MANAGER::getSingleton()
{
    static MaterielMenu_WINDOW_MANAGER windowManager;
    return &windowManager;
}

THUMB void MaterielMenu_WINDOW_MANAGER::openMaterielWindow(int menuType)
{
    menuType_ = (MATERIEL_MENU_WINDOW)menuType;
    int chara = g_cmnPartyInfo.ctrlID_;
    switch (menuType) {
    case MENU_INN:
    case MENU_INN_TYPE2:
        if (extraInnType_ != TYPE_BATTLE) {
            MaterielMenuPlayerControl::getSingleton()->allClear();
            ui_MsgSndSet(cmn::g_talkSound.getCharacterVoice(chara));
        }
        unkfunc_02039460(1);
        gMaterielMenu_INN_ROOT.open();
        break;
    case MENU_CHURCH:
    case MENU_CHURCH_TYPE2:
        unkfunc_02039460(2);
        ui_MsgSndSet(cmn::g_talkSound.getCharacterVoice(chara));
        gMaterielMenu_CHURCH_ROOT.open();
        break;
    case MENU_CHURCH_MIRACLE:
        unkfunc_02039460(2);
        gMaterielMenu_CHURCH_MIRACLE.open();
        break;
    case MENU_SHOP_WEAPON:
    case MENU_SHOP_PROTECTOR:
    case MENU_SHOP_ITEM:
    case MENU_SHOP_WEAPON_TYPE2:
    case MENU_SHOP_PROTECTOR_TYPE2:
    case MENU_SHOP_ITEM_TYPE2:
        unkfunc_02039460(3);
        if (extraInnType_ != TYPE_BATTLE) {
            ui_MsgSndSet(cmn::g_talkSound.getCharacterVoice(chara));
        }
        MaterielMenu_SHOP_MANAGER::getSingleton()->allClear();
        MaterielMenu_SHOP_MANAGER::getSingleton()->openShopMenu(menuType);
        break;
    case MENU_CHANGE_GIFT:
    case MENU_CHANGE_GIFT_TYPE2:
        unkfunc_02039460(3);
        ui_MsgSndSet(cmn::g_talkSound.getCharacterVoice(chara));
        MaterielMenu_SHOP_MANAGER::getSingleton()->allClear();
        MaterielMenu_SHOP_MANAGER::getSingleton()->setShopType(menuType);
        gMaterielMenu_CHANGEGIFT_ROOT.open();
        break;
    case MENU_COIN_SALEROOM:
        unkfunc_02039460(4);
        ui_MsgSndSet(cmn::g_talkSound.getCharacterVoice(chara));
        gMaterielMenu_COINSALEROOM_ROOT.open();
        break;
    case MENU_CASINO_SLOT:
        gMaterielMenu_SLOT.open();
        break;
    case MENU_CASINO_SLOT_ENTER:
        gMaterielMenu_SlotEnter.open();
        break;
    case MENU_CASINO_POKER:
        gMaterielMenu_POKER_BETCOIN.open();
        break;
    case MENU_CASINO_FIGHTSTADIUM:
        ui_MsgSndSet(cmn::g_talkSound.getCharacterVoice(chara));
        gMaterielMenu_FIGHT_STADIUM.open();
        break;
    case MENU_BANK:
        unkfunc_02039460(5);
        ui_MsgSndSet(cmn::g_talkSound.getCharacterVoice(chara));
        gMaterielMenu_BANK_ROOT.open();
        break;
    case MENU_MEDAL_KING:
        unkfunc_02039460(6);
        ui_MsgSndSet(cmn::g_talkSound.getCharacterVoice(chara));
        gMaterielMenu_MEDAL_KING.open();
        break;
    case MENU_EXTRA_FOX_TOWN:
        unkfunc_02039460(3);
        ui_MsgSndSet(cmn::g_talkSound.getCharacterVoice(chara));
        MaterielMenu_SHOP_MANAGER::getSingleton()->allClear();
        MaterielMenu_SHOP_MANAGER::getSingleton()->openShopMenu(MENU_SHOP_ITEM);
        MaterielMenu_SHOP_MANAGER::getSingleton()->setExtraShop(1);
        break;
    case MENU_EXTRA_BONMOL_CASTLE:
        unkfunc_02039460(3);
        ui_MsgSndSet(cmn::g_talkSound.getCharacterVoice(chara));
        MaterielMenu_SHOP_MANAGER::getSingleton()->allClear();
        MaterielMenu_SHOP_MANAGER::getSingleton()->setExtraShop(2);
        gMaterielMenu_SHOP_WHO_SELL.open();
        break;
    case MENU_EXTRA_IMUL:
        break;
    case MENU_EXTRA_PRESENT_EXP:
        unkfunc_02039460(7);
        gMaterielMenu_EXTRA_PRESENT_EXP.open();
        break;
    case MENU_EXTRA_COLOSSEUM:
        unkfunc_02039460(8);
        gMaterielMenu_MARTIAL_COLOSSEUM.open();
        break;
    case MENU_EXTRA_HOSTAGE:
        unkfunc_02039460(9);
        ui_MsgSndSet(cmn::g_talkSound.getCharacterVoice(chara));
        gMaterielMenuExtra_ChangeHostage.open();
        break;
    case MENU_EXTRA_NENE:
        unkfunc_02039460(10);
        ui_MsgSndSet(cmn::g_talkSound.getCharacterVoice(chara));
        gMaterielMenu_EXTRA_NENE.open();
        break;
    case MENU_EXTRA_CHAPTER_TITLE:
        unkfunc_02039460(11);
        gMaterielMenu_EXTRA_CHAPTER_TITLE.setChapterTitleInfo(chapter_, titleFlag_);
        gMaterielMenu_EXTRA_CHAPTER_TITLE.open();
        break;
    case MENU_SAVE:
        unkfunc_02039460(2);
        gMaterielMenu_SAVE.open();
        if (type_ == 0) {
            gMaterielMenu_SAVE.saveType_ = MaterielMenu_SAVE::TYPE_CHAPTER;
        }
        if (type_ == 1) {
            ui_MsgSndSet(cmn::g_talkSound.getCharacterVoice(chara));
            gMaterielMenu_SAVE.saveType_ = MaterielMenu_SAVE::TYPE_LASTDUNGEON;
        }
        if (type_ == 2) {
            gMaterielMenu_SAVE.saveType_ = MaterielMenu_SAVE::TYPE_CLEAR;
        }
        if (type_ == 3) {
            gMaterielMenu_SAVE.saveType_ = MaterielMenu_SAVE::TYPE_SURECHIGAI;
        }
        break;
    case MENU_EXTRA_SIDEJOB:
        unkfunc_02039460(12);
        ui_MsgSndSet(cmn::g_talkSound.getCharacterVoice(chara));
        gMaterielMenu_EXTRA_SIDEJOB_ROOT.open();
        break;
    case MENU_SURECHIGAI_MAKE_TAISHI:
        unkfunc_02039460(13);
        gMaterielMenu_SURECHIGAI_SELECT_OBJECT.open();
        break;
    case MENU_SURECHIGAI_ROOT:
        unkfunc_02039460(13);
        ui_MsgSndSet(cmn::g_talkSound.getCharacterVoice(chara));
        gMaterielMenu_SURECHIGAI_ROOT.open();
        break;
    case MENU_SURECHIGAI_MAP_NAME:
        unkfunc_02039460(13);
        gMaterielMenu_NameEdit.open();
        gMaterielMenu_NameEdit.clearName();
        if (editType_ == EDIT_TOWN_NAME) {
            gMaterielMenu_NameEdit.returnMenu_ = MaterielMenu_NameEdit::RETURN_MENU_SURECHIGAI_TOWNNAME;
            gMaterielMenu_NameEdit.setTownNameMode();
        } else if (editType_ == EDIT_MESSAGE) {
            gMaterielMenu_NameEdit.returnMenu_ = MaterielMenu_NameEdit::RETURN_MENU_SURECHIGAI_MESSAGE;
            gMaterielMenu_NameEdit.unkfunc_0203b004();
        }
        break;
    }
    endWindow_ = 0;
}

THUMB void MaterielMenu_WINDOW_MANAGER::closeMaterielWindow()
{
    switch (menuType_) {
    case MENU_INN:
    case MENU_INN_TYPE2:
        gMaterielMenu_INN_ROOT.close();
        gMaterielMenu_INN_ROOT.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        g_Stage.crusingPeopleEncount_ = 0;
        break;
    case MENU_CHURCH:
    case MENU_CHURCH_TYPE2:
    case MENU_CHURCH_MIRACLE:
        gMaterielMenu_CHURCH_ROOT.close();
        gMaterielMenu_CHURCH_MIRACLE.close();
        gMaterielMenu_CHURCH_ROOT.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        break;
    case MENU_SHOP_WEAPON:
    case MENU_SHOP_PROTECTOR:
    case MENU_SHOP_ITEM:
    case MENU_SHOP_WEAPON_TYPE2:
    case MENU_SHOP_PROTECTOR_TYPE2:
    case MENU_SHOP_ITEM_TYPE2:
    case MENU_EXTRA_FOX_TOWN:
        MaterielMenu_SHOP_MANAGER::getSingleton()->setExtraShop(0);
        gMaterielMenu_SHOP_ROOT.close();
        gMaterielMenu_SHOP_ROOT.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        g_Stage.crusingPeopleEncount_ = 0;
        break;
    case MENU_CHANGE_GIFT:
    case MENU_CHANGE_GIFT_TYPE2:
        gMaterielMenu_CHANGEGIFT_ROOT.close();
        gMaterielMenu_CHANGEGIFT_ROOT.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        break;
    case MENU_COIN_SALEROOM:
        gMaterielMenu_COINSALEROOM_ROOT.close();
        gMaterielMenu_COINSALEROOM_BUY.close();
        gMaterielMenu_COINSALEROOM_ROOT.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        break;
    case MENU_CASINO_SLOT:
        gMaterielMenu_SLOT.close();
        gMaterielMenu_SLOT.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        g_Global.startTown(g_Global.getPrevMapName());
        g_Stage.returnBookFlag_ = 1;
        break;
    case MENU_CASINO_SLOT_ENTER:
        gMaterielMenu_SlotEnter.close();
        gMaterielMenu_SlotEnter.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        g_Stage.returnBookFlag_ = 1;
        break;
    case MENU_CASINO_POKER:
        gMaterielMenu_POKER_BETCOIN.close();
        gMaterielMenu_POKER_CHANGECARD.close();
        gMaterielMenu_POKER_SELECTCARD.close();
        gMaterielMenu_POKER_BETCOIN.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        gMaterielMenu_POKER_CHANGECARD.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        gMaterielMenu_POKER_SELECTCARD.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        g_Global.startTown(g_Global.getPrevMapName());
        g_Stage.returnBookFlag_ = 1;
        break;
    case MENU_CASINO_FIGHTSTADIUM:
        gMaterielMenu_FIGHT_STADIUM.close();
        gMaterielMenu_FIGHT_STADIUM.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        break;
    case MENU_BANK:
        gMaterielMenu_BANK_ROOT.close();
        gMaterielMenu_BANK_DRAW.close();
        gMaterielMenu_BANK_PUTIN.close();
        gMaterielMenu_BANK_ROOT.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        break;
    case MENU_MEDAL_KING:
        gMaterielMenu_MEDAL_KING.close();
        gMaterielMenu_MEDAL_KING.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        break;
    case MENU_EXTRA_BONMOL_CASTLE:
        gUnkMaterielMenu_0216b304.close();
        gMaterielMenu_SHOP_WHO_SELL.close();
        gUnkMaterielMenu_0216b304.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        gMaterielMenu_SHOP_WHO_SELL.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        break;
    case MENU_EXTRA_IMUL:
    case MENU_EXTRA_HOSTAGE:
        break;
    case MENU_EXTRA_PRESENT_EXP:
        gMaterielMenu_EXTRA_PRESENT_EXP.close();
        gMaterielMenu_EXTRA_PRESENT_EXP.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        break;
    case MENU_EXTRA_COLOSSEUM:
        gMaterielMenu_MARTIAL_COLOSSEUM.close();
        gMaterielMenu_MARTIAL_COLOSSEUM.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        break;
    case MENU_EXTRA_NENE:
        gMaterielMenu_EXTRA_NENE.close();
        gMaterielMenu_EXTRA_NENE.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        break;
    case MENU_EXTRA_CHAPTER_TITLE:
        gMaterielMenu_EXTRA_CHAPTER_TITLE.close();
        gMaterielMenu_EXTRA_CHAPTER_TITLE.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        break;
    case MENU_SAVE:
        gMaterielMenu_SAVE.close();
        gMaterielMenu_SAVE.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        if (type_ == 3 && surechigaiStart_ == 0) {
            gMaterielMenu_SURECHIGAI_ROOT.open();
            menuType_ = MENU_SURECHIGAI_ROOT;
            return;
        }
        break;
    case MENU_EXTRA_SIDEJOB:
        gMaterielMenu_EXTRA_SIDEJOB_ROOT.close();
        gMaterielMenu_EXTRA_SIDEJOB_ROOT.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        gMaterielMenu_EXTRA_SIDEJOB_BUY.close();
        gMaterielMenu_EXTRA_SIDEJOB_BUY.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        gMaterielMenu_EXTRA_SIDEJOB_SELL.close();
        gMaterielMenu_EXTRA_SIDEJOB_SELL.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        break;
    case MENU_SURECHIGAI_MAKE_TAISHI:
        gMaterielMenu_SURECHIGAI_SELECT_OBJECT.close();
        gMaterielMenu_SURECHIGAI_SELECT_OBJECT.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        gUnkMaterielMenu_02189a80.close();
        gUnkMaterielMenu_02189a80.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        gMaterielMenu_SURECHIGAI_ROOT.close();
        gMaterielMenu_SURECHIGAI_ROOT.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        break;
    case MENU_SURECHIGAI_ROOT:
        gMaterielMenu_SURECHIGAI_SELECT_OBJECT.close();
        gMaterielMenu_SURECHIGAI_SELECT_OBJECT.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        gMaterielMenu_SURECHIGAI_ROOT.close();
        gMaterielMenu_SURECHIGAI_ROOT.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        break;
    case MENU_SURECHIGAI_MAP_NAME:
        gMaterielMenu_NameEdit.close();
        gMaterielMenu_NameEdit.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        break;
    }
    if (extraInnType_ != TYPE_BATTLE) {
        MaterielMenuPlayerControl::getSingleton()->allClear();
    }
    endWindow_ = 1;
}
