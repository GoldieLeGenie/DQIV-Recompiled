#pragma once
#include <span>
#include <cstdint>
namespace pc {
class Archive {
public:
    explicit Archive(std::span<const std::uint8_t> data);
    std::size_t count() const { return count_; }
    std::span<const std::uint8_t> file(std::size_t index) const;
private:
    std::span<const std::uint8_t> data_;
    std::size_t count_, offset_;
};
}
