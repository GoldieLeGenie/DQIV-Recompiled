#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"

#include "ov000/town/TownCharacter.hpp"
#include "ov000/town/TownCharacterStorage.hpp"

struct TownCharacterManager {
    static const int TOWN_CHARACTER_MAX = 32;

    int townCharacterCount_;                                // 0x0000
    TownCharacterBase* character_[TOWN_CHARACTER_MAX];      // 0x0004
    TownCharacterStorage characterStorage;                  // 0x0084
    int checkArea_;                                         // 0xDE3C
    int search_;                                            // 0xDE40

    TownCharacterManager();
    ~TownCharacterManager();
    static TownCharacterManager* getSingleton();
    void initialize();
    void terminate();
    void execute();
    int setup(TOWN_CHARACTER& chara);
    void draw();
    void requestCharacterReload();
    void setPosing(int index, int pose);
    int getCharatType(int index);
    void cleanup(int index);
    void setPlayerDirection(int index);
    dss::Fix32Vector3& getPosition(int index);
    void setRotate(int index, int rot);
    void setRotate(int index, dss::Vector3<short>& rot);
    void setTalkedArea(int index, int flag);
    void setTalked(int index, int flag);
    bool isTalked(int index);
    void setShadow(int index, int flag);
    void setAnimation(int index, int flag);
    void setWriggleCharacter(int index, int flag);
    void setNearCharacter(int index, int flag);
    void setDisplay(int index, int flag);
    void setAlpha(int index, unsigned char alpha);
    void setPosition(int index, dss::Fix32Vector3& pos);
    void setSleepCharacter(int index, int flag);
    void setCollFlag(int index, int flag);
    int getDirection(int index);
    void resetCharaTalk();
    bool charaToCharaColl(TownCharacterBase* chara);
    void characterColl(dss::Fix32Vector3& nowPos, dss::Fix32Vector3& nextPos, dss::Fix32Vector3& vecN, dss::Fix32 r, dss::Fix32 ctrLen, int polyNo, int farTalk, int menuFlag);
    bool checkAbortPlayerPos(dss::Fix32Vector3 pos);
    bool checkTalkingNearCharacter(dss::Fix32Vector3& pos, short dirIdx, int searchObject);
    void setCharaAnim(int index, short count);
    void eventLockAllChraraAnim();
    int getCharaIndex(int index);
    void restoreCharacterAnim();
    void setAllEventLock(int flag);
    void setCopyPlayerChara(int index, dss::Fix32Vector3& pos, short idx, int charNo);
    bool checkIkadaTalk(dss::Fix32Vector3& target, dss::Fix32Vector3& playerDir);
    void setMonsterSpeakAll(int flag);
    void checkObjectInTalk(int objectId);
    void setAllMotionLock(int flag);

    void setCheckArea(int flag) { TownCharacterBase::areaCheck_ = flag; }
    void setScriptData(int ctrl, TOWN_SCRIPT_DATA& data) { character_[ctrl]->setScriptData(data); }
    void setRotFrame(int ctrl, int frame, short idx, int flag, int type) { character_[ctrl]->setRotFrame(frame, idx, flag, type); }
    void setChangePaletteRate(int ctrl, unsigned char r, unsigned char g, unsigned char b, int frame) { character_[ctrl]->setChangePaletteRate(r, g, b, frame); }
    void setPaletteRate(int ctrl, dss::Fix32 r, dss::Fix32 g, dss::Fix32 b) { character_[ctrl]->setPaletteRate(r, g, b); }
    void setChangePalletRate(int ctrl, dss::Fix32Vector3& rgb, int frame) { character_[ctrl]->setChangePaletteRate(rgb, frame); }
    void setMoveWait(int ctrl, int frame) { character_[ctrl]->setEnableLockWait(frame); }
    void setFadeType(int ctrl, int type, int frame) { character_[ctrl]->setFadeType(type, frame); }
    void setJumpMove(int ctrl, dss::Fix32Vector3& pos, int frame) { character_[ctrl]->setJumpMove(pos, frame); }
    void setSimpleRot(int ctrl, short idx, int frame, int type) { character_[ctrl]->setSimpleRot(idx, frame, type); }
    void setSureId(int index, int value) { character_[index]->setSurechigaiMapNo(value); }
    void setLockRot(int index, int lock) { character_[index]->setLockRot(lock); }
    void setSwingRound(int index, int value) { character_[index]->setSwingRound(value); }
    void setMonsterTalk(int index, int value) { character_[index]->setMonsterSpeak(value); }
    void setMapUid(int index, int uid) { character_[index]->setMapUid(uid); }
    void setLockMove(int index, int lock) { character_[index]->setPersonalEventLock(lock); }
    void setAction(int index, int value) { character_[index]->setMotionLock(value); }
    void setMotion(int index, int motion, int flag) { character_[index]->setMotion(motion, flag); }
};
