#pragma once
#include "globaldefs.h"
#include "main/menu/MenuBase.hpp"
#include "main/menu/MaterielMenu_SlotEnter.hpp"

struct MaterielMenu_MEDAL_KING : menu::MenuBase
{
    static int rewardCount_[11];
    static int rewardItem_[11];

    int mode_;          /* 0x1C */
    int getReward_;     /* 0x20 */
    int systemMessage_; /* 0x24 */
    int haveMedal_;     /* 0x28 */
    int nextRewardNo_;  /* 0x2C */

    virtual void menuSetup();
    virtual void menuExecute();
    virtual void menuDraw();
    virtual void menuUpdate();
    void selectMessage();
    void kingJudge();
    void getReward();
    void haveAllReward();
    bool judgeReward();
};

extern MaterielMenu_MEDAL_KING gMaterielMenu_MEDAL_KING;
