#pragma ipa file
#include "ov002/mp/WirelessHelper.hpp"
#include "main/status/GameStatus.hpp"
#include "main/dss/DssUtils.hpp"
#include "nitro/dc.h"
#include "nitro/os/cpustat.h"

#define HW_VBLANK_COUNT_BUF 0x027ffc3c
#ifdef DQIV_NATIVE
#include "Settings.hpp" // pcVBlankCount
#endif

static u16 s_scanChannel;
static u16 s_initialized;
static u16 s_minBusyRatio;
static u16 s_connectBitmap;
static u16 s_autoConnect;
static u16 s_myAid;
static u16 s_busyChannels;
static u16 s_bestChannel;
static int s_beaconCount;
static u32 s_randSeed;
static int s_unk14;
static int s_sysState;
static u16 (*s_childWepKeyGenerator)(u16* wepKey, const WMBssDesc* desc);
static BOOL (*s_connectJudge)(void* arg);
static int s_recvSize;
static int s_connectMode;
static u16 (*s_parentWepKeyGenerator)(u16* wepKey, const WMParentParam* param);
static int s_sendSize;
static BOOL (*s_judge)(void* arg);
static int s_unk48;
static void (*s_scanCallback)(WMBssDesc* desc);
static void (*s_indCallback)(void* arg);
static int s_forceError;
static int s_errCode;
alignas(32) static u16 s_wepKey[10];
alignas(32) static WMScanParam s_scanParam;
alignas(32) static WMParentParam s_parentParam;
alignas(32) static WMBssDesc s_scanBuf;
alignas(32) static WMDataSet s_dsSet;
alignas(32) static WMDataSharingInfo s_dsInfo;
alignas(32) static WMKeySetBuf s_keySetBuf;
alignas(32) static u8 s_ssid[WM_SIZE_CHILD_SSID];
alignas(32) static u8 s_sendBuf[0x200];
alignas(32) static u8 s_recvBuf[0x440];
alignas(32) static u8 s_wmBuf[WM_SYSTEM_BUF_SIZE];

static inline BOOL IsValidGameInfo(const WMGameInfo* info, u16 length)
{
    return length >= 0x10 && info->magicNumber == 1;
}

ARM void printWhDebug(const char* fmt, ...)
{
}

ARM void changeWhSysState(int state)
{
    s_sysState = state;
}

ARM void setWhError(int code)
{
    if (s_sysState == WH_SYSSTATE_ERROR || s_sysState == WH_SYSSTATE_FATAL) {
        return;
    }
    s_errCode = code;
}

ARM BOOL setWhParentParam()
{
    changeWhSysState(WH_SYSSTATE_BUSY);
    WMErrCode result = func_0207d268(onSetParentParamCallback, &s_parentParam);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    setWhError(result);
    changeWhSysState(WH_SYSSTATE_ERROR);
    return FALSE;
}

ARM void onSetParentParamCallback(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        setWhError(cb->errcode);
        changeWhSysState(WH_SYSSTATE_ERROR);
        return;
    }
    if (s_parentWepKeyGenerator != NULL) {
        if (!setWhParentWepKey()) {
            changeWhSysState(WH_SYSSTATE_ERROR);
        }
    } else {
        if (!startWhParent()) {
            changeWhSysState(WH_SYSSTATE_ERROR);
        }
    }
}

ARM BOOL setWhParentWepKey()
{
    changeWhSysState(WH_SYSSTATE_BUSY);
    u16 wepMode = s_parentWepKeyGenerator(s_wepKey, &s_parentParam);
    WMErrCode result = func_0207e63c(onSetParentWepKeyCallback, wepMode, s_wepKey);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    setWhError(result);
    changeWhSysState(WH_SYSSTATE_ERROR);
    return FALSE;
}

ARM void onSetParentWepKeyCallback(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        setWhError(cb->errcode);
        changeWhSysState(WH_SYSSTATE_ERROR);
        return;
    }
    if (!startWhParent()) {
        changeWhSysState(WH_SYSSTATE_ERROR);
    }
}

ARM BOOL startWhParent()
{
    if (s_sysState == WH_SYSSTATE_CONNECTED || s_sysState == WH_SYSSTATE_DATASHARING || s_sysState == WH_SYSSTATE_KEYSHARING) {
        return TRUE;
    }
    WMErrCode result = func_0207d3f0(onStartParentCallback);
    if (result != WM_ERRCODE_OPERATING) {
        setWhError(result);
        return FALSE;
    }
    s_myAid = 0;
    s_connectBitmap = 1;
    return TRUE;
}

