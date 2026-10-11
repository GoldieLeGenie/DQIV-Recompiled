#include "main/status/ActionDefence.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/dss/Random.hpp"

THUMB status::ActionDefence::ActionDefence()

{
  return;
}



THUMB status::ActionDefence::~ActionDefence()
{
  return;
}



THUMB void status::ActionDefence::setup(unsigned short index, int flag)
{
    if (flag == 1) {
        setupPlayer();
        return;
    }
    setupMonster(index);
    return;
}

THUMB void status::ActionDefence::setupPlayer()

{
    this->characterType_ = PLAYER;
    this->io_ = '\0';
    this->gira_ = '\0';
    this->mera_ = '\0';
    this->dein_ = '\0';
    this->bagi_ = '\0';
    this->hyado_ = '\0';
    this->manusa_ = '\0';
    this->rariho_ = '\0';
    this->zaki_ = '\0';
    this->nifram_ = '\0';
    this->mahotora_ = '\0';
    this->mahotone_ = '\0';
    this->medapani_ = '\0';
    this->rukani_ = '\0';
    this->megante_ = '\0';
    this->dragon_ = '\0';
    this->metal_ = '\0';
    this->zombi_ = '\0';
    this->air_ = '\0';
    this->slime_ = '\0';
    this->poison_ = '\0';
    this->rest_ = '\0';
    this->antidance_ = '\0';
    this->fire_ = '\0';
    this->blizzard_ = '\0';
    this->clap_ = '\0';
    this->spazz_ = '\0';
    this->army_ = '\0';
    return;
}


THUMB void status::ActionDefence::setupMonster(int index) {
    characterType_ = MONSTER;
    param::MonsterData* data = excelParam.monsterData_;
    signed char b;

    io_    = data[index].byte_1 & 3;
    gira_  = (data[index].byte_1 & 0xC)  >> 2;
    mera_  = (data[index].byte_1 & 0x30) >> 4;
    dein_  = (data[index].byte_1 & 0xC0) >> 6;

    b = data[index].byte_2; bagi_      = b & 3;
    b = data[index].byte_2; hyado_     = (b & 0xC)  >> 2;
    b = data[index].byte_2; manusa_    = (b & 0x30) >> 4;
    b = data[index].byte_2; rariho_    = (b & 0xC0) >> 6;

    b = data[index].byte_3; zaki_      = b & 3;
    b = data[index].byte_3; nifram_    = (b & 0xC)  >> 2;
    b = data[index].byte_3; mahotora_  = (b & 0x30) >> 4;
    b = data[index].byte_3; mahotone_  = (b & 0xC0) >> 6;

    b = data[index].byte_4; medapani_  = b & 3;
    b = data[index].byte_4; rukani_    = (b & 0xC)  >> 2;
    b = data[index].byte_4; megante_   = (b & 0x30) >> 4;
    b = data[index].byte_4; dragon_    = (b & 0xC0) >> 6;

    b = data[index].byte_5; metal_     = b & 3;
    b = data[index].byte_5; zombi_     = (b & 0xC)  >> 2;
    b = data[index].byte_5; air_       = (b & 0x30) >> 4;
    b = data[index].byte_5; slime_     = (b & 0xC0) >> 6;

    b = data[index].byte_6; poison_    = b & 3;
    b = data[index].byte_6; rest_      = (b & 0xC)  >> 2;
    b = data[index].byte_6; antidance_ = (b & 0x30) >> 4;
    b = data[index].byte_6; fire_      = (b & 0xC0) >> 6;

    b = data[index].byte_7; blizzard_  = b & 3;
    b = data[index].byte_7; clap_      = (b & 0xC)  >> 2;
    b = data[index].byte_7; spazz_     = (b & 0x30) >> 4;
    b = data[index].byte_7; army_      = (b & 0xC0) >> 6;
}


