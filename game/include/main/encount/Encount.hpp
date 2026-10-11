#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/formation/FormationIdManager.hpp"
#include "main/encount/EncountParam.hpp"
#include "main/SpecialParty/SpecialParty.hpp"
#include "main/CountDown/CountDown.hpp"

namespace encount {

    struct Encount{
        enum BattleMode {
            Normal=0,
            CrusingTrader=1,
            CrusingInnKeeper=2
        };
        BattleMode battleMode_;
        LandType landType_;
        int tileId_;
        TIME_ZONE timeZone_;
        int chapter_;
        int partyCount_;
        int enable_;
        int brewCount_;
        unsigned short walkCount_;
        unsigned short disableCount_;
        int disableFlag_;
        int disableAction_;
        unsigned short easyCount_;
        int easyFlag_;
        unsigned short differentCount_;
        int differentFlag_;
        int monsterIndex_[4];
        int monsterCount_[4];
        formation::FormationIdManager formationIdMng_;
        encount::EncountParam encountParam_;
        SpecialParty specialParty_;
        CountDown countDown_;
        int monsterGroupWidth_[4];
        int encountNumberIndex_;
        int encountCountType_[4];

        enum {
            MONSTER_NONE = 1000,
            WALK_COUNT = 16,
        };

        Encount();
        ~Encount();
        static Encount* getSingleton();
        void initialize();
        void setup(EncountPart part, LandType land);
        void exec();
        void execDungeon();
        void execWalk();
        btl::FirstAttack getFirstAttack();
        bool isEncounted();
        bool isEncountedNext();
        void setStage(const char* stage, LandType land);
        void setBlock(int x, int y);
        void setBlockGot(int x, int y);
        void setBlockYami(int x, int y);
        void setTileId(int tile);
        void setTimeZone(TIME_ZONE timeZone);
        void setChapter(int chapter);
        void setPartyCount(int count);
        void setMonsterIndex(int index, int monster) { monsterIndex_[index] = monster; }
        void setMonsterCount(int index, int count) { monsterCount_[index] = count; }
        bool brew();
        void execThinning();
        void forceBrew(int tile);
        void forceEventBrew(int tile);
        void forceEncount();
        void disableEncount(int actionIndex);
        void easyEncount(int actionIndex);
        void differentEncount(int actionIndex);
        bool checkScreenOver();
        void setThinning(int index);
        void checkFiveGroup();
        bool checkFiveGroupMonster(int monsterIndex);
        void setFiveGroupMonster(int monsterIndex);
        void setCrusingPeople();
        void setMonsterCountName();
        bool getMonsterCountName(int monsterIndex);
        int getEncountNumberType();
    };
}  // namespace encount