ARM void onStartParentCallback(void* arg)
{
    WMStartParentCallback* cb = (WMStartParentCallback*)arg;
    u16 bit = (u16)(1 << cb->aid);
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        setWhError(cb->errcode);
        changeWhSysState(WH_SYSSTATE_ERROR);
        return;
    }
    switch (cb->state) {
    case 2:
        s_beaconCount++;
        break;
    case WM_STATECODE_CONNECTED:
        if (s_connectJudge != NULL && !s_connectJudge(cb)) {
            WMErrCode result = func_0207d638(NULL, cb->aid);
            if (result != WM_ERRCODE_OPERATING) {
                setWhError(result);
                changeWhSysState(WH_SYSSTATE_ERROR);
            }
            break;
        }
        s_connectBitmap |= bit;
        break;
    case WM_STATECODE_DISCONNECTED:
        s_connectBitmap &= ~bit;
        break;
    case 0x1a:
        break;
    case WM_STATECODE_PARENT_START:
        if (!startWhParentMP()) {
            changeWhSysState(WH_SYSSTATE_ERROR);
        }
        break;
    }
}

ARM BOOL startWhParentMP()
{
    if (s_sysState == WH_SYSSTATE_CONNECTED || s_sysState == WH_SYSSTATE_KEYSHARING || s_sysState == WH_SYSSTATE_DATASHARING) {
        return TRUE;
    }
    changeWhSysState(WH_SYSSTATE_CONNECTED);
    WMErrCode result = func_0207d880(onStartParentMPCallback, (u16*)s_recvBuf, s_recvSize, (u16*)s_sendBuf, s_sendSize, 1);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    setWhError(result);
    return FALSE;
}

ARM void onStartParentMPCallback(void* arg)
{
    WMStartMPCallback* cb = (WMStartMPCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        setWhError(cb->errcode);
        changeWhSysState(WH_SYSSTATE_ERROR);
        return;
    }
    switch (cb->state) {
    case WM_STATECODE_MP_START:
        if (s_connectMode == WH_CONNECTMODE_KS_PARENT) {
            if (s_sysState == WH_SYSSTATE_CONNECTED) {
                if (!startWhParentKeySharing()) {
                    changeWhSysState(WH_SYSSTATE_ERROR);
                }
                return;
            }
            if (s_sysState == WH_SYSSTATE_KEYSHARING) {
                return;
            }
        } else if (s_connectMode == WH_CONNECTMODE_DS_PARENT) {
            u16 aidBitmap = 3;
            WMErrCode result = func_0207da94(&s_dsInfo, 13, aidBitmap, 0xf0, TRUE);
            if (result != WM_ERRCODE_SUCCESS) {
                setWhError(result);
                changeWhSysState(WH_SYSSTATE_ERROR);
                return;
            }
            changeWhSysState(WH_SYSSTATE_DATASHARING);
            return;
        }
        changeWhSysState(WH_SYSSTATE_CONNECTED);
        break;
    case 11:
    case 12:
    case 13:
        break;
    }
}

ARM BOOL startWhParentKeySharing()
{
    changeWhSysState(WH_SYSSTATE_KEYSHARING);
    WMErrCode result = func_0207e614(&s_keySetBuf, 13);
    if (result == WM_ERRCODE_SUCCESS) {
        return TRUE;
    }
    setWhError(result);
    return FALSE;
}

ARM BOOL endWhParentKeySharing()
{
    WMErrCode result = func_0207e630(&s_keySetBuf);
    if (result != WM_ERRCODE_SUCCESS) {
        setWhError(result);
        return FALSE;
    }
    if (endWhParentMP()) {
        return TRUE;
    }
    resetWh();
    return FALSE;
}

ARM BOOL endWhParentMP()
{
    changeWhSysState(WH_SYSSTATE_BUSY);
    WMErrCode result = func_0207da24(onEndParentMPCallback);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    setWhError(result);
    return FALSE;
}

ARM void onEndParentMPCallback(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        setWhError(cb->errcode);
        resetWh();
        return;
    }
    if (!endWhParent()) {
        resetWh();
    }
}

ARM BOOL endWhParent()
{
    WMErrCode result = func_0207d400(onEndParentCallback);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    setWhError(result);
    return FALSE;
}

