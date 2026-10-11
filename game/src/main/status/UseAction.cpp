#pragma ipa file
#include "main/status/UseAction.hpp"
#include "main/status/BaseAction.hpp"
#include "main/status/HaveAction.hpp"

int status::UseAction::actionIndex_;
status::UseActionParam* status::UseAction::useActionParam_;
param::ActionParam* status::UseAction::actionParam_;
param::AbreactTurn* status::UseAction::abreactTurn_;

THUMB void status::UseAction::initialize() {
    actionParam_ = status::excelParam.actionParam_;
    abreactTurn_ = status::excelParam.abreactTurn_;
}

THUMB ActionDefenceKind status::UseAction::getActionDefenceKind(int actionIndex)
{
    switch (status::excelParam.actionParam_[actionIndex].magictype) {
    case 23:
        return ACT_DEF_ALL;
    case 0:
        return ACT_DEF_MERA;
    case 1:
        return ACT_DEF_GIRA;
    case 2:
        return ACT_DEF_IO;
    case 3:
        return ACT_DEF_HYADO;
    case 4:
        return ACT_DEF_BAGI;
    case 5:
        return ACT_DEF_DEIN;
    case 6:
        return ACT_DEF_ZAKI;
    case 7:
        return ACT_DEF_MEGANTE;
    case 8:
        return ACT_DEF_NIFRAM;
    case 9:
        return ACT_DEF_MANUSA;
    case 10:
        return ACT_DEF_MEDAPANI;
    case 11:
        return ACT_DEF_RARIHO;
    case 12:
        return ACT_DEF_MAHOTONE;
    case 13:
        return ACT_DEF_MAHOTORA;
    case 14:
        return ACT_DEF_RUKANI;
    case 15:
        return ACT_DEF_FIRE;
    case 16:
        return ACT_DEF_BLIZZARD;
    case 17:
        return ACT_DEF_POISON;
    case 18:
        return ACT_DEF_SPAZZ;
    case 19:
        return ACT_DEF_CLAP;
    case 20:
        return ACT_DEF_ARMY;
    case 21:
        return ACT_DEF_ANTIDANCE;
    case 22:
        return ACT_DEF_REST;
    case 24:
        return ACT_DEF_ALL;
    case 25:
        return ACT_DEF_ALL;
    case 26:
        return ACT_DEF_ALL;
    case 27:
        return ACT_DEF_ALL;
    case 28:
        return ACT_DEF_ALL;
    case 29:
        return ACT_DEF_ALL;
    default:
        return ACT_DEF_ALL;
    }
}

THUMB ActionDefenceType status::UseAction::getActionDefenceType(int actionIndex) {
    switch ((status::excelParam.actionParam_[actionIndex].byte_6 & 0x1C) << 0x16 >> 0x18) {
    case 0:  return ACT_DEF_A;
    case 1:  return ACT_DEF_B;
    case 2:  return ACT_DEF_C;
    case 3:  return ACT_DEF_D;
    case 4:  return ACT_DEF_E;
    case 5:  return ACT_DEF_F;
    case 6:  return ACT_DEF_H;
    default: return ACT_DEF_A;
    }
}

THUMB bool status::UseAction::isUsuallyUse(int ItemIndex) {
    if ((((status::excelParam.actionParam_[ItemIndex].byte_5 & 0x40) << 0x12) >> 0x18) == 1) {
        return 1;
    }
    return 0;
}

THUMB bool status::UseAction::isBattleUse(int itemIndex) {
    return (((status::excelParam.actionParam_[itemIndex].byte_5 & 0x20) << 0x13) >> 0x18) == 1;
}

THUMB int status::UseAction::getUseMp(int actionIndex) {
    return status::excelParam.actionParam_[actionIndex].useMP;
}

THUMB int status::UseAction::getMenuMessage(int actionIndex) {
    return status::excelParam.actionParam_[actionIndex].menuMes;
}

