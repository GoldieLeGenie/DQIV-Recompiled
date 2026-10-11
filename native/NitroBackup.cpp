#include "NitroBackup.hpp"
#include <nitro/card.hpp>
#include <mutex>
#include <stdexcept>
#include <vector>
static_assert(sizeof(void*)==4);
namespace {
std::recursive_mutex backupMutex;
pc::SaveStore* backup=nullptr;
int result=0;
thread_local std::vector<unsigned short> lockIds;
}
namespace pc {
NitroBackup::NitroBackup(SaveStore& store) {
    std::lock_guard lock(backupMutex);
    if(backup) throw std::runtime_error("Backup already mounted");
    backup=&store; result=0;
}
NitroBackup::~NitroBackup() {
    std::lock_guard lock(backupMutex);
    backup=nullptr;
}
}
extern "C" {
void func_0205d6f8(unsigned short id) {
    backupMutex.lock();
    try { lockIds.push_back(id); } catch(...) { backupMutex.unlock(); throw; }
}
void func_0205d708(unsigned short id) {
    if(lockIds.empty() || lockIds.back()!=id) throw std::runtime_error("Unbalanced backup lock");
    lockIds.pop_back(); backupMutex.unlock();
}
int func_0205dc74(unsigned int type) {
    std::lock_guard lock(backupMutex);
    result=!backup ? 6 : type==CARD_BACKUP_TYPE_EEPROM_512KBITS ? 0 : 3;
    return result==0;
}
int func_0205d6ac() { std::lock_guard lock(backupMutex); return result; }
int func_0205ddac() {
    // Writes finish and are verified before their command returns.
    std::lock_guard lock(backupMutex); return 1;
}
int func_0205db78(unsigned int src,unsigned int dst,unsigned int length,void* callback,void* argument,
                 int async,int request,int retries,int mode) {
    int success;
    {
        std::lock_guard lock(backupMutex);
        result=0;
        try {
            if(!backup) result=6;
            else if(request==6 && mode==0) {
                if(!dst && length) result=2;
                else backup->read(src,{reinterpret_cast<std::uint8_t*>(dst),length});
            } else if(request==8 && mode==2) {
                if(!src && length) result=2;
                else backup->write(dst,{reinterpret_cast<const std::uint8_t*>(src),length});
            } else result=3;
        } catch(const std::out_of_range&) { result=2; }
          catch(const std::exception&) { result=5; }
        success=result==0;
    }
    if(callback) reinterpret_cast<void(*)(void*)>(callback)(argument);
    // The API reports acceptance for async commands; callers then read the result.
    return async ? 1 : success;
}
}
