#include "NitroPower.hpp"
#include "NitroVideo.hpp"
#include <stdexcept>
namespace pc {
namespace {
thread_local NitroPower* power=nullptr;
std::uint16_t& displayPower() {return *static_cast<std::uint16_t*>(activeVideo().reg(0x04000304,2));}
void invoke(SleepCallback* callback) {
    for(;callback;callback=callback->next)callback->function(callback->argument);
}
}
NitroPower::NitroPower() {
    if(power)throw std::runtime_error("Native power context already active");power=this;
}
NitroPower::~NitroPower() {power=nullptr;}
NitroPower& NitroPower::current() {if(!power)throw std::runtime_error("No native power context");return *power;}
void NitroPower::add(SleepCallback* callback,bool post,bool prepend) {
    if(!callback || !callback->function)throw std::runtime_error("Invalid sleep callback");
    auto*& first=post?after_:before_;
    for(auto* p=first;p;p=p->next)if(p==callback)throw std::runtime_error("Sleep callback registered twice");
    if(prepend) {callback->next=first;first=callback;return;}
    auto** tail=&first;while(*tail)tail=&(*tail)->next;callback->next=nullptr;*tail=callback;
}
void NitroPower::remove(SleepCallback* callback,bool post) {
    auto** link=post?&after_:&before_;
    while(*link) {if(*link==callback) {*link=callback->next;return;}link=&(*link)->next;}
}
bool NitroPower::display() const {return (displayPower()&1)!=0;}
bool NitroPower::display(bool on) {
    if(on) {if(frame_-offFrame_<=7)return false;displayPower()|=1;}
    else {displayPower()&=~1;offFrame_=frame_;}
    return true;
}
void NitroPower::sleep(unsigned trigger,unsigned logic,unsigned keys) {
    if(sleeping_)throw std::runtime_error("Recursive native sleep request");
    if(!sleepHandler_)throw std::runtime_error("Sleep requires the native application event pump");
    sleeping_=true;
    auto& main=*static_cast<std::uint32_t*>(activeVideo().reg(0x04000000,4));
    auto& sub=*static_cast<std::uint32_t*>(activeVideo().reg(0x04001000,4));
    const auto savedMain=main,savedSub=sub;const auto savedPower=displayPower();
    try {
        invoke(before_);main&=~0x30000u;sub&=~0x10000u;displayPower()&=~1;
        sleepHandler_(trigger,logic,keys);
    }catch(...) {main=savedMain;sub=savedSub;displayPower()=savedPower;sleeping_=false;throw;}
    main=savedMain;sub=savedSub;displayPower()=savedPower;sleeping_=false;invoke(after_);
}
}
extern "C" {
void func_0207c1c4(pc::SleepCallback* info) {pc::NitroPower::current().add(info,false,false);}
void func_0207c1dc(pc::SleepCallback* info) {pc::NitroPower::current().add(info,false,true);}
void func_0207c1f4(pc::SleepCallback* info) {pc::NitroPower::current().add(info,true,false);}
void func_0207c20c(pc::SleepCallback* info) {pc::NitroPower::current().remove(info,false);}
void func_0207c224(pc::SleepCallback* info) {pc::NitroPower::current().remove(info,true);}
void func_0207bdb4(unsigned trigger,unsigned short logic,unsigned short keys) {pc::NitroPower::current().sleep(trigger,logic,keys);}
int func_0207c0b8(unsigned on) {return pc::NitroPower::current().display(on==1);}
unsigned pcVBlankCount() {return pc::NitroPower::current().vblankCount();}
unsigned func_0207c0d8() {return pc::NitroPower::current().display();}
}
