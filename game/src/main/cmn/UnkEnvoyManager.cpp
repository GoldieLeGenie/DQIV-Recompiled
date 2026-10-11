#include "main/cmn/UnkEnvoyManager.hpp"
#include "main/cmn/UnkImmigrantTown.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/text/TextAPI.hpp"

UnkEnvoyManager data_020f0078;

THUMB UnkEnvoyManager::UnkEnvoyManager()
{
}

THUMB UnkEnvoyManager::~UnkEnvoyManager()
{
}

THUMB void UnkEnvoyManager::unkfunc_0203a34c(int index)
{
    mode_ = 0;
    index_ = index;
}

THUMB int UnkEnvoyManager::unkfunc_0203a354()
{
    return myEnvoy_.enable;
}

THUMB int UnkEnvoyManager::unkfunc_0203a358(int index)
{
    return envoy_[index].enable;
}

THUMB int UnkEnvoyManager::unkfunc_0203a364()
{
    for (int i = 0; i < 24; i++) {
        if (unkfunc_0203a358(i) == 0) {
            return i;
        }
    }
    return -1;
}

THUMB int UnkEnvoyManager::unkfunc_0203a388()
{
    int count = 0;
    for (int i = 0; i < 24; i++) {
        if (unkfunc_0203a358(i) == 1) {
            count++;
        }
    }
    return count;
}

THUMB void UnkEnvoyManager::unkfunc_0203a3a8(int index)
{
    mode_ = 0;
    unkfunc_0203a954(index);
    for (; index + 1 < 24; index++) {
        if (envoy_[index + 1].enable == 0) {
            break;
        }
        index_ = index;
        unkfunc_0203a574(envoy_[index + 1].enable);
        unkfunc_0203a58c(envoy_[index + 1].unique);
        unkfunc_0203a5bc(envoy_[index + 1].type);
        unkfunc_0203a604((char*)envoy_[index + 1].name);
        unkfunc_0203a674((char*)envoy_[index + 1].heroName);
        unkfunc_0203a6f4(envoy_[index + 1].sex);
        unkfunc_0203a730(envoy_[index + 1].age);
        unkfunc_0203a76c(envoy_[index + 1].skill);
        unkfunc_0203a7a8((char*)envoy_[index + 1].townName);
        unkfunc_0203a83c((char*)envoy_[index + 1].comment);
    }
    if (index < 24) {
        unkfunc_0203a954(index);
    }
    UnkImmigrantTown::getSingleton()->unkfunc_02037ca4();
}

THUMB void UnkEnvoyManager::unkfunc_0203a48c(UnkEnvoyData* data, int arg)
{
    mode_ = 0;
    unk_1664 = arg;
    if (data != NULL) {
        dss::memcpy(&received_, data, sizeof(UnkEnvoyData));
        int index = unkfunc_0203a9cc(received_.unique);
        if (index == -1) {
            index = unkfunc_0203a364();
            if (index == -1) {
                return;
            }
        }
        index_ = index;
        unkfunc_0203a574(1);
        unkfunc_0203a58c(received_.unique);
        unkfunc_0203a5bc(received_.type);
        unkfunc_0203a604((char*)received_.name);
        unkfunc_0203a674((char*)received_.heroName);
        unkfunc_0203a6f4(received_.sex);
        unkfunc_0203a730(received_.age);
        unkfunc_0203a76c(received_.skill);
        unkfunc_0203a7a8((char*)received_.townName);
        unkfunc_0203a83c((char*)received_.comment);
        flags_[received_.type] = 1;
    }
    UnkImmigrantTown::getSingleton()->unkfunc_02037ca4();
}

THUMB void UnkEnvoyManager::unkfunc_0203a574(int enable)
{
    if (mode_ == 1) {
        myEnvoy_.enable = enable;
    } else {
        envoy_[index_].enable = enable;
    }
}

THUMB void UnkEnvoyManager::unkfunc_0203a58c(unsigned int unique)
{
    if (mode_ == 1) {
        myEnvoy_.unique = unique;
    } else {
        envoy_[index_].unique = unique;
    }
}

THUMB unsigned int UnkEnvoyManager::unkfunc_0203a5a4()
{
    if (mode_ == 1) {
        return myEnvoy_.unique;
    }
    return envoy_[index_].unique;
}

