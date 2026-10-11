#include "main/window/NormalControl.hpp"
#include "main/dss/Pad.hpp"
#include "main/global/Global.hpp"
#include "main/global/GlobalDQ4.hpp"
#include "main/encount/Encount.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/cmn/CommonCounterInfo.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov001/fld/FieldPlayerManager.hpp"
#ifdef DQIV_NATIVE
#include "Settings.hpp"
#endif

ARM void window::NormalControl::setup()
{
    state_ = CONTROL_NORMAL;
}

ARM void window::NormalControl::execute()
{
    if (g_Global.partChangeFlag_) {
        return;
    }
    if (encount::Encount::getSingleton()->isEncounted()) {
        return;
    }
    if (isPlayerLock()) {
        return;
    }
    if (state_ == CONTROL_NPC_MESSAGE) {
        if (data_020ed1bc.isOpen()) {
            if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK || data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
                data_020ed1bc.close();
                openMenu();
                state_ = CONTROL_NORMAL;
            }
        }
        return;
    }
#ifdef DQIV_NATIVE
    // The party chat control talks to the party at once (the phase the
    // Japanese version used; the European one only had the Talk command).
    // Only while walking since the frame before: the B that closed a menu
    // does not count.
    static unsigned s_lastFrame = 0;
    const unsigned frame = pcVBlankCount();
    const bool walking = frame - s_lastFrame <= 1;
    s_lastFrame = frame;
    if (pc::takePartyChatRequest() && walking) {
        if (permit_->check(PHASE_PARTY_TALK) && g_GlobalFade.isFadeEnd()) {
            goNext(PHASE_PARTY_TALK);
        }
        return;
    }
#endif
    if (dss::g_Pad.edge() & 0x800) {
        if (permit_->check(PHASE_MAP)) {
            openMap();
        } else if (permit_->check(PHASE_SHOPLIST)) {
            openShopList();
        }
        return;
    }
    if (dss::g_Pad.edge() & 0x400) {
        openMenu();
        return;
    }
    dss::g_Pad.edge();
    checkCamera();
    executePlayer();
}

ARM void window::NormalControl::executePlayer()
{
    if (data_0210bb94.unkfunc_02058114(0xe)) {
        FieldPlayerManager::getSingleton()->inputClear();
        if (NPCPulling()) {
            return;
        }
        if (dss::g_Pad.pad() & 0xf0) {
            FieldPlayerManager::getSingleton()->inputPad(dss::g_Pad.padDir());
        }
    }
    if (!data_0210bb94.unkfunc_02058114(0xc)) {
        return;
    }
    TownPlayerManager::getSingleton()->inputClear();
    if (NPCPulling()) {
        return;
    }
    if (dss::g_Pad.pad() & 0xf0) {
        TownPlayerManager::getSingleton()->inputPad(dss::g_Pad.padDir());
    }
}

ARM void window::NormalControl::checkCamera()
{
    if (!data_0210bb94.unkfunc_02058114(0xc)) {
        return;
    }
    switch ((dss::g_Pad.pad() & 0x100) | (dss::g_Pad.pad() & 0x200)) {
        case 0x300:
            TownPlayerManager::getSingleton()->setCameraRotToNorth();
            break;
        case 0x100:
            TownPlayerManager::getSingleton()->setCameraRotType((CAMERA_ROT_TYPE)1);
            break;
        case 0x200:
            TownPlayerManager::getSingleton()->setCameraRotType((CAMERA_ROT_TYPE)2);
            break;
        default:
            TownPlayerManager::getSingleton()->setCameraRotType((CAMERA_ROT_TYPE)0);
            break;
    }
}

ARM void window::NormalControl::openMap()
{
    if (g_GlobalFade.isFadeEnd()) {
        goNext(PHASE_MAP);
    }
}

ARM void window::NormalControl::openShopList()
{
    if (g_GlobalFade.isFadeEnd()) {
        goNext(PHASE_SHOPLIST);
    }
}

ARM void window::NormalControl::openMenu()
{
    if (g_GlobalFade.isFadeEnd()) {
        goNext(PHASE_MENU);
    }
}

ARM bool window::NormalControl::isNPCParty()
{
    status::g_Party.setNormalMode();
    int count = status::g_Party.getCarriageOutCount();
    bool ret = true;
    for (int i = 0; i < count; i++) {
        status::PlayerStatus* player = status::g_Party.getPlayerStatus(i);
        if (player->haveStatusInfo_.haveStatus_.isPlayer_ && !player->haveStatusInfo_.isDeath()) {
            ret = false;
        }
    }
    return ret;
}

ARM bool window::NormalControl::isDeadParty()
{
    status::g_Party.setNormalMode();
    int count = status::g_Party.getCarriageOutCount();
    bool ret = true;
    for (int i = 0; i < count; i++) {
        if (!status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath()) {
            ret = false;
        }
    }
    return ret;
}

ARM bool window::NormalControl::NPCPulling()
{
    if (isDeadParty() == true) {
        if (data_0211a5d4.touch_ != 0 || (dss::g_Pad.pad() & 0xf0)) {
            data_020ed1bc.openMessageForMENU();
            data_020ed1bc.addMessage(0xc3de0);
            data_020ed1bc.addMessage(0xc3de1);
            state_ = CONTROL_NPC_MESSAGE;
            return true;
        }
    } else if (isNPCParty() == true) {
        if (data_0211a5d4.touch_ != 0 || (dss::g_Pad.pad() & 0xf0)) {
            data_020ed1bc.openMessageForMENU();
            data_020ed1bc.addMessage(0xc3da0);
            data_020ed1bc.addMessage(0xc3da1);
            state_ = CONTROL_NPC_MESSAGE;
            return true;
        }
    } else {
        return false;
    }
    return false;
}

ARM int window::NormalControl::getPhase()
{
    return PHASE_NORMAL;
}
