#pragma ipa file
#include "ov003/btl/BattleExecVictory.hpp"
#include "ov003/btl/BattleActorManager2.hpp"
#include "ov003/btl/BattleMessage.hpp"
#include "ov003/btl/BattleActorAnimation.hpp"
#include "ov003/btl/BattleCamera.hpp"
#include "ov003/btl/BattleTransform.hpp"
#include "ov003/btl/SpecialMessageTask.hpp"
#include "ov003/status/MonsterPartyWithDraw.hpp"
#include "ov003/status/MonsterStatus.hpp"
#include "main/dss/Random.hpp"
#include "main/encount/Encount.hpp"
#include "main/global/Global.hpp"
#include "main/menu/TownMenu_MESSAGE.hpp"
#include "main/param/MonsterAnim.hpp"
#include "main/sound/SoundManager.hpp"
#include "main/status/BattleHistory.hpp"
#include "main/status/ExcelParam.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/text/TextAPI.hpp"

THUMB void btl::BattleExecEncount::setup()
{
    encount::Encount::getSingleton();
    BattleMessage::openEncountMessage();
    for (int i = 0; i < 4; i++) {
        int monsterIndex = encount::Encount::getSingleton()->monsterIndex_[i];
        int monsterCount = encount::Encount::getSingleton()->monsterCount_[i];
        if (monsterCount != 0) {
            TextAPI::setMACRO0(13, 0x60000000, monsterIndex);
            int special = 0;
            switch (monsterIndex) {
            case 0x8f:
            case 0x90:
            case 0x92:
            case 0x93:
            case 0xab:
            case 0xae:
            case 0xb3:
            case 0xb4:
            case 0xb5:
            case 0xbc:
            case 0xcd:
            case 0xce:
            case 0xcf:
            case 0xd0:
            case 0xd1:
            case 0xd2:
            case 0x12c:
            case 0x12d:
            case 0x12e:
            case 0x12f:
            case 0x130:
            case 0x131:
            case 0x132:
            case 0x133:
            case 0x134:
            case 0x135:
                special = 1;
                break;
            }
            if (special) {
                if (monsterCount == 1) {
                    BattleMessage::addEncountMessage(0xc3c77);
                }
                else {
                    BattleMessage::addEncountMessage(0xc3c79);
                }
            }
            else {
                if (monsterCount == 1) {
                    BattleMessage::addEncountMessage(0xc38ea);
                }
                else {
                    BattleMessage::addEncountMessage(0xc38ec);
                }
            }
        }
    }
    BattleAutoFeed::setCursor();
    BattleAutoFeed::setMessage();
    BattleAutoFeed::setEncountMessage();
}

THUMB bool btl::BattleExecEncount::isEnd()
{
    if (data_0210bb94.unkfunc_0205810c() == 13) {
        if (BattleAutoFeed::isEndEncountMessage()) {
            return true;
        }
    }
    else if (MenuAPI::isFinishMessageWindow()) {
        return true;
    }
    return false;
}

btl::BattleExecEncount btl::g_BattleExecEncount;

THUMB void btl::BattleExecStatus::setup()
{
    setupLast();
    isNext();
    BattleAutoFeed::setCursor();
    BattleAutoFeed::setMessage();
    BattleAutoFeed::setEncountMessage();
}

THUMB bool btl::BattleExecStatus::isEnd()
{
    if (data_0210bb94.unkfunc_0205810c() == 13) {
        if (BattleAutoFeed::isEndEncountMessage() && !isNext()) {
            return true;
        }
    }
    else if (MenuAPI::isFinishMessageWindow() && !isNext()) {
        return true;
    }
    return false;
}

THUMB void btl::BattleExecStatus::setupLast()
{
    index_ = 0;
    monsterCount_ = g_monster.getCount();
}

THUMB int btl::BattleExecStatus::isNext()
{
    status::MonsterStatus* monster;
    int result = 0;
    for (int i = index_; i < monsterCount_; i++) {
        monster = g_monster.getMonsterStatus(i);
        int monsterIndex = g_monster.getMonsterStatus(i)->characterIndex_;
        if (g_monster.getMonsterCountDeadOrAlive(monsterIndex) == 1 && !encount::Encount::getSingleton()->getMonsterCountName(monsterIndex)) {
            TextAPI::setMACRO0(13, 0x60000000, monsterIndex);
        }
        else {
            TextAPI::setMACRO0(13, 0x60000000, monsterIndex, g_monster.getMonsterStatus(i)->sortIndex_);
        }

        if (monster->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusSleep)) {
            BattleMessage::openEncountMessage();
            BattleMessage::addEncountMessage(0xc3906);
            result = 1;
            index_ = i + 1;
            break;
        }
        if (monster->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusConfusion)) {
            BattleMessage::openEncountMessage();
            BattleMessage::addEncountMessage(0xc390c);
            result = 1;
            index_ = i + 1;
            break;
        }
        if (monster->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusSpazz)) {
            BattleMessage::openEncountMessage();
            BattleMessage::addEncountMessage(0xc3912);
            result = 1;
            index_ = i + 1;
            break;
        }
    }
    return result;
}

