#pragma ipa file
#include "ov002/mp/UnkWH.hpp"
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

ARM void unkfunc_021210e4(const char* fmt, ...)
{
}

ARM void unkfunc_021210f0(int state)
{
    s_sysState = state;
}

ARM void unkfunc_02121100(int code)
{
    if (s_sysState == WH_SYSSTATE_ERROR || s_sysState == WH_SYSSTATE_FATAL) {
        return;
    }
    s_errCode = code;
}

ARM BOOL unkfunc_0212111c()
{
    unkfunc_021210f0(WH_SYSSTATE_BUSY);
    WMErrCode result = func_0207d268(unkfunc_0212115c, &s_parentParam);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    unkfunc_02121100(result);
    unkfunc_021210f0(WH_SYSSTATE_ERROR);
    return FALSE;
}

ARM void unkfunc_0212115c(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        unkfunc_02121100(cb->errcode);
        unkfunc_021210f0(WH_SYSSTATE_ERROR);
        return;
    }
    if (s_parentWepKeyGenerator != NULL) {
        if (!unkfunc_021211c0()) {
            unkfunc_021210f0(WH_SYSSTATE_ERROR);
        }
    } else {
        if (!unkfunc_02121258()) {
            unkfunc_021210f0(WH_SYSSTATE_ERROR);
        }
    }
}

ARM BOOL unkfunc_021211c0()
{
    unkfunc_021210f0(WH_SYSSTATE_BUSY);
    u16 wepMode = s_parentWepKeyGenerator(s_wepKey, &s_parentParam);
    WMErrCode result = func_0207e63c(unkfunc_02121220, wepMode, s_wepKey);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    unkfunc_02121100(result);
    unkfunc_021210f0(WH_SYSSTATE_ERROR);
    return FALSE;
}

ARM void unkfunc_02121220(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        unkfunc_02121100(cb->errcode);
        unkfunc_021210f0(WH_SYSSTATE_ERROR);
        return;
    }
    if (!unkfunc_02121258()) {
        unkfunc_021210f0(WH_SYSSTATE_ERROR);
    }
}

ARM BOOL unkfunc_02121258()
{
    if (s_sysState == WH_SYSSTATE_CONNECTED || s_sysState == WH_SYSSTATE_DATASHARING || s_sysState == WH_SYSSTATE_KEYSHARING) {
        return TRUE;
    }
    WMErrCode result = func_0207d3f0(unkfunc_021212b0);
    if (result != WM_ERRCODE_OPERATING) {
        unkfunc_02121100(result);
        return FALSE;
    }
    s_myAid = 0;
    s_connectBitmap = 1;
    return TRUE;
}

ARM void unkfunc_021212b0(void* arg)
{
    WMStartParentCallback* cb = (WMStartParentCallback*)arg;
    u16 bit = (u16)(1 << cb->aid);
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        unkfunc_02121100(cb->errcode);
        unkfunc_021210f0(WH_SYSSTATE_ERROR);
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
                unkfunc_02121100(result);
                unkfunc_021210f0(WH_SYSSTATE_ERROR);
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
        if (!unkfunc_021213cc()) {
            unkfunc_021210f0(WH_SYSSTATE_ERROR);
        }
        break;
    }
}

ARM BOOL unkfunc_021213cc()
{
    if (s_sysState == WH_SYSSTATE_CONNECTED || s_sysState == WH_SYSSTATE_KEYSHARING || s_sysState == WH_SYSSTATE_DATASHARING) {
        return TRUE;
    }
    unkfunc_021210f0(WH_SYSSTATE_CONNECTED);
    WMErrCode result = func_0207d880(unkfunc_02121460, (u16*)s_recvBuf, s_recvSize, (u16*)s_sendBuf, s_sendSize, 1);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    unkfunc_02121100(result);
    return FALSE;
}

