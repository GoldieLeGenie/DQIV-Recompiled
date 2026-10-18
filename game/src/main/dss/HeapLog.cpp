#include "main/dss/HeapLog.hpp"
#include "main/dss/DssUtils.hpp"
#include "nnsys/fnd.hpp"

char data_02120548[0x400];
UnkHeapLog data_02120948;

ARM void UnkHeapLog::unkfunc_02089214()
{
    UnkHeapLogEntry* entry = entries_;
    for (int i = 0; i < 100; i++) {
        entry->state_ = 0;
        entry->addr_ = NULL;
        entry->size_ = 0;
        entry->caller_ = NULL;
        entry++;
    }
    enabled_ = 1;
    unk_644 = 0;
}

ARM void UnkHeapLog::unkfunc_02089250(const char* name, void* addr, int size, void* caller, void** heap)
{
    func_02068684(*heap);
    func_020686ac(*heap, 4);
    if (enabled_ == 0) {
        return;
    }
    UnkHeapLogEntry* entry = entries_;
    for (int i = 0; i < 100; i++) {
        if (entry->state_ == 0) {
            entry->state_ = 1;
            entry->addr_ = addr;
            entry->size_ = size;
            entry->caller_ = caller;
            return;
        }
        entry++;
    }
    unkfunc_0208934c();
}

ARM void UnkHeapLog::unkfunc_020892cc(const char* name, void* addr, void* caller, void** heap)
{
    func_02068684(*heap);
    func_020686ac(*heap, 4);
    if (enabled_ == 0) {
        return;
    }
    UnkHeapLogEntry* entry = entries_;
    for (int i = 0; i < 100; i++) {
        if (entry->state_ == 1 && entry->addr_ == addr) {
            entry->state_ = 0;
            entry->addr_ = NULL;
            entry->size_ = 0;
            entry->caller_ = NULL;
            return;
        }
        entry++;
    }
    unkfunc_0208934c();
}

ARM char* UnkHeapLog::unkfunc_0208934c()
{
    data_02120548[0] = 0;
    UnkHeapLogEntry* entry = entries_;
    for (int i = 0; i < 100; i++) {
        if (entry->state_ != 0 && entry->state_ == 1) {
            char buf[0x100];
            dss::sprintf_s(buf, sizeof(buf), "A=%07x Z=%d F=%07x\n", entry->addr_, entry->size_, entry->caller_);
            dss::strcat_s(data_02120548, sizeof(data_02120548), buf);
        }
        entry++;
    }
    return data_02120548;
}

ARM void UnkHeapLog::unkfunc_020893e0()
{
    if (enabled_ == 0) {
        return;
    }
    UnkHeapLogEntry* entry = entries_;
    for (int i = 0; i < 100; i++) {
        if (entry->state_ == 1) {
            entry->state_ = 2;
        }
        entry++;
    }
}
