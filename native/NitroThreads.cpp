#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include "NitroThreads.hpp"
#include <algorithm>
#include <cstring>
#include <exception>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>
extern "C" {
#include <nitro/os/mutex.h>
unsigned OS_DisableIRQ();
unsigned OS_RestoreIRQ(unsigned);
OSThreadInfo ThreadInfo{};
}
static_assert(sizeof(OSThread)==0xc0 && sizeof(OSMutex)==0x18);
namespace pc {
namespace {
struct StopThread {};
struct IrqGuard {
    unsigned previous=OS_DisableIRQ();
    ~IrqGuard(){OS_RestoreIRQ(previous);}
};
thread_local ThreadRuntime* runtime=nullptr;
}
struct ThreadRuntime {
    struct Record {
        ThreadRuntime* owner;
        OSThread* sdk;
        void* fiber=nullptr;
        void (*function)(void*)=nullptr;
        void* argument=nullptr;
        bool main=false;
    };
    std::unordered_map<OSThread*,std::unique_ptr<Record>> records;
    std::unordered_set<OSMutex*> initializedMutexes;
    std::vector<std::exception_ptr> failures;
    std::function<void()> idleHandler;
    OSThread mainThread{};
    OSThread eventThread{};
    unsigned nextId=1;
    bool converted=false,stopping=false;
    DWORD ownerThread=GetCurrentThreadId();

