#ifndef SND_INTERNAL_H
#define SND_INTERNAL_H

// Placeholder declarations shared by the sound system files of this range.

#include <nitro/types.h>
#include <nitro/os/cpustat.h>
#include <nitro/os/thread.h>
#include <nitro/os/mutex.h>
#include <nitro/fs/file.h>

// ---- intrusive list (link at a fixed offset inside each object) ----
typedef struct UnkList {
    /* 0x00 */ void* head;
    /* 0x04 */ void* tail;
    /* 0x08 */ u16 numObjects;
    /* 0x0A */ u16 offset;
} UnkList; // size 0xC

typedef struct UnkLink {
    /* 0x00 */ void* prev;
    /* 0x04 */ void* next;
} UnkLink;

void  func_02067dec(UnkList* list, u16 offset);                 // init list
void  func_02067e30(UnkList* list, void* obj);                  // append
void  func_02067ed4(UnkList* list, void* target, void* obj);    // insert before target
void  func_02067f38(UnkList* list, void* obj);                  // remove
void* func_02067f98(UnkList* list, void* obj);                  // next
void* func_02067fb0(UnkList* list, void* obj);                  // prev

// ---- frame heap ----
void* func_0206886c(void* start, u32 size, u16 opt);            // create
void  func_020688a4(void* heap);                                // destroy
void* func_020688b0(void* heap, u32 size, int align);           // alloc
void  func_020688e4(void* heap, int mode);                      // free

// ---- file system ----
BOOL func_02060fec(FS_FileIdentifier* id, const char* path);
void func_02060e04(FS_File* file);
BOOL func_02061074(FS_File* file, FS_FileIdentifier id);
BOOL func_0206112c(FS_File* file);
BOOL func_02061228(FS_File* file);
s32  func_02061270(FS_File* file, void* dst, s32 len);
BOOL func_02061280(FS_File* file, s32 offset, int origin);

// ---- memory / cache ----
void MI_CpuCopyU8(const void* src, void* dest, u32 count);
void MI_CpuSet(void* dest, u8 value, u32 count);
void func_0206785c(u32 data, void* dest, u32 size);
void DC_PurgeRange(void* ptr, u32 size);
void DC_CleanRange(void* ptr, u32 size);
void DC_InvalidateRange(void* ptr, u32 size);

// ---- os ----
void func_020787c4(OSThread* thread, void (*func)(void*), void* arg, void* stack, u32 stackSize, u32 prio);
void func_02078d60(void* queue, void* msgs, s32 count);
BOOL func_02078e1c(void* queue, void* msg, s32 flags);

// ---- runtime ----
u32 _u32_div_f(u32 a, u32 b);
s32 _s32_div_f(s32 a, s32 b);
u64 func_02005fe4(u32 lo, u32 hi, u32 dlo, u32 dhi);

// ---- low level sound ----
void SND_SetupChannelPcm(int ch, int format, const void* data, int loop, int loopStart, int loopLen, int volume, int shift, int timer, int pan);
void SND_SetupAlarm(int alarm, u32 tick, u32 period, void (*callback)(void*), void* arg);
void SND_SetChannelVolume(u32 chBitMask, int volume, int shift);
void SND_SetChannelPan(u32 chBitMask, int pan);
void func_0207a390(int playerNo);
void func_0207a3b0(int playerNo, const void* seq, u32 offset, const void* bank);
void func_0207a3f8(int playerNo, int volume);
void func_0207a428(int playerNo, u32 trackBitMask, u32 chBitMask);
void func_0207a450(u32 chBitMask, u32 capBitMask, u32 alarmBitMask, u32 flags);
void func_0207a478(u32 chBitMask, u32 capBitMask, u32 alarmBitMask, u32 flags);
void func_0207a5f0(const void* start, const void* end);
void func_0207a610(const void* start, const void* end);
void func_0207a630(const void* start, const void* end);
void func_0207a650(int a, int b, int c, int d);
void func_0207a9e8(int flag);
void func_0207aba4(u32 tag);
u32  func_0207ac10(void);
s32  func_0207b024(int volume);
void func_0207b094(void* bank, int index, void* waveArc);
void func_0207b160(void* bank);
void func_0207b1f8(void* waveArc);


u32  func_0207b410(void* waveArc);
void* func_0207b44c(void* waveArc, u32 index);
void func_0207b418(void* waveArc, u32 index, void* wave);
void func_020732d0(u32 capBitMask);
BOOL func_020732a0(u32 chBitMask);
BOOL func_02073258(u32 chBitMask);
int  func_020732ec(void);
void func_02073334(int alarm);

#endif