// Unused on DS (dead-stripped); kept because their local initializers are part of the TU's data layout
THUMB void btl::BattleExecStatus::setupSleep()
{
    BattleMessage::openEncountMessage();
    int count = g_monster.getCount();
    int num[4] = { 0, 0, 0, 0 };
    int group[4] = { -1, -1, -1, -1 };
    for (int i = 0; i < count; i++) {
        int index = g_monster.getMonsterStatus(i)->characterIndex_;
        if (!g_monster.getMonsterStatus(i)->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusSleep)) {
            continue;
        }
        for (int j = 0; j < 4; j++) {
            if (group[j] == -1) {
                group[j] = index;
                num[j]++;
                break;
            }
            if (group[j] == index) {
                num[j]++;
                break;
            }
        }
    }
    if (group[1] != -1 && group[0] != group[1]) {
        TextAPI::setMACRO0(13, 0x60000000, 0x136);
        BattleMessage::addEncountMessage(0xc390a);
    }
    else if (num[0] >= 2) {
        TextAPI::setMACRO0(13, 0x60000000, group[0]);
        BattleMessage::addEncountMessage(0xc3908);
    }
    else if (num[0] == 1) {
        TextAPI::setMACRO0(13, 0x60000000, group[0]);
        BattleMessage::addEncountMessage(0xc3906);
    }
}

// Unused on DS (dead-stripped); kept because their local initializers are part of the TU's data layout
THUMB void btl::BattleExecStatus::setupConfusion()
{
    BattleMessage::openEncountMessage();
    int count = g_monster.getCount();
    int num[4] = { 0, 0, 0, 0 };
    int group[4] = { -1, -1, -1, -1 };
    for (int i = 0; i < count; i++) {
        int index = g_monster.getMonsterStatus(i)->characterIndex_;
        if (!g_monster.getMonsterStatus(i)->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusConfusion)) {
            continue;
        }
        for (int j = 0; j < 4; j++) {
            if (group[j] == -1) {
                group[j] = index;
                num[j]++;
                break;
            }
            if (group[j] == index) {
                num[j]++;
                break;
            }
        }
    }
    if (group[1] != -1 && group[0] != group[1]) {
        TextAPI::setMACRO0(13, 0x60000000, 0x136);
        BattleMessage::addEncountMessage(0xc3910);
    }
    else if (num[0] >= 2) {
        TextAPI::setMACRO0(13, 0x60000000, group[0]);
        BattleMessage::addEncountMessage(0xc390e);
    }
    else if (num[0] == 1) {
        TextAPI::setMACRO0(13, 0x60000000, group[0]);
        BattleMessage::addEncountMessage(0xc390c);
    }
}

// Unused on DS (dead-stripped); kept because their local initializers are part of the TU's data layout
THUMB void btl::BattleExecStatus::setupSpazz()
{
    BattleMessage::openEncountMessage();
    int count = g_monster.getCount();
    int num[4] = { 0, 0, 0, 0 };
    int group[4] = { -1, -1, -1, -1 };
    for (int i = 0; i < count; i++) {
        int index = g_monster.getMonsterStatus(i)->characterIndex_;
        if (!g_monster.getMonsterStatus(i)->haveStatusInfo_.statusChange_.isEnable(status::StatusChange::StatusSpazz)) {
            continue;
        }
        for (int j = 0; j < 4; j++) {
            if (group[j] == -1) {
                group[j] = index;
                num[j]++;
                break;
            }
            if (group[j] == index) {
                num[j]++;
                break;
            }
        }
    }
    if (group[1] != -1 && group[0] != group[1]) {
        TextAPI::setMACRO0(13, 0x60000000, 0x136);
        BattleMessage::addEncountMessage(0xc3916);
    }
    else if (num[0] >= 2) {
        TextAPI::setMACRO0(13, 0x60000000, group[0]);
        BattleMessage::addEncountMessage(0xc3914);
    }
    else if (num[0] == 1) {
        TextAPI::setMACRO0(13, 0x60000000, group[0]);
        BattleMessage::addEncountMessage(0xc3912);
    }
}

btl::BattleExecStatus btl::g_BattleExecStatus;

THUMB void btl::BattleExecFirstAttack::setup()
{
    int kind;
    FirstAttack firstAttack = BattleActorManager2::getSingleton()->getFirstAttack();
    if (firstAttack == 1) {
        kind = dssrand::rand(2);
    }
    else if (firstAttack == 2) {
        kind = dssrand::rand(2) + 2;
    }
    else {
        return;
    }

    BattleMessage::openEncountMessage();
    int message = 0;
    int callType = g_monster.getMonsterCallType();
    switch (kind) {
    case 0:
        if (callType == 0) message = 0xc38ee;
        if (callType == 1) message = 0xc38f0;
        if (callType == 2) message = 0xc38f2;
        break;
    case 1:
        if (callType == 0) message = 0xc38f4;
        if (callType == 1) message = 0xc38f6;
        if (callType == 2) message = 0xc38f8;
        break;
    case 2:
        if (callType == 0) message = 0xc38fa;
        if (callType == 1) message = 0xc38fc;
        if (callType == 2) message = 0xc38fe;
        break;
    case 3:
        if (callType == 0) message = 0xc3900;
        if (callType == 1) message = 0xc3902;
        if (callType == 2) message = 0xc3904;
        break;
    }
    if (callType == 2) {
        TextAPI::setMACRO0(13, 0x60000000, 0x136);
    }
    else {
        TextAPI::setMACRO0(13, 0x60000000, g_monster.getMonsterCallIndex());
    }
    BattleMessage::addEncountMessage(message);
    BattleAutoFeed::setCursor();
    BattleAutoFeed::setMessage();
    BattleAutoFeed::setEncountMessage();
}

THUMB bool btl::BattleExecFirstAttack::isEnd()
{
    if (data_0210bb94.unkfunc_0205810c() == 13) {
        if (BattleAutoFeed::isEndEncountMessage()) {
            return true;
        }
    }
    else if (MenuAPI::isFinishMessageWindow()) {
        return true;
    }
    return false;
}

btl::BattleExecFirstAttack btl::g_BattleExecFirstAttack;

