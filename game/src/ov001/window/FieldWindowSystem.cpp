#pragma ipa file
#include "ov001/window/FieldWindowSystem.hpp"
#include "ov001/window/FieldImageMap.hpp"
#include "ov001/fld/FieldSystem.hpp"

static FieldImageMap imageMap;

ARM FieldWindowSystem::FieldWindowSystem()
{
}

ARM FieldWindowSystem* FieldWindowSystem::getSingleton()
{
    static FieldWindowSystem m_singleton;
    return &m_singleton;
}

ARM void FieldWindowSystem::initialize()
{
    imageMap.setup(FieldSystem::getSingleton());
    cmdWindow_.initialize();
    cmdWindow_.registImageMap(&imageMap);
}

ARM void FieldWindowSystem::terminate()
{
    cmdWindow_.terminate();
    imageMap.cleanup();
}

ARM void FieldWindowSystem::execute()
{
    cmdWindow_.execute();
    imageMap.execute();
}

ARM void FieldWindowSystem::draw()
{
    imageMap.draw();
}

ARM void FieldWindowSystem::openMessage(int index, int count)
{
    cmdWindow_.MESSAGEWINDOW(index, count);
}

ARM void FieldWindowSystem::addCommonMessage(int index)
{
    cmdWindow_.ADDCOMMONWINDOW(index);
}

ARM void FieldWindowSystem::openCommonMessage()
{
    cmdWindow_.MESSAGECOMMONWINDOW();
}

ARM bool FieldWindowSystem::isOpen()
{
    return cmdWindow_.isOPENWINDOW();
}

ARM void FieldWindowSystem::setMenuPermit(bool flag)
{
    cmdWindow_.setMenuPermit(flag);
}

ARM void FieldWindowSystem::clearAllMap()
{
    imageMap.clearAllMap();
}
