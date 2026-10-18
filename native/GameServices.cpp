#include "GameServices.hpp"
#include "Compression.hpp"
#include "Archive.hpp"
#include "main/data/FileLoader.hpp"
#include <algorithm>
#include <cstring>
#include <climits>
#include <deque>
#include <map>
#include <memory>
#include <new>
#include <stdexcept>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

namespace {
struct Allocation { std::size_t size, alignment; void* heap; };
struct Heap { std::size_t capacity, used = 0; };
std::map<void*, Allocation>& allocations() { static std::map<void*, Allocation> value; return value; }
std::map<void*, std::unique_ptr<Heap>>& heaps() { static std::map<void*, std::unique_ptr<Heap>> value; return value; }
// DS heaps live in fixed RAM: a freed block keeps its contents until reused,
// and the game reads some right after freeing them (a monster's palette is
// copied from its texture file after the file is released). Freed blocks stay
// mapped in a bounded queue before returning to the host allocator.
struct Freed { void* pointer; std::size_t size, alignment; };
struct Quarantine { std::deque<Freed> blocks; std::size_t bytes = 0; };
Quarantine& quarantine() { static Quarantine value; return value; }
constexpr std::size_t quarantineLimit = 32 * 1024 * 1024;
void retire(void* p, std::size_t size, std::size_t alignment) {
    auto& q = quarantine(); q.blocks.push_back({p, size, alignment}); q.bytes += size;
    while (q.bytes > quarantineLimit && q.blocks.size() > 1) {
        const auto oldest = q.blocks.front(); q.blocks.pop_front(); q.bytes -= oldest.size;
        ::operator delete(oldest.pointer, std::align_val_t(oldest.alignment));
    }
}
void drainQuarantine() {
    auto& q = quarantine();
    for (const auto& block : q.blocks) ::operator delete(block.pointer, std::align_val_t(block.alignment));
    q.blocks.clear(); q.bytes = 0;
}
const pc::Assets* mounted = nullptr;
Heap& getHeap(void* p) {
    auto it = heaps().find(p);
    if (it == heaps().end()) throw std::runtime_error("Uninitialized game heap");
    return *it->second;
}
void* makeHeap(std::size_t capacity) {
    auto heap = std::make_unique<Heap>(); heap->capacity = capacity;
    auto pointer = heap.get(); heaps().emplace(pointer, std::move(heap)); return pointer;
}
void* allocate(void* heap, int size, int alignment) {
    if (size < 0 || alignment == INT_MIN) throw std::runtime_error("Invalid allocation size/alignment");
    auto align = static_cast<std::size_t>(alignment < 0 ? -alignment : alignment);
    if (align == 0 || (align & (align - 1))) throw std::runtime_error("Alignment must be a power of two");
    align = std::max(align, alignof(std::max_align_t));
    const auto n = std::max<std::size_t>(size, 1);
    auto& h = getHeap(heap);
    if (n > h.capacity - h.used) throw std::bad_alloc();
    void* p = ::operator new(n, std::align_val_t(align));
    try { allocations().emplace(p, Allocation{n, align, heap}); }
    catch (...) { ::operator delete(p, std::align_val_t(align)); throw; }
    h.used += n; return p;
}
void release(void* heap, void* p) {
    if (!p) return;
    auto it = allocations().find(p);
    if (it == allocations().end() || it->second.heap != heap) throw std::runtime_error("Invalid game heap free");
    auto a = it->second; getHeap(heap).used -= a.size;
    allocations().erase(it); retire(p, a.size, a.alignment);
}
struct AllocatorFunctions {
    void* (*alloc)(NNSFndAllocator*, unsigned int);
    void (*free)(NNSFndAllocator*, void*);
};
void* allocatorAlloc(NNSFndAllocator* allocator, unsigned int size) {
    if (size > INT_MAX) throw std::bad_alloc();
    return allocate(allocator->pHeap, static_cast<int>(size), static_cast<int>(allocator->heapParam1));
}
void allocatorFree(NNSFndAllocator* allocator, void* pointer) { release(allocator->pHeap,pointer); }
AllocatorFunctions hostAllocatorFunctions{allocatorAlloc,allocatorFree};
void initAllocator(NNSFndAllocator* allocator, void* heap, int alignment) {
    *allocator = {&hostAllocatorFunctions,heap,static_cast<unsigned int>(alignment),0};
}
const pc::Assets& assets() {
    if (!mounted) throw std::runtime_error("Game filesystem not mounted");
    return *mounted;
}
std::vector<std::uint8_t> read(const char* name) {
    if (!assets().contains(name)) return {};
    auto data = assets().read(name);
    if (data.size() > INT_MAX) throw std::runtime_error("Asset too large for game API");
    return data;
}
}
UnkHeap data_0211a60c{};
UnkTouchPanel::UnkTouchPanel() : unk_00{}, touch_(0), x_(0), y_(0), unk_24(0), unk_28(0), unk_2c(0) {}
UnkTouchPanel data_0211a5d4;
void* data_0211a604[2]{};
dss::File dss::g_File{};

