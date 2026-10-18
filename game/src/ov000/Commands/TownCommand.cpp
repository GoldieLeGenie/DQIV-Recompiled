#pragma ipa file

#include "ov000/Commands/TownCommand.hpp"
#include "main/dss/RenderObject.hpp"
#include "main/dss/Display.hpp"
#include "ov000/town/TownWindowSystem.hpp"
#include "ov000/town/TownPlayerManager.hpp"
#include "main/status/StageStatus.hpp"
#include "main/cmn/CommonPartyInfo.hpp"
#include "main/cmn/HengeNoTsueManager.hpp"
#include "main/cmn/TalkSoundManager.hpp"
#include "main/cmn/PartyTalk.hpp"
#include "ov000/town/TownIkadaAction2.hpp"
#include "ov000/Commands/TownScriptCommand.hpp"
#include "ov000/town/TownActionCalculate.hpp"
#include "ov000/town/TownStageManager.hpp"
#include "main/status/PartyStatus.hpp"
#include "main/status/PlayerStatus.hpp"
#include "main/global/Global.hpp"
#include "main/menu/MenuDataCommon.hpp"
#include "main/dss/Random.hpp"
#include "main/profile/Profile.hpp"
#include "ov000/town/TownCharacterManager.hpp"
#include "ov000/town/TownFurnitureControl.hpp"
#include "ov000/town/riseup/TownRiseup.hpp"
#include "ov016/UnkMaterielMenuDraw/MaterielMenuDraw.hpp"
#include "main/status/StoryStatus.hpp"
#include "main/status/ShopList.hpp"
#include "main/status/GameFlag.hpp"
#include "main/cmn/GameManager.hpp"
#include "main/cmn/NonBattleActionManager.hpp"
#include "ov000/town/TownDoorAction.hpp"
#include "ov000/town/TownExtraMapObjManager.hpp"
#include "ov016/TownMenu_ITEM/TownMenuItemUseManager.hpp"
#include "ov000/town/TownFurniture.hpp"
#include "ov000/town/TownActionWalk.hpp"
#include "main/cmn/CommonEffectLocation.hpp"
#include "main/status/BattleResult.hpp"
#include "main/menu/MaterielMenuPlayerControl.hpp"
#include "ov000/town/TownCharacter.hpp"
#include "main/cmn/CommonCalculate.hpp"
#include "main/sound/Sound.hpp"
#include "ov000/town/TownEndrollManager.hpp"
#include "main/dss/DssUtils.hpp"
#include "main/cmn/EnvoyManager.hpp"
#include "main/cmn/ImmigrantTown.hpp"

void searchItem(int index, int* found, int* items);

static const dss::Fix32 defaultSpeed(0x66);

THUMB int cmd_set_overview_point(int* param)
{
    dss::Fix32Vector3 pos;
    pos.vx.value = param[0];
    pos.vy.value = param[1];
    pos.vz.value = param[2];
    g_Stage.overviewTempPosition_ = pos;
    return 1;
}

THUMB int cmd_set_player_position(int* param)
{
    dss::Fix32Vector3 pos;
    pos.vx.value = param[0];
    pos.vy.value = param[1];
    pos.vz.value = param[2];
    TownPlayerManager::getSingleton()->setPosition(pos);
    if (TownPlayerManager::getSingleton()->player_.actionType_ == 9) {
        TownIkadaAction2::getSingleton()->setIkadaPosition(pos);
    }
    return 1;
}

THUMB int cmd_set_player_direction(int* param)
{
    TownPlayerManager::getSingleton()->setDirection(param[0] << 14);
    return 1;
}

THUMB int cmd_is_speaked(int* param)
{
    int index = getPlacementCtrlId();
    int id = getPlacementIndex(index);
    int voice = TownCharacterManager::getSingleton()->getCharaIndex(index);
    if (TownCharacterManager::getSingleton()->isTalked(index) != 0) {
        if (TownCharacterManager::getSingleton()->character_[index]->checkMonsterSpeak() == 0 && g_HengeNoTsue.isMonster() == 1) {
            int message = g_HengeNoTsue.getMessage(voice);
            if (message != -1) {
                ui_MsgSndSet(cmn::g_talkSound.getCharacterVoice(index));
                TownWindowSystem::getSingleton()->openCommonMessage();
                TownWindowSystem::getSingleton()->addCommonMessage(message);
                cmn::PartyTalk::getSingleton()->resetPartyTalk();
                if (TownCharacterManager::getSingleton()->getCharatType(index) == 0) {
                    TownCharacterManager::getSingleton()->setPlayerDirection(index);
                }
                return 0;
            }
        }
        cmn::g_talkSound.setVoice(voice);
        cmn::PartyTalk::getSingleton()->resetPartyTalk();
        cmn::PartyTalk::getSingleton()->setObjectNo(id);
        g_cmnPartyInfo.playerTalk = 1;
        return 1;
    }
    return 0;
}

THUMB int cmd_speak_to_player(int* param)
{
    int index = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setPlayerDirection(index);
    cmn::g_talkSound.setVoice(TownCharacterManager::getSingleton()->getCharaIndex(index));
    cmn::g_talkSound.setMessageSound(param[1], index);
    TownWindowSystem::getSingleton()->openMessage(param[0], param[1]);
    if (!g_HengeNoTsue.isMonster()) {
        cmn::PartyTalk::getSingleton()->setPreMessageNo(param[0]);
    }
    return 1;
}

THUMB int cmd_speak_to_player2(int* param)
{
    int index = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setPlayerDirection(index);
    if (!g_HengeNoTsue.isMonster()) {
        cmn::PartyTalk::getSingleton()->setPreMessageNo(param[0]);
    }
    cmn::g_talkSound.setVoice(TownCharacterManager::getSingleton()->getCharaIndex(index));
    int message[8];
    message[0] = param[0];
    message[1] = param[1];
    message[2] = param[2];
    message[3] = param[3];
    message[4] = param[4];
    message[5] = param[5];
    message[6] = param[6];
    message[7] = param[7];
    int count = 0;
    for (int i = 0; i < 8; i++) {
        if (message[i] == 0) {
            break;
        }
        count++;
    }
    cmn::g_talkSound.setMessageSound(count, index);
    if (param[0] != 0) {
        TownWindowSystem::getSingleton()->openCommonMessage();
        TownWindowSystem::getSingleton()->addCommonMessage(param[0]);
    }
    if (param[1] != 0) {
        TownWindowSystem::getSingleton()->addCommonMessage(param[1]);
    }
    if (param[2] != 0) {
        TownWindowSystem::getSingleton()->addCommonMessage(param[2]);
    }
    if (param[3] != 0) {
        TownWindowSystem::getSingleton()->addCommonMessage(param[3]);
    }
    if (param[4] != 0) {
        TownWindowSystem::getSingleton()->addCommonMessage(param[4]);
    }
    if (param[5] != 0) {
        TownWindowSystem::getSingleton()->addCommonMessage(param[5]);
    }
    if (param[6] != 0) {
        TownWindowSystem::getSingleton()->addCommonMessage(param[6]);
    }
    if (param[7] != 0) {
        TownWindowSystem::getSingleton()->addCommonMessage(param[7]);
    }
    return 1;
}

THUMB int cmd_set_x_wins(int* param)
{
    TextAPI::setMACRO0(0x55, 0xf0000000, param[0]);
    return 1;
}

THUMB void __cmd_player_move::initialize(char* scriptParam)
{
    PARAM_PLAYER_MOVE* param = (PARAM_PLAYER_MOVE*)scriptParam;
    dss::Fix32Vector3 startPos;
    dss::Fix32Vector3 endPos;
    startPos.vx.value = param->startX;
    startPos.vy.value = param->startY;
    startPos.vz.value = param->startZ;
    endPos.vx.value = param->endX;
    endPos.vy.value = param->endY;
    endPos.vz.value = param->endZ;
    TownPlayerManager::getSingleton()->setSimpleMove(startPos, endPos, param->frame);
}

THUMB int __cmd_player_move::isEnd()
{
    if (TownPlayerManager::getSingleton()->isFinish() != 0) {
        return true;
    }
    return false;
}

__cmd_player_move g_cmd_player_move;

THUMB void __cmd_player_move2::initialize(char* scriptParam)
{
    PARAM_PLAYER_MOVE2* param = (PARAM_PLAYER_MOVE2*)scriptParam;
    dss::Fix32Vector3 startPos;
    dss::Fix32Vector3 endPos;
    startPos.vx.value = param->startX;
    startPos.vy.value = param->startY;
    startPos.vz.value = param->startZ;
    endPos.vx.value = param->endX;
    endPos.vy.value = param->endY;
    endPos.vz.value = param->endZ;
    dss::Fix32 rate;
    rate.value = param->rate;
    TownPlayerManager::getSingleton()->setSpeedMove(startPos, endPos, rate);
}

THUMB int __cmd_player_move2::isEnd()
{
    if (TownPlayerManager::getSingleton()->isFinish() != 0) {
        return true;
    }
    return false;
}

__cmd_player_move2 g_cmd_player_move2;

THUMB void __cmd_player_wait::initialize(char* scriptParam)
{
    PARAM_PLAYER_WAIT* param = (PARAM_PLAYER_WAIT*)scriptParam;
    count_ = 0;
    countFrame_ = param->frame;
}

THUMB void __cmd_player_wait::execute()
{
    count_++;
}

THUMB int __cmd_player_wait::isEnd()
{
    if (count_ >= countFrame_) {
        return true;
    }
    return false;
}

__cmd_player_wait g_cmd_player_wait;

THUMB void __cmd_player_move_to::initialize(char* scriptParam)
{
    PARAM_PLAYER_MOVE_TO* param = (PARAM_PLAYER_MOVE_TO*)scriptParam;
    dss::Fix32Vector3 endPos;
    dss::Fix32Vector3 playerPos = TownPlayerManager::getSingleton()->getPosition();
    dss::Fix32Vector3 startPos(playerPos);
    endPos.vx.value = param->endX;
    endPos.vy.value = param->endY;
    endPos.vz.value = param->endZ;
    if (param->absFlag == 1) {
        endPos += playerPos;
    }
    TownPlayerManager::getSingleton()->setSimpleMove(startPos, endPos, param->frame);
}

THUMB int __cmd_player_move_to::isEnd()
{
    if (TownPlayerManager::getSingleton()->isFinish() != 0) {
        return true;
    }
    return false;
}

__cmd_player_move_to g_cmd_player_move_to;

THUMB void __cmd_player_move2_to::initialize(char* scriptParam)
{
    PARAM_PLAYER_MOVE2_TO* param = (PARAM_PLAYER_MOVE2_TO*)scriptParam;
    dss::Fix32Vector3 startPos;
    dss::Fix32Vector3 endPos;
    dss::Fix32Vector3 playerPos = TownPlayerManager::getSingleton()->getPosition();
    startPos = playerPos;
    endPos.vx.value = param->endX;
    endPos.vy.value = param->endY;
    endPos.vz.value = param->endZ;
    if (param->absFlag == 1) {
        endPos += playerPos;
    }
    dss::Fix32 rate;
    rate.value = param->rate;
    TownPlayerManager::getSingleton()->setSpeedMove(startPos, endPos, rate);
}

THUMB int __cmd_player_move2_to::isEnd()
{
    if (TownPlayerManager::getSingleton()->isFinish() != 0) {
        return true;
    }
    return false;
}

__cmd_player_move2_to g_cmd_player_move2_to;

THUMB void __cmd_party_move2_formation::initialize(char* scriptParam)
{
    PARAM_PARTY_MOVE2_FORMATION* param = (PARAM_PARTY_MOVE2_FORMATION*)scriptParam;
    dss::Fix32 speed;
    speed.value = param->rate;
    speed *= defaultSpeed;
    TownPlayerManager::getSingleton()->setFormation(param->frmDir, param->charaDir, speed);
}

THUMB void __cmd_party_move2_formation::execute()
{
}

THUMB int __cmd_party_move2_formation::isEnd()
{
    if (TownPlayerManager::getSingleton()->party_.isFormationEnd() != 0) {
        return true;
    }
    return false;
}

__cmd_party_move2_formation g_cmd_party_move2_formation;

THUMB void __cmd_player_move_jump::initialize(char* scriptParam)
{
    PARAM_PLAYER_MOVE_JUMP* param = (PARAM_PLAYER_MOVE_JUMP*)scriptParam;
    dss::Fix32Vector3 pos;
    pos.set(param->endX, param->endY, param->endZ);
    TownPlayerManager::getSingleton()->setJumpMove(pos, param->frame);
}

THUMB int __cmd_player_move_jump::isEnd()
{
    if (TownPlayerManager::getSingleton()->isFinish() != 0) {
        return true;
    }
    return false;
}

__cmd_player_move_jump g_cmd_player_move_jump;

THUMB void __cmd_player_move2_jump::initialize(char* scriptParam)
{
    PARAM_PLAYER_MOVE2_JUMP* param = (PARAM_PLAYER_MOVE2_JUMP*)scriptParam;
    dss::Fix32Vector3 pos;
    pos.set(param->endX, param->endY, param->endZ);
    dss::Fix32Vector3 start = TownPlayerManager::getSingleton()->getPosition();
    dss::Fix32 rate;
    rate.value = param->rate;
    dss::Fix32 speed = defaultSpeed * rate;
    int frame = cmn::CommonCalculate::getFrameByVector(start, pos, speed);
    TownPlayerManager::getSingleton()->setJumpMove(pos, frame);
}

THUMB int __cmd_player_move2_jump::isEnd()
{
    if (TownPlayerManager::getSingleton()->isFinish() != 0) {
        return true;
    }
    return false;
}

__cmd_player_move2_jump g_cmd_player_move2_jump;

THUMB int cmd_set_character_position(int* param)
{
    int index = getPlacementCtrlId();
    dss::Fix32Vector3 pos;
    pos.vx.value = param[0];
    pos.vy.value = param[1];
    pos.vz.value = param[2];
    TownCharacterManager::getSingleton()->setPosition(index, pos);
    return 1;
}

THUMB int cmd_set_character_direction(int* param)
{
    int index = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setRotate(index, param[0] << 14);
    return 1;
}

THUMB int cmd_character_action_sleep(int* param)
{
    int index = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setSleepCharacter(index, param[0]);
    return 1;
}

THUMB int cmd_character_action_display(int* param)
{
    int index = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setDisplay(index, param[0]);
    return 1;
}

THUMB int cmd_character_action_near(int* param)
{
    int index = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setNearCharacter(index, param[0]);
    return 1;
}

THUMB int cmd_map_animation_a(int* param)
{
    TownStageManager::getSingleton()->setObjectDraw(param[0], param[1], 1);
    return 1;
}

THUMB int cmd_set_map_collision(int* param)
{
    if (param[1] == 1) {
        TownStageManager::getSingleton()->setCollision(param[0], 0);
        TownStageManager::getSingleton()->setCollisionObject(param[0]);
    } else {
        TownStageManager::getSingleton()->setCollision(param[0], 1);
        TownStageManager::getSingleton()->collEraseMapUid(param[0]);
    }
    return 1;
}

THUMB int cmd_chara_set_priority_sure_appointment(int* param)
{
    int values[5];
    values[0] = param[3];
    values[1] = param[4];
    values[2] = param[5];
    values[3] = param[6];
    values[4] = param[7];
    for (unsigned int i = 0; i < (unsigned int)param[2]; i++) {
        if (values[i] < 100) {
            values[i]--;
        }
    }
    if (g_Global.isAreaChange() == 1) {
        ImmigrantTown::getSingleton()->placePriorityResident(param[0], param[1], param[2], values);
    }
    return 1;
}

THUMB int cmd_chara_set_normal_sure_appointment(int* param)
{
    if (g_Global.isAreaChange() == 1) {
        ImmigrantTown::getSingleton()->placeNextResident(param[0], param[1]);
    }
    return 1;
}

THUMB int cmd_chara_set_normal_sure(int* param)
{
    int index = getPlacementCtrlId();
    int id = getPlacementIndex(index);
    int result = ImmigrantTown::getSingleton()->getResidentPoseAt(id, param[0]);
    if (result != 0xff) {
        TownCharacterManager::getSingleton()->setSureId(index, param[0]);
        TownCharacterManager::getSingleton()->character_[index]->changePose(result);
    } else {
        TownCharacterManager::getSingleton()->setDisplay(index, 0);
    }
    return 0;
}