ARM void onEndParentCallback(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        setWhError(cb->errcode);
        return;
    }
    changeWhSysState(WH_SYSSTATE_IDLE);
}

ARM BOOL startWhScan(void (*callback)(WMBssDesc*), const u8* bssid, u16 channel)
{
    changeWhSysState(WH_SYSSTATE_SCANNING);
    s_scanCallback = callback;
    s_scanChannel = channel;
    s_scanParam.channel = 0;
    s_autoConnect = 0;
    ((u16*)s_scanParam.bssid)[2] = ((const u16*)bssid)[2];
    ((u16*)s_scanParam.bssid)[1] = ((const u16*)bssid)[1];
    ((u16*)s_scanParam.bssid)[0] = ((const u16*)bssid)[0];
    if (scanWhNextChannel()) {
        return TRUE;
    }
    changeWhSysState(WH_SYSSTATE_ERROR);
    return FALSE;
}

ARM BOOL scanWhNextChannel()
{
    u16 allowed = func_0207cfc0();
    if (allowed == 0x8000) {
        setWhError(WM_ERRCODE_ILLEGAL_STATE);
        return FALSE;
    }
    if (allowed == 0) {
        setWhError(22);
        return FALSE;
    }
    if (s_scanChannel == 0) {
        while (TRUE) {
            s_scanParam.channel++;
            if (s_scanParam.channel > 16) {
                s_scanParam.channel = 1;
            }
            if (allowed & (1 << (s_scanParam.channel - 1))) {
                break;
            }
        }
    } else {
        s_scanParam.channel = s_scanChannel;
    }
    s_scanParam.maxChannelTime = func_0207d100();
    s_scanParam.scanBuf = &s_scanBuf;
    WMErrCode result = func_0207d440(onStartScanCallback, &s_scanParam);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    setWhError(result);
    return FALSE;
}

ARM void onStartScanCallback(void* arg)
{
    WMStartScanCallback* cb = (WMStartScanCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        setWhError(cb->errcode);
        changeWhSysState(WH_SYSSTATE_ERROR);
        return;
    }
    if (s_sysState != WH_SYSSTATE_SCANNING) {
        s_autoConnect = 0;
        if (!stopWhScan()) {
            changeWhSysState(WH_SYSSTATE_ERROR);
        }
        return;
    }
    switch (cb->state) {
    case 3:
        return;
    case 4:
        break;
    case WM_STATECODE_PARENT_FOUND:
        DC_InvalidateRange(&s_scanBuf, sizeof(WMBssDesc));
        if (s_unk14 != 0 && func_0205e828(&s_scanBuf)) {
            if (s_scanCallback != NULL) {
                s_scanCallback(&s_scanBuf);
            }
            break;
        }
        if (IsValidGameInfo(&cb->gameInfo, cb->gameInfoLength) && cb->gameInfo.ggid == s_parentParam.ggid) {
            switch (cb->gameInfo.gameNameCount_attribute & (WM_ATTR_FLAG_ENTRY | WM_ATTR_FLAG_MB)) {
            case WM_ATTR_FLAG_ENTRY:
                if (s_scanCallback != NULL) {
                    s_scanCallback(&s_scanBuf);
                }
                if (s_autoConnect != 0) {
                    if (!stopWhScan()) {
                        changeWhSysState(WH_SYSSTATE_ERROR);
                    }
                    return;
                }
                break;
            }
        }
        break;
    }
    if (!scanWhNextChannel()) {
        changeWhSysState(WH_SYSSTATE_ERROR);
    }
}

ARM BOOL endWhScan()
{
    if (s_sysState != WH_SYSSTATE_SCANNING) {
        return FALSE;
    }
    s_autoConnect = 0;
    changeWhSysState(WH_SYSSTATE_BUSY);
    return TRUE;
}

ARM BOOL stopWhScan()
{
    WMErrCode result = func_0207d52c(onEndScanCallback);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    setWhError(result);
    return FALSE;
}

ARM void onEndScanCallback(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        setWhError(cb->errcode);
        return;
    }
    changeWhSysState(WH_SYSSTATE_IDLE);
    if (s_autoConnect == 0) {
        return;
    }
    if (s_childWepKeyGenerator != NULL) {
        if (!setWhChildWepKey()) {
            changeWhSysState(WH_SYSSTATE_ERROR);
        }
    } else {
        if (!startWhChildConnect()) {
            changeWhSysState(WH_SYSSTATE_ERROR);
        }
    }
}

