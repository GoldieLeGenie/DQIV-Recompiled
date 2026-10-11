#pragma ipa file
#include "ov000/town/TownWindowSystem.hpp"
#include "ov000/town/TownImageMap.hpp"
#include "ov000/town/TownShopListMap.hpp"
#include "ov000/town/UnkImageMap.hpp"
#include "main/global/Global.hpp"

static TownImageMap imageMap;
static TownShopListMap shopListMap;
static UnkImageMap_02141f6c unkImageMap;

ARM TownWindowSystem::TownWindowSystem()
{
    unk_70 = 0;
}

ARM TownWindowSystem* TownWindowSystem::getSingleton()
{
    static TownWindowSystem m_singleton;
    return &m_singleton;
}

ARM void TownWindowSystem::initialize()
{
    cmdWindow_.initialize();
    cmdWindow_.registImageMap(&imageMap);
    cmdWindow_.registShopList(&shopListMap);
    imageMap.initialize();
    imageMap.calcTargetPos();
    if (g_Global.fightStadiumResult_) {
        cmdWindow_.changeShopMenuPhase(0xf);
    }
    town_message_ = 0;
}

ARM void TownWindowSystem::terminate()
{
    imageMap.exitFloor();
    imageMap.cleanup();
    shopListMap.cleanup();
    unkImageMap.cleanup();
    cmdWindow_.terminate();
}

ARM void TownWindowSystem::execute()
{
    imageMap.execute();
    shopListMap.execute();
    unkImageMap.execute();
    cmdWindow_.execute();
}

ARM void TownWindowSystem::draw()
{
    imageMap.draw();
    shopListMap.draw();
    unkImageMap.draw();
}

ARM void TownWindowSystem::openMessage(int index, int count)
{
    town_message_ = 1;
    cmdWindow_.MESSAGEWINDOW(index, count);
}

ARM void TownWindowSystem::addCommonMessage(int index)
{
    cmdWindow_.ADDCOMMONWINDOW(index);
}

ARM void TownWindowSystem::openCommonMessage()
{
    cmdWindow_.MESSAGECOMMONWINDOW();
    town_message_ = 1;
}

ARM void TownWindowSystem::serialCommonMessage(int index)
{
    cmdWindow_.SERIALCOMMONWINDOW(index);
    town_message_ = 1;
}

ARM void TownWindowSystem::waitCommonMessage()
{
    cmdWindow_.WAITCOMMONWINDOW();
}

ARM void TownWindowSystem::clearCommonMessage()
{
    cmdWindow_.CLEARCOMMONWINDOW();
}

ARM bool TownWindowSystem::isWait()
{
    return cmdWindow_.isWAITWINDOW();
}

ARM bool TownWindowSystem::isOpen()
{
    return cmdWindow_.isOPENWINDOW();
}
