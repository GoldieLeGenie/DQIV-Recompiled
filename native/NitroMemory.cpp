#include <cstdint>
#include <cstring>
#include <atomic>

namespace {
template<class T> void fill(void* destination, T value, std::uint32_t size) {
    auto* bytes = static_cast<std::uint8_t*>(destination);
    // The ARM loops write a whole unit while offset < size (rounding up).
    for (std::size_t offset = 0; offset < size; offset += sizeof(T))
        std::memcpy(bytes + offset, &value, sizeof(T));
}
template<class T> void copy(const void* source, void* destination, std::uint32_t size) {
    const auto* src = static_cast<const std::uint8_t*>(source);
    auto* dst = static_cast<std::uint8_t*>(destination);
    for (std::size_t offset = 0; offset < size; offset += sizeof(T)) {
        T value;
        std::memcpy(&value, src + offset, sizeof(T));
        std::memcpy(dst + offset, &value, sizeof(T));
    }
}
}
extern "C" {
// ARM9 BIOS SWI 0x0c: length is in words, bit 24 selects fixed-source fill.
void CpuFastSet(const void* src,void* dst,std::uint32_t control) {
    const auto bytes=(control&0x1fffffu)*4;
    if(!bytes)return;
    if(control&(1u<<24)) {std::uint32_t value;std::memcpy(&value,src,4);fill(dst,value,bytes);}
    else copy<std::uint32_t>(src,dst,bytes);
}
void MI_CpuCopyU8(const void* src, void* dst, std::uint32_t size) { if (size) std::memmove(dst,src,size); }
void MI_CpuCopyU16(const void* src, void* dst, std::uint32_t size) { copy<std::uint16_t>(src,dst,size); }
void MI_CpuCopyU32(const void* src, void* dst, std::uint32_t size) { copy<std::uint32_t>(src,dst,size); }
void MI_CpuCopy(const void* src, void* dst, std::uint32_t size) { copy<std::uint32_t>(src,dst,size); }
void func_02067924(const void* src, void* dst) { std::memmove(dst,src,32); }
void func_02067940(const void* src, void* dst) { std::memmove(dst,src,36); }
void MI_CpuCopy48(const void* src, void* dst) { std::memmove(dst,src,48); }
void MI_CpuCopy64(const void* src, void* dst) { std::memmove(dst,src,64); }
// The legacy symbol's suffix is hexadecimal: nine ARM words = 0x24 bytes.
void MI_CpuClear24(void* dst) { std::memset(dst,0,36); }
std::uint32_t func_02067b80(std::uint32_t value,std::uint32_t* address) {
    return std::atomic_ref<std::uint32_t>(*address).exchange(value);
}
void MI_CpuSet(void* dst, std::uint8_t value, std::uint32_t size) { if (size) std::memset(dst,value,size); }
void MI_CpuFillU16(std::uint16_t value, void* dst, std::uint32_t size) { fill(dst,value,size); }
void func_0206785c(std::uint32_t value, void* dst, std::uint32_t size) { fill(dst,value,size); }
void MI_CpuFill(std::uint32_t value, void* dst, std::uint32_t size) { fill(dst,value,size); }
// Native transfers complete before returning; no DMA device remains pending.
void func_020670cc(std::uint32_t,void* dst,std::uint32_t value,std::uint32_t size) {fill(dst,value,size);}
void func_0206714c(std::uint32_t,const void* src,void* dst,std::uint32_t size) {copy<std::uint32_t>(src,dst,size);}
void func_020671bc(std::uint32_t,const void* src,void* dst,std::uint32_t size) {copy<std::uint16_t>(src,dst,size);}
void func_020672ec(std::uint32_t,const void* src,void* dst,std::uint32_t size,void (*done)(void*),void* arg) {
    copy<std::uint32_t>(src,dst,size);if(done)done(arg);
}
void func_02067228(std::uint32_t,void* dst,std::uint32_t value,std::uint32_t size,void (*done)(void*),void* arg) {
    fill(dst,value,size);if(done)done(arg);
}
void func_02067384(std::uint32_t) {}
}
