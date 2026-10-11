#pragma once
#include "globaldefs.h"

// Envoy received by surechigai (same fields as profile::PROFILE_ENVOY)
struct UnkEnvoyData {
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
struct UnkEnvoyManager {
    int mode_;                                  // 0x0000
    int index_;                                 // 0x0004
    UnkEnvoyData myEnvoy_;                      // 0x0008
    UnkEnvoyData envoy_[24];                    // 0x00DC
    UnkEnvoyData received_;                     // 0x14BC
    UnkEnvoyData backup_;                       // 0x1590 copy of myEnvoy_ while it is edited
    int unk_1664;                               // 0x1664
    int flags_[50];                             // 0x1668 by envoy type

    UnkEnvoyManager();
    ~UnkEnvoyManager();
    void unkfunc_0203a34c(int index);           // select envoy_[index]
    int unkfunc_0203a354();
    int unkfunc_0203a358(int index);
    int unkfunc_0203a364();                     // first free envoy (-1: full)
    int unkfunc_0203a388();                     // envoy count
    void unkfunc_0203a3a8(int index);           // remove envoy_[index]
    void unkfunc_0203a48c(UnkEnvoyData* data, int arg);
    void unkfunc_0203a574(int enable);
    void unkfunc_0203a58c(unsigned int unique);
    unsigned int unkfunc_0203a5a4();
    void unkfunc_0203a5bc(int type);
    int unkfunc_0203a5ec();
    void unkfunc_0203a604(const char* name);
    unsigned char* unkfunc_0203a65c();
    void unkfunc_0203a674(const char* name);
    unsigned char* unkfunc_0203a6d8();
    void unkfunc_0203a6f4(int sex);
    int unkfunc_0203a714();
    void unkfunc_0203a730(int age);
    int unkfunc_0203a750();
    void unkfunc_0203a76c(int skill);
    int unkfunc_0203a78c();
    void unkfunc_0203a7a8(const char* name);
    unsigned char* unkfunc_0203a820();
    void unkfunc_0203a83c(const char* comment);
    unsigned char* unkfunc_0203a938();
    void unkfunc_0203a954(int index);           // clear envoy_[index]
    int unkfunc_0203a9cc(unsigned int unique);
    void unkfunc_0203aa00();                    // myEnvoy_ -> backup_
    void unkfunc_0203aa58();                    // backup_ -> myEnvoy_
    void unkfunc_0203aaac();                    // clear backup_
    void unkfunc_0203ab20(int type, int flag);
    int unkfunc_0203ab30(int type);
};

extern UnkEnvoyManager data_020f0078;
