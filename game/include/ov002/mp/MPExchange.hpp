#pragma once
#include "globaldefs.h"
#include "main/cmn/EnvoyManager.hpp"
#include "ov002/mp/WirelessHelper.hpp"

// SYSMODE_* names printed by UnkMPExchange::getModeName
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

    int getNextRole();
    void buildTable();                    // build the table
    int advance();                     // next entry
    void printTable();                    // print the table
    void restartTimer();                    // restart the timer
    int isTimedOut();                     // time out
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

    virtual void updateSelectRole();            // SYSMODE_SELECT_ROLE
    virtual void updateSelectScan();            // SYSMODE_SELECT_SCAN
    virtual void updateSelectNext();            // SYSMODE_SELECT_NEXT
    virtual void updateParentLobby();            // SYSMODE_PARENT_LOBBY
    virtual void updateParentExec();            // SYSMODE_PARENT_EXEC
    virtual void updateChildScan();            // SYSMODE_CHILD_SCAN
    virtual void updateChildConnect();            // SYSMODE_CHILD_CONNECT
    virtual void updateChildReady();            // SYSMODE_CHILD_READY
    virtual void updateChildExec();            // SYSMODE_CHILD_EXEC
    virtual void updateFinishReady();            // SYSMODE_FINISH_READY
    virtual void receivePackets();            // receive
    virtual void sendPacket();            // send

    void printChildren();                    // print the children
    void setSendData(UnkEnvoyData* data);  // data to send
    UnkEnvoyData* getReceivedData();           // received data
    void start();                    // start
    void startRoleSelection();
    void update();                    // update
    void updateDataSharing();                    // debug print
    void setMode(int mode);            // set the mode
    const char* getModeName(int mode);     // mode name
    void unkfunc_021239d0();
    void updateGameInfo(int entry);           // set the game info
    static void onSetGameInfoCallback(void* arg);    // game info callback
    static void onIndicationCallback(void* arg);    // indication callback
    static void onScanCallback(WMBssDesc* desc);  // scan callback
    void startParent(u16 channel, int beaconPeriod);   // start as parent
    void startScan(int channel);         // start the scan
    void finish();
    void abort();
    void updateStopToReady0();                    // SYSMODE_STOP_toREADY00
    void updateStopToReady1();                    // SYSMODE_STOP_toREADY01
    void updateLightErrorInit();                    // SYSMODE_LIGHT_ERRORINIT
    void updateLightErrorWait();                    // SYSMODE_LIGHT_ERRORWAIT
    void updateFatalError();                    // SYSMODE_FATAL_ERROR

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
