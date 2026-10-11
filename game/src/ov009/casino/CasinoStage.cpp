#pragma ipa file
#include "ov009/casino/CasinoStage.hpp"
#include "ov009/casino/CasinoSystem.hpp"

static const char* stagePath = "data/map";

ARM CasinoStage::CasinoStage()
{
}

ARM CasinoStage::~CasinoStage()
{
}

ARM CasinoStage* CasinoStage::getSingleton()
{
    static CasinoStage casinoStage;
    return &casinoStage;
}

ARM void CasinoStage::initialize()
{
    stage_.setRender(&CasinoSystem::getSingleton()->render_);
    stage_.setPath(stagePath);
    stage_.load(CasinoSystem::getSingleton()->minigame->getStageName());
    stage_.setup();
}

ARM void CasinoStage::terminate()
{
    stage_.cleanup();
    stage_.terminate();
}

ARM void CasinoStage::setObjectDraw(int id, int draw, int uidFlag)
{
    if (draw == 1) {
        stage_.repop(id);
    }
    stage_.animLocation(id, draw, uidFlag);
}
