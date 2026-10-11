#pragma once
#include "globaldefs.h"
#include "main/cmn/UnkEnvoyManager.hpp"
#include "ov002/mp/UnkWH.hpp"

// SYSMODE_* names printed by UnkMPExchange::unkfunc_021237e8
enum {
    SYSMODE_STOP = 0,
    SYSMODE_STOP_toREADY00 = 10,
    SYSMODE_STOP_toREADY01 = 11,
    SYSMODE_READY = 12,
    SYSMODE_SELECT_ROLE = 21,
    SYSMODE_SELECT_SCAN = 22,
    SYSMODE_SELECT_NEXT = 23,
    SYSMODE_PARENT_LOBBY = 30,
    SYSMODE_PARENT_EXEC = 31,
    SYSMODE_CHILD_SCAN = 40,
    SYSMODE_CHILD_CONNECT = 41,
    SYSMODE_CHILD_READY = 42,
    SYSMODE_CHILD_EXEC = 43,
    SYSMODE_FINISH_READY = 50,
    SYSMODE_FINISH_WAIT = 51,
    SYSMODE_LIGHT_ERRORINIT = 60,
    SYSMODE_LIGHT_ERRORWAIT = 61,
    SYSMODE_LIGHT_ERROR = 62,
    SYSMODE_FATAL_ERROR = 72
};

// Value written to UnkMPExchange::scanning_ by the scan callback
enum UnkScanState {
    UNK_SCAN_STATE_0 = 0,
    UNK_SCAN_STATE_1 = 1
};

// Parent/child role sequence: 'C' (1) = scan as child, 'P' (2) = wait as parent
struct UnkMPExchangeRole {
    int mode_;                                  // 0x00 0: random, 1: always parent, 2: always child
    int index_;                                 // 0x04 current entry
    int next_;                                  // 0x08 next entry
    int table_[33];                             // 0x0C
    int role_;                                  // 0x90
    u64 start_;                                 // 0x94
    u64 now_;                                   // 0x9C
    u64 elapsed_;                               // 0xA4
    int limit_;                                 // 0xAC milliseconds

    int unkfunc_02123eac();
    void unkfunc_02123ed8();                    // build the table
    int unkfunc_02123fb0();                     // next entry
    void unkfunc_0212407c();                    // print the table
    void unkfunc_02124168();                    // restart the timer
    int unkfunc_02124180();                     // time out
};

// Packet shared with the other DS
struct UnkMPExchangePacket {
    int state_;                                 // 0x00 0: lobby, 1: ready, 2: end
    int linkLevel_;                             // 0x04
    int pad_;                                   // 0x08
    int frame_;                                 // 0x0C
    UnkEnvoyData data_;                         // 0x10
};


// ov002: wireless exchange of the envoys, member of MPExchangePart
struct UnkMPExchange {
    u16 nickName_[10];                          // 0x04 user game info
    u8 nickNameLength_;                         // 0x18
    u8 childCount_;                             // 0x19
    u16 unk_1a;                                 // 0x1A
    int unk_1c;                                 // 0x1C cancel
    int mode_;                                  // 0x20 SYSMODE_*
    int counter_;                               // 0x24 frames in the mode
    int prevMode_;                              // 0x28
    int frame_;                                 // 0x2C
    u16 tgid_;                                  // 0x30
    u16 channel_;                               // 0x32
    int unk_34;                                 // 0x34
    int unk_38;                                 // 0x38
    int unk_3c;                                 // 0x3C
    int unk_40;                                 // 0x40 scanned channel
    UnkMPExchangeRole role_;                    // 0x44
    int unk_f4;                                 // 0xF4
    int unk_f8;                                 // 0xF8 1: child
    int unk_fc;                                 // 0xFC received count
    unsigned char sendData_[0xe4];              // 0x100
    unsigned char receiveData_[0xe4];           // 0x1E4

    virtual void unkfunc_02122a38();            // SYSMODE_SELECT_ROLE
    virtual void unkfunc_02122d6c();            // SYSMODE_SELECT_SCAN
    virtual void unkfunc_02122fd8();            // SYSMODE_SELECT_NEXT
    virtual void unkfunc_02122ab0();            // SYSMODE_PARENT_LOBBY
    virtual void unkfunc_02122e50();            // SYSMODE_PARENT_EXEC
    virtual void unkfunc_02122c6c();            // SYSMODE_CHILD_SCAN
    virtual void unkfunc_02122e1c();            // SYSMODE_CHILD_CONNECT
    virtual void unkfunc_02122be4();            // SYSMODE_CHILD_READY
    virtual void unkfunc_02122f44();            // SYSMODE_CHILD_EXEC
    virtual void unkfunc_02122a10();            // SYSMODE_FINISH_READY
    virtual void unkfunc_02123004();            // receive
    virtual void unkfunc_02123188();            // send

    void unkfunc_02123230();                    // print the children
    void unkfunc_02123334(UnkEnvoyData* data);  // data to send
    UnkEnvoyData* unkfunc_02123378();           // received data
    void unkfunc_0212338c();                    // start
    void unkfunc_02123478();
    void unkfunc_02123498();                    // update
    void unkfunc_02123724();                    // debug print
    void unkfunc_021237d0(int mode);            // set the mode
    const char* unkfunc_021237e8(int mode);     // mode name
    void unkfunc_021239d0();
    void unkfunc_021239d4(int entry);           // set the game info
    static void unkfunc_02123a40(void* arg);    // game info callback
    static void unkfunc_02123a44(void* arg);    // indication callback
    static void unkfunc_02123a60(WMBssDesc* desc);  // scan callback
    void unkfunc_02123ca8(u16 channel, int beaconPeriod);   // start as parent
    void unkfunc_02123d18(int channel);         // start the scan
    void unkfunc_02123d4c();
    void unkfunc_02123d68();
    void unkfunc_02123d84();                    // SYSMODE_STOP_toREADY00
    void unkfunc_02123dd8();                    // SYSMODE_STOP_toREADY01
    void unkfunc_02123dfc();                    // SYSMODE_LIGHT_ERRORINIT
    void unkfunc_02123e18();                    // SYSMODE_LIGHT_ERRORWAIT
    void unkfunc_02123e40();                    // SYSMODE_FATAL_ERROR

    static int unk_02127140;                    // debug frame counter
    static int unk_02127144;
    static int parentCount_;                    // parents found by the scan
    static int scanning_;
    static int connected_[16];
    static WMBssDesc bssDesc_[4];
    alignas(32) static u8 sendBuffer_[0x1e0];               // DataSharing buffers (0xf0 per DS)
    alignas(32) static u8 recvBuffer_[0x1e0];
};

#define SEND_PACKET ((UnkMPExchangePacket*)UnkMPExchange::sendBuffer_)
#define RECV_PACKET(i) (&((UnkMPExchangePacket*)UnkMPExchange::recvBuffer_)[i])

extern "C" void ov002_entry();
