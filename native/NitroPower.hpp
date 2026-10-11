#pragma once
#include <cstdint>
#include <functional>
namespace pc {
struct SleepCallback {void (*function)(void*);void* argument;SleepCallback* next;};
class NitroPower {
public:
    NitroPower();
    ~NitroPower();
    NitroPower(const NitroPower&)=delete;
    NitroPower& operator=(const NitroPower&)=delete;
    static NitroPower& current();
    void advanceFrame() {++frame_;}
    // The VBlank count the system keeps in shared memory (HW_VBLANK_COUNT_BUF).
    std::uint32_t vblankCount() const {return static_cast<std::uint32_t>(frame_);}
    void setSleepHandler(std::function<void(unsigned,unsigned,unsigned)> handler) {sleepHandler_=std::move(handler);}
    void add(SleepCallback* callback,bool post,bool prepend);
    void remove(SleepCallback* callback,bool post);
    void sleep(unsigned trigger,unsigned logic,unsigned keys);
    bool display(bool on);
    bool display() const;
private:
    SleepCallback* before_=nullptr;
    SleepCallback* after_=nullptr;
    std::uint64_t frame_=8,offFrame_=0;
    bool sleeping_=false;
    std::function<void(unsigned,unsigned,unsigned)> sleepHandler_;
};
}
