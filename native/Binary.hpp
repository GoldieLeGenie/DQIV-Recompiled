#pragma once
#include <bit>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>

namespace pc {
// Offsets in Nintendo resource files remain offsets on 64-bit hosts.
class Binary {
public:
    explicit Binary(std::span<const std::uint8_t> bytes) : bytes_(bytes) {}
    std::size_t size() const { return bytes_.size(); }
    std::span<const std::uint8_t> slice(std::size_t offset, std::size_t count) const {
        if (offset > size() || count > size() - offset) throw std::runtime_error("Binary resource range out of bounds");
        return bytes_.subspan(offset, count);
    }
    Binary tail(std::size_t offset) const {
        if (offset > size()) throw std::runtime_error("Binary resource offset out of bounds");
        return Binary(slice(offset, size() - offset));
    }
    std::uint8_t u8(std::size_t o) const { return slice(o, 1)[0]; }
    std::uint16_t u16(std::size_t o) const { auto b = slice(o, 2); return b[0] | (std::uint16_t(b[1]) << 8); }
    std::uint32_t u32(std::size_t o) const { auto b = slice(o, 4); return b[0] | (std::uint32_t(b[1]) << 8) | (std::uint32_t(b[2]) << 16) | (std::uint32_t(b[3]) << 24); }
    std::uint64_t u64(std::size_t o) const { return u32(o) | (std::uint64_t(u32(o+4)) << 32); }
    std::int16_t i16(std::size_t o) const { return std::bit_cast<std::int16_t>(u16(o)); }
    std::int32_t i32(std::size_t o) const { return std::bit_cast<std::int32_t>(u32(o)); }
    float fx32(std::size_t o) const { return float(i32(o)) / 4096.0f; }
    float fx16(std::size_t o) const { return float(i16(o)) / 4096.0f; }
    bool tag(std::size_t o, std::string_view s) const {
        auto b = slice(o, s.size());
        return std::string_view(reinterpret_cast<const char*>(b.data()), b.size()) == s;
    }
    std::string name(std::size_t o, std::size_t length = 16) const {
        auto b = slice(o, length); std::string s;
        for (auto c : b) { if (!c) break; s += char(c); }
        return s;
    }
private:
    std::span<const std::uint8_t> bytes_;
};
}
