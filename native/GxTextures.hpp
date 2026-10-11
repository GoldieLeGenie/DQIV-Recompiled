#pragma once
#include "Texture.hpp"
#include <unordered_map>
namespace pc {
// Decoded textures from the 3D engine's texture memory, kept until the bytes
// they come from change (Texture::version then changes).
class GxTextures {
public:
    GxTextures();
    const Texture& texture(std::uint32_t parameters,std::uint32_t palette);
private:
    struct Entry {Texture texture;std::uint64_t hash=0,checked=0;};
    std::span<const std::uint8_t> bytes(bool palette) const;
    std::vector<std::uint8_t> textureBytes_,paletteBytes_;
    std::unordered_map<std::uint64_t,Entry> cache_;
    std::uint64_t videoRevision_=~std::uint64_t{0},bankSelection_=~std::uint64_t{0},memory_=1,versions_=0;
};
}
