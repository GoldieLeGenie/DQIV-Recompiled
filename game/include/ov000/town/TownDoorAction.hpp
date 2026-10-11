#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/cmn/ActionBase.hpp"


enum KEY_TYPE {
    KEY_NONE = 0,
    KEY_TOUZOKU = 1,
    KEY_MAHOU = 2,
    KEY_SAIGO = 3
};

/* vtable 0x02147d54 */
struct TownDoorAction : cmn::ActionBase {
    enum DOOR_TYPE {
        DOOR_S = 0,
        DOOR_T = 1,
        DOOR_W = 2
    };
    enum DOOR_OPEN_JUDGE {
        JUDGE_KEY = 0,
        JUDGE_EVENT_OPEN = 1,
        JUDGE_EVENT_CLOSE = 2
    };
    enum DOOR_OPEN_TYPE {
        DOOR_OPEN_KEY = 1,
        DOOR_OPEN_EVENT = 2,
        DOOR_NOT_OPEN_KEY = 3,
        DOOR_NOT_OPEN_EVENT = 4,
        DOOR_LOCK = 5,
        DOOR_CRACK_ORIN = 7,
        DOOR_NOT_OPEN_MAP_ID = 8
    };
    struct EVENTDOOR {
        int uid;                                // 0x00
        DOOR_OPEN_TYPE type;                    // 0x04
    };
    enum {
        DOOR_OPEN = 0,
        DOOR_CLOSE = 1
    };

    DOOR_TYPE doorType_;                        // 0x04
    DOOR_OPEN_JUDGE judgeType_;                 // 0x08
    DOOR_OPEN_TYPE openType_;                   // 0x0C
    KEY_TYPE haveKey_;                          // 0x10
    KEY_TYPE doorKeyType_;                      // 0x14
    int eventOpen_;                             // 0x18
    int message_;                               // 0x1C
    int crackOrin_;                             // 0x20
    int wDoor1_ObjNo_;                          // 0x24
    int wDoor2_ObjNo_;                          // 0x28
    int sDoor_ObjNo_;                           // 0x2C
    int tDoor_ObjNo_;                           // 0x30
    int prev_w1_objNo_;                         // 0x34
    int prev_w2_objNo_;                         // 0x38
    int prev_s_objNo_;                          // 0x3C
    int prev_tDoorNo_;                          // 0x40
    int nextAction_;                            // 0x44
    int counter_;                               // 0x48
    int backupObj_;                             // 0x4C
    EVENTDOOR eventDoor_[15];                   // 0x50
    int eventDoorCount_;                        // 0xC8
    dss::Fix32Vector3 prevPos_;                 // 0xCC
    int scriptDoor1Uid_;                        // 0xD8
    int scriptDoor2Uid_;                        // 0xDC
    int scriptType_;                            // 0xE0

    virtual int setup();
    virtual void execute();
    virtual int update();
    int startCheck();
    static TownDoorAction* getSingleton();
    bool checkSurface();
    bool checkObject();
    bool checkOpen(int objNo, int commonId);
    bool isDoorObject(int commonId);
    void setDoorT(int objNo);
    void setDoorS(int objNo);
    void setDoorW(int objNo);
    int getOpenType(int objNo);
    bool checkOpenMessage(int objNo);
    void setEventDoor(int uid, DOOR_OPEN_TYPE type);
    void scriptOpen(int uid1, int uid2, int type);
    bool scriptEnd();
    void setDoorFlag(int uid, int type, bool flag);
};
