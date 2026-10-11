#pragma once
#include "main/text/TextAPI.hpp"
#include "main/global/GlobalDQ4.hpp"
struct TownPartyDraw;
#include "main/cmn/CommonCalculate.hpp"
#include "main/script/ScriptBaseCommand.hpp"
#include "main/window/CommandWindow.hpp"
#include "globaldefs.h"
#include "GameInfo.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/object/DisplayCharacter.hpp"
#include "main/encount/Encount.hpp"
#include "main/cmn/GameManager.hpp"
#include "main/script/ScriptSystem.hpp"
#include "ov000/town/TownSystem.hpp"
#include "ov001/fld/FieldSystem.hpp"

struct FieldPlayerManager;
struct TownPlayerManager;
struct TownCharacterManager;
struct TownStageManager;

struct CommandParameter;

struct ScriptCommand {
    virtual void initialize(char* scriptParam) = 0;
    virtual void execute() {}   
    virtual void terminate() {} 
    virtual int isEnd() = 0;
    ScriptCommand() {}
    ~ScriptCommand() {}
    int exec(CommandParameter* command);
};
int checkCommandType(CommandParameter* command);

enum TriggerCheck {
    TRIGGER_CHECK_0,
    TRIGGER_CHECK_1,
    TRIGGER_CHECK_2,
    TRIGGER_CHECK_3,
    TRIGGER_CHECK_4,
    TRIGGER_CHECK_5,
};

int cmd_setup(int* param);
int cmd_message1(int* param);
int cmd_message2(int* param);
int cmd_random_message(int* param);
int cmd_is_timezone(int* param);
int cmd_set_timezone(int* param);
int cmd_is_player_status_dead(int* param);
int cmd_is_party_ride_carriage(int* param);
int cmd_is_party_member(int* param);
int cmd_is_party_order(int* param);
int cmd_set_party_total_recovery(int* param);

int cmd_get_flag(int* param);
int cmd_set_flag(int* param);
int cmd_encount(int* param);
int cmd_encount_set_flag(int* param);
int cmd_encount_first_strike(int* param);
int cmd_set_party_join_carriage();
int cmd_set_town_to_field_link(int* param);
int cmn_set_field_to_town_link(int* param);
int cmd_is_trigger_forward(int* param);
int cmd_set_encount_disable(int* param);
int cmd_set_encount_stage_disable(int* param);
int cmd_set_demolition_fightingarena(int* param);
int cmd_event_chapter_end(int* param);
int cmd_chapter_restore(int* param);
int cmd_chapter_restore_coin(int* param);
int cmd_is_battle_turn(int* param);
int cmd_set_random(int* param);
int cmd_set_random2(int* param);
int cmd_set_music(int* param);
int cmd_music_pause(int* param);
int cmd_play_music_now_map(int* param);
int cmd_set_wait_counter(int* param);
int cmd_is_wait_counter(int* param);
int cmd_is_push_key(int* param);
int cmd_wait_operation(int* param);
int cmd_set_day_count(int* param);
int cmd_is_day_count(int* param);
int cmd_add_nene_count(int* param);
int cmd_set_chapter(int* param);
int cmd_chapter_store(int* param);
int cmd_map_link_field_direct(int* param);
int cmd_floor_change(int* param);
int cmd_floor_exit(int* param);
int cmd_set_map_link_on_off(int* param);
int cmd_change_map_link(int* param);
int cmd_set_vehicle(int* param);
int cmd_not_play_normal_sound(int* param);
int cmd_check_member_type(int* param);
int cmd_check_member_num(int* param);
int cmd_set_sleep_near(int* param);
int cmd_string_print(int* param);
int cmd_player_lock(int* param);
int cmd_set_macro_actor();
int cmd_set_macro_target();
int cmd_set_macro_target_index(int* param);
int cmd_set_macro_prisoner();
int cmd_set_macro_x_item1(int* param);
int cmd_set_macro_i_name(int* param);
int cmd_barrier_disruption(int* param);
int cmd_is_barrier_disruption(int* param);
int cmd_set_party_join(int* param);
int cmd_set_party_quit(int* param);
int cmd_party_del2(int* param);
int cmd_battle_end_flag_set(int* param);
int cmd_not_use_load_message();
int cmd_set_message_sound(int* param);
int cmd_is_monster(int* param);
int cmd_set_party_mark(int* param);
int cmd_set_player_henge_endless(int* param);
int cmd_set_party_call_carriage(int* param);
int cmd_set_forward_counter(int* param);
int cmd_setup_music(int* param);
int cmd_check_hero_sex(int* param);
int cmd_stream_play(int* param);
int cmd_is_load_init(int* param);
int cmd_check_search_action(int* param);
int cmd_set_no_search_message(int* param);
int cmd_set_timezone_pause(int* param);
int cmd_is_party_top(int* param);
int cmd_is_not_party_top(int* param);
int cmd_is_party_all(int* param);
int cmd_is_not_party_all(int* param);
int cmd_is_party_head_count(int* param);
int cmd_is_not_party_head_count(int* param);
int cmd_set_se(int* param);
int cmd_cut_se(int* param);
int cmd_set_player_recovery(int* param);
int cmd_set_player_in_carriage(int* param);
int cmd_set_player_ride_on(int* param);
int cmd_set_ship_pos(int* param);
int cmd_set_title_part(int* param);


short unkfunc_0202528c(int* param);
short unkfunc_020254a4(short count, short alive, int mode);
int unkfunc_020254b8(int index, int type);
int unkfunc_02025514(int index, int type);
int unkfunc_0202555c(int index, int type);
