#pragma ipa file
#include "globaldefs.h"
#include "GameInfo.hpp"
#include "main/CommandParameter/CommandParameter.hpp"
#include "ov000/Commands/TownCommand.hpp"
#include "main/Commands/CommonCommand.hpp"
#include "ov001/Commands/FieldCommand.hpp"
#include "main/Commands/ScriptCommand.hpp"
#include "main/Commands/CommonScriptCommand.hpp"
#include "ov000/Commands/TownScriptCommand.hpp"
#include "ov001/Commands/FieldScriptCommand.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/cmn/TalkSoundManager.hpp"
#include "main/cmn/HengeNoTsueManager.hpp"
#include "main/cmn/PartyTalk.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/status/GameFlag.hpp"
#include "main/script/ScriptBaseCommand.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "ov000/town/TownCharacterManager.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "ov001/window/FieldWindowSystem.hpp"
#include "ov000/town/TownWindowSystem.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/global/Global.hpp"
#include "main/dss/Random.hpp"
#include "ov000/town/TownPlayerManager.hpp"

ARM void __cmd_count::initialize(char* scriptParam)
{
    PARAM_COUNT* param = (PARAM_COUNT*)scriptParam;
    start_ = param->start;
    end_ = param->end;
    add_ = param->add;
    count_ = start_;
}

ARM void __cmd_count::execute()
{
    count_ += add_;
    data_02116ce0.unkfunc_0207e88c(0xe, 0x28, "%04d", count_);
}

ARM int __cmd_count::isEnd()
{
    if (count_ == end_) {
        return true;
    }
    return false;
}

ARM void __cmd_wait::initialize(char* scriptParam)
{
    PARAM_WAIT* param = (PARAM_WAIT*)scriptParam;
    count_ = 0;
    countFrame_ = param->frame;
}

ARM void __cmd_wait::execute()
{
    count_++;
}

ARM int __cmd_wait::isEnd()
{
    if (count_ >= countFrame_) {
        return true;
    }
    return false;
}

ARM void __cmd_map_animation_b::initialize(char* scriptParam)
{
    PARAM_MAP_ANIMATION_B* param = (PARAM_MAP_ANIMATION_B*)scriptParam;
    TownStageManager::getSingleton()->setObjectDraw(param->target, param->animation, 1);
    m_target = param->target;
}

ARM int __cmd_map_animation_b::isEnd()
{
    return TownStageManager::getSingleton()->isCommonAnimationEnd(m_target) != false;
}

ARM void __cmd_character_action_tremble::initialize(char* scriptParam)
{
    PARAM_CHARACTER_ACTION_TREMBLE* param = (PARAM_CHARACTER_ACTION_TREMBLE*)scriptParam;
    TOWN_SCRIPT_DATA scriptData;
    dss::memset(&scriptData, 0, sizeof(scriptData));
    scriptData.frame = param->frame;
    scriptData.num[0] = param->dir;
    scriptData.num[1] = param->mode;
    scriptData.num[2] = param->sycle;
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->character_[ctrl]->setScriptData(scriptData);
}

ARM void __cmd_character_action_tremble::execute()
{
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->character_[ctrl]->execTremble();
}

ARM int __cmd_character_action_tremble::isEnd()
{
    int ctrl = getPlacementCtrlId();
    if (TownCharacterManager::getSingleton()->character_[ctrl]->isScriptEnd()) {
        return true;
    }
    return false;
}

ARM void __cmd_character_action_vanish::initialize(char* scriptParam)
{
    PARAM_CHARACTER_ACTION_VANISH* param = (PARAM_CHARACTER_ACTION_VANISH*)scriptParam;
    TOWN_SCRIPT_DATA scriptData;
    dss::memset(&scriptData, 0, sizeof(scriptData));
    scriptData.frame = param->frame;
    scriptData.num[0] = param->flash;
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->character_[ctrl]->setScriptData(scriptData);
}

ARM void __cmd_character_action_vanish::execute()
{
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->character_[ctrl]->execVanish();
}

ARM int __cmd_character_action_vanish::isEnd()
{
    int ctrl = getPlacementCtrlId();
    if (TownCharacterManager::getSingleton()->character_[ctrl]->isScriptEnd()) {
        return true;
    }
    return false;
}

ARM void __cmd_menu_yes_no::initialize(char* scriptParam)
{
    PARAM_MENU_YES_NO* param = (PARAM_MENU_YES_NO*)scriptParam;
    data_020ed1bc.setYesNo(param->position);
    type_ = param->type;
    index_ = param->index;
    position_ = param->position;
}

ARM int __cmd_menu_yes_no::isEnd()
{
    menu::MenuBase::MENUBASE_STAT stat = data_020ed1bc.stat_;
    if (stat == menu::MenuBase::MENUBASE_STAT_OK) {
        setFlag(true);
        return true;
    }
    if (stat == menu::MenuBase::MENUBASE_STAT_CANCEL) {
        setFlag(false);
        return true;
    }
    return false;
}

ARM void __cmd_menu_yes_no::setFlag(bool flag)
{
    switch (type_) {
        case 0:
            if (flag) {
                g_AreaFlag.set(index_);
            } else {
                g_AreaFlag.remove(index_);
            }
            break;
        case 1:
            if (flag) {
                g_LocalFlag.set(index_);
            } else {
                g_LocalFlag.remove(index_);
            }
            break;
        case 2:
            if (flag) {
                g_GlobalFlag.set(index_);
            } else {
                g_GlobalFlag.remove(index_);
            }
            break;
    }
}

ARM int cmd_setup(int* param)
{
    ScriptObjectId id;
    id.id_ = param[0];
    setScriptObjectEnable(*(int*)&id, true);
    return 1;
}

