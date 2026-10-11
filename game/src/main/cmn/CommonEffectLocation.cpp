#include "main/cmn/CommonEffectLocation.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "main/param/Param.hpp"
#include "main/status/ExcelParam.hpp"


#pragma profile on

ARM cmn::CommonEffectLocation::CommonEffectLocation()
{
}

ARM cmn::CommonEffectLocation::~CommonEffectLocation()
{
}

ARM cmn::CommonEffectLocation* cmn::CommonEffectLocation::getSingleton()
{
    static CommonEffectLocation m_singleton;
    return &m_singleton;
}

ARM void cmn::CommonEffectLocation::initialize()

{
  this->enable_ = 0;
  return;
}

ARM void cmn::CommonEffectLocation::terminate()
{
  return;
}


ARM void cmn::CommonEffectLocation::execute()
{
    if (this->enable_ != 0 && calcPaletteRate() == 0) {
        int i = this->index_ + 1;
        this->index_ = i;
        if (setPaletteRate(i) == 0)
            this->enable_ = 0;
    }
}

ARM void cmn::CommonEffectLocation::start(int index, int extend)
{

    this->index_ = index;
    this->enable_ = 1;
    this->extend_ = extend;
    setPaletteRate(index);

}

ARM int cmn::CommonEffectLocation::setPaletteRate(int index)
{
    param::EffectColorParam* tbl = status::excelParam.effectColor_;

    if (tbl[index + 1].frame == 0)
        return 0;

    this->prev_.vx.value = tbl[index].rPoint;
    this->prev_.vy.value = tbl[index].gPoint;
    this->prev_.vz.value = tbl[index].bPoint;

    this->next_.vx.value = tbl[index + 1].rPoint;
    this->next_.vy.value = tbl[index + 1].gPoint;
    this->next_.vz.value = tbl[index + 1].bPoint;

    this->frame_   = tbl[index + 1].frame;
    this->counter_ = 0;

    if (tbl[index + 3].frame == 0)
        this->frame_ = this->frame_ + this->extend_;

    return 1;
}



static inline void setFieldRGBRate(const dss::Fix32Vector3& rgb)
{
    VecFx32     v;
    dss::Fix32Vector3 tmp(rgb);
    BattleStage*     stage = BattleStage::getSingleton();

    v.x = tmp.vx.value;
    v.y = tmp.vy.value;
    v.z = tmp.vz.value;

    stage->stage_.m_fld.SetRGBRate(&v, 0);
}


ARM int cmn::CommonEffectLocation::calcPaletteRate()
{
    dss::Fix32Vector3 rgb;
    dss::Fix32 one;
    dss::Fix32 rate;

    if (this->counter_ >= this->frame_)
        return 0;

    if ((this->prev_ == this->next_) != 0) {
        this->counter_ = this->counter_ + 1;
        return 1;
    }

    one.value  = (fx32)0x1000;
    rate.value = (fx32)((this->counter_ << 12) / this->frame_);

    rgb = this->prev_ * (one - rate) + this->next_ * rate;

    if (data_0210bb94.unkfunc_02058114(0xC) != 0)
        TownStageManager::getSingleton()->SetRGBRate(rgb, 0);

    if (data_0210bb94.unkfunc_02058114(0xD) != 0)
        setFieldRGBRate(rgb);

    this->counter_ = this->counter_ + 1;
    return 1;
}


ARM dss::Fix32Vector3 cmn::CommonEffectLocation::getPaletteRate()
{
    dss::Fix32 one;
    dss::Fix32 rate;

    one.value  = (fx32)0x1000;
    rate.value = (fx32)((this->counter_ << 12) / this->frame_);

    return this->prev_ * (one - rate) + this->next_ * rate;
}
#pragma profile off