#pragma ipa file
#include "ov000/town/TownFurniture.hpp"
#include "ov000/town/TownFurnitureControl.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "ov000/town/TownWindowSystem.hpp"
#include "main/cmn/TalkSoundManager.hpp"
#include "main/data/FileLoader.hpp"
#include "main/dss/Random.hpp"
#include "main/fld/FldStage.hpp"
#include "main/global/Global.hpp"
#include "main/menu/MaterielMenu_SlotEnter.hpp"
#include "main/param/Event.hpp"
#include "main/script/ScriptSystem.hpp"
#include "main/status/BattleResult.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/GameFlag.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/status/StageStatus.hpp"
#include "main/status/Status.hpp"
#include "main/status/StoryStatus.hpp"
#include "ov000/town/riseup/TownRiseup.hpp"

static dss::Fix32Vector3 boxTestRotate;
static dss::Fix32Vector3 boxTestScale(0.96f, 0.96f, 0.96f);
static GXBoxTestParam boxTestParam;
static dss::Fix32 boxTestRate(FX32_ONE);

THUMB TownFurnitureManager::TownFurnitureManager()
{
}

THUMB TownFurnitureManager::~TownFurnitureManager()
{
}

THUMB TownFurnitureManager* TownFurnitureManager::getSingleton()
{
    static TownFurnitureManager m_singleton;
    return &m_singleton;
}

THUMB void TownFurnitureManager::initialize()
{
    char path[128];
    common_ = status::excelParam.commonParam_;
    dss::sprintf_s(path, sizeof(path), "data/param/param_item_%s.dat", g_Global.getMapName());
    if (dss::g_File.isExist(path)) {
        list_ = (param::CommonList*)ExcelBinaryData::checkSum(ExcelBinaryData::readFileData(&param::CommonList::data_, path), param::CommonList::ID_);
        size_ = list_->ListSize;
    } else {
        size_ = 0;
    }
    returnFurnitureEncount();
    for (int i = 0; i < size_; i++) {
        if (g_Stage.getFurnFlag(list_[i].flagIndex) && list_[i].type == 7) {
            TownStageManager::getSingleton()->setObjectDraw(list_[i].uid, 3, 1);
        }
        if (g_Stage.getDoorOpenFlag(list_[i].openIndex)) {
            TownStageManager::getSingleton()->eraseObject(list_[i].uid, 1);
        }
        if (g_Stage.getFurnBreakFlag(list_[i].furnIndex)) {
            TownStageManager::getSingleton()->eraseObject(list_[i].uid, 1);
        }
    }
    object_ = NULL;
    TownFurnitureControlManager::getSingleton()->initialize();
    boxTestParam.x = 0;
    boxTestParam.y = 0;
    boxTestParam.z = 0;
    boxTestParam.width = FX32_ONE;
    boxTestParam.height = FX32_ONE;
    boxTestParam.depth = FX32_ONE;
    force_ = 0;
    prevCheck_ = 0;
    phase_ = 0;
    remiIndex_ = -1;
}

THUMB void TownFurnitureManager::terminate()
{
    ExcelBinaryData::clearData(&param::CommonList::data_);
    object_ = NULL;
    TownFurnitureControlManager::getSingleton()->terminate();
}

THUMB void TownFurnitureManager::execute()
{
    TownFurnitureControlManager::getSingleton()->execute();
    if (object_ != NULL) {
        object_->execute();
        if (object_->isFinish()) {
            object_ = NULL;
            TownPlayerManager::getSingleton()->setLock(0);
        }
    }
    if (remiIndex_ >= 0) {
        if (TownRiseupManager::getSingleton()->isFinish(remiIndex_)) {
            TownPlayerManager::getSingleton()->setLock(0);
            remiIndex_ = -1;
        }
    }
}

THUMB bool TownFurnitureManager::isProcess()
{
    return object_ != NULL;
}