THUMB int cmd_chara_talk_to_player_sure(int* param)
{
    int index = getPlacementCtrlId();
    if (index < getObjectCount()) {
        ui_MsgSndSet(cmn::g_talkSound.getCharacterVoice(index));
    }
    if (param[1] == 1) {
        g_EnvoyManager.mode_ = 1;
    } else {
        int id = getPlacementIndex(index);
        int value = TownCharacterManager::getSingleton()->character_[index]->getSurechigaiMapNo();
        g_EnvoyManager.selectEnvoy(ImmigrantTown::getSingleton()->getResidentIndexAt(id, value));
    }
    if (param[0] == 1) {
        TownCharacterManager::getSingleton()->setPlayerDirection(index);
    }

    unsigned char* name = g_EnvoyManager.getName();
    unsigned char* comment = g_EnvoyManager.getComment();
    int sex = g_EnvoyManager.getSex();
    int aetas = g_EnvoyManager.getAge();
    int skill = g_EnvoyManager.getSkill();
    unsigned char* townName = g_EnvoyManager.getTownName();
    char text0[0x40];
    char text1[0x40];
    char text2[0x40];
    splitCommentLines(text0, text1, text2, (char*)comment);
    TextAPI::setUserString(0, (char*)name);
    TextAPI::setUserString(1, (char*)text0);
    TextAPI::setUserString(2, (char*)text1);
    TextAPI::setUserString(3, (char*)text2);
    TextAPI::setUserString(4, (char*)townName);
    TownWindowSystem::getSingleton()->openCommonMessage();

    if (param[1] == 1) {
        TextAPI::setMACRO0(0x1c, 0xd0000000, 0);
        TextAPI::setMACRO1(0x21, 0xd0000000, 1);
        TextAPI::setMACRO2(0x21, 0xd0000000, 2);
        TextAPI::setMACRO3(0x21, 0xd0000000, 3);
        TownWindowSystem::getSingleton()->addCommonMessage(0x92a8c);
    } else {
        TextAPI::setMACRO0(0x1d, 0xd0000000, 0);
        TextAPI::setMACRO1(0x20, 0xd0000000, 1);
        TextAPI::setMACRO2(0x20, 0xd0000000, 2);
        TextAPI::setMACRO3(0x20, 0xd0000000, 3);
        TownWindowSystem::getSingleton()->addCommonMessage(0x92ac0);
    }

    if (param[1] != 1) {
        ui_MsgSndSet(0x30);
        TextAPI::setUserString(0, (char*)name);
        TextAPI::setMACRO0(0x1d, 0xd0000000, 0);
        TextAPI::setMACRO0(0x22, 0xa0000000, menu::MenuDataCommon::getSurechigaiAetas(aetas) & 0xfffffff);
        TextAPI::setMACRO0(0x23, 0xa0000000, menu::MenuDataCommon::getSuretigaiSex(sex) & 0xfffffff);
        TextAPI::setMACRO0(0x24, 0xe0000000, menu::MenuDataCommon::getSurechigaiSkill(skill) & 0xfffffff);
        TextAPI::setMACRO0(0x1e, 0xd0000000, 4);
        TownWindowSystem::getSingleton()->addCommonMessage(0x92ac1);
    }
    return 1;
}

THUMB int cmd_set_surechigai_level(int* param)
{
    return 1;
}

THUMB int cmd_character_action_stepping(int* param)
{
    int index = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setWriggleCharacter(index, 0);
    TownCharacterManager::getSingleton()->setAnimation(index, 1);
    return 1;
}

THUMB int cmd_character_action_still(int* param)
{
    int index = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setWriggleCharacter(index, 0);
    TownCharacterManager::getSingleton()->setAnimation(index, 0);
    return 1;
}

THUMB int cmd_character_action_wriggle(int* param)
{
    int index = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setAnimation(index, 1);
    TownCharacterManager::getSingleton()->setWriggleCharacter(index, 1);
    return 1;
}

THUMB int cmd_player_action_stepping(int* param)
{
    TownPlayerManager::getSingleton()->partyDraw_.setWriggleCharacter(0);
    TownPlayerManager::getSingleton()->partyDraw_.setAnimation(1);
    return 1;
}

THUMB int cmd_player_action_still(int* param)
{
    TownPlayerManager::getSingleton()->partyDraw_.setWriggleCharacter(0);
    TownPlayerManager::getSingleton()->partyDraw_.setAnimation(0);
    return 1;
}

THUMB int cmd_player_action_wriggle(int* param)
{
    TownPlayerManager::getSingleton()->partyDraw_.setAnimationOne(1);
    TownPlayerManager::getSingleton()->partyDraw_.setWriggleCharacter(1);
    return 1;
}

THUMB int cmd_set_character_collision(int* param)
{
    int index = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setCollFlag(index, param[0]);
    return 1;
}

THUMB int cmd_is_trigger(int* param)
{
    dss::Fix32Vector3 pos = TownPlayerManager::getSingleton()->getPosition();
    if (param[0] < pos.vx.value && param[3] > pos.vx.value &&
        param[1] < pos.vy.value && param[4] > pos.vy.value &&
        param[2] < pos.vz.value && param[5] > pos.vz.value) {
        return 1;
    }
    return 0;
}

THUMB int cmd_is_trigger2(int* param)
{
    dss::Fix32Vector3 pos = TownPlayerManager::getSingleton()->getPosition();
    if (param[0] < pos.vx.value && param[3] > pos.vx.value &&
        param[1] < pos.vy.value && param[4] > pos.vy.value &&
        param[2] < pos.vz.value && param[5] > pos.vz.value) {
        short dir = TownPlayerManager::getSingleton()->getDirection();
        dss::Fix32Vector3 front;
        TownActionCalculate::getDirByIdx(dir, front);
        dss::Fix32Vector3 target = TownActionCalculate::getParamVec((unsigned char)param[6]);
        dss::Fix32 dot = front * target;
        if (dot >= dss::Fix32(0L)) {
            return 1;
        }
    }
    return 0;
}

THUMB int cmd_character_action_pursue(int* param)
{
    TownCharaMoveParam move;
    int index = getPlacementCtrlId();
    if (TownCharacterManager::getSingleton()->character_[index]->moveType_ != MOVE_TYPE_PURSUE) {
        TownCharacterManager::getSingleton()->setCollFlag(index, 0);
        index = getPlacementCtrlId();
        move.speed_.value = param[0];
        move.speed_ *= defaultSpeed;
        move.unk_38 = 0;
        move.unk_34 = 0;
        TownCharacterBase** chara = TownCharacterManager::getSingleton()->character_;
        chara[index]->moveType_ = MOVE_TYPE_PURSUE;
        TownCharacterBase* c = chara[index];
        c->moveData_.vector[0] = move.pos_[0];
        c->moveData_.vector[1] = move.pos_[1];
        c->moveData_.vector[2] = move.pos_[2];
        c->moveData_.vector[3] = move.pos_[3];
        c->moveData_.speed = move.speed_;
        c->moveData_.frame = move.unk_34;
        c->moveData_.counter = move.unk_38;
    }
    return 1;
}

THUMB int cmd_character_move_roam(int* param)
{
    int index = getPlacementCtrlId();
    TownCharaMoveParam move;
    move.unk_38 = dssrand::rand(0x50);
    move.unk_34 = 0;
    move.pos_[0].vx.value = param[0];
    move.pos_[0].vy.value = param[1];
    move.pos_[0].vz.value = param[2];
    move.pos_[1].vx.value = param[3];
    move.pos_[1].vy.value = param[4];
    move.pos_[1].vz.value = param[5];
    move.speed_.value = param[6];
    move.speed_ *= defaultSpeed;
    TownCharacterBase** chara = TownCharacterManager::getSingleton()->character_;
    chara[index]->moveType_ = MOVE_TYPE_AREA;
    TownCharacterBase* c = chara[index];
    c->moveData_.vector[0] = move.pos_[0];
    c->moveData_.vector[1] = move.pos_[1];
    c->moveData_.vector[2] = move.pos_[2];
    c->moveData_.vector[3] = move.pos_[3];
    c->moveData_.speed = move.speed_;
    c->moveData_.frame = move.unk_34;
    c->moveData_.counter = move.unk_38;
    return 1;
}

THUMB int cmd_is_trigger_character(int* param)
{
    int index = getPlacementCtrlId();
    dss::Fix32Vector3 pos = TownCharacterManager::getSingleton()->getPosition(index);
    short dir = TownCharacterManager::getSingleton()->getDirection(index);
    dss::Fix32Vector3 min;
    dss::Fix32Vector3 max;
    min.set(param[0], param[1], param[2]);
    max.set(param[3], param[4], param[5]);
    TriggerCheck check = param[6] == 0 ? TRIGGER_CHECK_0 : TRIGGER_CHECK_1;
    int type = param[7] == 0 ? 7 : 6;
    return cmn::CommonCalculate::areaCheck(pos, dir, min, max, check, type);
}

THUMB int cmd_is_trigger2_character(int* param)
{
    TriggerCheck check;
    int index = getPlacementCtrlId();
    dss::Fix32Vector3 pos = TownCharacterManager::getSingleton()->getPosition(index);
    short dir = TownCharacterManager::getSingleton()->getDirection(index);
    dss::Fix32Vector3 min;
    dss::Fix32Vector3 max;
    min.set(param[0], param[1], param[2]);
    max.set(param[3], param[4], param[5]);
    switch (param[6]) {
    case 0:
        check = TRIGGER_CHECK_2;
        break;
    case 1:
        check = TRIGGER_CHECK_3;
        break;
    case 2:
        check = TRIGGER_CHECK_4;
        break;
    case 3:
        check = TRIGGER_CHECK_5;
        break;
    }
    int type = param[7] == 0 ? 7 : 6;
    return cmn::CommonCalculate::areaCheck(pos, dir, min, max, check, type);
}

THUMB int cmd_party_join(int* param)
{
    int index = getPlacementCtrlId(param[0]);
    status::g_Party.add(param[1]);
    TownPlayerManager::getSingleton()->resetParty();
    TownCharacterManager::getSingleton()->setDisplay(index, 0);
    TownCharacterManager::getSingleton()->setCollFlag(index, 0);
    return 1;
}

THUMB int cmd_party_quit(int* param)
{
    status::g_Party.setNormalMode();
    int index = getPlacementCtrlId();
    int sortIndex = status::g_Party.getSortIndex(param[1]);
    int order[4] = {0, 0, 0, 0};
    if (sortIndex == -1) {
        return 1;
    }
    for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
        if (param[1] != status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_) {
            order[i] = status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.playerIndex_;
        }
    }
    dss::Fix32Vector3 pos = TownPlayerManager::getSingleton()->party_.getMemberPosition(sortIndex);
    TownCharacterManager::getSingleton()->setDisplay(index, 1);
    TownCharacterManager::getSingleton()->setCollFlag(index, 1);
    TownCharacterManager::getSingleton()->setPosition(index, pos);
    status::g_Party.del(param[1]);
    status::g_Party.reorder(order[0], order[1], order[2], order[3]);
    TownPlayerManager::getSingleton()->resetParty();
    return 1;
}

THUMB void __cmd_character_move::initialize(char* scriptParam)
{
    PARAM_CHARACTER_MOVE* param = (PARAM_CHARACTER_MOVE*)scriptParam;
    TOWN_SCRIPT_DATA scriptData;
    dss::memset(&scriptData, 0, sizeof(scriptData));
    scriptData.node[0].vx.value = param->startX;
    scriptData.node[0].vy.value = param->startY;
    scriptData.node[0].vz.value = param->startZ;
    scriptData.node[1].vx.value = param->endX;
    scriptData.node[1].vy.value = param->endY;
    scriptData.node[1].vz.value = param->endZ;
    scriptData.frame = param->frame;
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setScriptData(ctrl, scriptData);
    TownCharacterManager::getSingleton()->character_[ctrl]->setSimpleMove();
}

THUMB void __cmd_character_move::execute()
{
}

THUMB int __cmd_character_move::isEnd()
{
    int ctrl = getPlacementCtrlId();
    if (TownCharacterManager::getSingleton()->character_[ctrl]->isScriptEnd() != 0) {
        return true;
    }
    return false;
}

__cmd_character_move g_cmd_character_move;

THUMB void __cmd_character_move2::initialize(char* scriptParam)
{
    PARAM_CHARACTER_MOVE2* param = (PARAM_CHARACTER_MOVE2*)scriptParam;
    TOWN_SCRIPT_DATA scriptData;
    dss::memset(&scriptData, 0, sizeof(scriptData));
    scriptData.node[0].vx.value = param->startX;
    scriptData.node[0].vy.value = param->startY;
    scriptData.node[0].vz.value = param->startZ;
    scriptData.node[1].vx.value = param->endX;
    scriptData.node[1].vy.value = param->endY;
    scriptData.node[1].vz.value = param->endZ;
    if (param->rate == 0) {
        param->rate = 0x1000;
    }
    dss::Fix32 len = ((scriptData.node[1] - scriptData.node[0])).length();
    scriptData.frame = len.value / (param->rate * defaultSpeed.value / 0x1000);
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setScriptData(ctrl, scriptData);
    TownCharacterManager::getSingleton()->character_[ctrl]->setSimpleMove();
}

THUMB void __cmd_character_move2::execute()
{
}

THUMB int __cmd_character_move2::isEnd()
{
    int ctrl = getPlacementCtrlId();
    if (TownCharacterManager::getSingleton()->character_[ctrl]->isScriptEnd() != 0) {
        return true;
    }
    return false;
}

__cmd_character_move2 g_cmd_character_move2;

THUMB void __cmd_character_wait::initialize(char* scriptParam)
{
    PARAM_CHARACTER_WAIT* param = (PARAM_CHARACTER_WAIT*)scriptParam;
    TOWN_SCRIPT_DATA scriptData;
    dss::memset(&scriptData, 0, sizeof(scriptData));
    scriptData.frame = param->frame;
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setScriptData(ctrl, scriptData);
}

THUMB void __cmd_character_wait::execute()
{
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->character_[ctrl]->execWait();
}

THUMB int __cmd_character_wait::isEnd()
{
    int ctrl = getPlacementCtrlId();
    if (TownCharacterManager::getSingleton()->character_[ctrl]->isScriptEnd() != 0) {
        return true;
    }
    return false;
}

__cmd_character_wait g_cmd_character_wait;

THUMB void __cmd_character_effect_mark::initialize(char* scriptParam)
{
    PARAM_CHARACTER_EFFECT_MARK* param = (PARAM_CHARACTER_EFFECT_MARK*)scriptParam;
    TOWN_SCRIPT_DATA scriptData;
    dss::memset(&scriptData, 0, sizeof(scriptData));
    scriptData.num[0] = param->mark;
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setScriptData(ctrl, scriptData);
}

THUMB void __cmd_character_effect_mark::execute()
{
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->character_[ctrl]->execRiseup();
}

THUMB int __cmd_character_effect_mark::isEnd()
{
    int ctrl = getPlacementCtrlId();
    if (TownCharacterManager::getSingleton()->character_[ctrl]->isScriptEnd() != 0) {
        return true;
    }
    return false;
}

__cmd_character_effect_mark g_cmd_character_effect_mark;

THUMB void __cmd_character_move_to::initialize(char* scriptParam)
{
    PARAM_CHARACTER_MOVE_TO* param = (PARAM_CHARACTER_MOVE_TO*)scriptParam;
    int ctrl = getPlacementCtrlId();
    TOWN_SCRIPT_DATA scriptData;
    dss::memset(&scriptData, 0, sizeof(scriptData));
    scriptData.frame = param->frame;
    scriptData.node[0] = TownCharacterManager::getSingleton()->getPosition(ctrl);
    scriptData.node[1].vx.value = param->endX;
    scriptData.node[1].vy.value = param->endY;
    scriptData.node[1].vz.value = param->endZ;
    TownCharacterManager::getSingleton()->setScriptData(ctrl, scriptData);
    TownCharacterManager::getSingleton()->character_[ctrl]->setSimpleMove();
}

THUMB void __cmd_character_move_to::execute()
{
}

THUMB int __cmd_character_move_to::isEnd()
{
    int ctrl = getPlacementCtrlId();
    if (TownCharacterManager::getSingleton()->character_[ctrl]->isScriptEnd() != 0) {
        return true;
    }
    return false;
}

__cmd_character_move_to g_cmd_character_move_to;

THUMB void __cmd_character_move2_to::initialize(char* scriptParam)
{
    PARAM_CHARACTER_MOVE2_TO* param = (PARAM_CHARACTER_MOVE2_TO*)scriptParam;
    int ctrl = getPlacementCtrlId();
    TOWN_SCRIPT_DATA scriptData;
    dss::memset(&scriptData, 0, sizeof(scriptData));
    scriptData.node[0] = TownCharacterManager::getSingleton()->getPosition(ctrl);
    scriptData.node[1].vx.value = param->endX;
    scriptData.node[1].vy.value = param->endY;
    scriptData.node[1].vz.value = param->endZ;
    if (param->rate == 0) {
        param->rate = 0x1000;
    }
    dss::Fix32 len = ((scriptData.node[1] - scriptData.node[0])).length();
    scriptData.frame = len.value / (param->rate * defaultSpeed.value / 0x1000);
    TownCharacterManager::getSingleton()->setScriptData(ctrl, scriptData);
    TownCharacterManager::getSingleton()->character_[ctrl]->setSimpleMove();
}

THUMB void __cmd_character_move2_to::execute()
{
}

THUMB int __cmd_character_move2_to::isEnd()
{
    int ctrl = getPlacementCtrlId();
    if (TownCharacterManager::getSingleton()->character_[ctrl]->isScriptEnd() != 0) {
        return true;
    }
    return false;
}

__cmd_character_move2_to g_cmd_character_move2_to;

