#include "main/status/HaveBattleStatus.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/HaveStatusInfo.hpp"
#include "main/status/UseItem.hpp"
#include "main/status/HaveStatus.hpp"
#include "main/status/UseAction.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/PartyStatus.hpp"
#ifdef DQIV_NATIVE
#include <cstring>
#endif

const ConfuseArray3         confusePlayer7          = { { 0x1ab, 0x1ac, 0x1ad } };                                                                         
const ConfuseArray4         confusePlayer9          = { { 0x1c0, 0x1c1, 0x1c2, 0x1c3 } }; 
const TorunekoActionTable   torunekoParupunte       = { { 0x212, 0x213, 0x214, 0x215, 0x216, 0x217, 0x218, 0x219, 0x21a, 0x21b } };                        
const ConfuseArray4         confusePlayer4          = { { 0x1ae, 0x1af, 0x1b0, 0x1b1 } };                                                                                                                                                                                      // 0x020b4d8c
const ConfuseArray6         confuseOtherParupunte   = { { 0x1c4, 0x1c5, 0x1c6, 0x1c7, 0x1a3, 0x1a7 } };                                                    
const ConfuseArray6         confuseOtherNormal      = { { 0x1c4, 0x1c5, 0x1c6, 0x1c7, 0x1c8, 0x1a7 } };                                                    
const RestArray             restOneTable            = { { 1, 1, 1, 1, 1, 1 } };                                                                            
const ConfuseArray3         confusePlayer3          = { { 0x1a7, 0x1a8, 0x1a9 } };                                                                        
const RestArray             restNumTable            = { { 1, 1, 1, 1, 1, 1 } };                                                                            
const RoopCopy              roopPattern             = { { 8, 4, 3, 1 } };         
const ParupunteArrayLarge   parupunteLargeTable     = { { 0x1cb, 0x1cc, 0x1cd, 0x1ce, 0x1cf, 0x1d0, 0x1d1, 0x1d2, 0x1d5, 0x1d6, 0x1d7, 0x1d8, 0x1d9, 0x1da, 0x1dc, 0x1dd, 0x1de, 0x1df, 0x1e0, 0x1e1, 0x1e3, 0x1e4, 0x1e5 } };  
const ConfuseArray4         confusePlayer6          = { { 0x1b8, 0x1b9, 0x1ba, 0x1bb } };
const TorunekoActionTable   torunekoNormal          = { { 0x212, 0x213, 0x214, 0x215, 0x216, 0x217, 0x218, 0x219, 0x21a, 0x21b } };                        
const ConfuseArray4         confusePlayer8          = { { 0x1bc, 0x1bd, 0x1be, 0x1bf } }; 
const ConfuseArray4         confusePlayer5          = { { 0x1b2, 0x1b3, 0x1b4, 0x1b7 } };  
const ConfuseArray8         confuseMonsterNormal    = { { 0x1c4, 0x1c5, 0x1c6, 0x1c7, 0x1a7, 0x1c8, 0x1c9, 0x1ca } };                                      
const ConfuseArray3         confusePlayer12         = { { 0x1a4, 0x1a5, 0x1a6 } };                                                                         
const ParupunteArraySmall   parupunteSmallTable     = { { 0x1cc, 0x1cd, 0x1d6, 0x1d7, 0x1d9, 0x1da, 0x1dc, 0x1de, 0x1df, 0x1e0, 0x1e1, 0x1e3, 0x1e4 } };   
const TarotTable            tarotActionTable        = { { 0x21e, 0x220, 0x221, 0x222, 0x223, 0x224, 0x225, 0x226 } };                                      
const ConfuseArray8         confuseMonsterParupunte = { { 0x1c4, 0x1c5, 0x1c6, 0x1c7, 0x1a7, 0x1a3, 0x1a3, 0x1a3 } };                                      


unsigned char actionPattern[120] = {
    0x2B,0x00,0x00,0x00,0x2A,0x00,0x00,0x00,
    0x2B,0x00,0x00,0x00,0x2B,0x00,0x00,0x00,
    0x2A,0x00,0x00,0x00,0x2B,0x00,0x00,0x00,
    0x44,0x00,0x00,0x00,0x3A,0x00,0x00,0x00,
    0x30,0x00,0x00,0x00,0x26,0x00,0x00,0x00,
    0x1B,0x00,0x00,0x00,0x11,0x00,0x00,0x00,
    0xC8,0x00,0x00,0x00,0x28,0x00,0x00,0x00,
    0x08,0x00,0x00,0x00,0x04,0x00,0x00,0x00,
    0x03,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
    0x46,0x00,0x00,0x00,0x46,0x00,0x00,0x00,
    0x46,0x00,0x00,0x00,0x10,0x00,0x00,0x00,
    0x0F,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,
    0x46,0x00,0x00,0x00,0x46,0x00,0x00,0x00,
    0x46,0x00,0x00,0x00,0x10,0x00,0x00,0x00,
    0x0F,0x00,0x00,0x00,0x0F,0x00,0x00,0x00
};


param::MonsterData* status::HaveBattleStatus::monsterData_;
param::CharInitData* status::HaveBattleStatus::charInitData_;
int status::HaveBattleStatus::tarotDebugIndex_;
int status::HaveBattleStatus::parupunteDebugIndex_;
int status::HaveBattleStatus::confusionDebugIndex_;
int status::HaveBattleStatus::torunekoDebugIndex_;
int status::HaveBattleStatus::eventFlag_;
status::PlayerFlag status::HaveBattleStatus::groupControl_[4];
int status::HaveBattleStatus::groupRoopIndex_[4];

