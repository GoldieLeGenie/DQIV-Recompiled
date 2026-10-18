#include "ov036/MaterielMenuExtraChangeHostage/MaterielMenuExtraChangeHostage.hpp"
#include "main/object/DisplayCharacter.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/cmn/GameManager.hpp"
#include "main/cmn/PlayerManager.hpp"
#include "main/global/Global.hpp"
#include "main/cmn/ExtraMapLink.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/MaterielMenuScreens.hpp"

THUMB void MaterielMenuExtraChangeHostage::menuSetup()
{
    status::g_Party.setBattleMode();

    newHostageID_ = 0;
    for (int i = 0; i < 0x1A; i++) {
        if (status::g_Party.isHostage(i) != 0) {
            hostageID_ = i;
        }
    }

    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);

    menuItem_.active_ = 0;
    hostageStatus_ = HOSTAGE_ISCHANGE;

    if (g_Global.bookingFlag_ == Global::BOOKING_HOSTAGE) {
        hostageStatus_ = HOSTAGE_END;
    } else {
        ctrlID_ = getPlacementCtrlId();
    }
}


THUMB void MaterielMenuExtraChangeHostage::menuExecute()
{
    if (hostageStatus_ == HOSTAGE_SELECT) {
        status::g_Party.setNormalMode();
        int active = menuItem_.active_;
        MenuTemplate_materiel::MATERIEL_ICON32_5x2_CHURCH(&menuItem_, active, status::g_Party.getCount());
    }
}

THUMB void MaterielMenuExtraChangeHostage::menuDraw()
{
    if (hostageStatus_ == HOSTAGE_SELECT) {
        drawHostageScreen();
        menuItem_.drawActive();
    }
}


THUMB void MaterielMenuExtraChangeHostage::menuUpdate()
{
    if (g_GlobalFade.isFadeEnd() == 0) {
        return;
    }

    switch (hostageStatus_) {
    case HOSTAGE_ISCHANGE:
        if (g_TownMenuMessage.isOpen() != 0) {
            if (g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
                g_TownMenuMessage.close();
                g_TownMenuMessage.openMessageForTALK();
                g_TownMenuMessage.addMessageNOWAIT(0x1A049);
                g_TownMenuMessage.addMessageWAITKEY();
                hostageStatus_ = HOSTAGE_SELECT;
                break;
            }
            if (g_TownMenuMessage.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
                hostageStatus_ = HOSTAGE_END;
                g_TownMenuMessage.close();
            }
        } else {
            g_TownMenuMessage.openMessageForTALK();
            g_TownMenuMessage.addMessage(0x1A048);
            g_TownMenuMessage.setYesNo();
        }
        break;

    case HOSTAGE_SELECT:
        if (g_TownMenuMessage.isOpen() != 0
         && (unsigned int)(g_TownMenuMessage.stat_ - 1) <= 1) {
            g_TownMenuMessage.close();
        }
        memberUpdate();
        break;

    case HOSTAGE_CHANGING:
        if (g_TownMenuMessage.isOpen() != 0) {
            if ((unsigned int)(g_TownMenuMessage.stat_ - 1) <= 1) {
                g_TownMenuMessage.close();
                g_Global.fadeOutBlack(0x3C);
            }
        } else {
            memberChange();
        }
        break;

    case HOSTAGE_END:
        if (g_TownMenuMessage.isOpen() != 0) {
            if ((unsigned int)(g_TownMenuMessage.stat_ - 1) <= 1) {
                close();
                g_TownMenuMessage.close();

                if (g_Global.bookingFlag_ == Global::BOOKING_HOSTAGE) {
                    g_Global.bookingFlag_ = Global::BOOKING_NONE;
                    cmn::GameManager::getSingleton();
                    cmn::PlayerManager::setLock(0);
                    cmn::GameManager::getSingleton()->playerManager_->charaColl_ = 1;
                    BillboardCharacter::setAllCharaAnim(1);
                    TownCharacterManager::getSingleton()->setRotate(ctrlID_, 0x4000);
                }

                MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
            }
        } else {
            g_TownMenuMessage.openMessageForTALK();
            g_TownMenuMessage.addMessage(0x19E42);
        }
        break;
    }
}
THUMB void MaterielMenuExtraChangeHostage::memberUpdate()
{
    navigator_.setup(5, 2, status::g_Party.getCount());

    int r = MenuUpdate_Assist::menuSelect(menuItem_, navigator_);

    if (r != 0) {
        redraw_ = 1;

        if (r == 2) {
           newHostageID_ = status::g_Party.getPlayerStatus(menuItem_.active_)->haveStatusInfo_.haveStatus_.playerIndex_;

            if (isHostage() != 0) {
                status::g_Party.setHostage(hostageID_, false);
                status::g_Party.setHostage(newHostageID_, true);
                g_TownMenuMessage.openMessageForTALK();
                TextAPI::setMACRO0(0x12, 0x50000000, newHostageID_);
                g_TownMenuMessage.addMessage(0x1A051);
                TextAPI::setMACRO0(0x10, 0x50000000, hostageID_);
                g_TownMenuMessage.addMessage(0x1A052);
                hostageStatus_ = HOSTAGE_CHANGING;
                return;
            }

            g_TownMenuMessage.openMessageForTALK();
            TextAPI::setMACRO0(0x12, 0x50000000, newHostageID_);
            g_TownMenuMessage.addMessage(0x1A04D);
            g_TownMenuMessage.addMessage(0x1A048);
            g_TownMenuMessage.setYesNo();
            hostageStatus_ = HOSTAGE_ISCHANGE;
            return;
        } else if (r == 3) {
            g_TownMenuMessage.close();
            hostageStatus_ = HOSTAGE_END;
        }
    }
}

THUMB void MaterielMenuExtraChangeHostage::memberChange()
{
    status::g_Party.setMemberShiftMode();
    status::g_Party.del(newHostageID_);
    status::g_Party.setMemberShiftMode();

    int order[4] = {0, 0, 0, 0};

    for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
        order[i] = status::g_Party.getPlayerIndex(i);
    }

    status::g_Party.add(hostageID_);
    status::g_Party.reorder(order[0], order[1], order[2], order[3]);

    cmn::GameManager::getSingleton()->resetParty();

    hostageStatus_ = HOSTAGE_END;

    cmn::g_extraMapLink.setTownINN();

    cmn::GameManager::getSingleton();             
    cmn::PlayerManager::setLock(1);

    cmn::GameManager::getSingleton()->playerManager_->charaColl_ = 0;
    BillboardCharacter::setAllCharaAnim(0);

    g_Global.bookingFlag_ = Global::BOOKING_HOSTAGE;
    
    MaterielMenu_WINDOW_MANAGER::getSingleton()->closeMaterielWindow();
}

THUMB int MaterielMenuExtraChangeHostage::isHostage()
{
    status::g_Party.setNormalMode();

    if (status::PartyStatus::getPlayerStatusForPlayerIndex(newHostageID_)->haveStatusInfo_.haveStatus_.isPlayer_ == 0) {
        return 0;
    }

    if (newHostageID_ <= 2) {
        return 0;
    }

    int count = status::g_Party.getSortIndex(newHostageID_);
    int ret = 0;

    for (int i = 0; i < count; i++) {
        if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.isPlayer_ != 0
         && status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath() == 0) {
            ret = 1;
            break;
        }
    }

    return ret;
}