THUMB int status::ActionDefence::getEffect(ActionDefenceType type,unsigned char value)
{
  int randomValue ;
  int effectValue ;
  
  effectValue  = 0;
  switch(type) {
  case ACT_DEF_A:
    randomValue  = dssrand::rand(0x3e8);
    effectValue  = 0;
    if ((value == '\0') && (randomValue  < 1000)) {
      effectValue  = 1000;
    }
    if ((value == '\x01') && (randomValue  < 0x352)) {
      effectValue  = 1000;
    }
    if ((value == '\x02') && (randomValue  < 400)) {
      effectValue  = 1000;
    }
    if ((value == '\x03') && (randomValue  < 0)) {
      effectValue  = 1000;
    }
    break;
  case ACT_DEF_B:
    randomValue  = dssrand::rand(0x3e8);
    effectValue  = 0;
    if ((value == '\0') && (randomValue  < 1000)) {
      effectValue  = 1000;
    }
    if ((value == '\x01') && (randomValue  < 1000)) {
      effectValue  = 1000;
    }
    if ((value == '\x02') && (randomValue  < 0x2ee)) {
      effectValue  = 1000;
    }
    if ((value == '\x03') && (randomValue  < 0)) {
      effectValue  = 1000;
    }
    break;
  case ACT_DEF_C:
    randomValue  =  dssrand::rand(0x3e8);
    effectValue  = 0;
    if ((value == '\0') && (randomValue  < 500)) {
      effectValue  = 1000;
    }
    if ((value == '\x01') && (randomValue  < 200)) {
      effectValue  = 1000;
    }
    if ((value == '\x02') && (randomValue  < 100)) {
      effectValue  = 1000;
    }
    if ((value == '\x03') && (randomValue  < 0)) {
      effectValue  = 1000;
    }
    break;
  case ACT_DEF_D:
    if (value == 0) {
      effectValue  = 1000;
    }
    if (value == 1) {
      effectValue  = 700;
    }
    if (value == 2) {
      effectValue  = 400;
    }
    if (value == 3) {
      effectValue  = 0;
    }
    break;
  case ACT_DEF_E:
    if (value == 0) {
      effectValue  = 1000;
    }
    if (value == 1) {
      effectValue  = 800;
    }
    if (value == 2) {
      effectValue  = 500;
    }
    if (value == 3) {
      effectValue  = 0;
    }
    break;
  case ACT_DEF_F:
    if (value == 0) {
      effectValue  = 0x514;
    }
    if (value == 1) {
      effectValue  = 0x47e;
    }
    if (value == 2) {
      effectValue  = 0x2ee;
    }
    if (value == '\x03') {
      effectValue  = 300;
    }
    break;
  case ACT_DEF_G:
    if (value == 0) {
      effectValue  = 0x2ee;
    }
    if (value == 1) {
      effectValue  = 500;
    }
    if (value == 2) {
      effectValue  = 0xfa;
    }
    if (value == 3) {
      effectValue  = 0;
    }
    break;
  case ACT_DEF_H:
    randomValue  = dssrand::rand(0x3e8);
    effectValue  = 0;
    if ((value == '\0') && (randomValue  < 900)) {
      effectValue  = 1000;
    }
    if ((value == '\x01') && (randomValue  < 600)) {
      effectValue  = 1000;
    }
    if ((value == '\x02') && (randomValue  < 300)) {
      effectValue  = 1000;
    }
    if ((value == '\x03') && (randomValue  < 0)) {
      effectValue  = 1000;
    }
  }
  return effectValue;
}


