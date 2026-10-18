#pragma ipa file
#include "main/part/MPExchangePart.hpp"
#include "main/dss/Display.hpp"
#include "main/cmn/EnvoyManager.hpp"
#include "main/global/Global.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Pad.hpp"
#include "main/dss/BgBuffer.hpp"
#include "main/dss/SleepControl.hpp"
#include "main/dss/VramManager.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/sound/Sound.hpp"
#include "main/status/StageStatus.hpp"
#include "main/text/TextAPI.hpp"
#include "nitro/pad.h"
#include "main/dss/OverlaySlot.hpp"

MPExchangePart g_MPExchangePart;

ARM MPExchangePart::MPExchangePart()
{
}

ARM void MPExchangePart::initialize()
{
    loadOverlay(DQIV_OVERLAY_ID(2));
    ov002_entry();
    g_VramTransfer.initVramManagers(0x40000, 0x4000);
    changeDisplayPlugin(&dss::g_DISPLAYPLUGIN_SINGLE3D);
    lcdCounter_ = 0;
    g_Global.fadeIn(30);
    switch (g_SpriteFade.state_) {
        case GlobalFade::FADE_NONE:
            g_SpriteFade.state_ = GlobalFade::FADE_OUT_BLACK;
            break;
        case GlobalFade::FADE_IN_BLACK:
            g_SpriteFade.state_ = GlobalFade::FADE_OUT_WHITE;
            break;
        default:
            g_SpriteFade.state_ = GlobalFade::FADE_OUT_BLACK;
            break;
    }
    g_SpriteFade.count_ = 0;
    g_SpriteFade.frames_ = 30;
    g_GlobalGamePartManager.unkfunc_02058294(&g_SpriteFade);
    setSleepEnabled(0);
    setState(0);
}

ARM void MPExchangePart::terminate()
{
    setSleepEnabled(1);
    g_VramTransfer.unkfunc_02086278();
    unloadOverlay(DQIV_OVERLAY_ID(2));
}

