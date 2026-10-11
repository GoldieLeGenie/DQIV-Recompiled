#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/Commands/CommonCommand.hpp"
#include "ov000/town/TownWindowSystem.hpp"

struct PARAM_IS_TRIGGER3 {
    int startX;                      // 0x00
    int startY;                      // 0x04
    int startZ;                      // 0x08
    int endX;                        // 0x0C
    int endY;                        // 0x10
    int endZ;                        // 0x14
};

struct _check_shop_end {
    int m_flag;                      // 0x00

    void init() { m_flag = 0; }
    bool check()
    {
        if (!TownWindowSystem::getSingleton()->cmdWindow_.isShopMenu() && !m_flag) {
            m_flag = 1;
            return false;
        }
        if (!TownWindowSystem::getSingleton()->cmdWindow_.isShopMenu() && m_flag) {
            return true;
        }
        return false;
    }
};

struct PARAM_PLAYER_MOVE {
    int startX;                      // 0x00
    int startY;                      // 0x04
    int startZ;                      // 0x08
    int endX;                        // 0x0C
    int endY;                        // 0x10
    int endZ;                        // 0x14
    unsigned int frame;              // 0x18
};

struct PARAM_PLAYER_MOVE2 {
    int startX;                      // 0x00
    int startY;                      // 0x04
    int startZ;                      // 0x08
    int endX;                        // 0x0C
    int endY;                        // 0x10
    int endZ;                        // 0x14
    int rate;                        // 0x18
};

struct PARAM_PLAYER_WAIT {
    unsigned int frame;              // 0x00
};

struct PARAM_PLAYER_MOVE_TO {
    int endX;                        // 0x00
    int endY;                        // 0x04
    int endZ;                        // 0x08
    unsigned int frame;              // 0x0C
    unsigned int absFlag;            // 0x10
};

struct PARAM_PLAYER_MOVE2_TO {
    int endX;                        // 0x00
    int endY;                        // 0x04
    int endZ;                        // 0x08
    int rate;                        // 0x0C
    unsigned int absFlag;            // 0x10
};

struct PARAM_PARTY_MOVE2_FORMATION {
    unsigned int frmDir;             // 0x00
    unsigned int charaDir;           // 0x04
    int rate;                        // 0x08
};

struct PARAM_PLAYER_MOVE_JUMP {
    int endX;                        // 0x00
    int endY;                        // 0x04
    int endZ;                        // 0x08
    unsigned int direction;          // 0x0C
    unsigned int frame;              // 0x10
};

struct PARAM_PLAYER_MOVE2_JUMP {
    int endX;                        // 0x00
    int endY;                        // 0x04
    int endZ;                        // 0x08
    unsigned int direction;          // 0x0C
    int rate;                        // 0x10
};

struct PARAM_CHARACTER_MOVE {
    int startX;                      // 0x00
    int startY;                      // 0x04
    int startZ;                      // 0x08
    int endX;                        // 0x0C
    int endY;                        // 0x10
    int endZ;                        // 0x14
    unsigned int frame;              // 0x18
};

struct PARAM_CHARACTER_MOVE2 {
    int startX;                      // 0x00
    int startY;                      // 0x04
    int startZ;                      // 0x08
    int endX;                        // 0x0C
    int endY;                        // 0x10
    int endZ;                        // 0x14
    int rate;                        // 0x18
};

struct PARAM_CHARACTER_WAIT {
    unsigned int frame;              // 0x00
};

struct PARAM_CHARACTER_EFFECT_MARK {
    unsigned int mark;               // 0x00
};

struct PARAM_CHARACTER_MOVE_TO {
    int endX;                        // 0x00
    int endY;                        // 0x04
    int endZ;                        // 0x08
    unsigned int frame;              // 0x0C
};

struct PARAM_CHARACTER_MOVE2_TO {
    int endX;                        // 0x00
    int endY;                        // 0x04
    int endZ;                        // 0x08
    int rate;                        // 0x0C
};