ARM void unkfunc_02121460(void* arg)
{
    WMStartMPCallback* cb = (WMStartMPCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        unkfunc_02121100(cb->errcode);
        unkfunc_021210f0(WH_SYSSTATE_ERROR);
        return;
    }
    switch (cb->state) {
    case WM_STATECODE_MP_START:
        if (s_connectMode == WH_CONNECTMODE_KS_PARENT) {
            if (s_sysState == WH_SYSSTATE_CONNECTED) {
                if (!unkfunc_02121544()) {
                    unkfunc_021210f0(WH_SYSSTATE_ERROR);
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
                unkfunc_02121100(result);
                unkfunc_021210f0(WH_SYSSTATE_ERROR);
                return;
            }
            unkfunc_021210f0(WH_SYSSTATE_DATASHARING);
            return;
        }
        unkfunc_021210f0(WH_SYSSTATE_CONNECTED);
        break;
    case 11:
    case 12:
    case 13:
        break;
    }
}

ARM BOOL unkfunc_02121544()
{
    unkfunc_021210f0(WH_SYSSTATE_KEYSHARING);
    WMErrCode result = func_0207e614(&s_keySetBuf, 13);
    if (result == WM_ERRCODE_SUCCESS) {
        return TRUE;
    }
    unkfunc_02121100(result);
    return FALSE;
}

ARM BOOL unkfunc_02121578()
{
    WMErrCode result = func_0207e630(&s_keySetBuf);
    if (result != WM_ERRCODE_SUCCESS) {
        unkfunc_02121100(result);
        return FALSE;
    }
    if (unkfunc_021215b8()) {
        return TRUE;
    }
    unkfunc_02122840();
    return FALSE;
}

ARM BOOL unkfunc_021215b8()
{
    unkfunc_021210f0(WH_SYSSTATE_BUSY);
    WMErrCode result = func_0207da24(unkfunc_021215e8);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    unkfunc_02121100(result);
    return FALSE;
}

ARM void unkfunc_021215e8(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        unkfunc_02121100(cb->errcode);
        unkfunc_02122840();
        return;
    }
    if (!unkfunc_02121618()) {
        unkfunc_02122840();
    }
}

ARM BOOL unkfunc_02121618()
{
    WMErrCode result = func_0207d400(unkfunc_02121640);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    unkfunc_02121100(result);
    return FALSE;
}

ARM void unkfunc_02121640(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        unkfunc_02121100(cb->errcode);
        return;
    }
    unkfunc_021210f0(WH_SYSSTATE_IDLE);
}

ARM BOOL unkfunc_02121664(void (*callback)(WMBssDesc*), const u8* bssid, u16 channel)
{
    unkfunc_021210f0(WH_SYSSTATE_SCANNING);
    s_scanCallback = callback;
    s_scanChannel = channel;
    s_scanParam.channel = 0;
    s_autoConnect = 0;
    ((u16*)s_scanParam.bssid)[2] = ((const u16*)bssid)[2];
    ((u16*)s_scanParam.bssid)[1] = ((const u16*)bssid)[1];
    ((u16*)s_scanParam.bssid)[0] = ((const u16*)bssid)[0];
    if (unkfunc_021216d0()) {
        return TRUE;
    }
    unkfunc_021210f0(WH_SYSSTATE_ERROR);
    return FALSE;
}

ARM BOOL unkfunc_021216d0()
{
    u16 allowed = func_0207cfc0();
    if (allowed == 0x8000) {
        unkfunc_02121100(WM_ERRCODE_ILLEGAL_STATE);
        return FALSE;
    }
    if (allowed == 0) {
        unkfunc_02121100(22);
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
    WMErrCode result = func_0207d440(unkfunc_02121798, &s_scanParam);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    unkfunc_02121100(result);
    return FALSE;
}

ARM void unkfunc_02121798(void* arg)
{
    WMStartScanCallback* cb = (WMStartScanCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        unkfunc_02121100(cb->errcode);
        unkfunc_021210f0(WH_SYSSTATE_ERROR);
        return;
    }
    if (s_sysState != WH_SYSSTATE_SCANNING) {
        s_autoConnect = 0;
        if (!unkfunc_02121928()) {
            unkfunc_021210f0(WH_SYSSTATE_ERROR);
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
                    if (!unkfunc_02121928()) {
                        unkfunc_021210f0(WH_SYSSTATE_ERROR);
                    }
                    return;
                }
                break;
            }
        }
        break;
    }
    if (!unkfunc_021216d0()) {
        unkfunc_021210f0(WH_SYSSTATE_ERROR);
    }
}