THUMB void __cmd_character_move_party::initialize(char* scriptParam)
{
    PARAM_CHARACTER_MOVE_PARTY* param = (PARAM_CHARACTER_MOVE_PARTY*)scriptParam;
    int ctrl = getPlacementCtrlId();
    TOWN_SCRIPT_DATA scriptData;
    dss::memset(&scriptData, 0, sizeof(scriptData));
    scriptData.num[0] = param->mode;
    scriptData.node[0] = TownCharacterManager::getSingleton()->getPosition(ctrl);
    int drawCount = TownPlayerManager::getSingleton()->partyDraw_.countReal_;
    scriptData.node[1] = TownPlayerManager::getSingleton()->party_.getMemberPosition(drawCount);
    scriptData.frame = param->frame;
    TownCharacterManager::getSingleton()->setScriptData(ctrl, scriptData);
    TownCharacterManager::getSingleton()->character_[ctrl]->setMoveToParty();
    TownCharacterManager::getSingleton()->character_[ctrl]->setSimpleMove();
    TownCharacterManager::getSingleton()->setCollFlag(ctrl, 0);
}

THUMB void __cmd_character_move_party::execute()
{
}

THUMB int __cmd_character_move_party::isEnd()
{
    int ctrl = getPlacementCtrlId();
    if (TownCharacterManager::getSingleton()->character_[ctrl]->isScriptEnd() != 0) {
        return true;
    }
    return false;
}

__cmd_character_move_party g_cmd_character_move_party;

THUMB void __cmd_character_move2_party::initialize(char* scriptParam)
{
    PARAM_CHARACTER_MOVE2_PARTY* param = (PARAM_CHARACTER_MOVE2_PARTY*)scriptParam;
    int ctrl = getPlacementCtrlId();
    TOWN_SCRIPT_DATA scriptData;
    dss::memset(&scriptData, 0, sizeof(scriptData));
    scriptData.node[0] = TownCharacterManager::getSingleton()->getPosition(ctrl);
    int drawCount = TownPlayerManager::getSingleton()->partyDraw_.countReal_;
    scriptData.node[1] = TownPlayerManager::getSingleton()->party_.getMemberPosition(drawCount);
    if (param->rate == 0) {
        param->rate = 0x1000;
    }
    dss::Fix32Vector3 vec = (scriptData.node[1] - scriptData.node[0]);
    dss::Fix32 speed;
    dss::Fix32 dv;
    speed.value = param->rate;
    speed *= defaultSpeed;
    dv = vec.length() / speed;
    scriptData.frame = dv.value / 0x1000;
    scriptData.num[0] = param->mode;
    TownCharacterManager::getSingleton()->setScriptData(ctrl, scriptData);
    TownCharacterManager::getSingleton()->character_[ctrl]->setMoveToParty();
    TownCharacterManager::getSingleton()->character_[ctrl]->setSimpleMove();
    TownCharacterManager::getSingleton()->setCollFlag(ctrl, 0);
}

THUMB void __cmd_character_move2_party::execute()
{
}

THUMB int __cmd_character_move2_party::isEnd()
{
    int ctrl = getPlacementCtrlId();
    if (TownCharacterManager::getSingleton()->character_[ctrl]->isScriptEnd() != 0) {
        return true;
    }
    return false;
}

__cmd_character_move2_party g_cmd_character_move2_party;

THUMB void __cmd_character_move_player::initialize(char* scriptParam)
{
    PARAM_CHARACTER_MOVE_PLAYER* param = (PARAM_CHARACTER_MOVE_PLAYER*)scriptParam;
    int ctrl = getPlacementCtrlId();
    TOWN_SCRIPT_DATA scriptData;
    dss::memset(&scriptData, 0, sizeof(scriptData));
    scriptData.num[0] = param->mode;
    scriptData.node[0] = TownCharacterManager::getSingleton()->getPosition(ctrl);
    scriptData.node[1] = TownPlayerManager::getSingleton()->getPosition();
    unsigned char dir = param->direction;
    dss::Fix32Vector3 vec = TownActionCalculate::getParamVec(dir);
    dss::Fix32 rate;
    rate.value = param->adjust;
    scriptData.node[1] += vec * rate;
    scriptData.frame = param->frame;
    TownCharacterManager::getSingleton()->setScriptData(ctrl, scriptData);
    TownCharacterManager::getSingleton()->character_[ctrl]->setMoveToParty();
    TownCharacterManager::getSingleton()->character_[ctrl]->setSimpleMove();
    TownCharacterManager::getSingleton()->setCollFlag(ctrl, 0);
}

THUMB void __cmd_character_move_player::execute()
{
}

THUMB int __cmd_character_move_player::isEnd()
{
    int ctrl = getPlacementCtrlId();
    if (TownCharacterManager::getSingleton()->character_[ctrl]->isScriptEnd() != 0) {
        return true;
    }
    return false;
}

__cmd_character_move_player g_cmd_character_move_player;

THUMB void __cmd_character_move2_player::initialize(char* scriptParam)
{
    PARAM_CHARACTER_MOVE2_PLAYER* param = (PARAM_CHARACTER_MOVE2_PLAYER*)scriptParam;
    int ctrl = getPlacementCtrlId();
    TOWN_SCRIPT_DATA scriptData;
    dss::memset(&scriptData, 0, sizeof(scriptData));
    scriptData.num[0] = param->mode;
    scriptData.node[0] = TownCharacterManager::getSingleton()->getPosition(ctrl);
    scriptData.node[1] = TownPlayerManager::getSingleton()->getPosition();
    unsigned char dir = param->direction;
    dss::Fix32Vector3 vec = TownActionCalculate::getParamVec(dir);
    dss::Fix32 rate;
    rate.value = param->adjust;
    scriptData.node[1] += vec * rate;
    if (param->rate == 0) {
        param->rate = 0x1000;
    }
    dss::Fix32 len = ((scriptData.node[1] - scriptData.node[0])).length();
    scriptData.frame = len.value / (param->rate * defaultSpeed.value / 0x1000);
    TownCharacterManager::getSingleton()->setScriptData(ctrl, scriptData);
    TownCharacterManager::getSingleton()->character_[ctrl]->setMoveToParty();
    TownCharacterManager::getSingleton()->character_[ctrl]->setSimpleMove();
    TownCharacterManager::getSingleton()->setCollFlag(ctrl, 0);
}

THUMB void __cmd_character_move2_player::execute()
{
}

THUMB int __cmd_character_move2_player::isEnd()
{
    int ctrl = getPlacementCtrlId();
    if (TownCharacterManager::getSingleton()->character_[ctrl]->isScriptEnd() != 0) {
        return true;
    }
    return false;
}

__cmd_character_move2_player g_cmd_character_move2_player;

THUMB void __cmd_character_move_relative::initialize(char* scriptParam)
{
    PARAM_CHARACTER_MOVE_RELATIVE* param = (PARAM_CHARACTER_MOVE_RELATIVE*)scriptParam;
    int ctrl = getPlacementCtrlId();
    TOWN_SCRIPT_DATA scriptData;
    dss::memset(&scriptData, 0, sizeof(scriptData));
    scriptData.node[0] = TownCharacterManager::getSingleton()->getPosition(ctrl);
    scriptData.node[1] = scriptData.node[0];
    scriptData.node[1].vx.value += param->endX;
    scriptData.node[1].vy.value += param->endY;
    scriptData.node[1].vz.value += param->endZ;
    scriptData.frame = param->frame;
    TownCharacterManager::getSingleton()->setScriptData(ctrl, scriptData);
    TownCharacterManager::getSingleton()->character_[ctrl]->setSimpleMove();
}

THUMB int __cmd_character_move_relative::isEnd()
{
    int ctrl = getPlacementCtrlId();
    if (TownCharacterManager::getSingleton()->character_[ctrl]->isScriptEnd() != 0) {
        return true;
    }
    return false;
}

__cmd_character_move_relative g_cmd_character_move_relative;

THUMB void __cmd_character_move2_relative::initialize(char* scriptParam)
{
    PARAM_CHARACTER_MOVE2_RELATIVE* param = (PARAM_CHARACTER_MOVE2_RELATIVE*)scriptParam;
    int ctrl = getPlacementCtrlId();
    TOWN_SCRIPT_DATA scriptData;
    dss::memset(&scriptData, 0, sizeof(scriptData));
    scriptData.node[0] = TownCharacterManager::getSingleton()->getPosition(ctrl);
    scriptData.node[1] = scriptData.node[0];
    scriptData.node[1].vx.value += param->endX;
    scriptData.node[1].vy.value += param->endY;
    scriptData.node[1].vz.value += param->endZ;
    if (param->rate == 0) {
        param->rate = 0x1000;
    }
    dss::Fix32 len = ((scriptData.node[1] - scriptData.node[0])).length();
    scriptData.frame = len.value / (param->rate * defaultSpeed.value / 0x1000);
    TownCharacterManager::getSingleton()->setScriptData(ctrl, scriptData);
    TownCharacterManager::getSingleton()->character_[ctrl]->setSimpleMove();
}

THUMB int __cmd_character_move2_relative::isEnd()
{
    int ctrl = getPlacementCtrlId();
    if (TownCharacterManager::getSingleton()->character_[ctrl]->isScriptEnd() != 0) {
        return true;
    }
    return false;
}

__cmd_character_move2_relative g_cmd_character_move2_relative;

THUMB void __cmd_character_move_x::initialize(char* scriptParam)
{
    PARAM_CHARACTER_MOVE_X* param = (PARAM_CHARACTER_MOVE_X*)scriptParam;
    int ctrl = getPlacementCtrlId();
    TOWN_SCRIPT_DATA scriptData;
    dss::memset(&scriptData, 0, sizeof(scriptData));
    scriptData.node[0] = TownCharacterManager::getSingleton()->getPosition(ctrl);
    scriptData.node[1] = scriptData.node[0];
    scriptData.node[1].vx.value = param->endX;
    scriptData.frame = param->frame;
    TownCharacterManager::getSingleton()->setScriptData(ctrl, scriptData);
    TownCharacterManager::getSingleton()->character_[ctrl]->setSimpleMove();
}

THUMB int __cmd_character_move_x::isEnd()
{
    int ctrl = getPlacementCtrlId();
    if (TownCharacterManager::getSingleton()->character_[ctrl]->isScriptEnd() != 0) {
        return true;
    }
    return false;
}

__cmd_character_move_x g_cmd_character_move_x;

THUMB void __cmd_character_move2_x::initialize(char* scriptParam)
{
    PARAM_CHARACTER_MOVE2_X* param = (PARAM_CHARACTER_MOVE2_X*)scriptParam;
    int ctrl = getPlacementCtrlId();
    TOWN_SCRIPT_DATA scriptData;
    dss::memset(&scriptData, 0, sizeof(scriptData));
    scriptData.node[0] = TownCharacterManager::getSingleton()->getPosition(ctrl);
    scriptData.node[1] = scriptData.node[0];
    scriptData.node[1].vx.value = param->endX;
    if (param->rate == 0) {
        param->rate = 0x1000;
    }
    dss::Fix32 len = ((scriptData.node[1] - scriptData.node[0])).length();
    scriptData.frame = len.value / (param->rate * defaultSpeed.value / 0x1000);
    TownCharacterManager::getSingleton()->setScriptData(ctrl, scriptData);
    TownCharacterManager::getSingleton()->character_[ctrl]->setSimpleMove();
}

THUMB int __cmd_character_move2_x::isEnd()
{
    int ctrl = getPlacementCtrlId();
    if (TownCharacterManager::getSingleton()->character_[ctrl]->isScriptEnd() != 0) {
        return true;
    }
    return false;
}

__cmd_character_move2_x g_cmd_character_move2_x;

THUMB void __cmd_character_move_z::initialize(char* scriptParam)
{
    PARAM_CHARACTER_MOVE_Z* param = (PARAM_CHARACTER_MOVE_Z*)scriptParam;
    int ctrl = getPlacementCtrlId();
    TOWN_SCRIPT_DATA scriptData;
    dss::memset(&scriptData, 0, sizeof(scriptData));
    scriptData.node[0] = TownCharacterManager::getSingleton()->getPosition(ctrl);
    scriptData.node[1] = scriptData.node[0];
    scriptData.node[1].vz.value = param->endZ;
    scriptData.frame = param->frame;
    TownCharacterManager::getSingleton()->setScriptData(ctrl, scriptData);
    TownCharacterManager::getSingleton()->character_[ctrl]->setSimpleMove();
}

THUMB int __cmd_character_move_z::isEnd()
{
    int ctrl = getPlacementCtrlId();
    if (TownCharacterManager::getSingleton()->character_[ctrl]->isScriptEnd() != 0) {
        return true;
    }
    return false;
}

__cmd_character_move_z g_cmd_character_move_z;

THUMB void __cmd_character_move2_z::initialize(char* scriptParam)
{
    PARAM_CHARACTER_MOVE2_Z* param = (PARAM_CHARACTER_MOVE2_Z*)scriptParam;
    int ctrl = getPlacementCtrlId();
    TOWN_SCRIPT_DATA scriptData;
    dss::memset(&scriptData, 0, sizeof(scriptData));
    scriptData.node[0] = TownCharacterManager::getSingleton()->getPosition(ctrl);
    scriptData.node[1] = scriptData.node[0];
    scriptData.node[1].vz.value = param->endZ;
    if (param->rate == 0) {
        param->rate = 0x1000;
    }
    dss::Fix32 len = ((scriptData.node[1] - scriptData.node[0])).length();
    scriptData.frame = len.value / (param->rate * defaultSpeed.value / 0x1000);
    TownCharacterManager::getSingleton()->setScriptData(ctrl, scriptData);
    TownCharacterManager::getSingleton()->character_[ctrl]->setSimpleMove();
}

THUMB int __cmd_character_move2_z::isEnd()
{
    int ctrl = getPlacementCtrlId();
    if (TownCharacterManager::getSingleton()->character_[ctrl]->isScriptEnd() != 0) {
        return true;
    }
    return false;
}

__cmd_character_move2_z g_cmd_character_move2_z;

THUMB void __cmd_character_action_turn::initialize(char* scriptParam)
{
    PARAM_CHARACTER_ACTION_TURN* param = (PARAM_CHARACTER_ACTION_TURN*)scriptParam;
    int ctrl = getPlacementCtrlId();
    short idx = TownActionCalculate::getIdxByParam(param->direction);
    TownCharacterManager::getSingleton()->setSimpleRot(ctrl, idx, param->frame, param->rot);
}

THUMB int __cmd_character_action_turn::isEnd()
{
    int ctrl = getPlacementCtrlId();
    if (TownCharacterManager::getSingleton()->character_[ctrl]->isRotEnd() != 0) {
        return true;
    }
    return false;
}

__cmd_character_action_turn g_cmd_character_action_turn;

THUMB void __cmd_character_action_gaze::initialize(char* scriptParam)
{
    PARAM_CHARACTER_ACTION_GAZE* param = (PARAM_CHARACTER_ACTION_GAZE*)scriptParam;
    int ctrl = getPlacementCtrlId();
    dss::Fix32Vector3 vec = (TownPlayerManager::getSingleton()->getPosition() - TownCharacterManager::getSingleton()->getPosition(ctrl));
    short idx = TownCharacterManager::getSingleton()->getDirection(ctrl);
    TownActionCalculate::getIdxByVec(idx, vec);
    TownCharacterManager::getSingleton()->setSimpleRot(ctrl, idx, param->frame, 0);
}

THUMB int __cmd_character_action_gaze::isEnd()
{
    int ctrl = getPlacementCtrlId();
    if (TownCharacterManager::getSingleton()->character_[ctrl]->isRotEnd() != 0) {
        return true;
    }
    return false;
}

__cmd_character_action_gaze g_cmd_character_action_gaze;

THUMB void __cmd_furniture_move::initialize(char* scriptParam)
{
    PARAM_FURNITURE_MOVE* param = (PARAM_FURNITURE_MOVE*)scriptParam;
    dss::Fix32Vector3 end;
    dss::Fix32Vector3 start;
    start = TownStageManager::getSingleton()->getMapUidPos(param->target);
    end.vx.value = param->endX + start.vx.value;
    end.vy.value = param->endY + start.vy.value;
    end.vz.value = param->endZ + start.vz.value;
    m_index = TownFurnitureControlManager::getSingleton()->setFurnitureMove(param->target, param->frame, end);
    TownFurnitureControlManager::getSingleton()->setGarbageCorrect(m_index, false);
}

THUMB int __cmd_furniture_move::isEnd()
{
    if (TownFurnitureControlManager::getSingleton()->isEnd(m_index) != 0) {
        TownFurnitureControlManager::getSingleton()->cleanup(m_index);
        return true;
    }
    return false;
}

__cmd_furniture_move g_cmd_furniture_move;

THUMB void __cmd_furniture_move2::initialize(char* scriptParam)
{
    PARAM_FURNITURE_MOVE2* param = (PARAM_FURNITURE_MOVE2*)scriptParam;
    dss::Fix32Vector3 end;
    dss::Fix32Vector3 start;
    start = TownStageManager::getSingleton()->getMapUidPos(param->target);
    end.vx.value = param->endX + start.vx.value;
    end.vy.value = param->endY + start.vy.value;
    end.vz.value = param->endZ + start.vz.value;
    if (param->rate == 0) {
        param->rate = 0x1000;
    }
    dss::Fix32 len = ((start - end)).length();
    int frame = len.value / (param->rate * defaultSpeed.value / 0x1000);
    TownFurnitureControlManager* mgr = TownFurnitureControlManager::getSingleton();
    m_index = mgr->setFurnitureMove(param->target, frame, end);
    TownFurnitureControlManager::getSingleton()->setGarbageCorrect(m_index, false);
}

