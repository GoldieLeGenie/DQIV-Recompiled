#pragma once
#include "Assets.hpp"
#include "main/data/DataObject.hpp"
#include <span>

namespace pc {
// Owns the mounted asset set and the host equivalents of the game's heaps.
class GameServices {
public:
    explicit GameServices(const Assets& assets);
    ~GameServices();
    GameServices(const GameServices&) = delete;
    GameServices& operator=(const GameServices&) = delete;
};
std::size_t liveGameAllocations();
std::span<const std::uint8_t> gameAllocation(const void* pointer);
// Bytes readable from `pointer` to the end of its game allocation (0 when it is not in one).
std::size_t gameBytesAfter(const void* pointer);
bool isGameTextPointer(std::uintptr_t value);
class Resource {
public:
    explicit Resource(const char* path) { object_.setup(path, 0, 0); }
    ~Resource() { object_.cleanup(); }
    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;
    std::span<const std::uint8_t> bytes() { return {static_cast<const std::uint8_t*>(object_.getAddr()), static_cast<std::size_t>(object_.getSize())}; }
private:
    LZDataObject object_;
};
}
