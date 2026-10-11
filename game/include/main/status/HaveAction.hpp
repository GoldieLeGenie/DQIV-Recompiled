#pragma once
#include <globaldefs.h>
#include "GameInfo.hpp"
#include "main/status/Action.hpp"


namespace status {
    struct HaveAction {
        enum ActionMode 
        {
            TownMode   = 0,
            BattleMode = 1,
        };

        Action newAction_[24];
        static int mode_; //data_020d06ac
        HaveAction();
        ~HaveAction();
        static void setBattleMode();
        static int isBattleMode();
        static void setTownMode();
        static int isTownMode();
        static ActionMode getActionMode();
        static void setActionMode(ActionMode md);
        void setup(int index);
        void clear();
        void levelup(int index, int level, int wisdom, int flag);
        void add(int battleAction, int usuallyAction);
        int learn(int level, int wisdom);
        void getLearnTypeA(int index, int level, int wisdom);
        int getAction(int index);
        int getHaveAction(int index);
        int getCount();
        int getAllCount();
        int isAction(int actionIndex);
        int isActionAlreadyLearned(int actionIndex);
        int getRememberingAction();
        void forceLearn();

    };
}