THUMB status::HaveBattleStatus::HaveBattleStatus()
{
    
    this->originalIndex_ = 0;
    this->originalType_ = PLAYER;
    this->haveStatusInfo_ = 0;
    this->patternFailedFlag_.flag_ = 0;
    this->disablePattern_.flag_ = 0;
    this->disablePattern2nd_.flag_ = 0;
    this->selectCommand_ = NoSelect;
}

THUMB status::HaveBattleStatus::~HaveBattleStatus()
{

}

THUMB void status::HaveBattleStatus::initialize() {
    charInitData_ = status::excelParam.getCharaInitData();
    monsterData_ = status::excelParam.monsterData_;
}


THUMB void status::HaveBattleStatus::setup(CharacterType type, int group, int index)
{
    this->groupIndex_ = group;
    this->index_ = index;
    this->escape_ = 0;
    this->actionIndex_ = 0;
    this->actionCount_ = 0;
    this->patternFailedFlag_.flag_ = 0;
    this->disablePattern_.flag_ = 0;
    this->disablePattern2nd_.flag_ = 0;
    this->selectCommand_ = NoSelect;
    this->type_ = type;
    this->change_ = 0;
    this->tarotCount_ = 0;
    this->mosyasActionCount_ = 0;

    for (int i = 0; i < 17; i++) {
        this->mosyasAction_[i] = 0x47; 
    }

    this->monsterIndexForNpc_ = charInitData_[index].monsterID;

    if ((this->monsterIndexForNpc_ != 0) && (this->type_ == PLAYER)) {
        this->index_ = this->monsterIndexForNpc_;
        this->type_ = MONSTER;
    }

    if (this->type_ == PLAYER) {
        setupPlayer();
    }

    if (this->type_ == MONSTER) {
        setupMonster();
    }
}

THUMB void status::HaveBattleStatus::newBaseChangeMonster(int index)
{

    this->multiCount2_ = 0;
    groupControl_[this->groupIndex_].flag_ = 0;

    this->index_ = index;

    if (this->type_ == PLAYER) {
        this->haveStatusInfo_->setupStatus(this->index_, true);
        setupMonster();
    }
    else if (this->type_ == MONSTER) {
        this->haveStatusInfo_->setupStatus(index, false);
        setupMonster();
    }
}

THUMB void status::HaveBattleStatus::newBaseChangeMonsterFromPlayer(int index)
{
    if (this->change_ == 0) {
        this->originalIndex_ = this->index_; 
        this->originalType_ = this->type_;  
    }
    this->index_ = index; 
    this->type_ = MONSTER;
    
    if (this->type_ == 0) { 
        this->haveStatusInfo_->setupActionDefence(this->index_ , 1);
    } else if (this->type_ == 1) { 
        this->haveStatusInfo_->setupActionDefence(index, 0);
        setupMonster();
    }
    this->change_ = 1; 
}

THUMB void status::HaveBattleStatus::newBaseChangeMonsterReverse2()
{
    this->change_ = false;
    this->index_ = this->originalIndex_;
    this->type_ = this->originalType_;

    if (this->type_ == PLAYER) {
        this->haveStatusInfo_->setupActionDefence(this->index_ , 1);
    }
    else if (this->type_ == MONSTER) {
        this->haveStatusInfo_->setupActionDefence(this->index_ , 0);
        setupMonster();
    }
}



THUMB void status::HaveBattleStatus::newBaseChangeMonsterWithHpMp(int index)
{
    int hp;
    int mp;
    hp = this->haveStatusInfo_->getHp();
    mp = this->haveStatusInfo_->getMp();
    newBaseChangeMonster(index);
    this->haveStatusInfo_->setHp(hp);
    this->haveStatusInfo_->setMp(mp);
    return;
}


THUMB void status::HaveBattleStatus::changeMonsterFromPlayer(int index)
{
    int hp = this->haveStatusInfo_->getHp();
    unsigned int mp = haveStatusInfo_->getMp();

    int diffAttack = 0x82 - this->haveStatusInfo_->getAttack(0);
    int diffDefence = 100 - this->haveStatusInfo_->getDefence(0);

    this->strength_ = haveStatusInfo_->haveStatus_.baseStatus_.strength_;
    this->haveStatusInfo_->addStrength(diffAttack);

    this->protection_ = haveStatusInfo_->haveStatus_.baseStatus_.protection_;
    this->haveStatusInfo_->addProtection(diffDefence);

    this->agility_ = haveStatusInfo_->haveStatus_.getAgility();
    this->haveStatusInfo_->setAgility(0x37);

    newBaseChangeMonsterFromPlayer(index);

    this->haveStatusInfo_->setHp(hp);
    haveStatusInfo_->setMp(mp);
}


THUMB void status::HaveBattleStatus::changeMonsterReverse()
{
    if (this->change_) {
        newBaseChangeMonsterReverse2();
        if (this->strength_ != 0) {
            (this->haveStatusInfo_->haveStatus_).baseStatus_.strength_ = this->strength_;
        }
        if (this->protection_ != 0) {
            (this->haveStatusInfo_->haveStatus_).baseStatus_.protection_ = this->protection_ & 0xff;
        }
        if (this->agility_ != 0) {
            (this->haveStatusInfo_->haveStatus_).baseStatus_.agility_ = this->agility_;
        }
    }
    return;
}

