#pragma once
#include "globaldefs.h"

// Envoy received by surechigai (same fields as profile::PROFILE_ENVOY)
struct EnvoyData {
    int enable;                                 // 0x00
    unsigned int unique;                        // 0x04
    int type;                                   // 0x08
    unsigned char name[26];                     // 0x0C
    int sex;                                    // 0x28
    int age;                                    // 0x2C
    int skill;                                  // 0x30
    unsigned char heroName[26];                 // 0x34
    unsigned char townName[42];                 // 0x4E
    unsigned char comment[92];                  // 0x78
};

// Envoys of the immigrant town; the accessors work on myEnvoy_ (mode_ 1) or on envoy_[index_]
struct EnvoyManager {
    int mode_;                                  // 0x0000
    int index_;                                 // 0x0004
    EnvoyData myEnvoy_;                      // 0x0008
    EnvoyData envoy_[24];                    // 0x00DC
    EnvoyData received_;                     // 0x14BC
    EnvoyData backup_;                       // 0x1590 copy of myEnvoy_ while it is edited
    int hasReceived_;                               // 0x1664
    int flags_[50];                             // 0x1668 by envoy type

    EnvoyManager();
    ~EnvoyManager();
    void selectEnvoy(int index);           // select envoy_[index]
    int isMyEnvoyEnabled();
    int isEnvoyEnabled(int index);
    int findFreeEnvoy();                     // first free envoy (-1: full)
    int getEnvoyCount();                     // envoy count
    void removeEnvoy(int index);           // remove envoy_[index]
    void receiveEnvoy(EnvoyData* data, int arg);
    void setEnable(int enable);
    void setUnique(unsigned int unique);
    unsigned int getUnique();
    void setType(int type);
    int getType();
    void setName(const char* name);
    unsigned char* getName();
    void setHeroName(const char* name);
    unsigned char* getHeroName();
    void setSex(int sex);
    int getSex();
    void setAge(int age);
    int getAge();
    void setSkill(int skill);
    int getSkill();
    void setTownName(const char* name);
    unsigned char* getTownName();
    void setComment(const char* comment);
    unsigned char* getComment();
    void clearEnvoy(int index);           // clear envoy_[index]
    int findEnvoyByUnique(unsigned int unique);
    void backupMyEnvoy();                    // myEnvoy_ -> backup_
    void restoreMyEnvoy();                    // backup_ -> myEnvoy_
    void clearBackup();                    // clear backup_
    void setTypeSelectable(int type, int flag);
    int isTypeSelectable(int type);
};

extern EnvoyManager g_EnvoyManager;
