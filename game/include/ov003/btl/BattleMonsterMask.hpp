#pragma once
#include <globaldefs.h>
#include "main/dss/Camera.hpp"
#include "main/dss/UnkSprite2D.hpp"

struct BattleMonsterMask {
    int select_;                                // 0x000
    UnkSprite2D sprite_;                        // 0x004
    UnkMenuSprite mask_[2];                     // 0x03C
    dss::Vector2<int> targetPos[12];            // 0x0DC
    dss::Fix32 scale_;                          // 0x13C

    BattleMonsterMask();
    ~BattleMonsterMask();
    static BattleMonsterMask* getSingleton();
    void initialize();
    void setup();
    void terminate();
    void execute();
    void select(int groupId);
    void draw();
    void calcTargetPos(int actorindex);
    dss::Vector2<int> getTargetPos(int actorindex);
    int* getMonsterTouchRect(int actorindex);

    static int monsterRectTemp[12];
};

