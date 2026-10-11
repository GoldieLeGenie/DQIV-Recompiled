#pragma once
#include <globaldefs.h>
#include "main/dss/Position.hpp"
#include "main/object/DSSAObject.hpp"
#include "main/object/DSSACharacter.hpp"
#include "main/object/GameMonsterData.hpp"

struct GameMonster : DSSACharacter {
    int index_;                                 // 0xD30
    DSSACharacterData* dssaCharacterData_;      // 0xD34

    static GameMonsterData gameMonsterData_;

    GameMonster();
    ~GameMonster();
    void setup(int index);
    void cleanup();
    dss::Fix32 getWidth();
    int getWidthInt();
    static void setupTexture(int index);
    static void cleanupTexture(int index);
};
