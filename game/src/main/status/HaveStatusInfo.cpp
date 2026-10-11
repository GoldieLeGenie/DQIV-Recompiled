#include "main/status/HaveStatusInfo.hpp"
#include "main/status/ActionDefence.hpp"
#include "main/status/UseItem.hpp"


dss::Flag status::HaveStatusInfo::globalFlag_;

THUMB status::HaveStatusInfo::HaveStatusInfo()
{
    battleCommand_ = COMMAND_MEIREISASERO;
    noDamage_ = 0;
    defenceChange_ = 0;
    agilityChange_ = 0;
    flag_.flag_ = 0;
    flag2_.flag_ = 0;
    battleFlag_.flag_ = 0;
}

THUMB status::HaveStatusInfo::~HaveStatusInfo()
{
}


THUMB void status::HaveStatusInfo::setup(int index, int flag)
{
    const param::CharInitData *charaInitDataPtr;
    unsigned int monsterID;
    int maxLevel;

    this->index_ = index;

    charaInitDataPtr = status::excelParam.getCharaInitData();
    monsterID = charaInitDataPtr[index].monsterID;

    this->haveBattleStatus_ = 0;
    this->effectValue_ = 0;
    this->effectValueMax_ = 0;
    this->flag_.flag_ = 0;
    this->flag2_.flag_ = 0;

    if (flag == 1)
    {
        this->characterType_ = PLAYER;
        this->haveStatus_.setup(this->index_ & 0xffff, 1);
        this->haveEquipment_.setup(this->index_);
        this->haveItem_.setup(this->index_);
        this->haveAction_.setup(this->index_);

        if (this->index_ == 0x19)
        {
            this->haveAction_.clear();
            maxLevel = this->haveStatus_.level_;

            for (flag = 0; flag < maxLevel; flag++)
            {
                this->haveAction_.levelup(this->index_, flag + 1, this->haveStatus_.getWisdom(), 1);
            }
        }

        if (this->haveStatus_.isBattleNpc_ != 0)
        {
            this->haveStatus_.setupMonster(monsterID, 0);
            this->actionDefence_.setup(monsterID & 0xffff, 0);

            this->haveEquipment_.recalculateStats();
            this->haveStatus_.baseStatus_.strength_ -= this->haveEquipment_.attack_;

            this->haveEquipment_.recalculateStats();
            this->haveStatus_.baseStatus_.protection_ -= this->haveEquipment_.defence_;

            this->haveEquipment_.recalculateStats();

            unsigned int agilityMod = this->haveEquipment_.agility_;
            int agi = this->haveStatus_.getAgility();
            this->haveStatus_.baseStatus_.agility_ = (agi - agilityMod);
        }
        else
        {
            this->actionDefence_.setup(this->index_ & 0xffff, 1);
        }

        this->statusChange_.clear();
    }
    else if (flag == 0)
    {
        this->characterType_ = MONSTER;
        this->haveStatus_.setup(this->index_ & 0xffff, 0);
        this->actionDefence_.setup(this->index_ & 0xffff, 0);
        this->statusChange_.clear();
    }
}

THUMB void status::HaveStatusInfo::setupStatus(int index, bool flag)
{
    this->index_ = index;
    if (flag == 1)
    {
        this->characterType_ = PLAYER;

        this->haveStatus_.setup(this->index_, 1);

        this->actionDefence_.setup(this->index_, 1);
        return;
    }

    if (flag == 0)
    {

        this->characterType_ = MONSTER;

        this->haveStatus_.setup(this->index_, 0);
        this->actionDefence_.setup(this->index_, 0);
    }
}

THUMB void status::HaveStatusInfo::setupActionDefence(int index, int flag)
{
    if (flag == 1)
    {
        this->actionDefence_.setup(index, 1);
    }
    else if (flag == 0)
    {
        this->actionDefence_.setup(index, 0);
    }
}

THUMB void status::HaveStatusInfo::levelup(int level)
{
    int currentWisdom;
    if (level == 0)
    {
        this->haveStatus_.levelup(false);
        int level = this->haveStatus_.level_;
        currentWisdom = this->haveStatus_.getWisdom();

        this->haveAction_.levelup(this->index_, level, currentWisdom, true); // func_02011d74
        return;
    }

    this->haveStatus_.debugLevelup(level);
    this->haveAction_.clear();

    for (int i = 0; i < level; i++)
    {
        currentWisdom = this->haveStatus_.getWisdom();
        this->haveAction_.levelup(this->index_, i + 1, currentWisdom, true);
    }

    this->haveAction_.forceLearn();
    return;
}

THUMB unsigned short status::HaveStatusInfo::getStrength(int effect)
{
    if (effect & 1)
    {
        return this->haveStatus_.baseStatus_.strength_;
    }

    this->haveEquipment_.recalculateStats();
    unsigned short baseStrength = this->haveStatus_.baseStatus_.strength_;
    unsigned short equipStrength = this->haveEquipment_.strength_;
    unsigned short totalStrength = baseStrength + equipStrength;

    if (this->characterType_ == PLAYER)
    {
        unsigned char limited = BaseStatus::getClampValue2(totalStrength, 0, 0xFF);
        return limited;
    }

    return totalStrength;
}

THUMB void status::HaveStatusInfo::addStrength(int str)
{
    this->haveStatus_.addStrength(str);
}

THUMB void status::HaveStatusInfo::setAgility(unsigned char agi)
{
    this->haveStatus_.baseStatus_.agility_ = agi;
    return;
}

THUMB unsigned char status::HaveStatusInfo::getBaseAgility()
{
    unsigned char equipAgility;
    short agility;
    int equipment;
    unsigned short result;

    this->haveEquipment_.recalculateStats();
    this->haveStatus_.getAgility();
    equipment = this->haveEquipment_.getEquipment(ITEM_ARMOR);

    if ((equipment == 0x46) && (isPlayer(0x19) == 0))
    {
        result = 0;
    }
    else
    {
        equipment = this->haveEquipment_.getEquipment(ITEM_ACCESSORY);
        if (equipment == 0x5f)
        {
            this->haveEquipment_.recalculateStats();
            equipAgility = this->haveEquipment_.agility_;
            agility = this->haveStatus_.getAgility();
            result = equipAgility + agility * 2;
        }
        else
        {
            this->haveEquipment_.recalculateStats();
            equipAgility = this->haveEquipment_.agility_;
            agility = this->haveStatus_.getAgility();
            result = agility + equipAgility;
        }
    }

    return BaseStatus::getClampValue2(result, 0, 0xff);
}

