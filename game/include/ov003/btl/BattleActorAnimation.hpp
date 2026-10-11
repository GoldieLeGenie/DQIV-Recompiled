#pragma once
#include "main/btl/MonsterData.hpp"
#include "main/btl/BattleAutoFeed.hpp"
#include <globaldefs.h>
#include "main/dss/Camera.hpp"
#include "GameInfo.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/status/UseActionParam.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"
#include "main/object/DSSAObject.hpp"

namespace btl {
    struct BattleActorAnimation
    {
        static int setExecAnimation(status::UseActionParam* useActionParam);
        static void setExecAnimationMonster(status::CharacterStatus* actor);
        static int checkExecAnimation(status::UseActionParam* useActionParam);
        static int checkNormalAnimation(status::UseActionParam* useActionParam);
        static void setResultAnimation(status::UseActionParam* useActionParam, int currentTarget);
        static void setPlayerSE(status::UseActionParam* useActionParam, int currentTarget);
        static void setCommonSE(status::UseActionParam* useActionParam, int currentTarget);
        static void setCommonSEFromAction(status::UseActionParam* useActionParam);
        static int checkResultAnimation(status::UseActionParam* useActionParam, int currentTarget);
        static void setResultAnimationMonster(status::CharacterStatus* actor, status::CharacterStatus* target, int currentTarget);
        static void setAfterAnimation(status::CharacterStatus* actor, status::CharacterStatus* target, int targetCount, int currentIndex);
        static void setAfterAnimation2(status::CharacterStatus* actor, status::CharacterStatus* target);
        static void setMosyasChange(status::CharacterStatus* actor);
        static void setMosyasReverse(status::CharacterStatus* actor);
        static void setCallFriend(status::CharacterStatus* chara);
        static void setCallFriend();
        static void gattaiSlimeStart(status::CharacterStatus* actor, int actionIndex);
        static void gattaiSlime(status::CharacterStatus* actor, int actionIndex);
        static void setMonsterChangeSetup(status::CharacterStatus* actor);
        static int isMonsterChangeSetupEnd();
        static void setMonsterChange(status::CharacterStatus* actor);
        static void setMonstersDisappear(status::UseActionParam* useActionParam);
    };

}

extern int monsterChangeCount;
