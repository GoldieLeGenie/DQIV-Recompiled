#pragma once
#include "main/cmn/ActionBase.hpp"
#include "main/text/TextAPI.hpp"
#include "main/cmn/CommonCalculate.hpp"
#include "main/script/ScriptBaseCommand.hpp"
#include "main/window/CommandWindow.hpp"
#include "globaldefs.h"
#include "GameInfo.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/UnkLanguage.hpp"
#include "main/dss/UnkModelMember.hpp"
#include "main/global/StageLink.hpp"
#include "ov000/town/TownCharacterManager.hpp"
#include "ov000/town/TownCamera.hpp"
#include "ov000/town/TownDoorAction.hpp"
#include "main/Commands/CommonCommand.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/cmn/CommonEffectLocation.hpp"
#include "ov016/casino/MaterielMenu_SLOT/MaterielMenu_SLOT.hpp"
#include "main/profile/Profile.hpp"
#include "main/dss/UnkPaletteEffect.hpp"

struct TownPartyDraw;
struct TownFurnitureManager;
struct TownStageManager;
struct TownPlayerManager;

struct TownCharaMoveParam {
    dss::Fix32Vector3 pos_[4];
    dss::Fix32 speed_;
    int unk_34;
    int unk_38;
};

void searchItem(int index, int* found, int* items);
int cmd_enable_event_item(int* param);
int cmn_set_event_door(int* param);
int cmd_map_change_timezone(int* param);
int cmd_map_effect_sepia();
int cmd_character_not_change_direction(int* param);
int cmd_player_action_not_change_direction(int* param);
int cmd_is_character_direction(int* param);
int cmd_is_player_direction(int* param);
int cmd_map_animation(int* param);
int cmd_is_party_item(int* param);
int cmd_is_not_party_item(int* param);
int cmd_set_hostage();
int cmd_set_party_reserve_order(int* param);
int cmd_map_texture(int* param);
int cmd_set_map_texture(int* param);
int cmd_map_shake(int* param);
int cmd_effect_blur(int* param);
int cmd_party_display(int* param);
int cmd_is_character_front(int* param);
int cmd_is_talked_at_shop(int* param);
int cmd_search_map_object(int* param);
int cmd_set_floor_map_object(int* param);
int cmd_character_move_passive(int* param);
int cmd_character_move_random(int* param);
int cmd_character_move_reverse(int* param);
int cmd_is_trigger_distance(int* param);
int cmd_character_set_coll_stage(int* param);
int cmd_party_redisplay(int* param);

int cmd_party_display2(int* param);
int cmn_camera_lock_pov(int* param);
int cmd_furniture_fadeout(int* param);
int cmd_effect_dream(int* param);
int cmd_set_script_object_direction(int* param);
int cmd_charcter_3d_rotate(int* param);
int cmd_effect_transfer(int* param);
int cmd_character_pose_change(int* param);
int cmd_player_action_dance(int* param);
int cmd_map_camera_lock_target_player(int* param);
int cmd_player_set_coll(int* param);
int cmd_map_clipping(int* param);
int cmd_camera_clip_distance(int* param);
int cmd_map_camera_near(int* param);
int cmd_disable_demolition();
int cmd_set_camera_target(int* param);
int cmd_character_swing_round(int* param);
int cmd_character_anim(int* param);
int cmd_party_copy_character(int* param);
int cmd_is_map_treasure(int* param);
int cmd_set_furniture_position(int* param);
int cmd_is_doorway(int* param);
int cmd_copy_party_chara(int* param);
int cmd_chara_shadow(int* param);
int cmd_chara_alpha(int* param);
int cmd_ikada_set_position(int* param);
int cmd_crack_key_by_orin(int* param);
int cmd_set_big_rock_move(int* param);
int cmd_set_monster_talk(int* param);
int cmd_set_monster_talk_all(int* param);
int cmd_set_chara_map_uid(int* param);
int cmd_change_surechigai_part(int* param);
int cmd_check_surechigai_success(int* param);
int cmd_set_default_map_name(int* param);
int cmd_set_ikada_info(int* param);
int cmd_set_player_sleep(int* param);
int cmd_is_open_door(int* param);
int cmd_chara_lock_move(int* param);
int cmd_get_reward_tom(int* param);
int cmd_check_hit_surface(int* param);
int cmd_set_door_close(int* param);
int cmd_start_game(int* param);
int cmd_opening_backcolor(int* param);
int cmd_set_camera_limit(int* param);
int cmd_set_van_and_basha(int* param);
int cmd_set_basha_go_into(int* param);
int cmd_chara_voice(int* param);
int cmd_check_player_item(int* param);
int cmd_save_last_party();
int cmd_set_chara_motion2(int* param);
int cmd_set_unused_extra_chara(int* param);
int cmd_check_shoplist(int* param);
int cmd_is_not_go_into_tenku(int* param);
int cmd_set_end_roll_clear(int* param);

int cmd_set_my_taishi(int* param);
int cmd_check_taishi_max(int* param);
int cmd_map_camera_default_angle(int* param);
int cmd_chara_mortion_lock(int* param);
int cmd_set_fighting_colosseum_mode(int* param);
int cmd_map_black(int* param);

int cmd_debug_print(int* param);
int cmd_set_item(int* param);
int cmd_set_gold(int* param);
int cmd_set_coin(int* param);
int cmd_is_procure_item(int* param);
int cmd_mini_game(int* param);
int cmd_is_hostage(int* param);
int cmd_set_ruura_lock(int* param);
int cmd_set_ranaruta(int* param);
int cmd_invalidation_rula(int* param);
int cmd_check_money(int* param);
int cmd_check_hero_level(int* param);
int cmd_reset_sidejob_pay(int* param);
int cmd_get_sidejob_pay(int* param);
int cmd_check_sidejob_pay(int* param);
int cmd_set_endor_event_item(int* param);
int cmd_check_endor_event_item(int* param);
int cmd_furniture_move_request(int* param);

int cmd_set_overview_point(int* param);
int cmd_set_player_position(int* param);
int cmd_set_player_direction(int* param);
int cmd_is_speaked(int* param);
int cmd_speak_to_player(int* param);
int cmd_speak_to_player2(int* param);
int cmd_set_x_wins(int* param);

int cmd_set_character_position(int* param);
int cmd_set_character_direction(int* param);
int cmd_character_action_sleep(int* param);
int cmd_character_action_display(int* param);
int cmd_character_action_near(int* param);
int cmd_map_animation_a(int* param);
int cmd_set_map_collision(int* param);
int cmd_chara_set_priority_sure_appointment(int* param);
int cmd_chara_set_normal_sure_appointment(int* param);
int cmd_chara_set_normal_sure(int* param);
int cmd_chara_talk_to_player_sure(int* param);
int cmd_set_surechigai_level(int* param);
int cmd_character_action_stepping(int* param);
int cmd_character_action_still(int* param);
int cmd_character_action_wriggle(int* param);
int cmd_player_action_stepping(int* param);
int cmd_player_action_still(int* param);
int cmd_player_action_wriggle(int* param);
int cmd_set_character_collision(int* param);
int cmd_is_trigger(int* param);
int cmd_is_trigger2(int* param);
int cmd_is_trigger3(int* param);
int cmd_character_action_pursue(int* param);
int cmd_character_move_roam(int* param);
int cmd_is_trigger_character(int* param);
int cmd_is_trigger2_character(int* param);
int cmd_party_join(int* param);
int cmd_party_quit(int* param);

extern "C" {
}
