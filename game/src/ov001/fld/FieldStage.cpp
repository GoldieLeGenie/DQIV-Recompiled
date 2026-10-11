#pragma ipa file
#include "ov001/fld/FieldStage.hpp"
#include "ov001/fld/FieldPlayerManager.hpp"
#include "ov001/fld/FieldRectCollManager.hpp"
#include "ov001/window/FieldWindowSystem.hpp"
#include "main/cmn/CommonCounterInfo.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/cmn/ExtraMapLink.hpp"
#include "main/cmn/WorldLocation.hpp"
#include "main/encount/Encount.hpp"
#include "main/global/Global.hpp"
#include "main/menu/UiMsg.hpp"
#include "main/object/SpriteCharacter.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/GameFlag.hpp"
#include "main/status/StageStatus.hpp"
#include "main/text/TextAPI.hpp"

ARM fld::FieldStage::FieldStage()
{
}

ARM fld::FieldStage::~FieldStage()
{
}

ARM fld::FieldStage* fld::FieldStage::getSingleton()
{
    static FieldStage fieldStage;
    return &fieldStage;
}

ARM void fld::FieldStage::initialize()
{
    map_ = g_Global.getFieldType();
    SpriteCharacter::unkfunc_0204b704();
    fieldData.setup(map_, change_);
    dss::Fix32Vector3 pos1;
    dss::Fix32Vector3 pos2;
    FieldRectCollManager::getSingleton()->setup();
    int kekai = 1;
    int kekaiNum = 4;
    if (g_AreaFlag.check(0x17b) == true) {
        kekaiNum--;
    }
    if (g_AreaFlag.check(0x17c) == true) {
        kekaiNum--;
    }
    if (g_AreaFlag.check(0x17d) == true) {
        kekaiNum--;
    }
    if (g_AreaFlag.check(0x17f) == true) {
        kekaiNum--;
    }
    g_Stage.maxKekai = kekaiNum;
    if (kekaiNum == 0) {
        kekai = 0;
    }
    if (g_AreaFlag.check(0x180) == true) {
        kekai = 0;
        g_Stage.maxKekai = 0;
    }
    switch (map_) {
    case 2:
        g_Stage.setMapName("darkfield");
        g_Stage.setBtlMapName("darkfield");
        if (kekai == 1) {
            pos1.vx.value = 0x5e6000;
            pos1.vy.value = 0x62c000;
            pos2.vx.value = 0x61a000;
            pos2.vy.value = 0x64a000;
            FieldRectCollManager::getSingleton()->setRectColl(pos1, pos2, RECT_YAMI_BARRIER);
        }
        pos1.vx.value = 0x5c6000;
        pos1.vy.value = 0x58a000;
        pos2.vx.value = 0x63e000;
        pos2.vy.value = 0x60e000;
        FieldRectCollManager::getSingleton()->setRectColl(pos1, pos2, RECT_BALLON);
        break;
    case 1:
        g_Stage.setMapName("gotside");
        g_Stage.setBtlMapName("gotside");
        break;
    default:
        g_Stage.setMapName("field");
        g_Stage.setBtlMapName("field");
        pos1.vx.value = 0xe60000;
        pos1.vy.value = 0x190000;
        pos2.vx.value = 0xe88000;
        pos2.vy.value = 0x1a4000;
        FieldRectCollManager::getSingleton()->setRectColl(pos1, pos2, RECT_UMINARI);
        break;
    }
    g_Stage.setEncount(1);
}

ARM void fld::FieldStage::terminate()
{
    cmn::WorldLocation::setCurrentTimeZone();
    fieldData.cleanup();
    SpriteCharacter::unkfunc_0204b744();
}

ARM void fld::FieldStage::execute()
{
    if (encount::Encount::getSingleton()->disableFlag_ != 0) {
        encount::Encount::getSingleton()->disableFlag_ = 0;
        switch (encount::Encount::getSingleton()->disableAction_) {
        case 0xa3:
            ui_MsgSndSet(0x30);
            TextAPI::setMACRO0(10, 0x40000000, 0x71);
            FieldWindowSystem::getSingleton()->openMessage(0xc3d90, 1);
            break;
        case 0xcf:
            ui_MsgSndSet(0x30);
            FieldWindowSystem::getSingleton()->openMessage(0xc3d92, 1);
            break;
        case 0xd7:
            ui_MsgSndSet(0x30);
            FieldWindowSystem::getSingleton()->openMessage(0xc3d94, 1);
            break;
        }
    }
    if (encount::Encount::getSingleton()->easyFlag_ != 0) {
        encount::Encount::getSingleton()->easyFlag_ = 0;
        if (encount::Encount::getSingleton()->disableAction_ == 0xa6) {
            ui_MsgSndSet(0x30);
            TextAPI::setMACRO0(10, 0x40000000, 0x74);
            FieldWindowSystem::getSingleton()->openMessage(0xc3d90, 1);
        }
    }
}

