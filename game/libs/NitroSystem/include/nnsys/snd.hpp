#pragma once

// NitroSystem sound functions without a name in the libs headers
extern "C" {
    void func_020731e4(void);                                       // NNS_SndMain
    void func_02073370(int playerNo, int count);
    void func_0207343c(void* handle, int fadeFrame);                // NNS_SndPlayerStopSeq
    void func_0207344c(void* handle);
    int func_02073478(int playerNo);
    void func_020734cc(void* handle, int volume);                   // NNS_SndPlayerSetVolume
    void func_020734e0(void* handle, int value);
    int func_02073564(void* handle);
    void func_020741d0(int level);
    void func_02074550(void* arc, void* file, void* heap, int flag);
    void* func_02074bd8(void* memory, unsigned int size);           // NNS_SndHeapCreate
    void func_02074c60(void* heap);                                 // NNS_SndHeapClear
    int func_02074e24(int seqNo, void* heap);                       // NNS_SndArcLoadSeq
    int func_02074e50(int no, void* heap);
    int func_02074e7c(int no, void* heap);
    void func_02075780(void* heap);
    int func_0207581c(void* handle, int seqNo);
    int func_02075864(void* handle, int no, int index);
    int func_02075aa4(int groupNo, void* heap);
    void func_02075cec(void* handle, int no, int flag);
    void func_02075d14(void* handle, int value);
    void func_02075d30(void* handle, int volume);
    void func_02075d44(void* handle);
}
