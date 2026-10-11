#include "main/profile/SaveLoad.hpp"
#include "main/data/DataObject.hpp"
#include "main/dss/UnkBgBuffer.hpp"
#include "main/dss/UnkSleep.hpp"
#include "main/text/TextAPI.hpp"
#include "main/status/StageStatus.hpp"
#include "nitro/card.hpp"

#define BACKUP_LOCK_ID 0x1234
#define BANK_SIZE 0x3c00
#define BANK_OFFSET(bank) ((bank) * BANK_SIZE + 0x400)
#define PROFILE_MAGIC 0x65747261
#define PROFILE_VERSION 0x0bf7ff5c

int profile::SaveLoad::asyncBank_;
int profile::SaveLoad::catalogRecent_;
int profile::SaveLoad::lastSleep_;
profile::Profile* profile::SaveLoad::asyncBuff_;
int profile::SaveLoad::asyncResult_;
profile::CatalogView profile::SaveLoad::catalogView_[3];
static char s_name[0x200];

THUMB int profile::SaveLoad::unkfunc_0202b684(int bank)
{
    unsigned char buff[0x200];
    PROFILE_SYSTEM* system = (PROFILE_SYSTEM*)buff;
    if (!memoryload(BANK_OFFSET(bank), buff, 0x200)) {
        return -3;
    }
    if (system->MAGIC == PROFILE_MAGIC && system->VER == PROFILE_VERSION) {
        return 1;
    }
    return -1;
}

THUMB profile::CatalogView* profile::SaveLoad::getCatalogView()
{
    unsigned char* buff = (unsigned char*)unkfunc_0207f834(&data_0211a60c, BANK_SIZE * 3, 0x20);
    if (buff == NULL) {
        return NULL;
    }
    if (!memoryload(0x400, buff, BANK_SIZE * 3)) {
        if (buff != NULL) {
            unkfunc_0207f840(&data_0211a60c, buff);
        }
        return NULL;
    }
    PROFILE_RECORD* records[3];
    records[0] = (PROFILE_RECORD*)buff;
    records[1] = (PROFILE_RECORD*)(buff + BANK_SIZE);
    records[2] = (PROFILE_RECORD*)(buff + BANK_SIZE * 2);
    CatalogView* views[3];
    views[0] = &catalogView_[0];
    views[1] = &catalogView_[1];
    views[2] = &catalogView_[2];
    for (int i = 0; i < 3; i++) {
        if (records[i]->system.MAGIC == PROFILE_MAGIC && records[i]->system.VER == PROFILE_VERSION) {
            int town;
            views[i]->useFlag_ = 1;
            views[i]->id_ = records[i]->system.BOOKNO;
            switch (records[i]->system.SAVETYPE) {
                case SAVETYPE_CHURCH:
                    town = getPlaceNameNo(records[i]->party.CHAPTER, (char*)records[i]->party.RESTART);
                    break;
                case SAVETYPE_INTERVAL:
                    town = getPlaceNameByChapter(records[i]->party.CHAPTER);
                    break;
                default:
                    town = 0x80000032;
                    break;
            }
            int sex = 1;
            if (records[i]->party.SEX != 0) {
                sex = 2;
            }
            dss::strcpy_s(views[i]->name_, sizeof(views[i]->name_), unkfunc_0202bbe0(sex, records[i]->party.NAME));
            views[i]->savetype_ = records[i]->system.SAVETYPE;
            views[i]->chapter_ = records[i]->party.CHAPTER;
            for (int j = 0; j < 14; j++) {
                if (sex == records[i]->party.PARTY[j]) {
                    views[i]->level_ = records[i]->player[j].LEVEL;
                }
            }
            views[i]->town_ = town;
            views[i]->time_ = records[i]->party.PLAYTIME;
        } else {
            views[i]->useFlag_ = 0;
        }
    }
    unkfunc_0207f840(&data_0211a60c, buff);
    catalogRecent_ = getSaveBank();
    return catalogView_;
}

THUMB int profile::SaveLoad::getCatalogRecent()
{
    return catalogRecent_;
}

