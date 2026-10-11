#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/UseActionParam.hpp"
#include "main/param/Param.hpp"
#include "main/status/UseActionMessage.hpp" 
#include "main/status/CharacterStatus.hpp"
#include "ov003/status/MonsterParty.hpp"

extern "C" void func_0202d014();  // empty, called every frame by UnkGameApplication::vf08

namespace status {
    struct BaseActionMessageData {
        param::SplitMsg* splitMsg_;
        param::ActionParam* actionParam_;   
        status::UseActionParam* useActionParam_;  

    };
    struct BaseActionMessage {
        unsigned int actionIndex_;     // 0x0
        int instantDeath_;    // 0x4
        int splitFlag_;       // 0x8
        BaseActionMessage();
        ~BaseActionMessage();
        static void initialize();
        void setup(status::UseActionParam* useActionParam);
        void setExecMessage(status::UseActionMessage* message);
        void setExecMessageAdd(int actionIndex, status::UseActionMessage* message);
        void setResultMessage(status::UseActionMessage* message, int result0, int result1);
        void setExecMessage(UseActionMessage *useActionMessage,int mes0,int mes1,int mes2,int mes3);
        void setResultMessage(status::CharacterStatus* target, status::UseActionMessage* message);
        void setAddMessage(status::UseActionMessage* message, int msg0, int msg1);
        void setMessageNotEnoughMp(status::UseActionMessage* message);
        int getExecMessage(int index);
        int getResultMessage();
        int getResultSuccessMessage();
        int getResultFailedMessage();
        int setSplitMessage(status::CharacterStatus* actor, status::CharacterStatus* target, status::UseActionMessage* message, int actionIndex);
        int setSplitMessage(status::CharacterStatus* actor, status::CharacterStatus* target, int message);
        int getMessagePlayerOne(status::CharacterStatus* target, int splitIndex);
        int getMessagePlayerMany(status::CharacterStatus* target, int splitIndex);
        int getMessageMonster1G(status::CharacterStatus* target, int splitIndex);
        int getMessageMonster2G(status::CharacterStatus* target, int splitIndex);
        int getMessageMonsterD(status::CharacterStatus* target, int splitIndex);
        int getMessageTargetNoSleepNoSpazz(status::CharacterStatus* target, int splitIndex);
        int getMessageTargetSleepSpazz(status::CharacterStatus* target, int splitIndex);
        int getMessageTargetAlive(status::CharacterStatus* target, int splitIndex);
        int getMessageTargetDead(status::CharacterStatus* target, int splitIndex);
        int getMessageTargetAstoron(status::CharacterStatus* target, int splitIndex);
        int getMessageTargetNoMosyasu(status::CharacterStatus* target, int splitIndex);
        int getMessageTargetNoSplitNoJouk(status::CharacterStatus* target, int splitIndex);
        int getMessageTargetSplit(status::CharacterStatus* target, int splitIndex);
        int getMessageTargetJouk(status::CharacterStatus* target, int splitIndex);
        int getMessageWeaponEquipment(status::CharacterStatus* actor, int splitIndex);
        int getMessageNoWeaponEquipment(status::CharacterStatus* actor, int splitIndex);
        int getMessageActorMale(status::CharacterStatus* actor, int splitIndex);
        int getMessageActorFemale(status::CharacterStatus* actor, int splitIndex);
        int getMessageNoUse(status::CharacterStatus* target, int splitIndex);
        int getMessageRulaOff(int splitIndex);
        int getMessageRiremitoOff(int splitIndex);
        int getMessageVainTimeZone(int splitIndex);
        int getMessageItemInBox(int splitIndex);
        int getMessageMonsterInBox(int splitIndex);
        int getMessageGoldInBox(int splitIndex);
        int getMessageZeroInBox(int splitIndex);
        int getMessageItemInPot(int splitIndex);
        int getMessageMonsterInPot(int splitIndex);
        int getMessageGoldInPot(int splitIndex);
        int getMessageZeroInPot(int splitIndex);
        int getMessageNoTarget(int splitIndex);
        int getMessageNorthEast(int splitIndex);
        int getMessageSouthEast(int splitIndex);
        int getMessageNorthWest(int splitIndex);
        int getMessageSouthWest(int splitIndex);
        int getMessageZero(int splitIndex);
    };
    extern BaseActionMessageData messageData_; //data_020eecc0

}

