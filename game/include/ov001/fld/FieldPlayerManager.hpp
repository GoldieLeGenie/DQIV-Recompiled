#pragma once
#include "globaldefs.h"
#include "GameInfo.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/cmn/PlayerManager.hpp"
#include "main/cmn/PlayerAction.hpp"
#include "main/cmn/MoveBase.hpp"
#include "main/object/SpriteCharacter.hpp"
#include "ov001/fld/FieldActionCalculate.hpp"
#include "ov001/fld/FieldCarrierDraw.hpp"

enum {
    WMAP_ATTR_NOT = 0,
    WMAP_ATTR_SOUG = 1,
    WMAP_ATTR_SIGE = 2,
    WMAP_ATTR_SUNA = 3,
    WMAP_ATTR_MORI = 4,
    WMAP_ATTR_YAMA1 = 5,
    WMAP_ATTR_YAMA2 = 6,
    WMAP_ATTR_KAIG = 7,
    WMAP_ATTR_UMI = 8,
    WMAP_ATTR_ASAS = 9,
    WMAP_ATTR_DOKU = 10
};

enum {
    DIR_8_UU = 0,
    DIR_8_RU = 1,
    DIR_8_RR = 2,
    DIR_8_RD = 3,
    DIR_8_DD = 4,
    DIR_8_LD = 5,
    DIR_8_LL = 6,
    DIR_8_LU = 7
};

/* vtable 0x021609f8 */
struct FieldPlayer : cmn::PlayerAction {
    enum MOVETYPE {
        MOVE_WALK = 0,
        MOVE_SHIP_GET_ON = 1,
        MOVE_SHIP_GET_OUT = 2,
        MOVE_SHIP = 3,
        MOVE_BALLOON_GET_ON_WALK = 4,
        MOVE_BALLOON_GET_ON = 5,
        MOVE_BALLOON = 6,
        MOVE_BALLOON_GET_OUT = 7,
        MOVE_RURA_UP = 8,
        MOVE_RURA_DOWN = 9,
        MOVE_RURA_END = 10,
        MOVE_SORATOBU = 11,
        MOVE_SORATOBU_END = 12,
        MOVE_WAIT = 13,
        HENGE_START = 14,
        HENGE_RESET = 15,
        BALLON_HORN = 16
    };

    dss::Fix32Vector3* position_;                                                   // 0x0C
    dss::Fix32Vector3 positionN_;                                                   // 0x10
    dss::Fix32Vector3 speed_;                                                       // 0x1C
    short* dirIdx_;                                                                 // 0x28
    int type_;                                                                      // 0x2C
    int backupType_;                                                                // 0x30
    int waitCounter_;                                                               // 0x34
    int flagFixPos_;                                                                // 0x38
    int ballonCounter_;                                                             // 0x3C
    FieldCollInfo fieldCollInfo_;                                                   // 0x40
    int blockType_[9];                                                              // 0x84
    int collSE_;                                                                    // 0xA8
    int move_;                                                                      // 0xAC

    static const dss::Fix32 shipSearchR;
    static const dss::Fix32 balloonSearchR;
    static const dss::Fix32 Speed;
    static const dss::Fix32 balSpeed;
    static const int shadowX_umi = 16;
    static const int shadowY_umi = 10;
    static const int shadowX_kaigan = 17;
    static const int shadowY_kaigan = 11;
    static const int shadowX_normal = 18;
    static const int shadowY_normal = 12;
    static const int shadowX_mori = 19;
    static const int shadowY_mori = 13;
    static const int shadowX_yama = 20;
    static const int shadowY_yama = 14;
    static const int shadowX_iwa = 21;
    static const int shadowY_iwa = 15;

    virtual void unkfunc_0213c9b4();
    virtual void unkfunc_0213c9b0();

    FieldPlayer();
    ~FieldPlayer();
    void setup();
    void cleanup();
    void execute();
    void balloonExec();
    void balloonMove();
    void getBalloonCollPoint(int index, dss::Fix32Vector3& pos);
    void checkGetOffBalloon();
    void shipExec();
    void walkExec();
    bool checkShip();
    void move();
    void walkCollision();
    void coll(int blkX, int blkY, int collLength, int fixLength);
    int getAttr();
    void setPosition(dss::Fix32Vector3& pos);
    const dss::Fix32Vector3& getPosition();
    void setMoveType(int type);
    int getMoveType();
    bool searchObject(dss::Fix32Vector3& searchPos, dss::Fix32 dr);
    bool playerSearch();
    void fldSearch();
    void setWalkColl(int bx, int by);
    void setChipAttr(int bx, int by);
    void setShipColl(int bx, int by);
    bool getShipColl(int blkX, int blkY);
    bool checkGetDownShip(int blkX, int blkY, int dirIdx, dss::Fix32Vector3& fixPos);
    void setBalloonShadow();
    void setPositionPointer(dss::Fix32Vector3* pPos);
    void setDirIdxPointer(short* dirIdx);
    void setWait(int count);
    bool isEnableGetOff(dss::Fix32Vector3& target);
};

struct FieldParty {
    dss::Fix32Vector3* position_;                                                   // 0x00
    short* dirIdx_;                                                                 // 0x04
    short prevDirIdx_;                                                              // 0x08
    dss::Fix32Vector3 bashaLPos_;                                                   // 0x0C
    short bashaLIdx_;                                                               // 0x18
    dss::Fix32Vector3 bashaRPos_;                                                   // 0x1C
    short bashaRIdx_;                                                               // 0x28
    int countPartyArray_;                                                           // 0x2C
    int countRFix_;                                                                 // 0x30
    int countLFix_;                                                                 // 0x34
    int flagMoveToFirst_;                                                           // 0x38
    dss::Fix32Vector3* m_pos_array;                                                 // 0x3C
    short* m_dir_array;                                                             // 0x40
    int flagBashaArray_;                                                            // 0x44
    char unk_48[0x18];                                                              // 0x48

