#include "main/status/ItemData.hpp"


THUMB status::ItemData::ItemData()

{
  this->index_ = '\0';
  this->equip_ = '\0';
  this->count_ = 0;
  return;
}

THUMB status::ItemData::~ItemData()
{
  return;
}

THUMB void status::ItemData::clear()
{
	this->index_ = 0;
	this->equip_ = 0;
	this->count_ = 0;
	return;
}

