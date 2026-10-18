#pragma ipa file
#include "main/part/MPExchangePart.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/cmn/EnvoyManager.hpp"
#include "main/global/Global.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/dss/Pad.hpp"
#include "main/dss/UnkBgBuffer.hpp"
#include "main/dss/UnkSleep.hpp"
#include "main/dss/UnkVramTransfer.hpp"
#include "main/menu/MenuManager.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/sound/Sound.hpp"
#include "main/status/StageStatus.hpp"
#include "main/text/TextAPI.hpp"
#include "nitro/pad.h"
#include "main/dss/UnkOverlaySlot.hpp"

MPExchangePart g_MPExchangePart;

ARM MPExchangePart::MPExchangePart()
{
}

ARM void MPExchangePart::initialize()
{
    unkfunc_02087590(DQIV_OVERLAY_ID(2));
    ov002_entry();
    data_0211e450.unkfunc_020861c4(0x40000, 0x4000);
    unkfunc_02080e90(&dss::g_DISPLAYPLUGIN_SINGLE3D);
    lcdCounter_ = 0;
    g_Global.fadeIn(30);
    switch (data_020f21f8.state_) {
        case GlobalFade::FADE_NONE:
            data_020f21f8.state_ = GlobalFade::FADE_OUT_BLACK;
            break;
        case GlobalFade::FADE_IN_BLACK:
            data_020f21f8.state_ = GlobalFade::FADE_OUT_WHITE;
            break;
        default:
            data_020f21f8.state_ = GlobalFade::FADE_OUT_BLACK;
            break;
    }
    data_020f21f8.count_ = 0;
    data_020f21f8.frames_ = 30;
    data_0210bc18.unkfunc_02058294(&data_020f21f8);
    unkfunc_02089558(0);
    unkfunc_0203a228(0);
}

ARM void MPExchangePart::terminate()
{
    unkfunc_02089558(1);
    data_0211e450.unkfunc_02086278();
    unkfunc_020875a4(DQIV_OVERLAY_ID(2));
}

