#pragma ipa file
#include "ov000/town/riseup/TownRiseup.hpp"

THUMB BillboardItemResource::BillboardItemResource()
{
}

THUMB BillboardItemResource::~BillboardItemResource()
{
}

THUMB void BillboardItemResource::initialize()
{
    maxStorage_ = 4;
    ResourceStorage::initialize();
}

THUMB void BillboardItemResource::terminate()
{
    ResourceStorage::terminate();
}

THUMB BillboardItem* BillboardItemResource::getResource(int id)
{
    return &m_item[ResourceStorage::getResource(id)];
}

THUMB int BillboardItemResource::loadResource(int id)
{
    int area = getEmptyArea();
    if (id <= 0xa1) {
        m_item[area].setup("data/RISE/item.tex", id);
    } else {
        switch (id) {
        case 200:
            m_item[area].setup("data/rise/gold.tex");
            break;
        case 201:
            m_item[area].setup("data/rise/exclamation.tex");
            break;
        case 202:
            m_item[area].setup("data/rise/question.tex");
            break;
        case 999:
            m_item[area].setup("data/rise/number.tex");
            break;
        }
    }
    return area;
}

THUMB void BillboardItemResource::releaseResource(int id)
{
    m_item[getResourceArea(id)].cleanup();
}
