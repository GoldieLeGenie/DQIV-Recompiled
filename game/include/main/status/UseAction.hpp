#pragma once 
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/UseActionParam.hpp"
#include "main/status/UseItem.hpp"
#include "main/status/HaveStatusInfo.hpp"
#include "main/param/Param.hpp"


namespace status
{
    struct BaseAction;   // forward decl for getAction()

    struct UseAction {
        enum UseType {
            Myself = 0,
            Friend = 1,
            Enemy = 2,
            Both = 3,
            None = 4
        };
        enum ActionType {
            ActionTypeNone = 0,
            ActionTypeMagic = 1,
            ActionTypeSkill = 2
        };
        enum AIType {
            Attack = 0,
            Defence = 1,
            Assist = 2,
            Recovery = 3,
            Invalid = 4
        };
        enum DamageType {
            DamageTypeNone = 0,
            DamageTypeDamage = 1,
            DamageTypeRecovery = 2,
            DamageTypeInstantDeath = 3,
            DamageTypeRebirth = 4,
            DamageTypeAddMp = 5,
            DamageTypeSubMp = 6,
            DamageTypeStatusClear = 7,
            DamageTypeAstoron = 8,
            DamageTypeSpazz = 9,
            DamageTypeSleep = 10,
            DamageTypeManusa = 11,
            DamageTypeBaikiruto = 12,
            DamageTypeFubaha = 13,
            DamageTypeMahokanta = 14,
            DamageTypeMahosute = 15,
            DamageTypeMosyasu = 16,
            DamageTypePowerSave = 17,
            DamageTypeMahoton = 18,
            DamageTypeDragoram = 19,
            DamageTypeConfusion = 20,
            DamageTypePath1 = 21,
            DamageTypePoison = 22,
            DamageTypeMOUDOKU = 23,
            DamageTypeDefenceChange = 24,
            DamageTypeAgilityChange = 25,
            DamageTypeFeather = 26,
            DamageTypeKOORITSUKI = 27,
            DamageTypeLight = 28,
            DamageTypeWOOLGUARD = 29,
            DamageTypeMAHOTAN = 30,
            DamageTypeMAHOKITE = 31,
            DamageTypeOIKAZE = 32,
            DamageTypeHENSHIN = 33,
            DamageTypeMIKAWASHIKYAKU = 34,
            DamageTypeSEIREINOUTA = 35,
            DamageTypeODORIFUUJI = 36,
            DamageTypeKUSUGURINOKEI = 37,
            DamageTypeMDEF_HENKA = 38,
            DamageTypeDefence = 39,
            DamageTypeFizzleZone = 40,
            DamageTypeTimeStop = 41,
            DamageTypeHIKARI = 42,
            DamageTypeMAX = 43
        };
            static int actionIndex_;
            static UseActionParam* useActionParam_;
            static param::ActionParam* actionParam_;
            static param::AbreactTurn* abreactTurn_;
            static void initialize();
            static ActionDefenceKind getActionDefenceKind(int actionIndex);
            static ActionDefenceType getActionDefenceType(int actionIndex);
            static bool isUsuallyUse(int ItemIndex);
            static bool isBattleUse(int itemIndex);
            static int getUseMp(int actionIndex);
            static int getMenuMessage(int actionIndex);
            static int getWordDBIndex(int actionIndex);
            static UseItem::UseType getUseType(int itemIndex);
            static UseItem::UseArea getUseArea(int itemIndex);
            static int getEffectValueToPlayer(int actionIndex);
            static int getEffectValueToMonster(int actionIndex);
            static int getEffectValueForAvarage(int actionIndex);
            static int getEffectValueToPlayerMin(int actionIndex);
            static int getEffectValueToMonsterMin(int actionIndex);
            static DamageType getDamageType(int actionIndex);
            static bool isFubaha(int actionIndex);
            static bool isManusa(int actionIndex);
            static bool isJouk(int actionIndex);
            static bool isPowerSave(int actionIndex);
            static bool isBaikiruto(int actionIndex);
            static bool isKaishin(int actionIndex);
            static bool isCrossFire(int actionIndex);
            static bool isAstoron(int actionIndex);
            static bool isMahokanta(int actionIndex);
            static bool isMahosute(int actionIndex);
            static bool isReleaseConfusion(int actionIndex);
            static bool isSplitJouk(int actionIndex);
            static bool isErrorA(int actionIndex);
            static bool isErrorB(int actionIndex);
            static bool isDamageC(int actionIndex);
            static bool isDamageD(int actionIndex);
            static bool isForceE(int actionIndex);
            static bool isMultiF(int actionIndex);
            static bool isAddHp(int actionIndex);
            static bool isMahoton(int actionIndex);
            static bool isTargetDeadOrAlive(int actionIndex);
            static bool isSpecialSelectTarget(int actionIndex);
            static bool isUse(int actionIndex, status::HaveStatusInfo* actorStatusInfo);
            static AIType getAIType(int actionIndex);
            static bool isMosyasAction(int actionIndex);
            static ActionType getActionType(int actionIndex);
            static BaseAction* getAction();
            static void execUse(UseActionParam* useActionParam);

        };
        
} 
