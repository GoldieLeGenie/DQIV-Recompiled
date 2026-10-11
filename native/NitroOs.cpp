#include <nitro/os/os_owner.h>
#include "Settings.hpp"
#include <atomic>
#include <chrono>
#include <cstdint>
#include <mutex>
#include <stdexcept>
#include <array>

namespace {
const auto start=std::chrono::steady_clock::now();
std::recursive_mutex criticalSection;
thread_local unsigned irqDepth=0;
thread_local unsigned short masterInterruptEnable=1;
std::mutex lockIdMutex;
std::array<bool,48> lockIds{};
std::array<unsigned short,9> vramOwners{};
}
namespace pc {
unsigned suspendNativeInterrupts() {
    const unsigned saved=irqDepth;irqDepth=0;
    for(unsigned i=0;i<saved;++i)criticalSection.unlock();
    return saved;
}
void resumeNativeInterrupts(unsigned saved) {
    if(irqDepth)throw std::runtime_error("Interrupt state leaked across native fiber switch");
    for(unsigned i=0;i<saved;++i)criticalSection.lock();
    irqDepth=saved;
}
}
extern "C" {
unsigned short* pcMasterInterruptRegister() {return &masterInterruptEnable;}
void OS_Terminate() { throw std::runtime_error("Original game requested OS_Terminate"); }
int OS_GetLockID() {
    std::lock_guard guard(lockIdMutex);
    for(unsigned i=0;i<lockIds.size();++i)if(!lockIds[i]) {lockIds[i]=true;return 0x40+i;}
    return -3;
}
void OS_ReleaseLockID(unsigned short id) {
    std::lock_guard guard(lockIdMutex);
    if(id<0x40 || id>=0x40+lockIds.size() || !lockIds[id-0x40])throw std::runtime_error("Invalid SDK lock id release");
    lockIds[id-0x40]=false;
}
void func_02079f00(unsigned int banks,unsigned short id) {
    std::lock_guard guard(lockIdMutex);
    for(unsigned i=0;i<vramOwners.size();++i)if((banks&(1u<<i)) && vramOwners[i]==id)vramOwners[i]=0;
}
std::uint64_t func_02079bf0() {
    const auto ns=static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()-start).count());
    constexpr std::uint64_t denominator=64000000000ull;
    return (ns/denominator)*33513982ull+(ns%denominator)*33513982ull/denominator;
}
unsigned short func_02079c90() { return static_cast<unsigned short>(func_02079bf0()); }
void func_02079e40(OSOwnerInfo* info) {
    if(!info) throw std::runtime_error("Null owner info");
    *info={}; info->language=static_cast<u8>(pc::gameSettings().language);
}
unsigned int OS_DisableIRQ() {
    criticalSection.lock();
    const unsigned int previous=irqDepth ? 0x80u : 0;
    ++irqDepth; return previous;
}
unsigned int OS_RestoreIRQ(unsigned int previous) {
    if(!irqDepth || (previous & 0x80u)!=(irqDepth>1 ? 0x80u : 0u))
        throw std::runtime_error("Unbalanced native critical section");
    --irqDepth;
    criticalSection.unlock();
    return 0x80u;
}
// Host CPU caches are coherent. SDK barriers become host memory fences.
void DC_CleanAll() { std::atomic_thread_fence(std::memory_order_seq_cst); }
void DC_CleanRange(const void*,unsigned int) { std::atomic_thread_fence(std::memory_order_seq_cst); }
void DC_InvalidateRange(const void*,unsigned int) { std::atomic_thread_fence(std::memory_order_seq_cst); }
void DC_PurgeRange(const void*,unsigned int) { std::atomic_thread_fence(std::memory_order_seq_cst); }
void DC_DrainWriteBuffer() { std::atomic_thread_fence(std::memory_order_seq_cst); }
}
