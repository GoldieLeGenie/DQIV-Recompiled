#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/cmn/MoveBase.hpp"
#include "main/dss/Render.hpp"
#include "main/data/DataObject.hpp"
#include "main/object/DisplayCharacter.hpp"
#include "main/object/GameMonster.hpp"
#include "main/object/ModelObject.hpp"
#include "main/cmn/TalkSoundManager.hpp"


struct TOWN_CHARACTER {
    int enable;                                 // 0x00 
    int index;                                  // 0x04
    int charaIndex;                             // 0x08
    int dir;                                    // 0x0C
    int ctrlNo;                                 // 0x10
    dss::Fix32Vector3 position;                 // 0x14
    dss::Vector2<int> position2d[2];            // 0x20
    dss::Flag flag;                             // 0x30

    TOWN_CHARACTER() { flag.flag_ = 0; }
};

struct TOWN_SCRIPT_DATA {
    dss::Fix32Vector3 node[4];                  // 0x00
    int num[4];                                 // 0x30
    int frame;                                  // 0x40
    int counter;                                // 0x44
    int isEnd;                                  // 0x48
};

enum SCRIPT_MOVE_TYPE {
    MOVE_TYPE_NONE = 0,
    MOVE_TYPE_AREA = 1,
    MOVE_TYPE_ROOT = 2,
    MOVE_TYPE_PURSUE = 3,
    MOVE_TYPE_TO_PARTY = 4,
    MOVE_TYPE_SIMPLE_MOVE = 5,
    MOVE_TYPE_JUMP = 6,
    MOVE_TYPE_PASSIVE = 7,
    MOVE_TYPE_RANDOM = 8,
    MOVE_TYPE_REVESE = 9,
    MOVE_TYPE_BIG_ROCK = 10,
    MOVE_TYPE_WAIT = 11
};

struct SCRIPT_MOVE_DATA {
    dss::Fix32Vector3 vector[4];                // 0x00
    dss::Fix32 speed;                           // 0x30
    int frame;                                  // 0x34
    int counter;                                // 0x38
};

struct TownCharacterBase
{
    enum {
        CHANGE_NONE = 0,
        CHANGE_LINEARLY = 1,
        CHANGE_FADE_IN1 = 2,
        CHANGE_FADE_IN2 = 3,
        CHANGE_FADE_OUT1 = 4,
        CHANGE_FADE_OUT2 = 5
    };
    enum {
        TOWN_CHARACTER_NORMAL = 0,
        TOWN_CHARACTER_SLEEP = 1,
        TOWN_CHARACTER_MONSTER = 2,
        TOWN_CHARACTER_MODEL = 3,
        TOWN_CHARACTER_FURNITURE = 4
    };
    enum {
        RGB_CHANGE1 = 0,
        RGB_CHANGE2 = 1
    };

    int collFlag_;                              // 0x004
    TOWN_CHARACTER data_;                       // 0x008
    TOWN_SCRIPT_DATA script_;                   // 0x03C
    int type_;                                  // 0x088
    SCRIPT_MOVE_TYPE moveType_;                 // 0x08C
    SCRIPT_MOVE_DATA moveData_;                 // 0x090
    Render* render_;                            // 0x0CC
    dss::Camera* unk_d0;                        // 0x0D0 
    cmn::MoveBase simpleMove_;                  // 0x0D4
    dss::Fix32Vector3 addRGB;                   // 0x128
    dss::Fix32Vector3 setRGB;                   // 0x134
    int waitCounter_;                           // 0x140
    int testFrame;                              // 0x144
    cmn::TalkSoundManager::MESSAGESOUND voice_; // 0x148
    short swingIdx_;                            // 0x14C
    short blinkCounter_;                        // 0x14E
    short alphaCounter_;                        // 0x150
    short alphaFrame_;                          // 0x152
    short changeAlphaType_;                     // 0x154
    short rgbFrame_;                            // 0x156
    short animCounter_;                         // 0x158
    short moveIdx_;                             // 0x15A
    short rgbFrameMax_;                         // 0x15C
    unsigned char alphaAdd_;                    // 0x15E
    unsigned char remoteAlpha_;                 // 0x15F
    signed char mapNo_;                         // 0x160
    signed char stageColl_;                     // 0x161
    signed char rgbChangeType_;                 // 0x162
    signed char m_talk_enable;                  // 0x163

