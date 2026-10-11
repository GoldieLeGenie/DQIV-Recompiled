#pragma ipa file
#include "ov001/fld/FieldPlayerManager.hpp"
#include "ov001/fld/FieldStage.hpp"
#include "ov001/fld/FieldSymbolManager.hpp"
#include "ov001/fld/FieldPlayerDoku.hpp"
#include "ov001/fld/FieldActionCalculate.hpp"
#include "ov001/fld/FieldRectCollManager.hpp"
#include "ov001/window/FieldWindowSystem.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/cmn/ExtraMapLink.hpp"
#include "main/cmn/HengeNoTsueManager.hpp"
#include "main/cmn/WorldLocation.hpp"
#include "main/global/Global.hpp"
#include "main/menu/UiMsg.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/status/HaveEquipment.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/StageStatus.hpp"

const dss::Fix32 FieldPlayer::shipSearchR(0x10000);
const dss::Fix32 FieldPlayer::balloonSearchR(0x14000);
const dss::Fix32 FieldPlayer::Speed(1.5f);
static const dss::Fix32 xSpeed(1.0f);
static const dss::Fix32 ySpeed(1.0f);
const dss::Fix32 FieldPlayer::balSpeed(2.0f);
static const dss::Fix32 balXSpeed(1.5f);
static const dss::Fix32 balYSpeed(1.5f);

ARM FieldPlayer::FieldPlayer()
{
}

ARM FieldPlayer::~FieldPlayer()
{
}

ARM void FieldPlayer::setup()
{
    *dirIdx_ = DIR_8_DD;
    type_ = MOVE_WALK;
    flagFixPos_ = -1;
    positionN_ = *position_;
    collSE_ = 0;
    move_ = 0;
}

ARM void FieldPlayer::cleanup()
{
}

ARM void FieldPlayer::execute()
{
    switch (type_) {
    case MOVE_WALK:
        walkExec();
        break;
    case MOVE_SHIP:
        shipExec();
        break;
    case MOVE_BALLOON:
        balloonExec();
        break;
    case MOVE_WAIT:
        if (--waitCounter_ <= 0) {
            type_ = backupType_;
        }
        break;
    }
    *position_ = positionN_;
    cmn::WorldLocation::calcWorldPos(&position_->vx.value, &position_->vy.value);
}

ARM void FieldPlayer::balloonExec()
{
    balloonMove();
    checkGetOffBalloon();
    setBalloonShadow();
}

ARM void FieldPlayer::balloonMove()
{
    static const dss::Fix32Vector3 balVec[8] = {
        dss::Fix32Vector3(balSpeed * 0, balSpeed * -1, 0L),
        dss::Fix32Vector3(balXSpeed * 1, balYSpeed * -1, 0L),
        dss::Fix32Vector3(balSpeed * 1, balSpeed * 0, 0L),
        dss::Fix32Vector3(balXSpeed * 1, balYSpeed * 1, 0L),
        dss::Fix32Vector3(balSpeed * 0, balSpeed * 1, 0L),
        dss::Fix32Vector3(balXSpeed * -1, balYSpeed * 1, 0L),
        dss::Fix32Vector3(balSpeed * -1, balSpeed * 0, 0L),
        dss::Fix32Vector3(balXSpeed * -1, balYSpeed * -1, 0L),
    };
    if (padInput_ != 0) {
        *dirIdx_ = (dirInput_ / 0x2000) & 7;
        speed_ = balVec[*dirIdx_];
    }
    positionN_ = *position_ + speed_;
}

ARM void FieldPlayer::getBalloonCollPoint(int index, dss::Fix32Vector3& pos)
{
    static const dss::Fix32Vector3 vec[4] = {
        dss::Fix32Vector3(-6, -6, 0),
        dss::Fix32Vector3(6, -6, 0),
        dss::Fix32Vector3(6, 6, 0),
        dss::Fix32Vector3(-6, 6, 0),
    };
    pos += vec[index];
}