struct PARAM_CHARACTER_MOVE_PARTY {
    unsigned int frame;              // 0x00
    unsigned int mode;               // 0x04
};

struct PARAM_CHARACTER_MOVE2_PARTY {
    int rate;                        // 0x00
    unsigned int mode;               // 0x04
};

struct PARAM_CHARACTER_MOVE_PLAYER {
    unsigned int direction;          // 0x00
    int adjust;                      // 0x04
    unsigned int frame;              // 0x08
    unsigned int mode;               // 0x0C
};

struct PARAM_CHARACTER_MOVE2_PLAYER {
    unsigned int direction;          // 0x00
    int adjust;                      // 0x04
    int rate;                        // 0x08
    unsigned int mode;               // 0x0C
};

struct PARAM_CHARACTER_MOVE_RELATIVE {
    int endX;                        // 0x00
    int endY;                        // 0x04
    int endZ;                        // 0x08
    unsigned int frame;              // 0x0C
};

struct PARAM_CHARACTER_MOVE2_RELATIVE {
    int endX;                        // 0x00
    int endY;                        // 0x04
    int endZ;                        // 0x08
    int rate;                        // 0x0C
};

struct PARAM_CHARACTER_MOVE_X {
    int endX;                        // 0x00
    unsigned int frame;              // 0x04
};

struct PARAM_CHARACTER_MOVE2_X {
    int endX;                        // 0x00
    int rate;                        // 0x04
};

struct PARAM_CHARACTER_MOVE_Z {
    int endZ;                        // 0x00
    unsigned int frame;              // 0x04
};

struct PARAM_CHARACTER_MOVE2_Z {
    int endZ;                        // 0x00
    int rate;                        // 0x04
};

struct PARAM_CHARACTER_ACTION_TURN {
    unsigned int direction;          // 0x00
    unsigned int frame;              // 0x04
    unsigned int rot;                // 0x08
};

struct PARAM_CHARACTER_ACTION_GAZE {
    unsigned int frame;              // 0x00
};

struct PARAM_FURNITURE_MOVE {
    unsigned int target;             // 0x00
    int endX;                        // 0x04
    int endY;                        // 0x08
    int endZ;                        // 0x0C
    unsigned int frame;              // 0x10
};

struct PARAM_FURNITURE_MOVE2 {
    unsigned int target;             // 0x00
    int endX;                        // 0x04
    int endY;                        // 0x08
    int endZ;                        // 0x0C
    int rate;                        // 0x10
};

struct PARAM_EFFECT_WAIT {
    unsigned int effect;             // 0x00
    int posX;                        // 0x04
    int posY;                        // 0x08
    int posZ;                        // 0x0C
    unsigned int flag;               // 0x10
};

struct PARAM_EFFECT_MOVE {
    unsigned int effect;             // 0x00
    int startX;                      // 0x04
    int startY;                      // 0x08
    int startZ;                      // 0x0C
    int endX;                        // 0x10
    int endY;                        // 0x14
    int endZ;                        // 0x18
    unsigned int frame;              // 0x1C
};

struct PARAM_EFFECT_FADE {
    unsigned int effect;             // 0x00
    int posX;                        // 0x04
    int posY;                        // 0x08
    int posZ;                        // 0x0C
    unsigned int frame;              // 0x10
    unsigned int flag;               // 0x14
};

struct PARAM_MAP_FLASH {
    unsigned int r;                  // 0x00
    unsigned int g;                  // 0x04
    unsigned int b;                  // 0x08
    unsigned int frame;              // 0x0C
    unsigned int se;                 // 0x10
};

struct PARAM_MAP_BLEND_COLOR {
    int r;                           // 0x00
    int g;                           // 0x04
    int b;                           // 0x08
    unsigned int frame;              // 0x0C
};

struct PARAM_MAP_TEXTURE_SCALE {
    unsigned int frame;              // 0x00
    int scalex;                      // 0x04
    int scaley;                      // 0x08
};