ARM int cmd_message1(int* param)
{
    int index = getPlacementCtrlId();
    if (getObjectCount() > index) {
        if (data_0210bb94.unkfunc_02058114(0xc) != 0) {
            cmn::g_talkSound.setVoice(TownCharacterManager::getSingleton()->getCharaIndex(index));
        }
    } else {
        cmn::g_talkSound.setVoice(0);
        index = -1;
    }
    cmn::g_talkSound.setMessageSound(param[1], index);
    if (!g_HengeNoTsue.isMonster()) {
        cmn::PartyTalk::getSingleton()->setPreMessageNo(param[0]);
    }
    if (data_0210bb94.unkfunc_02058114(0xe) != 0) {
        FieldWindowSystem::getSingleton()->openMessage(param[0], param[1]);
    } else {
        TownWindowSystem::getSingleton()->openMessage(param[0], param[1]);
    }
    return 1;
}

ARM int cmd_message2(int* param)
{
    int index = getPlacementCtrlId();
    if (getObjectCount() > index) {
        if (data_0210bb94.unkfunc_02058114(0xc) != 0) {
            cmn::g_talkSound.setVoice(TownCharacterManager::getSingleton()->getCharaIndex(index));
        }
    } else {
        cmn::g_talkSound.setVoice(0);
        index = -1;
    }
    int message[8];
    message[0] = param[0];
    message[1] = param[1];
    message[2] = param[2];
    message[3] = param[3];
    message[4] = param[4];
    message[5] = param[5];
    message[6] = param[6];
    message[7] = param[7];
    int count = 0;
    for (int i = 0; i < 8; i++) {
        if (message[i] == 0) {
            break;
        }
        count++;
    }
    cmn::g_talkSound.setMessageSound(count, index);
    TownWindowSystem::getSingleton()->openCommonMessage();
    if (param[0] != 0) {
        TownWindowSystem::getSingleton()->addCommonMessage(param[0]);
        if (!g_HengeNoTsue.isMonster()) {
            cmn::PartyTalk::getSingleton()->setPreMessageNo(param[0]);
        }
    }
    if (param[1] != 0) {
        TownWindowSystem::getSingleton()->addCommonMessage(param[1]);
    }
    if (param[2] != 0) {
        TownWindowSystem::getSingleton()->addCommonMessage(param[2]);
    }
    if (param[3] != 0) {
        TownWindowSystem::getSingleton()->addCommonMessage(param[3]);
    }
    if (param[4] != 0) {
        TownWindowSystem::getSingleton()->addCommonMessage(param[4]);
    }
    if (param[5] != 0) {
        TownWindowSystem::getSingleton()->addCommonMessage(param[5]);
    }
    if (param[6] != 0) {
        TownWindowSystem::getSingleton()->addCommonMessage(param[6]);
    }
    if (param[7] != 0) {
        TownWindowSystem::getSingleton()->addCommonMessage(param[7]);
    }
    return 1;
}

ARM int cmd_random_message(int* param)
{
    int index = getPlacementCtrlId();
    if (getObjectCount() > index) {
        if (data_0210bb94.unkfunc_02058114(0xc) != 0) {
            cmn::g_talkSound.setVoice(TownCharacterManager::getSingleton()->getCharaIndex(index));
        }
    } else {
        cmn::g_talkSound.setVoice(0);
        index = -1;
    }
    int message[7];
    message[0] = param[1];
    message[1] = param[2];
    message[2] = param[3];
    message[3] = param[4];
    message[4] = param[5];
    message[5] = param[6];
    message[6] = param[7];
    int select = dssrand::rand(param[0]);
    cmn::g_talkSound.setMessageSound(param[0], index);
    TownWindowSystem::getSingleton()->openCommonMessage();
    int mes = message[select];
    TownWindowSystem::getSingleton()->addCommonMessage(mes);
    if (!g_HengeNoTsue.isMonster()) {
        cmn::PartyTalk::getSingleton()->setPreMessageNo(mes);
    }
    return 1;
}

ARM void __cmd_message_with_sound::initialize(char* scriptParam)
{
    PARAM_MESSAGE_WITH_SOUND* param = (PARAM_MESSAGE_WITH_SOUND*)scriptParam;
    preMusicNo_ = SoundManager::bgmIndex_;
    musicNo_ = param->sound;
    playTime_ = param->frame + STOP_WAIT_COUNT;
    lastMessage_ = 0;
    flag_ = param->flag;
    wait_ = param->automes;
    soundCount_ = 0;
    stopSound_ = 0;
    playEnd_ = 0;
    cmn::g_talkSound.setMessageSound(param->count, -1);
    if (!g_HengeNoTsue.isMonster()) {
        cmn::PartyTalk::getSingleton()->setPreMessageNo(param->message);
    }
    data_020ed1bc.openMessageForTALK();
    if (param->count == 1) {
        data_020ed1bc.addMessageNOWAIT(param->message);
        SoundManager::stopBgm(0);
    } else if (param->count == 2) {
        lastMessage_ = param->message + 1;
        data_020ed1bc.addMessageNOWAIT(param->message);
    } else if (param->count >= 3) {
        lastMessage_ = param->message + param->count - 1;
        data_020ed1bc.addMessageCount(param->message, param->count - 2);
        data_020ed1bc.addMessageNOWAIT(param->message + param->count - 2);
    }
    data_020ed1bc.addMessageWAITKEY();
}

