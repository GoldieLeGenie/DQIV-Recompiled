#pragma once
#include "globaldefs.h"
#include "ov000/town/TownCharacter.hpp"

struct TownCharacterStorage {
    static const int CHARACTER_MAX = 32;
    static const int MONSTER_MAX = 2;
    static const int MODEL_MAX = 5;
    static const int FUNITURE_MAX = 2;

    int characterCount_;                                    // 0x0000
    int monsterCount_;                                      // 0x0004
    int modelCount_;                                        // 0x0008
    int spriteCount_;                                       // 0x000C
    int funitureCount_;                                     // 0x0010
    TownCharacterDraw normal_[CHARACTER_MAX];               // 0x0014
    TownMonsterDraw monster_[MONSTER_MAX];                  // 0x7914
    TownModelDraw model_[MODEL_MAX];                        // 0x965C
    TownCharacterFuniture funitureChara_[FUNITURE_MAX];     // 0xDAE0

    TownCharacterStorage();
    ~TownCharacterStorage();
    void initialize();
    void terminate();
    TownCharacterBase* getContainer(int type);
    void restoreContainer(int type);
};