THUMB unsigned char status::HaveStatusInfo::getAgility(int effect)
{
    unsigned short agility;

    if (effect & 1)
    {
        return this->haveStatus_.getAgility();
    }

    this->haveEquipment_.recalculateStats();
    this->haveStatus_.getAgility();

    int equipment = this->haveEquipment_.getEquipment(ITEM_ARMOR);
    if (equipment == 0x46 && isPlayer(0x19) == 0)
    {
        agility = 0;
    }
    else
    {
        int accessory = this->haveEquipment_.getEquipment(ITEM_ACCESSORY);
        if (accessory == 0x5f)
        {
            this->haveEquipment_.recalculateStats();
            unsigned char equipAgility = this->haveEquipment_.agility_;
            short agilityChange = this->getAgilityChange();
            short baseAgility = this->haveStatus_.getAgility();
            agility = agilityChange + (equipAgility + baseAgility * 2);
        }
        else
        {
            this->haveEquipment_.recalculateStats();
            unsigned char equipAgility = this->haveEquipment_.agility_;
            short agilityChange = this->getAgilityChange();
            short baseAgility = this->haveStatus_.getAgility();
            agility = baseAgility + equipAgility + agilityChange;
        }
    }

    return status::BaseStatus::getClampValue2(agility, 0, 255);
}

THUMB int status::HaveStatusInfo::setAgilityChange()
{
    short baseAgility = getBaseAgility();

    if (getAgility(0) == 0xff)
    {
        baseAgility = 0;
    }
    else if (0xff < baseAgility + getAgility(0))
    {

        baseAgility = (0xff - getAgility(0));
    }

    this->agilityChange_ += baseAgility;

    return baseAgility;
}