THUMB void btl::BattleExecVictory00::setup()
{
    int message = 0;
    if ((BattleActorManager2::getSingleton()->winningStatus_ & 0xffff0000) == 0x40000) {
        TextAPI::setMACRO0(13, 0x60000000, (unsigned short)BattleActorManager2::getSingleton()->winningStatus_);
        message = 0xc3c2d;
    }
    else if ((BattleActorManager2::getSingleton()->winningStatus_ & 0xffff0000) == 0x10000) {
        TextAPI::setMACRO0(13, 0x60000000, (unsigned short)BattleActorManager2::getSingleton()->winningStatus_);
        message = 0xc3c2f;
    }
    else if ((BattleActorManager2::getSingleton()->winningStatus_ & 0xffff0000) == 0x20000) {
        TextAPI::setMACRO0(13, 0x60000000, 0x136);
        message = 0xc3c31;
    }
    BattleMessage::setMessage(message, 0, 0, 0);
    BattleAutoFeed::setMessageSend();
}

THUMB void btl::BattleExecVictory01::setup()
{
    TextAPI::setMACRO0(0x2f, 0xf0000000, status::g_Party.getBattleExp());
    BattleMessage::setMessage(0xc3c3f, 0, 0, 0);
    BattleAutoFeed::setMessageSend();
    if (status::g_Party.getLevelupPlayer() == -1 && status::g_Party.getBattleGold() == 0) {
        BattleAutoFeed::setDisableCursor(1);
        data_020ed1bc.setMessageLastCursor(false);
    }
}

THUMB void btl::BattleExecVictory02::setup()
{
    int type = 0;
    int message;
    for (int i = 0; i < 4; i++) {
        int monsterIndex = encount::Encount::getSingleton()->monsterIndex_[i];
        int monsterCount = encount::Encount::getSingleton()->monsterCount_[i];
        if (monsterCount != 0) {
            if (i == 0 && monsterCount == 1) {
                type = 0;
                TextAPI::setMACRO0(13, 0x60000000, monsterIndex);
            }
            else {
                type = 1;
                TextAPI::setMACRO0(13, 0x60000000, monsterIndex);
            }
            if (i == 1 && monsterCount != 0) {
                type = 2;
                TextAPI::setMACRO0(13, 0x60000000, monsterIndex);
            }
        }
    }
    switch (type) {
    case 0:
        message = 0xc3c39;
        break;
    case 1:
        message = 0xc3c3b;
        break;
    case 2:
        TextAPI::setMACRO0(13, 0x60000000, 0x136);
        message = 0xc3c3d;
        break;
    default:
        TextAPI::setMACRO0(13, 0x60000000, 0x136);
        message = 0xc3c3d;
        break;
    }
    BattleAutoFeed::setCursor();
    BattleMessage::setMessage(message, 0, 0, 0);
    BattleAutoFeed::setMessageSend();
    BattleAutoFeed::setDisableCursor(1);
}

THUMB void btl::BattleExecVictory03::setup()
{
    int type = 0;
    int firstIndex = 0;
    int message;
    for (int i = 0; i < 4; i++) {
        int monsterIndex = encount::Encount::getSingleton()->monsterIndex_[i];
        int monsterCount = encount::Encount::getSingleton()->monsterCount_[i];
        if (monsterCount != 0) {
            if (i == 0) {
                if (monsterCount == 1) {
                    type = 0;
                    TextAPI::setMACRO0(13, 0x60000000, monsterIndex);
                    firstIndex = monsterIndex;
                }
                else {
                    type = 1;
                    TextAPI::setMACRO0(13, 0x60000000, monsterIndex);
                    firstIndex = monsterIndex;
                }
            }
            if (i > 0) {
                if (firstIndex != monsterIndex) {
                    type = 2;
                    TextAPI::setMACRO0(13, 0x60000000, monsterIndex);
                }
                else {
                    type = 1;
                    TextAPI::setMACRO0(13, 0x60000000, monsterIndex);
                }
            }
        }
    }
    switch (type) {
    case 0:
        message = 0xc3c33;
        break;
    case 1:
        message = 0xc3c35;
        break;
    case 2:
        TextAPI::setMACRO0(13, 0x60000000, 0x136);
        message = 0xc3c37;
        break;
    default:
        TextAPI::setMACRO0(13, 0x60000000, 0x136);
        message = 0xc3c37;
        break;
    }
    BattleAutoFeed::setCursor();
    BattleMessage::setMessage(message, 0, 0, 0);
    BattleAutoFeed::setMessageSend();
    BattleAutoFeed::setDisableCursor(1);
}

THUMB void btl::BattleExecVictory20::setup()
{
    int gold = status::g_Party.getBattleGold();
    if (gold != 0) {
        TextAPI::setMACRO0(0x32, 0xf0000000, gold);
        BattleMessage::setMessage(0xc3c51, 0, 0, 0);
        BattleAutoFeed::setMessageSend();
        if (g_monster.getDropItem() == 0) {
            BattleAutoFeed::setDisableCursor(1);
        }
    }
}

THUMB void btl::BattleExecVictory30::setup()
{
    TextAPI::setMACRO0(0x12, 0x60000000, monsterIndex_);
    BattleMessage::setMessage(0xc3c54, 0, 0, 0);
    BattleAutoFeed::setMessageSend();
}

THUMB void btl::BattleExecVictory31::setup()
{
    TextAPI::setMACRO0(12, 0x50000000, status::g_Party.getPlayerStatus(status::g_Party.getLeaderIndex())->haveStatusInfo_.haveStatus_.playerIndex_);
    BattleMessage::setMessage(0xc3c56, 0, 0, 0);
    BattleAutoFeed::setMessageSend();
}

THUMB void btl::BattleExecVictory31a::setup()
{
    TextAPI::setMACRO0(10, 0x40000000, itemIndex_);
    BattleMessage::setMessage(0xc3c58, 0, 0, 0);
    BattleAutoFeed::setMessageSend();
}