ARM void FieldPlayer::checkGetOffBalloon()
{
    int blkX = position_->vx.value / 0x10000;
    int blkY = position_->vy.value / 0x10000;
    if (FieldPlayerManager::getSingleton()->getPlayerCommand() == PUSH_BENRI_BUTTON ||
        FieldPlayerManager::getSingleton()->getPlayerCommand() == PUSH_BALLOON_GETOUT_BUTTON) {
        bool hitAttr = fld::FieldStage::getSingleton()->getBlockAttr(blkX, blkY);
        for (int i = 0; i < 4; i++) {
            dss::Fix32Vector3 target(positionN_);
            getBalloonCollPoint(i, target);
            if (fld::FieldStage::getSingleton()->getBlockAttr(target.vx.value / 0x10000, target.vy.value / 0x10000) == false) {
                hitAttr = false;
            }
        }
        if (FieldRectCollManager::getSingleton()->checkFieldColl(positionN_) == RECT_UMINARI) {
            hitAttr = false;
        }
        if (hitAttr == true) {
            if (fld::FieldStage::getSingleton()->getSearchSymbolAttach(positionN_) == -1) {
                if (cmn::g_extraMapLink.checkFieldRectLinkByType(positionN_, 10)) {
                    g_Stage.balloonFieldType_ = 1;
                    FieldPlayerManager::getSingleton()->setLock(1);
                } else {
                    type_ = MOVE_BALLOON_GET_OUT;
                    fld::FieldStage::getSingleton()->fieldData.pause_ = 1;
                    FieldPlayerManager::getSingleton()->setBalloonPos(positionN_);
                    g_Stage.balloonFieldType_ = g_Global.getFieldType();
                    *position_ = positionN_;
                }
            } else {
                ui_MsgSndSet(0x30);
                FieldWindowSystem::getSingleton()->openCommonMessage();
                FieldWindowSystem::getSingleton()->addCommonMessage(0xc3dd7);
            }
        } else {
            ui_MsgSndSet(0x30);
            FieldWindowSystem::getSingleton()->openCommonMessage();
            FieldWindowSystem::getSingleton()->addCommonMessage(0xc3dd7);
        }
    }
    if (FieldPlayerManager::getSingleton()->getPlayerCommand() == PUSH_BALLOON_SEARCH_BUTTON) {
        ui_MsgSndSet(0x30);
        FieldWindowSystem::getSingleton()->openCommonMessage();
        FieldWindowSystem::getSingleton()->addCommonMessage(0xc40cc);
        FieldWindowSystem::getSingleton()->addCommonMessage(0xc40cd);
        FieldPlayerManager::getSingleton()->setPlayerCommand(START_SEARCH_COMMAND);
    }
    blockType_[8] = fld::FieldStage::getSingleton()->getBlockAttr2(blkX, blkY);
}

ARM void FieldPlayer::shipExec()
{
    move();
    int blkX = position_->vx.value / 0x10000;
    int blkY = position_->vy.value / 0x10000;
    blockType_[8] = fld::FieldStage::getSingleton()->getBlockAttr2(blkX, blkY);
    setShipColl(blkX, blkY);
    setChipAttr(blkX, blkY);
    coll(blkX, blkY, 1, 1);
    if (padInput_ != 0) {
        *dirIdx_ = dirInput_ / 0x2000;
    }
}

ARM void FieldPlayer::walkExec()
{
    move_ = 0;
    dss::Fix32Vector3 oldPos(*position_);
    move();
    walkCollision();
    dss::Fix32Vector3 vec = positionN_ - oldPos;
    fldSearch();
    if (oldPos != positionN_) {
        *dirIdx_ = FieldActionCalculate::getDir8ByVector3(vec);
        if (type_ == MOVE_WALK) {
            g_HengeNoTsue.execute();
            if (g_HengeNoTsue.isEnd() == 1) {
                FieldPlayerManager::getSingleton()->setLock(1);
                g_HengeNoTsue.setNextAction(getMoveType());
                setMoveType(HENGE_RESET);
                FieldPlayerManager::getSingleton()->hengeCounter_ = 0;
                SoundManager::playSe(0x169, 0);
                return;
            }
        }
        collSE_ = 1;
    } else if (move_ == 1 && collSE_ == 1) {
        SoundManager::playSe(0x133, 0);
        collSE_ = 0;
    }
}