struct PARAM_MAP_BLEND_INIT {
    unsigned int frame;              // 0x00
};

struct PARAM_MAP_CAMERA_MOVE {
    int endX;                        // 0x00
    int endY;                        // 0x04
    int endZ;                        // 0x08
    unsigned int moveFrame;          // 0x0C
    unsigned int axis;               // 0x10
    unsigned int angle;              // 0x14
    unsigned int rotFrame;           // 0x18
};

struct PARAM_MAP_CAMERA_POSITION {
    int endX;                        // 0x00
    int endY;                        // 0x04
    int endZ;                        // 0x08
    unsigned int frame;              // 0x0C
};

struct PARAM_CAMERA_MOVE_ABS {
    int posX;                        // 0x00
    int posY;                        // 0x04
    int posZ;                        // 0x08
    unsigned int frame;              // 0x0C
};

struct PARAM_MAP_CAMERA_ANGLE {
    unsigned int axis;               // 0x00
    unsigned int angle;              // 0x04
    unsigned int frame;              // 0x08
};

struct PARAM_MAP_CAMERA_GAZE {
    unsigned int frame;              // 0x00
};

struct PARAM_MAP_EVENT_CAMERA {
    unsigned int channel;            // 0x00
    unsigned int screen;             // 0x04
    unsigned int frame;              // 0x08
};

struct PARAM_RISEUP_MOVE {
    int startX;                      // 0x00
    int startY;                      // 0x04
    int startZ;                      // 0x08
    int endX;                        // 0x0C
    int endY;                        // 0x10
    int endZ;                        // 0x14
    unsigned int item;               // 0x18
    unsigned int frame;              // 0x1C
};

struct PARAM_MENU_EVENT_IMURU {
    unsigned int type;               // 0x00
    unsigned int index;              // 0x04
};

struct PARAM_MAP_SET_BACK_COLOR {
    unsigned int index;              // 0x00
    unsigned int frame;              // 0x04
};

struct PARAM_MAP_RESTORE_BACK_COLOR {
    unsigned int frame;              // 0x00
};

struct PARAM_FURNITURE_OPEN {
    unsigned int target;             // 0x00
};

struct PARAM_SET_PARTY_ORDER {
    unsigned int order1;             // 0x00
    unsigned int direction;          // 0x04
};

struct PARAM_CHARACTER_NORMAL_JUMP {
    int endX;                        // 0x00
    int endY;                        // 0x04
    int endZ;                        // 0x08
    unsigned int frame;              // 0x0C
};

struct PARAM_CAMERA_CHANGE_DISTANCE {
    int distance;                    // 0x00
    unsigned int frame;              // 0x04
};

struct PARAM_PLAYER_ROT {
    unsigned int frame;              // 0x00
    unsigned int idx;                // 0x04
    unsigned int rotFlag;            // 0x08
    unsigned int endFlag;            // 0x0C
};

struct PARAM_CAMERA_RESET_DISTANCE {
    unsigned int frame;              // 0x00
};

struct PARAM_CAMERA_MOVE_POV {
    int endX;                        // 0x00
    int endY;                        // 0x04
    int endZ;                        // 0x08
    unsigned int frame;              // 0x0C
    unsigned int absFlag;            // 0x10
};

struct PARAM_CAMERA_MOVE_TO_PLAYER {
    unsigned int frame;              // 0x00
};

struct PARAM_FADEIN_CHARACTER {
    unsigned int pattern;            // 0x00
    unsigned int frame;              // 0x04
};

struct PARAM_CHARCTER_3D_MOTION {
    unsigned int motion;             // 0x00
    unsigned int flag;               // 0x04
};

struct PARAM_CHARCTER_MOTION {
    unsigned int motion;             // 0x00
    unsigned int flag;               // 0x04
};

struct PARAM_MENU_SHOP {
    unsigned int shop;               // 0x00
};

