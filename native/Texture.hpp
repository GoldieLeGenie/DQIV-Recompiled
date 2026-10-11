#pragma once
#include <cstdint>
#include <span>
#include <vector>
namespace pc {
struct Texture {
    int width = 0, height = 0;
    std::vector<std::uint32_t> pixels;
    std::uint64_t version = 0; // changes when a cache decodes the texture again
    static Texture load(std::span<const std::uint8_t> file, bool transparentZero = true);
    static Texture decodeGX(int width, int height, unsigned format,
        std::span<const std::uint8_t> pixels, std::span<const std::uint8_t> palette, bool transparentZero,
        std::span<const std::uint8_t> blockInfo = {});
    void draw(std::vector<std::uint32_t>& target, int targetWidth, int targetHeight, int x, int y) const;
};
}