ARM void MPExchangePart::onExecutePart()
{
    switch (state_) {
        case 0:
            if (counter_ > 30) {
                unkfunc_0203a228(1);
            }
            break;
        case 1:
            if (counter_ == 0) {
                data_020ed1bc.openMessageForTALK();
                data_020ed1bc.addMessage(0x92a02);
                data_020ed1bc.suspendInput_ = 1;
                message_ = 0;
            }
            if (counter_ > 120) {
                unkfunc_0203a228(4);
            }
            break;
        case 4: {
            exchange_.unkfunc_0212338c();
            UnkEnvoyData* data = &data_020f0078.myEnvoy_;
            unkfunc_02088078((char*)data->name);
            unkfunc_02088078((char*)data->heroName);
            unkfunc_02088078((char*)data->townName);
            unkfunc_02088078((char*)data->comment);
            exchange_.unkfunc_02123334(data);
            unkfunc_0203a228(2);
            break;
        }
        case 2:
            if (exchange_.mode_ == SYSMODE_FATAL_ERROR) {
                unkfunc_0203a228(8);
            } else if (exchange_.mode_ == SYSMODE_READY) {
                exchange_.unkfunc_02123478();
                unkfunc_0203a228(3);
            }
            break;
        case 3:
            if (message_ == 0) {
                if (counter_ == 0) {
                    data_020ed1bc.openMessageForTALK();
                    data_020ed1bc.addMessage(0x92a04);
                    message_ = 1;
                }
                if (counter_ == 30) {
                    unkfunc_0203a228(5);
                }
            } else {
                unkfunc_0203a228(5);
            }
            break;
        case 5:
            unkfunc_0203a254();
            unkfunc_0203a2dc();
            if (exchange_.mode_ == SYSMODE_FATAL_ERROR) {
                unkfunc_0203a228(6);
            } else if (exchange_.mode_ == SYSMODE_LIGHT_ERROR) {
                unkfunc_0203a228(7);
            } else if (exchange_.mode_ == SYSMODE_FINISH_WAIT) {
                unkfunc_0203a228(9);
            } else if (PAD_DetectFold() == FALSE && (dss::g_Pad.edge() & PAD_BUTTON_A)) {
                unkfunc_0203a228(10);
            }
            break;
        case 6:
            if (counter_ == 0) {
                data_020ed1bc.openMessageForMENU();
                data_020ed1bc.addMessage(0);
                message_ = 0;
                exchange_.unkfunc_02123d68();
            }
            if (counter_ > 60 && data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
                unkfunc_0203a228(13);
            }
            break;
        case 7:
            unkfunc_0203a228(4);
            break;
        case 8:
            unkfunc_0203a318();
            if (counter_ == 0) {
                data_020ed1bc.openMessageForMENU();
                data_020ed1bc.addMessage(0x92ab8);
                message_ = 0;
                exchange_.unkfunc_02123d68();
            }
            if (counter_ > 60 && data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
                unkfunc_0203a228(13);
            }
            break;
        case 9: {
            UnkEnvoyData* data = exchange_.unkfunc_02123378();
            if (exchange_.unk_fc <= 4) {
                unkfunc_0203a228(7);
                break;
            }
            if (PAD_DetectFold() == FALSE) {
                Sound::sePlay(0x146);
            }
            if (data_020f0078.findEnvoyByUnique(data->unique) == -1) {
                known_ = FALSE;
            } else {
                known_ = TRUE;
            }
            data_020f0078.receiveEnvoy(data, 1);
            unkfunc_02088078((char*)data->name);
            unkfunc_02088078((char*)data->heroName);
            unkfunc_02088078((char*)data->townName);
            unkfunc_02088078((char*)data->comment);
            unkfunc_0203a228(12);
            break;
        }
        case 10:
            data_020f0078.receiveEnvoy(NULL, 0);
            unkfunc_0203a228(11);
            break;
        case 11:
            unkfunc_0203a318();
            if (counter_ == 0) {
                data_020ed1bc.close();
                data_020ed1bc.openMessageForTALK();
                data_020ed1bc.addMessage(0x92a08);
                data_020ed1bc.setYesNo(1);
                message_ = 0;
                exchange_.unk_1c = 1;
            }
            if (counter_ > 60) {
                if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
                    data_020ed1bc.close();
                    unkfunc_0203a228(13);
                }
                if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
                    data_020ed1bc.close();
                    unkfunc_02089558(0);
                    unkfunc_0203a228(4);
                }
            }
            break;
        case 12:
            unkfunc_0203a318();
            if (counter_ == 0) {
                int message;
                if (known_ != 0) {
                    message = 0x92a99;
                } else if (data_020f0078.getEnvoyCount() == 1) {
                    message = 0x92a11;
                } else {
                    message = 0x92a9a;
                }
                data_020ed1bc.close();
                data_020ed1bc.openMessageForMENU();
                data_020ed1bc.addMessage(message);
                message_ = 0;
            }
            if (counter_ > 60 && data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
                unkfunc_0203a228(13);
            }
            break;
        case 13:
            unkfunc_0203a318();
            if (counter_ == 0) {
                data_020ed1bc.close();
                exchange_.unk_1c = 1;
                g_Global.fadeOutBlack(30);
            }
            if (counter_ > 30 && exchange_.mode_ == SYSMODE_FINISH_WAIT) {
                g_Global.startTown("sshout");
                g_Stage.flagMapChange_ = 1;
                data_0210bb94.unkfunc_020580fc(TOWN_PART);
                unkfunc_0203a228(14);
            }
            break;
    }
    exchange_.unkfunc_02123498();
    unkfunc_0203a230();
}

ARM void MPExchangePart::onDebugPart()
{
    exchange_.unkfunc_02123724();
}

ARM void MPExchangePart::unkfunc_0203a228(int state)
{
    nextState_ = state;
}

/* not original: without it nextState_ is loaded before state_ (ldmib) */
#pragma push
#pragma opt_common_subs off
ARM void MPExchangePart::unkfunc_0203a230()
{
    if (state_ != nextState_) {
        state_ = nextState_;
        counter_ = 0;
    } else {
        counter_++;
    }
}
#pragma pop

ARM void MPExchangePart::unkfunc_0203a254()
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

ARM void MPExchangePart::unkfunc_0203a2dc()
{
    if (counter_ % 60 == 0) {
        Sound::sePlay(0x12f);
    }
}

ARM void MPExchangePart::unkfunc_0203a318()
{
    if (func_0207c0d8() == FALSE) {
        func_0207c0b8(TRUE);
    }
    unkfunc_02089558(1);
}

ARM void MPExchangePart::onWindowPart()
{
}

ARM void MPExchangePart::onDrawPart()
{
}
