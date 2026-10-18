#pragma ipa file
#include "ov002/mp/MPExchange.hpp"
#include "main/debug/DebugConsole.hpp"
#include "main/dss/DssCore.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Pad.hpp"
#include "main/dss/Random.hpp"
#include "main/text/TextAPI.hpp"
#include "nitro/os/os_owner.h"

static const u8 s_anyBssid[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

int MPExchange::unk_02127140;
int MPExchange::unk_02127144;
int MPExchange::connected_[16];
int MPExchange::scanning_;
int MPExchange::parentCount_;
alignas(32) u8 MPExchange::sendBuffer_[0x1e0];
alignas(32) u8 MPExchange::recvBuffer_[0x1e0];
WMBssDesc MPExchange::bssDesc_[4];

ARM void MPExchange::printChildren()
{
    char buf[0x20];
    char line[0x20];
    dss::sprintf_s(buf, sizeof(buf), "    0x%04x 0x%04x", SEND_PACKET->pad_, (u16)SEND_PACKET->frame_);
    debugConsolePrint(1, 0, buf);
    for (int i = 0; i < 16; i++) {
        if (connected_[i]) {
            MPExchangePacket* packet = RECV_PACKET(i);
            dss::sprintf_s(line, sizeof(line), "%2d  0x%04x 0x%04x  %02x %s", i, packet->pad_, (u16)packet->frame_,
                           ((u8*)&packet->data_)[0x9f], &packet->data_);
        } else {
            dss::sprintf_s(line, sizeof(line), "%2d  NO CHILD", i);
        }
        debugConsolePrint(1, i + 2, line);
    }
}

ARM void MPExchange::setSendData(EnvoyData* data)
{
    receiveCount_ = 0;
    dss::memset(sendData_, 0, 0xe4);
    dss::memset(receiveData_, 0, 0xe4);
    dss::memcpy(sendData_, data, 0xe4);
}

ARM EnvoyData* MPExchange::getReceivedData()
{
    if (receiveCount_ == 0) {
        return NULL;
    }
    return (EnvoyData*)receiveData_;
}

ARM void MPExchange::start()
{
    OSOwnerInfo info;
    unk_38 = g_debugConsolePrintEnabled;
    role_.mode_ = data_020f201c;
    forceError_ = data_020f2024;
    setWhForceError(forceError_);
    func_02079e40(&info);
    nickNameLength_ = info.nickNameLength;
    dss::memcpy(nickName_, info.nickName, info.nickNameLength);
    cancelRequest_ = 0;
    fatalErrorStep_ = 0;
    frame_ = 0;
    tgid_ = 0;
    channel_ = 0;
    dataSharingFailed_ = 0;
    parentCount_ = 0;
    unk_02127144 = 0;
    MI_CpuSet(bssDesc_, 0, sizeof(bssDesc_));
    MI_CpuSet(sendBuffer_, 0, sizeof(sendBuffer_));
    MI_CpuSet(recvBuffer_, 0, sizeof(recvBuffer_));
    MI_CpuSet(connected_, 0, sizeof(connected_));
    setMode(SYSMODE_STOP_toREADY00);
}

ARM void MPExchange::startRoleSelection()
{
    role_.buildTable();
    setMode(SYSMODE_SELECT_ROLE);
}

ARM void MPExchange::update()
{
    prevMode_ = mode_;
    switch (getWhSysState()) {
    case WH_SYSSTATE_FATAL:
        setMode(SYSMODE_FATAL_ERROR);
        break;
    case WH_SYSSTATE_ERROR:
        setMode(SYSMODE_LIGHT_ERRORINIT);
        break;
    case WH_SYSSTATE_MEASURECHANNEL: {
        MI_CpuSet(bssDesc_, 0, sizeof(bssDesc_));
        u16 channel = getWhBestChannel();
        tgid_++;
        connectWhParent(4, tgid_, channel, 0);
        break;
    }
    }
    unkfunc_021239d0();
    clearDebugConsole(getModeName(mode_));
    switch (mode_) {
    case SYSMODE_STOP:
        break;
    case SYSMODE_STOP_toREADY00:
        updateStopToReady0();
        break;
    case SYSMODE_STOP_toREADY01:
        updateStopToReady1();
        break;
    case SYSMODE_READY:
        break;
    case SYSMODE_SELECT_ROLE:
        updateSelectRole();
        break;
    case SYSMODE_SELECT_SCAN:
        updateSelectScan();
        break;
    case SYSMODE_SELECT_NEXT:
        updateSelectNext();
        break;
    case SYSMODE_PARENT_LOBBY:
        updateParentLobby();
        break;
    case SYSMODE_PARENT_EXEC:
        updateParentExec();
        break;
    case SYSMODE_CHILD_SCAN:
        updateChildScan();
        break;
    case SYSMODE_CHILD_CONNECT:
        updateChildConnect();
        break;
    case SYSMODE_CHILD_READY:
        updateChildReady();
        break;
    case SYSMODE_CHILD_EXEC:
        updateChildExec();
        break;
    case SYSMODE_FINISH_READY:
        updateFinishReady();
        break;
    case SYSMODE_FINISH_WAIT:
        break;
    case SYSMODE_LIGHT_ERRORINIT:
        updateLightErrorInit();
        break;
    case SYSMODE_LIGHT_ERRORWAIT:
        updateLightErrorWait();
        break;
    case SYSMODE_LIGHT_ERROR:
        break;
    case SYSMODE_FATAL_ERROR:
        updateFatalError();
        break;
    }
}

ARM void MPExchange::updateDataSharing()
{
    if (getWhSysState() != WH_SYSSTATE_STOP) {
        receivePackets();
        sendPacket();
        int state = getWhSysState();
        debugConsolePrintfIfEnabled(0, 23, "%02d %02d %s", state, unk_02127140, getWhSysStateName());
        if (++unk_02127140 == 100) {
            unk_02127140 = 0;
        }
    }
    if (prevMode_ == mode_) {
        counter_++;
    }
}

ARM void MPExchange::setMode(int mode)
{
    if (mode_ != mode) {
        mode_ = mode;
        counter_ = 0;
    }
}

ARM const char* MPExchange::getModeName(int mode)
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

ARM void MPExchange::unkfunc_021239d0()
{
}

ARM void MPExchange::updateGameInfo(int entry)
{
    unk_1a++;
    childCount_ = func_02066df0(getWhConnectBitmap()) - 1;
    func_0207e6b0(onSetGameInfoCallback, nickName_, 0x18, 0x8001d3, tgid_, entry ? 1 : 0);
}

ARM void MPExchange::onSetGameInfoCallback(void* arg)
{
}

ARM void MPExchange::onIndicationCallback(void* arg)
{
    if (((u16*)arg)[2] == 0x10) {
        unk_02127144 = 2;
    }
}

ARM void MPExchange::onScanCallback(WMBssDesc* desc)
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
        printWhBssDesc(desc);
    }
    scanning_ = UNK_SCAN_STATE_0;
}

