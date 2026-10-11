#pragma ipa file
#include "main/Commands/CommonCommand.hpp"
#include "main/status/GameFlag.hpp"
#include "main/cmn/CommonRuraData.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/object/DisplayCharacter.hpp"
#include "ov000/town/TownCharacter.hpp"
#include "main/btl/BattleScriptManager.hpp"
#include "main/status/BattleResult.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/cmn/GameManager.hpp"
#include "main/cmn/ExtraMapLink.hpp"
#include "main/cmn/CommonCounterInfo.hpp"
#include "main/global/Global.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/dss/Random.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/BattleHistory.hpp"
#include "main/cmn/PartyTalk.hpp"
#include "main/cmn/PlayerManager.hpp"
#include "main/cmn/TalkSoundManager.hpp"
#include "main/cmn/HengeNoTsueManager.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownCharacterManager.hpp"
#include "main/cmn/CommonChapterTitle.hpp"
#include "main/Commands/CommonScriptCommand.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/menu/MaterielMenuWindowManager.hpp"
#include "main/script/ScriptBaseCommand.hpp"
#include "ov000/town/TownSystem.hpp"
#include "ov000/town/TownWindowSystem.hpp"
#include "main/sound/Sound.hpp"
#include "main/cmn/UnkImmigrantTown.hpp"

THUMB int cmd_get_flag(int* param)
{
    switch (param[0]) {
    case 0:
        if (param[2] != 0) {
            if (g_AreaFlag.check(param[1])) {
                return 1;
            }
        } else if (!g_AreaFlag.check(param[1])) {
            return 1;
        }
        return 0;
    case 1:
        if (param[2] != 0) {
            if (g_LocalFlag.check(param[1])) {
                return 1;
            }
        } else if (!g_LocalFlag.check(param[1])) {
            return 1;
        }
        return 0;
    case 2:
        if (param[2] != 0) {
            if (g_GlobalFlag.check(param[1])) {
                return 1;
            }
        } else if (!g_GlobalFlag.check(param[1])) {
            return 1;
        }
        return 0;
    }
    return 0;
}

THUMB int cmd_set_flag(int* param)
{
    switch (param[0]) {
    case 0:
        if (param[2] != 0) {
            cmn::CommonRuraData::getSingleton();
            int id = param[1];
            if ((unsigned int)id < 0x2a) {
                cmn::CommonRuraData::getSingleton()->setEnableRuraMap(id);
            }
            g_AreaFlag.set(param[1]);
        } else {
            g_AreaFlag.remove(param[1]);
        }
        break;
    case 1:
        if (param[2] != 0) {
            g_LocalFlag.set(param[1]);
        } else {
            g_LocalFlag.remove(param[1]);
        }
        break;
    case 2:
        if (param[2] != 0) {
            g_GlobalFlag.set(param[1]);
        } else {
            g_GlobalFlag.remove(param[1]);
        }
        break;
    }
    return 1;
}

THUMB void __cmd_fade_in::initialize(char* scriptParam)
{
    PARAM_FADEIN* param = (PARAM_FADEIN*)scriptParam;
    g_Global.fadeIn(param->frame);
    count_ = 0;
    countFrame_ = param->frame;
}

THUMB void __cmd_fade_in::execute()
{
    count_++;
}

THUMB int __cmd_fade_in::isEnd()
{
    if (count_ >= countFrame_) {
        return true;
    }
    return false;
}

__cmd_fade_in g_cmd_fade_in;

THUMB void __cmd_fade_out::initialize(char* scriptParam)
{
    PARAM_FADEOUT* param = (PARAM_FADEOUT*)scriptParam;
    if (param->type == 0) {
        g_Global.fadeOutBlack(param->frame);
    } else {
        g_Global.fadeOutWhite(param->frame);
    }
    count_ = 0;
    countFrame_ = param->frame;
}

THUMB void __cmd_fade_out::execute()
{
    count_++;
}

THUMB int __cmd_fade_out::isEnd()
{
    if (count_ >= countFrame_) {
        return true;
    }
    return false;
}

__cmd_fade_out g_cmd_fade_out;

THUMB void __cmd_fade_in2::initialize(char* scriptParam)
{
    PARAM_FADEIN* param = (PARAM_FADEIN*)scriptParam;
    int frame = param->frame;
    switch (data_020f21f8.state_) {
        case GlobalFade::FADE_NONE:
            data_020f21f8.state_ = GlobalFade::FADE_OUT_BLACK;
            break;
        case GlobalFade::FADE_IN_BLACK:
            data_020f21f8.state_ = GlobalFade::FADE_OUT_WHITE;
            break;
        default:
            data_020f21f8.state_ = GlobalFade::FADE_OUT_BLACK;
            break;
    }
    data_020f21f8.count_ = 0;
    data_020f21f8.frames_ = frame;
    data_0210bc18.unkfunc_02058294(&data_020f21f8);
    count_ = 0;
    countFrame_ = param->frame;
}

THUMB void __cmd_fade_in2::execute()
{
    count_++;
}

THUMB int __cmd_fade_in2::isEnd()
{
    if (count_ >= countFrame_) {
        return true;
    }
    return false;
}

__cmd_fade_in2 g_cmd_fade_in2;

THUMB void __cmd_fade_out2::initialize(char* scriptParam)
{
    PARAM_FADEOUT* param = (PARAM_FADEOUT*)scriptParam;
    if (param->type == 0) {
        int frame = param->frame;
        data_020f21f8.state_ = GlobalFade::FADE_NONE;
        data_020f21f8.count_ = 0;
        data_020f21f8.frames_ = frame;
        data_020f21f8.sprite_[0].setColor(0, 0, 0);
        data_020f21f8.sprite_[1].setColor(0, 0, 0);
        data_0210bc18.unkfunc_02058294(&data_020f21f8);
    } else {
        int frame = param->frame;
        data_020f21f8.state_ = GlobalFade::FADE_IN_BLACK;
        data_020f21f8.count_ = 0;
        data_020f21f8.frames_ = frame;
        data_020f21f8.sprite_[0].setColor(0x1f, 0x1f, 0x1f);
        data_020f21f8.sprite_[1].setColor(0x1f, 0x1f, 0x1f);
        data_0210bc18.unkfunc_02058294(&data_020f21f8);
    }
    count_ = 0;
    countFrame_ = param->frame;
}

THUMB void __cmd_fade_out2::execute()
{
    count_++;
}

THUMB int __cmd_fade_out2::isEnd()
{
    if (count_ >= countFrame_) {
        return true;
    }
    return false;
}

__cmd_fade_out2 g_cmd_fade_out2;