THUMB int status::UseAction::getWordDBIndex(int actionIndex) {
    return status::excelParam.actionParam_[actionIndex].menuIndex;
}

THUMB status::UseItem::UseType status::UseAction::getUseType(int itemIndex) {
    int bitField = actionParam_[itemIndex].byte_6 & 0xE0U;
    int val = (bitField << 19) >> 24;

    switch (val) {
    case 0:  return status::UseItem::None;
    case 1:  return status::UseItem::Myself;
    case 2:  return status::UseItem::Friend;
    case 3:  return status::UseItem::Enemy;
    case 4:  return status::UseItem::Both;
    default: return status::UseItem::None;
    }
}

THUMB status::UseItem::UseArea status::UseAction::getUseArea(int itemIndex) {
    char val = actionParam_[itemIndex].byte_7;
    char result = (val & 7);

    switch (result) {
    case 0:  return status::UseItem::Zero;
    case 1:  return status::UseItem::One;
    case 2:  return status::UseItem::Group;
    case 3:  return status::UseItem::All;
    case 4:  return status::UseItem::Horse;
    case 5:  return status::UseItem::UsuallyHorse;
    case 6:  return status::UseItem::BattleHorse;
    default: return status::UseItem::One;
    }
}

THUMB int status::UseAction::getEffectValueToPlayer(int actionIndex) {
    volatile param::ActionParam* actionParam = actionParam_;

    unsigned int minVal = actionParam[actionIndex].PlayerMin;
    unsigned int maxVal = actionParam[actionIndex].PlayerMax;

    int randomVal = dssrand::rand((maxVal - minVal + 1));
    return (minVal + randomVal);
}

THUMB int status::UseAction::getEffectValueToMonster(int actionIndex) {
    volatile param::ActionParam* actionParam = actionParam_;

    unsigned int minVal = actionParam[actionIndex].MonsterMin;
    unsigned int maxVal = actionParam[actionIndex].MonsterMax;

    int randomVal = dssrand::rand((maxVal - minVal + 1));
    return (minVal + randomVal);
}

THUMB int status::UseAction::getEffectValueForAvarage(int actionIndex) {
    volatile param::ActionParam* actionParam = actionParam_;

    unsigned int sum =
        actionParam[actionIndex].PlayerMin +
        actionParam[actionIndex].PlayerMax;

    int tmp = sum + (sum >> 31);

    return (tmp >> 1);
}

THUMB int status::UseAction::getEffectValueToPlayerMin(int actionIndex) {
    return actionParam_[actionIndex].PlayerMin;
}

THUMB int status::UseAction::getEffectValueToMonsterMin(int actionIndex) {
    return actionParam_[actionIndex].MonsterMin;
}

THUMB status::UseAction::DamageType status::UseAction::getDamageType(int actionIndex) {
    if (actionIndex == 0x4B) return DamageTypeDefence;
    if (actionIndex == 0x9B) return DamageTypeDefence;
    if (actionIndex == 0x9C) return DamageTypeDefence;

    switch (actionParam_[actionIndex].canceltype) {
    case 0:  return DamageTypeNone;
    case 1:  return DamageTypeDamage;
    case 2:  return DamageTypeRecovery;
    case 3:  return DamageTypeInstantDeath;
    case 4:  return DamageTypeRebirth;
    case 5:  return DamageTypeAddMp;
    case 6:  return DamageTypeSubMp;
    case 7:  return DamageTypeStatusClear;
    case 8:  return DamageTypeAstoron;
    case 9:  return DamageTypeSpazz;
    case 10: return DamageTypeSleep;
    case 12: return DamageTypeBaikiruto;
    case 14: return DamageTypeMahokanta;
    case 15: return DamageTypeMahosute;
    case 18: return DamageTypeMahoton;
    case 20: return DamageTypeConfusion;
    case 22: return DamageTypePoison;
    case 24: return DamageTypeDefenceChange;
    case 25: return DamageTypeAgilityChange;
    case 19: return DamageTypeDragoram;
    case 11: return DamageTypeManusa;
    case 13: return DamageTypeFubaha;
    case 39: return DamageTypeDefence;
    case 26: return DamageTypeFeather;
    case 28: return DamageTypeLight;
    case 40: return DamageTypeFizzleZone;
    case 16: return DamageTypeMosyasu;
    case 21: return DamageTypePath1;
    case 17: return DamageTypePowerSave;
    case 41: return DamageTypeTimeStop;
    case 29:
    case 27:
    case 23:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
    case 38:
    case 42:
        return DamageTypeNone;
    default: return DamageTypeNone;
    }
}

