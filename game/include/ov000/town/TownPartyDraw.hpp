#pragma once
#include "globaldefs.h"
#include "main/dss/DssUtils.hpp"
#include "main/data/DataObject.hpp"
#include "main/object/DisplayCharacter.hpp"

struct TownPartyDraw {
    DisplayCharacter partyCharacter_[8];        // 0x0000
    int count_;                                 // 0x12A0
    int countReal_;                             // 0x12A4
    char partyDispAlpha_[8];                    // 0x12A8
    int exe_;                                   // 0x12B0
    int change_[8];                             // 0x12B4
    DataObject dataObject_;                     // 0x12D4

    static const unsigned short colorDoku;
    static const unsigned short colorBarrier;

    TownPartyDraw();
    ~TownPartyDraw();
    void setup();
    void cleanup();
    void setPosition(int index, const dss::Fix32Vector3& pos);
    void setRotate(int index, int dirIdx);
    void resetAlpha();
    void resetDrawPartyCount();
    void setDrawPartyOne();
    void setDrawPartyNone();
    void setAnimationOne(int anim);
    void setAnimation(int anim);
    void setWriggleCharacter(int flag);
    void setWriggleCharaAll(int flag);
    void setAlpha(int index, char alpha);
    void setPlayerAlpha(unsigned char alpha);
    void addAlpha(int index, char alpha);
    static int getMin(int a, int b);
    static int getMax(int a, int b);
    void execute();
    void setExcute(int flag);
    void changePose(int pose);
    void restorePose();
    void setSleep(int sleep);
    void requestCharacterReload();
    void setVanAndBasha();
};