ARM BOOL setWhChildWepKey()
{
    changeWhSysState(WH_SYSSTATE_BUSY);
    u16 wepMode = s_childWepKeyGenerator(s_wepKey, &s_scanBuf);
    WMErrCode result = func_0207e63c(onSetChildWepKeyCallback, wepMode, s_wepKey);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    setWhError(result);
    changeWhSysState(WH_SYSSTATE_ERROR);
    return FALSE;
}

ARM void onSetChildWepKeyCallback(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        setWhError(cb->errcode);
        changeWhSysState(WH_SYSSTATE_ERROR);
        return;
    }
    if (!startWhChildConnect()) {
        changeWhSysState(WH_SYSSTATE_ERROR);
    }
}

ARM BOOL startWhChildConnect()
{
    if (s_sysState == WH_SYSSTATE_CONNECTED || s_sysState == WH_SYSSTATE_KEYSHARING || s_sysState == WH_SYSSTATE_DATASHARING) {
        return TRUE;
    }
    changeWhSysState(WH_SYSSTATE_BUSY);
    WMErrCode result = func_0207d56c(onStartConnectCallback, &s_scanBuf, s_ssid, TRUE, s_childWepKeyGenerator != NULL ? 1 : 0);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    setWhError(result);
    return FALSE;
}

ARM void onStartConnectCallback(void* arg)
{
    WMStartConnectCallback* cb = (WMStartConnectCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        setWhError(cb->errcode);
        if (cb->errcode == WM_ERRCODE_OVER_MAX_ENTRY) {
            changeWhSysState(WH_SYSSTATE_ERROR);
        } else if (cb->errcode == WM_ERRCODE_NO_ENTRY) {
            changeWhSysState(WH_SYSSTATE_ERROR);
        } else if (cb->errcode == WM_ERRCODE_FAILED) {
            changeWhSysState(WH_SYSSTATE_CONNECT_FAIL);
        } else {
            changeWhSysState(WH_SYSSTATE_ERROR);
        }
        return;
    }
    if (cb->state == WM_STATECODE_BEACON_LOST) {
    } else if (cb->state == WM_STATECODE_CONNECTED) {
        changeWhSysState(WH_SYSSTATE_CONNECTED);
        if (!startWhChildMP()) {
            changeWhSysState(WH_SYSSTATE_BUSY);
            return;
        }
        s_myAid = cb->aid;
    } else if (cb->state == 6) {
    } else if (cb->state == WM_STATECODE_DISCONNECTED) {
        setWhError(20);
        changeWhSysState(WH_SYSSTATE_ERROR);
    } else if (cb->state == 0x1a) {
    } else {
        changeWhSysState(WH_SYSSTATE_ERROR);
    }
}

ARM BOOL startWhChildMP()
{
    WMErrCode result = func_0207d880(onStartChildMPCallback, (u16*)s_recvBuf, s_recvSize, (u16*)s_sendBuf, s_sendSize, 1);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    setWhError(result);
    return FALSE;
}

ARM void onStartChildMPCallback(void* arg)
{
    WMStartMPCallback* cb = (WMStartMPCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        if (cb->errcode == WM_ERRCODE_SEND_FAILED || cb->errcode == WM_ERRCODE_TIMEOUT || cb->errcode == 13) {
            return;
        }
        setWhError(cb->errcode);
        changeWhSysState(WH_SYSSTATE_ERROR);
        return;
    }
    switch (cb->state) {
    case WM_STATECODE_MP_START:
        if (s_connectMode == WH_CONNECTMODE_KS_CHILD) {
            if (s_sysState == WH_SYSSTATE_KEYSHARING) {
                break;
            }
            if (s_sysState == WH_SYSSTATE_CONNECTED) {
                if (!startWhChildKeySharing()) {
                    finalizeWh();
                }
                break;
            }
        } else if (s_connectMode == WH_CONNECTMODE_DS_CHILD) {
            u16 aidBitmap = 3;
            WMErrCode result = func_0207da94(&s_dsInfo, 13, aidBitmap, 0xf0, TRUE);
            if (result != WM_ERRCODE_SUCCESS) {
                setWhError(result);
                finalizeWh();
                break;
            }
            changeWhSysState(WH_SYSSTATE_DATASHARING);
            break;
        }
        changeWhSysState(WH_SYSSTATE_CONNECTED);
        break;
    case 11:
    case 12:
    case 13:
        break;
    }
}

