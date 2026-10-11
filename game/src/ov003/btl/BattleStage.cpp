#include "ov003/btl/BattleStage.hpp"
#include "ov003/btl/BattleSystem2.hpp"
#include "main/encount/Encount.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/param/BattleMap.hpp"

static const char* btlMapPath = "data/btlmap";

THUMB BattleStage::BattleStage()
{
    dss::strcpy(mapName_, "btl_pl_d");
}

THUMB BattleStage::~BattleStage()
{
}

THUMB BattleStage* BattleStage::getSingleton()
{
    static BattleStage m_singleton;
    return &m_singleton;
}

THUMB void BattleStage::initialize()
{
    param::BattleMap* battleMap = status::excelParam.battleMapData_;
    int index;

    stage_.setRender(&btl::BattleSystem2::getSingleton()->render_);
    stage_.setPath(btlMapPath);
    switch (encount::Encount::getSingleton()->battleMode_) {
    case 0:
        if (stage_.isExist(g_Stage.getBtlMapName())) {
            stage_.load(g_Stage.getBtlMapName());
            index = param::BattleMap::getBattleMap(battleMap, g_Stage.getBtlMapName());
        } else {
            stage_.load("btl_pl_d");
            index = 0;
        }
        break;
    case 1:
        if (stage_.isExist(g_Stage.getTraderMapName())) {
            stage_.load(g_Stage.getTraderMapName());
            index = param::BattleMap::getBattleMap(battleMap, g_Stage.getTraderMapName());
        } else {
            stage_.load("btl_pl_d");
            index = 0;
        }
        break;
    case 2:
        if (stage_.isExist(g_Stage.getInnKeeperMapName())) {
            stage_.load(g_Stage.getInnKeeperMapName());
            index = param::BattleMap::getBattleMap(battleMap, g_Stage.getInnKeeperMapName());
        } else {
            stage_.load("btl_pl_d");
            index = 0;
        }
        break;
    }
    stage_.setup();
    frame_ = 0;
    counter_ = 1;
    MenuAPI::setBattleBackDrop(battleMap[index].R | (battleMap[index].G << 5) | (battleMap[index].B << 10));
}

THUMB void BattleStage::terminate()
{
    MenuAPI::setBattleBackDrop(0);
    stage_.cleanup();
    stage_.terminate();
}

THUMB void BattleStage::execute()
{
    dss::Fix32 rate;
    rate.value = 0x6000;
    setRGBRate(rate, rate, rate);
    execFade();
}

THUMB void BattleStage::execFade()
{
    dss::Fix32Vector3 rgb;
    dss::Fix32 one;
    dss::Fix32 t;

    if (isFade()) {
        one.value = 0x1000;
        t.value = (counter_ << 12) / frame_;
        rgb = prev_ * (one - t) + next_ * t;
        setFldRGBRate(rgb);
        counter_++;
    }
}

THUMB int BattleStage::isFade()
{
    if (counter_ < frame_) {
        return 1;
    }
    return 0;
}

THUMB void BattleStage::setRGBRate(dss::Fix32 r, dss::Fix32 g, dss::Fix32 b)
{
    stage_.m_fld.unkfunc_02047350(r.value, g.value, b.value);
}