ARM void fld::FieldStage::draw()
{
    FieldCarrirerDraw* carrier;
    fieldData.draw();
    switch (FieldPlayerManager::getSingleton()->getMoveType()) {
    case FieldPlayer::MOVE_SHIP_GET_ON:
    case FieldPlayer::MOVE_SHIP_GET_OUT:
    case FieldPlayer::MOVE_SHIP:
        if (g_cmnPartyInfo.isBalloonEnable() == 1) {
            carrier = FieldPlayerManager::getSingleton()->getCarrierPos(FieldCarrirerDraw::CARRIER_BALLOON);
            carrier->draw(calcDrawPosition(carrier->getPosition()));
        }
        break;
    case FieldPlayer::MOVE_BALLOON_GET_ON:
    case FieldPlayer::MOVE_BALLOON:
    case FieldPlayer::MOVE_BALLOON_GET_OUT:
        if (g_cmnPartyInfo.isShipEnable() == 1) {
            carrier = FieldPlayerManager::getSingleton()->getCarrierPos(FieldCarrirerDraw::CARRIER_SHIP);
            carrier->draw(calcDrawPosition(carrier->getPosition()));
        }
        break;
    default:
        FieldPlayerManager::getSingleton()->setCarrierDepth();
        if (g_cmnPartyInfo.isShipEnable() == 1) {
            carrier = FieldPlayerManager::getSingleton()->getCarrierPos(FieldCarrirerDraw::CARRIER_SHIP);
            carrier->draw(calcDrawPosition(carrier->getPosition()));
        }
        if (g_cmnPartyInfo.isBalloonEnable() == 1) {
            carrier = FieldPlayerManager::getSingleton()->getCarrierPos(FieldCarrirerDraw::CARRIER_BALLOON);
            carrier->draw(calcDrawPosition(carrier->getPosition()));
        }
        break;
    }
}

ARM void fld::FieldStage::drawPlayer()
{
    FieldCarrirerDraw* carrier;
    fieldData.setPlayerCamera();
    switch (FieldPlayerManager::getSingleton()->getMoveType()) {
    case FieldPlayer::MOVE_SHIP_GET_ON:
    case FieldPlayer::MOVE_SHIP_GET_OUT:
    case FieldPlayer::MOVE_SHIP:
        carrier = FieldPlayerManager::getSingleton()->getCarrierPos(FieldCarrirerDraw::CARRIER_SHIP);
        carrier->draw(calcDrawPosition(carrier->getPosition()));
        break;
    case FieldPlayer::MOVE_BALLOON_GET_ON:
    case FieldPlayer::MOVE_BALLOON:
    case FieldPlayer::MOVE_BALLOON_GET_OUT:
        carrier = FieldPlayerManager::getSingleton()->getCarrierPos(FieldCarrirerDraw::CARRIER_BALLOON);
        carrier->draw(calcDrawPosition(carrier->getPosition()));
        break;
    }
}

ARM void fld::FieldStage::setOffset(int val)
{
    fieldData.offset_ = val;
}

ARM void fld::FieldStage::setPosition(dss::Fix32Vector3& pos)
{
    fieldData.setPosition(pos);
}

ARM bool fld::FieldStage::getBlockAttr(int bx, int by)
{
    return fieldData.isEnable(bx, by);
}

ARM int fld::FieldStage::getBlockAttr2(int bx, int by)
{
    return fieldData.getAttr(bx, by);
}

ARM dss::Fix32Vector3 fld::FieldStage::getSymbolPosition(int index)
{
    return fieldData.getSymbolPosition(index);
}

ARM int fld::FieldStage::getSearchSymbolAttach(dss::Fix32Vector3 pos)
{
    return fieldData.getSearchSymbolAttach(pos);
}

ARM void fld::FieldStage::ChangeTime(int apply)
{
    dss::Fix32Vector3 rgbRate;
    int t = g_Stage.getWorldTime();
    if (g_Stage.timestop_ == 1) {
        return;
    }
    if (t % 16 == 0 || apply == 1) {
        if (map_ != 2) {
            rgbRate = cmn::WorldLocation::getSingleton()->calcPaletteRate(t);
            fieldData.setPaletteRate(rgbRate);
        } else {
            rgbRate = cmn::WorldLocation::getSingleton()->calcYamiPaletteRate();
            fieldData.setPaletteRate(rgbRate);
        }
    }
    if (apply == 0) {
        t++;
    }
    if (t == 0xcc0) {
        cmn::g_CommonCounterInfo.setChangeDay();
    }
    if (t >= 0xd40) {
        t = 0;
    }
    g_Stage.setWorldTime(t);
}

ARM dss::Vector2<int> fld::FieldStage::calcDrawPosition(dss::Fix32Vector3 pos)
{
    dss::Fix32Vector3 fieldPos(fieldData.position_);
    dss::Vector2<int> posXY;
    int offset = fieldData.offset_ + 0x20;
    posXY.vx = pos.vx.value / FX32_ONE;
    posXY.vy = pos.vy.value / FX32_ONE;
    if (posXY.vx < offset + 0x100) {
        if (fieldPos.vx.value / FX32_ONE > 0xf00 - offset) {
            posXY.vx += 0x1000;
        }
    }
    if (posXY.vx > 0xf00 - offset) {
        if (fieldPos.vx.value / FX32_ONE < offset + 0x100) {
            posXY.vx -= 0x1000;
        }
    }
    if (posXY.vy < offset + 0xc0) {
        if (fieldPos.vy.value / FX32_ONE > 0xf40 - offset) {
            posXY.vy += 0x1000;
        }
    }
    if (posXY.vy > 0xf40 - offset) {
        if (fieldPos.vy.value / FX32_ONE < offset + 0xc0) {
            posXY.vy -= 0x1000;
        }
    }
    return posXY;
}

ARM void fld::FieldStage::setSymbolFlag(int uid)
{
    for (int i = 0; i < 114; i++) {
        if (uid == status::excelParam.fieldSymbol_[i].uid) {
            g_Stage.setSymbolFlag(i);
            return;
        }
    }
}

ARM void fld::FieldStage::eraseSymbol(int id)
{
    fieldData.setDispSymbol(id, 0);
    cmn::g_extraMapLink.setLinkData(id, 0, cmn::NOT_LINK_THIS_TOWN, NULL, NULL);
}
