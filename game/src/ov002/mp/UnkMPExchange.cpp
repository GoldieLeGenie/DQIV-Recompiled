#pragma ipa file
#include "ov002/mp/UnkMPExchange.hpp"
#include "main/debug/UnkDebugPrint.hpp"
#include "main/dss/DssCore.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Pad.hpp"
#include "main/dss/Random.hpp"
#include "main/text/TextAPI.hpp"
#include "nitro/os/os_owner.h"

static const u8 s_anyBssid[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

int UnkMPExchange::unk_02127140;
int UnkMPExchange::unk_02127144;
int UnkMPExchange::connected_[16];
int UnkMPExchange::scanning_;
int UnkMPExchange::parentCount_;
alignas(32) u8 UnkMPExchange::sendBuffer_[0x1e0];
alignas(32) u8 UnkMPExchange::recvBuffer_[0x1e0];
WMBssDesc UnkMPExchange::bssDesc_[4];

ARM void UnkMPExchange::unkfunc_02123230()
{
    char buf[0x20];
    char line[0x20];
    dss::sprintf_s(buf, sizeof(buf), "    0x%04x 0x%04x", SEND_PACKET->pad_, (u16)SEND_PACKET->frame_);
    unkfunc_0203d6e4(1, 0, buf);
    for (int i = 0; i < 16; i++) {
        if (connected_[i]) {
            UnkMPExchangePacket* packet = RECV_PACKET(i);
            dss::sprintf_s(line, sizeof(line), "%2d  0x%04x 0x%04x  %02x %s", i, packet->pad_, (u16)packet->frame_,
                           ((u8*)&packet->data_)[0x9f], &packet->data_);
        } else {
            dss::sprintf_s(line, sizeof(line), "%2d  NO CHILD", i);
        }
        unkfunc_0203d6e4(1, i + 2, line);
    }
}

ARM void UnkMPExchange::unkfunc_02123334(UnkEnvoyData* data)
{
    unk_fc = 0;
    dss::memset(sendData_, 0, 0xe4);
    dss::memset(receiveData_, 0, 0xe4);
    dss::memcpy(sendData_, data, 0xe4);
}

ARM UnkEnvoyData* UnkMPExchange::unkfunc_02123378()
{
    if (unk_fc == 0) {
        return NULL;
    }
    return (UnkEnvoyData*)receiveData_;
}

ARM void UnkMPExchange::unkfunc_0212338c()
{
    OSOwnerInfo info;
    unk_38 = data_020f2020;
    role_.mode_ = data_020f201c;
    unk_3c = data_020f2024;
    unkfunc_02122a00(unk_3c);
    func_02079e40(&info);
    nickNameLength_ = info.nickNameLength;
    dss::memcpy(nickName_, info.nickName, info.nickNameLength);
    unk_1c = 0;
    unk_f4 = 0;
    frame_ = 0;
    tgid_ = 0;
    channel_ = 0;
    unk_34 = 0;
    parentCount_ = 0;
    unk_02127144 = 0;
    MI_CpuSet(bssDesc_, 0, sizeof(bssDesc_));
    MI_CpuSet(sendBuffer_, 0, sizeof(sendBuffer_));
    MI_CpuSet(recvBuffer_, 0, sizeof(recvBuffer_));
    MI_CpuSet(connected_, 0, sizeof(connected_));
    unkfunc_021237d0(SYSMODE_STOP_toREADY00);
}

ARM void UnkMPExchange::unkfunc_02123478()
{
    role_.unkfunc_02123ed8();
    unkfunc_021237d0(SYSMODE_SELECT_ROLE);
}

ARM void UnkMPExchange::unkfunc_02123498()
{
    prevMode_ = mode_;
    switch (unkfunc_02121f50()) {
    case WH_SYSSTATE_FATAL:
        unkfunc_021237d0(SYSMODE_FATAL_ERROR);
        break;
    case WH_SYSSTATE_ERROR:
        unkfunc_021237d0(SYSMODE_LIGHT_ERRORINIT);
        break;
    case WH_SYSSTATE_MEASURECHANNEL: {
        MI_CpuSet(bssDesc_, 0, sizeof(bssDesc_));
        u16 channel = unkfunc_02122420();
        tgid_++;
        unkfunc_02122684(4, tgid_, channel, 0);
        break;
    }
    }
    unkfunc_021239d0();
    unkfunc_0203d688(unkfunc_021237e8(mode_));
    switch (mode_) {
    case SYSMODE_STOP:
        break;
    case SYSMODE_STOP_toREADY00:
        unkfunc_02123d84();
        break;
    case SYSMODE_STOP_toREADY01:
        unkfunc_02123dd8();
        break;
    case SYSMODE_READY:
        break;
    case SYSMODE_SELECT_ROLE:
        unkfunc_02122a38();
        break;
    case SYSMODE_SELECT_SCAN:
        unkfunc_02122d6c();
        break;
    case SYSMODE_SELECT_NEXT:
        unkfunc_02122fd8();
        break;
    case SYSMODE_PARENT_LOBBY:
        unkfunc_02122ab0();
        break;
    case SYSMODE_PARENT_EXEC:
        unkfunc_02122e50();
        break;
    case SYSMODE_CHILD_SCAN:
        unkfunc_02122c6c();
        break;
    case SYSMODE_CHILD_CONNECT:
        unkfunc_02122e1c();
        break;
    case SYSMODE_CHILD_READY:
        unkfunc_02122be4();
        break;
    case SYSMODE_CHILD_EXEC:
        unkfunc_02122f44();
        break;
    case SYSMODE_FINISH_READY:
        unkfunc_02122a10();
        break;
    case SYSMODE_FINISH_WAIT:
        break;
    case SYSMODE_LIGHT_ERRORINIT:
        unkfunc_02123dfc();
        break;
    case SYSMODE_LIGHT_ERRORWAIT:
        unkfunc_02123e18();
        break;
    case SYSMODE_LIGHT_ERROR:
        break;
    case SYSMODE_FATAL_ERROR:
        unkfunc_02123e40();
        break;
    }
}

ARM void UnkMPExchange::unkfunc_02123724()
{
    if (unkfunc_02121f50() != WH_SYSSTATE_STOP) {
        unkfunc_02123004();
        unkfunc_02123188();
        int state = unkfunc_02121f50();
        unkfunc_0203d720(0, 23, "%02d %02d %s", state, unk_02127140, unkfunc_02121f60());
        if (++unk_02127140 == 100) {
            unk_02127140 = 0;
        }
    }
    if (prevMode_ == mode_) {
        counter_++;
    }
}

ARM void UnkMPExchange::unkfunc_021237d0(int mode)
{
    if (mode_ != mode) {
        mode_ = mode;
        counter_ = 0;
    }
}

ARM const char* UnkMPExchange::unkfunc_021237e8(int mode)
{
    switch (mode) {
    case SYSMODE_STOP:
        return "SYSMODE_STOP";
    case SYSMODE_STOP_toREADY00:
        return "SYSMODE_STOP_toREADY00";
    case SYSMODE_STOP_toREADY01:
        return "SYSMODE_STOP_toREADY01";
    case SYSMODE_READY:
        return "SYSMODE_READY";
    case SYSMODE_SELECT_ROLE:
        return "SYSMODE_SELECT_ROLE";
    case SYSMODE_SELECT_SCAN:
        return "SYSMODE_SELECT_SCAN";
    case SYSMODE_SELECT_NEXT:
        return "SYSMODE_SELECT_NEXT";
    case SYSMODE_PARENT_LOBBY:
        return "SYSMODE_PARENT_LOBBY";
    case SYSMODE_PARENT_EXEC:
        return "SYSMODE_PARENT_EXEC";
    case SYSMODE_CHILD_SCAN:
        return "SYSMODE_CHILD_SCAN";
    case SYSMODE_CHILD_CONNECT:
        return "SYSMODE_CHILD_CONNECT";
    case SYSMODE_CHILD_READY:
        return "SYSMODE_CHILD_READY";
    case SYSMODE_CHILD_EXEC:
        return "SYSMODE_CHILD_EXEC";
    case SYSMODE_FINISH_READY:
        return "SYSMODE_FINISH_READY";
    case SYSMODE_FINISH_WAIT:
        return "SYSMODE_FINISH_WAIT";
    case SYSMODE_LIGHT_ERRORINIT:
        return "SYSMODE_LIGHT_ERRORINIT";
    case SYSMODE_LIGHT_ERRORWAIT:
        return "SYSMODE_LIGHT_ERRORWAIT";
    case SYSMODE_LIGHT_ERROR:
        return "SYSMODE_LIGHT_ERROR";
    case SYSMODE_FATAL_ERROR:
        return "SYSMODE_FATAL_ERROR";
    }
    return "UNKNOW STATE";
}

ARM void UnkMPExchange::unkfunc_021239d0()
{
}

ARM void UnkMPExchange::unkfunc_021239d4(int entry)
{
    unk_1a++;
    childCount_ = func_02066df0(unkfunc_02121f40()) - 1;
    func_0207e6b0(unkfunc_02123a40, nickName_, 0x18, 0x8001d3, tgid_, entry ? 1 : 0);
}

ARM void UnkMPExchange::unkfunc_02123a40(void* arg)
{
}

ARM void UnkMPExchange::unkfunc_02123a44(void* arg)
{
    if (((u16*)arg)[2] == 0x10) {
        unk_02127144 = 2;
    }
}

ARM void UnkMPExchange::unkfunc_02123a60(WMBssDesc* desc)
{
    char line[0x20];
    char name[0x15];
    int count;
    UnkScanState state = UNK_SCAN_STATE_1;
    u16 length = desc->gameInfoLength;
    scanning_ = state;
    if (length != 0 && desc->gameInfo.userGameInfoLength == 0x18) {
        MI_CpuCopyU8(&((u8*)&desc->gameInfo)[0x10], name, ((u8*)&desc->gameInfo)[0x24] * 2);
        count = ((u8*)&desc->gameInfo)[0x25];
    } else {
        STD_CopyLString(name, "unknown", sizeof(name));
        count = 0;
    }
    int i;
    for (i = 0; i < parentCount_; i++) {
        WMBssDesc* bss = &bssDesc_[i];
        if (bss->bssid[0] == desc->bssid[0] && bss->bssid[1] == desc->bssid[1] && bss->bssid[2] == desc->bssid[2] && bss->bssid[3] == desc->bssid[3] && bss->bssid[4] == desc->bssid[4] && bss->bssid[5] == desc->bssid[5]) {
            break;
        }
    }
    dss::sprintf_s(line, sizeof(line), "[%d]channel%d %s %d/%d", i + 1, desc->channel, name, count, 1);
    if (i >= parentCount_ && parentCount_ < 4) {
        bssDesc_[parentCount_] = *desc;
        parentCount_++;
        unkfunc_02122030(desc);
    }
    scanning_ = UNK_SCAN_STATE_0;
}

ARM void UnkMPExchange::unkfunc_02123ca8(u16 channel, int beaconPeriod)
{
    channel_ = channel;
    if (channel == 0) {
        unkfunc_021221fc();
    } else {
        tgid_++;
        unkfunc_021239d4(1);
        MI_CpuSet(bssDesc_, 0, sizeof(bssDesc_));
        unkfunc_02122684(4, tgid_, channel_, beaconPeriod);
    }
    unkfunc_021237d0(SYSMODE_PARENT_LOBBY);
}

ARM void UnkMPExchange::unkfunc_02123d18(int channel)
{
    unkfunc_02121664(unkfunc_02123a60, s_anyBssid, channel);
    unkfunc_021237d0(SYSMODE_CHILD_SCAN);
}

ARM void UnkMPExchange::unkfunc_02123d4c()
{
    unkfunc_02122888();
    unkfunc_021237d0(SYSMODE_FINISH_READY);
}

ARM void UnkMPExchange::unkfunc_02123d68()
{
    unkfunc_02122840();
    unkfunc_021237d0(SYSMODE_FINISH_READY);
}

ARM void UnkMPExchange::unkfunc_02123d84()
{
    unkfunc_021229e0(unkfunc_02123a44);
    if (unkfunc_02122528()) {
        unkfunc_02121f20(NULL);
        unkfunc_02121f10(0x8001d3);
        unkfunc_021237d0(SYSMODE_STOP_toREADY01);
    } else {
        unkfunc_021237d0(SYSMODE_FATAL_ERROR);
    }
}

ARM void UnkMPExchange::unkfunc_02123dd8()
{
    if (unkfunc_02121f50() == WH_SYSSTATE_IDLE) {
        unkfunc_021237d0(SYSMODE_READY);
    }
}

ARM void UnkMPExchange::unkfunc_02123dfc()
{
    unkfunc_02122888();
    unkfunc_021237d0(SYSMODE_LIGHT_ERRORWAIT);
}

ARM void UnkMPExchange::unkfunc_02123e18()
{
    if (unkfunc_02121f50() == WH_SYSSTATE_IDLE) {
        unkfunc_02122998();
        unkfunc_021237d0(SYSMODE_LIGHT_ERROR);
    }
}

ARM void UnkMPExchange::unkfunc_02123e40()
{
    switch (unk_f4) {
    case 0:
        unkfunc_02122840();
        unk_f4++;
        break;
    case 1:
        if (unkfunc_02121f50() == WH_SYSSTATE_IDLE) {
            unk_f4++;
        }
        break;
    case 2:
        unkfunc_02122998();
        unk_f4++;
        break;
    }
}

ARM int UnkMPExchangeRole::unkfunc_02123eac()
{
    if (unkfunc_02123fb0() == 0) {
        unkfunc_02123ed8();
        unkfunc_02123fb0();
    }
    return role_;
}

ARM void UnkMPExchangeRole::unkfunc_02123ed8()
{
    role_ = 0;
    index_ = 0;
    next_ = 0;
    if (mode_ == 0) {
        for (int i = 0; i < 32; i += 4) {
            table_[i] = table_[i + 1] = table_[i + 2] = table_[i + 3] = 2;
            table_[i + dssrand::rand(4)] = 1;
        }
    }
    if (mode_ == 1) {
        for (int i = 0; i < 32; i += 4) {
            table_[i] = table_[i + 1] = table_[i + 2] = table_[i + 3] = 2;
        }
    }
    if (mode_ == 2) {
        for (int i = 0; i < 32; i += 4) {
            table_[i] = table_[i + 1] = table_[i + 2] = table_[i + 3] = 1;
        }
    }
    table_[32] = 0;
}

ARM int UnkMPExchangeRole::unkfunc_02123fb0()
{
    unkfunc_02124168();
    index_ = next_;
    int role = table_[next_];
    if (role == 0) {
        role_ = 0;
        limit_ = 0;
        return 0;
    }
    if (role == 1) {
        role_ = 1;
        limit_ = 150;
        next_++;
        return 1;
    }
    if (role == 2) {
        role_ = 2;
        limit_ = 900;
        next_++;
        while (table_[next_] == 2) {
            limit_ += 900;
            next_++;
        }
        return 1;
    }
    return 0;
}

ARM void UnkMPExchangeRole::unkfunc_0212407c()
{
    char line[0x21];
    for (int i = 0; i < 0x21; i++) {
        switch (table_[i]) {
        case 1:
            line[i] = 'C';
            break;
        case 2:
            line[i] = 'P';
            break;
        default:
            line[i] = '-';
            break;
        }
    }
    line[0x20] = 0;
    u32 ms = OS_TicksToMilliSeconds(func_02079bf0() - start_);
    int pos = ms / 900;
    if (table_[index_] == 1) {
        pos = 0;
    }
    unkfunc_0203d6e4(0, 4, line);
    unkfunc_0203d6e4(index_ + pos, 5, "^");
}

ARM void UnkMPExchangeRole::unkfunc_02124168()
{
    start_ = func_02079bf0();
}

ARM int UnkMPExchangeRole::unkfunc_02124180()
{
    now_ = func_02079bf0();
    elapsed_ = now_ - start_;
    return OS_TicksToMilliSeconds(elapsed_) > limit_;
}
