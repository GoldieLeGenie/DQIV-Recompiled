#ifndef FX_INTERNAL_H
#define FX_INTERNAL_H

#include <nitro/types.h>
#include <nitro/reg.h>
#include <nitro/fx/fx_matrix.h>

typedef struct {
    s32 _00, _01;
    s32 _10, _11;
} UnkMtx22;

typedef struct {
    s16 x, y, z;
} UnkVecFx16;

/* fx_division */
s32  FX_Divide(s32 numer, s32 denom);
s32  FX_Inverse(s32 denom);
s64  FX_Divide64(s32 denom);
s32  FX_Sqrt(s32 value);
s64  FX_GetDivResultFx64c(void);
s32  FX_GetDivResult(void);
void FX_InvAsync(s32 denom);
s32  FX_GetSqrtResult(void);
void FX_DivAsync(s32 numer, s32 denom);

/* matrix */
void func_02061b70(UnkMtx22* m);
void func_02061b88(Mtx33* m);
void func_02061bac(const Mtx33* src, Mtx33* dst, s32 x, s32 y, s32 z);
void func_02061c6c(Mtx33* m, s32 sinVal, s32 cosVal);
void func_02061c88(Mtx33* m, s32 sinVal, s32 cosVal);
void func_02061ca4(Mtx33* m, s32 sinVal, s32 cosVal);
void MTX_Concat33(const Mtx33* a, const Mtx33* b, Mtx33* ab);
void func_02061edc(const Vec* vec, const Mtx33* m, Vec* dst);
void func_02061f58(Mtx43* m);
void func_02061f80(const Mtx43* src, Mtx44* dst);
void func_02061fb4(const Mtx43* src, Mtx43* dst, s32 x, s32 y, s32 z);
void func_02061fe8(Mtx43* m, s32 sinVal, s32 cosVal);
void func_02062008(Mtx43* m, s32 sinVal, s32 cosVal);
void func_02062024(Mtx43* m, s32 sinVal, s32 cosVal);
s32  func_02062040(const Mtx43* src, Mtx43* dst);
void func_020623cc(const Mtx43* a, const Mtx43* b, Mtx43* ab);
void func_020626a0(const Vec* vec, const Mtx43* m, Vec* dst);
void func_02062740(Mtx44* m);
void func_0206276c(const Mtx44* src, Mtx43* dst);
void func_020627a0(const Mtx44* a, const Mtx44* b, Mtx44* ab);

/* vector */
void func_02062f64(const Vec* a, const Vec* b, Vec* ab);
void func_02062f98(const Vec* a, const Vec* b, Vec* ab);
s32  func_02062fcc(const Vec* a, const Vec* b);
void func_02063008(const Vec* a, const Vec* b, Vec* axb);
s32  func_0206308c(const Vec* v);
void func_020630ec(const Vec* src, Vec* dst);
void func_02063204(const UnkVecFx16* src, UnkVecFx16* dst);
void func_02063330(s32 a, const Vec* v1, const Vec* v2, Vec* dst);
s32  func_0206338c(const Vec* a, const Vec* b);

void MI_CpuCopy48(const void* src, void* dest);

#endif
