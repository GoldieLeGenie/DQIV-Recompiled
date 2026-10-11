#pragma once
#ifdef __MWERKS__
#define AT_ADDRESS(xyz) : (xyz)
#define INIT __declspec(init)
#define EXPORT __declspec(export)
#define WEAK __declspec(weak)
#define ASM asm
#define ARM _Pragma("thumb off")
#define THUMB _Pragma("thumb on")
#else
#define AT_ADDRESS(xyz)
#define INIT
#define EXPORT
#define WEAK
#define ASM
#define ARM 
#define THUMB
#endif

#ifndef NULL
#define NULL 0
#endif

#if defined(__cplusplus) && !defined(DQIV_NATIVE)
inline void operator delete(void*) {}
inline void operator delete[](void*) {}
#endif