THUMB int status::ActionDefence::exec(ActionDefenceKind kind, ActionDefenceType type)
{
  int value;
  int result;
  
  switch(kind) {
  case ACT_DEF_ALL:
    result = 1000;
    break;
  case ACT_DEF_IO:
    result = getEffect(type, this->io_);
    if (this->characterType_ == PLAYER) {
      result = 1000;
    }
    break;
  case ACT_DEF_GIRA:
    result = getEffect(type, this->gira_);
    if (this->characterType_ == PLAYER) {
      result = 1000;
    }
    break;
  case ACT_DEF_MERA:
    result = getEffect(type, this->mera_);
    if (this->characterType_ == PLAYER) {
      result = 1000;
    }
    break;
  case ACT_DEF_DEIN:
    result = getEffect(type, this->dein_);
    if (this->characterType_ == PLAYER) {
      result = 1000;
    }
    break;
  case ACT_DEF_BAGI:
    result = getEffect(type, this->bagi_);
    if (this->characterType_ == PLAYER) {
      result = 1000;
    }
    break;
  case ACT_DEF_HYADO:
    result = getEffect(type, this->hyado_);
    if (this->characterType_ == PLAYER) {
      result = 1000;
    }
    break;
  case ACT_DEF_MANUSA:
    result = getEffect(type, this->manusa_);
    if (this->characterType_ == PLAYER) {
      result = 0;
      value = dssrand::rand(0x3e8);
      if (value < 0x271) {
        result = 1000;
      }
    }
    break;
  case ACT_DEF_RARIHO:
    result = getEffect(type, this->rariho_);
    if (this->characterType_ == PLAYER) {
      result = 0;
      value = dssrand::rand(0x3e8);
      if (value < 0x177) {
        result = 1000;
      }
    }
    break;
  case ACT_DEF_ZAKI:
    result = getEffect(type, this->zaki_);
    if (this->characterType_ == PLAYER) {
      result = 0;
      value = dssrand::rand(0x3e8);
      if (value < 0x177) {
        result = 1000;
      }
    }
    break;
  case ACT_DEF_NIFRAM:
    result = getEffect(type, this->nifram_);
    if (this->characterType_ == PLAYER) {
      result = 0;
    }
    break;
  case ACT_DEF_MAHOTORA:
    result = getEffect(type, this->mahotora_);
    if (this->characterType_ == PLAYER) {
      result = 1000;
    }
    break;
  case ACT_DEF_MAHOTONE:
    result = getEffect(type, this->mahotone_);
    if (this->characterType_ == PLAYER) {
      result = 0;
      value = dssrand::rand(0x3e8);
      if (value < 0x177) {
        result = 1000;
      }
    }
    break;
  case ACT_DEF_MEDAPANI:
    result = getEffect(type, this->medapani_);
    if (this->characterType_ == PLAYER) {
      result = 0;
      value = dssrand::rand(0x3e8);
      if (value < 0xfa) {
        result = 1000;
      }
    }
    break;
  case ACT_DEF_RUKANI:
    result = getEffect(type, this->rukani_);
    if (this->characterType_ == PLAYER) {
      result = 0;
      value = dssrand::rand(0x3e8);
      if (value < 0x2ee) {
        result = 1000;
      }
    }
    break;
  case ACT_DEF_MEGANTE:
    result = getEffect(type, this->megante_);
    if (this->characterType_ == PLAYER) {
      result = 1000;
    }
    break;
  case ACT_DEF_DRAGON:
    result = getEffect(type, this->dragon_);
    break;
  case ACT_DEF_METAL:
    result = getEffect(type, this->metal_);
    break;
  case ACT_DEF_ZONBI:
    result = getEffect(type, this->zombi_);
    break;
  case ACT_DEF_AIR:
    result = getEffect(type, this->air_);
    break;
  case ACT_DEF_SLIME:
    result = getEffect(type, this->slime_);
    break;
  case ACT_DEF_POISON:
    result = getEffect(type, this->poison_);
    if (this->characterType_ == PLAYER) {
      result = 0;
      value = dssrand::rand(0x3e8);
      if (value < 0x177) {
        result = 1000;
      }
    }
    break;
  case ACT_DEF_REST:
    result = getEffect(type, this->rest_);
    if (this->characterType_ == PLAYER) {
      result = 0;
      value = dssrand::rand(0x3e8);
      if (value < 0x177) {
        result = 1000;
      }
    }
    break;
  case ACT_DEF_ANTIDANCE:
    result = getEffect(type, this->antidance_);
    break;
  case ACT_DEF_FIRE:
    result = getEffect(type, this->fire_);
    if (this->characterType_ == PLAYER) {
      result = 1000;
    }
    break;
  case ACT_DEF_BLIZZARD:
    result = getEffect(type, this->blizzard_);
    if (this->characterType_ == PLAYER) {
      result = 1000;
    }
    break;
  case ACT_DEF_CLAP:
    result = getEffect(type, this->clap_);
    break;
  case ACT_DEF_SPAZZ:
    result = getEffect(type, this->spazz_);
    if (this->characterType_ == PLAYER) {
      result = 0;
      value = dssrand::rand(0x3e8);
      if (value < 0x7d) {
        result = 1000;
      }
    }
    break;
  case ACT_DEF_ARMY:
    result = getEffect(type, this->army_);
    break;
  }
  return result;
}


