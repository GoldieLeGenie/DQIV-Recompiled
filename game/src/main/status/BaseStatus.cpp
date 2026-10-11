#include "main/status/BaseStatus.hpp"
#include <globaldefs.h>

THUMB status::BaseStatus::BaseStatus() {
    this->exp_ = 0;
    this->strength_ = 0;
    this->protection_ = 0;
    this->agility_ = '\0';
    this->wisdom_ = '\0';
    this->hp_ = 0;
    this->hpMax_ = 0;
    this->mp_ = 0;
    this->mpMax_ = 0;
    this->luck_ = '\0';
}

THUMB status::BaseStatus::~BaseStatus() {
    return;
}


THUMB unsigned int status::BaseStatus::getClampValue(unsigned int value, unsigned int min, unsigned int max) {
	unsigned int clamped = getMin(value, min);
	return getMax(clamped, max);
}


THUMB unsigned int status::BaseStatus::getMin(unsigned int a, unsigned int b) {
	return (a > b) ? a : b;
}

THUMB unsigned int status::BaseStatus::getMax(unsigned int a, unsigned int b) {
	return (a < b) ? a : b;
}


THUMB void status::BaseStatus::addStrength(char str)
{
  this->strength_ =  dss::clamp<int>(this->strength_ + str,0,0xff) & 0xff;
  return;
}


THUMB void status::BaseStatus::addAgility(char agi)
{
  this->agility_ = dss::clamp<int>(this->agility_ + agi,0,0xff);
  return;
}


THUMB void status::BaseStatus::addProtection(char pr)
{
  this->protection_ = dss::clamp<int>(this->protection_ + pr,0,0xff) & 0xff;
  return;
}


THUMB void status::BaseStatus::addWisdom(char wis)
{  
  this->wisdom_ = dss::clamp<int>(this->wisdom_ + wis,0,0xff);
  return;
}

THUMB unsigned short status::BaseStatus::getClampValue2(unsigned short value, unsigned short min, unsigned short max) {
	unsigned short clamped = getMin2(value, min);
	return getMax2(clamped, max);
}

THUMB unsigned short status::BaseStatus::getMin2(unsigned short a, unsigned short b) {
	return (a > b) ? a : b;
}

THUMB unsigned short status::BaseStatus::getMax2(unsigned short a, unsigned short b) {
	return (a < b) ? a : b;
}

THUMB void status::BaseStatus::setHpMax(unsigned short hp){
  this->hpMax_ = getClampValue2(hp,0,9999);
  return;
}

THUMB void status::BaseStatus::addHpMax(char hp)
{  
  unsigned short currentHp = hp;
  this->hpMax_ = this->hpMax_ + currentHp;
  this->hpMax_ = getClampValue2(this->hpMax_,0,9999);
  return;
}

THUMB void status::BaseStatus::setMpMax(unsigned short mp)
{
  this->mpMax_ = getClampValue2(mp,0,999);
  return;
}

THUMB void status::BaseStatus::addMpMax(char mp)
{  
  unsigned short currentMp = mp;
  this->mpMax_ = this->mpMax_ + currentMp;
  this->mpMax_ = getClampValue2(this->mpMax_,0,999);
  return;
}