ARM bool FieldPlayer::checkShip()
{
    if (g_cmnPartyInfo.isShipEnable() == 0) {
        return false;
    }
    dss::Fix32Vector3 vec;
    dss::Fix32Vector3 unk_48;
    dss::Fix32Vector3 shipPos = FieldPlayerManager::getSingleton()->shipDraw_.getPosition();
    dss::Fix32 length[4];
    int blkX = shipPos.vx.value / 0x10000;
    int blkY = shipPos.vy.value / 0x10000;
    vec = shipPos - positionN_;
    vec.vz = 0L;
    vec.normalize();
    bool ret = searchObject(shipPos, shipSearchR) == true && blockType_[8] == WMAP_ATTR_KAIG;
    if (ret) {
        dss::Fix32Vector3 playerDir = FieldActionCalculate::getVector3ByDir8(*dirIdx_);
        if (vec * playerDir < dss::Fix32(0xc42)) {
            return false;
        }
        if (fld::FieldStage::getSingleton()->getBlockAttr2(blkX, blkY) == WMAP_ATTR_KAIG) {
            length[0] = shipPos.vy.value - (blkY << 16);
            length[1] = ((blkX + 1) << 16) - shipPos.vx.value;
            length[2] = ((blkY + 1) << 16) - shipPos.vy.value;
            length[3] = shipPos.vx.value - (blkX << 16);
            int temp = 0;
            for (int i = 0; i < 4; i++) {
                if (length[temp] > length[i]) {
                    temp = i;
                }
            }
            switch (temp) {
            case 0:
                shipPos.vy.value = (blkY << 16) - 1000;
                break;
            case 1:
                shipPos.vx.value = ((blkX + 1) << 16) + 1000;
                break;
            case 2:
                shipPos.vy.value = ((blkY + 1) << 16) + 1000;
                break;
            case 3:
                shipPos.vx.value = (blkX << 16) - 1000;
                break;
            }
        }
        type_ = MOVE_SHIP_GET_ON;
        SoundManager::stopBgm(0x1e);
        vec.normalize();
        FieldPlayerManager::getSingleton()->setLock(1);
        FieldPlayerManager::getSingleton()->targetPos_ = shipPos;
        FieldPlayerManager::getSingleton()->setSpeed(Speed);
        *dirIdx_ = FieldActionCalculate::getDir8ByVector3(vec);
        fld::FieldStage::getSingleton()->fieldData.pause_ = 1;
        return true;
    }
    return false;
}

ARM void FieldPlayer::move()
{
    speed_.vx = 0L;
    speed_.vy = 0L;
    if (padInput_ != 0) {
        *dirIdx_ = dirInput_ / 0x2000;
        move_ = 1;
        switch (*dirIdx_) {
        case DIR_8_UU:
            speed_.vx = 0L;
            speed_.vy = Speed * -1;
            break;
        case DIR_8_RU:
            speed_.vx = xSpeed;
            speed_.vy = ySpeed * -1;
            break;
        case DIR_8_RR:
            speed_.vx = Speed;
            speed_.vy = 0L;
            break;
        case DIR_8_RD:
            speed_.vx = xSpeed;
            speed_.vy = ySpeed;
            break;
        case DIR_8_DD:
            speed_.vx = 0L;
            speed_.vy = Speed;
            break;
        case DIR_8_LD:
            speed_.vx = xSpeed * -1;
            speed_.vy = ySpeed;
            break;
        case DIR_8_LL:
            speed_.vx = Speed * -1;
            speed_.vy = 0L;
            break;
        case DIR_8_LU:
            speed_.vx = xSpeed * -1;
            speed_.vy = ySpeed * -1;
            break;
        }
        positionN_ = *position_ + speed_;
    } else {
        positionN_ = *position_;
    }
    speed_.normalize();
    speed_ *= dss::Fix32(1.2f);
}

