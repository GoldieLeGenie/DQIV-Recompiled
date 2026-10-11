#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/global/StageLink.hpp"
#include "main/cmn/PlayerManager.hpp"
#include "main/cmn/MoveBase.hpp"
#include "main/object/DisplayCharacter.hpp"
#include "ov000/town/TownPlayerAction.hpp"
#include "ov000/town/TownPartyDraw.hpp"
#include "ov000/town/TownPartyAction.hpp"

enum CAMERA_ROT_TYPE {
    ROT_NONE = 0,
    ROT_TO_R = 1,
    ROT_TO_L = 2,
    ROT_TO_NORTH = 3,
    ROT_TO_NORTH_END = 4,
    ROT_TO_NORTH_FREE = 5
};

enum TEXTURE_ANIM {
    ANIM_NONE = 0,
    MANYA_DANCE1 = 1,
    MANYA_DANCE2 = 2,
    ANIM_END = 3
};

enum EXIT_LOCK_TYPE {
    EXIT_LOCK_DOOR = 1,
    EXIT_LOCK_KAIDAN = 2,
    EXIT_LOCK_TABI = 4,
    EXIT_LOCK_RURA = 8
};

enum RESET_EXIT_LOCK_TYPE {
    RESET_EXIT_LOCK_DOOR = 1,
    RESET_EXIT_LOCK_KAIDAN = 2,
    RESET_EXIT_LOCK_TABI = 4,
    RESET_EXIT_LOCK_RURA = 8,
    RESET_EXIT_ALL = -1
};

struct TownFurnitureManager;

extern "C" {


}



/* vtable 0x02148348 */
struct TownPlayerManager : cmn::PlayerManager {
    virtual void setPosition(dss::Fix32Vector3& pos);
    virtual dss::Fix32Vector3 getPosition();
    virtual short getDirection();
    virtual void resetParty();

    TownPartyDraw partyDraw_;                   // 0x000C
    TownPlayerAction player_;                   // 0x12F0
    TownPartyAction party_;                     // 0x134C
    int flagIdoLink_;                           // 0x1698
    int encountLock_;                           // 0x169C
    int flagEncount_;                           // 0x16A0
    short countEncount_;                        // 0x16A4
    short countMaxEncount_;                     // 0x16A6
    dss::Fix32Vector3 posEncount_;              // 0x16A8
    dss::Vector3<short> angleEncount_;          // 0x16B4
    int exitLock_;                              // 0x16BC
    int remoteFlag_;                            // 0x16C0
    int rotLock_;                               // 0x16C4
    int scriptType_;                            // 0x16C8
    cmn::MoveBase scriptMove_;                  // 0x16CC
    int searchMapUid_;                          // 0x1720
    char frmDir_;                               // 0x1724
    char charDir_;                              // 0x1725
    short frmDirIdx_;                           // 0x1726
    int idoMess_;                               // 0x1728
    int tabiLink_;                              // 0x172C
    CAMERA_ROT_TYPE cameraRot_;                 // 0x1730
    CAMERA_ROT_TYPE prev_cameraRot_;            // 0x1734
    int allShadowReset_;                        // 0x1738
    int shadowSet_;                             // 0x173C
    TEXTURE_ANIM txAction_;                     // 0x1740
    int mapChangeSE_;                           // 0x1744
    int txCounter_;                             // 0x1748
    int scriptRotFlag_;                         // 0x174C
    int scriptColl_;                            // 0x1750
    int riseupIndex_;                           // 0x1754
    int mapChangeCounter_;                      // 0x1758
    int nextEncount_;                           // 0x175C
    int encountTile_;                           // 0x1760
    int searchAction_;                          // 0x1764
    int wait_;                                  // 0x1768
    int church_;                                // 0x176C
    unsigned int walkCounter_;                  // 0x1770
    int menuSearch_;                            // 0x1774
    int defaultClip_;                           // 0x1778
    int mapFKLock_;                             // 0x177C
    int effectPosFlag_;                         // 0x1780
    int eventEncount_;                          // 0x1784
    int battleLose_;                            // 0x1788
    int notIntoTenkujou_;                       // 0x178C
    dss::Fix32Vector3 effectPos_;               // 0x1790

    TownPlayerManager();
    static TownPlayerManager* getSingleton();
    void initialize();
    void terminate();
    void execute();
    void draw();
    void setup();
    void cleanup();
    void unkfunc_02133764();
    void normalExec();
    void mormalMapLink();
    void execMapLink();
    void inputPad(int padDir);
    void inputClear();
    void lockMapLink(EXIT_LOCK_TYPE type);
    void resetMapLink(RESET_EXIT_LOCK_TYPE type);
    void setPartyToFirst(dss::Fix32Vector3& pos);
    void setDirection(short dirIdx);
    void unkfunc_02133f60();
    void unkfunc_02134058();
    void setFormation(int frmDir, int charaDir, dss::Fix32 speed);
    void setRemote(int flag);
    bool isLock();
    void setLock(int lock);
    void setCureFloor();
    void setCameraRotToNorth();
    void setCameraRot();
    void setSimpleMove(dss::Fix32Vector3& prev, dss::Fix32Vector3& next, int frame);
    void setSpeedMove(dss::Fix32Vector3& prev, dss::Fix32Vector3& next, dss::Fix32 speed);
    void setJumpMove(dss::Fix32Vector3& endPos, int frame);
    bool isFinish();
    void setCameraRotType(CAMERA_ROT_TYPE type);
    bool isEncountLock();
    void setEncountLock(int flag);
    bool isIdoLinkPos();
    int getDamageColor(int type);
    void setShadow();
    void checkMenuAction();
    void scriptExecute();
    void setManyaDance(int flag);
    void textureAnimExecute();
    void setScriptRot(int frame, short idx, int flag);
    void setStartEraseParty();
    void rizeupSet(int icon);
    bool rizeupEnd();
    int getInpasMapObj();
    bool checkTalkToCharacter();
    bool getPlayerCopyInfo(int playerNo, dss::Fix32Vector3& pos, short& idx, int& charNo);
    bool setupDelPartyNotMoveFirst(int playerNo);
    void setIkadaSpeedMove(dss::Fix32Vector3& prev, dss::Fix32Vector3& next, dss::Fix32 speed);
    void setIkadaFrameMove(dss::Fix32Vector3& prev, dss::Fix32Vector3& next, int frame);
    void setEncount(int tile);
    void setMessage();
    bool isSearch();
    void demolitionChurch();
    void loadChurch();
    void rizeupSetParty(int charaNo, int markNo);
    void setVanAndBasha();
    void setLockByEventEncount(int flag);
    void resetLockByEventEncount();
    bool isSaveAndBattleOK();

    void setLockRot(int lock) { rotLock_ = lock; }
    void setPlayerSleep(int sleep) { partyDraw_.setSleep(sleep); }
    void setEffectPos(dss::Fix32Vector3& pos) { effectPos_ = pos; }
};