    static int areaCheck_;                      // 
    static int allEventLock_;                   // 
    static int monsterTalk_;                    // 
    static void unkfunc_resetStatics();         // 

    TownCharacterBase();
    ~TownCharacterBase();
    virtual void setup(TOWN_CHARACTER& data);
    virtual void cleanup() = 0;
    virtual void execute();
    virtual void draw() = 0;
    virtual void setDir(int dir);
    virtual int getDir();
    virtual void setRotation(dss::Vector3<short>& rot);
    virtual void setShadow(int flag);
    virtual void setAnimation(int flag);
    virtual void setNearCharacter(int flag);
    virtual void setDisplay(int flag);
    virtual int isDisplay();
    virtual void setSleepCharacter(int flag);
    virtual void setWriggleCharacter(int flag);
    virtual void setAlpha(unsigned char alpha);
    virtual void changePose(int pose);
    virtual void restorePose();
    virtual void requestReload();
    virtual void setPaletteRate(unsigned char r, unsigned char g, unsigned char b, dss::Fix32 rate);
    virtual void setPaletteRate(dss::Fix32 r, dss::Fix32 g, dss::Fix32 b);
    virtual void setChangePaletteRate(dss::Fix32Vector3& rgb, int frame);
    virtual void setChangePaletteRate(unsigned char r, unsigned char g, unsigned char b, int frame);
    virtual bool isEndPalletRate();
    virtual void setPalletRate(dss::Fix32 rate);
    virtual void setPosition(dss::Fix32Vector3& pos);
    virtual void execPursueMove();
    virtual void execAreaMove();
    virtual void execRootMove();
    virtual void setMotion(int motion, int loop);
    virtual bool isMotion();
    virtual void execMovePassive();
    virtual void setMoveBigRock();
    virtual void execMoveBigRock();
    virtual void setMapUid(int uid);

    void setScriptData(TOWN_SCRIPT_DATA& script);
    int isScriptEnd();
    void execWait();
    void execMove();
    void execRiseup();
    void execVanish();
    void execTremble();
    void setCollFlag(int flag);
    int getCollFlag();
    void changeRGB();
    void changeAlpha();
    void checkMoveColl(dss::Fix32Vector3& pos, dss::Fix32Vector3& next, dss::Fix32& a, dss::Fix32& b);
    void setMoveToParty();
    void setNextMoveToParty();
    void setSimpleMove();
    void setSimpleRot(short rot, int frame, int type);
    void setJumpMove(dss::Fix32Vector3 endPos, int frame);
    void jumpMove();
    void setLockRot(int flag);
    void setPosing(int pose);
    void setMovePassive();
    void setMoveReverse();
    void execMoveReverse();
    void setMoveRandom();
    void execMoveRandom();
    bool checkPlayerColl(dss::Fix32Vector3& pos);
    void setSwingRoundIdx();
    void setFadeType(int type, int frame);
    bool isEndFade();
    void setPersonalEventLock(int flag);
    void setMonsterSpeak(int flag);
    bool checkMonsterSpeak();
    void resetTalk();
    bool getSpeak();
    void setSpeak(int flag);
    bool getTalked();
    void setTalked(int flag);
    void setCounterTalk(int flag);
    bool getCounterTalk();
    bool isRotEnd();
    void setSwingRound(int flag);
    void setEnableLockWait(int count);
    void execMoveWait();
    bool isMoveWaitEnd();
    void setMotionLock(int flag);
    bool isMotionLock();
    void setRotFrame(int frame, short idx, int flag, int typeA);
    bool isRotFrameEnd();
    void setSurechigaiMapNo(int no);
    int getSurechigaiMapNo();
    void setVoice(int voice);
    bool checkVoice();
    cmn::TalkSoundManager::MESSAGESOUND getVoice();
    void setPosition2d(int idx, dss::Vector2<int> pos) { data_.position2d[idx] = pos; }
};

struct TownCharacterFuniture : TownCharacterBase
{
    int mapUid_;                                // 0x164
    dss::Fix32 collR_;                          // 0x168

    TownCharacterFuniture() {}
    ~TownCharacterFuniture() {}
    virtual void setup(TOWN_CHARACTER& data);
    virtual void setDisplay(int flag);
    virtual int isDisplay();
    virtual void execute();
    virtual void setAnimation(int flag);
    virtual void setNearCharacter(int flag);
    virtual void cleanup();
    virtual void setPosition(dss::Fix32Vector3& pos);
    virtual void execMovePassive();
    virtual void setMapUid(int uid);
    virtual void draw();
};