THUMB int profile::SaveLoad::loadbank(int bank)
{
    if (!isCardOK()) {
        return 0;
    }
    Profile* padr = (Profile*)unkfunc_0207f834(&data_0211a60c, sizeof(Profile), 0x20);
    if (padr == NULL) {
        return 0;
    }
    memoryload(BANK_OFFSET(bank), &padr->profiledata_, BANK_SIZE);
    int result = padr->deliverDATA(bank);
    unkfunc_0207f840(&data_0211a60c, padr);
    return result;
}

THUMB bool profile::SaveLoad::savebank(int bank, SAVETYPE type)
{
    if (!isCardOK()) {
        return 0;
    }
    Profile* padr = (Profile*)unkfunc_0207f834(&data_0211a60c, sizeof(Profile), 0x20);
    if (padr == NULL) {
        return 0;
    }
    padr->collectDATA(bank, type);
    bool result = memorysave(BANK_OFFSET(bank), &padr->profiledata_, BANK_SIZE);
    unkfunc_0207f840(&data_0211a60c, padr);
    if (bank < 3) {
        setSaveBank(bank);
    } else {
        setSaveBank(g_Stage.profileBank_);
    }
    return result;
}

THUMB void profile::SaveLoad::savebankAsync(int bank, SAVETYPE type)
{
    asyncResult_ = 1;
    asyncBuff_ = NULL;
    asyncBank_ = bank;
    if (!isCardOK()) {
        asyncResult_ = 0;
        return;
    }
    asyncBuff_ = (Profile*)unkfunc_0207f834(&data_0211a60c, sizeof(Profile), 0x20);
    if (asyncBuff_ == NULL) {
        asyncResult_ = 0;
        return;
    }
    asyncBuff_->collectDATA(bank, type);
    memorysaveAsync(BANK_OFFSET(bank), &asyncBuff_->profiledata_, BANK_SIZE);
}

THUMB int profile::SaveLoad::savebankAsyncWait()
{
    if (asyncResult_ == 0) {
        return 1;
    }
    return memorysaveAsyncWait();
}

THUMB int profile::SaveLoad::savebankAsyncResult()
{
    if (asyncBuff_ != NULL) {
        unkfunc_0207f840(&data_0211a60c, asyncBuff_);
        asyncBuff_ = NULL;
    }
    if (asyncResult_ == 0) {
        return 0;
    }
    asyncResult_ = memorysaveAsyncResult();
    if (asyncBank_ < 3) {
        setSaveBank(asyncBank_);
    } else {
        setSaveBank(g_Stage.profileBank_);
    }
    return asyncResult_;
}

THUMB bool profile::SaveLoad::killbank(int bank)
{
    unsigned char* addr = (unsigned char*)unkfunc_0207f834(&data_0211a60c, BANK_SIZE, 0x20);
    dss::memset(addr, 0xff, BANK_SIZE);
    bool result = memorysave(BANK_OFFSET(bank), addr, 0x200);
    unkfunc_0207f840(&data_0211a60c, addr);
    return result;
}

THUMB int profile::SaveLoad::unkfunc_0202ba3c(int bank)
{
    if (!isCardOK()) {
        return 0;
    }
    Profile* padr = (Profile*)unkfunc_0207f834(&data_0211a60c, sizeof(Profile), 0x20);
    if (padr == NULL) {
        return 0;
    }
    int result = memoryload(BANK_OFFSET(bank), &padr->profiledata_, BANK_SIZE);
    if (padr->isValidData()) {
        result = padr->calcCheckSum();
    }
    unkfunc_0207f840(&data_0211a60c, padr);
    return result;
}

THUMB bool profile::SaveLoad::memorysave(unsigned int seek, void* buff, unsigned int size)
{
    int sleep = unkfunc_02089568();
    unkfunc_02089558(0);
    func_0205d6f8(BACKUP_LOCK_ID);
    func_0205dc74(CARD_BACKUP_TYPE_EEPROM_512KBITS);
    CARD_WriteAndVerifyBackup(seek, buff, size);
    int result = func_0205d6ac();
    func_0205d708(BACKUP_LOCK_ID);
    unkfunc_02089558(sleep);
    if (result == 0) {
        return 1;
    }
    return 0;
}