THUMB void __cmd_message1_self_closing::initialize(char* scriptParam)
{
    PARAM_MESSAGE1_SELF_CLOSING* param = (PARAM_MESSAGE1_SELF_CLOSING*)scriptParam;
    count_ = 0;
    countFrame_ = param->frame;
    cmn::g_talkSound.setMessageSound(1, -1);
    data_020ed1bc.openMessageForTALK();
    data_020ed1bc.addMessage(param->message);
    if (!g_HengeNoTsue.isMonster()) {
        cmn::PartyTalk::getSingleton()->setPreMessageNo(param->message);
    }
    cmn::GameManager::getSingleton()->playerManager_->setLock(1);
}

THUMB void __cmd_message1_self_closing::execute()
{
    count_++;
}

THUMB int __cmd_message1_self_closing::isEnd()
{
    if (count_ >= countFrame_) {
        data_020ed1bc.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        data_020ed1bc.close();
        cmn::GameManager::getSingleton()->playerManager_->setLock(0);
        return true;
    }
    return false;
}

__cmd_message1_self_closing g_cmd_message1_self_closing;

THUMB void __cmd_speak_to_player_self_closing::initialize(char* scriptParam)
{
    PARAM_SPEAK_TO_PLAYER_SELF_CLOSING* param = (PARAM_SPEAK_TO_PLAYER_SELF_CLOSING*)scriptParam;
    count_ = 0;
    countFrame_ = param->frame;
    int ctrl = getPlacementCtrlId();
    if (getObjectCount() > ctrl) {
        if (data_0210bb94.unkfunc_02058114(0xc) != 0) {
            cmn::g_talkSound.setVoice(TownCharacterManager::getSingleton()->getCharaIndex(ctrl));
        }
    } else {
        cmn::g_talkSound.setVoice(0);
        ctrl = -1;
    }
    TownCharacterManager::getSingleton()->setPlayerDirection(ctrl);
    cmn::g_talkSound.setMessageSound(1, ctrl);
    data_020ed1bc.openMessageForTALK();
    data_020ed1bc.addMessage(param->message);
    if (!g_HengeNoTsue.isMonster()) {
        cmn::PartyTalk::getSingleton()->setPreMessageNo(param->message);
    }
    cmn::GameManager::getSingleton()->playerManager_->setLock(1);
}

THUMB void __cmd_speak_to_player_self_closing::execute()
{
    count_++;
}

THUMB int __cmd_speak_to_player_self_closing::isEnd()
{
    menu::MenuBase::MENUBASE_STAT stat = data_020ed1bc.stat_;
    if (stat == menu::MenuBase::MENUBASE_STAT_OK || stat == menu::MenuBase::MENUBASE_STAT_CANCEL || count_ >= countFrame_) {
        data_020ed1bc.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        data_020ed1bc.close();
        cmn::GameManager::getSingleton()->playerManager_->setLock(0);
        return true;
    }
    return false;
}

__cmd_speak_to_player_self_closing g_cmd_speak_to_player_self_closing;

THUMB void __cmd_menu_save::initialize(char* scriptParam)
{
    PARAM_MENU_SAVE* param = (PARAM_MENU_SAVE*)scriptParam;
    waitFlag_ = 0;
    int ctrl = getPlacementCtrlId();
    if (data_0210bb94.unkfunc_02058114(0xc) == 1) {
        TownWindowSystem::getSingleton()->changeShopMenuPhase(MaterielMenu_WINDOW_MANAGER::MENU_SAVE);
    } else {
        MaterielMenu_WINDOW_MANAGER::getSingleton()->openMaterielWindow(MaterielMenu_WINDOW_MANAGER::MENU_SAVE);
    }
    MaterielMenu_WINDOW_MANAGER::getSingleton()->setSaveMenuType(param->type);
    g_cmnPartyInfo.ctrlID_ = ctrl;
    type_ = param->type;
    if (param->type == 2) {
        status::g_Party.clear();
        status::g_Party.setNormalMode();
        for (int i = 0; i < 10; i++) {
            status::g_Party.add(g_Stage.lastParty_[i]);
        }
        status::g_Story.setChapter(6);
    }
}

THUMB int __cmd_menu_save::isEnd()
{
    if (data_0210bb94.unkfunc_02058114(0xc) == 1) {
        if (!TownWindowSystem::getSingleton()->cmdWindow_.isShopMenu() && !waitFlag_) {
            waitFlag_ = 1;
            return false;
        }
        if (!TownWindowSystem::getSingleton()->cmdWindow_.isShopMenu() && waitFlag_) {
            if (type_ == 0) {
                const char* name = "zaout";
            g_Global.setMapName(name);
            }
            return true;
        }
    } else if (MaterielMenu_WINDOW_MANAGER::getSingleton()->endWindow_) {
        if (type_ == 0) {
            const char* name = "zaout";
            g_Global.setMapName(name);
        }
        return true;
    }
    return false;
}

__cmd_menu_save g_cmd_menu_save;

THUMB int cmd_encount(int* param)
{
    if (param[0] == 0) {
        encount::Encount::getSingleton()->forceBrew(param[1]);
    } else if (param[0] == 1) {
        btl::BattleScriptManager::getSingleton()->setEncountMap(param[1]);
        encount::Encount::getSingleton()->forceEventBrew(param[1]);
    }
    ScriptSystem::getSingleton()->executeEnable_ = 0;
    if (data_0210bb94.unkfunc_02058114(0xe) == 1) {
        FieldPlayerManager::getSingleton()->setLockByEventEncount(1);
    } else {
        TownPlayerManager::getSingleton()->setLockByEventEncount(1);
    }
    return 1;
}

THUMB int cmd_encount_set_flag(int* param)
{
    if (param[0] == 0) {
        encount::Encount::getSingleton()->forceBrew(param[1]);
    } else if (param[0] == 1) {
        btl::BattleScriptManager::getSingleton()->setEncountMap(param[1]);
        encount::Encount::getSingleton()->forceEventBrew(param[1]);
    }
    if (data_0210bb94.unkfunc_02058114(0xe) == 1) {
        FieldPlayerManager::getSingleton()->setLockByEventEncount(1);
    } else {
        TownPlayerManager::getSingleton()->setLockByEventEncount(1);
    }
    ScriptSystem::getSingleton()->executeEnable_ = 0;
    return 1;
}