struct TownCharacterDraw : TownCharacterBase
{
    DisplayCharacter character_;                // 0x164
    DataObject dataObject_;                     // 0x3B8

    TownCharacterDraw();
    ~TownCharacterDraw();
    virtual void setup(TOWN_CHARACTER& data);
    virtual void execute();
    virtual void cleanup();
    virtual void setDir(int dir);
    virtual int getDir();
    virtual void draw();
    virtual void setDisplay(int flag);
    virtual int isDisplay();
    virtual void setShadow(int flag);
    virtual void setAnimation(int flag);
    virtual void setNearCharacter(int flag);
    virtual void setWriggleCharacter(int flag);
    virtual void setPosition(dss::Fix32Vector3& pos);
    virtual void setSleepCharacter(int flag);
    virtual void setAlpha(unsigned char alpha);
    virtual void changePose(int pose);
    virtual void restorePose();
    virtual void setPaletteRate(dss::Fix32 r, dss::Fix32 g, dss::Fix32 b);
    virtual bool isEndPalletRate();
    virtual void requestReload();
};

struct TownModelDraw : TownCharacterBase
{
    enum {
        X_MOVE = 0,
        Z_MOVE = 1
    };
    enum {
        ROOT0 = 0,
        ROOT1 = 1,
        ROOT2 = 2,
        ROOT3 = 3,
        ROOT4 = 4,
        ROOT5 = 5,
        ROOT_END1 = 6,
        ROOT_END2 = 7,
        FALL1 = 8,
        FALL2 = 9,
        END_FALL = 10
    };

    ModelObject model_;                         // 0x164
    short dirIdx_;                              // 0xD98
    int loopSe_;                                // 0xD9C
    dss::Vector3<short> modelIdx3d_;            // 0xDA0
    int display_;                               // 0xDA8
    int defaultIndex_;                          // 0xDAC
    dss::Fix32 basePalletRate_;                 // 0xDB0

    TownModelDraw();
    ~TownModelDraw();
    virtual void setup(TOWN_CHARACTER& data);
    virtual void cleanup();
    virtual void execute();
    virtual void draw();
    virtual void setDisplay(int flag);
    virtual int isDisplay();
    virtual void changePose(int pose);
    virtual void restorePose();
    virtual void requestReload();
    virtual void setAnimation(int flag);
    virtual void setPosition(dss::Fix32Vector3& pos);
    virtual void setRotation(dss::Vector3<short>& rot);
    virtual void setPaletteRate(dss::Fix32 r, dss::Fix32 g, dss::Fix32 b);
    virtual void setPaletteRate(unsigned char r, unsigned char g, unsigned char b, dss::Fix32 rate);
    virtual void setMotion(int motion, int loop);
    virtual bool isMotion();
    virtual void setDir(int dir);
    virtual void setMoveBigRock();
    virtual void execMoveBigRock();
    virtual void setPalletRate(dss::Fix32 rate);
    virtual int getDir();

    void unkfunc_0212f19c(int index);           //  loads data/chr/<name>.nsbmd + animations
    dss::Fix32Vector3 getPosition();
    void setRoot(int root, dss::Fix32Vector3& target, dss::Fix32Vector3& pos);
};

struct TownMonsterDraw : TownCharacterBase
{
    GameMonster monster_;                       // 0x164
    int display_;                               // 0xE9C
    int defaultIndex_;                          // 0xEA0

    TownMonsterDraw();
    ~TownMonsterDraw();
    virtual void setup(TOWN_CHARACTER& data);
    virtual void cleanup();
    virtual void execute();
    virtual void draw();
    virtual void setDisplay(int flag);
    virtual int isDisplay();
    virtual void setNearCharacter(int flag);
    virtual void setAnimation(int flag);
    virtual void setPosition(dss::Fix32Vector3& pos);
    virtual void setPaletteRate(dss::Fix32 r, dss::Fix32 g, dss::Fix32 b);
    virtual void setMotion(int motion, int loop);
    virtual bool isMotion();
    virtual void setDir(int dir);
    virtual int getDir();
    virtual void changePose(int pose);
    virtual void restorePose();
    virtual void requestReload();
    virtual void setAlpha(unsigned char alpha);

    static void unkfunc_0212ed58(dss::Camera* camera);   
};