THUMB void status::HaveBattleStatus::setupPlayer()
{
	this->brains_ = 0;
	this->dragon_ = 0;
	this->metal_ = 0;
	this->zombi_ = 0;
	this->air_ = 0;
	this->slime_ = 0;
	this->jouk_ = 1;
	return;
}

THUMB void status::HaveBattleStatus::setupMonster() {    
    this->brains_ = monsterData_[this->index_].integer;        
    this->multi_ = (char)monsterData_[this->index_].times;           
    this->multiCount_ = 0;                              
    this->multiCount2_ = 0;
    
    this->group_[0] = monsterData_[this->index_].byte_8 & 7;              
    this->group_[1] = ((monsterData_[this->index_].byte_8 & 0x38) >> 3);
    this->group_[2] = monsterData_[this->index_].byte_9 & 7;              
    this->group_[3] = ((monsterData_[this->index_].byte_9 & 0x38) >> 3);
    this->group_[4] = monsterData_[this->index_].byte_10 & 7;             
    this->group_[5] = ((monsterData_[this->index_].byte_10 & 0x38) >> 3);
    
    this->crossFire_ = monsterData_[this->index_].focus;       
    this->crossFireTarget_ = -1;                        
    this->level_ = monsterData_[this->index_].level;          
    
    this->dragon_ = ((monsterData_[this->index_].byte_4 & 0xC0) >> 6);    
    this->metal_ = monsterData_[this->index_].byte_5 & 3;                 
    this->zombi_ = ((monsterData_[this->index_].byte_5 & 0x0C) >> 2);
    this->air_ = ((monsterData_[this->index_].byte_5 & 0x30) >> 4);
    this->slime_ = ((monsterData_[this->index_].byte_5 & 0xC0) >> 6);
    
    this->jouk_ = monsterData_[this->index_].avoid;            
}

THUMB bool status::HaveBattleStatus::isJouk()
{
    int value;

    switch (this->jouk_) {
    case 0:
        return false;

    case 1:
        value = dssrand::rand(0x40);
        if (value > 0) return false;
        return true;

    case 2:
        value = dssrand::rand(0x40);
        if (value > 1) return false;
        return true;

    case 3:
        value = dssrand::rand(0x40);
        if (value > 2) return false;
        return true;

    case 4:
        value = dssrand::rand(0x40);
        if (value > 3) return false;
        return true;

    case 5:
        value = dssrand::rand(0x40);
        if (value > 4) return false;
        return true;

    case 6:
        value = dssrand::rand(0x40);
        if (value > 5) return false;
        return true;

    case 7:
        value = dssrand::rand(0x40);
        if (value > 6) return false;
        return true;

    default:
        return false;
    }
}

THUMB void status::HaveBattleStatus::setActionIndex(int index)

{
    this->actionIndex_ = index;
    return;
}



THUMB void status::HaveBattleStatus::setActionSelect(CallStart callStart)
{
    if (this->haveStatusInfo_) {
        if (this->type_ == PLAYER) {
            if (this->haveStatusInfo_->haveEquipment_.isEquipment(0x5a)) {
                this->haveStatusInfo_->statusChange_.setup2(status::StatusChange::StatusConfusion, false);
            }
            setActionSelectForPlayer();
        }
        else if (this->type_ == MONSTER) {
            setActionSelectForMonster(callStart);
        }

        if (this->actionIndex_ == 0x9b) {
            this->haveStatusInfo_->statusChange_.setup(this->actionIndex_, true);
        }
    }
}

THUMB void status::HaveBattleStatus::setActionSelectForPlayer()
{
    int item;

    switch (this->selectCommand_) {
    case Attack:
        this->actionIndex_ = 0x47;
        return;
    case Defence:
        this->actionIndex_ = 0x9b;
        return;
    case UseAction:
        this->actionIndex_ = this->selectIndex_;
        return;
    case UseItem:
        this->sortIndex_ = this->selectIndex_;
        item = this->haveStatusInfo_->haveItem_.getItem(this->selectIndex_);
        this->selectIndex_ = item;
        this->actionIndex_ = status::UseItem::getBattleUseAction(item);
        return;
    case NoSelect:
        this->actionIndex_ = 0;
        this->selectedTarget_ = 0;
        return;
    }
}




THUMB void status::HaveBattleStatus::setActionSelectForMonster(CallStart callStart)
{
    int savedPatternIndex;
    int i;

    if (this->brains_ == 0) {
        setActionPatternForMonster();
        setActionIndexForMonster();
    }

    if (this->brains_ == 1) {
        savedPatternIndex = this->patternIndex_;
        if (isActionFailed()) {
            for (i = 0; i < 6; i++) {
                this->patternIndex_ = i;
                setActionIndexForMonster();
                checkRestMP_Mahoton();
            }
        }
        if (callStart == StartTurn) {
            this->patternIndex_ = savedPatternIndex;
            if (!isActionDisable()) {
                setActionIndexForMonster();
                return;
            }
        }
        setActionPatternForMonster();
        setActionIndexForMonster();
    }

    if (this->brains_ == 2) {
        savedPatternIndex = this->patternIndex_;
        for (i = 0; i < 6; i++) {
            this->patternIndex_ = i;
            setActionIndexForMonster();
            checkRestMP_Mahoton();
        }
        if (callStart == StartTurn) {
            this->patternIndex_ = savedPatternIndex;
            if (!isActionDisable()) {
                setActionIndexForMonster();
                return;
            }
        }
        setActionPatternForMonster();
        setActionIndexForMonster();
    }

    if (this->brains_ == 3) {
        setActionPatternForMonster();
        setActionIndexForMonster();
    }

    checkGroupControl();

    this->multiGlobalCount_++;
    this->selectCommand_ = AISelect;

    if (this->index_ >= 250 && this->index_ <= 259) {
        if (this->mosyasActionCount_ != 0) {
            int randIndex = dssrand::rand(this->mosyasActionCount_);
            this->actionIndex_ = getMosyasAction(randIndex);
        }
    }
}


