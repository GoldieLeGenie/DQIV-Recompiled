#include <bit>
#include <cstdint>
#include <limits>

namespace {
thread_local std::int64_t division = 0;
std::uint32_t integerSqrt(std::uint64_t n) {
    std::uint64_t root = 0, bit = std::uint64_t{1} << 62;
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= root + bit) { n -= root + bit; root = (root >> 1) + bit; }
        else root >>= 1;
        bit >>= 2;
    }
    return static_cast<std::uint32_t>(root);
}
}
// NitroSDK FX division uses a signed 64/32 quotient with 32 fractional bits,
// then adds 2^19 and shifts 20. Preserve that rounding instead of using floats.
extern "C" void FX_DivAsync(std::int32_t numerator, std::int32_t denominator) {
    const auto n = std::int64_t(numerator) * 0x100000000LL;
    if (denominator == 0) division = n < 0 ? 1 : -1;
    else if (n == std::numeric_limits<std::int64_t>::min() && denominator == -1) division = n;
    else division = n / denominator;
}
extern "C" void FX_InvAsync(std::int32_t denominator) { FX_DivAsync(4096, denominator); }
extern "C" std::int64_t FX_GetDivResultFx64c() { return division; }
extern "C" std::int32_t FX_GetDivResult() {
    auto rounded = std::bit_cast<std::int64_t>(std::uint64_t(division) + (1u << 19));
    return std::bit_cast<std::int32_t>(std::uint32_t(rounded >> 20));
}
extern "C" std::int32_t FX_Divide(std::int32_t numerator, std::int32_t denominator) {
    FX_DivAsync(numerator, denominator); return FX_GetDivResult();
}
extern "C" std::int32_t FX_Inverse(std::int32_t denominator) { FX_InvAsync(denominator); return FX_GetDivResult(); }
extern "C" std::int64_t FX_Divide64(std::int32_t denominator) { FX_InvAsync(denominator); return division; }
extern "C" std::int32_t FX_Sqrt(std::int32_t value) {
    if (value <= 0) return 0;
    const auto root = integerSqrt(std::uint64_t(value) << 32);
    return static_cast<std::int32_t>((root + (1u << 9)) >> 10);
}
