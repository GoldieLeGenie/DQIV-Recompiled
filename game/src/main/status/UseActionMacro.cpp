#include "main/status/UseActionMacro.hpp"
#include "ov016/TownMenuPlayerControl/TownMenuPlayerControl.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/status/UseAction.hpp"
#include "main/status/ActionExec.hpp"
#include "main/status/BaseAction.hpp"
#include "main/status/BaseActionStatus.hpp"
#include "main/status/BaseActionMessage.hpp"
#include "main/status/MonsterStatus.hpp"
#include "main/global/Global.hpp"
#include "main/dss/Random.hpp"
#include "ov003/status/MonsterParty.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"
#include "ov000/town/TownFurniture.hpp"

THUMB void status::UseActionMacro::setBeforeMacro(CharacterStatus* actor, int actionIndex)
{
    TextAPI::setMACRO0(0x11, 0x70000000, UseAction::getWordDBIndex(actionIndex));
    if (actionIndex >= 0x1cb && actionIndex <= 0x1e5) {
        TextAPI::setMACRO0(0x11, 0x70000000, UseAction::getWordDBIndex(0x42));
    }
}

THUMB void status::UseActionMacro::setExecMacro(CharacterStatus* actor, CharacterStatus* target, int actionIndex)
{
    TextAPI::setMACRO0(0x11, 0x70000000, UseAction::getWordDBIndex(actionIndex));
    if (actor->haveBattleStatus_.getSelectCommand() == HaveBattleStatus::UseItem) {
        TextAPI::setMACRO0(0xa, 0x40000000, actor->haveBattleStatus_.selectIndex_);
    }
    if (actionIndex == 0x15d) {
        TextAPI::setMACRO0(0xa, 0x40000000, 0x6f);
    }
    if (actionIndex == 0x14d) {
        TextAPI::setMACRO0(0xa, 0x40000000, 0x18);
    }
    if (actionIndex == 0x14b) {
        TextAPI::setMACRO0(0xa, 0x40000000, 0x12);
    }
    if (actionIndex == 0x16c) {
        TextAPI::setMACRO0(0xa, 0x40000000, 0x82);
    }
    if (actionIndex == 0x170) {
        TextAPI::setMACRO0(0xa, 0x40000000, 0x8e);
    }
    if (actionIndex == 0x100) {
        TextAPI::setMACRO0(4, 0x70000000, 0x100);
    }
    if (actionIndex == 0x101) {
        TextAPI::setMACRO0(4, 0x70000000, 0x101);
    }
    if (actionIndex == 0x103) {
        TextAPI::setMACRO0(4, 0x70000000, 0x103);
    }
    if (actionIndex == 0x97) {
        TextAPI::setMACRO0(0, 0x70000000, 0x97);
    }
    if (actionIndex == 0x8c) {
        TextAPI::setMACRO0(0, 0x70000000, 0x8c);
    }
    if (actionIndex == 0x8e) {
        TextAPI::setMACRO0(0, 0x70000000, 0x8e);
    }
    if (actionIndex == 0x6a || actionIndex == 0x154) {
        TextAPI::setMACRO0(0xd, 0x60000000, g_monster.getMonsterCallIndex());
    }
    if (actionIndex == 0x94) {
        TextAPI::setMACRO0(0, 0x70000000, 0x94);
    }
    if (actionIndex >= 0x21e && actionIndex <= 0x226) {
        TextAPI::setMACRO0(0x15, 0x70000000, actionIndex);
    }
    if (actionIndex >= 0x10e && actionIndex <= 0x11a) {
        TextAPI::setMACRO0(0x12, 0x60000000, getCallDifferentMonsterIndex());
    }
    if (actionIndex == 0x1a5) {
        TextAPI::setMACRO0(0x14, 0x40000000, actor->haveStatusInfo_.haveEquipment_.getEquipment(ITEM_WEAPON));
    }
    if (actionIndex == 0x1ab) {
        TextAPI::setMACRO0(0xa, 0x40000000, actor->haveStatusInfo_.haveItem_.getItem(dssrand::rand(actor->haveStatusInfo_.haveItem_.getCount())));
    }
    if (actionIndex == 0x1b4) {
        TextAPI::setMACRO0(0x11, 0x70000000, 0x32);
    }
    if (actionIndex == 0x1b5) {
        TextAPI::setMACRO0(0x11, 0x70000000, 0x35);
    }
    if (actionIndex == 0x1b6) {
        TextAPI::setMACRO0(0x11, 0x70000000, 0x38);
    }
    if (actionIndex == 0x1ba) {
        TextAPI::setMACRO0(0x11, 0x70000000, 0x27);
    }
    if (actionIndex == 0x1bc) {
        TextAPI::setMACRO0(0xa, 0x40000000, BaseActionStatus::work_);
    }
    if (actionIndex >= 0x1cb && actionIndex <= 0x1e5) {
        TextAPI::setMACRO0(0x11, 0x70000000, UseAction::getWordDBIndex(0x42));
    }
    if (actionIndex == 0x1e0) {
        TextAPI::setMACRO0(0xd, 0x60000000, BaseAction::callMonster_[0]);
    }
    if (actionIndex == 0x201) {
        TextAPI::setMACRO0(0xa, 0x40000000, 0x61);
    }
    if (actionIndex == 0x202) {
        TextAPI::setMACRO0(0xa, 0x40000000, 0x62);
    }
}