THUMB void btl::BattleExecVictory32::setup()
{
    int index = status::g_Party.giveItem(itemIndex_);
    if (index != -1) {
        TextAPI::setMACRO0(0x12, 0x50000000, status::g_Party.getPlayerStatus(index)->haveStatusInfo_.haveStatus_.playerIndex_);
        TextAPI::setMACRO0(10, 0x40000000, itemIndex_);
        BattleMessage::setMessage(0xc3c5a, 0, 0, 0);
    }
    else {
        TextAPI::setMACRO0(10, 0x40000000, itemIndex_);
        BattleMessage::setMessage(0xc3c5d, 0, 0, 0);
    }
    BattleAutoFeed::setMessageSend();
    BattleAutoFeed::setDisableCursor(1);
}

THUMB void btl::BattleExecVictory33::setup()
{
    TextAPI::setMACRO0(1, 0x50000000, playerIndex_);
    BattleMessage::setMessage(0xc3c1f, 0, 0, 0);
    BattleAutoFeed::setMessage();
}

THUMB bool btl::BattleExecVictory33::isEnd()
{
    if (BattleAutoFeed::isEndMessage() != 0) {
        return true;
    }
    return false;
}

THUMB void btl::BattleExecVictory34::setup()
{
    TextAPI::setMACRO0(1, 0x50000000, playerIndex_);
    BattleMessage::setMessage(0xc3c1f, 0, 0, 0);
    BattleAutoFeed::setMessage();
}

THUMB bool btl::BattleExecVictory34::isEnd()
{
    if (BattleAutoFeed::isEndMessage() != 0) {
        return true;
    }
    return false;
}

THUMB void btl::BattleExecVictory35::setup()
{
    TextAPI::setMACRO0(1, 0x50000000, playerIndex_);
    BattleMessage::setMessage(0xc3c1f, 0, 0, 0);
    BattleAutoFeed::setMessage();
}

THUMB bool btl::BattleExecVictory35::isEnd()
{
    if (BattleAutoFeed::isEndMessage() != 0) {
        return true;
    }
    return false;
}

THUMB void btl::BattleExecVictory36::setup()
{
    TextAPI::setMACRO0(1, 0x50000000, playerIndex_);
    BattleMessage::setMessage(0xc3c1f, 0, 0, 0);
    BattleAutoFeed::setMessage();
}

THUMB bool btl::BattleExecVictory36::isEnd()
{
    if (BattleAutoFeed::isEndMessage() != 0) {
        return true;
    }
    return false;
}

THUMB void btl::BattleExecVictory37::setup()
{
    status::g_Party.setBattleMode();
    if (status::g_Party.getCount() == 1) {
        BattleMessage::setMessage(0xc3c21, 0, 0, 0);
    }
    else {
        BattleMessage::setMessage(0xc3c24, 0, 0, 0);
    }
    BattleAutoFeed::setMessageSend();
    BattleAutoFeed::setDisableCursor(1);
}

THUMB void btl::BattleExecVictory37::cleanup() {}

THUMB void btl::BattleExecVictory38::setup()
{
    SoundManager::play(0x17, 0xf);
    data_020f21f8.state_ = GlobalFade::FADE_NONE;
    data_020f21f8.count_ = 0;
    data_020f21f8.frames_ = 30;
    data_020f21f8.sprite_[0].setColor(0, 0, 0);
    data_020f21f8.sprite_[1].setColor(0, 0, 0);
    data_0210bc18.unkfunc_02058294(&data_020f21f8);
    MenuAPI::closeMenu();
    counter_ = 0;
}

THUMB bool btl::BattleExecVictory38::isEnd()
{
    if (counter_ > 120) {
        return true;
    }
    counter_++;
    return false;
}

THUMB void btl::BattleExecVictory39::setup()
{
    BattleMessage::setMessageInTown(0xc3c27, 0, 0, 0);
    BattleAutoFeed::disableAutoFeed();
    BattleAutoFeed::setMessageSend();
}

THUMB void btl::BattleExecVictory40::setup()
{
    g_Global.fadeOutBlack(30);
    counter_ = 0;
}

THUMB void btl::BattleExecVictory40::cleanup() {}

THUMB bool btl::BattleExecVictory40::isEnd()
{
    if (counter_ > 30) {
        return true;
    }
    if (counter_ == 25) {
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
        data_020f21f8.frames_ = 5;
        data_0210bc18.unkfunc_02058294(&data_020f21f8);
    }
    counter_++;
    return false;
}

static const int VELORINMAN_POS2[4] = { -0xf6cc, -0x5244, 0x5244, 0xf6cc };
static int VELORINMAN_POS[4];
static int nowRealVelorinmanIdx;
static int preRealVelorinmanIdx = -1;
static dss::Fix32Vector3 verolinmanPos[4];
const int velorinmanUnknown = 0x2f4;   // unreferenced (dead-stripped); needed for the TU data layout

THUMB void InitBattleExecEvent00()
{
    nowRealVelorinmanIdx = 0;
    for (int i = 0; i < 4; i++) {
        VELORINMAN_POS[i] = 0;
    }
    preRealVelorinmanIdx = -1;
}

THUMB int btl::BattleExecEvent00::getRealVelorinman()
{
    return nowRealVelorinmanIdx;
}

