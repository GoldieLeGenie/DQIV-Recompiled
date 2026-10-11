#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/BaseActionMessage.hpp"
#include "main/status/ActionExec.hpp"
#include "main/status/UseAction.hpp"
#include "main/dss/DssUtils.hpp"

namespace status{
    struct BaseActionWorkParam {
        volatile int value_;
        BaseActionWorkParam() { value_ = 0; }
    };

    struct BaseAction {
        static UseActionParam* useActionParam_;
        static int mirrorDamage_;
        static BaseActionWorkParam workParam_;
        static param::ActionParam* actionParam_;
        static int flag_;
        static int multiFlag_;
        static int eventBattle_;
        static int missFlag_;
        static int tsukon2Flag_;
        static int tsukonFlag_;
        static volatile int kaishinFlag_;
        static int allKaishinFlag_;
        static int timeReverseFlag_;
        static int joukFlag_;
        static int splitFlag_;
        static int doubleFlag_;
        static int callMonster_[4];
        int actionIndex_;
        CharacterType targetType_;
        int effectValue_;
        int executeAction_;
        int resultFlag_;
        int instantDeath_;
        int mahokantaFlag_;
        int moonSaltCount_;
        int rollingKick_;
        int sleepTurn_;
        int playerEffectValue_;
        int monsterEffectValue_;
        int originalEffectValue_;
        BaseActionMessage message_;
        BaseAction();
        ~BaseAction();
        static void initialize();
        static void clear();
        void exec(UseActionParam& useActionParam, int flag);
        int execBefore();
        int execAfterOne(int index);
        int execAfter(int flag);
        int execMainRoot(status::CharacterStatus* actor, status::CharacterStatus* target);
        int execMain(status::CharacterStatus *actor,status::CharacterStatus *target);
        void execActionEffect(status::CharacterStatus* actor, status::CharacterStatus* target);
        void execAddDamage(status::CharacterStatus* actor, status::CharacterStatus* target);
        void execAddDamageItemForceE(status::CharacterStatus* actor, status::CharacterStatus* target);
        int checkStatusRelease();
        int checkActorStatusChangeRelease(status::CharacterStatus* actor);
        int checkActorAstoron();
        int checkSpazz();
        int checkSleep();
        int checkPath1();
        int checkTimeStop();
        void checkConfusion();
        void execActionDefence(status::CharacterStatus* target);
        void execActionDefenceException(status::CharacterStatus *target);
        void setActionDefenceValueAdd(int ratio);
        int checkUseMp();
        int isUseMp();
        int checkMahoton();
        int checkFizzleZone();  
        int checkPowerSave(status::CharacterStatus* actor);
        int checkActorBaikiruto(status::CharacterStatus* actor);
        status::CharacterStatus* checkTargetSpecialSelect(
        status::CharacterStatus *actor,
        status::CharacterStatus *target);
        int checkTargetZaoraruZaoriku(status::CharacterStatus *target);
        int checkTargetBaikiruto(status::CharacterStatus *target);
        int checkTargetMahosute(status::CharacterStatus *target, status::CharacterStatus *actor);
        int checkTargetAstoron(status::CharacterStatus *target);
        int checkTargetFubaha(status::CharacterStatus *target);
        int checkTargetDefence(status::CharacterStatus *target);
        int checkTargetJouk(status::CharacterStatus *target);
        int checkTargetSplitJouk(status::CharacterStatus *target);
        int checkTargetReleaseConfusionSleep(status::CharacterStatus *target);
        int checkTargetReleaseItetsukuhadou(status::CharacterStatus *target);
        int checkTargetMahokanta(status::CharacterStatus *actor,status::CharacterStatus *target);
        int checkTargetEquipment(status::CharacterStatus *target);
        int checkActorManusa(status::CharacterStatus *actor,status::CharacterStatus *target);
        int checkTargetFeather(status::CharacterStatus *target);
        int checkActorKaishin(status::CharacterStatus *target);
        int checkActorDouble(status::CharacterStatus *target);
        int checkItemMissA(status::CharacterStatus *actor, status::CharacterStatus *target);
        int checkItemMuchiB(status::CharacterStatus *actor, status::CharacterStatus *target);
        int checkItemDamageC(status::CharacterStatus *actor, status::CharacterStatus *target);
        int checkItemDamageD(status::CharacterStatus *actor, status::CharacterStatus *target);
        int checkItemForceE(status::CharacterStatus *actor, status::CharacterStatus *target);
        int checkItemRecoveryI(status::CharacterStatus *actor,status::CharacterStatus *target);
        int checkItemRecoveryJ(status::CharacterStatus *actor,status::CharacterStatus *target);
        int checkItemOther(status::CharacterStatus *actor,status::CharacterStatus *target);
        void checkSleep(status::CharacterStatus *target);
        void useMp();
        virtual void setEffectValue(status::CharacterStatus *target);
        void setEffectValueAdd(int ratio, int offset);
        void setEffectValueException(status::CharacterStatus *actor,status::CharacterStatus *target);
        static void setBreakPrayRing(int flag);
        static bool isBreakPrayRing();

    };
    
}

struct SplitJoukTable { int v[3]; };
extern const SplitJoukTable splitJoukTable;