ARM BOOL unkfunc_021218f4()
{
    if (s_sysState != WH_SYSSTATE_SCANNING) {
        return FALSE;
    }
    s_autoConnect = 0;
    unkfunc_021210f0(WH_SYSSTATE_BUSY);
    return TRUE;
}

ARM BOOL unkfunc_02121928()
{
    WMErrCode result = func_0207d52c(unkfunc_02121950);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    unkfunc_02121100(result);
    return FALSE;
}

ARM void unkfunc_02121950(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        unkfunc_02121100(cb->errcode);
        return;
    }
    unkfunc_021210f0(WH_SYSSTATE_IDLE);
    if (s_autoConnect == 0) {
        return;
    }
    if (s_childWepKeyGenerator != NULL) {
        if (!unkfunc_021219c0()) {
            unkfunc_021210f0(WH_SYSSTATE_ERROR);
        }
    } else {
        if (!unkfunc_02121a58()) {
            unkfunc_021210f0(WH_SYSSTATE_ERROR);
        }
    }
}

ARM BOOL unkfunc_021219c0()
{
    unkfunc_021210f0(WH_SYSSTATE_BUSY);
    u16 wepMode = s_childWepKeyGenerator(s_wepKey, &s_scanBuf);
    WMErrCode result = func_0207e63c(unkfunc_02121a20, wepMode, s_wepKey);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    unkfunc_02121100(result);
    unkfunc_021210f0(WH_SYSSTATE_ERROR);
    return FALSE;
}

ARM void unkfunc_02121a20(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        unkfunc_02121100(cb->errcode);
        unkfunc_021210f0(WH_SYSSTATE_ERROR);
        return;
    }
    if (!unkfunc_02121a58()) {
        unkfunc_021210f0(WH_SYSSTATE_ERROR);
    }
}

ARM BOOL unkfunc_02121a58()
{
    if (s_sysState == WH_SYSSTATE_CONNECTED || s_sysState == WH_SYSSTATE_KEYSHARING || s_sysState == WH_SYSSTATE_DATASHARING) {
        return TRUE;
    }
    unkfunc_021210f0(WH_SYSSTATE_BUSY);
    WMErrCode result = func_0207d56c(unkfunc_02121adc, &s_scanBuf, s_ssid, TRUE, s_childWepKeyGenerator != NULL ? 1 : 0);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    unkfunc_02121100(result);
    return FALSE;
}

ARM void unkfunc_02121adc(void* arg)
{
    WMStartConnectCallback* cb = (WMStartConnectCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        unkfunc_02121100(cb->errcode);
        if (cb->errcode == WM_ERRCODE_OVER_MAX_ENTRY) {
            unkfunc_021210f0(WH_SYSSTATE_ERROR);
        } else if (cb->errcode == WM_ERRCODE_NO_ENTRY) {
            unkfunc_021210f0(WH_SYSSTATE_ERROR);
        } else if (cb->errcode == WM_ERRCODE_FAILED) {
            unkfunc_021210f0(WH_SYSSTATE_CONNECT_FAIL);
        } else {
            unkfunc_021210f0(WH_SYSSTATE_ERROR);
        }
        return;
    }
    if (cb->state == WM_STATECODE_BEACON_LOST) {
    } else if (cb->state == WM_STATECODE_CONNECTED) {
        unkfunc_021210f0(WH_SYSSTATE_CONNECTED);
        if (!unkfunc_02121bc0()) {
            unkfunc_021210f0(WH_SYSSTATE_BUSY);
            return;
        }
        s_myAid = cb->aid;
    } else if (cb->state == 6) {
    } else if (cb->state == WM_STATECODE_DISCONNECTED) {
        unkfunc_02121100(20);
        unkfunc_021210f0(WH_SYSSTATE_ERROR);
    } else if (cb->state == 0x1a) {
    } else {
        unkfunc_021210f0(WH_SYSSTATE_ERROR);
    }
}

ARM BOOL unkfunc_02121bc0()
{
    WMErrCode result = func_0207d880(unkfunc_02121c2c, (u16*)s_recvBuf, s_recvSize, (u16*)s_sendBuf, s_sendSize, 1);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    unkfunc_02121100(result);
    return FALSE;
}