THUMB void btl::BattleExecEvent00::setup()
{
    counter_ = 0;
    for (int i = 0; i < 4; i++) {
        setupMonster(i);
    }
    if (preRealVelorinmanIdx == -1) {
        for (int i = 0; i < 4; i++) {
            dss::Fix32Vector3 pos(0, 0, 0);
            pos.vx.value = VELORINMAN_POS[i];
            btl::BattleMonsterDraw2::getSingleton()->monsters_[i].setPosition(pos);
            btl::BattleMonsterDraw2::getSingleton()->monsters_[i].startAnimation(0x25);
        }
    }
    else {
        VELORINMAN_POS[nowRealVelorinmanIdx] = VELORINMAN_POS2[nowRealVelorinmanIdx];
        for (int i = 0; i < 4; i++) {
            if (!(g_monster.getMonsterStatus(i)->eventFlag_.flag_ & 1)) {
                btl::BattleMonsterDraw2::getSingleton()->monsters_[i].startAnimation(0x24);
            }
        }
    }
    int count = g_monster.getCount();
    for (int i = 0; i < count; i++) {
        g_monster.getMonsterStatus(i)->eventFlag_.flag_ &= ~1;
    }
    preRealVelorinmanIdx = nowRealVelorinmanIdx;
    nowRealVelorinmanIdx = dssrand::rand(4);
    g_monster.getMonsterStatus(nowRealVelorinmanIdx)->eventFlag_.flag_ |= 1;
    TextAPI::setMACRO0(1, 0x60000000, 0xaa);
    SoundManager::playSe(0x40b, 0);
    data_020ed1bc.openMessageForBATTLE();
    data_020ed1bc.addMessage(0xc3a64);
    data_020ed1bc.setMessageLastCursor(false);
}

THUMB void btl::BattleExecEvent00::cleanup()
{
    for (int i = 0; i < 4; i++) {
        verolinmanPos[i].set(0, 0, 0);
        verolinmanPos[i].vx.value = VELORINMAN_POS[preRealVelorinmanIdx];
        int ctrlId = g_monster.getMonsterInGroup(i, 0)->ctrlId_;
        btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].setPosition(verolinmanPos[i]);
        btl::BattleMonsterDraw2::getSingleton()->monsters_[i].startAnimation(0x25);
    }
}

THUMB bool btl::BattleExecEvent00::isEnd()
{
    if (++counter_ > 60) {
        return true;
    }
    return false;
}

THUMB int btl::BattleExecEvent00::setupMonster(int index)
{
    if (g_monster.getMonsterCountInGroup(index) == 0) {
        return g_monster.add(index, 0xaa, 1);
    }
    return g_monster.getMonsterInGroup(index, 0)->ctrlId_;
}

THUMB void btl::BattleExecEvent00b::setup()
{
    counter_ = 0;
}

THUMB void btl::BattleExecEvent00b::cleanup()
{
    for (int i = 0; i < 4; i++) {
        verolinmanPos[i].set(0, 0, 0);
        verolinmanPos[i].vx.value = VELORINMAN_POS2[i];
        int ctrlId = g_monster.getMonsterInGroup(i, 0)->ctrlId_;
        btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].setPosition(verolinmanPos[i]);
    }
}

THUMB bool btl::BattleExecEvent00b::isEnd()
{
    counter_++;
    move();
    if (counter_ > 120) {
        return true;
    }
    return false;
}

THUMB void btl::BattleExecEvent00b::move()
{
    for (int i = 0; i < 4; i++) {
        int ctrlId;
        verolinmanPos[i].set(0, 0, 0);
        int start = VELORINMAN_POS[preRealVelorinmanIdx];
        verolinmanPos[i].vx.value = start + (VELORINMAN_POS2[i] - start) * counter_ / 120;
        verolinmanPos[i].vz.value += i * 0x400;
        ctrlId = g_monster.getMonsterInGroup(i, 0)->ctrlId_;
        btl::BattleMonsterDraw2::getSingleton()->monsters_[ctrlId].setPosition(verolinmanPos[i]);
    }
}

THUMB void btl::BattleExecEvent01::setup() {}

THUMB void btl::BattleExecEvent01::cleanup() {}

THUMB bool btl::BattleExecEvent01::isEnd()
{
    if (btl::BattleEffectManager::getSingleton()->isAllEnd()) {
        return true;
    }
    return false;
}

THUMB void btl::BattleExecEvent02::setup()
{
    status_ = 0;
    int index = g_monster.getMonsterStatus(0)->haveBattleStatus_.index_;
    if ((unsigned int)(index - 0x134) > 1) {
        GameMonster::setupTexture(btl::BattleTransform::getDummyFromMonster(index));
    }
}

THUMB void btl::BattleExecEvent02::cleanup() {}

THUMB bool btl::BattleExecEvent02::isEnd()
{
    execChange();
    if (status_ == 6) {
        return true;
    }
    return false;
}

THUMB void btl::BattleExecEvent02::execChange()
{
    int index = g_monster.getMonsterStatus(0)->haveBattleStatus_.index_;
    if (index == 0xd2) {
        status_ = 6;
        return;
    }
    switch (status_) {
    case 0:
        if (!BattleCamera::getSingleton()->isCameraAnimation()) {
            if (index == 0xd1) {
                SoundManager::battleStop();
            }
            int ctrlId = g_monster.getCtrlId(0);
            btl::BattleMonsterDraw2::getSingleton()->cleanup(ctrlId);
            int dummy = btl::BattleTransform::getDummyFromMonster(index);
            g_monster.setCtrlId(0, btl::BattleMonsterDraw2::getSingleton()->setup(0, dummy));
            status_ = 1;
        }
        break;
    case 1:
        btl::BattleTransform::getSingleton()->setup(index);
    case 2: {
        GameMonster::cleanupTexture(btl::BattleTransform::getDummyFromTrans());
        int ctrlId = g_monster.getCtrlId(0);
        btl::BattleMonsterDraw2::getSingleton()->cleanup(ctrlId);
        status_ = 3;
        btl::BattleTransform::getSingleton()->draw();
        break;
    }
    case 3:
        btl::BattleTransform::getSingleton()->draw();
        if (btl::BattleTransform::getSingleton()->isEnd()) {
            GameMonster::setupTexture(btl::BattleTransform::getDummyFromTrans() + 1);
            btl::BattleTransform::getSingleton()->cleanup();
            status_ = 4;
        }
        break;
    case 4: {
        int dummy = btl::BattleTransform::getDummyFromTrans();
        g_monster.setCtrlId(0, btl::BattleMonsterDraw2::getSingleton()->setup(0, dummy + 1));
        status_ = 5;
        break;
    }
    case 5:
        if (btl::BattleTransform::getSingleton()->startNext()) {
            status_ = 2;
        }
        else {
            status_ = 6;
        }
        btl::BattleTransform::getSingleton()->draw();
        break;
    case 6:
        break;
    }
}

