#pragma once
#include <globaldefs.h>


namespace status{
    struct Action {
        Action();
        unsigned short battleAction_;
        unsigned short usuallyAction_;
        unsigned char actionLevel_;
        unsigned char actionFlag_;
        void setup(int battleAction, int usuallyAction, int actionLevel, int actionFlag);
        void clear();
        int isEnable();
        void add(int battleAction, int usuallyAction);
        int getAction();
        int getUsuallyAction();
        void remember();
        int isRemember();
        int isRemembering();
        void clearRemembering();
        void learnTypeA(int actionIndex, int characterLevel);

    };
}