struct PARAM_MENU_PRESENT_EXP {
    unsigned int exp;                // 0x00
    unsigned int index;              // 0x04
};

struct PARAM_MENU_COLOSSEUM {
    unsigned int wins;               // 0x00
};

struct PARAM_PLAYER_LINE_MOVE {
    unsigned int axis;               // 0x00
    unsigned int absFlag;            // 0x04
    int value;                       // 0x08
    unsigned int frame;              // 0x0C
};

struct PARAM_PLAYER_LINE_MOVE2 {
    unsigned int axis;               // 0x00
    unsigned int absFlag;            // 0x04
    int value;                       // 0x08
    int rate;                        // 0x0C
};

struct PARAM_IKADA_MOVE2_PLAYER_GET_ON {
    int posX;                        // 0x00
    int posY;                        // 0x04
    int posZ;                        // 0x08
    unsigned int absFlag;            // 0x0C
    int rate;                        // 0x10
};

struct PARAM_IKADA_MOVE_PLAYER_GET_ON {
    int posX;                        // 0x00
    int posY;                        // 0x04
    int posZ;                        // 0x08
    unsigned int absFlag;            // 0x0C
    unsigned int frame;              // 0x10
};

struct PARAM_SET_CAMERA_TARGET_CHARA_FRAME {
    unsigned int frame;              // 0x00
    unsigned int objId;              // 0x04
};

struct PARAM_SET_CAMERA_ANGLE_ABS {
    unsigned int angleY;             // 0x00
    unsigned int frame;              // 0x04
};

struct PARAM_DOOR_ACTION {
    unsigned int door1;              // 0x00
    unsigned int door2;              // 0x04
    unsigned int type;               // 0x08
};

struct PARAM_SET_WAIT_ENABLE_LOCK {
    unsigned int frame;              // 0x00
};

struct PARAM_CHARA_MOVE_LINE_TO_PLAYER {
    unsigned int line;               // 0x00
    int offset;                      // 0x04
    int rate;                        // 0x08
};

struct PARAM_SET_CHARA_ROT {
    unsigned int frame;              // 0x00
    unsigned int idx;                // 0x04
    unsigned int rotFlag;            // 0x08
    unsigned int endFlag;            // 0x0C
};

struct PARAM_PARTY_MOVE_TO_FIRST2 {
    unsigned int rate;               // 0x00
};

struct PARAM_CHARACTER_RGB_ANIM2 {
    unsigned int R;                  // 0x00
    unsigned int G;                  // 0x04
    unsigned int B;                  // 0x08
    unsigned int frame;              // 0x0C
    unsigned int flag;               // 0x10
};