THUMB void status::UseActionMacro::setResultMacro(CharacterStatus* actor, CharacterStatus* target, int actionIndex)
{
    int damage = 0;
    if (target) {
        damage = target->haveStatusInfo_.effectValue_;
    }
    TextAPI::setMACRO0(0x12, 0x50000000, target->haveStatusInfo_.haveStatus_.playerIndex_);
    switch (UseAction::getDamageType(actionIndex)) {
        case UseAction::DamageTypeRecovery:
            TextAPI::setMACRO0(0x4d, 0xf0000000, damage);
            break;
        case UseAction::DamageTypeAddMp:
        case UseAction::DamageTypeSubMp:
            TextAPI::setMACRO0(0x51, 0xf0000000, damage);
            break;
        case UseAction::DamageTypeDefenceChange:
            TextAPI::setMACRO0(0x52, 0xf0000000, damage);
            break;
    }
    if (actionIndex == 0x47 && actor && actor->characterType_ == PLAYER) {
        TextAPI::setMACRO0(0xa, 0x40000000, actor->haveStatusInfo_.haveEquipment_.getEquipment(ITEM_WEAPON));
    }
    if ((actionIndex == 0x6a || actionIndex == 0x154) && target) {
        TextAPI::setMACRO0(0xd, 0x60000000, g_monster.getMonsterCallIndex());
    }
    if (actor->haveStatusInfo_.isWeaponAddDamage()) {
        actor->haveStatusInfo_.setWeaponAddDamage(false);
        TextAPI::setMACRO0(0xa, 0x40000000, actor->haveStatusInfo_.haveEquipment_.getEquipment(ITEM_WEAPON));
    }
    if (actionIndex == 0x16a) {
        TextAPI::setMACRO0(2, 0x50000000, ((MonsterStatus*)target)->mosyasIndex_);
    }
    if (actionIndex == 0x27 || actionIndex == 0x28 || actionIndex == 0x29 || actionIndex == 0x2a || actionIndex == 0x44 || actionIndex == 0x1e1) {
        TextAPI::setMACRO0(0x52, 0xf0000000, damage);
    }
    if (actionIndex == 0xa7 || actionIndex == 0x162) {
        TextAPI::setMACRO0(0x17, 0xa0000000, 7);
        TextAPI::setMACRO0(0x52, 0xf0000000, damage);
    }
    if (actionIndex == 0xb9 || actionIndex == 0x163) {
        TextAPI::setMACRO0(0x17, 0xa0000000, 8);
        TextAPI::setMACRO0(0x52, 0xf0000000, damage);
    }
    if (actionIndex == 0xba || actionIndex == 0x164) {
        TextAPI::setMACRO0(0x17, 0xa0000000, 0xa);
        TextAPI::setMACRO0(0x52, 0xf0000000, damage);
    }
    if (actionIndex == 0xbb || actionIndex == 0x165) {
        TextAPI::setMACRO0(0x17, 0xa0000000, 9);
        TextAPI::setMACRO0(0x52, 0xf0000000, damage);
    }
    if (actionIndex == 0xbc || actionIndex == 0x166) {
        TextAPI::setMACRO0(0x17, 0xa0000000, 0xe);
        TextAPI::setMACRO0(0x4d, 0xf0000000, damage);
    }
    if (actionIndex == 0xa8 || actionIndex == 0x167) {
        TextAPI::setMACRO0(0x17, 0xa0000000, 0xf);
        TextAPI::setMACRO0(0x51, 0xf0000000, damage);
    }
    if (actionIndex == 0xd4 && data_0210bb94.unkfunc_02058114(0xc) == 1) {
        TextAPI::setMACRO0(0x3d, 0xf0000000, TownFurnitureManager::getSingleton()->searchFloorItem());
    }
    if (actionIndex == 0xd1) {
        char x = TownMenuPlayerControl::getSingleton()->takanomeX_;
        char y = TownMenuPlayerControl::getSingleton()->takanomeY_;
        if (x <= 0) {
            TextAPI::setMACRO0(0x59, 0xf0000000, HaveEquipment::getAbsoluteValue(x));
        } else {
            TextAPI::setMACRO0(0x5a, 0xf0000000, HaveEquipment::getAbsoluteValue(x));
        }
        if (y >= 0) {
            TextAPI::setMACRO0(0x5b, 0xf0000000, HaveEquipment::getAbsoluteValue(y));
        } else {
            TextAPI::setMACRO0(0x5c, 0xf0000000, HaveEquipment::getAbsoluteValue(y));
        }
    }
    if (actionIndex == 0x1cb) {
        TextAPI::setMACRO0(0xd, 0x60000000, BaseAction::callMonster_[0]);
    }
    if (actionIndex == 0x1db) {
        TextAPI::setMACRO0(0xd, 0x60000000, getParupunteMetalSlimeBeforeIndex());
    }
    if (actionIndex == 0x1dd) {
        TextAPI::setMACRO0(0xd, 0x60000000, BaseAction::callMonster_[0]);
    }
    if (actionIndex == 0x210) {
        TextAPI::setMACRO0(0xd, 0x60000000, BaseAction::callMonster_[1]);
    }
    if (actionIndex == 0x211) {
        TextAPI::setMACRO0(0xd, 0x60000000, BaseAction::callMonster_[1]);
    }
    if (actionIndex == 0x2b) {
        TextAPI::setMACRO0(0xd, 0x60000000, g_monster.getMonsterCallIndex());
    }
}