ARM BOOL startWhChildKeySharing()
{
    if (s_sysState == WH_SYSSTATE_KEYSHARING) {
        return TRUE;
    }
    if (s_sysState != WH_SYSSTATE_CONNECTED) {
        return FALSE;
    }
    changeWhSysState(WH_SYSSTATE_KEYSHARING);
    WMErrCode result = func_0207e614(&s_keySetBuf, 13);
    if (result == WM_ERRCODE_SUCCESS) {
        return TRUE;
    }
    setWhError(result);
    return FALSE;
}

ARM BOOL endWhChildKeySharing()
{
    if (s_sysState != WH_SYSSTATE_KEYSHARING) {
        return FALSE;
    }
    changeWhSysState(WH_SYSSTATE_BUSY);
    WMErrCode result = func_0207e630(&s_keySetBuf);
    if (result != WM_ERRCODE_SUCCESS) {
        setWhError(result);
        return FALSE;
    }
    if (!endWhChildMP()) {
        return FALSE;
    }
    return TRUE;
}

ARM BOOL endWhChildMP()
{
    changeWhSysState(WH_SYSSTATE_BUSY);
    WMErrCode result = func_0207da24(onEndChildMPCallback);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    setWhError(result);
    return FALSE;
}

ARM void onEndChildMPCallback(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        setWhError(cb->errcode);
        finalizeWh();
        return;
    }
    if (!disconnectWhChild()) {
        changeWhSysState(WH_SYSSTATE_ERROR);
    }
}

ARM BOOL disconnectWhChild()
{
    changeWhSysState(WH_SYSSTATE_BUSY);
    WMErrCode result = func_0207d638(onEndChildCallback, 0);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    setWhError(result);
    resetWh();
    return FALSE;
}

ARM void onEndChildCallback(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        setWhError(cb->errcode);
        return;
    }
    changeWhSysState(WH_SYSSTATE_IDLE);
}

ARM BOOL requestWhReset()
{
    changeWhSysState(WH_SYSSTATE_BUSY);
    WMErrCode result = func_0207d1f0(onResetCallback);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    setWhError(result);
    return FALSE;
}

ARM void onResetCallback(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        changeWhSysState(WH_SYSSTATE_ERROR);
        setWhError(cb->errcode);
        return;
    }
    changeWhSysState(WH_SYSSTATE_IDLE);
}

ARM void onWhEndCallback(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        changeWhSysState(WH_SYSSTATE_FATAL);
        return;
    }
    changeWhSysState(WH_SYSSTATE_STOP);
}

ARM void setWhGgid(u32 ggid)
{
    s_parentParam.ggid = ggid;
}

ARM void setWhJudgeAcceptFunc(BOOL (*judge)(void* arg))
{
    u32 enabled = OS_DisableIRQ();
    s_judge = judge;
    OS_RestoreIRQ(enabled);
}

ARM u16 getWhConnectBitmap()
{
    return s_connectBitmap;
}

ARM int getWhSysState()
{
    return s_sysState;
}

ARM const char* getWhSysStateName()
{
    const char* name = "";
    switch (s_sysState) {
    case WH_SYSSTATE_STOP:
        return "WH_SYSSTATE_STOP";
    case WH_SYSSTATE_IDLE:
        return "WH_SYSSTATE_IDLE";
    case WH_SYSSTATE_SCANNING:
        return "WH_SYSSTATE_SCANNING";
    case WH_SYSSTATE_BUSY:
        return "WH_SYSSTATE_BUSY";
    case WH_SYSSTATE_CONNECTED:
        return "WH_SYSSTATE_CONNECTED";
    case WH_SYSSTATE_DATASHARING:
        return "WH_SYSSTATE_DATASHARING";
    case WH_SYSSTATE_KEYSHARING:
        return "WH_SYSSTATE_KEYSHARING";
    case WH_SYSSTATE_MEASURECHANNEL:
        return "WH_SYSSTATE_MEASURECHANNEL";
    case WH_SYSSTATE_CONNECT_FAIL:
        return "WH_SYSSTATE_CONNECT_FAIL";
    case WH_SYSSTATE_ERROR:
        return "WH_SYSSTATE_ERROR";
    case WH_SYSSTATE_FATAL:
        return "WH_SYSSTATE_FATAL";
    }
    return name;
}

