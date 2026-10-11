#include "main/status/OptionStatus.hpp"
#include "main/sound/Sound.hpp"

status::OptionStatus g_Option;


THUMB status::OptionStatus::OptionStatus()
{
  this->sackSort_ = '\0';
  this->bgmVolume_ = '\x02';
  this->seVolume_ = '\x02';
  this->stereo_ = '\0';
  this->speaker_ = '\0';
  this->battleSpeed_ = '\x02';
  this->button_ = '\x02';
  return;
}

THUMB status::OptionStatus::~OptionStatus()
{
  return;
}

THUMB void status::OptionStatus::initialize()

{
  setBgmVolume(2);
  setSeVolume(2);
  setBattleSpeed(2);
  return;
}


THUMB void status::OptionStatus::setSackSort(char id)

{
  this->sackSort_ = id;
  return;
}

THUMB char status::OptionStatus::getSackSort()

{
  return this->sackSort_;
}


THUMB void status::OptionStatus::setBgmVolume(char vol) {
    this->bgmVolume_ = vol;
    Sound::setBgmVolumeSys((0x7F * (this->bgmVolume_ + 1)) / 5);
}

THUMB char status::OptionStatus::getBgmVolume()

{
  return this->bgmVolume_;
}

THUMB void status::OptionStatus::setSeVolume(char vol) {
    this->seVolume_ = vol;
    Sound::setSeVolumeSys((0x7F * (this->seVolume_ + 1)) / 5);
}

THUMB char status::OptionStatus::getSeVolume()

{
  return this->seVolume_;
}

THUMB void status::OptionStatus::setBattleSpeed(char id)

{
  this->battleSpeed_ = id;
  return;
}


THUMB char status::OptionStatus::getBattleSpeed()

{
  return this->battleSpeed_;
}


THUMB char status::OptionStatus::getButton()

{
  return this->button_;
}

