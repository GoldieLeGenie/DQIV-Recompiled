#pragma ipa file
#include "ov002/mp/UnkMPExchange.hpp"
#include "main/debug/UnkDebugPrint.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Pad.hpp"
#include "main/text/TextAPI.hpp"

ARM void UnkMPExchange::unkfunc_02122a10()
{
    if (unkfunc_02121f50() == WH_SYSSTATE_IDLE) {
        unkfunc_02122998();
        unkfunc_021237d0(SYSMODE_FINISH_WAIT);
    }
}

ARM void UnkMPExchange::unkfunc_02122a38()
{
    if (unkfunc_02121f50() != WH_SYSSTATE_IDLE) {
        return;
    }
    if (unk_1c != 0) {
        unkfunc_021237d0(SYSMODE_FINISH_READY);
        return;
    }
    int role = role_.unkfunc_02123eac();
    if (role == 2) {
        unk_f8 = 0;
        unkfunc_02123ca8(0, 90);
    }
    if (role == 1) {
        unk_f8 = 1;
        unk_40 = 1;
        unkfunc_02123d18(1);
    }
}

ARM void UnkMPExchange::unkfunc_02122ab0()
{
    unkfunc_021239d4(1);
    unkfunc_0203d720(2, 1, "BEACON CNT %d", unkfunc_021229f0());
    role_.unkfunc_0212407c();
    if (unkfunc_021229f0() > 1 && unk_1c != 0) {
        unkfunc_02123d4c();
        return;
    }
    if (unkfunc_021229f0() > 60) {
        SEND_PACKET->state_ = 0;
        unkfunc_02122888();
        unkfunc_021237d0(SYSMODE_SELECT_NEXT);
        return;
    }
    int count = 0;
    u16 bitmap = unkfunc_02121f40();
    for (int i = 0; i < 16; i++) {
        if (bitmap & (1 << i)) {
            count++;
        }
    }
    if (count == 2) {
        SEND_PACKET->state_ = 0;
        for (int i = 0; i < 2; i++) {
            UnkMPExchangePacket* packet = RECV_PACKET(i);
            packet->state_ = 0;
        }
        unkfunc_021239d4(0);
        unkfunc_021237d0(SYSMODE_PARENT_EXEC);
    }
    if (role_.unkfunc_02124180()) {
        SEND_PACKET->state_ = 0;
        unkfunc_02122888();
        unkfunc_021237d0(SYSMODE_SELECT_NEXT);
    }
}

ARM void UnkMPExchange::unkfunc_02122be4()
{
    if (counter_ > 60) {
        unkfunc_021237d0(SYSMODE_LIGHT_ERRORINIT);
    }
    if (unk_1c != 0) {
        unkfunc_02123d4c();
        return;
    }
    if (RECV_PACKET(0)->state_ == 1) {
        SEND_PACKET->state_ = 0;
        for (int i = 0; i < 2; i++) {
            UnkMPExchangePacket* packet = RECV_PACKET(i);
            packet->state_ = 0;
        }
        unkfunc_021237d0(SYSMODE_CHILD_EXEC);
    }
}

ARM void UnkMPExchange::unkfunc_02122c6c()
{
    char buf[0x20];
    unkfunc_0203d720(2, 1, "CHANNEL %d", unk_40);
    role_.unkfunc_0212407c();
    if (unk_1c != 0) {
        if (unkfunc_02121f50() == WH_SYSSTATE_SCANNING) {
            unkfunc_021218f4();
        }
        unkfunc_02123d4c();
        return;
    }
    if (parentCount_ > 0) {
        dss::sprintf_s(buf, sizeof(buf), "%2d %08x", 0, bssDesc_[0].gameInfo.ggid);
        unkfunc_0203d6e4(1, 2, buf);
        if (unkfunc_02121f50() == WH_SYSSTATE_SCANNING) {
            unkfunc_021218f4();
        }
        unkfunc_021237d0(SYSMODE_CHILD_CONNECT);
        return;
    }
    if (role_.unkfunc_02124180()) {
        if (unkfunc_02121f50() == WH_SYSSTATE_SCANNING) {
            unkfunc_021218f4();
        }
        unkfunc_021237d0(SYSMODE_SELECT_SCAN);
    }
}

