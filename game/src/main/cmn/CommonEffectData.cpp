#pragma ipa file
#include "main/cmn/CommonEffectData.hpp"
#include "main/cmn/CommonEffectResource.hpp"
#include "main/cmn/CommonEffect.hpp"
#include "main/script/ScriptSystem.hpp"
#include "main/script/sys/ScriptParam.hpp"

ARM cmn::CommonEffectResource::CommonEffectResource()
{
}

ARM cmn::CommonEffectResource::~CommonEffectResource()
{
}

ARM void cmn::CommonEffectResource::initialize()
{
    maxStorage_ = 5;
    ResourceStorage::initialize();
}

ARM void cmn::CommonEffectResource::terminate()
{
    ResourceStorage::terminate();
}

ARM cmn::CommonEffectData* cmn::CommonEffectResource::getResource(int id)
{
    return &storage_[ResourceStorage::getResource(id)];
}

ARM int cmn::CommonEffectResource::loadResource(int id)
{
    int area = getEmptyArea();
    storage_[area].setup(id);
    return area;
}

ARM void cmn::CommonEffectResource::releaseResource(int id)
{
    storage_[getResourceArea(id)].cleanup();
}

ARM int cmn::CommonEffectResource::getResourceStock()
{
    int stock = 0;
    for (unsigned int i = 0; i < 5; i++) {
        if (!storage_[i].isEnable()) {
            stock++;
        }
    }
    return stock;
}

ARM cmn::CommonEffectSimple::CommonEffectSimple()
{
    effectData_ = 0;
}

ARM cmn::CommonEffectSimple::~CommonEffectSimple()
{
}

ARM int cmn::CommonEffectSimple::isEnable()
{
    return effectData_ != 0;
}

ARM cmn::CommonEffectFlat::CommonEffectFlat()
{
}

ARM cmn::CommonEffectFlat::~CommonEffectFlat()
{
}

ARM void cmn::CommonEffectFlat::setup(CommonEffectData* data, int flag)
{
    effectData_ = data;
    texture_ = data->getTextureData();
    dssaEffect_.setup(effectData_->getAnimationData());
    dssaEffect_.setTexture(texture_);
    dssaEffect_.type_ = DSSAObjectWithCamera::Near;
    if (effectData_->isPamEnable()) {
        paletteAnim_.anim_.setup(effectData_->getPaletteAnimData());
        paletteAnim_.texture_ = (TextureObject*)texture_;
        paletteAnim_.colorCount_ = paletteAnim_.anim_.getColorCount();
    }
    rate_.value = 0x1000;
}

ARM void cmn::CommonEffectFlat::cleanup(int flag)
{
    if (!isEnable()) {
        return;
    }
    paletteAnim_.unkfunc_0205b648();
    dssaEffect_.cleanup();
    effectData_ = 0;
}

ARM void cmn::CommonEffectFlat::draw()
{
    dssaEffect_.draw();
    dssaEffect_.execute();
    if (effectData_->isPamEnable()) {
        paletteAnim_.unkfunc_0205b44c();
    }
}

ARM void cmn::CommonEffectFlat::setPosition(dss::Fix32Vector3& position)
{
    dssaEffect_.setPosition(position);
}

ARM void cmn::CommonEffectFlat::setScale(dss::Fix32 scale)
{
    dss::Fix32Vector3 one(1, 1, 1);
    dssaEffect_.setScale(one * scale);
    rate_ = scale;
}

ARM void cmn::CommonEffectFlat::setDisplayType(int type)
{
    dssaEffect_.type_ = (DSSAObjectWithCamera::CameraType)type;
}

ARM void cmn::CommonEffectFlat::start()
{
    dssaEffect_.start(0);
    if (effectData_->isPamEnable()) {
        paletteAnim_.unkfunc_0205b3d0();
    }
}

ARM int cmn::CommonEffectFlat::isEnd()
{
    return dssaEffect_.isEnd();
}

ARM cmn::CommonEffectCubic::CommonEffectCubic()
{
}

ARM cmn::CommonEffectCubic::~CommonEffectCubic()
{
}

