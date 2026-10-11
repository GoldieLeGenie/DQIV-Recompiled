#pragma ipa file
#include "ov009/casino/CasinoSystem.hpp"
#include "ov009/casino/CasinoSlot.hpp"
#include "ov009/casino/CasinoPoker.hpp"
#include "ov009/casino/CasinoCamera.hpp"
#include "ov009/casino/CasinoStage.hpp"
#include "main/global/Global.hpp"
#include "main/sound/SoundManager.hpp"

ARM void CasinoSystem::unkfunc_021227cc()
{
}

ARM CasinoSystem::CasinoSystem()
{
}

ARM CasinoSystem::~CasinoSystem()
{
}

ARM CasinoSystem* CasinoSystem::getSingleton()
{
    static CasinoSystem casinoSystem;
    return &casinoSystem;
}

ARM void CasinoSystem::initialize()
{
    render_.unkfunc_02084efc();
    if (g_Global.getMinigame() == 0) {
        minigame = CasinoPoker::getSingleton();
    } else {
        minigame = CasinoSlot::getSingleton();
    }
    minigame->render_ = &render_;
    CasinoCamera::getSingleton()->initialize();
    CasinoStage::getSingleton()->initialize();
    minigame->initialize();
    SoundManager::townPlay();
}

ARM void CasinoSystem::terminate()
{
    minigame->terminate();
    CasinoStage::getSingleton()->terminate();
    CasinoCamera::getSingleton()->terminate();
    render_.unkfunc_02084f50();
}

ARM void CasinoSystem::execute()
{
    minigame->execute();
}

ARM void CasinoSystem::draw()
{
    CasinoCamera::getSingleton()->draw();
    minigame->draw();
    render_.unkfunc_02084fa4();
}