THUMB void status::HaveBattleStatus::setActionPatternForMonster() {
    RoopCopy local_70;
    RoopCopy tmpPattern;
    unsigned char pattern = monsterData_[this->index_].pattern;
    this->patternIndex_ = 0;
    int offset;
    volatile int* vp = &offset;
    *vp = pattern * 0x18;
    int* pActionCount = &this->actionCount_;
#ifdef DQIV_NATIVE
    std::memcpy(&tmpPattern, &roopPattern, sizeof(RoopCopy));
#else
    __memcpy(&tmpPattern, &roopPattern, sizeof(RoopCopy));
#endif
    
    for (;;) {
        switch (pattern) {
        case 0:
        case 1:
        case 2:
        case 4: {
            int r = dssrand::rand(0x100);
            this->patternIndex_ = dss::arrayToIndex((int*)(actionPattern + pattern * 0x18), r, 6);
            if (pattern == 2) {
                if ((this->disablePattern_.flag_ & 1) && (this->disablePattern_.flag_ & 2)) {
                    local_70 = tmpPattern;
                    this->patternIndex_ = dss::arrayToIndex(local_70.v, dssrand::rand(0x10), 4) + 2;
                }
            }
            break;
        }
        case 3:
            this->actionCount_ = getClampValue(this->actionCount_, 0, 5);
            this->patternIndex_ = this->actionCount_;
            (*pActionCount)++;
            break;
        case 5: {
            int old = *pActionCount;
            int val = this->actionCount_;
            *pActionCount = old + 1;
            this->patternIndex_ = val;
            this->actionCount_ = getClampValue(this->actionCount_, 0, 2);
            this->patternIndex_ = this->patternIndex_ * 2 + dssrand::rand(2);
            break;
        }
        case 6:
            if (this->actionCount_ % 2 == 0) {
                this->patternIndex_ = 0;
            } else {
                this->patternIndex_ = dssrand::rand(5) + 1;
            }
            (*pActionCount)++;
            break;
        case 7:
            this->patternIndex_ = groupRoopIndex_[this->groupIndex_];
            groupRoopIndex_[this->groupIndex_]++;
            groupRoopIndex_[this->groupIndex_] = getClampValue(groupRoopIndex_[this->groupIndex_], 0, 5);
            break;
        default:
            break;
        }

        if (getRestNum() == 0) {
            this->patternIndex_ = 7;
            return;
        }
        if (getGroupControl() == 0) {
            continue;
        }
        if (isActionAllDisable() != 0) {
            this->patternIndex_ = 7;
            return;
        }
        if (isActionAllDisable2nd() != 0) {
            this->patternIndex_ = 7;
            return;
        }
        if (isActionDisable() == 0 && isActionDisable2nd() == 0) {
            return;
        }

        setRestOne();
        if (isRestOneAction2nd() == 0) {
            continue;
        }
        if (getGroupControl() == 0) {
            continue;
        }
        if (isActionAllDisable() != 0) {
            this->patternIndex_ = 7;
            return;
        }
        if (isActionAllDisable2nd() != 0) {
            this->patternIndex_ = 7;
            return;
        }
        if (isActionDisable()!= 0) {
            continue;
        }
        if (isActionDisable2nd() != 0) {
            continue;
        }
        return;
    }
}


THUMB int status::HaveBattleStatus::getClampValue(int value, int min, int max) {
    if (value > max) {
        return min;
    }
    if (value < min) {
        return max;
    }
    return value;
}


THUMB void status::HaveBattleStatus::setActionIndexForMonster()
{
    switch (this->patternIndex_) {
    case 0:
        this->actionIndex_ = monsterData_[this->index_].action1;
        return;
    case 1:
        this->actionIndex_ = monsterData_[this->index_].action2;
        return;
    case 2:
        this->actionIndex_ = monsterData_[this->index_].action3;
        return;
    case 3:
        this->actionIndex_ = monsterData_[this->index_].action4;
        return;
    case 4:
        this->actionIndex_ = monsterData_[this->index_].action5;
        return;
    case 5:
        this->actionIndex_ = monsterData_[this->index_].action6;
        return;
    case 7:
        this->actionIndex_ = 0x1e9;
    }
    return;
}

THUMB int status::HaveBattleStatus::getActionIndex(int index) {
    int idx = this->index_;
    if (idx >= 0xFA && idx <= 0x103) {
        if (this->actionIndex_ == 0x47) {
            return 0;
        }
        return 1;
    }
    switch (this->patternIndex_) {
    case 0:
        return monsterData_[index].action1;
    case 1:
        return monsterData_[index].action2;
    case 2:
        return monsterData_[index].action3;
    case 3:
        return monsterData_[index].action4;
    case 4:
        return monsterData_[idx].action5;
    case 5:
        return monsterData_[idx].action6;
    default:
        return 0;
    }
}