    ThreadRuntime() {
        if(runtime)throw std::runtime_error("Native thread runtime already active");
        void* fiber;
        if(IsThreadAFiber())fiber=GetCurrentFiber();
        else {fiber=ConvertThreadToFiberEx(nullptr,FIBER_FLAG_FLOAT_SWITCH);converted=true;}
        if(!fiber)throw std::runtime_error("Could not initialize Windows fibers");
        mainThread.state=1;mainThread.priority=16;
        records.emplace(&mainThread,std::make_unique<Record>(Record{this,&mainThread,fiber,nullptr,nullptr,true}));
        ThreadInfo={};ThreadInfo.current=&mainThread;ThreadInfo.list=&mainThread;runtime=this;
    }
    void assertOwner() const {
        if(GetCurrentThreadId()!=ownerThread)throw std::runtime_error("SDK scheduler called outside its owning OS thread");
    }
    static ThreadRuntime& get() {if(!runtime)throw std::runtime_error("No native thread runtime is active");runtime->assertOwner();return *runtime;}
    void insert(OSThread* thread) {
        auto** next=&ThreadInfo.list;
        while(*next && (*next)->priority<thread->priority)next=&(*next)->next;
        thread->next=*next;*next=thread;
    }
    void erase(OSThread* thread) {
        auto** next=&ThreadInfo.list;
        while(*next && *next!=thread)next=&(*next)->next;
        if(*next)*next=thread->next;
        thread->next=nullptr;
    }
    void unlinkWait(OSThread* thread) {
        auto* q=thread->queue;if(!q)return;
        if(thread->list.prev)thread->list.prev->list.next=thread->list.next;else q->head=thread->list.next;
        if(thread->list.next)thread->list.next->list.prev=thread->list.prev;else q->tail=thread->list.prev;
        thread->list={};thread->queue=nullptr;
    }
    void enqueue(OSThreadQueue* q,OSThread* thread) {
        unlinkWait(thread);thread->queue=q;
        if(!q)return;
        OSThread* next=q->head;OSThread* previous=nullptr;
        while(next && next->priority<=thread->priority) {previous=next;next=next->list.next;}
        thread->list.prev=previous;thread->list.next=next;
        if(previous)previous->list.next=thread;else q->head=thread;
        if(next)next->list.prev=thread;else q->tail=thread;
    }
    void wake(OSThread* thread) {
        if(!records.contains(thread))throw std::runtime_error("Wake of unknown SDK thread");
        if(thread->state==2)return;
        unlinkWait(thread);thread->state=1;
    }
    void wakeQueue(OSThreadQueue* queue) {while(queue->head)wake(queue->head);}
    void switchTo(Record& next) {
        if(next.sdk==ThreadInfo.current)return;
        const auto depth=suspendNativeInterrupts();
        ThreadInfo.current=next.sdk;SwitchToFiber(next.fiber);
        resumeNativeInterrupts(depth);
    }
    static VOID WINAPI eventEntry(void* pointer) {
        auto& r=*static_cast<ThreadRuntime*>(pointer);
        try {
            while(!r.stopping) {
                if(!r.idleHandler)throw std::runtime_error("Native event source was removed while waiting");
                auto callback=r.idleHandler;callback();r.schedule();
            }
        }catch(const StopThread&) {}
        catch(...) {r.failures.push_back(std::current_exception());}
        suspendNativeInterrupts();r.finish(&r.eventThread);r.wake(&r.mainThread);
        r.switchTo(*r.records.at(&r.mainThread));std::terminate();
    }
    Record& eventSource() {
        if(auto found=records.find(&eventThread);found!=records.end()) {
            if(eventThread.state==2)throw std::runtime_error("Native event source has failed");
            return *found->second;
        }
        eventThread.state=1;eventThread.priority=32;
        auto record=std::make_unique<Record>(Record{this,&eventThread});
        record->fiber=CreateFiberEx(64*1024,1024*1024,FIBER_FLAG_FLOAT_SWITCH,eventEntry,this);
        if(!record->fiber)throw std::runtime_error("Could not allocate native event fiber");
        auto* result=record.get();records.emplace(&eventThread,std::move(record));return *result;
    }
    void schedule() {
        Record* selected=nullptr;
        for(auto* sdk=ThreadInfo.list;sdk;sdk=sdk->next)if(sdk->state==1) {selected=records.at(sdk).get();break;}
        if(selected) {switchTo(*selected);return;}
        if(!idleHandler)throw std::runtime_error("All SDK threads are waiting without a native event source");
        // An event pump needs its own stack. A worker awakened by an audio
        // alarm can block again, then return here to the suspended pump instead
        // of recursively rendering audio on that worker's wait stack.
        if(ThreadInfo.current!=&eventThread)switchTo(eventSource());
    }
    void removeMutex(OSMutex* mutex) {
        auto* owner=mutex->thread;
        if(mutex->list.prev)mutex->list.prev->list.next=mutex->list.next;else owner->mutexQueue.head=mutex->list.next;
        if(mutex->list.next)mutex->list.next->list.prev=mutex->list.prev;else owner->mutexQueue.tail=mutex->list.prev;
        mutex->thread=nullptr;mutex->count=0;mutex->list={};wakeQueue(&mutex->queue);
    }
    void finish(OSThread* sdk) {
        IrqGuard guard;
        while(sdk->mutexQueue.head)removeMutex(sdk->mutexQueue.head);
        unlinkWait(sdk);erase(sdk);sdk->state=2;wakeQueue(&sdk->joinQueue);
    }
    static VOID WINAPI entry(void* pointer) {
        auto& r=*static_cast<Record*>(pointer);auto& owner=*r.owner;
        try {
            if(owner.stopping)throw StopThread{};
            r.function(r.argument);
        }catch(const StopThread&) {}
        catch(...) {owner.failures.push_back(std::current_exception());}
        // Original C wait functions may have IRQ masks outstanding when a native
        // exception unwinds them. A terminated task cannot retain those locks.
        suspendNativeInterrupts();
        try {
            if(r.sdk->destructor) {auto callback=r.sdk->destructor;r.sdk->destructor=nullptr;callback(nullptr);}
        }catch(...) {owner.failures.push_back(std::current_exception());}
        suspendNativeInterrupts();
        owner.finish(r.sdk);
        if(!owner.failures.empty())owner.wake(&owner.mainThread);
        try {owner.schedule();}
        catch(...) {
            owner.failures.push_back(std::current_exception());
            owner.wake(&owner.mainThread);
            owner.switchTo(*owner.records.at(&owner.mainThread));
        }
        // Terminated fibers must never be scheduled again.
        std::terminate();
    }
    ~ThreadRuntime() {
        stopping=true;
        for(auto& [sdk,record]:records)if(!record->main && sdk!=&eventThread && sdk->state!=2)wake(sdk);
        for(auto& [sdk,record]:records)if(!record->main && sdk!=&eventThread && sdk->state!=2)switchTo(*record);
        if(auto event=records.find(&eventThread);event!=records.end() && eventThread.state!=2)switchTo(*event->second);
        for(auto& [sdk,record]:records)if(!record->main)DeleteFiber(record->fiber);
        ThreadInfo={};runtime=nullptr;
        if(converted)ConvertFiberToThread();
    }
};
NitroThreads::NitroThreads():impl_(std::make_unique<ThreadRuntime>()) {}
NitroThreads::~NitroThreads()=default;
void NitroThreads::checkErrors() {if(!impl_->failures.empty())std::rethrow_exception(impl_->failures.front());}
void NitroThreads::setIdleHandler(std::function<void()> handler) {impl_->idleHandler=std::move(handler);}
}
extern "C" {
void OS_InitThread() {(void)pc::ThreadRuntime::get();}
int func_02078d40() {return pc::runtime!=nullptr;}
void func_020787c4(OSThread* sdk,void (*function)(void*),void* argument,void* stack,unsigned size,unsigned priority) {
    auto& r=pc::ThreadRuntime::get();pc::IrqGuard guard;
    if(!sdk || !function || !stack || size<8 || priority>31 || r.records.contains(sdk))throw std::runtime_error("Invalid native SDK thread creation");
    *sdk={};sdk->priority=priority;sdk->id=r.nextId++;
    sdk->stackBottom=reinterpret_cast<u32>(stack);sdk->stackTop=sdk->stackBottom-size;
    auto* memory=static_cast<unsigned char*>(stack)-size;std::memset(memory,0,size);
    const unsigned top=0x7bf9dd5b,bottom=0xfddb597d;
    std::memcpy(memory,&top,4);std::memcpy(memory+size-4,&bottom,4);
    auto record=std::make_unique<pc::ThreadRuntime::Record>(pc::ThreadRuntime::Record{&r,sdk,nullptr,function,argument,false});
    record->fiber=CreateFiberEx(64*1024,1024*1024,FIBER_FLAG_FLOAT_SWITCH,pc::ThreadRuntime::entry,record.get());
    if(!record->fiber)throw std::runtime_error("Could not allocate native SDK fiber");
    r.records.emplace(sdk,std::move(record));
    r.insert(sdk);
}
void OS_CreateThread(OSThread* sdk,void (*function)(void*),void* argument,void* stack,unsigned size,unsigned priority) {
    func_020787c4(sdk,function,argument,stack,size,priority);
}
void OS_PauseThread(OSThreadQueue* queue) {
    auto& r=pc::ThreadRuntime::get();pc::IrqGuard guard;
    if(r.stopping)throw pc::StopThread{};
    if(ThreadInfo.current==&r.eventThread)throw std::runtime_error("Native event callback attempted to block on an SDK wait queue");
    auto* self=ThreadInfo.current;r.enqueue(queue,self);self->state=0;
    try {r.schedule();}catch(...) {r.wake(self);throw;}
    if(r.stopping)throw pc::StopThread{};
}
void OS_UnpauseThread(OSThreadQueue* queue) {
    auto& r=pc::ThreadRuntime::get();pc::IrqGuard guard;
    if(!queue)throw std::runtime_error("Null SDK wait queue");r.wakeQueue(queue);r.schedule();
}
void OS_WakeupThreadDirect(void* pointer) {
    auto& r=pc::ThreadRuntime::get();pc::IrqGuard guard;r.wake(static_cast<OSThread*>(pointer));r.schedule();
}
void OS_ExitThread() {throw pc::StopThread{};}
void func_020788c0() {OS_ExitThread();}
void OS_InitMutex(OSMutex* m) {
    auto& r=pc::ThreadRuntime::get();pc::IrqGuard guard;
    if(!m || (r.initializedMutexes.contains(m) && (m->thread || m->queue.head)))throw std::runtime_error("Reinitializing a busy SDK mutex");
    *m={};r.initializedMutexes.insert(m);
}
int OS_TryLockMutex(OSMutex* m) {
    auto& r=pc::ThreadRuntime::get();pc::IrqGuard guard;
    if(!m)throw std::runtime_error("Null SDK mutex");
    if(!r.initializedMutexes.contains(m)) {
        // NNS stream setup uses its static, zero-filled mutex before the
        // explicit thread initializer. That is already the SDK's empty state.
        if(m->queue.head || m->queue.tail || m->thread || m->count || m->list.prev || m->list.next)
            throw std::runtime_error("Uninitialized SDK mutex contains live state");
        r.initializedMutexes.insert(m);
    }
    auto* self=ThreadInfo.current;
    if(m->thread && m->thread!=self)return 0;
    if(!m->thread) {
        m->thread=self;m->list.prev=self->mutexQueue.tail;
        if(self->mutexQueue.tail)self->mutexQueue.tail->list.next=m;else self->mutexQueue.head=m;
        self->mutexQueue.tail=m;
    }
    ++m->count;return 1;
}
void OS_LockMutex(OSMutex* m) {
    pc::IrqGuard guard;
    while(!OS_TryLockMutex(m)) {
        ThreadInfo.current->mutex=m;
        OS_PauseThread(&m->queue);
        ThreadInfo.current->mutex=nullptr;
    }
}
void OS_UnlockMutex(OSMutex* m) {
    auto& r=pc::ThreadRuntime::get();pc::IrqGuard guard;
    if(!r.initializedMutexes.contains(m) || m->thread!=ThreadInfo.current || !m->count)throw std::runtime_error("SDK mutex released by nonowner");
    if(!--m->count) {r.removeMutex(m);r.schedule();}
}
}
