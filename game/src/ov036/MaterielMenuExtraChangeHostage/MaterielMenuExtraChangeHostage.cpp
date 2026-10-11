#include "ov036/MaterielMenuExtraChangeHostage/MaterielMenuExtraChangeHostage.hpp"
#include "main/object/DisplayCharacter.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/cmn/GameManager.hpp"
#include "main/cmn/PlayerManager.hpp"
#include "main/global/Global.hpp"
#include "main/cmn/ExtraMapLink.hpp"
#include "ov016/MenuTemplate/MenuTemplate_materiel.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_0216fb14.hpp"

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
        unkfunc_0216fdb8();
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
        if (data_020ed1bc.isOpen() != 0) {
            if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_OK) {
                data_020ed1bc.close();
                data_020ed1bc.openMessageForTALK();
                data_020ed1bc.addMessageNOWAIT(0x1A049);
                data_020ed1bc.addMessageWAITKEY();
                hostageStatus_ = HOSTAGE_SELECT;
                break;
            }
            if (data_020ed1bc.stat_ == menu::MenuBase::MENUBASE_STAT_CANCEL) {
                hostageStatus_ = HOSTAGE_END;
                data_020ed1bc.close();
            }
        } else {
            data_020ed1bc.openMessageForTALK();
            data_020ed1bc.addMessage(0x1A048);
            data_020ed1bc.setYesNo();
        }
        break;

    case HOSTAGE_SELECT:
        if (data_020ed1bc.isOpen() != 0
         && (unsigned int)(data_020ed1bc.stat_ - 1) <= 1) {
            data_020ed1bc.close();
        }
        memberUpdate();
        break;

    case HOSTAGE_CHANGING:
        if (data_020ed1bc.isOpen() != 0) {
            if ((unsigned int)(data_020ed1bc.stat_ - 1) <= 1) {
                data_020ed1bc.close();
                g_Global.fadeOutBlack(0x3C);
            }
        } else {
            memberChange();
        }
        break;

    case HOSTAGE_END:
        if (data_020ed1bc.isOpen() != 0) {
            if ((unsigned int)(data_020ed1bc.stat_ - 1) <= 1) {
                close();
                data_020ed1bc.close();

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
            data_020ed1bc.openMessageForTALK();
            data_020ed1bc.addMessage(0x19E42);
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
                data_020ed1bc.openMessageForTALK();
                TextAPI::setMACRO0(0x12, 0x50000000, newHostageID_);
                data_020ed1bc.addMessage(0x1A051);
                TextAPI::setMACRO0(0x10, 0x50000000, hostageID_);
                data_020ed1bc.addMessage(0x1A052);
                hostageStatus_ = HOSTAGE_CHANGING;
                return;
            }

            data_020ed1bc.openMessageForTALK();
            TextAPI::setMACRO0(0x12, 0x50000000, newHostageID_);
            data_020ed1bc.addMessage(0x1A04D);
            data_020ed1bc.addMessage(0x1A048);
            data_020ed1bc.setYesNo();
            hostageStatus_ = HOSTAGE_ISCHANGE;
            return;
        } else if (r == 3) {
            data_020ed1bc.close();
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