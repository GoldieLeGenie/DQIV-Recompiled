#include "NitroFilesystem.hpp"
#include "NitroFileApi.hpp"
#include <algorithm>
#include <climits>
#include <cstring>
#include <map>
#include <mutex>
#include <stdexcept>

static_assert(sizeof(FS_File)==0x48 && offsetof(FS_File,currentPosition)==0x2c);
namespace {
std::mutex fsMutex;
const pc::Assets* mountedAssets=nullptr;
FS_Record archive{};
std::vector<std::string> fileNames;
std::map<FS_File*,std::vector<std::uint8_t>> openFiles;
std::string normalize(const char* path) {
    if (!path) return {};
    std::string name(path);
    std::replace(name.begin(),name.end(),'\\','/');
    for (char& c:name) if(c>='A' && c<='Z') c += 'a'-'A';
    if (name.starts_with("rom:")) name.erase(0,4);
    if (name.starts_with('/')) name.erase(0,1);
    return name;
}
int openFile(FS_File* file,unsigned id) {
    if(!mountedAssets || !file || id>=fileNames.size() || openFiles.contains(file)) return 0;
    auto bytes=mountedAssets->read(fileNames[id]);
    if(bytes.size()>INT_MAX) throw std::runtime_error("Nitro file exceeds signed 32-bit size");
    file->startPosition=0; file->endPosition=static_cast<u32>(bytes.size()); file->currentPosition=0;
    file->record=&archive; file->status=0x10; file->result=FS_RESULT_SUCCESS;
    openFiles.emplace(file,std::move(bytes));
    return 1;
}
}
namespace pc {
NitroFilesystem::NitroFilesystem(const Assets& assets) {
    std::lock_guard lock(fsMutex);
    if(mountedAssets) throw std::runtime_error("Nitro filesystem already mounted");
    fileNames=assets.names(); mountedAssets=&assets;
}
NitroFilesystem::~NitroFilesystem() {
    std::lock_guard lock(fsMutex);
    openFiles.clear(); fileNames.clear(); mountedAssets=nullptr;
}
}
extern "C" {
void func_02060e04(FS_File* file) {
    std::lock_guard lock(fsMutex);
    if(!file) throw std::runtime_error("Null Nitro file");
    openFiles.erase(file); *file={}; file->unk_10=14;
}
int func_02060fec(FS_FileIdentifier* id,const char* path) {
    std::lock_guard lock(fsMutex);
    if(!id || !mountedAssets) return 0;
    const auto name=normalize(path);
    const auto found=std::lower_bound(fileNames.begin(),fileNames.end(),name);
    if(found==fileNames.end() || *found!=name) return 0;
    *id={&archive,static_cast<u32>(found-fileNames.begin())}; return 1;
}
int func_02061074(FS_File* file,FS_FileIdentifier id) {
    std::lock_guard lock(fsMutex);
    return id.record==&archive ? openFile(file,id.fileID) : 0;
}
int func_020610e4(FS_File* file,const char* path) {
    FS_FileIdentifier id;
    return func_02060fec(&id,path) && func_02061074(file,id);
}
int func_0206112c(FS_File* file) {
    std::lock_guard lock(fsMutex);
    if(!file || !openFiles.erase(file)) return 0;
    file->record=nullptr; file->status&=~0x30u; file->unk_10=14; return 1;
}
void func_02061228(FS_File* file) {
    // Reads are synchronous: taking the mutex waits for any active read to finish.
    std::lock_guard lock(fsMutex);
    if(!file) throw std::runtime_error("Null Nitro file cancellation");
}
int func_02061270(FS_File* file,void* dst,int requested) {
    std::lock_guard lock(fsMutex);
    const auto found=openFiles.find(file);
    if(found==openFiles.end()) return -1;
    if(requested<0) requested=0;
    const auto& bytes=found->second;
    const auto start=std::min<std::size_t>(file->currentPosition,bytes.size());
    const auto size=std::min<std::size_t>(requested,bytes.size()-start);
    if(size && !dst) return -1;
    if(size) std::memcpy(dst,bytes.data()+start,size);
    file->currentPosition=static_cast<u32>(start+size); file->result=FS_RESULT_SUCCESS;
    return static_cast<int>(size);
}
int func_02061280(FS_File* file,int offset,int mode) {
    std::lock_guard lock(fsMutex);
    if(!openFiles.contains(file) || mode<0 || mode>2) return 0;
    const std::int64_t base=mode==0 ? 0 : mode==1 ? file->currentPosition : file->endPosition;
    file->currentPosition=static_cast<u32>(std::clamp<std::int64_t>(base+offset,0,file->endPosition));
    return 1;
}
}
