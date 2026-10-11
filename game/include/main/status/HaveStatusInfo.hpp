#pragma once
#include "GameInfo.hpp"
#include <globaldefs.h>
#include "main/profile/Profile.hpp"
#include "main/status/GameStatus.hpp"
#include "main/status/BattleResult.hpp"
#include "main/status/HaveBattleStatus.hpp"
#include "main/status/HaveStatus.hpp"
#include "main/status/HaveItem.hpp"
#include "main/status/HaveAction.hpp"
#include "main/status/DQ4HaveEquipment.hpp"
#include "main/status/ActionDefence.hpp"
#include "main/status/StatusChange.hpp"
#include "main/dss/DssUtils.hpp"

struct HaveBattleStatus;
struct UseItem;

namespace status{
    struct HaveStatusInfo {
        enum Condition {
            ALIVE = 0,
            DEAD = 1,
            SLEEP = 2,
            CONFUSION = 3,
            SPAZZ = 4,
            SPELL = 5,
            POISON = 6
        };
        enum DiffStatus {
            BeforeAction = 0,
            ExecuteAction = 1,
            ResultAction = 2,
            SpecialAction = 3,
            MaxAction = 3
        };
        enum MenuStatusChange {
            Manusa = 0,
            Mahoton = 1,
            Sleep = 2,
            Confusion = 3,
            Spazz = 4,
            Poison = 5,
            MaxStatus = 6
        };
        struct StatusFlag {
            unsigned int flag_;
            StatusFlag() { flag_ = 0; }
        };
        int hp_[4];
        int mp_[4];
        StatusFlag status_[6];
        CharacterType characterType_;
        int index_;
        HaveBattleStatus* haveBattleStatus_;
        HaveStatus haveStatus_;
        HaveItem haveItem_;
        HaveAction haveAction_;
        DQ4HaveEquipment haveEquipment_;
        ActionDefence actionDefence_;
        StatusChange statusChange_;
        CommandType battleCommand_;
        int effectValue_;
        int effectValueMax_;
        int effectValueMulti_[2];
        int mahokantaEffectValue_[12];
        int specialTargetDamage_;
        int addDamage_;
        int drawCtrlId_;
        int noDamage_;
        short defenceChange_;
        short agilityChange_;
        short attackChange_;
        int rebirthFlag_;
        dss::BitFlaguint flag_;
        dss::BitFlaguint flag2_;
        dss::BitFlaguint battleFlag_;
        static dss::Flag globalFlag_;
        HaveStatusInfo();
        ~HaveStatusInfo();
        void setup(int index, int flag);
        void setupStatus(int index, bool flag);
        void setupActionDefence(int index, int flag);
        void levelup(int level);
        unsigned short getStrength(int effect);
        void addStrength(int str);
        void setAgility(unsigned char agi);
        unsigned char getBaseAgility();
        unsigned char getAgility(int effect);
        int setAgilityChange();
        short getAgilityChange();
        void clearAgilityChange();
        void setProtection(unsigned char pro);
        unsigned int getProtection(int effect);
        void addProtection(int pro);
        unsigned int getWisdom(int effect);
        unsigned char getLuck(int effect);
        int getExp();
        void addExp(int exp);
        int getLevelupExp();
        int getGold();
        void setHp(unsigned short hp);
        int getHp();
        void addBaseHp(int hp);
        void execDeath();
        void setHpMax(unsigned short hp);
        unsigned short getHpMax();
        void addHpMax(unsigned short hp);
        void setMp(unsigned short mp);
        int getMp();
        void addMp(int mp);
        void setMpMax(unsigned short mp);
        unsigned short getMpMax();
        void clearHpInBattle();
        void addHpInBattle(DiffStatus status, int hp);
        int getHpInBattle(DiffStatus status);
        bool isNearDeathInBattle(int index);
        bool isDeathInBattle(DiffStatus status);
        void clearMpInBattle();
        void addMpInBattle(DiffStatus status, int mp);
        int getMpInBattle(DiffStatus status);
        void clearStatusChangeInBattle();
        void setStatusChangeInBattle(DiffStatus status);
        bool isStatusChangeInBattle(DiffStatus status, MenuStatusChange statusChange);
        void setAttack(unsigned short atk);
        unsigned short getAttack(int effect);
        unsigned int getBaseAttack();
        unsigned int setAttackChange();
        short getAttackChange();
        unsigned short getBaseDefence();
        static unsigned int getClampValue(int value, int min, int max);
        static unsigned int getMin(int a, int b);
        static unsigned int getMax(int a, int b);
        void setDefence(unsigned short def);
        unsigned short getDefence(int effect);
        short setDefenceChange(int actionIndex);
        short getDefenceChange();
        void clearDefenceChange();
        unsigned char getChangeStrength(unsigned int itemIndex);
        unsigned char getChangeAgility(int itemIndex);
        unsigned char getChangeWisdom(unsigned int itemIndex);
        unsigned char getChangeLuck(unsigned int itemIndex);
        short getChangeAttack(unsigned int itemIndex);
        short getChangeDefence(unsigned int itemIndex);
        int setNearDeath();
        bool isDeath();
        void setDeath();
        void rebirth();
        bool isPoison();
        void setPoison();
        void detoxPoison();
        bool isSpell();
        void setEquipment(int index);
        void resetEquipment1(int index);
        void resetEquipment2(ItemType itemType);
        void clearAllItem();
        int setItemEquipment(int itemIndex);
        bool isEquipEnable(int itemIndex);
        void execThrow(int index);
        void setCondition(Condition con);
        Condition getCondition();
        bool isPlayer(int playerIndex);
        void recovery();
        void revival();
        void setUseActionEffectValue(int value);
        void setUseActionEffectValueMulti(int index, int value);
        void setSpecialTargetDamage(int value);
        int getSpecialTargetDamage();
        void setFubahaFlag(bool flag);
        bool isFubahaFlag();
        void setBossDeathFlag(bool flag);
        bool isBossDeathFlag();
        void setEscapeFlag(bool flag);
        bool isEscapeFlag();
        void setDisappearFlag(bool flag);
        bool isDisappearFlag();
        void setMosyasExec(bool flag);
        bool isMosyasExec();
        void setMosyasRelease(bool flag);
        bool isMosyasRelease();
        void setEvilPriest(bool flag);
        void setSleepAttack(bool flag);
        bool isSleepAttack();
        void setMultiAttack(bool flag);
        void setCallFriend(bool flag);
        bool isCallFriend();
        void setSilverTarot(bool flag);
        bool isSilverTarot();
        void setWeaponAddDamage(bool flag);
        bool isWeaponAddDamage();
        void setConfuseMissAttack(bool flag);
        bool isConfuseMissAttack();
        void setAddEffectSleep(bool flag);
        bool isAddEffectSleep();
        void setAddEffectSpazz(bool flag);
        void setAddEffectPoison(bool flag);
        void setAddEffectRecovery(bool flag);
        bool isAddEffectRecovery();
        void setAddEffectMahotora(bool flag);
        bool isAddEffectMahotora();
        void setAddEffectDamage(bool flag);
        bool isAddEffectDamage();
        void setTargetJouk(bool flag);
        bool isTargetJouk();
        void setMosyasTarget(bool flag);
        bool isMosyasTarget();
        void setKillMyself(bool flag);
        bool isKillMyself();
        void setMonsterChange(bool flag);
        bool isMonsterChange();
        void setBaikirutoDisable(bool flag);
        void setFirstMosyas(bool flag);
        bool isFirstMosyas();
        void setMahokantaCounter(bool flag);
        bool isMahokantaCounter();
        void setCounterDamage(bool flag);
        void setDamage(bool flag);
        bool isDamage();
        void setLastActor(bool flag);
        void setAddMahotoraExecute(bool flag);
        bool isAddMahotoraExecute();
        void setDamageMyself(bool flag);
        bool isDamageMyself();
        void setSelfImmolation(bool flag);
        bool isSelfImmolation();
        void setFirstKaishin(bool flag);
        bool isFirstKaishin();
        void setSecondKaishin(bool flag);
        bool isSecondKaishin();
        void setStatusChangeRelease(bool flag);
        bool isStatusChangeRelease();
        void setAstoron(bool flag);
        bool isAstoron();
        void setMpFailure(bool flag);
        bool isMpFailure();
        void setMahotoneFailure(bool flag);
        void setMegazaruRebirth(bool flag);
        bool isMegazaruRebirth();
        void setMegazaruRecovery(bool flag);
        void setZaorikuRebirth(bool flag);
        void setImmidiateDeath(bool flag);
        bool isImmidiateDeath();
        void setMultiFirstDeath(bool flag);
        void setExecuteMeganteRing(bool flag);
        bool isExecuteMeganteRing();
        void setMeganteRing(bool flag);
        bool isMeganteRing();
        void setMegazaruRing(bool flag);
        bool isMegazaruRing();
        void setDisableTextureCache(bool flag);
        bool isDisableTextureCache();
        void setAllKaishin(bool flag);
        bool isAllKaishin();
        static void setGlbMeganteRing(bool flag);
        static bool isGlbMeganteRing();
        static void setGlbMegazaruRing(bool flag);
        static bool isGlbMegazaruRing();
        void releaseMosyas();
        void execStartOfBattle();
        int isActionEnable();
        int isAttackEnable();
    };
}