THUMB int cmd_encount_first_strike(int* param)
{
    if (param[2] == 1) {
        status::g_BattleResult.playerFirstAttack_ = 1;
        status::g_BattleResult.monsterFirstAttack_ = 0;
    } else if (param[2] == 2) {
        status::g_BattleResult.playerFirstAttack_ = 0;
        status::g_BattleResult.monsterFirstAttack_ = 1;
    }
    if (param[0] == 0) {
        encount::Encount::getSingleton()->forceBrew(param[1]);
    } else if (param[0] == 1) {
        btl::BattleScriptManager::getSingleton()->setEncountMap(param[1]);
        encount::Encount::getSingleton()->forceEventBrew(param[1]);
    }
    if (data_0210bb94.unkfunc_02058114(0xe) == 1) {
        FieldPlayerManager::getSingleton()->setLockByEventEncount(1);
    } else {
        TownPlayerManager::getSingleton()->setLockByEventEncount(1);
    }
    ScriptSystem::getSingleton()->executeEnable_ = 0;
    return 1;
}

THUMB int cmd_set_party_join_carriage()
{
    status::g_Party.basha_ = 1;
    cmn::GameManager::getSingleton()->resetParty();
    return 1;
}

THUMB int cmd_set_town_to_field_link(int* param)
{
    dss::Fix32Vector3 offset;
    offset.set(param[2], param[3], 0);
    cmn::g_extraMapLink.setLinkData(param[5], param[1], cmn::LINK_TOWN_TO_FIELD, g_Global.getMapName(), 0, offset);
    return 1;
}

THUMB int cmn_set_field_to_town_link(int* param)
{
    cmn::g_extraMapLink.setLinkData(param[5], param[4], cmn::LINK_FIELD_TO_TOWN, 0, (const char*)param);
    return 1;
}

THUMB int cmd_is_trigger_forward(int* param)
{
    dss::Fix32Vector3 pos = cmn::GameManager::getSingleton()->playerManager_->getPosition();
    short dir = cmn::GameManager::getSingleton()->playerManager_->getDirection();
    dss::Fix32Vector3 min;
    dss::Fix32Vector3 max;
    min.set(param[0], param[1], param[2]);
    max.set(param[3], param[4], param[5]);
    dss::Fix32Vector3 diff = (min + max) / 2 - pos;
    TriggerCheck check = param[6] == 0 ? TRIGGER_CHECK_0 : TRIGGER_CHECK_1;
    int type = param[7] == 0 ? 7 : 6;
    return cmn::CommonCalculate::areaCheck(pos, dir, min, max, check, type);
}

THUMB int cmd_set_encount_disable(int* param)
{
    int flag = param[0] == 0 ? 1 : 0;
    encount::Encount::getSingleton()->enable_ = flag;
    return 1;
}

THUMB int cmd_set_encount_stage_disable(int* param)
{
    g_Stage.setEncount(param[0] == 0 ? 1 : 0);
    return 1;
}

THUMB int cmd_set_demolition_fightingarena(int* param)
{
    dss::Fix32Vector3 pos;
    pos.vx.value = param[4];
    pos.vy.value = param[5];
    pos.vz.value = param[6];
    g_Global.setFightingArenaMapName((const char*)param, pos);
    return 1;
}

THUMB int cmd_event_chapter_end(int* param)
{
    return 1;
}

THUMB int cmd_set_chapter(int* param)
{
    int top = 0;
    const char* church[6] = { "hak1f1", "mbk1f1", "mfk1f1", "mhk1f1", "cff1", "hck1f1" };
    const char* map[5] = { "caf1", "cbf2", "mfm1f1", "mhg1n1", "mlm1f1" };
    status::g_Story.setChapter(param[0]);
    dss::Fix32Vector3 pos;
    dss::Fix32Vector3 unused;
    switch (param[0]) {
    case 1: {
        top = 3;
        g_Global.setMapName(map[0]);
        g_Stage.setChurchMapName((char*)church[0]);
        pos.vx = dss::Fix32(0.0f);
        pos.vy = dss::Fix32(0.0f);
        pos.vz = dss::Fix32(-0.43f);
        status::BaseHaveItem* haveItem = &status::g_Party.getPlayerStatus(0)->haveStatusInfo_.haveItem_;
        if (status::g_Party.haveItemSack_.getCount() != 0) {
            status::HaveItemSack* sack = &status::g_Party.haveItemSack_;
            do {
                for (int i = 0; i < sack->getCount(); i++) {
                    haveItem->add(sack->getItem(i));
                    sack->execThrow(i);
                }
            } while (sack->getCount() != 0);
        }
        status::g_Party.setGold(0);
        break;
    }
    case 2:
        top = 4;
        g_Global.setMapName(map[1]);
        g_Stage.setChurchMapName((char*)church[1]);
        break;
    case 3:
        top = 7;
        g_Global.setMapName(map[2]);
        g_Stage.setChurchMapName((char*)church[2]);
        pos.vx = dss::Fix32(5.23f);
        pos.vy = dss::Fix32(0.0f);
        pos.vz = dss::Fix32(8.5f);
        break;
    case 4:
        top = 9;
        g_Global.setMapName(map[3]);
        g_Stage.setChurchMapName((char*)church[3]);
        pos.vx = dss::Fix32(9.6f);
        pos.vy = dss::Fix32(0.0f);
        pos.vz = dss::Fix32(-10.23f);
        break;
    case 5:
        top = 1;
        if (status::g_Story.sex_ == 1) {
            top = 2;
        }
        g_Global.setMapName(map[4]);
        g_Stage.setChurchMapName((char*)church[4]);
        break;
    default:
        g_Stage.setChurchMapName((char*)church[5]);
        break;
    }
    g_Stage.overviewTempPosition_ = pos;
    if ((unsigned int)param[0] <= 5) {
        g_Stage.ruraEnable_.flag_ = 0;
        status::g_Party.clear();
        status::g_Party.add(top);
        status::g_BattleHistory.setChapterBattleCount(0);
        status::g_BattleHistory.setChapterWipeoutCount(0);
        status::g_BattleHistory.setChapterEscapeCount(0);
    }
    if ((unsigned int)param[0] <= 6) {
        encount::Encount::getSingleton()->enable_ = 1;
        status::g_BattleResult.setDisablePlayerDemolition(false);
    }
    return 1;
}

THUMB int cmd_chapter_store(int* param)
{
    status::g_Party.setPlayerMode();
    int count = status::g_Party.getCount();
    for (int i = 0; i < count; i++) {
        int k;
        status::BaseHaveItem* haveItem = &status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveItem_;
        for (int j = 0; j < haveItem->getMaxCount(); j++) {
            for (k = 0; k < haveItem->getCount(); k++) {
                if (haveItem->getItem(k) == 0x85 || haveItem->getItem(k) == 0x89 ||
                    haveItem->getItem(k) == 0x8e || haveItem->getItem(k) == 0x7b) {
                    ((status::BaseHaveItem*)&status::g_Party.haveItemSack_)->add(haveItem->getItem(k));
                    haveItem->del(k);
                    break;
                }
            }
        }
    }
    status::g_Story.store();
    return 1;
}