namespace pc {
GameServices::GameServices(const Assets& files) {
    if (mounted) throw std::runtime_error("Game services already running");
    mounted = &files;
    try { initHeap(&data_0211a60c, 0x18000, 0x1000, 64 * 1024 * 1024, 0); dss::g_File.unkfunc_0207ea64(0); }
    catch (...) { heaps().clear(); mounted = nullptr; throw; }
}
GameServices::~GameServices() {
    for (const auto& [p, a] : allocations()) ::operator delete(p, std::align_val_t(a.alignment));
    drainQuarantine();
    allocations().clear(); heaps().clear(); data_0211a60c = {}; data_0211a604[0] = data_0211a604[1] = nullptr; mounted = nullptr;
}
std::size_t liveGameAllocations() { return allocations().size(); }
bool isGameTextPointer(std::uintptr_t value) {
    // The original menu API encodes either a string pointer or a message ID
    // in an integer and recognizes pointers by the DS RAM address range.
    // Host stacks/static data are often below that range. Check ownership
    // without dereferencing arbitrary message IDs as addresses.
    if(!value)return false;
    auto it=allocations().upper_bound(reinterpret_cast<void*>(value));
    if(it!=allocations().begin()) {
        --it;const auto start=reinterpret_cast<std::uintptr_t>(it->first);
        if(value>=start&&value-start<it->second.size)return true;
    }
    MEMORY_BASIC_INFORMATION info{};
    if(!VirtualQuery(reinterpret_cast<void*>(value),&info,sizeof(info))||info.State!=MEM_COMMIT||
        (info.Protect&(PAGE_GUARD|PAGE_NOACCESS))||!(info.Protect&(PAGE_READONLY|PAGE_READWRITE|PAGE_WRITECOPY)))return false;
    ULONG_PTR low=0,high=0;GetCurrentThreadStackLimits(&low,&high);
    // Reserved stack space can overlap numeric message IDs in a Win32 process.
    // Only committed bytes in active caller frames can hold a supplied string.
    if(value>=std::max<std::uintptr_t>(low,reinterpret_cast<std::uintptr_t>(&info))&&value<high)return true;
    return info.Type==MEM_IMAGE&&info.AllocationBase==GetModuleHandleW(nullptr);
}
std::size_t gameBytesAfter(const void* pointer) {
    const auto value=reinterpret_cast<std::uintptr_t>(pointer);
    auto it=allocations().upper_bound(const_cast<void*>(pointer));
    if(it==allocations().begin())return 0;
    --it;const auto start=reinterpret_cast<std::uintptr_t>(it->first);
    return value>=start&&value-start<it->second.size?it->second.size-(value-start):0;
}
std::span<const std::uint8_t> gameAllocation(const void* pointer) {
    const auto address = reinterpret_cast<std::uintptr_t>(pointer);
    for (const auto& [p, a] : allocations()) {
        const auto begin = reinterpret_cast<std::uintptr_t>(p);
        if (address >= begin && address - begin < a.size)
            return {static_cast<const std::uint8_t*>(pointer), a.size - (address - begin)};
    }
    throw std::runtime_error("Resource pointer is outside a game allocation");
}
}
void initHeap(UnkHeap* h, unsigned int s0, unsigned int s1, unsigned int s2, int) {
    if (h->heap0_ || h->heap1_ || h->heap2_) throw std::runtime_error("Game heaps already initialized");
    h->heap0_ = makeHeap(s0); h->heap1_ = makeHeap(s1); h->heap2_ = makeHeap(s2);
    h->size0_ = s0; h->size1_ = s1; h->size2_ = s2;
    h->free0_ = s0; h->free1_ = s1; h->free2_ = s2;
    initAllocator(&h->allocator0_,h->heap0_,32);
    initAllocator(&h->allocator1_,h->heap1_,32);
    initAllocator(&h->allocator2_,h->heap2_,32);
}
extern "C" int pcTryInitAllocator(void* allocator, void* heap, int alignment) {
    if (!heaps().contains(heap)) return 0;
    initAllocator(static_cast<NNSFndAllocator*>(allocator),heap,alignment);
    return 1;
}
void* allocAppHeap(UnkHeap* h, int n, int a) { return allocate(h->heap2_, n, a); }
void* dssAlloc(UnkHeap* h, int n, int a) { return allocate(h->heap2_, n, a); }
void freeSystemHeap(UnkHeap* h, void* p) { release(h->heap1_, p); }
void freeAppHeap(UnkHeap* h, void* p) { release(h->heap2_, p); }
void dssFree(UnkHeap* h, void* p) { release(h->heap2_, p); }
unsigned int getHeap0FreeSize(UnkHeap* h) { auto& v = getHeap(h->heap0_); return static_cast<unsigned int>(v.capacity - v.used); }
unsigned int getSystemHeapFreeSize(UnkHeap* h) { auto& v = getHeap(h->heap1_); return static_cast<unsigned int>(v.capacity - v.used); }
unsigned int getAppHeapFreeSize(UnkHeap* h) { auto& v = getHeap(h->heap2_); return static_cast<unsigned int>(v.capacity - v.used); }
unsigned int getAppHeapTotalFreeSize(UnkHeap* h) { return getAppHeapFreeSize(h); }
void** getHeap0(UnkHeap* h) { return &h->heap0_; }
void** getSystemHeap(UnkHeap* h) { return &h->heap1_; }
void freeSystemMemory(void* p) { freeSystemHeap(&data_0211a60c, p); }

