#pragma ipa file
#include "main/window/CommandWindow.hpp"
#include "main/dss/UnkDisplay.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/menu/MenuAPI.hpp"
#include "main/status/StageStatus.hpp"
#include "main/cmn/CommonCounterInfo.hpp"
#include "nitro/g3.hpp"

static int unk_020bea5c = -1;
static UnkG2dImageAttr unk_020bea60 = {5, 5, 7, 0, 0, 0x100010};
static char s_field[] = "field";

ARM window::CommandWindow::CommandWindow()
{
}

ARM window::CommandWindow::~CommandWindow()
{
}

ARM void window::CommandWindow::initialize()
{
    setPermit();
    setIcon();
    phase_->initialize(&permit_, &icon_);
    phase_->setupIcon();
    map_.initialize();
    phase_ = &normal_;
    changePhase_ = 0;
}

ARM void window::CommandWindow::terminate()
{
    MenuAPI::clearMenuAll();
}

ARM void window::CommandWindow::changeNextPhase(int phase)
{
    if (changePhase_ == 1) {
        changePhase_ = 0;
        phase_->playerLock(false);
    }
    switch (phase) {
        case PHASE_NORMAL:
            phase_ = &normal_;
            break;
        case PHASE_MENU:
            phase_ = &menu_;
            break;
        case PHASE_MESSAGE:
            phase_ = &message_;
            break;
        case PHASE_MAP:
            phase_ = &map_;
            break;
        case PHASE_SHOPLIST:
            phase_ = &list_;
            break;
        case PHASE_SHOPMENU:
            phase_ = &shop_;
            break;
        case PHASE_EVENT:
            phase_ = &event_;
            break;
        case PHASE_PARTY_TALK:
            phase_ = &talk_;
            break;
    }
    phase_->setup();
}

ARM void window::CommandWindow::setPermit()
{
    permit_.clear();
    permit_.flag_ |= (PHASE_NORMAL | PHASE_SHOPMENU | PHASE_MENU | PHASE_MESSAGE | PHASE_EVENT | PHASE_PARTY_TALK);
    if (g_Stage.isMapIcon()) {
        permit_.flag_ |= PHASE_MAP;
    }
    if (g_Stage.isShopIcon()) {
        permit_.flag_ |= PHASE_SHOPLIST;
    }
}

ARM void window::CommandWindow::setIcon()
{
    icon_.flag_ = 0;
    if (dss::strcmp(g_Stage.getMapName(), s_field) == 0) {
        icon_.flag_ |= 2;
        icon_.flag_ |= 8;
        return;
    }
    if (g_Stage.isCameraIcon()) {
        icon_.flag_ |= 1;
    }
    if (g_Stage.isMapIcon()) {
        icon_.flag_ |= 2;
    }
    if (g_Stage.isShopIcon()) {
        icon_.flag_ |= 4;
    }
    icon_.flag_ |= 8;
}

ARM void window::CommandWindow::setMenuPermit(bool flag)
{
    if (flag) {
        permit_.flag_ |= PHASE_MENU;
        icon_.flag_ |= 8;
    } else {
        permit_.flag_ &= ~PHASE_MENU;
        icon_.flag_ &= ~8;
    }
    phase_->setupIcon();
}

ARM void window::CommandWindow::setShoplistPermit(bool flag)
{
    if (flag) {
        permit_.flag_ |= PHASE_SHOPLIST;
        icon_.flag_ |= 4;
    } else {
        permit_.flag_ &= ~PHASE_SHOPLIST;
        icon_.flag_ &= ~4;
    }
    phase_->setupIcon();
}

ARM void window::CommandWindow::changeShopMenuPhase(int type)
{
    shop_.menuType_ = type;
    phase_->goNext(PHASE_SHOPMENU);
}

ARM void window::CommandWindow::changeNormalPhase()
{
    phase_->goNext(PHASE_NORMAL);
}

ARM void window::CommandWindow::menuRefresh()
{
    if (phase_->getPhase() == PHASE_MENU) {
        menu_.regist_ = PHASE_MENU;
    }
}

ARM bool window::CommandWindow::isShopMenu()
{
    return phase_->getPhase() == PHASE_SHOPMENU;
}

ARM bool window::CommandWindow::isMessage()
{
    return phase_->getPhase() == PHASE_MESSAGE;
}

