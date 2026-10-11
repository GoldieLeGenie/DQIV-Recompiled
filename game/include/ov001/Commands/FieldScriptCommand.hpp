#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/Commands/CommonCommand.hpp"
#include "ov001/fld/FieldPlayerManager.hpp"

struct PARAM_FIELD_PLAYER_SET_BALLON {
    unsigned int flag;               // 0x00
};

struct PARAM_FIELD_PLAYER_MOVE_TO {
    int endX;                        // 0x00
    int endY;                        // 0x04
    int endZ;                        // 0x08
    int rate;                        // 0x0C
    unsigned int absFlag;            // 0x10
};

struct PARAM_FIELD_MOVE_LINE {
    unsigned int line;               // 0x00
    int target;                      // 0x04
};

struct PARAM_FIELD_GET_DOWN_SHIP {
    unsigned int direction;          // 0x00
};

/* vtable 0x02160e34, object 0x02164a28 */
struct __cmd_field_player_set_ballon : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02160dec, object 0x02164a2c */
struct __cmd_field_get_down_ship : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02160e1c, object 0x02164a24 */
struct __cmd_field_player_move_to : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

/* vtable 0x02160e04, object 0x02164a20 */
struct __cmd_field_move_line : ScriptCommand {
    virtual void initialize(char* scriptParam);
    virtual void execute() {}
    virtual void terminate() {}
    virtual int isEnd();
};

extern __cmd_field_move_line g_cmd_field_move_line;
extern __cmd_field_player_move_to g_cmd_field_player_move_to;
extern __cmd_field_player_set_ballon g_cmd_field_player_set_ballon;
extern __cmd_field_get_down_ship g_cmd_field_get_down_ship;