ARM void FieldPlayer::walkCollision()
{
    int blkX = position_->vx.value / 0x10000;
    int blkY = position_->vy.value / 0x10000;
    blockType_[8] = fld::FieldStage::getSingleton()->getBlockAttr2(blkX, blkY);
    setWalkColl(blkX, blkY);
    setChipAttr(blkX, blkY);
    dss::Fix32Vector3 pvec = FieldActionCalculate::getVector3ByDir8(*dirIdx_);
    dss::Fix32Vector3 searchPos = *position_ + pvec * 8;
    dss::Vector2<dss::Fix32> kpos;
    dss::Vector2<dss::Fix32> pos;
    if (fld::FieldStage::getSingleton()->searchKanban(searchPos.vx.value, searchPos.vy.value, &kpos) != -1) {
        pos.vx = searchPos.vx;
        pos.vy = searchPos.vy;
        dss::Vector2<dss::Fix32> vec = kpos - pos;
        vec.normalize();
        dss::Vector2<dss::Fix32> pvec2(pvec.vx, pvec.vy);
        if (vec * pvec2 < dss::Fix32(0xf09)) {
            positionN_ = *position_;
        }
    }
    coll(blkX, blkY, 6, 6);
}

ARM void FieldPlayer::coll(int blkX, int blkY, int collLength, int fixLength)
{
    int ret = FieldRectCollManager::getSingleton()->checkFieldColl(positionN_);
    if (ret != RECT_NONE) {
        positionN_ = *position_;
        if (ret == RECT_YAMI_BARRIER) {
            ui_MsgSndSet(0x30);
            FieldWindowSystem::getSingleton()->openCommonMessage();
            FieldWindowSystem::getSingleton()->addCommonMessage(0x1ec31);
        }
        return;
    }
    fieldCollInfo_.fixLine[0] = dss::Fix32((long)(blkY * 16 + fixLength));
    fieldCollInfo_.fixLine[1] = dss::Fix32((long)(blkX * 16 + 16 - fixLength));
    fieldCollInfo_.fixLine[2] = dss::Fix32((long)(blkY * 16 + 16 - fixLength));
    fieldCollInfo_.fixLine[3] = dss::Fix32((long)(blkX * 16 + fixLength));
    fieldCollInfo_.collLine[0] = dss::Fix32((long)(blkY * 16 + collLength));
    fieldCollInfo_.collLine[1] = dss::Fix32((long)(blkX * 16 + 16 - collLength));
    fieldCollInfo_.collLine[2] = dss::Fix32((long)(blkY * 16 + 16 - collLength));
    fieldCollInfo_.collLine[3] = dss::Fix32((long)(blkX * 16 + collLength));
    FieldPlayerInfo fldPlayerInfo = { *position_, positionN_, *dirIdx_ };
    if (type_ == MOVE_WALK) {
        if (checkShip() == false) {
            FieldActionCalculate::playerFixMove(&fldPlayerInfo, &fieldCollInfo_, blkX, blkY, Speed);
        }
    } else if (type_ == MOVE_SHIP) {
        FieldActionCalculate::playerFixMove(&fldPlayerInfo, &fieldCollInfo_, blkX, blkY, Speed);
        checkGetDownShip(blkX, blkY, *dirIdx_, fldPlayerInfo.nextPos);
    }
    positionN_ = fldPlayerInfo.nextPos;
    FieldPlayerDoku::getSingleton()->setBlockAttr(blockType_[8]);
}

ARM int FieldPlayer::getAttr()
{
    int blkX = position_->vx.value / 0x10000;
    int blkY = position_->vy.value / 0x10000;
    return fld::FieldStage::getSingleton()->getBlockAttr2(blkX, blkY);
}

ARM void FieldPlayer::setPosition(dss::Fix32Vector3& pos)
{
    *position_ = pos;
    positionN_ = pos;
}

