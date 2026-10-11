#pragma once
#include <array>
#include <cstdint>
#include <filesystem>
#include <span>

namespace pc {
// The game's files (save, settings, journal) live in a "saves" folder beside
// the executable.
std::filesystem::path saveDirectory();
std::filesystem::path defaultSavePath();
// Where earlier versions kept them (%LOCALAPPDATA%\DQIV-PC), empty if unknown.
std::filesystem::path previousSaveDirectory();
// Original 512-kbit cartridge backup image, published atomically after each write.
class SaveStore {
public:
    static constexpr std::size_t size=65536;
    explicit SaveStore(std::filesystem::path path);
    ~SaveStore();
    SaveStore(const SaveStore&)=delete;
    SaveStore& operator=(const SaveStore&)=delete;
    void read(std::uint32_t offset,std::span<std::uint8_t> destination) const;
    void write(std::uint32_t offset,std::span<const std::uint8_t> source);
private:
    std::filesystem::path path_;
    std::array<std::uint8_t,size> bytes_;
    void* lock_=nullptr;
};
}