ARM void printWhBssDesc(WMBssDesc* desc)
{
    u16 i;
    printWhDebug("length = 0x%04x\n", desc->length);
    printWhDebug("rssi   = 0x%04x\n", desc->rssi);
    printWhDebug("bssid = %02x%02x%02x%02x%02x%02x\n", desc->bssid[0], desc->bssid[1], desc->bssid[2], desc->bssid[3],
                     desc->bssid[4], desc->bssid[5]);
    printWhDebug("ssidLength = 0x%04x\n", desc->ssidLength);
    printWhDebug("ssid = ");
    for (i = 0; i < WM_SIZE_SSID; i++) {
        printWhDebug("0x%02x", desc->ssid[i]);
    }
    printWhDebug("\n");
    printWhDebug("capaInfo        = 0x%04x\n", desc->capaInfo);
    printWhDebug("rateSet.basic   = 0x%04x\n", desc->rateSet.basic);
    printWhDebug("rateSet.support = 0x%04x\n", desc->rateSet.support);
    printWhDebug("beaconPeriod    = 0x%04x\n", desc->beaconPeriod);
    printWhDebug("dtimPeriod      = 0x%04x\n", desc->dtimPeriod);
    printWhDebug("channel         = 0x%04x\n", desc->channel);
    printWhDebug("cfpPeriod       = 0x%04x\n", desc->cfpPeriod);
    printWhDebug("cfpMaxDuration  = 0x%04x\n", desc->cfpMaxDuration);
    printWhDebug("gameInfoLength  = 0x%04x\n", desc->gameInfoLength);
    printWhDebug("gameInfo.magicNumber = 0x%04x\n", desc->gameInfo.magicNumber);
    printWhDebug("gameInfo.ver    = 0x%02x\n", desc->gameInfo.ver);
    printWhDebug("gameInfo.ggid   = 0x%08x\n", desc->gameInfo.ggid);
    printWhDebug("gameInfo.tgid   = 0x%04x\n", desc->gameInfo.tgid);
    printWhDebug("gameInfo.userGameInfoLength = 0x%02x\n", desc->gameInfo.userGameInfoLength);
    printWhDebug("gameInfo.gameNameCount_attribute = 0x%02x\n", desc->gameInfo.gameNameCount_attribute);
    printWhDebug("gameInfo.parentMaxSize   = 0x%04x\n", desc->gameInfo.parentMaxSize);
    printWhDebug("gameInfo.childMaxSize    = 0x%04x\n", desc->gameInfo.childMaxSize);
}

ARM BOOL startWhMeasureChannel()
{
    u16 macAddress[3];
    func_02079e24((u8*)macAddress);
#ifdef DQIV_NATIVE
    u32 vcount = pcVBlankCount();
#else
    u32 vcount = *(vu32*)HW_VBLANK_COUNT_BUF;
#endif
    s_randSeed = (macAddress[0] + vcount + macAddress[1] + macAddress[2]) * 0x10dcd + 0x3039;
    s_bestChannel = 0;
    s_minBusyRatio = 101;
    changeWhSysState(WH_SYSSTATE_BUSY);
    u16 result = measureWhNextChannel(1);
    if (result == 24) {
        setWhError(24);
        changeWhSysState(WH_SYSSTATE_ERROR);
        return FALSE;
    }
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    setWhError(result);
    changeWhSysState(WH_SYSSTATE_ERROR);
    return FALSE;
}

ARM u16 measureWhNextChannel(u16 channel)
{
    u16 allowed = func_0207cfc0();
    if (allowed == 0x8000) {
        setWhError(WM_ERRCODE_ILLEGAL_STATE);
        changeWhSysState(WH_SYSSTATE_ERROR);
        return WM_ERRCODE_ILLEGAL_STATE;
    }
    if (allowed == 0) {
        setWhError(22);
        changeWhSysState(WH_SYSSTATE_ERROR);
        return 24;
    }
    while (!(allowed & (1 << (channel - 1)))) {
        channel++;
        if (channel > 16) {
            return 24;
        }
    }
    return measureWhChannel(onMeasureChannelCallback, channel);
}