ARM const dss::Fix32Vector3& FieldPlayer::getPosition()
{
    return *position_;
}

ARM void FieldPlayer::setMoveType(int type)
{
    type_ = type;
}

ARM int FieldPlayer::getMoveType()
{
    return type_;
}

ARM bool FieldPlayer::searchObject(dss::Fix32Vector3& searchPos, dss::Fix32 dr)
{
    bool ret = false;
    dss::Fix32Vector3 vec;
    int pblkX = positionN_.vx.value / 0x10000;
    int pblkY = positionN_.vy.value / 0x10000;
    int sblkX = searchPos.vx.value / 0x10000;
    int sblkY = searchPos.vy.value / 0x10000;
    int dx = pblkX - sblkX;
    int dy = pblkY - sblkY;
    if (status::HaveEquipment::getAbsoluteValue(dx) < 3 && status::HaveEquipment::getAbsoluteValue(dy) < 3) {
        vec = searchPos - positionN_;
        if (vec.length() < dr) {
            ret = true;
        }
    }
    return ret;
}

ARM bool FieldPlayer::playerSearch()
{
    dss::Fix32Vector3 unk_3c;
    dss::Fix32Vector3 unk_30;
    dss::Fix32Vector3 searchPos;
    searchPos = *position_ + FieldActionCalculate::getVector3ByDir8(*dirIdx_) * 20;
    int kanbanId = fld::FieldStage::getSingleton()->searchKanban(searchPos.vx.value, searchPos.vy.value, NULL);
    if (kanbanId != -1) {
        if (FieldSymbolManager::getSingleton()->checkSymbol(kanbanId) == true) {
            return true;
        }
    }
    return false;
}

ARM void FieldPlayer::fldSearch()
{
    dss::Fix32Vector3 balPos;
    dss::Fix32Vector3 unk_34;
    dss::Fix32Vector3 searchPos;
    searchPos = *position_ + FieldActionCalculate::getVector3ByDir8(*dirIdx_) * 20;
    if (FieldPlayerManager::getSingleton()->getPlayerCommand() != PUSH_BENRI_BUTTON) {
        return;
    }
    int kanbanId = fld::FieldStage::getSingleton()->searchKanban(searchPos.vx.value, searchPos.vy.value, NULL);
    if (kanbanId != -1) {
        FieldSymbolManager::getSingleton()->checkSymbol(kanbanId);
        FieldPlayerManager::getSingleton()->setPlayerCommand(START_SEARCH_COMMAND);
        return;
    }
    status::g_Party.setPlayerMode();
    if (status::g_Party.getCarriageOutAliveCount() == 0) {
        return;
    }
    if (g_cmnPartyInfo.isBalloonEnable() != 1) {
        return;
    }
    balPos = FieldPlayerManager::getSingleton()->balloonDraw_.getPosition();
    if (searchObject(balPos, balloonSearchR) == false) {
        return;
    }
    FieldWindowSystem::getSingleton()->setMenuPermit(false);
    if (type_ == MOVE_WALK) {
        type_ = MOVE_BALLOON_GET_ON_WALK;
        fld::FieldStage::getSingleton()->fieldData.pause_ = 1;
        FieldPlayerManager::getSingleton()->setPlayerCommand(START_RIDE_BALLOON_COMMAND);
        FieldPlayerManager::getSingleton()->setLock(1);
        speed_.set(0, 0, 0);
    } else {
        return;
    }
}

ARM void FieldPlayer::setWalkColl(int bx, int by)
{
    fieldCollInfo_.blockColl[0] = fld::FieldStage::getSingleton()->getBlockAttr(bx, by - 1);
    fieldCollInfo_.blockColl[1] = fld::FieldStage::getSingleton()->getBlockAttr(bx + 1, by - 1);
    fieldCollInfo_.blockColl[2] = fld::FieldStage::getSingleton()->getBlockAttr(bx + 1, by);
    fieldCollInfo_.blockColl[3] = fld::FieldStage::getSingleton()->getBlockAttr(bx + 1, by + 1);
    fieldCollInfo_.blockColl[4] = fld::FieldStage::getSingleton()->getBlockAttr(bx, by + 1);
    fieldCollInfo_.blockColl[5] = fld::FieldStage::getSingleton()->getBlockAttr(bx - 1, by + 1);
    fieldCollInfo_.blockColl[6] = fld::FieldStage::getSingleton()->getBlockAttr(bx - 1, by);
    fieldCollInfo_.blockColl[7] = fld::FieldStage::getSingleton()->getBlockAttr(bx - 1, by - 1);
}

