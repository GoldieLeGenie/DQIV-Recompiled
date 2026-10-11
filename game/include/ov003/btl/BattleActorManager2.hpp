#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "ov003/btl/BattleActor2.hpp"

namespace btl {
    struct BattleActorManager2 {
        enum EventType
            {                                      
            EventNone    = 0x0,
            Velorinman   = 0x1,
            EvilPriest   = 0x2,
            DeathPissaro = 0x3,
            End          = 0x4,
        };
        BattleActor2 actor_[20];
        int actorCount_;
        int turnCount_;
        int escape_;
        int escapeSuccess_;
        int escapeCount_;
        int eventFlag_;
        EventType eventType_;
        int eventTile_;
        int eventEnd_;
        FirstAttack firstAttack_;
        int monsterDeathCount_;        // 0x66a8
        int monsterEscapeCount_;       // 0x66ac
        int monsterDisappearCount_;    // 0x66b0
        int winningStatus_;            // 0x66b4
        short deathLog_;
        BattleActorManager2();
        ~BattleActorManager2();
        static BattleActorManager2* getSingleton();
        void initialize();
        void selectActor();
        void selectActorPlayer();
        void selectActorMonster();
        BattleActor2* add(status::CharacterStatus* chara);
        void setActorOrder();
        void setActorAction();
        int getActorCount();
        BattleActor2*  getBattleActor(int index);
        void execStartOfRound();
        void execEndOfRound();
        void retireActor();
        void checkDeathMonster();
        void clearDeadMonster(int all);
        void execStartOfBattle();
        void execEndOfBattle();
        int isBattleEnd();
        void execMonsterDeath(int index);
        void execMonsterDeathForItem();
        int isActionEnable();
        void setFirstAttack(FirstAttack firstAttack);
        FirstAttack getFirstAttack();
        void clearFirstAttack();
        void setEscape(int flag);
        void setEventBattle(int flag, int tile);
        int isImpEventBattle();
        void addMonsterDeathCount(int count);
        int getMonsterDeathCount();
        void addMonsterEscapeCount(int count);
        int getMonsterEscapeCount();
        void addMonsterDisappearCount(int count);
        int getMonsterDisappearCount();
        void setMegazaruRing(status::UseActionParam* useActionParam);
    };
}
extern status::PlayerStatus dummyPlayer_;