THUMB bool status::UseAction::isFubaha(int actionIndex) {
    unsigned short flags = actionParam_[actionIndex].byte_1;
    if ((((flags & 0x04) << 0x16) >> 0x18)) {
        return true;
    }
    return false;
}

THUMB bool status::UseAction::isManusa(int actionIndex) {
    unsigned short flags = actionParam_[actionIndex].byte_1;
    if ((((flags & 0x10) << 0x14) >> 0x18)) {
        return true;
    }
    return false;
}

THUMB bool status::UseAction::isJouk(int actionIndex) {
    if ((((actionParam_[actionIndex].byte_1 & 0x40) << 0x12) >> 0x18)) {
        return 1;
    }
    return 0;
}

THUMB bool status::UseAction::isPowerSave(int actionIndex) {
    if ((((actionParam_[actionIndex].byte_1 & 0x80) << 0x11) >> 0x18)) {
        return 1;
    }
    return 0;
}

THUMB bool status::UseAction::isBaikiruto(int actionIndex) {
    return (char)(actionParam_[actionIndex].byte_2 & 1);
}

THUMB bool status::UseAction::isKaishin(int actionIndex) {
    if ((((actionParam_[actionIndex].byte_2 & 2) << 0x17) >> 0x18)) {
        return 1;
    }
    return 0;
}

THUMB bool status::UseAction::isCrossFire(int actionIndex) {
    if ((((actionParam_[actionIndex].byte_2 & 4) << 0x16) >> 0x18)) {
        return 1;
    }
    return 0;
}

THUMB bool status::UseAction::isAstoron(int actionIndex) {
    if ((((actionParam_[actionIndex].byte_2 & 8) << 0x15) >> 0x18)) {
        return 1;
    }
    return 0;
}

THUMB bool status::UseAction::isMahokanta(int actionIndex) {
    if ((((actionParam_[actionIndex].byte_2 & 0x20) << 0x13) >> 0x18)) {
        return 1;
    }
    return 0;
}

THUMB bool status::UseAction::isMahosute(int actionIndex) {
    if ((((actionParam_[actionIndex].byte_2 & 0x40) << 0x12) >> 0x18)) {
        return 1;
    }
    return 0;
}

THUMB bool status::UseAction::isReleaseConfusion(int actionIndex) {
    return (char)(actionParam_[actionIndex].byte_3 & 1);
}

THUMB bool status::UseAction::isSplitJouk(int actionIndex) {
    if ((((actionParam_[actionIndex].byte_3 & 8) << 0x15) >> 0x18)) {
        return 1;
    }
    return 0;
}

THUMB bool status::UseAction::isErrorA(int actionIndex) {
    if ((((actionParam_[actionIndex].byte_3 & 0x10) << 0x14) >> 0x18)) {
        return 1;
    }
    return 0;
}

THUMB bool status::UseAction::isErrorB(int actionIndex) {
    if ((((actionParam_[actionIndex].byte_3 & 0x20) << 0x13) >> 0x18)) {
        return 1;
    }
    return 0;
}

THUMB bool status::UseAction::isDamageC(int actionIndex) {
    if ((((actionParam_[actionIndex].byte_3 & 0x40) << 0x12) >> 0x18)) {
        return 1;
    }
    return 0;
}

