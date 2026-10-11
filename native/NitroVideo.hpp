#pragma once
#include <array>
#include <cstdint>
#include <span>
#include <vector>
namespace pc {
class NitroVideo {
public:
    NitroVideo();
    ~NitroVideo();
    NitroVideo(const NitroVideo&)=delete;
    NitroVideo& operator=(const NitroVideo&)=delete;
    void* reg(std::uint32_t address,std::size_t width);
    void write(std::uint32_t address,std::span<const std::uint8_t>);
    void read(std::uint32_t address,std::span<std::uint8_t>) const;
    // Stable backing for deferred copies of a captured LCDC image.
    void* lcdcSource(std::uint32_t address,std::size_t size);
    void readTexture(bool palette,std::size_t offset,std::span<std::uint8_t>) const;
    // Display engines see zero on unallocated VRAM; CPU transfers remain strict.
    void readDisplay(std::uint32_t address,std::span<std::uint8_t>) const;
    void readExtendedPalette(bool sub,bool obj,std::span<std::uint8_t>) const;
    bool lcdcBank(unsigned bank) const;
    void capture(unsigned bank,unsigned offset,std::span<const std::uint16_t>);
    std::uint64_t revision() const {return revision_;}
    // Changes only when memory of a bank mapped for textures or texture palettes is written.
    std::uint64_t textureRevision() const {return textureRevision_;}
    std::uint64_t textureBanks() const;
    std::span<const std::uint8_t> physicalVram() const {return vram_;}
    std::span<const std::uint8_t> palette() const {return palette_;}
    std::span<const std::uint8_t> oam() const {return oam_;}
private:
    struct Mapping {std::size_t offset,size;};
    std::vector<Mapping> mappings(std::uint32_t) const;
    alignas(8) std::array<std::uint8_t,0x2000> registers_{};
    std::vector<std::uint8_t> vram_;
    std::array<std::uint8_t,0x800> palette_{},oam_{};
    NitroVideo* previous_;
    std::uint64_t revision_=0,textureRevision_=0;
};
NitroVideo& activeVideo();
}