ARM void __cmd_message_with_sound::execute()
{
    if (data_020ed1bc.isMessageWAITPROG()) {
        if (stopSound_ == 0 && lastMessage_ != 0) {
            stopSound_ = 1;
            data_020ed1bc.close();
            data_020ed1bc.clearMessageWAITPROG();
            SoundManager::stopBgm(0);
        } else if (soundCount_ == -1) {
            data_020ed1bc.clearMessageWAITPROG();
            if (wait_ == 0) {
                playEnd_ = 1;
            }
        } else if (soundCount_ == STOP_WAIT_COUNT) {
            SoundManager::playBgm(musicNo_, 0);
            soundCount_++;
        } else if (soundCount_ < playTime_) {
            soundCount_++;
        } else if (soundCount_ == playTime_) {
            SoundManager::stopBgm(0);
            soundCount_++;
        } else if (soundCount_ > playTime_) {
            if (flag_ == 0) {
                SoundManager::playBgm(preMusicNo_, 0);
            }
            soundCount_ = -1;
        }
    } else if (stopSound_ == 1) {
        if (!data_020ed1bc.isOpen()) {
            data_020ed1bc.openMessageForTALK();
            data_020ed1bc.addMessageNOWAIT(lastMessage_);
            data_020ed1bc.addMessageWAITKEY();
        }
    }
}

ARM int __cmd_message_with_sound::isEnd()
{
    if (wait_ == 1) {
        menu::MenuBase::MENUBASE_STAT stat = data_020ed1bc.stat_;
        if (stat == menu::MenuBase::MENUBASE_STAT_OK || stat == menu::MenuBase::MENUBASE_STAT_CANCEL) {
            data_020ed1bc.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
            data_020ed1bc.close();
            playEnd_ = 1;
            wait_ = 0;
        }
        return false;
    }
    if (playEnd_ == 1 && wait_ == 0) {
        data_020ed1bc.stat_ = menu::MenuBase::MENUBASE_STAT_OK;
        data_020ed1bc.close();
        return true;
    }
    return false;
}

ARM int cmd_is_timezone(int* param)
{
    TIME_ZONE zone = (TIME_ZONE)(param[0] + 1);
    if (zone == 2 || zone == 3) {
        if (g_Stage.getTimeZone() == 3 || g_Stage.getTimeZone() == 2) {
            return 1;
        }
        return 0;
    }
    if (g_Stage.getTimeZone() == 1 || g_Stage.getTimeZone() == 4) {
        return 1;
    }
    return 0;
}

ARM int cmd_set_timezone(int* param)
{
    g_Stage.setTimeZone((TIME_ZONE)(param[0] + 1));
    TownStageManager::getSingleton()->loadStage(g_Global.getMapName());
    return 1;
}

ARM int cmd_is_player_status_dead(int* param)
{
    if (param[1] == 0) {
        return !status::PartyStatus::getPlayerStatusForPlayerIndex(param[0])->haveStatusInfo_.isDeath() == 1;
    }
    return status::PartyStatus::getPlayerStatusForPlayerIndex(param[0])->haveStatusInfo_.isDeath() == 1;
}

ARM int cmd_is_party_ride_carriage(int* param)
{
    return status::PartyStatus::isInsideCarriageForPlayerIndex(param[0]) != 0;
}

ARM int cmd_is_party_member(int* param)
{
    status::g_Party.setNormalMode();
    return status::g_Party.getSortIndex(param[0]) != -1;
}

ARM int cmd_is_party_order(int* param)
{
    status::g_Party.setNormalMode();
    return param[0] == status::g_Party.getSortIndex(param[1]);
}

ARM int cmd_set_party_total_recovery(int* param)
{
    status::g_Party.setBattleMode();
    for (int i = 0; i < status::g_Party.getCount(); i++) {
        if (param[0] == 1) {
            status::g_Party.getPlayerStatus(i)->haveStatusInfo_.revival();
            status::g_Party.getPlayerStatus(i)->haveStatusInfo_.statusChange_.clear();
        } else if (!status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath()) {
            status::g_Party.getPlayerStatus(i)->haveStatusInfo_.revival();
        }
    }
    if (param[0] == 1) {
        if (data_0210bb94.unkfunc_02058114(0xc) != 0) {
            TownPlayerManager::getSingleton()->resetParty();
        } else {
            FieldPlayerManager::getSingleton()->resetParty();
        }
    }
    return 1;
}

ARM int checkCommandType(CommandParameter* command)
{
    if (command->flag_ & 1) {
        if (command->flag_ & 0x40) {
            return 0;
        }
        command->flag_ |= 0x40;
    }
    return 1;
}