ARM void UnkMPExchange::unkfunc_02122d6c()
{
    unkfunc_0203d720(2, 1, "CHANNEL %d", unk_40);
    role_.unkfunc_0212407c();
    if (unkfunc_02121f50() != WH_SYSSTATE_IDLE) {
        return;
    }
    if (unk_1c != 0) {
        unkfunc_02123d4c();
        return;
    }
    role_.unkfunc_02124168();
    switch (unk_40) {
    case 1:
        unk_40 = 7;
        unkfunc_02123d18(7);
        break;
    case 7:
        unk_40 = 13;
        unkfunc_02123d18(13);
        break;
    case 13:
        unkfunc_02122888();
        unkfunc_021237d0(SYSMODE_SELECT_NEXT);
        break;
    }
}

ARM void UnkMPExchange::unkfunc_02122e1c()
{
    if (unkfunc_02121f50() == WH_SYSSTATE_IDLE) {
        unkfunc_02122740(5, &bssDesc_[0]);
        unkfunc_021237d0(SYSMODE_CHILD_READY);
    }
}

ARM void UnkMPExchange::unkfunc_02122e50()
{
    unkfunc_02123230();
    dss::memcpy(&SEND_PACKET->data_, sendData_, 0xe4);
    if (unkfunc_02121f40() == 1) {
        unkfunc_02123d4c();
        return;
    }
    if (unk_3c == 4) {
        unkfunc_02123d4c();
        return;
    }
    if (unk_3c == 6) {
        unkfunc_02123d4c();
        return;
    }
    if (unk_1c != 0) {
        unkfunc_02123d4c();
        return;
    }
    if (RECV_PACKET(0)->state_ == 2) {
        unkfunc_02123d4c();
        return;
    }
    for (int i = 0; i < 2; i++) {
        if (RECV_PACKET(i)->state_ == 2) {
            SEND_PACKET->state_ = 2;
            return;
        }
    }
    if (unk_fc == 4) {
        SEND_PACKET->state_ = 2;
        return;
    }
}

ARM void UnkMPExchange::unkfunc_02122f44()
{
    unkfunc_02123230();
    dss::memcpy(&SEND_PACKET->data_, sendData_, 0xe4);
    if (unk_3c == 5) {
        unkfunc_02123d4c();
        return;
    }
    if (unk_3c == 6) {
        if (unk_fc == 1) {
            unkfunc_02123d4c();
        }
        return;
    }
    if (unk_1c != 0) {
        unkfunc_02123d4c();
        return;
    }
    if (RECV_PACKET(0)->state_ == 2) {
        unkfunc_02123d4c();
    }
}

ARM void UnkMPExchange::unkfunc_02122fd8()
{
    role_.unkfunc_0212407c();
    if (unkfunc_02121f50() == WH_SYSSTATE_IDLE) {
        unkfunc_021237d0(SYSMODE_SELECT_ROLE);
    }
}

ARM void UnkMPExchange::unkfunc_02123004()
{
    if (unkfunc_02121f50() == WH_SYSSTATE_DATASHARING) {
        if (unkfunc_021227ec(sendBuffer_)) {
            int i;
            for (i = 0; i < 2; i++) {
                void* data = unkfunc_021227cc(i);
                UnkMPExchangePacket* packet = RECV_PACKET(i);
                if (data != NULL) {
                    MI_CpuCopyU8(data, packet, 0xe4);
                    connected_[i] = 1;
                    if (unk_f8 == 0) {
                        if (i == 1) {
                            dss::memcpy(receiveData_, &packet->data_, 0xe4);
                            unk_fc++;
                            unkfunc_02088078((char*)((UnkEnvoyData*)receiveData_)->name);
                            unkfunc_02088078((char*)((UnkEnvoyData*)receiveData_)->comment);
                        }
                    } else if (i == 0) {
                        dss::memcpy(receiveData_, &packet->data_, 0xe4);
                        unk_fc++;
                        unkfunc_02088078((char*)((UnkEnvoyData*)receiveData_)->name);
                        unkfunc_02088078((char*)((UnkEnvoyData*)receiveData_)->comment);
                    }
                } else {
                    packet->linkLevel_ = 0;
                    connected_[i] = 0;
                }
            }
            for (; i < 16; i++) {
                connected_[i] = 0;
            }
            unk_34 = 0;
        } else {
            for (int i = 0; i < 16; i++) {
                connected_[i] = 0;
            }
            unk_34 = 1;
        }
    } else {
        for (int i = 0; i < 16; i++) {
            connected_[i] = 0;
        }
        unk_34 = 0;
    }
}

ARM void UnkMPExchange::unkfunc_02123188()
{
    if (unk_34 != 0) {
        return;
    }
    UnkMPExchangePacket* packet = SEND_PACKET;
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