/* vtable 0x02148cc0, object 0x0215fbc8 */
struct __cmd_player_move : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148a80, object 0x0215fb8c */
struct __cmd_player_move2 : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148918, object 0x0216010c */
struct __cmd_player_wait : ScriptCommand {
    int count_;                      // 0x04
    int countFrame_;                 // 0x08

    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148f90, object 0x0215fbc4 */
struct __cmd_player_move_to : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148e28, object 0x0215fbb0 */
struct __cmd_player_move2_to : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148ca8, object 0x0215fbe0 */
struct __cmd_party_move2_formation : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148b40, object 0x0215fbc0 */
struct __cmd_player_move_jump : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148a68, object 0x0215fbf4 */
struct __cmd_player_move2_jump : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x021489c0, object 0x0215fc4c */
struct __cmd_party_move_overlap : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148900, object 0x0215fcf0 */
struct __cmd_player_rot : ScriptCommand {
    int endFlag_;                   // 0x04

    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148858, object 0x0215fba4 */
struct __cmd_character_move : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148f78, object 0x0215fb90 */
struct __cmd_character_move2 : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148ed0, object 0x0215fbf8 */
struct __cmd_character_wait : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148e10, object 0x0215fbb8 */
struct __cmd_character_effect_mark : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148d68, object 0x0215fbd0 */
struct __cmd_character_move_to : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148c90, object 0x0215fb94 */
struct __cmd_character_move2_to : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148be8, object 0x0215fbf0 */
struct __cmd_character_move_party : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148b28, object 0x0215fba0 */
struct __cmd_character_move2_party : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148ac8, object 0x0215fc0c */
struct __cmd_character_move_player : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148a50, object 0x0215fbe8 */
struct __cmd_character_move2_player : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148a08, object 0x0215fc30 */
struct __cmd_character_move_relative : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x021489a8, object 0x0215fbe4 */
struct __cmd_character_move2_relative : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148960, object 0x0215fc1c */
struct __cmd_character_move_x : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x021488e8, object 0x0215fc08 */
struct __cmd_character_move2_x : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x021488a0, object 0x0215fc54 */
struct __cmd_character_move_z : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148840, object 0x0215fbd8 */
struct __cmd_character_move2_z : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x021487f8, object 0x0215fc40 */
struct __cmd_character_action_turn : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148f60, object 0x0215fbb4 */
struct __cmd_character_action_gaze : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148f18, object 0x0215fc34 */
struct __cmd_character_action_jump : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148eb8, object 0x0215fbd4 */
struct __cmd_character_normal_jump : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148e70, object 0x0215fc68 */
struct __cmd_furniture_move : ScriptCommand {
    int m_index;                     // 0x04

    virtual void initialize(char* scriptParam);
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148df8, object 0x0215fc58 */
struct __cmd_furniture_move2 : ScriptCommand {
    int m_index;                     // 0x04

    virtual void initialize(char* scriptParam);
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148db0, object 0x02160150 */
struct __cmd_map_texture_scale : ScriptCommand {
    int frame_;                      // 0x04
    int counter_;                    // 0x08
    fx32 scaleX_;                    // 0x0C
    fx32 scaleY_;                    // 0x10

    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual int isEnd();
};

/* vtable 0x02148d50, object 0x0215fc70 */
struct __cmd_effect_wait : ScriptCommand {
    int m_index;                     // 0x04

    virtual void initialize(char* scriptParam);
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148d08, object 0x0215fc88 */
struct __cmd_effect_move : ScriptCommand {
    int m_index;                     // 0x04

    virtual void initialize(char* scriptParam);
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148c78, object 0x0215fc78 */
struct __cmd_effect_fade : ScriptCommand {
    int m_index;                     // 0x04