ARM int CommandFunction(CommandParameter *arg0) {
    int var_r4;

    var_r4 = 1;
    switch (arg0->command_) {
    case 2:
        if (checkCommandType(arg0) == 0) {
            var_r4 = 1;
        }
        break;
    case 0:
        break;
    case 1:
        break;
    case 3:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_message1(arg0->param_);
        }
        break;
    case 4:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_message2(arg0->param_);
        }
        break;
    case 0x18:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_get_flag(arg0->param_);
        }
        break;
    case 0x17:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_flag(arg0->param_);
        }
        break;
    case 0x7:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_setup(arg0->param_);
        }
        break;
    case 0x4A:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_speaked(arg0->param_);
        }
        break;
    case 0x5:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_speak_to_player(arg0->param_);
        }
        break;
    case 0x6:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_speak_to_player2(arg0->param_);
        }
        break;
    case 0x19:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_timezone(arg0->param_);
        }
        break;
    case 0x1A:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_timezone(arg0->param_);
        }
        break;
    case 0x8E:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_debug_print(arg0->param_);
        }
        break;
    case 0x8:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_player_lock(arg0->param_);
        }
        break;
    case 0x4D:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_character_position(arg0->param_);
        }
        break;
    case 0x4E:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_character_direction(arg0->param_);
        }
        break;
    case 0x16D:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_overview_point(arg0->param_);
        }
        break;
    case 0x79:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_player_position(arg0->param_);
        }
        break;
    case 0x7A:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_player_direction(arg0->param_);
        }
        break;
    case 0x11B:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_x_wins(arg0->param_);
        }
        break;
    case 0x10E:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_battle_end_flag_set(arg0->param_);
        }
        break;
    case 0x11A:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_message_sound(arg0->param_);
        }
        break;
    case 0x126:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_monster(arg0->param_);
        }
        break;
    case 0x1D:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_map_animation_a(arg0->param_);
        }
        break;
    case 0x1E:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_map_collision(arg0->param_);
        }
        break;
    case 0x1B:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_timezone_pause(arg0->param_);
        }
        break;
    case 0x52:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_character_action_stepping(arg0->param_);
        }
        break;
    case 0x53:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_character_action_still(arg0->param_);
        }
        break;
    case 0x54:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_character_action_wriggle(arg0->param_);
        }
        break;
    case 0x58:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_character_action_display(arg0->param_);
        }
        break;
    case 0x55:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_character_action_near(arg0->param_);
        }
        break;
    case 0x7B:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_player_action_wriggle(arg0->param_);
        }
        break;
    case 0x7C:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_player_action_stepping(arg0->param_);
        }
        break;
    case 0x50:
        if (checkCommandType(arg0) != 0) {
            cmd_character_action_sleep(arg0->param_);
            var_r4 = 0;
        }
        break;
    case 0x16E:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_sleep_near(arg0->param_);
        }
        break;
    case 0x7D:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_player_action_still(arg0->param_);
        }
        break;
    case 0x59:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_character_collision(arg0->param_);
        }
        break;
    case 0xA:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_trigger(arg0->param_);
        }
        break;
    case 0xB:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_trigger2(arg0->param_);
        }
        break;
    case 0x5A:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_character_action_pursue(arg0->param_);
        }
        break;
    case 0x5B:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_character_move_roam(arg0->param_);
        }
        break;
    case 0x78:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_encount(arg0->param_);
        }
        break;
    case 0x88:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_encount_set_flag(arg0->param_);
        }
        break;
    case 0x164:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_encount_first_strike(arg0->param_);
        }
        break;
    case 0x16A:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_player_ride_on(arg0->param_);
        }
        break;
    case 0x64:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_party_join(arg0->param_);
        }
        break;
    case 0x65:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_party_quit(arg0->param_);
        }
        break;
    case 0x66:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_party_join(arg0->param_);
        }
        break;
    case 0x67:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_party_quit(arg0->param_);
        }
        break;
    case 0xAF:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_hostage();
        }
        break;
    case 0x120:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_macro_actor();
        }
        break;
    case 0x128:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_macro_target();
        }
        break;
    case 0x160:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_macro_target_index(arg0->param_);
        }
        break;
    case 0x140:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_macro_prisoner();
        }
        break;
    case 0x12:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_item(arg0->param_);
        }
        break;
    case 0x49:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_procure_item(arg0->param_);
        }
        break;
    case 0x13:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_gold(arg0->param_);
        }
        break;
    case 0x14:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_coin(arg0->param_);
        }
        break;
    case 0x11:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_mini_game(arg0->param_);
        }
        break;
    case 0x6A:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_party_total_recovery(arg0->param_);
        }
        break;
    case 0x6B:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_party_order(arg0->param_);
        }
        break;
    case 0x6C:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_party_member(arg0->param_);
        }
        break;
    case 0x6D:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_party_ride_carriage(arg0->param_);
        }
        break;
    case 0x80:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_player_status_dead(arg0->param_);
        }
        break;
    case 0x1F:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_floor_change(arg0->param_);
        }
        break;
    case 0x20:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_floor_exit(arg0->param_);
        }
        break;
    case 0x6E:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_party_join_carriage();
        }
        break;
    case 0x6F:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_party_top(arg0->param_);
        }
        break;
    case 0x70:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_not_party_top(arg0->param_);
        }
        break;
    case 0x71:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_party_all(arg0->param_);
        }
        break;
    case 0x72:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_not_party_all(arg0->param_);
        }
        break;
    case 0x73:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_party_head_count(arg0->param_);
        }
        break;
    case 0x74:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_not_party_head_count(arg0->param_);
        }
        break;
    case 0x28:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_map_change_timezone(arg0->param_);
        }
        break;
    case 0xC9:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_map_effect_sepia();
        }
        break;
    case 0xCA:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_disable_demolition();
        }
        break;
    case 0x62:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_se(arg0->param_);
        }
        break;
    case 0x63:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_cut_se(arg0->param_);
        }
        break;
    case 0x10:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_doorway(arg0->param_);
        }
        break;
    case 0x25:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_map_link_on_off(arg0->param_);
        }
        break;
    case 0x13D:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_not_use_load_message();
        }
        break;
    case 0x26:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_change_map_link(arg0->param_);
        }
        break;
    case 0xFF:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_vehicle(arg0->param_);
        }
        break;
    case 0x27:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmn_set_event_door(arg0->param_);
        }
        break;
    case 0x176:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_title_part(arg0->param_);
        }
        break;
    case 0xFD:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_map_texture(arg0->param_);
        }
        break;
    case 0x42:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_trigger_forward(arg0->param_);
        }
        break;
    case 0x43:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_trigger_character(arg0->param_);
        }
        break;
    case 0x44:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_trigger2_character(arg0->param_);
        }
        break;
    case 0x47:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_town_to_field_link(arg0->param_);
        }
        break;
    case 0x48:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmn_set_field_to_town_link(arg0->param_);
        }
        break;
    case 0x34:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_character_not_change_direction(arg0->param_);
        }
        break;
    case 0x38:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_player_action_not_change_direction(arg0->param_);
        }
        break;
    case 0x35:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_character_direction(arg0->param_);
        }
        break;
    case 0x39:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_player_direction(arg0->param_);
        }
        break;
    case 0x97:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_map_animation(arg0->param_);
        }
        break;
    case 0x81:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_player_recovery(arg0->param_);
        }
        break;
    case 0x77:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_player_in_carriage(arg0->param_);
        }
        break;
    case 0x29:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_map_shake(arg0->param_);
        }
        break;
    case 0xB2:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_effect_blur(arg0->param_);
        }
        break;
    case 0x3A:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_party_display(arg0->param_);
        }
        break;
    case 0x37:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_character_front(arg0->param_);
        }
        break;
    case 0x36:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_talked_at_shop(arg0->param_);
        }
        break;
    case 0x2A:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_search_map_object(arg0->param_);
        }
        break;
    case 0x3E:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_encount_disable(arg0->param_);
        }
        break;
    case 0x3F:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_encount_stage_disable(arg0->param_);
        }
        break;
    case 0x40:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_demolition_fightingarena(arg0->param_);
        }
        break;
    case 0x41:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_battle_turn(arg0->param_);
        }
        break;
    case 0x45:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_random(arg0->param_);
        }
        break;
    case 0x46:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_random2(arg0->param_);
        }
        break;
    case 0x2B:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_floor_map_object(arg0->param_);
        }
        break;
    case 0x16:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_hostage(arg0->param_);
        }
        break;
    case 0x9E:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_character_set_coll_stage(arg0->param_);
        }
        break;
    case 0x9D:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_trigger_distance(arg0->param_);
        }
        break;
    case 0xA7:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_charcter_3d_rotate(arg0->param_);
        }
        break;
    case 0xAC:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_effect_transfer(arg0->param_);
        }
        break;
    case 0xB1:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_character_pose_change(arg0->param_);
        }
        break;
    case 0x123:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_furniture_move_request(arg0->param_);
        }
        break;
    case 0xB0:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_party_display2(arg0->param_);
        }
        break;
    case 0x86:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_music(arg0->param_);
        }
        break;
    case 0x87:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_music_pause(arg0->param_);
        }
        break;
    case 0x146:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_play_music_now_map(arg0->param_);
        }
        break;
    case 0xB5:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmn_camera_lock_pov(arg0->param_);
        }
        break;
    case 0xA5:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_party_redisplay(arg0->param_);
        }
        break;
    case 0xB7:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_player_action_dance(arg0->param_);
        }
        break;
    case 0xB8:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_enable_event_item(arg0->param_);
        }
        break;
    case 0xBB:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_player_set_coll(arg0->param_);
        }
        break;
    case 0xC2:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_ruura_lock(arg0->param_);
        }
        break;
    case 0x169:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_ranaruta(arg0->param_);
        }
        break;
    case 0xFB:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_invalidation_rula(arg0->param_);
        }
        break;
    case 0xBE:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_map_link_field_direct(arg0->param_);
        }
        break;
    case 0xC4:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_check_money(arg0->param_);
        }
        break;
    case 0x100:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_not_play_normal_sound(arg0->param_);
        }
        break;
    case 0x163:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_save_last_party();
        }
        break;
    case 0x101:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_check_member_type(arg0->param_);
        }
        break;
    case 0x102:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_check_member_num(arg0->param_);
        }
        break;
    case 0x103:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_chara_set_priority_sure_appointment(arg0->param_);
        }
        break;
    case 0x104:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_chara_set_normal_sure_appointment(arg0->param_);
        }
        break;
    case 0x105:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_chara_set_normal_sure(arg0->param_);
        }
        break;
    case 0x106:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_chara_talk_to_player_sure(arg0->param_);
        }
        break;
    case 0x107:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_surechigai_level(arg0->param_);
        }
        break;
    case 0x108:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_copy_party_chara(arg0->param_);
        }
        break;
    case 0x10B:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_party_del2(arg0->param_);
        }
        break;
    case 0x112:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_chara_shadow(arg0->param_);
        }
        break;
    case 0x113:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_chara_alpha(arg0->param_);
        }
        break;
    case 0x118:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_ikada_set_position(arg0->param_);
        }
        break;
    case 0x119:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_crack_key_by_orin(arg0->param_);
        }
        break;
    case 0x11F:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_big_rock_move(arg0->param_);
        }
        break;
    case 0x124:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_monster_talk(arg0->param_);
        }
        break;
    case 0x125:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_monster_talk_all(arg0->param_);
        }
        break;
    case 0x127:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_field_symbol_disp(arg0->param_);
        }
        break;
    case 0x129:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_chara_map_uid(arg0->param_);
        }
        break;
    case 0x12A:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_my_taishi(arg0->param_);
        }
        break;
    case 0x144:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_forward_counter(arg0->param_);
        }
        break;
    case 0x147:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_setup_music(arg0->param_);
        }
        break;
    case 0x155:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_stream_play(arg0->param_);
        }
        break;
    case 0x157:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_camera_limit(arg0->param_);
        }
        break;
    case 0x161:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_chara_voice(arg0->param_);
        }
        break;
    case 0xBC:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_field_player_set_ship(arg0->param_);
        }
        break;
    case 0x145:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_field_player_pos(arg0->param_);
        }
        break;
    case 0xC7:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_camera_clip_distance(arg0->param_);
        }
        break;
    case 0xBF:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_map_camera_lock_target_player(arg0->param_);
        }
        break;
    case 0xCB:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_map_camera_near(arg0->param_);
        }
        break;
    case 0x9A:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_character_move_passive(arg0->param_);
        }
        break;
    case 0x85:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_camera_target(arg0->param_);
        }
        break;
    case 0x9B:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_character_move_random(arg0->param_);
        }
        break;
    case 0xD4:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_wait_counter(arg0->param_);
        }
        break;
    case 0xD6:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_day_count(arg0->param_);
        }
        break;
    case 0xD8:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_barrier_disruption(arg0->param_);
        }
        break;
    case 0xD9:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_barrier_disruption(arg0->param_);
        }
        break;
    case 0xDC:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_trigger3(arg0->param_);
        }
        break;
    case 0x135:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_player_sleep(arg0->param_);
        }
        break;
    case 0xEA:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_party_reserve_order(arg0->param_);
        }
        break;
    case 0xEB:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_party_copy_character(arg0->param_);
        }
        break;
    case 0xEC:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_party_call_carriage(arg0->param_);
        }
        break;
    case 0xF2:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_map_treasure(arg0->param_);
        }
        break;
    case 0xA2:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_furniture_position(arg0->param_);
        }
        break;
    case 0xF3:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_map_texture(arg0->param_);
        }
        break;
    case 0xF5:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_effect_dream(arg0->param_);
        }
        break;
    case 0x9C:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_character_move_reverse(arg0->param_);
        }
        break;
    case 0xF6:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_furniture_fadeout(arg0->param_);
        }
        break;
    case 0xD2:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_wait_operation(arg0->param_);
        }
        break;
    case 0xD5:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_wait_counter(arg0->param_);
        }
        break;
    case 0x152:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_push_key(arg0->param_);
        }
        break;
    case 0xD7:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_day_count(arg0->param_);
        }
        break;
    case 0xFA:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_chapter(arg0->param_);
        }
        break;
    case 0xFC:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_check_hero_level(arg0->param_);
        }
        break;
    case 0xEE:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_event_chapter_end(arg0->param_);
        }
        break;
    case 0xF8:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_character_swing_round(arg0->param_);
        }
        break;
    case 0xF7:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_character_anim(arg0->param_);
        }
        break;
    case 0x75:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_party_item(arg0->param_);
        }
        break;
    case 0x76:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_not_party_item(arg0->param_);
        }
        break;
    case 0xFE:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_field_erase_symbol(arg0->param_);
        }
        break;
    case 0x10C:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_chapter_store(arg0->param_);
        }
        break;
    case 0x10D:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_chapter_restore(arg0->param_);
        }
        break;
    case 0x170:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_chapter_restore_coin(arg0->param_);
        }
        break;
    case 0x10F:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_reset_sidejob_pay(arg0->param_);
        }
        break;
    case 0x110:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_get_sidejob_pay(arg0->param_);
        }
        break;
    case 0x111:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_check_sidejob_pay(arg0->param_);
        }
        break;
    case 0x175:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_ship_pos(arg0->param_);
        }
        break;
    case 0x173:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_not_go_into_tenku(arg0->param_);
        }
        break;
    case 0x174:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_map_black(arg0->param_);
        }
        break;
    case 0x177:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_end_roll_clear(arg0->param_);
        }
        break;
    case 0x114:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_endor_event_item(arg0->param_);
        }
        break;
    case 0x115:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_check_endor_event_item(arg0->param_);
        }
        break;
    case 0x12E:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_change_surechigai_part(arg0->param_);
        }
        break;
    case 0x12F:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_check_surechigai_success(arg0->param_);
        }
        break;
    case 0x132:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_ikada_info(arg0->param_);
        }
        break;
    case 0x131:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_check_taishi_max(arg0->param_);
        }
        break;
    case 0x134:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_default_map_name(arg0->param_);
        }
        break;
    case 0x136:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_player_henge_endless(arg0->param_);
        }
        break;
    case 0x137:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_open_door(arg0->param_);
        }
        break;
    case 0x138:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_chara_lock_move(arg0->param_);
        }
        break;
    case 0x139:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_random_message(arg0->param_);
        }
        break;
    case 0x13B:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_get_reward_tom(arg0->param_);
        }
        break;
    case 0x13E:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_add_nene_count(arg0->param_);
        }
        break;
    case 0x141:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_check_hit_surface(arg0->param_);
        }
        break;
    case 0x143:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_party_mark(arg0->param_);
        }
        break;
    case 0x14B:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_door_close(arg0->param_);
        }
        break;
    case 0x14C:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_map_camera_default_angle(arg0->param_);
        }
        break;
    case 0x153:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_start_game(arg0->param_);
        }
        break;
    case 0x14D:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_chara_mortion_lock(arg0->param_);
        }
        break;
    case 0x14E:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_check_hero_sex(arg0->param_);
        }
        break;
    case 0x154:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_opening_backcolor(arg0->param_);
        }
        break;
    case 0x156:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_fighting_colosseum_mode(arg0->param_);
        }
        break;
    case 0x159:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_van_and_basha(arg0->param_);
        }
        break;
    case 0x15F:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_basha_go_into(arg0->param_);
        }
        break;
    case 0x15A:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_is_load_init(arg0->param_);
        }
        break;
    case 0x15B:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_check_search_action(arg0->param_);
        }
        break;
    case 0x15C:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_no_search_message(arg0->param_);
        }
        break;
    case 0x150:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_macro_x_item1(arg0->param_);
        }
        break;
    case 0x14F:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_macro_i_name(arg0->param_);
        }
        break;
    case 0x162:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_check_player_item(arg0->param_);
        }
        break;
    case 0x167:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_chara_motion2(arg0->param_);
        }
        break;
    case 0x16B:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_unused_extra_chara(arg0->param_);
        }
        break;
    case 0x16C:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_check_shoplist(arg0->param_);
        }
        break;
    case 0x8D:
        var_r4 = g_cmd_count.exec(arg0);
        break;
    case 0x4B:
        var_r4 = g_cmd_character_move.exec(arg0);
        break;
    case 0x4C:
        var_r4 = g_cmd_character_move2.exec(arg0);
        break;
    case 0x4F:
        var_r4 = g_cmd_character_wait.exec(arg0);
        break;
    case 0x94:
        var_r4 = g_cmd_player_move.exec(arg0);
        break;
    case 0x95:
        var_r4 = g_cmd_player_move2.exec(arg0);
        break;
    case 0x96:
        var_r4 = g_cmd_player_wait.exec(arg0);
        break;
    case 0x9:
        var_r4 = g_cmd_wait.exec(arg0);
        break;
    case 0x51:
        var_r4 = g_cmd_character_effect_mark.exec(arg0);
        break;
    case 0x1C:
        var_r4 = g_cmd_map_animation_b.exec(arg0);
        break;
    case 0x98:
        var_r4 = g_cmd_menu_shop.exec(arg0);
        break;
    case 0x3C:
        var_r4 = g_cmd_menu_extra_shop.exec(arg0);
        break;
    case 0x8A:
        var_r4 = g_cmd_menu_present_exp.exec(arg0);
        break;
    case 0x3D:
        var_r4 = g_cmd_menu_colosseum.exec(arg0);
        break;
    case 0x8B:
        var_r4 = g_cmd_menu_hostage.exec(arg0);
        break;
    case 0x8C:
        var_r4 = g_cmd_menu_nene.exec(arg0);
        break;
    case 0x56:
        var_r4 = g_cmd_character_action_tremble.exec(arg0);
        break;
    case 0x57:
        var_r4 = g_cmd_character_action_vanish.exec(arg0);
        break;
    case 0x99:
        var_r4 = g_cmd_menu_yes_no.exec(arg0);
        break;
    case 0xC:
        var_r4 = g_cmd_fade_in.exec(arg0);
        break;
    case 0xD:
        var_r4 = g_cmd_fade_out.exec(arg0);
        break;
    case 0xE:
        var_r4 = g_cmd_fade_in2.exec(arg0);
        break;
    case 0xF:
        var_r4 = g_cmd_fade_out2.exec(arg0);
        break;
    case 0x68:
        var_r4 = g_cmd_furniture_move.exec(arg0);
        break;
    case 0x69:
        var_r4 = g_cmd_furniture_move2.exec(arg0);
        break;
    case 0x11C:
        var_r4 = g_cmd_furniture_open.exec(arg0);
        break;
    case 0xAB:
        var_r4 = g_cmd_effect_wait.exec(arg0);
        break;
    case 0xAD:
        var_r4 = g_cmd_effect_move.exec(arg0);
        break;
    case 0xAE:
        var_r4 = g_cmd_effect_fade.exec(arg0);
        break;
    case 0xA8:
        var_r4 = g_cmd_map_flash.exec(arg0);
        break;
    case 0xA9:
        var_r4 = g_cmd_map_blend_color.exec(arg0);
        break;
    case 0xAA:
        var_r4 = g_cmd_map_blend_init.exec(arg0);
        break;
    case 0xB3:
        var_r4 = g_cmd_event_chapter_title.exec(arg0);
        break;
    case 0xA3:
        var_r4 = g_cmd_set_party_order.exec(arg0);
        break;
    case 0x5C:
        var_r4 = g_cmd_character_move_to.exec(arg0);
        break;
    case 0x5D:
        var_r4 = g_cmd_character_move2_to.exec(arg0);
        break;
    case 0x7E:
        var_r4 = g_cmd_player_move_to.exec(arg0);
        break;
    case 0x7F:
        var_r4 = g_cmd_player_move2_to.exec(arg0);
        break;
    case 0xA6:
        var_r4 = g_cmd_party_move_overlap.exec(arg0);
        break;
    case 0x166:
        var_r4 = g_cmd_party_move_to_first2.exec(arg0);
        break;
    case 0x5E:
        var_r4 = g_cmd_character_move_party.exec(arg0);
        break;
    case 0x5F:
        var_r4 = g_cmd_character_move2_party.exec(arg0);
        break;
    case 0x60:
        var_r4 = g_cmd_character_move_player.exec(arg0);
        break;
    case 0x61:
        var_r4 = g_cmd_character_move2_player.exec(arg0);
        break;
    case 0x2E:
        var_r4 = g_cmd_character_move_relative.exec(arg0);
        break;
    case 0x2F:
        var_r4 = g_cmd_character_move2_relative.exec(arg0);
        break;
    case 0x30:
        var_r4 = g_cmd_character_move_x.exec(arg0);
        break;
    case 0x31:
        var_r4 = g_cmd_character_move2_x.exec(arg0);
        break;
    case 0x32:
        var_r4 = g_cmd_character_move_z.exec(arg0);
        break;
    case 0x33:
        var_r4 = g_cmd_character_move2_z.exec(arg0);
        break;
    case 0x91:
        var_r4 = g_cmd_party_move2_formation.exec(arg0);
        break;
    case 0x21:
        var_r4 = g_cmd_map_camera_move.exec(arg0);
        break;
    case 0x22:
        var_r4 = g_cmd_map_camera_position.exec(arg0);
        break;
    case 0xB6:
        var_r4 = g_cmd_camera_move_abs.exec(arg0);
        break;
    case 0x23:
        var_r4 = g_cmd_map_camera_angle.exec(arg0);
        break;
    case 0x24:
        var_r4 = g_cmd_map_camera_gaze.exec(arg0);
        break;
    case 0x15:
        var_r4 = g_cmd_message1_self_closing.exec(arg0);
        break;
    case 0x2C:
        var_r4 = g_cmd_character_action_turn.exec(arg0);
        break;
    case 0x2D:
        var_r4 = g_cmd_character_action_gaze.exec(arg0);
        break;
    case 0x92:
        var_r4 = g_cmd_player_move_jump.exec(arg0);
        break;
    case 0x93:
        var_r4 = g_cmd_player_move2_jump.exec(arg0);
        break;
    case 0xA4:
        var_r4 = g_cmd_map_event_camera.exec(arg0);
        break;
    case 0xB4:
        var_r4 = g_cmd_camera_change_distance.exec(arg0);
        break;
    case 0xC6:
        var_r4 = g_cmd_camera_reset_distance.exec(arg0);
        break;
    case 0xBA:
        var_r4 = g_cmd_player_rot.exec(arg0);
        break;
    case 0xC5:
        var_r4 = g_cmd_menu_event_imuru.exec(arg0);
        break;
    case 0xCC:
        var_r4 = g_cmd_map_set_back_color.exec(arg0);
        break;
    case 0x171:
        var_r4 = g_cmd_character_rgb_anim2.exec(arg0);
        break;
    case 0xCD:
        var_r4 = g_cmd_map_restore_back_color.exec(arg0);
        break;
    case 0xC3:
        var_r4 = g_cmd_camera_move_pov.exec(arg0);
        break;
    case 0xB9:
        var_r4 = g_cmd_camera_move_to_player.exec(arg0);
        break;
    case 0xC0:
        var_r4 = g_cmd_field_player_move_to.exec(arg0);
        break;
    case 0xBD:
        var_r4 = g_cmd_field_player_set_ballon.exec(arg0);
        break;
    case 0xC8:
        var_r4 = g_cmd_field_get_down_ship.exec(arg0);
        break;
    case 0xCF:
        var_r4 = g_cmd_menu_save.exec(arg0);
        break;
    case 0xDB:
        var_r4 = g_cmd_speak_to_player_self_closing.exec(arg0);
        break;
    case 0xDD:
        var_r4 = g_cmd_riseup_move.exec(arg0);
        break;
    case 0xE7:
        var_r4 = g_cmd_charcter_3d_motion.exec(arg0);
        break;
    case 0x14A:
        var_r4 = g_cmd_charcter_motion.exec(arg0);
        break;
    case 0xE8:
        var_r4 = g_cmd_character_palette.exec(arg0);
        break;
    case 0xEF:
        var_r4 = g_cmd_music_volume.exec(arg0);
        break;
    case 0xF4:
        var_r4 = g_cmd_map_texture_scale.exec(arg0);
        break;
    case 0x82:
        var_r4 = g_cmd_character_action_jump.exec(arg0);
        break;
    case 0xF9:
        var_r4 = g_cmd_character_normal_jump.exec(arg0);
        break;
    case 0x83:
        var_r4 = g_cmd_fadein_character.exec(arg0);
        break;
    case 0x84:
        var_r4 = g_cmd_fadeout_character.exec(arg0);
        break;
    case 0xED:
        var_r4 = g_cmd_player_effect_mark.exec(arg0);
        break;
    case 0x109:
        var_r4 = g_cmd_player_line_move.exec(arg0);
        break;
    case 0x10A:
        var_r4 = g_cmd_player_line_move2.exec(arg0);
        break;
    case 0x116:
        var_r4 = g_cmd_ikada_move2_player_get_on.exec(arg0);
        break;
    case 0x117:
        var_r4 = g_cmd_ikada_move_player_get_on.exec(arg0);
        break;
    case 0x121:
        var_r4 = g_cmd_set_camera_target_chara_frame.exec(arg0);
        break;
    case 0x122:
        var_r4 = g_cmd_set_camera_angle_abs.exec(arg0);
        break;
    case 0x11D:
        var_r4 = g_cmd_door_action.exec(arg0);
        break;
    case 0x130:
        var_r4 = g_cmd_surechigai_mapname.exec(arg0);
        break;
    case 0x12B:
        var_r4 = g_cmd_make_surechigai_taishi.exec(arg0);
        break;
    case 0x12C:
        var_r4 = g_cmd_surechigai_save.exec(arg0);
        break;
    case 0x12D:
        var_r4 = g_cmd_surechigai_root.exec(arg0);
        break;
    case 0x133:
        var_r4 = g_cmd_surechigai_message.exec(arg0);
        break;
    case 0x11E:
        var_r4 = g_cmd_key_wait_type_b.exec(arg0);
        break;
    case 0x172:
        var_r4 = g_cmd_the_end.exec(arg0);
        break;
    case 0x13A:
        var_r4 = g_cmd_play_music.exec(arg0);
        break;
    case 0x13F:
        var_r4 = g_cmd_set_wait_enable_lock.exec(arg0);
        break;
    case 0x142:
        var_r4 = g_cmd_message_with_sound.exec(arg0);
        break;
    case 0x148:
        var_r4 = g_cmd_chara_move_line_to_player.exec(arg0);
        break;
    case 0x15D:
        var_r4 = g_cmd_field_move_line.exec(arg0);
        break;
    case 0x151:
        var_r4 = g_cmd_set_chara_rot.exec(arg0);
        break;
    case 0x168:
        var_r4 = g_cmd_set_end_roll.exec(arg0);
        break;
    case 0x90:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_string_print(arg0->param_);
        }
        break;
    case 0x8F:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_set_script_object_direction(arg0->param_);
        }
        break;
    case 0xC1:
        if (checkCommandType(arg0) != 0) {
            var_r4 = cmd_map_clipping(arg0->param_);
        }
        break;
    default:
        if (checkCommandType(arg0) != 0) {
            var_r4 = 1;
        }
        break;
    }
    return var_r4;
}

__cmd_count g_cmd_count;
__cmd_wait g_cmd_wait;
__cmd_map_animation_b g_cmd_map_animation_b;
__cmd_character_action_tremble g_cmd_character_action_tremble;
__cmd_character_action_vanish g_cmd_character_action_vanish;
__cmd_menu_yes_no g_cmd_menu_yes_no;
__cmd_message_with_sound g_cmd_message_with_sound;