THUMB int status::ActionDefence::getEffectAI(ActionDefenceType type,unsigned char value)
{
  int randomValue;
  int effectValue;
  
  effectValue = 0;
  switch(type) {
  case ACT_DEF_A:
    randomValue = dssrand::rand(0x3e8);
    effectValue = 0;
    if ((value == '\0') && (randomValue < 1000)) {
      effectValue = 1000;
    }
    if (value == '\x01') {
      effectValue = 0x352;
    }
    if (value == '\x02') {
      effectValue = 400;
    }
    if (value == '\x03') {
      effectValue = 0;
    }
    break;
  case ACT_DEF_B:
    dssrand::rand(0x3e8);
    effectValue = 0;
    if (value == '\0') {
      effectValue = 1000;
    }
    if (value == '\x01') {
      effectValue = 1000;
    }
    if (value == '\x02') {
      effectValue = 0x2ee;
    }
    if (value == '\x03') {
      effectValue = 0;
    }
    break;
  case ACT_DEF_C:
    dssrand::rand(0x3e8);
    effectValue = 0;
    if (value == '\0') {
      effectValue = 500;
    }
    if (value == '\x01') {
      effectValue = 200;
    }
    if (value == '\x02') {
      effectValue = 100;
    }
    if (value == '\x03') {
      effectValue = 0;
    }
    break;
  case ACT_DEF_D:
    if (value == '\0') {
      effectValue = 1000;
    }
    if (value == '\x01') {
      effectValue = 700;
    }
    if (value == '\x02') {
      effectValue = 400;
    }
    if (value == '\x03') {
      effectValue = 0;
    }
    break;
  case ACT_DEF_E:
    if (value == '\0') {
      effectValue = 1000;
    }
    if (value == '\x01') {
      effectValue = 800;
    }
    if (value == '\x02') {
      effectValue = 500;
    }
    if (value == '\x03') {
      effectValue = 0;
    }
    break;
  case ACT_DEF_F:
    if (value == '\0') {
      effectValue = 0x514;
    }
    if (value == '\x01') {
      effectValue = 0x47e;
    }
    if (value == '\x02') {
      effectValue = 0x2ee;
    }
    if (value == '\x03') {
      effectValue = 300;
    }
    break;
  case ACT_DEF_G:
    if (value == '\0') {
      effectValue = 0x2ee;
    }
    if (value == '\x01') {
      effectValue = 500;
    }
    if (value == '\x02') {
      effectValue = 0xfa;
    }
    if (value == '\x03') {
      effectValue = 0;
    }
    break;
  case ACT_DEF_H:
    randomValue = dssrand::rand(0x3e8);
    effectValue = 0;
    if ((value == '\0') && (randomValue < 900)) {
      effectValue = 1000;
    }
    if ((value == '\x01') && (randomValue < 600)) {
      effectValue = 1000;
    }
    if ((value == '\x02') && (randomValue < 300)) {
      effectValue = 1000;
    }
    if ((value == '\x03') && (randomValue < 0)) {
      effectValue = 1000;
    }
  }
  return effectValue;
}


