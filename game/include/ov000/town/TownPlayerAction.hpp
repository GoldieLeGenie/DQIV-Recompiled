#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/cmn/PlayerAction.hpp"
#include "main/cmn/ActionBase.hpp"

enum TOWN_PLAYER_ACTION_TYPE {
    ACTION_TYPE_WALK = 0,
    ACTION_TYPE_ROPE = 1,
    ACTION_TYPE_SUBE = 2,
    ACTION_TYPE_FALL = 3,
    ACTION_TYPE_BUMP = 4,
    ACTION_TYPE_KAIDAN = 5,
    ACTION_TYPE_FRAME_MOVE = 6,
    ACTION_TYPE_FRAME_ROT = 7,
    ACTION_TYPE_SHIP = 8,
    ACTION_TYPE_IKADA = 9,
    ACTION_TYPE_DOOR = 10,
    ACTION_TYPE_RURA = 11,
    ACTION_TYPE_RURA_FAILED = 12,
    ACTION_TYPE_SORATOBU = 13,
    ACTION_TYPE_HENGE = 14,
    ACTION_TYPE_BALLON_HORN = 15
};

/* vtable 0x02147d78 */
struct TownFallAction : cmn::ActionBase {
    enum {
        FALL_START = 0,
        FALL_COLL = 1
    };

    int sePlay_;                                // 0x04
    int count_;                                 // 0x08
    int fallType_;                              // 0x0C
    int moveMode_;                              // 0x10
    int partyMove_;                             // 0x14
    dss::Fix32Vector3 cameraPos_;               // 0x18
    dss::Fix32Vector3 vecXZ_;                   // 0x24

    static const dss::Fix32 fallStartFix;
    static const dss::Fix32 fallSpeed;

    virtual int setup();
    virtual void execute();
    virtual int update();
    int startCheck();
    static TownFallAction* getSingleton();
    void setCollFall();
    void setFixXZ();
    void exitFall();
};

/* vtable 0x02148798 */
struct TownPlayerAction : cmn::PlayerAction {
    TOWN_PLAYER_ACTION_TYPE actionType_;        // 0x0C
    cmn::ActionBase* action_[16];               // 0x10
    int allShadowReset_;                        // 0x50
    char charaShadowStay_[8];                   // 0x54

    static const dss::Fix32 collR;
    static const dss::Fix32 collRR;
    static const dss::Fix32 coll2RR;
    static const dss::Fix32 walkSpeed;
    static const dss::Fix32 surfaceR;
    static const dss::Fix32 rageSurfaceR;
    static const dss::Fix32 objectR;
    static const dss::Fix32 fixR;
    static const dss::Fix32 ropeSpeed;
    static const dss::Fix32 fallH;
    static const dss::Fix32 bumpH;
    static const dss::Fix32 kaidanR;
    static const dss::Fix32 shipR;
    static const dss::Fix32 shipCollR;
    static const dss::Fix32 getDownL;
    static const dss::Fix32 shipSpeed;
    static const dss::Fix32 ikadaR;
    static const dss::Fix32 getOnOffSpeed;
    static const dss::Fix32 shipCtrLen;
    static const dss::Fix32 walkCtrLen;
    static const dss::Fix32 ruraSpeed;
    static const dss::Fix32 talkR;
    static const dss::Fix32 townCharaR;
    static const dss::Fix32 townCharaPreR;
    static const dss::Fix32 changePreR;

    TownPlayerAction();
    ~TownPlayerAction();
    void setup();
    void cleanup();
    void checkAbortPos();
    void execute();
    virtual void unkfunc_0213c9b4();
    virtual void unkfunc_0213c9b0();
};