THUMB int status::HaveBattleStatus::getActionAnimation()
{

    if (this->index_ >= 0xFA && this->index_ <= 0x103) {
        if (this->actionIndex_ == 0x47) {
            return 0;
        }
        return 1;
    }
    unsigned char result = 0;

    switch (this->patternIndex_) {
        case 0:
            result = monsterData_[this->index_].animation1;
            break;
        case 1:
            result = monsterData_[this->index_].animation2;
            break;
        case 2:
            result = monsterData_[this->index_].animation3;
            break;
        case 3:
            result = monsterData_[this->index_].animation4;
            break;
        case 4:
            result = monsterData_[this->index_].animation5;
            break;
        case 5:
            result = monsterData_[this->index_].animation6;
            break;
        default:
            break;
    }
    return result;
}

THUMB void status::HaveBattleStatus::setActionFailed(int index)
{
  index = this->patternIndex_;
  for (int i = 0; i < 6; i++) {
    this->patternIndex_ = i;
    this->setActionIndexForMonster();
    this->patternFailedFlag_.flag_ |= (1 << i);
  }
  this->patternIndex_ = index;
}

THUMB bool status::HaveBattleStatus::isActionFailed()
{
  if (this->patternFailedFlag_.flag_) {
    return 1;
  }
  return 0;
}

THUMB void status::HaveBattleStatus::setActionDisable(int index)
{
  if (index != 0) {
    this->disablePattern_.flag_ |= (1 << this->patternIndex_);
  }
  else {
    this->disablePattern_.flag_ &= ~(1 << this->patternIndex_);
  }
  return;
}

THUMB bool status::HaveBattleStatus::isActionDisable()
{
    return (this->disablePattern_.flag_ & (1 << this->patternIndex_)) != 0;
}

THUMB bool status::HaveBattleStatus::isActionAllDisable()
{
  if ((this->disablePattern_.flag_ & 0x3f) == 0x3f) {
    return 1;
  }
  return 0;
}


THUMB void status::HaveBattleStatus::setActionDisable2nd()
{
  this->disablePattern2nd_.flag_ = this->disablePattern2nd_.flag_ | 1 << this->patternIndex_;
  return;
}

THUMB bool status::HaveBattleStatus::isActionDisable2nd()
{
  return (this->disablePattern2nd_.flag_ & 1 << this->patternIndex_ ) != 0;
}

THUMB bool status::HaveBattleStatus::isActionAllDisable2nd()
{
  if ((this->disablePattern2nd_.flag_ & 0x3f) == 0x3f) {
    return 1;
  }
  return 0;
}

THUMB void status::HaveBattleStatus::clearActionDisable2nd()
{
    this->disablePattern2nd_.flag_ = 0;
    return;
}

THUMB int status::HaveBattleStatus::isRestOneAction2nd() {
    int count = 0;
    int last = -1;
    for (int i = 0; i < 6; i++) {
        if ((1 << i) & this->disablePattern2nd_.flag_) {
            count++;
        } else {
            last = i;
        }
    }
    if (count == 5) {
        this->patternIndex_ = last;
        return 1;
    }
    return 0;
}


THUMB void status::HaveBattleStatus::setRestOne() {
    RestArray tmp = restOneTable;
    unsigned int i = 0;
    int* p = tmp.v;
    do {
        int mask = 1 << i;
        if (this->disablePattern_.flag_ & mask) {
            *p = 0;
        }
        if (this->disablePattern2nd_.flag_ & mask) {
            *p = 0;
        }
        if (mask & groupControl_[this->groupIndex_].flag_) {
            *p = 0;
        }
        i++;
        p++;
    } while ((int)i < 6);
    int count = 0;
    int last = 0;
    int idx = 0;
    int* q = tmp.v;
    do {
        if (*q != 0) {
            count++;
            last = idx;
        }
        idx++;
        q++;
    } while (idx < 6);
    if (count == 1) {
        this->patternIndex_ = last;
    }
}


THUMB int status::HaveBattleStatus::getRestNum() {
    RestArray tmp = restNumTable;
    unsigned int i = 0;
    int* p = tmp.v;
    do {
        int mask = 1 << i;
        if (this->disablePattern_.flag_ & mask) {
            *p = 0;
        }
        if (this->disablePattern2nd_.flag_ & mask) {
            *p = 0;
        }
        if (mask & groupControl_[this->groupIndex_].flag_) {
            *p = 0;
        }
        i++;
        p++;
    } while ((int)i < 6);
    int count = 0;
    int idx = 0;
    int* q = tmp.v;
    do {
        if (*q != 0) {
            count++;
        }
        idx++;
        q++;
    } while (idx < 6);
    return count;
}



THUMB bool status::HaveBattleStatus::isUseMp()
{
    int useMp;

    if (!this->haveStatusInfo_) {
        return true;
    }
    useMp = status::UseAction::getUseMp(this->actionIndex_);
    if (useMp == 255) {
        if (this->haveStatusInfo_->getMp()) {
            return true;
        }
    }
    if (this->haveStatusInfo_->getMp() >= useMp) {
        return true;
    }
    return false;
}


