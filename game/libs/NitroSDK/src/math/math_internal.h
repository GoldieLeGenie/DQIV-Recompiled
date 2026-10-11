#ifndef MATH_INTERNAL_H
#define MATH_INTERNAL_H

#include <nitro/types.h>
#include <nitro/mi/cpumem.h>

/* SHA-1 context (0x68 bytes). */
typedef struct UnkSha1Context {
    /* 0x00 */ u32 unk_00[5]; /* hash state */
    /* 0x14 */ u32 unk_14;    /* bit count, low */
    /* 0x18 */ u32 unk_18;    /* bit count, high */
    /* 0x1C */ s32 unk_1c;    /* bytes in the block buffer */
    /* 0x20 */ u8  unk_20[64]; /* block buffer */
    /* 0x60 */ u32 unk_60[2];  /* not used by this code; the context occupies 0x68 bytes on the stack */
} UnkSha1Context;

typedef void (*UnkSha1BlockFunc)(UnkSha1Context* context, const void* data, u32 len);
extern UnkSha1BlockFunc data_020c3c90; /* = func_0205f73c */

/* Generic hash description used by the HMAC routine (0x1C bytes). */
typedef struct UnkHmacParam {
    /* 0x00 */ s32   unk_00; /* digest length */
    /* 0x04 */ s32   unk_04; /* block length */
    /* 0x08 */ void* unk_08; /* hash context */
    /* 0x0C */ void* unk_0c; /* digest buffer */
    /* 0x10 */ void (*unk_10)(void* context);
    /* 0x14 */ void (*unk_14)(void* context, const void* data, u32 len);
    /* 0x18 */ void (*unk_18)(void* context, void* digest);
} UnkHmacParam;

void func_0206785c(u32 value, void* dest, u32 size);

void func_0205f13c(UnkSha1Context* context);
void func_0205f18c(UnkSha1Context* context, const void* data, u32 len);
void func_0205f2e0(UnkSha1Context* context, u8* digest);
void func_0205f4dc(u8* digest, const void* data, u32 dataLen, const void* key, u32 keyLen);
void func_0205f570(u8* digest, const void* data, u32 dataLen, const u8* key, s32 keyLen, UnkHmacParam* param);
void func_0205f73c(UnkSha1Context* context, const void* data, u32 len);

#endif
