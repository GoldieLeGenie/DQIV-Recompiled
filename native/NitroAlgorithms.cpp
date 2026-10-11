#include <algorithm>
#include <bit>
#include <cstdint>
#include <stdexcept>
#include <utility>
#include <vector>
extern "C" {
std::uint8_t func_02066df0(std::uint32_t value) {return static_cast<std::uint8_t>(std::popcount(value));}
// Preserve the original ARM quicksort's pivot and equal-key permutations:
// scene ordering can depend on them even though the sort is not stable.
void func_02066e34(void* head,unsigned count,unsigned size,int (*compare)(void*,void*),void*) {
    if(count<=1)return;
    if(!head || !size || !compare || std::uint64_t(count)*size>0xffffffffu)throw std::runtime_error("Invalid SDK sort range");
    auto* bytes=static_cast<std::uint8_t*>(head);
    auto at=[&](std::size_t index) {return bytes+index*size;};
    auto swap=[&](std::size_t a,std::size_t b) {if(a!=b)std::swap_ranges(at(a),at(a)+size,at(b));};
    std::vector<std::pair<std::size_t,std::size_t>> ranges{{0,count-1}};
    while(!ranges.empty()) {
        const auto [first,last]=ranges.back();ranges.pop_back();
        if(last-first==1) {if(compare(at(first),at(last))>0)swap(first,last);continue;}
        const auto pivot=first+(((last-first)*size)>>std::bit_width(size));swap(first,pivot);
        auto left=first+1,right=last;
        for(;;) {
            while(left<last && compare(at(left),at(first))<0)++left;
            while(compare(at(right),at(first))>0)--right;
            if(left>=right)break;
            swap(left,right);++left;--right;if(left>right)break;
        }
        swap(first,right);
        auto pushLeft=[&]() {if(right>first+1)ranges.emplace_back(first,right-1);};
        auto pushRight=[&]() {if(right+1<last)ranges.emplace_back(right+1,last);};
        if(right-first>last-right) {pushLeft();pushRight();}else {pushRight();pushLeft();}
    }
}
}