ARM void MPExchangePart::onExecutePart()
{
    switch (state_) {
        case 0:
            if (counter_ > 30) {
                setState(1);
            }
            break;
        case 1:
            if (counter_ == 0) {
                g_TownMenuMessage.openMessageForTALK();
                g_TownMenuMessage.addMessage(0x92a02);
                g_TownMenuMessage.suspendInput_ = 1;
                message_ = 0;
            }
            if (counter_ > 120) {
                setState(4);
            }
            break;
        case 4: {
            exchange_.start();
            UnkEnvoyData* data = &g_EnvoyManager.myEnvoy_;
            convertUtf8ToSjisWork((char*)data->name);
            convertUtf8ToSjisWork((char*)data->heroName);
            convertUtf8ToSjisWork((char*)data->townName);
            convertUtf8ToSjisWork((char*)data->comment);
            exchange_.setSendData(data);
            setState(2);
            break;
        }
        case 2:
            if (exchange_.mode_ == SYSMODE_FATAL_ERROR) {
                setState(8);
            } else if (exchange_.mode_ == SYSMODE_READY) {
                exchange_.startRoleSelection();
                setState(3);
            }
            break;
        case 3:
            if (message_ == 0) {
                if (counter_ == 0) {
                    g_TownMenuMessage.openMessageForTALK();
                    g_TownMenuMessage.addMessage(0x92a04);
                    message_ = 1;
                }
                if (counter_ == 30) {
                    setState(5);
                }
            } else {
                setState(5);
            }
            break;
        case 5:
            updateLcdPower();
            playWaitSe();
            if (exchange_.mode_ == SYSMODE_FATAL_ERROR) {
                setState(6);
            } else if (exchange_.mode_ == SYSMODE_LIGHT_ERROR) {
                setState(7);
            } else if (exchange_.mode_ == SYSMODE_FINISH_WAIT) {
                setState(9);
            } else if (PAD_DetectFold() == FALSE && (dss::g_Pad.edge() & PAD_BUTTON_A)) {
                setState(10);
            }
            break;
        case 6:
            if (counter_ == 0) {
                g_TownMenuMessage.openMessageForMENU();
                g_TownMenuMessage.addMessage(0);
                message_ = 0;
                exchange_.abort();
            }
            if (counter_ > 60 && g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
                setState(13);
            }
            break;
        case 7:
            setState(4);
            break;
        case 8:
            restoreLcdAndSleep();
            if (counter_ == 0) {
                g_TownMenuMessage.openMessageForMENU();
                g_TownMenuMessage.addMessage(0x92ab8);
                message_ = 0;
                exchange_.abort();
            }
            if (counter_ > 60 && g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
                setState(13);
            }
            break;
        case 9: {
            UnkEnvoyData* data = exchange_.getReceivedData();
            if (exchange_.unk_fc <= 4) {
                setState(7);
                break;
            }
            if (PAD_DetectFold() == FALSE) {
                Sound::sePlay(0x146);
            }
            if (g_EnvoyManager.findEnvoyByUnique(data->unique) == -1) {
                known_ = FALSE;
            } else {
                known_ = TRUE;
            }
            g_EnvoyManager.receiveEnvoy(data, 1);
            convertUtf8ToSjisWork((char*)data->name);
            convertUtf8ToSjisWork((char*)data->heroName);
            convertUtf8ToSjisWork((char*)data->townName);
            convertUtf8ToSjisWork((char*)data->comment);
            setState(12);
            break;
        }
        case 10:
            g_EnvoyManager.receiveEnvoy(NULL, 0);
            setState(11);
            break;
        case 11:
            restoreLcdAndSleep();
            if (counter_ == 0) {
                g_TownMenuMessage.close();
                g_TownMenuMessage.openMessageForTALK();
                g_TownMenuMessage.addMessage(0x92a08);
                g_TownMenuMessage.setYesNo(1);
                message_ = 0;
                exchange_.unk_1c = 1;
            }
            if (counter_ > 60) {
                if (g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
                    g_TownMenuMessage.close();
                    setState(13);
                }
                if (g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
                    g_TownMenuMessage.close();
                    setSleepEnabled(0);
                    setState(4);
                }
            }
            break;
        case 12:
            restoreLcdAndSleep();
            if (counter_ == 0) {
                int message;
                if (known_ != 0) {
                    message = 0x92a99;
                } else if (g_EnvoyManager.getEnvoyCount() == 1) {
                    message = 0x92a11;
                } else {
                    message = 0x92a9a;
                }
                g_TownMenuMessage.close();
                g_TownMenuMessage.openMessageForMENU();
                g_TownMenuMessage.addMessage(message);
                message_ = 0;
            }
            if (counter_ > 60 && g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
                setState(13);
            }
            break;
        case 13:
            restoreLcdAndSleep();
            if (counter_ == 0) {
                g_TownMenuMessage.close();
                exchange_.unk_1c = 1;
                g_Global.fadeOutBlack(30);
            }
            if (counter_ > 30 && exchange_.mode_ == SYSMODE_FINISH_WAIT) {
                g_Global.startTown("sshout");
                g_Stage.flagMapChange_ = 1;
                g_GlobalDQ4.unkfunc_020580fc(TOWN_PART);
                setState(14);
            }
            break;
    }
    exchange_.update();
    updateState();
}

ARM void MPExchangePart::onDebugPart()
{
    exchange_.updateDataSharing();
}

ARM void MPExchangePart::setState(int state)
{
    nextState_ = state;
}

/* not original: without it nextState_ is loaded before state_ (ldmib) */
#pragma push
#pragma opt_common_subs off
ARM void MPExchangePart::updateState()
{
    if (state_ != nextState_) {
        state_ = nextState_;
        counter_ = 0;
    } else {
        counter_++;
    }
}
#pragma pop

ARM void MPExchangePart::updateLcdPower()
{
    if (PAD_DetectFold()) {
        if (func_0207c0d8() == TRUE) {
            func_0207c0b8(FALSE);
            lcdCounter_ = 0;
        }
    }
    if (PAD_DetectFold() == FALSE && lcdCounter_ > 10 && func_0207c0d8() == FALSE) {
        func_0207c0b8(TRUE);
        lcdCounter_ = 0;
    }
    lcdCounter_++;
}

ARM void MPExchangePart::playWaitSe()
{
    if (counter_ % 60 == 0) {
        Sound::sePlay(0x12f);
    }
}

ARM void MPExchangePart::restoreLcdAndSleep()
{
    if (func_0207c0d8() == FALSE) {
        func_0207c0b8(TRUE);
    }
    setSleepEnabled(1);
}

ARM void MPExchangePart::onWindowPart()
{
}

ARM void MPExchangePart::onDrawPart()
{
}
