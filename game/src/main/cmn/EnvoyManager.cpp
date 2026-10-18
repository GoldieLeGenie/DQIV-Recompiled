#include "main/cmn/EnvoyManager.hpp"
#include "main/cmn/ImmigrantTown.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/text/TextAPI.hpp"

UnkEnvoyManager data_020f0078;

THUMB UnkEnvoyManager::UnkEnvoyManager()
{
}

THUMB UnkEnvoyManager::~UnkEnvoyManager()
{
}

THUMB void UnkEnvoyManager::selectEnvoy(int index)
{
    mode_ = 0;
    index_ = index;
}

THUMB int UnkEnvoyManager::isMyEnvoyEnabled()
{
    return myEnvoy_.enable;
}

THUMB int UnkEnvoyManager::isEnvoyEnabled(int index)
{
    return envoy_[index].enable;
}

THUMB int UnkEnvoyManager::findFreeEnvoy()
{
    for (int i = 0; i < 24; i++) {
        if (isEnvoyEnabled(i) == 0) {
            return i;
        }
    }
    return -1;
}

THUMB int UnkEnvoyManager::getEnvoyCount()
{
    int count = 0;
    for (int i = 0; i < 24; i++) {
        if (isEnvoyEnabled(i) == 1) {
            count++;
        }
    }
    return count;
}

THUMB void UnkEnvoyManager::removeEnvoy(int index)
{
    mode_ = 0;
    clearEnvoy(index);
    for (; index + 1 < 24; index++) {
        if (envoy_[index + 1].enable == 0) {
            break;
        }
        index_ = index;
        setEnable(envoy_[index + 1].enable);
        setUnique(envoy_[index + 1].unique);
        setType(envoy_[index + 1].type);
        setName((char*)envoy_[index + 1].name);
        setHeroName((char*)envoy_[index + 1].heroName);
        setSex(envoy_[index + 1].sex);
        setAge(envoy_[index + 1].age);
        setSkill(envoy_[index + 1].skill);
        setTownName((char*)envoy_[index + 1].townName);
        setComment((char*)envoy_[index + 1].comment);
    }
    if (index < 24) {
        clearEnvoy(index);
    }
    UnkImmigrantTown::getSingleton()->rebuildResidents();
}

THUMB void UnkEnvoyManager::receiveEnvoy(UnkEnvoyData* data, int arg)
{
    mode_ = 0;
    unk_1664 = arg;
    if (data != NULL) {
        dss::memcpy(&received_, data, sizeof(UnkEnvoyData));
        int index = findEnvoyByUnique(received_.unique);
        if (index == -1) {
            index = findFreeEnvoy();
            if (index == -1) {
                return;
            }
        }
        index_ = index;
        setEnable(1);
        setUnique(received_.unique);
        setType(received_.type);
        setName((char*)received_.name);
        setHeroName((char*)received_.heroName);
        setSex(received_.sex);
        setAge(received_.age);
        setSkill(received_.skill);
        setTownName((char*)received_.townName);
        setComment((char*)received_.comment);
        flags_[received_.type] = 1;
    }
    UnkImmigrantTown::getSingleton()->rebuildResidents();
}

THUMB void UnkEnvoyManager::setEnable(int enable)
{
    if (mode_ == 1) {
        myEnvoy_.enable = enable;
    } else {
        envoy_[index_].enable = enable;
    }
}

THUMB void UnkEnvoyManager::setUnique(unsigned int unique)
{
    if (mode_ == 1) {
        myEnvoy_.unique = unique;
    } else {
        envoy_[index_].unique = unique;
    }
}

THUMB unsigned int UnkEnvoyManager::getUnique()
{
    if (mode_ == 1) {
        return myEnvoy_.unique;
    }
    return envoy_[index_].unique;
}

THUMB void UnkEnvoyManager::setType(int type)
{
    if (mode_ == 1) {
        myEnvoy_.enable = 1;
        myEnvoy_.type = type;
    } else {
        envoy_[index_].enable = 1;
        envoy_[index_].type = type;
    }
}

THUMB int UnkEnvoyManager::getType()
{
    if (mode_ == 1) {
        return myEnvoy_.type;
    }
    return envoy_[index_].type;
}

THUMB void UnkEnvoyManager::setName(const char* name)
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

THUMB unsigned char* UnkEnvoyManager::getName()
{
    if (mode_ == 1) {
        return myEnvoy_.name;
    }
    return envoy_[index_].name;
}

THUMB void UnkEnvoyManager::setHeroName(const char* name)
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

THUMB unsigned char* UnkEnvoyManager::getHeroName()
{
    if (mode_ == 1) {
        return myEnvoy_.heroName;
    }
    return envoy_[index_].heroName;
}

THUMB void UnkEnvoyManager::setSex(int sex)
{
    if (mode_ == 1) {
        myEnvoy_.sex = sex;
    } else {
        envoy_[index_].sex = sex;
    }
}

THUMB int UnkEnvoyManager::getSex()
{
    if (mode_ == 1) {
        return myEnvoy_.sex;
    }
    return envoy_[index_].sex;
}

THUMB void UnkEnvoyManager::setAge(int age)
{
    if (mode_ == 1) {
        myEnvoy_.age = age;
    } else {
        envoy_[index_].age = age;
    }
}

THUMB int UnkEnvoyManager::getAge()
{
    if (mode_ == 1) {
        return myEnvoy_.age;
    }
    return envoy_[index_].age;
}

THUMB void UnkEnvoyManager::setSkill(int skill)
{
    if (mode_ == 1) {
        myEnvoy_.skill = skill;
    } else {
        envoy_[index_].skill = skill;
    }
}

THUMB int UnkEnvoyManager::getSkill()
{
    if (mode_ == 1) {
        return myEnvoy_.skill;
    }
    return envoy_[index_].skill;
}

THUMB void UnkEnvoyManager::setTownName(const char* name)
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

THUMB unsigned char* UnkEnvoyManager::getTownName()
{
    if (mode_ == 1) {
        return myEnvoy_.townName;
    }
    return envoy_[index_].townName;
}

THUMB void UnkEnvoyManager::setComment(const char* comment)
{
    Utf8Iterator src;
    src.setText((char*)comment);
    Utf8Iterator dst;
    char buf[0x200];
    dst.initBuffer(buf, 0x200);
    while (true) {
        int c = src.getChar();
        src.nextChar();
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
        dst.putChar(c);
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

THUMB unsigned char* UnkEnvoyManager::getComment()
{
    if (mode_ == 1) {
        return myEnvoy_.comment;
    }
    return envoy_[index_].comment;
}

THUMB void UnkEnvoyManager::clearEnvoy(int index)
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

THUMB int UnkEnvoyManager::findEnvoyByUnique(unsigned int unique)
{
    for (int i = 0; i < 24; i++) {
        if (isEnvoyEnabled(i) == 1 && unique == envoy_[i].unique) {
            return i;
        }
    }
    return -1;
}

THUMB void UnkEnvoyManager::backupMyEnvoy()
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

THUMB void UnkEnvoyManager::restoreMyEnvoy()
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

THUMB void UnkEnvoyManager::clearBackup()
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

THUMB void UnkEnvoyManager::setTypeSelectable(int type, int flag)
{
    flags_[type] = flag;
}

THUMB int UnkEnvoyManager::isTypeSelectable(int type)
{
    return flags_[type];
}