THUMB short status::HaveStatusInfo::getAgilityChange()
{
    status::StatusChange *sc = &this->statusChange_;
    if (sc->isEnable(status::StatusChange::StatusAgilityChange))
    {
        return this->agilityChange_;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::clearAgilityChange()
{
    this->agilityChange_ = 0;
}

THUMB void status::HaveStatusInfo::setProtection(unsigned char pro)
{
    this->haveStatus_.baseStatus_.protection_ = pro;
}

THUMB unsigned int status::HaveStatusInfo::getProtection(int effect)
{
    if (effect & 1)
    {
        return this->haveStatus_.baseStatus_.protection_;
    }

    unsigned short protection = this->haveStatus_.baseStatus_.protection_;
    if (this->characterType_ == PLAYER)
    {
        protection = BaseStatus::getClampValue2(protection, 0, 0xFF);
    }
    return protection;
}

THUMB void status::HaveStatusInfo::addProtection(int pro)
{
    this->haveStatus_.addProtection(pro);
}

THUMB unsigned int status::HaveStatusInfo::getWisdom(int effect)
{
    if (effect & 1)
    {
        return this->haveStatus_.getWisdom();
    }

    this->haveEquipment_.recalculateStats();
    unsigned short baseWisdom = this->haveEquipment_.wisdom_;
    unsigned short equipWisdom = this->haveStatus_.getWisdom();
    unsigned short total = equipWisdom + baseWisdom;

    return BaseStatus::getClampValue2(total, 0, 0xff) & 0xff;
}

THUMB unsigned char status::HaveStatusInfo::getLuck(int effect)
{
    if (effect & 1)
    {
        return this->haveStatus_.getLuck();
    }

    this->haveEquipment_.recalculateStats();
    unsigned char luck = this->haveEquipment_.luck_;
    unsigned char currentLuck = this->haveStatus_.getLuck();

    unsigned short total = currentLuck + luck;
    return BaseStatus::getClampValue2(total, 0, 0xFF);
}

THUMB int status::HaveStatusInfo::getExp()
{
    return haveStatus_.exp_;
}

THUMB void status::HaveStatusInfo::addExp(int exp)
{
    this->haveStatus_.addExp(exp);
}

THUMB int status::HaveStatusInfo::getLevelupExp()
{
    return this->haveStatus_.getLevelupExp();
}

THUMB int status::HaveStatusInfo::getGold()
{
    return this->haveStatus_.gold_;
}

THUMB void status::HaveStatusInfo::setHp(unsigned short hp)
{
    if (!this->noDamage_)
    {
        this->haveStatus_.setHp(hp);
        execDeath();
    }
    return;
}

THUMB int status::HaveStatusInfo::getHp()
{
    short hp;
    hp = haveStatus_.getHp();
    return hp;
}

THUMB void status::HaveStatusInfo::addBaseHp(int hp)
{
    if (this->noDamage_ == 0)
    {
        this->haveStatus_.addBaseHp(hp);

        if (getHp() == 0)
        {
            if (this->haveEquipment_.isEquipment(0x62))
            {
                setMegazaruRing(true);
            }

            if (this->haveEquipment_.isEquipment(0x61))
            {
                setMeganteRing(true);
            }
        }

        execDeath();
    }
}

THUMB void status::HaveStatusInfo::execDeath()
{
    int hp;

    hp = getHp();

    if (hp == 0)
    {
        this->statusChange_.release(StatusChange::StatusSleep);
        this->statusChange_.release(StatusChange::StatusConfusion);
        this->statusChange_.release(StatusChange::StatusMahokanta);
        this->statusChange_.release(StatusChange::StatusMahosute);

        if (this->haveBattleStatus_ != 0)
        {
            this->haveBattleStatus_->changeMonsterReverse();
        }

        this->statusChange_.clear();
    }
}

THUMB void status::HaveStatusInfo::setHpMax(unsigned short hp)
{
    this->haveStatus_.baseStatus_.setHpMax(hp);
}

THUMB unsigned short status::HaveStatusInfo::getHpMax()
{
    if (this->characterType_ == PLAYER)
    {
        unsigned short hpmax = this->haveStatus_.getHpMax();
        return BaseStatus::getClampValue2(hpmax, 0, 0x3E7);
    }
    return this->haveStatus_.getHpMax();
}

THUMB void status::HaveStatusInfo::addHpMax(unsigned short hp)
{
    unsigned int uVar1;

    this->haveStatus_.addHpMax(hp);
    if ((this->characterType_ == PLAYER) && (this->haveStatus_.getHpMax() > 999))
    {
        this->haveStatus_.baseStatus_.setHpMax(999);
    }
    return;
}

THUMB void status::HaveStatusInfo::setMp(unsigned short mp)
{
    if (!this->noDamage_)
        this->haveStatus_.setMp(mp);
}

THUMB int status::HaveStatusInfo::getMp()
{
    return this->haveStatus_.getMp();
}

THUMB void status::HaveStatusInfo::addMp(int mp)
{
    if (!this->noDamage_)
    {
        this->haveStatus_.addBaseMp(mp);
        return;
    }
    return;
}

THUMB void status::HaveStatusInfo::setMpMax(unsigned short mp)
{
    this->haveStatus_.baseStatus_.setMpMax(mp);
}

THUMB unsigned short status::HaveStatusInfo::getMpMax()
{
    if (this->characterType_ == PLAYER)
    {
        unsigned short mpmax = this->haveStatus_.getMpMax();
        return BaseStatus::getClampValue2(mpmax, 0, 999);
    }
    return this->haveStatus_.getMpMax();
}

THUMB void status::HaveStatusInfo::clearHpInBattle()
{
    int currentHP;

    currentHP = getHp();
    this->hp_[0] = currentHP;
    currentHP = getHp();
    this->hp_[1] = currentHP;
    currentHP = getHp();
    this->hp_[2] = currentHP;
    currentHP = getHp();
    this->hp_[3] = currentHP;
    return;
}

THUMB void status::HaveStatusInfo::addHpInBattle(DiffStatus status, int hp)
{
    int currentHp;

    switch (status)
    {
    case BeforeAction:
        currentHp = getHp();
        this->hp_[0] = currentHp;
        currentHp = getHp();
        this->hp_[1] = currentHp;
        currentHp = getHp();
        this->hp_[2] = currentHp;
        currentHp = getHp();
        this->hp_[3] = currentHp;
        return;

    case ExecuteAction:
        addBaseHp(hp);
        currentHp = getHp();
        this->hp_[1] = currentHp;
        currentHp = getHp();
        this->hp_[2] = currentHp;
        currentHp = getHp();
        this->hp_[3] = currentHp;
        return;

    case ResultAction:
        addBaseHp(hp);
        currentHp = getHp();
        this->hp_[2] = currentHp;
        currentHp = getHp();
        this->hp_[3] = currentHp;
        return;

    case SpecialAction:
        addBaseHp(hp);
        currentHp = getHp();
        this->hp_[3] = currentHp;
        return;
    }
}

THUMB int status::HaveStatusInfo::getHpInBattle(DiffStatus status)
{
    return this->hp_[status];
}

THUMB bool status::HaveStatusInfo::isNearDeathInBattle(int index)
{
    if (hp_[index] == 0)
    {
        return false;
    }

    return hp_[index] <= (getHpMax() / 4 - 1);
}

THUMB bool status::HaveStatusInfo::isDeathInBattle(DiffStatus status)
{
    if (!this->hp_[status])
    {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::clearMpInBattle()
{
    this->mp_[0] = 0;
    this->mp_[1] = 0;
    this->mp_[2] = 0;
    this->mp_[3] = 0;
    addMpInBattle(BeforeAction, 0);
    addMpInBattle(ExecuteAction, 0);
    addMpInBattle(ResultAction, 0);
    addMpInBattle(SpecialAction, 0);
    return;
}

THUMB void status::HaveStatusInfo::addMpInBattle(DiffStatus status, int mp)
{
    int currentMp;

    switch (status)
    {
    case BeforeAction:
        currentMp = getMp();
        this->mp_[0] = currentMp;
        return;

    case ExecuteAction:
        addMp(mp);
        currentMp = getMp();
        this->mp_[1] = currentMp;
        return;

    case ResultAction:
        addMp(mp);
        currentMp = getMp();
        this->mp_[2] = currentMp;
        return;

    case SpecialAction:
        addMp(mp);
        currentMp = getMp();
        this->mp_[3] = currentMp;
        return;
    }
}

THUMB int status::HaveStatusInfo::getMpInBattle(DiffStatus status)
{
    return this->mp_[status];
}

THUMB void status::HaveStatusInfo::clearStatusChangeInBattle()
{
    this->status_[0].flag_ = 0;
    this->status_[1].flag_ = 0;
    this->status_[2].flag_ = 0;
    this->status_[3].flag_ = 0;
    setStatusChangeInBattle(BeforeAction);
    setStatusChangeInBattle(ExecuteAction);
    setStatusChangeInBattle(ResultAction);
    setStatusChangeInBattle(SpecialAction);
    return;
}

THUMB void status::HaveStatusInfo::setStatusChangeInBattle(DiffStatus status)
{
    unsigned int &flag = this->status_[status].flag_;

    flag &= ~1;
    if (this->statusChange_.isEnable(StatusChange::StatusManusa))
    {
        flag |= 1;
    }
    // Bit 1 : StatusMahoton
    flag &= ~2;
    if (this->statusChange_.isEnable(StatusChange::StatusMahoton))
    {
        flag |= 2;
    }
    // Bit 2 : StatusSleep
    flag &= ~4;
    if (this->statusChange_.isEnable(StatusChange::StatusSleep))
    {
        flag |= 4;
    }
    // Bit 3 : StatusConfusion
    flag &= ~8;
    if (this->statusChange_.isEnable(StatusChange::StatusConfusion))
    {
        flag |= 8;
    }
    // Bit 4 : StatusSpazz
    flag &= ~0x10;
    if (this->statusChange_.isEnable(StatusChange::StatusSpazz))
    {
        flag |= 0x10;
    }
    // Bit 5 : StatusPoison
    flag &= ~0x20;
    if (this->statusChange_.isEnable(StatusChange::StatusPoison))
    {
        flag |= 0x20;
    }
}

THUMB bool status::HaveStatusInfo::isStatusChangeInBattle(DiffStatus status, MenuStatusChange statusChange)
{
    unsigned int mask = (1 << statusChange);
    int flag = this->status_[status].flag_;
    bool hasStatus = (flag & mask) != 0;

    if (hasStatus && statusChange == Mahoton)
    {
        hasStatus = (haveStatus_.getMpMax() != 0);
    }

    return hasStatus;
}

THUMB void status::HaveStatusInfo::setAttack(unsigned short atk)
{
    this->haveStatus_.baseStatus_.strength_ = atk;
    return;
}

THUMB unsigned short status::HaveStatusInfo::getAttack(int effect)
{
    int attackChange;
    unsigned short equipAttack;
    int temp;
    unsigned short result;

    if (effect & 1)
    {
        return this->haveStatus_.baseStatus_.strength_;
    }

    this->haveEquipment_.recalculateStats();
    equipAttack = this->haveEquipment_.attack_;
    attackChange = getAttackChange();
    temp = getStrength(0);
    result = attackChange + (temp + equipAttack);
    return BaseStatus::getClampValue2(result, 0, 0x270F);
}

THUMB unsigned int status::HaveStatusInfo::getBaseAttack()
{
    int equipAttack;
    int strength;

    this->haveEquipment_.recalculateStats();
    equipAttack = this->haveEquipment_.attack_;
    strength = getStrength(0);
    return BaseStatus::getClampValue2((strength + equipAttack), 0, 9999);
}

THUMB unsigned int status::HaveStatusInfo::setAttackChange()
{
    short baseAttack;
    int attackChange;
    int currentAttack;

    baseAttack = getBaseAttack();
    attackChange = baseAttack;
    currentAttack = getAttack(0);

    if (currentAttack == 9999)
    {
        attackChange = 0;
    }
    else
    {
        currentAttack = getAttack(0);
        if (9999 < attackChange + currentAttack)
        {
            currentAttack = getAttack(0);
            attackChange = (9999 - currentAttack) * 0x10000 >> 0x10;
        }
    }

    if (this->attackChange_ == 0)
    {
        this->attackChange_ = this->attackChange_ + attackChange;
    }

    return attackChange;
}

THUMB short status::HaveStatusInfo::getAttackChange()

{
    return 0;
}

THUMB unsigned short status::HaveStatusInfo::getBaseDefence()
{
    int weaponIndex;
    unsigned short equipDefence;

    weaponIndex = this->haveEquipment_.getEquipment(ITEM_WEAPON);
    if ((weaponIndex == 0x1f) && (weaponIndex = isPlayer(0x19), weaponIndex == 0))
    {
        weaponIndex = 0;
    }
    else
    {
        this->haveEquipment_.recalculateStats();
        equipDefence = this->haveEquipment_.defence_;
        weaponIndex = this->getProtection(0);
        weaponIndex = (short)(weaponIndex + (unsigned int)equipDefence);
    }
    return getClampValue(weaponIndex, 0, 999);
}

THUMB unsigned int status::HaveStatusInfo::getClampValue(int value, int min, int max)
{
    int clamped = getMin(value, min);
    return getMax(clamped, max);
}

THUMB unsigned int status::HaveStatusInfo::getMin(int a, int b)
{
    return (a > b) ? a : b;
}
THUMB unsigned int status::HaveStatusInfo::getMax(int a, int b)
{
    return (a < b) ? a : b;
}

THUMB void status::HaveStatusInfo::setDefence(unsigned short def)
{
    this->haveStatus_.baseStatus_.protection_ = def;
}

THUMB unsigned short status::HaveStatusInfo::getDefence(int effect)
{
    short defence;
    int defenceChange;
    unsigned short equipDefence;
    unsigned int prot;

    if (effect & 1)
    {
        return this->haveStatus_.baseStatus_.protection_;
    }

    if ((this->haveEquipment_.getEquipment(ITEM_WEAPON) == 0x1F) && (isPlayer(0x19) == 0))
    {
        defence = 0;
    }
    else
    {
        this->haveEquipment_.recalculateStats();
        equipDefence = this->haveEquipment_.defence_;
        defenceChange = getDefenceChange();
        prot = getProtection(0);
        defence = defenceChange + (prot + equipDefence);
    }

    return getClampValue(defence, 0, 0x3E7);
}

THUMB short status::HaveStatusInfo::setDefenceChange(int actionIndex)
{
    short delta;
    short baseDefence;

    delta = 0;
    baseDefence = getBaseDefence();

    switch (actionIndex)
    {
    case 0x25: // -100%
        delta = -baseDefence;
        break;
    case 0x26: // -50%
    case 0x152:
    case 0x21E:
        delta = -baseDefence / 2;
        break;
    case 0x27: // +150%
    case 0x28:
    case 0x29:
    case 0x1BA:
    case 0x1DF:
        delta = (baseDefence * 3) / 2;
        break;
    case 0x2A: // +100%
        delta = baseDefence;
        break;
    default:
        break;
    }

    if (delta > 0)
    {
        if (this->getDefence(0) == 999)
            delta = 0;
        else if ((delta + getDefence(0)) > 999)
            delta = (999 - getDefence(0));
    }
    if (delta < 0)
    {
        if (getDefence(0) == 0)
            delta = 0;
        else if ((delta + getDefence(0)) < 0)
            delta = (-getDefence(0));
    }

    this->defenceChange_ += delta;
    return delta;
}

THUMB short status::HaveStatusInfo::getDefenceChange()
{
    status::StatusChange *sc = &this->statusChange_;
    if (sc->isEnable(status::StatusChange::StatusDefenceChange))
    {
        return this->defenceChange_;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::clearDefenceChange()
{
    this->defenceChange_ = 0;
}

THUMB unsigned char status::HaveStatusInfo::getChangeStrength(unsigned int itemIndex)
{
    status::HaveEquipment havequipment = this->haveEquipment_;

    havequipment.setEquipment(itemIndex);
    havequipment.recalculateStats();
    unsigned int result = BaseStatus::getClampValue2(
        this->haveStatus_.baseStatus_.strength_ + (unsigned short)havequipment.strength_,
        0, 0xFF);
    return result;
}

unsigned char status::HaveStatusInfo::getChangeAgility(int itemIndex)
{
    status::HaveEquipment havequipment = this->haveEquipment_;

    havequipment.setEquipment(itemIndex);
    havequipment.recalculateStats();
    this->haveStatus_.getAgility();
    unsigned short result;
    unsigned char equipArmor = havequipment.getEquipment(ITEM_ARMOR);
    if ((equipArmor == 0x46) && (equipArmor = isPlayer(0x19), equipArmor == 0))
    {
        result = 0;
    }
    else
    {
        unsigned char equipAccessory = havequipment.getEquipment(ITEM_ACCESSORY);
        if (equipAccessory == 0x5f)
        {
            havequipment.recalculateStats();
            unsigned short equipAgility = havequipment.agility_;
            result = (equipAgility + this->haveStatus_.getAgility() * 2);
        }
        else
        {
            havequipment.recalculateStats();
            unsigned short equipAgility = havequipment.agility_;
            unsigned short baseAgility = this->haveStatus_.getAgility();
            result = (baseAgility + equipAgility);
        }
    }
    unsigned int clampedResult = BaseStatus::getClampValue2(result, 0, 0xFF);
    return clampedResult;
}

THUMB unsigned char status::HaveStatusInfo::getChangeWisdom(unsigned int itemIndex)
{
        
    status::HaveEquipment havequipment = this->haveEquipment_;
    havequipment.setEquipment(itemIndex);
    havequipment.recalculateStats();
    unsigned char equipWisdom = havequipment.wisdom_;
    short baseWisdom = this->haveStatus_.getWisdom();
    unsigned int result = BaseStatus::getClampValue2(
        (baseWisdom + equipWisdom), 0, 0xFF);
    return result;
}

THUMB unsigned char status::HaveStatusInfo::getChangeLuck(unsigned int itemIndex)
{
    status::HaveEquipment havequipment = this->haveEquipment_;
    havequipment.setEquipment(itemIndex);
    havequipment.recalculateStats();
    unsigned char equipLuck = havequipment.luck_;
    short baseLuck = this->haveStatus_.getLuck();
    unsigned int result = BaseStatus::getClampValue2(
        (baseLuck + equipLuck), 0, 0xFF);
    return result;
}

THUMB short status::HaveStatusInfo::getChangeAttack(unsigned int itemIndex)
{
    status::HaveEquipment havequipment = this->haveEquipment_;
    havequipment.setEquipment(itemIndex);
    havequipment.recalculateStats();

    unsigned char equipStrength = havequipment.strength_;
    int baseStrength = this->haveStatus_.baseStatus_.strength_;

    havequipment.recalculateStats();

    unsigned short equipAttack = havequipment.attack_;

    int total = baseStrength + equipStrength + equipAttack;
    unsigned int result = dss::clamp<int>(total, 0, 9999);
    return result;
}

THUMB short status::HaveStatusInfo::getChangeDefence(unsigned int itemIndex)
{
    status::HaveEquipment havequipment = this->haveEquipment_;
    havequipment.setEquipment(itemIndex);
    havequipment.recalculateStats();

    unsigned int equipDefence = this->haveStatus_.baseStatus_.protection_ + havequipment.defence_;
    unsigned int weaponId = this->haveEquipment_.getEquipment(ITEM_WEAPON);
    if (weaponId == 0x1f)
    {
        if (isPlayer(0x19) == 0)
        {
            equipDefence = 0;
        }
    }

    unsigned int result = dss::clamp<int>(equipDefence, 0, 999);
    return result;
}

THUMB int status::HaveStatusInfo::setNearDeath()
{
    int hp = this->getHp();
    if (hp == 0)
    {
        return false;
    }
    int currentHp = this->getHp();
    unsigned int maxHp = this->getHpMax();
    if (currentHp <= (int)((maxHp >> 2) - 1))
    {
        return true;
    }
    return false;
}

THUMB bool status::HaveStatusInfo::isDeath()
{
    if (this->getHp() == 0)
    {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setDeath()
{
    setHp(0);
    return;
}

THUMB void status::HaveStatusInfo::rebirth()
{
    unsigned short MaxHp;

    MaxHp = this->getHpMax();
    setHp(MaxHp);
    return;
}

THUMB bool status::HaveStatusInfo::isPoison()
{
    status::StatusChange *p_statusChange;
    p_statusChange = &this->statusChange_;
    if (p_statusChange->isEnable(status::StatusChange::StatusPoison))
    {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setPoison()
{
    this->statusChange_.setup2(status::StatusChange::StatusPoison, false);
    return;
}

THUMB void status::HaveStatusInfo::detoxPoison()
{
    if (this->isPoison())
    {
        this->statusChange_.release(status::StatusChange::StatusPoison);
    }
}

THUMB bool status::HaveStatusInfo::isSpell()
{
    if (this->haveEquipment_.hasSpellEffect() != 0)
    {
        return true;
    }
    return false;
}

THUMB void status::HaveStatusInfo::setEquipment(int index)
{
    unsigned int itemIndex = this->haveItem_.getItem(index);
    unsigned int itemType = UseItem::getItemType(itemIndex);
    unsigned int equipped = 0;

    switch (itemType)
    {
    case ITEM_WEAPON:
        equipped = this->haveEquipment_.getEquipment(ITEM_WEAPON);
        this->haveEquipment_.removeItem(equipped);
        break;
    case ITEM_ARMOR:
        equipped = this->haveEquipment_.getEquipment(ITEM_ARMOR);
        this->haveEquipment_.removeItem(equipped);
        break;
    case ITEM_SHIELD:
        equipped = this->haveEquipment_.getEquipment(ITEM_SHIELD);
        this->haveEquipment_.removeItem(equipped);
        break;
    case ITEM_HELMET:
        equipped = this->haveEquipment_.getEquipment(ITEM_HELMET);
        this->haveEquipment_.removeItem(equipped);
        break;
    case ITEM_ACCESSORY:
        equipped = this->haveEquipment_.getEquipment(ITEM_ACCESSORY);
        this->haveEquipment_.removeItem(equipped);
        break;
    }

    this->haveItem_.resetEquipmentWithItemIndex(equipped);
    this->haveItem_.sort(index);

    switch (itemType)
    {
    case ITEM_WEAPON:
        this->haveEquipment_.setEquipment(itemIndex);
        return;
    case ITEM_ARMOR:
        this->haveEquipment_.setEquipment(itemIndex);
        return;
    case ITEM_SHIELD:
        this->haveEquipment_.setEquipment(itemIndex);
        return;
    case ITEM_HELMET:
        this->haveEquipment_.setEquipment(itemIndex);
        return;
    case ITEM_ACCESSORY:
        this->haveEquipment_.setEquipment(itemIndex);
        break;
    }
}

THUMB void status::HaveStatusInfo::resetEquipment1(int index)
{
    unsigned int item = this->haveItem_.getItem(index);
    this->haveEquipment_.removeItem(item);
    this->haveItem_.sortEquipmentForTest(index);
    return;
}

THUMB void status::HaveStatusInfo::resetEquipment2(ItemType itemType)
{
    unsigned int item = this->haveEquipment_.getEquipment(itemType);
    this->haveEquipment_.removeItem(item);
    this->haveItem_.resetEquipmentWithItemIndex(item);
    return;
}

THUMB void status::HaveStatusInfo::clearAllItem()
{
    this->haveItem_.clear();
    this->haveEquipment_.clear();
    return;
}

THUMB int status::HaveStatusInfo::setItemEquipment(int itemId)
{
    if (itemId == 0)
    {
        return -1;
    }
    int index = this->haveItem_.add(itemId);
    this->setEquipment(index);
    return index;
}

THUMB bool status::HaveStatusInfo::isEquipEnable(int itemIndex)
{
    int result = UseItem::isEquipEnable(
        this->haveStatus_.job_,
        itemIndex, this->haveStatus_.sex_);
    if (result != 0)
    {
        return true;
    }
    return false;
}

THUMB void status::HaveStatusInfo::execThrow(int index)
{
    unsigned int item = 0;
    if (this->haveItem_.isEquipment(index) != 0)
    {
        item = this->haveItem_.getItem(index);
    }
    this->haveItem_.del(index);
    this->haveEquipment_.removeItem(item);
}

THUMB void status::HaveStatusInfo::setCondition(Condition con)
{
    switch (con)
    {
    case ALIVE:
    {
        unsigned short hp = this->haveStatus_.getHpMax();
        this->haveStatus_.setHp(hp);
        return;
    }
    case DEAD:
        this->haveStatus_.setHp(0);
        return;
    case SLEEP:
    case CONFUSION:
    case SPAZZ:
    case SPELL:
        break;
    case POISON:
        this->statusChange_.setup2(StatusChange::StatusPoison, 0);
        break;
    }
}

THUMB status::HaveStatusInfo::Condition status::HaveStatusInfo::getCondition()
{
    if (isDeath())
    {
        return DEAD;
    }
    if (this->statusChange_.isEnable(status::StatusChange::StatusPoison))
    {
        return POISON;
    }
    if (this->statusChange_.isEnable(status::StatusChange::StatusSpazz))
    {
        return SPAZZ;
    }
    return ALIVE;
}

THUMB bool status::HaveStatusInfo::isPlayer(int playerIndex)
{
    return this->haveStatus_.isPlayer_ && playerIndex == this->haveStatus_.playerIndex_;
}

THUMB void status::HaveStatusInfo::recovery()
{
    int Hp;
    unsigned short HpMpMax;
    Hp = this->haveStatus_.getHp();
    if (Hp != 0)
    {
        HpMpMax = this->haveStatus_.getHpMax();
        this->haveStatus_.setHp(HpMpMax);
    }
    HpMpMax = this->haveStatus_.getMpMax();
    this->haveStatus_.setMp(HpMpMax);
    this->statusChange_.clear();
    return;
}

THUMB void status::HaveStatusInfo::revival()
{
    unsigned short HpMax;
    unsigned short MpMax;

    HpMax = this->haveStatus_.getHpMax();
    this->haveStatus_.setHp(HpMax);
    MpMax = this->haveStatus_.getMpMax();
    this->haveStatus_.setMp(MpMax);
    return;
}

THUMB void status::HaveStatusInfo::setUseActionEffectValue(int value)
{
    this->effectValue_ = value;
    if (isDamage() != 0)
    {
        if (this->effectValueMax_ < this->effectValue_)
        {
            this->effectValueMax_ = this->effectValue_;
        }
    }
}

THUMB void status::HaveStatusInfo::setUseActionEffectValueMulti(int index, int value)
{
    this->effectValueMulti_[index] = value;
    return;
}

THUMB void status::HaveStatusInfo::setSpecialTargetDamage(int value)
{
    this->specialTargetDamage_ = value;
}

THUMB int status::HaveStatusInfo::getSpecialTargetDamage()
{
    return this->specialTargetDamage_;
}

THUMB void status::HaveStatusInfo::setFubahaFlag(bool flag)
{
    if (flag)
    {
        this->flag_.flag_ |= 2;
    }
}

THUMB bool status::HaveStatusInfo::isFubahaFlag()

{
    if (2 & this->flag_.flag_)
    {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setBossDeathFlag(bool flag)
{
    if (flag)
    {
        this->flag_.flag_ |= 4;
    }
}

THUMB bool status::HaveStatusInfo::isBossDeathFlag()
{
    if ((this->flag_.flag_ & 4) != 0)
    {
        return true;
    }
    return false;
}

THUMB void status::HaveStatusInfo::setEscapeFlag(bool flag)
{
    if (flag)
    {
        this->flag_.flag_ |= 0x10;
    }
    else
    {
        this->flag_.flag_ &= ~0x10;
    }
    return;
}

THUMB bool status::HaveStatusInfo::isEscapeFlag()
{
    if (0x10 & this->flag_.flag_)
    {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setDisappearFlag(bool flag)
{
    if (flag)
    {
        this->flag_.flag_ |= 0x20;
    }
    else
    {
        this->flag_.flag_ &= ~0x20;
    }
}

THUMB bool status::HaveStatusInfo::isDisappearFlag()

{
    if (0x20 & this->flag_.flag_)
    {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setMosyasExec(bool flag)
{
    if (flag)
    {
        this->flag_.flag_ |= 0x40;
    }
    else
    {
        this->flag_.flag_ &= ~0x40;
    }
}

THUMB bool status::HaveStatusInfo::isMosyasExec()
{
    if (0x40 & this->flag_.flag_)
    {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setMosyasRelease(bool flag)
{
    if (flag)
    {
        this->flag_.flag_ |= 0x800;
    }
    else
    {
        this->flag_.flag_ &= ~0x800;
    }
}

THUMB bool status::HaveStatusInfo::isMosyasRelease()
{
    if (0x800 & this->flag_.flag_)
    {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setEvilPriest(bool flag)
{
    if (flag)
    {
        this->flag_.flag_ |= 0x80;
    }
    else
    {
        this->flag_.flag_ &= ~0x80;
    }
}

THUMB void status::HaveStatusInfo::setSleepAttack(bool flag)
{
    if (flag)
    {
        this->flag_.flag_ |= 0x200;
    }
    else
    {
        this->flag_.flag_ &= ~0x200;
    }
}

THUMB bool status::HaveStatusInfo::isSleepAttack()

{
    if (0x200 & this->flag_.flag_)
    {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setMultiAttack(bool flag)
{
    if (flag)
    {
        this->flag_.flag_ |= 0x400;
    }
    else
    {
        this->flag_.flag_ &= ~0x400;
    }
}

THUMB void status::HaveStatusInfo::setCallFriend(bool flag)
{
    if (flag)
    {
        this->flag_.flag_ |= 0x1000;
    }
    else
    {
        this->flag_.flag_ &= 0xffffefff;
    }
}

THUMB bool status::HaveStatusInfo::isCallFriend()
{
    if (0x1000 & this->flag_.flag_)
    {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setSilverTarot(bool flag)
{
    if (flag)
    {
        this->flag_.flag_ |= 0x2000;
    }
    else
    {
        this->flag_.flag_ &= ~0x2000;
    }
}

THUMB bool status::HaveStatusInfo::isSilverTarot()
{
    if (0x2000 & this->flag_.flag_)
    {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setWeaponAddDamage(bool flag)
{
    if (flag)
    {
        this->flag_.flag_ |= 0x4000;
    }
    else
    {
        this->flag_.flag_ &= ~0x4000;
    }
}

THUMB bool status::HaveStatusInfo::isWeaponAddDamage()
{
    if ((this->flag_.flag_ & 0x4000) != 0)
    {
        return true;
    }
    return false;
}

THUMB void status::HaveStatusInfo::setConfuseMissAttack(bool flag)
{
    if (flag != 0)
    {
        this->flag_.flag_ |= 0x8000;
    }
    else
    {
        this->flag_.flag_ &= 0xffff7fff;
    }
}

THUMB bool status::HaveStatusInfo::isConfuseMissAttack()
{
    if (0x8000 & this->flag_.flag_)
    {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setAddEffectSleep(bool flag)
{
    if (flag)
    {
        this->flag_.flag_ |= 0x20000;
    }
    else
    {
        this->flag_.flag_ &= 0xfffdffff;
    }
}

THUMB bool status::HaveStatusInfo::isAddEffectSleep()
{
    if (0x20000 & this->flag_.flag_) {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setAddEffectSpazz(bool flag)

{
    if (flag) {
        this->flag_.flag_ |= 0x40000;
    }
    else {
        this->flag_.flag_ &= 0xfffbffff;
    }
}

THUMB void status::HaveStatusInfo::setAddEffectPoison(bool flag)
{
    if (flag) {
        this->flag_.flag_ |= 0x80000;
    }
    else {
        this->flag_.flag_ &= 0xfff7ffff;
    }
}

THUMB void status::HaveStatusInfo::setAddEffectRecovery(bool flag)
{
    if (flag) {
        this->flag_.flag_ |= 0x100000;
    }
    else {
        this->flag_.flag_ &= 0xffefffff;
    }
}

THUMB bool status::HaveStatusInfo::isAddEffectRecovery(){
    if (0x100000 & this->flag_.flag_) {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setAddEffectMahotora(bool flag)
{
    if (flag) {
        this->flag_.flag_ |= 0x200000;
    }
    else {
        this->flag_.flag_ &= 0xffdfffff;
    }
}

THUMB bool status::HaveStatusInfo::isAddEffectMahotora()
{
    if ((this->flag_.flag_ & 0x200000) != 0) {
        return true;
    }
    return false;
}


THUMB void status::HaveStatusInfo::setAddEffectDamage(bool flag) {
    if (flag) {
        this->flag_.flag_ |= 0x400000;
    }
    else {
        this->flag_.flag_ &= 0xFFBFFFFF;
    }
}

THUMB bool status::HaveStatusInfo::isAddEffectDamage() {
    if (this->flag_.flag_ & 0x400000) {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setTargetJouk(bool flag)
{
    if (flag) {
        this->flag_.flag_ |= 0x800000;
    } else {
        this->flag_.flag_ &= ~0x800000;
    }
}

THUMB bool status::HaveStatusInfo::isTargetJouk()
{
    if (this->flag_.flag_ & 0x800000 ){
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setMosyasTarget(bool flag)
{
    if (flag) {
        this->flag_.flag_ |= 1;
    } else {
        this->flag_.flag_ &= ~1;
    }
}

THUMB bool status::HaveStatusInfo::isMosyasTarget() {
    if (this->flag_.flag_ & 1) {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setKillMyself(bool flag)
{
    if (flag) {
        this->flag_.flag_ |= 0x10000;
    } else {
        this->flag_.flag_ &= ~0x10000;
    }
}

THUMB bool status::HaveStatusInfo::isKillMyself()
{
    if (this->flag_.flag_ & 0x10000) {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setMonsterChange(bool flag)
{
    if (flag) {
        this->flag_.flag_ |= 0x1000000;
    } else {
        this->flag_.flag_ &= ~0x1000000;
    }
}

THUMB bool status::HaveStatusInfo::isMonsterChange()
{
    if (this->flag_.flag_ & 0x1000000) {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setBaikirutoDisable(bool flag)
{
    if (flag) {
        this->flag_.flag_ |= 0x2000000;
    }
    else {
        this->flag_.flag_ &= 0xfdffffff;
    }
}

THUMB void status::HaveStatusInfo::setFirstMosyas(bool flag)
{
    if (flag) {
        this->flag_.flag_ |= 0x4000000;
    } else {
        this->flag_.flag_ &= ~0x4000000;
    }
}

THUMB bool status::HaveStatusInfo::isFirstMosyas()
{
    if (this->flag_.flag_ & 0x4000000) {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setMahokantaCounter(bool flag)
{
    if (flag) {
        this->flag_.flag_ |= 0x8000000;
    } else {
        this->flag_.flag_ &= ~0x8000000;
    }
}

THUMB bool status::HaveStatusInfo::isMahokantaCounter()
{
    if (this->flag_.flag_ & 0x8000000) {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setCounterDamage(bool flag)
{
    if (flag) {
        this->flag_.flag_ |= 0x10000000;
    }
    else {
        this->flag_.flag_ &= 0xefffffff;
    }
}

THUMB void status::HaveStatusInfo::setDamage(bool flag)

{
    if (flag) {
        this->flag_.flag_ |= 0x20000000;
    }
    else {
        this->flag_.flag_ &= 0xdfffffff;
    }
}

THUMB bool status::HaveStatusInfo::isDamage() {
    if (this->flag_.flag_ & 0x20000000) {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setLastActor(bool flag)
{
    if (flag) {
        this->flag_.flag_ |= 0x40000000;
    } else {
        this->flag_.flag_ &= ~0x40000000;
    }
}

THUMB void status::HaveStatusInfo::setAddMahotoraExecute(bool flag)
{
    if (flag) {
        this->flag_.flag_ |= 0x80000000;
        return;
    }
    else {
        this->flag_.flag_ &= 0x7fffffff;
    }
    return;
}

THUMB bool status::HaveStatusInfo::isAddMahotoraExecute() {
    if (this->flag_.flag_ & 0x80000000) {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setDamageMyself(bool flag)
{
    if (flag) {
        this->flag_.flag_ |= 8;
    }
    else {
        this->flag_.flag_ &= 0xfffffff7;
    }
}

THUMB bool status::HaveStatusInfo::isDamageMyself()
{
    if (this->flag_.flag_ & 8){
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setSelfImmolation(bool flag)
{
    if (flag) {
        this->flag2_.flag_ |= 1;
    } else {
        this->flag2_.flag_ &= ~1;
    }
}

THUMB bool status::HaveStatusInfo::isSelfImmolation()
{
    if (this->flag2_.flag_ & 1) {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setFirstKaishin(bool flag)
{
    if (flag) {
        this->flag2_.flag_ |= 2;
    } else {
        this->flag2_.flag_ &= ~2;
    }
}

THUMB bool status::HaveStatusInfo::isFirstKaishin()
{
    if (this->flag2_.flag_ & 2) {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setSecondKaishin(bool flag)
{
    if (flag) {
        this->flag2_.flag_ |= 4;
    } else {
        this->flag2_.flag_ &= ~4;
    }
}

THUMB bool status::HaveStatusInfo::isSecondKaishin()
{
    if (this->flag2_.flag_ & 4) {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setStatusChangeRelease(bool flag)
{
    if (flag) {
        this->flag2_.flag_ |= 8;
    } else {
        this->flag2_.flag_ &= ~8;
    }
}

THUMB bool status::HaveStatusInfo::isStatusChangeRelease()
{
    if (8 & this->flag2_.flag_) {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setAstoron(bool flag)
{
    if (flag) {
        this->flag2_.flag_ |= 0x10;
        return;
    }
    else {
        this->flag2_.flag_ &= 0xffffffef;
    }
    return;
}

THUMB bool status::HaveStatusInfo::isAstoron() {
    if (this->flag2_.flag_ & 0x10) {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setMpFailure(bool flag)
{
    if (flag) {
        this->flag2_.flag_ |= 0x20;
    } else {
        this->flag2_.flag_ &= ~0x20;
    }
}

THUMB bool status::HaveStatusInfo::isMpFailure()
{
    if (0x20 & this->flag2_.flag_) {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setMahotoneFailure(bool flag)
{
    if (flag) {
        this->flag2_.flag_ |= 0x40;
    } else {
        this->flag2_.flag_ &= ~0x40;
    }
}

THUMB void status::HaveStatusInfo::setMegazaruRebirth(bool flag)
{
    if (flag) {
        this->flag2_.flag_ |= 0x80;
    } else {
        this->flag2_.flag_ &= ~0x80;
    }
}

THUMB bool status::HaveStatusInfo::isMegazaruRebirth()
{
    if ((this->flag2_.flag_ & 0x80) != 0) {
        return true;
    }
    return false;
}

THUMB void status::HaveStatusInfo::setMegazaruRecovery(bool flag)
{
    if (flag) {
        this->flag2_.flag_ |= 0x100;
    } else {
        this->flag2_.flag_ &= ~0x100;
    }
}

THUMB void status::HaveStatusInfo::setZaorikuRebirth(bool flag)
{
    if (flag) {
        this->flag2_.flag_ |= 0x200;
    } else {
        this->flag2_.flag_ &= ~0x200;
    }
}

THUMB void status::HaveStatusInfo::setImmidiateDeath(bool flag)
{
    if (flag) {
        this->flag2_.flag_ |= 0x400;
    } else {
        this->flag2_.flag_ &= ~0x400;
    }
}

THUMB bool status::HaveStatusInfo::isImmidiateDeath()
{
    if (this->flag2_.flag_ & 0x400) {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setMultiFirstDeath(bool flag)
{
    if (flag) {
        this->flag2_.flag_ |= 0x800;
    } else {
        this->flag2_.flag_ &= ~0x800;
    }
}


THUMB void status::HaveStatusInfo::setExecuteMeganteRing(bool flag)
{
    if (flag) {
        this->flag2_.flag_ |= 0x1000;
    } else {
        this->flag2_.flag_ &= ~0x1000;
    }
}

THUMB bool status::HaveStatusInfo::isExecuteMeganteRing()
{
    if (this->flag2_.flag_ & 0x1000) {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setMeganteRing(bool flag)
{
    if (flag) {
        this->flag2_.flag_ |= 0x2000;
    } else {
        this->flag2_.flag_ &= ~0x2000;
    }
}


THUMB bool status::HaveStatusInfo::isMeganteRing()
{
    if (0x2000 & this->flag2_.flag_) {
        return 1;
    }
    return 0;
}


THUMB void status::HaveStatusInfo::setMegazaruRing(bool flag)
{
    if (flag) {
        this->flag2_.flag_ |= 0x4000;
    } else {
        this->flag2_.flag_ &= ~0x4000;
    }
}

THUMB bool status::HaveStatusInfo::isMegazaruRing()
{
    if (this->flag2_.flag_ & 0x4000) {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setDisableTextureCache(bool flag)
{
    if (flag) {
        this->flag2_.flag_ |= 0x8000;
    } else {
        this->flag2_.flag_ &= ~0x8000;
    }
}

THUMB bool status::HaveStatusInfo::isDisableTextureCache()
{
    if (this->flag2_.flag_ & 0x8000) {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::setAllKaishin(bool flag) {
    if (flag) {
        this->battleFlag_.flag_ |= 1;
        return;
    }
    else {
        this->battleFlag_.flag_ &= 0xfffffffe;
    }
}

THUMB bool status::HaveStatusInfo::isAllKaishin()
{
    return this->battleFlag_.flag_ & 1;
}

THUMB void status::HaveStatusInfo::setGlbMeganteRing(bool flag)
{
    if (flag) {
        globalFlag_.flag_ |= 1;
    } else {
        globalFlag_.flag_ &= ~1;
    }
}

THUMB bool status::HaveStatusInfo::isGlbMeganteRing() {
    return (globalFlag_.flag_ & 1) != 0;
}

THUMB void status::HaveStatusInfo::setGlbMegazaruRing(bool flag)
{
    if (flag) {
        globalFlag_.flag_ |= 2;
    } else {
        globalFlag_.flag_ &= ~2;
    }
}

THUMB bool status::HaveStatusInfo::isGlbMegazaruRing()
{
    if (globalFlag_.flag_ & 2) {
        return 1;
    }
    return 0;
}

THUMB void status::HaveStatusInfo::releaseMosyas()
{
    int hp;
    int maxMp;

    hp = this->getHp();
    maxMp = this->getMp();
    this->haveStatus_.setup(this->index_, 0);
    this->setHp(hp);
    this->setMp(maxMp);
    return;
}

THUMB void status::HaveStatusInfo::execStartOfBattle()
{
    this->flag_.flag_ = 0;
    this->flag2_.flag_ = 0;
    this->clearHpInBattle();
    this->clearMpInBattle();
    this->clearStatusChangeInBattle();

    if (this->haveEquipment_.isEquipment(0x5A)) {
        this->statusChange_.setup2(StatusChange::StatusConfusion, 0);
    }

    this->battleFlag_.flag_ = 0;
}

THUMB int status::HaveStatusInfo::isActionEnable() {
    status::StatusChange* p_statusChange;

    if (this->isDeath() != 0) {
        return 0;
    }

    p_statusChange = &this->statusChange_;
    if (p_statusChange->isEnable(status::StatusChange::StatusSleep) != 0) {
        return 0;
    }

    p_statusChange = &this->statusChange_;
    if (p_statusChange->isEnable(status::StatusChange::StatusSpazz) != 0) {
        return 0;
    }

    p_statusChange = &this->statusChange_;
    if (p_statusChange->isEnable(status::StatusChange::StatusAstoron) != 0) {
        return 0;
    }

    p_statusChange = &this->statusChange_;
    if (p_statusChange->isEnable(status::StatusChange::StatusPath1) != 0) {
        return 0;
    }

    p_statusChange = &this->statusChange_;
    if (p_statusChange->isEnable(status::StatusChange::StatusDragoram) == 0) {
        return 1;
    }
    return 0;
}

THUMB int status::HaveStatusInfo::isAttackEnable()
{
    status::StatusChange* p_statusChange;

    if (isDeath() != 0) {
        return 0;
    }

    p_statusChange = &this->statusChange_;
    if (p_statusChange->isEnable(status::StatusChange::StatusSleep) != 0) {
        return 0;
    }
    p_statusChange = &this->statusChange_;
    if (p_statusChange->isEnable(status::StatusChange::StatusSpazz) != 0) {
        return 0;
    }
    p_statusChange = &this->statusChange_;

    if (p_statusChange->isEnable(status::StatusChange::StatusAstoron) != 0) {
        return 0;
    }

    p_statusChange = &this->statusChange_;
    if (p_statusChange->isEnable(status::StatusChange::StatusPath1) == 0) {
        return 1;
    }

    return 0;
}