THUMB int __cmd_furniture_move2::isEnd()
{
    if (TownFurnitureControlManager::getSingleton()->isEnd(m_index) != 0) {
        TownFurnitureControlManager::getSingleton()->cleanup(m_index);
        return true;
    }
    return false;
}

__cmd_furniture_move2 g_cmd_furniture_move2;

THUMB void __cmd_effect_wait::initialize(char* scriptParam)
{
    PARAM_EFFECT_WAIT* param = (PARAM_EFFECT_WAIT*)scriptParam;
    dss::Fix32Vector3 pos;
    pos.vx.value = param->posX;
    pos.vy.value = param->posY;
    pos.vz.value = param->posZ;
    m_index = TownRiseupManager::getSingleton()->setupSprite(param->effect, pos, param->flag, 0);
}

THUMB int __cmd_effect_wait::isEnd()
{
    return TownRiseupManager::getSingleton()->isFinish(m_index);
}

__cmd_effect_wait g_cmd_effect_wait;

THUMB void __cmd_effect_move::initialize(char* scriptParam)
{
    PARAM_EFFECT_MOVE* param = (PARAM_EFFECT_MOVE*)scriptParam;
    dss::Fix32Vector3 start;
    dss::Fix32Vector3 end;
    start.vx.value = param->startX;
    start.vy.value = param->startY;
    start.vz.value = param->startZ;
    end.vx.value = param->endX;
    end.vy.value = param->endY;
    end.vz.value = param->endZ;
    m_index = TownRiseupManager::getSingleton()->setupSpriteMove(param->effect, start, end, param->frame);
}

THUMB int __cmd_effect_move::isEnd()
{
    return TownRiseupManager::getSingleton()->isFinish(m_index);
}

__cmd_effect_move g_cmd_effect_move;

THUMB void __cmd_effect_fade::initialize(char* scriptParam)
{
    PARAM_EFFECT_FADE* param = (PARAM_EFFECT_FADE*)scriptParam;
    dss::Fix32Vector3 pos;
    pos.vx.value = param->posX;
    pos.vy.value = param->posY;
    pos.vz.value = param->posZ;
    m_index = TownRiseupManager::getSingleton()->setupSpriteFade(param->effect, pos, param->frame, param->flag);
}

THUMB int __cmd_effect_fade::isEnd()
{
    return TownRiseupManager::getSingleton()->isFinish(m_index);
}

__cmd_effect_fade g_cmd_effect_fade;

THUMB void __cmd_map_flash::initialize(char* scriptParam)
{
    PARAM_MAP_FLASH* param = (PARAM_MAP_FLASH*)scriptParam;
    count_ = 0;
    countFrame_ = param->frame;
    g_SoundPlayer.unkfunc_0205c948(0, param->se);
    int flash_frame = countFrame_ / 2;
    g_SpriteFade.flashWhite(flash_frame, (unsigned char)param->r, (unsigned char)param->g, (unsigned char)param->b);
}

THUMB void __cmd_map_flash::execute()
{
    count_++;
    int flash_frame = countFrame_ / 2;
    if (count_ == flash_frame) {
        g_SpriteFade.state_ = (GlobalFade::FADE_STATE)6;
        g_SpriteFade.count_ = 0;
        g_SpriteFade.frames_ = flash_frame;
    }
}

THUMB int __cmd_map_flash::isEnd()
{
    if (count_ >= countFrame_) {
        return true;
    }
    return false;
}

__cmd_map_flash g_cmd_map_flash;

THUMB void __cmd_map_blend_color::initialize(char* scriptParam)
{
    PARAM_MAP_BLEND_COLOR* param = (PARAM_MAP_BLEND_COLOR*)scriptParam;
    rate_.vx.value = param->r;
    rate_.vy.value = param->g;
    rate_.vz.value = param->b;
    count_ = 0;
    countFrame_ = param->frame;
    TownStageManager::getSingleton()->mapEffect_.setPisaroEvent(countFrame_);
}

THUMB void __cmd_map_blend_color::execute()
{
    dss::Fix32 ratio;
    dss::Fix32 one;
    one.value = 0x1000;
    dss::Fix32Vector3 defaultRate = TownStageManager::getSingleton()->townData_.getDefaultPaletteRate();
    dss::Fix32Vector3 current;
    count_++;
#ifdef DQIV_NATIVE
    // A fade of zero frames: the DS's division by zero gives the dividend
    // (straight to the final colours); x86 would trap.
    ratio.value = countFrame_ != 0 ? (count_ << 12) / countFrame_ : count_ << 12;
#else
    ratio.value = (count_ << 12) / countFrame_;
#endif
    current = defaultRate * (one - ratio) + rate_ * ratio;
    TownStageManager::getSingleton()->SetRGBRate(current, 0);
}

THUMB int __cmd_map_blend_color::isEnd()
{
    if (count_ >= countFrame_) {
        return true;
    }
    return false;
}

__cmd_map_blend_color g_cmd_map_blend_color;

THUMB void __cmd_map_texture_scale::initialize(char* scriptParam)
{
    PARAM_MAP_TEXTURE_SCALE* param = (PARAM_MAP_TEXTURE_SCALE*)scriptParam;
    fx32 x;
    fx32 y;
    TownStageManager::getSingleton()->getTextureScaling(x, y);
    frame_ = param->frame;
    scaleX_ = (param->scalex - x) / frame_;
    scaleY_ = (param->scaley - y) / frame_;
    counter_ = 0;
}

THUMB void __cmd_map_texture_scale::execute()
{
    fx32 x;
    fx32 y;
    TownStageManager::getSingleton()->getTextureScaling(x, y);
    TownStageManager::getSingleton()->setTextureScaling(x + scaleX_, y + scaleY_);
    counter_++;
}

THUMB int __cmd_map_texture_scale::isEnd()
{
    if (frame_ <= counter_) {
        return true;
    }
    return false;
}

__cmd_map_texture_scale g_cmd_map_texture_scale;

THUMB void __cmd_map_blend_init::initialize(char* scriptParam)
{
    PARAM_MAP_BLEND_INIT* param = (PARAM_MAP_BLEND_INIT*)scriptParam;
    rate_ = TownStageManager::getSingleton()->townData_.rate_;
    count_ = 0;
    countFrame_ = param->frame;
}

THUMB void __cmd_map_blend_init::execute()
{
    dss::Fix32 ratio;
    dss::Fix32 one;
    one.value = 0x1000;
    dss::Fix32Vector3 defaultRate = TownStageManager::getSingleton()->townData_.getDefaultPaletteRate();
    dss::Fix32Vector3 current;
    count_++;
#ifdef DQIV_NATIVE
    // A fade of zero frames: the DS's division by zero gives the dividend
    // (straight to the final colours); x86 would trap.
    ratio.value = countFrame_ != 0 ? (count_ << 12) / countFrame_ : count_ << 12;
#else
    ratio.value = (count_ << 12) / countFrame_;
#endif
    current = rate_ * (one - ratio) + defaultRate * ratio;
    TownStageManager::getSingleton()->SetRGBRate(current, 0);
}

THUMB int __cmd_map_blend_init::isEnd()
{
    if (count_ >= countFrame_) {
        return true;
    }
    return false;
}

__cmd_map_blend_init g_cmd_map_blend_init;

THUMB int cmd_debug_print(int* param)
{
    g_DebugConsole.unkfunc_0207e88c(10, 10, "%d", param[0]);
    return 1;
}

THUMB int cmd_set_item(int* param)
{
    status::g_Party.setPlayerMode();
    if (param[0] == 0x84) {
        status::g_Party.addPlayerMedalCoin(param[1]);
        return 1;
    }

    if (param[2] == 0) {
        int toSack = 1;
        for (int i = 0; i < status::g_Party.getCount(); i++) {
            status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i)->haveStatusInfo_;
            if (info->isDeath() == 1) {
                continue;
            }
            int count = info->haveItem_.getCount();
            if (count + param[1] > 12) {
                continue;
            }
            for (unsigned int j = 0; j < (unsigned int)param[1]; j++) {
                status::BaseHaveItem* haveItem = &info->haveItem_;
                haveItem->add(param[0]);
            }
            toSack = 0;
            break;
        }
        if (toSack) {
            status::g_Party.haveItemSack_.adds(param[0], param[1]);
        }
        TextAPI::setMACRO0(10, 0x40000000, param[0]);
        return 1;
    }

    int remain = param[1];
    for (int i = 0; i < status::g_Party.getCount(); i++) {
        status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(i)->haveStatusInfo_;
        if (info->haveItem_.isItem(param[0])) {
            for (int j = 0; j < info->haveItem_.getCount(); j++) {
                if (param[0] == info->haveItem_.getItem(j)) {
                    info->execThrow(j);
                    remain--;
                    j--;
                    if (remain == 0) {
                        return 1;
                    }
                }
            }
        }
    }

    if (status::g_Party.fukuro_ != 0 && status::g_Party.haveItemSack_.isItem(param[0])) {
        for (int j = 0; j < status::g_Party.haveItemSack_.getCount(); j++) {
            if (param[0] == status::g_Party.haveItemSack_.getItem(j)) {
                int count = status::g_Party.haveItemSack_.getItemCount(j);
                for (int k = 0; k < count; k++) {
                    status::g_Party.haveItemSack_.execThrow(j);
                    if (--remain == 0) {
                        return 1;
                    }
                }
                break;
            }
        }
    }
    return 1;
}

THUMB int cmd_set_gold(int* param)
{
    if (param[1] == 0) {
        status::g_Party.setGold(param[0] + status::g_Party.gold_);
    } else {
        status::g_Party.setGold(status::g_Party.gold_ - param[0]);
    }
    return 1;
}

THUMB int cmd_set_coin(int* param)
{
    if (param[1] == 0) {
        status::g_Party.setCasinoCoin(param[0] + status::g_Party.casinoCoin_);
    } else {
        status::g_Party.setCasinoCoin(status::g_Party.casinoCoin_ - param[0]);
    }
    return 1;
}

THUMB int cmd_is_procure_item(int* param)
{
    for (int i = 0; i < status::g_Party.getCount(); i++) {
        if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveItem_.isItem(param[0])) {
            return 1;
        }
    }
    if (status::g_Party.fukuro_ != 0 && status::g_Party.haveItemSack_.isItem(param[0])) {
        return 1;
    }
    return 0;
}

THUMB int cmd_mini_game(int* param)
{
    if (param[0] != 2) {
        TownPlayerManager::getSingleton()->setLock(1);
        g_cmnPartyInfo.prevLocation_ = 1;
        g_Global.setMinigame(param[0]);
        gMaterielMenu_SLOT.setSlotType(param[1]);
        g_Global.startCasino();
    } else {
        int index = getPlacementCtrlId();
        TownCharacterManager::getSingleton()->setPlayerDirection(index);
        TownWindowSystem::getSingleton()->changeShopMenuPhase(0xf);
        g_cmnPartyInfo.ctrlID_ = index;
    }
    return 1;
}

THUMB int cmd_is_hostage(int* param)
{
    if (status::g_Party.isHostage(param[0])) {
        return 1;
    }
    return 0;
}

THUMB int cmd_set_ruura_lock(int* param)
{
    g_Stage.setRulaDisable(param[1]);
    g_Stage.setRiremitoDisable(param[0]);
    return 1;
}

THUMB int cmd_set_ranaruta(int* param)
{
    g_Stage.setLanarutaDisable(param[0]);
    return 1;
}

THUMB int cmd_invalidation_rula(int* param)
{
    g_Stage.setRula(param[0]);
    g_Stage.setRiremito(param[0]);
    return 1;
}

THUMB int cmd_check_money(int* param)
{
    int result = 0;
    if (param[1] == 0) {
        if (status::g_Party.gold_ >= (unsigned int)param[0]) {
            result = 1;
        }
    } else {
        if (status::g_Party.gold_ < (unsigned int)param[0]) {
            result = 1;
        }
    }
    return result;
}

THUMB int cmd_check_hero_level(int* param)
{
    int index = status::g_Party.getSortIndex(1);
    if (index == -1) {
        index = status::g_Party.getSortIndex(2);
        if (index == -1) {
            return 0;
        }
    }
    unsigned char level = status::g_Party.getPlayerStatus(index)->haveStatusInfo_.haveStatus_.level_;
    if (level >= (unsigned int)param[0] && level <= (unsigned int)param[1]) {
        return 1;
    }
    return 0;
}

THUMB int cmd_reset_sidejob_pay(int* param)
{
    status::g_Shop.pay_ = 0;
    return 1;
}

THUMB int cmd_get_sidejob_pay(int* param)
{
    status::g_Party.addGold(status::g_Shop.pay_);
    status::g_Shop.pay_ = 0;
    return 1;
}

THUMB int cmd_check_sidejob_pay(int* param)
{
    if (param[0] == 0) {
        if (status::g_Shop.pay_ == 0) {
            if (param[1] == 0) {
                return 1;
            }
            return 0;
        }
        if (param[1] != 0) {
            return 1;
        }
        return 0;
    }
    if (status::g_Shop.pay_ >= 100) {
        if (param[1] == 0) {
            return 1;
        }
        return 0;
    }
    if (param[1] != 0) {
        return 1;
    }
    return 0;
}

THUMB int cmd_set_endor_event_item(int* param)
{
    status::g_Party.setBattleMode();
    int sortIndex = status::g_Party.getSortIndex(7);
    int i = 0;
    int swordCount = 0;
    int armorCount = 0;
    int result = 0;
    status::BaseHaveItem* haveItem = &status::g_Party.getPlayerStatus(sortIndex)->haveStatusInfo_.haveItem_;

    while (i < haveItem->getCount()) {
        if (haveItem->getItem(i) == 7) {
            if (status::g_Story.getEndorEventItemCount(status::StoryStatus::EVENT_HAGANENOTURUGI) < 6) {
                status::g_Party.getPlayerStatus(sortIndex)->haveStatusInfo_.execThrow(i);
                status::g_Story.addEndorEventItemCount(status::StoryStatus::EVENT_HAGANENOTURUGI, 1);
                swordCount++;
                i = 0;
                result = 1;
            } else {
                i++;
            }
        } else if (haveItem->getItem(i) == 0x2f) {
            if (status::g_Story.getEndorEventItemCount(status::StoryStatus::EVENT_TETUNOYOROI) < 6) {
                status::g_Party.getPlayerStatus(sortIndex)->haveStatusInfo_.execThrow(i);
                status::g_Story.addEndorEventItemCount(status::StoryStatus::EVENT_TETUNOYOROI, 1);
                armorCount++;
                i = 0;
                result = 1;
            } else {
                i++;
            }
        } else {
            i++;
        }
    }

    status::BaseHaveItem* sack = &status::g_Party.haveItemSack_;
    i = 0;
    while (i < sack->getCount()) {
        if (sack->getItem(i) == 7) {
            if (status::g_Story.getEndorEventItemCount(status::StoryStatus::EVENT_HAGANENOTURUGI) < 6) {
                status::g_Party.haveItemSack_.execThrow(i);
                status::g_Story.addEndorEventItemCount(status::StoryStatus::EVENT_HAGANENOTURUGI, 1);
                swordCount++;
                i = 0;
                result = 1;
            } else {
                i++;
            }
        } else if (sack->getItem(i) == 0x2f) {
            if (status::g_Story.getEndorEventItemCount(status::StoryStatus::EVENT_TETUNOYOROI) < 6) {
                status::g_Party.haveItemSack_.execThrow(i);
                status::g_Story.addEndorEventItemCount(status::StoryStatus::EVENT_TETUNOYOROI, 1);
                armorCount++;
                i = 0;
                result = 1;
            } else {
                i++;
            }
        } else {
            i++;
        }
    }

    status::g_Story.setGiveEventItemCount(status::StoryStatus::EVENT_HAGANENOTURUGI, swordCount);
    status::g_Story.setGiveEventItemCount(status::StoryStatus::EVENT_TETUNOYOROI, armorCount);
    return result;
}

THUMB int cmd_check_endor_event_item(int* param)
{
    if (status::g_Story.getEndorEventItemCount(status::StoryStatus::EVENT_HAGANENOTURUGI) >= 6 &&
        status::g_Story.getEndorEventItemCount(status::StoryStatus::EVENT_TETUNOYOROI) >= 6) {
        return 1;
    }
    return 0;
}

THUMB int cmd_furniture_move_request(int* param)
{
    dss::Fix32Vector3 pos;
    dss::Fix32Vector3 base;
    base = TownStageManager::getSingleton()->getMapUidPos(param[0]);
    pos.vx.value = param[1] + base.vx.value;
    pos.vy.value = param[2] + base.vy.value;
    pos.vz.value = param[3] + base.vz.value;
    if (param[4] == 0) {
        param[4] = 0x1000;
    }
    int frame = ((base - pos)).length().value / ((param[4] * defaultSpeed.value) / 4096);
    TownFurnitureControlManager::getSingleton()->setFurnitureMove(param[0], frame, pos);
    return 1;
}

