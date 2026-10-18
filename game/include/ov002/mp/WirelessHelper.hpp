#pragma once
#include "globaldefs.h"
#include "nitro/wm.h"
#include "nitro/os/os_owner.h"

extern "C" {
    int func_02066df0(u32 value);                                  // bit count
    void func_02079e40(OSOwnerInfo* info);                         // owner info
    char* STD_CopyLString(char* dst, const char* src, int size);
    BOOL func_0205e828(WMBssDesc* desc);

    // WM library
    WMErrCode func_0207cd74(void (*callback)(void* arg));         // set the indication callback
    u16 func_0207cfc0(void);                                       // allowed channels
    int func_0207cfe0(void);                                       // link level
    u16 func_0207d070(void);                                       // beacon period
    u16 func_0207d100(void);                                       // scan period
    WMErrCode func_0207d194(void* buf, void (*callback)(void* arg), u16 dmaNo);    // initialize
    WMErrCode func_0207d1f0(void (*callback)(void* arg));         // reset
    WMErrCode func_0207d228(void (*callback)(void* arg));         // end
    WMErrCode func_0207d268(void (*callback)(void* arg), WMParentParam* param);    // parent parameter
    WMErrCode func_0207d3f0(void (*callback)(void* arg));         // start parent
    WMErrCode func_0207d400(void (*callback)(void* arg));         // end parent
    WMErrCode func_0207d440(void (*callback)(void* arg), WMScanParam* param);      // start scan
    WMErrCode func_0207d52c(void (*callback)(void* arg));         // end scan
    WMErrCode func_0207d56c(void (*callback)(void* arg), WMBssDesc* desc, u8* ssid, BOOL powerSave, u16 authMode);  // start connect
    WMErrCode func_0207d638(void (*callback)(void* arg), u16 aid);                  // disconnect
    WMErrCode func_0207d880(void (*callback)(void* arg), u16* recvBuf, u16 recvBufSize, u16* sendBuf, u16 sendBufSize, u16 mpFreq);  // start MP
    WMErrCode func_0207da24(void (*callback)(void* arg));         // end MP
    WMErrCode func_0207da94(WMDataSharingInfo* info, u16 port, u16 aidBitmap, u16 dataLength, BOOL doubleMode);  // start data sharing
    WMErrCode func_0207dce8(WMDataSharingInfo* info);             // end data sharing
    WMErrCode func_0207dd30(WMDataSharingInfo* info, const u16* sendData, WMDataSet* receiveData);  // step data sharing
    u16* func_0207e590(WMDataSharingInfo* info, WMDataSet* receiveData, u16 aid);  // shared data address
    WMErrCode func_0207e614(WMKeySetBuf* buf, u16 port);          // start key sharing
    WMErrCode func_0207e630(WMKeySetBuf* buf);                    // end key sharing
    WMErrCode func_0207e63c(void (*callback)(void* arg), u16 wepMode, const u16* wepKey);  // WEP key
    int func_0207e6b0(void (*callback)(void* arg), void* info, u16 size, u32 ggid, u16 tgid, u8 attr);   // game info
    WMErrCode func_0207e768(void (*callback)(void* arg), u16 ccaMode, u16 edThreshold, u16 channel, u16 measureTime);  // measure channel
}

inline u64 OS_TicksToMilliSeconds(u64 ticks)
{
    return (ticks * 64) / 33514;
}

// ov002 wireless layer above WM (state names WH_SYSSTATE_*)
enum {
    WH_SYSSTATE_STOP = 0,
    WH_SYSSTATE_IDLE = 1,
    WH_SYSSTATE_SCANNING = 2,
    WH_SYSSTATE_BUSY = 3,
    WH_SYSSTATE_CONNECTED = 4,
    WH_SYSSTATE_DATASHARING = 5,
    WH_SYSSTATE_KEYSHARING = 6,
    WH_SYSSTATE_MEASURECHANNEL = 7,
    WH_SYSSTATE_CONNECT_FAIL = 8,
    WH_SYSSTATE_ERROR = 9,
    WH_SYSSTATE_FATAL = 10
};