THUMB int cmd_chapter_restore(int* param)
{
    status::g_Story.restoreSack(param[0]);
    return 1;
}

THUMB int cmd_chapter_restore_coin(int* param)
{
    status::g_Story.restoreCoin(param[0]);
    return 1;
}

THUMB int cmd_is_battle_turn(int* param)
{
    unsigned int turn = status::g_BattleResult.battleTurnCount_;
    if (param[1] == 0) {
        if ((unsigned int)param[0] < turn) {
            return 1;
        }
        return 0;
    }
    if ((unsigned int)param[0] >= turn) {
        return 1;
    }
    return 0;
}

THUMB int cmd_set_random(int* param)
{
    if (dssrand::rand(param[1]) % param[1] == 0) {
        g_GlobalFlag.set(param[0]);
    } else {
        g_GlobalFlag.remove(param[0]);
    }
    return 1;
}

THUMB int cmd_set_random2(int* param)
{
    if ((unsigned int)(dssrand::rand(param[2]) % param[2]) < (unsigned int)param[1]) {
        g_GlobalFlag.set(param[0]);
    } else {
        g_GlobalFlag.remove(param[0]);
    }
    return 1;
}

THUMB int cmd_set_music(int* param)
{
    SoundManager::play(param[0], 0xf);
    return 1;
}

THUMB int cmd_music_pause(int* param)
{
    if (param[0] == 1) {
        SoundManager::stop(0xf);
    }
    return 1;
}

THUMB int cmd_play_music_now_map(int* param)
{
    if (data_0210bb94.unkfunc_02058114(0xc) == 1) {
        SoundManager::townPlay();
    } else if (data_0210bb94.unkfunc_02058114(0xe) == 1) {
        SoundManager::fieldPlay();
    }
    return 1;
}

THUMB int cmd_set_wait_counter(int* param)
{
    cmn::g_CommonCounterInfo.setWaitZero(param[0]);
    return 1;
}

THUMB int cmd_is_wait_counter(int* param)
{
    return cmn::g_CommonCounterInfo.isEndWaitCounter();
}

THUMB int cmd_is_push_key(int* param)
{
    return cmn::g_CommonCounterInfo.checkBottun();
}

THUMB int cmd_wait_operation(int* param)
{
    return cmn::g_CommonCounterInfo.checkBottun();
}

THUMB int cmd_set_day_count(int* param)
{
    cmn::g_CommonCounterInfo.setDayCounter(param[0], param[1]);
    return 1;
}

THUMB int cmd_is_day_count(int* param)
{
    return cmn::g_CommonCounterInfo.isEndDayCounter(param[0]);
}

THUMB int cmd_add_nene_count(int* param)
{
    cmn::g_CommonCounterInfo.freeCounter_[0]++;
    return 1;
}

THUMB int cmd_map_link_field_direct(int* param)
{
    dss::Fix32Vector3 pos;
    pos.vx.value = param[1];
    pos.vy.value = param[2];
    pos.vz = 0L;
    cmn::g_extraMapLink.setExtraExitField(param[0], pos);
    if (data_0210bb94.unkfunc_02058114(0xc) != 0) {
        TownCharacterBase::allEventLock_ = 0;
        BillboardCharacter::setAllCharaAnim(1);
        TownCharacterManager::getSingleton()->restoreCharacterAnim();
        TownPlayerManager::getSingleton()->partyDraw_.setAnimation(2);
        if (param[3] == 1) {
            TownSystem::getSingleton()->playExitSE_ = 1;
        }
    } else if (param[3] == 1) {
        FieldSystem::getSingleton()->exitSound_ = 1;
    }
    return 1;
}

THUMB int cmd_floor_change(int* param)
{
    dss::Fix32Vector3 pos = cmn::CommonCalculate::setVecByParam(param[4], param[5], param[6]);
    cmn::g_extraMapLink.setExtraLinkTown((const char*)param, pos, cmn::CommonCalculate::getIdxByParam((unsigned char)param[7]));
    ScriptSystem::getSingleton()->executeEnable_ = 0;
    if (data_0210bb94.unkfunc_02058114(0xc) != 0) {
        TownCharacterBase::allEventLock_ = 0;
        BillboardCharacter::setAllCharaAnim(1);
        TownCharacterManager::getSingleton()->restoreCharacterAnim();
        TownPlayerManager::getSingleton()->partyDraw_.setAnimation(2);
        if (param[8] == 1) {
            TownSystem::getSingleton()->playExitSE_ = 1;
        }
    } else if (param[8] == 1) {
        FieldSystem::getSingleton()->exitSound_ = 1;
    }
    return 1;
}

THUMB int cmd_floor_exit(int* param)
{
    cmn::g_extraMapLink.setExtraExitTown((const char*)param, param[4]);
    ScriptSystem::getSingleton()->executeEnable_ = 0;
    if (data_0210bb94.unkfunc_02058114(0xc) != 0) {
        TownCharacterBase::allEventLock_ = 0;
        BillboardCharacter::setAllCharaAnim(1);
        TownCharacterManager::getSingleton()->restoreCharacterAnim();
        TownPlayerManager::getSingleton()->partyDraw_.setAnimation(2);
        if (param[5] == 1) {
            TownSystem::getSingleton()->playExitSE_ = 1;
        }
    } else if (param[5] == 1) {
        FieldSystem::getSingleton()->exitSound_ = 1;
    }
    return 1;
}

THUMB int cmd_set_map_link_on_off(int* param)
{
    cmn::LINK_TYPE type = param[1] == 0 ? cmn::NOT_LINK_THIS_TOWN : cmn::LINK_DEFAULT;
    if (data_0210bb94.unkfunc_02058114(0xe) != 0) {
        cmn::g_extraMapLink.setLinkData(param[0], -1, type, 0, 0);
    }
    if (data_0210bb94.unkfunc_02058114(0xc) != 0) {
        cmn::g_extraMapLink.setLinkData(param[0], -1, type, g_Global.getMapName(), 0);
    }
    return 1;
}

THUMB int cmd_change_map_link(int* param)
{
    cmn::g_extraMapLink.setLinkData(param[5], param[4], cmn::LINK_TOWN_TO_TOWN, g_Global.getMapName(), (const char*)param);
    return 1;
}

THUMB int cmd_set_vehicle(int* param)
{
    int value = param[1];
    switch (param[0]) {
    case 0:
        if (value == 1) {
            status::g_Party.basha_ = 1;
        } else {
            status::g_Party.basha_ = 0;
        }
        break;
    case 1:
        status::g_Party.ship_ = value;
        break;
    case 2:
        status::g_Party.balloon_ = value;
        break;
    }
    return 1;
}

