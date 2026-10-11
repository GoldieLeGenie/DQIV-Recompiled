#include "Profile.hpp"
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>

namespace pc {
namespace {
struct Total {double microseconds=0;unsigned count=0;};
std::map<std::string,Total>& totals() {static std::map<std::string,Total> value;return value;}
unsigned frames=0;
}
bool profiling() {static const bool enabled=std::getenv("DQIV_PROFILE")!=nullptr;return enabled;}
void profileAdd(const char* stage,double microseconds,unsigned count) {
    auto& total=totals()[stage];total.microseconds+=microseconds;total.count+=count;
}
void profileFrame(unsigned every) {
    // DQIV_PROFILE=n prints every n frames instead.
    static const unsigned chosen=[] {const char* v=std::getenv("DQIV_PROFILE");return v?unsigned(std::atoi(v)):0u;}();
    if(chosen>0)every=chosen;
    if(!profiling()||++frames<every)return;
    std::printf("Profile per frame:");
    for(const auto& [stage,total]:totals())std::printf(" %s %.0fus/%u",stage.c_str(),total.microseconds/frames,total.count/frames);
    std::printf("\n");std::fflush(stdout);
    totals().clear();frames=0;
}
}