THUMB int TownFurnitureManager::getFurnitureIndex(int uid)
{
    int index = -1;
    for (int i = 0; i < size_; i++) {
        if (uid == list_[i].uid) {
            index = i;
            break;
        }
    }
    return index;
}

THUMB void TownFurnitureManager::draw()
{
    switch (phase_) {
    case 0:
        break;
    case 1:
        setTwinklePoint();
        phase_ = 2;
        break;
    case 2:
        setTwinklePoint();
        phase_ = 3;
        break;
    case 3:
        drawTwinklePoint();
        phase_ = 0;
        break;
    }
}

THUMB void TownFurnitureManager::returnFurnitureEncount()
{
    if (g_Stage.encountMapUid_ != 0 && status::g_BattleResult.playerVictory_ == 1) {
        param::CommonList* list = &list_[getFurnitureIndex(g_Stage.encountMapUid_)];
        g_Stage.setFurnFlag(list->flagIndex);
    }
    g_Stage.encountMapUid_ = 0;
}

THUMB void TownFurnitureManager::openDoor(int uid)
{
    int index = getFurnitureIndex(uid);
    if (index >= 0) {
        g_Stage.setDoorOpenFlag(list_[index].openIndex);
    }
}

THUMB void TownFurnitureManager::closeDoor(int uid)
{
    int index = getFurnitureIndex(uid);
    if (index >= 0) {
        g_Stage.removeDoorOpenFlag(list_[index].openIndex);
    }
}

THUMB bool TownFurnitureManager::isOpenDoor(int uid)
{
    int index = getFurnitureIndex(uid);
    if (index < 0) {
        return false;
    }
    return g_Stage.getDoorOpenFlag(list_[index].openIndex);
}

THUMB void TownFurnitureManager::setFurnFlag(int uid, bool flag)
{
    int index = getFurnitureIndex(uid);
    if (index >= 0) {
        if (flag) {
            g_Stage.setFurnFlag(list_[index].flagIndex);
        } else {
            g_Stage.removeFurnFlag(list_[index].flagIndex);
        }
    }
}

THUMB bool TownFurnitureManager::checkObject(int uid, int rev, int search, int floor)
{
    bool nothing = false;
    param::CommonParam* pCommon;
    int index = getFurnitureIndex(uid);
    if (index < 0) {
        return false;
    }
    param::CommonList* list = &list_[index];
    if ((char)(list->byte_1 & 1) && force_ == 0) {
        return false;
    }
    force_ = 0;
    if (rev && checkRevMessage(index)) {
        return true;
    }
    if (list->type == 0) {
        if (list->message == 0 && list->item == 0 && list->gold == 0) {
            return false;
        }
        pCommon = common_;
    } else {
        pCommon = &common_[list->type];
    }
    if (list->type == 9) {
        if (!g_AreaFlag.check(0x99) && status::g_Story.chapter_ == 3) {
            return false;
        }
        bootSlot(list->uid);
        return true;
    }
    TownPlayerManager::getSingleton()->setLock(1);
    if (list->type == 0x13) {
        mirrorTalk(list->uid);
        return true;
    }
    if ((char)(pCommon->byte_1 & 1) && !search) {
        g_Stage.setFurnBreakFlag(list->furnIndex);
    }
    if (list->item != 0 || list->gold != 0 || list->encount != 0) {
        if (g_Stage.getFurnFlag(list->flagIndex)) {
            nothing = true;
        }
    } else if (list->message == 0 && list->item == 0 && list->gold == 0 && list->encount == 0) {
        nothing = true;
    }
    if (nothing) {
        prevCheck_ = 0;
        if (floor == 1) {
            TownPlayerManager::getSingleton()->setLock(0);
            return false;
        }
        object_ = &nothingObject_;
        object_->setup(list->uid, pCommon->NothingMsg, pCommon, search);
        if (list->type == 7) {
            if (!g_Stage.getFurnFlag(list->flagIndex)) {
                g_Stage.setFurnFlag(list->flagIndex);
                object_->furniture_.flag_ &= ~TownFurnitureObject::CLOSE_ANIM_ENABLE;
            } else {
                object_->furniture_.flag_ &= ~(TownFurnitureObject::OPEN_ANIM_ENABLE | TownFurnitureObject::CLOSE_ANIM_ENABLE | TownFurnitureObject::CHECK_ANIM_FLAG);
            }
        }
    } else {
        if (list->message != 0) {
            prevCheck_ = 1;
            object_ = &msgObject_;
            object_->setup(list->uid, list->message, pCommon, search);
        } else if (list->item != 0) {
            prevCheck_ = 2;
            object_ = &itemObject_;
            object_->setup(list->uid, list->item, pCommon, search);
            g_Stage.setFurnFlag(list->flagIndex);
        } else if (list->gold != 0) {
            prevCheck_ = 3;
            object_ = &goldObject_;
            object_->setup(list->uid, list->gold, pCommon, search);
            g_Stage.setFurnFlag(list->flagIndex);
        } else if (list->encount != 0) {
            prevCheck_ = 4;
            object_ = &encountObject_;
            object_->setup(list->uid, list->encount, pCommon, search);
            object_->setupExtend(list->monster);
        }
        if (list->type == 7) {
            object_->furniture_.flag_ &= ~TownFurnitureObject::CLOSE_ANIM_ENABLE;
        }
    }
    return true;
}