THUMB int status::HaveBattleStatus::isMahoton()
{
    if (!this->haveStatusInfo_) {
        return 0;
    }
    if (!status::UseAction::isMahoton(this->actionIndex_)) {
        return 0;
    }
    return this->haveStatusInfo_->statusChange_.isEnable(status::StatusChange::StatusMahoton);
}

THUMB int status::HaveBattleStatus::isFizzleZone() {
    if (this->haveStatusInfo_ == 0) {
        return 0;
    }
    if (status::UseAction::isMahoton(this->actionIndex_)) {
        return this->haveStatusInfo_->statusChange_.isEnable(status::StatusChange::StatusFizzleZone);
    }
    return 0;
}

THUMB bool status::HaveBattleStatus::checkRestMP_Mahoton()
{
    if (isUseMp() == 0 || isMahoton() != 0 || isFizzleZone() != 0) {
        setActionDisable(1);
        return false;
    }
    setActionDisable(0);
    return true;
}


THUMB void status::HaveBattleStatus::clearGroupControl() {
    for (int i = 0; i < 4; i++) {
        groupControl_[i].flag_ = 0;
    }
}


THUMB void status::HaveBattleStatus::checkGroupControl()
{
    int patternIndex;

    patternIndex = this->patternIndex_;
    if (patternIndex <= 6 && this->group_[patternIndex] != 0) {
        groupControl_[this->groupIndex_].flag_ |= (1 << patternIndex);
    }
}


THUMB int status::HaveBattleStatus::getGroupControl() {
    int idx = this->patternIndex_;
    if (this->group_[idx] != 0) {
        int ctrl = groupControl_[this->groupIndex_].flag_;
        if (ctrl & (1 << idx)) {
            return 0;
        }
    }
    return 1;
}

THUMB void status::HaveBattleStatus::setSelectCommand(SelectCommand command, int index)
{
    this->selectCommand_ = command;
    this->selectIndex_ = index;
    return;
}

THUMB status::HaveBattleStatus::SelectCommand status::HaveBattleStatus::getSelectCommand()
{
    return this->selectCommand_;
}


THUMB void status::HaveBattleStatus::clearSelectCommand()
{
    this->selectCommand_ = NoSelect;
    this->selectedTarget_ = -1;
    this->selectedGroup_ = -1;
    return;
}

THUMB void status::HaveBattleStatus::startBattle()
{
    this->selectCommand_ = NoSelect;
    this->multiCount_ = 0;
    this->multiCount2_ = 0;
    this->multiGlobalCount_ = -1;

    for (int i = 0; i < 4; i++) {
        groupRoopIndex_[i] = 0;
    }

    this->tarotCount_ = 0;
}


THUMB void status::HaveBattleStatus::execStartOfRound()
{
    this->multiTurnCount_ = '\0';
    clearActionDisable2nd();
    this->multiExecCount_ = '\0';
    return;
}


THUMB void status::HaveBattleStatus::setMultiAction()
{
    this->multiCount2_++;
    return;
}

THUMB bool status::HaveBattleStatus::isMultiAction()
{
    int r;
    bool bVar2 = false;

    switch (this->multi_) {
    case 0:
        bVar2 = false;
        this->multiCount_ = 0;
        break;

    case 1:
        if (this->multiTurnCount_ == 0 && dssrand::rand(2) != 0) {
            bVar2 = true;
        }
        break;

    case 2:
        bVar2 = this->multiCount_ == 0;
        this->multiCount_++;
        if (this->multiCount_ > 1) {
            this->multiCount_ = 0;
        }
        break;

    case 3:
        if (this->multiCount_ < 2) {
            bVar2 = true;
        }
        break;

    case 4:
        r = this->multiCount2_ % 3;
        if (r == 0) goto trigger_on_zero;
        if (r == 1) goto skip_turn;
        if (r != 2) goto skip_turn;
        goto trigger_on_two;
    trigger_on_zero:
        bVar2 = true;
        goto skip_turn;
    trigger_on_two:
        bVar2 = true;
    skip_turn:
        break;

    case 6:
        r = this->multiCount2_ % 3;
        if (r == 0) goto increment_turn;
        if (r == 1) goto trigger_on_one;
        if (r != 2) goto increment_turn;
        goto no_trigger;
    trigger_on_one:
        bVar2 = true;
        goto increment_turn;
    no_trigger:
        bVar2 = false;
    increment_turn:
        break;

    default:
        break;
    }
    this->multiTurnCount_++;

    return bVar2;
}

THUMB void status::HaveBattleStatus::setupTarotAction()
{
    TarotTable table;

    this->tarotActionIndex_ = -1;

    if ((this->index_ == 8) && (this->actionIndex_ == 0x148)) {
        this->selectCommand_ = UseAction;

        table = tarotActionTable;

        this->actionIndex_ = table.v[dssrand::rand(8)];
        this->tarotCount_++;
        if ((this->tarotCount_ > 2) && (dssrand::rand(2) != 0)) {
            this->actionIndex_ = 0x21F;
        }

        if (tarotDebugIndex_ != 0) {
            this->actionIndex_ = tarotDebugIndex_;
            tarotDebugIndex_ = 0;
        }

        this->tarotActionIndex_ = this->actionIndex_;
        if (this->actionIndex_ == 0x225) {
            this->selectedGroup_ = 0;
        }
    }

    this->selectCommand_ = UseAction;
    this->selectedTarget_ = -1;
    this->selectedGroup_ = -1;
}



