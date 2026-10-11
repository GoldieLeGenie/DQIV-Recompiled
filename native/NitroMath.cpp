// Native counterparts of the ARM matrix routines and the DS integer math unit.
// Portable SDK matrix/vector algorithms remain in game/libs/NitroSDK/src/fx.
#include <bit>
#include <cstdint>
#include <cstring>
extern "C" {
#include "../game/libs/NitroSDK/src/fx/fx_internal.h"
}

namespace {
std::uint32_t root64(std::uint64_t n) {
    std::uint64_t result = 0, bit = std::uint64_t{1} << 62;
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= result + bit) { n -= result + bit; result = (result >> 1) + bit; }
        else result >>= 1;
        bit >>= 2;
    }
    return static_cast<std::uint32_t>(result);
}
std::uint64_t square(s32 n) { return static_cast<std::uint64_t>(s64(n) * n); }
std::uint64_t normSquared(const Vec& v) { return square(v.x) + square(v.y) + square(v.z); }
s32 wrappedSub(s32 a, s32 b) { return std::bit_cast<s32>(u32(a) - u32(b)); }
s32 length(std::uint64_t squared) {
    // Preserve the register's signed conversion and the ARM 32-bit addition.
    return std::bit_cast<s32>(root64(squared << 2) + 1u) >> 1;
}
Vec normalize(const Vec& v) {
    const auto squared = normSquared(v);
    const auto denominator = std::bit_cast<s64>(squared);
    const s64 quotient = denominator == 0 ? -1 : (s64{1} << 56) / denominator;
    const auto root = std::bit_cast<s32>(root64(squared << 2));
    const auto factor = u64(quotient) * u64(s64(root));
    const auto component = [factor](s32 n) {
        const u64 product = factor * u64(s64(n)) + (u64{1} << 44);
        return std::bit_cast<s32>(u32(std::bit_cast<s64>(product) >> 45));
    };
    return {component(v.x), component(v.y), component(v.z)};
}
}

extern "C" {
void func_02061b70(UnkMtx22* m) { *m = {4096, 0, 0, 4096}; }
void func_02061b88(Mtx33* m) { *m = {4096,0,0, 0,4096,0, 0,0,4096}; }
void func_02061c6c(Mtx33* m, s32 s, s32 c) { *m = {4096,0,0, 0,c,s, 0,-s,c}; }
void func_02061c88(Mtx33* m, s32 s, s32 c) { *m = {c,0,-s, 0,4096,0, s,0,c}; }
void func_02061ca4(Mtx33* m, s32 s, s32 c) { *m = {c,s,0, -s,c,0, 0,0,4096}; }
void func_02061f58(Mtx43* m) { *m = {4096,0,0, 0,4096,0, 0,0,4096, 0,0,0}; }
void func_02061fe8(Mtx43* m, s32 s, s32 c) { *m = {4096,0,0, 0,c,s, 0,-s,c, 0,0,0}; }
void func_02062008(Mtx43* m, s32 s, s32 c) { *m = {c,0,-s, 0,4096,0, s,0,c, 0,0,0}; }
void func_02062024(Mtx43* m, s32 s, s32 c) { *m = {c,s,0, -s,c,0, 0,0,4096, 0,0,0}; }
void func_02062740(Mtx44* m) { *m = {4096,0,0,0, 0,4096,0,0, 0,0,4096,0, 0,0,0,4096}; }
void func_02061f80(const Mtx43* s, Mtx44* d) {
    const Mtx44 result = {s->_00,s->_01,s->_02,0, s->_10,s->_11,s->_12,0,
                         s->_20,s->_21,s->_22,0, s->_30,s->_31,s->_32,4096};
    *d = result;
}
void func_0206276c(const Mtx44* s, Mtx43* d) {
    const Mtx43 result = {s->_00,s->_01,s->_02, s->_10,s->_11,s->_12,
                         s->_20,s->_21,s->_22, s->_30,s->_31,s->_32};
    *d = result;
}
s32 func_0206308c(const Vec* v) { return length(normSquared(*v)); }
void func_020630ec(const Vec* src, Vec* dst) { *dst = normalize(*src); }
void func_02063204(const UnkVecFx16* src, UnkVecFx16* dst) {
    const auto n = normalize({src->x,src->y,src->z});
    *dst = {static_cast<s16>(n.x), static_cast<s16>(n.y), static_cast<s16>(n.z)};
}
s32 func_0206338c(const Vec* a, const Vec* b) {
    return length(normSquared({wrappedSub(a->x,b->x),wrappedSub(a->y,b->y),wrappedSub(a->z,b->z)}));
}
}