THUMB void __cmd_map_camera_move::initialize(char* param)
{
    PARAM_MAP_CAMERA_MOVE* data = (PARAM_MAP_CAMERA_MOVE*)param;
    dss::Fix32Vector3 target;
    target.vx.value = data->endX;
    target.vy.value = data->endY;
    target.vz.value = data->endZ;
    TownCamera::getSingleton()->setMoveTo(target, data->moveFrame, false);
    dss::Vector3<short> angle;
    TownActionCalculate::setAngle(data->axis, data->angle, angle);
    TownCamera::getSingleton()->setRotTo(angle, data->rotFrame, false);
}

THUMB int __cmd_map_camera_move::isEnd()
{
    return TownCamera::getSingleton()->cameraMove_.isEnd();
}

__cmd_map_camera_move g_cmd_map_camera_move;

THUMB void __cmd_map_camera_position::initialize(char* param)
{
    PARAM_MAP_CAMERA_POSITION* data = (PARAM_MAP_CAMERA_POSITION*)param;
    dss::Fix32Vector3 target;
    target.vx.value = data->endX;
    target.vy.value = data->endY;
    target.vz.value = data->endZ;
    TownCamera::getSingleton()->setMoveTo(target, data->frame, false);
}

THUMB int __cmd_map_camera_position::isEnd()
{
    return TownCamera::getSingleton()->cameraMove_.isEnd();
}

__cmd_map_camera_position g_cmd_map_camera_position;

THUMB void __cmd_camera_move_abs::initialize(char* param)
{
    PARAM_CAMERA_MOVE_ABS* data = (PARAM_CAMERA_MOVE_ABS*)param;
    dss::Fix32Vector3 target;
    target.vx.value = data->posX;
    target.vy.value = data->posY;
    target.vz.value = data->posZ;
    TownCamera::getSingleton()->setMoveTo(target, data->frame, true);
}

THUMB int __cmd_camera_move_abs::isEnd()
{
    return TownCamera::getSingleton()->cameraMove_.isEnd();
}

__cmd_camera_move_abs g_cmd_camera_move_abs;

THUMB void __cmd_map_camera_angle::initialize(char* param)
{
    PARAM_MAP_CAMERA_ANGLE* data = (PARAM_MAP_CAMERA_ANGLE*)param;
    dss::Vector3<short> angle;
    TownActionCalculate::setAngle(data->axis, data->angle, angle);
    TownCamera::getSingleton()->setRotTo(angle, data->frame, false);
}

THUMB int __cmd_map_camera_angle::isEnd()
{
    return TownCamera::getSingleton()->cameraMove_.isEnd();
}

__cmd_map_camera_angle g_cmd_map_camera_angle;

THUMB void __cmd_map_camera_gaze::initialize(char* param)
{
    PARAM_MAP_CAMERA_GAZE* data = (PARAM_MAP_CAMERA_GAZE*)param;
    TownCamera::getSingleton()->resetCameraMove(data->frame);
}

THUMB int __cmd_map_camera_gaze::isEnd()
{
    return TownCamera::getSingleton()->cameraMove_.isEnd();
}

__cmd_map_camera_gaze g_cmd_map_camera_gaze;

THUMB void __cmd_map_event_camera::initialize(char* param)
{
    PARAM_MAP_EVENT_CAMERA* data = (PARAM_MAP_EVENT_CAMERA*)param;
    TownStageManager::getSingleton()->setCameraNo(data->channel, data->screen);
    TownCamera::getSingleton()->setCameraLock(true);
    countFrame_ = data->frame;
    count_ = 0;
}

THUMB void __cmd_map_event_camera::execute()
{
    count_++;
}

THUMB int __cmd_map_event_camera::isEnd()
{
    if (count_ >= countFrame_) {
        TownStageManager::getSingleton()->setCameraNo(0, 0);
        TownStageManager::getSingleton()->setCameraNo(0, 1);
        TownCamera::getSingleton()->setCameraLock(false);
        return true;
    }
    return false;
}

__cmd_map_event_camera g_cmd_map_event_camera;

THUMB void __cmd_riseup_move::initialize(char* scriptParam)
{
    PARAM_RISEUP_MOVE* param = (PARAM_RISEUP_MOVE*)scriptParam;
    dss::Fix32Vector3 startPos;
    dss::Fix32Vector3 endPos;
    startPos.vx.value = param->startX;
    startPos.vy.value = param->startY;
    startPos.vz.value = param->startZ;
    endPos.vx.value = param->endX;
    endPos.vy.value = param->endY;
    endPos.vz.value = param->endZ;
    m_index = TownRiseupManager::getSingleton()->setupScript(param->item, startPos, endPos, param->frame);
}

THUMB int __cmd_riseup_move::isEnd()
{
    return TownRiseupManager::getSingleton()->isFinish(m_index);
}

__cmd_riseup_move g_cmd_riseup_move;

THUMB void __cmd_menu_event_imuru::initialize(char* scriptParam)
{
    PARAM_MENU_EVENT_IMURU* param = (PARAM_MENU_EVENT_IMURU*)scriptParam;
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setPlayerDirection(ctrl);
    TownWindowSystem::getSingleton()->changeShopMenuPhase(0);
    MaterielMenu_WINDOW_MANAGER::getSingleton()->extraInnType_ = 1;
    g_cmnPartyInfo.ctrlID_ = ctrl;
    type_ = param->type;
    index_ = param->index;
    waitCounter_ = 0;
}

THUMB int __cmd_menu_event_imuru::isEnd()
{
    if (waitCounter_ < 1) {
        waitCounter_++;
        return false;
    }
    if (!TownWindowSystem::getSingleton()->cmdWindow_.isShopMenu()) {
        if (MaterielMenu_WINDOW_MANAGER::getSingleton()->extraImuruEnd_) {
            setFlag(true);
        } else {
            setFlag(false);
        }
        return true;
    }
    return false;
}

THUMB void __cmd_menu_event_imuru::setFlag(bool flag)
{
    switch (type_) {
    case 0:
        if (flag) {
            g_AreaFlag.set(index_);
        } else {
            g_AreaFlag.remove(index_);
        }
        break;
    case 1:
        if (flag) {
            g_LocalFlag.set(index_);
        } else {
            g_LocalFlag.remove(index_);
        }
        break;
    case 2:
        if (flag) {
            g_GlobalFlag.set(index_);
        } else {
            g_GlobalFlag.remove(index_);
        }
        break;
    }
}

__cmd_menu_event_imuru g_cmd_menu_event_imuru;

THUMB void __cmd_map_set_back_color::initialize(char* param)
{
    PARAM_MAP_SET_BACK_COLOR* data = (PARAM_MAP_SET_BACK_COLOR*)param;
    current_ = TownStageManager::getSingleton()->townData_.getCurrentBackColor();
    TownStageManager::getSingleton()->setNextBackColor(data->index);
    index_ = data->index;
    countFrame_ = data->frame;
    count_ = 0;
}

THUMB void __cmd_map_set_back_color::execute()
{
    param::FloorBackColor* current = &status::excelParam.floorBackColor_[current_];
    param::FloorBackColor* next = &status::excelParam.floorBackColor_[index_];
    dss::Fix32 ratio;
    dss::Fix32 one;
    one.value = 0x1000;
    count_++;
#ifdef DQIV_NATIVE
    // A fade of zero frames: the DS's division by zero gives the dividend
    // (straight to the final colours); x86 would trap.
    ratio.value = countFrame_ != 0 ? (count_ << 12) / countFrame_ : count_ << 12;
#else
    ratio.value = (count_ << 12) / countFrame_;
#endif
    unsigned char color[24];
    for (int i = 0; i < 24; i++) {
        color[i] = ((one - ratio) * current->color[i] + ratio * next->color[i]).value / 0x1000;
    }
    setBackgroundGradient(&color[12], &color[15], &color[18], &color[21], &color[0], &color[3], &color[6], &color[9]);
}

THUMB int __cmd_map_set_back_color::isEnd()
{
    if (count_ >= countFrame_) {
        return true;
    }
    return false;
}

__cmd_map_set_back_color g_cmd_map_set_back_color;

THUMB void __cmd_map_restore_back_color::initialize(char* param)
{
    PARAM_MAP_RESTORE_BACK_COLOR* data = (PARAM_MAP_RESTORE_BACK_COLOR*)param;
    current_ = TownStageManager::getSingleton()->townData_.getCurrentBackColor();
    index_ = TownStageManager::getSingleton()->getNextBackColor();
    countFrame_ = data->frame;
    count_ = 0;
}

THUMB void __cmd_map_restore_back_color::execute()
{
    param::FloorBackColor* current = &status::excelParam.floorBackColor_[current_];
    param::FloorBackColor* next = &status::excelParam.floorBackColor_[index_];
    dss::Fix32 ratio;
    dss::Fix32 one;
    one.value = 0x1000;
    count_++;
#ifdef DQIV_NATIVE
    // A fade of zero frames: the DS's division by zero gives the dividend
    // (straight to the final colours); x86 would trap.
    ratio.value = countFrame_ != 0 ? (count_ << 12) / countFrame_ : count_ << 12;
#else
    ratio.value = (count_ << 12) / countFrame_;
#endif
    unsigned char color[24];
    for (int i = 0; i < 24; i++) {
        color[i] = ((one - ratio) * next->color[i] + ratio * current->color[i]).value / 0x1000;
    }
    setBackgroundGradient(&color[12], &color[15], &color[18], &color[21], &color[0], &color[3], &color[6], &color[9]);
}

THUMB int __cmd_map_restore_back_color::isEnd()
{
    if (count_ >= countFrame_) {
        return true;
    }
    return false;
}

__cmd_map_restore_back_color g_cmd_map_restore_back_color;

THUMB int cmd_enable_event_item(int* param)
{
    if (TownMenuItemUseManager::getSingleton()->eventItemFlag_ != 0) {
        TownMenuItemUseManager::getSingleton()->eventItem_ = 0;
        TownMenuItemUseManager::getSingleton()->eventItemFlag_ = 0;
        return 1;
    }
    TownMenuItemUseManager::getSingleton()->setEventItem(param[0]);
    return 0;
}

THUMB int cmn_set_event_door(int* param)
{
    TownDoorAction::DOOR_OPEN_TYPE type;
    switch (param[1]) {
    case 1:
        type = TownDoorAction::DOOR_NOT_OPEN_EVENT;
        break;
    case 2:
        type = TownDoorAction::DOOR_OPEN_EVENT;
        break;
    case 3:
        type = TownDoorAction::DOOR_LOCK;
        break;
    default:
        type = TownDoorAction::DOOR_OPEN_KEY;
        break;
    }
    TownDoorAction::getSingleton()->setEventDoor(param[0], type);
    return 1;
}

THUMB int cmd_map_change_timezone(int* param)
{
    g_Stage.setTimeZoneEnable(1);
    g_Stage.setTimeZone((TIME_ZONE)param[0]);
    return 1;
}

THUMB int cmd_map_effect_sepia()
{
    applyModelPaletteSepia(getLastModelTexture());
    TownStageManager::getSingleton()->stage_.m_fld.SetSepia();
    return 1;
}

THUMB int cmd_character_not_change_direction(int* param)
{
    int index = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setLockRot(index, param[0]);
    return 1;
}

THUMB int cmd_player_action_not_change_direction(int* param)
{
    TownPlayerManager::getSingleton()->setLockRot(param[0]);
    return 1;
}

THUMB int cmd_is_character_direction(int* param)
{
    int index = getPlacementCtrlId();
    short dir = TownCharacterManager::getSingleton()->getDirection(index);
    return cmn::CommonCalculate::directionCheckByScriptParam(param[0], dir);
}

THUMB int cmd_is_player_direction(int* param)
{
    return cmn::CommonCalculate::directionCheckByScriptParam(param[0], TownPlayerManager::getSingleton()->getDirection());
}

THUMB int cmd_map_animation(int* param)
{
    TownStageManager::getSingleton()->eventAnim(param[0], 0);
    return 1;
}

THUMB int cmd_is_party_item(int* param)
{
    status::g_Party.setAllPlayerMode();
    int found[4] = {0, 0, 0, 0};
    int items[4];
    items[0] = param[3];
    items[1] = param[4];
    items[2] = param[5];
    items[3] = param[6];
    switch (param[0]) {
    case 0:
        for (int i = 0; i < status::g_Party.getCount(); i++) {
            searchItem(i, found, items);
        }
        break;
    case 1:
        for (int i = 0; i < status::g_Party.getCarriageOutCount(); i++) {
            searchItem(i, found, items);
        }
        break;
    case 2:
        if (status::g_Party.basha_ != 0) {
            int start = status::g_Party.getCarriageOutCount();
            int count = status::g_Party.getCount();
            for (int i = start; i < count; i++) {
                searchItem(i, found, items);
            }
        }
        break;
    }
    if (param[1] == 0) {
        for (int j = 0; j < status::g_Party.haveItemSack_.getCount(); j++) {
            for (int k = 0; k < 4; k++) {
                if (items[k] == status::g_Party.haveItemSack_.getItem(j)) {
                    found[k] = 1;
                }
            }
        }
    }
    if (param[2] == 0) {
        if (found[0] == 1 || found[1] == 1 || found[2] == 1 || found[3] == 1) {
            return 1;
        }
    } else {
        if (found[0] == 1 && found[1] == 1 && found[2] == 1 && found[3] == 1) {
            return 1;
        }
    }
    return 0;
}

THUMB int cmd_is_not_party_item(int* param)
{
    if (cmd_is_party_item(param) == 0) {
        return 1;
    }
    return 0;
}

THUMB void searchItem(int index, int* found, int* items)
{
    int count;
    status::HaveStatusInfo* info = &status::g_Party.getPlayerStatus(index)->haveStatusInfo_;
    if (info->haveStatus_.isPlayer() != 0) {
        count = info->haveItem_.getCount();
        for (int j = 0; j < count; j++) {
            for (int k = 0; k < 4; k++) {
                if (items[k] == info->haveItem_.getItem(j)) {
                    found[k] = 1;
                }
            }
        }
    }
}

THUMB int cmd_set_hostage()
{
    status::g_Party.setBattleMode();
    int count = status::g_Party.getCarriageOutCount();
    int player = -1;
    for (int i = 0; i < count; i++) {
        if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.isPlayer_ != 0) {
            player++;
        }
    }
    int start = 1;
    if (player > 0) {
        start = status::g_Party.getCarriageOutCount();
    }
    int hostage = -1;
    int max = status::g_Party.getCount();
    for (int i = start; i < max; i++) {
        int playerIndex = status::g_Party.getPlayerIndex(i);
        if (playerIndex != 1 && playerIndex != 2 &&
            status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveStatus_.isPlayer_ != 0) {
            hostage = playerIndex;
        }
    }
    if (hostage != -1) {
        status::g_Party.del(hostage);
        status::g_Party.setHostage(hostage, true);
        return 1;
    }
    return 0;
}

THUMB int cmd_set_party_reserve_order(int* param)
{
    status::g_Party.setBattleMode();
    int order[4] = {0, 0, 0, 0};
    int count = status::g_Party.getCarriageOutCount();
    int dead = 0;
    if (count < 4) {
        return 1;
    }
    for (int i = 0; i < count; i++) {
        order[i] = status::g_Party.getPlayerIndex(i);
        if (dead == 0 && status::g_Party.getPlayerStatus(i)->haveStatusInfo_.isDeath()) {
            order[i] = 0;
            dead = 1;
        }
    }
    if (dead == 0) {
        order[3] = 0;
    }
    status::g_Party.reorder(order[0], order[1], order[2], order[3]);
    cmn::GameManager::getSingleton()->resetParty();
    return 1;
}

THUMB int cmd_map_texture(int* param)
{
    TownStageManager::getSingleton()->setMapTexture(param[0]);
    return 1;
}

THUMB int cmd_set_map_texture(int* param)
{
    switch (param[0]) {
    case 0:
        TownStageManager::getSingleton()->setEffect((TownMapEffect::EFFECT_TYPE)0);
        break;
    case 1:
        TownStageManager::getSingleton()->setEffect((TownMapEffect::EFFECT_TYPE)1);
        break;
    }
    return 1;
}

THUMB int cmd_map_shake(int* param)
{
    TownCamera::getSingleton()->setShake(param[0], param[1]);
    return 1;
}

THUMB int cmd_effect_blur(int* param)
{
    if (param[0] == 1) {
        dss::g_DISPLAYPLUGIN_DOUBLE3D.ReqBlurMode(0);
    } else {
        dss::g_DISPLAYPLUGIN_DOUBLE3D.ReqBlurMode(1);
        dss::g_DISPLAYPLUGIN_DOUBLE3D.SetBlur(param[1], param[2]);
    }
    return 1;
}

THUMB int cmd_party_display(int* param)
{
    if (param[0] == 1) {
        TownPlayerManager::getSingleton()->partyDraw_.resetDrawPartyCount();
        TownPlayerManager::getSingleton()->partyDraw_.resetAlpha();
    } else {
        TownPlayerManager::getSingleton()->partyDraw_.setDrawPartyNone();
    }
    return 1;
}

