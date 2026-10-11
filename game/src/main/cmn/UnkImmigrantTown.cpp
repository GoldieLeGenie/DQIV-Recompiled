#include "main/cmn/UnkImmigrantTown.hpp"
#include "main/cmn/UnkEnvoyManager.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/GameFlag.hpp"

ARM void UnkImmigrantTown::unkfunc_02037ca4()
{
    for (int i = 0; i < 42; i++) {
        type_[i] = 0xff;
        x_[i] = 0xff;
        y_[i] = 0xff;
    }
    count_ = 0;
    int count = data_020f0078.unkfunc_0203a388();
    for (int i = 0; i < count; i++) {
        data_020f0078.unkfunc_0203a34c(i);
        unkfunc_02037d50(data_020f0078.unkfunc_0203a5ec());
    }
}

ARM void UnkImmigrantTown::unkfunc_02037d28()
{
    queueFlag_ = 0;
    queueCount_ = 0;
    for (int i = 0; i < 42; i++) {
        x_[i] = 0xff;
    }
}

ARM void UnkImmigrantTown::unkfunc_02037d50(int type)
{
    type_[count_] = type;
    count_++;
}

ARM int UnkImmigrantTown::unkfunc_02037d6c(int type)
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

ARM void UnkImmigrantTown::unkfunc_02037db0(int x, int y)
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

ARM void UnkImmigrantTown::unkfunc_02037e20(int x, int y, int count, int* types)
{
    for (int i = 0; i < count_; i++) {
        for (int j = 0; j < count; j++) {
            if (unkfunc_02038034(i, types[j]) == 1) {
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

ARM int UnkImmigrantTown::unkfunc_02037ef4(int x, int y)
{
    for (int i = 0; i < count_; i++) {
        if (x == x_[i] && y == y_[i]) {
            return unkfunc_02037f84(type_[i]);
        }
    }
    return 0xff;
}

ARM int UnkImmigrantTown::unkfunc_02037f40(int x, int y)
{
    for (int i = 0; i < count_; i++) {
        if (x == x_[i] && y == y_[i]) {
            return i;
        }
    }
    return 0;
}

ARM int UnkImmigrantTown::unkfunc_02037f84(int type)
{
    return status::excelParam.surechigai_[type].index;
}

ARM void UnkImmigrantTown::unkfunc_02037f98()
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

ARM int UnkImmigrantTown::unkfunc_02038034(int index, int type)
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

ARM int UnkImmigrantTown::unkfunc_02038140()
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