ARM void onMeasureChannelCallback(void* arg)
{
    WMMeasureChannelCallback* cb = (WMMeasureChannelCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        setWhError(cb->errcode);
        changeWhSysState(WH_SYSSTATE_ERROR);
        return;
    }
    u16 channel = cb->channel;
    if (s_minBusyRatio > cb->ccaBusyRatio) {
        s_minBusyRatio = cb->ccaBusyRatio;
        s_busyChannels = 1 << (channel - 1);
    } else if (s_minBusyRatio == cb->ccaBusyRatio) {
        s_busyChannels |= 1 << (channel - 1);
    }
    u16 result = measureWhNextChannel(channel + 1);
    if (result == 24) {
        changeWhSysState(WH_SYSSTATE_MEASURECHANNEL);
        return;
    }
    if (result != WM_ERRCODE_OPERATING) {
        changeWhSysState(WH_SYSSTATE_ERROR);
    }
}

ARM int measureWhChannel(void (*callback)(void* arg), u16 channel)
{
    return func_0207e768(callback, 3, 17, channel, 30);
}

ARM u16 getWhBestChannel()
{
    changeWhSysState(WH_SYSSTATE_IDLE);
    s_bestChannel = selectWhRandomChannel(s_busyChannels);
    return s_bestChannel;
}

ARM s16 selectWhRandomChannel(u16 bitmap)
{
    s16 i;
    s16 channel = 0;
    u16 count = 0;
    for (i = 0; i < 16; i++) {
        if (bitmap & (1 << i)) {
            channel = (s16)(i + 1);
            count++;
        }
    }
    if (count <= 1) {
        return channel;
    }
    s_randSeed = s_randSeed * 0x10dcd + 0x3039;
    u16 select = (u16)((count * (s_randSeed & 0xff)) >> 8);
    for (i = 0; i < 16; i++) {
        if (bitmap & 1) {
            if (select == 0) {
                return (s16)(i + 1);
            }
            select--;
        }
        bitmap >>= 1;
    }
    return 0;
}

ARM BOOL initWh()
{
    if (s_initialized) {
        return FALSE;
    }
    if (s_indCallback == NULL) {
        s_indCallback = onWhIndicateCallback;
    }
    s_recvSize = 0;
    s_sendSize = 0;
    s_unk48 = 0;
    s_myAid = 0;
    s_connectBitmap = 1;
    s_errCode = 0;
    s_parentParam.userGameInfo = NULL;
    s_parentParam.userGameInfoLength = 0;
    MI_CpuSet(s_ssid, 0, sizeof(s_ssid));
    s_connectJudge = NULL;
    if (!initWhWM()) {
        return FALSE;
    }
    s_initialized = TRUE;
    return TRUE;
}

ARM void onWhIndicateCallback(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (cb->errcode == 8) {
        changeWhSysState(WH_SYSSTATE_ERROR);
    }
}

ARM BOOL initWhWM()
{
    changeWhSysState(WH_SYSSTATE_BUSY);
    WMErrCode result = func_0207d194(s_wmBuf, onWhInitializeCallback, 0);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    setWhError(result);
    changeWhSysState(WH_SYSSTATE_FATAL);
    return FALSE;
}

ARM void onWhInitializeCallback(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (s_forceError == 1) {
        cb->errcode = WM_ERRCODE_FAILED;
    }
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        setWhError(cb->errcode);
        changeWhSysState(WH_SYSSTATE_FATAL);
        return;
    }
    WMErrCode result = func_0207cd74(s_indCallback);
    if (result != WM_ERRCODE_SUCCESS) {
        setWhError(result);
        changeWhSysState(WH_SYSSTATE_FATAL);
        return;
    }
    changeWhSysState(WH_SYSSTATE_IDLE);
}

ARM BOOL connectWhParent(int mode, u16 tgid, u16 channel, u16 beaconPeriod)
{
    s_recvSize = 0x240;
    s_sendSize = 0x200;
    s_connectMode = mode;
    changeWhSysState(WH_SYSSTATE_BUSY);
    s_beaconCount = 0;
    s_parentParam.tgid = tgid;
    s_parentParam.channel = channel;
    if (beaconPeriod == 0) {
        beaconPeriod = func_0207d070();
    }
    s_parentParam.beaconPeriod = beaconPeriod;
    s_parentParam.parentMaxSize = 0x1e4;
    s_parentParam.childMaxSize = 0xf0;
    s_parentParam.maxEntry = 1;
    s_parentParam.CS_Flag = 0;
    s_parentParam.multiBootFlag = 0;
    s_parentParam.entryFlag = 1;
    s_parentParam.KS_Flag = mode == WH_CONNECTMODE_KS_PARENT ? 1 : 0;
    switch (mode) {
    case WH_CONNECTMODE_MP_PARENT:
    case WH_CONNECTMODE_KS_PARENT:
    case WH_CONNECTMODE_DS_PARENT:
        return setWhParentParam();
    }
    return FALSE;
}

