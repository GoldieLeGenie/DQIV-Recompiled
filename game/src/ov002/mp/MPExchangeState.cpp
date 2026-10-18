#pragma ipa file
#include "ov002/mp/MPExchange.hpp"
#include "main/debug/DebugConsole.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Pad.hpp"
#include "main/text/TextAPI.hpp"

ARM void MPExchange::updateFinishReady()
{
    if (getWhSysState() == WH_SYSSTATE_IDLE) {
        endWh();
        setMode(SYSMODE_FINISH_WAIT);
    }
}

ARM void MPExchange::updateSelectRole()
{
    if (getWhSysState() != WH_SYSSTATE_IDLE) {
        return;
    }
    if (cancelRequest_ != 0) {
        setMode(SYSMODE_FINISH_READY);
        return;
    }
    int role = role_.getNextRole();
    if (role == 2) {
        isChild_ = 0;
        startParent(0, 90);
    }
    if (role == 1) {
        isChild_ = 1;
        scanChannel_ = 1;
        startScan(1);
    }
}

ARM void MPExchange::updateParentLobby()
{
    updateGameInfo(1);
    debugConsolePrintfIfEnabled(2, 1, "BEACON CNT %d", getWhBeaconCount());
    role_.printTable();
    if (getWhBeaconCount() > 1 && cancelRequest_ != 0) {
        finish();
        return;
    }
    if (getWhBeaconCount() > 60) {
        SEND_PACKET->state_ = 0;
        finalizeWh();
        setMode(SYSMODE_SELECT_NEXT);
        return;
    }
    int count = 0;
    u16 bitmap = getWhConnectBitmap();
    for (int i = 0; i < 16; i++) {
        if (bitmap & (1 << i)) {
            count++;
        }
    }
    if (count == 2) {
        SEND_PACKET->state_ = 0;
        for (int i = 0; i < 2; i++) {
            MPExchangePacket* packet = RECV_PACKET(i);
            packet->state_ = 0;
        }
        updateGameInfo(0);
        setMode(SYSMODE_PARENT_EXEC);
    }
    if (role_.isTimedOut()) {
        SEND_PACKET->state_ = 0;
        finalizeWh();
        setMode(SYSMODE_SELECT_NEXT);
    }
}

ARM void MPExchange::updateChildReady()
{
    if (counter_ > 60) {
        setMode(SYSMODE_LIGHT_ERRORINIT);
    }
    if (cancelRequest_ != 0) {
        finish();
        return;
    }
    if (RECV_PACKET(0)->state_ == 1) {
        SEND_PACKET->state_ = 0;
        for (int i = 0; i < 2; i++) {
            MPExchangePacket* packet = RECV_PACKET(i);
            packet->state_ = 0;
        }
        setMode(SYSMODE_CHILD_EXEC);
    }
}

ARM void MPExchange::updateChildScan()
{
    char buf[0x20];
    debugConsolePrintfIfEnabled(2, 1, "CHANNEL %d", scanChannel_);
    role_.printTable();
    if (cancelRequest_ != 0) {
        if (getWhSysState() == WH_SYSSTATE_SCANNING) {
            endWhScan();
        }
        finish();
        return;
    }
    if (parentCount_ > 0) {
        dss::sprintf_s(buf, sizeof(buf), "%2d %08x", 0, bssDesc_[0].gameInfo.ggid);
        debugConsolePrint(1, 2, buf);
        if (getWhSysState() == WH_SYSSTATE_SCANNING) {
            endWhScan();
        }
        setMode(SYSMODE_CHILD_CONNECT);
        return;
    }
    if (role_.isTimedOut()) {
        if (getWhSysState() == WH_SYSSTATE_SCANNING) {
            endWhScan();
        }
        setMode(SYSMODE_SELECT_SCAN);
    }
}

ARM void MPExchange::updateSelectScan()
{
    debugConsolePrintfIfEnabled(2, 1, "CHANNEL %d", scanChannel_);
    role_.printTable();
    if (getWhSysState() != WH_SYSSTATE_IDLE) {
        return;
    }
    if (cancelRequest_ != 0) {
        finish();
        return;
    }
    role_.restartTimer();
    switch (scanChannel_) {
    case 1:
        scanChannel_ = 7;
        startScan(7);
        break;
    case 7:
        scanChannel_ = 13;
        startScan(13);
        break;
    case 13:
        finalizeWh();
        setMode(SYSMODE_SELECT_NEXT);
        break;
    }
}

