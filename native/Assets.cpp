#include "Assets.hpp"
#include "AssetPack.hpp"
#include <algorithm>
#include <fstream>
#include <stdexcept>

namespace pc {
std::string assetKey(std::string name) {
    for (char& c : name) {
        if (c == '\\') c = '/';
        if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
    }
    const std::filesystem::path path(name);
    if (path.is_absolute() || path.has_root_name()) throw std::runtime_error("Absolute asset path rejected");
    for (const auto& part : path)
        if (part == "..") throw std::runtime_error("Asset path traversal rejected");
    return path.lexically_normal().generic_string();
}
void Assets::add(const std::filesystem::path& root) {
    std::unordered_map<std::string, Entry> files;
    if(std::filesystem::is_regular_file(root)) {
        for(const auto& e:readAssetIndex(root)) {
            const auto name=assetKey(e.name);
            if(name!=e.name || !files.emplace(name,Entry{root,e.offset,e.size,e.hash,true}).second)
                throw std::runtime_error("Invalid or duplicate packed asset path: "+e.name);
        }
    } else {
        if (!std::filesystem::is_directory(root))
            throw std::runtime_error("Assets directory missing: " + root.string());
        for (const auto& entry : std::filesystem::recursive_directory_iterator(root)) {
            if (!entry.is_regular_file()) continue;
            const auto name = assetKey(entry.path().lexically_relative(root).generic_string());
            if (!files.emplace(name, Entry{entry.path()}).second)
                throw std::runtime_error("Ambiguous asset name: " + name);
        }
    }
    for (auto& [name, entry] : files) files_.insert_or_assign(name, std::move(entry));
}
std::vector<std::uint8_t> Assets::read(const std::string& name) const {
    const auto found = files_.find(assetKey(name));
    if (found == files_.end()) throw std::runtime_error("Asset missing: " + name);
    const auto& entry=found->second;
    std::ifstream stream(entry.path, std::ios::binary | std::ios::ate);
    if (!stream) throw std::runtime_error("Cannot open asset: " + name);
    const auto size = entry.packed ? std::streamoff(entry.size) : std::streamoff(stream.tellg());
    if (size < 0) throw std::runtime_error("Cannot size asset: " + name);
    std::vector<std::uint8_t> data(static_cast<std::size_t>(size));
    stream.seekg(static_cast<std::streamoff>(entry.offset));
    if (!stream.read(reinterpret_cast<char*>(data.data()), size))
        throw std::runtime_error("Cannot read asset: " + name);
    if(entry.packed && assetHash(data)!=entry.hash) throw std::runtime_error("Asset checksum mismatch: "+name);
    return data;
}
bool Assets::contains(const std::string& name) const { return files_.contains(assetKey(name)); }
std::vector<std::string> Assets::names() const {
    std::vector<std::string> result; result.reserve(files_.size());
    for(const auto& [name,entry]:files_) result.push_back(name);
    std::sort(result.begin(),result.end()); return result;
}
}