THUMB void status::HaveBattleStatus::setupParupunteAction() {
    if (this->haveStatusInfo_ != 0) {
        StatusChange* sc = &this->haveStatusInfo_->statusChange_;
        if (!sc->isEnable(StatusChange::StatusFizzleZone) && this->actionIndex_ == 0x42) {
            if (eventFlag_ != 0) {
                ParupunteArraySmall tmp = parupunteSmallTable;
                int r = dssrand::rand(0xd);
                this->actionIndex_ = tmp.v[r];
            } else {
                ParupunteArrayLarge tmp = parupunteLargeTable;
                int r = dssrand::rand(0x17);
                this->actionIndex_ = tmp.v[r];
            }
            if (parupunteDebugIndex_ != 0) {
                this->actionIndex_ = parupunteDebugIndex_;
                parupunteDebugIndex_ = 0;
            }
            this->selectCommand_ = UseAction;
            this->selectedTarget_ = -1;
            this->selectedGroup_ = -1;
        }
    }
}


THUMB void status::HaveBattleStatus::setupConfuseAction() {
    if (this->haveStatusInfo_ == 0) {
        return;
    }
    StatusChange* sc = &this->haveStatusInfo_->statusChange_;
    if (!sc->isEnable(StatusChange::StatusConfusion) &&
        !this->haveStatusInfo_->haveEquipment_.isEquipment(0x5a)) {
        return;
    }
    this->haveStatusInfo_->setConfuseMissAttack(false);
    if (this->type_ == PLAYER || this->monsterIndexForNpc_ != 0) {
        int r = dssrand::rand(2);
        if (r == 0) {
            this->actionIndex_ = 0x1a2;
            r = dssrand::rand(4);
            if (r == 0) {
                this->haveStatusInfo_->setConfuseMissAttack(true);
            }
        } else {
            r = dssrand::rand(4);
            if (r == 0) {
                this->actionIndex_ = 0x1a1;
            } else {
                int idx = this->index_;
                if (idx - 1U <= 1) {
                    ConfuseArray3 tmp = confusePlayer12;
                    r = dssrand::rand(3);
                    this->actionIndex_ = tmp.v[r];
                } else if (idx == 3) {
                    ConfuseArray3 tmp = confusePlayer3;
                    r = dssrand::rand(3);
                    int act = tmp.v[r];
                    this->actionIndex_ = act;
                    if (act == 0x1a9) {
                        g_Party.setBattleMode();
                        if (g_Party.getCount() == 1) {
                            this->actionIndex_ = 0x1a3;
                        }
                    }
                } else if (idx == 7) {
                    ConfuseArray3 tmp = confusePlayer7;
                    r = dssrand::rand(3);
                    int act = tmp.v[r];
                    this->actionIndex_ = act;
                    if (act == 0x1ab) {
                        if (this->haveStatusInfo_->haveItem_.getCount() == 0) {
                            this->actionIndex_ = 0x1a3;
                        }
                    }
                    if (this->actionIndex_ - 0x1acU <= 1) {
                        if (status::g_Party.gold_ == 0) {
                            this->actionIndex_ = 0x1a3;
                        }
                    }
                } else if (idx == 4) {
                    ConfuseArray4 tmp = confusePlayer4;
                    r = dssrand::rand(4);
                    this->actionIndex_ = tmp.v[r];
                } else if (idx == 5) {
                    ConfuseArray4 tmp = confusePlayer5;
                    r = dssrand::rand(4);
                    int act = tmp.v[r];
                    this->actionIndex_ = act;
                    if (act == 0x1b4) {
                        this->actionIndex_ = 0;
                        if (this->haveStatusInfo_->haveAction_.isAction(0x32)) {
                            this->actionIndex_ = 0x1b4;
                        }
                        if (this->haveStatusInfo_->haveAction_.isAction(0x35)) {
                            this->actionIndex_ = 0x1b5;
                        }
                        if (this->haveStatusInfo_->haveAction_.isAction(0x38)) {
                            this->actionIndex_ = 0x1b6;
                        }
                        if (this->actionIndex_ == 0) {
                            this->actionIndex_ = 0x1a3;
                        }
                    }
                } else if (idx == 6) {
                    ConfuseArray4 tmp = confusePlayer6;
                    r = dssrand::rand(4);
                    this->actionIndex_ = tmp.v[r];
                } else if (idx == 8) {
                    ConfuseArray4 tmp = confusePlayer8;
                    r = dssrand::rand(4);
                    int act = tmp.v[r];
                    this->actionIndex_ = act;
                    if (act == 0x1bc) {
                        if (this->haveStatusInfo_->haveEquipment_.getEquipment(ITEM_ARMOR) == 0) {
                            this->actionIndex_ = 0x1a3;
                        }
                    }
                } else if (idx == 9) {
                    ConfuseArray4 tmp = confusePlayer9;
                    r = dssrand::rand(4);
                    int act = tmp.v[r];
                    this->actionIndex_ = act;
                    if (act == 0x1c2) {
                        g_Party.setBattleMode();
                        status::PlayerStatus* ps = g_Party.getPlayerStatus(0);
                        if (ps->haveStatusInfo_.haveStatus_.playerIndex_ == 9) {
                            this->actionIndex_ = 0x1a3;
                        }
                    }
                } else if (eventFlag_ != 0) {
                    ConfuseArray6 tmp = confuseOtherParupunte;
                    r = dssrand::rand(6);
                    this->actionIndex_ = tmp.v[r];
                } else {
                    ConfuseArray6 tmp = confuseOtherNormal;
                    r = dssrand::rand(6);
                    this->actionIndex_ = tmp.v[r];
                }
            }
        }
        if (confusionDebugIndex_ != 0) {
            this->actionIndex_ = confusionDebugIndex_;
            confusionDebugIndex_ = 0;
        }
    } else if (this->type_ == MONSTER) {
        int r = dssrand::rand(2);
        if (r == 0) {
            this->actionIndex_ = 0x1a2;
            r = dssrand::rand(4);
            if (r == 0) {
                this->haveStatusInfo_->setConfuseMissAttack(true);
            }
        } else if (eventFlag_ != 0) {
            ConfuseArray8 tmp = confuseMonsterParupunte;
            r = dssrand::rand(8);
            this->actionIndex_ = tmp.v[r];
        } else {
            ConfuseArray8 tmp = confuseMonsterNormal;
            r = dssrand::rand(8);
            this->actionIndex_ = tmp.v[r];
        }
    }
    this->selectCommand_ = UseAction;
    this->selectedTarget_ = -1;
    this->selectedGroup_ = -1;
}