// Connection modes
enum {
    WH_CONNECTMODE_MP_PARENT = 0,
    WH_CONNECTMODE_MP_CHILD = 1,
    WH_CONNECTMODE_KS_PARENT = 2,
    WH_CONNECTMODE_KS_CHILD = 3,
    WH_CONNECTMODE_DS_PARENT = 4,
    WH_CONNECTMODE_DS_CHILD = 5
};

void printWhDebug(const char* fmt, ...);                    // debug print (empty)
void changeWhSysState(int state);                               // set the system state
void setWhError(int code);                                // set the error code
BOOL setWhParentParam();                                        // set the parent parameter
void onSetParentParamCallback(void* arg);
BOOL setWhParentWepKey();                                        // set the parent WEP key
void onSetParentWepKeyCallback(void* arg);
BOOL startWhParent();                                        // start the parent
void onStartParentCallback(void* arg);
BOOL startWhParentMP();                                        // start MP (parent)
void onStartParentMPCallback(void* arg);
BOOL startWhParentKeySharing();                                        // start key sharing (parent)
BOOL endWhParentKeySharing();                                        // end key sharing (parent)
BOOL endWhParentMP();                                        // end MP (parent)
void onEndParentMPCallback(void* arg);
BOOL endWhParent();                                        // end the parent
void onEndParentCallback(void* arg);
BOOL startWhScan(void (*callback)(WMBssDesc*), const u8* bssid, u16 channel);  // start the scan
BOOL scanWhNextChannel();                                        // scan the next channel
void onStartScanCallback(void* arg);
BOOL endWhScan();                                        // end the scan
BOOL stopWhScan();                                        // end the scan (WM)
void onEndScanCallback(void* arg);
BOOL setWhChildWepKey();                                        // set the child WEP key
void onSetChildWepKeyCallback(void* arg);
BOOL startWhChildConnect();                                        // start the connection
void onStartConnectCallback(void* arg);
BOOL startWhChildMP();                                        // start MP (child)
void onStartChildMPCallback(void* arg);
BOOL startWhChildKeySharing();                                        // start key sharing (child)
BOOL endWhChildKeySharing();                                        // end key sharing (child)
BOOL endWhChildMP();                                        // end MP (child)
void onEndChildMPCallback(void* arg);
BOOL disconnectWhChild();                                        // disconnect (child)
void onEndChildCallback(void* arg);
BOOL requestWhReset();                                        // reset
void onResetCallback(void* arg);
void onWhEndCallback(void* arg);
void setWhGgid(u32 ggid);
void setWhJudgeAcceptFunc(BOOL (*judge)(void* arg));
u16 getWhConnectBitmap();                                         // connected bitmap
int getWhSysState();                                         // system state
const char* getWhSysStateName();                                 // system state name
void printWhBssDesc(WMBssDesc* desc);                         // print a parent description
BOOL startWhMeasureChannel();                                        // measure the channels
u16 measureWhNextChannel(u16 channel);                              // measure a channel
void onMeasureChannelCallback(void* arg);
int measureWhChannel(void (*callback)(void* arg), u16 channel);
u16 getWhBestChannel();                                         // best channel
s16 selectWhRandomChannel(u16 bitmap);                               // pick a channel
BOOL initWh();                                        // initialize
void onWhIndicateCallback(void* arg);
BOOL initWhWM();
void onWhInitializeCallback(void* arg);
BOOL connectWhParent(int mode, u16 tgid, u16 channel, u16 beaconPeriod);   // parent connect
BOOL connectWhChild(int mode, WMBssDesc* desc);               // child connect
u16* getWhSharedData(u16 aid);                                 // shared data of a child
BOOL stepWhDataSharing(const void* data);                        // data sharing step
void resetWh();                                        // reset
void finalizeWh();                                        // finalize
BOOL endWh();                                        // end
void setWhIndicateCallback(void (*callback)(void* arg));             // indication callback
int getWhBeaconCount();                                         // beacon count
void setWhForceError(int value);
