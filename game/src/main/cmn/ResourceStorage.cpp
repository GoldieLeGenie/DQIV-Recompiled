#include "main/cmn/ResourceStorage.hpp"

int cmn::ResourceStorage::maxStorage_;

ARM cmn::ResourceStorage::ResourceStorage()
{
}

ARM cmn::ResourceStorage::~ResourceStorage()
{
}

ARM void cmn::ResourceStorage::initialize()
{
    for (unsigned int i = 0; i < maxStorage_; i++) {
        refCounter_[i] = 0;
        index_[i] = 0;
    }
    counter_ = 0;
}

ARM void cmn::ResourceStorage::terminate()
{
}

ARM int cmn::ResourceStorage::getResource(int id)
{
    int area = -1;
    for (unsigned int i = 0; i < maxStorage_; i++) {
        if (id == index_[i]) {
            area = i;
        }
    }
    if (area < 0) {
        area = loadResource(id);
        index_[area] = id;
        counter_++;
    }
    refCounter_[area]++;
    return area;
}

ARM int cmn::ResourceStorage::getRefCounter(int id)
{
    int area = -1;
    for (unsigned int i = 0; i < maxStorage_; i++) {
        if (id == index_[i]) {
            area = i;
        }
    }
    if (area < 0) {
        return 0;
    }
    return refCounter_[area];
}

ARM void cmn::ResourceStorage::restoreResource(int id)
{
    int area = getResourceArea(id);
    refCounter_[area]--;
    if (refCounter_[area] == 0) {
        releaseResource(id);
        index_[area] = 0;
        counter_--;
    }
}

ARM int cmn::ResourceStorage::getEmptyArea()
{
    for (unsigned int i = 0; i < maxStorage_; i++) {
        if (index_[i] == 0) {
            return i;
        }
    }
    return 0;
}

ARM int cmn::ResourceStorage::getResourceArea(int id)
{
    for (unsigned int i = 0; i < maxStorage_; i++) {
        if (id == index_[i]) {
            return i;
        }
    }
    return 0;
}