THUMB bool TownFurnitureManager::checkRevMessage(int index)
{
    param::CommonParam* pCommon = &common_[list_[index].type];
    if (pCommon->BackMsg != 0) {
        ui_MsgSndSet(0x30);
        TownWindowSystem::getSingleton()->openCommonMessage();
        unsigned int msg = pCommon->checkMsg;
        if (msg != 0) {
            TownWindowSystem::getSingleton()->addCommonMessage(msg);
        }
        TownWindowSystem::getSingleton()->serialCommonMessage(pCommon->BackMsg);
        return true;
    }
    return false;
}

THUMB void TownFurnitureManager::nothingGround()
{
    ui_MsgSndSet(0x30);
    TownWindowSystem::getSingleton()->openCommonMessage();
    TownWindowSystem::getSingleton()->addCommonMessage(common_[44].checkMsg);
    TownWindowSystem::getSingleton()->addCommonMessage(common_[44].NothingMsg);
}

THUMB void TownFurnitureManager::nothingWater()
{
    ui_MsgSndSet(0x30);
    TownWindowSystem::getSingleton()->openCommonMessage();
    TownWindowSystem::getSingleton()->addCommonMessage(common_[45].checkMsg);
    TownWindowSystem::getSingleton()->addCommonMessage(common_[45].NothingMsg);
}

THUMB int TownFurnitureManager::checkCoffer(int uid)
{
    int index = getFurnitureIndex(uid);
    if (index < 0) {
        return 0;
    }
    if (g_Stage.getFurnFlag(list_[index].flagIndex)) {
        return 1;
    }
    if (list_[index].item != 0) {
        return 2;
    }
    if (list_[index].gold != 0) {
        return 3;
    }
    if (list_[index].encount != 0) {
        return 4;
    }
    return 1;
}

THUMB int TownFurnitureManager::getCofferType(int uid)
{
    int index = getFurnitureIndex(uid);
    if (index < 0) {
        return 0;
    }
    return list_[index].type;
}

THUMB void TownFurnitureManager::searchItem()
{
    floorItem_ = 0;
    for (int i = 0; i < 16; i++) {
        twinkle[i].enable = 0;
    }
    for (int i = 0; i < size_; i++) {
        int item = list_[i].item;
        if (item == 0 && list_[i].gold == 0 && list_[i].encount == 0) {
            continue;
        }
        if (item == 0x82 || item == 0x96 || item == 0x21 || item == 0x22) {
            continue;
        }
        if (item == 0x87 && !g_AreaFlag.check(0x63)) {
            continue;
        }
        if (list_[i].item == 0x9c && !g_AreaFlag.check(0x1a8)) {
            continue;
        }
        if ((list_[i].monster == 0xe2 || list_[i].item == 0xe3) && status::g_Story.chapter_ < 5) {
            continue;
        }
        if (g_Stage.getFurnFlag(list_[i].flagIndex)) {
            continue;
        }
        twinkle[floorItem_].position = TownStageManager::getSingleton()->getRiseupPos(list_[i].uid, common_[list_[i].type].type);
        floorItem_++;
    }
    TownPlayerManager::getSingleton()->setLock(1);
    phase_ = 1;
}