THUMB int cmd_is_character_front(int* param)
{
    int index = getPlacementCtrlId();
    dss::Fix32Vector3 playerPos = TownPlayerManager::getSingleton()->getPosition();
    dss::Fix32Vector3 charaPos = TownCharacterManager::getSingleton()->getPosition(index);
    dss::Fix32Vector3 front;
    TownActionCalculate::getDirByIdx((short)TownCharacterManager::getSingleton()->getDirection(index), front);
    if (front * (playerPos - charaPos) >= dss::Fix32(0L)) {
        if (param[0] == 1) {
            return 1;
        }
    } else {
        if (param[0] == 0) {
            return 1;
        }
    }
    return 0;
}

THUMB int cmd_is_talked_at_shop(int* param)
{
    int index = getPlacementCtrlId();
    if (param[0] == 1) {
        if (TownCharacterManager::getSingleton()->character_[index]->getCounterTalk() == 1 &&
            TownCharacterManager::getSingleton()->isTalked(index) == 1) {
            return 1;
        }
    } else {
        if (TownCharacterManager::getSingleton()->character_[index]->getCounterTalk() == 0 &&
            TownCharacterManager::getSingleton()->isTalked(index) == 1) {
            return 1;
        }
    }
    return 0;
}

THUMB int cmd_search_map_object(int* param)
{
    if (param[0] == TownPlayerManager::getSingleton()->searchMapUid_) {
        TownPlayerManager::getSingleton()->searchAction_ = 5;
        cmn::PartyTalk::getSingleton()->resetPartyTalk();
        return 1;
    }
    return 0;
}

THUMB int cmd_set_floor_map_object(int* param)
{
    dss::Fix32Vector3 pos;
    pos.vx.value = param[1];
    pos.vy.value = param[2];
    pos.vz.value = param[3];
    TownExtraMapObjManager::getSingleton()->setData(param[0], pos);
    return 1;
}

THUMB int cmd_character_move_passive(int* param)
{
    int index = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->character_[index]->setMovePassive();
    return 1;
}

THUMB int cmd_character_move_random(int* param)
{
    int index = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->character_[index]->setMoveRandom();
    return 1;
}

THUMB int cmd_character_move_reverse(int* param)
{
    int index = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->character_[index]->setMoveReverse();
    return 1;
}

THUMB int cmd_is_trigger_distance(int* param)
{
    int index = getPlacementCtrlId();
    dss::Fix32Vector3 playerPos = TownPlayerManager::getSingleton()->getPosition();
    dss::Fix32Vector3 charaPos = TownCharacterManager::getSingleton()->getPosition(index);
    dss::Fix32 distance;
    distance.value = param[0];
    distance *= distance;
    if (TownSystem::getSingleton()->trigger_ == 1) {
        dss::Fix32 length = playerPos.lengthsq(charaPos);
        if (length <= distance) {
            TownSystem::getSingleton()->trigger_ = 0;
            return 1;
        }
    }
    return 0;
}

THUMB int cmd_character_set_coll_stage(int* param)
{
    int index = getPlacementCtrlId();
    signed char flag = 0;
    if (param[0] == 1) {
        flag |= 2;
    }
    if (param[1] == 1) {
        flag |= 1;
    }
    if (param[2] == 1) {
        flag |= 4;
    }
    TownCharacterManager::getSingleton()->character_[index]->stageColl_ = flag;
    return 1;
}

THUMB int cmd_party_redisplay(int* param)
{
    TownPlayerManager::getSingleton()->partyDraw_.resetDrawPartyCount();
    TownPlayerManager::getSingleton()->partyDraw_.resetAlpha();
    return 1;
}

THUMB void __cmd_party_move_overlap::initialize(char* scriptParam)
{
    TownPlayerManager::getSingleton()->party_.moveFirstFlag_ = 1;
    TownPlayerManager::getSingleton()->party_.setMoveToFirstHalfSpeed(SPEED_TYPE1);
}

THUMB int __cmd_party_move_overlap::isEnd()
{
    if (TownPlayerManager::getSingleton()->party_.moveFirstFlag_ == 0) {
        return true;
    }
    return false;
}

__cmd_party_move_overlap g_cmd_party_move_overlap;

THUMB void __cmd_furniture_open::initialize(char* scriptParam)
{
    PARAM_FURNITURE_OPEN* param = (PARAM_FURNITURE_OPEN*)scriptParam;
    TownFurnitureManager::getSingleton()->force_ = 1;
    TownFurnitureManager::getSingleton()->checkObject(param->target, 0, 0, 0);
    uid_ = param->target;
}

THUMB int __cmd_furniture_open::isEnd()
{
    if (!TownFurnitureManager::getSingleton()->isProcess()) {
        int objectId = TownStageManager::getSingleton()->getObjectIDfromMapUid(uid_);
        int common = TownStageManager::getSingleton()->getMapObjCommonId(objectId);
        switch (common) {
        case 0xef:
        case 0x106:
        case 0x107:
            TownActionWalk::getSingleton()->searchObjectId_ = -1;
            TownPlayerManager::getSingleton()->searchMapUid_ = 0;
            break;
        }
        return true;
    }
    return false;
}

__cmd_furniture_open g_cmd_furniture_open;

THUMB void __cmd_set_party_order::initialize(char* scriptParam)
{
    PARAM_SET_PARTY_ORDER* param = (PARAM_SET_PARTY_ORDER*)scriptParam;
    int party[4] = { 0, 0, 0, 0 };
    party[0] = param->order1;
    status::g_Party.setMemberShiftMode();
    int tmp = 0;
    for (int i = 1; i < 4; i++) {
        if (party[0] == status::g_Party.getPlayerIndex(tmp)) {
            tmp++;
        }
        if (tmp > status::g_Party.getCarriageOutCount()) {
            party[i] = 0;
        } else {
            party[i] = status::g_Party.getPlayerIndex(tmp);
        }
        tmp++;
    }
    change_ = 1;
    for (int i = 0; i < 4; i++) {
        if (party[i] != status::g_Party.getPlayerIndex(i)) {
            change_ = 0;
        }
    }
    if (change_ == 0) {
        status::g_Party.reorder(party[0], party[1], party[2], party[3]);
        cmn::GameManager::getSingleton()->resetParty();
        TownPlayerManager::getSingleton()->party_.moveFirstFlag_ = 1;
        TownPlayerManager::getSingleton()->party_.setMoveToFirstHalfSpeed(SPEED_TYPE1);
    }
}

THUMB int __cmd_set_party_order::isEnd()
{
    if (change_ == 0) {
        if (TownPlayerManager::getSingleton()->party_.moveFirstFlag_ == 0) {
            return true;
        }
        return false;
    }
    return true;
}

__cmd_set_party_order g_cmd_set_party_order;

THUMB void __cmd_character_action_jump::initialize(char* scriptParam)
{
    int ctrl = getPlacementCtrlId();
    dss::Fix32Vector3 pos(TownCharacterManager::getSingleton()->getPosition(ctrl));
    TownCharacterManager::getSingleton()->setJumpMove(ctrl, pos, 0x10);
}

THUMB int __cmd_character_action_jump::isEnd()
{
    int ctrl = getPlacementCtrlId();
    return TownCharacterManager::getSingleton()->character_[ctrl]->isScriptEnd();
}

__cmd_character_action_jump g_cmd_character_action_jump;

THUMB void __cmd_character_normal_jump::initialize(char* scriptParam)
{
    PARAM_CHARACTER_NORMAL_JUMP* param = (PARAM_CHARACTER_NORMAL_JUMP*)scriptParam;
    int ctrl = getPlacementCtrlId();
    dss::Fix32Vector3 pos;
    pos.vx.value = param->endX;
    pos.vy.value = param->endY;
    pos.vz.value = param->endZ;
    TownCharacterManager::getSingleton()->setJumpMove(ctrl, pos, param->frame);
}

THUMB int __cmd_character_normal_jump::isEnd()
{
    int ctrl = getPlacementCtrlId();
    return TownCharacterManager::getSingleton()->character_[ctrl]->isScriptEnd();
}

__cmd_character_normal_jump g_cmd_character_normal_jump;

THUMB int cmd_party_display2(int* param)
{
    if (param[0] == 0) {
        TownPlayerManager::getSingleton()->partyDraw_.setDrawPartyOne();
    } else {
        TownPlayerManager::getSingleton();
        dss::Fix32Vector3 pos = TownPlayerManager::getSingleton()->getPosition();
        TownPlayerManager::getSingleton()->setPartyToFirst(pos);
        TownPlayerManager::getSingleton()->partyDraw_.resetDrawPartyCount();
        TownPlayerManager::getSingleton()->partyDraw_.resetAlpha();
    }
    return 1;
}

THUMB void __cmd_camera_change_distance::initialize(char* scriptParam)
{
    PARAM_CAMERA_CHANGE_DISTANCE* param = (PARAM_CAMERA_CHANGE_DISTANCE*)scriptParam;
    dss::Fix32 distance;
    distance.value = param->distance;
    TownCamera::getSingleton()->setChangeDistance(param->frame, distance);
}

THUMB int __cmd_camera_change_distance::isEnd()
{
    return TownCamera::getSingleton()->isEndChangeDistance();
}

__cmd_camera_change_distance g_cmd_camera_change_distance;

THUMB int cmn_camera_lock_pov(int* param)
{
    TownCamera::getSingleton()->setLockPov(param[0]);
    return 1;
}

THUMB int cmd_furniture_fadeout(int* param)
{
    TownFurnitureControlManager::getSingleton()->setFurnitureFade(param[0], param[1], param[2], param[3]);
    return 1;
}

THUMB int cmd_effect_dream(int* param)
{
    return 1;
}

THUMB int cmd_set_script_object_direction(int* param)
{
    int index = getPlacementCtrlId(param[0]);
    TownCharacterManager::getSingleton()->setRotate(index, param[1] << 14);
    return 1;
}

THUMB int cmd_charcter_3d_rotate(int* param)
{
    dss::Vector3<short> rot;
    rot.vx = param[0];
    rot.vy = param[1];
    rot.vz = param[2];
    int index = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setRotate(index, rot);
    return 1;
}

THUMB int cmd_effect_transfer(int* param)
{
    dss::Fix32Vector3 pos;
    pos.vx.value = param[1];
    pos.vy.value = param[2];
    pos.vz.value = param[3];
    TownRiseupManager::getSingleton()->setupSprite(param[0], pos, param[4], 0);
    return 1;
}

THUMB int cmd_character_pose_change(int* param)
{
    int index = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setPosing(index, param[0]);
    return 1;
}

THUMB int cmd_player_action_dance(int* param)
{
    TownPlayerManager::getSingleton()->setManyaDance(param[0]);
    return 1;
}

THUMB int cmd_map_camera_lock_target_player(int* param)
{
    TownCamera::getSingleton()->setTargetPlayer(param[0]);
    return 1;
}

THUMB int cmd_player_set_coll(int* param)
{
    int flag = 0;
    if (param[0] == 1) {
        flag |= 2;
    }
    if (param[1] == 1) {
        flag |= 1;
    }
    TownPlayerManager::getSingleton()->scriptColl_ = flag;
    return 1;
}

THUMB int cmd_map_clipping(int* param)
{
    TownStageManager::getSingleton()->setClipping(param[0]);
    return 1;
}

THUMB void __cmd_player_rot::initialize(char* scriptParam)
{
    PARAM_PLAYER_ROT* param = (PARAM_PLAYER_ROT*)scriptParam;
    TownPlayerManager::getSingleton()->setScriptRot(param->frame, param->idx, param->rotFlag);
    endFlag_ = param->endFlag;
}

THUMB int __cmd_player_rot::isEnd()
{
    if (endFlag_ == 0) {
        if (TownPlayerManager::getSingleton()->scriptRotFlag_ == 0) {
            return true;
        }
        return false;
    }
    return true;
}

__cmd_player_rot g_cmd_player_rot;

THUMB int cmd_camera_clip_distance(int* param)
{
    dss::Fix32 dist;
    dist.value = param[0];
    TownStageManager::getSingleton()->setClipDistance(dist);
    return 1;
}

THUMB int cmd_map_camera_near(int* param)
{
    TownCamera::getSingleton()->camera_.setNear(param[0]);
    return 1;
}

THUMB void __cmd_camera_reset_distance::initialize(char* scriptParam)
{
    PARAM_CAMERA_RESET_DISTANCE* param = (PARAM_CAMERA_RESET_DISTANCE*)scriptParam;
    TownCamera::getSingleton()->resetDistance(param->frame);
}

THUMB int __cmd_camera_reset_distance::isEnd()
{
    return TownCamera::getSingleton()->isEndChangeDistance();
}

__cmd_camera_reset_distance g_cmd_camera_reset_distance;

THUMB void __cmd_camera_move_pov::initialize(char* scriptParam)
{
    PARAM_CAMERA_MOVE_POV* param = (PARAM_CAMERA_MOVE_POV*)scriptParam;
    dss::Fix32Vector3 pos = cmn::CommonCalculate::setVecByParam(param->endX, param->endY, param->endZ);
    TownCamera::getSingleton()->setPovMove(pos, param->frame, param->absFlag);
}

THUMB int __cmd_camera_move_pov::isEnd()
{
    if (TownCamera::getSingleton()->isPovMove_ == 0) {
        return true;
    }
    return false;
}

__cmd_camera_move_pov g_cmd_camera_move_pov;

THUMB void __cmd_camera_move_to_player::initialize(char* scriptParam)
{
    PARAM_CAMERA_MOVE_TO_PLAYER* param = (PARAM_CAMERA_MOVE_TO_PLAYER*)scriptParam;
    TownCamera::getSingleton()->setMoveTargetPlayer(param->frame);
}

THUMB int __cmd_camera_move_to_player::isEnd()
{
    if (TownCamera::getSingleton()->cameraMove_.isEnd() == 1) {
        TownCamera::getSingleton()->setCameraLock(false);
        return true;
    }
    return false;
}

__cmd_camera_move_to_player g_cmd_camera_move_to_player;

THUMB int cmd_disable_demolition()
{
    status::g_BattleResult.setDisablePlayerDemolition(true);
    return 1;
}

THUMB int cmd_set_camera_target(int* param)
{
    int index = getPlacementCtrlId();
    TownCamera::getSingleton()->setMoveTragetChara(index);
    return 1;
}

THUMB int cmd_character_swing_round(int* param)
{
    int index = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setSwingRound(index, param[0]);
    return 1;
}

THUMB int cmd_character_anim(int* param)
{
    int index = getPlacementCtrlId();
    int anim = param[0];
    if (anim == 0) {
        anim = -1;
    }
    TownCharacterManager::getSingleton()->setCharaAnim(index, anim);
    return 1;
}

THUMB int cmd_party_copy_character(int* param)
{
    int index = getPlacementCtrlId(param[1]);
    status::PlayerStatus* player = status::g_Party.getPlayerStatus(param[0]);
    TownCharacterManager::getSingleton()->setPosing(index, player->haveStatusInfo_.haveStatus_.charaIndex_);
    return 1;
}

THUMB int cmd_is_map_treasure(int* param)
{
    int open;
    switch (TownFurnitureManager::getSingleton()->checkCoffer(param[0])) {
    case 0:
    case 1:
        open = 0;
        break;
    case 2:
    case 3:
    case 4:
        open = 1;
        break;
    }
    if (open == param[1]) {
        return 1;
    }
    return 0;
}

THUMB int cmd_set_furniture_position(int* param)
{
    dss::Fix32Vector3 pos;
    pos.vx.value = param[0];
    pos.vy.value = param[1];
    pos.vz.value = param[2];
    TownStageManager::getSingleton()->setMapUidPosFX32(param[3], pos);
    return 1;
}

THUMB void __cmd_fadein_character::initialize(char* scriptParam)
{
    PARAM_FADEIN_CHARACTER* param = (PARAM_FADEIN_CHARACTER*)scriptParam;
    int type;
    int ctrl = getPlacementCtrlId();
    switch (param->pattern) {
    case 0:
        type = 2;
        break;
    case 1:
        type = 3;
        break;
    }
    TownCharacterManager::getSingleton()->setFadeType(ctrl, type, param->frame);
}

THUMB int __cmd_fadein_character::isEnd()
{
    int ctrl = getPlacementCtrlId();
    return TownCharacterManager::getSingleton()->character_[ctrl]->isEndFade();
}

__cmd_fadein_character g_cmd_fadein_character;

THUMB void __cmd_fadeout_character::initialize(char* scriptParam)
{
    PARAM_FADEIN_CHARACTER* param = (PARAM_FADEIN_CHARACTER*)scriptParam;
    int type;
    int ctrl = getPlacementCtrlId();
    switch (param->pattern) {
    case 0:
        type = 4;
        break;
    case 1:
        type = 5;
        break;
    }
    TownCharacterManager::getSingleton()->setFadeType(ctrl, type, param->frame);
}

THUMB int __cmd_fadeout_character::isEnd()
{
    int ctrl = getPlacementCtrlId();
    return TownCharacterManager::getSingleton()->character_[ctrl]->isEndFade();
}

__cmd_fadeout_character g_cmd_fadeout_character;