ARM bool window::CommandWindow::unkfunc_0202a8fc()
{
    if (data_0211a5d4.touch_ != 0) {
        int x = data_0211a5d4.x_;
        int y = data_0211a5d4.y_;
        if (x < 0x20 && y < 0x20) {
            unkfunc_020817d8();
            unk_020bea5c = 0;
            return true;
        }
        if (x > 0xe0 && y < 0x20) {
            unk_020bea5c = -1;
        }
    }
    if (unk_020bea5c == -1) {
        return false;
    }
    if (!unkfunc_0208198c()) {
        return false;
    }
    unkfunc_02068ec8(0x6b);
    UnkG2dSprite sprite;
    sprite.posX = 0;
    sprite.posY = 0;
    sprite.sizeX = 0x40;
    sprite.sizeY = 0x60;
    sprite.rotZ = 0;
    sprite.priority = 0;
    sprite.alpha = 0x1f;
    sprite.attr = &unk_020bea60;
    sprite.texAddr = 0;
    sprite.plttAddr = 0;
    sprite.unk_18 = 0;
    sprite.color = 0x1f;
    sprite.uvULx = 0;
    sprite.uvULy = 0;
    sprite.uvLRx = 0x100000;
    sprite.uvLRy = 0xc0000;
    sprite.unk_2c = 0;
    sprite.unk_30 = 0;
    sprite.unk_34 = 0;
    sprite.unk_36 = 0;
    sprite.unk_38 = 0;
    sprite.unk_3a = 0;
    G3_PushMtx();
    unkfunc_02068eec(&sprite);
    G3_PopMtx(1);
    UnkG2dSprite sprite2;
    sprite2.posX = 0;
    sprite2.posY = 0x60;
    sprite2.sizeX = 0x40;
    sprite2.sizeY = 0x60;
    sprite2.rotZ = 0;
    sprite2.priority = 0;
    sprite2.alpha = 0x1f;
    sprite2.attr = &unk_020bea60;
    sprite2.texAddr = 0x20000;
    sprite2.plttAddr = 0;
    sprite2.unk_18 = 0;
    sprite2.color = 0x1f;
    sprite2.uvULx = 0;
    sprite2.uvULy = 0;
    sprite2.uvLRx = 0x100000;
    sprite2.uvLRy = 0xc0000;
    sprite2.unk_2c = 0;
    sprite2.unk_30 = 0;
    sprite2.unk_34 = 0;
    sprite2.unk_36 = 0;
    sprite2.unk_38 = 0;
    sprite2.unk_3a = 0;
    G3_PushMtx();
    unkfunc_02068eec(&sprite2);
    G3_PopMtx(1);
    return false;
}

ARM void window::CommandWindow::execute()
{
    if (unkfunc_0202a8fc()) {
        return;
    }
    if (phase_->isNext()) {
        changeNextPhase(InputControl::next_);
    }
    phase_->execute();
    if (phase_->isNext()) {
        phase_->playerLock(true);
        changePhase_ = 1;
    }
}

ARM void window::CommandWindow::registImageMap(ImageMap* imageMap)
{
    map_.registImageMap(imageMap);
}

ARM void window::CommandWindow::registShopList(ImageMap* shoplist)
{
    list_.registShopList(shoplist);
}

ARM void window::CommandWindow::MESSAGEWINDOW(int index, int count)
{
    if (phase_->goNext(PHASE_MESSAGE)) {
        MenuAPI::openMessage(index, count);
        changeNextPhase(PHASE_MESSAGE);
    }
}

ARM void window::CommandWindow::MESSAGECOMMONWINDOW()
{
    if (phase_->goNext(PHASE_MESSAGE)) {
        MenuAPI::openCommonMessage();
        changeNextPhase(PHASE_MESSAGE);
    }
}

ARM void window::CommandWindow::ADDCOMMONWINDOW(int index)
{
    MenuAPI::addCommonMessage(index);
}

ARM void window::CommandWindow::WAITCOMMONWINDOW()
{
    MenuAPI::waitCommonMessage();
}

ARM void window::CommandWindow::CLEARCOMMONWINDOW()
{
    MenuAPI::clearCommonMessage();
}

ARM void window::CommandWindow::SERIALCOMMONWINDOW(int index)
{
    MenuAPI::addMessageSerial(index);
}

ARM bool window::CommandWindow::isWAITWINDOW()
{
    return MenuAPI::isWaitMessage();
}

ARM bool window::CommandWindow::isOPENWINDOW()
{
    return phase_->getPhase() != PHASE_NORMAL;
}

ARM void window::EventControl::setup()
{
}

ARM void window::EventControl::execute()
{
}

ARM int window::EventControl::getPhase()
{
    return PHASE_EVENT;
}