    virtual void initialize(char* scriptParam);
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148c30, object 0x0215fda0 */
struct __cmd_map_flash : ScriptCommand {
    int count_;                      // 0x04
    int countFrame_;                 // 0x08

    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148bd0, object 0x021601a4 */
struct __cmd_map_blend_color : ScriptCommand {
    dss::Fix32Vector3 rate_;         // 0x04
    int count_;                      // 0x10
    int countFrame_;                 // 0x14

    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148b88, object 0x021601bc */
struct __cmd_map_blend_init : ScriptCommand {
    dss::Fix32Vector3 rate_;         // 0x04
    int count_;                      // 0x10
    int countFrame_;                 // 0x14

    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148b10, object 0x0215fca0 */
struct __cmd_riseup_move : ScriptCommand {
    int m_index;                     // 0x04

    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual int isEnd();
};

/* vtable 0x02148ae0, object 0x0215fc28 */
struct __cmd_map_camera_move : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148ab0, object 0x0215fbcc */
struct __cmd_map_camera_position : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148a98, object 0x0215fc38 */
struct __cmd_camera_move_abs : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148a38, object 0x0215fc48 */
struct __cmd_map_camera_angle : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148a20, object 0x0215fc44 */
struct __cmd_map_camera_gaze : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x021489f0, object 0x0215fb88 */
struct __cmd_camera_change_distance : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x021489d8, object 0x0215fbfc */
struct __cmd_camera_move_pov : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148990, object 0x0215fc24 */
struct __cmd_camera_reset_distance : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148978, object 0x0215fb9c */
struct __cmd_camera_move_to_player : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148948, object 0x0215fd10 */
struct __cmd_map_event_camera : ScriptCommand {
    int count_;                      // 0x04
    int countFrame_;                 // 0x08

    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148930, object 0x02160140 */
struct __cmd_menu_event_imuru : ScriptCommand {
    unsigned int type_;              // 0x04
    unsigned int index_;             // 0x08
    unsigned int waitCounter_;       // 0x0C

    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
    void setFlag(bool flag);
};

/* vtable 0x021488d0, object 0x02160164 */
struct __cmd_map_set_back_color : ScriptCommand {
    int current_;                    // 0x04
    int index_;                      // 0x08
    int count_;                      // 0x0C
    int countFrame_;                 // 0x10

    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x021488b8, object 0x02160178 */
struct __cmd_map_restore_back_color : ScriptCommand {
    int current_;                    // 0x04
    int index_;                      // 0x08
    int count_;                      // 0x0C
    int countFrame_;                 // 0x10

    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148888, object 0x0215fbbc */
struct __cmd_charcter_3d_motion : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual int isEnd();
};

/* vtable 0x02148870, object 0x0215fbec */
struct __cmd_charcter_motion : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual int isEnd();
};

/* vtable 0x02148828, object 0x0215fce0 */
struct __cmd_set_party_order : ScriptCommand {
    int change_;                    // 0x04

    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148810, object 0x021600f4 */
struct __cmd_furniture_open : ScriptCommand {
    int unk_04;                      // 0x04
    int uid_;                        // 0x08

    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x021487e0, object 0x0215fc00 */
struct __cmd_fadein_character : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x021487c8, object 0x0215fc04 */
struct __cmd_fadeout_character : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148f48, object 0x02160070 */
struct __cmd_menu_shop : ScriptCommand {
    int shop_;                       // 0x04
    _check_shop_end m_end;           // 0x08

    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148f30, object 0x0215fcb8 */
struct __cmd_menu_extra_shop : ScriptCommand {
    _check_shop_end m_end;           // 0x04

    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148f00, object 0x0215fcc0 */
struct __cmd_menu_present_exp : ScriptCommand {
    _check_shop_end m_end;           // 0x04

    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148ee8, object 0x0215fcc8 */
struct __cmd_menu_colosseum : ScriptCommand {
    _check_shop_end m_end;           // 0x04

    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148ea0, object 0x0215fcd0 */
struct __cmd_menu_hostage : ScriptCommand {
    _check_shop_end m_end;           // 0x04

    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148e88, object 0x0215fcd8 */
struct __cmd_menu_nene : ScriptCommand {
    _check_shop_end m_end;           // 0x04

    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148e58, object 0x0215fc50 */
struct __cmd_player_line_move : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual int isEnd();
};

/* vtable 0x02148e40, object 0x0215fc14 */
struct __cmd_player_line_move2 : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual int isEnd();
};

/* vtable 0x02148de0, object 0x0215fc10 */
struct __cmd_ikada_move2_player_get_on : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual int isEnd();
};

/* vtable 0x02148dc8, object 0x0215fbac */
struct __cmd_ikada_move_player_get_on : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual int isEnd();
};

/* vtable 0x02148d98, object 0x0215fcb0 */
struct __cmd_set_camera_target_chara_frame : ScriptCommand {
    int objNo_;                      // 0x04

    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual int isEnd();
};

/* vtable 0x02148d80, object 0x0215fba8 */
struct __cmd_set_camera_angle_abs : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual int isEnd();
};

/* vtable 0x02148d38, object 0x0215fb98 */
struct __cmd_door_action : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual int isEnd();
};

/* vtable 0x02148d20, object 0x0215fce8 */
struct __cmd_make_surechigai_taishi : ScriptCommand {
    _check_shop_end m_end;           // 0x04

    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual int isEnd();
};

/* vtable 0x02148cf0, object 0x02160130 */
struct __cmd_surechigai_save : ScriptCommand {
    _check_shop_end m_end;           // 0x04
    int type_;                       // 0x08
    int index_;                      // 0x0C

    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual int isEnd();
    void setFlag(bool flag);
};

/* vtable 0x02148cd8, object 0x0216018c */
struct __cmd_surechigai_root : ScriptCommand {
    _check_shop_end m_end;           // 0x04
    int type1_;                      // 0x08
    int index1_;                     // 0x0C
    int type2_;                      // 0x10
    int index2_;                     // 0x14

    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual int isEnd();
    void setFlag(bool flag, bool first);
};

/* vtable 0x02148c60, object 0x0215fca8 */
struct __cmd_surechigai_mapname : ScriptCommand {
    _check_shop_end m_end;           // 0x04

    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual int isEnd();
};

/* vtable 0x02148c48, object 0x0215fc98 */
struct __cmd_surechigai_message : ScriptCommand {
    _check_shop_end m_end;           // 0x04

    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual int isEnd();
};

/* vtable 0x02148c18, object 0x0215fc80 */
struct __cmd_set_wait_enable_lock : ScriptCommand {
    int unk_04;                      // 0x04

    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual int isEnd();
};

/* vtable 0x02148c00, object 0x0215fc60 */
struct __cmd_chara_move_line_to_player : ScriptCommand {
    int unk_04;                      // 0x04

    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual int isEnd();
};

/* vtable 0x02148bb8, object 0x0215fc2c */
struct __cmd_set_chara_rot : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148ba0, object 0x0215fbdc */
struct __cmd_set_end_roll : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148b70, object 0x0215fc18 */
struct __cmd_party_move_to_first2 : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148b58, object 0x0215fc90 */
struct __cmd_character_rgb_anim2 : ScriptCommand {
    int endFlag_;                    // 0x04

    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02148af8, object 0x0215fc3c */
struct __cmd_the_end : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

extern __cmd_camera_change_distance g_cmd_camera_change_distance;
extern __cmd_player_move2 g_cmd_player_move2;
extern __cmd_character_move2 g_cmd_character_move2;
extern __cmd_character_move2_to g_cmd_character_move2_to;
extern __cmd_door_action g_cmd_door_action;
extern __cmd_camera_move_to_player g_cmd_camera_move_to_player;
extern __cmd_character_move2_party g_cmd_character_move2_party;
extern __cmd_character_move g_cmd_character_move;
extern __cmd_set_camera_angle_abs g_cmd_set_camera_angle_abs;
extern __cmd_ikada_move_player_get_on g_cmd_ikada_move_player_get_on;
extern __cmd_player_move2_to g_cmd_player_move2_to;
extern __cmd_character_action_gaze g_cmd_character_action_gaze;
extern __cmd_character_effect_mark g_cmd_character_effect_mark;
extern __cmd_charcter_3d_motion g_cmd_charcter_3d_motion;
extern __cmd_player_move_jump g_cmd_player_move_jump;
extern __cmd_player_move_to g_cmd_player_move_to;
extern __cmd_player_move g_cmd_player_move;
extern __cmd_map_camera_position g_cmd_map_camera_position;
extern __cmd_character_move_to g_cmd_character_move_to;
extern __cmd_character_normal_jump g_cmd_character_normal_jump;
extern __cmd_character_move2_z g_cmd_character_move2_z;
extern __cmd_set_end_roll g_cmd_set_end_roll;
extern __cmd_party_move2_formation g_cmd_party_move2_formation;
extern __cmd_character_move2_relative g_cmd_character_move2_relative;
extern __cmd_character_move2_player g_cmd_character_move2_player;
extern __cmd_charcter_motion g_cmd_charcter_motion;
extern __cmd_character_move_party g_cmd_character_move_party;
extern __cmd_player_move2_jump g_cmd_player_move2_jump;
extern __cmd_character_wait g_cmd_character_wait;
extern __cmd_camera_move_pov g_cmd_camera_move_pov;
extern __cmd_fadein_character g_cmd_fadein_character;
extern __cmd_fadeout_character g_cmd_fadeout_character;
extern __cmd_character_move2_x g_cmd_character_move2_x;
extern __cmd_character_move_player g_cmd_character_move_player;
extern __cmd_ikada_move2_player_get_on g_cmd_ikada_move2_player_get_on;
extern __cmd_player_line_move2 g_cmd_player_line_move2;
extern __cmd_party_move_to_first2 g_cmd_party_move_to_first2;
extern __cmd_character_move_x g_cmd_character_move_x;
extern __cmd_camera_reset_distance g_cmd_camera_reset_distance;
extern __cmd_map_camera_move g_cmd_map_camera_move;
extern __cmd_set_chara_rot g_cmd_set_chara_rot;
extern __cmd_character_move_relative g_cmd_character_move_relative;
extern __cmd_character_action_jump g_cmd_character_action_jump;
extern __cmd_camera_move_abs g_cmd_camera_move_abs;
extern __cmd_the_end g_cmd_the_end;
extern __cmd_character_action_turn g_cmd_character_action_turn;
extern __cmd_map_camera_gaze g_cmd_map_camera_gaze;
extern __cmd_map_camera_angle g_cmd_map_camera_angle;
extern __cmd_party_move_overlap g_cmd_party_move_overlap;
extern __cmd_player_line_move g_cmd_player_line_move;
extern __cmd_character_move_z g_cmd_character_move_z;
extern __cmd_furniture_move2 g_cmd_furniture_move2;
extern __cmd_chara_move_line_to_player g_cmd_chara_move_line_to_player;
extern __cmd_furniture_move g_cmd_furniture_move;
extern __cmd_effect_wait g_cmd_effect_wait;
extern __cmd_effect_fade g_cmd_effect_fade;
extern __cmd_set_wait_enable_lock g_cmd_set_wait_enable_lock;
extern __cmd_effect_move g_cmd_effect_move;
extern __cmd_character_rgb_anim2 g_cmd_character_rgb_anim2;
extern __cmd_surechigai_message g_cmd_surechigai_message;
extern __cmd_riseup_move g_cmd_riseup_move;
extern __cmd_surechigai_mapname g_cmd_surechigai_mapname;
extern __cmd_set_camera_target_chara_frame g_cmd_set_camera_target_chara_frame;
extern __cmd_menu_extra_shop g_cmd_menu_extra_shop;
extern __cmd_menu_present_exp g_cmd_menu_present_exp;
extern __cmd_menu_colosseum g_cmd_menu_colosseum;
extern __cmd_menu_hostage g_cmd_menu_hostage;
extern __cmd_menu_nene g_cmd_menu_nene;
extern __cmd_set_party_order g_cmd_set_party_order;
extern __cmd_make_surechigai_taishi g_cmd_make_surechigai_taishi;
extern __cmd_player_rot g_cmd_player_rot;
extern __cmd_map_event_camera g_cmd_map_event_camera;
extern __cmd_map_flash g_cmd_map_flash;
extern __cmd_menu_shop g_cmd_menu_shop;
extern __cmd_furniture_open g_cmd_furniture_open;
extern __cmd_player_wait g_cmd_player_wait;
extern __cmd_surechigai_save g_cmd_surechigai_save;
extern __cmd_menu_event_imuru g_cmd_menu_event_imuru;
extern __cmd_map_texture_scale g_cmd_map_texture_scale;
extern __cmd_map_set_back_color g_cmd_map_set_back_color;
extern __cmd_map_restore_back_color g_cmd_map_restore_back_color;
extern __cmd_surechigai_root g_cmd_surechigai_root;
extern __cmd_map_blend_color g_cmd_map_blend_color;
extern __cmd_map_blend_init g_cmd_map_blend_init;