ARM void unkfunc_02121c2c(void* arg)
{
    WMStartMPCallback* cb = (WMStartMPCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        if (cb->errcode == WM_ERRCODE_SEND_FAILED || cb->errcode == WM_ERRCODE_TIMEOUT || cb->errcode == 13) {
            return;
        }
        unkfunc_02121100(cb->errcode);
        unkfunc_021210f0(WH_SYSSTATE_ERROR);
        return;
    }
    switch (cb->state) {
    case WM_STATECODE_MP_START:
        if (s_connectMode == WH_CONNECTMODE_KS_CHILD) {
            if (s_sysState == WH_SYSSTATE_KEYSHARING) {
                break;
            }
            if (s_sysState == WH_SYSSTATE_CONNECTED) {
                if (!unkfunc_02121d14()) {
                    unkfunc_02122888();
                }
                break;
            }
        } else if (s_connectMode == WH_CONNECTMODE_DS_CHILD) {
            u16 aidBitmap = 3;
            WMErrCode result = func_0207da94(&s_dsInfo, 13, aidBitmap, 0xf0, TRUE);
            if (result != WM_ERRCODE_SUCCESS) {
                unkfunc_02121100(result);
                unkfunc_02122888();
                break;
            }
            unkfunc_021210f0(WH_SYSSTATE_DATASHARING);
            break;
        }
        unkfunc_021210f0(WH_SYSSTATE_CONNECTED);
        break;
    case 11:
    case 12:
    case 13:
        break;
    }
}

ARM BOOL unkfunc_02121d14()
{
    if (s_sysState == WH_SYSSTATE_KEYSHARING) {
        return TRUE;
    }
    if (s_sysState != WH_SYSSTATE_CONNECTED) {
        return FALSE;
    }
    unkfunc_021210f0(WH_SYSSTATE_KEYSHARING);
    WMErrCode result = func_0207e614(&s_keySetBuf, 13);
    if (result == WM_ERRCODE_SUCCESS) {
        return TRUE;
    }
    unkfunc_02121100(result);
    return FALSE;
}

ARM BOOL unkfunc_02121d6c()
{
    if (s_sysState != WH_SYSSTATE_KEYSHARING) {
        return FALSE;
    }
    unkfunc_021210f0(WH_SYSSTATE_BUSY);
    WMErrCode result = func_0207e630(&s_keySetBuf);
    if (result != WM_ERRCODE_SUCCESS) {
        unkfunc_02121100(result);
        return FALSE;
    }
    if (!unkfunc_02121dc4()) {
        return FALSE;
    }
    return TRUE;
}

ARM BOOL unkfunc_02121dc4()
{
    unkfunc_021210f0(WH_SYSSTATE_BUSY);
    WMErrCode result = func_0207da24(unkfunc_02121df4);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    unkfunc_02121100(result);
    return FALSE;
}

ARM void unkfunc_02121df4(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        unkfunc_02121100(cb->errcode);
        unkfunc_02122888();
        return;
    }
    if (!unkfunc_02121e28()) {
        unkfunc_021210f0(WH_SYSSTATE_ERROR);
    }
}

ARM BOOL unkfunc_02121e28()
{
    unkfunc_021210f0(WH_SYSSTATE_BUSY);
    WMErrCode result = func_0207d638(unkfunc_02121e60, 0);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    unkfunc_02121100(result);
    unkfunc_02122840();
    return FALSE;
}

ARM void unkfunc_02121e60(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        unkfunc_02121100(cb->errcode);
        return;
    }
    unkfunc_021210f0(WH_SYSSTATE_IDLE);
}

ARM BOOL unkfunc_02121e84()
{
    unkfunc_021210f0(WH_SYSSTATE_BUSY);
    WMErrCode result = func_0207d1f0(unkfunc_02121eb4);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    unkfunc_02121100(result);
    return FALSE;
}

ARM void unkfunc_02121eb4(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        unkfunc_021210f0(WH_SYSSTATE_ERROR);
        unkfunc_02121100(cb->errcode);
        return;
    }
    unkfunc_021210f0(WH_SYSSTATE_IDLE);
}