THUMB int TownFurnitureManager::searchFloorItem()
{
    int count = 0;
    for (int i = 0; i < size_; i++) {
        int item = list_[i].item;
        if (item == 0 && list_[i].gold == 0 && list_[i].encount == 0) {
            continue;
        }
        if (item == 0x82 || item == 0x96 || item == 0x21 || item == 0x22) {
            continue;
        }
        if (item == 0x87 && !g_AreaFlag.check(0x63)) {
            continue;
        }
        if (list_[i].item == 0x9c && !g_AreaFlag.check(0x1a8)) {
            continue;
        }
        if ((list_[i].monster == 0xe2 || list_[i].item == 0xe3) && status::g_Story.chapter_ < 5) {
            continue;
        }
        if (!g_Stage.getFurnFlag(list_[i].flagIndex)) {
            count++;
        }
    }
    return count;
}

THUMB void TownFurnitureManager::setTwinklePoint()
{
    for (int i = 0; i < floorItem_; i++) {
        if (unkfunc_020484ec((VecFx32*)&twinkle[i].position, (VecFx32*)&boxTestRotate, (VecFx32*)&boxTestScale, (VecFx32*)&boxTestParam, &boxTestRate)) {
            twinkle[i].enable = 1;
        }
    }
}

THUMB void TownFurnitureManager::drawTwinklePoint()
{
    int count = 0;
    for (int i = 0; i < floorItem_; i++) {
        if (twinkle[i].enable) {
            remiIndex_ = TownRiseupManager::getSingleton()->setupSprite(0x38c, twinkle[i].position, 1, count * 32);
            count++;
        }
    }
    if (remiIndex_ < 0) {
        TownPlayerManager::getSingleton()->setLock(0);
    }
}

THUMB void TownFurnitureManager::bootSlot(int uid)
{
    int index = getFurnitureIndex(uid);
    TownWindowSystem::getSingleton()->changeShopMenuPhase(0x1b);
    gMaterielMenu_SlotEnter.setSlotType((char)((list_[index].byte_1 & 0x1e) >> 1));
}

THUMB void TownFurnitureManager::mirrorTalk(int uid)
{
    unsigned int msg[32];
    int counter = 0;
    param::MirrorMessage* mirror = status::excelParam.mirrorMessage_;
    unsigned int* p;
    unsigned int i = 0;
    if (i < param::MirrorMessage::size_) {
        p = msg;
        do {
            int leader = mirror->leader;
            if (leader == 0 || leader == getLeaderIndex()) {
                *p++ = mirror->message;
                counter++;
            }
            mirror++;
            i++;
        } while (i < param::MirrorMessage::size_);
    }
    object_ = &msgObject_;
    object_->setup(uid, msg[dssrand::rand(counter)], &common_[19], 0);
}

THUMB int TownFurnitureManager::monsterEncount(int uid)
{
    int index = getFurnitureIndex(uid);
    if (index < 0) {
        return 0;
    }
    if (list_[index].encount != 0) {
        if (g_Stage.getFurnFlag(list_[index].flagIndex)) {
            return 0;
        }
        return list_[index].encount;
    }
    return 0;
}

THUMB int TownFurnitureManager::getLeaderIndex()
{
    status::g_Party.setNormalMode();
    for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
        if (!status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath()) {
            return status::g_Party.getPlayerIndex(i);
        }
    }
    return 0;
}
