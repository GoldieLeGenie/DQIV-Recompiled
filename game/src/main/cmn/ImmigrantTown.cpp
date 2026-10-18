#include "main/cmn/ImmigrantTown.hpp"
#include "main/cmn/EnvoyManager.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/GameFlag.hpp"

ARM void UnkImmigrantTown::rebuildResidents()
{
    for (int i = 0; i < 42; i++) {
        type_[i] = 0xff;
        x_[i] = 0xff;
        y_[i] = 0xff;
    }
    count_ = 0;
    int count = data_020f0078.getEnvoyCount();
    for (int i = 0; i < count; i++) {
        data_020f0078.selectEnvoy(i);
        addResident(data_020f0078.getType());
    }
}

ARM void UnkImmigrantTown::resetPositions()
{
    queueFlag_ = 0;
    queueCount_ = 0;
    for (int i = 0; i < 42; i++) {
        x_[i] = 0xff;
    }
}

ARM void UnkImmigrantTown::addResident(int type)
{
    type_[count_] = type;
    count_++;
}

ARM int UnkImmigrantTown::hasResidentType(int type)
{
    for (int i = 0; i < count_; i++) {
        if (type == type_[i]) {
            return 1;
        }
    }
    return 0;
}

ARM UnkImmigrantTown* UnkImmigrantTown::getSingleton()
{
    static UnkImmigrantTown instance;
    return &instance;
}

ARM void UnkImmigrantTown::placeNextResident(int x, int y)
{
    for (int i = 0; i < queueCount_; i++) {
        if (x == queueX_[i] && y == queueY_[i]) {
            return;
        }
    }
    for (int i = 0; i < count_; i++) {
        if (x_[i] == 0xff) {
            x_[i] = x;
            y_[i] = y;
            return;
        }
    }
}

ARM void UnkImmigrantTown::placePriorityResident(int x, int y, int count, int* types)
{
    for (int i = 0; i < count_; i++) {
        for (int j = 0; j < count; j++) {
            if (matchesResidentCategory(i, types[j]) == 1) {
                x_[i] = x;
                y_[i] = y;
                return;
            }
            if (type_[i] == types[j] && x_[i] == 0xff) {
                x_[i] = x;
                y_[i] = y;
                return;
            }
        }
    }
    queueFlag_ = 1;
    queueX_[queueCount_] = x;
    queueY_[queueCount_] = y;
    queueCount_++;
}

ARM int UnkImmigrantTown::getResidentPoseAt(int x, int y)
{
    for (int i = 0; i < count_; i++) {
        if (x == x_[i] && y == y_[i]) {
            return getTypePose(type_[i]);
        }
    }
    return 0xff;
}

ARM int UnkImmigrantTown::getResidentIndexAt(int x, int y)
{
    for (int i = 0; i < count_; i++) {
        if (x == x_[i] && y == y_[i]) {
            return i;
        }
    }
    return 0;
}

ARM int UnkImmigrantTown::getTypePose(int type)
{
    return status::excelParam.surechigai_[type].index;
}

ARM void UnkImmigrantTown::placeQueuedResidents()
{
    if (queueFlag_ != 1) {
        return;
    }
    for (int i = 0; i < queueCount_; i++) {
        for (int j = 0; j < count_; j++) {
            if (x_[j] == 0xff) {
                x_[j] = queueX_[queueCount_ - 1 - i];
                y_[j] = queueY_[queueCount_ - 1 - i];
                break;
            }
        }
    }
    queueFlag_ = 0;
}

ARM int UnkImmigrantTown::matchesResidentCategory(int index, int type)
{
    if (x_[index] != 0xff) {
        return 0;
    }
    if (type < 100) {
        return 0;
    }
    char flags = status::excelParam.surechigai_[type_[index]].byte_1;
    int age = (char)((flags & 0xc) >> 2);
    int sex = (char)((flags & 0x30) >> 4);
    switch (sex) {
        case 0: sex = 0x67; break;
        case 1: sex = 0x66; break;
        case 2: sex = 0x68; break;
    }
    switch (age) {
        case 0:
        case 2: age = 0x64; break;
        case 1: age = 0x65; break;
    }
    switch (type) {
        case 100: return age == 0x64;
        case 101: return age == 0x65;
        case 103: return age == 0x64 && sex == 0x67;
        case 102: return age == 0x64 && sex == 0x66;
    }
    return 0;
}

ARM int UnkImmigrantTown::getTownLevel()
{
    if (g_AreaFlag.check(0x1f8) == 1) {
        return 5;
    }
    if (g_AreaFlag.check(0x1f6) == 1) {
        return 4;
    }
    if (g_AreaFlag.check(0x1ea) == 1) {
        return 3;
    }
    if (g_AreaFlag.check(0x1e4) == 1) {
        return 2;
    }
    if (g_AreaFlag.check(0x1e1) == 1) {
        return 1;
    }
    return 0;
}