ARM BOOL connectWhChild(int mode, WMBssDesc* desc)
{
    s_recvSize = 0x440;
    s_sendSize = 0x100;
    s_connectMode = mode;
    changeWhSysState(WH_SYSSTATE_BUSY);
    switch (mode) {
    case WH_CONNECTMODE_MP_CHILD:
    case WH_CONNECTMODE_KS_CHILD:
    case WH_CONNECTMODE_DS_CHILD:
        MI_CpuCopyU8(desc, &s_scanBuf, sizeof(WMBssDesc));
        DC_PurgeRange(&s_scanBuf, sizeof(WMBssDesc));
        DC_DrainWriteBuffer();
        if (s_childWepKeyGenerator != NULL) {
            return setWhChildWepKey();
        }
        return startWhChildConnect();
    }
    return FALSE;
}

ARM u16* getWhSharedData(u16 aid)
{
    return func_0207e590(&s_dsInfo, &s_dsSet, aid);
}

ARM BOOL stepWhDataSharing(const void* data)
{
    WMErrCode result = func_0207dd30(&s_dsInfo, (const u16*)data, &s_dsSet);
    if (result == 7) {
        return TRUE;
    }
    if (result == 5) {
        setWhError(result);
        return FALSE;
    }
    if (result == WM_ERRCODE_SUCCESS) {
        return TRUE;
    }
    setWhError(result);
    return FALSE;
}

ARM void resetWh()
{
    if (s_sysState == WH_SYSSTATE_DATASHARING) {
        WMErrCode result = func_0207dce8(&s_dsInfo);
        if (result != WM_ERRCODE_SUCCESS) {
            setWhError(result);
        }
    }
    if (!requestWhReset()) {
        changeWhSysState(WH_SYSSTATE_FATAL);
    }
}

ARM void finalizeWh()
{
    s_beaconCount = 0;
    if (s_sysState == WH_SYSSTATE_IDLE) {
        return;
    }
    if (s_sysState == WH_SYSSTATE_SCANNING) {
        if (!endWhScan()) {
            resetWh();
        }
        return;
    }
    if (s_sysState != WH_SYSSTATE_KEYSHARING && s_sysState != WH_SYSSTATE_DATASHARING && s_sysState != WH_SYSSTATE_CONNECTED) {
        changeWhSysState(WH_SYSSTATE_BUSY);
        resetWh();
        return;
    }
    changeWhSysState(WH_SYSSTATE_BUSY);
    switch (s_connectMode) {
    case WH_CONNECTMODE_KS_CHILD:
        if (!endWhChildKeySharing()) {
            resetWh();
        }
        break;
    case WH_CONNECTMODE_DS_CHILD:
        if (func_0207dce8(&s_dsInfo) != WM_ERRCODE_SUCCESS) {
            resetWh();
            break;
        }
    case WH_CONNECTMODE_MP_CHILD:
        if (!endWhChildMP()) {
            resetWh();
        }
        break;
    case WH_CONNECTMODE_KS_PARENT:
        if (!endWhParentKeySharing()) {
            resetWh();
        }
        break;
    case WH_CONNECTMODE_DS_PARENT:
        if (func_0207dce8(&s_dsInfo) != WM_ERRCODE_SUCCESS) {
            resetWh();
            break;
        }
    case WH_CONNECTMODE_MP_PARENT:
        if (!endWhParentMP()) {
            resetWh();
        }
        break;
    }
}

ARM BOOL endWh()
{
    changeWhSysState(WH_SYSSTATE_BUSY);
    if (func_0207d228(onWhEndCallback) != WM_ERRCODE_OPERATING) {
        changeWhSysState(WH_SYSSTATE_ERROR);
        return FALSE;
    }
    s_initialized = FALSE;
    return TRUE;
}

ARM void setWhIndicateCallback(void (*callback)(void* arg))
{
    s_indCallback = callback;
}

ARM int getWhBeaconCount()
{
    return s_beaconCount;
}

ARM void setWhForceError(int value)
{
    s_forceError = value;
}