ARM void MPExchange::startParent(u16 channel, int beaconPeriod)
{
    channel_ = channel;
    if (channel == 0) {
        startWhMeasureChannel();
    } else {
        tgid_++;
        updateGameInfo(1);
        MI_CpuSet(bssDesc_, 0, sizeof(bssDesc_));
        connectWhParent(4, tgid_, channel_, beaconPeriod);
    }
    setMode(SYSMODE_PARENT_LOBBY);
}

ARM void MPExchange::startScan(int channel)
{
    startWhScan(onScanCallback, s_anyBssid, channel);
    setMode(SYSMODE_CHILD_SCAN);
}

ARM void MPExchange::finish()
{
    finalizeWh();
    setMode(SYSMODE_FINISH_READY);
}

ARM void MPExchange::abort()
{
    resetWh();
    setMode(SYSMODE_FINISH_READY);
}

ARM void MPExchange::updateStopToReady0()
{
    setWhIndicateCallback(onIndicationCallback);
    if (initWh()) {
        setWhJudgeAcceptFunc(NULL);
        setWhGgid(0x8001d3);
        setMode(SYSMODE_STOP_toREADY01);
    } else {
        setMode(SYSMODE_FATAL_ERROR);
    }
}

ARM void MPExchange::updateStopToReady1()
{
    if (getWhSysState() == WH_SYSSTATE_IDLE) {
        setMode(SYSMODE_READY);
    }
}

ARM void MPExchange::updateLightErrorInit()
{
    finalizeWh();
    setMode(SYSMODE_LIGHT_ERRORWAIT);
}

ARM void MPExchange::updateLightErrorWait()
{
    if (getWhSysState() == WH_SYSSTATE_IDLE) {
        endWh();
        setMode(SYSMODE_LIGHT_ERROR);
    }
}

ARM void MPExchange::updateFatalError()
{
    switch (fatalErrorStep_) {
    case 0:
        resetWh();
        fatalErrorStep_++;
        break;
    case 1:
        if (getWhSysState() == WH_SYSSTATE_IDLE) {
            fatalErrorStep_++;
        }
        break;
    case 2:
        endWh();
        fatalErrorStep_++;
        break;
    }
}

ARM int MPExchangeRole::getNextRole()
{
    if (advance() == 0) {
        buildTable();
        advance();
    }
    return role_;
}

ARM void MPExchangeRole::buildTable()
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

ARM int MPExchangeRole::advance()
{
    restartTimer();
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

ARM void MPExchangeRole::printTable()
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
    debugConsolePrint(0, 4, line);
    debugConsolePrint(index_ + pos, 5, "^");
}

ARM void MPExchangeRole::restartTimer()
{
    start_ = func_02079bf0();
}

ARM int MPExchangeRole::isTimedOut()
{
    now_ = func_02079bf0();
    elapsed_ = now_ - start_;
    return OS_TicksToMilliSeconds(elapsed_) > limit_;
}