THUMB int cmd_not_play_normal_sound(int* param)
{
    SoundManager::setTownPlayDisable();
    return 1;
}

THUMB int cmd_check_member_type(int* param)
{
    return UnkImmigrantTown::getSingleton()->unkfunc_02037d6c(param[0]);
}

THUMB int cmd_check_member_num(int* param)
{
    unsigned int count = UnkImmigrantTown::getSingleton()->count_;
    if (count < (unsigned int)param[1] && count >= (unsigned int)param[0]) {
        return 1;
    }
    return 0;
}

THUMB int cmd_set_sleep_near(int* param)
{
    dss::Fix32 x(0L);
    x.value = param[1];
    dss::Fix32 y(0L);
    y.value = param[2];
    dss::Fix32 z(0L);
    z.value = param[3];
    if (param[0] == 0) {
        if (x == dss::Fix32(0L)) {
            x = dss::Fix32(0.27f);
        }
        if (y == dss::Fix32(0L)) {
            y = dss::Fix32(0.173f);
        }
        if (z == dss::Fix32(0L)) {
            z = dss::Fix32(0.194f);
        }
    } else if (param[0] == 1) {
        if (x == dss::Fix32(0L)) {
            x = dss::Fix32(0.254f);
        }
        if (y == dss::Fix32(0L)) {
            y = dss::Fix32(0.169f);
        }
        if (z == dss::Fix32(0L)) {
            z = dss::Fix32(0.455f);
        }
    }
    DisplayCharacter::sleepBodyOffset_ = dss::Fix32(x);
    DisplayCharacter::sleepHeadOffset_ = dss::Fix32(y);
    DisplayCharacter::sleepHeight_ = dss::Fix32(z);
    return 1;
}

THUMB int cmd_string_print(int* param)
{
    return 1;
}

static int g_scp_fade_flag;

THUMB int cmd_player_lock(int* param)
{
    if (param[0] != 0) {
        g_scp_fade_flag++;
        if (data_0210bb94.unkfunc_02058114(0xc) != 0) {
            TownPlayerManager::getSingleton()->setLock(1);
            TownPlayerManager::getSingleton()->charaColl_ = 0;
        } else {
            FieldPlayerManager::getSingleton();
            cmn::PlayerManager::setLock(1);
        }
        cmn::PartyTalk::getSingleton()->resetPartyTalk();
    } else {
        g_scp_fade_flag--;
        if (data_0210bb94.unkfunc_02058114(0xc) != 0) {
            TownPlayerManager::getSingleton()->setLock(0);
            if (!cmn::PlayerManager::isLock()) {
                TownPlayerManager::getSingleton()->charaColl_ = 1;
                TownPlayerManager::getSingleton()->flagMapLink_ = 1;
            }
        } else {
            FieldPlayerManager::getSingleton();
            cmn::PlayerManager::setLock(0);
        }
    }
    return 1;
}

THUMB int cmd_set_macro_actor()
{
    TextAPI::setMACRO0(1, 0x50000000, g_cmnPartyInfo.actorIndex_);
    return 1;
}

THUMB int cmd_set_macro_target()
{
    status::g_Party.setPlayerMode();
    int i = 0;
    int target = 0;
    int first = 0;
    for (; i < status::g_Party.getCount(); i++) {
        status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i)->haveStatusInfo_;
        if (!info->isDeath()) {
            if (info->haveStatus_.isPlayer() == 1 && first == 0) {
                first = status::g_Party.getPlayerIndex(i);
            }
            if (info->haveItem_.getCount() != 12) {
                target = status::g_Party.getPlayerIndex(i);
                TextAPI::setMACRO0(0x12, 0x50000000, target);
                break;
            }
        }
    }
    if (target == 0) {
        TextAPI::setMACRO0(0x12, 0x50000000, first);
    }
    return 1;
}

THUMB int cmd_set_macro_target_index(int* param)
{
    int index = param[0];
    if (index == 1 || index == 2) {
        if (status::g_Story.sex_ == 0) {
            index = 1;
        }
        if (status::g_Story.sex_ == 1) {
            index = 2;
        }
    }
    TextAPI::setMACRO0(0x12, 0x50000000, index);
    return 1;
}

THUMB int cmd_set_macro_prisoner()
{
    status::g_Party.setBattleMode();
    int i = 0;
    int index = status::g_Party.getPlayerIndex(i);
    for (; i < 0x1a; i++) {
        if (status::g_Party.isHostage(i)) {
            index = i;
            break;
        }
    }
    TextAPI::setMACRO0(0x10, 0x50000000, index);
    return 1;
}

THUMB int cmd_set_macro_x_item1(int* param)
{
    int sword;
    int armor;
    if (param[0] == 0) {
        sword = status::g_Story.getGiveEventItemCount(status::StoryStatus::EVENT_HAGANENOTURUGI);
        armor = status::g_Story.getGiveEventItemCount(status::StoryStatus::EVENT_TETUNOYOROI);
    } else {
        sword = 6 - status::g_Story.getEndorEventItemCount(status::StoryStatus::EVENT_HAGANENOTURUGI);
        armor = 6 - status::g_Story.getEndorEventItemCount(status::StoryStatus::EVENT_TETUNOYOROI);
    }
    TextAPI::setMACRO1(0x4f, 0xf0000000, sword);
    TextAPI::setMACRO2(0x4f, 0xf0000000, armor);
    return 1;
}

THUMB int cmd_set_macro_i_name(int* param)
{
    TextAPI::setMACRO0(10, 0x40000000, param[0]);
    return 1;
}

THUMB int cmd_barrier_disruption(int* param)
{
    g_Stage.maxKekai--;
    return 1;
}

THUMB int cmd_is_barrier_disruption(int* param)
{
    if (param[0] == 4 - g_Stage.maxKekai) {
        return 1;
    }
    return 0;
}

THUMB int cmd_set_party_join(int* param)
{
    status::g_Party.add(param[0]);
    if (data_0210bb94.unkfunc_02058114(0xc) != 0) {
        TownPlayerManager::getSingleton()->resetParty();
    } else {
        FieldPlayerManager::getSingleton()->resetParty();
    }
    return 1;
}

THUMB int cmd_set_party_quit(int* param)
{
    status::g_Party.del(param[0]);
    if (data_0210bb94.unkfunc_02058114(0xc) != 0) {
        TownPlayerManager::getSingleton()->resetParty();
    } else {
        FieldPlayerManager::getSingleton()->resetParty();
    }
    return 1;
}

