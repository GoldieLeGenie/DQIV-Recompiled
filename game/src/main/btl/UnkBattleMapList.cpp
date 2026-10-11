#include "main/btl/UnkBattleMapList.hpp"
#include "main/dss/DssUtils.hpp"

THUMB UnkBattleMapList::UnkBattleMapList()
{
    group_ = 0;
    map_ = 0;
    unk_28 = 0;
    loaded_ = 0;
}

THUMB UnkBattleMapList::~UnkBattleMapList()
{
}

THUMB void UnkBattleMapList::unkfunc_02048e34(const char* filename)
{
    unkfunc_0205726c(filename, 0);
    group_ = 0;
    map_ = 0;
    unk_28 = 0;
    loaded_ = 1;
}

THUMB int UnkBattleMapList::unkfunc_02048e4c()
{
    return loaded_;
}

THUMB char* UnkBattleMapList::unkfunc_02048e50()
{
    return unkfunc_02057368(group_ + map_, 1);
}

THUMB void UnkBattleMapList::unkfunc_02048e60()
{
    int i = 0;
    char* name = unkfunc_02057368(group_ + map_, 1);
    while (true) {
        if (unkfunc_02057368(group_ + map_ + i, 1) == NULL) {
            return;
        }
        if (dss::strcmp(name, unkfunc_02057368(group_ + map_ + i, 1)) != 0) {
            map_ += i;
            unk_28 = 0;
            return;
        }
        i++;
        char* first = unkfunc_02057368(group_, 0);
        char* text = unkfunc_02057368(group_ + map_ + i, 0);
        if (text == NULL) {
            return;
        }
        if (first[0] != text[0]) {
            return;
        }
    }
}

THUMB void UnkBattleMapList::unkfunc_02048ee0()
{
    int i = 0;
    int offset = 0;
    if (map_ == 0) {
        return;
    }
    char* prev = unkfunc_02057368(group_ + map_, 1);
    char* name = unkfunc_02057368(group_, 1);
    while (true) {
        char* text = unkfunc_02057368(group_ + offset + i, 1);
        if (dss::strcmp(name, text) != 0) {
            if (dss::strcmp(prev, text) == 0) {
                map_ = offset;
                unk_28 = 0;
                return;
            }
            offset += i;
            name = unkfunc_02057368(group_ + offset, 1);
            i = 0;
        } else {
            i++;
        }
        char* first = unkfunc_02057368(group_, 0);
        char* cur = unkfunc_02057368(group_ + offset + i, 0);
        if (cur == NULL) {
            return;
        }
        if (first[0] != cur[0]) {
            return;
        }
    }
}

THUMB char* UnkBattleMapList::unkfunc_02048f7c()
{
    return unkfunc_02057368(group_ + map_ + unk_28, 2);
}

THUMB void UnkBattleMapList::unkfunc_02048f90()
{
    unkfunc_02057368(group_, 0);
    char* name = unkfunc_02057368(group_ + map_, 1);
    if (unkfunc_02057368(group_ + map_ + unk_28 + 1, 2) == NULL) {
        return;
    }
    char* next = unkfunc_02057368(group_ + map_ + unk_28 + 1, 2);
    if (dss::strncmp(name, next, dss::strlen(name)) == 0) {
        unk_28++;
    }
}

THUMB void UnkBattleMapList::unkfunc_02048ff8()
{
    unkfunc_02057368(group_, 0);
    char* name = unkfunc_02057368(group_ + map_, 1);
    int row = group_ + map_ + unk_28 - 1;
    if (row < 0) {
        return;
    }
    char* prev = unkfunc_02057368(row, 2);
    if (dss::strncmp(name, prev, dss::strlen(name)) == 0) {
        unk_28--;
    }
}
