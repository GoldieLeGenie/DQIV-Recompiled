#pragma once
#include <filesystem>
#include <span>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>
namespace pc {
struct PackedAsset { std::string name; std::uint64_t offset,size,hash; };
std::uint64_t assetHash(std::span<const std::uint8_t> bytes);
bool hasAssetPack(const std::filesystem::path& file);
std::vector<PackedAsset> readAssetIndex(const std::filesystem::path& file);
// Writes `prefix` (an executable, or nothing for a pack file of its own)
// followed by the named resources (asset keys) and their index, verified
// before it replaces `output`.
void writePack(const std::filesystem::path& prefix,const std::vector<std::string>& names,
    const std::function<std::vector<std::uint8_t>(const std::string&)>& read,const std::filesystem::path& output);
// Packs the folders' files into a copy of the executable (a later folder adds
// files and replaces those of the same name).
void writeAssetPack(const std::filesystem::path& executable,const std::vector<std::filesystem::path>& folders,const std::filesystem::path& output);
std::filesystem::path executablePath();
}