THUMB int cmd_party_del2(int* param)
{
    if (TownPlayerManager::getSingleton()->setupDelPartyNotMoveFirst(param[0]) == 1) {
        int index = param[0];
        cmd_set_party_quit(&index);
    }
    return 1;
}

THUMB int cmd_battle_end_flag_set(int* param)
{
    if (param[0] == 0) {
        encount::Encount::getSingleton()->forceBrew(param[1]);
    } else if (param[0] == 1) {
        btl::BattleScriptManager::getSingleton()->setEncountMap(param[1]);
        encount::Encount::getSingleton()->forceEventBrew(param[1]);
    }
    if (data_0210bb94.unkfunc_02058114(0xe) == 1) {
        FieldPlayerManager::getSingleton()->setLockByEventEncount(1);
    } else {
        TownPlayerManager::getSingleton()->setLockByEventEncount(1);
    }
    btl::BattleScriptManager::getSingleton()->setScriptBattleResult(param[3], param[2], 0);
    btl::BattleScriptManager::getSingleton()->setScriptBattleResult(param[5], param[4], 1);
    return 1;
}

THUMB int cmd_not_use_load_message()
{
    g_Stage.chapterLoad_ = 1;
    return 1;
}

THUMB int cmd_set_message_sound(int* param)
{
    cmn::TalkSoundManager::MESSAGESOUND sound;
    int type[7];
    type[0] = param[1];
    type[1] = param[2];
    type[2] = param[3];
    type[3] = param[4];
    type[4] = param[5];
    type[5] = param[6];
    type[6] = param[7];
    for (unsigned int i = 0; i < (unsigned int)param[0]; i++) {
        switch (type[i]) {
        case 0:
            sound = cmn::TalkSoundManager::MESSAGESOUND_NONE;
            break;
        case 1:
            sound = cmn::TalkSoundManager::MESSAGESOUND_HIGH;
            break;
        case 2:
            sound = cmn::TalkSoundManager::MESSAGESOUND_MIDDLE;
            break;
        case 3:
            sound = cmn::TalkSoundManager::MESSAGESOUND_LOW;
            break;
        case 4:
            sound = cmn::g_talkSound.getPlayerVoice();
            break;
        case 5:
            sound = cmn::g_talkSound.getMostHeroicVoice();
            break;
        }
        cmn::g_talkSound.setOrderMessage(sound);
    }
    return 1;
}

THUMB int cmd_is_monster(int* param)
{
    int monster = g_HengeNoTsue.isMonster();
    if (param[0] == 1) {
        return monster;
    }
    if (monster == 0) {
        return 1;
    }
    return 0;
}

THUMB void __cmd_player_effect_mark::initialize(char* scriptParam)
{
    PARAM_PLAYER_EFFECT_MARK* param = (PARAM_PLAYER_EFFECT_MARK*)scriptParam;
    TownPlayerManager::getSingleton()->rizeupSet(param->mark);
}

THUMB int __cmd_player_effect_mark::isEnd()
{
    return TownPlayerManager::getSingleton()->rizeupEnd();
}

__cmd_player_effect_mark g_cmd_player_effect_mark;

THUMB int cmd_set_party_mark(int* param)
{
    TownPlayerManager::getSingleton()->rizeupSetParty(param[0], param[1]);
    return 1;
}

THUMB void __cmd_character_palette::initialize(char* scriptParam)
{
    PARAM_CHARACTER_PALETTE* param = (PARAM_CHARACTER_PALETTE*)scriptParam;
    int ctrl = getPlacementCtrlId();
    dss::Fix32Vector3 rgb;
    rgb.vx.value = param->r;
    rgb.vy.value = param->g;
    rgb.vz.value = param->b;
    TownCharacterManager::getSingleton()->setPaletteRate(ctrl, rgb.vx, rgb.vy, rgb.vz);
    rgb.vx.value = param->endR;
    rgb.vy.value = param->endG;
    rgb.vz.value = param->endB;
    TownCharacterManager::getSingleton()->setChangePalletRate(ctrl, rgb, param->frame);
}

THUMB int __cmd_character_palette::isEnd()
{
    int ctrl = getPlacementCtrlId();
    return TownCharacterManager::getSingleton()->character_[ctrl]->isEndPalletRate();
}

__cmd_character_palette g_cmd_character_palette;

THUMB void __cmd_music_volume::initialize(char* scriptParam)
{
    PARAM_MUSIC_VOLUME* param = (PARAM_MUSIC_VOLUME*)scriptParam;
    m_end_vol = param->volume * 0x7f / 100;
    dss::clamp<int>(0, m_end_vol, 0x7f);
    m_start_vol = Sound::getBgmVolume();
    m_frame = param->frame;
    m_add = (long)((m_end_vol - m_start_vol) / m_frame);
    m_counter = 0;
}

THUMB int __cmd_music_volume::isEnd()
{
    if (m_counter >= m_frame) {
        Sound::setBgmVolume(m_end_vol);
        return true;
    }
    int vol = m_start_vol + m_add.value * m_counter / 0x1000;
    dss::clamp<int>(0, vol, 0x7f);
    Sound::setBgmVolume(vol);
    m_counter++;
    return false;
}

__cmd_music_volume g_cmd_music_volume;

THUMB void __cmd_play_music::initialize(char* scriptParam)
{
    PARAM_PLAY_MUSIC* param = (PARAM_PLAY_MUSIC*)scriptParam;
    preMusicNo_ = SoundManager::bgmIndex_;
    musicNo_ = param->musicNo;
    playTime_ = param->frame + STOP_WAIT_COUNT;
    flag_ = param->flag;
    soundCount_ = 0;
    playEnd_ = 0;
    SoundManager::stopBgm(0);
}

THUMB void __cmd_play_music::execute()
{
    if (soundCount_ == -1) {
        playEnd_ = 1;
    }
    if (soundCount_ == STOP_WAIT_COUNT) {
        SoundManager::play(musicNo_, 0xf);
        soundCount_++;
    } else if (soundCount_ < playTime_) {
        soundCount_++;
    } else if (soundCount_ == playTime_) {
        SoundManager::stopBgm(0);
        soundCount_++;
    } else if (soundCount_ > playTime_) {
        if (flag_ == 0) {
            SoundManager::play(preMusicNo_, 0xf);
        }
        soundCount_ = -1;
    }
}

THUMB int __cmd_play_music::isEnd()
{
    return playEnd_;
}

__cmd_play_music g_cmd_play_music;

THUMB void __cmd_key_wait_type_b::initialize(char* scriptParam)
{
}

THUMB void __cmd_key_wait_type_b::execute()
{
}

THUMB int __cmd_key_wait_type_b::isEnd()
{
    return cmn::g_CommonCounterInfo.checkBottun();
}

