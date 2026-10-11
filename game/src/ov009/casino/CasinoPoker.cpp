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
    upperSprite_.unkfunc_02057d60(filename, 0);
    upperSprite_.unkfunc_02057ee8();
    upperSprite_.unkfunc_02057f18(0x3e);
    upperSprite_.unkfunc_02057f30(0);
    upperSprite_.sprite_.unk_32 = 1;
    upperSprite_.unkfunc_02057ea8(0, 0, 0x100, 0xc0);
    upperSprite_.unkfunc_02057e98(0x100, 0xc0);
    upperSprite_.unkfunc_02057e58(&CasinoSystem::getSingleton()->render_);
}

ARM void CasinoPoker::terminate()
{
    upperSprite_.unkfunc_02057e34();
    CasinoPokerDraw::getSingleton()->terminate();
}

ARM void CasinoPoker::execute()
{
}

ARM void CasinoPoker::draw()
{
    CasinoPokerDraw::getSingleton()->draw();
    unkfunc_020847e8();
    upperSprite_.unkfunc_02057ec0();
}

ARM char* CasinoPoker::getStageName()
{
    return stagePoker;
}