void setCompressedData(void* src) { data_0211a604[0] = src; }
unsigned int getUncompressedSize(void* src) {
    auto bytes = pc::gameAllocation(src ? src : data_0211a604[0]);
    if (bytes.size() < 4) throw std::runtime_error("Compression header truncated");
    return bytes[1] | (unsigned(bytes[2]) << 8) | (unsigned(bytes[3]) << 16);
}
int isCompressed(void* src) { return pc::isCompressed(pc::gameAllocation(src)); }
void uncompressData(void* dst) {
    auto decoded = pc::decompress(pc::gameAllocation(data_0211a604[0]));
    if (pc::gameAllocation(dst).size() < decoded.size()) throw std::runtime_error("Decompression destination too small");
    std::memcpy(dst, decoded.data(), decoded.size()); data_0211a604[1] = dst;
}
void* uncompressAlloc(void* src) {
    auto decoded = pc::decompress(pc::gameAllocation(src));
    auto* p = dssAlloc(&data_0211a60c, static_cast<int>(decoded.size()), 4);
    std::memcpy(p, decoded.data(), decoded.size()); return p;
}
namespace {
void decompressSdk(const void* src, void* dst, unsigned format) {
    const auto input=pc::gameAllocation(src);
    if(input.empty() || (input[0]&0xf0)!=format) throw std::runtime_error("Wrong SDK decompression format");
    const auto decoded=pc::decompress(input);
    if(!dst && !decoded.empty()) throw std::runtime_error("Null decompression destination");
    // Like the original SDK entry points, the caller provides output capacity.
    if(!decoded.empty()) std::memcpy(dst,decoded.data(),decoded.size());
}
}
extern "C" void func_02067b88(const void* src,void* dst) { decompressSdk(src,dst,0x10); }
extern "C" void func_02067c1c(const void* src,void* dst) { decompressSdk(src,dst,0x20); }
extern "C" void func_02067cf4(const void* src,void* dst) { decompressSdk(src,dst,0x30); }
unsigned int roundUp(unsigned int n, unsigned int a) { return (n + a - 1) & ~(a - 1); }
unsigned int roundDown(unsigned int n, unsigned int a) { return n & ~(a - 1); }
void dss::File::unkfunc_0207ea64(int) { unkfunc_0207ea80(); unk_4c = unk_50 = size_ = 0; }
void dss::File::unkfunc_0207ea80() { assets(); unk_54 = 0x200; }
int dss::File::unkfunc_0207eac0(const char* name, void* dst, int a) {
    int result = unkfunc_0207ec08(name, dst); if (!a) unk_4c = 0; return result;
}
int dss::File::unkfunc_0207eaf8(const char* n, void* p) { return unkfunc_0207ec08(n, p); }
int dss::File::unkfunc_0207eb10(const char* n, void* p) { return unkfunc_0207ec08(n, p); }
void* dss::File::unkfunc_0207eb28(const char* name, int tail, int mode) {
    const int n = unkfunc_0207ecb0(name);
    if (!assets().contains(name)) throw std::runtime_error(std::string("Game resource missing: ") + name);
    void* p = dssAlloc(&data_0211a60c, n, tail ? -32 : 32);
    try { unkfunc_0207eac0(name, p, mode); }
    catch (...) { dssFree(&data_0211a60c, p); throw; }
    return p;
}
int dss::File::unkfunc_0207eba0(const char* n, int align) { auto size = unkfunc_0207ecb0(n); return align ? roundUp(size, 0x200) : size; }
bool dss::File::isExist(const char* n) { return unkfunc_0207ecb0(n) != 0; }
int dss::File::unkfunc_0207ebf0() { return size_; }
int dss::File::unkfunc_0207ecb0(const char* n) { return static_cast<int>(read(n).size()); }
int dss::File::unkfunc_0207ec08(const char* name, void* dst) {
    auto bytes = read(name);
    if (unk_4c < 0 || unk_50 < 0) throw std::runtime_error("Negative game file range");
    const auto offset = std::min<std::size_t>(unk_4c, bytes.size());
    auto count = bytes.size() - offset;
    if (unk_50) { count = std::min<std::size_t>(count, unk_50); unk_50 = 0; }
    if (count) { if (!dst) throw std::runtime_error("Null game file destination"); std::memcpy(dst, bytes.data() + offset, count); }
    size_ = static_cast<int>(count); return size_;
}
int getArchiveFileCount(void* archive) { return static_cast<int>(pc::Archive(pc::gameAllocation(archive)).count()); }
int getArchiveFileSize(void* archive, int index) { return static_cast<int>(pc::Archive(pc::gameAllocation(archive)).file(index).size()); }
void* getArchiveFile(void* archive, int index) { return const_cast<std::uint8_t*>(pc::Archive(pc::gameAllocation(archive)).file(index).data()); }