ARM void unkfunc_02121ee8(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        unkfunc_021210f0(WH_SYSSTATE_FATAL);
        return;
    }
    unkfunc_021210f0(WH_SYSSTATE_STOP);
}

ARM void unkfunc_02121f10(u32 ggid)
{
    s_parentParam.ggid = ggid;
}

ARM void unkfunc_02121f20(BOOL (*judge)(void* arg))
{
    u32 enabled = OS_DisableIRQ();
    s_judge = judge;
    OS_RestoreIRQ(enabled);
}

ARM u16 unkfunc_02121f40()
{
    return s_connectBitmap;
}

ARM int unkfunc_02121f50()
{
    return s_sysState;
}

ARM const char* unkfunc_02121f60()
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

ARM void unkfunc_02122030(WMBssDesc* desc)
{
    u16 i;
    unkfunc_021210e4("length = 0x%04x\n", desc->length);
    unkfunc_021210e4("rssi   = 0x%04x\n", desc->rssi);
    unkfunc_021210e4("bssid = %02x%02x%02x%02x%02x%02x\n", desc->bssid[0], desc->bssid[1], desc->bssid[2], desc->bssid[3],
                     desc->bssid[4], desc->bssid[5]);
    unkfunc_021210e4("ssidLength = 0x%04x\n", desc->ssidLength);
    unkfunc_021210e4("ssid = ");
    for (i = 0; i < WM_SIZE_SSID; i++) {
        unkfunc_021210e4("0x%02x", desc->ssid[i]);
    }
    unkfunc_021210e4("\n");
    unkfunc_021210e4("capaInfo        = 0x%04x\n", desc->capaInfo);
    unkfunc_021210e4("rateSet.basic   = 0x%04x\n", desc->rateSet.basic);
    unkfunc_021210e4("rateSet.support = 0x%04x\n", desc->rateSet.support);
    unkfunc_021210e4("beaconPeriod    = 0x%04x\n", desc->beaconPeriod);
    unkfunc_021210e4("dtimPeriod      = 0x%04x\n", desc->dtimPeriod);
    unkfunc_021210e4("channel         = 0x%04x\n", desc->channel);
    unkfunc_021210e4("cfpPeriod       = 0x%04x\n", desc->cfpPeriod);
    unkfunc_021210e4("cfpMaxDuration  = 0x%04x\n", desc->cfpMaxDuration);
    unkfunc_021210e4("gameInfoLength  = 0x%04x\n", desc->gameInfoLength);
    unkfunc_021210e4("gameInfo.magicNumber = 0x%04x\n", desc->gameInfo.magicNumber);
    unkfunc_021210e4("gameInfo.ver    = 0x%02x\n", desc->gameInfo.ver);
    unkfunc_021210e4("gameInfo.ggid   = 0x%08x\n", desc->gameInfo.ggid);
    unkfunc_021210e4("gameInfo.tgid   = 0x%04x\n", desc->gameInfo.tgid);
    unkfunc_021210e4("gameInfo.userGameInfoLength = 0x%02x\n", desc->gameInfo.userGameInfoLength);
    unkfunc_021210e4("gameInfo.gameNameCount_attribute = 0x%02x\n", desc->gameInfo.gameNameCount_attribute);
    unkfunc_021210e4("gameInfo.parentMaxSize   = 0x%04x\n", desc->gameInfo.parentMaxSize);
    unkfunc_021210e4("gameInfo.childMaxSize    = 0x%04x\n", desc->gameInfo.childMaxSize);
}

ARM BOOL unkfunc_021221fc()
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
    unkfunc_021210f0(WH_SYSSTATE_BUSY);
    u16 result = unkfunc_021222bc(1);
    if (result == 24) {
        unkfunc_02121100(24);
        unkfunc_021210f0(WH_SYSSTATE_ERROR);
        return FALSE;
    }
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    unkfunc_02121100(result);
    unkfunc_021210f0(WH_SYSSTATE_ERROR);
    return FALSE;
}