ARM void MPExchange::updateChildConnect()
{
    if (getWhSysState() == WH_SYSSTATE_IDLE) {
        connectWhChild(5, &bssDesc_[0]);
        setMode(SYSMODE_CHILD_READY);
    }
}

ARM void MPExchange::updateParentExec()
{
    printChildren();
    dss::memcpy(&SEND_PACKET->data_, sendData_, 0xe4);
    if (getWhConnectBitmap() == 1) {
        finish();
        return;
    }
    if (forceError_ == 4) {
        finish();
        return;
    }
    if (forceError_ == 6) {
        finish();
        return;
    }
    if (cancelRequest_ != 0) {
        finish();
        return;
    }
    if (RECV_PACKET(0)->state_ == 2) {
        finish();
        return;
    }
    for (int i = 0; i < 2; i++) {
        if (RECV_PACKET(i)->state_ == 2) {
            SEND_PACKET->state_ = 2;
            return;
        }
    }
    if (receiveCount_ == 4) {
        SEND_PACKET->state_ = 2;
        return;
    }
}

ARM void MPExchange::updateChildExec()
{
    printChildren();
    dss::memcpy(&SEND_PACKET->data_, sendData_, 0xe4);
    if (forceError_ == 5) {
        finish();
        return;
    }
    if (forceError_ == 6) {
        if (receiveCount_ == 1) {
            finish();
        }
        return;
    }
    if (cancelRequest_ != 0) {
        finish();
        return;
    }
    if (RECV_PACKET(0)->state_ == 2) {
        finish();
    }
}

ARM void MPExchange::updateSelectNext()
{
    role_.printTable();
    if (getWhSysState() == WH_SYSSTATE_IDLE) {
        setMode(SYSMODE_SELECT_ROLE);
    }
}

ARM void MPExchange::receivePackets()
{
    if (getWhSysState() == WH_SYSSTATE_DATASHARING) {
        if (stepWhDataSharing(sendBuffer_)) {
            int i;
            for (i = 0; i < 2; i++) {
                void* data = getWhSharedData(i);
                MPExchangePacket* packet = RECV_PACKET(i);
                if (data != NULL) {
                    MI_CpuCopyU8(data, packet, 0xe4);
                    connected_[i] = 1;
                    if (isChild_ == 0) {
                        if (i == 1) {
                            dss::memcpy(receiveData_, &packet->data_, 0xe4);
                            receiveCount_++;
                            convertUtf8ToSjisWork((char*)((EnvoyData*)receiveData_)->name);
                            convertUtf8ToSjisWork((char*)((EnvoyData*)receiveData_)->comment);
                        }
                    } else if (i == 0) {
                        dss::memcpy(receiveData_, &packet->data_, 0xe4);
                        receiveCount_++;
                        convertUtf8ToSjisWork((char*)((EnvoyData*)receiveData_)->name);
                        convertUtf8ToSjisWork((char*)((EnvoyData*)receiveData_)->comment);
                    }
                } else {
                    packet->linkLevel_ = 0;
                    connected_[i] = 0;
                }
            }
            for (; i < 16; i++) {
                connected_[i] = 0;
            }
            dataSharingFailed_ = 0;
        } else {
            for (int i = 0; i < 16; i++) {
                connected_[i] = 0;
            }
            dataSharingFailed_ = 1;
        }
    } else {
        for (int i = 0; i < 16; i++) {
            connected_[i] = 0;
        }
        dataSharingFailed_ = 0;
    }
}

ARM void MPExchange::sendPacket()
{
    if (dataSharingFailed_ != 0) {
        return;
    }
    MPExchangePacket* packet = SEND_PACKET;
    if (packet->state_ != 2) {
        packet->state_ = mode_ == SYSMODE_PARENT_LOBBY ? 0 : 1;
    }
    packet->linkLevel_ = (u16)func_0207cfe0();
    packet->pad_ = dss::g_Pad.pad();
    packet->frame_ = (u16)frame_;
    if (unk_02127144 != 0) {
        if (unk_02127144 > 0) {
            unk_02127144 = -unk_02127144 + 1;
        } else {
            unk_02127144 = -unk_02127144 - 1;
        }
    }
    frame_++;
}