THUMB int status::HaveBattleStatus::setupTorunekoAction()
{
    if (haveStatusInfo_ == 0)
    {
        return 0;
    }
    if (type_ != PLAYER)
    {
        return 0;
    }
    if (haveStatusInfo_->haveStatus_.playerIndex_ != 7)
    {
        return 0;
    }
    if (g_Story.chapter_ == 3)
    {
        return 0;
    }
    if (haveStatusInfo_->isDeath())
    {
        return 0;
    }
    if (haveStatusInfo_->statusChange_.isEnable(status::StatusChange::StatusConfusion))
    {
        return 0;
    }
    if (haveStatusInfo_->statusChange_.isEnable(status::StatusChange::StatusSpazz))
    {
        return 0;
    }
    if (haveStatusInfo_->statusChange_.isEnable(status::StatusChange::StatusSleep))
    {
        return 0;
    }
    if (haveStatusInfo_->statusChange_.isEnable(status::StatusChange::StatusSleep))
    {
        return 0;
    }
    if (torunekoDebugIndex_ != 0)
    {
        actionIndex_ = torunekoDebugIndex_;
        torunekoDebugIndex_ = 0;
        selectCommand_ = UseAction;
        selectedTarget_ = -1;
        selectedGroup_ = -1;
        return 1;
    }
    if (dssrand::rand(4) == 0)
    {
        if (eventFlag_ != 0)
        {
            TorunekoActionTable table = torunekoParupunte;
            actionIndex_ = table.v[dssrand::rand(10)];
        }
        else
        {
            TorunekoActionTable table = torunekoNormal;
            actionIndex_ = table.v[dssrand::rand(10)];
        }
        selectCommand_ = UseAction;
        selectedTarget_ = -1;
        selectedGroup_ = -1;
        return 1;
    }
    return 0;
}

THUMB void status::HaveBattleStatus::setupSpecialAction(int actionIndex)
{
  this->actionIndex_ = actionIndex;
  this->selectCommand_ = UseAction;
  this->selectedTarget_ = -1;
  this->selectedGroup_ = -1;
  return;
}

THUMB void status::HaveBattleStatus::updateSpecialSleepBehaviors()

{
  setupRollOver();
  setupSleepAction();
  return;
}


THUMB void status::HaveBattleStatus::setupRollOver()
{
    if (this->haveStatusInfo_ != 0 && 
        this->type_ == MONSTER && 
        this->index_ == 0x26) 
    {   
        if (this->haveStatusInfo_->statusChange_.isEnable(StatusChange::StatusSleep)) {
            if (dssrand::rand(3) != 0) { 
                this->actionIndex_ = 0x144; 
            }
        }
    }
    return;
}

THUMB void status::HaveBattleStatus::setupSleepAction()
{
    if (this->haveStatusInfo_ != 0 && 
        this->type_ == MONSTER && 
        this->index_ == 0xBC) 
    {
        int isAsleep = this->haveStatusInfo_->statusChange_.isEnable(StatusChange::StatusSleep);
        if (isAsleep) {
            this->actionIndex_ = 0x145;
        }
    }
    return;
}

THUMB void status::HaveBattleStatus::setupTurnBeforeAction() {
    updateSpecialSleepBehaviors();
    return;
}


THUMB void  status::HaveBattleStatus::setMosyasAction(int action)
{
  int mosyasActionCount; 

  mosyasActionCount = this->mosyasActionCount_;
  this->mosyasActionCount_ = this->mosyasActionCount_ + 1;
  this->mosyasAction_[mosyasActionCount] = action;
  return;
}

THUMB int status::HaveBattleStatus::getMosyasAction(int index)
{
    int action;
    
    action = this->mosyasAction_[index];
    if (action == 0x47) {
        this->patternIndex_ = 0;
    } else {
        this->patternIndex_ = 1;
    }
    return action;
}

THUMB void status::HaveBattleStatus::clearMosyasAction()
{
  this->mosyasActionCount_ = 0;
  return;
}

THUMB void status::HaveBattleStatus::print()

{
  return;
}