THUMB void btl::BattleExecEvent03::setup()
{
    counter_ = 0;
    enable_ = 1;
}

THUMB void btl::BattleExecEvent03::cleanup()
{
    if (enable_) {
        g_monster.getMonsterStatus(0);
        GameMonster::cleanupTexture(btl::BattleTransform::getDummyFromTrans() + 1);
    }
}

THUMB void btl::BattleExecEvent03::endTransform()
{
    if (g_monster.getMonsterStatus(0)->haveStatusInfo_.isDeath()) {
        int index = g_monster.getMonsterStatus(0)->haveBattleStatus_.index_;
        int next = 0;
        switch (index) {
        case 0xae:
            next = 0xcd;
            break;
        case 0xcd:
            next = 0xce;
            break;
        case 0xce:
            next = 0xcf;
            break;
        case 0xcf:
            next = 0xd0;
            break;
        case 0xd0:
            next = 0xd1;
            break;
        case 0xd1:
            SoundManager::lastBossPlay();
            next = 0xd2;
            break;
        case 0xd2:
            enable_ = 0;
            counter_ = 0x1f;
            g_monster.getMonsterStatus(0)->haveStatusInfo_.setBossDeathFlag(true);
            BattleActorManager2::getSingleton()->retireActor();
            break;
        }
        if (next != 0) {
            g_monster.getMonsterStatus(0)->haveStatusInfo_.statusChange_.clear();
            g_monster.getMonsterStatus(0)->haveBattleStatus_.newBaseChangeMonster(next);
            g_monster.getMonsterStatus(0)->characterIndex_ = next;
            g_monster.getMonsterStatus(0)->setStartStatus();
            int ctrlId = g_monster.getCtrlId(0);
            btl::BattleMonsterDraw2::getSingleton()->cleanup(ctrlId);
            g_monster.setCtrlId(0, btl::BattleMonsterDraw2::getSingleton()->setup(0, next));
            if (next == 0xd2) {
                g_BattleExecDeathPissaroMahokanta.flag_ = 1;
            }
        }
    }
}

THUMB bool btl::BattleExecEvent03::isEnd()
{
    if (!enable_) {
        return true;
    }
    if (!g_monster.getMonsterStatus(0)->haveStatusInfo_.isDeath()) {
        return true;
    }
    if (counter_ == 0) {
        endTransform();
    }
    else if (counter_ >= 1) {
        return true;
    }
    counter_++;
    return false;
}

THUMB void btl::BattleExecEvent11::setup() {}

THUMB void btl::BattleExecEvent11::cleanup() {}

THUMB bool btl::BattleExecEvent11::isEnd()
{
    return true;
}

THUMB void btl::BattleExecEvent12::setup()
{
    status_ = 0;
    int index = g_monster.getMonsterStatus(0)->haveBattleStatus_.index_;
    if ((unsigned int)(index - 0x134) > 1) {
        GameMonster::setupTexture(btl::BattleTransform::getDummyFromMonster(index));
    }
}

THUMB void btl::BattleExecEvent12::cleanup() {}

THUMB bool btl::BattleExecEvent12::isEnd()
{
    execChange();
    if (status_ == 6) {
        return true;
    }
    return false;
}

THUMB void btl::BattleExecEvent12::execChange()
{
    int index = g_monster.getMonsterStatus(0)->haveBattleStatus_.index_;
    if (index == 0x134 || index == 0x135) {
        status_ = 6;
        return;
    }
    switch (status_) {
    case 0:
        if (!BattleCamera::getSingleton()->isCameraAnimation()) {
            if (index == 0x131) {
                SoundManager::battleStop();
            }
            int ctrlId = g_monster.getCtrlId(0);
            btl::BattleMonsterDraw2::getSingleton()->cleanup(ctrlId);
            int dummy = btl::BattleTransform::getDummyFromMonster(index);
            g_monster.setCtrlId(0, btl::BattleMonsterDraw2::getSingleton()->setup(0, dummy));
            status_ = 1;
        }
        break;
    case 1:
        btl::BattleTransform::getSingleton()->setup(index);
    case 2: {
        GameMonster::cleanupTexture(btl::BattleTransform::getDummyFromTrans());
        int ctrlId = g_monster.getCtrlId(0);
        btl::BattleMonsterDraw2::getSingleton()->cleanup(ctrlId);
        status_ = 3;
        btl::BattleTransform::getSingleton()->draw();
        break;
    }
    case 3:
        btl::BattleTransform::getSingleton()->draw();
        if (btl::BattleTransform::getSingleton()->isEnd()) {
            GameMonster::setupTexture(btl::BattleTransform::getDummyFromTrans() + 1);
            btl::BattleTransform::getSingleton()->cleanup();
            status_ = 4;
        }
        break;
    case 4: {
        int dummy = btl::BattleTransform::getDummyFromTrans();
        g_monster.setCtrlId(0, btl::BattleMonsterDraw2::getSingleton()->setup(0, dummy + 1));
        status_ = 5;
        break;
    }
    case 5:
        if (btl::BattleTransform::getSingleton()->startNext()) {
            status_ = 2;
        }
        else {
            status_ = 6;
        }
        btl::BattleTransform::getSingleton()->draw();
        break;
    case 6:
        break;
    }
}