THUMB int cmd_is_doorway(int* param)
{
    int doorway = StageLink::getTownExitIndex();
    if (doorway == param[0]) {
        return 1;
    }
    return 0;
}

THUMB void __cmd_charcter_3d_motion::initialize(char* scriptParam)
{
    PARAM_CHARCTER_3D_MOTION* param = (PARAM_CHARCTER_3D_MOTION*)scriptParam;
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setMotion(ctrl, param->motion, param->flag == 0);
}

THUMB int __cmd_charcter_3d_motion::isEnd()
{
    int ctrl = getPlacementCtrlId();
    return TownCharacterManager::getSingleton()->character_[ctrl]->isMotion();
}

__cmd_charcter_3d_motion g_cmd_charcter_3d_motion;

THUMB void __cmd_charcter_motion::initialize(char* scriptParam)
{
    PARAM_CHARCTER_MOTION* param = (PARAM_CHARCTER_MOTION*)scriptParam;
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setMotion(ctrl, param->motion, param->flag == 0);
}

THUMB int __cmd_charcter_motion::isEnd()
{
    int ctrl = getPlacementCtrlId();
    return TownCharacterManager::getSingleton()->character_[ctrl]->isMotion();
}

__cmd_charcter_motion g_cmd_charcter_motion;

THUMB void __cmd_menu_shop::initialize(char* scriptParam)
{
    PARAM_MENU_SHOP* param = (PARAM_MENU_SHOP*)scriptParam;
    shop_ = param->shop;
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setPlayerDirection(ctrl);
    TownWindowSystem::getSingleton()->changeShopMenuPhase(shop_);
    MaterielMenu_WINDOW_MANAGER::getSingleton()->extraInnType_ = 0;
    g_cmnPartyInfo.ctrlID_ = ctrl;
}

THUMB int __cmd_menu_shop::isEnd()
{
    return m_end.check();
}

__cmd_menu_shop g_cmd_menu_shop;

THUMB void __cmd_menu_extra_shop::initialize(char* scriptParam)
{
    PARAM_MENU_SHOP* param = (PARAM_MENU_SHOP*)scriptParam;
    int ctrl = getPlacementCtrlId();
    int type = 0;
    TownCharacterManager::getSingleton()->setPlayerDirection(ctrl);
    if (param->shop == 0) {
        type = MaterielMenu_WINDOW_MANAGER::MENU_EXTRA_FOX_TOWN;
    }
    if (param->shop == 1) {
        type = MaterielMenu_WINDOW_MANAGER::MENU_EXTRA_BONMOL_CASTLE;
    }
    if (param->shop == 2) {
        type = MaterielMenu_WINDOW_MANAGER::MENU_EXTRA_IMUL;
    }
    TownWindowSystem::getSingleton()->changeShopMenuPhase(type);
    g_cmnPartyInfo.ctrlID_ = ctrl;
}

THUMB int __cmd_menu_extra_shop::isEnd()
{
    return m_end.check();
}

__cmd_menu_extra_shop g_cmd_menu_extra_shop;

THUMB void __cmd_menu_present_exp::initialize(char* scriptParam)
{
    PARAM_MENU_PRESENT_EXP* param = (PARAM_MENU_PRESENT_EXP*)scriptParam;
    int ctrl = getPlacementCtrlId();
    int index = status::g_Party.getSortIndex(param->index);
    if (param->index <= 2 || index != -1) {
        if (param->index == 1 && index == -1) {
            index = status::g_Party.getSortIndex(2);
        } else if (param->index == 2 && index == -1) {
            index = status::g_Party.getSortIndex(1);
        }
        MaterielMenuPlayerControl::getSingleton()->activeChara_ = index;
        MaterielMenuPlayerControl::getSingleton()->setExtraExp(param->exp);
        TownCharacterManager::getSingleton()->setPlayerDirection(ctrl);
        TownWindowSystem::getSingleton()->changeShopMenuPhase(MaterielMenu_WINDOW_MANAGER::MENU_EXTRA_PRESENT_EXP);
        g_cmnPartyInfo.ctrlID_ = ctrl;
    }
}

THUMB int __cmd_menu_present_exp::isEnd()
{
    return m_end.check();
}

__cmd_menu_present_exp g_cmd_menu_present_exp;

THUMB void __cmd_menu_colosseum::initialize(char* scriptParam)
{
    PARAM_MENU_COLOSSEUM* param = (PARAM_MENU_COLOSSEUM*)scriptParam;
    int ctrl = getPlacementCtrlId();
    MaterielMenuPlayerControl::getSingleton()->allClear();
    MaterielMenuPlayerControl::getSingleton()->setWins(param->wins);
    TownCharacterManager::getSingleton()->setPlayerDirection(ctrl);
    TownWindowSystem::getSingleton()->changeShopMenuPhase(MaterielMenu_WINDOW_MANAGER::MENU_EXTRA_COLOSSEUM);
    g_cmnPartyInfo.ctrlID_ = ctrl;
}

THUMB int __cmd_menu_colosseum::isEnd()
{
    return m_end.check();
}

__cmd_menu_colosseum g_cmd_menu_colosseum;

THUMB void __cmd_menu_hostage::initialize(char* scriptParam)
{
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setPlayerDirection(ctrl);
    m_end.init();
    TownWindowSystem::getSingleton()->changeShopMenuPhase(MaterielMenu_WINDOW_MANAGER::MENU_EXTRA_HOSTAGE);
    g_cmnPartyInfo.ctrlID_ = ctrl;
}

THUMB int __cmd_menu_hostage::isEnd()
{
    return m_end.check();
}

__cmd_menu_hostage g_cmd_menu_hostage;

THUMB void __cmd_menu_nene::initialize(char* scriptParam)
{
    m_end.init();
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setPlayerDirection(ctrl);
    TownWindowSystem::getSingleton()->changeShopMenuPhase(MaterielMenu_WINDOW_MANAGER::MENU_EXTRA_NENE);
    g_cmnPartyInfo.ctrlID_ = ctrl;
}

THUMB int __cmd_menu_nene::isEnd()
{
    return m_end.check();
}

__cmd_menu_nene g_cmd_menu_nene;

THUMB void __cmd_player_line_move::initialize(char* scriptParam)
{
    PARAM_PLAYER_LINE_MOVE* param = (PARAM_PLAYER_LINE_MOVE*)scriptParam;
    dss::Fix32Vector3 start = TownPlayerManager::getSingleton()->getPosition();
    dss::Fix32Vector3 target = cmn::CommonCalculate::getAxisMoveTargetByParam(param->axis, param->absFlag, param->value, start);
    TownPlayerManager::getSingleton()->setSimpleMove(start, target, param->frame);
}

THUMB void __cmd_player_line_move::execute()
{
}

THUMB int __cmd_player_line_move::isEnd()
{
    if (TownPlayerManager::getSingleton()->isFinish() != 0) {
        return true;
    }
    return false;
}

__cmd_player_line_move g_cmd_player_line_move;

THUMB void __cmd_player_line_move2::initialize(char* scriptParam)
{
    PARAM_PLAYER_LINE_MOVE2* param = (PARAM_PLAYER_LINE_MOVE2*)scriptParam;
    dss::Fix32Vector3 start = TownPlayerManager::getSingleton()->getPosition();
    dss::Fix32Vector3 target = cmn::CommonCalculate::getAxisMoveTargetByParam(param->axis, param->absFlag, param->value, start);
    dss::Fix32 speed;
    speed.value = param->rate;
    TownPlayerManager::getSingleton()->setSpeedMove(start, target, speed);
}

THUMB void __cmd_player_line_move2::execute()
{
}

THUMB int __cmd_player_line_move2::isEnd()
{
    if (TownPlayerManager::getSingleton()->isFinish() != 0) {
        return true;
    }
    return false;
}

__cmd_player_line_move2 g_cmd_player_line_move2;

THUMB void __cmd_ikada_move2_player_get_on::initialize(char* scriptParam)
{
    PARAM_IKADA_MOVE2_PLAYER_GET_ON* param = (PARAM_IKADA_MOVE2_PLAYER_GET_ON*)scriptParam;
    dss::Fix32Vector3 start = TownPlayerManager::getSingleton()->getPosition();
    dss::Fix32Vector3 target = cmn::CommonCalculate::setVecByParam(param->posX, param->posY, param->posZ);
    dss::Fix32 speed;
    speed.value = param->rate;
    if (param->absFlag == 1) {
        target += start;
    }
    TownPlayerManager::getSingleton()->setIkadaSpeedMove(start, target, speed);
}

THUMB int __cmd_ikada_move2_player_get_on::isEnd()
{
    if (TownPlayerManager::getSingleton()->isFinish() != 0) {
        return true;
    }
    return false;
}

__cmd_ikada_move2_player_get_on g_cmd_ikada_move2_player_get_on;

THUMB void __cmd_ikada_move_player_get_on::initialize(char* scriptParam)
{
    PARAM_IKADA_MOVE_PLAYER_GET_ON* param = (PARAM_IKADA_MOVE_PLAYER_GET_ON*)scriptParam;
    dss::Fix32Vector3 start = TownPlayerManager::getSingleton()->getPosition();
    dss::Fix32Vector3 target = cmn::CommonCalculate::setVecByParam(param->posX, param->posY, param->posZ);
    if (param->absFlag == 1) {
        target += start;
    }
    TownPlayerManager::getSingleton()->setIkadaFrameMove(start, target, param->frame);
}

THUMB int __cmd_ikada_move_player_get_on::isEnd()
{
    if (TownPlayerManager::getSingleton()->isFinish() != 0) {
        return true;
    }
    return false;
}

__cmd_ikada_move_player_get_on g_cmd_ikada_move_player_get_on;

THUMB int cmd_copy_party_chara(int* param)
{
    dss::Fix32Vector3 pos;
    int index = getPlacementCtrlId();
    int value;
    short dir;
    if (TownPlayerManager::getSingleton()->getPlayerCopyInfo(param[0], pos, dir, value) == 1) {
        TownCharacterManager::getSingleton()->setCopyPlayerChara(index, pos, dir, value);
    }
    return 1;
}

THUMB int cmd_chara_shadow(int* param)
{
    int index = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setShadow(index, param[0]);
    return 1;
}

THUMB int cmd_chara_alpha(int* param)
{
    int index = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setAlpha(index, param[0]);
    return 1;
}

THUMB int cmd_ikada_set_position(int* param)
{
    dss::Fix32Vector3 pos = cmn::CommonCalculate::setVecByParam(param[0], param[1], param[2]);
    TownIkadaAction2::getSingleton()->setIkadaPosition(pos);
    return 1;
}

THUMB int cmd_crack_key_by_orin(int* param)
{
    TownDoorAction::getSingleton()->crackOrin_ = 1;
    return 1;
}

THUMB int cmd_set_big_rock_move(int* param)
{
    int index = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->character_[index]->setMoveBigRock();
    return 1;
}

THUMB void __cmd_set_camera_target_chara_frame::initialize(char* scriptParam)
{
    PARAM_SET_CAMERA_TARGET_CHARA_FRAME* data = (PARAM_SET_CAMERA_TARGET_CHARA_FRAME*)scriptParam;
    objNo_ = data->objId;
    int ctrl = getPlacementCtrlId(objNo_);
    dss::Fix32Vector3 target(TownCharacterManager::getSingleton()->getPosition(ctrl));
    TownCamera::getSingleton()->setMoveTo(target, data->frame, true);
}

THUMB int __cmd_set_camera_target_chara_frame::isEnd()
{
    int ctrl = getPlacementCtrlId(objNo_);
    if (TownCamera::getSingleton()->cameraMove_.isEnd() == 1) {
        TownCamera::getSingleton()->setMoveTragetChara(ctrl);
        return true;
    }
    return false;
}

__cmd_set_camera_target_chara_frame g_cmd_set_camera_target_chara_frame;

THUMB void __cmd_set_camera_angle_abs::initialize(char* scriptParam)
{
    PARAM_SET_CAMERA_ANGLE_ABS* data = (PARAM_SET_CAMERA_ANGLE_ABS*)scriptParam;
    dss::Vector3short& nowAngle = TownCamera::getSingleton()->camera_.unk_004.getAngle();
    short x = nowAngle.vx;
    short z = nowAngle.vz;
    dss::Vector3<short> angle;
    angle.set(x, data->angleY, z);
    TownCamera::getSingleton()->setRotTo(angle, data->frame, true);
}

THUMB int __cmd_set_camera_angle_abs::isEnd()
{
    getPlacementCtrlId();
    if (TownCamera::getSingleton()->cameraMove_.isEnd() == 1) {
        return true;
    }
    return false;
}

__cmd_set_camera_angle_abs g_cmd_set_camera_angle_abs;

THUMB int cmd_set_monster_talk(int* param)
{
    int index = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setMonsterTalk(index, param[0]);
    return 1;
}

THUMB int cmd_set_monster_talk_all(int* param)
{
    getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setMonsterSpeakAll(param[0]);
    TownCharacterBase::monsterTalk_ = param[0];
    return 1;
}

THUMB void __cmd_door_action::initialize(char* scriptParam)
{
    PARAM_DOOR_ACTION* data = (PARAM_DOOR_ACTION*)scriptParam;
    TownDoorAction::getSingleton()->scriptOpen(data->door1, data->door2, data->type);
}

THUMB int __cmd_door_action::isEnd()
{
    return TownDoorAction::getSingleton()->scriptEnd();
}

__cmd_door_action g_cmd_door_action;

THUMB int cmd_set_chara_map_uid(int* param)
{
    int index = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setMapUid(index, param[0]);
    return 1;
}

THUMB void __cmd_make_surechigai_taishi::initialize(char* scriptParam)
{
    m_end.init();
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setPlayerDirection(ctrl);
    TownWindowSystem::getSingleton()->changeShopMenuPhase(MaterielMenu_WINDOW_MANAGER::MENU_SURECHIGAI_MAKE_TAISHI);
    g_cmnPartyInfo.ctrlID_ = ctrl;
}

THUMB int __cmd_make_surechigai_taishi::isEnd()
{
    return m_end.check();
}

__cmd_make_surechigai_taishi g_cmd_make_surechigai_taishi;

THUMB void __cmd_surechigai_save::initialize(char* scriptParam)
{
    m_end.init();
    type_ = ((int*)scriptParam)[0];
    index_ = ((int*)scriptParam)[1];
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setPlayerDirection(ctrl);
    TownWindowSystem::getSingleton()->changeShopMenuPhase(MaterielMenu_WINDOW_MANAGER::MENU_SAVE);
    MaterielMenu_WINDOW_MANAGER::getSingleton()->type_ = 3;
    g_cmnPartyInfo.ctrlID_ = ctrl;
}

THUMB int __cmd_surechigai_save::isEnd()
{
    if (!TownWindowSystem::getSingleton()->cmdWindow_.isShopMenu() && !m_end.m_flag) {
        m_end.m_flag = 1;
        return false;
    }
    if (!TownWindowSystem::getSingleton()->cmdWindow_.isShopMenu() && m_end.m_flag) {
        if (MaterielMenu_WINDOW_MANAGER::getSingleton()->surechigaiStart_ == 1) {
            setFlag(true);
        } else {
            setFlag(false);
        }
        return true;
    }
    return false;
}

THUMB void __cmd_surechigai_save::setFlag(bool flag)
{
    switch (type_) {
    case 0:
        if (flag) {
            g_AreaFlag.set(index_);
        } else {
            g_AreaFlag.remove(index_);
        }
        break;
    case 1:
        if (flag) {
            g_LocalFlag.set(index_);
        } else {
            g_LocalFlag.remove(index_);
        }
        break;
    case 2:
        if (flag) {
            g_GlobalFlag.set(index_);
        } else {
            g_GlobalFlag.remove(index_);
        }
        break;
    }
}

__cmd_surechigai_save g_cmd_surechigai_save;

THUMB int cmd_set_my_taishi(int* param)
{
    g_EnvoyManager.mode_ = 1;
    int index = getPlacementCtrlId();
    int type = g_EnvoyManager.getType();
    int value = ImmigrantTown::getSingleton()->getTypePose(type);
    TownCharacterManager::getSingleton()->character_[index]->changePose(value);
    return 1;
}

THUMB void __cmd_surechigai_root::initialize(char* scriptParam)
{
    int* param = (int*)scriptParam;
    m_end.init();
    type1_ = param[0];
    type2_ = param[2];
    index1_ = param[1];
    index2_ = param[3];
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setPlayerDirection(ctrl);
    TownWindowSystem::getSingleton()->changeShopMenuPhase(MaterielMenu_WINDOW_MANAGER::MENU_SURECHIGAI_ROOT);
    g_cmnPartyInfo.ctrlID_ = ctrl;
}

THUMB int __cmd_surechigai_root::isEnd()
{
    if (!TownWindowSystem::getSingleton()->cmdWindow_.isShopMenu() && !m_end.m_flag) {
        m_end.m_flag = 1;
        return false;
    }
    if (!TownWindowSystem::getSingleton()->cmdWindow_.isShopMenu() && m_end.m_flag) {
        if (MaterielMenu_WINDOW_MANAGER::getSingleton()->surechigaiStart_ == 1) {
            setFlag(true, true);
        } else {
            setFlag(false, true);
        }
        if (MaterielMenu_WINDOW_MANAGER::getSingleton()->changeTaishi_ == 1) {
            setFlag(true, false);
        } else {
            setFlag(false, false);
        }
        MaterielMenu_WINDOW_MANAGER::getSingleton()->surechigaiStart_ = 0;
        MaterielMenu_WINDOW_MANAGER::getSingleton()->changeTaishi_ = 0;
        return true;
    }
    return false;
}

