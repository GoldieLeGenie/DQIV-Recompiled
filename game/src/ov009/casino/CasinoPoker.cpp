#pragma ipa file
#include "ov009/casino/CasinoPoker.hpp"
#include "main/dss/RenderObject.hpp"
#include "ov009/casino/CasinoPokerDraw.hpp"
#include "ov009/casino/PokerManager.hpp"
#include "ov009/casino/CasinoSystem.hpp"
#include "main/menu/MaterielMenuWindowManager.hpp"
#include "main/dss/DssUtils.hpp"

static char nativePokerStage[] = "ev04";
char* CasinoPoker::stagePoker = nativePokerStage;

ARM CasinoPoker::CasinoPoker()
{
}

ARM CasinoPoker::~CasinoPoker()
{
}

ARM CasinoPoker* CasinoPoker::getSingleton()
{
    static CasinoPoker casinoPoker;
    return &casinoPoker;
}

ARM void CasinoPoker::initialize()
{
    char filename[0x80];
    CasinoPokerDraw::getSingleton()->initialize();
    PokerManager::getSingleton()->allClear();
    MaterielMenu_WINDOW_MANAGER::getSingleton()->openMaterielWindow(14);
    dss::sprintf_s(filename, sizeof(filename), "data/minigame/poker/casino_upper.tex");
    upperSprite_.loadFile(filename, 0);
    upperSprite_.setSubScreen();
    upperSprite_.setPolygonID(0x3e);
    upperSprite_.setDepth(0);
    upperSprite_.sprite_.color0Opaque_ = 1;
    upperSprite_.setTexCoords(0, 0, 0x100, 0xc0);
    upperSprite_.setSize(0x100, 0xc0);
    upperSprite_.attachRender(&CasinoSystem::getSingleton()->render_);
}

ARM void CasinoPoker::terminate()
{
    upperSprite_.unload();
    CasinoPokerDraw::getSingleton()->terminate();
}

ARM void CasinoPoker::execute()
{
}

ARM void CasinoPoker::draw()
{
    CasinoPokerDraw::getSingleton()->draw();
    setupOrtho2D();
    upperSprite_.draw();
}

ARM char* CasinoPoker::getStageName()
{
    return stagePoker;
}