THUMB void UnkEnvoyManager::unkfunc_0203a5bc(int type)
{
    if (mode_ == 1) {
        myEnvoy_.enable = 1;
        myEnvoy_.type = type;
    } else {
        envoy_[index_].enable = 1;
        envoy_[index_].type = type;
    }
}

THUMB int UnkEnvoyManager::unkfunc_0203a5ec()
{
    if (mode_ == 1) {
        return myEnvoy_.type;
    }
    return envoy_[index_].type;
}

THUMB void UnkEnvoyManager::unkfunc_0203a604(const char* name)
{
    if (mode_ == 1) {
        for (int i = 0; i < 26; i++) {
            myEnvoy_.name[i] = 0;
        }
        if (name != NULL) {
            dss::strcpy_s((char*)myEnvoy_.name, 26, name);
        }
    } else {
        for (int i = 0; i < 26; i++) {
            envoy_[index_].name[i] = 0;
        }
        if (name != NULL) {
            dss::strcpy_s((char*)envoy_[index_].name, 26, name);
        }
    }
}

THUMB unsigned char* UnkEnvoyManager::unkfunc_0203a65c()
{
    if (mode_ == 1) {
        return myEnvoy_.name;
    }
    return envoy_[index_].name;
}

THUMB void UnkEnvoyManager::unkfunc_0203a674(const char* name)
{
    if (mode_ == 1) {
        for (int i = 0; i < 26; i++) {
            myEnvoy_.heroName[i] = 0;
        }
        if (name != NULL) {
            dss::strcpy_s((char*)myEnvoy_.heroName, 26, name);
        }
    } else {
        for (int i = 0; i < 26; i++) {
            envoy_[index_].heroName[i] = 0;
        }
        if (name != NULL) {
            dss::strcpy_s((char*)envoy_[index_].heroName, 26, name);
        }
    }
}

THUMB unsigned char* UnkEnvoyManager::unkfunc_0203a6d8()
{
    if (mode_ == 1) {
        return myEnvoy_.heroName;
    }
    return envoy_[index_].heroName;
}

THUMB void UnkEnvoyManager::unkfunc_0203a6f4(int sex)
{
    if (mode_ == 1) {
        myEnvoy_.sex = sex;
    } else {
        envoy_[index_].sex = sex;
    }
}

THUMB int UnkEnvoyManager::unkfunc_0203a714()
{
    if (mode_ == 1) {
        return myEnvoy_.sex;
    }
    return envoy_[index_].sex;
}

THUMB void UnkEnvoyManager::unkfunc_0203a730(int age)
{
    if (mode_ == 1) {
        myEnvoy_.age = age;
    } else {
        envoy_[index_].age = age;
    }
}

THUMB int UnkEnvoyManager::unkfunc_0203a750()
{
    if (mode_ == 1) {
        return myEnvoy_.age;
    }
    return envoy_[index_].age;
}

THUMB void UnkEnvoyManager::unkfunc_0203a76c(int skill)
{
    if (mode_ == 1) {
        myEnvoy_.skill = skill;
    } else {
        envoy_[index_].skill = skill;
    }
}

THUMB int UnkEnvoyManager::unkfunc_0203a78c()
{
    if (mode_ == 1) {
        return myEnvoy_.skill;
    }
    return envoy_[index_].skill;
}

THUMB void UnkEnvoyManager::unkfunc_0203a7a8(const char* name)
{
    if (mode_ == 1) {
        myEnvoy_.enable = 1;
        for (int i = 0; i < 42; i++) {
            myEnvoy_.townName[i] = 0;
        }
        if (name != NULL) {
            dss::strcpy_s((char*)myEnvoy_.townName, 42, name);
        }
    } else {
        envoy_[index_].enable = 1;
        for (int i = 0; i < 42; i++) {
            envoy_[index_].townName[i] = 0;
        }
        if (name != NULL) {
            dss::strcpy_s((char*)envoy_[index_].townName, 42, name);
        }
    }
}

THUMB unsigned char* UnkEnvoyManager::unkfunc_0203a820()
{
    if (mode_ == 1) {
        return myEnvoy_.townName;
    }
    return envoy_[index_].townName;
}

