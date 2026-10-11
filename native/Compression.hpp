#pragma once
#include <cstdint>
#include <span>
#include <vector>
namespace pc {
bool isCompressed(std::span<const std::uint8_t> data);
std::vector<std::uint8_t> decompress(std::span<const std::uint8_t> data);
}