__cmd_key_wait_type_b g_cmd_key_wait_type_b;

THUMB int cmd_set_player_henge_endless(int* param)
{
    g_HengeNoTsue.endLess_ = param[0];
    return 1;
}

THUMB int cmd_set_party_call_carriage(int* param)
{
    g_cmnPartyInfo.callCarriage();
    return 1;
}

THUMB int cmd_set_forward_counter(int* param)
{
    cmn::g_CommonCounterInfo.setChangeDay();
    status::g_Story.setTarot(0);
    return 1;
}

THUMB int cmd_setup_music(int* param)
{
    if (param[0] == 0) {
        SoundManager::stop(param[0]);
    } else {
        SoundManager::play(param[0], 0xf);
    }
    SoundManager::setTownPlayDisable();
    return 1;
}

THUMB int cmd_check_hero_sex(int* param)
{
    if (param[0] == 0) {
        if (status::g_Story.sex_ == 0) {
            return 1;
        }
    } else if (status::g_Story.sex_ == 1) {
        return 1;
    }
    return 0;
}

THUMB void __cmd_event_chapter_title::initialize(char* scriptParam)
{
    PARAM_EVENT_CHAPTER_TITLE* param = (PARAM_EVENT_CHAPTER_TITLE*)scriptParam;
    MaterielMenu_WINDOW_MANAGER::getSingleton()->setChapterTitleInfo(param->chapter, param->flag);
    MaterielMenu_WINDOW_MANAGER::getSingleton()->openMaterielWindow(MaterielMenu_WINDOW_MANAGER::MENU_EXTRA_CHAPTER_TITLE);
    int type = param->chapter;
    if (type != 0 || param->flag != 1) {
        cmn::CommonChapterTitle::getSingleton()->setup(type, param->flag);
    }
}

THUMB int __cmd_event_chapter_title::isEnd()
{
    if (MaterielMenu_WINDOW_MANAGER::getSingleton()->chapterEnd_) {
        cmn::CommonChapterTitle::getSingleton()->cleanup();
        MaterielMenu_WINDOW_MANAGER::getSingleton()->chapterEnd_ = 0;
    }
    if (MaterielMenu_WINDOW_MANAGER::getSingleton()->endWindow_) {
        return true;
    }
    return false;
}

__cmd_event_chapter_title g_cmd_event_chapter_title;

THUMB int cmd_stream_play(int* param)
{
    Sound::unkfunc_02055980(param[0]);
    return 1;
}

THUMB int cmd_is_load_init(int* param)
{
    int type = g_Stage.loadType_;
    if (type == 0 || type == 1 || type == 3) {
        if (param[0] == 1) {
            return 1;
        }
        return 0;
    }
    if (TownPlayerManager::getSingleton()->battleLose_ == 1) {
        if (param[0] == 1) {
            return 1;
        }
        return 0;
    }
    if (param[0] != 1) {
        return 1;
    }
    return 0;
}

THUMB int cmd_check_search_action(int* param)
{
    if (TownPlayerManager::getSingleton()->isSearch() == 1) {
        return 1;
    }
    return 0;
}

THUMB int cmd_set_no_search_message(int* param)
{
    TownPlayerManager::getSingleton()->searchAction_ = 8;
    return 1;
}

THUMB int cmd_set_timezone_pause(int* param)
{
    g_Stage.timestop_ = param[0];
    return 1;
}

THUMB int cmd_is_party_top(int* param)
{
    status::g_Party.setMemberShiftMode();
    status::g_Party.getPlayerStatus(0);
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(0)->haveStatusInfo_;
    int i = 0;
    for (; i < status::g_Party.getCarriageOutCount(); i++) {
        info = &status::g_Party.getPlayerStatus(i)->haveStatusInfo_;
        if (!info->isDeath()) {
            break;
        }
    }
    int player[5];
    player[0] = param[3];
    player[1] = param[4];
    player[2] = param[5];
    player[3] = param[6];
    player[4] = param[7];
    int found = 0;
    for (int j = 0; j < 5; j++) {
        int id = player[j];
        if (id == info->haveStatus_.playerIndex_) {
            found = 1;
        }
    }
    if (found == 0 && param[3] != 0) {
        return 0;
    }
    if (param[0] != 0 && unkfunc_020254b8(i, param[0]) == 0) {
        return 0;
    }
    if (param[1] != 0 && unkfunc_02025514(i, param[1]) == 0) {
        return 0;
    }
    return 1;
}

THUMB int cmd_is_not_party_top(int* param)
{
    if (cmd_is_party_top(param) == 0) {
        return 1;
    }
    return 0;
}

THUMB int cmd_is_party_all(int* param)
{
    status::g_Party.setNormalMode();
    short matchCount = 0;
    int player[5];
    player[0] = param[3];
    player[1] = param[4];
    player[2] = param[5];
    player[3] = param[6];
    player[4] = param[7];
    short playerCount = 0;
    for (int j = 0; j < 5; j++) {
        if (player[j] != 0) {
            playerCount++;
        }
    }
    int match[5];
    for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
        status::g_Party.getPlayerStatus(i);
        int k;
        status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i)->haveStatusInfo_;
        for (k = 0; k < 5; k++) {
            int id = player[k];
            if (id == info->haveStatus_.playerIndex_) {
                match[matchCount] = i;
                matchCount++;
            }
        }
    }
    if (playerCount == 0) {
        for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
            match[i] = i;
        }
    } else {
        switch (param[2]) {
        case 0:
            if (matchCount == 0) {
                return 0;
            }
            break;
        case 1:
            if (matchCount < playerCount) {
                return 0;
            }
            break;
        case 2:
            if (matchCount != status::g_Party.getCarriageOutCount()) {
                return 0;
            }
            break;
        }
    }
    int result = 0;
    for (int j = 0; j < matchCount; j++) {
        if (param[0] != 0 && unkfunc_02025514(match[j], param[0])) {
            result = 1;
        }
        if (param[1] != 0 && unkfunc_0202555c(match[j], param[1])) {
            result = 1;
        }
    }
    if (param[0] + param[1] == 0) {
        result = 1;
    }
    return result;
}

THUMB int cmd_is_not_party_all(int* param)
{
    if (cmd_is_party_all(param) == 0) {
        return 1;
    }
    return 0;
}

THUMB int cmd_is_party_head_count(int* param)
{
    unsigned int count = unkfunc_0202528c(param);
    if (param[3] == 0) {
        if (count == (unsigned int)param[4]) {
            return 1;
        }
    } else if (param[3] == 1) {
        if (count <= (unsigned int)param[4]) {
            return 1;
        }
    } else if (count >= (unsigned int)param[4]) {
        return 1;
    }
    return 0;
}