ARM void cmn::CommonEffectCubic::setup(CommonEffectData* data, int flag)
{
    effectData_ = data;
    int count = 0;
    model_.setup(data->getModelData(), flag);
    for (int i = 0; i < 4; i++) {
        if (effectData_->getAnimData(count)) {
            model_.unkfunc_0205887c(effectData_->getAnimData(count), count);
            count++;
        }
    }
    rate_.value = 0x1000;
}

ARM void cmn::CommonEffectCubic::cleanup(int flag)
{
    if (!isEnable()) {
        return;
    }
    model_.cleanup(flag);
    effectData_ = 0;
}

ARM void cmn::CommonEffectCubic::draw()
{
    model_.draw();
}

ARM void cmn::CommonEffectCubic::setPosition(dss::Fix32Vector3& position)
{
    model_.setPosition(position);
}

ARM void cmn::CommonEffectCubic::setScale(dss::Fix32 scale)
{
    model_.setScale(scale);
    rate_ = scale;
}

ARM void cmn::CommonEffectCubic::start()
{
    model_.start(0);
}

ARM int cmn::CommonEffectCubic::isEnd()
{
    return model_.m_play_flag == 0;
}

ARM void cmn::CommonEffectCubic::setDisplayType(int type)
{
    model_.type_ = (ModelObjectWithCamera::CameraType)type;
}

ARM int cmn::CommonEffectCubic::getType()
{
    return 1;
}

ARM int cmn::CommonEffectFlat::getType()
{
    return 0;
}

ARM cmn::CommonEffectData::CommonEffectData()
{
}

ARM cmn::CommonEffectData::~CommonEffectData()
{
}

ARM void cmn::CommonEffectData::setup(int index)
{
    char path[0x80];
    if (index < 10000) {
        dss::sprintf_s(path, 0x80, "data/effect/effect%03d.lz", index);
    } else {
        dss::sprintf_s(path, 0x80, "data/effect/effect%03db.lz", index - 10000);
    }
    effectData_.setup(path, 1, 1);
    m_effect_type = *(int*)unkfunc_0207f8dc(effectData_.getAddr(), 0);
    if (m_effect_type == 0) {
        setupTexture();
    }
}

ARM void cmn::CommonEffectData::setupTexture()
{
    ((TextureObject*)(getTextureData()))->unkfunc_02086798(0);
}

ARM void cmn::CommonEffectData::cleanup()
{
    if (isEnable()) {
        if (m_effect_type == 0) {
            cleanupTexture();
        }
        effectData_.cleanup();
    }
}

ARM void cmn::CommonEffectData::cleanupTexture()
{
    ((TextureObject*)(getTextureData()))->unkfunc_02086868();
}

ARM void* cmn::CommonEffectData::getTextureData()
{
    return unkfunc_0207f8dc(effectData_.getAddr(), 1);
}

ARM void* cmn::CommonEffectData::getAnimationData()
{
    return unkfunc_0207f8dc(effectData_.getAddr(), 2);
}

ARM void* cmn::CommonEffectData::getPaletteAnimData()
{
    if (!isPamEnable()) {
        return 0;
    }
    return unkfunc_0207f8dc(effectData_.getAddr(), 3);
}

ARM int cmn::CommonEffectData::isPamEnable()
{
    return unkfunc_0207f8c4(effectData_.getAddr()) > 3;
}

ARM void* cmn::CommonEffectData::getModelData()
{
    return unkfunc_0207f8dc(effectData_.getAddr(), 1);
}

ARM void* cmn::CommonEffectData::getAnimData(int index)
{
    if (unkfunc_0207f8c4(effectData_.getAddr()) <= index + 2) {
        return 0;
    }
    return unkfunc_0207f8dc(effectData_.getAddr(), index + 2);
}

ARM int cmn::CommonEffectData::isEnable()
{
    return effectData_.getAddr() != 0;
}

ARM int cmn::CommonEffectData::getEffectType()
{
    return m_effect_type;
}

ARM int cmn::CommonEffectData::isSecondEffect(int index)
{
    char path[0x100];
    dss::sprintf_s(path, 0x80, "data/effect/effect%03db.lz", index);
    return dss::g_File.isExist(path);
}