THUMB void __cmd_surechigai_root::setFlag(bool flag, bool first)
{
    int type = type1_;
    int index = index1_;
    if (!first) {
        type = type2_;
        index = index2_;
    }
    switch (type) {
    case 0:
        if (flag) {
            g_AreaFlag.set(index);
        } else {
            g_AreaFlag.remove(index);
        }
        break;
    case 1:
        if (flag) {
            g_LocalFlag.set(index);
        } else {
            g_LocalFlag.remove(index);
        }
        break;
    case 2:
        if (flag) {
            g_GlobalFlag.set(index);
        } else {
            g_GlobalFlag.remove(index);
        }
        break;
    }
}

__cmd_surechigai_root g_cmd_surechigai_root;

THUMB void __cmd_surechigai_mapname::initialize(char* scriptParam)
{
    m_end.init();
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setPlayerDirection(ctrl);
    TownWindowSystem::getSingleton()->changeShopMenuPhase(MaterielMenu_WINDOW_MANAGER::MENU_SURECHIGAI_MAP_NAME);
    MaterielMenu_WINDOW_MANAGER::getSingleton()->editType_ = MaterielMenu_WINDOW_MANAGER::EDIT_TOWN_NAME;
    g_cmnPartyInfo.ctrlID_ = ctrl;
}

THUMB int __cmd_surechigai_mapname::isEnd()
{
    return m_end.check();
}

__cmd_surechigai_mapname g_cmd_surechigai_mapname;

THUMB void __cmd_surechigai_message::initialize(char* scriptParam)
{
    m_end.init();
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setPlayerDirection(ctrl);
    TownWindowSystem::getSingleton()->changeShopMenuPhase(MaterielMenu_WINDOW_MANAGER::MENU_SURECHIGAI_MAP_NAME);
    MaterielMenu_WINDOW_MANAGER::getSingleton()->editType_ = MaterielMenu_WINDOW_MANAGER::EDIT_MESSAGE;
    MaterielMenu_WINDOW_MANAGER::getSingleton()->editMessageForScript_ = 1;
    g_cmnPartyInfo.ctrlID_ = ctrl;
}

THUMB int __cmd_surechigai_message::isEnd()
{
    if (!TownWindowSystem::getSingleton()->cmdWindow_.isShopMenu() && !m_end.m_flag) {
        m_end.m_flag = 1;
        return false;
    }
    if (!TownWindowSystem::getSingleton()->cmdWindow_.isShopMenu() && m_end.m_flag) {
        MaterielMenu_WINDOW_MANAGER::getSingleton()->editMessageForScript_ = 0;
        return true;
    }
    return false;
}

__cmd_surechigai_message g_cmd_surechigai_message;

THUMB int cmd_change_surechigai_part(int* param)
{
    g_Global.startSurechigai();
    return 1;
}

THUMB int cmd_check_surechigai_success(int* param)
{
    return 0;
}

THUMB int cmd_set_default_map_name(int* param)
{
    char name[42];
    for (int i = 0; i < 42; i++) {
        name[i] = 0;
    }
    getDefaultTownName(name, 42, param[0]);
    g_EnvoyManager.mode_ = 1;
    g_EnvoyManager.setTownName(name);
    return 0;
}

THUMB int cmd_check_taishi_max(int* param)
{
    if (g_EnvoyManager.getEnvoyCount() == 0x18) {
        return 1;
    }
    return 0;
}

THUMB int cmd_set_ikada_info(int* param)
{
    dss::Fix32Vector3 pos = cmn::CommonCalculate::setVecByParam(param[4], param[5], param[6]);
    TownIkadaAction2::getSingleton()->setIkadaDataByScript((const char*)param, pos);
    return 1;
}

THUMB int cmd_is_trigger3(int* scriptParam)
{
    PARAM_IS_TRIGGER3* param = (PARAM_IS_TRIGGER3*)scriptParam;
    dss::Fix32Vector3 start;
    dss::Fix32Vector3 end;
    start.vx.value = param->startX;
    start.vy.value = param->startY;
    start.vz.value = param->startZ;
    end.vx.value = param->endX;
    end.vy.value = param->endY;
    end.vz.value = param->endZ;
    dss::Fix32Vector3 pos = TownPlayerManager::getSingleton()->getPosition();
    short idx = TownPlayerManager::getSingleton()->getDirection();
    static dss::Fix32Vector3 returnPos(0, 0, 0);
    static int firstFlag = true;
    if (g_cmnPartyInfo.prevFrameBattle_ == 1) {
        g_cmnPartyInfo.position_ = g_cmnPartyInfo.beforeBattlePos_;
        return false;
    }
    if (cmn::CommonCalculate::areaCheck(pos, idx, start, end, 0, 6) == 1) {
        g_cmnPartyInfo.position_ = g_cmnPartyInfo.prev_position_;
        if (firstFlag == 0) {
            if (cmn::CommonCalculate::areaCheck(g_cmnPartyInfo.position_, idx, start, end, 0, 6) == 1) {
                g_cmnPartyInfo.position_ = returnPos;
            }
        }
        return true;
    }
    returnPos = pos;
    firstFlag = 0;
    return false;
}

THUMB int cmd_set_player_sleep(int* param)
{
    TownPlayerManager::getSingleton()->setPlayerSleep(param[0]);
    return 1;
}

THUMB int cmd_is_open_door(int* param)
{
    int open = TownFurnitureManager::getSingleton()->isOpenDoor(param[1]);
    if (param[0] == 1) {
        return open;
    }
    if (open == 0) {
        return 1;
    }
    return 0;
}

THUMB int cmd_chara_lock_move(int* param)
{
    int index = getPlacementCtrlId(param[1]);
    TownCharacterManager::getSingleton()->setLockMove(index, param[0]);
    return 1;
}

THUMB int cmd_get_reward_tom(int* param)
{
    int gold = dssrand::rand(11);
    status::g_Party.addGold(gold + 2);
    TextAPI::setMACRO0(0x35, 0xf0000000, gold + 2);
    return 1;
}

THUMB void __cmd_set_wait_enable_lock::initialize(char* scriptParam)
{
    PARAM_SET_WAIT_ENABLE_LOCK* param = (PARAM_SET_WAIT_ENABLE_LOCK*)scriptParam;
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setMoveWait(ctrl, param->frame);
}

THUMB int __cmd_set_wait_enable_lock::isEnd()
{
    int ctrl = getPlacementCtrlId();
    return TownCharacterManager::getSingleton()->character_[ctrl]->isMoveWaitEnd();
}

__cmd_set_wait_enable_lock g_cmd_set_wait_enable_lock;

THUMB int cmd_check_hit_surface(int* param)
{
    for (int i = 0; i < 14; i++) {
        int id = TownStageManager::getSingleton()->getHitSurfaceIdByType(i);
        if (id == param[0]) {
            return 1;
        }
    }
    return 0;
}

THUMB void __cmd_chara_move_line_to_player::initialize(char* scriptParam)
{
    PARAM_CHARA_MOVE_LINE_TO_PLAYER* param = (PARAM_CHARA_MOVE_LINE_TO_PLAYER*)scriptParam;
    int ctrl = getPlacementCtrlId();
    dss::Fix32Vector3 start(TownCharacterManager::getSingleton()->getPosition(ctrl));
    dss::Fix32Vector3 end = TownPlayerManager::getSingleton()->getPosition();
    end.vy = start.vy;
    dss::Fix32Vector3 vec = (end - start);
    if (param->line == 0) {
        if (vec.vx < dss::Fix32(0L)) {
            end.vx.value += param->offset;
        } else {
            end.vx.value -= param->offset;
        }
        end.vz = start.vz;
    } else {
        if (vec.vz < dss::Fix32(0L)) {
            end.vz.value += param->offset;
        } else {
            end.vz.value -= param->offset;
        }
        end.vx = start.vx;
    }
    TOWN_SCRIPT_DATA scriptData;
    dss::memset(&scriptData, 0, sizeof(scriptData));
    scriptData.node[0] = start;
    scriptData.node[1] = end;
    if (param->rate == 0) {
        param->rate = 0x1000;
    }
    scriptData.frame = ((scriptData.node[1] - scriptData.node[0])).length().value / (param->rate * defaultSpeed.value / 0x1000);
    TownCharacterManager::getSingleton()->setScriptData(ctrl, scriptData);
    TownCharacterManager::getSingleton()->character_[ctrl]->setSimpleMove();
}

THUMB int __cmd_chara_move_line_to_player::isEnd()
{
    int ctrl = getPlacementCtrlId();
    if (TownCharacterManager::getSingleton()->character_[ctrl]->isScriptEnd() != 0) {
        return true;
    }
    return false;
}

__cmd_chara_move_line_to_player g_cmd_chara_move_line_to_player;

THUMB int cmd_set_door_close(int* param)
{
    g_Stage.initDoorOpenFlag();
    return 1;
}

THUMB int cmd_map_camera_default_angle(int* param)
{
    dss::Vector3<short> rot;
    rot.set(param[0], param[1], param[2]);
    TownCamera::getSingleton()->setDefaultAngle(rot);
    return 1;
}

THUMB int cmd_chara_mortion_lock(int* param)
{
    int index = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setAction(index, param[0]);
    TownCharacterManager::getSingleton()->setAnimation(index, 0);
    return 1;
}

THUMB int cmd_start_game(int* param)
{
    g_Global.startGame();
    Sound::unkfunc_02055998(0xf);
    return 1;
}

THUMB int cmd_opening_backcolor(int* param)
{
    const int openingIndex = 0x9b;
    // two more local consts: like openingIndex they only live in the dead-stripped .rodata of this TU,
    // created when the functions are generated; the data layout needs them (their real functions are unknown)
    const int unused0 = 0;
    const int unused1 = 0;
    cmn::CommonEffectLocation::getSingleton()->start(openingIndex, 0);
    return 1;
}

THUMB int cmd_set_fighting_colosseum_mode(int* param)
{
    return 1;
}

THUMB int cmd_set_camera_limit(int* param)
{
    dss::Fix32 limit;
    if (param[0] == 0) {
        limit.value = -0x1000;
        TownCamera::getSingleton()->setLimitL(limit);
        TownCamera::getSingleton()->setLimitR(limit);
    } else {
        limit.value = 0x1e000;
        TownCamera::getSingleton()->setLimitL(limit);
        TownCamera::getSingleton()->setLimitR(limit);
    }
    return 1;
}

THUMB int cmd_set_van_and_basha(int* param)
{
    TownPlayerManager::getSingleton()->setVanAndBasha();
    return 1;
}

THUMB void __cmd_set_chara_rot::initialize(char* scriptParam)
{
    PARAM_SET_CHARA_ROT* param = (PARAM_SET_CHARA_ROT*)scriptParam;
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setRotFrame(ctrl, param->frame, param->idx, param->rotFlag, param->endFlag);
}

THUMB int __cmd_set_chara_rot::isEnd()
{
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->character_[ctrl]->isRotFrameEnd();
    return true;
}

__cmd_set_chara_rot g_cmd_set_chara_rot;

THUMB int cmd_set_basha_go_into(int* param)
{
    g_Stage.setBashaEnter(param[0]);
    return 1;
}

THUMB int cmd_chara_voice(int* param)
{
    int index = getPlacementCtrlId();
    int voice;
    switch (param[0]) {
    case 0:
        voice = 0x30;
        break;
    case 1:
        voice = 0x31;
        break;
    case 2:
        voice = 0x32;
        break;
    case 3:
        voice = 0x33;
        break;
    case 4:
        voice = 0x39;
        break;
    default:
        voice = 0x32;
        break;
    }
    TownCharacterManager::getSingleton()->character_[index]->setVoice(voice);
    return 1;
}

THUMB int cmd_check_player_item(int* param)
{
    status::g_Party.setPlayerMode();
    int i = 0;
    int end;
    if (param[0] == 0) {
        end = status::g_Party.getCarriageOutCount();
    } else if (param[0] == 1) {
        i = status::g_Party.getCarriageOutCount();
        end = status::g_Party.getCount();
    } else {
        end = status::g_Party.getCount();
    }
    for (; i < end; i++) {
        if (param[1] == 0) {
            if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveItem_.getCount() < 12) {
                return 1;
            }
        } else if (param[1] == status::g_Party.getPlayerIndex(i)) {
            if (status::g_Party.getPlayerStatus(i)->haveStatusInfo_.haveItem_.getCount() < 12) {
                return 1;
            }
            return 0;
        }
    }
    return 0;
}

THUMB int cmd_save_last_party()
{
    g_Stage.setBashaEnable(true);
    status::g_Party.basha_ = 1;
    status::g_Party.setNormalMode();
    for (int i = 0; i < 10; i++) {
        if (status::g_Party.getCount() > i) {
            g_Stage.lastParty_[i] = status::g_Party.getPlayerIndex(i);
        } else {
            g_Stage.lastParty_[i] = 0;
        }
    }
    return 1;
}

THUMB void __cmd_set_end_roll::initialize(char*)
{
    TownEndrollManager::getSingleton()->setup();
}

THUMB int __cmd_set_end_roll::isEnd()
{
    return TownEndrollManager::getSingleton()->isStaffRollEnd();
}

__cmd_set_end_roll g_cmd_set_end_roll;

THUMB int cmd_set_chara_motion2(int* param)
{
    int index = getPlacementCtrlId();
    int flag = param[1] == 0 ? 1 : 0;
    TownCharacterManager::getSingleton()->setMotion(index, param[0], flag);
    return 1;
}

THUMB void __cmd_party_move_to_first2::initialize(char* scriptParam)
{
    PARAM_PARTY_MOVE_TO_FIRST2* param = (PARAM_PARTY_MOVE_TO_FIRST2*)scriptParam;
    SPEED_TYPE rate;
    switch (param->rate) {
    case 0:
        rate = SPEED_TYPE2;
        break;
    case 1:
        rate = SPEED_TYPE1;
        break;
    default:
        rate = SPEED_TYPE1;
        break;
    }
    TownPlayerManager::getSingleton()->party_.moveFirstFlag_ = 1;
    TownPlayerManager::getSingleton()->party_.setMoveToFirstHalfSpeed(rate);
}

THUMB int __cmd_party_move_to_first2::isEnd()
{
    if (TownPlayerManager::getSingleton()->party_.moveFirstFlag_ == 0) {
        return true;
    }
    return false;
}

__cmd_party_move_to_first2 g_cmd_party_move_to_first2;

THUMB int cmd_set_unused_extra_chara(int* param)
{
    ImmigrantTown::getSingleton()->placeQueuedResidents();
    return 1;
}

THUMB int cmd_check_shoplist(int* param)
{
    char name[3] = {0, 0, 0};
    const char* cc = "cc";
    const char* mc = "mc";
    name[0] = g_Global.getMapName()[0];
    name[1] = g_Global.getMapName()[1];

    if (dss::strcmp(name, mc) == 0 && !g_AreaFlag.check(0x57)) {
        TownWindowSystem::getSingleton()->cmdWindow_.setShoplistPermit(false);
    }
    if (dss::strcmp(name, cc) == 0 && status::g_Story.chapter_ != 2) {
        TownWindowSystem::getSingleton()->cmdWindow_.setShoplistPermit(false);
    }
    return 1;
}

THUMB void __cmd_character_rgb_anim2::initialize(char* scriptParam)
{
    PARAM_CHARACTER_RGB_ANIM2* param = (PARAM_CHARACTER_RGB_ANIM2*)scriptParam;
    int ctrl = getPlacementCtrlId();
    TownCharacterManager::getSingleton()->setChangePaletteRate(ctrl, param->R, param->G, param->B, param->frame);
    endFlag_ = param->flag;
}

THUMB int __cmd_character_rgb_anim2::isEnd()
{
    int ctrl = getPlacementCtrlId();
    if (endFlag_ == 0) {
        return true;
    }
    return TownCharacterManager::getSingleton()->character_[ctrl]->isEndPalletRate();
}

__cmd_character_rgb_anim2 g_cmd_character_rgb_anim2;

THUMB int cmd_map_black(int* param)
{
    NNSG3dResTex* obj = getLastModelTexture();
    dss::Fix32Vector3 pos(0, 0, 0);
    applyModelPaletteRgbRate(obj, pos);
    return 1;
}

THUMB int cmd_is_not_go_into_tenku(int* param)
{
    return TownPlayerManager::getSingleton()->notIntoTenkujou_;
}

THUMB int cmd_set_end_roll_clear(int* param)
{
    TownEndrollManager::getSingleton()->enableScroll_ = 0;
    return 1;
}

THUMB void __cmd_the_end::initialize(char*)
{
    TownEndrollManager::getSingleton()->startTheEnd();
}

THUMB int __cmd_the_end::isEnd()
{
    return TownEndrollManager::getSingleton()->isEndTheEnd();
}

__cmd_the_end g_cmd_the_end;
