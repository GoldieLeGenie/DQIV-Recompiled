#include "Archive.hpp"
#include <stdexcept>
namespace {
std::uint32_t u32(std::span<const std::uint8_t> b, std::size_t o) {
    if (o > b.size() || b.size() - o < 4) throw std::runtime_error("Truncated dss archive");
    return b[o] | (std::uint32_t(b[o+1]) << 8) | (std::uint32_t(b[o+2]) << 16) | (std::uint32_t(b[o+3]) << 24);
}
}
namespace pc {
Archive::Archive(std::span<const std::uint8_t> data) : data_(data), count_(u32(data,16)), offset_(u32(data,20) & ~3u) {
    if (data.size() < 32 || count_ > (data.size() - 32) / 8 || offset_ < 32 + count_ * 8 || offset_ > data.size())
        throw std::runtime_error("Invalid dss archive table");
    for (std::size_t i = 0; i < count_; ++i) file(i);
}
std::span<const std::uint8_t> Archive::file(std::size_t index) const {
    if (index >= count_) throw std::runtime_error("dss archive index out of range");
    const std::size_t start = offset_ + (u32(data_, 32 + index * 8) & ~3u), size = u32(data_, 36 + index * 8);
    if (start > data_.size() || size > data_.size() - start) throw std::runtime_error("dss archive entry out of range");
    return data_.subspan(start, size);
}
}
