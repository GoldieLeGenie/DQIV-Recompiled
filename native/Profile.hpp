#pragma once
// Frame stage timings, printed every 120 frames when DQIV_PROFILE is set.
#include <chrono>

namespace pc {
bool profiling();
void profileAdd(const char* stage,double microseconds,unsigned count=1);
// Prints and resets the averages after `frames` calls.
void profileFrame(unsigned frames=120);
class ProfileScope {
public:
    explicit ProfileScope(const char* stage):stage_(stage),start_(std::chrono::steady_clock::now()) {}
    ~ProfileScope() {
        if(profiling())profileAdd(stage_,std::chrono::duration<double,std::micro>(std::chrono::steady_clock::now()-start_).count());
    }
private:
    const char* stage_;
    std::chrono::steady_clock::time_point start_;
};
}