THUMB void profile::SaveLoad::memorysaveAsync(unsigned int seek, void* buff, unsigned int size)
{
    lastSleep_ = unkfunc_02089568();
    unkfunc_02089558(0);
    func_0205d6f8(BACKUP_LOCK_ID);
    func_0205dc74(CARD_BACKUP_TYPE_EEPROM_512KBITS);
    CARD_WriteAndVerifyBackupAsync(seek, buff, size, NULL, NULL);
}

THUMB int profile::SaveLoad::memorysaveAsyncWait()
{
    return func_0205ddac();
}

THUMB int profile::SaveLoad::memorysaveAsyncResult()
{
    int result = func_0205d6ac();
    func_0205d708(BACKUP_LOCK_ID);
    unkfunc_02089558(lastSleep_);
    if (result == 0) {
        return 1;
    }
    return 0;
}

THUMB bool profile::SaveLoad::memoryload(unsigned int seek, void* buff, unsigned int size)
{
    func_0205d6f8(BACKUP_LOCK_ID);
    func_0205dc74(CARD_BACKUP_TYPE_EEPROM_512KBITS);
    CARD_ReadBackup(seek, buff, size);
    int result = func_0205d6ac();
    func_0205d708(BACKUP_LOCK_ID);
    if (result == 0) {
        return 1;
    }
    return 0;
}

THUMB char* profile::SaveLoad::unkfunc_0202bbe0(int type, unsigned char* name)
{
    if (type == 1 || type == 2) {
        dss::strcpy_s(s_name, 0x20, (char*)name);
        return s_name;
    }
    TextAPI::extractText(s_name, sizeof(s_name), 0x50000000, type);
    return s_name;
}

THUMB void profile::SaveLoad::setCatalogMacro(CatalogView* view)
{
    char text[0x40];
    char time[0x40];
    int chapter = view->chapter_;
    unsigned int playTime = view->time_;
    int town = view->town_;
    int level = view->level_;
    TextAPI::setMACRO0(0x42, 0xf0000000, view->id_ + 1);
    TextAPI::setMACRO0(9, 0xd0000000, 0);
    TextAPI::setMACRO0(0x5f, 0xc0000000, town & 0x0fffffff);
    TextAPI::setMACRO0(0x5d, 0xf0000000, chapter);
    TextAPI::setMACRO0(0x18, 0xd0000000, 1);
    TextAPI::setMACRO0(0x5e, 0xf0000000, level);
    dss::sprintf_s(text, sizeof(text), "%4d:%02d", playTime / (60 * 60 * 60), playTime % (60 * 60 * 60) / (60 * 60));
    unkfunc_02087f14(data_020c47d4, data_020c4894, time, sizeof(time), text);
    TextAPI::setUserString(0, view->name_);
    TextAPI::setUserString(1, time);
}