THUMB void status::UseActionMacro::setAddMacro(CharacterStatus* actor, CharacterStatus* target, int actionIndex)
{
    if (actionIndex == 0x47 && actor->haveStatusInfo_.haveEquipment_.isEquipment(0x1d)) {
        TextAPI::setMACRO0(0x51, 0xf0000000, actor->haveStatusInfo_.effectValue_);
    }
    if (target) {
        if (target->haveStatusInfo_.isAddEffectMahotora()) {
            TextAPI::setMACRO0(0x51, 0xf0000000, target->haveStatusInfo_.addDamage_);
        }
        if (target->haveStatusInfo_.isAddMahotoraExecute()) {
            TextAPI::setMACRO0(0x51, 0xf0000000, target->haveStatusInfo_.addDamage_);
        }
    }
    if (actionIndex == 0x201) {
        TextAPI::setMACRO0(0xa, 0x40000000, 0x61);
    }
    if (actionIndex == 0x202) {
        TextAPI::setMACRO0(0xa, 0x40000000, 0x62);
    }
}

THUMB void status::UseActionMacro::setStatusChangeMacro(CharacterStatus* actor)
{
    StatusChange* statusChange = &actor->haveStatusInfo_.statusChange_;
    int actionIndex = statusChange->getActionIndex((StatusChange::Status)statusChange->isRelease());
    TextAPI::setMACRO0(0x19, 0x70000000, actionIndex);
    if (actionIndex == 0x159) {
        TextAPI::setMACRO0(0x19, 0x70000000, 0x23);
    }
    if (actionIndex == 0x15a) {
        TextAPI::setMACRO0(0x19, 0x70000000, 0x43);
    }
    if (actionIndex == 0x170) {
        TextAPI::setMACRO0(0x19, 0x70000000, 0x1e);
    }
    if (actionIndex == 0x224) {
        TextAPI::setMACRO0(0x19, 0x70000000, 0x1f);
    }
}