ARM u16 unkfunc_021222bc(u16 channel)
{
    u16 allowed = func_0207cfc0();
    if (allowed == 0x8000) {
        unkfunc_02121100(WM_ERRCODE_ILLEGAL_STATE);
        unkfunc_021210f0(WH_SYSSTATE_ERROR);
        return WM_ERRCODE_ILLEGAL_STATE;
    }
    if (allowed == 0) {
        unkfunc_02121100(22);
        unkfunc_021210f0(WH_SYSSTATE_ERROR);
        return 24;
    }
    while (!(allowed & (1 << (channel - 1)))) {
        channel++;
        if (channel > 16) {
            return 24;
        }
    }
    return unkfunc_02122400(unkfunc_02122358, channel);
}

ARM void unkfunc_02122358(void* arg)
{
    WMMeasureChannelCallback* cb = (WMMeasureChannelCallback*)arg;
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        unkfunc_02121100(cb->errcode);
        unkfunc_021210f0(WH_SYSSTATE_ERROR);
        return;
    }
    u16 channel = cb->channel;
    if (s_minBusyRatio > cb->ccaBusyRatio) {
        s_minBusyRatio = cb->ccaBusyRatio;
        s_busyChannels = 1 << (channel - 1);
    } else if (s_minBusyRatio == cb->ccaBusyRatio) {
        s_busyChannels |= 1 << (channel - 1);
    }
    u16 result = unkfunc_021222bc(channel + 1);
    if (result == 24) {
        unkfunc_021210f0(WH_SYSSTATE_MEASURECHANNEL);
        return;
    }
    if (result != WM_ERRCODE_OPERATING) {
        unkfunc_021210f0(WH_SYSSTATE_ERROR);
    }
}

ARM int unkfunc_02122400(void (*callback)(void* arg), u16 channel)
{
    return func_0207e768(callback, 3, 17, channel, 30);
}

ARM u16 unkfunc_02122420()
{
    unkfunc_021210f0(WH_SYSSTATE_IDLE);
    s_bestChannel = unkfunc_0212244c(s_busyChannels);
    return s_bestChannel;
}

ARM s16 unkfunc_0212244c(u16 bitmap)
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

ARM BOOL unkfunc_02122528()
{
    if (s_initialized) {
        return FALSE;
    }
    if (s_indCallback == NULL) {
        s_indCallback = unkfunc_021225bc;
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
    if (!unkfunc_021225d8()) {
        return FALSE;
    }
    s_initialized = TRUE;
    return TRUE;
}

ARM void unkfunc_021225bc(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (cb->errcode == 8) {
        unkfunc_021210f0(WH_SYSSTATE_ERROR);
    }
}

ARM BOOL unkfunc_021225d8()
{
    unkfunc_021210f0(WH_SYSSTATE_BUSY);
    WMErrCode result = func_0207d194(s_wmBuf, unkfunc_0212261c, 0);
    if (result == WM_ERRCODE_OPERATING) {
        return TRUE;
    }
    unkfunc_02121100(result);
    unkfunc_021210f0(WH_SYSSTATE_FATAL);
    return FALSE;
}

ARM void unkfunc_0212261c(void* arg)
{
    WMCallback* cb = (WMCallback*)arg;
    if (s_forceError == 1) {
        cb->errcode = WM_ERRCODE_FAILED;
    }
    if (cb->errcode != WM_ERRCODE_SUCCESS) {
        unkfunc_02121100(cb->errcode);
        unkfunc_021210f0(WH_SYSSTATE_FATAL);
        return;
    }
    WMErrCode result = func_0207cd74(s_indCallback);
    if (result != WM_ERRCODE_SUCCESS) {
        unkfunc_02121100(result);
        unkfunc_021210f0(WH_SYSSTATE_FATAL);
        return;
    }
    unkfunc_021210f0(WH_SYSSTATE_IDLE);
}

ARM BOOL unkfunc_02122684(int mode, u16 tgid, u16 channel, u16 beaconPeriod)
{
    s_recvSize = 0x240;
    s_sendSize = 0x200;
    s_connectMode = mode;
    unkfunc_021210f0(WH_SYSSTATE_BUSY);
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
        return unkfunc_0212111c();
    }
    return FALSE;
}

