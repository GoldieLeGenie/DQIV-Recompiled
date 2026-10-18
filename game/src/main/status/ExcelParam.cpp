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
	this->surechigai_ = (param::SurechigaiObjectData*)surechigaiObjectDataTable.data;
	this->charaInitData_ = (param::CharInitData*)charInitDataTable.data;
	this->heroData_ = (param::HeroData*)heroDataTable.data;
	this->warriorData_ = (param::WarriorData*)warriorDataTable.data;
	this->princessData_ = (param::PrincessData*)princessDataTable.data;
	this->priestData_ = (param::PriestData*)priestDataTable.data;
	this->mageData_ = (param::MageData*)mageDataTable.data;
	this->traderData_ = (param::TraderData*)traderDataTable.data;
	this->warlockData_ = (param::WarlockData*)warlockDataTable.data;
	this->dancerData_ = (param::DancerData*)dancerDataTable.data;
	this->pissaroData_ = (param::PissaroData*)pissaroDataTable.data;
	this->itemData_ = (param::ItemData*)itemDataTable.data;
	this->charaVoice_ = (param::CharaVoice*)charaVoiceTable.data;
	this->actionParam_ = (param::ActionParam*)actionParamTable.data;
	this->abreactTurn_ = (param::AbreactTurn*)abreactTurnTable.data;
	this->monsterData_ = (param::MonsterData*)monsterDataTable.data;
	this->bookData_ = (param::BookData*)bookDataTable.data;
	this->splitMsg_ = (param::SplitMsg*)splitMsgTable.data;
	this->encountData_ = (param::EncountData*)encountDataTable.data;
	this->encountFormationID_ = (param::EncountFormationID*)encountFormationIdTable.data;
	this->encountFormNum_ = (param::EncountFormNum*)encountFormNumTable.data;
	this->encountSpecial_ = (param::EncountSpecial*)encountSpecialTable.data;
	this->monsterMap_ = (param::MonsterMap*)monsterMapTable.data;
	this->encountSeaTile_.tile_ = (unsigned char*)encountSeaTileTable.data;
	this->encountTile1_.tile_ = (unsigned char*)encountTile1Table.data;
	this->encountTile2_.tile_ = (unsigned char*)encountTile2Table.data;
	this->encountTile3_.tile_ = (unsigned char*)encountTile3Table.data;
	this->encountGotTile_.tile_ = (unsigned char*)encountGotTileTable.data;
	this->encountYamiTile_.tile_ = (unsigned char*)encountYamiTileTable.data;
	this->colorCorrect_ = (param::ColorCorrect*)colorCorrectTable.data;
	this->floorFog_ = (param::FloorFog*)floorFogTable.data;
	this->clutCode_ = (param::CLUTCode*)clutCodeTable.data;
	this->floorBackColor_ = (param::FloorBackColor*)floorBackColorTable.data;
	this->floorParam_ = (param::FloorParam*)floorParamTable.data;
	this->shopDataFirst_ = (param::ShopDataFirst*)shopDataFirstTable.data;
	this->shopDataSecond_ = (param::ShopDataSecond*)shopDataSecondTable.data;
	this->vehicle_ = (param::VehicleData*)vehicleDataTable.data;
	this->mapChurch_ = (param::MapChurch*)mapChurchTable.data;
	this->effectColor_ = (param::EffectColorParam*)effectColorTable.data;
	this->surechigaiTenant_ = (param::SurechigaiTenant*)surechigaiTenantTable.data;
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