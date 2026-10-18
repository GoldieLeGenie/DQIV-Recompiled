#pragma once
#include <globaldefs.h>

// DS-only sound handle
struct UnkSoundHandle {
    int unk_00;                                 // 0x00
    int unk_04;                                 // 0x04
    int unk_08;                                 // 0x08  volume (sys)
    int unk_0c;                                 // 0x0C  volume
    int unk_10;                                 // 0x10
    int unk_14;                                 // 0x14
    int unk_18;                                 // 0x18
    int unk_1c;                                 // 0x1C

    UnkSoundHandle();
    ~UnkSoundHandle();
    void unkfunc_02086494(int a);
    void unkfunc_020864ac(int bgm);
    void unkfunc_020864e0(int fade);
    int unkfunc_020864ec();
    void unkfunc_02086508(int a, int se);
    int unkfunc_02086530(int a, int se);
    void unkfunc_02086554(int fade);
    int unkfunc_02086578();
    void unkfunc_020865a4(int volume);
    void unkfunc_020865b4();                    // apply the volume
};

struct UnkSoundMember {
    int unk_00;                                 // 0x00
    int unk_04;                                 // 0x04

    UnkSoundMember();
    ~UnkSoundMember();
    void unkfunc_02086750();
    void unkfunc_0208675c(int value);
    void unkfunc_02086784(int value);
};

// DS-only sound player
struct UnkSoundPlayer {
    int enable_;                                // 0x000
    int bgm_;                                   // 0x004
    UnkSoundMember unk_008;                     // 0x008
    UnkSoundHandle bgmHandle_[2];               // 0x010
    UnkSoundHandle seHandle_[8];                // 0x050
    int bgmVolumeSys_;                          // 0x150
    int bgmVolume_;                             // 0x154
    int seVolumeSys_;                           // 0x158
    int seVolume_;                              // 0x15C

    UnkSoundPlayer();
    ~UnkSoundPlayer();
    void unkfunc_0205c710();                        // init
    void unkfunc_0205c76c(int bgm);                 // load bgm
    void unkfunc_0205c78c(int a, int b);
    void unkfunc_0205c7ac(int player, int bgm);     // play bgm
    void unkfunc_0205c800(int player, int fade);    // stop bgm
    int unkfunc_0205c824(int player);               // bgm playing
    void unkfunc_0205c838(int volume);              // set bgm volume (sys)
    void unkfunc_0205c848(int volume);              // set bgm volume
    int unkfunc_0205c858();                         // get bgm volume
    void unkfunc_0205c860(int volume);              // set se volume (sys)
    void unkfunc_0205c870();                        // apply bgm volume
    void unkfunc_0205c8c0();                        // apply se volume
    void unkfunc_0205c910(int index);
    int unkfunc_0205c948(int a, int se);            // play se
    void unkfunc_0205c9d0(int a, int se, int fade); // stop se
    void unkfunc_0205ca2c(int fade);                // stop all se
    void unkfunc_0205ca68(int value);
    void unkfunc_0205ca84(int value);
    void unkfunc_0205caa0();                        // update
};

// sound system: sound heap + sound archive
struct UnkSoundSystem {
    void* unk_00;                               // 0x00 sound heap memory
    void* unk_04;                               // 0x04 sound heap
    unsigned char unk_08[0x90];                 // 0x08 sound archive
    int unk_98;                                 // 0x98
    void* unk_9c;                               // 0x9C sound archive file
    int unk_a0;                                 // 0xA0

    UnkSoundSystem();
    ~UnkSoundSystem();
    void unkfunc_020865f4(void* file, int flag);    // init
    void unkfunc_02086634();                        // load the sound archive
    void unkfunc_020866d8();
    void unkfunc_020866e4(int bgm);
    void unkfunc_02086720(int a, int b);
};

extern UnkSoundPlayer data_0210bd4c;
extern UnkSoundSystem data_0211fc7c;
extern int data_020c4514;
extern int data_020c4518;
extern int data_0211fc68[2];