THUMB void btl::BattleExecEvent13::setup()
{
    counter_ = 0;
    enable_ = 1;
}

THUMB void btl::BattleExecEvent13::cleanup()
{
    if (enable_) {
        g_monster.getMonsterStatus(0);
        GameMonster::cleanupTexture(btl::BattleTransform::getDummyFromTrans() + 1);
    }
}

THUMB void btl::BattleExecEvent13::endTransform()
{
    if (g_monster.getMonsterStatus(0)->haveStatusInfo_.isDeath()) {
        int index = g_monster.getMonsterStatus(0)->haveBattleStatus_.index_;
        int next = 0;
        switch (index) {
        case 0x130:
            next = 0x131;
            break;
        case 0x131:
            SoundManager::lastBossPlay();
            next = 0x132;
            break;
        case 0x132:
        case 0x133:
            next = 0x134;
            break;
        case 0x134:
        case 0x135:
            enable_ = 0;
            counter_ = 0x1f;
            g_monster.getMonsterStatus(0)->haveStatusInfo_.setBossDeathFlag(true);
            BattleActorManager2::getSingleton()->retireActor();
            break;
        }
        if (next != 0) {
            g_monster.getMonsterStatus(0)->haveStatusInfo_.statusChange_.clear();
            g_monster.getMonsterStatus(0)->haveBattleStatus_.newBaseChangeMonster(next);
            g_monster.getMonsterStatus(0)->characterIndex_ = next;
            g_monster.getMonsterStatus(0)->setStartStatus();
            int ctrlId = g_monster.getCtrlId(0);
            btl::BattleMonsterDraw2::getSingleton()->cleanup(ctrlId);
            g_monster.setCtrlId(0, btl::BattleMonsterDraw2::getSingleton()->setup(0, next));
        }
    }
}

THUMB bool btl::BattleExecEvent13::isEnd()
{
    if (!enable_) {
        return true;
    }
    if (!g_monster.getMonsterStatus(0)->haveStatusInfo_.isDeath()) {
        return true;
    }
    if (counter_ == 0) {
        endTransform();
    }
    else if (counter_ >= 1) {
        return true;
    }
    counter_++;
    return false;
}

THUMB void btl::BattleExecEvent14::setup()
{
    TextAPI::setMACRO0(1, 0x60000000, 0xae);
    TextAPI::setMACRO0(0x11, 0x70000000, 0x23);
    BattleMessage::setMessage(0xc3938, 0, 0, 0);
    BattleAutoFeed::setMessage();
    int ctrlId = g_monster.getCtrlId(0);
    if (param::MonsterAnim::getAnimData(status::excelParam.monsterAnim_, btl::BattleMonsterDraw2::getSingleton()->monsters_[0].monsterIndex_, 0x23, 5) >= 0) {
        btl::BattleMonsterDraw2::getSingleton()->monsters_[0].startAnimation(0x23, 5);
    }
}

THUMB bool btl::BattleExecEvent14::isEnd()
{
    bool animation = BattleCamera::getSingleton()->isCameraAnimation();
    if (BattleAutoFeed::isEndMessage() && !animation) {
        return true;
    }
    return false;
}

THUMB void btl::BattleExecEvent15::setup()
{
    TextAPI::setMACRO0(1, 0x60000000, 0xae);
    BattleMessage::setMessage(0xc3a8f, 0, 0, 0);
    BattleAutoFeed::setMessage();
    int ctrlId = g_monster.getCtrlId(0);
    int animIndex = param::MonsterAnim::getAnimData(status::excelParam.monsterAnim_, btl::BattleMonsterDraw2::getSingleton()->monsters_[0].monsterIndex_, 0x23, 5);
    if (animIndex >= 0) {
        param::MonsterAnim* anim = &status::excelParam.monsterAnim_[animIndex];
        int effect = btl::BattleEffectManager::getSingleton()->setupEffect(0x67);
        if (effect < 0) {
            return;
        }
        btl::BattleEffectManager::getSingleton()->setSpecialTarget(effect, ctrlId, anim->animfile);
        btl::BattleEffectManager::getSingleton()->setWaitTime(effect, anim->hitframe);
    }
    counter = 0;
}

THUMB bool btl::BattleExecEvent15::isEnd()
{
    if (BattleAutoFeed::isEndMessage() && btl::BattleEffectManager::getSingleton()->isEnd()) {
        if (counter >= 60) {
            return true;
        }
        counter++;
        return false;
    }
    return false;
}

THUMB void btl::BattleExecMonsterEscape::initialize()
{
    ExecTaskManager::initialize();
    resister(0, &battleExecVictory02);
}

THUMB void btl::BattleExecMonsterEscape::terminate() {}

btl::BattleExecMonsterEscape btl::g_BattleExecMonsterEscape;

THUMB void btl::BattleExecMonsterDisappear::initialize()
{
    ExecTaskManager::initialize();
    resister(0, &battleExecVictory03);
}

THUMB void btl::BattleExecMonsterDisappear::terminate() {}

btl::BattleExecMonsterDisappear btl::g_BattleExecMonsterDisappear;

THUMB void btl::BattleExecDefeatMonster::initialize()
{
    ExecTaskManager::initialize();
    if (status::g_Party.getBattleExp() != 0) {
        status::g_Party.reflectBattleExp();
        resister(0, &battleExecVictory00);
        resister(1, &battleExecVictory01);
    }
    else {
        resister(0, &battleExecVictory00);
    }
}

THUMB void btl::BattleExecDefeatMonster::terminate()
{
    status::g_Party.battleExp_ = 0;
    status::g_Party.battleMonsterCount_ = 0;
}