THUMB int status::ActionDefence::execAI(ActionDefenceKind kind, ActionDefenceType type)
{
    int value;
    int result;

    switch(kind) {
        
    case ACT_DEF_ALL:
        result = 1000;
        break;
        

    case ACT_DEF_IO:
        result = getEffectAI(type, this->io_);
        if (this->characterType_ == PLAYER) {
            result = 1000;
        }
        break;

        
    case ACT_DEF_GIRA:
        result = getEffectAI(type, this->gira_);
        if (this->characterType_ == PLAYER) {
            result = 1000;
        }
        break;

    case ACT_DEF_MERA:
        result = getEffectAI(type, this->mera_);
        if (this->characterType_ == PLAYER) {
            result = 1000;
        }
        break;

    case ACT_DEF_DEIN:
        result = getEffectAI(type, this->dein_);
        if (this->characterType_ == PLAYER) {
            result = 1000;
        }
        break;
        

    case ACT_DEF_BAGI:
        result = getEffectAI(type, this->bagi_);
        if (this->characterType_ == PLAYER) {
            result = 1000;
        }
        break;

    case ACT_DEF_HYADO:
        result = getEffectAI(type, this->hyado_);
        if (this->characterType_ == PLAYER) {
            result = 1000;
        }
        break;

    case ACT_DEF_MANUSA:
        result = getEffectAI(type, this->manusa_);
        if (this->characterType_ == PLAYER) {
            result = 0;
            value = dssrand::rand(0x3e8);
            if (value < 0x271) {
                result = 1000;
            }
        }
        break;

    case ACT_DEF_RARIHO:
        result = getEffectAI(type, this->rariho_);
        if (this->characterType_ == PLAYER) {
            result = 0;
            value = dssrand::rand(0x3e8);
            if (value < 0x177) {
                result = 1000;
            }
        }
        break;

    case ACT_DEF_ZAKI:
        result = getEffectAI(type, this->zaki_);
        if (this->characterType_ == PLAYER) {
            result = 0;
            value = dssrand::rand(0x3e8);
            if (value < 0x177) {
                result = 1000;
            }
        }
        break;

    case ACT_DEF_NIFRAM:
        result = getEffectAI(type, this->nifram_);
        if (this->characterType_ == PLAYER) {
            result = 0;
        }
        break;

    case ACT_DEF_MAHOTORA:
        result = getEffectAI(type, this->mahotora_);
        if (this->characterType_ == PLAYER) {
            result = 1000;
        }
        break;

    case ACT_DEF_MAHOTONE:
        result = getEffectAI(type, this->mahotone_);
        if (this->characterType_ == PLAYER) {
            result = 0;
            value = dssrand::rand(0x3e8);
            if (value < 0x177) {
                result = 1000;
            }
        }
        break;

    case ACT_DEF_MEDAPANI:
        result = getEffectAI(type, this->medapani_);
        if (this->characterType_ == PLAYER) {
            result = 0;
            value = dssrand::rand(0x3e8);
            if (value < 0xfa) {
                result = 1000;
            }
        }
        break;

    case ACT_DEF_RUKANI:
        result = getEffectAI(type, this->rukani_);
        if (this->characterType_ == PLAYER) {
            result = 0x2ee;
            value = dssrand::rand(0x3e8);
            if (value < 0x2ee) {
                result = 1000;
            } 
        }
        break;

    
    case ACT_DEF_MEGANTE:
        result = getEffectAI(type, this->megante_);
        if (this->characterType_ == PLAYER) {
            result = 1000;
        }
        break;

        
    case ACT_DEF_DRAGON:
        result = getEffectAI(type, this->dragon_);
        break;

        
    case ACT_DEF_METAL:
        result = getEffectAI(type, this->metal_);
        break;
        

    case ACT_DEF_ZONBI:
        result = getEffectAI(type, this->zombi_);
        break;

    case ACT_DEF_AIR:
        result = getEffectAI(type, this->air_);
        break;

    case ACT_DEF_SLIME:
        result = getEffectAI(type, this->slime_);
        break;

    case ACT_DEF_POISON:
        result = getEffectAI(type, this->poison_);
        if (this->characterType_ == PLAYER) {
            result = 0;
            value = dssrand::rand(0x3e8);
            if (value < 0x177) {
                result = 1000;
            }
        }
        break;

        
    case ACT_DEF_REST:
        result = getEffectAI(type, this->rest_);
        if (this->characterType_ == PLAYER) {
            result = 0;
            value = dssrand::rand(0x3e8);
            if (value < 0x177) {
                result = 1000;
            }
        }
        break;

    case ACT_DEF_ANTIDANCE:
        result = getEffectAI(type, this->antidance_);
        break;

        
    case ACT_DEF_FIRE:
        result = getEffectAI(type, this->fire_);
        if (this->characterType_ == PLAYER) {
            result = 1000;
        }
        break;

        
    case ACT_DEF_BLIZZARD:
        result = getEffectAI(type, this->blizzard_);
        if (this->characterType_ == PLAYER) {
            result = 1000;
        }
        break;

        
    case ACT_DEF_CLAP:
        result = getEffectAI(type, this->clap_);
        break;

    case ACT_DEF_SPAZZ:
        result = getEffectAI(type, this->spazz_);
        if (this->characterType_ == PLAYER) {
            result = 0;
            value = dssrand::rand(0x3e8);
            if (value < 0x7d) {
                result = 1000;
            }
        }
        break;

    case ACT_DEF_ARMY:
        result = getEffectAI(type, this->army_);
        break;
    }

    return result;
}