ARM void FieldPlayer::setChipAttr(int bx, int by)
{
    blockType_[0] = fld::FieldStage::getSingleton()->getBlockAttr2(bx, by - 1);
    blockType_[1] = fld::FieldStage::getSingleton()->getBlockAttr2(bx + 1, by - 1);
    blockType_[2] = fld::FieldStage::getSingleton()->getBlockAttr2(bx + 1, by);
    blockType_[3] = fld::FieldStage::getSingleton()->getBlockAttr2(bx + 1, by + 1);
    blockType_[4] = fld::FieldStage::getSingleton()->getBlockAttr2(bx, by + 1);
    blockType_[5] = fld::FieldStage::getSingleton()->getBlockAttr2(bx - 1, by + 1);
    blockType_[6] = fld::FieldStage::getSingleton()->getBlockAttr2(bx - 1, by);
    blockType_[7] = fld::FieldStage::getSingleton()->getBlockAttr2(bx - 1, by - 1);
}

ARM void FieldPlayer::setShipColl(int bx, int by)
{
    fieldCollInfo_.blockColl[0] = getShipColl(bx, by - 1);
    fieldCollInfo_.blockColl[1] = getShipColl(bx + 1, by - 1);
    fieldCollInfo_.blockColl[2] = getShipColl(bx + 1, by);
    fieldCollInfo_.blockColl[3] = getShipColl(bx + 1, by + 1);
    fieldCollInfo_.blockColl[4] = getShipColl(bx, by + 1);
    fieldCollInfo_.blockColl[5] = getShipColl(bx - 1, by + 1);
    fieldCollInfo_.blockColl[6] = getShipColl(bx - 1, by);
    fieldCollInfo_.blockColl[7] = getShipColl(bx - 1, by - 1);
}

ARM bool FieldPlayer::getShipColl(int blkX, int blkY)
{
    bool ret = false;
    if (fld::FieldStage::getSingleton()->getBlockAttr2(blkX, blkY) == WMAP_ATTR_UMI || blkX < 0 || blkY < 0 || blkX >= 0x100 || blkY >= 0x100) {
        ret = true;
    }
    return ret;
}