THUMB int profile::SaveLoad::getPlaceNameNo(int chapter, char* name)
{
    if (isThisMap(name, "mb")) {
        return 0xc0000032;
    }
    if (isThisMap(name, "md")) {
        return 0xc0000005;
    }
    if (isThisMap(name, "mf")) {
        return 0xc0000008;
    }
    if (isThisMap(name, "mi")) {
        return 0xc000000c;
    }
    if (isThisMap(name, "cf")) {
        return 0xc0000010;
    }
    if (isThisMap(name, "hc")) {
        return 0xc0000011;
    }
    if (isThisMap(name, "cd")) {
        return 0xc0000012;
    }
    if (isThisMap(name, "mm")) {
        return 0xc0000013;
    }
    if (isThisMap(name, "mn")) {
        return 0xc0000014;
    }
    if (isThisMap(name, "sh")) {
        return 0xc0000015;
    }
    if (isThisMap(name, "mo")) {
        return 0xc0000016;
    }
    if (isThisMap(name, "cg")) {
        return 0xc0000017;
    }
    if (isThisMap(name, "ce")) {
        return 0xc0000018;
    }
    if (isThisMap(name, "mj")) {
        return 0xc0000019;
    }
    if (isThisMap(name, "mh")) {
        return 0xc000001a;
    }
    if (isThisMap(name, "cb")) {
        return 0xc000001b;
    }
    if (isThisMap(name, "mc")) {
        return 0xc000001c;
    }
    if (isThisMap(name, "me")) {
        return 0xc000001d;
    }
    if (isThisMap(name, "ss")) {
        return 0xc0000033;
    }
    if (isThisMap(name, "hh")) {
        return 0xc000001f;
    }
    if (isThisMap(name, "ha")) {
        return 0xc0000020;
    }
    if (isThisMap(name, "ma")) {
        return 0xc0000021;
    }
    if (isThisMap(name, "ci")) {
        return 0xc0000022;
    }
    if (isThisMap(name, "mq")) {
        return 0xc0000023;
    }
    if (isThisMap(name, "mr")) {
        return 0xc0000024;
    }
    if (isThisMap(name, "cj")) {
        return 0xc0000025;
    }
    if (isThisMap(name, "mk")) {
        return 0xc0000026;
    }
    if (isThisMap(name, "ms")) {
        return 0xc0000027;
    }
    if (isThisMap(name, "ck")) {
        return 0xc0000028;
    }
    if (isThisMap(name, "sm")) {
        return 0xc0000029;
    }
    if (isThisMap(name, "dp")) {
        return 0xc000002a;
    }
    return 0;
}

THUMB int profile::SaveLoad::getPlaceNameByChapter(int chap)
{
    if (chap == 1) {
        return 0xc0000020;
    }
    if (chap == 2) {
        return 0xc000001b;
    }
    if (chap == 3) {
        return 0xc0000008;
    }
    if (chap == 4) {
        return 0xc000001a;
    }
    if (chap == 5) {
        return 0xc0000035;
    }
    if (chap == 5) {
        return 0xc0000010;
    }
    if (chap == 6) {
        return 0xc0000011;
    }
    return 0x80000032;
}

THUMB int profile::SaveLoad::isThisMap(const char* map, const char* name)
{
    if (map[0] != name[0]) {
        return false;
    }
    if (map[1] != name[1]) {
        return false;
    }
    return true;
}

THUMB int profile::SaveLoad::isCardOK()
{
    unsigned char buff[0x200];
    return memoryload(0, buff, sizeof(buff));
}

THUMB int profile::SaveLoad::unkfunc_0202c058()
{
    int result = 1;
    unsigned char buff[0x200];
    if (!memoryload(0, buff, sizeof(buff))) {
        return -3;
    }
    unsigned int* header = (unsigned int*)buff;
    if (header[0] != 0x20345144 || header[1] != 0x45545241) {
        dss::memset(buff, 0, sizeof(buff));
        header[0] = 0x20345144;
        header[1] = 0x45545241;
        if (!memorysave(0, buff, sizeof(buff))) {
            return -4;
        }
        if (!setSaveBank(0)) {
            return -4;
        }
        for (int i = 0; i < 4; i++) {
            if (!killbank(i)) {
                return -5;
            }
        }
    } else {
        for (int i = 0; i < 4; i++) {
            if (!unkfunc_0202ba3c(i)) {
                result = -2;
                if (!killbank(i)) {
                    return -5;
                }
            }
        }
    }
    return result;
}

THUMB int profile::SaveLoad::setSaveBank(int bank)
{
    unsigned int buff[0x80];
    dss::memset(buff, 0, sizeof(buff));
    buff[0] = bank;
    return memorysave(0x200, buff, sizeof(buff));
}

THUMB int profile::SaveLoad::getSaveBank()
{
    unsigned int buff[0x80];
    if (!memoryload(0x200, buff, sizeof(buff))) {
        return -1;
    }
    unsigned int bank = buff[0];
    if (bank > 2) {
        bank = 0;
    }
    return bank;
}
