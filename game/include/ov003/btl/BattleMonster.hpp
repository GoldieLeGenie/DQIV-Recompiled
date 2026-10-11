#pragma once
#include <globaldefs.h>
#include "main/object/GameMonster.hpp"
#include "main/object/PaletteAnimationObject.hpp"
#include "main/data/DataObject.hpp"
#include "main/btl/MonsterData.hpp"


namespace btl {
    struct BattleMonster {
        GameMonster monsterDraw_;               // 0x000
        int monsterGroup_;                      // 0xD38
        int monsterIndex_;                      // 0xD3C
        int screenPosition_;                    // 0xD40
        int screenWidth_;                       // 0xD44
        int actionindex_;                       // 0xD48
        int animindex_;                         // 0xD4C
        dss::Vector2<int> screen;               // 0xD50
        int animationFlag_;                     // 0xD58
        DataObject paletteData_;                // 0xD5C
        PaletteAnimationObject paletteAnim_;    // 0xD6C

        BattleMonster();
        void setup(int monsterGroup, int monsterIndex);
        void cleanup();
        bool isEnable();
        dss::Fix32 getWidth();
        int getWidthInt();
        void setPosition(const dss::Fix32Vector3& pos);
        void draw();
        void startAnimation(int actionIndex, int animIndex);
        void setCameraAnimation(int index);
        void startAnimation(int animIndex);
        void startAnimationWithLoop(int animIndex, int flag);
        bool startGattai();
        void disappearGattaiSlime();
        bool isAppearKingSlime2();
        void setPaletteAnim(int animNo);
        void setTransOfEnd();

        dss::Fix32Vector3 getNullPosition(int index, int type) { return monsterDraw_.getNullPosition(index, type); }
    };

    struct BattleMonsterDraw2 {
        BattleMonster monsters_[12];            // 0x0000
        int monsterCount_;                      // 0xBA90
        signed char array_[300];                // 0xBA94
        int spaceArray_[32];                    // 0xBBC0
        int arrayInt_;                          // 0xBC40
        int spaceCount_;                        // 0xBC44
        int spacePos_;                          // 0xBC48
        int spaceWidth_;                        // 0xBC4C
        int enable_;                            // 0xBC50

        BattleMonsterDraw2();
        ~BattleMonsterDraw2();
        static BattleMonsterDraw2* getSingleton();
        void setup();
        void cleanup();
        int setup(int monsterGroup, int monsterIndex);
        void cleanup(int ctrl);
        int getCount();
        void draw();
        void setArrayPos();
        void resetArrayPos();
        void searchArrayPos(int monsterIndex);
        const char* printArrayPos(int index);
        bool isCallFriend(int monsterIndex);
        bool isAppearKingSlime2();
        void startAnimationWithLoop(int ctrl, int index, int loop);
    };
}