ARM bool FieldPlayer::checkGetDownShip(int blkX, int blkY, int dirIdx, dss::Fix32Vector3& fixPos)
{
    setWalkColl(blkX, blkY);
    dss::Fix32Vector3 dir = FieldActionCalculate::getVector3ByDir8(dirIdx);
    int targetBlkX = (positionN_.vx.value + dir.vx.value * 4) / 0x10000;
    int targetBlkY;
    if (dir.vy <= 0L) {
        targetBlkY = (positionN_.vy.value + dir.vy.value * 6) / 0x10000;
    } else {
        targetBlkY = (positionN_.vy.value + dir.vy.value) / 0x10000;
    }
    if (targetBlkX == blkX && targetBlkY == blkY) {
        return false;
    }
    if (dirIdx % 2 != 0) {
        return false;
    }
    if (fieldCollInfo_.blockColl[dirIdx] == 1 && blockType_[dirIdx] == WMAP_ATTR_KAIG) {
        dss::Fix32Vector3 target(positionN_);
        if (dir.vx < 0L) {
            target.vx.value = targetBlkX * 0x10000 - dir.vx.value * 8;
        } else if (dir.vx > 0L) {
            target.vx.value = dir.vx.value * 8 + targetBlkX * 0x10000;
        }
        if (dir.vy < 0L) {
            target.vy.value = targetBlkY * 0x10000 - dir.vy.value * 8;
        } else if (dir.vy > 0L) {
            target.vy.value = dir.vy.value * 8 + targetBlkY * 0x10000;
        }
        bool hitAttr = isEnableGetOff(target);
        short shipDir = *dirIdx_;
        if (hitAttr == false) {
            target.vx.value = targetBlkX * 0x10000 + 0x8000;
            target.vy.value = targetBlkY * 0x10000 + 0x8000;
            hitAttr = isEnableGetOff(target);
        }
        if (hitAttr == true) {
            FieldPlayerManager::getSingleton()->targetPos_ = target;
            FieldPlayerManager::getSingleton()->setSpeed(Speed);
            type_ = MOVE_SHIP_GET_OUT;
            SoundManager::stopBgm(0x1e);
            FieldPlayerManager::getSingleton()->setLock(1);
            FieldPlayerManager::getSingleton()->partyDraw_.resetDrawCount();
            FieldPlayerManager::getSingleton()->setPartyToFirst();
            FieldPlayerManager::getSingleton()->setShipPos(*position_);
            FieldPlayerManager::getSingleton()->shipDraw_.setRotate(shipDir);
            dss::Fix32Vector3 vec = target - positionN_;
            *dirIdx_ = FieldActionCalculate::getDir8ByVector3(vec);
            fld::FieldStage::getSingleton()->fieldData.pause_ = 1;
            return true;
        }
    }
    return false;
}

ARM void FieldPlayer::setBalloonShadow()
{
    switch (blockType_[8]) {
    case WMAP_ATTR_UMI:
        FieldPlayerManager::getSingleton()->balloonDraw_.carrier_->setShadowWH(shadowX_umi, shadowY_umi);
        break;
    case WMAP_ATTR_KAIG:
        FieldPlayerManager::getSingleton()->balloonDraw_.carrier_->setShadowWH(shadowX_kaigan, shadowY_kaigan);
        break;
    case WMAP_ATTR_MORI:
        FieldPlayerManager::getSingleton()->balloonDraw_.carrier_->setShadowWH(shadowX_mori, shadowY_mori);
        break;
    case WMAP_ATTR_YAMA1:
        FieldPlayerManager::getSingleton()->balloonDraw_.carrier_->setShadowWH(shadowX_yama, shadowY_yama);
        break;
    case WMAP_ATTR_YAMA2:
        FieldPlayerManager::getSingleton()->balloonDraw_.carrier_->setShadowWH(shadowX_iwa, shadowY_iwa);
        break;
    default:
        FieldPlayerManager::getSingleton()->balloonDraw_.carrier_->setShadowWH(shadowX_normal, shadowY_normal);
        break;
    }
}

ARM void FieldPlayer::setPositionPointer(dss::Fix32Vector3* pPos)
{
    position_ = pPos;
}

ARM void FieldPlayer::setDirIdxPointer(short* dirIdx)
{
    dirIdx_ = dirIdx;
}

ARM void FieldPlayer::setWait(int count)
{
    backupType_ = type_;
    waitCounter_ = count;
}

ARM bool FieldPlayer::isEnableGetOff(dss::Fix32Vector3& target)
{
    int x = target.vx.value / 0x10000;
    int y = target.vy.value / 0x10000;
    bool hitAttr = fld::FieldStage::getSingleton()->getBlockAttr(x, y);
    for (int i = 0; i < 4; i++) {
        dss::Fix32Vector3 pos(target);
        getBalloonCollPoint(i, pos);
        if (fld::FieldStage::getSingleton()->getBlockAttr(pos.vx.value / 0x10000, pos.vy.value / 0x10000) == false) {
            hitAttr = false;
        }
    }
    return hitAttr;
}

ARM void FieldPlayer::unkfunc_0213c9b0()
{
}

ARM void FieldPlayer::unkfunc_0213c9b4()
{
}

#include "main/dss/DssVectorDefault.hpp"