THUMB int cmd_is_not_party_head_count(int* param)
{
    if (cmd_is_party_head_count(param) == 0) {
        return 1;
    }
    return 0;
}

//gonna check ingame soon to name it didn't find it in the mobile vers yet  
THUMB short unkfunc_0202528c(int* param)
{
    status::g_Party.setNormalMode();
    short normalCount = status::g_Party.getCount();
    short normalOut = status::g_Party.getCarriageOutCount();
    status::g_Party.setPlayerMode();
    short playerCount = status::g_Party.getCount();
    short playerOut = status::g_Party.getCarriageOutCount();
    short playerAlive = 0;
    short playerOutAlive = 0;
    for (int i = 0; i < playerCount; i++) {
        bool alive = !status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath();
        if (alive) {
            playerAlive++;
            if (playerOut > i) {
                playerOutAlive++;
            }
        }
    }
    status::g_Party.setBattleMode();
    short npcCount = status::g_Party.getCount() - playerCount;
    short npcOut = status::g_Party.getCarriageOutCount() - playerOut;
    short npcAlive = 0;
    short npcOutAlive = 0;
    for (int i = 0; i < status::g_Party.getCount(); i++) {
        bool alive = !status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath();
        if (alive && status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.isBattleNpc_ != 0) {
            npcAlive++;
            if (status::g_Party.getCarriageOutCount() > i) {
                npcOutAlive++;
            }
        }
    }
    if (param[0] == 0) {
        short other = normalCount - (npcCount + playerCount);
        short total = playerAlive + (other + npcAlive);
        short totalOut = playerOutAlive + (other + npcOutAlive);
        if (param[1] == 0) {
            return unkfunc_020254a4(normalCount, total, param[2]);
        }
        if (param[1] == 1) {
            return unkfunc_020254a4(normalOut, totalOut, param[2]);
        }
        return unkfunc_020254a4(normalCount - normalOut, total - totalOut, param[2]);
    }
    if (param[0] == 1) {
        if (param[1] == 0) {
            return unkfunc_020254a4(playerCount, playerAlive, param[2]);
        }
        if (param[1] == 1) {
            return unkfunc_020254a4(playerOut, playerOutAlive, param[2]);
        }
        return unkfunc_020254a4(playerCount - playerOut, playerAlive - playerOutAlive, param[2]);
    }
    if (param[1] == 0) {
        return unkfunc_020254a4(npcCount, npcAlive, param[2]);
    }
    if (param[1] == 1) {
        return unkfunc_020254a4(npcOut, npcOutAlive, param[2]);
    }
    return unkfunc_020254a4(npcCount - npcOut, npcAlive - npcOutAlive, param[2]);
}

//gonna check ingame soon to name it didn't find it in the mobile vers yet  
THUMB short unkfunc_020254a4(short count, short alive, int mode)
{
    if (mode == 1) {
        return alive;
    }
    if (mode == 2) {
        return count - alive;
    }
    return count;
}


//gonna check ingame soon to name it didn't find it in the mobile vers yet  
THUMB int unkfunc_020254b8(int index, int type)
{
    status::g_Party.setMemberShiftMode();
    status::g_Party.getPlayerStatus(index);
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(index)->haveStatusInfo_;
    if (info->haveStatus_.isBattleNpc_ != 0) {
        if (type != 2) {
            return 0;
        }
    } else if (info->haveStatus_.isPlayer_ != 0) {
        if (type == 2) {
            return 0;
        }
        int playerIndex = info->haveStatus_.playerIndex_;
        if (playerIndex <= 2 && type == 1) {
            return 0;
        }
    } else {
        return 0;
    }
    return 1;
}

//gonna check ingame soon to name it didn't find it in the mobile vers yet  
THUMB int unkfunc_02025514(int index, int type)
{
    status::g_Party.setMemberShiftMode();
    status::g_Party.getPlayerStatus(index);
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(index)->haveStatusInfo_;
    if (info->haveStatus_.sex_ == 0) {
        if (type != 1) {
            return 0;
        }
    } else if (info->haveStatus_.sex_ == 1) {
        if (type != 2) {
            return 0;
        }
    } else {
        return 0;
    }
    return 1;
}


//gonna check ingame soon to name it didn't find it in the mobile vers yet  
THUMB int unkfunc_0202555c(int index, int type)
{
    status::g_Party.setMemberShiftMode();
    status::PlayerStatus* player = status::g_Party.getPlayerStatus(index);
    status::g_Party.getPlayerStatus(index);
    bool alive = !player->haveStatusInfo_.isDeath();
    if (alive) {
        if (type == 2) {
            return 0;
        }
    } else if (type == 1) {
        return 0;
    }
    return 1;
}

THUMB int cmd_set_se(int* param)
{
    SoundManager::playSe(param[0], 0);
    return 1;
}

THUMB int cmd_cut_se(int* param)
{
    SoundManager::stopSeWithIndex(param[0], 0);
    return 1;
}

THUMB int cmd_set_player_recovery(int* param)
{
    status::g_Party.setBattleMode();
    status::HaveStatusInfo* info = &status::PartyStatus::getPlayerStatusForPlayerIndex(param[0])->haveStatusInfo_;
    if (param[1] == 0) {
        info->recovery();
    } else {
        info->rebirth();
        status::PartyStatus::getPlayerStatusForPlayerIndex(param[0])->setBestCondition();
        info->statusChange_.clear();
        cmn::GameManager::getSingleton()->playerManager_->resetParty();
    }
    return 1;
}

THUMB int cmd_set_player_in_carriage(int* param)
{
    int order[4] = {0, 0, 0, 0};
    status::g_Party.setBattleMode();
    for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
        order[i] = status::g_Party.getPlayerIndex(i);
    }
    status::g_Party.add(param[0]);
    status::g_Party.reorder(order[0], order[1], order[2], order[3]);
    TownPlayerManager::getSingleton()->resetParty();
    return 1;
}

THUMB int cmd_set_player_ride_on(int* param)
{
    g_cmnPartyInfo.rideOnType_ = (cmn::PARTY_RIDE_ON_TYPE)param[0];
    return 1;
}

THUMB int cmd_set_ship_pos(int* param)
{
    dss::Fix32Vector3 pos;
    pos.vx.value = param[0];
    pos.vy.value = param[1];
    pos.vz.value = param[2];
    pos *= 0x10;
    g_Stage.shipPosition_ = dss::Fix32Vector3(pos);
    return 1;
}

THUMB int cmd_set_title_part(int* param)
{
    g_Global.startTitle();
    return 1;
}