THUMB bool status::UseAction::isDamageD(int actionIndex) {
    if ((((actionParam_[actionIndex].byte_3 & 0x80) << 0x11) >> 0x18)) {
        return 1;
    }
    return 0;
}

THUMB bool status::UseAction::isForceE(int actionIndex) {
    return (char)(actionParam_[actionIndex].byte_4 & 1);
}

THUMB bool status::UseAction::isMultiF(int actionIndex) {
    if ((((actionParam_[actionIndex].byte_4 & 2) << 0x17) >> 0x18)) {
        return 1;
    }
    return 0;
}

THUMB bool status::UseAction::isAddHp(int actionIndex) {
    if ((((actionParam_[actionIndex].byte_4 & 8) << 0x15) >> 0x18)) {
        return 1;
    }
    return 0;
}

THUMB bool status::UseAction::isMahoton(int actionIndex) {
    if ((((actionParam_[actionIndex].byte_5 & 0x10) << 0x14) >> 0x18)) {
        return 1;
    }
    return 0;
}

THUMB bool status::UseAction::isTargetDeadOrAlive(int actionIndex) {
    if ((((actionParam_[actionIndex].byte_4 & 0x40) << 0x12) >> 0x18)) {
        return 1;
    }
    return 0;
}

THUMB bool status::UseAction::isSpecialSelectTarget(int actionIndex) {
    if ((((actionParam_[actionIndex].byte_3 & 4) << 0x16) >> 0x18)) {
        return 1;
    }
    return 0;
}

THUMB bool status::UseAction::isUse(int actionIndex, HaveStatusInfo* actorStatusInfo) {
    int useMp = getUseMp(actionIndex);
    int mp = actorStatusInfo->getMp();

    if (useMp <= mp) {
        return true;
    }

    useMp = getUseMp(actionIndex);
    if (useMp == 0xff && actorStatusInfo->getMp() != 0) {
        return true;
    }

    return false;
}

THUMB status::UseAction::AIType status::UseAction::getAIType(int actionIndex) {
    if (actionIndex == 0x25) {
        return Defence;
    }
    if (actionIndex == 0x26) {
        return Defence;
    }
    if (actionIndex == 0x2c) {
        return Defence;
    }

    char val = actionParam_[actionIndex].byte_7;
    int temp = (val & 0x78) << 0x15 >> 0x18;

    switch (temp) {
    case 1:
        return Attack;
    case 2:
        return Defence;
    case 3:
        return Assist;
    case 4:
        return Recovery;
    case 0:
        return Invalid;
    default:
        return Attack;
    }
}

THUMB bool status::UseAction::isMosyasAction(int actionIndex) {
    if ((((actionParam_[actionIndex].byte_3 & 2) << 0x17) >> 0x18) == 1) {
        return 1;
    }
    return 0;
}

THUMB status::UseAction::ActionType status::UseAction::getActionType(int actionIndex) {
    unsigned char actionType = actionParam_[actionIndex].type;
    switch (actionType) {
    case ActionTypeMagic:
        return ActionTypeMagic;
    case ActionTypeSkill:
        return ActionTypeSkill;
    default:
        return ActionTypeNone;
    }
}

THUMB status::BaseAction* status::UseAction::getAction()
{
    static status::BaseAction action;
    return &action;
}

// only needed for the .bss layout
THUMB status::BaseAction* getUnusedAction()
{
    static status::BaseAction action;
    return &action;
}

THUMB void status::UseAction::execUse(UseActionParam* useActionParam) {
    useActionParam_ = useActionParam;
    actionIndex_ = useActionParam->actionIndex_;
    status::BaseAction* action = getAction();
    if (status::HaveAction::isBattleMode()) {
        action->exec(*useActionParam, 1);
    }
    else {
        action->exec(*useActionParam, 0);
    }
    useActionParam_->result_ = action->resultFlag_;
}