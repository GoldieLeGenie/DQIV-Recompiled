#include "main/global/StageLink.hpp"
#include "main/fld/MapLink.hpp"
#include "main/dss/DssUtils.hpp"

int StageLink::townExitIndex_ = -1;
int StageLink::fieldSymbolIndex_ = -1;
DataObject StageLink::mapLinkData_;
static CMapLink g_mapLink_;

THUMB void StageLink::initialize()
{
    mapLinkData_.setup("data/map/map_link.bin", 1, 0);
    g_mapLink_.setup(mapLinkData_.getAddr());
}

THUMB char* StageLink::getName(const char* name, int index)
{
    if (dss::strcmp(name, "world")) {
        return g_mapLink_.search(name, index);
    }
    return g_mapLink_.search(name, index | 0x7000);
}

THUMB int StageLink::getSymbolIndex()
{
    return g_mapLink_.m_exit_id;
}

THUMB void StageLink::setTownExitIndex(int index)
{
    townExitIndex_ = index;
}

THUMB void StageLink::resetTownExitIndex()
{
    townExitIndex_ = -1;
}

THUMB int StageLink::getTownExitIndex()
{
    return townExitIndex_;
}

THUMB void StageLink::setFieldSymbolIndex(int index)
{
    fieldSymbolIndex_ = index;
}

THUMB int StageLink::getFieldSymbolIndex()
{
    return fieldSymbolIndex_;
}
