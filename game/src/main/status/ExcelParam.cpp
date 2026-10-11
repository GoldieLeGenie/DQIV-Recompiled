#include "main/status/ExcelParam.hpp"


status::ExcelParam status::excelParam;

THUMB status::ExcelParam::ExcelParam(){
	return;
}

THUMB status::ExcelParam::~ExcelParam(){
	return;
}

THUMB void status::ExcelParam::setup()
{
	this->surechigai_ = (param::SurechigaiObjectData*)data_0208cc04.data;
	this->charaInitData_ = (param::CharInitData*)charInitDataTable.data;
	this->heroData_ = (param::HeroData*)heroDataTable.data;
	this->warriorData_ = (param::WarriorData*)warriorDataTable.data;
	this->princessData_ = (param::PrincessData*)princessDataTable.data;
	this->priestData_ = (param::PriestData*)priestDataTable.data;
	this->mageData_ = (param::MageData*)data_020919d8.data;
	this->traderData_ = (param::TraderData*)data_0209233c.data;
	this->warlockData_ = (param::WarlockData*)data_02092ca0.data;
	this->dancerData_ = (param::DancerData*)data_02093604.data;
	this->pissaroData_ = (param::PissaroData*)data_02093f68.data;
	this->itemData_ = (param::ItemData*)data_020978d8.data;
	this->charaVoice_ = (param::CharaVoice*)data_0208d9ac.data;
	this->actionParam_ = (param::ActionParam*)data_020aa600.data;
	this->abreactTurn_ = (param::AbreactTurn*)data_0208e3a8.data;
	this->monsterData_ = (param::MonsterData*)data_020a02e4.data;
	this->bookData_ = (param::BookData*)data_0208dd14.data;
	this->splitMsg_ = (param::SplitMsg*)data_02098d1c.data;
	this->encountData_ = (param::EncountData*)data_0209dae0.data;
	this->encountFormationID_ = (param::EncountFormationID*)data_0208cdc4.data;
	this->encountFormNum_ = (param::EncountFormNum*)data_0208cab4.data;
	this->encountSpecial_ = (param::EncountSpecial*)data_02095c60.data;
	this->monsterMap_ = (param::MonsterMap*)data_020948cc.data;
	this->encountSeaTile_.tile_ = (unsigned char*)data_0208d0cc.data;
	this->encountTile1_.tile_ = (unsigned char*)data_0208cfc8.data;
	this->encountTile2_.tile_ = (unsigned char*)data_0208d1d0.data;
	this->encountTile3_.tile_ = (unsigned char*)data_0208cec4.data;
	this->encountGotTile_.tile_ = (unsigned char*)data_0208ca74.data;
	this->encountYamiTile_.tile_ = (unsigned char*)data_0208ca64.data;
	this->colorCorrect_ = (param::ColorCorrect*)data_0209a448.data;
	this->floorFog_ = (param::FloorFog*)data_0208ca88.data;
	this->clutCode_ = (param::CLUTCode*)data_0208ccd0.data;
	this->floorBackColor_ = (param::FloorBackColor*)data_0208d2d4.data;
	this->floorParam_ = (param::FloorParam*)data_020a4cbc.data;
	this->shopDataFirst_ = (param::ShopDataFirst*)data_02096964.data;
	this->shopDataSecond_ = (param::ShopDataSecond*)data_0209bd9c.data;
	this->vehicle_ = (param::VehicleData*)data_0208d6f0.data;
	this->mapChurch_ = (param::MapChurch*)data_0208d47c.data;
	this->effectColor_ = (param::EffectColorParam*)data_0209523c.data;
	this->surechigaiTenant_ = (param::SurechigaiTenant*)data_0208cb48.data;
	return;
}

THUMB param::CharInitData* status::ExcelParam::getCharaInitData()
{
  return this->charaInitData_;
}

THUMB param::HeroData* status::ExcelParam::getHeroData()
{
  return this->heroData_;
}

THUMB param::WarriorData* status::ExcelParam::getWarriorData()
{
  return this->warriorData_;
}

THUMB param::PrincessData* status::ExcelParam::getPrincessData()
{
  return this->princessData_;
}

THUMB param::PriestData* status::ExcelParam::getPriestData()
{
  return this->priestData_;
}

THUMB param::MageData* status::ExcelParam::getMageData()
{
  return this->mageData_;
}

THUMB param::TraderData* status::ExcelParam::getTraderData()
{
  return this->traderData_;
}

THUMB param::WarlockData* status::ExcelParam::getWarlockData()
{
  return this->warlockData_;
}

THUMB param::DancerData* status::ExcelParam::getDancerData()
{
  return this->dancerData_;
}

THUMB param::PissaroData* status::ExcelParam::getPissaroData()
{
  return this->pissaroData_;
}

THUMB param::ItemData* status::ExcelParam::getItemData()
{
  return this->itemData_;
}

THUMB param::AppriseItem* status::ExcelParam::getAppriseItem()
{
	return this->appriseItem_;
}

THUMB param::AlterMessage* status::ExcelParam::getAlterMessage()
{
	return this->alterMessage_;
}

THUMB param::CharaVoice* status::ExcelParam::getCharaVoice()
{
  return this->charaVoice_;
}