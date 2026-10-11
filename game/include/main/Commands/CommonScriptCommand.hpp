#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/Commands/CommonCommand.hpp"

struct PARAM_FADEIN {
    unsigned int frame;          // 0x00
};

struct PARAM_FADEOUT {
    unsigned int frame;          // 0x00
    unsigned int type;           // 0x04
};

struct PARAM_MESSAGE1_SELF_CLOSING {
    unsigned int message;        // 0x00
    unsigned int frame;          // 0x04
};

struct PARAM_SPEAK_TO_PLAYER_SELF_CLOSING {
    unsigned int message;        // 0x00
    unsigned int frame;          // 0x04
};

struct PARAM_MENU_SAVE {
    unsigned int type;           // 0x00
};

struct PARAM_PLAYER_EFFECT_MARK {
    unsigned int mark;           // 0x00
};

struct PARAM_CHARACTER_PALETTE {
    int r;                       // 0x00
    int g;                       // 0x04
    int b;                       // 0x08
    int endR;                    // 0x0C
    int endG;                    // 0x10
    int endB;                    // 0x14
    unsigned int frame;          // 0x18
};

struct PARAM_MUSIC_VOLUME {
    int volume;                  // 0x00
    unsigned int frame;          // 0x04
};

struct PARAM_PLAY_MUSIC {
    unsigned int musicNo;        // 0x00
    unsigned int frame;          // 0x04
    unsigned int flag;           // 0x08
};

struct PARAM_EVENT_CHAPTER_TITLE {
    unsigned int chapter;        // 0x00
    unsigned int flag;           // 0x04
};

/* vtable 0x020be22c, object 0x020ecf58 */
struct __cmd_fade_in : ScriptCommand {
    short count_;                // 0x04
    short countFrame_;           // 0x06

    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x020be184, object 0x020ecf68 */
struct __cmd_fade_out : ScriptCommand {
    short count_;                // 0x04
    short countFrame_;           // 0x06

    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x020be1fc, object 0x020ecf60 */
struct __cmd_fade_in2 : ScriptCommand {
    short count_;                // 0x04
    short countFrame_;           // 0x06

    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x020be1cc, object 0x020ecf50 */
struct __cmd_fade_out2 : ScriptCommand {
    short count_;                // 0x04
    short countFrame_;           // 0x06

    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x020be16c, object 0x020ecfe8 */
struct __cmd_message1_self_closing : ScriptCommand {
    unsigned int count_;         // 0x04
    unsigned int countFrame_;    // 0x08

    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x020be124, object 0x020ed000 */
struct __cmd_speak_to_player_self_closing : ScriptCommand {
    unsigned int countFrame_;    // 0x04
    unsigned int count_;         // 0x08

    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual int isEnd();
};

/* vtable 0x020be214, object 0x020ed024 */
struct __cmd_menu_save : ScriptCommand {
    int type_;                   // 0x04
    int chapter_;                // 0x08
    int waitFlag_;               // 0x0C

    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual int isEnd();
};

/* vtable 0x020be1e4, object 0x020ecf4c */
struct __cmd_player_effect_mark : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual int isEnd();
};

/* vtable 0x020be1b4, object 0x020ed034 */
struct __cmd_music_volume : ScriptCommand {
    int m_end_vol;               // 0x04
    dss::Fix32 m_add;            // 0x08
    int m_frame;                 // 0x0C
    int m_counter;               // 0x10
    int m_start_vol;             // 0x14

    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual int isEnd();
};

/* vtable 0x020be19c, object 0x020ecf44 */
struct __cmd_character_palette : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual int isEnd();
};

/* vtable 0x020be154, object 0x020ed04c */
struct __cmd_play_music : ScriptCommand {
    static const int STOP_WAIT_COUNT = 30;

    int musicNo_;                // 0x04
    int playTime_;               // 0x08
    int soundCount_;             // 0x0C
    int preMusicNo_;             // 0x10
    int playEnd_;                // 0x14
    int flag_;                   // 0x18

    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual int isEnd();
};

/* vtable 0x020be13c, object 0x020ecf48 */
struct __cmd_key_wait_type_b : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute();
    virtual int isEnd();
};

/* vtable 0x020be10c, object 0x020ecf40 */
struct __cmd_event_chapter_title : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

extern __cmd_event_chapter_title g_cmd_event_chapter_title;
extern __cmd_character_palette g_cmd_character_palette;
extern __cmd_key_wait_type_b g_cmd_key_wait_type_b;
extern __cmd_player_effect_mark g_cmd_player_effect_mark;
extern __cmd_fade_out2 g_cmd_fade_out2;
extern __cmd_fade_in g_cmd_fade_in;
extern __cmd_fade_in2 g_cmd_fade_in2;
extern __cmd_fade_out g_cmd_fade_out;
extern __cmd_message1_self_closing g_cmd_message1_self_closing;
extern __cmd_speak_to_player_self_closing g_cmd_speak_to_player_self_closing;
extern __cmd_menu_save g_cmd_menu_save;
extern __cmd_music_volume g_cmd_music_volume;
extern __cmd_play_music g_cmd_play_music;