THUMB void UnkEnvoyManager::unkfunc_0203a83c(const char* comment)
{
    Utf8Iterator src;
    src.unkfunc_020875ec((char*)comment);
    Utf8Iterator dst;
    char buf[0x200];
    dst.unkfunc_02087634(buf, 0x200);
    while (true) {
        int c = src.unkfunc_0208771c();
        src.unkfunc_020877b8();
        if (c == 0) {
            break;
        }
        switch (c) {
            case 0x24d5: c = 0x80; break;
            case 0x24d6: c = 0x81; break;
            case 0x24d7: c = 0x82; break;
            case 0x24d8: c = 0x83; break;
            case 0x24d9: c = 0x84; break;
            case 0x24c6: c = 0x85; break;
        }
        dst.unkfunc_02087734(c);
    }
    comment = buf;
    if (mode_ == 1) {
        for (int i = 0; i < 92; i++) {
            myEnvoy_.comment[i] = 0;
        }
        if (comment != NULL) {
            dss::strcpy_s((char*)myEnvoy_.comment, 92, comment);
        }
    } else {
        for (int i = 0; i < 92; i++) {
            envoy_[index_].comment[i] = 0;
        }
        if (comment != NULL) {
            dss::strcpy_s((char*)envoy_[index_].comment, 92, comment);
        }
    }
}

THUMB unsigned char* UnkEnvoyManager::unkfunc_0203a938()
{
    if (mode_ == 1) {
        return myEnvoy_.comment;
    }
    return envoy_[index_].comment;
}

THUMB void UnkEnvoyManager::unkfunc_0203a954(int index)
{
    index_ = index;
    envoy_[index].type = 0;
    for (int i = 0; i < 26; i++) {
        envoy_[index].name[i] = 0;
    }
    envoy_[index].sex = 0;
    envoy_[index].age = 8;
    envoy_[index].skill = 0;
    for (int i = 0; i < 26; i++) {
        envoy_[index].heroName[i] = 0;
    }
    for (int i = 0; i < 42; i++) {
        envoy_[index].townName[i] = 0;
    }
    for (int i = 0; i < 92; i++) {
        envoy_[index].comment[i] = 0;
    }
    envoy_[index].enable = 0;
}

THUMB int UnkEnvoyManager::unkfunc_0203a9cc(unsigned int unique)
{
    for (int i = 0; i < 24; i++) {
        if (unkfunc_0203a358(i) == 1 && unique == envoy_[i].unique) {
            return i;
        }
    }
    return -1;
}

THUMB void UnkEnvoyManager::unkfunc_0203aa00()
{
    if (backup_.enable == 1) {
        return;
    }
    backup_.enable = 1;
    backup_.type = myEnvoy_.type;
    backup_.unique = myEnvoy_.unique;
    dss::strcpy_s((char*)backup_.name, 26, (char*)myEnvoy_.name);
    backup_.sex = myEnvoy_.sex;
    backup_.age = myEnvoy_.age;
    backup_.skill = myEnvoy_.skill;
    dss::strcpy_s((char*)backup_.comment, 92, (char*)myEnvoy_.comment);
}

THUMB void UnkEnvoyManager::unkfunc_0203aa58()
{
    backup_.enable = 0;
    myEnvoy_.type = backup_.type;
    myEnvoy_.unique = backup_.unique;
    dss::strcpy_s((char*)myEnvoy_.name, 26, (char*)backup_.name);
    myEnvoy_.sex = backup_.sex;
    myEnvoy_.age = backup_.age;
    myEnvoy_.skill = backup_.skill;
    dss::strcpy_s((char*)myEnvoy_.comment, 92, (char*)backup_.comment);
}

THUMB void UnkEnvoyManager::unkfunc_0203aaac()
{
    backup_.enable = 0;
    backup_.type = 0;
    backup_.unique = 0;
    for (int i = 0; i < 26; i++) {
        backup_.name[i] = 0;
    }
    backup_.sex = 0;
    backup_.age = 8;
    backup_.skill = 0;
    for (int i = 0; i < 26; i++) {
        backup_.heroName[i] = 0;
    }
    for (int i = 0; i < 42; i++) {
        backup_.townName[i] = 0;
    }
    for (int i = 0; i < 92; i++) {
        backup_.comment[i] = 0;
    }
}

THUMB void UnkEnvoyManager::unkfunc_0203ab20(int type, int flag)
{
    flags_[type] = flag;
}

THUMB int UnkEnvoyManager::unkfunc_0203ab30(int type)
{
    return flags_[type];
}
