#include "AssetPack.hpp"
#include "Assets.hpp"
#include "Binary.hpp"
#include <fstream>
#include <algorithm>
#include <array>
#include <limits>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

namespace pc {
namespace {
constexpr std::string_view magic="DQIVPK01";
constexpr std::size_t footerSize=40;
std::vector<std::uint8_t> readRange(std::ifstream& stream,std::uint64_t offset,std::uint64_t size) {
    if(size>std::numeric_limits<std::size_t>::max() || offset>std::uint64_t(std::numeric_limits<std::streamoff>::max()) ||
        size>std::uint64_t(std::numeric_limits<std::streamsize>::max())) throw std::runtime_error("Asset pack range too large");
    std::vector<std::uint8_t> b(static_cast<std::size_t>(size));
    stream.seekg(static_cast<std::streamoff>(offset));
    if(!stream.read(reinterpret_cast<char*>(b.data()),static_cast<std::streamsize>(size))) throw std::runtime_error("Truncated asset pack");
    return b;
}
void number(std::vector<std::uint8_t>& b,std::uint64_t value,int count) {
    for(int i=0;i<count;++i) b.push_back(std::uint8_t(value>>(i*8)));
}
void write(std::ofstream& out,std::span<const std::uint8_t> b) {
    if(!out.write(reinterpret_cast<const char*>(b.data()),static_cast<std::streamsize>(b.size()))) throw std::runtime_error("Cannot write asset pack");
}
}
std::uint64_t assetHash(std::span<const std::uint8_t> bytes) {
    // FNV-1a detects damaged/truncated resources; this is not an authenticity signature.
    std::uint64_t h=14695981039346656037ull;
    for(auto b:bytes) {h^=b;h*=1099511628211ull;} return h;
}
bool hasAssetPack(const std::filesystem::path& file) {
    std::ifstream stream(file,std::ios::binary|std::ios::ate);
    if(!stream || stream.tellg()<std::streamoff(footerSize)) return false;
    auto bytes=readRange(stream,std::uint64_t(stream.tellg())-footerSize,footerSize);
    return Binary(bytes).tag(0,magic);
}
std::vector<PackedAsset> readAssetIndex(const std::filesystem::path& file) {
    std::ifstream stream(file,std::ios::binary|std::ios::ate);
    if(!stream || stream.tellg()<std::streamoff(footerSize)) throw std::runtime_error("Asset pack footer missing");
    const auto fileSize=std::uint64_t(stream.tellg()),footerOffset=fileSize-footerSize;
    auto footerBytes=readRange(stream,footerOffset,footerSize); Binary f(footerBytes);
    if(!f.tag(0,magic)) throw std::runtime_error("Invalid asset pack signature");
    const auto offset=f.u64(8),size=f.u64(16),count=f.u64(24);
    if(offset>footerOffset || size!=footerOffset-offset || size>16*1024*1024 || count>100000 || count>size/29)
        throw std::runtime_error("Invalid asset pack index range");
    auto indexBytes=readRange(stream,offset,size); Binary b(indexBytes);
    if(assetHash(indexBytes)!=f.u64(32)) throw std::runtime_error("Asset pack index checksum mismatch");
    std::vector<PackedAsset> entries; std::size_t p=0; std::uint64_t previousEnd=0;
    for(std::uint64_t i=0;i<count;++i) {
        auto len=b.u32(p); if(len==0 || len>4096) throw std::runtime_error("Invalid packed asset name length");
        PackedAsset e; e.offset=b.u64(p+4);e.size=b.u64(p+12);e.hash=b.u64(p+20);p+=28;
        auto name=b.slice(p,len);p+=len;
        if(std::find(name.begin(),name.end(),0)!=name.end()) throw std::runtime_error("NUL in packed asset path");
        e.name.assign(reinterpret_cast<const char*>(name.data()),name.size());
        if(e.offset<previousEnd || e.offset>offset || e.size>offset-e.offset) throw std::runtime_error("Invalid packed asset payload range");
        previousEnd=e.offset+e.size;
        entries.push_back(std::move(e));
    }
    if(p!=b.size()) throw std::runtime_error("Unexpected asset index tail");
    return entries;
}
void writeAssetPack(const std::filesystem::path& executable,const std::vector<std::filesystem::path>& folders,const std::filesystem::path& output) {
    if(std::filesystem::weakly_canonical(executable)==std::filesystem::weakly_canonical(output)) throw std::runtime_error("Cannot overwrite input executable");
    if(hasAssetPack(executable)) throw std::runtime_error("Input executable is already packaged");
    Assets assets;
    for(const auto& folder:folders) {
        if(!std::filesystem::is_directory(folder)) throw std::runtime_error("Asset input must be a directory: "+folder.string());
        assets.add(folder);
    }
    writePack(executable,assets.names(),[&](const std::string& name){return assets.read(name);},output);
}
void writePack(const std::filesystem::path& prefixFile,const std::vector<std::string>& names,
    const std::function<std::vector<std::uint8_t>(const std::string&)>& read,const std::filesystem::path& output) {
    if(names.empty()) throw std::runtime_error("Cannot create an empty asset pack");
    for(const auto& name:names) if(assetKey(name)!=name) throw std::runtime_error("Pack names must be asset keys: "+name);
    if(!output.parent_path().empty()) std::filesystem::create_directories(output.parent_path());
    auto temporary=output; temporary+=".packing";
    // Refuse to trample an unrelated or interrupted packaging output.
    if(std::filesystem::exists(temporary)) throw std::runtime_error("Packaging temporary file already exists: "+temporary.string());
    try {
        std::ofstream stream(temporary,std::ios::binary|std::ios::trunc);
        if(!prefixFile.empty()) {
            std::ifstream prefix(prefixFile,std::ios::binary);
            if(!prefix) throw std::runtime_error("Input executable missing");
            stream<<prefix.rdbuf();
        }
        if(!stream) throw std::runtime_error("Cannot write "+temporary.string());
        std::vector<std::uint8_t> index;
        for(const auto& name:names) {
            auto bytes=read(name);
            auto offset=stream.tellp(); if(offset<0) throw std::runtime_error("Cannot determine pack offset");
            number(index,name.size(),4);number(index,std::uint64_t(offset),8);number(index,bytes.size(),8);number(index,assetHash(bytes),8);
            index.insert(index.end(),name.begin(),name.end()); write(stream,bytes);
        }
        const auto offset=stream.tellp(); if(offset<0) throw std::runtime_error("Cannot determine index offset");
        write(stream,index);
        std::vector<std::uint8_t> footer(magic.begin(),magic.end());
        number(footer,std::uint64_t(offset),8);number(footer,index.size(),8);number(footer,names.size(),8);number(footer,assetHash(index),8);
        write(stream,footer);stream.close();
        if(!stream) throw std::runtime_error("Cannot flush asset pack");
        // Verify every payload before publishing the file.
        Assets packed(temporary);
        for(const auto& name:names) if(packed.read(name)!=read(name)) throw std::runtime_error("Pack verification failed: "+name);
#ifdef _WIN32
        if(!MoveFileExW(temporary.c_str(),output.c_str(),MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH)) throw std::runtime_error("Cannot write "+output.string());
#else
        std::filesystem::rename(temporary,output);
#endif
    } catch(...) { std::error_code ignored; std::filesystem::remove(temporary,ignored); throw; }
}
std::filesystem::path executablePath() {
#ifdef _WIN32
    std::vector<wchar_t> path(512);
    for(;;) {
        auto count=GetModuleFileNameW(nullptr,path.data(),static_cast<DWORD>(path.size()));
        if(!count) throw std::runtime_error("Cannot locate executable");
        if(count<path.size()) return std::filesystem::path(std::wstring(path.data(),count));
        if(path.size()>32768) throw std::runtime_error("Executable path too long");
        path.resize(path.size()*2);
    }
#else
    return std::filesystem::read_symlink("/proc/self/exe");
#endif
}
}
