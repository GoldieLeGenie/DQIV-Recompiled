#include "main/global/Global.hpp"

GlobalGamePartManager data_0210bc18;

ARM GlobalGamePartManager::GlobalGamePartManager()
{
    for (int i = 0; i < 8; i++) {
        part_[i] = 0;
    }
}

ARM GlobalGamePartManager::~GlobalGamePartManager()
{
}

ARM void GlobalGamePartManager::unkfunc_020581f4()
{
    for (int i = 0; i < 8; i++) {
        if (part_[i] != 0) {
            part_[i]->update();
            if (part_[i]->isEnd()) {
                part_[i] = 0;
            }
        }
    }
}

ARM void GlobalGamePartManager::unkfunc_02058244()
{
    for (int i = 0; i < 8; i++) {
        if (part_[i] != 0) {
            part_[i]->draw();
            if (part_[i]->isEnd()) {
                part_[i] = 0;
            }
        }
    }
}

ARM void GlobalGamePartManager::unkfunc_02058294(UnkGlobalPart* part)
{
    for (int i = 0; i < 8; i++) {
        if (part_[i] == 0) {
            part_[i] = part;
            return;
        }
    }
}

ARM void GlobalGamePartManager::unkfunc_020582b8(UnkGlobalPart* part)
{
    for (int i = 0; i < 8; i++) {
        if (part_[i] == part) {
            part_[i] = 0;
            return;
        }
    }
}
