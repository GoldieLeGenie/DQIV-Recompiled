#include "NitroWireless.hpp"
#include "ov002/mp/WirelessHelper.hpp"
#include <algorithm>
#include <array>
#include <cstring>
#include <deque>

namespace pc {
namespace {
// Room for the largest WM callback structure (WMStartScanCallback).
using Reply=std::array<std::uint8_t,0x100>;
struct Pending {
    unsigned due;
    void (*callback)(void*);
    Reply reply;
};
struct Radio {
    unsigned frame=0;
    std::deque<Pending> pending;
    void (*indication)(void*)=nullptr;
    void (*parent)(void*)=nullptr;
    unsigned beaconFrames=12,nextBeacon=0;
};
Radio& radio() {static Radio value;return value;}
unsigned frames(unsigned milliseconds) {return std::max(1u,(milliseconds*60+999)/1000);}
Reply reply(WMErrCode errcode) {
    Reply value{};std::memcpy(value.data()+2,&errcode,2);return value;
}
template<class T> void field(Reply& value,unsigned offset,T data) {std::memcpy(value.data()+offset,&data,sizeof data);}
WMErrCode queue(void (*callback)(void*),Reply value,unsigned delay=1) {
    if(callback)radio().pending.push_back({radio().frame+delay,callback,value});
    return WM_ERRCODE_OPERATING;
}
}
void advanceWireless() {
    auto& r=radio();++r.frame;
    if(r.parent&&r.frame>=r.nextBeacon) {
        // A parent reports each beacon it sends (state 2); nobody answers.
        auto value=reply(WM_ERRCODE_SUCCESS);field<u16>(value,8,2);
        r.pending.push_back({r.frame,r.parent,value});r.nextBeacon=r.frame+r.beaconFrames;
    }
    // Callbacks may queue further requests: deliver only those already due.
    for(std::size_t count=r.pending.size();count&&!r.pending.empty();--count) {
        auto next=r.pending.front();r.pending.pop_front();
        if(next.due>r.frame) {r.pending.push_back(next);continue;}
        next.callback(next.reply.data());
    }
}
}

using pc::queue;using pc::reply;using pc::field;
extern "C" {
WMErrCode func_0207cd74(void (*callback)(void* arg)) {pc::radio().indication=callback;return WM_ERRCODE_SUCCESS;}
u16 func_0207cfc0(void) {return 0x2082;} // channels 1, 7 and 13
int func_0207cfe0(void) {return 0;}
u16 func_0207d070(void) {return 200;}
u16 func_0207d100(void) {return 30;}
WMErrCode func_0207d194(void*,void (*callback)(void* arg),u16) {
    auto& r=pc::radio();r.pending.clear();r.parent=nullptr;
    return queue(callback,reply(WM_ERRCODE_SUCCESS));
}
WMErrCode func_0207d1f0(void (*callback)(void* arg)) {pc::radio().parent=nullptr;return queue(callback,reply(WM_ERRCODE_SUCCESS));}
WMErrCode func_0207d228(void (*callback)(void* arg)) {pc::radio().parent=nullptr;return queue(callback,reply(WM_ERRCODE_SUCCESS));}
WMErrCode func_0207d268(void (*callback)(void* arg),WMParentParam* param) {
    if(param)pc::radio().beaconFrames=pc::frames(param->beaconPeriod?param->beaconPeriod:200);
    return queue(callback,reply(WM_ERRCODE_SUCCESS));
}
WMErrCode func_0207d3f0(void (*callback)(void* arg)) {
    auto& r=pc::radio();
    auto value=reply(WM_ERRCODE_SUCCESS);field<u16>(value,8,WM_STATECODE_PARENT_START);
    r.parent=callback;r.nextBeacon=r.frame+1+r.beaconFrames;
    return queue(callback,value);
}
WMErrCode func_0207d400(void (*callback)(void* arg)) {pc::radio().parent=nullptr;return queue(callback,reply(WM_ERRCODE_SUCCESS));}
WMErrCode func_0207d440(void (*callback)(void* arg),WMScanParam* param) {
    // The channel is listened to for its whole time; no parent is found (state 4).
    auto value=reply(WM_ERRCODE_SUCCESS);field<u16>(value,8,4);
    if(param)field<u16>(value,0x10,param->channel);
    return queue(callback,value,pc::frames(param&&param->maxChannelTime?param->maxChannelTime:30));
}
WMErrCode func_0207d52c(void (*callback)(void* arg)) {return queue(callback,reply(WM_ERRCODE_SUCCESS));}
WMErrCode func_0207d56c(void (*callback)(void* arg),WMBssDesc*,u8*,BOOL,u16) {
    // The parent is out of range: the connection attempt fails.
    return queue(callback,reply(WM_ERRCODE_FAILED),pc::frames(1000));
}
WMErrCode func_0207d638(void (*callback)(void* arg),u16) {return queue(callback,reply(WM_ERRCODE_SUCCESS));}
WMErrCode func_0207d880(void (*callback)(void* arg),u16*,u16,u16*,u16,u16) {
    auto value=reply(WM_ERRCODE_SUCCESS);field<u16>(value,4,WM_STATECODE_MP_START);
    return queue(callback,value);
}
WMErrCode func_0207da24(void (*callback)(void* arg)) {return queue(callback,reply(WM_ERRCODE_SUCCESS));}
WMErrCode func_0207da94(WMDataSharingInfo* info,u16,u16,u16,BOOL) {
    if(info)std::memset(info,0,sizeof *info);
    return WM_ERRCODE_SUCCESS;
}
WMErrCode func_0207dce8(WMDataSharingInfo*) {return WM_ERRCODE_SUCCESS;}
// No partner data ever arrives (7: no data set yet).
WMErrCode func_0207dd30(WMDataSharingInfo*,const u16*,WMDataSet*) {return 7;}
u16* func_0207e590(WMDataSharingInfo*,WMDataSet*,u16) {return nullptr;}
WMErrCode func_0207e614(WMKeySetBuf* buf,u16) {if(buf)std::memset(buf,0,sizeof *buf);return WM_ERRCODE_SUCCESS;}
WMErrCode func_0207e630(WMKeySetBuf*) {return WM_ERRCODE_SUCCESS;}
WMErrCode func_0207e63c(void (*callback)(void* arg),u16,const u16*) {return queue(callback,reply(WM_ERRCODE_SUCCESS));}
int func_0207e6b0(void (*callback)(void* arg),void*,u16,u32,u16,u8) {return queue(callback,reply(WM_ERRCODE_SUCCESS));}
WMErrCode func_0207e768(void (*callback)(void* arg),u16,u16,u16 channel,u16 measureTime) {
    // A quiet channel: no traffic measured.
    auto value=reply(WM_ERRCODE_SUCCESS);field<u16>(value,8,channel);field<u16>(value,10,0);
    return queue(callback,value,pc::frames(measureTime));
}
}