    static const int POSITION_ARRAY_MAX = 160;
    static const int COUNT_MAX = 20;

    FieldParty();
    ~FieldParty();
    void setup();
    void cleanup();
    void execute();
    int isBashaEnable();
    void setBashaArray(int flag);
    void normalArray();
    void bashaArray();
    void getSidePos(int side, dss::Fix32Vector3& nowPos, dss::Fix32Vector3* nextPos, short* retIdx);
    void fixSidePos(int side, int fix);
    void collisionSide(int side, dss::Fix32Vector3* nextPos, int dirIdx);
    bool moveAllPlayerToFirst(int count);
    void setPosition(dss::Fix32Vector3 pos);
    void setDirIdx(int dirIdx);
    dss::Fix32Vector3 getMemberPosition(int index);
    int getMemberDirIdx(int index);
    void setAllPlayerAtFirst();
    void resetBashaCount();
    void savePartyDrawInfo();
    void setPositionArrayPointer(dss::Fix32Vector3* pos);
    void setDirIdxArrayPointer(short* dirIdx);
    void setPositionPointer(dss::Fix32Vector3* pos);
    void setDirIdxPointer(short* dirIdx);
};

struct FieldPartyDraw {
    SpriteCharacter partyCharacter_[8];                                             // 0x000
    int count_;                                                                     // 0x6E0
    int countReal_;                                                                 // 0x6E4
    int backupCount_;                                                               // 0x6E8
    int valueY_[8];                                                                 // 0x6EC

    static const unsigned short colorDoku;
    static const unsigned short colorBarrier;

    unsigned short colorDokuParty() { return colorDoku; }
    unsigned short colorBarrierParty() { return colorBarrier; }

    FieldPartyDraw();
    ~FieldPartyDraw();
    void setup();
    void cleanup();
    void draw();
    void setPosition(int index, dss::Vector2<int> pos);
    void setDepth(int index, int depth);
    void setRotate(int index, int rot);
    void setDrawNone();
    void resetDrawCount();
    void setHengeDrawNone();
};

/* vtable 0x02160a4c */
struct FieldPlayerManager : cmn::PlayerManager {
    enum {
        WHITE_IN = 0,
        WHITE_OUT = 1
    };

    cmn::MoveBase scriptMove_;                                                      // 0x00C
    int scriptMoveFlag_;                                                            // 0x060
    FieldPlayer player_;                                                            // 0x064
    FieldParty party_;                                                              // 0x114
    FieldPartyDraw partyDraw_;                                                      // 0x174
    FieldShipDraw shipDraw_;                                                        // 0x880
    FieldBalloonDraw balloonDraw_;                                                  // 0xA50
    dss::Fix32Vector3 targetPos_;                                                   // 0xB48
    dss::Fix32 speedToTarget_;                                                      // 0xB54
    dss::Fix32Vector3 position_;                                                    // 0xB58
    dss::Fix32Vector3 drawRuraOffset_;                                              // 0xB64
    dss::Fix32Vector3 ruraPos_;                                                     // 0xB70
    short dirIdx_;                                                                  // 0xB7C
    int hengeCounter_;                                                              // 0xB80
    int mapChangeCounter_;                                                          // 0xB84
    short ballonCounter_;                                                           // 0xB88
    char ballonType_;                                                               // 0xB8A
    int prevAction_;                                                                // 0xB8C
    int eventEncount_;                                                              // 0xB90

    virtual void setPosition(dss::Fix32Vector3& pos);
    virtual dss::Fix32Vector3 getPosition();                                        // only copy is in main (0x02029094)
    virtual short getDirection() { return dirIdx_; }
    virtual void resetParty();

    FieldPlayerManager();
    static FieldPlayerManager* getSingleton();
    void initialize();
    void terminate();
    void execute();
    void draw();
    void walkExec();
    void getOnShip();
    void getDownShip();
    void ruraDownExec();
    void shipExec();
    void setup();
    void cleanup();
    void everyExec();
    void normalExec();
    void execMapLink();
    void inputPad(int padDir);
    void inputClear();
    LandType getLandType();
    FieldCarrirerDraw* getCarrierPos(int type);
    void rideBalloon();
    void downBalloon();
    void balloonExec();
    void rideBalloonWalk();
    bool moveToTarget();
    void savePartyDrawInfo();
    void setStartRura();
    void ruraUpExec();
    int getDamageColor(int type);
    bool isEncountLock();
    void setScriptBalloon(int flag);
    void setSimpleMove(dss::Fix32Vector3 target, dss::Fix32 rate, int flag);
    void setDirectionMove(dss::Fix32 target, int dir);
    void setScriptGetDownShip(int dir);
    bool isEndScriptGetDownShip();
    void hengeStartExec();
    void hengeResetExec();
    dss::Fix32Vector3 getDrawPosition();
    bool checkBarronArea(dss::Fix32Vector3& pos);
    void ballonWhistle();
    void setBallonWhistle();
    void setLockByEventEncount(int flag);
    void resetLockByEventEncount();
    void setCarrierDepth();

    int getMoveType() { return player_.getMoveType(); }
    void setBalloonPos(dss::Fix32Vector3& pos) { balloonDraw_.setPosition(pos); }
    void setShipPos(dss::Fix32Vector3& pos) { shipDraw_.setPosition(pos); }
    void setSpeed(dss::Fix32 speed) { speedToTarget_ = speed; }

    void setPartyToFirst()
    {
        party_.setDirIdx(dirIdx_);
        party_.setPosition(position_);
        party_.setAllPlayerAtFirst();
    }
};