btl::BattleExecDefeatMonster btl::g_BattleExecDefeatMonster;

THUMB void btl::BattleExecGold::initialize()
{
    ExecTaskManager::initialize();
    status::g_Party.reflectBattleGold();
    resister(0, &battleExecVictory20);
}

THUMB void btl::BattleExecGold::terminate()
{
    status::g_Party.battleGold_ = 0;
}

btl::BattleExecGold btl::g_BattleExecGold;

THUMB void btl::BattleExecItem::initialize()
{
    ExecTaskManager::initialize();
    int item = g_monster.getDropItem();
    if (item != 0) {
        resister(0, &battleExecVictory30);
        resister(1, &battleExecVictory31);
        resister(2, &battleExecVictory31a);
        resister(3, &battleExecVictory32);
        battleExecVictory30.monsterIndex_ = g_monster.getDropItemMonster();
        battleExecVictory31a.itemIndex_ = item;
        battleExecVictory32.itemIndex_ = item;
    }
}

THUMB void btl::BattleExecItem::terminate() {}

btl::BattleExecItem btl::g_BattleExecItem;

THUMB void btl::BattleExecReorder::initialize()
{
    status::g_Party.forceReorder();
    ExecTaskManager::initialize();
    status::g_Party.setBattleMode();
    int count = status::g_Party.getCount();
    int n = 0;
    for (int i = 0; i < count; i++) {
        if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.isPlayer_) {
            if (n == 0 && !status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath()) {
                resister(n, &battleExecVictory33);
                n++;
                battleExecVictory33.playerIndex_ = status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_;
            }
            else if (n == 1 && !status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath()) {
                resister(n, &battleExecVictory34);
                n++;
                battleExecVictory34.playerIndex_ = status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_;
            }
            else if (n == 2 && !status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath()) {
                resister(n, &battleExecVictory35);
                n++;
                battleExecVictory35.playerIndex_ = status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_;
            }
            else if (n == 3 && !status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath()) {
                resister(n, &battleExecVictory36);
                n++;
                battleExecVictory36.playerIndex_ = status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_;
            }
        }
    }
}

THUMB void btl::BattleExecReorder::terminate() {}

btl::BattleExecReorder btl::g_BattleExecReorder;

THUMB void btl::BattleExecDemolition::initialize()
{
    ExecTaskManager::initialize();
    resister(0, &battleExecVictory37);
    resister(1, &battleExecVictory38);
    resister(2, &battleExecVictory39);
    resister(3, &battleExecVictory40);
}

THUMB void btl::BattleExecDemolition::terminate() {}

btl::BattleExecDemolition btl::g_BattleExecDemolition;

THUMB void btl::BattleExecVelorinman::initialize()
{
    ExecTaskManager::initialize();
    resister(0, &battleExecEvent00);
    resister(1, &battleExecEvent00b);
}

btl::BattleExecVelorinman btl::g_BattleExecVelorinman;

THUMB void btl::BattleExecDeathPissaro::initialize()
{
    ExecTaskManager::initialize();
    resister(0, &battleExecEvent01);
    resister(1, &battleExecEvent02);
    resister(2, &battleExecEvent03);
}

THUMB void btl::BattleExecDeathPissaro::terminate()
{
    ExecTaskManager::terminate();
}

btl::BattleExecDeathPissaro btl::g_BattleExecDeathPissaro;

THUMB void btl::BattleExecEvilPriest::initialize()
{
    ExecTaskManager::initialize();
    resister(0, &battleExecEvent11);
    resister(1, &battleExecEvent12);
    resister(2, &battleExecEvent13);
}

THUMB void btl::BattleExecEvilPriest::terminate()
{
    ExecTaskManager::terminate();
}

btl::BattleExecEvilPriest btl::g_BattleExecEvilPriest;

THUMB void btl::BattleExecDeathPissaroMahokanta::initialize()
{
    ExecTaskManager::initialize();
    resister(0, &battleExecEvent14);
    resister(1, &battleExecEvent15);
}

THUMB void btl::BattleExecDeathPissaroMahokanta::terminate()
{
    ExecTaskManager::terminate();
}

btl::BattleExecDeathPissaroMahokanta btl::g_BattleExecDeathPissaroMahokanta;

THUMB void btl::BattleExecEscape::setup()
{
    TextAPI::setMACRO0(12, 0x50000000, status::g_Party.getPlayerStatus(status::g_Party.getLeaderIndex())->haveStatusInfo_.haveStatus_.playerIndex_);
    status::g_Party.setBattleMode();
    int outside = 0;
    int count = status::g_Party.getCount();
    for (int i = 0; i < count; i++) {
        if (!status::g_Party.isInsideCarriage(i)) {
            outside++;
        }
    }
    if (BattleActorManager2::getSingleton()->escapeSuccess_) {
        if (outside == 1) {
            BattleMessage::setMessage(0xc3a57, 0, 0, 0);
        }
        else {
            BattleMessage::setMessage(0xc3a5a, 0, 0, 0);
        }
        status::g_BattleHistory.historyType_ = status::BattleHistory::RightNow;
        status::g_BattleHistory.regenesisChapterEscapeCount();
    }
    else if (outside == 1) {
        BattleMessage::setMessage(0xc3a57, 0xc3a5d, 0, 0);
    }
    else {
        BattleMessage::setMessage(0xc3a5a, 0xc3a5d, 0, 0);
    }
    SoundManager::playSe(0x198, 0);
    BattleAutoFeed::setCursor();
    BattleAutoFeed::setMessage();
}

THUMB bool btl::BattleExecEscape::isEnd()
{
    if (BattleAutoFeed::isEndMessage() != 0) {
        return true;
    }
    return false;
}

btl::BattleExecEscape btl::g_BattleExecEscape;
