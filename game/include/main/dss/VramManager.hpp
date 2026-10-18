#pragma once
#include <globaldefs.h>

// VRAM transfer task (NitroSystem)
struct UnkVramTransferTask {
    int type_;                                  // 0x00
    void* src_;                                 // 0x04
    int address_;                               // 0x08
    int size_;                                  // 0x0C
};

// texture/palette VRAM managers and the VRAM transfer queue of the dss engine
struct UnkVramTransfer {
    int unk_00;                                 // 0x00
    int unk_04;                                 // 0x04 palette VRAM in use
    unsigned int unk_08;                        // 0x08 texture VRAM size
    int texSize_;                               // 0x0C
    UnkVramTransferTask tasks_[0x80];           // 0x10
    int unk_810;                                // 0x810
    int unk_814;                                // 0x814 transfer in the V-blank

    UnkVramTransfer();
    ~UnkVramTransfer();
    void initTransferQueue();                        // init the transfer queue
    void initVramManagers(int texSize, int plttSize); // init the texture/palette VRAM managers
    void unkfunc_02086278();
    int allocTexVram(int size);                 // allocates a texture block, returns its handle
    void freeTexVram(unsigned int handle);     // frees a texture block
    int getTexVramAddr(unsigned int handle);      // address of a texture block
    int getTexVramSize(unsigned int handle);      // size of a texture block
    int allocPlttVram(int size);                 // allocates a palette block, returns its handle
    void freePlttVram(unsigned int handle);     // frees a palette block
    int getPlttVramAddr(unsigned int handle);      // address of a palette block
    int getPlttVramSize(unsigned int handle);      // size of a palette block
    void transferToVram(int type, void* src, int address, unsigned int size, int flag);
    void doTransfers();                        // do the queued transfers
};

extern UnkVramTransfer g_VramTransfer;
extern unsigned char g_plttVramManagerWork[0x800];      // palette VRAM manager work
extern unsigned char g_texVramManagerWork[0x800];      // texture VRAM manager work

extern "C" {
    void func_02072800(UnkVramTransferTask* tasks, int count);             // NNS_GfdInitVramTransferManager
    void func_02072824(void);                                               // NNS_GfdDoVramTransfer
    int func_02072884(int type, int address, void* src, unsigned int size); // NNS_GfdRegisterNewVramTransferTask
    int func_020728ec(void);
    unsigned int func_02072c54(int count);                                  // NNS_GfdGetLnkTexVramManagerWorkSize
    void func_02072c5c(int size, int size4x4, void* work, unsigned int workSize, int useAsDefault);    // NNS_GfdInitLnkTexVramManager
    unsigned int func_02072fa0(int count);                                  // NNS_GfdGetLnkPlttVramManagerWorkSize
    void func_02072fa8(int size, void* work, unsigned int workSize, int useAsDefault);                  // NNS_GfdInitLnkPlttVramManager
}

extern "C" int (*data_020c403c)(int size, int is4x4, int opt);     // NNS_GfdDefaultFuncAllocTexVram
extern "C" int (*data_020c4040)(int key);                          // NNS_GfdDefaultFuncFreeTexVram
extern "C" int (*data_020c4044)(int size, int is4Pltt, int opt);   // NNS_GfdDefaultFuncAllocPlttVram
extern "C" int (*data_020c4048)(int key);                          // NNS_GfdDefaultFuncFreePlttVram
