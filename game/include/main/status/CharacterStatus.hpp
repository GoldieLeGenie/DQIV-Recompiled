
#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/HaveStatusInfo.hpp"
#include "main/status/HaveBattleStatus.hpp"
#include "main/dss/DssUtils.hpp"

namespace status{
    
    struct CharacterStatus {
        enum DamageAnimation {
            NoAnim = 0,
            Damage = 1,
            Death = 2,
            Disappear = 3,
            MultiDamage = 4
        };

        enum DamageSound {
            NoSe = 0,
            KaishinSe = 1,
            TsukonSe = 2
        };

        CharacterType characterType_;
        HaveStatusInfo haveStatusInfo_;
        HaveBattleStatus haveBattleStatus_;
        int characterIndex_;
        int characterGroup_;
        int arrayIndex_;
        dss::BitFlaguint menuStatusFlag_;
        DamageAnimation damageAnimation_;
        DamageAnimation damageAnimationMulti_;
        DamageSound damageSound_;
        dss::BitFlaguint damageAnimationFlag_;
        dss::BitFlaguint type_;
        CharacterStatus();
        ~CharacterStatus();
        HaveStatusInfo& getHaveStatusInfo() { return haveStatusInfo_; }
        int isActionEnable();
        void setCommandType(CommandType type) { haveStatusInfo_.battleCommand_ = type; }
        void setup(CharacterType type, int group, int index);
        void setMenuStatusFlag(status::HaveStatusInfo::DiffStatus status);
        bool isMenuStatusFlag(status::HaveStatusInfo::DiffStatus status);
        void clearMenuStatusFlag();
        void setNoneAnimation();
        void setDamageAnimation();
        void clearDamageAnimation();
        bool isDamageAnimation();
        void setAddDamageAnimation();
        void setDeathAnimation();
        void clearDeathAnimation();
        bool isDeathAnimation();
        void setDisappearAnimation();
        void clearDisappearAnimation();
        bool isDisappearAnimation();
        void setMultiDamageAnimation1();
        void clearMultiDamageAnimation1();
        bool isMultiDamageAnimation1();
        void setMultiDamageAnimation2();
        void clearMultiDamageAnimation2();
        bool isMultiDamageAnimation2();
        void setMultiDamageAnimation();
        void clearMultiDamageAnimation();
        bool isMultiDamageAnimation();
        void setMultiDamageAnimation2nd();
        bool isMultiDamageAnimation2nd();
        void setEscapeAnimation();
        void clearEscapeAnimation();
        bool isEscapeAnimation();
        void setRebirthAnimation();
        void clearRebirthAnimation();
        bool isRebirthAnimation();
        
       
#ifdef DQIV_NATIVE
        virtual void setMosyasChange(int monsterIndex);
        virtual void setMosyasRelease();
        virtual bool isDeathDisable();
#else
        inline virtual void setMosyasChange(int monsterIndex);
        inline virtual void setMosyasRelease();
        inline virtual bool isDeathDisable();
#endif
        virtual void print();    
        virtual void printAction();        
    };
}