ARM BOOL unkfunc_02122740(int mode, WMBssDesc* desc)
{
    s_recvSize = 0x440;
    s_sendSize = 0x100;
    s_connectMode = mode;
    unkfunc_021210f0(WH_SYSSTATE_BUSY);
    switch (mode) {
    case WH_CONNECTMODE_MP_CHILD:
    case WH_CONNECTMODE_KS_CHILD:
    case WH_CONNECTMODE_DS_CHILD:
        MI_CpuCopyU8(desc, &s_scanBuf, sizeof(WMBssDesc));
        DC_PurgeRange(&s_scanBuf, sizeof(WMBssDesc));
        DC_DrainWriteBuffer();
        if (s_childWepKeyGenerator != NULL) {
            return unkfunc_021219c0();
        }
        return unkfunc_02121a58();
    }
    return FALSE;
}

ARM u16* unkfunc_021227cc(u16 aid)
{
    return func_0207e590(&s_dsInfo, &s_dsSet, aid);
}

ARM BOOL unkfunc_021227ec(const void* data)
{
    WMErrCode result = func_0207dd30(&s_dsInfo, (const u16*)data, &s_dsSet);
    if (result == 7) {
        return TRUE;
    }
    if (result == 5) {
        unkfunc_02121100(result);
        return FALSE;
    }
    if (result == WM_ERRCODE_SUCCESS) {
        return TRUE;
    }
    unkfunc_02121100(result);
    return FALSE;
}

ARM void unkfunc_02122840()
{
    if (s_sysState == WH_SYSSTATE_DATASHARING) {
        WMErrCode result = func_0207dce8(&s_dsInfo);
        if (result != WM_ERRCODE_SUCCESS) {
            unkfunc_02121100(result);
        }
    }
    if (!unkfunc_02121e84()) {
        unkfunc_021210f0(WH_SYSSTATE_FATAL);
    }
}

ARM void unkfunc_02122888()
{
    s_beaconCount = 0;
    if (s_sysState == WH_SYSSTATE_IDLE) {
        return;
    }
    if (s_sysState == WH_SYSSTATE_SCANNING) {
        if (!unkfunc_021218f4()) {
            unkfunc_02122840();
        }
        return;
    }
    if (s_sysState != WH_SYSSTATE_KEYSHARING && s_sysState != WH_SYSSTATE_DATASHARING && s_sysState != WH_SYSSTATE_CONNECTED) {
        unkfunc_021210f0(WH_SYSSTATE_BUSY);
        unkfunc_02122840();
        return;
    }
    unkfunc_021210f0(WH_SYSSTATE_BUSY);
    switch (s_connectMode) {
    case WH_CONNECTMODE_KS_CHILD:
        if (!unkfunc_02121d6c()) {
            unkfunc_02122840();
        }
        break;
    case WH_CONNECTMODE_DS_CHILD:
        if (func_0207dce8(&s_dsInfo) != WM_ERRCODE_SUCCESS) {
            unkfunc_02122840();
            break;
        }
    case WH_CONNECTMODE_MP_CHILD:
        if (!unkfunc_02121dc4()) {
            unkfunc_02122840();
        }
        break;
    case WH_CONNECTMODE_KS_PARENT:
        if (!unkfunc_02121578()) {
            unkfunc_02122840();
        }
        break;
    case WH_CONNECTMODE_DS_PARENT:
        if (func_0207dce8(&s_dsInfo) != WM_ERRCODE_SUCCESS) {
            unkfunc_02122840();
            break;
        }
    case WH_CONNECTMODE_MP_PARENT:
        if (!unkfunc_021215b8()) {
            unkfunc_02122840();
        }
        break;
    }
}

ARM BOOL unkfunc_02122998()
{
    unkfunc_021210f0(WH_SYSSTATE_BUSY);
    if (func_0207d228(unkfunc_02121ee8) != WM_ERRCODE_OPERATING) {
        unkfunc_021210f0(WH_SYSSTATE_ERROR);
        return FALSE;
    }
    s_initialized = FALSE;
    return TRUE;
}

ARM void unkfunc_021229e0(void (*callback)(void* arg))
{
    s_indCallback = callback;
}

ARM int unkfunc_021229f0()
{
    return s_beaconCount;
}

ARM void unkfunc_02122a00(int value)
{
    s_forceError = value;
}
