#pragma ipa file
#include "main/menu/UnkMaterielMenu_0203d7e4.hpp"
#include "main/menu/MenuUpdateAssist.hpp"
#include "main/cmn/ExtraMapLink.hpp"
#include "main/data/ExcelBinaryData.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/global/Global.hpp"
#include "main/param/Event.hpp"
#include "main/status/BattleHistory.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/Status.hpp"
#include "main/status/StoryStatus.hpp"
#include "ov016/UnkMaterielMenuDraw/UnkMaterielMenuDraw_02177bac.hpp"

static short s_angleX = -8556;

static MENUITEM_DATA s_menuItemData[] = {
    {1, 2, 0x12, 0x5c, 0x28, 0x08},
    {1, 2, 0x12, 0x76, 0x28, 0x08},
    {-1, -1, 0, 0, 0, 0},
};

THUMB void UnkMaterielMenu_0203d7e4::menuSetup()
{
    status::g_Party.setPlayerMode();
    menuItem_.setup(menu::MenuItem::MENUITEM_TYPE_TOUCH_PAD_CANCEL, menu::MenuItem::CURSORTYPE_ACTIVE);
    menuItem_.active_ = 0;
    cursor_.setupBase();
}

THUMB void UnkMaterielMenu_0203d7e4::menuExecute()
{
    unkfunc_0203d9c0(&menuItem_, menuItem_.active_);
}

THUMB void UnkMaterielMenu_0203d7e4::menuDraw()
{
    int message[] = { (int)"@\x82\xda\x82\xa4\x82\xaf\x82\xf1\x82\xf0\x82\xb7\x82\xe9" };  // @ぼうけんをする
    char name[] = "@\x83\x5c\x83\x8d";  // @ソロ
    unkfunc_02177fe0(message, 8, 0x38, 0xf0, 0x50);
    unkfunc_0217800c(0, name, 0, 1, 0, 0, 0x5a, 1);
    unkfunc_0217800c(1, name, 5, 15, 0, 0x1e3660, 0x5a, 1);
    unkfunc_02177c00(8, 0x38, 0xf0, 0x60, 0x50);
    menuItem_.drawActive();
}

THUMB void UnkMaterielMenu_0203d7e4::menuUpdate()
{
    cursor_.setup(1, 2, 2);
    int result = MenuUpdate_Assist::menuSelect(menuItem_, cursor_);
    if (result == 0) {
        return;
    }
    if (result == 2) {
        char name[10];
        dss::memset(name, 0, sizeof(name));
        close();
        status::g_Story.sex_ = SEX_MALE;
        g_Stage.setEncount(0);
        if (menuItem_.active_ == 0) {
            status::Status::setFlagShopIndex(0);
            status::Status::setFlagShopExec();
            dss::strcpy_s(name, sizeof(name), param::Event::getFileData(0)->floor);
            g_Global.startTown(name);
        } else {
            dss::Fix32Vector3 pos;
            pos.set(-10.0f, 0.0f, -0.5f);
            status::Status::setFlagShopIndex(0x9e);
            status::Status::setFlagShopExec();
            dss::strcpy_s(name, sizeof(name), param::Event::getFileData(0x9e)->floor);
            unkfunc_0203d9dc(0x9d);
            unkfunc_0203d9dc(0x93);
            cmn::g_extraMapLink.setExtraLinkTown(name, pos, 0x4000);
            status::g_BattleHistory.historyType_ = status::BattleHistory::RightNow;
            status::g_BattleHistory.setAdventureTime(0x1e3660);
        }
        ExcelBinaryData::clearData(&param::Event::data_);
        g_Stage.pushCameraAngle(dss::Vector3<short>(s_angleX, 0, 0));
    } else {
        redraw_ = 1;
    }
}

THUMB void UnkMaterielMenu_0203d7e4::unkfunc_0203d9c0(menu::MenuItem* item, int active)
{
    item->setMenuItem(s_menuItemData, 1, 2, 2);
    item->active_ = active;
}

THUMB void UnkMaterielMenu_0203d7e4::unkfunc_0203d9dc(int item)
{
    int count = status::g_Party.haveItemSack_.getCount();
    for (int i = 0; i < count; i++) {
        if (item == status::g_Party.haveItemSack_.getItem(i)) {
            status::g_Party.haveItemSack_.del(i);
            return;
        }
    }
}
